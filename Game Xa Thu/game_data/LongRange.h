#include "Powder.h"
#include "Line.h"

class LongRange : public Line
{
public:
	BulletUnit** all = new BulletUnit * [so_luong_slot];
	void Input();
	//void Output(int x, int y);
};

