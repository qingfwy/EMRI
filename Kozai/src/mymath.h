#pragma once
#include <iostream>
#include <vector>

class Array
{
protected:
	int length = 0;
	double* data = nullptr;

public:
	Array();
	Array(int n);
	Array(int n, double val);
	Array(int n, double* val);
	Array(const Array& arr);
	~Array();
	int len();
	Array absolute() const;
	double maximum() const;

	double& operator[](int n);
	double operator[](int n) const;
	void operator=(const Array& arr);

	Array operator+(double x) const;
	Array operator+(const Array& arr) const;
	void operator+=(double x);
	void operator+=(const Array& arr);
	Array operator-() const;
	Array operator-(double x) const;
	Array operator-(const Array& arr) const;
	void operator-=(double x);
	void operator-=(const Array& arr);
	Array operator*(double x) const;
	void operator*=(double x);
	double operator*(const Array& arr) const;
	Array operator/(double x) const;
	void operator/=(double x);

	friend std::ostream& operator<<(std::ostream& os, const Array& arr);
	friend Array operator+(double x, const Array& arr);
	friend Array operator-(double x, const Array& arr);
	friend Array operator*(double x, const Array& arr);
};

class Vec2
{
public:
    double                                   x, y;
    constexpr Vec2()                      : x(0.0), y(0.0) { }
    constexpr Vec2(double _x, double _y)    : x(_x), y(_y) { }
    double& operator[] (size_t idx)          { return ((double*)(void*)(char*)this)[idx]; } // We very rarely use this [] operator, so the assert overhead is fine.
    double operator[] (size_t idx) const    { return ((const double*)(const void*)(const char*)this)[idx]; }
	void show() const
	{
		std::cout << x << "\t" << y << std::endl;
	}
};

class ScrollBuffer {
public:
    ScrollBuffer(int max_size) {
        MaxSize = max_size;
        Offset  = 0;
        Data.reserve(MaxSize);
    }
    void AddPoint(double x, double y) {
        if (Data.size() < MaxSize)
            Data.push_back(Vec2(x, y));
        else {
            Data[Offset] = Vec2(x, y);
            Offset =  (Offset + 1) % MaxSize;
        }
    }
	void show()
	{
		if (Data.size() == 0) return;
		if (Data.size() < MaxSize)
		{
			Data[Data.size()-1].show();
		}
		else if (Offset == 0)
		{
			Data[Data.size() - 1].show();
		}
		else
		{
			Data[Offset - 1].show();
		}
		std::cout << std::endl;
	}
    void Erase() {
        if (Data.size() > 0) {
            Data.resize(0);
            Offset  = 0;
        }
    }
public:
    int MaxSize;
    int Offset;
    std::vector<Vec2> Data;
};