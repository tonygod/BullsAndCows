/*
FBullCowGame.cpp
An adaptation of Bulls & Cows, the inspiration for Mastermind
(see https://en.wikipedia.org/wiki/Mastermind_(board_game))

A "hidden word" is chosen from a list of isograms (words without repeating letters).
The player is given the number of digits in the word and a number of guesses to figure
out what the hidden word is.  The number of guesses is determined by the length of the
hidden word.  When the player submits a guess, each letter is checked against
the hidden word and if the letter is contained in the hidden word and is in the correct spot,
a Bull is awarded.  If the letter is contained in the hidden word, but is not in the correct
spot, a cow is awarded.

Last updated: 1/12/2019
*/

#pragma once

#include "pch.h"
#include "FBullCowGame.h"
#include <map>

// to make standard C++ types more like Unreal types
#define TMap std::map


FBullCowGame::FBullCowGame()
{
	Reset();
}


void FBullCowGame::Reset()
{
	MyCurrentTry = 1;
	bMyGameWon = false;
	return;
}


void FBullCowGame::UpdateHiddenWord()
{
	FString HiddenWords[] = {
		"cow",
		"and",
		"cat",
		"dog",
		"ant",
		"rug",
		"more",
		"case",
		"fish",
		"bird",
		"plane",
		"snail",
		"flame",
		"brush",
		"house",
		"mouse",
		"frame",
		"planet",
		"superb",
		"engulf",
	};
	MyWordIndex++;
	int32 HiddenWordsLength = sizeof(HiddenWords) / sizeof(*HiddenWords);
	if (MyWordIndex >= HiddenWordsLength)
	{
		MyWordIndex = 0;
	}
	MyHiddenWord = HiddenWords[MyWordIndex];
}

int32 FBullCowGame::GetMaxTries() const
{
	TMap<int32, int32> WordLengthToMaxTries = {
		{3, 5},
		{4, 10},
		{5, 20},
		{6, 25},
		{7, 35},
		{8, 50},
		{9, 75},
		{10, 100}
	};
	return WordLengthToMaxTries[MyHiddenWord.length()];
}

int32 FBullCowGame::GetCurrentTry() const
{
	return MyCurrentTry;
}

int32 FBullCowGame::GetHiddenWordLength() const
{
	return MyHiddenWord.length();
}

bool FBullCowGame::IsGameWon() const
{
	return bMyGameWon;
}

EWordStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if (Guess == "")
	{
		return EWordStatus::NullOrEmptyString;
	}
	if (Guess.length() != MyHiddenWord.length())
	{
		return EWordStatus::WrongLength;
	}
	if (!IsIsogram(Guess))
	{
		return EWordStatus::NotIsogram;
	}
	if (!IsLowercase(Guess))
	{
		return EWordStatus::NotLowercase;
	}

	return EWordStatus::OK;
}


bool FBullCowGame::IsIsogram(FString Word) const
{
	if (HasDuplicatesTMap(Word))
	{
		return false;
	}
	return true;
}


/// O(n) function to check a string for duplicate characters
bool FBullCowGame::HasDuplicatesTMap(FString Word) const
{
	if (Word[0] == 0)
		return false;

	TMap<char, bool> LetterSeen;
	for (auto Letter : Word)
	{
		Letter = tolower(Letter);
		if (LetterSeen[Letter])
		{
			return true;
		}
		LetterSeen[Letter] = true;
	}
	return false;
}


// O(n) function to check a string for duplicate characters
bool FBullCowGame::HasDuplicates(FString Word) const
{
	if (Word[0] == 0)
		return false;

	int32 LetterArray[256] = {};
	for (int32 i = 0; i < (int32)Word.length(); i++)
	{
		if (LetterArray[Word[i]] > 0)
			return true;
		LetterArray[Word[i]] = 1;
	}
	return false;
}


// returns true of all letters are lowercase
bool FBullCowGame::IsLowercase(FString Word) const
{
	if (Word[0] == 0) // it's neither uppercase nor lowercase
		return false;

	for (auto Letter : Word)
	{
		if (!islower(Letter))
			return false;
	}
	return true;
}


void FBullCowGame::IncrementCurrentTry()
{
	MyCurrentTry++;
}

FBullCowCount FBullCowGame::SubmitGuess(FString Guess)
{
	// setup return value
	FBullCowCount BullCowCount;

	// loop through letters in guess
	int32 LoopLength = (int32)MyHiddenWord.length();
	if ((int32)Guess.length() < LoopLength)
		LoopLength = Guess.length();
	for (int32 i = 0; i < LoopLength; i++)
	{
		// compare letters against hidden word
		if (tolower(Guess[i]) == MyHiddenWord[i]) // letter and position matches
		{
			BullCowCount.Bulls++;
		}
		else if ((MyHiddenWord.find(tolower(Guess[i]), 0)) != FString::npos) // only letter matches
		{
			BullCowCount.Cows++;
		}
	}

	if (BullCowCount.Bulls == MyHiddenWord.length())
	{
		bMyGameWon = true;
	}

	// incrememnt turn number
	IncrementCurrentTry();

	return BullCowCount;
}


