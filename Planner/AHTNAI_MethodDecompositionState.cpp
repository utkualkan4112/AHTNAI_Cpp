// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020


#include "AHTNAI_MethodDecompositionState.h"

void UAHTNAI_MethodDecompositionState::Initialize(UAHTNAI_MethodDecomposition* Md)
{
	ExecutionState = Md->GetExecutionState();
	OperatorExecutingState = Md->GetOperatorExecutingState();
	if (Md->GetOperatorsBeingExecuted().IsValidIndex(0)) {
		OperatorsBeingExecuted.Append(Md->GetOperatorsBeingExecuted());
	}
}

void UAHTNAI_MethodDecompositionState::RestoreState(UAHTNAI_MethodDecomposition* Md)
{
	Md->SetExecutionState(ExecutionState);
	Md->SetOperatorExecutingState(OperatorExecutingState);
	if (!OperatorsBeingExecuted.IsValidIndex(0)) {
	} else {
		TArray<UAHTNAI_MethodDecomposition*> l = Md->GetOperatorsBeingExecuted();
		if (!l.IsValidIndex(0)) {
			Md->SetOperatorsBeingExecuted(l);
		}
		l.Empty();
		l.Append(OperatorsBeingExecuted);
	}
}  

