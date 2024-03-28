// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "AHTNAI_Parameter.h"
#include "AHTNAI_Variable.generated.h"



class UAHTNAI_Symboll;
UCLASS()
class HITANDBOOM_API UAHTNAI_Variable : public UAHTNAI_Parameter
{
	GENERATED_BODY()

    private:

	UPROPERTY()
	UAHTNAI_Symboll* VariableToIgnore;  // Replacement for static Symbol variable_to_ignore

	UPROPERTY()
	UAHTNAI_Symboll* Name;

	int32 Renaming = 0;   // Replacement for int renaming
	
	public:
	
	UAHTNAI_Variable();
	
	void InitializeWithSymbol(UAHTNAI_Symboll* Sym);
	void InitializeWithString(FString Sym);
	
	FORCEINLINE UAHTNAI_Symboll* GetNameSymbol() const { return Name; }
	
	FORCEINLINE void SetRenaming(int32 r) { Renaming = r; }
	
	bool Ignore();

	virtual TOptional<TArray<UAHTNAI_Bindings*>> Match(int32 v) override;

	virtual TOptional<TArray<UAHTNAI_Bindings*>> MatchString(const FString& s) override;

	virtual UAHTNAI_Parameter* CloneParameter() override;
	virtual UAHTNAI_Parameter* ResolveParameter(const TArray<UAHTNAI_Bindings*>& l, ABaseGameState* gs) override;
	virtual UAHTNAI_Parameter* ApplyBindingsParameter(const TArray<UAHTNAI_Bindings*>& l) override;

	bool Equals(UObject* o);

	virtual FString ToString() const override;
};
