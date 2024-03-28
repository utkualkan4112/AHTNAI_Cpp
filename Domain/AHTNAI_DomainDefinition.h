// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AHTNAI_DomainDefinition.generated.h"

class UAHTNAI_LispElement;
class UAHTNAI_Symboll;
class UAHTNAI_HTNMethod;
class UAHTNAI_HTNOperator;
/**
 * 
 */
UCLASS()
class HITANDBOOM_API UAHTNAI_DomainDefinition : public UObject
{
	GENERATED_BODY()
public:
    
	static int32 DEBUG;

private:
    
    FString Name;
    
    TArray<UAHTNAI_HTNOperator*> Operators;
    
    TArray<UAHTNAI_HTNMethod*> Methods;
    
    TMap<UAHTNAI_Symboll*,TArray<UAHTNAI_HTNMethod*>> MethodsPerGoal;
    
public:
    FORCEINLINE TArray<UAHTNAI_HTNOperator*> GetOperators() { return Operators; }
    
    FORCEINLINE TArray<UAHTNAI_HTNMethod*> GetMethods() { return Methods; }
    
    void AddMethod(UAHTNAI_HTNMethod* M);

    static UAHTNAI_DomainDefinition* FromLispFile(FString FileName);
        
    static UAHTNAI_DomainDefinition* FromLispElement(UAHTNAI_LispElement* E);
    
    FString ToString();
    
	TArray<UAHTNAI_HTNMethod*> GetMethodsForGoal(UAHTNAI_Symboll* Functor);
	
};
