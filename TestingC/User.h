#include "Main.h"
class User : public Person {
private:
	static int user_id;
public:
	User() { // Конструктор
		ifstream file(users_file);
		if (!file.is_open()) // Проверка на наличие файла
			OpenFileError(users_file);
		else if (file.peek() == EOF) { // Проверка на наличие хотя бы одного аккаунта пользователя
			cout << "\nНе найдено ни одного пользователя!\n\tРЕГИСТРАЦИЯ\n";
			RegisterUser();
			system("cls");
		}
		else { // Выгрузка учетной записи
			string str, temp;
			while (getline(file, str))
				temp += str;
			if (!temp.empty())
				info_base = json::parse(temp);
		}
		file.close();
	}
	virtual void Login() { // Авторизация пользователя
		system("cls");
		ifstream file(users_file);
		if (!file.is_open()) // Проверка на наличие файла
			OpenFileError(users_file);
		else if (!isAuth) {
			string login, password;
			cout << "\n\tАВТОРИЗАЦИЯ ПОЛЬЗОВАТЕЛЯ\n";
			cout << "\n\tЛогин: ";
			getline(cin, login);
			cout << "\n\tПароль: ";
			getline(cin, password);
			system("cls");
			user_id = Check(login, password);
			if (user_id != -1) {
				SetConsoleTextAttribute(handle, FOREGROUND_GREEN);
				message = "\n\tУспешная авторизация!\n";
				isAuth = true;
				status = "Пользователь " + string(info_base[user_id]["name"]);
			}
			else {
				SetConsoleTextAttribute(handle, FOREGROUND_RED);
				message = "\n\tАвторизация не пройдена!\n";
			}
		}
		else {
			SetConsoleTextAttribute(handle, 33);
			message = "\n\tВы уже авторизованы!";
		}
		file.close();
	}
	void RegisterUser() { // Регистрация пользователя
		string login, password, FIO, adress, telephone;
		cout << "\nУкажите: ";
		cout << "\n\tЛогин: ";
		getline(cin, login);
		cout << "\n\tПароль: ";
		getline(cin, password);
		cout << "\n\tФИО: ";
		getline(cin, FIO);
		cout << "\n\tДомашний адрес: ";
		getline(cin, adress);
		cout << "\n\tТелефон: ";
		getline(cin, telephone);
		bool is_here = true;
		for (int i = 0; i < info_base.size(); i++)
			if (info_base[i]["login"] == Crypt(login))
				is_here = false;
		if (is_here) {
			info_base.push_back({
				{"login", Crypt(login)},
				{"password", Crypt(password)},
				{"name", FIO},
				{"adress", adress},
				{"telephone", telephone} });
			Save();
		}
		else {
			SetConsoleTextAttribute(handle, FOREGROUND_RED);
			message = "\nПользователь с таким логином существует!\n";
		}
	}
	void DeleteUser(int index) { // Удаление пользователя
		info_base.erase(index);
	}
	virtual int Check(string login, string password) { // Проверка совпадения логина и пароля
		for (size_t i = 0; i < info_base.size(); i++)
			if (info_base[i]["login"] == Crypt(login) && info_base[i]["password"] == Crypt(password))
				return i;
		return -1;
	}
	void Edit(int index) { // Изменение пользователя(полное стирание и перезапись)
		DeleteUser(index);
		RegisterUser();
	}
	virtual void LogOut() { // Выйти 
		status = "Не авторизован";
		isAuth = false;
		user_id = -1;
		message = {};
		system("cls");
	}
	virtual void Save() { // Сохранение базы пользователей в файл
		ofstream file_write(users_file);
		if (!file_write.is_open())
			OpenFileError(users_file);
		else
			file_write << info_base.dump(2);
		file_write.close();
	}
	static bool auth_pos() { return isAuth; } // Статус авторизации пользователя
	void Tests() { // Тестирование пользователя
		system("cls");
		json test = ReadTests(tests_file);
		vector<string> categories;
		for (const auto& n : test.items()) {
			categories.push_back(n.key());
			cout << "\t" << categories.size() << ". " << categories[categories.size() - 1] << endl;
			for (const auto& l : test[n.key()].items())
				cout << "\t\t" << l.key() << endl;
		}
		int cat = Menu(categories);
		cat++;
		vector<string> bot_categories;
		system("cls");
		if (cat > 0 && cat <= categories.size()) {
			cout << endl << categories[cat - 1] << endl;
			for (const auto& l : test[categories[cat - 1]].items()) {
				bot_categories.push_back(l.key());
				cout << "\t" << bot_categories.size() << ". " << bot_categories[bot_categories.size() - 1] << endl;
			}
			int bot_cat = Menu(bot_categories);
			bot_cat++;
			if (bot_cat > 0 && bot_cat <= bot_categories.size()) {
				double count = 0;
				int begin = 0;
				int my_answer;
				if (info_base[user_id]["tests"][categories[cat - 1]][bot_categories[bot_cat - 1]].contains("last question"))
					if (info_base[user_id]["tests"][categories[cat - 1]][bot_categories[bot_cat - 1]]["last question"] != info_base[user_id]["tests"][categories[cat - 1]][bot_categories[bot_cat - 1]]["num questions"]) {
						count = info_base[user_id]["tests"][categories[cat - 1]][bot_categories[bot_cat - 1]]["right answers"];
						begin = info_base[user_id]["tests"][categories[cat - 1]][bot_categories[bot_cat - 1]]["last question"];
					}
				for (int i = begin; i < test[categories[cat - 1]][bot_categories[bot_cat - 1]].size(); i++) {
					system("cls");
					message = categories[cat - 1] + " >>> " + string(bot_categories[bot_cat - 1]);
					status = (i + 1) + ". " + string(test[categories[cat - 1]][bot_categories[bot_cat - 1]][i]["question"]);
					my_answer = Menu({ " 1. " + string(test[categories[cat - 1]][bot_categories[bot_cat - 1]][i]["answers"][0]),  " 2. " + string(test[categories[cat - 1]][bot_categories[bot_cat - 1]][i]["answers"][1]), " 3. " + string(test[categories[cat - 1]][bot_categories[bot_cat - 1]][i]["answers"][2]), " <<< Пауза" });
					my_answer++;
					info_base[user_id]["tests"][categories[cat - 1]][bot_categories[bot_cat - 1]]["last question"] = i;
					if (my_answer == 4)
						break;
					else {
						cout << my_answer;
						if (test[categories[cat - 1]][bot_categories[bot_cat - 1]][i]["answers"][my_answer - 1] == test[categories[cat - 1]][bot_categories[bot_cat - 1]][i]["right answer"])
							count++;
					}
				}
				if (my_answer != 4)
					info_base[user_id]["tests"][categories[cat - 1]][bot_categories[bot_cat - 1]]["last question"] = info_base[user_id]["tests"][categories[cat - 1]][bot_categories[bot_cat - 1]]["last question"] + 1;
				info_base[user_id]["tests"][categories[cat - 1]][bot_categories[bot_cat - 1]]["num questions"] = test[categories[cat - 1]][bot_categories[bot_cat - 1]].size();
				info_base[user_id]["tests"][categories[cat - 1]][bot_categories[bot_cat - 1]]["right answers"] = (int)count;
				info_base[user_id]["tests"][categories[cat - 1]][bot_categories[bot_cat - 1]]["mark"] = (int)(12 * (double)(count / test[categories[cat - 1]][bot_categories[bot_cat - 1]].size()));
				Save();
				if (info_base[user_id]["tests"][categories[cat - 1]][bot_categories[bot_cat - 1]]["last question"] == info_base[user_id]["tests"][categories[cat - 1]][bot_categories[bot_cat - 1]]["num questions"]) {
					cout << "\nВопросов: " << test[categories[cat - 1]][bot_categories[bot_cat - 1]].size();
					cout << "\nПравильных ответов: " << count << "(" << (double)(count / test[categories[cat - 1]][bot_categories[bot_cat - 1]].size()) * 100 << "%)";
					cout << "\nОценка: " << 12 * (double)(count / test[categories[cat - 1]][bot_categories[bot_cat - 1]].size()) << endl;
					system("pause");
				}
			}
		}
		status = {};
		message = {};
	}
	virtual void ShowStatistic() { // Статистика конкретного пользователя
		if (info_base[user_id].contains("tests")) {
			for (const auto& categories : info_base[user_id]["tests"].items()) {
				cout << endl << categories.key() << ": " << endl;
				for (const auto& bot_categories : info_base[user_id]["tests"][categories.key()].items()) {
					cout << "\t" << bot_categories.key() << ": ";
					if (info_base[user_id]["tests"][categories.key()][bot_categories.key()]["last question"] == info_base[user_id]["tests"][categories.key()][bot_categories.key()]["num questions"]) {
						double right = info_base[user_id]["tests"][categories.key()][bot_categories.key()]["right answers"];
						double nums = info_base[user_id]["tests"][categories.key()][bot_categories.key()]["num questions"];
						cout << "\n\t\t\tВопросов: ";
						SetConsoleTextAttribute(handle, 14);
						cout << nums;
						SetConsoleTextAttribute(handle, 15);
						cout << "\n\t\t\tПравильных ответов: ";
						SetConsoleTextAttribute(handle, 14);
						cout << right << "(" << (right / nums) * 100 << "%)";
						SetConsoleTextAttribute(handle, 15);
						cout << "\n\t\t\tОценка: ";
						SetConsoleTextAttribute(handle, 14);
						cout << info_base[user_id]["tests"][categories.key()][bot_categories.key()]["mark"];
						SetConsoleTextAttribute(handle, 15);
					}
					else
						cout << "не закончено" << endl;
				}
			}
		}
		cout << endl;
		system("pause");
	}
	json GetUsers() { return info_base; } // Получение базы пользователей
	int UserChoice() { // Панель пользователя
		SetConsoleTextAttribute(handle, 15);
		int choice = Menu({ " Просмотр результатов", " Пройти тест", " Выйти из учетой записи" });
		message = {};
		return ++choice;
	}
};