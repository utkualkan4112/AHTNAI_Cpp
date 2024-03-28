// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020


#include "AHTNAI_VectorConstant.h"

TOptional<TArray<UAHTNAI_Bindings*>> UAHTNAI_VectorConstant::MatchVector(const FVector& v)
{
	if(Location == v)
	{
		return TArray<UAHTNAI_Bindings*>();
	}
	return TOptional<TArray<UAHTNAI_Bindings*>>();
}

bool UAHTNAI_VectorConstant::Equals(UObject* o)
{
	if(Cast<UAHTNAI_VectorConstant>(o))
	{
		return Cast<UAHTNAI_VectorConstant>(o)->Location == Location;
	}
	return false;
}
