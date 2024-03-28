// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "AHTNAI_Symboll.h"
#include "AHTNAI_SymbolConstant.generated.h"

class UAHTNAI_Symboll;
class UAHTNAI_Parameter;
/**
 * 
 */
UCLASS()
class HITANDBOOM_API UAHTNAI_SymbolConstant : public UAHTNAI_Symboll
{
	GENERATED_BODY()
public:
	FORCEINLINE virtual void InitializeWithString(FString sym) override {Super::InitializeWithString(sym);}
	FORCEINLINE virtual void Initialize(UAHTNAI_Symboll* sym) override {Super::Initialize(sym);}
	FORCEINLINE virtual TOptional<TArray<UAHTNAI_Bindings*>> Match(int32 v) override { return TOptional<TArray<UAHTNAI_Bindings*>>(); }
	virtual TOptional<TArray<UAHTNAI_Bindings*>> MatchString(const FString& s) override;
	FORCEINLINE virtual UAHTNAI_Parameter* CloneParameter() override { return this; }
	FORCEINLINE virtual UAHTNAI_Parameter* ResolveParameter(const TArray<UAHTNAI_Bindings*>& l, ABaseGameState* gs) override { return this; }
	FORCEINLINE virtual UAHTNAI_Parameter* ApplyBindingsParameter(const TArray<UAHTNAI_Bindings*>& l) override { return this; }
	bool EqualsObject(const UObject* o);
	
};
