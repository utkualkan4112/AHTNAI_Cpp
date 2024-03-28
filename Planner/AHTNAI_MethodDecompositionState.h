// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "AdvancedAI/AHTNAI/Domain/AHTNAI_MethodDecomposition.h"
#include "UObject/Object.h"
#include "AHTNAI_MethodDecompositionState.generated.h"
/**
 * 
 */
UCLASS()
class HITANDBOOM_API UAHTNAI_MethodDecompositionState : public UObject
{
	GENERATED_BODY()
private:
	EExecutionType ExecutionState = EExecutionType::EXECUTION_SUCCESS;
	
	EExecutionType OperatorExecutingState = EExecutionType::EXECUTION_SUCCESS;

	UPROPERTY()
	TArray<UAHTNAI_MethodDecomposition*> OperatorsBeingExecuted;
public:
	void Initialize(UAHTNAI_MethodDecomposition* Md);

	void RestoreState(UAHTNAI_MethodDecomposition* Md);
};
