#include <iostream>
#include <chrono>
#include <thread>
#include <Windows.h>


int main()
{
	auto mili = [](int count)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(count));
	};

	for (int i = 0; i != 800; ++i)
	{
		SetCursorPos(i, i);
		mili(1);
		std::cout << "(" << i << "," << i << ")" << std::endl;
	}

	system("pause");
	return 0;
}