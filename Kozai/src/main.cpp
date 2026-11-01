#include <iostream>
#include "dynamic.h"
#define _USE_MATH_DEFINES
#include "mymath.h"

int main()
{
	//double initdata[10] = { 1.4, 0.3, 4e6, 5, 50, 0.5, 1e-5,M_PI * (1.2 / 1.8),0, M_PI * (0.7 / 1.8) };
 //   // 3个质量，2个a，2个e，2个w，1个itot
	//Par par(initdata);
	ScrollBuffer sb(10);
	for (int i = 0; i < 12; i++)
	{
		sb.AddPoint(i, 2 * i);
	}
	for (auto& i : sb.Data)
	{
		std::cout << i.x << "\t" << i.y << std::endl;
	}
}