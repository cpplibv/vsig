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

	// Invalid parameter compositions which should result in static assert:
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


template <template <typename...> class SignalType>
void static_compile_time_module_same_test() {
	using Ref0 = SignalType<void(int)>;
	using Ref1 = SignalType<int(int)>;
	using Ref2 = SignalType<void(int, int)>;
	using Ref3 = SignalType<void(void)>;

	CHECK((std::is_same<Ref0, SignalType<int>>::value));
	CHECK((std::is_same<Ref0, SignalType<int, SingleThread>>::value));
	CHECK((std::is_same<Ref0, SignalType<void(int), SingleThread>>::value));

	CHECK((std::is_same<Ref1, SignalType<int(int), AccumulatorSum<int>, SingleThread>>::value));
	CHECK((std::is_same<Ref1, SignalType<int(int), SingleThread, AccumulatorSum<int>>>::value));
	CHECK((std::is_same<Ref1, SignalType<AccumulatorSum<int>, int(int), SingleThread>>::value));
	CHECK((std::is_same<Ref1, SignalType<SingleThread, int(int), AccumulatorSum<int>>>::value));
	CHECK((std::is_same<Ref1, SignalType<AccumulatorSum<int>, SingleThread, int(int)>>::value));
	CHECK((std::is_same<Ref1, SignalType<SingleThread, AccumulatorSum<int>, int(int)>>::value));

	CHECK((std::is_same<Ref2, SignalType<int, int>>::value));
	CHECK((std::is_same<Ref2, SignalType<void(int, int)>>::value));
	CHECK((std::is_same<Ref2, SignalType<int, int, SingleThread>>::value));
	CHECK((std::is_same<Ref2, SignalType<void(int, int), SingleThread>>::value));
	CHECK((std::is_same<Ref2, SignalType<SingleThread, int, int>>::value));
	CHECK((std::is_same<Ref2, SignalType<SingleThread, void(int, int)>>::value));

	CHECK((std::is_same<Ref3, SignalType<>>::value));
	CHECK((std::is_same<Ref3, SignalType<void(void)>>::value));
	CHECK((std::is_same<Ref3, SignalType<SingleThread>>::value));
	CHECK((std::is_same<Ref3, SignalType<SingleThread, void(void)>>::value));

	CHECK(!(std::is_same<SignalType<int(int)>, SignalType<void(int)>>::value));
}

TEST_CASE("Static compile time module test of alias sameness") {
	static_compile_time_module_same_test<Signal>();
	static_compile_time_module_same_test<CapacitiveSignal>();
	static_compile_time_module_same_test<SwitchSignal>();
	static_compile_time_module_same_test<HistorySignal>();
	static_compile_time_module_same_test<ConditionalSignal>();
}

// -------------------------------------------------------------------------------------------------
