// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AHTNAI_AdversarialChoicePoint.generated.h"

class UAHTNAI_MethodDecompositionState;
class UAHTNAI_DomainDefinition;
class UAHTNAI_Bindings;
class UAHTNAI_Clause;
class UAHTNAI_HTNMethod;
class ABaseGameState;
class UAHTNAI_MethodDecomposition;
/**
 * 
 */
UCLASS()
class HITANDBOOM_API UAHTNAI_AdversarialChoicePoint : public UObject
{
	GENERATED_BODY()
public:
    
	 static int32 DEBUG;
    
    // these are the root nodes:
    UPROPERTY()
    UAHTNAI_MethodDecomposition* MaxPlanRoot;
    UPROPERTY()
    UAHTNAI_MethodDecomposition* MinPlanRoot;
    
    // these are the choicePoint nodes (should be descendants of the root nodes, or null):
    UPROPERTY()
    UAHTNAI_MethodDecomposition* ChoicePointPlayerMax;
	
    UPROPERTY()
    UAHTNAI_MethodDecomposition* ChoicePointPlayerMin;

    UPROPERTY()
    ABaseGameState* Gs;

private:
    int32 LastTimeOperatorsIssued = -1;   
    int32 OperatorDepth = 0;      // this is to keep track of the depth of the search tree
                                // keeps track of the number of instants when operators where
                                // executed (if only one operator is executed at a time, this
                                // is equivalent to the number of operators executed so far)
    
    // Method: variables to continue finding expansions after the first:
    TArray<UAHTNAI_HTNMethod*> PossibleMethods;
    
    // Condition: variables to continue finding expansions after the first:
    UPROPERTY()
    UAHTNAI_Clause* UpdatedClause;
    
    bool UpdatedClauseHadAnyMatches = false;
    
    TArray<UAHTNAI_Bindings*> LastBindings;  // If the bindings found for a next match are the same as
                                        // the previous one, the new match is ignored (to reduce
                                        // useless branching)
    
    // Variables to restore the execution point of the plan after backtracking:
    TMap<UAHTNAI_MethodDecomposition*, UAHTNAI_MethodDecompositionState*> ExecutionState;

    bool NextConditionExpansion(UAHTNAI_MethodDecomposition* ChoicePoint, TArray<UAHTNAI_Bindings*> Bindings);
public:
    // evaluation function:
    int MinimaxType = -1;   // 0: max, 1: min, -1: not yet set.
    float BestEvaluation = 0;
    UPROPERTY()
    UAHTNAI_MethodDecomposition* BestMaxPlan;
    UPROPERTY()
    UAHTNAI_MethodDecomposition* BestMinPlan;
    float Alpha = 0;
    float Beta = 0;

	void Initialize(UAHTNAI_MethodDecomposition* CpMax, UAHTNAI_MethodDecomposition* CpMin, 
								  UAHTNAI_MethodDecomposition* RootMax, UAHTNAI_MethodDecomposition* RootMin, ABaseGameState* A_gs, int32 Nops, int32 Ltoi);   
    
    void Initialize(UAHTNAI_MethodDecomposition* CpMax, UAHTNAI_MethodDecomposition* CpMin, 
                                  UAHTNAI_MethodDecomposition* RootMax, UAHTNAI_MethodDecomposition* RootMin, ABaseGameState* A_gs, int32 Nops, int32 Ltoi,
                                  float A, float B, bool SaveExecutionState);    
    
    
    UAHTNAI_AdversarialChoicePoint* CloneForMcts();
    
    
    FORCEINLINE float GetAlpha() { return Alpha; }
    
    FORCEINLINE float GetBeta() { return Beta; }
            
    FORCEINLINE void SetAlpha(float A) { Alpha = A; }
    
    FORCEINLINE void SetBeta(float B) { Beta = B; }
    
    FORCEINLINE int32 GetOperatorDepth() { return OperatorDepth; }
    
    FORCEINLINE int32 GetLastTimeOperatorsIssued() { return LastTimeOperatorsIssued; }
    
    FORCEINLINE void SetLastTimeOperatorsIssued(int32 Ltoi) { LastTimeOperatorsIssued = Ltoi; }
    
    FORCEINLINE float GetBestEvaluation() { return BestEvaluation; }
    
    FORCEINLINE void SetBestEvaluation(float Be) { BestEvaluation = Be; }
    
    FORCEINLINE int32 GetMiniMaxType() { return MinimaxType; }
    
    FORCEINLINE ABaseGameState* GetGameState() { return Gs; }
    
    void CaptureExecutionState(UAHTNAI_MethodDecomposition* Md);
    
    
    void CaptureExecutionStateNonRecursive(UAHTNAI_MethodDecomposition* Md);   

    
    void RestoreExecutionState();
    
    
    void RestoreAfterPop();
    
    // "previous_cp" is usually == this, and it is the choice point where the state should be saved for restoring it later when backtracking
    bool NextExpansion(UAHTNAI_DomainDefinition* Dd, TArray<UAHTNAI_Bindings*> Bindings, int32 RenamingIndex, UAHTNAI_AdversarialChoicePoint* Previous_cp);
    
    bool NextMethodExpansion(UAHTNAI_MethodDecomposition* ChoicePoint, ABaseGameState* Gs, UAHTNAI_DomainDefinition* Dd, TArray<UAHTNAI_Bindings*> Bindings, int32 RenamingIndex);
    
    
    void SetMethod(UAHTNAI_MethodDecomposition* ChoicePoint, UAHTNAI_HTNMethod* Original, ABaseGameState* Gs, TArray<UAHTNAI_Bindings*> Bindings, int32 RenamingIndex);
    
    
    // returns "true" if the alpha-beta test determines it's time to prune the tree:
    bool ProcessEvaluation(float f, UAHTNAI_MethodDecomposition* MaxPlan, UAHTNAI_MethodDecomposition* MinPlan, bool Clone);
    
    
    
	FString ToString();
};
