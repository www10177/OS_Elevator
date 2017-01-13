Main: Main.o
	g++ Main.o -o Main
Main.o:Main.cpp
	g++ -c -std=c++11 Main.cpp
clean:
	rm -v Main.o Main
