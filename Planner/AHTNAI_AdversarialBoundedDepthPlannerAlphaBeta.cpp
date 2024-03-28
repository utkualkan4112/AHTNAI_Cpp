// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020


#include "AHTNAI_AdversarialBoundedDepthPlannerAlphaBeta.h"
#include "AdvancedAI/AHTNAI/Domain/AHTNAI_MethodDecomposition.h"
#include "AHTNAI_AdversarialChoicePoint.h"
#include "AdvancedAI/AHTNAI/System/AHTNAI_AIPlayout.h"
#include "AdvancedAI/AHTNAI/System/AHTNAI_EvaluationFunction.h"
#include "AdvancedAI/AHTNAI/Domain/AHTNAI_PredefinedOperators.h"

int32 UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::DEBUG = 0;

bool UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::ALPHA_BETA_CUT = true;

bool UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::SIMULATE_UNTIL_NEXT_CHOICEPOINT = true;

int32 UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::MAX_TREE_DEPTH = 10;

int32 UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::nPlayouts = 5;

int32 UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::n_iterative_deepening_runs = 0;
double UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::max_iterative_deepening_depth = 0;
double UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::average_iterative_deepening_depth = 0;

int32 UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::n_trees = 0;
double UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::max_tree_leaves = 0;
double UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::last_tree_leaves = 0;
double UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::average_tree_leaves = 0;
    
double UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::max_tree_nodes = 0;
double UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::last_tree_nodes = 0;
double UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::average_tree_nodes = 0;

double UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::max_tree_depth = 0;
double UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::last_tree_depth = 0;
double UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::average_tree_depth = 0;
    
double UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::max_time_depth = 0;
double UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::last_time_depth = 0;
double UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::average_time_depth = 0;



void UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::ClearStatistics()
{
	n_iterative_deepening_runs = 0;
	max_iterative_deepening_depth = 0;
	average_iterative_deepening_depth = 0;
	n_trees = 0;
	max_tree_leaves = 0;
	last_tree_leaves = 0;
	average_tree_leaves = 0;
	max_tree_nodes = 0;
	last_tree_nodes = 0;
	average_tree_nodes = 0;
	max_tree_depth = 0;
	last_tree_depth = 0;
	average_tree_depth = 0;
	max_time_depth = 0;
	last_time_depth = 0;
	average_time_depth = 0;
}

void UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::Initialize(UAHTNAI_Term* GoalPlayerMax, UAHTNAI_Term* GoalPlayerMin,
    int32 A_maxPlayer, int32 Depth, int32 PlayoutLookahead, ABaseGameState* A_gs, UAHTNAI_DomainDefinition* A_dd,
    UAHTNAI_EvaluationFunction* A_f, UAHTNAI_AIPlayout* A_playoutAI)
{
	UAHTNAI_MethodDecomposition* NewMin = NewObject<UAHTNAI_MethodDecomposition>();
	NewMin->Initialize(nullptr, GoalPlayerMin);
	UAHTNAI_MethodDecomposition* NewMax = NewObject<UAHTNAI_MethodDecomposition>();
	NewMax->Initialize(nullptr, GoalPlayerMax);
	MaxPlanRoot = NewMax;
	MinPlanRoot = NewMin;
	MinPlanRoot->RenameVariables(1);
	RenamingIndex = 2;
	MaxPlayer = A_maxPlayer;
	Gs = A_gs;
	Dd = A_dd;
	Stack.Empty();
	maxDepth = Depth;
	PLAYOUT_LOOKAHEAD = PlayoutLookahead;
	f = A_f;
	PlayoutAI = A_playoutAI;
        
	//        System.out.println(a_dd);
}


TOptional<TPair<UAHTNAI_MethodDecomposition*, UAHTNAI_MethodDecomposition*>> UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::
GetBestPlan(int64 TimeLimit, int32 MaxPlayouts, bool ForceAnswer)
{
	if (DEBUG>=1) UE_LOG(LogTemp, Warning, TEXT("AdversarialBoundedDepthPlanner.getBestPlan"));
        if (Stack.Num()==0) {
            if (DEBUG>=1) UE_LOG(LogTemp, Warning, TEXT("AdversarialBoundedDepthPlanner.getBestPlan: first time, initializing stack"));
        	UAHTNAI_AdversarialChoicePoint* Ncp = NewObject<UAHTNAI_AdversarialChoicePoint>();
        	Ncp->Initialize(MaxPlanRoot, MinPlanRoot, MaxPlanRoot, MinPlanRoot, Gs, 0, -1, -UAHTNAI_EvaluationFunction::VICTORY, UAHTNAI_EvaluationFunction::VICTORY, false);
            Stack.Insert(Ncp, 0);
            Trail.Insert(0,0);
        }
        last_tree_leaves = 0;
        last_tree_nodes = 0;
        last_tree_depth = 0;
        last_time_depth = 0;
        UAHTNAI_AdversarialChoicePoint* Root = Stack[Stack.Num()-1];
        bool timeout = false;
//        int n_times_deadend_is_reached = 0;

        do{
            if ((TimeLimit>0 && FDateTime::UtcNow().ToUnixTimestamp() * 1000>=TimeLimit) ||
                (MaxPlayouts>0 && nPlayouts>=MaxPlayouts)){
//                if (!timeout) System.out.println(root.gs.getTime() + ": timeout!!!! depth: " + maxDepth +", with leaves: " + last_tree_leaves + " (timelimit: " + timeLimit + " currenttime: " + System.currentTimeMillis() + ")");
//                System.out.println("maxDepth: " + maxDepth + ", nPlayouts: " + nPlayouts + ", n_times_deadend_is_reached: " + n_times_deadend_is_reached);
                if (ForceAnswer) {
                    timeout = true;
                } else {
                    return TOptional<TPair<UAHTNAI_MethodDecomposition*, UAHTNAI_MethodDecomposition*>>();
                }
            }

            // statistics:
            int32 treedepth = Stack.Num();
            if (treedepth>=last_tree_depth) last_tree_depth = treedepth;

            UAHTNAI_AdversarialChoicePoint* ChoicePoint = Stack[0];
            ChoicePoint->RestoreExecutionState();

            if (DEBUG>=2) {
            	//UE_LOG(LogTemp, Warning, TEXT("\nAdversarialBoundedDepthPlanner.getBestPlan: stack size: %i bindings: %i, gs.time: %i, operators: " + root.choicePointPlayerMin.getOperatorsBeingExecuted() + ", " + root.choicePointPlayerMax.getOperatorsBeingExecuted()"));
                //System.out.println("\nAdversarialBoundedDepthPlanner.getBestPlan: stack size: " + stack.size() + ", bindings: " + bindings.size() + ", gs.time: " + choicePoint.gs.getTime() + ", operators: " + root.choicePointPlayerMin.getOperatorsBeingExecuted() + ", " + root.choicePointPlayerMax.getOperatorsBeingExecuted());
/*
                if (stack.size()==23) {
                    maxPlanRoot.printDetailed();
                    minPlanRoot.printDetailed();                    
                }
    */
            }
            if (DEBUG>=3) {
            	/*
                System.out.println("AdversarialBoundedDepthPlanner.getBestPlan: bindings: " + bindings);
                System.out.println("AdversarialBoundedDepthPlanner.getBestPlan: trail: " + trail);
                System.out.println("AdversarialBoundedDepthPlanner.getBestPlan: stack:");
                for(int i = 0;i<stack.size();i++) {
                    System.out.println((stack.size()-i) + ": " + stack.get(i));
                }
                maxPlanRoot.printDetailed();
                minPlanRoot.printDetailed();*/
            }

            int32 bl = Bindings.Num();
            bool pop = true;
            while(!timeout && ChoicePoint->NextExpansion(Dd, Bindings, RenamingIndex, ChoicePoint)) {
                RenamingIndex++;
//                System.out.println("bindings: " + bindings);
                UAHTNAI_AdversarialChoicePoint* tmp = SimulateUntilNextChoicePoint(Bindings, ChoicePoint);

                if (tmp==nullptr) {
//                    System.out.println("  tmp = null");
                    // execution failure:
                    //if (DEBUG>=2) //System.out.println("AdversarialBoundedDepthPlanner.getBestPlan: plan execution failed");
                    ChoicePoint->RestoreExecutionState();
//                    n_times_deadend_is_reached++;
                } else if ((tmp->ChoicePointPlayerMax==nullptr && tmp->ChoicePointPlayerMin==nullptr)) {
                    last_tree_nodes++;
                    // execution success:
                    //if (DEBUG>=2) System.out.println("AdversarialBoundedDepthPlanner.getBestPlan: plan execution success or depth limit reached");
                    //if (DEBUG>=3) System.out.println(tmp.gs);
                	/*
                    if (DEBUG>=3) {
                        maxPlanRoot.printDetailed();
                        minPlanRoot.printDetailed();
                    }*/
                    // use evaluation function
                    float Eval = Playout(MaxPlayer, tmp->Gs);
//                    float eval = f.evaluate(maxPlayer, 1-maxPlayer, tmp.gs);
                    last_tree_leaves++;
                    bool AlphaBetaTest = ChoicePoint->ProcessEvaluation(Eval, ChoicePoint->MaxPlanRoot, ChoicePoint->MinPlanRoot, true);
                    if (!ALPHA_BETA_CUT) AlphaBetaTest = false;

                    double Time = tmp->Gs->Seconds - Root->Gs->Seconds;
                    if (Time > last_time_depth) last_time_depth = Time;
/*
                    if (DEBUG>=2) {
                        System.out.println("---- ---- ---- ----");
                        System.out.println(tmp.gs);
                        System.out.println("Evaluation: " + eval);
                        System.out.println("Bindings: " + bindings.size());
                        System.out.println("Bindings: " + bindings);
                        System.out.println("Max plan:");
                        List<Pair<Integer,List<Term>>> l = maxPlanRoot.convertToOperatorList();
                        for(Pair<Integer, List<Term>> a:l) System.out.println("  " + a.m_a + ": " + a.m_b);
                        System.out.println("Min plan:");
                        List<Pair<Integer,List<Term>>> l2 = minPlanRoot.convertToOperatorList();
                        for(Pair<Integer, List<Term>> a:l2) System.out.println("  " + a.m_a + ": " + a.m_b);
                    }*/
                    ChoicePoint->RestoreExecutionState();
                    // undo the bindings:
                    int32 n = Bindings.Num() - bl;
                    for(int i = 0;i<n;i++) Bindings.RemoveAt(i);
                    if (AlphaBetaTest) break;
                } else {
                    last_tree_nodes++;
//                    System.out.println("  next choice point");
                    // next choice point:
                    //if (DEBUG>=2) System.out.println("AdversarialBoundedDepthPlanner.getBestPlan: stack push");
                    Stack.Insert(tmp,0);
                    Trail.Insert(Bindings.Num() - bl,0);
//                    System.out.println("trail.add(" + (bindings.size() - bl) + ")");
                    pop = false;
                    break;
                }
            }
            if (pop) {
                if (!timeout && nPlayouts==0) {
                    UAHTNAI_AdversarialChoicePoint* cp = Stack[0];
                    if ((cp->ChoicePointPlayerMax != nullptr && 
                         cp->ChoicePointPlayerMax->GetType() == EMethodType::METHOD_METHOD) ||
                        (cp->ChoicePointPlayerMax==nullptr && 
                         cp->ChoicePointPlayerMin!=nullptr && 
                         cp->ChoicePointPlayerMin->GetType() == EMethodType::METHOD_METHOD)) {
                        //System.err.println("Popping without finding any decomposition:");
                        //System.err.println(cp);
                        //System.err.println(bindings);
                        //System.err.println(cp.gs);
                        //throw new Error("Popping without finding any decomposition");
                    }
                }
//                System.out.println("Popping:");
                do {
//                    System.out.println("  pop");
                    pop = false;
                    StackPop();
                    //if (DEBUG>=2) System.out.println("AdversarialBoundedDepthPlanner.nextPlan: stack pop");

                    // propagate the value up:
                    if (Stack.Num() != 0) {
                        if (ChoicePoint->MinimaxType==-1) {
                            // this means that this was a dead-end
                            /*
                            float eval = f.evaluate(0, 1, choicePoint.gs);
                            nleaves++;
                            choicePoint.processEvaluation(eval,
                                    (choicePoint.choicePointPlayerMax!=null ? null:maxPlanRoot),
                                    (choicePoint.choicePointPlayerMax==null ? null:minPlanRoot));
    //                        float eval = f.evaluate(0, 1, choicePoint.gs);
    //                        stack.get(0).processEvaluaiton(eval, maxPlanRoot, minPlanRoot);
                            pop = stack.get(0).processEvaluation(choicePoint.bestEvaluation, choicePoint.bestMaxPlan, choicePoint.bestMinPlan);
                            */
                        } else {
                            pop = Stack[0]->ProcessEvaluation(ChoicePoint->BestEvaluation, ChoicePoint->BestMaxPlan, ChoicePoint->BestMinPlan, false);
                            if (!ALPHA_BETA_CUT) pop = false;
                        }
                        if (pop) {
                            ChoicePoint = Stack[0];
                        }
                    }
                }while(pop && Stack.Num() != 0);
            }
        }while(Stack.Num() != 0);

        /*
        if (last_time_depth==0) {
            System.out.println(root.gs);
            System.exit(1);
        }
        */

//        System.out.println(root.gs.getTime() + ": depth: " + maxDepth + ", maxtreedepth: " + last_tree_depth + ", maxtimedepth: " + last_time_depth + ", nleaves: " + last_tree_leaves + ", evaluation: " + root.bestEvaluation);

        //if (DEBUG>=1) System.out.println(last_tree_leaves);
        /*
        {
            System.out.println(nleaves + " -> best move: " + root.bestEvaluation);
            System.out.println("Max plan:");
            if (root.bestMaxPlan!=null) {
                List<Pair<Integer,List<Term>>> l = root.bestMaxPlan.convertToOperatorList();
                for(Pair<Integer, List<Term>> a:l) System.out.println("  " + a.m_a + ": " + a.m_b);
            }
            if (root.bestMinPlan!=null) {
                System.out.println("Min plan:");
                List<Pair<Integer,List<Term>>> l2 = root.bestMinPlan.convertToOperatorList();
                for(Pair<Integer, List<Term>> a:l2) System.out.println("  " + a.m_a + ": " + a.m_b);
            }
        }
        */

        Stack.Empty();
//        System.out.println("AdversarialBoundedDepthPlanner.nextPlan: options exhausted for rootPlan (timeout = " + timeout + ", nPLayouts = "+nPlayouts+")");
        //if (DEBUG>=1) System.out.println("AdversarialBoundedDepthPlanner.nextPlan: options exhausted for rootPlan");
        //if (DEBUG>=1) System.out.println("best evaluation: " + root.bestEvaluation);
        if (Root->BestEvaluation==UAHTNAI_EvaluationFunction::VICTORY ||
            Root->BestEvaluation==-UAHTNAI_EvaluationFunction::VICTORY) LastRunSolvedTheProblem = true;
        // if this happens, it means that there is no plan that can be made for the current situation:
        if (Root->BestMaxPlan==nullptr &&
            Root->BestMinPlan==nullptr) {
            LastRunSolvedTheProblem = true;
            //System.out.println("No AHTN can be found for situation:");
            //System.out.println(gs);
        }
	    TPair<UAHTNAI_MethodDecomposition*, UAHTNAI_MethodDecomposition*> ReturnPair;
	    ReturnPair.Key = Root->BestMaxPlan;
	    ReturnPair.Value = Root->BestMinPlan;
        return ReturnPair;
}

TPair<UAHTNAI_MethodDecomposition*, UAHTNAI_MethodDecomposition*> UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::
GetBestPlanIterativeDeepening(UAHTNAI_Term* GoalPlayerMax, UAHTNAI_Term* GoalPlayerMin, int32 A_maxPlayer, int32 Timeout,
    int32 MaxPlayouts, int32 A_playoutLookahead, ABaseGameState* A_gs, UAHTNAI_DomainDefinition* A_dd,
    UAHTNAI_EvaluationFunction* A_f, UAHTNAI_AIPlayout* A_playoutAI)
{
	int64 Start = A_gs->Seconds;
    int64 TimeLimit = Start + Timeout;
    if (Timeout<=0) TimeLimit = 0;
        TPair<UAHTNAI_MethodDecomposition*,UAHTNAI_MethodDecomposition*> BestLastDepth;
        float tmp_leaves = 0, tmp_nodes = 0, tmp_depth = 0, tmp_time = 0;
        int32 nPlayoutsBeforeStartingLastTime = 0, nPlayoutsUSedLastTime = 0;
        nPlayouts = 0;
        for(int32 depth = 1;;depth++) {
//        for(int depth = 6;depth<7;depth++) {
            TPair<UAHTNAI_MethodDecomposition*,UAHTNAI_MethodDecomposition*> Best;
            int64 CurrentTime = A_gs->GetWorld()->GetTimeSeconds();
            //if (DEBUG>=1) System.out.println("Iterative Deepening depth: " + depth + " (total time so far: " + (currentTime - start) + "/" + timeout + ")" + " (total playouts so far: " + nPlayouts + "/" + maxPlayouts + ")");
            UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta* Planner = NewObject<UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta>();
            Planner->Initialize(GoalPlayerMax, GoalPlayerMin, A_maxPlayer, depth, A_playoutLookahead, A_gs, A_dd, A_f, A_playoutAI);
            nPlayoutsBeforeStartingLastTime = nPlayouts;
            if (depth<=MAX_TREE_DEPTH) {
                int nPlayoutsleft = MaxPlayouts - nPlayouts;
                if (MaxPlayouts<0 || nPlayoutsleft>nPlayoutsUSedLastTime) {
                    //if (DEBUG>=1) System.out.println("last time we used " + nPlayoutsUSedLastTime + ", and there are " + nPlayoutsleft + " left, trying one more depth!");
                    Best = Planner->GetBestPlan(TimeLimit, MaxPlayouts, (BestLastDepth.Key == nullptr || BestLastDepth.Value == nullptr)).GetValue();
                } else {
                    //if (DEBUG>=1) System.out.println("last time we used " + nPlayoutsUSedLastTime + ", and there are only " + nPlayoutsleft + " left..., canceling search");
                }
                
            }
            nPlayoutsUSedLastTime = nPlayouts - nPlayoutsBeforeStartingLastTime;
            //if (DEBUG>=1) System.out.println("    time taken: " + (System.currentTimeMillis() - currentTime));

            // print best plan:
            /*
            if (DEBUG>=1) {
                if (Best!=nullptr) {
//                    if (best.m_a!=null) best.m_a.printDetailed();
                    //System.out.println("Max plan:");
                    if (Best.Key!=nullptr) {
                        TArray<TPair<int32,TArray<UAHTNAI_Term*>>> l = Best.Key->ConvertToOperatorList();
                        //for(TPair<int32, TArray<UAHTNAI_Term*>> a:l) System.out.println("  " + a.m_a + ": " + a.m_b);
                    }
                    if (Best.Value!=nullptr) {
                        //System.out.println("Min plan:");
                        List<Pair<Integer,List<Term>>> l2 = best.m_b.convertToOperatorList();
                        //for(Pair<Integer, List<Term>> a:l2) System.out.println("  " + a.m_a + ": " + a.m_b);
                    }
                }
            }
            */

            if (Best.Key != nullptr || Best.Value != nullptr) {
                BestLastDepth = Best;
                tmp_leaves = last_tree_leaves;
                tmp_nodes = last_tree_nodes;
//                System.out.println("nodes: " + tmp_nodes + ", leaves: " + tmp_leaves);
                tmp_depth = last_tree_depth;
                tmp_time = last_time_depth;
                if (Planner->LastRunSolvedTheProblem) {
                    // statistics:
                    n_trees++;
                    if (tmp_leaves>max_tree_leaves) max_tree_leaves=tmp_leaves;
                    average_tree_leaves += tmp_leaves;
                    if (tmp_nodes>max_tree_nodes) max_tree_nodes=tmp_nodes;
                    average_tree_nodes += tmp_nodes;
                    average_tree_depth += tmp_depth;
                    if (tmp_depth>=max_tree_depth) max_tree_depth = tmp_depth;
                    average_time_depth += tmp_time;
                    if (tmp_time>=max_time_depth) max_time_depth = tmp_time;

                    n_iterative_deepening_runs++;
                    average_iterative_deepening_depth+=depth;
                    if (depth>max_iterative_deepening_depth) max_iterative_deepening_depth = depth;
                    return BestLastDepth;
                }
            } else {
                // statistics:
                n_trees++;
                if (tmp_leaves>max_tree_leaves) max_tree_leaves=tmp_leaves;
                average_tree_leaves += tmp_leaves;
                if (tmp_nodes>max_tree_nodes) max_tree_nodes=tmp_nodes;
                average_tree_nodes += tmp_nodes;
                average_tree_depth += tmp_depth;
                if (tmp_depth>=max_tree_depth) max_tree_depth = tmp_depth;
                average_time_depth += tmp_time;
                if (tmp_time>=max_time_depth) max_time_depth = tmp_time;

                n_iterative_deepening_runs++;
                average_iterative_deepening_depth+=depth-1; // the last one couldn't finish, so we have to add "depth-1"
                if ((depth-1)>max_iterative_deepening_depth) max_iterative_deepening_depth = depth-1;
                return BestLastDepth;
            }
        }
//        return bestLastDepth;
}

UAHTNAI_AdversarialChoicePoint* UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::SimulateUntilNextChoicePoint(
	TArray<UAHTNAI_Bindings*> Bindings, UAHTNAI_AdversarialChoicePoint* Previous_cp)
{
	ABaseGameState* State = Previous_cp->Gs;
        ABaseGameState* State2 = Gs; //->Clone();
        int lastTimeOperatorsIssued = Previous_cp->GetLastTimeOperatorsIssued();
        int operatorDepth = Previous_cp->GetOperatorDepth();
//        System.out.println(gs2.getTime() + " - " + lastTimeOperatorsIssued + " - " + operatorDepth);
        while(true) {
//            System.out.println(bindings);
            if (!SIMULATE_UNTIL_NEXT_CHOICEPOINT) {
                if (operatorDepth>=maxDepth && State2->Seconds >lastTimeOperatorsIssued) {
                    // max depth reached:
                    UAHTNAI_AdversarialChoicePoint* NewPoint = NewObject<UAHTNAI_AdversarialChoicePoint>();
                    NewPoint->Initialize(nullptr,nullptr,Previous_cp->MaxPlanRoot,Previous_cp->MinPlanRoot, State2,operatorDepth,lastTimeOperatorsIssued,Previous_cp->GetAlpha(),Previous_cp->GetBeta(),false);
                    return NewPoint;
                }
            }
            TArray<UAHTNAI_MethodDecomposition*> Actions1;
            TArray<UAHTNAI_MethodDecomposition*> Actions2;
            TArray<UAHTNAI_MethodDecomposition*> ChoicePoints1;
            TArray<UAHTNAI_MethodDecomposition*> ChoicePoints2;
            EExecutionType Er1 = Previous_cp->MaxPlanRoot->ExecutionCycle(State2, Actions1, ChoicePoints1, Previous_cp);
            EExecutionType Er2 = Previous_cp->MinPlanRoot->ExecutionCycle(State2, Actions2, ChoicePoints2, Previous_cp);
            if (SIMULATE_UNTIL_NEXT_CHOICEPOINT) {
                if (operatorDepth>=maxDepth && State2->Seconds >lastTimeOperatorsIssued &&
                    (Er1==EExecutionType::EXECUTION_CHOICE_POINT ||
                     Er2==EExecutionType::EXECUTION_CHOICE_POINT)) {
                    // max depth reached:
//                    System.out.println(operatorDepth + " >= " + maxDepth);
                    UAHTNAI_AdversarialChoicePoint* NewPoint = NewObject<UAHTNAI_AdversarialChoicePoint>();
                    NewPoint->Initialize(nullptr,nullptr,Previous_cp->MaxPlanRoot,Previous_cp->MinPlanRoot, State2,operatorDepth,lastTimeOperatorsIssued,Previous_cp->GetAlpha(),Previous_cp->GetBeta(),false);
                    return NewPoint;
                }
            }
            if (Er1==EExecutionType::EXECUTION_SUCCESS &&
                Er2==EExecutionType::EXECUTION_SUCCESS) {
                UAHTNAI_AdversarialChoicePoint* NewPoint = NewObject<UAHTNAI_AdversarialChoicePoint>();
                NewPoint->Initialize(nullptr,nullptr,Previous_cp->MaxPlanRoot,Previous_cp->MinPlanRoot, State2,operatorDepth,lastTimeOperatorsIssued,Previous_cp->GetAlpha(),Previous_cp->GetBeta(),false);
                return NewPoint;
            } else if (Er1==EExecutionType::EXECUTION_FAILURE ||
                       Er2==EExecutionType::EXECUTION_FAILURE) {
                //if (DEBUG>=2) System.out.println("AdversarialBoundedDepthPlanner.simulateUntilNextChoicePoint: execution failure " + er1 + ", " + er2);
                return nullptr;
            } else if (Er1==EExecutionType::EXECUTION_CHOICE_POINT ||
                       Er2==EExecutionType::EXECUTION_CHOICE_POINT) {
                UAHTNAI_MethodDecomposition* Cp_md = (ChoicePoints1.Num() == 0 ? nullptr:ChoicePoints1[0]);
                if (Cp_md==nullptr) Cp_md = (ChoicePoints2.Num() == 0 ? nullptr : ChoicePoints2[0]);
                if (Cp_md->GetType() == EMethodType::METHOD_NON_BRANCHING_CONDITION) {
                     UAHTNAI_AdversarialChoicePoint* Acp = NewObject<UAHTNAI_AdversarialChoicePoint>();
                    Acp->Initialize((ChoicePoints1.Num() == 0 ? nullptr : ChoicePoints1[0]),
                                                                             (ChoicePoints2.Num() == 0 ? nullptr : ChoicePoints2[0]),
                                                                             Previous_cp->MaxPlanRoot,Previous_cp->MinPlanRoot,
                                                                             State2,operatorDepth,lastTimeOperatorsIssued,
                                                                             Previous_cp->GetAlpha(),Previous_cp->GetBeta(),false);
//                     System.out.println("testing non-branching condition: " + cp_md);
                     if (!Acp->NextExpansion(Dd, Bindings, RenamingIndex, Acp)) return nullptr;
                     RenamingIndex++;
                } else {
                    UAHTNAI_AdversarialChoicePoint* ReturnPoint = NewObject<UAHTNAI_AdversarialChoicePoint>();
                    ReturnPoint->Initialize((ChoicePoints1.Num() == 0 ? nullptr : ChoicePoints1[0]),
                                                      (ChoicePoints2.Num() == 0 ? nullptr : ChoicePoints2[0]),
                                                      Previous_cp->MaxPlanRoot,Previous_cp->MinPlanRoot,
                                                      State2,operatorDepth,lastTimeOperatorsIssued,
                                                      Previous_cp->GetAlpha(),Previous_cp->GetBeta(),false);
                    return ReturnPoint;
                }
            } else if ((Er1 == EExecutionType::EXECUTION_WAITING_FOR_ACTION ||
                        Er2 == EExecutionType::EXECUTION_WAITING_FOR_ACTION) &&
                       Er1 != EExecutionType::EXECUTION_ACTION_ISSUE &&
                       Er2 != EExecutionType::EXECUTION_ACTION_ISSUE) {
                bool gameover = State2->DeathSquads.Num()>8; // TO_DOO
                if (gameover)
                {
                    UAHTNAI_AdversarialChoicePoint* ReturnPoint = NewObject<UAHTNAI_AdversarialChoicePoint>();
                    ReturnPoint->Initialize(nullptr,nullptr,Previous_cp->MaxPlanRoot,Previous_cp->MinPlanRoot, State2,operatorDepth,lastTimeOperatorsIssued,Previous_cp->GetAlpha(),Previous_cp->GetBeta(),false);
                    return ReturnPoint;
                }
                TArray<UAHTNAI_MethodDecomposition*> ToDelete;
                if (Previous_cp->MaxPlanRoot->GetOperatorsBeingExecuted().Num() != 0) {
                    for(UAHTNAI_MethodDecomposition* md:Previous_cp->MaxPlanRoot->GetOperatorsBeingExecuted()) {
                        Previous_cp->CaptureExecutionStateNonRecursive(md);
                        // issue action:
                        //if (DEBUG>=2) System.out.println("AdversarialBoundedDepthPlanner.simulateUntilNextChoicePoint: continuing executing operator " + md.getUpdatedTerm());
                        if (UAHTNAI_PredefinedOperators::Execute(md, State2, nullptr) ||
                            State2->Seconds > md->GetUpdatedTermCycle()+operatorExecutionTimeout) {
//                            if (gs2.getTime()>md.getUpdatedTermCycle()+operatorExecutionTimeout) System.out.println("operator timed out: " + md.getUpdatedTerm());
                            md->SetExecutionState(EExecutionType::EXECUTION_ACTION_ISSUE);
                            if (ToDelete.Num() == 0) ToDelete.Empty();
                            ToDelete.Add(md);
                            //if (DEBUG>=2) System.out.println("AdversarialBoundedDepthPlanner.simulateUntilNextChoicePoint: operator complete (1).");
                        } else {
                            md->SetExecutionState(EExecutionType::EXECUTION_FAILURE);
                        }
                    }
                    if (ToDelete.Num() != 0)
                    {
                        Previous_cp->MaxPlanRoot->GetOperatorsBeingExecuted().RemoveAll([&](UAHTNAI_MethodDecomposition* Item) {
                        return ToDelete.Contains(Item);
                        });
                    }
                }
                if (Previous_cp->MinPlanRoot->GetOperatorsBeingExecuted().Num() != 0) {
                    ToDelete.Empty();
                    for(UAHTNAI_MethodDecomposition* md:Previous_cp->MinPlanRoot->GetOperatorsBeingExecuted()) {
                        Previous_cp->CaptureExecutionStateNonRecursive(md);
                        // issue action:
                        //if (DEBUG>=2) System.out.println("AdversarialBoundedDepthPlanner.simulateUntilNextChoicePoint: continuing executing operator " + md.getUpdatedTerm());
                        if (UAHTNAI_PredefinedOperators::Execute(md, State2, nullptr) ||
                            State2->Seconds > md->GetUpdatedTermCycle()+operatorExecutionTimeout) {
//                            if (gs2.getTime()>md.getUpdatedTermCycle()+operatorExecutionTimeout) System.out.println("operator timed out: " + md.getUpdatedTerm());
                            md->SetExecutionState(EExecutionType::EXECUTION_ACTION_ISSUE);
                            if (ToDelete.Num() == 0) ToDelete.Empty();
                            ToDelete.Add(md);
                            //if (DEBUG>=2) System.out.println("AdversarialBoundedDepthPlanner.simulateUntilNextChoicePoint: operator complete (2).");
                        } else {
                            md->SetExecutionState(EExecutionType::EXECUTION_FAILURE);
                        }
                    }
                    if (ToDelete.Num() != 0)
                    {
                        Previous_cp->MinPlanRoot->GetOperatorsBeingExecuted().RemoveAll([&](UAHTNAI_MethodDecomposition* Item)
                        {
                            return ToDelete.Contains(Item);
                        });
                    }
                }
            }

            if (Er1==EExecutionType::EXECUTION_ACTION_ISSUE ||
                Er2==EExecutionType::EXECUTION_ACTION_ISSUE) {
                if (State2->Seconds > lastTimeOperatorsIssued) {
                    lastTimeOperatorsIssued = State2->Seconds;
                    operatorDepth++;
                }
            }
            
            if (Er1==EExecutionType::EXECUTION_ACTION_ISSUE) {
                for(UAHTNAI_MethodDecomposition* md:Actions1) {
                    Previous_cp->CaptureExecutionStateNonRecursive(md);
                    md->SetUpdatedTerm(md->GetTerm()->Clone());
                    md->GetUpdatedTerm()->ApplyBindings(Bindings);
                    md->SetUpdatedTermCycle(State2->Seconds);
                    // issue action:
                    //if (DEBUG>=2) System.out.println("AdversarialBoundedDepthPlanner.simulateUntilNextChoicePoint: executing operator " + md.getUpdatedTerm());
                    md->SetOperatorExecutingState(EExecutionType::EXECUTION_SUCCESS);
//                    System.out.println(md.getUpdatedTerm() + " <- " + bindings);
                    if (UAHTNAI_PredefinedOperators::Execute(md, State2, nullptr)) {
                        md->SetExecutionState(EExecutionType::EXECUTION_ACTION_ISSUE);
                        //if (DEBUG>=2) System.out.println("AdversarialBoundedDepthPlanner.simulateUntilNextChoicePoint: operator complete (3).");
                    } else {
                        md->SetExecutionState(EExecutionType::EXECUTION_FAILURE);
                        if (Previous_cp->MaxPlanRoot->GetOperatorsBeingExecuted().Num() == 0) {
                            //Previous_cp.maxPlanRoot.setOperatorsBeingExecuted(new ArrayList<>());
                        }
                        Previous_cp->MaxPlanRoot->GetOperatorsBeingExecuted().Add(md);
                    }
                }
            }
            if (Er2== EExecutionType::EXECUTION_ACTION_ISSUE) {
                for(UAHTNAI_MethodDecomposition* md:Actions2) {
                    Previous_cp->CaptureExecutionStateNonRecursive(md);
                    md->SetUpdatedTerm(md->GetTerm()->Clone());
                    md->GetUpdatedTerm()->ApplyBindings(Bindings);
                    md->SetUpdatedTermCycle(State2->Seconds);
                    // issue action:
                    //if (DEBUG>=2) System.out.println("AdversarialBoundedDepthPlanner.simulateUntilNextChoicePoint: executing operator " + md.getUpdatedTerm());
                    md->SetOperatorExecutingState(EExecutionType::EXECUTION_SUCCESS);
                    if (UAHTNAI_PredefinedOperators::Execute(md, State2, nullptr)) {
                        md->SetExecutionState(EExecutionType::EXECUTION_ACTION_ISSUE);
                        //if (DEBUG>=2) System.out.println("AdversarialBoundedDepthPlanner.simulateUntilNextChoicePoint: operator complete (4).");
                    } else {
                        md->SetExecutionState(EExecutionType::EXECUTION_FAILURE);
                        if (Previous_cp->MinPlanRoot->GetOperatorsBeingExecuted().Num() == 0) {
                            //Previous_cp->MinPlanRoot->SetOperatorsBeingExecuted(new ArrayList<>());
                        }
                        Previous_cp->MinPlanRoot->GetOperatorsBeingExecuted().Add(md);
                    }
                }
            }
        }
}

void UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::StackPop()
{
    UAHTNAI_AdversarialChoicePoint* cp = Stack[0];
    Stack.RemoveAt(0);
    cp->RestoreAfterPop();
    int tmp = Trail[0];
    Trail.RemoveAt(0);
    //if (DEBUG>=2) System.out.println("StackPop! removing " + tmp + " bindings.");
    for(int i = 0;i<tmp;i++) Bindings.RemoveAt(i);
    if (Stack.Num() != 0) Stack[0]->RestoreExecutionState();
}

float UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::Playout(int32 player, ABaseGameState* Gs)
{
    nPlayouts++;
    ABaseGameState* State2 = Gs;
        
    if (PLAYOUT_LOOKAHEAD> 0 && PlayoutAI != nullptr) {
        UAHTNAI_AIPlayout* ai1 = PlayoutAI->Clone();
        UAHTNAI_AIPlayout* ai2 = PlayoutAI->Clone();
        State2 = Gs; //.clone();
        ai1->Reset();
        ai2->Reset();
        int timeLimit = State2->Seconds + PLAYOUT_LOOKAHEAD;
        bool gameover = false;
                        
        while(!gameover && State2->Seconds<timeLimit) {
            /*
            if (gs2.isComplete()) {
                gameover = gs2.cycle();
            } else {
                PlayerAction pa1 = ai1.getAction(player, gs2);
                PlayerAction pa2 = ai2.getAction(1-player, gs2);
                //                    System.out.println("time: " + gs2.getTime() + " resources: " + gs2.getPlayer(0).getResources() + "/" + gs2.getPlayer(1).getResources());
                //                    System.out.println("  pa1: " + pa1);
                //                    System.out.println("  pa2: " + pa2);
                gs2.issue(pa1);
                gs2.issue(pa2);
            }*/
        }        
    } 
    float e = f->Evaluate(player, 1-player, State2);
    //        if (DEBUG>=1) System.out.println("  done: " + e);
    return e;
}
