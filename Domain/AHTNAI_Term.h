// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "AHTNAI_Parameter.h"
#include "LispParser/AHTNAI_LispParser.h"
#include "AHTNAI_Term.generated.h"


class UAHTNAI_LispElement;
class UAHTNAI_Symboll;
/**
 * 
 */
UCLASS()
class HITANDBOOM_API UAHTNAI_Term : public UAHTNAI_Parameter
{
	GENERATED_BODY()
private:
	int DEBUG = 0;
	
public:

	UAHTNAI_Term();

	UPROPERTY()
	UAHTNAI_Symboll* Functor;
	UPROPERTY()
	TArray<UAHTNAI_Parameter*> Parameters;

	void Initialize(UAHTNAI_Symboll* f, UAHTNAI_Parameter* p1, UAHTNAI_Parameter* p2, TArray<UAHTNAI_Parameter*> Plist);

	FORCEINLINE UAHTNAI_Symboll* GetFunctor() const { return Functor; }

	static UAHTNAI_Term* FromString(const FString& S);

	static UAHTNAI_Term* FromLispElement(UAHTNAI_LispElement* e);

	virtual void RenameVariable(int32 r);

	TOptional<TArray<UAHTNAI_Bindings*>> SimpleUnificationDestructiveNoSharedVariables(UAHTNAI_Term* T, ABaseGameState* gs);

	UAHTNAI_Term* Resolve(TArray<UAHTNAI_Bindings*> l, ABaseGameState* gs);

	virtual UAHTNAI_Term* Clone();

	void ApplyBindings(TArray<UAHTNAI_Bindings*> l);

	bool IsGround();

	void CountVariableAppearances(TMap<UAHTNAI_Symboll*, int32>& Appearances);

	virtual void ReplaceSingeltonsByWildcards(TArray<UAHTNAI_Symboll*>& Singeltons);

	bool Equals(UObject* o);

	FString ToString() const;
	
};
