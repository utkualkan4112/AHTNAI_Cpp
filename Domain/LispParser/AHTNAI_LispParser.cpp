// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020


#include "AHTNAI_LispParser.h"

#include "AHTNAI_LispTokinizer.h"
#include "AHTNAI_LispElement.h"

int UAHTNAI_LispParser::DEBUG = 0;

TArray<UAHTNAI_LispElement*> UAHTNAI_LispParser::ParseString(const FString& s)
{
	UAHTNAI_LispTokinizer* Tokenizer = NewObject<UAHTNAI_LispTokinizer>();
	Tokenizer->InitializeTokenizerFromString(s); // Assuming you'd add such a method to ULispTokenizer
	return ParseLisp(Tokenizer);
}

TArray<UAHTNAI_LispElement*> UAHTNAI_LispParser::ParseLispFile(const FString& FileName)
{
	UAHTNAI_LispTokinizer* Tokenizer = NewObject<UAHTNAI_LispTokinizer>();
	if (!Tokenizer->InitializeTokenizer(FileName))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to initialize tokenizer with file %s"), *FileName);
		return TArray<UAHTNAI_LispElement*>();
	}

	return ParseLisp(Tokenizer);
}

TArray<UAHTNAI_LispElement*> UAHTNAI_LispParser::ParseLisp(UAHTNAI_LispTokinizer* Tokenizer)
{
	TArray<UAHTNAI_LispElement*> Result;
	TArray<UAHTNAI_LispElement*> Stack;

	FString Token = Tokenizer->NextToken();
	while (!Token.IsEmpty())
	{
		if (DEBUG >= 1)
		{
			UE_LOG(LogTemp, Log, TEXT("next token: %s"), *Token);
		}

		if (Token == TEXT("("))
		{
			Stack.Insert(NewObject<UAHTNAI_LispElement>(), 0);
		}
		else if (Token == TEXT(")"))
		{
			UAHTNAI_LispElement* Element = Stack[0];
			Stack.RemoveAt(0);
			if (Stack.Num() == 0)
			{
				Result.Add(Element);
			}
			else
			{
				Stack[0]->Children.Add(Element); // Assuming ULispElement has a TArray<ULispElement*> Children member
			}
		}
		else
		{
			UAHTNAI_LispElement* Element = NewObject<UAHTNAI_LispElement>();
			Element->Element = Token; // Assuming ULispElement has a Value member of type FString

			if (Stack.Num() == 0)
			{
				Result.Add(Element);
			}
			else
			{
				Stack[0]->Children.Add(Element);
			}
		}

		Token = Tokenizer->NextToken();
	}

	return Result;
}