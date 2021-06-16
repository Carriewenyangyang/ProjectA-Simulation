#include "ns3/core-module.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
using namespace ns3;


int main(int argc, char *argv[] )
{
	std::ofstream file_("mytext.txt");
	
	const int a = 13;
	const int m = 100;
	const int b = 1;
	int x[1000];
	float c[1000];
	for (int i=1; i<=1000; i++){
		x[0] = 1;
		x[i] = (a * x[i-1] + b) % m;
		//c = (x=a*x+b)%m;
		c[i] = x[i] / (float)m;
		file_<<i<<"\t"<<c[i]<<"\n";
		//file_ << "\t \n"; 
		}
	file_.close();
		return 0;
}

