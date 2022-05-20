#include "Front.h"
#include "Sword.h"
#include "Hop.h"
#include "Spear.h"

//1. Sword
//2. Spear
//3. Hop

Front::~Front()
{
	for (int i = 0; i < so_luong_slot; i++)
	{
		delete[] all[i];
	}
	delete[] all;
}

void Front::Input()
{
	Line::Input();
	NonBulletUnit** all = new NonBulletUnit * [so_luong_slot];
	for (int i = 0; i < so_luong_slot; i++)
	{
		int type;
		cout << "Loai quan = ";
		cin >> type;
		int quantity;
		cout << "So quan trong slot " << i << " = ";
		cin >> quantity;
		slot[i].da_chua = quantity;
		if (type == 1)
		{
			all[i] = new NonBulletUnit[capacity]; // sẽ có TH 1 slot chứa hết quân của hàng nên cấp phát sẵn
			for (int j = 0; j < quantity; j++)
			{
				all[i][j] = Sword();
			}
		}
		if (type == 2)
		{
			all[i] = new NonBulletUnit[capacity];
			for (int j = 0; j < quantity; j++)
			{
				all[i][j] = Spear();
			}
		}
		if (type == 3)
		{
			all[i] = new NonBulletUnit[capacity];
			for (int j = 0; j < quantity; j++)
			{
				all[i][j] = Hop();
			}
		}
	}
	this->all = all; // dùng mảng all của class thì bị lỗi mà khai báo thêm mảng all khác rồi gán thì hết :v
}
//void Front::Output(int x, int y)
//{
//	for (int j = capacity - 1; j >= 0; j--)
//	{
//		gotoxy(x, y);
//		for (int i = 0; i < slot; i++)
//		{
//			cout << all[i][j].Type << "  ";
//		}
//		y++;
//	}
//}
