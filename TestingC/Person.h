#include "Main.h"
class Person {
protected:
	json info_base{};
	static bool isAuth;
public:
	virtual void Login() = 0;
	virtual int Check(string login, string password) = 0;
	virtual void LogOut() = 0;
	virtual void Save() = 0;
	virtual void ShowStatistic() = 0;
	void OpenFileError(string _message) { // Вывод ошибки открытия файла
		SetConsoleTextAttribute(handle, FOREGROUND_RED);
		cerr << "Ошибка чтения файла \"" << _message << "\"" << endl;
		SetConsoleTextAttribute(handle, 15);
		system("pause");
	}
	string Crypt(string no_crypt_string) { // Шифрование строки
		for (int i = 0; i < no_crypt_string.length(); i++)
			no_crypt_string[i] = no_crypt_string[i] ^ crypt_key;
		return no_crypt_string;
	}
	json ReadTests(string dest) { // Чтение тестов из файла
		json temp_tests;
		ifstream file(dest);
		if (!file.is_open())
			OpenFileError(dest);
		else {
			string str, temp;
			while (getline(file, str))
				temp += str;
			if (!temp.empty())
				temp_tests = json::parse(temp);
		}
		file.close();
		return temp_tests;
	}
};