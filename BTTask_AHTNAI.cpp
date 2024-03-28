// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020


#include "BTTask_AHTNAI.h"

#include "Domain/AHTNAI_DomainDefinition.h"
#include "Domain/AHTNAI_Term.h"
#include "Planner/AHTNAI_AdversarialBoundedDepthPlannerAlphaBeta.h"
#include "System/AHTNAI_AIPlayout.h"
#include "System/AHTNAI_EvaluationFunction.h"

void UBTTask_AHTNAI::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	DomainFileName = "C:/Users/FunverseGames58/wkspaces/HitAndBoom/Content/microrts-ahtn-definition-lowest-level.lisp";
	DomainDefinition = UAHTNAI_DomainDefinition::FromLispFile(DomainFileName);
	PLAYOUT_LOOKAHEAD = 100;
	EFunction = NewObject<UAHTNAI_EvaluationFunction>();
	AIPlayout = NewObject<UAHTNAI_AIPlayout>();
}

EBTNodeResult::Type UBTTask_AHTNAI::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABaseGameState* Gs = Cast<ABaseGameState>(GetWorld()->GetGameState());
	int32 Player = 0;
	UAHTNAI_Term* Goal1 = UAHTNAI_Term::FromString("(destroy-player "+FString::FromInt(Player)+" "+FString::FromInt((1-Player))+" )");
	UAHTNAI_Term* Goal2 = UAHTNAI_Term::FromString("(destroy-player "+FString::FromInt(1-Player)+" "+FString::FromInt(Player)+" )");
	TPair<UAHTNAI_MethodDecomposition*, UAHTNAI_MethodDecomposition*> Plan = UAHTNAI_AdversarialBoundedDepthPlannerAlphaBeta::GetBestPlanIterativeDeepening(Goal1,
		Goal2,
		Player,
		TIME_BUDGET,
		ITERATIONS_BUDGET,
		PLAYOUT_LOOKAHEAD,
		Gs,
		DomainDefinition,
		EFunction,
		AIPlayout);
	
	return EBTNodeResult::Succeeded;
}
