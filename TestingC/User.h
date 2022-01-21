#include "Main.h"
class User : public Person {
private:
	static int user_id;
public:
	User() { // �����������
		ifstream file(users_file);
		if (!file.is_open()) // �������� �� ������� �����
			OpenFileError(users_file);
		else if (file.peek() == EOF) { // �������� �� ������� ���� �� ������ �������� ������������
			cout << "\n�� ������� �� ������ ������������!\n\t�����������\n";
			RegisterUser();
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
	virtual void Login() { // ����������� ������������
		system("cls");
		ifstream file(users_file);
		if (!file.is_open()) // �������� �� ������� �����
			OpenFileError(users_file);
		else if (!isAuth) {
			string login, password;
			cout << "\n\t����������� ������������\n";
			cout << "\n\t�����: ";
			getline(cin, login);
			cout << "\n\t������: ";
			getline(cin, password);
			system("cls");
			user_id = Check(login, password);
			if (user_id != -1) {
				SetConsoleTextAttribute(handle, FOREGROUND_GREEN);
				message = "\n\t�������� �����������!\n";
				isAuth = true;
				status = "������������ " + string(info_base[user_id]["name"]);
			}
			else {
				SetConsoleTextAttribute(handle, FOREGROUND_RED);
				message = "\n\t����������� �� ��������!\n";
			}
		}
		else {
			SetConsoleTextAttribute(handle, 33);
			message = "\n\t�� ��� ������������!";
		}
		file.close();
	}
	void RegisterUser() { // ����������� ������������
		string login, password, FIO, adress, telephone;
		cout << "\n�������: ";
		cout << "\n\t�����: ";
		getline(cin, login);
		cout << "\n\t������: ";
		getline(cin, password);
		cout << "\n\t���: ";
		getline(cin, FIO);
		cout << "\n\t�������� �����: ";
		getline(cin, adress);
		cout << "\n\t�������: ";
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
			message = "\n������������ � ����� ������� ����������!\n";
		}
	}
	void DeleteUser(int index) { // �������� ������������
		info_base.erase(index);
	}
	virtual int Check(string login, string password) { // �������� ���������� ������ � ������
		for (size_t i = 0; i < info_base.size(); i++)
			if (info_base[i]["login"] == Crypt(login) && info_base[i]["password"] == Crypt(password))
				return i;
		return -1;
	}
	void Edit(int index) { // ��������� ������������(������ �������� � ����������)
		DeleteUser(index);
		RegisterUser();
	}
	virtual void LogOut() { // ����� 
		status = "�� �����������";
		isAuth = false;
		user_id = -1;
		message = {};
		system("cls");
	}
	virtual void Save() { // ���������� ���� ������������� � ����
		ofstream file_write(users_file);
		if (!file_write.is_open())
			OpenFileError(users_file);
		else
			file_write << info_base.dump(2);
		file_write.close();
	}
	static bool auth_pos() { return isAuth; } // ������ ����������� ������������
	void Tests() { // ������������ ������������
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
					my_answer = Menu({ " 1. " + string(test[categories[cat - 1]][bot_categories[bot_cat - 1]][i]["answers"][0]),  " 2. " + string(test[categories[cat - 1]][bot_categories[bot_cat - 1]][i]["answers"][1]), " 3. " + string(test[categories[cat - 1]][bot_categories[bot_cat - 1]][i]["answers"][2]), " <<< �����" });
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
					cout << "\n��������: " << test[categories[cat - 1]][bot_categories[bot_cat - 1]].size();
					cout << "\n���������� �������: " << count << "(" << (double)(count / test[categories[cat - 1]][bot_categories[bot_cat - 1]].size()) * 100 << "%)";
					cout << "\n������: " << 12 * (double)(count / test[categories[cat - 1]][bot_categories[bot_cat - 1]].size()) << endl;
					system("pause");
				}
			}
		}
		status = {};
		message = {};
	}
	virtual void ShowStatistic() { // ���������� ����������� ������������
		if (info_base[user_id].contains("tests")) {
			for (const auto& categories : info_base[user_id]["tests"].items()) {
				cout << endl << categories.key() << ": " << endl;
				for (const auto& bot_categories : info_base[user_id]["tests"][categories.key()].items()) {
					cout << "\t" << bot_categories.key() << ": ";
					if (info_base[user_id]["tests"][categories.key()][bot_categories.key()]["last question"] == info_base[user_id]["tests"][categories.key()][bot_categories.key()]["num questions"]) {
						double right = info_base[user_id]["tests"][categories.key()][bot_categories.key()]["right answers"];
						double nums = info_base[user_id]["tests"][categories.key()][bot_categories.key()]["num questions"];
						cout << "\n\t\t\t��������: ";
						SetConsoleTextAttribute(handle, 14);
						cout << nums;
						SetConsoleTextAttribute(handle, 15);
						cout << "\n\t\t\t���������� �������: ";
						SetConsoleTextAttribute(handle, 14);
						cout << right << "(" << (right / nums) * 100 << "%)";
						SetConsoleTextAttribute(handle, 15);
						cout << "\n\t\t\t������: ";
						SetConsoleTextAttribute(handle, 14);
						cout << info_base[user_id]["tests"][categories.key()][bot_categories.key()]["mark"];
						SetConsoleTextAttribute(handle, 15);
					}
					else
						cout << "�� ���������" << endl;
				}
			}
		}
		cout << endl;
		system("pause");
	}
	json GetUsers() { return info_base; } // ��������� ���� �������������
	int UserChoice() { // ������ ������������
		SetConsoleTextAttribute(handle, 15);
		int choice = Menu({ " �������� �����������", " ������ ����", " ����� �� ������ ������" });
		message = {};
		return ++choice;
	}
};