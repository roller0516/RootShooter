// Fill out your copyright notice in the Description page of Project Settings.


#include "ASPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Item/ASWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AASPlayerCharacter::AASPlayerCharacter() :
	bIsAiming(false),
	CameraDefaultFOV(60.f),
	CameraZoomedFOV(40.f),
	CurrentCameraFOV(0.f),
	ZoomSpeed(5.f),
	BaseTurnRate(45.f),
	BaseLookUpRate(45.f),
	HipTurnRate(90.f),
	HipLookUpRate(90.f),
	AimingTurnRate(20.f),
	AimingLookUpRate(20.0f),
	CrossHairSpreadMultiplier(1.25f),
	ShootTimeDuration(0.05f),
	bFiringBullet(false),
	AttackDelayTime(0.1)
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 250.f;
	CameraBoom->bUsePawnControlRotation = true; // 컨트롤러를 기준으로 회전
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 50.f);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCam"));
	Camera->SetupAttachment(CameraBoom);
	Camera->bUsePawnControlRotation = false; // 스프링 암 기준으로 회전 x

	bUseControllerRotationYaw = true;

	GetCharacterMovement()->bOrientRotationToMovement = false; //입력한 방향으로 로테이션
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	defaultMovementSpeed = GetCharacterMovement()->MaxWalkSpeed;

	//------------------------------------------------------------------------------Cross Spread Init
	//생성자에서 초기화 하려니까 너무 길어져서.. 이쪽으로 옮김 
	SpreadMin = 0;
	SpreadHipMax = 0.3f;
	SpreadAimingMax = 0.3f;
	SpreadCurrent = 0.f;
	SpreadDecreaseSpeed = 0.25f;
	SpreadIncreaseSpeed = 0.01f;

	//------------------------------------------------------------------------------Skill
	bUseSkill = false;
	TraceDistance = 300;
}

// Called when the game starts or when spawned
void AASPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(defaultMappingContext, 0);
		}
	}

	EquipWeapon(SpawnDefaultWeapon());
	
	Camera->FieldOfView = CameraDefaultFOV;
	CurrentCameraFOV = CameraDefaultFOV;
}

// Called every frame
void AASPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CameraInterpZoom(DeltaTime);
	SetLookRates();
	CalcCrossHairSpread(DeltaTime);
	CalcAimingSpeed();
	//SKill
	BuildTypeSkillTrace();
}

// Called to bind functionality to input
void AASPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(IAJump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(IAJump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(IAMove, ETriggerEvent::Triggered, this, &AASPlayerCharacter::CharacterMove);

		//Look
		EnhancedInputComponent->BindAction(IALook, ETriggerEvent::Triggered, this, &AASPlayerCharacter::CharacterLook);

		//Fire
		EnhancedInputComponent->BindAction(IAAttack, ETriggerEvent::Triggered, this, &AASPlayerCharacter::MouseLeftClick);

		//Test Skill
		EnhancedInputComponent->BindAction(IASkill1, ETriggerEvent::Triggered, this, &AASPlayerCharacter::UseSkill);

		//Aiming
		EnhancedInputComponent->BindAction(IAAiming,ETriggerEvent::Triggered,this,&AASPlayerCharacter::AimingButtonPressed);
		EnhancedInputComponent->BindAction(IAAiming,ETriggerEvent::Completed,this,&AASPlayerCharacter::AimingButtonReleased);

		//Reloading
		EnhancedInputComponent->BindAction(IAReloading,ETriggerEvent::Triggered,this,&AASPlayerCharacter::Reloading);
	}
}


void AASPlayerCharacter::CharacterMove(const FInputActionValue& value)
{
	FVector2D movementVector = value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, movementVector.Y);
		AddMovementInput(RightDirection, movementVector.X);
	}
}

void AASPlayerCharacter::CharacterLook(const FInputActionValue& value)
{
	FVector2D LookAxisVector = value.Get<FVector2D>() ;

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X * BaseTurnRate * GetWorld()->GetDeltaSeconds());
		AddControllerPitchInput(LookAxisVector.Y * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void AASPlayerCharacter::MouseLeftClick()
{
	if(bUseSkill)
	{
		CreateBarrier();
		bUseSkill = false;
	}
	else
	{
		FireWeapon();
	}
}

void AASPlayerCharacter::CalcAimingSpeed()
{
	//if(bIsAiming)
	//	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	//else
	//	GetCharacterMovement()->MaxWalkSpeed = defaultMovementSpeed;
}

void AASPlayerCharacter::EquipWeapon(AASWeapon* WeaponToEquip)
{
	if(WeaponToEquip)
	{
		//WeaponToEquip->GetAreaSphere()->SetCollisionResponseToAllChannels(
		//	ECollisionResponse::ECR_Ignore);
		//WeaponToEquip->GetCollisionBox()->SetCollisionResponseToAllChannels(
		//	ECollisionResponse::ECR_Ignore);
		
		
		const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName("RightHandSocket");
		if(HandSocket)
		{
			HandSocket->AttachActor(Cast<AActor>(WeaponToEquip),GetMesh());
		}

		EquippedWeapon = WeaponToEquip;
	}
}

void AASPlayerCharacter::DropWeapon()
{
	if(EquippedWeapon)
	{
		FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld,true);
		EquippedWeapon->GetItemMesh()->DetachFromComponent(DetachmentTransformRules);
	}
}

AASWeapon* AASPlayerCharacter::SpawnDefaultWeapon()
{
	if(DefaultWeapon)
	{
		AASWeapon* spawnWeapon = GetWorld()->SpawnActor<AASWeapon>(DefaultWeapon);

		return spawnWeapon;
	}
	return nullptr;
}

void AASPlayerCharacter::Reloading()
{
	UAnimMontage* reloading = Montages[MontageType::Reloading];
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && reloading)
	{
		AnimInstance->Montage_Play(reloading);
		AnimInstance->Montage_JumpToSection(FName("StartReloading"));
	}
}

void AASPlayerCharacter::GrapClip()
{
	if(EquippedWeapon == nullptr) return;

	int32 ClipBoneIndex{ EquippedWeapon->GetItemMesh()->GetBoneIndex(EquippedWeapon->GetClipBoneName())};
	ClipTransform = EquippedWeapon->GetItemMesh()->GetBoneTransform(ClipBoneIndex);
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, true);
	HandSceneComponent->AttachToComponent(GetMesh(), AttachmentRules,FName("Hand_L"));
	HandSceneComponent->SetWorldTransform(ClipTransform);

	EquippedWeapon->SetMovingClip(true);
}

void AASPlayerCharacter::ReplaceClip()
{
	EquippedWeapon->SetMovingClip(false);
}

void AASPlayerCharacter::FireWeapon()
{
	if(bFiringBullet) return;
	
	const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName("BarrelSocket");
	if (BarrelSocket)
	{
		const FTransform muzzleTr = BarrelSocket->GetSocketTransform(GetMesh());

		if (MuzzleFlash)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, muzzleTr);
		FVector BeamEnd;
		bool bIsBeam = GetBeamEndLocation(muzzleTr.GetLocation(),BeamEnd);
		if(bIsBeam)
		{
			if (ShotLineParticle)
			{
				UParticleSystemComponent* beam = UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					ShotLineParticle,
					muzzleTr);
					
				if (beam)
				{
					beam->SetVectorParameter(FName("Target"), BeamEnd);
				}
			}
		}
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montages[MontageType::Attack])
	{
		AnimInstance->Montage_Play(Montages[MontageType::Attack]);
		AnimInstance->Montage_JumpToSection(FName("StartFire"));
	}

	StartCrossHairBulletFire();
}

void AASPlayerCharacter::StartCrossHairBulletFire()
{
	bFiringBullet = true;
	
	GetWorldTimerManager().SetTimer(CrossHairShootTimer,this,
		&AASPlayerCharacter::FinishCrossHairBulletFire,ShootTimeDuration);

	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Red,"Fire");
}

void AASPlayerCharacter::FinishCrossHairBulletFire()
{
	bFiringBullet = false;
	GetWorldTimerManager().ClearTimer(CrossHairShootTimer);
}

void AASPlayerCharacter::CreateBarrier()
{
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	FTransform ft;

	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1,1,FColor::Red,FString::Printf(TEXT("%f : %f :%f"),GroundPlacementPoint.X,GroundPlacementPoint.Y,GroundPlacementPoint.Z));
	}
	
	ft.SetLocation(GroundPlacementPoint);
	GetWorld()->SpawnActor<AActor>(barrierActor, ft, spawnParams);
}

void AASPlayerCharacter::UseSkill()
{
	bUseSkill = true;
}

void AASPlayerCharacter::BuildTypeSkillTrace()
{
	if(!bUseSkill) return;
	
	FVector ground = (FVector::UpVector * -100.f); //지면까지의 거리
	
	FVector Start = GetActorLocation() + (GetActorForwardVector() * TraceDistance) + ground;
	FVector End = Start + FVector::UpVector * 1000;
	FVector HalfSize =  FVector(5,5,5);
	
	TArray<AActor*> IgnoreActors;
	FHitResult Hit;
	
	UKismetSystemLibrary::BoxTraceSingle(
		GetWorld(),
		Start,
		End,
		HalfSize,
		FRotator::ZeroRotator,
		TraceTypeQuery1,
		false,
		IgnoreActors,
		EDrawDebugTrace::ForOneFrame,
		Hit,true
		);


	if(Hit.bBlockingHit)
	{
		GroundPlacementPoint = Hit.ImpactPoint;
	}
}

void AASPlayerCharacter::AimingButtonPressed()
{
	bIsAiming = true;
}

void AASPlayerCharacter::AimingButtonReleased()
{
	bIsAiming = false;
}

void AASPlayerCharacter::CameraInterpZoom(float deltaTime)
{
	if(bIsAiming)
	{
		CurrentCameraFOV = FMath::FInterpTo(CurrentCameraFOV,CameraZoomedFOV,deltaTime,ZoomSpeed);
		Camera->SetFieldOfView(CurrentCameraFOV);
	}
	else
	{	
		CurrentCameraFOV =  FMath::FInterpTo(CurrentCameraFOV,CameraDefaultFOV,deltaTime,ZoomSpeed);
		Camera->SetFieldOfView(CurrentCameraFOV);
	}
}

void AASPlayerCharacter::SetLookRates()
{
	if(bIsAiming)
	{
		BaseTurnRate = AimingTurnRate;
		BaseLookUpRate = AimingLookUpRate;
	}
	else
	{
		BaseTurnRate = HipTurnRate;
		BaseLookUpRate = HipLookUpRate;
	}
}

void AASPlayerCharacter::CalcCrossHairSpread(float deltaTime)
{
	//속도에 따라 크로스헤드 벌어지는 정도
	//ex) 현재 속도 0 ~ 600 : 0 ~ 1
	FVector2D walkSpeed = FVector2D(0.f,600.f);
	FVector2D velocityMultiply = FVector2D{0.0f,1.0f};
	FVector velocity = GetVelocity();
	velocity.Z = 0;
	
	if(bFiringBullet)
	{
		IncreaseSpread(SpreadIncreaseSpeed);
	}
	
	if(SpreadMin <= SpreadCurrent)
	{
		DecreaseSpread(deltaTime * SpreadDecreaseSpeed);
	}

	CrossHairShootingFactor = SpreadCurrent;

	CrossHairVelocityFactor = FMath::GetMappedRangeValueClamped(walkSpeed,velocityMultiply,velocity.Size());
	CrossHairSpreadMultiplier = 0.5f + CrossHairVelocityFactor + CrossHairShootingFactor - CrossHairAimFactor;
}

void AASPlayerCharacter::IncreaseSpread(float increaseAmount)
{
	float max  = 0;
	bIsAiming ? max = SpreadAimingMax : max = SpreadHipMax;
	if(SpreadCurrent >= max)
	{
		SpreadCurrent = max;
	}
	else
	{
		SpreadCurrent += increaseAmount;
	}
}

void AASPlayerCharacter::DecreaseSpread(float decreaseAmount)
{
	if(SpreadCurrent <= SpreadMin)
	{
		SpreadCurrent = SpreadMin;
	}
	else
	{
		SpreadCurrent -= decreaseAmount;
	}
}

bool AASPlayerCharacter::GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrossHairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	CrossHairLocation.Y -= 50.f;

	FVector CrossHairWorldPosition;
	FVector CrossHairWorldDirection;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
			UGameplayStatics::GetPlayerController(this, 0),
			CrossHairLocation, CrossHairWorldPosition,CrossHairWorldDirection);

	if (bScreenToWorld)
	{
		FHitResult ScreenTraceHit;
		const FVector Start{ CrossHairWorldPosition };
		const FVector End{ CrossHairWorldPosition + CrossHairWorldDirection * 50000.f};

		OutBeamLocation = End;

		GetWorld()->LineTraceSingleByChannel(
			ScreenTraceHit,
			Start,
			End,
			ECollisionChannel::ECC_Visibility);

		if (ScreenTraceHit.bBlockingHit)
		{
			OutBeamLocation = ScreenTraceHit.Location;
			// 라인 트레이스이기 때문에 물체의 부피가 없어 뚫리는 현상을 없애기 위함.
		}
		FHitResult WeaponTraceHit;
		const FVector WeaponTraceStart{MuzzleSocketLocation};
		const FVector WeaponTraceEnd{OutBeamLocation};
				
		GetWorld()->LineTraceSingleByChannel(
			WeaponTraceHit,
			WeaponTraceStart,
			WeaponTraceEnd,
			ECollisionChannel::ECC_Visibility);
				
		if(WeaponTraceHit.bBlockingHit)
		{
			OutBeamLocation = WeaponTraceHit.Location;
		}
		return true;
	}

	return false;
}

