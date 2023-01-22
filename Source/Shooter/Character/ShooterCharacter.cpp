// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Shooter/Components/WeaponComponent.h"
#include "Shooter/Weapon/Weapon.h"


//Engine
#include "Animation/AnimInstance.h"

//components
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

//Utils
#include "Animation/AnimMontage.h"

//Utils
#include "Kismet/GameplayStatics.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AShooterCharacter::AShooterCharacter():
	BaseTurnRate{ 45.f },
	BaseLookUpRate{ 45.f },
	PawYawRate{ 540.f }
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a Camera Boom ( Pulls in towards the character if there is a collision )
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f; //The canera follow at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; //Rotate the arm base base on the controller

	//Create Follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamwera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // attach camera to end of boom
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//Don´t rotate when the con trroller rotates. Let the controller only affect the camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement=  true ;
	GetCharacterMovement()->RotationRate =  FRotator{ 0.f, PawYawRate, 0.f  } ;
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = .2f;

	CombatWeaponComp = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
	CombatWeaponComp->SetIsReplicated(true);



}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AShooterCharacter::PlayFireMontage(bool bAiming)
{
	if (CombatWeaponComp == nullptr || CombatWeaponComp->GetEquippedWeapon() == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && FireMontage)
	{
		AnimInstance->Montage_Play(FireMontage);
		/*FName SectionName;
		SectionName = bAiming ? FName("RifleAim") : FName("RifleHip");
		AnimInstance->Montage_JumpToSection(SectionName);*/
	}
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	/*UE_LOG(LogTemp, Warning, TEXT("BeginPlay()  Call"));

	int myInt{ 42 };

	UE_LOG(LogTemp, Warning, TEXT("int myInt %d"), myInt);*/
	
}


// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	
	//Setup Move bindings.
	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);

	// Set up "look" bindings.
	//Mouse
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//GamePad
	PlayerInputComponent->BindAxis("TurnRate", this, &AShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterCharacter::LookUpRate);


	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AShooterCharacter::FireWeaponPressed);

	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &AShooterCharacter::EquipButtonPressed);

}

//void AShooterCharacter::PostInitializeComponents()
//{
//	Super::PostInitializeComponents();
//	if (Combat)
//	{
//		Combat->Character = this;
//	}
//}



void AShooterCharacter::TurnAtRate(float Rate)
{
	//Calcuñate delta for this frame from the rate information
	//Input * velgiroBase * DeltaTime       ( DeltaTime =  1 / FPS,  Tiempo entre frames , DeltaTime -> 1 tick   )
	// input[0,1] * deg/sec * sec/frames -> deg/frames
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds()); 
}

void AShooterCharacter::LookUpRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());

}

void AShooterCharacter::EquipButtonPressed()
{
	
	
	if (CombatWeaponComp)//&& HasAuthority())
	{
		CombatWeaponComp->EquipWeapon(OverlappingWeapon);
	}
}

void AShooterCharacter::FireWeaponPressed()
{
	if (!OverlappingWeapon)
		return;
	if (OverlappingWeapon->GetWeaponState() != EWeaponState::EWS_Equipped)
		 return;
	
	//UE_LOG(LogTemp, Warning, TEXT("Firing Wepaon"))
	if (CombatWeaponComp)
	{
		CombatWeaponComp->FireWeapon();
	}
	
	//Server_FireWeapon();
}

//
//void AShooterCharacter::FireWeaponHandle()
//{
//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//	if (AnimInstance && FireMontage)
//	{
//
//		AnimInstance->Montage_Play(FireMontage);
//		AnimInstance->Montage_JumpToSection(FName("StartFire"));
//
//	}
//
//	if (!HasAuthority()) return;
//
//
//	const USkeletalMeshSocket* MuzzleFlash_BarrelSocket = GetMesh()->GetSocketByName(FName("BarrelSocket"));
//	//Line trace by channel
//}
//
//
//void AShooterCharacter::Server_FireWeapon_Implementation()
//{
//	Multicast_FireWeapon();
//}
//
//void AShooterCharacter::Multicast_FireWeapon_Implementation()
//{
//	FireWeaponHandle();
//}
//




void AShooterCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}
	if (LastWeapon)
	{
		LastWeapon->ShowPickupWidget(false);
	}
}

void AShooterCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(false);
	}
	OverlappingWeapon = Weapon;
	
	if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}



void AShooterCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.f))
	{
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0.f, Rotation.Yaw , 0.f };

		const FVector Direction{ FRotationMatrix{ YawRotation }.GetUnitAxis(EAxis::X) };
		
		//FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);

	}
}

void AShooterCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.f))
	{
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0.f, Rotation.Yaw , 0.f };

		const FVector Direction{ FRotationMatrix{ YawRotation }.GetUnitAxis(EAxis::Y) };

		//FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AShooterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AShooterCharacter, OverlappingWeapon, COND_OwnerOnly);
}

