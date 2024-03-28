// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "AHTNAI_Clause.h"
#include "AHTNAI_Term.h"
#include "UObject/Object.h"
#include "AHTNAI_HTNOperator.generated.h"

class UAHTNAI_LispElement;
class UAHTNAI_Clause;
class UAHTNAI_Term;
/**
 * 
 */
UCLASS()
class HITANDBOOM_API UAHTNAI_HTNOperator : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UAHTNAI_Term* Head;

	UPROPERTY()
	UAHTNAI_Clause* Precondition;
	// postconditions are not defined in the operators, but directly as part of the simulator

	void Initialize(UAHTNAI_Term* a_head, UAHTNAI_Clause* a_prec);
    
	FORCEINLINE UAHTNAI_Term* GetHead() { return Head; }
    
	FORCEINLINE UAHTNAI_Clause* GetPrecondition() { return Precondition; }
    
	static UAHTNAI_HTNOperator* FromLispElement(UAHTNAI_LispElement* E);
    
	FORCEINLINE FString ToString() { return "operator: " + Head->ToString() + ", precondition: " + Precondition->ToString(); }
};
