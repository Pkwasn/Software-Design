#include <iostream>
#include <chrono>

struct Timer
{
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;

	Timer()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		end = std::chrono::high_resolution_clock::now();
		duration = end - start;

		float ms = duration.count() * 1'000.0f;

		std::cout << "Elapsed time: " << duration.count() << "s or " << ms << "ms" << std::endl;
	}

};

template<typename T, typename Fct>
void test_function(T value, Fct&& func)
{
	{
		Timer timer;
		for (int i = 0; i < 1'000'000; i++)
		{
			func(value);
		}
	}
}

int main()
{
	float f_a = 10.0f;
	int i_b = 10;

	test_function(f_a, [=](float value) { value += 2.0f; }); // [=] is a Lambda capture, more specifically "=" means: "capture variables by copy"
	test_function<float>(f_a, [=](float value) { value *= 2.0f; });	//Specifying the type, optional with this setup (<float>)
	test_function(i_b, [=](int value) { value += 2; });
	test_function<int>(i_b, [=](int value) { value *= 2; });
}
