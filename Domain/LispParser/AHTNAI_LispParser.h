// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AHTNAI_LispParser.generated.h"

class UAHTNAI_LispElement;
class UAHTNAI_LispTokinizer;
/**
 * 
 */
UCLASS()
class HITANDBOOM_API UAHTNAI_LispParser : public UObject
{
	GENERATED_BODY()

public:
	static int DEBUG;

	static TArray<UAHTNAI_LispElement*> ParseString(const FString& s);
	static TArray<UAHTNAI_LispElement*> ParseLispFile(const FString& FileName);
	static TArray<UAHTNAI_LispElement*> ParseLisp(UAHTNAI_LispTokinizer* Tokenizer);
};
