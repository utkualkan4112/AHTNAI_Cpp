// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "AHTNAI_Parameter.h"
#include "AHTNAI_IntegerConstant.generated.h"

/**
 * 
 */
UCLASS()
class HITANDBOOM_API UAHTNAI_IntegerConstant : public UAHTNAI_Parameter
{
	GENERATED_BODY()
public:
	
	int32 value;

	FORCEINLINE void Initilaze(int32 NewValue) { value = NewValue; }

	FORCEINLINE FString ToString() const override { return "" + value; }

	FORCEINLINE virtual TOptional<TArray<UAHTNAI_Bindings*>> MatchString(const FString& s) override { return TOptional<TArray<UAHTNAI_Bindings*>>(); }

	FORCEINLINE virtual UAHTNAI_Parameter* ResolveParameter(const TArray<UAHTNAI_Bindings*>& l, ABaseGameState* gs) override { return this; }

	FORCEINLINE virtual UAHTNAI_Parameter* ApplyBindingsParameter(const TArray<UAHTNAI_Bindings*>& l) override { return this; }

	FORCEINLINE virtual UAHTNAI_Parameter* CloneParameter() override { return this; }

	virtual TOptional<TArray<UAHTNAI_Bindings*>> Match(int32 v) override;
	
	bool Equals(UObject* o);
	
};
