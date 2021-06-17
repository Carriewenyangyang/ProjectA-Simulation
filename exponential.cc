#include "ns3/core-module.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
using namespace ns3;

int main(int argc, char *argv[] )
{
	int mean = 1;
	float xu;
	const int a = 13;
	const int m = 100;
	const int b = 1;
	int x[1000];
	double c[1000];
	x[0] = 1;
	for (int i=1; i<=1000; i++){
	// LCG

		x[i] = (a * x[i-1] + b) % m;
		c[i] = x[i] / (float)m;
		
	// ExponentialRandomVariable
	
	xu = -mean * log(1-c[i]);
	
	std::cout<<xu<<std::endl;
	}
}