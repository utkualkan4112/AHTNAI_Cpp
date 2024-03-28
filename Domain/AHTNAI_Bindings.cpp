// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020


#include "AHTNAI_Bindings.h"
#include "AHTNAI_Variable.h"

void UAHTNAI_Bindings::InitBindings(UAHTNAI_Variable* a_V, UAHTNAI_Parameter* a_P)
{
	P = a_P;
	V = a_V;
}

bool UAHTNAI_Bindings::Equals(UObject* o)
{
	
if(Cast<UAHTNAI_Bindings>(o))
{
	return V->Equals(Cast<UAHTNAI_Bindings>(o)->V); // Add Symboll Constant
}
	return false;
}

FString UAHTNAI_Bindings::ToString()
{
	return "(" + V->ToString() + " -> " + P->ToString() + ")"; 
}
