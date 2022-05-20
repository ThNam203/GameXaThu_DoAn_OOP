#include "Flank.h"
#include "Sword.h"
#include "Hop.h"
#include "Spear.h"

//1. Sword
//2. Spear

Flank::Flank(int Left)
{
	if (Left) isLeft = 1;
	else
		isLeft = 0;
}
Flank::~Flank()
{
	for (int i = 0; i < so_luong_slot; i++)
	{
		delete[] all[i];
	}
	delete[] all;
}
void Flank::Arrange(NonBulletUnit group[])
{

}

void Flank::Input()
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
			all[i] = new NonBulletUnit[capacity];
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
	}
	this->all = all;
}
//void Flank::Output(int x, int y)
//{
//	for (int j = capacity - 1; j >= 0; j--)
//	{
//		gotoxy(x, y);
//		for (int i = 0; i < so_luong_slot; i++)
//		{
//			cout << all[i][j].Type << "  ";
//		}
//		y++;
//	}
//}
