// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020


#include "AHTNAI_IntegerConstant.h"

TOptional<TArray<UAHTNAI_Bindings*>> UAHTNAI_IntegerConstant::Match(int32 v)
{
	if(value == v)
	{
		return TArray<UAHTNAI_Bindings*>();
	}
	return TOptional<TArray<UAHTNAI_Bindings*>>();
}

bool UAHTNAI_IntegerConstant::Equals(UObject* o)
{
	if(Cast<UAHTNAI_IntegerConstant>(o))
	{
		return Cast<UAHTNAI_IntegerConstant>(o)->value == value;
	}
	return false;
}
