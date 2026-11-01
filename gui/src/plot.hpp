#pragma once
#include "ui.hpp"
#include <future>
#include <iostream>
#include <mymath.h>

template<typename T>
class MyPlot
{
public:
	MyPlot(T& t, int max_size) : obj(t), myui(t), d1(max_size), d2(max_size), sl(max_size)
	{
		add_point();
	}

	void add_point()
	{
		double data[3];
		obj.get_data(data);
		d1.AddPoint(data[0], data[1]);
		d2.AddPoint(data[0], data[2]);
		if (d1.Data.size() == 1)
		{
			sl.AddPoint(data[0], 0);
		}
		else if (d1.Data.size() < d1.Data.capacity() or d1.Offset == 0)
		{
			sl.AddPoint(data[0], data[0] - d1.Data[d1.Data.size() - 2].x);
		}
		else if (d1.Offset == 1)
		{
			sl.AddPoint(data[0], data[0] - d1.Data[d1.Data.capacity() - 1].x);
		}
		else
		{
			sl.AddPoint(data[0], data[0] - d1.Data[d1.Offset - 2].x);
		}
		//std::cout << sl.Data[sl.Data.size()-1].x << "\t" << sl.Data[sl.Data.size()-1].y << std::endl;
	}

	void plot(int times = 1)
	{
		myui.plot(d1, d2, sl);
		//obj.show();

		if (myui.want_reset())
		{
			if (f._Is_ready())
			{
				obj.reset();
				d1.Erase();
				d2.Erase();
				sl.Erase();
				add_point();
				myui.reset_finish();
			}
		}

		if (!myui.runornot())
		{
			return;
		}

		if (!running)
		{
			f = std::async(std::launch::async, &MyPlot<T>::run, this, times);
			running = true;
		}
		else if (f._Is_ready())
		{
			running = false;
		}
	}

	void run(int times)
	{
		for (size_t i = 0; i < times; i++)
		{
			obj.run(true);
			add_point();
			//d1.show();
			//d2.show();
			//std::cout << d1.Offset << "\t" << d1.Data.size() << std::endl;
		}
	}

private:
	T& obj;
	bool running = false;
	std::future<void> f;
	ui::UI<T> myui;
	ScrollBuffer d1, d2, sl;
};
