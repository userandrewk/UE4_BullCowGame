// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"



void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    
    SetupGame();
    
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    ClearScreen();

    if (!bGameOver)
    {

        PrintLine(TEXT("Your lives: %i <3"), Lives);
        PrintLine(TEXT("The word contains %i letters, try to guees\n"), HiddenWord.Len());
        //PrintLine(TEXT("Bulls = %i  Cows = %i"), Bulls, Cows);

        if (PlayerInput == HiddenWord)
        {
            bGameOver = true;
            GameOver();
        }
        else if (PlayerInput.Len() != HiddenWord.Len() && PlayerInput.Len() != 0)
        {
            Lives--;

            ClearScreen();

            PrintLine(TEXT("- 1 LIFE\n"));

            PrintLine(TEXT("Your lives: %i <3"), Lives);
            PrintLine(TEXT("The word contains %i letters, try to guees\n"), HiddenWord.Len());

            if (Lives < 1)
            {
                bGameOver = true;
                GameOver();
            }
        }
        else
        {
            if (!IsIsogram(PlayerInput))
            {
                Lives--;

                ClearScreen();

                PrintLine(TEXT("- 1 LIFE\n"));

                PrintLine(TEXT("Your lives: %i <3"), Lives);
                PrintLine(TEXT("Your guess is not isogram, try again"));

                if (Lives < 1)
                {
                    bGameOver = true;
                    GameOver();
                    
                }
            }
            else if (PlayerInput.Len() != 0)
            {

                CheckBullCow(PlayerInput);
                if (Bulls == 0 && Cows == 0) 
                {
                    Lives--;

                    ClearScreen();

                    PrintLine(TEXT("- 1 LIFE\n"));
                    PrintLine(TEXT("Your lives: %i <3"), Lives);
                    PrintLine(TEXT("Bulls = %i  Cows = %i"), Bulls, Cows);
                    
                    if (Lives < 0)
                    {
                        bGameOver = true;
                        GameOver();

                    }

                }
                else
                {
                    ClearScreen();
                    PrintLine(TEXT("Your lives: %i <3"), Lives);
                    PrintLine(TEXT("Bulls = %i  Cows = %i"), Bulls, Cows);
                }
                Bulls = 0;
                Cows = 0;

            }
        }

    }
    else 
    {
        bGameOver = false;
        SetupGame();
    }
}

void UBullCowCartridge::SetupGame() 
{
    // prepare isogram words 
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("Texts/HiddenWordsList.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(Words, *WordListPath, [](const FString& Word)
        { return Word.Len() >= 3 && Word.Len() <=7 && IsIsogram(Word);});

    //Welcome message, instructions
    PrintLine(TEXT("Welcome to Bulls and Cows game Player!"));
    PrintLine(TEXT("Here we have a word to guees for you"));
    
    // choose random isogram to become a hidden word
    HiddenWord = Words[FMath::RandRange(0, Words.Num())];
    
    PrintLine(HiddenWord);// debug print
    Lives = 3;
    bGameOver = false;

    PrintLine(TEXT("The word contains %i letters, try to guees\n"), HiddenWord.Len());
    PrintLine(TEXT("Try to type smth \nto type press \"Tab\"\ntype and than press \"Enter\""));
}

void UBullCowCartridge::GameOver()
{
    ClearScreen();
    if (Lives > 0) 
    {
        PrintLine(TEXT("\nYOU WON\n"));

    }
    else
    {
        PrintLine(TEXT("\nYOU LOSE\n"));
        
    }

    PrintLine(TEXT("\nPress ENTER to play again\n"));
    
}

void UBullCowCartridge::CheckBullCow(const FString& PlayerInput)
{
    Bulls = 0;
    Cows = 0;

    for (size_t InputIndex = 0; InputIndex < PlayerInput.Len(); InputIndex++)
    {
        for (size_t HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (PlayerInput[InputIndex] == HiddenWord[HiddenIndex] && InputIndex == HiddenIndex)
            {
                Bulls++;
                
            }
            else if (PlayerInput[InputIndex] == HiddenWord[HiddenIndex])
            {
                Cows++;
                
            }
        }
    }
}

bool UBullCowCartridge::IsIsogram(const FString& PlayerInput)
{
    for (size_t i = 0; i < PlayerInput.Len(); i++)
    {
        for (size_t b = 0; b < PlayerInput.Len(); b++)
        {
            if (i != b && PlayerInput[i] == PlayerInput[b]) 
            {
                return false;
            }
        }
    }
    return true;
}