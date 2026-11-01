#pragma once

namespace cgs
{
	static double G = 6.67349e-8;
	static double Msun = 1.989e33;
	static double au = 1.495978707e13;
	static double y = 8.64e4 * 365;
}

namespace my // units: au, Msun, year
{
	static double G = cgs::G * cgs::Msun * pow(cgs::au, -3) * pow(cgs::y, 2);
	static double Msun = 1.0;
	static double au = 1.0;
	static double y = 1.0;
}