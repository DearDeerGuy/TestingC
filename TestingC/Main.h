#pragma once
#include <iostream>
#include <windows.h>
#include <fstream>
#include <conio.h>
#include <iomanip>
#include <string>
#include "nlohmann/json.hpp"
using namespace std;
using namespace nlohmann;

//---------------------- ВСПОМОГАТЕЛЬНЫЕ ПЕРЕМЕННЫЕ ----------------------
HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
string admin_file = "admin.json"; // Путь к логам админа
string users_file = "users.json"; // Путь к логам пользователей
string tests_file = "tests.json"; // Путь к тестам
int crypt_key = 15;				  // Ключ шифрования. После изменения ключа следует очистить файлы admin.json и users.json
string status = "Не авторизован"; // Статус пользователя
string message = {};			  // Сообщение
//---------------------- ВСПОМОГАТЕЛЬНЫЕ ПЕРЕМЕННЫЕ ----------------------
enum ConsoleColor { // Цвета
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};
void Show(vector<string> menu, int x) { // Показ меню
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	system("cls");
	cout << "\t" << message;
	SetConsoleTextAttribute(handle, 14);
	cout << "\t" << status << endl << endl;
	SetConsoleTextAttribute(handle, 15);
	for (int i = 0; i < menu.size(); i++) {
		if (i == x)
			SetConsoleTextAttribute(hConsole, (WORD)(LightCyan));
		cout << menu[i] << endl;
		SetConsoleTextAttribute(hConsole, (WORD)(White));
	}
}
int Menu(vector<string> menu) { // Взаимодействие с меню
	int x = 0;
	char symbol = 72;
	int size = menu.size();
	do {
		Show(menu, x);
		symbol = _getch();
		switch (symbol) {
		case 72: // Вверх
			x--;
			if (x == -1)
				x = (size - 1);
			break;
		case 80: // Вниз
			x++;
			if (x == size)
				x = 0;
			break;
		default:
			break;
		}
	} while (symbol != static_cast<char>(13));
	return x;
}