#include "raylib.h"
#include <stdio.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 650

int GetWinner(int p1, int p2)
{
    if (p1 == p2) return 0;

    switch(p1)
    {
        case 1: return (p2 == 3 || p2 == 4) ? 1 : 2;
        case 2: return (p2 == 1 || p2 == 5) ? 1 : 2;
        case 3: return (p2 == 2 || p2 == 4) ? 1 : 2;
        case 4: return (p2 == 2 || p2 == 5) ? 1 : 2;
        case 5: return (p2 == 1 || p2 == 3) ? 1 : 2;
    }
    return 0;
}

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "RPSLS - Hidden Choice Version");
    SetTargetFPS(60);

    int gameMode = 0; 
    // 0=mode select
    // 1=round select
    // 2=player1 turn
    // 3=hide screen
    // 4=player2 turn
    // 5=result
    // 6=final

    int multiplayer = 0;
    int totalRounds = 3;
    int currentRound = 1;

    int score1 = 0, score2 = 0;
    int p1Choice = 0, p2Choice = 0;
    int result = -1;

    Rectangle modeButtons[2] = {
        {350, 250, 300, 60},
        {350, 350, 300, 60}
    };

    Rectangle roundButtons[3] = {
        {350, 250, 100, 60},
        {470, 250, 100, 60},
        {590, 250, 100, 60}
    };

    Rectangle choiceButtons[5] = {
        {150, 450, 120, 50},
        {300, 450, 120, 50},
        {450, 450, 120, 50},
        {600, 450, 120, 50},
        {750, 450, 120, 50}
    };

    const char *choices[5] = {"Rock", "Paper", "Scissors", "Lizard", "Spock"};

    while (!WindowShouldClose())
    {
        Vector2 mouse = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (gameMode == 0)
            {
                if (CheckCollisionPointRec(mouse, modeButtons[0]))
                {
                    multiplayer = 0;
                    gameMode = 1;
                }
                if (CheckCollisionPointRec(mouse, modeButtons[1]))
                {
                    multiplayer = 1;
                    gameMode = 1;
                }
            }

            else if (gameMode == 1)
            {
                for (int i = 0; i < 3; i++)
                {
                    if (CheckCollisionPointRec(mouse, roundButtons[i]))
                    {
                        totalRounds = (i == 0) ? 3 : (i == 1) ? 5 : 7;
                        gameMode = 2;
                    }
                }
            }

            else if (gameMode == 2)
            {
                for (int i = 0; i < 5; i++)
                {
                    if (CheckCollisionPointRec(mouse, choiceButtons[i]))
                    {
                        p1Choice = i + 1;

                        if (multiplayer)
                            gameMode = 3;   // hide screen
                        else
                        {
                            p2Choice = GetRandomValue(1, 5);
                            result = GetWinner(p1Choice, p2Choice);
                            if (result == 1) score1++;
                            else if (result == 2) score2++;
                            gameMode = 5;
                        }
                    }
                }
            }

            else if (gameMode == 3)
            {
                gameMode = 4;   // move to player 2 turn
            }

            else if (gameMode == 4)
            {
                for (int i = 0; i < 5; i++)
                {
                    if (CheckCollisionPointRec(mouse, choiceButtons[i]))
                    {
                        p2Choice = i + 1;
                        result = GetWinner(p1Choice, p2Choice);
                        if (result == 1) score1++;
                        else if (result == 2) score2++;
                        gameMode = 5;
                    }
                }
            }

            else if (gameMode == 5)
            {
                currentRound++;
                p1Choice = 0;
                p2Choice = 0;

                if (currentRound > totalRounds)
                    gameMode = 6;
                else
                    gameMode = 2;
            }

            else if (gameMode == 6)
            {
                currentRound = 1;
                score1 = 0;
                score2 = 0;
                gameMode = 0;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("ROCK PAPER SCISSORS LIZARD SPOCK", 200, 50, 30, DARKBLUE);

        if (gameMode == 0)
        {
            DrawText("Select Game Mode", 370, 180, 30, BLACK);
            DrawRectangleRec(modeButtons[0], LIGHTGRAY);
            DrawText("Play vs Computer", 400, 270, 25, BLACK);
            DrawRectangleRec(modeButtons[1], LIGHTGRAY);
            DrawText("Multiplayer (2 Players)", 370, 370, 25, BLACK);
        }

        else if (gameMode == 1)
        {
            DrawText("Select Number of Rounds", 330, 180, 30, BLACK);
            const char *roundLabels[3] = {"3 Rounds", "5 Rounds", "7 Rounds"};
            for (int i = 0; i < 3; i++)
            {
                DrawRectangleRec(roundButtons[i], LIGHTGRAY);
                DrawText(roundLabels[i], roundButtons[i].x + 5, roundButtons[i].y + 20, 20, BLACK);
            }
        }

        else if (gameMode == 2)
        {
            DrawText("Player 1 - Choose", 380, 180, 30, GREEN);
            for (int i = 0; i < 5; i++)
            {
                DrawRectangleRec(choiceButtons[i], LIGHTGRAY);
                DrawText(choices[i], choiceButtons[i].x + 15, choiceButtons[i].y + 15, 20, BLACK);
            }
        }

        else if (gameMode == 3)
        {
            DrawText("Pass to Player 2", 380, 250, 30, BLACK);
            DrawText("Player 1 choice is hidden", 340, 300, 25, DARKGRAY);
            DrawText("Click to continue", 400, 350, 20, GRAY);
        }

        else if (gameMode == 4)
        {
            DrawText("Player 2 - Choose", 380, 180, 30, RED);
            for (int i = 0; i < 5; i++)
            {
                DrawRectangleRec(choiceButtons[i], LIGHTGRAY);
                DrawText(choices[i], choiceButtons[i].x + 15, choiceButtons[i].y + 15, 20, BLACK);
            }
        }

        else if (gameMode == 5)
        {
            DrawText("Round Result", 400, 150, 30, BLACK);
            DrawText(TextFormat("Player 1 chose: %s", choices[p1Choice-1]), 300, 250, 25, GREEN);
            DrawText(TextFormat("Player 2 chose: %s", choices[p2Choice-1]), 300, 290, 25, RED);

            if (result == 0)
                DrawText("Draw!", 450, 350, 30, ORANGE);
            else if (result == 1)
                DrawText("Player 1 Wins!", 400, 350, 30, GREEN);
            else
                DrawText("Player 2 Wins!", 400, 350, 30, RED);

            DrawText("Click for next round", 380, 450, 20, DARKGRAY);
        }

        else if (gameMode == 6)
        {
            DrawText("GAME OVER", 380, 150, 40, BLACK);

            if (score1 > score2)
                DrawText("Player 1 Wins The Game!", 320, 300, 30, GREEN);
            else if (score2 > score1)
                DrawText("Player 2 Wins The Game!", 320, 300, 30, RED);
            else
                DrawText("Game Draw!", 420, 300, 30, ORANGE);

            DrawText("Click to Restart", 420, 450, 20, DARKGRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}