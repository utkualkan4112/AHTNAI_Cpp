// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AHTNAI_Clause.generated.h"

class UAHTNAI_LispElement;
class UAHTNAI_Symboll;
class UAHTNAI_Bindings;
class UAHTNAI_Term;
class ABaseGameState;
/**
 * 
 */
UENUM(BlueprintType)
enum class EClauseType : uint8
{
	CLAUSE_TERM,
	CLAUSE_AND,
	CLAUSE_OR,
	CLAUSE_NOT,
	CLAUSE_TRUE,
	CLAUSE_FALSE
};

UCLASS()
class HITANDBOOM_API UAHTNAI_Clause : public UObject
{
	GENERATED_BODY()
public:
	int32 DEBUG = 1;
	
	EClauseType Type = EClauseType::CLAUSE_AND;

	UPROPERTY()
	UAHTNAI_Term* Term;

	TArray<UAHTNAI_Clause*> Clauses;

	TArray<UAHTNAI_Clause*> Matches_Resolved;

	// variables to continue finding matches after the first:
	TArray<TArray<UAHTNAI_Bindings*>> Matches_Left;

	int32 Matches_Current;

	int32 Matches_Previous;

	TArray<UAHTNAI_Bindings*> Matches_L;

	TArray<int32> Matches_Trail;

	static UAHTNAI_Clause* FromLispElement(UAHTNAI_LispElement* E);
    
    FString ToString();

    // applies all the bindings and evaluates in case it is a function:
    UAHTNAI_Clause* Resolve(TArray<UAHTNAI_Bindings*> l, ABaseGameState* gs);  

    
    UAHTNAI_Clause* Clone();    
    
    
    void RenameVariables(int32 RenamingIndex);
    
    // applies all the bindings
    void ApplyBindings(TArray<UAHTNAI_Bindings*> l);     
    
        
    // returns the first match, and sets up the internal state to return the subsequent matches later on:
    TOptional<TArray<UAHTNAI_Bindings*>> FirstMatch(ABaseGameState* Gs);
    
    
    TOptional<TArray<UAHTNAI_Bindings*>> NextMatch(ABaseGameState* Gs);
    
    
    void CountVariableAppearances(TMap<UAHTNAI_Symboll*, int32>& Appearances);
    
    void ReplaceSingletonsByWildcards(TArray<UAHTNAI_Symboll*>& Singletons);  

	
	
};
