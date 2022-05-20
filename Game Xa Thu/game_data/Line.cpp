#include "Line.h"

Line::Line()
{
	so_luong_slot = 0;
	slot = 0;
	capacity = 0;
}
void Line::Input()
{
	cout << "So slot cua hang = ";
	cin >> so_luong_slot;
	slot = new SLOT[so_luong_slot];
	cout << "Suc chua cua hang = ";
	cin >> capacity;
}
bool Line::full()
{
	int sum = 0;
	for (int i = 0; i < so_luong_slot; i++)
	{
		sum += slot[i].da_chua;
	}
	return sum >= capacity;
}