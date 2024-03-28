// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AHTNAI_LispTokinizer.generated.h"

/**
 * 
 */
UCLASS()
class HITANDBOOM_API UAHTNAI_LispTokinizer : public UObject
{
	GENERATED_BODY()
	private:
    TArray<FString> Lines;
    FString CurrentLine;
    int32 LineIndex;
    int32 CharIndex;
    TCHAR NextCharacter;

public:
    UAHTNAI_LispTokinizer();

    bool InitializeTokenizer(const FString& FileName);
	bool InitializeTokenizerFromString(const FString& InputString);
    TCHAR GetNextCharacter();
    FString NextToken();
	
};
