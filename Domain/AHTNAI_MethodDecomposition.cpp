// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020


#include "AHTNAI_MethodDecomposition.h"


#include "AdvancedAI/AHTNAI/Planner/AHTNAI_AdversarialChoicePoint.h"
#include "AHTNAI_Clause.h"
#include "AHTNAI_HTNMethod.h"
#include "AdvancedAI/AHTNAI/Domain/LispParser/AHTNAI_LispElement.h"


void UAHTNAI_MethodDecomposition::Initialize(UAHTNAI_HTNMethod* M, UAHTNAI_Term* T)
{
	if(M && T)
	{
		Type = EMethodType::METHOD_METHOD;
		Method = M;
		Term = T;
	}
	if(T && !M)
	{
		Type = EMethodType::METHOD_OPERATOR;
		Term = T;
	}
}

UAHTNAI_MethodDecomposition* UAHTNAI_MethodDecomposition::FromLispElement(UAHTNAI_LispElement* E)
{
	UAHTNAI_LispElement* Head = E->Children[0];
	if(Head->Element == ":condition")
	{
		UAHTNAI_MethodDecomposition* D = NewObject<UAHTNAI_MethodDecomposition>();
		D->Type = EMethodType::METHOD_CONDITION;
		D->Clause = UAHTNAI_Clause::FromLispElement(E->Children[1]);
		return D;
	}
	else if(Head->Element == ":!condition")
	{
		UAHTNAI_MethodDecomposition* D = NewObject<UAHTNAI_MethodDecomposition>();
		D->Type = EMethodType::METHOD_NON_BRANCHING_CONDITION;
		D->Clause = UAHTNAI_Clause::FromLispElement(E->Children[1]);
		return D;
	}
	else if(Head->Element == ":operator")
	{
		UAHTNAI_MethodDecomposition* D = NewObject<UAHTNAI_MethodDecomposition>();
		D->Type = EMethodType::METHOD_OPERATOR;
		D->Clause = UAHTNAI_Clause::FromLispElement(E->Children[1]);
		return D;
	}
	else if(Head->Element == ":method")
	{
		UAHTNAI_MethodDecomposition* D = NewObject<UAHTNAI_MethodDecomposition>();
		D->Type = EMethodType::METHOD_METHOD;
		D->Clause = UAHTNAI_Clause::FromLispElement(E->Children[1]);
		return D;
	}
	else if(Head->Element == ":sequence")
	{
		UAHTNAI_MethodDecomposition* D = NewObject<UAHTNAI_MethodDecomposition>();
		D->Type = EMethodType::METHOD_SEQUENCE;
		TArray<UAHTNAI_MethodDecomposition*> NewList;
		NewList.Init(nullptr, E->Children.Num() -1);
		D->Subelements = NewList;
		for (int i = 0; i < E->Children.Num() - 1; i++) {
			D->Subelements[i] = UAHTNAI_MethodDecomposition::FromLispElement(E->Children[i+1]);
		}
		return D;
	}
	else if(Head->Element == ":parallel")
	{
		UAHTNAI_MethodDecomposition* D = NewObject<UAHTNAI_MethodDecomposition>();
		D->Type = EMethodType::METHOD_PARALLEL;
		TArray<UAHTNAI_MethodDecomposition*> NewList;
		NewList.Init(nullptr, E->Children.Num() -1);
		D->Subelements = NewList;
		for (int i = 0; i < E->Children.Num() - 1; i++) {
			D->Subelements[i] = UAHTNAI_MethodDecomposition::FromLispElement(E->Children[i+1]);
		}
		return D;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("unrecognized method decomposition!: %s"), *Head->Element);
		return nullptr;
	}
}

FString UAHTNAI_MethodDecomposition::ToString()
{
	switch(Type) {
	case EMethodType::METHOD_CONDITION:
		return "(:condition " + Clause->ToString() + ")";
	case EMethodType::METHOD_NON_BRANCHING_CONDITION:
		return "(:!condition " + Clause->ToString() + ")";
	case EMethodType::METHOD_OPERATOR:
		if (UpdatedTerm!=nullptr) {
			return "(:operator " + UpdatedTerm->ToString() + ")";
		} else {
			return "(:operator " + Term->ToString() + ")";
		}
	case EMethodType::METHOD_METHOD:
		if (Method==nullptr) {
			return "(:method " + Term->ToString() + ")";                    
		} else {
			return "(" + Method->ToString() +")";
		}
                
	case EMethodType::METHOD_SEQUENCE:
		{
			FString S;
			S.Append("(:sequence");
			for (UAHTNAI_MethodDecomposition* Subelement : Subelements) {
				S.Append(" ");
				S.Append(Subelement->ToString());
			}
			S.Append(")");
			return S;
		}
	case EMethodType::METHOD_PARALLEL:
		{
			FString S;
			S.Append("(:parallel");
			for (UAHTNAI_MethodDecomposition* Subelement : Subelements) {
				S.Append(" ");
				S.Append(Subelement->ToString());
			}
			S.Append(")");
			return S;
		}
	}
	return "";
}

void UAHTNAI_MethodDecomposition::PrintDetailed(int32 Tabs)
{
	for(int j = 0;j<Tabs;j++) UE_LOG(LogTemp, Warning, TEXT(" "));
        switch(Type) {
            case EMethodType::METHOD_CONDITION:
            	UE_LOG(LogTemp, Warning, TEXT("%i - %i - (:condition %s) "), PointerHash(this), ExecutionState, *Clause->ToString());
                break;
            case EMethodType::METHOD_NON_BRANCHING_CONDITION:
            	UE_LOG(LogTemp, Warning, TEXT("%i - %i - (:!condition %s) "), PointerHash(this), ExecutionState, *Clause->ToString());
                break;
            case EMethodType::METHOD_OPERATOR:
                if (UpdatedTerm!=nullptr) {
                	UE_LOG(LogTemp, Warning, TEXT("%i - %i - (:operator %s) "), PointerHash(this), ExecutionState, *UpdatedTerm->ToString());
                } else {
                	UE_LOG(LogTemp, Warning, TEXT("%i - %i - (:operator %s) "), PointerHash(this), ExecutionState, *Term->ToString());
                }
                break;
            case EMethodType::METHOD_METHOD:
                if (Method!=nullptr) {
                	UE_LOG(LogTemp, Warning, TEXT("%i - %i - (:method %s) "), PointerHash(this), ExecutionState, *Method->GetHead()->ToString());
                    for(int j = 0;j<Tabs;j++) UE_LOG(LogTemp, Warning, TEXT(" "));
                	UE_LOG(LogTemp, Warning, TEXT("Decomposition:"));
                    Method->GetDecomposition()->PrintDetailed(Tabs+1);
                } else {
                	UE_LOG(LogTemp, Warning, TEXT("%i - %i - (:method %s) "), PointerHash(this), ExecutionState, *Term->ToString());
                }
                break;
            case EMethodType::METHOD_SEQUENCE:
                {
            		UE_LOG(LogTemp, Warning, TEXT("%i - %i - (:sequence"), PointerHash(this), ExecutionState);
                    for (UAHTNAI_MethodDecomposition* Subelement : Subelements) {
                        Subelement->PrintDetailed(Tabs + 1);
                    }
                    for(int j = 0;j<Tabs;j++) UE_LOG(LogTemp, Warning, TEXT(" "));
            		UE_LOG(LogTemp, Warning, TEXT(")"));
                }
                break;
            case EMethodType::METHOD_PARALLEL:
                {
            		UE_LOG(LogTemp, Warning, TEXT("%i - %i - (:parallel"), PointerHash(this), ExecutionState);
                    for (UAHTNAI_MethodDecomposition* Subelement : Subelements) {
                        Subelement->PrintDetailed(Tabs + 1);
                    }
                    for(int j = 0;j<Tabs;j++) UE_LOG(LogTemp, Warning, TEXT(" "));
            		UE_LOG(LogTemp, Warning, TEXT(")"));
                }
                break;
        }        
}

TArray<UAHTNAI_MethodDecomposition*> UAHTNAI_MethodDecomposition::GetLeaves()
{
	TArray<UAHTNAI_MethodDecomposition*> L;
	if (Subelements.Num() == 0) {
		if (Method!=nullptr) {
			L.Append(Method->GetDecomposition()->GetLeaves());
		} else {
			L.Add(this);
		}
	} else {
		for(UAHTNAI_MethodDecomposition* Md:Subelements) {
			L.Append(Md->GetLeaves());
		}
	}
	return L;
}

UAHTNAI_MethodDecomposition* UAHTNAI_MethodDecomposition::Clone()
{
	UAHTNAI_MethodDecomposition* C = NewObject<UAHTNAI_MethodDecomposition>();
	C->Type = Type;
	if (Clause!=nullptr) C->Clause = Clause->Clone(); 
	if (Term!=nullptr) C->Term = Term->Clone();
	if (UpdatedTerm!=nullptr) C->UpdatedTerm = UpdatedTerm->Clone();
	C->UpdatedTermCycle = UpdatedTermCycle;
	C->ExecutionState = ExecutionState;
	C->OperatorExecutingState = OperatorExecutingState;
	if (Subelements.Num() != 0) {
		TArray<UAHTNAI_MethodDecomposition*> List;
		List.Init(nullptr, Subelements.Num());
		C->Subelements = List;
		for(int i = 0;i<Subelements.Num();i++) {
			C->Subelements[i] = Subelements[i]->Clone();
		}
	}
	if (Method!=nullptr) {
		C->Method = Method->Clone();
	}
        
	return C;
}

UAHTNAI_MethodDecomposition* UAHTNAI_MethodDecomposition::CloneTrackingDescendants(
	TArray<UAHTNAI_MethodDecomposition*> DescendantsToTrack, TArray<UAHTNAI_MethodDecomposition*> NewDescendants)
{
	UAHTNAI_MethodDecomposition* c = NewObject<UAHTNAI_MethodDecomposition>();
	for(int i = 0;i<DescendantsToTrack.Num();i++) {
		if (DescendantsToTrack[i]==this) NewDescendants[i] = c;
	}
	c->Type = Type;
	if (Clause!=nullptr) c->Clause = Clause->Clone(); 
	if (Term!=nullptr) c->Term = Term->Clone();
	if (UpdatedTerm!=nullptr) c->UpdatedTerm = UpdatedTerm->Clone();
	c->UpdatedTermCycle = UpdatedTermCycle;
	c->ExecutionState = ExecutionState;
	c->OperatorExecutingState = OperatorExecutingState;
	if (Subelements.Num()!=0) {
		TArray<UAHTNAI_MethodDecomposition*> NewList;
		NewList.Init(nullptr, Subelements.Num());
		c->Subelements = NewList;
		for(int i = 0;i<Subelements.Num();i++) {
			c->Subelements[i] = Subelements[i]->CloneTrackingDescendants(DescendantsToTrack, NewDescendants);
		}
	}
	if (Method!=nullptr) {
		c->Method = Method->CloneTrackingDescendants(DescendantsToTrack, NewDescendants);
	}
        
	return c;
}

void UAHTNAI_MethodDecomposition::RenameVariables(int32 RenamingIndex)
{
	if (Clause!=nullptr) Clause->RenameVariables(RenamingIndex);
	if (Term!=nullptr) Term->RenameVariable(RenamingIndex);
	if (UpdatedTerm!=nullptr) UpdatedTerm->RenameVariable(RenamingIndex);
	if (Subelements.Num()!=0) {
		for (UAHTNAI_MethodDecomposition* Subelement : Subelements) {
			Subelement->RenameVariables(RenamingIndex);
		}
	}
	if (Method!=nullptr) Method->RenameVariables(RenamingIndex);
}

void UAHTNAI_MethodDecomposition::ApplyBindings(TArray<UAHTNAI_Bindings*> L)
{
	if (Clause!=nullptr) Clause->ApplyBindings(L);
	if (Term!=nullptr) Term->ApplyBindings(L);
	if (UpdatedTerm!=nullptr) UpdatedTerm->ApplyBindings(L);
	if (Subelements.Num()!=0) {
		for (UAHTNAI_MethodDecomposition* Subelement : Subelements) {
			Subelement->ApplyBindings(L);
		}
	}
	if (Method!=nullptr) Method->ApplyBindings(L);
}

void UAHTNAI_MethodDecomposition::ExecutionReset()
{
	ExecutionState = EExecutionType::EXECUTION_SUCCESS;
	OperatorExecutingState = EExecutionType::EXECUTION_SUCCESS;
	OperatorsBeingExecuted.Empty();
        
	if (Subelements.Num()!=0) {
		for (UAHTNAI_MethodDecomposition* Subelement : Subelements) {
			Subelement->ExecutionReset();
		}
	}
	if (Method!=nullptr) {
		Method->GetDecomposition()->ExecutionReset();
	}
}

EExecutionType UAHTNAI_MethodDecomposition::ExecutionCycle(ABaseGameState* Gs, TArray<UAHTNAI_MethodDecomposition*> Actions,
                                                  TArray<UAHTNAI_MethodDecomposition*> ChoicePoints)
{
	switch(Type) {
            case EMethodType::METHOD_CONDITION:
            case EMethodType::METHOD_NON_BRANCHING_CONDITION:
                if (ExecutionState==EExecutionType::EXECUTION_SUCCESS) {
                    ChoicePoints.Add(this);
                    return EExecutionType::EXECUTION_CHOICE_POINT;
                } else if (ExecutionState==EExecutionType::EXECUTION_FAILURE) {
                    return EExecutionType::EXECUTION_SUCCESS;
                } else {
                    return EExecutionType::EXECUTION_FAILURE;
                }
            case EMethodType::METHOD_OPERATOR:
                if (ExecutionState==EExecutionType::EXECUTION_SUCCESS) {
                    // we rely on external code to set the executionSTate to 1 and 
                    // to fill in the waitingForUnit
                    Actions.Add(this);
                    return EExecutionType::EXECUTION_ACTION_ISSUE;
                } else if (ExecutionState==EExecutionType::EXECUTION_FAILURE) {
                    // test whether the operator is complete (this will be updated externally)
                    return EExecutionType::EXECUTION_WAITING_FOR_ACTION;
                } else {
                    return EExecutionType::EXECUTION_SUCCESS;
                }
            case EMethodType::METHOD_METHOD:
                if (Method==nullptr) {
                    // return choice point
                    ChoicePoints.Add(this);
                    return EExecutionType::EXECUTION_CHOICE_POINT;
                } else {
                    // recursive call:
                    return Method->ExecutionCycle(Gs, Actions, ChoicePoints);
                }
            case EMethodType::METHOD_SEQUENCE:
                do {
                	
                    if (static_cast<int32>(ExecutionState) >=Subelements.Num()) 
                        return EExecutionType::EXECUTION_SUCCESS;
                    EExecutionType tmp = Subelements[static_cast<int32>(ExecutionState)]->ExecutionCycle(Gs, Actions, ChoicePoints);
                    if (tmp==EExecutionType::EXECUTION_SUCCESS) {
                    	int32 Tmp_i = static_cast<int32>(ExecutionState);
                    	Tmp_i++;
                        ExecutionState = static_cast<EExecutionType>(Tmp_i);
                    } else {
                        return tmp;
                    }
                }while(true);
            case EMethodType::METHOD_PARALLEL:
                {
                    bool AllSuccess = true;
                    bool AnyActionIssue = false;
                    for (UAHTNAI_MethodDecomposition* Subelement : Subelements) {
                        EExecutionType tmp = Subelement->ExecutionCycle(Gs, Actions, ChoicePoints);
                        if (tmp == EExecutionType::EXECUTION_ACTION_ISSUE) AnyActionIssue = true;
                        if (tmp == EExecutionType::EXECUTION_CHOICE_POINT ||
                                tmp == EExecutionType::EXECUTION_FAILURE) return tmp;
                        if (tmp != EExecutionType::EXECUTION_SUCCESS) AllSuccess = false;
                    }
                    if (AllSuccess) return EExecutionType::EXECUTION_SUCCESS;
                    if (AnyActionIssue) return EExecutionType::EXECUTION_ACTION_ISSUE;
                    return EExecutionType::EXECUTION_WAITING_FOR_ACTION;
                }
        }        
        return EExecutionType::EXECUTION_SUCCESS;
}

EExecutionType UAHTNAI_MethodDecomposition::ExecutionCycle(ABaseGameState* Gs, TArray<UAHTNAI_MethodDecomposition*> Actions,
	TArray<UAHTNAI_MethodDecomposition*> ChoicePoints, UAHTNAI_AdversarialChoicePoint* Previous_cp)
{
	switch(Type) {
            case EMethodType::METHOD_CONDITION:
            case EMethodType::METHOD_NON_BRANCHING_CONDITION:
                if (ExecutionState==EExecutionType::EXECUTION_SUCCESS) {
                    ChoicePoints.Add(this);
                    return EExecutionType::EXECUTION_CHOICE_POINT;
                } else if (ExecutionState==EExecutionType::EXECUTION_FAILURE) {
                    return EExecutionType::EXECUTION_SUCCESS;
                } else {
                    return EExecutionType::EXECUTION_FAILURE;
                }
            case EMethodType::METHOD_OPERATOR:
                if (ExecutionState==EExecutionType::EXECUTION_SUCCESS) {
                    Previous_cp->CaptureExecutionStateNonRecursive(this);
                    // we rely on external code to set the executionSTate to 1 and 
                    // to fill in the waitingForUnit
                    Actions.Add(this);
                    return EExecutionType::EXECUTION_ACTION_ISSUE;
                } else if (ExecutionState==EExecutionType::EXECUTION_FAILURE) {
                    // test whether the operator is complete (this will be updated externally)
                    return EExecutionType::EXECUTION_WAITING_FOR_ACTION;
                } else {
                    return EExecutionType::EXECUTION_SUCCESS;
                }
            case EMethodType::METHOD_METHOD:
                if (Method==nullptr) {
                    // return choice point
                    ChoicePoints.Add(this);
                    return EExecutionType::EXECUTION_CHOICE_POINT;
                } else {
                    // recursive call:
                    return Method->ExecutionCycle(Gs, Actions, ChoicePoints, Previous_cp);
                }
            case EMethodType::METHOD_SEQUENCE:
                do {
                    if (static_cast<int32>(ExecutionState) >= Subelements.Num()) 
                        return EExecutionType::EXECUTION_SUCCESS;
                    EExecutionType tmp = Subelements[static_cast<int32>(ExecutionState)]->ExecutionCycle(Gs, Actions, ChoicePoints, Previous_cp);
                    if (tmp==EExecutionType::EXECUTION_SUCCESS) {
                        Previous_cp->CaptureExecutionStateNonRecursive(this);
                    	int32 Tmp_i = static_cast<int32>(ExecutionState);
                    	Tmp_i++;
                        ExecutionState = static_cast<EExecutionType>(Tmp_i);
                    } else {
                        return tmp;
                    }
                }while(true);
            case EMethodType::METHOD_PARALLEL:
                {
                    bool AllSuccess = true;
                    bool AnyActionIssue = false;
                    for (UAHTNAI_MethodDecomposition* Subelement : Subelements) {
                        EExecutionType tmp = Subelement->ExecutionCycle(Gs, Actions, ChoicePoints, Previous_cp);
                        if (tmp == EExecutionType::EXECUTION_ACTION_ISSUE) AnyActionIssue = true;
                        if (tmp == EExecutionType::EXECUTION_CHOICE_POINT ||
                                tmp == EExecutionType::EXECUTION_FAILURE) return tmp;
                        if (tmp != EExecutionType::EXECUTION_SUCCESS) AllSuccess = false;
                    }
                    if (AllSuccess) return EExecutionType::EXECUTION_SUCCESS;
                    if (AnyActionIssue) return EExecutionType::EXECUTION_ACTION_ISSUE;
                    return EExecutionType::EXECUTION_WAITING_FOR_ACTION;
                }
        }        
        return EExecutionType::EXECUTION_SUCCESS;
}

TArray<TPair<int32, TArray<UAHTNAI_Term*>>> UAHTNAI_MethodDecomposition::ConvertToOperatorList()
{
	TArray<TPair<int32,TArray<UAHTNAI_Term*>>> l;
	ConvertToOperatorList(l);
	// sort the list:
	l.Sort([](const TPair<int32, TArray<UAHTNAI_Term*>>& A, const TPair<int32, TArray<UAHTNAI_Term*>>& B) {
	return A.Key < B.Key; 
    });
	return l;
}

void UAHTNAI_MethodDecomposition::ConvertToOperatorList(TArray<TPair<int32, TArray<UAHTNAI_Term*>>>& L)
{
	switch(Type) {
	case EMethodType::METHOD_CONDITION:
		return;
	case EMethodType::METHOD_NON_BRANCHING_CONDITION:
		return;
	case EMethodType::METHOD_OPERATOR:
		if (UpdatedTerm!=nullptr) {
			if (L.Num() == 0) {
				TPair<int32,TArray<UAHTNAI_Term*>> tmp;
				tmp.Value.Add(UpdatedTerm);
				L.Add(tmp);
			} else {
				TPair<int32,TArray<UAHTNAI_Term*>> tmp = L[L.Num() -1];
				if (tmp.Key == UpdatedTermCycle) {
					tmp.Value.Add(UpdatedTerm);
				} else {
					tmp.Key = UpdatedTermCycle;
					tmp.Value.Add(UpdatedTerm);
					L.Add(tmp);
				}
			}
		}
		break;
	case EMethodType::METHOD_METHOD:
		if (Method!=nullptr) Method->GetDecomposition()->ConvertToOperatorList(L);
		break;
	case EMethodType::METHOD_SEQUENCE:
	case EMethodType::METHOD_PARALLEL:
		if (Subelements.Num()!=0) {
			for (UAHTNAI_MethodDecomposition* Subelement : Subelements) {
				Subelement->ConvertToOperatorList(L);
			}
		}
		break;
	}
}

void UAHTNAI_MethodDecomposition::CountVariableAppearances(TMap<UAHTNAI_Symboll*, int32>& Appearances)
{
	if (Clause!=nullptr) Clause->CountVariableAppearances(Appearances);
	if (Term!=nullptr) Term->CountVariableAppearances(Appearances);
	if (Subelements.Num()!=0) {
		for(UAHTNAI_MethodDecomposition* md:Subelements) {
			md->CountVariableAppearances(Appearances);
		}
	}
}

void UAHTNAI_MethodDecomposition::ReplaceSingletonsByWildcards(TArray<UAHTNAI_Symboll*>& Singletons)
{
	if (Clause!=nullptr) Clause->ReplaceSingletonsByWildcards(Singletons);
	if (Term!=nullptr) Term->ReplaceSingeltonsByWildcards(Singletons);
	if (Subelements.Num()!=0) {
		for(UAHTNAI_MethodDecomposition* md:Subelements) {
			md->ReplaceSingletonsByWildcards(Singletons);
		}
	}
}
