// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020


#include "AHTNAI_Term.h"

#include "AHTNAI_Function.h"
#include "AHTNAI_Symboll.h"
#include "BaseClasses/BaseGameState.h"
#include "AHTNAI_IntegerConstant.h"
#include "AdvancedAI/AHTNAI/Domain/LispParser/AHTNAI_LispElement.h"
#include "AdvancedAI/AHTNAI/Domain/LispParser/AHTNAI_LispParser.h"
#include "AHTNAI_Variable.h"

UAHTNAI_Term::UAHTNAI_Term()
{
	Functor = NewObject<UAHTNAI_Symboll>();
}

void UAHTNAI_Term::Initialize(UAHTNAI_Symboll* f, UAHTNAI_Parameter* p1, UAHTNAI_Parameter* p2, TArray<UAHTNAI_Parameter*> Plist)
{
	Functor = nullptr;
	Parameters.Empty();
	if(f)
	{
		Functor = f;
	}
	if(Parameters.IsValidIndex(0))
	{
		Parameters = Plist;
	}
	if(p1)
	{
		Parameters[0] = p1;
	}
	if(p2)
	{
		Parameters[1] = p2;
	}
}

UAHTNAI_Term* UAHTNAI_Term::FromString(const FString& S)
{
	TArray<UAHTNAI_LispElement*> A = UAHTNAI_LispParser::ParseString(S);
	if(A.Num() == 0)
	{
		return nullptr;
	}
	return FromLispElement(A[0]);
}

UAHTNAI_Term* UAHTNAI_Term::FromLispElement(UAHTNAI_LispElement* e)
{
	UAHTNAI_Term* t = NewObject<UAHTNAI_Term>();
	UAHTNAI_Symboll* sym = NewObject<UAHTNAI_Symboll>();
	sym->InitializeWithString(e->Children[0]->Element);
	t->Functor = sym;
	int32 l = e->Children.Num() - 1;
	if (l==0) {
		t->Parameters.Empty();
	} else {
		t->Parameters.Init(nullptr, l);
		for(int32 i = 0;i<l;i++) {
			// Determine whether it's a constant, variable, or function:
			UAHTNAI_LispElement* p_e = e->Children[i+1];
			if (p_e->Children.IsValidIndex(0)) {
				t->Parameters[i] = UAHTNAI_Function::FromLispElement(p_e);
			} else {
				FString v = p_e->Element;
				if (v.StartsWith("?")) {
					UAHTNAI_Variable* NewVariable = NewObject<UAHTNAI_Variable>();
					NewVariable->InitializeWithString(v);
					t->Parameters[i] = NewVariable;
				} else {
					int32 iv = FCString::Atoi(*v);
					UAHTNAI_IntegerConstant* NewIntegerConstant = NewObject<UAHTNAI_IntegerConstant>();
					NewIntegerConstant->Initilaze(iv);
					t->Parameters[i] = NewIntegerConstant;
					
				}
			}
		}
	}
        
	return t;
}

void UAHTNAI_Term::RenameVariable(int32 r)
{
	if (Parameters.IsValidIndex(0)) {
		for(UAHTNAI_Parameter* p:Parameters) {
			if (Cast<UAHTNAI_Variable>(p)) {
				Cast<UAHTNAI_Variable>(p)->SetRenaming(r);
			} else if (Cast<UAHTNAI_Function>(p)) {
				Cast<UAHTNAI_Function>(p)->RenameVariable(r);
			}
		}
	}
}

TOptional<TArray<UAHTNAI_Bindings*>> UAHTNAI_Term::SimpleUnificationDestructiveNoSharedVariables(UAHTNAI_Term* T, ABaseGameState* gs)
{
	if (DEBUG>=1) UE_LOG(LogTemp, Warning, TEXT("simpleUnificationDestructiveNoSharedVariables: start"));
	if (!Functor->Equals(*T->GetFunctor()))
	{
		TArray<UAHTNAI_Bindings*> NewList;
		NewList.Empty();
		return  NewList;
	}
	
	if (Parameters.Num() != T->Parameters.Num())
	{
		TArray<UAHTNAI_Bindings*> NewList;
		NewList.Empty();
		return  NewList;
	}
	
	TArray<UAHTNAI_Bindings*> Bindings;
	TArray<UAHTNAI_Parameter*> Resolved;
	Resolved.Init(nullptr, Parameters.Num());
        
	for(int i = 0;i<Parameters.Num();i++)
	{
		UAHTNAI_Parameter* p1 = Parameters[i]->ResolveParameter(Bindings,gs);        //resolveParameter(bindings, gs);
		UAHTNAI_Parameter* p2 =T-> Parameters[i]->ResolveParameter(Bindings,gs);            //resolveParameter(bindings, gs);
           
		if (DEBUG>=1) UE_LOG(LogTemp, Warning, TEXT("simpleUnificationDestructiveNoSharedVariables: %i -> %s U %s"), i, *p1->ToString(), *p2->ToString());
            
		Resolved[i] = p1;
		
		if (Cast<UAHTNAI_Variable>(p1))
		{
			if (!Cast<UAHTNAI_Variable>(p2) &&
				  Cast<UAHTNAI_Variable>(p2)->Equals(Cast<UAHTNAI_Variable>(p1))) {
				if (!(Cast<UAHTNAI_Variable>(p1))->Ignore())
				{
					UAHTNAI_Bindings* NewBinding = NewObject<UAHTNAI_Bindings>();
					NewBinding->InitBindings(Cast<UAHTNAI_Variable>(p1), Cast<UAHTNAI_Variable>(p2));
					Bindings.Add(NewBinding);
				}
				  }
		} else {
				  	if (Cast<UAHTNAI_Variable>(p2)) {
				  		if (!(Cast<UAHTNAI_Variable>(p2))->Ignore())
				  		{
				  			UAHTNAI_Bindings* NewBinding = NewObject<UAHTNAI_Bindings>();
				  			NewBinding->InitBindings(Cast<UAHTNAI_Variable>(p2), Cast<UAHTNAI_Variable>(p1));
				  			Bindings.Add(NewBinding);
				  		}
					
				  	} else {
				  		// otherwise, they are constants, and must be identical:
				  		if (p1 != p2) return TOptional<TArray<UAHTNAI_Bindings*>>();
				  	}
		}
		
	}
		// if unification is successful, apply it:
		Parameters = Resolved;
		return Bindings;
}


UAHTNAI_Term* UAHTNAI_Term::Resolve(TArray<UAHTNAI_Bindings*> l, ABaseGameState* gs)
{
	if (l.Num() == 0) return this;
        
	UAHTNAI_Term* t = NewObject<UAHTNAI_Term>();
	t->Functor = Functor;
	TArray<UAHTNAI_Parameter*> NewParameters;
	NewParameters.Init(nullptr, Parameters.Num());
 	t->Parameters = NewParameters;
	for(int i = 0;i<t->Parameters.Num();i++) {
		t->Parameters[i] = Parameters[i]->ResolveParameter(l, gs);
	}
	return t;
}

UAHTNAI_Term* UAHTNAI_Term::Clone()
{
	UAHTNAI_Term* t = NewObject<UAHTNAI_Term>();
	TArray<UAHTNAI_Parameter*> dummy;
	t->Initialize(Functor, nullptr, nullptr, dummy);
	if (Parameters.IsValidIndex(0)) {
		dummy.Init(nullptr, Parameters.Num());
		t->Parameters = dummy;
		for(int i = 0;i<t->Parameters.Num();i++) {
			t->Parameters[i] = Parameters[i]->CloneParameter();
		}
	}
        
	return t; 
}

void UAHTNAI_Term::ApplyBindings(TArray<UAHTNAI_Bindings*> l)
{
	if (l.Num() == 0) return;
	if (Parameters.IsValidIndex(0)) {
		for(int i = 0;i<Parameters.Num();i++) {
			Parameters[i] = Parameters[i]->ApplyBindingsParameter(l);
		}
	}
}

bool UAHTNAI_Term::IsGround()
{
	for(UAHTNAI_Parameter* p:Parameters) {
		if (Cast<UAHTNAI_Variable>(p)) return false;
		if (Cast<UAHTNAI_Function>(p)) {
			if (!(Cast<UAHTNAI_Function>(p))->IsGround()) return false;
		}
	}
        
	return true;
}

void UAHTNAI_Term::CountVariableAppearances(TMap<UAHTNAI_Symboll*, int32>& Appearances)
{
	for(UAHTNAI_Parameter* p:Parameters) {
		if (Cast<UAHTNAI_Variable>(p) && !(Cast<UAHTNAI_Variable>(p))->Ignore()) {
			UAHTNAI_Symboll* name = NewObject<UAHTNAI_Symboll>();
			name->InitializeWithString(Cast<UAHTNAI_Variable>(p)->GetNameSymbol()->ToString());
			if (Appearances.Find(name)) {
				Appearances.Add(name, *Appearances.Find(name) + 1);
			} else {
				Appearances.Add(name,1);
			}
		}
		if (Cast<UAHTNAI_Function>(p)) {
			Cast<UAHTNAI_Function>(p)->CountVariableAppearances(Appearances);
		}
	}
}

void UAHTNAI_Term::ReplaceSingeltonsByWildcards(TArray<UAHTNAI_Symboll*>& Singeltons)
{
	for(int i = 0;i<Parameters.Num();i++) {
		UAHTNAI_Parameter* p = Parameters[i];
		if (Cast<UAHTNAI_Variable>(p)) {
			UAHTNAI_Symboll* name = (Cast<UAHTNAI_Variable>(p))->GetNameSymbol();
			if (Singeltons.Contains(name)) {
				UAHTNAI_Symboll* NewSymboll = NewObject<UAHTNAI_Symboll>();
				NewSymboll->InitializeWithString("?_");
				Parameters[i] = NewSymboll;
			}
		}
		if (Cast<UAHTNAI_Function>(p)) {
			(Cast<UAHTNAI_Function>(p))->ReplaceSingeltonsByWildcards(Singeltons);
		}
	}
}

bool UAHTNAI_Term::Equals(UObject* o)
{
	if (!Cast<UAHTNAI_Term>(o)) return false;
	UAHTNAI_Term* t = Cast<UAHTNAI_Term>(o);
        
	if (!Functor->Equals(*t->Functor)) return false;
	if (Parameters.Num()!=t->Parameters.Num()) return false;
	for(int i = 0;i<Parameters.Num();i++) {
		if (Parameters[i] != (t->Parameters[i])) return false;
	}
        
	return true;
}

FString UAHTNAI_Term::ToString() const
{
	FString Result = "(";
	Result += Functor->ToString(); 
    
	if (Parameters.Num() > 0) 
	{
		for (const UAHTNAI_Parameter* Parameter : Parameters) 
		{
			Result += " ";
			Result += Parameter->ToString(); 
		}
	}

	Result += ")";
	return Result;
}

