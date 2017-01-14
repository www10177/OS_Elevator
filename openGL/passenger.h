#ifndef PASSENGER_H
#define PASSENGER_H 
#endif
#include "Vec.h"
#include "RGBpixmapController.h"

class passenger{
	private:
	Vec3 chromaKey;
    RGBApixmap * nowPixmap;
	public:
	passenger();
	passenger(Vec3);
    Vec3 position;
	void display();
};
