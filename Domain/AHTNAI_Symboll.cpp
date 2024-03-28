// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020


#include "AHTNAI_Symboll.h"


UAHTNAI_Symboll::UAHTNAI_Symboll()
{
}

UAHTNAI_Symboll::~UAHTNAI_Symboll()
{
}

void UAHTNAI_Symboll::InitializeWithString(FString sym)
{
	if (sym.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("null name in a Symbol!!!"));
		return;
	}
	if (sSymbolHash.Contains(sym))
	{
		mSym = *sSymbolHash.Find(sym);
	}
	else
	{
		mSym = MakeShared<FString>(sym);
		sSymbolHash.Add(sym, mSym);
	}
}

void UAHTNAI_Symboll::Initialize(UAHTNAI_Symboll* sym)
{
	mSym = sym->mSym;
}

bool UAHTNAI_Symboll::EqualsString(const FString& str)
{
	if (!mSym) return str.IsEmpty();
	return *mSym == str;
}

bool UAHTNAI_Symboll::EqualsStringPtr(const TSharedPtr<FString>& strPtr) const
{
	if (!mSym) return !strPtr.IsValid();
	if (!strPtr.IsValid()) return false;
	return *mSym == *strPtr;
}

void UAHTNAI_Symboll::ArrangeString(FString& str)
{
	str = str.TrimStartAndEnd();
}

