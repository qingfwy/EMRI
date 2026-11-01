#pragma once
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "units.h"
#include "mymath.h"

class Par
{
public:
	Array val;
	double m[3]; // m*3
	double c[3]; // constant of L1L2, and C2

public:
	Par() : val(8) {} // a*2,e*2,w*2, i*2
	Par(double* initdata);


private:
	void setConstant();
	void setAngle(double angle, int type = 0);

public:
	double L1() const;
	double L2() const;
	double G1() const;
	double G2() const;
	double Theta();
	Par absolute() const;
	double maximum() const;
	double abs_max() const;

	bool valid();
	static Par func(const Par&);

public:
	double operator[](int n) const;
	void operator+=(const Par& p);
	Par operator+(const Par& p) const;
	Par operator*(double x) const;
	friend Par operator*(double x, const Par& p);

	void show();
	void get_data(double* data);
};