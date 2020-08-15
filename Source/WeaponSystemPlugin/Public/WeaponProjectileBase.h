// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "WeaponProjectileBase.generated.h"

UCLASS()
class WEAPONSYSTEMPLUGIN_API AWeaponProjectileBase : public AActor
{
	GENERATED_BODY()
	
protected:
	
public:
	virtual void BeginPlay() override;

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;




	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage = 10.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ShouldApplyImpulse = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ImpulseMultiplier = 1.f;

	/*Needed for calculations of effects*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		/*Needed for calculations of effects*/
		FVector StartLocation;

	AWeaponProjectileBase();

	/** called when projectile hits something */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }


};
