// File: main.cpp, Created on 2014. április 25. 21:23, Author: Vader

#include <functional>
#include <iostream>
#include <type_traits>
#include <mutex>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <thread>

//#include <libv/sig/accumulator.hpp>
#include <libv/sig/signal.hpp>
//#include <libv/sig/type_traits.hpp>

namespace libv {
} //namespace libv

using namespace libv;
using namespace std;

void foo(Signal<int>* s, Signal<int>* s2){
	for (int i = 0; i < 2000000; i++) {
		s->output(*s2);
	}
}

// -------------------------------------------------------------------------------------------------

int main(int, char**) {
	Signal<int> source1;
	Signal<int> source2;

	thread t1(foo, &source1, &source2);
	thread t2(foo, &source2, &source1);

	t1.join();
	t2.join();

	return 0;
}

// -------------------------------------------------------------------------------------------------
