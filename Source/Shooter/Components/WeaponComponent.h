// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//friend class AShooterCharacter;
	
	/** Called to request server to equpi weapon*/
	void EquipWeapon(class AWeapon* WeaponToEquip);
		
	/** Request to server  Equip Weapom */
	UFUNCTION(Server, Reliable)
	void Server_EquipWeapon(class AWeapon* WeaponToEquip);


    void FireWeapon();

	UFUNCTION()
		void FireWeapon_Handle();

	UFUNCTION(Server, Reliable)
		void Server_FireWeapon();    //(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_FireWeapon();

	/*UFUNCTION()
	void EquipWeaponHandle(class AWeapon* WeaponToEquip)*/

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
private:
	
	class AShooterCharacter*     Character;
	class UShooterAnimInstance*  AnimInstance;
	
	UPROPERTY(Replicated)
	AWeapon* EquippedWeapon;

	

public:	
	
	FORCEINLINE AWeapon* GetEquippedWeapon() const { return EquippedWeapon; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
