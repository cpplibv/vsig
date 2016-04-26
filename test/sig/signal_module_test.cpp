// File: VecTest.cpp, Created on 2014. december 8. 2:19, Author: Vader

#include <catch.hpp>

#include <libv/sig/signal.hpp>

using namespace libv;

// -------------------------------------------------------------------------------------------------

template <template <typename...> class SignalType>
void static_compile_time_module_test() {
	SignalType<int(int), AccumulatorOr<int>> sig0;
	SignalType<int(int), SingleThread> sig1;
	SignalType<int(int), AccumulatorSum<int>, SingleThread> sig2;
	SignalType<int(int), SingleThread, AccumulatorSum<int>> sig3;
	SignalType<int, int> sig5;
	SignalType<SingleThread, int(int)> sig6;
	SignalType<SingleThread, AccumulatorSum<int>, int(int)> sig7;
	SignalType<SingleThread> sig8;
	SignalType<SingleThread, int> sig9;

//	SignalType<int, int(int)> sig10; // invalid
//	SignalType<int(int), int(int)> sig11; // invalid
//	SignalType<SingleThread, SingleThread> sig12; // invalid
	CHECK(true);
}

TEST_CASE("Static compile time module test") {
	static_compile_time_module_test<Signal>();
	static_compile_time_module_test<CapacitiveSignal>();
	static_compile_time_module_test<SwitchSignal>();
	static_compile_time_module_test<HistorySignal>();
	static_compile_time_module_test<ConditionalSignal>();
}

// -------------------------------------------------------------------------------------------------
