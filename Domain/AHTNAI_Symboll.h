// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "AHTNAI_Parameter.h"
#include "AHTNAI_Symboll.generated.h"

/**
 * 
 */
UCLASS()
class HITANDBOOM_API UAHTNAI_Symboll : public UAHTNAI_Parameter
{
	GENERATED_BODY()

	
private:

	TMap<FString, TSharedPtr<FString>> sSymbolHash;
	
	
public:
	TSharedPtr<FString> mSym;
	UAHTNAI_Symboll();
	~UAHTNAI_Symboll();

	virtual void InitializeWithString(FString sym);
	virtual void Initialize(UAHTNAI_Symboll* sym);

	FORCEINLINE FString Get(){ return *mSym;}
	FORCEINLINE void Set(FString str){ mSym = MakeShared<FString>(str); }
	FORCEINLINE virtual bool Equals(const UAHTNAI_Symboll& sym) { return mSym == sym.mSym; }
	bool EqualsString(const FString& str);
	
	bool EqualsStringPtr(const TSharedPtr<FString>& strPtr) const;
	
	static void ArrangeString(FString& str);
	
	FORCEINLINE FString ToString() const override { return *mSym; }
};
