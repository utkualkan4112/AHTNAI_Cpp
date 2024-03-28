// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#include "AHTNAI_Parameter.h"


FString UAHTNAI_Parameter::ToString() const
{
	FString Return = "";
	return Return;
}

TOptional<TArray<UAHTNAI_Bindings*>> UAHTNAI_Parameter::Match(int32 v)
{
	return TOptional<TArray<UAHTNAI_Bindings*>>();
}

TOptional<TArray<UAHTNAI_Bindings*>> UAHTNAI_Parameter::MatchString(const FString& s)
{
	return TOptional<TArray<UAHTNAI_Bindings*>>();
}

TOptional<TArray<UAHTNAI_Bindings*>> UAHTNAI_Parameter::MatchVector(const FVector& v)
{
	return TOptional<TArray<UAHTNAI_Bindings*>>();
}

UAHTNAI_Parameter* UAHTNAI_Parameter::CloneParameter()
{
	return nullptr;
}

UAHTNAI_Parameter* UAHTNAI_Parameter::ApplyBindingsParameter(const TArray<UAHTNAI_Bindings*>& l)
{
	return nullptr;
}

UAHTNAI_Parameter* UAHTNAI_Parameter::ResolveParameter(const TArray<UAHTNAI_Bindings*>& l, ABaseGameState* gs)
{
	return nullptr;
}
