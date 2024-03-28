// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020


#include "AHTNAI_Function.h"

#include "AHTNAI_PreDefinedFunctions.h"

UAHTNAI_Function* UAHTNAI_Function::FromLispElement(UAHTNAI_LispElement* e)
{
	// this is a hack (load a term, and just copy it over to a function):
	UAHTNAI_Term* t = NewObject<UAHTNAI_Term>();
	t = UAHTNAI_Term::FromLispElement(e);
	UAHTNAI_Function* f = NewObject<UAHTNAI_Function>();
	f->Functor = t->Functor;
	f->Parameters = t->Parameters;
	return f;
}

TOptional<TArray<UAHTNAI_Bindings*>> UAHTNAI_Function::Match(int v)
{
	UE_LOG(LogTemp, Warning, TEXT("Function.match not implemented yet"));
	return TOptional<TArray<UAHTNAI_Bindings*>>();
}

TOptional<TArray<UAHTNAI_Bindings*>> UAHTNAI_Function::MatchString(const FString& s)
{
	UE_LOG(LogTemp, Warning, TEXT("Function.match not implemented yet"));
	return TOptional<TArray<UAHTNAI_Bindings*>>();
}

UAHTNAI_Term* UAHTNAI_Function::Clone()
{
	UAHTNAI_Function* t = NewObject<UAHTNAI_Function>();
	t->Functor = Functor;
	TArray<UAHTNAI_Parameter*> NewParameters;
	NewParameters.Init(nullptr, t->Parameters.Num());
	t->Parameters = NewParameters;
	for(int i = 0;i<t->Parameters.Num();i++) {
		t->Parameters[i] = Parameters[i]->CloneParameter();
	}
        
	return t; 
}

UAHTNAI_Parameter* UAHTNAI_Function::CloneParameter()
{
	return UAHTNAI_Function::Clone();
}

UAHTNAI_Parameter* UAHTNAI_Function::ResolveParameter(const TArray<UAHTNAI_Bindings*>& l, ABaseGameState* gs)
{
	UAHTNAI_Function* f = this;
	if (l.IsValidIndex(0)) {
		f->Functor = Functor;
		TArray<UAHTNAI_Parameter*> NewParameters;
		NewParameters.Init(nullptr, Parameters.Num());
		f->Parameters = NewParameters;
		for(int i = 0;i<f->Parameters.Num();i++) {
			f->Parameters[i] = Parameters[i]->ResolveParameter(l, gs);
		}
	}
        
	if (f->IsGround()) {
		UAHTNAI_Parameter* p = UAHTNAI_PreDefinedFunctions::Evaluate(f, gs); 
		if (DEBUG>=1) UE_LOG(LogTemp, Warning, TEXT("Function.resolveParameter: %s -> %s"), *this->ToString(), *p->ToString());
		return p;
	} else {
		return f;
	}
}

UAHTNAI_Parameter* UAHTNAI_Function::ApplyBindingsParameter(const TArray<UAHTNAI_Bindings*>& l)
{
	UAHTNAI_Function* f = this;
	if (l.IsValidIndex(0)) {
		f->Functor = Functor;
		TArray<UAHTNAI_Parameter*> NewParameters;
		NewParameters.Init(nullptr, Parameters.Num());
		f->Parameters = NewParameters;
		for(int i = 0;i<f->Parameters.Num();i++) {
			f->Parameters[i] = Parameters[i]->ApplyBindingsParameter(l);
		}
	}
        
	return f;
}    

