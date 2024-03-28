// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_BaseClass.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class HITANDBOOM_API UBTTask_BaseClass : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	AActor* GetEnemy() const;

	UFUNCTION(BlueprintImplementableEvent)
	void SetEnemy(AActor* Enemy);

	UFUNCTION(BlueprintImplementableEvent)
	float GetFleeDistance() const;

	UFUNCTION(BlueprintImplementableEvent)
	void SetFleeDistance(float NewDistance);

	UFUNCTION(BlueprintImplementableEvent)
	void Execute(AAIController* Controller, APawn* ControlledPawn);
};
