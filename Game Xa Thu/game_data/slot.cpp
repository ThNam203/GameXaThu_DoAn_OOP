#include "slot.h"

SLOT::SLOT()
{
	da_chua = 0;
}
bool SLOT::full(int x)
{
	return da_chua >= x;
}