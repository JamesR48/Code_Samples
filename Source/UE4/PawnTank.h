/*
 *****************************************
	Crazy Tank - Driving/shooting game prototype
	By James Romero. Made with Unreal Engine 4
	2021
 *****************************************
 */

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "PawnTank.generated.h"

/*

	Engine classes

*/

class USpringArmComponent;
class UCameraComponent;

/*

	Crazy Tank classes 

*/

class AGunBase;

// Delegate to notify suscribed classes when the current Tank's regular projectiles amount has changed
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProjectileCountChanged, int32, ProjectileCount);

// Delegate to notify suscribed classes when the current Tank's homing projectiles amount has changed
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHomingProjectileCountChanged, int32, HomingProjectileCount);

////////////////////////////////////////////////////////////////////////////// 
//
// This class handles the Tank's behavior (moving, attacking and destruction)
//
//////////////////////////////////////////////////////////////////////////////
UCLASS()
class CRAZYTANK_API APawnTank : public APawnBase
{
	GENERATED_BODY()
	
private:

	/*
		VARIABLES
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ParticleTrail = nullptr; // Dust trail made by the Tank when moving

	FVector MoveDirection = FVector::ZeroVector;

	FQuat RotationDirection = FQuat::Identity; // The Tank's body rotation direction given by the WASD keys input

	FQuat CounterRotation = FQuat::Identity; // The Tank's turret rotation direction given by the mouse input

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float TurnSpeed = 100.0f;

	APlayerController* PlayerControllerRef = nullptr;

	bool bIsPlayerAlive = true; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float GroundRayLength = 20.0f; // Raycast length for checking if the Tank is grounded (touching the floor)

	float TankGravity = 10.0f; // Tank's custom down force
	
	float DragOnGround = 1.5f; // Drag force experimented by the Tank when moving on ground
	
	bool bIsGrounded = false;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGunBase> GunClass; //Blueprint GunActor class to spawn

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> HomingProjectileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* HomingProjectileSpawnPoint = nullptr; //visual representation of where homing projectiles will be spawned from when fired

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
	int ProjectileAmmoMax = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
	int HomingProjectileAmmoMax = 4;

	int ProjectileAmmoCurrent;

	int HomingProjectileAmmoCurrent;

	bool bIsFiringRifle = false; // To know if the Tank is firing its gun

	UPROPERTY();
	AGunBase* Gun = nullptr; // Here we will store the actual Gun instance

	TArray<AActor*> HomingTarget; // Selected targets array that will be attacked with a homing projectile

	/*
		METHODS
	*/
	
	void CalculateMoveInput(float value); // Calculate the Tank's capsule component movement from keyboard input and move speed

	// Calculate the Tank's body rotation from keyboard input and turn speed
	void CalculateRotateInput(float value); // Also calculates the counter rotation for the Tank's turret from the results of the body rotation
	
	void RotateView(float value); // Calculates and applies the Tank's turret rotation from mouse input and turn speed
	
	// Raycast down from the Tank's body to know if it's grounded and align its body to the surface if that's the case  
	void Move(); // Also applies a force to move the Tank if it's grounded or a down force (gravity) in case it's not 
	
	// Applies the rotation and counter rotation of the Tank's base and turret, only if the Tank is moving first, if not it'll not rotate
	void Rotate(); // Also manages a dust particle system when the Tank is moving

	void FireRifle(); // Activates the firing of the Tank's gun if there's a Gun Class assigned
	
	// Sends a raycast to find enemies to target for the Tank's homing projectile
	void TargetHomingProjectile(); // Also draws an outline to every found target 
	
	void FireHomingProjectile(); // Spawns and shoots a homing projectile for every found target
	
	void DrawTargetOutline(AActor* Target, bool bShouldDraw); // Draws an outline to every found target mesh
	
	virtual void Fire() override; // Activates the firing of the Tank's regular projectiles using the "PawnBase" parent class virtual method
	
	// Calculates the current ammo of a projectile (homing or regular) depending on whether the player is shooting or recolecting ammo pick ups
	int ProcessNewAmmo(int CurrentAmmo, int AddedAmount, int MaxAmmo);

public:

	/*
		METHODS
	*/

	APawnTank(); // Sets default values for this pawn's properties

	virtual void Tick(float DeltaTime) override; // Called every frame

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void HandleDestruction() override; // Manages this pawn's behaviour when it's destroyed

	bool GetIsPlayerAlive(); // Getter for the bIsPlayerAlive variable

	// Adds ammo to a specified type of projectile (homing or regular)
	void AddAmmo(int AmmoType, int Amount); // This method is public because it's used in the Pick Up classes

	// Delegate to notify suscribed classes when the current Tank's regular projectiles amount has changed
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Delegates")
		FOnProjectileCountChanged OnProjectileCountChanged;

	// Delegate to notify suscribed classes when the current Tank's homing projectiles amount has changed
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Delegates")
		FOnHomingProjectileCountChanged OnHomingProjectileCountChanged;

protected:
	
	/*
		METHODS
	*/

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
