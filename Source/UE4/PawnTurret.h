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
#include "PawnTurret.generated.h"

 /*

	 Crazy Tank classes

 */

class APawnTank;
class APickUpBase;

////////////////////////////////////////////////////////////////////////////// 
//
// This class handles the Enemy Turret's behavior (attacking and destruction)
//
//////////////////////////////////////////////////////////////////////////////
UCLASS()
class CRAZYTANK_API APawnTurret : public APawnBase
{
	GENERATED_BODY()
	
private:

	/*
		VARIABLES
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float FireRange = 500.0f; // Turret's "view" range for checking for the player and start attacking

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float FireRate = 2.0f; // If the player is in range, the Turret will fire every FireRate seconds

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pick-Up Type", meta = (AllowPrivateAccess = "true"))
	TArray< TSubclassOf<APickUpBase> > PickUpClass; // The kind of Pick Up/s that the Turret will drop when destroyed

	// Timers allow us to trigger events based on elapsed time in the form of creating asynchronous
	// callbacks to specific function pointers.
	// This Timer is for firing every X amount of seconds based on this fire rate
	// FTimerHandles: unique handle that can be used to distinguish timers that have identical delegates
	// FTimerHandle allows us to bind and unbind our timer (control when to start or stop them)
	FTimerHandle FireRateTimerHandle;

	APawnTank* PlayerPawn = nullptr; // Reference to the Player's Tank
	
	/*
		METHODS
	*/

	void CheckFireCondition(); // Checking that desired conditions have been met to allow the firing functionality to be called on the parent class
	
	float ReturnDistanceToPlayer(); // Calculate the distance to the player's Tank to see if it's in firing range

public:

	/*
		METHODS
	*/

	// Sets default values for this pawn's properties
	APawnTurret();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void HandleDestruction() override; // Manages this pawn's behaviour when it's destroyed

protected:

	/*
		METHODS
	*/

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
