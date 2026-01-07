#include <iostream>
#include "ProverkaInput.h"
#include <cstdlib>

using namespace std;

int CheckKolichestvoPlayers(int KolichestvoPlayers) {
    bool ok = false;

    while (!ok && (2 > KolichestvoPlayers || KolichestvoPlayers > 5)) {
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
        }
        else if (2 > KolichestvoPlayers || KolichestvoPlayers > 5) {
            cout << "Количество игроков не находится в диапозоне от 2 до 5 включительно.\n";
            cout << "Пожалуйства повторите ввод. ";
            cout << "Сколько игроков будет участвать в игре?\n";
            cout << "Количество: ";
            cin >> KolichestvoPlayers;
            system("cls");
        }
        else {
            ok = true;
        }
    }
	return KolichestvoPlayers;
}