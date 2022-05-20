#include <Windows.h>
#include <iostream>

using namespace std;

void gotoxy(int x, int y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}

int Max3x(int A, int B, int C)
{
	if (A >= B && A >= C) return A;
	if (B >= C) return B;
	return C;
}

void Menu()
{
	cout << "     ==================================================" << endl;
	cout << "1. Xep quan hang Front" << endl;
	cout << "2. Xep quan hang Flank Left" << endl;
	cout << "3. Xep quan hang Flank Rigth" << endl;
	cout << "4. Xep quan hang Long Range" << endl;
	cout << "   voi cac loai quan : " << endl;
	cout << "       Loai 1 : Quan Sword  =|--" << endl; // này là biểu tượng quân :v
	cout << "       Loai 2 : Quan Spear  --->" << endl;
	cout << "       Loai 3 : Quan Hop    --)>" << endl;
	cout << "                Quan Powder oooo" << endl;
	cout << "                O trong     ____" << endl; // ô trống
	cout << "       !Hang Long Range mac dinh chi chua quan Powder" << endl;
	cout << "5. Xuat doi hinh" << endl;
	cout << "6. Xep them quan vao hang Flank" << endl; // goi ham arrange
	cout << "0. Thoat game" << endl;
	cout << "     ==================================================" << endl;
}