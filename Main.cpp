#include <iostream>
#include <chrono>
#include <stack>
#include <random>

int main(int argc, char* argv[])
{
	auto start = std::chrono::steady_clock::now();


	auto end = std::chrono::steady_clock::now();

	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "elapsed time: " << elapsed_seconds.count() << "sec" << std::endl;
}

