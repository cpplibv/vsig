// File: main.cpp, Created on 2014. április 25. 21:23, Author: Vader

#include <iostream>
#include <libv/sig/signal.hpp>

int main() {
	libv::RoutingSignal<int(int), libv::AccumulatorSum<int>, libv::RoutingFirstArgAsInRange<int>> source;

	source.output(libv::RoutingRangeAddress<int>{5, 15}, [](int){ return 1; });
	source.output(10, 20, [](int){ return 2; });

	std::cout << source(6) << std::endl; //output: 1
	std::cout << source(12) << std::endl; //output: 3
}
