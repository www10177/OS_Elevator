# Define a macro for the object files 
OBJECTS= main.o RGBpixmap.o  KeyEventController.o  RGBpixmapController.o Timer.o elevator.o Vec.o passenger.o 


# Define a macro for the library file 
LIBES= -framework GLUT -framework OpenGL 

# use macros rewrite makefile 
prog: $(OBJECTS) 
	c++ $(OBJECTS) $(LIBES) -o prog 
main.o:main.cpp
	g++ -std=c++11 -c main.cpp -o main.o
#coreThread.o:coreThread.cpp coreThread.h
#	c++ -c -std=c++11 coreThread.cpp -o coreThread.o
clean:
	rm -rvf $(OBJECTS) prog
