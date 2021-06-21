#include <cstddef>
#include <cmath>

namespace mcg
{
	struct state
	{
		std::size_t previous_x;
		std::size_t a;
		std::size_t b;
		std::size_t m;
	};
    //Update Xi: call exp_transform()-->Xn_norm_to_U()-->gen_Xn()
	std::size_t gen_Xn(state& s)
	{
		return s.previous_x = (s.a * s.previous_x + s.b) % s.m;//Update Xi
	}

	double Xn_norm_to_U(state& s)//Uniform distributed
	{
		return gen_Xn(s) / (double)s.m;
	}

	double exp_transform(double U, double mean)//exponential distributed
	{
		return -(mean) * std::log(1 - U);
	}
}