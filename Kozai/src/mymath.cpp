#include "mymath.h"

Array::Array()
{
}

Array::Array(int n)
{
	length = n;
	data = new double[n];
}

Array::Array(int n, double val)
{
	length = n;
	data = new double[n];
	for (int i = 0; i < n; i++)
	{
		data[i] = val;
	}
}

Array::Array(int n, double* val)
{
	length = n;
	data = new double[n];
	for (int i = 0; i < n; i++)
	{
		data[i] = val[i];
	}
}

Array::Array(const Array& arr)
{
	length = arr.length;
	data = new double[length];
	for (int i = 0; i < length; i++)
	{
		data[i] = arr.data[i];
	}
}

Array::~Array()
{
	delete[] data;
	data = nullptr;
}

int Array::len()
{
	return length;
}

Array Array::absolute() const
{
	Array res(*this);
	for (int i = 0; i < length; i++)
	{
		res.data[i] = abs(res.data[i]);
	}
	return res;
}

double Array::maximum() const
{
	if (length == 0)
	{
		std::cout << "empty!!! no maximum." << std::endl;
		return 0;
	}
	double res = data[0];
	for (int i = 1; i < length; i++)
	{
		if (data[i] > res)
		{
			res = data[i];
		}
	}
	return res;
}

double& Array::operator[](int n)
{
	return data[n];
}

double Array::operator[](int n) const
{
	return data[n];
}

void Array::operator=(const Array& arr)
{
	if (arr.length == 0)
	{
		if (length != 0)
		{
			length = 0;
			delete[] data;
			data = nullptr;
		}
	}
	else
	{
		if (length != arr.length)
		{
			if (length != 0)
			{
				delete[] data;
			}
			length = arr.length;
			data = new double[length];
		}
		for (int i = 0; i < length; i++)
		{
			data[i] = arr.data[i];
		}
	}
}

Array Array::operator+(double x) const
{
	Array res(*this);
	res += x;
	return res;
}

Array Array::operator+(const Array& arr) const
{
	Array res(*this);
	res += arr;
	return res;
}

void Array::operator+=(double x)
{
	for (int i = 0; i < length; i++)
	{
		data[i] += x;
	}
}

void Array::operator+=(const Array& arr)
{
	for (int i = 0; i < length; i++)
	{
		data[i] += arr.data[i];
	}
}

Array Array::operator-() const
{
	Array res(*this);
	for (int i = 0; i < length; i++)
	{
		res.data[i] = -data[i];
	}
	return res;
}

Array Array::operator-(double x) const
{
	Array res(*this);
	res -= x;
	return res;
}

Array Array::operator-(const Array& arr) const
{
	Array res(*this);
	res -= arr;
	return res;
}

void Array::operator-=(double x)
{
	for (int i = 0; i < length; i++)
	{
		data[i] -= x;
	}
}

void Array::operator-=(const Array& arr)
{
	for (int i = 0; i < length; i++)
	{
		data[i] -= arr.data[i];
	}
}

Array Array::operator*(double x) const
{
	Array res(*this);
	res *= x;
	return res;
}

void Array::operator*=(double x)
{
	for (int i = 0; i < length; i++)
	{
		data[i] *= x;
	}
}

double Array::operator*(const Array& arr) const
{
	double res = 0;
	for (int i = 0; i < length; i++)
	{
		res += data[i] * arr.data[i];
	}
	return res;
}

Array Array::operator/(double x) const
{
	return (*this) * (1.0 / x);
}

void Array::operator/=(double x)
{
	(*this) *= (1.0 / x);
}

std::ostream& operator<<(std::ostream& os, const Array& arr)
{
	for (int i = 0; i < arr.length - 1; i++)
	{
		os << arr.data[i] << "\t";
	}
	if (arr.length > 0)
	{
		os << arr.data[arr.length - 1];
	}
	else
	{
		std::cout << "empty arr!";
	}
	return os;
}

Array operator+(double x, const Array& arr)
{
	return arr + x;
}

Array operator-(double x, const Array& arr)
{
	return -arr + x;
}

Array operator*(double x, const Array& arr)
{
	return arr * x;
}