/*
main.cpp
Main executable responsible for executing the main game loop
Game description and logic is contained in FBullCowGame

ASCII art from http://ascii.co.uk

Last updated: 1/12/2019

THE MIT LICENSE
Copyright 2019 shark apps, LLC

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "pch.h"
#include <iostream>
#include <string>
#include "FBullCowGame.h"

// to make standard C++ types more like Unreal types
using FText = std::string;
using int32 = int;

// prototypes
void PrintIntro();
void ShowBull(int32);
void ShowCow(int32);
void ShowBullAndCow();
FText GetGuess();
void ValidateAndSubmitGuess(FText);
bool AskToPlayAgain();
void GameLoop();
void PlayGame();
void PrintGameSummary();

// Game instance
FBullCowGame BCGame;

int main()
{
	GameLoop();
	return 0;
}


/// <summary>
/// Main game loop that plays a round of the game and then asks the player to continue.  Quits if player choice is negative.
/// </summary>
void GameLoop()
{
	do
	{
		// introduce the game
		PrintIntro();
		PlayGame();
		PrintGameSummary();
	} while (AskToPlayAgain());
}


/// <summary>
/// Resets game state, selects the next challenge and plays a round
/// </summary>
void PlayGame()
{
	BCGame.Reset();
	BCGame.UpdateHiddenWord();

	std::cout << "\nCan you guess the " << BCGame.GetHiddenWordLength();
	std::cout << "-letter isogram I am thinking of?\n";
	std::cout << "- You have " << BCGame.GetMaxTries() << " tries to guess this isogram.\n";

	// loop asking for answers until out of tries or game is won
	int32 MaxTries = BCGame.GetMaxTries();
	while ((BCGame.GetCurrentTry() <= MaxTries) && !BCGame.IsGameWon())
	{
		// get a guess from the player
		FText Guess = GetGuess();
	}
}


/// <summary>
/// Outputs to the console the win/lose state of the current round
/// </summary>
void PrintGameSummary()
{
	if (BCGame.IsGameWon())
	{
		std::cout << "\nYou won the game!\n";
	}
	else {
		std::cout << "\nYou ran out of tries.\n";
	}
}

/// <summary>
/// Outputs to the console the game introduction
/// </summary>
void PrintIntro()
{
	std::cout << std::endl << std::endl;
	ShowBullAndCow();
	std::cout << "\n            BULLS & COWS\n";
	std::cout << "\n- An isogram is a word that does not contain any duplicate letters.\n";
	std::cout << "- If your guess has a correct letter, you will get 1 cow.\n";
	std::cout << "- If your guess has a correct letter and it is in the correct spot, you will get 1 bull.\n";
}

struct Bull {
	FString bull[9] =
	{
		"  ,           ,    ",
		" /             \\   ",
		"((__-^^-,-^^-__)   ",
		" `-_---' `---_-'   ",
		"  <__|o` 'o|__>    ",
		"     \\  ` /        ",
		"      ): :(        ",
		"      :o_o:        ",
		"       \"-\"         "
	};
};


struct Cow {
	FString cow[9] =
	{
		"                    ",
		"     /)   (\\        ",
		".-._((,~\"~.))_.-,   ",
		" `-.   e e   ,-'    ",
		"   / ,o---o. \\      ",
		"  ( ( .___. ) )     ",
		"   ) `-----' (      ",
		"  /`-._____.-'\\     ",
		"                    "
	};
};

void ShowBull(int32 count)
{
	if (count == 0)
		return;

	Bull MyBull;
	for (auto bull : MyBull.bull)
	{
		for (int32 i = 0; i < count; i++)
		{
			std::cout << bull;
		}
		std::cout << std::endl;
	}
}

void ShowCow(int32 count)
{
	if (count == 0)
		return;

	Cow MyCow;
	for (auto cow : MyCow.cow)
	{
		for (int32 i = 0; i < count; i++)
		{
			std::cout << cow;
		}
		std::cout << std::endl;
	}
}

void ShowBullAndCow()
{
	Bull MyBull;
	Cow MyCow;
	for (int32 i = 0; i < 9; i++)
	{
		std::cout << MyBull.bull[i] << MyCow.cow[i] << std::endl;
	}
}


/// <summary>
/// Prompts player for a valid isogram guess and evaluates it for a match against the current challenge
/// </summary>
/// <returns>
/// Guess input string from player
/// </returns>
FText GetGuess()
{
	FText _guess = "";
	std::cout << "\nWord Length : ";
	for (int32 i = 0; i < BCGame.GetHiddenWordLength(); i++)
	{
		std::cout << '-';
	}
	std::cout << "\nGuess " << BCGame.GetCurrentTry() << " of " << BCGame.GetMaxTries() << ": ";
	std::getline(std::cin, _guess);

	EWordStatus GuessStatus = BCGame.CheckGuessValidity(_guess);
	switch (GuessStatus)
	{
	case EWordStatus::NotIsogram:
		std::cout << "Guess is not an isogram - contains duplicate letters\n";
		break;
	case EWordStatus::NotLowercase:
		std::cout << "Guess contains uppercase letters\n";
		break;
	case EWordStatus::NullOrEmptyString:
		std::cout << "Guess is null or empty\n";
		break;
	case EWordStatus::WrongLength:
		std::cout << "Guess is the wrong length\n";
		break;

	case EWordStatus::OK:
		// submit the guess

		std::cout << "Guess " << BCGame.GetCurrentTry() << ": ";
		FBullCowCount BCCount = BCGame.SubmitGuess(_guess); // increments MyCurrentTry
		std::cout << "Bulls = " << BCCount.Bulls;
		std::cout << ", Cows = " << BCCount.Cows << std::endl;
		ShowBull(BCCount.Bulls);
		ShowCow(BCCount.Cows);
		break;
	}
	return _guess;
}

/// <summary>
/// Prompts player to play again with y/n choice
/// </summary>
/// <returns>
/// true if player response is 'y', false if player response is 'n'
/// </returns>
bool AskToPlayAgain()
{
	char c;
	do
	{
		std::cout << "\nPlay again? (y/n): ";
		FText Response = "";
		std::getline(std::cin, Response);
		c = tolower(Response[0]);
	} while (c != 'y' && c != 'n');

	return (c == 'y');
}

