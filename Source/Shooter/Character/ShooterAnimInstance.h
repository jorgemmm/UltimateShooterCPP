// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ShooterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UShooterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// Sets default values for this classes properties
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProprerties(float DeltaTime);

	

	//float AO_Yaw;
protected:
	

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement, Meta = ( AllowPrivateAccess="true"))
	class AShooterCharacter* ShooterCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = movement, Meta = (AllowPrivateAccess = "true"))
	float Direction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = movement, Meta = (AllowPrivateAccess = "true"))
	float Yaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= movement, Meta = (AllowPrivateAccess="true"))
	bool bIsAccelerating;
	
	UAnimMontage* CharactarFireAnimMON;
	UAnimMontage* WeaponFireMON;

public:
	//FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	void playMontageFire();
	
};
