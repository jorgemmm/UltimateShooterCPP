// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponComponent.h"
//#include "Components/WeaponComponent.h"

#include "Shooter/Character/ShooterCharacter.h"
#include "Shooter/Weapon/Weapon.h"
#include "GameFramework/Actor.h"
#include "Shooter/Character/ShooterAnimInstance.h"

//Components
#include "Engine/SkeletalMeshSocket.h"
#include "Engine/SkeletalMesh.h"
#include "Components/SkeletalMeshComponent.h"

//Utils
//#include "Kismet/GameplayStatics.h"

#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	SetIsReplicated(true);
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AShooterCharacter>(GetOwner());


	if(!Character)
	{
		UE_LOG(LogTemp, Error, TEXT("No se encuentra Owner in WepaonCompoonent::Beginplay"))
		return;
	}

	AnimInstance = Cast< UShooterAnimInstance >( Character->GetMesh()->GetAnimInstance() );

	// ...
	
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UWeaponComponent::EquipWeapon(AWeapon* WeaponToEquip)
{

	Server_EquipWeapon(WeaponToEquip);
		
}

void UWeaponComponent::Server_EquipWeapon_Implementation(AWeapon* WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr) return;

	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	
	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	
	if (HandSocket)
	{
		HandSocket->AttachActor(EquippedWeapon, Character->GetMesh());
	}
	EquippedWeapon->SetOwner(Character);

	EquippedWeapon->ShowPickupWidget(false);

}



//EWeaponState UWeaponComponent::GetWeaponState() const
//{
//	return EquippedWeapon->GetWeaponState();
//}

void UWeaponComponent::FireWeapon()
{
	Server_FireWeapon();
}

void UWeaponComponent::FireWeapon_Handle()
{
	if (!EquippedWeapon) return;
	
	    //if (AnimInstance && Character->GetFireMontage())
		//AnimInstance->playMontageFire();
		//AnimInstance->Montage_Play(Character->GetFireMontage());
		//AnimInstance->Montage_JumpToSection(FName("StartFire"));

	if(Character) 	
	{
		Character->PlayFireMontage(true);
	 	EquippedWeapon->Fire();

	}

	//if (!HasAuthority()) return;
	
	//Coloca el MuzzleFlash en el skeletalmesh del arma
	const USkeletalMeshSocket* MuzzleFlashSocket  =  EquippedWeapon->GetMesh()->GetSocketByName(FName("MuzzleFlash"));
    
	if (!MuzzleFlashSocket) return;
	
	//Line trace by channel
	const FTransform BarrelSocketTransform = MuzzleFlashSocket->GetSocketTransform(EquippedWeapon->GetMesh());
	
	FHitResult FireHit;

	const FVector Start{  BarrelSocketTransform.GetLocation() };
	const FQuat  Rotation{ BarrelSocketTransform.GetRotation() };
	const FVector RotationAxis{ Rotation.GetAxisX() };
	const FVector End{ Start + RotationAxis * 50'000.f };

	//FVector BeamEndPoint{ End };

	GetWorld()->LineTraceSingleByChannel(FireHit, Start, End, ECollisionChannel::ECC_Visibility);

}


void UWeaponComponent::Server_FireWeapon_Implementation()
{
	Multicast_FireWeapon();
}

void UWeaponComponent::Multicast_FireWeapon_Implementation()
{
	FireWeapon_Handle();
}




void UWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWeaponComponent, EquippedWeapon);
	//DOREPLIFETIME(UWeaponComponent, bAiming);

}