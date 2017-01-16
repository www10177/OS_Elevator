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
		RGBApixmap * movePixmap;
		float speedPass ;
		float movPosX;
	public:
		bool passOut;
		bool upOrDown;
		int nowFloor;
		elevator();
		elevator(Vec3 pos);
		Vec3 position;
		void getPos(int nowFloor, bool upOrDown);
		void update();
		void display();
		void passengerOut();
};
#endif
