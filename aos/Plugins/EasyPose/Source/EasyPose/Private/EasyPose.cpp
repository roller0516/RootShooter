// Copyright longlt00502@gmail.com 2023. All rights reserved.

#include "EasyPose.h"
#include "EasyPoseStyle.h"
#include "EasyPoseCommands.h"
#include "Misc/MessageDialog.h"
#include "Toolkits/ToolkitManager.h"

#include "Algo/Reverse.h"

#include "RetargetEditor/IKRetargetEditor.h"

#include "Subsystems/AssetEditorSubsystem.h"
#include "Widgets/Input/STextComboBox.h"
#include "Misc/ScopedSlowTask.h"

// #include "tll/log.h"

#define LOCTEXT_NAMESPACE "FEasyPoseModule"

// #define EP_DEVELOPMENT

template <int AxisType>
bool IsAxisAligned(FQuat const &QA, FQuat const &QB, float Scalar, float *OutDotProdResult = nullptr)
{
    FVector AxisVec = (AxisType == 0) ? FVector::UnitX() : (AxisType == 1) ? FVector::UnitY()
                                                                           : FVector::UnitZ();
    FVector QAX = QA * AxisVec;
    FVector QBX = QB * AxisVec;
    auto DotProdResult = QAX.Dot(QBX);

    if (OutDotProdResult != nullptr)
        *OutDotProdResult = DotProdResult;

    UE_LOG(EasyPose, VeryVerbose, TEXT("IsAxisAligned<%d>: %d, %f, %f"), AxisType, DotProdResult >= Scalar, DotProdResult, Scalar);
    return DotProdResult >= Scalar;
}

FQuat GetRotationDifference(FQuat const &QA, FQuat const &QB)
{
    return (QA.Inverse() * QB).GetNormalized();
}

FQuat GetRotationDifference(FVector const &VA, FVector const &VB)
{
    auto DirA = VA.GetSafeNormal();
    auto DirB = VB.GetSafeNormal();

    return FQuat::FindBetweenNormals(DirA, DirB);
}

FQuat GetRotationOffset(FQuat const &QA, FQuat const &QB, float ScalarX, float ScalarY)
{
    float DotProdX = 0;
    bool IsXAligned = IsAxisAligned<0>(QA, QB, ScalarX, &DotProdX);
    float DotProdY = 0;
    bool IsYAligned = IsAxisAligned<1>(QA, QB, ScalarY, &DotProdY);

    if (!IsXAligned || !IsYAligned)
    {
        return FQuat::Identity;
    }

    return GetRotationDifference(QA, QB);
}

FQuat GetRotationXOffset(FQuat const &QA, FQuat const &QB, float ScalarX, float ScalarY)
{
    float DotProdX = 0;
    bool IsXAligned = IsAxisAligned<0>(QA, QB, ScalarX, &DotProdX);
    float DotProdY = 0;
    bool IsYAligned = IsAxisAligned<1>(QA, QB, ScalarY, &DotProdY);

    if (!IsXAligned || !IsYAligned)
    {
        return FQuat::Identity;
    }

    auto QAY = QA * FVector::UnitY();
    auto QBY = QB * FVector::UnitY();
    auto QAX = QA * FVector::UnitX();

    auto XAYBNormal = QAX.Cross(QBY).GetSafeNormal();

    if (XAYBNormal == FVector::Zero())
        return FQuat::Identity;

    auto ProjectedYAOnXAYB = (QAY - (QAY.Dot(XAYBNormal) * XAYBNormal)).GetSafeNormal();

    if (ProjectedYAOnXAYB == FVector::Zero())
        return FQuat::Identity;

    return GetRotationDifference(QAY, ProjectedYAOnXAYB);
}

FQuat GetBoneSpaceOffset(FQuat const &GlobalOffset, FTransform const &BoneGlobalTransform)
{
    return GetRotationDifference(BoneGlobalTransform.GetRotation(), GlobalOffset * BoneGlobalTransform.GetRotation());
}

int32 GetRetargetBoneIdFromRigSkeleton(FRetargetSkeleton const &RtgSkeleton, FIKRigSkeleton const &IKRigSkeleton, int32 RigBoneId)
{
    return RtgSkeleton.FindBoneIndexByName(IKRigSkeleton.BoneNames[RigBoneId]);
}

FQuat GetBoneDirectionOffset(FRetargetSkeleton const &RtgSkeleton, int32 RtgIdFrom, int32 RtgIdTo, FVector const &DirNormal)
{
    if (RtgIdFrom == INDEX_NONE || RtgIdTo == INDEX_NONE)
    {
        UE_LOG(EasyPose, Warning, TEXT("Bone not found: %d %d"), RtgIdFrom, RtgIdTo);
        return FQuat::Identity;
    }

    FVector RtgDirection = (RtgSkeleton.RetargetGlobalPose[RtgIdTo].GetTranslation() - RtgSkeleton.RetargetGlobalPose[RtgIdFrom].GetTranslation()).GetSafeNormal();

    return FQuat::FindBetweenNormals(RtgDirection, DirNormal);
}

struct FIKRigRetargetInfo
{
    int32 IKRigId = INDEX_NONE;
    int32 RetargetId = INDEX_NONE;
    float LenToNext = 0;
    float TotalLenUntilCurrent = 0;
};

struct FIKRigRetargetChainDetail
{
    float TotalLength = 0;
    int32 LongestChildChain=0;
    FBoneChain const *BoneChain = nullptr;
    TArray<FIKRigRetargetInfo> IKRigRetargetMap{};
    TArray<int32> ChildrenId;

    FIKRigRetargetChainDetail(FBoneChain const *BoneChain, FIKRigSkeleton const &IKRigSkeleton, FRetargetSkeleton const &RtgSkeleton)
    {
        if (BoneChain == nullptr)
            return;

        this->BoneChain = BoneChain;

        TArray<int32> RigBoneIndices{};
        IKRigSkeleton.ValidateChainAndGetBones(*BoneChain, RigBoneIndices);

        for (int32 i = 0; i < RigBoneIndices.Num() - 1; i++)
        {
            int32 IKRigId = RigBoneIndices[i];
            int32 RtgId = GetRetargetBoneIdFromRigSkeleton(RtgSkeleton, IKRigSkeleton, IKRigId);

            if (RtgId == INDEX_NONE)
            {
                UE_LOG(EasyPose, Log, TEXT("Bone not found: %s (%d) in %s"), *IKRigSkeleton.BoneNames[IKRigId].ToString(), IKRigId, *RtgSkeleton.RetargetPoseName.ToString());
                continue;
            }

            IKRigRetargetMap.Add(FIKRigRetargetInfo{IKRigId, RtgId, 0, 0});

            int32 NextRtgId = INDEX_NONE;

            for (int32 j = i + 1; j < RigBoneIndices.Num(); j++)
            {
                int32 NextIKRigId = RigBoneIndices[j];
                NextRtgId = GetRetargetBoneIdFromRigSkeleton(RtgSkeleton, IKRigSkeleton, NextIKRigId);

                if (NextRtgId != INDEX_NONE)
                    break;
            }

            if (NextRtgId == INDEX_NONE)
            {
                UE_LOG(EasyPose, Warning, TEXT("Cannot find child bone in Chain of bone %s (%d)"), *IKRigSkeleton.BoneNames[RtgId].ToString(), RtgId);
                break;
            }
            float Length = (RtgSkeleton.RetargetGlobalPose[NextRtgId].GetTranslation() - RtgSkeleton.RetargetGlobalPose[RtgId].GetTranslation()).Length();
            IKRigRetargetMap.Last().TotalLenUntilCurrent = TotalLength;
            IKRigRetargetMap.Last().LenToNext = Length;
            TotalLength += Length;
        }

        if (RigBoneIndices.Num() > 0)
        {
            int32 IKRigId = RigBoneIndices.Last();
            int32 RtgId = GetRetargetBoneIdFromRigSkeleton(RtgSkeleton, IKRigSkeleton, IKRigId);

            if (RtgId != INDEX_NONE)
            {
                IKRigRetargetMap.Add(FIKRigRetargetInfo{IKRigId, RtgId, 0, TotalLength});
            }
            else
            {
                UE_LOG(EasyPose, Warning, TEXT("No such bone in RetargetSkeleton: %s (%d)"), *IKRigSkeleton.BoneNames[IKRigId].ToString(), IKRigId);
            }
        }
    }
};

bool AddChildren(FRetargetSkeleton const &RtgSkeleton, TArray<FIKRigRetargetChainDetail> &IkRigRtgChainsDetailList)
{
    int32 CrrRtgId = IkRigRtgChainsDetailList.Last().IKRigRetargetMap[0].RetargetId;

    for (int i = IkRigRtgChainsDetailList.Num() - 2; i >= 0; i--)
    {
        auto &ParentChainDetail = IkRigRtgChainsDetailList[i];
        int LastRtgId = ParentChainDetail.IKRigRetargetMap.Last().RetargetId;

        int32 ParentId = RtgSkeleton.ParentIndices[CrrRtgId];

        while(ParentId != INDEX_NONE && ParentId > LastRtgId)
        {
            ParentId = RtgSkeleton.ParentIndices[ParentId];
        }
        
        if (ParentId == LastRtgId)
        {
            if(ParentChainDetail.LongestChildChain <= IkRigRtgChainsDetailList.Last().IKRigRetargetMap.Num())
            {
                if(ParentChainDetail.LongestChildChain < IkRigRtgChainsDetailList.Last().IKRigRetargetMap.Num())
                {
                    ParentChainDetail.ChildrenId.Empty();
                    ParentChainDetail.LongestChildChain = IkRigRtgChainsDetailList.Last().IKRigRetargetMap.Num();
                }

                ParentChainDetail.ChildrenId.Add(IkRigRtgChainsDetailList.Num() - 1);
            }

            return true;
        }
    }

    return false;
}

void ApplyRotationOffset(FQuat const &RotOffset, int32 RtgId, FRetargetSkeleton &OutRtgSkeleton)
{
    FQuat const LocalBoneRotation = (OutRtgSkeleton.RetargetLocalPose[RtgId].GetRotation() * RotOffset).GetNormalized();
    OutRtgSkeleton.RetargetLocalPose[RtgId].SetRotation(LocalBoneRotation);
    OutRtgSkeleton.UpdateGlobalTransformsBelowBone(RtgId - 1, OutRtgSkeleton.SkeletalMesh->GetRefSkeleton().GetRefBonePose(), OutRtgSkeleton.RetargetGlobalPose);
    OutRtgSkeleton.UpdateGlobalTransformsBelowBone(RtgId - 1, OutRtgSkeleton.RetargetLocalPose, OutRtgSkeleton.RetargetGlobalPose);
}

bool NotEmptyOrIdentity(FQuat const &Quat, float Tolerance)
{
    if (Quat.Equals(FQuat(0, 0, 0, 0), Tolerance))
    {
        UE_LOG(EasyPose, Error, TEXT("Ooops! Empty quaternion observed."));
        return false;
    }

    return !Quat.Equals(FQuat::Identity, Tolerance);
}

template <class FEditor, class UAssetType>
TSharedPtr<FEditor> GetAssetEditorInstance(const UAssetType *Asset)
{
    if (Asset == nullptr)
    {
        return nullptr;
    }

    TSharedPtr<IToolkit> AssetEditorInstance = FToolkitManager::Get().FindEditorForAsset(Asset);

    if (AssetEditorInstance.IsValid())
    {
        return StaticCastSharedPtr<FEditor>(AssetEditorInstance);
    }

    return nullptr;
}

void MatchStep13(FRetargetSkeleton const &SrcRtgSkeleton, FRetargetSkeleton &TgtRtgSkeleton, int32 SrcRtgId, int32 TgtRtgId, int32 TgtNextRtgId, FVector const &Direction, uint32 Flag, float FloatingPointTolerance, float BoneAxesDotScalar, bool StepMiddleBoneXOnly)
{
    FQuat RotOffset;
    auto const &TgtRtgBoneName = TgtRtgSkeleton.BoneNames[TgtRtgId];
    auto const &SrcRtgBoneName = SrcRtgSkeleton.BoneNames[SrcRtgId];
    auto const &TgtGlobalTransform = TgtRtgSkeleton.RetargetGlobalPose[TgtRtgId];

    RotOffset = GetBoneDirectionOffset(TgtRtgSkeleton, TgtRtgId, TgtNextRtgId, Direction);
    UE_LOG(EasyPose, VeryVerbose, TEXT("Matching Direction: %s -> %s: %s"), *SrcRtgBoneName.ToString(), *TgtRtgBoneName.ToString(), *RotOffset.ToString());

    if (NotEmptyOrIdentity(RotOffset, FloatingPointTolerance))
    {
        UE_LOG(EasyPose, Verbose, TEXT("Matched Direction: %s -> %s"), *SrcRtgBoneName.ToString(), *TgtRtgBoneName.ToString());
        RotOffset = GetBoneSpaceOffset(RotOffset, TgtGlobalTransform);
        ApplyRotationOffset(RotOffset, TgtRtgId, TgtRtgSkeleton);
    }

    if ((Flag & static_cast<uint32>(matching::EFlag::StepMatchMiddleBoneRotation)) != 0)
    {
        if (StepMiddleBoneXOnly)
        {
            RotOffset = GetRotationXOffset(TgtRtgSkeleton.RetargetGlobalPose[TgtRtgId].GetRotation(), SrcRtgSkeleton.RetargetGlobalPose[SrcRtgId].GetRotation(), BoneAxesDotScalar, BoneAxesDotScalar);
        }
        else
        {
            RotOffset = GetRotationOffset(TgtRtgSkeleton.RetargetGlobalPose[TgtRtgId].GetRotation(), SrcRtgSkeleton.RetargetGlobalPose[SrcRtgId].GetRotation(), BoneAxesDotScalar, BoneAxesDotScalar);
        }
        UE_LOG(EasyPose, VeryVerbose, TEXT("Matching Middle Bone Rotation: %s -> %s: %s"), *SrcRtgBoneName.ToString(), *TgtRtgBoneName.ToString(), *RotOffset.ToString());

        if (NotEmptyOrIdentity(RotOffset, FloatingPointTolerance))
        {
            UE_LOG(EasyPose, Verbose, TEXT("Matched Middle Bone Rotation: %s -> %s"), *SrcRtgBoneName.ToString(), *TgtRtgBoneName.ToString());
            ApplyRotationOffset(RotOffset, TgtRtgId, TgtRtgSkeleton);

            RotOffset = GetBoneDirectionOffset(TgtRtgSkeleton, TgtRtgId, TgtNextRtgId, Direction);
            UE_LOG(EasyPose, VeryVerbose, TEXT("Matching Direction again: %s -> %s: %s"), *SrcRtgBoneName.ToString(), *TgtRtgBoneName.ToString(), *RotOffset.ToString());

            if (NotEmptyOrIdentity(RotOffset, FloatingPointTolerance))
            {
                RotOffset = GetBoneSpaceOffset(RotOffset, TgtGlobalTransform);
                UE_LOG(EasyPose, Verbose, TEXT("Matched Direction again: %s -> %s"), *SrcRtgBoneName.ToString(), *TgtRtgBoneName.ToString());
                ApplyRotationOffset(RotOffset, TgtRtgId, TgtRtgSkeleton);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void FEasyPoseModule::StartupModule()
{
    FEasyPoseStyle::Initialize();
    FEasyPoseStyle::ReloadTextures();

    FEasyPoseCommands::Register();

    PluginCommands = MakeShareable(new FUICommandList);
    PluginCommands->MapAction(
        FEasyPoseCommands::Get().PluginAction,
        FExecuteAction::CreateRaw(this, &FEasyPoseModule::PluginButtonClicked),
        FCanExecuteAction());

    FCoreDelegates::OnPostEngineInit.AddRaw(this, &FEasyPoseModule::OnPostEngineInit);
}

void FEasyPoseModule::OnPostEngineInit()
{
    if (GEditor != nullptr)
    {
        auto AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();

        AssetEditorSubsystem->OnAssetEditorOpened().AddLambda([this](UObject *Obj)
                                                              {
            TSharedPtr<FIKRetargetEditor> IKRetargetEditor = GetAssetEditorInstance<FIKRetargetEditor, UIKRetargeter>(Cast<UIKRetargeter>(Obj));
            if(!IKRetargetEditor.IsValid()) return;

            TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);

            IKRetargetEditor->AddToolbarExtender(ToolbarExtender);
            ToolbarExtender->AddToolBarExtension(
                "Asset",
                EExtensionHook::After, PluginCommands,
                FToolBarExtensionDelegate::CreateLambda([this, Obj](FToolBarBuilder& ToolbarBuilder) {
                    ToolbarBuilder.BeginSection("Easy Pose");
                    ToolbarBuilder.AddToolBarButton(
                        FExecuteAction::CreateLambda([this, Obj] {
                            MatchPose(Cast<UIKRetargeter>(Obj));
                        }), /// FExecuteAction::CreateLambda
                        NAME_None, 
                        TAttribute<FText>::Create(TAttribute<FText>::FGetter::CreateLambda([this]() { return FText::FromString("Easy Pose"); })),
                        TAttribute<FText>(), 
                        TAttribute<FSlateIcon>::Create(TAttribute<FSlateIcon>::FGetter::CreateLambda([this]() { return FSlateIcon("EditorStyle", "GraphEditor.CustomEvent_16x"); }))
                    ); /// ToolbarBuilder.AddToolBarButton

                    ToolbarBuilder.AddComboButton(
                        FUIAction(),
                        FOnGetContent::CreateLambda([this]() {
                            FMenuBuilder MenuBuilder(true, PluginCommands);

                            MenuBuilder.BeginSection(NAME_None, TAttribute<FText>(FText::FromString("Matching Steps")));
                            MenuBuilder.AddMenuEntry(
                                LOCTEXT("", "Match Middle Bone Rotation"),
                                TAttribute<FText>(),
                                FSlateIcon(),
                                FUIAction(
                                    FExecuteAction::CreateLambda([this]() { StepFlag ^= static_cast<uint32>(matching::EFlag::StepMatchMiddleBoneRotation); }),
                                    FCanExecuteAction(),
                                    FIsActionChecked::CreateLambda([this]() { return (StepFlag & static_cast<uint32>(matching::EFlag::StepMatchMiddleBoneRotation)) != 0; })
                                ), NAME_None,
                                EUserInterfaceActionType::ToggleButton
                            );

                            MenuBuilder.AddMenuEntry(
                                LOCTEXT("", "Match Subsequence Chain"),
                                TAttribute<FText>(),
                                FSlateIcon(),
                                FUIAction(
                                    FExecuteAction::CreateLambda([this]() { StepFlag ^= static_cast<uint32>(matching::EFlag::StepMatchSubsequenceChain); }),
                                    FCanExecuteAction(),
                                    FIsActionChecked::CreateLambda([this]() { return (StepFlag & static_cast<uint32>(matching::EFlag::StepMatchSubsequenceChain)) != 0; })
                                ), NAME_None,
                                EUserInterfaceActionType::ToggleButton
                            );

                            MenuBuilder.AddMenuEntry(
                                LOCTEXT("", "Match Bone Tail Rotation"),
                                TAttribute<FText>(),
                                FSlateIcon(),
                                FUIAction(
                                    FExecuteAction::CreateLambda([this]() { StepFlag ^= static_cast<uint32>(matching::EFlag::StepMatchBoneTailRotation); }),
                                    FCanExecuteAction(),
                                    FIsActionChecked::CreateLambda([this]() { return (StepFlag & static_cast<uint32>(matching::EFlag::StepMatchBoneTailRotation)) != 0; })
                                ), NAME_None,
                                EUserInterfaceActionType::ToggleButton
                            );

                            MenuBuilder.EndSection(); /// EP Development

#if (defined EP_DEVELOPMENT)
                            MenuBuilder.BeginSection(NAME_None, TAttribute<FText>(FText::FromString("Floating Point Tolerance")));

                            MenuBuilder.AddMenuEntry(FUIAction(),
                                SNew(STextComboBox)
                                .ToolTipText(LOCTEXT("", ""))
                                .OptionsSource(&FPToleranceOptions)
                                .OnSelectionChanged_Lambda([this](TSharedPtr<FString> InString, ESelectInfo::Type SelectInfo)
                                {
                                    if(InString == FPToleranceOptions[0]) {
                                        FPTolerance = 1e-2;
                                        RotationOffsetFloatingPointNegativeExp = 2;
                                    } else if(InString == FPToleranceOptions[1]) {
                                        FPTolerance = 1e-4;
                                        RotationOffsetFloatingPointNegativeExp = 4;
                                    } else {
                                        FPTolerance = 1e-8;
                                        RotationOffsetFloatingPointNegativeExp = 8;
                                    }

                                    FPTSelected = InString;
                                })
                                .InitiallySelectedItem(FPTSelected),
                                NAME_None, TAttribute<FText>(), EUserInterfaceActionType::None, NAME_None
                            );

                            MenuBuilder.EndSection();

                            MenuBuilder.BeginSection(NAME_None, TAttribute<FText>(FText::FromString("Matching Mode")));
                            MenuBuilder.AddMenuEntry(
                                LOCTEXT("", "Dynamic"),
                                TAttribute<FText>(),
                                FSlateIcon(),
                                FUIAction(
                                    FExecuteAction::CreateLambda([this]() { MatchingMode = static_cast<MatchMode>(static_cast<uint8>(MatchingMode) ^ 1); }),
                                    FCanExecuteAction(),
                                    FIsActionChecked::CreateLambda([this]() { return MatchingMode == MatchMode::DYNAMIC; })
                                ), NAME_None,
                                EUserInterfaceActionType::ToggleButton
                            );
                            MenuBuilder.EndSection();

                            MenuBuilder.BeginSection(NAME_None, TAttribute<FText>(FText::FromString("Development")));
                            MenuBuilder.AddMenuEntry(FUIAction(),
                                SNew(SNumericEntryBox<float>)
                                    .Value_Lambda([this](){ return (BoneAxesDotProductScalar + 1) / 2; })
                                    .AllowSpin(true)
                                    .MinValue(0).MaxValue(1.0f)
                                    .MinSliderValue(0).MaxSliderValue(1.0f)
                                    .WheelStep(0.01f)
                                    .OnValueChanged_Lambda([this](float NewValue){
                                        BoneAxesDotProductScalar = FMath::Clamp(NewValue, 0, 1.f) * 2 - 1;
                                    }),
                                NAME_None, TAttribute<FText>(), EUserInterfaceActionType::None, NAME_None
                            );

                            MenuBuilder.AddMenuEntry(FUIAction(),
                                SNew(SNumericEntryBox<int32>)
                                    .Value_Lambda([this](){ return RotationOffsetFloatingPointNegativeExp; })
                                    .AllowSpin(true)
                                    .MinValue(1).MaxValue(8).MinSliderValue(1).MaxSliderValue(8)
                                    .WheelStep(1)
                                    .OnValueChanged_Lambda([this](int32 NewValue){
                                        RotationOffsetFloatingPointNegativeExp = FMath::Clamp(NewValue, 1, 8);
                                        FPTolerance = FMath::Pow(10.0f, RotationOffsetFloatingPointNegativeExp * (-1));
                                    }),
                                NAME_None, TAttribute<FText>(), EUserInterfaceActionType::None, NAME_None
                            );

                            MenuBuilder.AddMenuEntry(
                                LOCTEXT("ESPMATitle", "Dynamic"),
                                LOCTEXT("ESPMATooltip", "Mix between Similar and Exact"),
                                FSlateIcon(),
                                FUIAction(
                                    FExecuteAction::CreateLambda([this]() { MatchingMode = MatchMode::DYNAMIC; }),
                                    FCanExecuteAction(),
                                    FIsActionChecked::CreateLambda([this]() { return MatchingMode == MatchMode::DYNAMIC; })
                                ), NAME_None,
                                EUserInterfaceActionType::RadioButton
                            );

                            MenuBuilder.AddMenuEntry(
                                LOCTEXT("ESPMETitle", "Exact"),
                                LOCTEXT("ESPMETooltip", "Always matching Exact"),
                                FSlateIcon(),
                                FUIAction(
                                    FExecuteAction::CreateLambda([this]() { MatchingMode = MatchMode::EXACT; }),
                                    FCanExecuteAction(),
                                    FIsActionChecked::CreateLambda([this]() { return MatchingMode == MatchMode::EXACT; })
                                ), NAME_None,
                                EUserInterfaceActionType::RadioButton
                            );

                            MenuBuilder.AddMenuEntry(
                                LOCTEXT("ESPMSTitle", "Similar"),
                                LOCTEXT("ESPMSTooltip", "Always matching Similar"),
                                FSlateIcon(),
                                FUIAction(
                                    FExecuteAction::CreateLambda([this]() { MatchingMode = MatchMode::SIMILAR; }),
                                    FCanExecuteAction(),
                                    FIsActionChecked::CreateLambda([this]() { return MatchingMode == MatchMode::SIMILAR; })
                                ), NAME_None,
                                EUserInterfaceActionType::RadioButton
                            );

                            MenuBuilder.AddMenuEntry(
                                LOCTEXT("", "Always Reset"),
                                TAttribute<FText>(),
                                FSlateIcon(),
                                FUIAction(
                                    FExecuteAction::CreateLambda([this]() { AlwaysReset = !AlwaysReset; }),
                                    FCanExecuteAction(),
                                    FIsActionChecked::CreateLambda([this]() { return AlwaysReset; })
                                ), NAME_None,
                                EUserInterfaceActionType::ToggleButton
                            );

                            MenuBuilder.AddMenuEntry(
                                LOCTEXT("", "StepMiddleBoneXOnly"),
                                TAttribute<FText>(),
                                FSlateIcon(),
                                FUIAction(
                                    FExecuteAction::CreateLambda([this]() { StepMiddleBoneXOnly = !StepMiddleBoneXOnly; }),
                                    FCanExecuteAction(),
                                    FIsActionChecked::CreateLambda([this]() { return StepMiddleBoneXOnly; })
                                ), NAME_None,
                                EUserInterfaceActionType::ToggleButton
                            );
#endif // EP_DEVELOPMENT

                            return MenuBuilder.MakeWidget();
                        }), /// FOnGetContent::CreateLambda
                        LOCTEXT("EPS_Label", "EPS label"),
                        LOCTEXT("EPS_ToolTip", "EPS ToolTip."),
                        FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Recompile"),
                        true); /// ToolbarBuilder.AddComboButton

                    ToolbarBuilder.AddSeparator();
                    ToolbarBuilder.EndSection();
                }) /// FToolBarExtensionDelegate::CreateLambda

            ); /// ToolbarExtender->AddToolBarExtension

            IKRetargetEditor->RegenerateMenusAndToolbars(); }); /// OnAssetEditorOpened
    }
}

void FEasyPoseModule::ShutdownModule()
{
    UToolMenus::UnRegisterStartupCallback(this);

    UToolMenus::UnregisterOwner(this);

    FEasyPoseStyle::Shutdown();

    FEasyPoseCommands::Unregister();
}

int32 FEasyPoseModule::MatchPose(UIKRetargeter *IKRetargeter)
{
    int TotalRotationsCount = 0;

    if (IKRetargeter == nullptr)
        return 0;

    auto Target = ERetargetSourceOrTarget::Target;
    auto Source = ERetargetSourceOrTarget::Source;
    TSharedPtr<FIKRetargetEditor> IKRetargetEditor = GetAssetEditorInstance<FIKRetargetEditor, UIKRetargeter>(IKRetargeter);

    if (!IKRetargetEditor.IsValid())
        return 0;

    UIKRetargeterController *IKRtgCtrl = UIKRetargeterController::GetController(IKRetargeter);
    TSharedRef<FIKRetargetEditorController> IKRtgEditCtrl = IKRetargetEditor->GetController();

    if (IKRtgCtrl == nullptr)
        return 0;

    const UIKRigDefinition *SourceIKRig = IKRetargeter->GetSourceIKRig();
    const UIKRigDefinition *TargetIKRig = IKRetargeter->GetTargetIKRig();

    const TArray<FBoneChain> &SrcRtgChains = SourceIKRig->GetRetargetChains();
    const TArray<FBoneChain> &TgtRtgChains = TargetIKRig->GetRetargetChains();

    const FName &SrcRtgRootName = SourceIKRig->GetRetargetRoot();
    const FName &TgtRtgRootName = TargetIKRig->GetRetargetRoot();

    FRetargetSkeleton SrcRtgSkeleton;
    FRetargetSkeleton TgtRtgSkeleton, OrgTgtRtgSkeleton;

    auto TgtRtgPoseName = IKRetargeter->GetCurrentRetargetPoseName(ERetargetSourceOrTarget::Target);
    FIKRetargetPose const *TgtRtgPose = IKRetargeter->GetCurrentRetargetPose(ERetargetSourceOrTarget::Target);

    SrcRtgSkeleton.Initialize(
        IKRtgCtrl->GetPreviewMesh(Source),
        SrcRtgChains,
        IKRetargeter->GetCurrentRetargetPoseName(ERetargetSourceOrTarget::Source),
        IKRetargeter->GetCurrentRetargetPose(ERetargetSourceOrTarget::Source),
        SrcRtgRootName);
    TgtRtgSkeleton.Initialize(
        IKRtgCtrl->GetPreviewMesh(Target),
        TgtRtgChains,
        TgtRtgPoseName,
        IKRetargeter->GetCurrentRetargetPose(ERetargetSourceOrTarget::Target),
        TgtRtgRootName);
    OrgTgtRtgSkeleton.Initialize(
        IKRtgCtrl->GetPreviewMesh(Target),
        TgtRtgChains,
        TgtRtgPoseName,
        IKRetargeter->GetCurrentRetargetPose(ERetargetSourceOrTarget::Target),
        TgtRtgRootName);

#if ENGINE_MAJOR_VERSION == 5
#if ENGINE_MINOR_VERSION >= 2
    const FIKRigSkeleton &SrcRigSkeleton = SourceIKRig->GetSkeleton();
    const FIKRigSkeleton &TgtRigSkeleton = TargetIKRig->GetSkeleton();
#elif ENGINE_MINOR_VERSION == 1
    const FIKRigSkeleton& SrcRigSkeleton = SourceIKRig->Skeleton;
    const FIKRigSkeleton& TgtRigSkeleton = TargetIKRig->Skeleton;
#endif
#endif

    TArray<FIKRigRetargetChainDetail> SrcIkRigRtgChainsDetailList;
    TArray<FIKRigRetargetChainDetail> TgtIkRigRtgChainsDetailList;
    TArray<TObjectPtr<URetargetChainSettings>> const &AllChainSettings = IKRetargeter->GetAllChainSettings();

    for (TObjectPtr<URetargetChainSettings> const &ChainSettings : AllChainSettings)
    {
        if (ChainSettings == nullptr)
            continue;

        const FBoneChain *SrcBoneChain = SourceIKRig->GetRetargetChainByName(ChainSettings->SourceChain);
        const FBoneChain *TgtBoneChain = TargetIKRig->GetRetargetChainByName(ChainSettings->TargetChain);

        if (SrcBoneChain == nullptr || TgtBoneChain == nullptr)
            continue;

        auto ChainRoot = FName(TEXT("Root"));
        if (ChainSettings->SourceChain == ChainRoot || ChainSettings->TargetChain == ChainRoot)
        {
            continue;
        }

        FIKRigRetargetChainDetail SrcIkRigRtgChainDetail{SrcBoneChain, SrcRigSkeleton, SrcRtgSkeleton};
        FIKRigRetargetChainDetail TgtIkRigRtgChainDetail{TgtBoneChain, TgtRigSkeleton, TgtRtgSkeleton};

        if (SrcIkRigRtgChainDetail.IKRigRetargetMap.IsEmpty() || TgtIkRigRtgChainDetail.IKRigRetargetMap.IsEmpty())
        {
            UE_LOG(EasyPose, Log, TEXT("Chain Empty: %s: %d, %s: %d"), *SrcBoneChain->ChainName.ToString(), SrcIkRigRtgChainDetail.IKRigRetargetMap.Num(), *TgtBoneChain->ChainName.ToString(), TgtIkRigRtgChainDetail.IKRigRetargetMap.Num());
            continue;
        }

        SrcIkRigRtgChainsDetailList.Add(SrcIkRigRtgChainDetail);
        TgtIkRigRtgChainsDetailList.Add(TgtIkRigRtgChainDetail);

        if(ShouldMatch(matching::EFlag::StepMatchSubsequenceChain))
        {
            bool SrcAddedChild = AddChildren(SrcRtgSkeleton, SrcIkRigRtgChainsDetailList);
            bool TgtAddedChild = AddChildren(TgtRtgSkeleton, TgtIkRigRtgChainsDetailList);

            if(SrcAddedChild != TgtAddedChild)
            {
                UE_LOG(EasyPose, Warning, TEXT("inconsistent subsequence chain between source and target: %s, %s"),
                    *SrcIkRigRtgChainDetail.BoneChain->ChainName.ToString()
                    , *TgtIkRigRtgChainDetail.BoneChain->ChainName.ToString()
                    );
            }
        }
    }

    for(int i=0; i<SrcIkRigRtgChainsDetailList.Num(); i++)
    {
        auto &SrcIkRigRtgChainDetail = SrcIkRigRtgChainsDetailList[i];
        auto &TgtIkRigRtgChainDetail = TgtIkRigRtgChainsDetailList[i];
        int32 SrcRtgId = SrcIkRigRtgChainDetail.IKRigRetargetMap.Last().RetargetId;
        int32 TgtRtgId = TgtIkRigRtgChainDetail.IKRigRetargetMap.Last().RetargetId;

        if (!SrcIkRigRtgChainDetail.ChildrenId.IsEmpty() && !TgtIkRigRtgChainDetail.ChildrenId.IsEmpty())
        {
            int32 SrcMostAlignedId = 0;

            if(SrcIkRigRtgChainDetail.ChildrenId.Num() > 1)
            {
                FVector SrcAvgDirection = FVector::ZeroVector;
                TArray<FVector> SrcChildrenDirectionNormal;

                for(auto const Child : SrcIkRigRtgChainDetail.ChildrenId)
                {
                    auto &SrcChildIkRigRtgChainDetail = SrcIkRigRtgChainsDetailList[Child];
                    auto const &SrcNextChainInfo = SrcChildIkRigRtgChainDetail.IKRigRetargetMap[0];
                    auto SrcNextRtgId = SrcNextChainInfo.RetargetId;
                    SrcChildrenDirectionNormal.Add((SrcRtgSkeleton.RetargetGlobalPose[SrcNextRtgId].GetTranslation() - SrcRtgSkeleton.RetargetGlobalPose[SrcRtgId].GetTranslation()).GetSafeNormal());
                    SrcAvgDirection += SrcChildrenDirectionNormal.Last();
                }

                double SrcMostAlignedVal = 0;

                for(int32 CId=0; CId<SrcChildrenDirectionNormal.Num(); CId++)
                {
                    auto const &ChildDirectionNormal = SrcChildrenDirectionNormal[CId];
                    auto ChildAligned = FMath::Abs(SrcAvgDirection.Dot(ChildDirectionNormal));

                    if(SrcMostAlignedVal < ChildAligned)
                    {
                        SrcMostAlignedVal = ChildAligned;
                        SrcMostAlignedId = CId;
                    }
                }
            }

            int32 BestChildChainDetailId = SrcIkRigRtgChainDetail.ChildrenId[SrcMostAlignedId];
            SrcIkRigRtgChainDetail.ChildrenId.Empty();
            SrcIkRigRtgChainDetail.ChildrenId.Add(BestChildChainDetailId);

            auto &SrcBestChildIkRigRtgChainDetail = SrcIkRigRtgChainsDetailList[BestChildChainDetailId];
            auto const &SrcBestChildChainName = SrcBestChildIkRigRtgChainDetail.BoneChain->ChainName;

            UE_LOG(EasyPose, VeryVerbose, TEXT("Best Child: %s"), *SrcBestChildChainName.ToString());

            auto ChainSetting = AllChainSettings.FindByPredicate([&SrcBestChildChainName](TObjectPtr<URetargetChainSettings> const &El) {
                return El->SourceChain == SrcBestChildChainName;
            });

            check(ChainSetting != nullptr);
            BestChildChainDetailId = INDEX_NONE;

            for(auto const ChildChainDetailId : TgtIkRigRtgChainDetail.ChildrenId)
            {
                auto &TgtChildIkRigRtgChainDetail = TgtIkRigRtgChainsDetailList[ChildChainDetailId];
                if(TgtChildIkRigRtgChainDetail.BoneChain->ChainName == (*ChainSetting)->TargetChain)
                {
                    BestChildChainDetailId = ChildChainDetailId;
                    break;
                }
            }

            if(BestChildChainDetailId == INDEX_NONE)
            {
                UE_LOG(EasyPose, Warning, TEXT("Subsequence chain not found in target: %s"), *(*ChainSetting)->TargetChain.ToString());
                SrcIkRigRtgChainDetail.ChildrenId.Empty();
                TgtIkRigRtgChainDetail.ChildrenId.Empty();
            }
            else
            {
                TgtIkRigRtgChainDetail.ChildrenId.Empty();
                TgtIkRigRtgChainDetail.ChildrenId.Add(BestChildChainDetailId);
            }
        }
    } /// preparing child data

    FScopedSlowTask MatchingPoseProgress(SrcIkRigRtgChainsDetailList.Num(), FText::FromString(TEXT("Matching poses...")));
	MatchingPoseProgress.MakeDialog();

    UE_LOG(EasyPose, Verbose, TEXT("Bone Alignment: %f, Rotataion Offset Tolerance: %f"), BoneAxesDotProductScalar, FPTolerance);

    for (int i = 0; i < SrcIkRigRtgChainsDetailList.Num(); i++)
    {
        auto const &SrcIkRigRtgChainDetail = SrcIkRigRtgChainsDetailList[i];
        auto const &TgtIkRigRtgChainDetail = TgtIkRigRtgChainsDetailList[i];

        auto const &SrcChainName = SrcIkRigRtgChainDetail.BoneChain->ChainName;
        auto const &TgtChainName = TgtIkRigRtgChainDetail.BoneChain->ChainName;
        MatchingPoseProgress.EnterProgressFrame(1, FText::FromString(FString::Printf(TEXT("Matching %s -> %s..."),
            *SrcChainName.ToString()
            , *TgtChainName.ToString()
            )));

        auto SrcWatermarkLoc = SrcRtgSkeleton.RetargetGlobalPose[SrcIkRigRtgChainDetail.IKRigRetargetMap[0].RetargetId].GetTranslation();
        int RotationsCount = 0;

        FQuat RotOffset;
        FVector Direction;

        bool MatchSimilar = MatchingMode == MatchMode::SIMILAR;

        if (MatchingMode == MatchMode::DYNAMIC)
        {
            MatchSimilar = SrcIkRigRtgChainDetail.IKRigRetargetMap.Num() != TgtIkRigRtgChainDetail.IKRigRetargetMap.Num();
        }

        for (int32 SrcId = 0, TgtId = 0; SrcId < SrcIkRigRtgChainDetail.IKRigRetargetMap.Num() - 1 && TgtId < TgtIkRigRtgChainDetail.IKRigRetargetMap.Num() - 1;)
        {
            auto const &SrcChainInfo = SrcIkRigRtgChainDetail.IKRigRetargetMap[SrcId];
            auto const &SrcNextChainInfo = SrcIkRigRtgChainDetail.IKRigRetargetMap[SrcId + 1];
            auto SrcRtgId = SrcChainInfo.RetargetId;
            auto SrcNextRtgId = SrcNextChainInfo.RetargetId;

            auto const &TgtChainInfo = TgtIkRigRtgChainDetail.IKRigRetargetMap[TgtId];
            auto const &TgtNextChainInfo = TgtIkRigRtgChainDetail.IKRigRetargetMap[TgtId + 1];
            auto TgtRtgId = TgtChainInfo.RetargetId;
            auto TgtNextRtgId = TgtNextChainInfo.RetargetId;

            auto const &TgtRtgBoneName = TgtRigSkeleton.BoneNames[TgtChainInfo.IKRigId];

            if (AlwaysReset)
            {
                IKRtgCtrl->ResetRetargetPose(TgtRtgPoseName, TArray<FName>{TgtRtgBoneName}, Target);
            }

            TgtRtgSkeleton.GenerateRetargetPose(TgtRtgPoseName, TgtRtgPose, TgtRtgRootName);
            OrgTgtRtgSkeleton.GenerateRetargetPose(TgtRtgPoseName, TgtRtgPose, TgtRtgRootName);

            if (MatchSimilar)
            {
                float SrcTotalLenToNextPercent = (SrcChainInfo.TotalLenUntilCurrent + SrcChainInfo.LenToNext) / SrcIkRigRtgChainDetail.TotalLength;
                float TgtTotalLenToNextPercent = (TgtChainInfo.TotalLenUntilCurrent + TgtChainInfo.LenToNext) / TgtIkRigRtgChainDetail.TotalLength;

                UE_LOG(EasyPose, VeryVerbose, TEXT("Src: %f = (%f + %f) / %f"), SrcTotalLenToNextPercent, SrcChainInfo.TotalLenUntilCurrent, SrcChainInfo.LenToNext, SrcIkRigRtgChainDetail.TotalLength);
                UE_LOG(EasyPose, VeryVerbose, TEXT("Tgt: %f = (%f + %f) / %f"), TgtTotalLenToNextPercent, TgtChainInfo.TotalLenUntilCurrent, TgtChainInfo.LenToNext, TgtIkRigRtgChainDetail.TotalLength);

                if (TgtTotalLenToNextPercent > SrcTotalLenToNextPercent)
                {
                    SrcId++;
                    continue;
                }

                auto DiffLenPercentToNext = 1 - ((SrcTotalLenToNextPercent - TgtTotalLenToNextPercent) * SrcIkRigRtgChainDetail.TotalLength / SrcChainInfo.LenToNext);
                auto SrcCurrentBoneLoc = SrcRtgSkeleton.RetargetGlobalPose[SrcRtgId].GetTranslation();
                auto SrcNextBoneLoc = SrcRtgSkeleton.RetargetGlobalPose[SrcNextRtgId].GetTranslation();

                auto NextPoint = (SrcNextBoneLoc - SrcCurrentBoneLoc) * (DiffLenPercentToNext) + SrcCurrentBoneLoc;
                Direction = (NextPoint - SrcWatermarkLoc).GetSafeNormal();
                SrcWatermarkLoc = NextPoint;
            }
            else
            {
                Direction = (SrcRtgSkeleton.RetargetGlobalPose[SrcNextRtgId].GetTranslation() - SrcRtgSkeleton.RetargetGlobalPose[SrcRtgId].GetTranslation()).GetSafeNormal();
            }

            MatchStep13(SrcRtgSkeleton, TgtRtgSkeleton, SrcRtgId, TgtRtgId, TgtNextRtgId, Direction, StepFlag, FPTolerance, BoneAxesDotProductScalar, StepMiddleBoneXOnly);

            auto FinalOffset = GetRotationDifference(OrgTgtRtgSkeleton.RetargetLocalPose[TgtRtgId].GetRotation(), TgtRtgSkeleton.RetargetLocalPose[TgtRtgId].GetRotation());

            if (NotEmptyOrIdentity(FinalOffset, FPTolerance))
            {
                RotationsCount++;
                IKRtgCtrl->SetRotationOffsetForRetargetPoseBone(TgtRtgBoneName, FinalOffset, Target);
                UE_LOG(EasyPose, VeryVerbose, TEXT("Final Offset: %s: %s"), *TgtRtgBoneName.ToString(), *FinalOffset.ToString());
            }

            TgtId++;

            if (!MatchSimilar)
                SrcId++;
        }

        auto const &SrcChainInfo = SrcIkRigRtgChainDetail.IKRigRetargetMap.Last();
        auto SrcRtgId = SrcChainInfo.RetargetId;
        auto const &TgtChainInfo = TgtIkRigRtgChainDetail.IKRigRetargetMap.Last();
        auto TgtRtgId = TgtChainInfo.RetargetId;

        auto TgtRtgBoneName = TgtRtgSkeleton.BoneNames[TgtRtgId];
        auto SrcRtgBoneName = SrcRtgSkeleton.BoneNames[SrcRtgId];

        if (!SrcIkRigRtgChainDetail.ChildrenId.IsEmpty() && !TgtIkRigRtgChainDetail.ChildrenId.IsEmpty())
        {
            if (AlwaysReset)
            {
                IKRtgCtrl->ResetRetargetPose(TgtRtgPoseName, TArray<FName>{TgtRtgBoneName}, Target);
            }

            TgtRtgSkeleton.GenerateRetargetPose(TgtRtgPoseName, TgtRtgPose, TgtRtgRootName);
            OrgTgtRtgSkeleton.GenerateRetargetPose(TgtRtgPoseName, TgtRtgPose, TgtRtgRootName);
            
            auto SrcChildIkRigRtgChainDetail = SrcIkRigRtgChainsDetailList[SrcIkRigRtgChainDetail.ChildrenId[0]];
            auto TgtChildIkRigRtgChainDetail = TgtIkRigRtgChainsDetailList[TgtIkRigRtgChainDetail.ChildrenId[0]];

            auto SrcNextRtgId = SrcChildIkRigRtgChainDetail.IKRigRetargetMap[0].RetargetId;
            auto TgtNextRtgId = TgtChildIkRigRtgChainDetail.IKRigRetargetMap[0].RetargetId;
            UE_LOG(EasyPose, Verbose, TEXT("Matching Subsequence Chain: %s -> %s"),
                *SrcChildIkRigRtgChainDetail.BoneChain->ChainName.ToString(), *TgtChildIkRigRtgChainDetail.BoneChain->ChainName.ToString());
            Direction = (SrcRtgSkeleton.RetargetGlobalPose[SrcNextRtgId].GetTranslation() - SrcRtgSkeleton.RetargetGlobalPose[SrcRtgId].GetTranslation()).GetSafeNormal();

            MatchStep13(SrcRtgSkeleton, TgtRtgSkeleton, SrcRtgId, TgtRtgId, TgtNextRtgId, Direction, StepFlag, FPTolerance, BoneAxesDotProductScalar, StepMiddleBoneXOnly);

            auto FinalOffset = GetRotationDifference(OrgTgtRtgSkeleton.RetargetLocalPose[TgtRtgId].GetRotation(), TgtRtgSkeleton.RetargetLocalPose[TgtRtgId].GetRotation());

            if (NotEmptyOrIdentity(FinalOffset, FPTolerance))
            {
                RotationsCount++;
                IKRtgCtrl->SetRotationOffsetForRetargetPoseBone(TgtRtgBoneName, FinalOffset, Target);
                UE_LOG(EasyPose, VeryVerbose, TEXT("Final Offset: %s: %s"), *TgtRtgBoneName.ToString(), *FinalOffset.ToString());
            }
        }
        else if (ShouldMatch(matching::EFlag::StepMatchBoneTailRotation))
        {

            if (AlwaysReset)
            {
                IKRtgCtrl->ResetRetargetPose(TgtRtgPoseName, TArray<FName>{TgtRtgBoneName}, Target);
            }

            TgtRtgSkeleton.GenerateRetargetPose(TgtRtgPoseName, TgtRtgPose, TgtRtgRootName);
            OrgTgtRtgSkeleton.GenerateRetargetPose(TgtRtgPoseName, TgtRtgPose, TgtRtgRootName);

            RotOffset = GetRotationOffset(TgtRtgSkeleton.RetargetGlobalPose[TgtRtgId].GetRotation(), SrcRtgSkeleton.RetargetGlobalPose[SrcRtgId].GetRotation(), BoneAxesDotProductScalar, BoneAxesDotProductScalar);
            UE_LOG(EasyPose, VeryVerbose, TEXT("Matching Bone Tail Rotation: %s -> %s: %s"), *SrcRtgBoneName.ToString(), *TgtRtgBoneName.ToString(), *RotOffset.ToString());

            if (NotEmptyOrIdentity(RotOffset, FPTolerance))
            {
                RotationsCount++;
                UE_LOG(EasyPose, Verbose, TEXT("Matched Bone Tail Rotation: %s -> %s"), *SrcRtgBoneName.ToString(), *TgtRtgBoneName.ToString());
                IKRtgCtrl->SetRotationOffsetForRetargetPoseBone(TgtRtgBoneName, (IKRtgCtrl->GetRotationOffsetForRetargetPoseBone(TgtRtgBoneName, Target) * RotOffset).GetNormalized(), Target);
            }
        }

        UE_LOG(EasyPose, Log, TEXT("Match %s: %s (%d) - %s (%d): %d rotations"), MatchSimilar ? TEXT("Similar") : TEXT("Exact"), *SrcChainName.ToString(), SrcIkRigRtgChainDetail.IKRigRetargetMap.Num(), *TgtChainName.ToString(), TgtIkRigRtgChainDetail.IKRigRetargetMap.Num(), RotationsCount);

        if (RotationsCount > 0)
        {
            TotalRotationsCount += RotationsCount;
        }
    }

    if (TotalRotationsCount > 0)
    {
        UE_LOG(EasyPose, Log, TEXT("Asset \"%s\": made %d adjustments with pose: \"%s\"."),
               *IKRetargeter->GetName(), TotalRotationsCount,
               *IKRtgCtrl->GetCurrentRetargetPoseName(Target).ToString());
    }
    else
    {
        UE_LOG(EasyPose, Log, TEXT("Asset \"%s\": pose \"%s\" fit already!"),
               *IKRetargeter->GetName(),
               *IKRtgCtrl->GetCurrentRetargetPoseName(Target).ToString());
    }

    return TotalRotationsCount;
}

void FEasyPoseModule::PluginButtonClicked()
{
    auto ToolMenus = UToolMenus::Get();

    TSharedPtr<SWindow> ActiveWindow = FSlateApplication::Get().GetActiveTopLevelWindow();
    FEditorModeTools &EditorModeTools = GLevelEditorModeTools();

    UAssetEditorSubsystem *AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();

    if (AssetEditorSubsystem)
    {
        TArray<UObject *> OpenAssetEditors = AssetEditorSubsystem->GetAllEditedAssets();

        for (auto RtgAsset : OpenAssetEditors)
        {
            MatchPose(Cast<UIKRetargeter>(RtgAsset));
        }
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FEasyPoseModule, EasyPose)