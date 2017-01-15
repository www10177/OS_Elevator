# Define a macro for the object files 
OBJECTS= main.o RGBpixmap.o  KeyEventController.o  RGBpixmapController.o Timer.o elevator.o Vec.o passenger.o coreThread.ojL

# Define a macro for the library file 
LIBES= -framework GLUT -framework OpenGL

# use macros rewrite makefile 
prog: $(OBJECTS) 
	c++ $(OBJECTS) $(LIBES) -o prog 
clean:
	rm -rvf $(OBJECTS) prog
