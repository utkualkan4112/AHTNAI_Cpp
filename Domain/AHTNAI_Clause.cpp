// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020


#include "AHTNAI_Clause.h"

#include "AdvancedAI/AHTNAI/Domain/LispParser/AHTNAI_LispElement.h"
#include "AHTNAI_PredefinedPredicates.h"
#include "AHTNAI_Term.h"
#include "BaseClasses/BaseGameState.h"

UAHTNAI_Clause* UAHTNAI_Clause::FromLispElement(UAHTNAI_LispElement* E)
{
	UAHTNAI_LispElement* Head = E->Children[0];
	if(Head->Element == "and")
	{
		UAHTNAI_Clause* c = NewObject<UAHTNAI_Clause>();
		c->Type = EClauseType::CLAUSE_AND;
		TArray<UAHTNAI_Clause*> NewList;
		NewList.Init(nullptr, E->Children.Num() -1);
		c->Clauses = NewList;
		for (int i = 0; i < E->Children.Num() -1; i++) {
			c->Clauses[i] = UAHTNAI_Clause::FromLispElement(E->Children[i + 1]);
		}
		return c;
	}
	else if(Head->Element == "or")
	{
		UAHTNAI_Clause* c = NewObject<UAHTNAI_Clause>();
		c->Type = EClauseType::CLAUSE_OR;
		TArray<UAHTNAI_Clause*> NewList;
		NewList.Init(nullptr, E->Children.Num() -1);
		c->Clauses = NewList;
		for (int i = 0; i < E->Children.Num() -1; i++) {
			c->Clauses[i] = UAHTNAI_Clause::FromLispElement(E->Children[i + 1]);
		}
		return c;
	}
	else if(Head->Element == "not")
	{
		UAHTNAI_Clause* c = NewObject<UAHTNAI_Clause>();
		c->Type = EClauseType::CLAUSE_NOT;
		TArray<UAHTNAI_Clause*> NewList;
		NewList.Init(nullptr, 1);
		c->Clauses = NewList;
		c->Clauses[0] = UAHTNAI_Clause::FromLispElement(E->Children[1]);
		return c;
	}
	else if(Head->Element == "true")
	{
		UAHTNAI_Clause* c = NewObject<UAHTNAI_Clause>();
		c->Type = EClauseType::CLAUSE_TRUE;
		return c;
	}
	else if (Head->Element == "false")
	{
		UAHTNAI_Clause* c = NewObject<UAHTNAI_Clause>();
		c->Type = EClauseType::CLAUSE_FALSE;
		return c;
	}
	else
	{
		UAHTNAI_Clause* c = NewObject<UAHTNAI_Clause>();
		c->Type = EClauseType::CLAUSE_TERM;
		c->Term = UAHTNAI_Term::FromLispElement(E);
		return c;
	}
}

FString UAHTNAI_Clause::ToString()
{
	switch(Type) {
	case EClauseType::CLAUSE_TERM:
		return Term->ToString();
	case EClauseType::CLAUSE_AND:
		{
			FString s;
			s.Append("(and");
			for (UAHTNAI_Clause* Clause : Clauses) {
				s.Append(" ");
				s.Append(Clause->GetName());
			}
			s.Append(")");
			return s;
		}
	case EClauseType::CLAUSE_OR:
		{
			FString s;
			s.Append("(or");
			for (UAHTNAI_Clause* Clause : Clauses) {
				s.Append(" ");
				s.Append(Clause->GetName());
			}
			s.Append(")");
			return s;
		}
	case EClauseType::CLAUSE_NOT:
		{
			FString s;
			s.Append("(not");
			for (UAHTNAI_Clause* Clause : Clauses) {
				s.Append(" ");
				s.Append(Clause->GetName());
			}
			s.Append(")");
			return s;
		}
	case EClauseType::CLAUSE_TRUE:
		return "(true)";
	case EClauseType::CLAUSE_FALSE:
		return "(false)";
	}
	return "";
}

UAHTNAI_Clause* UAHTNAI_Clause::Resolve(TArray<UAHTNAI_Bindings*> l, ABaseGameState* gs)
{
	if (l.Num() == 0) return this;
	UAHTNAI_Clause* c = NewObject<UAHTNAI_Clause>();
	c->Type = Type;
	if (Term!=nullptr) c->Term = Term->Resolve(l, gs);
	if (Clauses.IsValidIndex(0)) {
		TArray<UAHTNAI_Clause*> NewClauses;
		NewClauses.Init(nullptr, Clauses.Num());
		c->Clauses = NewClauses;
		for(int i = 0;i<Clauses.Num();i++) {
			c->Clauses[i] = Clauses[i]->Resolve(l, gs);
		}
	} else {
		c->Clauses.Empty();
	}
	return c;
}

UAHTNAI_Clause* UAHTNAI_Clause::Clone()
{
	UAHTNAI_Clause* c = NewObject<UAHTNAI_Clause>();
	c->Type = Type;
	if (Term!=nullptr) c->Term = Term->Clone();
	if (Clauses.IsValidIndex(0)) {
		TArray<UAHTNAI_Clause*> NewList;
		NewList.Init(nullptr, Clauses.Num());
		c->Clauses = NewList;
		for(int i = 0;i<Clauses.Num();i++) {
			c->Clauses[i] = Clauses[i]->Clone();
		}
	} else {
		c->Clauses.Empty();
	}
	return c;
}

void UAHTNAI_Clause::RenameVariables(int32 RenamingIndex)
{
	if (Term!=nullptr) Term->RenameVariable(RenamingIndex);
	if (Clauses.IsValidIndex(0)) {
		for (UAHTNAI_Clause* Clause : Clauses) {
			Clause->RenameVariables(RenamingIndex);
		}
	}
}

void UAHTNAI_Clause::ApplyBindings(TArray<UAHTNAI_Bindings*> l)
{
	if (l.Num() == 0) return;
	if (Term!=nullptr) Term->ApplyBindings(l);
	if (Clauses.IsValidIndex(0)) {
		for (UAHTNAI_Clause* Clause : Clauses) {
			Clause->ApplyBindings(l);
		}
	}
}

TOptional<TArray<UAHTNAI_Bindings*>> UAHTNAI_Clause::FirstMatch(ABaseGameState* Gs)
{
	if (DEBUG>=1) UE_LOG(LogTemp, Warning, TEXT("Clause.firstMatch"));
	TArray<UAHTNAI_Bindings*> ReturnList;
	TArray<UAHTNAI_Bindings*> l3;
        switch(Type) {
            case EClauseType::CLAUSE_TERM:
                Matches_Left =  UAHTNAI_PredefinedPredicates::AllMatches(Term, Gs);
                if (Matches_Left.Num() == 0)
                {
	                return TOptional<TArray<UAHTNAI_Bindings*>>();;
                }
        	    ReturnList = Matches_Left[0];
        	    Matches_Left.RemoveAt(0);
                return ReturnList;
        	
            case EClauseType::CLAUSE_AND:
                {
                    Matches_Current = 0;
                    Matches_Previous = -1;
            		Matches_Resolved.Init(nullptr, Clauses.Num());
                    while(true) {
                        if (DEBUG>=1) UE_LOG(LogTemp, Warning, TEXT("Clause.firstMatch(AND): current = %i (previous: %i)"), Matches_Current, Matches_Previous);
                        UAHTNAI_Clause* c = Clauses[Matches_Current];
                        TArray<UAHTNAI_Bindings*> l2;
                        if (Matches_Previous < Matches_Current) {
                            // trying a clause for first time with the new bindings:
                            Matches_Resolved[Matches_Current] = c;
                            if (Matches_L.Num() != 0) Matches_Resolved[Matches_Current] = c->Resolve(Matches_L, Gs);
                            if (DEBUG>=1) UE_LOG(LogTemp, Warning, TEXT("Clause.firstMatch(AND): resolved clause = %s"), *Matches_Resolved[Matches_Current]->ToString());
                            l2 = Matches_Resolved[Matches_Current]->FirstMatch(Gs).GetValue();
//                            if (l2==null) System.out.println("Failed: " + matches_resolved[matches_current]);
                            if (DEBUG>=1)
                            {
                            	FMemoryToStringContext OutString;
                            	FPlatformTypeLayoutParameters LayoutParameters;
                            	l2.ToString(LayoutParameters, OutString);
                            	UE_LOG(LogTemp, Warning, TEXT("Clause.firstMatch(AND): match = %s"), OutString.String->ToString());
                            }
                        } else {
                            // backtracking:
                            l2 = Matches_Resolved[Matches_Current]->NextMatch(Gs).GetValue();
                        	FMemoryToStringContext OutString;
                        	FPlatformTypeLayoutParameters LayoutParameters;
                        	l2.ToString(LayoutParameters, OutString);
                            if (DEBUG>=1) UE_LOG(LogTemp, Warning, TEXT("Clause.firstMatch(AND): match = %s"), OutString.String->ToString());
                        }
                        Matches_Previous = Matches_Current;
                        if (l2.Num() == 0) {
                            // backtracking:
                            Matches_Current--;
                            if (Matches_Current<0) {
                                Matches_Left.Empty();
                                return TOptional<TArray<UAHTNAI_Bindings*>>();;
                            }
                            // undo bindings:
                            while( Matches_L.Num() > Matches_Trail[Matches_Current]) Matches_L.RemoveAt(Matches_L.Num()-1);
                        } else {
                            // success:
                            Matches_Trail[Matches_Current] = Matches_L.Num();
                            Matches_L.Append(l2); 
                            Matches_Current++;
                            if (Matches_Current>=Clauses.Num()) {
                                return Matches_L;
                            }
                        }
                    } 
                }
            case EClauseType::CLAUSE_OR:
                {
                    Matches_Current = 0;
                    for(;Matches_Current<Clauses.Num();Matches_Current++) {
                        TArray<UAHTNAI_Bindings*> l = Clauses[Matches_Current]->FirstMatch(Gs).GetValue();
                        if (l.Num() == 0) return l;
                    }
                    Matches_Left.Empty();
            		return TOptional<TArray<UAHTNAI_Bindings*>>();
                }
            case EClauseType::CLAUSE_NOT:
            	l3 = Clauses[0]->FirstMatch(Gs).GetValue();
                if (l3.Num() == 0) {
                    return ReturnList;
                } else {
                    return TOptional<TArray<UAHTNAI_Bindings*>>();
                }
            case EClauseType::CLAUSE_TRUE:
                return ReturnList;
            case EClauseType::CLAUSE_FALSE:
                return TOptional<TArray<UAHTNAI_Bindings*>>();
                
        }
        
        return TOptional<TArray<UAHTNAI_Bindings*>>();
}

TOptional<TArray<UAHTNAI_Bindings*>> UAHTNAI_Clause::NextMatch(ABaseGameState* Gs)
{
	if (DEBUG>=1) UE_LOG(LogTemp, Warning, TEXT("Clause.nextMatch"));
        if (Matches_Left.Num() == 0) return FirstMatch(Gs);
	TArray<UAHTNAI_Bindings*> ReturnList;
        switch(Type) {
            case EClauseType::CLAUSE_TERM:
                if (Matches_Left.Num() == 0) {
                    Matches_Left.Empty();
                    return TOptional<TArray<UAHTNAI_Bindings*>>();
                }
        	    ReturnList = Matches_Left[0];
        	    Matches_Left.RemoveAt(0);
                return ReturnList;
            case EClauseType::CLAUSE_AND:
                {
                    if (Matches_Current>=Clauses.Num()) {
                        Matches_Current--;
                        while(Matches_L.Num() > Matches_Trail[Matches_Current]) Matches_L.RemoveAt(Matches_L.Num()-1);
                    }
                    while(true) {
                        UAHTNAI_Clause* c = Clauses[Matches_Current];
                    	TArray<UAHTNAI_Bindings*> l2;
                        if (Matches_Previous < Matches_Current) {
                            // trying a clause for first time with the new bindings:
                            Matches_Resolved[Matches_Current] = c;
                            if (Matches_L.Num() != 0) Matches_Resolved[Matches_Current] = c->Resolve(Matches_L, Gs);
                            l2 = Matches_Resolved[Matches_Current]->FirstMatch(Gs).GetValue();
                        } else {
                            // backtracking:
                            l2 = Matches_Resolved[Matches_Current]->NextMatch(Gs).GetValue();
                        }
                        Matches_Previous = Matches_Current;
                        if (l2.Num() == 0) {
                            // backtracking:
                            Matches_Current--;
                            if (Matches_Current<0) {
                                Matches_Left.Empty();                  
                                return TOptional<TArray<UAHTNAI_Bindings*>>();
                            }
                            // undo bindings:
                            while(Matches_L.Num()>Matches_Trail[Matches_Current]) Matches_L.RemoveAt(Matches_L.Num()-1);
                        } else {
                            // success:
                            Matches_Trail[Matches_Current] = Matches_L.Num();
                            Matches_L.Append(l2);
                            Matches_Current++;
                            if (Matches_Current>=Clauses.Num()) {
                                return Matches_L;
                            }
                        }
                    } 
                }
            case EClauseType::CLAUSE_OR:
                {
                    for(;Matches_Current<Clauses.Num();Matches_Current++) {
                        TArray<UAHTNAI_Bindings*> l = Clauses[Matches_Current]->NextMatch(Gs).GetValue();
                        if (l.Num() != 0) return l;
                    }
                    Matches_Left.Empty();                  
                    return TOptional<TArray<UAHTNAI_Bindings*>>();
                }
            case EClauseType::CLAUSE_NOT:
                Matches_Left.Empty();
                return TOptional<TArray<UAHTNAI_Bindings*>>();
            case EClauseType::CLAUSE_TRUE:
                Matches_Left.Empty();
                return TOptional<TArray<UAHTNAI_Bindings*>>();
            case EClauseType::CLAUSE_FALSE:
                Matches_Left.Empty();
                return TOptional<TArray<UAHTNAI_Bindings*>>();
        }
        
        return TOptional<TArray<UAHTNAI_Bindings*>>();    
}

void UAHTNAI_Clause::CountVariableAppearances(TMap<UAHTNAI_Symboll*, int32>& Appearances)
{
	if (Term != nullptr) Term->CountVariableAppearances(Appearances);
	if (Clauses.Num() != 0) {
		for(UAHTNAI_Clause* c: Clauses) {
			c->CountVariableAppearances(Appearances);
		}
	}
}

void UAHTNAI_Clause::ReplaceSingletonsByWildcards(TArray<UAHTNAI_Symboll*>& Singletons)
{
	if (Term != nullptr ) Term->ReplaceSingeltonsByWildcards(Singletons);
	if (Clauses.Num() != 0) {
		for(UAHTNAI_Clause* c: Clauses) {
			c->ReplaceSingletonsByWildcards(Singletons);
		}
	}
}
