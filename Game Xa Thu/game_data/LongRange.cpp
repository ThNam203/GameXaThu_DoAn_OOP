#include "LongRange.h"

void LongRange::Input()
{
	Line::Input();
	all = new BulletUnit * [so_luong_slot];
	for (int i = 0; i < so_luong_slot; i++)
	{
		int quantity;
		cout << "So quan trong slot " << i << " = ";
		cin >> quantity;
		slot[i].da_chua = quantity;
		all[i] = new BulletUnit[capacity];
		for (int j = 0; j < quantity; j++)
		{
			all[i][j] = Powder();
		}
	}
}
//void LongRange::Output(int x, int y)
//{
//	int x1 = x, y1 = y;
//	for (int i = 0; i < so_luong_slot; i++)
//	{
//		x1 += 6;
//		y1 = y;
//		for (int j = 0; j < slot[i].contained; j++)
//		{
//			gotoxy(x1, y1);
//			cout << all[i][j].show;
//			y1++;
//		}
//	}
//}