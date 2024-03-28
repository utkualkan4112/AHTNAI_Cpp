// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_AHTNAI.generated.h"

class UAHTNAI_MethodDecomposition;
class UAHTNAI_AIPlayout;
class UAHTNAI_EvaluationFunction;
class UAHTNAI_DomainDefinition;
/**
 * 
 */
UCLASS()
class HITANDBOOM_API UBTTask_AHTNAI : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FString DomainFileName;

	UPROPERTY()
	UAHTNAI_DomainDefinition* DomainDefinition;

	UPROPERTY(EditAnywhere)
	UAHTNAI_EvaluationFunction* EFunction;

	UPROPERTY()
	UAHTNAI_AIPlayout* AIPlayout;

	int32 PLAYOUT_LOOKAHEAD = 100;

	int32 TIME_BUDGET = 100;

	int32 ITERATIONS_BUDGET = 100;

	TArray<UAHTNAI_MethodDecomposition*> ActionsBeingExecuted;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
 	
	
};
