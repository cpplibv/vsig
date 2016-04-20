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

	CHECK(true);
}

TEST_CASE("Static compile time modul test") {
	static_compile_time_module_test<Signal>();
	static_compile_time_module_test<CapacitivSignal>();
	static_compile_time_module_test<SwitchSignal>();
	static_compile_time_module_test<HistorySignal>();
}

// -------------------------------------------------------------------------------------------------

//SignalType<int(int), int(int)> sig4; // invalid
//SignalType<AccumulatorSum<int>> sig4; // invalid
//SignalType<int, AccumulatorSum<int>> sig4; // invalid
//SignalType<int, SingleThread> sig4; // invalid
//SignalType<int, SingleThread> sig4; // invalid
//SignalType<SingleThread> sig4; // invalid
//SignalType<AccumulatorSum<int>, SingleThread> sig4; // invalid
//
//SignalType<SingleThread, int(int)> sig4; // it could be valid
//SignalType<SingleThread, AccumulatorSum<int>, int(int)> sig4; // it could be valid