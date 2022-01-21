#include "Main.h"
#include "Person.h"
#include "User.h"
#include "Admin.h"

int MainChoice() { // �������� ������
	SetConsoleTextAttribute(handle, 15);
	cout << "\n\t" << status << endl;
	int control_choice = Menu({ " <<< �����" , " �������������", " ������������" });
	return control_choice;
}

//------------ ���������� ����������� ------------
bool Person::isAuth = false;
int User::user_id = -1;
//------------ ���������� ����������� ------------

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	int log_choice;
	do {
		log_choice = MainChoice();
		switch (log_choice) {
		case 1: { // ��� ������
			Admin a;
			int choice;
			a.Login();
			if (Admin::auth_pos())
				do {
					choice = a.AdminChoice();
					switch (choice) {
					case 1:
						a.UsersControlPanel();
						break;
					case 2:
						a.ShowStatistic();
						break;
					case 3:
						a.TestControl();
						break;
					case 4:
						a.Change();
						break;
					case 5:
						a.LogOut();
						break;
					default:
						system("cls");
						break;
					}
					status = { "ADMIN" };
				} while (choice != 5);
				status = { "�� �����������" };
		}
			  break;
		case 2: { // ��� ������������
			User u;
			int choice;
			u.Login();
			if (User::auth_pos())
				do {
					choice = u.UserChoice();
					switch (choice) {
					case 1:
						u.ShowStatistic();
						break;
					case 2:
						u.Tests();
						break;
					case 3:
						u.LogOut();
						break;
					}
				} while (choice != 3);
		}
			  break;
		default:
			system("cls");
			break;
		}
	} while (log_choice);
}