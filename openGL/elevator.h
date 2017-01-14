#ifndef ELEVATOR_H
#define ELEVATOR_H
#include <iostream>
#include "Vec3.h"
#include "RGBpixmapController.h"

class elevator{
	private:
	Vec3 chromaKey;
    RGBApixmap * nowPixmap;
	public:
	elevator();
    Vec3 position;
	void display();
};
#endif
