#pragma once
#include <iostream>
#include <utility>

template<typename T>
class Rkf45
{
private:
	double x, end, sigma, h, hmax;
	T y;
	bool flag;

	double c1[2] = { 0.25, 0.25 };
	double c2[3] = { 3.0 / 8, 3.0 / 32, 9.0 / 32 };
	double c3[4] = { 1.2 / 1.3, 1.932 / 2.197,-7.2 / 2.197,7.296 / 2.197 };
	double c4[5] = { 1.0,4.39 / 2.16,-8.0,36.8 / 5.13,-8.45 / 41.04 };
	double c5[6] = { 0.5,-8.0 / 27,2.0,-3.544 / 2.565,1.859 / 4.104,-1.1 / 4 };

	double co4[4] = { 2.5 / 21.6,1.408 / 2.565,2.197 / 4.104,-0.2 };
	double cd[5] = { 1.0 / 360,-1.28 / 42.75,-2.197 / 75.24,1.0 / 50,2.0 / 55 };

public:
	int times_total = 0, times_apparent = 0;

public:
	Rkf45() {}

	Rkf45(double start, double end, T y0, double sigma, double h, double hmax)
	{
		x = start;
		this->end = end;
		y = y0;
		this->sigma = sigma;
		this->h = h;
		this->hmax = hmax;
		flag = false;
	}

	~Rkf45() {}

	void set(double start, double end, T y0, double sigma, double h, double hmax)
	{
		x = start;
		this->end = end;
		y = y0;
		this->sigma = sigma;
		this->h = h;
		this->hmax = hmax;
		flag = false;
	}

	bool done()
	{
		return flag;
	}

	double getx()
	{
		return x;
	}

	T gety()
	{
		return y;
	}

	double geth()
	{
		return h;
	}

	std::pair<double, T> run()
	{
		double q, ratio;
		std::pair<double, T> res;
		T k1, k2, k3, k4, k5, k6, delta, tmp;
		bool no_accepted = true;
		while (no_accepted)
		{
			k1 = h * y.dfdt(x);

			tmp = y + c1[1] * k1;
			if (tmp.valid())
			{
				k2 = h * tmp.dfdt(x + c1[0] * h);
			}
			else
			{
				h *= 0.5;
				continue;
			}

			tmp = y + c2[1] * k1 + c2[2] * k2;
			if (tmp.valid())
			{
				k3 = h * tmp.dfdt(x + c2[0] * h);
			}
			else
			{
				h *= 0.5;
				continue;
			}

			tmp = y + c3[1] * k1 + c3[2] * k2 + c3[3] * k3;
			if (tmp.valid())
			{
				k4 = h * tmp.dfdt(x + c3[0] * h);
			}
			else
			{
				h *= 0.5;
				continue;
			}

			tmp = y + c4[1] * k1 + c4[2] * k2 + c4[3] * k3 + c4[4] * k4;
			if (tmp.valid())
			{
				k5 = h * tmp.dfdt(x + c4[0] * h);
			}
			else
			{
				h *= 0.5;
				continue;
			}

			tmp = y + c5[1] * k1 + c5[2] * k2 + c5[3] * k3 + c5[4] * k4 + c5[5] * k5;
			if (tmp.valid())
			{
				k6 = h * tmp.dfdt(x + c5[0] * h);
			}
			else
			{
				h *= 0.5;
				continue;
			}
			
			/*k2 = h * y.dfdt(x + c1[0] * h, y + c1[1] * k1);
			k3 = h * y.dfdt(x + c2[0] * h, y + c2[1] * k1 + c2[2] * k2);
			k4 = h * y.dfdt(x + c3[0] * h, y + c3[1] * k1 + c3[2] * k2 + c3[3] * k3);
			k5 = h * y.dfdt(x + c4[0] * h, y + c4[1] * k1 + c4[2] * k2 + c4[3] * k3 + c4[4] * k4);
			k6 = h * y.dfdt(x + c5[0] * h, y + c5[1] * k1 + c5[2] * k2 + c5[3] * k3 + c5[4] * k4 + c5[5] * k5);*/

			delta = cd[0] * k1 + cd[1] * k3 + cd[2] * k4 + cd[3] * k5 + cd[4] * k6;
			ratio = (delta.absolute()).maximum() / h;

			if (ratio < sigma) // accepted
			{
				x += h;
				y += co4[0] * k1 + co4[1] * k3 + co4[2] * k4 + co4[3] * k5 + delta;
				res.first = x;
				res.second = y;
				no_accepted = false;
				times_apparent++;
			}
			times_total++;

			q = 0.84 * pow(sigma / ratio, 0.25);
			if (q < 0.1)
			{
				h *= 0.1;
			}
			else if (q > 2)
			{
				h *= 2;
			}
			else
			{
				h *= q;
			}

			if (h > hmax)
			{
				h = hmax;
			}

			if (end <= 0) // end 为0时跳过后续对h的和终点的判断
			{
				continue;
			}

			if (x >= end)
			{
				flag = true;
			}
			else if (x + h > end)
			{
				h = end - x;
			}
		}
		return res;
	}

};