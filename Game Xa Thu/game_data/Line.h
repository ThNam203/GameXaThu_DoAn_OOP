#ifndef LINE_HEADER
#define LINE_HEADER

#include <iostream>
#include "slot.h"

using namespace std;

class Line
{
protected:
	int so_luong_slot;
	SLOT* slot;
	int capacity;  // sức chứa của hàng 
	bool full();
public:
	Line();
	~Line() {};
	virtual void Input();
	//virtual void Output(int x, int y) = 0;
};

#endif