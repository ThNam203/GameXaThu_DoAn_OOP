#include "Line.h"
#include "NonBulletUnit.h"

class Flank : public Line
{
public:
	Flank(int Left);
	~Flank();
	NonBulletUnit** all = new NonBulletUnit* [so_luong_slot];
	bool isLeft;
	void Input();
	//void Output(int x, int y);
	void Arrange(NonBulletUnit group[]);
};

