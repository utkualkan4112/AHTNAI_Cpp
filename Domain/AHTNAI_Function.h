// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "AHTNAI_Term.h"
#include "AHTNAI_Function.generated.h"

/**
 * 
 */
UCLASS()
class HITANDBOOM_API UAHTNAI_Function : public UAHTNAI_Term
{
	GENERATED_BODY()
public:
	 int DEBUG = 0;
    
	static UAHTNAI_Function* FromLispElement(UAHTNAI_LispElement* e);  
    
	virtual TOptional<TArray<UAHTNAI_Bindings*>> Match(int v) override;


	virtual TOptional<TArray<UAHTNAI_Bindings*>> MatchString(const FString& s) override;

	UAHTNAI_Term* Clone();
    
	UAHTNAI_Parameter* CloneParameter() override;
    
	virtual UAHTNAI_Parameter* ResolveParameter(const TArray<UAHTNAI_Bindings*>& l, ABaseGameState* gs) override;

	virtual UAHTNAI_Parameter* ApplyBindingsParameter(const TArray<UAHTNAI_Bindings*>& l) override;

	FORCEINLINE virtual void RenameVariable(int32 r) override { Super::RenameVariable(r); }

	FORCEINLINE virtual void ReplaceSingeltonsByWildcards(TArray<UAHTNAI_Symboll*>& Singeltons) override { Super::ReplaceSingeltonsByWildcards(Singeltons); }

};
