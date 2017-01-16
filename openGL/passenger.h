#ifndef PASSENGER_H
#define PASSENGER_H 
#endif
#include "Vec.h"
#include "RGBpixmapController.h"
extern int screenHeight;
extern float itemXLocation[2]	; 
class passenger{
	private:
		bool visible;
		Vec3 chromaKey;
		RGBApixmap * nowPixmap;
		RGBApixmap * movePixmap;
		int io;
		float speed;

	public:
		float movPosX;
		int count;
		bool goIn;
		bool goOut;
		passenger(Vec3);
		Vec3 position;
		void display();
		void updateP();
		void setIO(int io); // 1 for in,2for out

};
