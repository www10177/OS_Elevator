#include <iostream>
#include <random>
#include<thread>
#include<chrono>
#include<stdlib.h>
#include<time.h>

int main()
{
	unsigned seed;
	seed=(unsigned)time(NULL);
	std::default_random_engine generator(seed);
	std::poisson_distribution<int> distribution(10);

	for(int i = 0;i<10;i++){
	
		int number = distribution(generator);
		std::this_thread::sleep_for(std::chrono::seconds(number));
		std::cout<<number<<std::endl;
	}
	return 0;
}
