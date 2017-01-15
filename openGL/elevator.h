#ifndef ELEVATOR_H
#define ELEVATOR_H
#include "Vec.h"
#include "RGBpixmapController.h"
#include <cmath>
extern int floorHeight;

class elevator{
	private:
	Vec3 chromaKey;
    RGBApixmap * nowPixmap;
	float speed ;
	public:
	bool upOrDown;
	int nowFloor;
	elevator();
	elevator(Vec3 pos);
    Vec3 position;
	void getPos(int nowFloor, bool upOrDown);
	void update();
	void display();
};
#endif
