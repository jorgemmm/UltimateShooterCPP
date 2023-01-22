// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAnimInstance.h"

#include "ShooterCharacter.h"
#include "Shooter/Weapon/Weapon.h"
#include "Animation/AnimMontage.h"

//Components
#include "GameFramework/CharacterMovementComponent.h"

//Utils
#include "Kismet/KismetMathLibrary.h"

#include "Net/UnrealNetwork.h"


void UShooterAnimInstance::NativeInitializeAnimation()
{
	
	ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
	
	
	

	
	
}

void UShooterAnimInstance::UpdateAnimationProprerties(float DeltaTime)
{
	if (!ShooterCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Shooter Character is nullptr in UShooterAnimInstance"))
			return;
	}

	FVector Velocity{ ShooterCharacter->GetVelocity() };
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = ShooterCharacter->GetCharacterMovement()->IsFalling();


	const float HeroAcelerating = ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size();

	(HeroAcelerating > 0.f) ? bIsAccelerating = true : bIsAccelerating = false;

	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(ShooterCharacter->GetVelocity());
	Yaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, ShooterCharacter->GetBaseAimRotation()).Yaw;
    
	Direction = CalculateDirection(ShooterCharacter->GetVelocity(), ShooterCharacter->GetActorRotation());
}

void UShooterAnimInstance::playMontageFire()
{		
	   if(ShooterCharacter->GetFireMontage())
	     
		   Montage_Play(ShooterCharacter->GetFireMontage());
	   
	   if (!ShooterCharacter->GetOverlappingWeapon()) return;

	   if (ShooterCharacter->GetOverlappingWeapon()->GetWeaponFireMontage())

		   ShooterCharacter->GetOverlappingWeapon()->Fire();
	 

}


