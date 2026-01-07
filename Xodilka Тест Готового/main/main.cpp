#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <windows.h>

#include "ProverkaInput.h"
#include "OutConsol.h"
#include "GamePlay.h"


using namespace std;

int main()
{
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    WelcomeX();

    cout << "Сколько игроков будет участвовать в игре? (минимум 2, максимум 5 игроков)\n";
    int KolichestvoPlayers;
    cout << "Количество: ";
    cin >> KolichestvoPlayers;
    cout << "\n";

    system("cls");

    // Проверяем ввод и сохраняем в глобальную переменную
    ::KolichestvoPlayers = CheckKolichestvoPlayers(KolichestvoPlayers);
    cout << "\n\n\n\n\n\n\n\n                    Отлично! Участников игры: " << ::KolichestvoPlayers;
    cout << "\n\n\n\n\n\nЧтобы продолжить нажмите любую клавишу...";
    _getch();

    system("cls");

    GameRules();
    cout << "\nЧтобы продолжить нажмите любую клавишу...";
    _getch();
    system("cls");

    cout << "\n\n\n\n\n\n\n\n                           Игра начинается...\n\n\n\n\n";
    cout << "Нажмите любую клавишу...";
    _getch();
    system("cls");




    // ЗАПУСКАЕМ ИГРУ
    StartGame();

    cout << "\n\n\nНажмите любую клавишу для завершения...";
    _getch();
    system("cls");

    
    // Получаем победителя и передаем в EndGame
    int winner = GetWinner();
    EndGame(winner);
    _getch();

    return 0;
}