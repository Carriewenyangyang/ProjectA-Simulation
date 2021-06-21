/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include "MCG.h"

using namespace std;
using namespace mcg;
int main()
{
    //s={previous_x, a, b, m}
	state s = {1, 13, 1, (unsigned int)(1 << 31)};

	for(int i = 0; i < 10; i++)
		std::cout << exp_transform(Xn_norm_to_U(s), 1.0) << ",";

	return 0;
}
