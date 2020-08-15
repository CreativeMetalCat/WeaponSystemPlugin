// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBase.h"

#ifdef DEBUG
#include "Engine.h"
#endif // DEBUG

#include "Kismet/GameplayStatics.h"
//#define ONLINE_BASE_CODE

// Sets default values
AWeaponBase::AWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	//SetRootComponent(WeaponMesh);

	RootComponent = WeaponMesh;


	CapsuleCollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapluseCollision"));
	CapsuleCollisionComponent->InitCapsuleSize(34.0f, 88.0f);

	CapsuleCollisionComponent->CanCharacterStepUpOn = ECB_No;
	CapsuleCollisionComponent->SetShouldUpdatePhysicsVolume(true);
	CapsuleCollisionComponent->SetCanEverAffectNavigation(false);
	CapsuleCollisionComponent->bDynamicObstacle = true;
	CapsuleCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CapsuleCollisionComponent->SetupAttachment(WeaponMesh);

	CapsuleCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::BeginOverlap);

	bReplicates = true;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

}

bool AWeaponBase::SpawnBulletNotServer(FVector location, FRotator rotation)
{
	if (WeaponOwner != nullptr)
	{

			//PrimaryFire(location, rotation);
			//if (WeaponOwner->Implements<UWeaponInterface>() || (Cast<IWeaponInterface>(WeaponOwner) != nullptr)) { IWeaponInterface::Execute_RequestSpawnProjectile(WeaponOwner, PrimaryProjectileClass, location, rotation); }

			auto bullet = GetWorld()->SpawnActor<AWeaponProjectileBase>(PrimaryProjectileClass, location, rotation);
			if (bullet == nullptr) { return false; }
			bullet->Damage = PrimaryDamage;
			bullet->SetOwner(WeaponOwner);
			bullet->StartLocation = location;
			bullet->GetCollisionComp()->IgnoreActorWhenMoving(WeaponOwner, true);
		
		return true;
	}
	return false;
}

bool AWeaponBase::PrimaryFire_Implementation(FVector location, FRotator rotation)
{
	/*if (Role < ROLE_Authority)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ServerPrimaryFire_Implementation"));
		ServerPrimaryFire(location, rotation);
	}*/

	if (CanShoot())
	{

		if (GetWorld() != nullptr)
		{
			if (PrimaryProjectileClass != nullptr)
			{

				FRandomStream randomStream = FRandomStream(FMath::Rand());
				
				for (int i = 0; i < BulletsPerShot; i++)
				{
					
					FVector ShootDir = randomStream.VRandCone(rotation.Vector(), FMath::DegreesToRadians((bAiming ? DefaultFiringSpread / 2 : DefaultFiringSpread)+CurrentFiringSpread) * 0.5f, FMath::DegreesToRadians((bAiming ? DefaultFiringSpread / 2 : DefaultFiringSpread) + CurrentFiringSpread) * 0.5f);
#ifdef ONLINE_BASE_CODE
					if (GetLocalRole() < ENetRole::ROLE_Authority)
					{
						if (SpawnBulletNotServer(location, ShootDir.Rotation()))
						{
							PrimaryFireEffects(location, rotation);
							AmmoInTheClip -= 1;
							if (AmmoInTheClip < 0) { AmmoInTheClip = 0; }
							bCanShoot = false;
							GetWorld()->GetTimerManager().SetTimer(CooldownTimerHadle, this, &AWeaponBase::EndCooldown, WeaponCooldown);
							return true;
						}
						else { return false; }
					}
					else
					{

						ServerPrimaryFire(location, ShootDir.Rotation());
						PrimaryFireEffects(location, rotation);
						AmmoInTheClip -= 1;
						if (AmmoInTheClip < 0) { AmmoInTheClip = 0; }
						bCanShoot = false;
						GetWorld()->GetTimerManager().SetTimer(CooldownTimerHadle, this, &AWeaponBase::EndCooldown, WeaponCooldown);
						return true;
					}
#else
					if (SpawnBulletNotServer(location, ShootDir.Rotation()))
					{
						
						CurrentFiringSpread += SpreadIncrement;
					}
					else { return false; }
				}
				PrimaryFireEffects(location, rotation);
				AmmoInTheClip -= 1;
				if (AmmoInTheClip < 0) { AmmoInTheClip = 0; }
				bCanShoot = false;
				GetWorld()->GetTimerManager().SetTimer(CooldownTimerHadle, this, &AWeaponBase::EndCooldown, WeaponCooldown);
				return true;
#endif
			}
			else
			{
				return false;
			}
		}
		return false;
	}
	else
	{
		if (AmmoInTheClip <= 0) { GetWorld()->GetTimerManager().SetTimer(CooldownTimerHadle, this, &AWeaponBase::EndCooldown, WeaponCooldown); bCanShoot = false;/*we still create timer so empty clip sounds would play normally*/ }
		return false;
	}

}


bool AWeaponBase::SecondaryFire_Implementation(FVector location, FRotator rotation)
{
	if (!bSecondaryFireIsAim)//aiming is actually done by PlayerCharcter itself but this allows for better control
	{
		if (CanShoot())
		{
			if (GetWorld() != nullptr)
			{
				if (SecondaryProjectileClass != nullptr)
				{
					GetWorld()->SpawnActor<AWeaponProjectileBase>(SecondaryProjectileClass, location, rotation);
					PrimaryFireEffects(location, rotation);
					bCanShoot = false;
					GetWorld()->GetTimerManager().SetTimer(CooldownTimerHadle, this, &AWeaponBase::EndCooldown, WeaponCooldown);
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return false;
}

void AWeaponBase::PrimaryFireEffects_Implementation(FVector location, FRotator rotation)
{
	if (PrimaryFireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PrimaryFireSound, location);
	}
}

void AWeaponBase::EndCooldown_Implementation()
{
	if (CooldownTimerHadle.IsValid()) { CooldownTimerHadle.Invalidate(); }
	CurrentFiringSpread = 0;
	bCanShoot = true;
}


bool AWeaponBase::CanShoot_Implementation()
{
	return AmmoInTheClip > 0 && bCanShoot && !bHolstered;
}

void AWeaponBase::Holster()
{
	bHolstered = true;
}

void AWeaponBase::UnHolster()
{
	bHolstered = false;
}

void AWeaponBase::SecondaryFireEffects_Implementation(FVector location, FRotator rotation)
{
	if (SecondaryFireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SecondaryFireSound, location);
	}
}

void AWeaponBase::BeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bCanBePickedUp)
	{
		if (OtherActor->Implements<UWeaponInterface>() || (Cast<IWeaponInterface>(OtherActor) != nullptr))
		{
			if (IWeaponInterface::Execute_PickupWeapon(OtherActor, GetClass(), AmmoInTheClip))
			{
				if (PickupSound != nullptr)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
				}
				Destroy();
			}
		}
	}
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::ServerPrimaryFire_Implementation(FVector location, FRotator rotation)
{
	if (WeaponOwner != nullptr)
	{
		//PrimaryFire(location, rotation);
		//if (WeaponOwner->Implements<UWeaponInterface>() || (Cast<IWeaponInterface>(WeaponOwner) != nullptr)) { IWeaponInterface::Execute_RequestSpawnProjectile(WeaponOwner, PrimaryProjectileClass, location, rotation); }
		auto bullet = GetWorld()->SpawnActor<AWeaponProjectileBase>(PrimaryProjectileClass, location, rotation);
		bullet->Damage = PrimaryDamage;
		bullet->SetOwner(WeaponOwner);
		bullet->StartLocation = location;
		bullet->GetCollisionComp()->IgnoreActorWhenMoving(WeaponOwner, true);
	}
}

bool AWeaponBase::ServerPrimaryFire_Validate(FVector location, FRotator rotation)
{
	return true;
}

bool AWeaponBase::ServerSecondaryFire_Validate(FVector location, FRotator rotation)
{
	return true;
}


void AWeaponBase::ServerSecondaryFire_Implementation(FVector location, FRotator rotation)
{
	SecondaryFire(location, rotation);
}

void AWeaponBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeaponBase, bSecondaryFireIsAim);
	DOREPLIFETIME(AWeaponBase, WeaponOwner);
	DOREPLIFETIME(AWeaponBase, AmmoTypeId);
	DOREPLIFETIME(AWeaponBase, WeaponMesh);
	DOREPLIFETIME(AWeaponBase, WeaponCooldown);

	DOREPLIFETIME(AWeaponBase, PrimaryProjectileClass);
	DOREPLIFETIME(AWeaponBase, SecondaryProjectileClass);

	DOREPLIFETIME(AWeaponBase, CooldownTimerHadle);
	DOREPLIFETIME(AWeaponBase, bCanShoot);

	DOREPLIFETIME(AWeaponBase, bAutoFire);

	DOREPLIFETIME(AWeaponBase, DefaultAmmoInTheClip);
	DOREPLIFETIME(AWeaponBase, AmmoInTheClip);

	DOREPLIFETIME(AWeaponBase, SecondaryOutOfAmmoSound);
	DOREPLIFETIME(AWeaponBase, PrimaryOutOfAmmoSound);

	DOREPLIFETIME(AWeaponBase, PrimaryDamage);

	DOREPLIFETIME(AWeaponBase, bAiming);
}
