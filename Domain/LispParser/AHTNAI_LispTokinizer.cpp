// Any unauthorized usage, copy, derivation is strictly prohibited. Funmoth Games 2020


#include "AHTNAI_LispTokinizer.h"

UAHTNAI_LispTokinizer::UAHTNAI_LispTokinizer()
	: LineIndex(0), CharIndex(0), NextCharacter('\0')
{
}


bool UAHTNAI_LispTokinizer::InitializeTokenizer(const FString& FileName)
{
	return FFileHelper::LoadFileToStringArray(Lines, *FileName);
}

bool UAHTNAI_LispTokinizer::InitializeTokenizerFromString(const FString& InputString)
{
	Lines.Add(FString());
	Lines.Add(InputString);
	LineIndex = 0;
	CharIndex = 0;
	return true;
}

TCHAR UAHTNAI_LispTokinizer::GetNextCharacter()
{
	if (CharIndex < CurrentLine.Len())
	{
		return CurrentLine[CharIndex++];
	}
	else if (++LineIndex < Lines.Num())
	{
		CurrentLine = Lines[LineIndex];
		CharIndex = 0;
		return GetNextCharacter();
	}

	return '\0'; // End of file
}

FString UAHTNAI_LispTokinizer::NextToken()
{
	FString Token;
	bool bCommentBlock = false;

	while (LineIndex < Lines.Num())
	{
		TCHAR c = GetNextCharacter();

		if (c == ';')
		{
			CharIndex = CurrentLine.Len(); // Skip the whole line
			continue;
		}
		else if (c == ' ' || c == '\n' || c == '\t' || c == '\0')
		{
			if (!Token.IsEmpty())
				return Token;
		}
		else if (c == '(' || c == ')')
		{
			if (Token.IsEmpty())
				return FString(1, &c);
			else
			{
				NextCharacter = c;
				return Token;
			}
		}
		else
		{
			Token.AppendChar(c);

			// Handle comment blocks
			if (Token.EndsWith(TEXT("#|")))
			{
				Token = Token.LeftChop(2); // Remove last two characters
				bCommentBlock = true;
			}
			else if (bCommentBlock && Token.EndsWith(TEXT("|#")))
			{
				Token = Token.LeftChop(2); // Remove last two characters
				bCommentBlock = false;
			}

			if (bCommentBlock)
			{
				Token.Empty(); // Discard current token content within the comment block
			}
		}
	}

	return Token;
}