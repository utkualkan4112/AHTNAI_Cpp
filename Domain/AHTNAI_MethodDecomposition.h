// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "AHTNAI_Term.h"
#include "UObject/Object.h"
#include "AHTNAI_MethodDecomposition.generated.h"

class UAHTNAI_AdversarialChoicePoint;
class UAHTNAI_Clause;
class UAHTNAI_Term;
class UAHTNAI_HTNMethod;
/**
 * 
 */
UENUM(BlueprintType)
enum class EMethodType : uint8
{
	METHOD_CONDITION,
	METHOD_OPERATOR,
	METHOD_METHOD,
	METHOD_SEQUENCE,
	METHOD_PARALLEL,
	METHOD_NON_BRANCHING_CONDITION
};

UENUM(BlueprintType)
enum class EExecutionType : uint8
{
	EXECUTION_SUCCESS,
	EXECUTION_FAILURE,
	EXECUTION_ACTION_ISSUE,
	EXECUTION_WAITING_FOR_ACTION,
	EXECUTION_CHOICE_POINT
};


UCLASS()
class HITANDBOOM_API UAHTNAI_MethodDecomposition : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UAHTNAI_HTNMethod* Method;
    
	/*
	executionState:
	- METHOD_CONDITION: 0: not tested (return as choice point), 1: already tested with success, 2: already tested with failure
	- METHOD_OPERATOR: 0: not sent, 1: waiting for completion, 2: success
	- METHOD_METHOD: -
	- METHOD_SEQUENCE: stores the index of the current subelement being executed
	- METHOD_PARALLEL: -
	*/
	EExecutionType ExecutionState = EExecutionType::EXECUTION_SUCCESS;
	EExecutionType OperatorExecutingState = EExecutionType::EXECUTION_SUCCESS;
	TArray<UAHTNAI_MethodDecomposition*> OperatorsBeingExecuted;

	UPROPERTY()
	UAHTNAI_Term* UpdatedTerm;
	
	int32 UpdatedTermCycle = -1;  // at which game cycle did we update the term last time

protected:
	EMethodType Type = EMethodType::METHOD_CONDITION;
	UPROPERTY()
	UAHTNAI_Clause* Clause;
	UPROPERTY()
	UAHTNAI_Term* Term;
	
	TArray<UAHTNAI_MethodDecomposition*> Subelements;

public:
	void Initialize(UAHTNAI_HTNMethod* M, UAHTNAI_Term* T);

	FORCEINLINE EMethodType GetType() { return Type; }
    
	FORCEINLINE void SetType(EMethodType A_type) { Type = A_type; }
    
	FORCEINLINE UAHTNAI_Clause* GetClause() { return Clause; }
    
	FORCEINLINE UAHTNAI_Term* GetTerm() { return Term; }
    
	FORCEINLINE UAHTNAI_Term* GetUpdatedTerm() { return UpdatedTerm; }
    
	FORCEINLINE void SetUpdatedTerm(UAHTNAI_Term* Ut) { UpdatedTerm = Ut; }
    
	FORCEINLINE int32 GetUpdatedTermCycle() { return UpdatedTermCycle; }
    
	FORCEINLINE void SetUpdatedTermCycle(int32 Utc) { UpdatedTermCycle = Utc; }
        
	FORCEINLINE UAHTNAI_HTNMethod* GetMethod() { return Method; }
    
	FORCEINLINE TArray<UAHTNAI_MethodDecomposition*> GetSubparts() { return Subelements; }
    
	FORCEINLINE void SetSubparts(TArray<UAHTNAI_MethodDecomposition*> Se) { Subelements = Se; }
    
	FORCEINLINE EExecutionType GetExecutionState() { return ExecutionState; }
        
	FORCEINLINE EExecutionType GetOperatorExecutingState() { return OperatorExecutingState; }
    
	FORCEINLINE TArray<UAHTNAI_MethodDecomposition*> GetOperatorsBeingExecuted() { return OperatorsBeingExecuted; }
    
	FORCEINLINE void SetOperatorsBeingExecuted(TArray<UAHTNAI_MethodDecomposition*> L) { OperatorsBeingExecuted = L; }

	FORCEINLINE void SetOperatorExecutingState(EExecutionType S) { OperatorExecutingState = S; }
    
	FORCEINLINE void SetMethod(UAHTNAI_HTNMethod* M) { Method = M; }
    
    FORCEINLINE void SetExecutionState(EExecutionType Es) { ExecutionState = Es; }
    
    static UAHTNAI_MethodDecomposition* FromLispElement(UAHTNAI_LispElement* E); 
    
    
    FString ToString();
    
    
    void PrintDetailed(int32 Tabs);
        
    
    TArray<UAHTNAI_MethodDecomposition*> GetLeaves();
    
    
    // note: operatorsBeingExecuted is not cloned by this method
    UAHTNAI_MethodDecomposition* Clone();
    
    
    UAHTNAI_MethodDecomposition* CloneTrackingDescendants(TArray<UAHTNAI_MethodDecomposition*> DescendantsToTrack, TArray<UAHTNAI_MethodDecomposition*> NewDescendants);
    
    
    
    void RenameVariables(int32 RenamingIndex);


	void ApplyBindings(TArray<UAHTNAI_Bindings*> L);


	void ExecutionReset();
    
    EExecutionType ExecutionCycle(ABaseGameState* Gs, TArray<UAHTNAI_MethodDecomposition*> Actions, TArray<UAHTNAI_MethodDecomposition*> ChoicePoints);
    
    /*
    This method is the same as the one above, but every time a MethodDecompositinn changes, 
    it stores its previous execution state in 'previous_cp'
    */
    EExecutionType ExecutionCycle(ABaseGameState* Gs, TArray<UAHTNAI_MethodDecomposition*> Actions, TArray<UAHTNAI_MethodDecomposition*> ChoicePoints, UAHTNAI_AdversarialChoicePoint* Previous_cp);
    

    TArray<TPair<int32,TArray<UAHTNAI_Term*>>> ConvertToOperatorList();
    
    void ConvertToOperatorList(TArray<TPair<int32,TArray<UAHTNAI_Term*>>>& L);
    
    
    void CountVariableAppearances(TMap<UAHTNAI_Symboll*,int32>& Appearances);
    
    
    void ReplaceSingletonsByWildcards(TArray<UAHTNAI_Symboll*>& Singletons);   
};
