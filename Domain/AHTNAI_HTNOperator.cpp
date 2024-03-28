// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020


#include "AHTNAI_HTNOperator.h"

#include "AdvancedAI/AHTNAI/Domain/LispParser/AHTNAI_LispElement.h"

void UAHTNAI_HTNOperator::Initialize(UAHTNAI_Term* a_head, UAHTNAI_Clause* a_prec)
{
	Head = a_head;
	Precondition = a_prec;
}

UAHTNAI_HTNOperator* UAHTNAI_HTNOperator::FromLispElement(UAHTNAI_LispElement* E)
{
	UAHTNAI_LispElement* Head_e = E->Children[1];
	UAHTNAI_LispElement* Precondition_e = E->Children[2];
        
	UAHTNAI_Term* Head = UAHTNAI_Term::FromLispElement(Head_e);
	UAHTNAI_Clause* Prec = UAHTNAI_Clause::FromLispElement(Precondition_e);

	UAHTNAI_HTNOperator* ReturnOperator = NewObject<UAHTNAI_HTNOperator>();
	ReturnOperator->Initialize(Head, Prec);
	return ReturnOperator;
}
