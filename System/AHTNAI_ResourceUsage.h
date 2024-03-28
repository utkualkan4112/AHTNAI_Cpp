// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AHTNAI_ResourceUsage.generated.h"

class ABaseRobot;
class ABaseGameState;
/**
 * 
 */
UCLASS()
class HITANDBOOM_API UAHTNAI_ResourceUsage : public UObject
{
	GENERATED_BODY()
private:

	TArray<FVector> PositionsUsed;

	int32 ResourceUsed;
 int32 CooldownCost;
public:


	bool ConsistentWith(UAHTNAI_ResourceUsage* AnotherUsage, ABaseGameState* Gs);

    /**
     * Returns the list with used resource positions
     */
 FORCEINLINE TArray<FVector> GetPositionsUsed() { return PositionsUsed;}

    /**
     * Returns the amount of resources used by the player
     */
    FORCEINLINE int32 GetResourcesUsed() const { return ResourceUsed;}

 FORCEINLINE int32 GetCooldownCost() const { return CooldownCost; }

 FORCEINLINE void SetPositionUsed(TArray<FVector> List) { PositionsUsed = List; }
 FORCEINLINE void SetResourcesUsed(int32 Resource) { ResourceUsed = Resource; }
 FORCEINLINE void SetCooldownCost(int32 Cost) { CooldownCost = Cost; }

    /**
     * Merges this and another instance of ResourceUsage into a new one
     */
    UAHTNAI_ResourceUsage* MergeIntoNew(UAHTNAI_ResourceUsage* Other);

    /**
     * Merges another instance of ResourceUsage into this one
     */
    void Merge(UAHTNAI_ResourceUsage* Other);

    UAHTNAI_ResourceUsage* Clone();

    FString ToString();
	
};
