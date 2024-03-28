// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020


#include "AHTNAI_Variable.h"
#include "AHTNAI_Symboll.h"
#include "AHTNAI_Bindings.h"
#include "AHTNAI_IntegerConstant.h"
#include "AHTNAI_SymbolConstant.h"

UAHTNAI_Variable::UAHTNAI_Variable()
{
	VariableToIgnore = NewObject<UAHTNAI_Symboll>();  // Replacement for static Symbol variable_to_ignore in Java
	Name = NewObject<UAHTNAI_Symboll>();
}

void UAHTNAI_Variable::InitializeWithSymbol(UAHTNAI_Symboll* Sym)
{
	Name = Sym;
}

void UAHTNAI_Variable::InitializeWithString(FString Sym)
{
	UAHTNAI_Symboll* NewName = NewObject<UAHTNAI_Symboll>();
	NewName->InitializeWithString(Sym);
	Name = NewName;
}

bool UAHTNAI_Variable::Ignore()
{
	if (!VariableToIgnore)
	{
		VariableToIgnore = NewObject<UAHTNAI_Symboll>();
		VariableToIgnore->InitializeWithString("?_");
	}
	return Name->Equals(*VariableToIgnore);
}

TOptional<TArray<UAHTNAI_Bindings*>> UAHTNAI_Variable::Match(int32 v)
{
	TArray<UAHTNAI_Bindings*> l = TArray<UAHTNAI_Bindings*>();
	UAHTNAI_Bindings* NewBinding = NewObject<UAHTNAI_Bindings>();
	UAHTNAI_IntegerConstant* NewIntegerConstant = NewObject<UAHTNAI_IntegerConstant>();
	NewIntegerConstant->Initilaze(v);
	NewBinding->InitBindings(this, NewIntegerConstant);
	if (!Ignore()) l.Add(NewBinding);
	return l;
}

TOptional<TArray<UAHTNAI_Bindings*>> UAHTNAI_Variable::MatchString(const FString& s)
{
	TArray<UAHTNAI_Bindings*> l = TArray<UAHTNAI_Bindings*>();
	UAHTNAI_Bindings* NewBinding = NewObject<UAHTNAI_Bindings>();
	UAHTNAI_SymbolConstant* NewSymbollConstant = NewObject<UAHTNAI_SymbolConstant>();
	NewSymbollConstant->InitializeWithString(s);
	NewBinding->InitBindings(this, NewSymbollConstant);
	if(!Ignore()) l.Add((NewBinding));
	return l;
}

UAHTNAI_Parameter* UAHTNAI_Variable::CloneParameter()
{
	UAHTNAI_Variable* v = NewObject<UAHTNAI_Variable>();
	v->InitializeWithSymbol(Name);
	v->Renaming = Renaming;
	return v;
}

UAHTNAI_Parameter* UAHTNAI_Variable::ResolveParameter(const TArray<UAHTNAI_Bindings*>& l, ABaseGameState* gs)
{
	if (l.IsValidIndex(0)) return this;
	return ApplyBindingsParameter(l);
}

UAHTNAI_Parameter* UAHTNAI_Variable::ApplyBindingsParameter(const TArray<UAHTNAI_Bindings*>& l)
{
	if (Ignore()) return this;
	UAHTNAI_Parameter* tmp = this;
	for(UAHTNAI_Bindings* b:l) {
		if (b->V->Equals(tmp)) tmp = b->P;
	}
	return tmp;
}

bool UAHTNAI_Variable::Equals(UObject* o)
{
	if(Cast<UAHTNAI_Variable>(o))
	{
		UAHTNAI_Variable* V = Cast<UAHTNAI_Variable>(o);
		return Name->Equals(*V->Name) && (Renaming == V->Renaming);
	}
	else
	{
		return false;
	}
}

FString UAHTNAI_Variable::ToString() const
{
	if(Renaming == 0)
	{
		return Name->ToString();
	}
	else
	{
		return Name->ToString() + "/" + FString::FromInt(Renaming);
	}
}

