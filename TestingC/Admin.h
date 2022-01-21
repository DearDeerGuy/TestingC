#include "Main.h"
class Admin : public Person {
public:
	Admin() { // �����������
		ifstream file(admin_file);
		if (!file.is_open()) // �������� �� ������� �����
			OpenFileError(admin_file);
		else if (file.peek() == EOF) { // �������� �� ������� �������� ������
			cout << "\n������� �������������� �� ������! \n\t�����������\n";
			string login, password;
			cout << "\n�������: ";
			cout << "\n\t�����: ";
			getline(cin, login);
			cout << "\n\t������: ";
			getline(cin, password);
			info_base.push_back({
				{"login", Crypt(login)},
				{"password", Crypt(password)} });
			Save();
			system("cls");
		}
		else { // �������� ������� ������
			string str, temp;
			while (getline(file, str))
				temp += str;
			if (!temp.empty())
				info_base = json::parse(temp);
		}
		file.close();
	}
	virtual void Login() { // �����������
		system("cls");
		ifstream file(admin_file);
		if (!file.is_open()) // �������� �� ������� �����
			OpenFileError(admin_file);
		else if (!isAuth) {
			string login, password;
			cout << "\n\t����������� ��������������\n";
			cout << "\n\t�����: ";
			getline(cin, login);
			cout << "\n\t������: ";
			getline(cin, password);
			system("cls");
			if (Check(login, password)) {
				SetConsoleTextAttribute(handle, FOREGROUND_GREEN);
				message = "\n\t�������� �����������!\n";
				isAuth = true;
				status = "�������������";
			}
			else {
				SetConsoleTextAttribute(handle, FOREGROUND_RED);
				message = "\n\t����������� �� ��������!\n";
			}
		}
		else {
			message = "\n\t�� ��� ������������!";
			SetConsoleTextAttribute(handle, 33);
		}
		file.close();
	}
	virtual void Save() { // ���������� ������ � ����
		ofstream file_write(admin_file);
		if (!file_write.is_open())
			OpenFileError(admin_file);
		else
			file_write << info_base.dump(2);
		file_write.close();
	}
	virtual int Check(string login, string password) { // �������� ���������� ������ � ������
		if (info_base[0]["login"] == Crypt(login) && info_base[0]["password"] == Crypt(password))
			return 1;
		else
			return 0;
	}
	void Change() { // ����� ������
		system("cls");
		if (!isAuth)
			message = "\n\t��������� �������������� ��� ����� ������\n";
		else {
			string login, password;
			cout << "\n\t��������� ��������������\n";
			cout << "\n������� �����: ";
			cout << "\n\t�����: ";
			getline(cin, login);
			cout << "\n\t������: ";
			getline(cin, password);
			info_base[0] = {
				{"login", Crypt(login)},
				{"password", Crypt(password)} };
			Save();
			system("cls");
		}
	}
	virtual void LogOut() { // ����� 
		status = "�� �����������";
		isAuth = false;
		message = {};
		system("cls");
	}
	void UsersControlPanel() { // ������ ���������� ��������������
		User u;
		int choice = Menu({ " <<< �����", " ���������������� ������������", " ������� ������������", " �������� ������������" });
		system("cls");
		status = "�������� ������������: ";
		json user_base = u.GetUsers();
		vector<string> usernames;
		for (int i = 0; i < user_base.size(); i++)
			usernames.push_back(user_base[i]["name"]);
		switch (choice) {
		case 1: {
			u.RegisterUser();
		}
			  break;
		case 2: {
			int user_id = Menu(usernames);
			u.DeleteUser(user_id);
		}
			  break;
		case 3: {
			int user_id = Menu(usernames);
			u.Edit(user_id);
		}
			  break;
		}
		u.Save();
	}
	void TestControl() { // ���������� ������
		json temp_tests = ReadTests(tests_file);
		system("cls");
		status = "�����";
		json test = ReadTests(tests_file);

		int choice = Menu({ " <<< �����", " �������� ���������", " �������� ���� � ������������ ���������" });
		switch (choice) {
		case 1: {
			string category;
			cout << "\n������� �������� ���������: ";
			getline(cin, category);
			temp_tests[category];
		}
			  break;
		case 2: {
			vector<string> categories;
			for (const auto& n : test.items())
				categories.push_back(n.key());
			vector<string> answers{"answer 1", "answer 2", "answer 3"};
			string question, test_name;
			int que_num;
			status = "�������� ���������";
			int cat_ind = Menu(categories);
			cout << "\n����� ���������� �� ����������!";
			cout << "\n������� �������� �����: ";
			getline(cin, test_name);
			cout << "\n������� ���������� ��������: ";
			cin >> que_num;
			cin.ignore();
			if (que_num > 0) {
				for (int i = 0; i < que_num; i++) {
					system("cls");
					cout << "\n������� ������: ";
					getline(cin, question);
					cout << "������� 3 �������� ������: \n";
					for (int i = 0; i < 3; i++) {
						cout << i + 1 << ". ";
						getline(cin, answers[i]);
					}
					int right_answer = Menu(answers);
					temp_tests[categories[cat_ind]][test_name].push_back({
						{"question", question},
						{"answers", answers},
						{"right answer", answers[right_answer]}
						});
				}
			}
		}
			  break;
		}
		ofstream write(tests_file);
		write << temp_tests.dump(2);
		write.close();
		status = {};
	}
	static bool auth_pos() { return isAuth; } // ������ ����������� ��������������
	virtual void ShowStatistic() { // ���������� �� �������������
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
								cout << "\n\t\t\t������������ ";
								SetConsoleTextAttribute(handle, 14);
								cout << (string)user_base[i]["name"];
								SetConsoleTextAttribute(handle, 15);
								cout << ":";
								if (user_base[i]["tests"][categories.key()][bot_categories.key()]["last question"] == user_base[i]["tests"][categories.key()][bot_categories.key()]["num questions"]) {
									double right = user_base[i]["tests"][categories.key()][bot_categories.key()]["right answers"];
									double nums = user_base[i]["tests"][categories.key()][bot_categories.key()]["num questions"];
									cout << "\n\t\t\t\t\t\t��������: ";
									SetConsoleTextAttribute(handle, 14);
									cout << nums;
									SetConsoleTextAttribute(handle, 15);
									cout << "\n\t\t\t\t\t\t���������� �������: ";
									SetConsoleTextAttribute(handle, 14);
									cout << right << "(" << (right / nums) * 100 << "%)";
									SetConsoleTextAttribute(handle, 15);
									cout << "\n\t\t\t\t\t\t������: ";
									SetConsoleTextAttribute(handle, 14);
									cout << user_base[i]["tests"][categories.key()][bot_categories.key()]["mark"];
									SetConsoleTextAttribute(handle, 15);
								}
								else cout << "\t�� ���������";
							}
				}
			}
		}
		cout << endl;
		system("pause");
	}
	int AdminChoice() { // ������ ������
		int choice = Menu({ " ���������� ��������������", " �������� ����������", " ���������� �������",  " ����� ������ � ������", " ����� �� ������ ������" });
		message = {};
		return ++choice;
	}
};