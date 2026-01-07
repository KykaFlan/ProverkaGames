#pragma once
#include <vector>

// Глобальные переменные
extern int KolichestvoPlayers;
extern int playerPositions[5];
extern int skipTurns[5];
extern bool frozen[5];
extern int currentPlayer;
extern bool gameOver;
extern char gameMap[50];
extern int winnerPlayer;

// Функции игры
void InitGame();
void StartGame();
void ProcessTurn();
void ShowGameState();
void ShowFinalMap();  // Новая функция
void ShowMapWithPlayers();
int RollDice();
void MovePlayer(int playerNum, int steps);
bool CheckWin(int playerNum);
void ProcessCell(int playerNum);
void NextPlayer();
int GetWinner();
int GetNumberInput(int minValue, int maxValue);
int ChoosePlayerToFreeze(int currentPlayerNum);
bool AskBonusQuestion();
int FindTeleportDestination(int position);
void DrawSeparator(int length = 40, char symbol = '-');  // Здесь можно оставить аргументы по умолчанию
void ShowTurnHeader(int turn);  // Новая функция