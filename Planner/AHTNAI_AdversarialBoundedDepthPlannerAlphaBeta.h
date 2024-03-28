// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AHTNAI_AdversarialBoundedDepthPlannerAlphaBeta.generated.h"

class UAHTNAI_AIPlayout;
class UAHTNAI_EvaluationFunction;
class UAHTNAI_Term;
class UAHTNAI_Bindings;
class UAHTNAI_AdversarialChoicePoint;
class UAHTNAI_DomainDefinition;
class ABaseGameState;
class UAHTNAI_MethodDecomposition;
/**
 * 
 */
UCLASS()
class HITANDBOOM_API UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta : public UObject
{
	GENERATED_BODY()
public:
    static int32 DEBUG;

    static bool ALPHA_BETA_CUT;

    // - if the following variable is set to "false", the search will stop
    // as soon as the is a point where we have the number of desired actions
    // - if the following variable is set to "true", even after the number of
    // desired actions is reached, simulation will continue until the next choice
    // point, in order to get as much information as possible about the effects of the
    // selected actions.
    static bool SIMULATE_UNTIL_NEXT_CHOICEPOINT;
private:
    UPROPERTY()
    UAHTNAI_MethodDecomposition* MaxPlanRoot;

    UPROPERTY()
    UAHTNAI_MethodDecomposition* MinPlanRoot;
    
    int32 MaxPlayer;

    UPROPERTY()
    ABaseGameState* Gs;

    UPROPERTY()
    UAHTNAI_DomainDefinition* Dd;

    UPROPERTY()
    UAHTNAI_EvaluationFunction* f; 

    UPROPERTY()
    UAHTNAI_AIPlayout* PlayoutAI; 
    
    int32 PLAYOUT_LOOKAHEAD = 100;
    
    int32 maxDepth = 3;
    
    int32 operatorExecutionTimeout = 1000;
    
    static int32 MAX_TREE_DEPTH;
    
    static int32 nPlayouts;    // number of leaves in the current search (includes all the trees in the current ID process)
    
    TArray<UAHTNAI_AdversarialChoicePoint*> Stack;
    
    TArray<int32> Trail;    // how many bindings to remove, when popping this element of the stack

    TArray<UAHTNAI_Bindings*> Bindings;

    int32 RenamingIndex = 1;  // for renaming variables

    bool LastRunSolvedTheProblem = false;    // if this is true, no need to continue iterative deepening

public:
    // statistics
    static int32 n_iterative_deepening_runs;
    static double max_iterative_deepening_depth;
    static double average_iterative_deepening_depth;

    static int32 n_trees;
    static double max_tree_leaves;
    static double last_tree_leaves;
    static double average_tree_leaves;
    
    static double max_tree_nodes;
    static double last_tree_nodes;
    static double average_tree_nodes;

    static double max_tree_depth;
    static double last_tree_depth;
    static double average_tree_depth;
    
    static double max_time_depth;
    static double last_time_depth;
    static double average_time_depth;

    
    static void ClearStatistics();

    // depth is in actions:
    void Initialize(UAHTNAI_Term* GoalPlayerMax, UAHTNAI_Term* GoalPlayerMin, int32 A_maxPlayer, int32 Depth, int32 PlayoutLookahead, ABaseGameState* A_gs, UAHTNAI_DomainDefinition* A_dd, UAHTNAI_EvaluationFunction* A_f, UAHTNAI_AIPlayout* A_playoutAI);
    

    FORCEINLINE TOptional<TPair<UAHTNAI_MethodDecomposition*,UAHTNAI_MethodDecomposition*>> GetBestPlan() { return GetBestPlan(-1, -1, false); }

    // if "forceAnswer == true", then even if the search does not finish due to the time limit,
    // the best action found so far is returned
    TOptional<TPair<UAHTNAI_MethodDecomposition*,UAHTNAI_MethodDecomposition*>> GetBestPlan(int64 TimeLimit, int32 MaxPlayouts, bool ForceAnswer);

    /*
    The search will end when:
        - the tree is searched to the maximum depth
        - or when System.currentTimeMillis() is larger or equal than timeLimit
        - or when int is larger or equal to maxPlayouts
    */
    static TPair<UAHTNAI_MethodDecomposition*,UAHTNAI_MethodDecomposition*> GetBestPlanIterativeDeepening(UAHTNAI_Term* GoalPlayerMax, UAHTNAI_Term* GoalPlayerMin, int32 A_maxPlayer, int32 Timeout, int32 MaxPlayouts, int32 A_playoutLookahead, ABaseGameState* A_gs, UAHTNAI_DomainDefinition* A_dd, UAHTNAI_EvaluationFunction* A_f, UAHTNAI_AIPlayout* A_playoutAI);
        


    /*
        - Return value "null" means execution failure
        - Return value <null,GameState> means execution success
        - <md,GameState> represents a choice point
    */
    UAHTNAI_AdversarialChoicePoint* SimulateUntilNextChoicePoint(TArray<UAHTNAI_Bindings*> Bindings, UAHTNAI_AdversarialChoicePoint* Previous_cp);


    void StackPop();
    
    
    float Playout(int32 player, ABaseGameState* Gs); 
};
