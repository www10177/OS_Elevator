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
		RGBApixmap * PixmapPic[2];
		RGBApixmap * movePixmapPic[2];
		int io;
		float speed;
		bool isGoingup;
		int textOffsetX;
		Vec3 color;
		const int offset = 30;

	public:
		float movPosX;
		int count;
		bool goIn;
		bool goOut;
		passenger(Vec3,int textOffsetX,Vec3 color);
		Vec3 position;
		void display();
		void updateP();
		void setIO(int io,bool upOrDown=false); // 1 for in,2for out
};
