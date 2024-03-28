// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020


#include "AHTNAI_ResourceUsage.h"
#include "BaseClasses/BaseGameState.h"
#include "BaseClasses/BaseRobot.h"

bool UAHTNAI_ResourceUsage::ConsistentWith(UAHTNAI_ResourceUsage* AnotherUsage, ABaseGameState* Gs)
{
	for (FVector pos : AnotherUsage->GetPositionsUsed()) {
		if (PositionsUsed.Contains(pos)) {
			return false;
		}
	}
	if (AnotherUsage->GetResourcesUsed() == 0) return true;
	if (ResourceUsed + AnotherUsage->GetResourcesUsed() > 0){
		return false;
			}


	return true;
}

UAHTNAI_ResourceUsage* UAHTNAI_ResourceUsage::MergeIntoNew(UAHTNAI_ResourceUsage* Other)
{
	UAHTNAI_ResourceUsage* NewResourceUsage = NewObject<UAHTNAI_ResourceUsage>();
	NewResourceUsage->SetPositionUsed(PositionsUsed);
	for(int32 i = NewResourceUsage->GetPositionsUsed().Num(); i < Other->GetPositionsUsed().Num(); i++)
	{
		NewResourceUsage->GetPositionsUsed().Add(Other->GetPositionsUsed()[i]);
	}
	NewResourceUsage->SetResourcesUsed(ResourceUsed + Other->GetResourcesUsed());

	return NewResourceUsage;
}

void UAHTNAI_ResourceUsage::Merge(UAHTNAI_ResourceUsage* Other)
{
	for(int32 i = PositionsUsed.Num(); i < Other->GetPositionsUsed().Num(); i++)
	{
		PositionsUsed.Add(Other->GetPositionsUsed()[i]);
	}
	ResourceUsed += Other->GetResourcesUsed();
}

UAHTNAI_ResourceUsage* UAHTNAI_ResourceUsage::Clone()
{
	UAHTNAI_ResourceUsage* ru = NewObject<UAHTNAI_ResourceUsage>();
	for(int32 i = ru->GetPositionsUsed().Num(); i < PositionsUsed.Num(); i++)
	{
		ru->GetPositionsUsed().Add(PositionsUsed[i]);
	}
	ru->SetResourcesUsed(ResourceUsed);
	ru->SetCooldownCost(CooldownCost);
	return ru;
}

FString UAHTNAI_ResourceUsage::ToString()
{
	FString ReturnString = "ResourceUsage: " + FString::FromInt(ResourceUsed) +" Cooldown Cost: " + FString::FromInt(CooldownCost) + " positions: ";
	for(int32 i = 0; i < PositionsUsed.Num(); i++)
	{
		ReturnString.Append("X: " + FString::FromInt(PositionsUsed[i].X) + " Y: "+ FString::FromInt(PositionsUsed[i].Y));
	}
	return ReturnString;
}
