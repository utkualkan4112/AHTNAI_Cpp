// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020


#include "AHTNAI_AdversarialChoicePoint.h"

#include "AdvancedAI/AHTNAI/Domain/AHTNAI_Clause.h"
#include "AdvancedAI/AHTNAI/Domain/AHTNAI_DomainDefinition.h"
#include "AdvancedAI/AHTNAI/Domain/AHTNAI_HTNMethod.h"
#include "AdvancedAI/AHTNAI/Domain/AHTNAI_MethodDecomposition.h"
#include "AHTNAI_MethodDecompositionState.h"
#include "AdvancedAI/AHTNAI/Domain/AHTNAI_Symboll.h"

int32 UAHTNAI_AdversarialChoicePoint::DEBUG = 0;

void UAHTNAI_AdversarialChoicePoint::Initialize(UAHTNAI_MethodDecomposition* CpMax, UAHTNAI_MethodDecomposition* CpMin,
                                                UAHTNAI_MethodDecomposition* RootMax, UAHTNAI_MethodDecomposition* RootMin, ABaseGameState* A_gs, int32 Nops,
                                                int32 Ltoi)
{
	MaxPlanRoot = RootMax;
	MinPlanRoot = RootMin;
	ChoicePointPlayerMax = CpMax;
	ChoicePointPlayerMin = CpMin;
	Gs = A_gs;
	OperatorDepth = Nops;
	LastTimeOperatorsIssued = Ltoi;
	
	CaptureExecutionState(RootMax);
	CaptureExecutionState(RootMin);  
}

void UAHTNAI_AdversarialChoicePoint::Initialize(UAHTNAI_MethodDecomposition* CpMax, UAHTNAI_MethodDecomposition* CpMin,
                                                UAHTNAI_MethodDecomposition* RootMax, UAHTNAI_MethodDecomposition* RootMin, ABaseGameState* A_gs, int32 Nops,
                                                int32 Ltoi, float A, float B, bool SaveExecutionState)
{

	MaxPlanRoot = RootMax;
	MinPlanRoot = RootMin;
	ChoicePointPlayerMax = CpMax;
	ChoicePointPlayerMin = CpMin;
	Gs = A_gs;
	OperatorDepth = Nops;
	LastTimeOperatorsIssued = Ltoi;
	
	CaptureExecutionState(RootMax);
	CaptureExecutionState(RootMin);  
	Alpha = A;
	Beta = B;
	//        System.out.println("new: " + alpha + " , " + beta);S
	if (SaveExecutionState) {
		CaptureExecutionState(RootMax);
		CaptureExecutionState(RootMin);
	} else {
		CaptureExecutionStateNonRecursive(RootMax);
		CaptureExecutionStateNonRecursive(RootMin);
            
	}
}

bool UAHTNAI_AdversarialChoicePoint::NextConditionExpansion(UAHTNAI_MethodDecomposition* ChoicePoint,
	TArray<UAHTNAI_Bindings*> Bindings)
{
	if (DEBUG>=1) UE_LOG(LogTemp, Warning, TEXT("AdversarialChoicePoint.nextExpansionCondition: testing %s"), *ChoicePoint->GetClause()->ToString());
	if (UpdatedClause==nullptr) {
		UpdatedClause = ChoicePoint->GetClause()->Clone();
		UpdatedClause->ApplyBindings(Bindings);
		UpdatedClauseHadAnyMatches = false;
		LastBindings.Empty();
	}
        
	while(true) {
		//            System.out.println("  Condition: " + updatedClause);
		//            System.out.println("    " + bindings);
		TOptional<TArray<UAHTNAI_Bindings*>> l = UpdatedClause->NextMatch(Gs);
		if (DEBUG>=1)
		{
			FString PrintS = "AdversarialChoicePoint.nextExpansionCondition: bindings ";
			for(int32 i = 0; i < l.GetValue().Num(); i++)
			{
				PrintS += l.GetValue()[i]->ToString();
			}
			UE_LOG(LogTemp, Warning, TEXT("AdversarialChoicePoint.nextExpansionCondition: bindings %s"), *PrintS);
		}
		if (!l.IsSet()) {
			if (UpdatedClauseHadAnyMatches) {
				UpdatedClause = nullptr;
				return false;
			} else {
				ChoicePoint->SetExecutionState(EExecutionType::EXECUTION_ACTION_ISSUE);
				UpdatedClauseHadAnyMatches = true;
				return true;
			}
		} else {
			if (LastBindings.Num()!=0 && l.GetValue() == LastBindings) {
				//                    System.out.println(this.hashCode() + " - !!! " + updatedClause + "\n    " + lastBindings + " ==\n    " + l);
				continue;
			} else
			{
				//                    System.out.println(this.hashCode() + " - ok " + l);
			}
			LastBindings.Append(l.GetValue());

			UpdatedClauseHadAnyMatches = true;
			ChoicePoint->SetExecutionState(EExecutionType::EXECUTION_FAILURE);
			for(UAHTNAI_Bindings* b:l.GetValue()) Bindings.Insert(b, 0);
			return true;
		}                
	}
}

UAHTNAI_AdversarialChoicePoint* UAHTNAI_AdversarialChoicePoint::CloneForMcts()
{
	UAHTNAI_AdversarialChoicePoint* Acp = NewObject<UAHTNAI_AdversarialChoicePoint>();
        
	TArray<UAHTNAI_MethodDecomposition*> in1 = {ChoicePointPlayerMax};
	TArray<UAHTNAI_MethodDecomposition*> out1 = {nullptr};        
	Acp->MaxPlanRoot = MaxPlanRoot->CloneTrackingDescendants(in1,out1);

	TArray<UAHTNAI_MethodDecomposition*> in2 = {ChoicePointPlayerMin};
	TArray<UAHTNAI_MethodDecomposition*> out2 = {nullptr};        
	Acp->MinPlanRoot = MinPlanRoot->CloneTrackingDescendants(in2,out2);
        
	Acp->ChoicePointPlayerMax = out1[0];
	Acp->ChoicePointPlayerMin = out2[0];
	Acp->Gs = Gs; // Make gamestate clone TO_DO
	Acp->LastTimeOperatorsIssued = LastTimeOperatorsIssued;
	Acp->OperatorDepth = OperatorDepth;
        
	/*    
	// no need to clone these ones for the purposes of MCTS:
	List<HTNMethod> possibleMethods = null;    
	Clause updatedClause = null;
	boolean updatedClauseHadAnyMatches = false;
	List<Binding> lastBindings = null;
	HashMap<MethodDecomposition, MethodDecompositionState> executionState = null;
	public int minimaxType = -1;
	public float bestEvaluation = 0;
	public MethodDecomposition bestMaxPlan = null;
	public MethodDecomposition bestMinPlan = null;
	float alpha = 0;
	float beta = 0;
	*/
        
	return Acp;
}

void UAHTNAI_AdversarialChoicePoint::CaptureExecutionState(UAHTNAI_MethodDecomposition* Md)
{
	UAHTNAI_MethodDecompositionState* NewState = NewObject<UAHTNAI_MethodDecompositionState>();
	NewState->Initialize(Md);
	ExecutionState.Add(Md, NewState);
	if (Md->GetSubparts().Num() != 0) {
		for(int i = 0;i<Md->GetSubparts().Num();i++) {
			CaptureExecutionState(Md->GetSubparts()[i]);
		}
	}
	if (Md->GetMethod()!=nullptr) {
		CaptureExecutionState(Md->GetMethod()->GetDecomposition());
	}
}

void UAHTNAI_AdversarialChoicePoint::CaptureExecutionStateNonRecursive(UAHTNAI_MethodDecomposition* Md)
{
	if (!ExecutionState.Find(Md))
	{
		UAHTNAI_MethodDecompositionState* NewState = NewObject<UAHTNAI_MethodDecompositionState>();
	    NewState->Initialize(Md);
		ExecutionState.Add(Md, NewState);
	}
}

void UAHTNAI_AdversarialChoicePoint::RestoreExecutionState()
{
	TArray<UAHTNAI_MethodDecomposition*> Keys;
	ExecutionState.GetKeys(Keys);
	for(UAHTNAI_MethodDecomposition* Md: Keys) {
		UAHTNAI_MethodDecompositionState* Mth = *ExecutionState.Find(Md);
		Mth->RestoreState(Md);
	}
}

void UAHTNAI_AdversarialChoicePoint::RestoreAfterPop()
{
	if (ChoicePointPlayerMax!=nullptr && ChoicePointPlayerMax->GetType() == EMethodType::METHOD_METHOD) {
		ChoicePointPlayerMax->SetMethod(nullptr);
	}
	if (ChoicePointPlayerMin!=nullptr && ChoicePointPlayerMin->GetType() == EMethodType::METHOD_METHOD) {
		ChoicePointPlayerMin->SetMethod(nullptr);            
	}
}

bool UAHTNAI_AdversarialChoicePoint::NextExpansion(UAHTNAI_DomainDefinition* Dd, TArray<UAHTNAI_Bindings*> Bindings,
	int32 RenamingIndex, UAHTNAI_AdversarialChoicePoint* Previous_cp)
{
	if (ChoicePointPlayerMax!=nullptr) {
		Previous_cp->CaptureExecutionStateNonRecursive(ChoicePointPlayerMax);
		if (ChoicePointPlayerMax->GetType() == EMethodType::METHOD_METHOD) {
			return NextMethodExpansion(ChoicePointPlayerMax, Gs, Dd, Bindings, RenamingIndex);
		} else if (ChoicePointPlayerMax->GetType() == EMethodType::METHOD_CONDITION ||
				   ChoicePointPlayerMax->GetType() == EMethodType::METHOD_NON_BRANCHING_CONDITION) {
			return NextConditionExpansion(ChoicePointPlayerMax, Bindings);
				   } else {
				   	UE_LOG(LogTemp, Warning, TEXT("Wrong MethodDecomposition in choicePoint!"));
				   	return false;
				   }
	} else {
		Previous_cp->CaptureExecutionStateNonRecursive(ChoicePointPlayerMin);
		if (ChoicePointPlayerMin->GetType() == EMethodType::METHOD_METHOD) {
			return NextMethodExpansion(ChoicePointPlayerMin, Gs, Dd, Bindings, RenamingIndex);
		} else if (ChoicePointPlayerMin->GetType() == EMethodType::METHOD_CONDITION ||
				   ChoicePointPlayerMin->GetType() == EMethodType::METHOD_NON_BRANCHING_CONDITION) {
			return NextConditionExpansion(ChoicePointPlayerMin, Bindings);
				   } else {
				   	UE_LOG(LogTemp, Warning, TEXT("Wrong MethodDecomposition in choicePoint!"));
				   	return false;
				   }
	}
}

bool UAHTNAI_AdversarialChoicePoint::NextMethodExpansion(UAHTNAI_MethodDecomposition* ChoicePoint, ABaseGameState* Gs,
	UAHTNAI_DomainDefinition* Dd, TArray<UAHTNAI_Bindings*> Bindings, int32 RenamingIndex)
{
	if (DEBUG>=1) UE_LOG(LogTemp, Warning, TEXT("AdversarialChoicePoint.nextMethodExpansion: testing %s"), *ChoicePoint->GetTerm()->ToString());
	if (PossibleMethods.Num() == 0) {
		//            System.out.println("possibleMethods = null");
		//            System.out.println("  Method:" + choicePoint.getTerm());6
		//            System.out.println("    " + bindings);

		TArray<UAHTNAI_HTNMethod*> methods = Dd->GetMethodsForGoal(ChoicePoint->GetTerm()->GetFunctor()); 
		if (methods.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("No methods for:  %s"), *ChoicePoint->GetTerm()->GetFunctor()->ToString());
			return false;
		}
		PossibleMethods.Append(methods);
		if (DEBUG>=1) UE_LOG(LogTemp, Warning, TEXT("AdversarialChoicePoint.nextMethodExpansion: Goal:  %s"), *ChoicePoint->GetTerm()->ToString());
		if (DEBUG>=1) UE_LOG(LogTemp, Warning, TEXT("AdversarialChoicePoint.nextMethodExpansion: Methods found:  %i"), methods.Num());
	}
        
	// if no more methods, reset, and return false:
	if (PossibleMethods.Num() == 0) {
		PossibleMethods.Empty();
		ChoicePoint->SetMethod(nullptr);
		return false;
	}
        
	// otherwise, nextExpansion:
	UAHTNAI_HTNMethod* m = PossibleMethods[0];
	PossibleMethods.RemoveAt(0);
        
	// clone the method (this clones the method, and applies any required variable substitution,
	SetMethod(ChoicePoint, m, Gs, Bindings, RenamingIndex);      
        
	return true;
}

void UAHTNAI_AdversarialChoicePoint::SetMethod(UAHTNAI_MethodDecomposition* ChoicePoint, UAHTNAI_HTNMethod* Original,
	ABaseGameState* Gs, TArray<UAHTNAI_Bindings*> Bindings, int32 RenamingIndex)
{
	// 1. Clone method:
	UAHTNAI_HTNMethod* m = Original->Clone();
	ChoicePoint->SetMethod(m);
        
	// 2. Find variable bindings:
	TOptional<TArray<UAHTNAI_Bindings*>> bindings2 = m->GetHead()->SimpleUnificationDestructiveNoSharedVariables(ChoicePoint->GetTerm(), Gs);
	if (DEBUG>=1)
	{
		FString PrintS = "AdversarialChoicePoint.setMethod: bindings2 ";
		for(int32 i = 0; i < bindings2.GetValue().Num(); i++)
		{
			PrintS += bindings2.GetValue()[i]->ToString();
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *PrintS);
	}
	// 3. Apply bindings to the rest of the method:
	m->ApplyBindings(bindings2.GetValue());
	m->ApplyBindings(Bindings);
	m->RenameVariables(RenamingIndex);
}

bool UAHTNAI_AdversarialChoicePoint::ProcessEvaluation(float f, UAHTNAI_MethodDecomposition* MaxPlan,
	UAHTNAI_MethodDecomposition* MinPlan, bool Clone)
{
	switch(MinimaxType) {
	case -1:
		if (ChoicePointPlayerMax != nullptr) {
			// in "max" choicepoints, we can only accept values that come when "max" can find a plan
			BestEvaluation = f;
			BestMaxPlan = (MaxPlan==nullptr ? nullptr:(Clone ? MaxPlan->Clone():MaxPlan));
			BestMinPlan = (MinPlan==nullptr ? nullptr:(Clone ? MinPlan->Clone():MinPlan));
			MinimaxType = 0;
		} else {
			BestEvaluation = f;
			BestMaxPlan = (MaxPlan==nullptr ? nullptr:(Clone ? MaxPlan->Clone():MaxPlan));
			BestMinPlan = (MinPlan==nullptr ? nullptr:(Clone ? MinPlan->Clone():MinPlan));
			MinimaxType = 1;
		}
		break;
	case 0:
		if ((BestMaxPlan==nullptr && (MaxPlan!=nullptr || f > BestEvaluation)) ||
			(BestMaxPlan!=nullptr && MinPlan!=nullptr && f > BestEvaluation)) {
			BestEvaluation = f;
			BestMaxPlan = (MaxPlan==nullptr ? nullptr:(Clone ? MaxPlan->Clone():MaxPlan));
			BestMinPlan = (MinPlan==nullptr ? nullptr:(Clone ? MinPlan->Clone():MinPlan));
			}
		break;
	case 1:
		if ((BestMinPlan==nullptr && (MinPlan!=nullptr || f < BestEvaluation)) ||
			(BestMinPlan != nullptr && MinPlan!=nullptr && f < BestEvaluation)) {
			BestEvaluation = f;
			BestMaxPlan = (MaxPlan==nullptr ? nullptr:(Clone ? MaxPlan->Clone():MaxPlan));
			BestMinPlan = (MinPlan==nullptr ? nullptr:(Clone ? MinPlan->Clone():MinPlan));
			}
		break;
	default:
		break;
	}

	// alpha-beta pruning
	if (MinimaxType==0)
		Alpha = FMath::Max(Alpha, f);
	else if (MinimaxType==1)
		Beta = FMath::Min(Beta, f);

	return Beta <= Alpha;
}

FString UAHTNAI_AdversarialChoicePoint::ToString()
{
	FString tmp = "( ";
	if (ChoicePointPlayerMax==nullptr) {
		tmp += "null";
	} else {
		if (ChoicePointPlayerMax->GetType()==EMethodType::METHOD_METHOD) {
			tmp+= "choicePointPlayerMax(" + ChoicePointPlayerMax->GetTerm()->ToString() + ")";
		} else if (ChoicePointPlayerMax->GetType()==EMethodType::METHOD_CONDITION) {
			tmp+= "choicePointPlayerMax(" + ChoicePointPlayerMax->GetClause()->ToString() + ")";
		} else {
			tmp+= "choicePointPlayerMax(???)";
		}
	}
	tmp += " , ";
	if (ChoicePointPlayerMin==nullptr) {
		tmp += "null";
	} else {
		if (ChoicePointPlayerMin->GetType()==EMethodType::METHOD_METHOD) {
			tmp+= "choicePointPlayerMin(" + ChoicePointPlayerMin->GetTerm()->ToString() + ")";
		} else if (ChoicePointPlayerMin->GetType()== EMethodType::METHOD_CONDITION) {
			tmp+= "choicePointPlayerMin(" + ChoicePointPlayerMin->GetClause()->ToString() + ")";
		} else {
			tmp+= "choicePointPlayerMin(???)";
		}
	}
	return tmp + ")";
}
