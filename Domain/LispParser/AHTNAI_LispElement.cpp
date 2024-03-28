// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020


#include "AHTNAI_LispElement.h"

UAHTNAI_LispElement::UAHTNAI_LispElement()
{
	TArray<UAHTNAI_LispElement*> NewList;
	Children = NewList;
	Element = nullptr;
}

FString UAHTNAI_LispElement::ToStringInt(int32 Tabs)
{
	FString TabString = TEXT("");
	for (int32 i = 0; i < Tabs; ++i)
	{
		TabString += TEXT("  ");
	}

	if (!Children.Num())
	{
		return TabString + Element;
	}
	else
	{
		FString Temp = TabString + TEXT("(\n");
		for (UAHTNAI_LispElement* Child : Children)
		{
			Temp += Child->ToStringInt(Tabs + 1) + TEXT("\n");
		}
		Temp += TabString + TEXT(")");
		return Temp;
	}
}
