// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020


#include "AHTNAI_DomainDefinition.h"

#include "AHTNAI_HTNMethod.h"
#include "AHTNAI_HTNOperator.h"
#include "AdvancedAI/AHTNAI/Domain/LispParser/AHTNAI_LispElement.h"
#include "AdvancedAI/AHTNAI/Domain/LispParser/AHTNAI_LispParser.h"
#include "AHTNAI_Symboll.h"

int32 UAHTNAI_DomainDefinition::DEBUG = 0;

void UAHTNAI_DomainDefinition::AddMethod(UAHTNAI_HTNMethod* M)
{
	// Add the method to the main Methods array.
	Methods.Add(M);

	// Get the goal.
	UAHTNAI_Symboll* Goal = M->GetHead()->Functor;

	// Try to find the TArray associated with this goal.
	TArray<UAHTNAI_HTNMethod*>* MethodsForGoal = MethodsPerGoal.Find(Goal);

	// If the TArray doesn't exist for this goal, add it.
	if (!MethodsForGoal)
	{
		MethodsForGoal = &MethodsPerGoal.Add(Goal, TArray<UAHTNAI_HTNMethod*>());
	}

	// Now, simply add the method to the TArray.
	MethodsForGoal->Add(M);
}

UAHTNAI_DomainDefinition* UAHTNAI_DomainDefinition::FromLispFile(FString FileName)
{
	
	TArray<UAHTNAI_LispElement*> l = UAHTNAI_LispParser::ParseLispFile(FileName);
	if (DEBUG>=1) {
		for(UAHTNAI_LispElement* e:l)
			UE_LOG(LogTemp, Warning, TEXT("%s"), *e->ToString());
	}
        
	if (l.Num() == 0) return nullptr;        
	return FromLispElement(l[0]);
}

UAHTNAI_DomainDefinition* UAHTNAI_DomainDefinition::FromLispElement(UAHTNAI_LispElement* E)
{
	
	UAHTNAI_DomainDefinition* Dd = NewObject<UAHTNAI_DomainDefinition>();

        // verify it's a domain definition:
        if (E->Children.Num()!=3) UE_LOG(LogTemp, Warning, TEXT("Lisp domain definition does not have 3 arguments"));
        UAHTNAI_LispElement* Defdomain = E->Children[0];
        if (Defdomain->Element== "" || !Defdomain->Element.Equals("defdomain")) UE_LOG(LogTemp, Warning, TEXT("Lisp domain definition does not start by 'defdomain'"));
        UAHTNAI_LispElement* Name_e = E->Children[1];
        if (Name_e->Element.IsEmpty()) UE_LOG(LogTemp, Warning, TEXT("second parameter of defdomain is not a domain name"));
        Dd->Name = Name_e->Element;
        
        UAHTNAI_LispElement* Rest_e = E->Children[2];
        if (Rest_e->Children.Num() == 0) UE_LOG(LogTemp, Warning, TEXT("third parameter of defdomain is not a list"));
        
        for(UAHTNAI_LispElement* Def_e:Rest_e->Children) {
            // load operators and methods:
            if (Def_e->Children.Num()!= 0 && Def_e->Children.Num()>0) {
                UAHTNAI_LispElement* head = Def_e->Children[0];
                if (!head->Element.IsEmpty() && head->Element.Equals(":operator")) {
                    UAHTNAI_HTNOperator* Op = UAHTNAI_HTNOperator::FromLispElement(Def_e);
                    Dd->Operators.Add(Op);
                } else if (!head->Element.IsEmpty() && head->Element.Equals(":method")) {
                    UAHTNAI_HTNMethod* M = UAHTNAI_HTNMethod::FromLispElement(Def_e);
                    Dd->AddMethod(M);
                } else {
                	UE_LOG(LogTemp, Warning, TEXT("Element in domain definition is not an operator nor method"));
                	return nullptr;
                }
            } else {
            	UE_LOG(LogTemp, Warning, TEXT("Element in domain definition is not an operator nor method"));
            }
        }
        
        // remove singletons:
        for(UAHTNAI_HTNMethod* M:Dd->GetMethods()) {
            if (DEBUG>=1) {
                TArray<UAHTNAI_Symboll*> l = M->FindSingletons();
                if (!l.Num() == 0)
                {
                	FString ResultString = "Singletons in '" + M->GetName() + "'";
                	for(UAHTNAI_Symboll* S:l)
                	{
                		ResultString += S->ToString();
                	}
	                UE_LOG(LogTemp, Warning, TEXT("%s"), *ResultString);
                }
            }
            M->ReplaceSingletonsByWildcards();
        }
        
        return Dd;
}

FString UAHTNAI_DomainDefinition::ToString()
{
	
	FString S;
        
	S.Append("Domain: ").Append(Name).Append("\n").Append("Operators:\n");
	for(UAHTNAI_HTNOperator* Op:Operators) {
		S.Append("  ").Append(Op->ToString()).Append("\n");
	}
	S.Append("Methods:\n");
	for(UAHTNAI_HTNMethod* M:Methods) {
		S.Append("  ").Append(M->ToString()).Append("\n");
	}
        
	return S;
}

TArray<UAHTNAI_HTNMethod*> UAHTNAI_DomainDefinition::GetMethodsForGoal(UAHTNAI_Symboll* Functor)
{
	return *MethodsPerGoal.Find(Functor);
}
