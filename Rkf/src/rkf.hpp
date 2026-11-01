#pragma once
#include <iostream>
#include "mtx.h"

namespace rkf {

	template<typename T>
	class Rkf45
	{
	public:
		Rkf45(double begin, double end, T y, int logsigma, double dxmax = 0)
		{
			x = begin;
			this->y = y;
			this->xend = end;
			this->logsigma = logsigma;
			this->sigma = pow(10, this->logsigma);
			this->dx = (dxmax > 1e-8) ? dxmax : 1;
			this->dxmax = dxmax;

			// backups
			x_b = begin;
			this->y_b = y;
			this->dx_b = dxmax;
		}

		void reset()
		{
			x = x_b;
			y = y_b;
			dx = (dx_b > 1e-8) ? dx_b : 1;
			count[0] = 0;
			count[1] = 0;
			flag = true;
		}

		void set_logsigma(int x)
		{
			logsigma = x;
			sigma = pow(10, logsigma);
		}

		int get_logsigma()
		{
			return logsigma;
		}

		void run(bool once = false)
		{
			std::lock_guard<std::mutex> lk(MTX::mtx);
			bool onlyone = false;
			T k[6], delta, tmp;
			double delta_max, q;
			while (flag)
			{
				k[0] = dx * T::func(y);

				tmp = y + c1[1] * k[0];
				if (tmp.valid())
				{
					k[1] = dx * T::func(tmp);
				}
				else
				{
					dx *= 0.5;
					continue;
				}

				tmp = y + c2[1] * k[0] + c2[2] * k[1];
				if (tmp.valid())
				{
					k[2] = dx * T::func(tmp);
				}
				else
				{
					dx *= 0.5;
					continue;
				}

				tmp = y + c3[1] * k[0] + c3[2] * k[1] + c3[3] * k[2];
				if (tmp.valid())
				{
					k[3] = dx * T::func(tmp);
				}
				else
				{
					dx *= 0.5;
					continue;
				}

				tmp = y + c4[1] * k[0] + c4[2] * k[1] + c4[3] * k[2] + c4[4] * k[3];
				if (tmp.valid())
				{
					k[4] = dx * T::func(tmp);
				}
				else
				{
					dx *= 0.5;
					continue;
				}

				tmp = y + c5[1] * k[0] + c5[2] * k[1] + c5[3] * k[2] + c5[4] * k[3] + c5[5] * k[4];
				if (tmp.valid())
				{
					k[5] = dx * T::func(tmp);
				}
				else
				{
					dx *= 0.5;
					continue;
				}

				delta = cd[0] * k[0] + cd[1] * k[2] + cd[2] * k[3] + cd[3] * k[4] + cd[4] * k[5];
				delta_max = delta.abs_max() / dx;

				if (delta_max < sigma) // accurate is acceptable
				{
					x += dx;
					//std::cout << dx << "\t" << x  << "\t" << delta_max << std::endl;
					y += crk4[0] * k[0] + crk4[1] * k[2] + crk4[2] * k[3] + crk4[3] * k[4] + delta;
					//y.show();
					count[0]++;
					//std::cout << count[0] << std::endl;
					if (once)
					{
						onlyone = true;
					}
				}
				// already done, then we need prepare next dx

				q = 0.84 * pow(sigma / delta_max, 0.25);
				dx = (q < 2) ? q * dx : 2 * dx; // maxinum growth rate is 2

				count[1]++; // cycles

				// terminate condition
				if (dxmax >= 1e-8 and dx > dxmax)
				{
					dx = dxmax;
				} // maxinum of dx is dxmax

				if (x > xend)
				{
					flag = false;
				}
				else if (x + dx > xend)
				{
					dx = xend - x;
					if (dx < 1e-8) flag = false;
				}
				if (onlyone)
					break;
			}
			//std::cout << "accept times:\t" << count[0] << "\t" << "total times:\t" << count[1] << std::endl;
		}

		void get_data(double* data)
		{
			data[0] = x;
			y.get_data(data);
		}

		int get_steps() const
		{
			return count[0];
		}

		void show()
		{
			y.show();
		}

	private:
		// constants for k1-k6
		const double c1[2] = { 0.25, 0.25 };
		const double c2[3] = { 3.0 / 8, 3.0 / 32, 9.0 / 32 };
		const double c3[4] = { 1.2 / 1.3, 1.932 / 2.197,-7.2 / 2.197,7.296 / 2.197 };
		const double c4[5] = { 1.0,4.39 / 2.16,-8.0,36.8 / 5.13,-8.45 / 41.04 };
		const double c5[6] = { 0.5,-8.0 / 27,2.0,-3.544 / 2.565,1.859 / 4.104,-1.1 / 4 };
		// constants for rk4
		const double crk4[4] = { 2.5 / 21.6,1.408 / 2.565,2.197 / 4.104,-0.2 };

		// first calculate delta, if dx is proper, then calculate rk4 + delta, because terms of rk4 is less then rk5
		const double cd[5] = { 1.0 / 360, -1.28 / 42.75, -2.197 / 75.24, 1.0 / 50, 2.0 / 55 };

		// local variables
		double x, xend, dx, dxmax, sigma;
		int logsigma;
		T y;
		int count[2] = { 0,0 };
		bool flag = true;
		double x_b, dx_b;
		T y_b;
	};


}


