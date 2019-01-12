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

#include <string>

// to make standard C++ types more like Unreal types
using FString = std::string;
using int32 = int;

enum class EWordStatus {
	Invalid,
	OK,
	NotIsogram,
	NotLowercase,
	WrongLength,
	NullOrEmptyString
};

struct FBullCowCount
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

class FBullCowGame
{

public:
	// constructor
	FBullCowGame();

	/// <summary>
	/// Resets necessary variables for a new game (does not reset hidden word index)
	/// </summary>
	void Reset();

	/// <summary>
	/// Sets MyHiddenWord to the next word in the word list
	/// </summary>
	void UpdateHiddenWord();

	/// <summary>
	/// Returns the maximum number of tries based on hidden word length
	/// </summary>
	/// <returns>
	/// maximum number of tries for the length of MyHiddenWord
	/// </returns>
	int32 GetMaxTries() const;

	/// <summary>
	/// Returns the current try or guess number
	/// </summary>
	/// <returns>
	/// current player guess number
	/// </returns>
	int32 GetCurrentTry() const;

	/// <summary>
	/// Returns the length of the hidden word
	/// </summary>
	/// <returns>
	/// length of MyHiddenWord
	/// </returns>
	int32 GetHiddenWordLength() const;

	/// <summary>
	/// Determines if the player has won the game based on the current guess
	/// </summary>
	/// <returns>
	/// true if the player's guess is correct, otherwise false
	/// </returns>
	bool IsGameWon() const;

	/// <summary>
	/// Determines if the player's current guess has any duplicate letters
	/// </summary>
	/// <param name="Guess">string to evaluate</param>
	/// <returns>
	/// true if the string provided does NOT have duplicate letters, otherwise false
	/// </returns>
	bool IsIsogram(FString) const;

	/// <summary>
	/// Determines if the provided string is a valid guess based on case, length, duplicate letters
	/// </summary>
	/// <param name="Guess">string to evaluate</param>
	/// <returns>
	/// EWordStatus::OK
	/// EWordStatus::NullOrEmptyString
	/// EWordStatus::WrongLength
	/// EWordStatus::NotLowercase
	/// </returns>
	EWordStatus CheckGuessValidity(FString) const;

	/// <summary>
	/// Determines if the provided string has all lowercase letters
	/// </summary>
	/// <param name="Guess">string to evaluate</param>
	/// <returns>
	/// true if all letters in the provided string are lowercase, otherwise false
	/// </returns>
	bool IsLowercase(FString) const;

	/// <summary>
	/// Increments MyCurrentTry by 1, resets to 0 if over the hidden word array length
	/// </summary>
	void IncrementCurrentTry();

	/// <summary>
	/// Compare a string against the current hidden word and return number of bulls and cows
	/// NOTE: string should be pre-validated using CheckGuessValidity first
	/// </summary>
	/// <param name="Guess">string to submit</param>
	/// <returns>
	/// Number of bulls and cows awarded to the player based on the provided string
	/// </returns>
	FBullCowCount SubmitGuess(FString);



private:
	int32 MyWordIndex = -1;

	// initialized in constructor
	int32 MyCurrentTry;
	FString MyHiddenWord;
	bool bMyGameWon;
	bool HasDuplicatesTMap(FString Word) const;
	bool HasDuplicates(FString Word) const;
};