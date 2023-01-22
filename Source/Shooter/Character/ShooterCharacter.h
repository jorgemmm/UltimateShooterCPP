// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class SHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	//virtual void PostInitializeComponents() override;

	void PlayFireMontage(bool bAiming);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//Inputs	
	void MoveForward(float Value);	
	void MoveRight(float Value);
	/**
	* Called to turn at a given rate
	* Input value rate rate [ 0, 1]
	* Rate-> 1.0 means 100% of desired turn rate -> BaseTurnRate or BaseLookUpRate 
	*/
	void TurnAtRate(float Rate );
	void LookUpRate(float Rate);

	/** Called to Equip Wepaon from floor */
	void EquipButtonPressed();

	void FireWeaponPressed();


	//UFUNCTION()
	//	void FireWeaponHandle();

	//UFUNCTION(Server, Reliable)
	//	void Server_FireWeapon();    //(const FVector_NetQuantize& TraceHitTarget);

	//UFUNCTION(NetMulticast, Reliable)
	//	void Multicast_FireWeapon();  //(const FVector_NetQuantize& TraceHitTarget);

private:
	/**Camera Boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Camera, meta=( AllowPrivateAccess ="true"))
		class USpringArmComponent* CameraBoom;

	/**Camera tha follows character*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
	
	/** Base  turn rate, in deg/sec, Other scaling may affect final turn rate*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float BaseTurnRate;

	/** Base  Look Up/Down, in deg/sec, Other scaling may affect final turn rate*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float BaseLookUpRate;

	/** Base  Look Up/Down, in deg/sec, Other scaling may affect final turn rate*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float PawYawRate;

	
	/** flasd spwaned particle fx for fire shot at Barrel socket*/	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* FireMontage;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
		class AWeapon* OverlappingWeapon;

	UFUNCTION()
		void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class UWeaponComponent* CombatWeaponComp;

public:

	void SetOverlappingWeapon(AWeapon* Weapon);
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE AWeapon* GetOverlappingWeapon() const { return  OverlappingWeapon;}
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE UAnimMontage* GetFireMontage() const { return FireMontage; }

	/** Returns CameraBooms subobject*/
	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	

};
