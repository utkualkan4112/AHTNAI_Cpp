// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "BaseClasses/BaseGameState.h"
#include "UObject/Object.h"
#include "AHTNAI_PredefinedPredicates.generated.h"

class ABaseGameState;
class UAHTNAI_Term;
class UAHTNAI_Bindings;
class UAHTNAI_Symboll;
/**
 * 
 */
UCLASS()
class HITANDBOOM_API UAHTNAI_PredefinedPredicates : public UObject
{
	GENERATED_BODY()
public:
	UAHTNAI_PredefinedPredicates();

	static int32 DEBUG;

	class PredicateTester
	{
	public:
		virtual TArray<UAHTNAI_Bindings*> FirstMatch(UAHTNAI_Term* Term, ABaseGameState* Gs) = 0;

		virtual TArray<TArray<UAHTNAI_Bindings*>> AllMatches(UAHTNAI_Term* Term, ABaseGameState* Gs) = 0; 
	};

private:
	static TMap<UAHTNAI_Symboll*, PredicateTester*> Predicates;

public:
	static TArray<UAHTNAI_Bindings*> FirstMatch(UAHTNAI_Term* Term, ABaseGameState* Gs);


	static TArray<TArray<UAHTNAI_Bindings*>> AllMatches(UAHTNAI_Term* Term, ABaseGameState* Gs);
};
