// File: main.cpp, Created on 2014. április 25. 21:23, Author: Vader

#include <functional>
#include <iostream>
#include <type_traits>
#include <mutex>
#include <vector>
#include <memory>

//#include <libv/sig/accumulator.hpp>
#include <libv/sig/signal.hpp>
//#include <libv/sig/type_traits.hpp>
namespace libv {
} //namespace libv

using namespace libv;

// -------------------------------------------------------------------------------------------------

int main(int, char**) {
	Signal<int> s;

	std::cout << "-- eof --" << std::endl;
	return 0;
}