#include "dynamic.h"

Par::Par(double* initdata) : val(8)
{
	for (int i = 0; i < 3; i++)
	{
		m[i] = initdata[i];
	}

	setConstant();

	for (int i = 0; i < 6; i++)
	{
		val[i] = initdata[i+3];
	}
	
	setAngle(initdata[9]);
}

void Par::setConstant()
{
	c[0] = m[0] * m[1] * sqrt(my::G / (m[0] + m[1])); // constant of L1
	c[1] = (m[0] + m[1]) * m[2] * sqrt(my::G / (m[0] + m[1] + m[2])); // constant of L2
	c[2] = my::G * my::G / 16.0 * pow((m[0] + m[1]) * m[2], 7) * pow((m[0] + m[1] + m[2]) * m[0] * m[1], -3); // constant of C2
}

void Par::setAngle(double angle, int type)
{
	double a, b;
	angle = acos(cos(angle));
	switch (type)
	{
	case 0:
		b = G2();
		a = G1() + b * cos(angle);
		b *= sin(angle);
		if (angle == M_PI and a == G2())
		{
			val[6] = M_PI / 2;
		}
		else
		{
			val[6] = acos(a / sqrt(a * a + b * b));
		}
		val[7] = angle - val[6];
		break;
		/*case 1:
			i[1] = angle;
			i[2] = asin(cp.G[0] / cp.G[1] * sin(i[1]));
			i[0] = i[1] + i[2];
			break;
		case 2:
			i[2] = angle;
			i[1] = asin(cp.G[1] * sin(i[2]) / cp.G[0]);
			i[0] = i[1] + i[2];
			break;*/
	default:
		break;
	}
}

double Par::L1() const
{
	return c[0] * sqrt(val[0]);
}

double Par::L2() const
{
	return c[1] * sqrt(val[1]);
}

double Par::G1() const
{
	return L1() * sqrt(1.0 - val[2] * val[2]);
}

double Par::G2() const
{
	return L2() * sqrt(1.0 - val[3] * val[3]);
}

double Par::Theta()
{
	return val[6] + val[7];
}

Par Par::absolute() const
{
	Par res(*this);
	res.val = res.val.absolute();
	return res;
}

double Par::maximum() const
{
	return val.maximum();
}

double Par::abs_max() const
{
	return absolute().maximum();
}

bool Par::valid()
{
	return val[2] >= 0 and val[2] < 1 and val[3] >= 0 and val[3] < 1;
}

Par Par::func(const Par& p)
{
	Par res(p);
	double a1 = p.val[0], a2 = p.val[1], e1 = p.val[2], e2 = p.val[3], w1 = p.val[4], w2 = p.val[5]; // a1a2e1e2w1w2
	double e12 = e1 * e1, e22 = e2 * e2;
	double i1 = p.val[6], i2 = p.val[7], t = i1 + i2; // angle
	double sint = sin(t), cost = cos(t), sint2 = sint * sint, cost2 = cost * cost; // sint2 = sin(t)*sin(t), cost2=cos(t)*cost(t)
	double sini1 = sin(i1), cosi1 = cos(i1), sini2 = sin(i2), cosi2 = cos(i2);
	double sinw1 = sin(w1), sinw2 = sin(w2), cosw1 = cos(w1), cosw2 = cos(w2);
	double sin2w1 = sin(2 * w1), cos2w1 = cos(2 * w1);
	double g1 = p.G1(), g2 = p.G2();
	double c2 = p.c[2] * pow(p.L1(), 4) * pow(p.L2() * g2, -3);
	double c3 = -c2 * 15 / 4.0 * (p.m[0] - p.m[1])/(p.m[0]+p.m[1])*a1/a2/(1-e22);
	double c2g1 = c2 / g1, c2g2 = c2 / g2, c3g1 = c3 / g1, c3g2 = c3 / g2; // c2g1 = c2/g1,c2g2 = c2/g2;
	double B = 2 + 5 * e12 - 7 * e12 * cos2w1;
	double A = 4 + 3 * e12 - 2.5 * B * sint2;
	double phi1, phi2, phi3;
	double dg1dt, dg2dt, dhdt; // dg1 = dg1/dt, dh=dh1/dt

	phi1 = -cosw1 * cosw2 - cost * sinw1 * sinw2; // cosphi
	phi2 = sinw1 * cosw2 - cost * cosw1 * sinw2;
	phi3 = cosw1 * sinw2 - cost * sinw1 * cosw2;

	// da/dt
	res.val[0] = 0; // da1/dt
	res.val[1] = 0; // da2/dt

	// de/dt
	res.val[2] = c2g1 * (1-e12) * 30*e1*sint2*sin2w1 +
		c3g1*e2*(1-e12)*(35*phi1*sint2*e12*sin2w1 -
			10*cost*sint2*cosw1*sinw2*(1-e12) -
			A*phi2); // de1/dt
	res.val[3] = -c3g2 * e1 * (1 - e22) * (10 * cost * sint2 * (1 - e12) * sinw1 * cosw2 +
		A * phi3); // de2/dt

	double de1dt = res.val[2], de2dt = res.val[3];

	//dw/dt
	res.val[4] = 6 * (c2g1 * (4 * cost2 + (5 * cos2w1 - 1) * (1 - e12 - cost2)) +
		cost * c2g2 * (2 + e12 * (3 - 5 * cos2w1)));
	if (e1 != 0)
	//if (true)
	{
		res.val[4] -=
			e2 * (e1 * (c3g2 + cost * c3g1) * (sinw1 * sinw2 * (10 * (3 * cost2 - 1) * (1 - e12) + A) -
				5 * B * cost * phi1) -
				c3g1 * (1 - e12) / e1 * (sinw1 * sinw2 * 10 * cost * sint2 * (1 - 3 * e12) +
					phi1 * (3 * A - 10 * cost2 + 2)));
	}

	res.val[5] = 3 * (2 * cost * c2g1 * (2 + e12 * (3 - 5 * cos2w1)) +
		c2g2 * (4 + 6 * e12 + (5 * cost2 - 3) * (2 + e12 * (3 - 5 * cos2w1))));
	if (e2 != 0)
	//if (true)
	{
		res.val[5] +=
			e1 * (sinw1 * sinw2 * (c3g2 * (1 + 4 * e22) / e2 * 10 * cost * sint2 * (1 - e12) -
				e2 * (c3g1 + cost * c3g2) * (A + 10 * (3 * cost2 - 1) * (1 - e12))) +
				phi1 * (5 * B * cost * e2 * (c3g1 + cost * c3g2) + c3g2 * (1 + 4 * e22) / e2 * A));
	}

	double dw1dt = res.val[4], dw2dt = res.val[5];

	// dg1/dt,dg2/dt, prepare for dh1/dt, dh2/dt
	dg1dt = -c2 * 30 * e12 * sin2w1 * sint2 +
		c3*e1*e2*(-35*e12*sint2*sin2w1*phi1+A*phi2 +
			10*cost*sint2*(1-e12)*cosw1*sinw2);
	dg2dt = c3*e1*e2*(A*phi3 + 10*cost*sint2*(1-e12)*sinw1*cosw2);

	// dh1/dt, prepare for di1/dt, di2/dt
	dhdt = (sini2 * dg1dt - sini1 * dg2dt) / sint;

	//di/dt
	res.val[6] = (dg1dt*cosi1-dhdt)/(g1*sini1);
	res.val[7] = (dg2dt*cosi2+dhdt)/(g2*sini2);

	return res;
}

void Par::operator+=(const Par& p)
{
	val += p.val;
}

Par Par::operator+(const Par& p) const
{
	Par res(*this);
	res.val += p.val;
	return res;
}

double Par::operator[](int n) const
{
	return val[n];
}

Par Par::operator*(double x) const
{
	Par res(*this);
	res.val *= x;
	return res;
}

void Par::show()
{
	std::cout << val[2] << "\t" << val[3] << std::endl;
}

void Par::get_data(double* data)
{
	data[1] = val[2];
	data[2] = val[3];
}

Par operator*(double x, const Par& p)
{
	return p * x;
}
