#ifndef ELEVATOR_H
#define ELEVATOR_H
#include "Vec.h"
#include "RGBpixmapController.h"

class elevator{
	private:
	Vec3 chromaKey;
    RGBApixmap * nowPixmap;
	public:
	elevator();
	elevator(Vec3 pos);
    Vec3 position;
	void display();
};
#endif
