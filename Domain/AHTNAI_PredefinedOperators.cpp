// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020


#include "AHTNAI_PredefinedOperators.h"

#include "AHTNAI_IntegerConstant.h"
#include "AHTNAI_MethodDecomposition.h"
#include "AHTNAI_Symboll.h"

int32 UAHTNAI_PredefinedOperators::DEBUG = 0;

TMap<UAHTNAI_Symboll*, UAHTNAI_PredefinedOperators::OperatorExecutor*> UAHTNAI_PredefinedOperators::Operators;

class MoveOperator : public UAHTNAI_PredefinedOperators::OperatorExecutor
{
public:
	UAHTNAI_Parameter* Execute(UAHTNAI_Term* T, UAHTNAI_MethodDecomposition* State, ABaseGameState* Gs, UAHTNAI_PlayerAction* Pa) override
	{
		//int32 UID1 = Cast<UAHTNAI_IntegerConstant>(T->Parameters[0])->value;
		
		return nullptr;
	}
};

UAHTNAI_Parameter* UAHTNAI_PredefinedOperators::Execute(UAHTNAI_MethodDecomposition* State, ABaseGameState* Gs, UAHTNAI_PlayerAction* Pa)
{
	UAHTNAI_Term* T = State->GetUpdatedTerm();
	if (T==nullptr) T = State->GetTerm();
	OperatorExecutor* Oe = *Operators.Find(T->Functor);
        
	if (Oe==nullptr) UE_LOG(LogTemp, Warning, TEXT("PredefinedFunctions.evaluate: undefined operator %s"), *T->ToString());
	return Oe->Execute(T, State, Gs, Pa);
}



UAHTNAI_PredefinedOperators::UAHTNAI_PredefinedOperators()
{
	UAHTNAI_Symboll* MoveSymboll = NewObject<UAHTNAI_Symboll>();
	MoveSymboll->InitializeWithString("!move");
	
	Operators.Add(MoveSymboll, new MoveOperator());
}
