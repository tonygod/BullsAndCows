# BullsAndCows
Bulls and Cows C++ console game using ASCII art.  Built using lessons from Unreal Engine C++ course on Udemy

An adaptation of Bulls & Cows, the inspiration for Mastermind
(see https://en.wikipedia.org/wiki/Mastermind_(board_game))

A "hidden word" is chosen from a list of isograms (words without repeating letters).
The player is given the number of digits in the word and a number of guesses to figure
out what the hidden word is.  The number of guesses is determined by the length of the
hidden word.  When the player submits a guess, each letter is checked against
the hidden word and if the letter is contained in the hidden word and is in the correct spot,
a Bull is awarded.  If the letter is contained in the hidden word, but is not in the correct
spot, a cow is awarded.
