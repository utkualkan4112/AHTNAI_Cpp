// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AHTNAI_AIPlayout.generated.h"

class UAHTNAI_PlayerAction;
/**
 * 
 */
class ABaseGameState;
class UAHTNAI_ParameterSpecification;
class ABaseRobot;

UCLASS(Abstract)
class HITANDBOOM_API UAHTNAI_AIPlayout : public UObject
{
	GENERATED_BODY()
public:
	// Reset the AI.
	virtual void Reset() PURE_VIRTUAL(UAI::Reset, );

	// Indicates to the AI that the new game will be played with a new UnitTypeTable.
	virtual void Reset(ABaseRobot* RobotType)
	{
		Reset();
	}

	// Main thinking method of the AI.
	virtual UAHTNAI_PlayerAction* GetAction(int32 Player, ABaseGameState* GS) PURE_VIRTUAL(UAHTNAI_AIPlayout::GetAction, return nullptr;);

	// This function is not supposed to do an exact clone, just a copy of the AI with the same configuration.
	virtual UAHTNAI_AIPlayout* Clone() const PURE_VIRTUAL(UAHTNAI_AIPlayout::Clone, return nullptr;);

	// Returns a list of ParameterSpecification with this controller's parameters.
	virtual TOptional<TArray<UAHTNAI_ParameterSpecification*>> GetParameters() const PURE_VIRTUAL(UAHTNAI_AIPlayout::GetParameters(), return TOptional<TArray<UAHTNAI_ParameterSpecification*>>();) ;

	// This method can be used to report any meaningful statistics once the game is over.
	virtual FString StatisticsString() const
	{
		return FString();
	}

	// Prints the statistics.
	void PrintStats()
	{
		FString Stats = StatisticsString();
		if (!Stats.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Stats);
		}
	}

	// Return class name as string.
	virtual FString ToString() const
	{
		return GetClass()->GetName();
	}

	// In this function, you can implement some pre-game reasoning.
	virtual void PreGameAnalysis(ABaseGameState* GS, int64 Milliseconds) {}

	// Pre-game reasoning with the possibility of reading/writing to disk.
	virtual void PreGameAnalysis(ABaseGameState* GS, int64 Milliseconds, const FString& ReadWriteFolder)
	{
		PreGameAnalysis(GS, Milliseconds);
	}

	// Notifies the AI that the game is over, and reports who was the winner.
	virtual void GameOver(int32 Winner) {}
};
