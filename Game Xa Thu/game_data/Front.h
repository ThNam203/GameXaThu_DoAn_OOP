#include "NonBulletUnit.h"
#include "Line.h"

class Front : public Line
{
public:
	Front() {};
	~Front();
	NonBulletUnit** all = new NonBulletUnit* [so_luong_slot];
	void Input();
	//void Output(int x, int y);
};

