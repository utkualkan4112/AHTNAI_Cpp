// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020


#include "AHTNAI_HTNMethod.h"

#include "AdvancedAI/AHTNAI/Domain/LispParser/AHTNAI_LispElement.h"

void UAHTNAI_HTNMethod::Initialize(FString N, UAHTNAI_Term* T, UAHTNAI_MethodDecomposition* M)
{
	Name = N;
	Head = T;
	Method = M;
}

UAHTNAI_HTNMethod* UAHTNAI_HTNMethod::Clone()
{
	UAHTNAI_HTNMethod* NewMethod = NewObject<UAHTNAI_HTNMethod>();
	NewMethod->Initialize(Name, Head, Method);
	return NewMethod;
}

UAHTNAI_HTNMethod* UAHTNAI_HTNMethod::CloneTrackingDescendants(TArray<UAHTNAI_MethodDecomposition*> DescendantsToTrack,
	TArray<UAHTNAI_MethodDecomposition*> NewDescendants)
{
	UAHTNAI_HTNMethod* NewMethod = NewObject<UAHTNAI_HTNMethod>();
	NewMethod->Initialize(Name, Head->Clone(), Method->CloneTrackingDescendants(DescendantsToTrack, NewDescendants));
	return NewMethod;
}

void UAHTNAI_HTNMethod::RenameVariables(int32 RenamingIndex)
{
	Head->RenameVariable(RenamingIndex);
	Method->RenameVariables(RenamingIndex);
}

void UAHTNAI_HTNMethod::ApplyBindings(TArray<UAHTNAI_Bindings*> L)
{
	Head->ApplyBindings(L);
	Method->ApplyBindings(L);
}

UAHTNAI_HTNMethod* UAHTNAI_HTNMethod::FromLispElement(UAHTNAI_LispElement* E)
{
	UAHTNAI_LispElement* Name_e = E->Children[1];
	UAHTNAI_LispElement* Head_e = E->Children[2];
	UAHTNAI_LispElement* Method_e = E->Children[3];
	
	FString name = Name_e->Element;
	UAHTNAI_Term* head = UAHTNAI_Term::FromLispElement(Head_e);
	UAHTNAI_MethodDecomposition* m = UAHTNAI_MethodDecomposition::FromLispElement(Method_e);
	UAHTNAI_HTNMethod* ReturnMethod = NewObject<UAHTNAI_HTNMethod>();
	ReturnMethod->Initialize(name, head, m);
	return ReturnMethod;
}

void UAHTNAI_HTNMethod::ReplaceSingletonsByWildcards()
{
	TArray<UAHTNAI_Symboll*> Singeltons = FindSingletons();
	Head->ReplaceSingeltonsByWildcards(Singeltons);
	Method->ReplaceSingletonsByWildcards(Singeltons);
}

TArray<UAHTNAI_Symboll*> UAHTNAI_HTNMethod::FindSingletons()
{
	TMap<UAHTNAI_Symboll*, int32> Appearances;
	CountVariableAppearances(Appearances);

	TArray<UAHTNAI_Symboll*> L;
	TArray<UAHTNAI_Symboll*> Keys;
	Appearances.GetKeys(Keys);

	for(UAHTNAI_Symboll* v: Keys) {
		if (int32* Value = Appearances.Find(v)) {
			if (*Value == 1) L.Add(v);
		}
	}
	return L;

}

void UAHTNAI_HTNMethod::CountVariableAppearances(TMap<UAHTNAI_Symboll*, int32>& Appearances)
{
	Head->CountVariableAppearances(Appearances);
	Method->CountVariableAppearances(Appearances);
}

