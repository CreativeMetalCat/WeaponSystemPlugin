// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "WeaponProjectileBase.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "WeaponInterface.h"
#include "WeaponBase.generated.h"


UCLASS()
class WEAPONSYSTEMPLUGIN_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool SpawnBulletNotServer(FVector location, FRotator rotation);
public:



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HandOffset)
		FVector LeftHandOffset = FVector(0, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponFiring)
		bool bUseMuzzleFlashOfTheMesh = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponFiring)
		FString WeaponEffectsSocketName = "b_gun_muzzleflash";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponFiring)
		bool bUpdateShootingPositionWhenAiming = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		FTimerHandle CooldownTimerHadle;

	/**CapsuleCollisionComponent is used for picking up weapons */
	UPROPERTY(Category = PickUp, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* CapsuleCollisionComponent;

	/*Mesh used during reload animations*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Clip)
		UStaticMesh* EmptyClipMesh;

	/*Mesh used during reload animations*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Clip)
		UStaticMesh* AmmoShellMesh;

	/*Mesh used during reload animations*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Clip)
		UStaticMesh* ClipMesh;


	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
		USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		TSubclassOf<AWeaponProjectileBase>PrimaryProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DisplayName)
		FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		TSubclassOf<AWeaponProjectileBase>SecondaryProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		USoundBase* PrimaryFireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		USoundBase* PrimaryOutOfAmmoSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		USoundBase* SecondaryOutOfAmmoSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		USoundBase* PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		USoundBase* SecondaryFireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		USoundBase* ReloadSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Scope)
		bool bHasScope = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, SaveGame)
		AActor* WeaponOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Category, SaveGame)
		int Category = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, SaveGame)
		int AmmoInTheClip = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		int DefaultAmmoInTheClip = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float WeaponCooldown = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		bool bSecondaryFireIsAim = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, SaveGame)
		bool bCanShoot = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float PrimaryDamage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, SaveGame)
		float CurrentFiringSpread = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float DefaultFiringSpread = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float SpreadIncrement = 0.45f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aiming, Replicated, SaveGame)
		bool bAiming = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aiming)
		float AimFOVOffset = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aiming)
		FVector AimLocationOffset = FVector(5.f, 0.f, -3.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aiming)
		FVector HandMeshLocationOffset = FVector(0, 0, -5.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aiming)
		bool bOffsetHandMesh = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aiming)
		FRotator DefaultShootingOffset = FRotator(0.f, -2.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aiming)
		FRotator AimOffset = FRotator(0.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aiming)
		FRotator DefaultCrouchedShootingOffset = FRotator(0.f, -2.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aiming)
		FRotator CrouchedAimOffset = FRotator(0.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aiming)
		FRotator AimRotation = FRotator(0.f, -91.f, 0.f);

	//What ammo type does this weapon need
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Ammo, Meta = (ClampMin = "0", UIMin = "0"))
		//What ammo type does this weapon need
		int AmmoTypeId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		bool bCanBePickedUp = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, SaveGame)
		float AIDamageModifier = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, SaveGame)
		bool bHolstered = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
		TSubclassOf<UCameraShake> CameraShakeClass;

	//What king of animation this weapon needs(such as Rifle,Pistol,Shotgun etc.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		//What king of animation this weapon needs(such as Rifle,Pistol,Shotgun etc.)
		FString AnimationTypeName = "Rifle";

	//if true weapon will shoot as long as shooting button is hold(although this depends on implementation of the shooter)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = ShootingStyle)
		//if true weapon will shoot as long as shooting button is hold(although this depends on implementation of the shooter)
		bool bAutoFire = false;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool PrimaryFire(FVector location, FRotator rotation);

	virtual bool PrimaryFire_Implementation(FVector location, FRotator rotation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool CanShoot();

	virtual bool CanShoot_Implementation();

	UFUNCTION(BlueprintCallable)
		void Holster();

	UFUNCTION(BlueprintCallable)
		void UnHolster();

	UFUNCTION(reliable, server, WithValidation)
		void ServerPrimaryFire(FVector location, FRotator rotation);

	UFUNCTION(reliable, server, WithValidation)
		void ServerSecondaryFire(FVector location, FRotator rotation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool  SecondaryFire(FVector location, FRotator rotation);

	bool  SecondaryFire_Implementation(FVector location, FRotator rotation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void PrimaryFireEffects(FVector location, FRotator rotation);

	void PrimaryFireEffects_Implementation(FVector location, FRotator rotation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void EndCooldown();

	void EndCooldown_Implementation();

	UFUNCTION(BlueprintCallable)
		void SetOnwerPlayer(AActor* player) { this->SetOwner(player); WeaponOwner = player; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void  SecondaryFireEffects(FVector location, FRotator rotation);

	void  SecondaryFireEffects_Implementation(FVector location, FRotator rotation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);


	void BeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
