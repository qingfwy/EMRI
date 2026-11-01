#pragma once
#include <cmath>
#include <iostream>

class Dynamic
{
private:
	double m[2] = { 1, 1 };
	double val[2][4] = { {0,0,0,0}, {0,0,0,0} };
	// x,y,vx,vy

public:
	Dynamic() {}
	Dynamic(double* mass, double* val1, double* val2)
	{
		m[0] = mass[0];
		m[1] = mass[1];
		for (size_t i = 0; i < 4; i++)
		{
			val[0][i] = val1[i];
			val[1][i] = val2[i];
		}
	}

	static Dynamic func(const Dynamic& y)
	{
		Dynamic res = y;
		double G = 1.0;
		double r3 = pow(pow(y.val[0][0] - y.val[1][0], 2) + pow(y.val[0][1] - y.val[1][1], 2), 1.5);
		res.val[0][0] = y.val[0][2];
		res.val[0][1] = y.val[0][3];
		res.val[0][2] = G*y.m[1]*(y.val[1][0]-y.val[0][0])/r3;
		res.val[0][3] = G*y.m[1]*(y.val[1][1]-y.val[0][1])/r3;
		res.val[1][0] = y.val[1][2];
		res.val[1][1] = y.val[1][3];
		res.val[1][2] = G*y.m[0]*(y.val[0][0]-y.val[1][0])/r3;
		res.val[1][3] = G*y.m[0]*(y.val[0][1]-y.val[1][1])/r3;
		return res;
	}

	void operator+=(const Dynamic& dynamic)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				this->val[i][j] += dynamic.val[i][j];
			}
		}
	}

	Dynamic operator+(const Dynamic& dynamic) const
	{
		Dynamic res = *this;
		res += dynamic;
		return res;
	}

	Dynamic operator-(const Dynamic& dynamic) const
	{
		Dynamic res = *this;
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				res.val[i][j] -= dynamic.val[i][j];
			}
		}
		return res;
	}

	Dynamic operator*(double dx) const
	{
		Dynamic res = *this;
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				res.val[i][j] = dx * (this->val[i][j]);
			}
		}
		return res;
	}

	static friend Dynamic operator*(double x, const Dynamic& dynamic)
	{
		return dynamic * x;
	}

	double abs_max() const
	{
		double res = 0;
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				res = std::max(abs(val[i][j]), res);
			}
		}
		return res;
	}

	void get_data(double* data) const
	{
		for (size_t i = 0; i < 4; i++)
		{
			data[i+1] = val[0][i];
			data[i+5] = val[1][i];
		}
	}
	
	void show() const
	{
		//for (size_t i = 0; i < 2; i++)
		//{
		//	for (size_t j = 0; j < 4; j++)
		//	{
		//		std::cout << val[i][j] << "\t";
		//	}
		//	std::cout << std::endl;
		//}
		std::cout << "sigma" << std::endl;
		std::cout << std::endl;
	}
};
