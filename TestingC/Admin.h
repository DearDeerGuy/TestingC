#include "Main.h"
class Admin : public Person {
public:
	Admin() { // Конструктор
		ifstream file(admin_file);
		if (!file.is_open()) // Проверка на наличие файла
			OpenFileError(admin_file);
		else if (file.peek() == EOF) { // Проверка на наличие аккаунта админа
			cout << "\nАккаунт администратора не найден! \n\tРЕГИСТРАЦИЯ\n";
			string login, password;
			cout << "\nУкажите: ";
			cout << "\n\tЛогин: ";
			getline(cin, login);
			cout << "\n\tПароль: ";
			getline(cin, password);
			info_base.push_back({
				{"login", Crypt(login)},
				{"password", Crypt(password)} });
			Save();
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
	virtual void Login() { // Авторизация
		system("cls");
		ifstream file(admin_file);
		if (!file.is_open()) // Проверка на наличие файла
			OpenFileError(admin_file);
		else if (!isAuth) {
			string login, password;
			cout << "\n\tАВТОРИЗАЦИЯ АДМИНИСТРАТОРА\n";
			cout << "\n\tЛогин: ";
			getline(cin, login);
			cout << "\n\tПароль: ";
			getline(cin, password);
			system("cls");
			if (Check(login, password)) {
				SetConsoleTextAttribute(handle, FOREGROUND_GREEN);
				message = "\n\tУспешная авторизация!\n";
				isAuth = true;
				status = "АДМИНИСТРАТОР";
			}
			else {
				SetConsoleTextAttribute(handle, FOREGROUND_RED);
				message = "\n\tАвторизация не пройдена!\n";
			}
		}
		else {
			message = "\n\tВы уже авторизованы!";
			SetConsoleTextAttribute(handle, 33);
		}
		file.close();
	}
	virtual void Save() { // Сохранение данных в файл
		ofstream file_write(admin_file);
		if (!file_write.is_open())
			OpenFileError(admin_file);
		else
			file_write << info_base.dump(2);
		file_write.close();
	}
	virtual int Check(string login, string password) { // Проверка совпадения логина и пароля
		if (info_base[0]["login"] == Crypt(login) && info_base[0]["password"] == Crypt(password))
			return 1;
		else
			return 0;
	}
	void Change() { // Смена пароля
		system("cls");
		if (!isAuth)
			message = "\n\tТребуется авторизоваться для смены данных\n";
		else {
			string login, password;
			cout << "\n\tИЗМЕНЕНИЕ АДМИНИСТРАТОРА\n";
			cout << "\nУкажите новые: ";
			cout << "\n\tЛогин: ";
			getline(cin, login);
			cout << "\n\tПароль: ";
			getline(cin, password);
			info_base[0] = {
				{"login", Crypt(login)},
				{"password", Crypt(password)} };
			Save();
			system("cls");
		}
	}
	virtual void LogOut() { // Выйти 
		status = "Не авторизован";
		isAuth = false;
		message = {};
		system("cls");
	}
	void UsersControlPanel() { // Панель управления пользователями
		User u;
		int choice = Menu({ " <<< Выход", " Зарегистрировать пользователя", " Удалить пользователя", " Изменить пользователя" });
		system("cls");
		switch (choice) {
		case 1: {
			u.RegisterUser();
		}
			  break;
		case 2: {
			string login;
			cout << "Укажите логин пользователя: ";
			getline(cin, login);
			u.DeleteUser(login);
		}
			  break;
		case 3: {
			string login;
			cout << "Укажите логин пользователя: ";
			getline(cin, login);
			u.Edit(login);
		}
		}
		u.Save();
	}
	void TestControl() { // Добавление тестов
		json temp_tests = ReadTests(tests_file);
		system("cls");
		cout << "\n\tТЕСТЫ\n\n";
		//Показ категорий и тестов
		json test = ReadTests(tests_file);
		vector<string> categories;
		int cat;
		for (const auto& n : test.items()) {
			categories.push_back(n.key());
			cout << "\t" << categories.size() << ". " << categories[categories.size() - 1] << endl;
			for (const auto& l : test[n.key()].items())
				cout << "\t\t" << l.key() << endl;
		}
		int choice = Menu({ " <<< Выход", " Добавить категорию", " Добавить тест в существующую категорию" });
		switch (choice) {
		case 1: {
			string category;
			cout << "\nУкажите название категории: ";
			getline(cin, category);
			temp_tests[category];
		}
			  break;
		case 2: {
			string answers[3], question, test_name;
			int que_num, cat_ind;
			cout << "\nДалее заполнение на английском!";
			cout << "\nУкажите номер категории(до " << categories.size() << ", 0 - отмена): ";
			cin >> cat_ind;
			cin.ignore();
			if (cat_ind != 0 || (cat_ind > 0 && cat_ind <= categories.size())) {
				cout << "\nУкажите название теста: ";
				getline(cin, test_name);
				cout << "\nУкажите количество вопросов: ";
				cin >> que_num;
				cin.ignore();
				for (int i = 0; i < que_num; i++) {
					cout << "\nУкажите вопрос: ";
					getline(cin, question);
					cout << "Укажите 3 варианта ответа: \n";
					for (int i = 0; i < 3; i++) {
						cout << i + 1 << ". ";
						getline(cin, answers[i]);
					}
					int right_answer;
					do {
						cout << "Укажите номер правильного ответа: ";
						cin >> right_answer;
						cin.ignore();
					} while (right_answer < 1 || right_answer > 3);
					temp_tests[categories[cat_ind - 1]][test_name].push_back({
						{"question", question},
						{"answers", answers},
						{"right answer", answers[right_answer - 1]}
						});
				}

			}
		}
			  break;
		}
		ofstream write(tests_file);
		write << temp_tests.dump(2);
		write.close();
	}
	static bool auth_pos() { return isAuth; } // Статус авторизации администратора
	virtual void ShowStatistic() { // Статистика по пользователям
		system("cls");
		User users;
		json tests = ReadTests(tests_file);
		json user_base = users.GetUsers();
		for (const auto& categories : tests.items()) {
			cout << endl << categories.key() << ": " << endl;
			for (const auto& bot_categories : tests[categories.key()].items()) {
				cout << "\t" << bot_categories.key() << ": ";
				for (int i = 0; i < user_base.size(); i++) {
					if (user_base[i].contains("tests"))
						if (user_base[i]["tests"].contains(categories.key()))
							if (user_base[i]["tests"][categories.key()].contains(bot_categories.key())) {
								cout << "\n\t\t\tПользователь ";
								SetConsoleTextAttribute(handle, 14);
								cout << (string)user_base[i]["name"];
								SetConsoleTextAttribute(handle, 15);
								cout << ":";
								if (user_base[i]["tests"][categories.key()][bot_categories.key()]["last question"] == user_base[i]["tests"][categories.key()][bot_categories.key()]["num questions"]) {
									double right = user_base[i]["tests"][categories.key()][bot_categories.key()]["right answers"];
									double nums = user_base[i]["tests"][categories.key()][bot_categories.key()]["num questions"];
									cout << "\n\t\t\t\t\t\tВопросов: ";
									SetConsoleTextAttribute(handle, 14);
									cout << nums;
									SetConsoleTextAttribute(handle, 15);
									cout << "\n\t\t\t\t\t\tПравильных ответов: ";
									SetConsoleTextAttribute(handle, 14);
									cout << right << "(" << (right / nums) * 100 << "%)";
									SetConsoleTextAttribute(handle, 15);
									cout << "\n\t\t\t\t\t\tОценка: ";
									SetConsoleTextAttribute(handle, 14);
									cout << user_base[i]["tests"][categories.key()][bot_categories.key()]["mark"];
									SetConsoleTextAttribute(handle, 15);
								}
								else cout << "\tне закончено";
							}
				}
			}
		}
		cout << endl;
		system("pause");
	}
	int AdminChoice() { // Панель админа
		int choice = Menu({ " Управление пользователями", " Просмотр статистики", " Управление тестами",  " Смена логина и пароля", " Выйти из учетой записи" });
		message = {};
		return ++choice;
	}
};