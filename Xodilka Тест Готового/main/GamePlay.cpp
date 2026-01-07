#include <iostream>
#include "GamePlay.h"
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

// Определяем глобальные переменные
int KolichestvoPlayers = 2;
int playerPositions[5] = { 0, 0, 0, 0, 0 };
int skipTurns[5] = { 0, 0, 0, 0, 0 };
bool frozen[5] = { false, false, false, false, false };
int currentPlayer = 0;
bool gameOver = false;
char gameMap[50];
int winnerPlayer = 0;

// Пара телепортов
int teleportPairs[1][2] = {
    {22, 37}
};

// Инициализация игры
void InitGame() {
    for (int i = 0; i < 50; i++) {
        gameMap[i] = '.';
    }

    gameMap[0] = 'S';
    gameMap[49] = 'F';

    gameMap[3] = 'J';
    gameMap[23] = 'J';
    gameMap[36] = 'J';
    gameMap[40] = 'J';

    gameMap[5] = 'B';
    gameMap[10] = 'B';
    gameMap[20] = 'B';
    gameMap[30] = 'B';

    gameMap[22] = 'P';
    gameMap[37] = 'P';

    gameMap[8] = '.';
    gameMap[42] = '.';

    gameMap[15] = 'B';
    gameMap[25] = 'B';
    gameMap[45] = 'B';
    gameMap[33] = 'B';
    gameMap[17] = 'B';

    for (int i = 0; i < 5; i++) {
        playerPositions[i] = 0;
        skipTurns[i] = 0;
        frozen[i] = false;
    }
    currentPlayer = 0;
    gameOver = false;
    winnerPlayer = 0;

    srand(time(0));
}

vector<int> GetPlayersOnCell(int cell) {
    vector<int> players;
    for (int i = 0; i < KolichestvoPlayers; i++) {
        if (playerPositions[i] == cell) {
            players.push_back(i + 1);
        }
    }
    return players;
}

int FindTeleportDestination(int position) {
    if (position == teleportPairs[0][0]) {
        return teleportPairs[0][1];
    }
    if (position == teleportPairs[0][1]) {
        return teleportPairs[0][0];
    }
    return position;
}

void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void ResetColor() {
    SetColor(7);
}

// Функция для рисования разделительной линии
void DrawSeparator(int length, char symbol) {
    for (int i = 0; i < length; i++) {
        cout << symbol;
    }
    cout << endl;
}

int GetNumberInput(int minValue, int maxValue) {
    int input;
    while (true) {
        cout << "Введите число от " << minValue << " до " << maxValue << ": ";
        cin >> input;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Ошибка! Введите число.\n";
        }
        else if (input < minValue || input > maxValue) {
            cout << "Число должно быть от " << minValue << " до " << maxValue << "!\n";
        }
        else {
            cin.ignore(1000, '\n');
            return input;
        }
    }
}

int ChoosePlayerToFreeze(int currentPlayerNum) {
    cout << "\n";
    DrawSeparator(45, '=');
    SetColor(13);
    cout << "   ВЫБЕРИТЕ ИГРОКА ДЛЯ ЗАМОРОЗКИ   \n";
    ResetColor();
    DrawSeparator(45, '=');

    SetColor(11);
    cout << "Доступные игроки:\n";
    ResetColor();

    for (int i = 0; i < KolichestvoPlayers; i++) {
        if (i != currentPlayerNum) {
            cout << "  ";
            SetColor(14);
            cout << "Игрок " << (i + 1);
            ResetColor();
            cout << " - клетка ";
            SetColor(13);
            cout << playerPositions[i];
            ResetColor();

            if (skipTurns[i] > 0) {
                SetColor(12);
                cout << " (пропуск: " << skipTurns[i] << ")";
                ResetColor();
            }
            if (frozen[i]) {
                SetColor(9);
                cout << " (заморожен)";
                ResetColor();
            }
            cout << "\n";
        }
    }

    DrawSeparator(45, '-');

    int choice;
    while (true) {
        choice = GetNumberInput(1, KolichestvoPlayers);

        if (choice == currentPlayerNum + 1) {
            cout << "Нельзя выбрать самого себя!\n";
        }
        else if (choice < 1 || choice > KolichestvoPlayers) {
            cout << "Такого игрока нет!\n";
        }
        else {
            break;
        }
    }

    return choice - 1;
}

void ShowGameState() {
    system("cls");

    SetColor(14);
    DrawSeparator(45, '=');
    cout << "         И Г Р А   ' Х О Д И Л К А '         \n";
    DrawSeparator(45, '=');
    ResetColor();

    cout << "  ";
    SetColor(11);
    cout << "Игроков: " << KolichestvoPlayers;
    SetColor(10);
    cout << "   |   Текущий: " << (currentPlayer + 1) << "\n";
    ResetColor();

    DrawSeparator(45, '-');

    SetColor(11);
    cout << "  Позиции игроков:\n";
    ResetColor();

    for (int i = 0; i < KolichestvoPlayers; i++) {
        cout << "  ";
        SetColor(14);
        cout << "Игрок " << (i + 1);
        ResetColor();
        cout << ": ";
        SetColor(13);
        cout << "клетка " << playerPositions[i];
        ResetColor();

        if (skipTurns[i] > 0) {
            SetColor(12);
            cout << " [пропуск: " << skipTurns[i] << "]";
            ResetColor();
        }
        if (frozen[i]) {
            SetColor(9);
            cout << " [заморожен]";
            ResetColor();
        }

        if (i == currentPlayer) {
            SetColor(10);
            cout << " <-- ХОДИТ";
            ResetColor();
        }
        cout << endl;
    }

    DrawSeparator(45, '=');
    cout << endl;
}

void ShowFinalMap() {
    const int N = 50;
    char displayMap[N];
    vector<int> playersOnCell[N];

    for (int i = 0; i < N; i++) {
        playersOnCell[i] = GetPlayersOnCell(i);
    }

    SetColor(14);
    DrawSeparator(45, '=');
    cout << "       Ф И Н А Л Ь Н А Я   К А Р Т А       \n";
    DrawSeparator(45, '=');
    ResetColor();

    for (int i = 0; i < 5; i++) {
        cout << "  ";

        if (i % 2 == 0) {
            for (int j = 0; j < 10; j++) {
                int index = i * 10 + j;
                if (index >= N) break;

                char cell = gameMap[index];
                vector<int> players = playersOnCell[index];

                cout << "[";

                if (!players.empty()) {
                    sort(players.begin(), players.end());

                    if (cell == 'J') SetColor(12);
                    else if (cell == 'B') SetColor(11);
                    else if (cell == 'P') SetColor(13);
                    else if (cell == 'F') SetColor(10);
                    else if (cell == 'S') SetColor(9);
                    else SetColor(7);

                    cout << cell;

                    SetColor(14);
                    for (int playerNum : players) {
                        cout << playerNum;
                    }
                    ResetColor();
                }
                else {
                    if (cell == 'J') SetColor(12);
                    else if (cell == 'B') SetColor(11);
                    else if (cell == 'P') SetColor(13);
                    else if (cell == 'F') SetColor(10);
                    else if (cell == 'S') SetColor(9);
                    else SetColor(8);

                    cout << cell;
                    ResetColor();
                }

                cout << "]";

                if (j != 9 && (i * 10 + j + 1) < N) {
                    cout << " -> ";
                }
            }
        }
        else {
            for (int j = 9; j >= 0; j--) {
                int index = i * 10 + j;
                if (index >= N) continue;

                char cell = gameMap[index];
                vector<int> players = playersOnCell[index];

                cout << "[";

                if (!players.empty()) {
                    sort(players.begin(), players.end());

                    if (cell == 'J') SetColor(12);
                    else if (cell == 'B') SetColor(11);
                    else if (cell == 'P') SetColor(13);
                    else if (cell == 'F') SetColor(10);
                    else if (cell == 'S') SetColor(9);
                    else SetColor(7);

                    cout << cell;

                    SetColor(14);
                    for (int playerNum : players) {
                        cout << playerNum;
                    }
                    ResetColor();
                }
                else {
                    if (cell == 'J') SetColor(12);
                    else if (cell == 'B') SetColor(11);
                    else if (cell == 'P') SetColor(13);
                    else if (cell == 'F') SetColor(10);
                    else if (cell == 'S') SetColor(9);
                    else SetColor(8);

                    cout << cell;
                    ResetColor();
                }

                cout << "]";

                if (j != 0) {
                    cout << " <- ";
                }
            }
        }

        cout << "\n\n";
    }

    cout << "  ";
    SetColor(9); cout << "S"; ResetColor(); cout << "-старт  ";
    SetColor(10); cout << "F"; ResetColor(); cout << "-финиш  ";
    SetColor(11); cout << "B"; ResetColor(); cout << "-бонус  ";
    SetColor(12); cout << "J"; ResetColor(); cout << "-ловушка  ";
    SetColor(13); cout << "P"; ResetColor(); cout << "-телепорт\n";

    DrawSeparator(45, '=');
    cout << endl;
}

void ShowMapWithPlayers() {
    const int N = 50;
    char displayMap[N];
    vector<int> playersOnCell[N];

    for (int i = 0; i < N; i++) {
        playersOnCell[i] = GetPlayersOnCell(i);
    }

    for (int i = 0; i < 5; i++) {
        cout << "  ";

        if (i % 2 == 0) {
            for (int j = 0; j < 10; j++) {
                int index = i * 10 + j;
                if (index >= N) break;

                char cell = gameMap[index];
                vector<int> players = playersOnCell[index];

                cout << "[";

                if (!players.empty()) {
                    sort(players.begin(), players.end());

                    if (cell == 'J') SetColor(12);
                    else if (cell == 'B') SetColor(11);
                    else if (cell == 'P') SetColor(13);
                    else if (cell == 'F') SetColor(10);
                    else if (cell == 'S') SetColor(9);
                    else SetColor(7);

                    cout << cell;

                    SetColor(14);
                    for (int playerNum : players) {
                        cout << playerNum;
                    }
                    ResetColor();
                }
                else {
                    if (cell == 'J') SetColor(12);
                    else if (cell == 'B') SetColor(11);
                    else if (cell == 'P') SetColor(13);
                    else if (cell == 'F') SetColor(10);
                    else if (cell == 'S') SetColor(9);
                    else SetColor(8);

                    cout << cell;
                    ResetColor();
                }

                cout << "]";

                if (j != 9 && (i * 10 + j + 1) < N) {
                    cout << " -> ";
                }
            }
        }
        else {
            for (int j = 9; j >= 0; j--) {
                int index = i * 10 + j;
                if (index >= N) continue;

                char cell = gameMap[index];
                vector<int> players = playersOnCell[index];

                cout << "[";

                if (!players.empty()) {
                    sort(players.begin(), players.end());

                    if (cell == 'J') SetColor(12);
                    else if (cell == 'B') SetColor(11);
                    else if (cell == 'P') SetColor(13);
                    else if (cell == 'F') SetColor(10);
                    else if (cell == 'S') SetColor(9);
                    else SetColor(7);

                    cout << cell;

                    SetColor(14);
                    for (int playerNum : players) {
                        cout << playerNum;
                    }
                    ResetColor();
                }
                else {
                    if (cell == 'J') SetColor(12);
                    else if (cell == 'B') SetColor(11);
                    else if (cell == 'P') SetColor(13);
                    else if (cell == 'F') SetColor(10);
                    else if (cell == 'S') SetColor(9);
                    else SetColor(8);

                    cout << cell;
                    ResetColor();
                }

                cout << "]";

                if (j != 0) {
                    cout << " <- ";
                }
            }
        }

        cout << "\n\n";
    }

    cout << "  ";
    SetColor(9); cout << "S"; ResetColor(); cout << "-старт  ";
    SetColor(10); cout << "F"; ResetColor(); cout << "-финиш  ";
    SetColor(11); cout << "B"; ResetColor(); cout << "-бонус  ";
    SetColor(12); cout << "J"; ResetColor(); cout << "-ловушка  ";
    SetColor(13); cout << "P"; ResetColor(); cout << "-телепорт\n";

    DrawSeparator(45, '=');
    cout << endl;
}

int RollDice() {
    return rand() % 6 + 1;
}

void MovePlayer(int playerNum, int steps) {
    int oldPos = playerPositions[playerNum];
    playerPositions[playerNum] += steps;

    if (playerPositions[playerNum] > 49) {
        playerPositions[playerNum] = 49;
    }

    cout << "  ";
    SetColor(14);
    cout << "Игрок " << (playerNum + 1);
    ResetColor();
    cout << " двигается: ";
    SetColor(8);
    cout << oldPos;
    ResetColor();
    cout << " -> ";
    SetColor(13);
    cout << playerPositions[playerNum];
    ResetColor();
    cout << "\n";
}

// ГЛАВНОЕ ИСПРАВЛЕНИЕ: функция CheckWin теперь правильно завершает игру
bool CheckWin(int playerNum) {
    // Игрок побеждает, если достиг клетки 49 (финиш) или перешел ее
    if (playerPositions[playerNum] >= 49) {
        winnerPlayer = playerNum + 1;
        gameOver = true;  // Добавил эту строку - теперь игра завершается
        return true;
    }
    return false;
}

// Сложные вопросы
bool AskBonusQuestion() {
    int questionType = rand() % 10;
    int answer = 0;

    cout << "\n";
    DrawSeparator(45, '=');
    SetColor(11);
    cout << "        БОНУСНЫЙ ВОПРОС        \n";
    ResetColor();
    DrawSeparator(45, '=');

    SetColor(14);
    switch (questionType) {
    case 0:
        cout << "Чему равно число Пи с точностью до двух знаков?\n";
        cout << "1) 3.14  2) 3.16  3) 3.18: ";
        break;
    case 1:
        cout << "Сколько хромосом у человека?\n";
        cout << "1) 44  2) 46  3) 48: ";
        break;
    case 2:
        cout << "Кто написал 'Мастер и Маргарита'?\n";
        cout << "1) Булгаков  2) Достоевский  3) Толстой: ";
        break;
    case 3:
        cout << "Сколько будет 7! (факториал)?\n";
        cout << "1) 5040  2) 720  3) 40320: ";
        break;
    case 4:
        cout << "Какой химический элемент имеет символ 'Fe'?\n";
        cout << "1) Фтор  2) Железо  3) Фермий: ";
        break;
    case 5:
        cout << "Какой язык является родным для компьютера?\n";
        cout << "1) Java  2) Машинный код  3) Python: ";
        break;
    case 6:
        cout << "Год основания Москвы?\n";
        cout << "1) 1147  2) 1247  3) 1347: ";
        break;
    case 7:
        cout << "Сколько планет в Солнечной системе?\n";
        cout << "1) 8  2) 9  3) 10: ";
        break;
    case 8:
        cout << "Кто открыл закон всемирного тяготения?\n";
        cout << "1) Ньютон  2) Эйнштейн  3) Галилей: ";
        break;
    case 9:
        cout << "Сколько бит в одном байте?\n";
        cout << "1) 8  2) 16  3) 32: ";
        break;
    }
    ResetColor();

    answer = GetNumberInput(1, 3);

    DrawSeparator(45, '-');

    SetColor(10);
    switch (questionType) {
    case 0:
        if (answer == 1) {
            cout << "Верно! Пи ? 3.14\n";
            return true;
        }
        else {
            cout << "Неверно. Правильный ответ: 3.14\n";
            return false;
        }
    case 1:
        if (answer == 2) {
            cout << "Верно! 46 хромосом\n";
            return true;
        }
        else {
            cout << "Неверно. Правильный ответ: 46\n";
            return false;
        }
    case 2:
        if (answer == 1) {
            cout << "Верно! Михаил Булгаков\n";
            return true;
        }
        else {
            cout << "Неверно. Правильный ответ: Булгаков\n";
            return false;
        }
    case 3:
        if (answer == 1) {
            cout << "Верно! 7! = 5040\n";
            return true;
        }
        else {
            cout << "Неверно. 7! = 5040\n";
            return false;
        }
    case 4:
        if (answer == 2) {
            cout << "Верно! Fe - Железо\n";
            return true;
        }
        else {
            cout << "Неверно. Fe - Железо (Ferrum)\n";
            return false;
        }
    case 5:
        if (answer == 2) {
            cout << "Верно! Машинный код\n";
            return true;
        }
        else {
            cout << "Неверно. Машинный код (бинарный)\n";
            return false;
        }
    case 6:
        if (answer == 1) {
            cout << "Верно! 1147 год\n";
            return true;
        }
        else {
            cout << "Неверно. Москва основана в 1147\n";
            return false;
        }
    case 7:
        if (answer == 1) {
            cout << "Верно! 8 планет\n";
            return true;
        }
        else {
            cout << "Неверно. 8 планет (Плутон - карликовая)\n";
            return false;
        }
    case 8:
        if (answer == 1) {
            cout << "Верно! Исаак Ньютон\n";
            return true;
        }
        else {
            cout << "Неверно. Исаак Ньютон\n";
            return false;
        }
    case 9:
        if (answer == 1) {
            cout << "Верно! 8 бит = 1 байт\n";
            return true;
        }
        else {
            cout << "Неверно. 8 бит = 1 байт\n";
            return false;
        }
    default:
        return false;
    }
}

void ProcessCell(int playerNum) {
    int pos = playerPositions[playerNum];
    char cell = gameMap[pos];

    cout << "\n";
    DrawSeparator(40, '-');
    SetColor(13);
    cout << "         ЭФФЕКТ КЛЕТКИ " << pos << "\n";
    ResetColor();
    DrawSeparator(40, '-');

    int messageColor = 7;

    if (cell == 'J') messageColor = 12;
    else if (cell == 'B') messageColor = 11;
    else if (cell == 'P') messageColor = 13;
    else if (cell == 'F') messageColor = 10;
    else if (cell == 'S') messageColor = 9;

    SetColor(messageColor);

    if (cell == '.') {
        cout << "  Обычная клетка. Ничего не происходит.\n";
    }
    else if (cell == 'J') {
        cout << "  ЛОВУШКА! Пропуск 1 хода.\n";
        skipTurns[playerNum]++;
    }
    else if (cell == 'B') {
        cout << "  БОНУС! Ответьте на вопрос.\n";
        ResetColor();

        if (AskBonusQuestion()) {
            SetColor(10);
            cout << "\n  ПРАВИЛЬНО! Вы получаете награду!\n";
            ResetColor();

            if (rand() % 2 == 0) {
                cout << "  Награда: +2 шага вперед!\n";
                MovePlayer(playerNum, 2);
            }
            else {
                cout << "  Награда: Заморозка другого игрока!\n";

                if (KolichestvoPlayers == 1) {
                    SetColor(10);
                    cout << "  Но вы единственный игрок, получаете +2 шага!\n";
                    ResetColor();
                    MovePlayer(playerNum, 2);
                }
                else {
                    int playerToFreeze = ChoosePlayerToFreeze(playerNum);

                    if (playerToFreeze >= 0 && playerToFreeze < KolichestvoPlayers && playerToFreeze != playerNum) {
                        SetColor(9);
                        cout << "  Игрок " << (playerToFreeze + 1) << " заморожен на 1 ход!\n";
                        frozen[playerToFreeze] = true;
                        ResetColor();
                    }
                }
            }
        }
        else {
            SetColor(12);
            cout << "\n  НЕПРАВИЛЬНО! Остаетесь на месте.\n";
        }
    }
    else if (cell == 'P') {
        cout << "  ТЕЛЕПОРТ! Мгновенное перемещение.\n";
        ResetColor();

        int destination = FindTeleportDestination(pos);
        int oldPos = playerPositions[playerNum];

        if (destination != oldPos) {
            cout << "  Перемещение: " << oldPos << " -> " << destination << "\n";
            playerPositions[playerNum] = destination;

            SetColor(13);
            cout << "  Телепорты связаны: " << oldPos << " ? " << destination << "\n";
            ResetColor();

            char newCell = gameMap[destination];
            if (newCell != '.' && newCell != 'S' && newCell != 'P') {
                cout << "  Новая клетка: ";

                if (newCell == 'J') SetColor(12);
                else if (newCell == 'B') SetColor(11);
                else if (newCell == 'F') SetColor(10);

                cout << newCell;
                ResetColor();
                cout << "\n";

                if (newCell == 'J') {
                    SetColor(12);
                    cout << "  ЛОВУШКА! Пропуск 1 хода.\n";
                    skipTurns[playerNum]++;
                }
                else if (newCell == 'B') {
                    SetColor(11);
                    cout << "  БОНУС! Сразу отвечайте на вопрос.\n";
                    ResetColor();
                    if (AskBonusQuestion()) {
                        cout << "  Правильно! +2 шага!\n";
                        MovePlayer(playerNum, 2);
                    }
                }
                else if (newCell == 'F') {
                    SetColor(10);
                    cout << "  ФИНИШ! Поздравляем!\n";
                }
                ResetColor();
            }
        }
    }
    else if (cell == 'F') {
        cout << "  ФИНИШ! Вы достигли конечной клетки!\n";
    }
    else if (cell == 'S') {
        cout << "  СТАРТ. Начальная позиция.\n";
    }

    ResetColor();
    DrawSeparator(40, '-');
}

void NextPlayer() {
    currentPlayer = (currentPlayer + 1) % KolichestvoPlayers;
}

void ShowTurnHeader(int turn) {
    cout << "\n";
    DrawSeparator(45, '=');
    SetColor(10);
    cout << "           Х О Д   № " << turn << "           \n";
    ResetColor();
    DrawSeparator(45, '=');
}

void ProcessTurn() {
    if (skipTurns[currentPlayer] > 0) {
        cout << "\n";
        DrawSeparator(40, '=');
        SetColor(12);
        cout << "  Игрок " << (currentPlayer + 1) << " пропускает ход!\n";
        cout << "  Осталось пропусков: " << skipTurns[currentPlayer] << "\n";
        ResetColor();
        DrawSeparator(40, '=');
        skipTurns[currentPlayer]--;
        return;
    }

    if (frozen[currentPlayer]) {
        cout << "\n";
        DrawSeparator(40, '=');
        SetColor(9);
        cout << "  Игрок " << (currentPlayer + 1) << " заморожен!\n";
        cout << "  Пропуск этого хода.\n";
        ResetColor();
        DrawSeparator(40, '=');
        frozen[currentPlayer] = false;
        return;
    }

    // Бросок кубика
    cout << "\n";
    DrawSeparator(35, '-');
    SetColor(14);
    cout << "    БРОСОК КУБИКА\n";
    ResetColor();
    DrawSeparator(35, '-');

    cout << "  Игрок ";
    SetColor(14);
    cout << (currentPlayer + 1);
    ResetColor();
    cout << ", нажмите любую клавишу...";
    _getch();

    int dice = RollDice();
    cout << "\n  Выпало: ";
    SetColor(13);
    cout << "[" << dice << "]";
    ResetColor();
    cout << "\n";
    DrawSeparator(35, '-');

    MovePlayer(currentPlayer, dice);

    // ПРОВЕРЯЕМ ПОБЕДУ СРАЗУ ПОСЛЕ ДВИЖЕНИЯ
    if (CheckWin(currentPlayer)) {
        cout << "\n";
        DrawSeparator(50, '=');
        SetColor(14);
        cout << "  ПОБЕДИТЕЛЬ: ИГРОК " << (currentPlayer + 1) << "!\n";
        ResetColor();
        DrawSeparator(50, '=');
        return;
    }

    // Обрабатываем клетку только если игрок еще не выиграл
    if (!gameOver) {
        ProcessCell(currentPlayer);
    }

    // ПРОВЕРЯЕМ ПОБЕДУ ПОСЛЕ ОБРАБОТКИ КЛЕТКИ
    if (CheckWin(currentPlayer)) {
        cout << "\n";
        DrawSeparator(50, '=');
        SetColor(14);
        cout << "  ПОБЕДИТЕЛЬ: ИГРОК " << (currentPlayer + 1) << "!\n";
        ResetColor();
        DrawSeparator(50, '=');
        return;
    }
}

void StartGame() {
    InitGame();
    int turn = 1;

    while (!gameOver) {
        ShowGameState();
        ShowMapWithPlayers();

        ShowTurnHeader(turn);
        ProcessTurn();

        if (gameOver) {
            // Пауза перед показом финальной карты
            cout << "\nНажмите любую клавишу для просмотра финальной карты...";
            _getch();

            // Показываем финальную карту перед завершением
            system("cls");
            ShowFinalMap();

            cout << "\nНажмите любую клавишу для завершения...";
            _getch();
            break;
        }

        NextPlayer();
        turn++;

        cout << "\nНажмите любую клавишу для продолжения...";
        _getch();
    }
}

int GetWinner() {
    return winnerPlayer;
}