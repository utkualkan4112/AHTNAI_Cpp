// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020


#include "AHTNAI_PredefinedPredicates.h"

#include "AHTNAI_Term.h"
#include "AHTNAI_Symboll.h"

int32 UAHTNAI_PredefinedPredicates::DEBUG = 0;

TMap<UAHTNAI_Symboll*, UAHTNAI_PredefinedPredicates::PredicateTester*> UAHTNAI_PredefinedPredicates::Predicates;

class MovePredicate : public UAHTNAI_PredefinedPredicates::PredicateTester
{
public:
	TArray<UAHTNAI_Bindings*> FirstMatch(UAHTNAI_Term* Term, ABaseGameState* Gs) override
	{
		
	}

	TArray<TArray<UAHTNAI_Bindings*>> AllMatches(UAHTNAI_Term* Term, ABaseGameState* Gs) override
	{
		
	}
	
};

UAHTNAI_PredefinedPredicates::UAHTNAI_PredefinedPredicates()
{
	
}

TArray<UAHTNAI_Bindings*> UAHTNAI_PredefinedPredicates::FirstMatch(UAHTNAI_Term* Term, ABaseGameState* Gs)
{
	PredicateTester* fe = Predicates.FindRef(Term->Functor);
    
	if (fe == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PredefinedPredicates.firstMatch: undefined predicate %s"), *Term->ToString());
		TArray<UAHTNAI_Bindings*> NewList;
		return NewList;
	}
	return fe->FirstMatch(Term, Gs);
}

TArray<TArray<UAHTNAI_Bindings*>> UAHTNAI_PredefinedPredicates::AllMatches(UAHTNAI_Term* Term, ABaseGameState* Gs)
{
	PredicateTester* fe = Predicates.FindRef(Term->Functor);
    
	if (fe == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PredefinedPredicates.allMatches: undefined predicate %s"), *Term->ToString());
		TArray<TArray<UAHTNAI_Bindings*>> NewList;
		return NewList;
	}
	return fe->AllMatches(Term, Gs);
}
