// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AHTNAI_PlayerAction.generated.h"

class UAHTNAI_Robot;
class UAHTNAI_ResourceUsage;
class UAHTNAI_RobotAction;
class UAHTNAI_LightWeightGameState;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FRobotActionPair
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAHTNAI_Robot* Robot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAHTNAI_RobotAction* Action;
};

UCLASS()
class HITANDBOOM_API UAHTNAI_PlayerAction : public UObject
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<FRobotActionPair> Actions;

	UPROPERTY()
	UAHTNAI_ResourceUsage* ResourceUsage;

	UAHTNAI_PlayerAction();

	bool Equals(UAHTNAI_PlayerAction* Other);

	bool IsEmpty() const;

	bool HasNonNoneActions() const;

	int32 HasNumNoneActions() const;
	
	void SetResourceUsage(UAHTNAI_ResourceUsage* NewResourceUsage);

	void AddUnitAction(UAHTNAI_Robot* Unit, UAHTNAI_RobotAction* Action);

	void RemoveUnitAction(UAHTNAI_Robot* Unit, UAHTNAI_RobotAction* Action);
	
	UAHTNAI_PlayerAction* Merge(UAHTNAI_PlayerAction* Other);
	
	TArray<FRobotActionPair> GetActions() const;

	UAHTNAI_RobotAction* GetAction(UAHTNAI_Robot* Unit) const;

	TArray<UAHTNAI_PlayerAction*> CartesianProduct(TArray<UAHTNAI_PlayerAction*> UnitActions, ABaseRobot* Unit, UAHTNAI_LightWeightGameState* State);

	bool ConsistentWith(UAHTNAI_ResourceUsage* Usage, UAHTNAI_LightWeightGameState* State) const;

	void FillWithNones(UAHTNAI_LightWeightGameState* State, int32 PlayerID, int32 Duration);

	bool IntegrityCheck() const;

	virtual UAHTNAI_PlayerAction* Clone();

	void Clear();

	virtual FString ToString() const;
};
