// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020


#include "AHTNAI_SymbolConstant.h"
#include "AHTNAI_Parameter.h"

TOptional<TArray<UAHTNAI_Bindings*>> UAHTNAI_SymbolConstant::MatchString(const FString& s)
{
	if(this->EqualsString(s))
	{
		return TArray<UAHTNAI_Bindings*>();
	}
	return TOptional<TArray<UAHTNAI_Bindings*>>();
}

bool UAHTNAI_SymbolConstant::EqualsObject(const UObject* o)
{
	if(Cast<UAHTNAI_SymbolConstant>(o))
	{
		const UAHTNAI_SymbolConstant* Sym = Cast<UAHTNAI_SymbolConstant>(o);
		return mSym == Sym->mSym;
	}
	else
	{
		return false;
	}
}


