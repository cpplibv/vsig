// File: signal_test.cpp, Created on 2014. december 8. 2:19, Author: Vader

#include <catch.hpp>

#include "signal_test_util.hpp"
#include <libv/sig/signal.hpp>
#include <thread>

// -------------------------------------------------------------------------------------------------

using libv::Signal;
using libv::CapacitiveSignal;
using libv::SwitchSignal;
using libv::HistorySignal;
using libv::ConditionalSignal;

// -------------------------------------------------------------------------------------------------

TEST_CASE("SignalConstruct") {
	Signal<> testObj;

	CHECK(testObj.outputSize() == 0u);
}

// --- Input / Output ------------------------------------------------------------------------------

// Input member function currently disabled
//TEST_CASE("SignalInputSignal") {
//	Signal<> source, target;
//
//	target.input(source);
//	CHECK(source.outputSize() == 1u);
//	CHECK(target.outputSize() == 0u);
//
//	target.input(&source);
//	CHECK(source.outputSize() == 2u);
//	CHECK(target.outputSize() == 0u);
//}

template <typename Source, typename Target>
struct signal_output_into_signal {
	void operator()() {
		Source source;
		Target target;

		source.output(target);

		CHECK(source.outputSize() == 1u);
		CHECK(target.outputSize() == 0u);

		source.output(&target);
		CHECK(source.outputSize() == 2u);
		CHECK(target.outputSize() == 0u);
	}
};

TEST_CASE("Signal output into Signal") {
	test_type_permutator_2<signal_output_into_signal, Signal<>, CapacitiveSignal<>>();
	test_type_permutator_2<signal_output_into_signal, Signal<>, HistorySignal<>>();
	test_type_permutator_2<signal_output_into_signal, Signal<>, SwitchSignal<>>();
	test_type_permutator_2<signal_output_into_signal, Signal<>, ConditionalSignal<>>();

	test_type_permutator_2<signal_output_into_signal, CapacitiveSignal<>, HistorySignal<>>();
	test_type_permutator_2<signal_output_into_signal, CapacitiveSignal<>, SwitchSignal<>>();
	test_type_permutator_2<signal_output_into_signal, CapacitiveSignal<>, ConditionalSignal<>>();

	test_type_permutator_2<signal_output_into_signal, HistorySignal<>, SwitchSignal<>>();
	test_type_permutator_2<signal_output_into_signal, HistorySignal<>, ConditionalSignal<>>();

	test_type_permutator_2<signal_output_into_signal, SwitchSignal<>, ConditionalSignal<>>();
}

TEST_CASE("SignalRelaySignal") {
	Signal<> source, relay, target;

//	relay.input(source);
	source.output(relay);
	relay.output(target);
	CHECK(source.outputSize() == 1u);
	CHECK(relay.outputSize() == 1u);
	CHECK(target.outputSize() == 0u);
}

TEST_CASE("SignalOutputLamda") {
	Signal<> source;

	source.output([] {
	});
	CHECK(source.outputSize() == 1u);
}

TEST_CASE("SignalOutputStdFunction") {
	Signal<> source;

	source.output(std::function<void()>(dummyGlobalFunction<void>));
	CHECK(source.outputSize() == 1u);
}

TEST_CASE("SignalOutputStdBind") {
	Signal<> source;

	source.output(std::bind(dummyGlobalFunction<void, int>, 1));
	CHECK(source.outputSize() == 1u);
}

TEST_CASE("SignalOutputGloablFunction") {
	Signal<> source;

	source.output(dummyGlobalFunction<>);
	CHECK(source.outputSize() == 1u);

	source.output(&dummyGlobalFunction<>);
	CHECK(source.outputSize() == 2u);
}

TEST_CASE("SignalOutputStaticFunction") {
	Signal<> source;

	source.output(dummyType<>::staticFunction);
	CHECK(source.outputSize() == 1u);

	source.output(&dummyType<>::staticFunction);
	CHECK(source.outputSize() == 2u);
}

TEST_CASE("SignalOutputMemberFunction") {
	Signal<> source;
	dummyType<> target;

	source.output(target, &dummyType<>::memberFunction);
	CHECK(source.outputSize() == 1u);

	source.output(&target, &dummyType<>::memberFunction);
	CHECK(source.outputSize() == 2u);
}

// --- Connection Lifetime -------------------------------------------------------------------------

TEST_CASE("SignalConnectionSignalLifeTime") {
	Signal<> source;
	{
		Signal<> target;
		source.output(target);
		CHECK(source.outputSize() == 1u);
	}
	source.fire(); // Current system working of fire initiated garbage collection based output cleanup
	CHECK(source.outputSize() == 0u);
}

TEST_CASE("SignalConnectionTrackableLifeTime") {
	Signal<> source;
	{
		dummyType<> dummyTarget;
		source.output(dummyTarget, &dummyType<>::memberFunction);
		CHECK(source.outputSize() == 1u);
	}
	source.fire(); // Current system working of fire initiated garbage collection based output cleanup
	CHECK(source.outputSize() == 0u);
}

TEST_CASE("SignalConnectionGlobalLifeTime") {
	Signal<> source;
	{
		source.output(dummyGlobalFunction<>);
		CHECK(source.outputSize() == 1u);
	}
	CHECK(source.outputSize() == 1u);
}

// --- Fire ----------------------------------------------------------------------------------------

TEST_CASE("SignalFire") {
	Signal<> source;
	SpyResultTypeFor(source) result;
	source.output(spyInto<void>(result));

	CHECK(result.size() == 0u);
	source.fire();
	CHECK(result.size() == 1u);
	source.fire();
	source.fire();
	CHECK(result.size() == 3u);
}

TEST_CASE("SignalRelayFire") {
	Signal<> source, relay;
	SpyResultTypeFor(source) result;
	source.output(relay);
	relay.output(spyInto<void>(result));

	CHECK(result.size() == 0u);
	source.fire();
	CHECK(result.size() == 1u);
	source.fire();
	source.fire();
	CHECK(result.size() == 3u);
}

TEST_CASE("SignalFireArgs") {
	Signal<int, int> source;
	SpyResultTypeFor(source) result;
	source.output(spyInto<void, int, int>(result));

	source.fire(0, 1);
	source.fire(1, 2);
	REQUIRE(result.size() == 2u);
	CHECK(result[0] == std::make_tuple(0, 1));
	CHECK(result[1] == std::make_tuple(1, 2));
}

TEST_CASE("SignalFireConstArgs") {
	Signal<const int, const int> source;
	SpyResultTypeFor(source) result;
	source.output(spyInto<void, const int, const int>(result));

	source.fire(0, 1);
	source.fire(1, 2);
	REQUIRE(result.size() == 2u);
	CHECK(result[0] == std::make_tuple(0, 1));
	CHECK(result[1] == std::make_tuple(1, 2));
}

TEST_CASE("SignalFireStringArgs") {
	Signal<const char*, const std::string&> source;
	SpyResultTypeFor(source) result;
	source.output(spyInto<void, const char*, const std::string&>(result));

	const char* hello = "Hello";
	std::string world("World!");

	source.fire(hello, "World!");
	source.fire(hello, world);

	REQUIRE(result.size() == 2u);
	CHECK(std::get<0>(result[0]) == hello);
	CHECK(std::get<1>(result[0]) == "World!");
	CHECK(std::get<1>(result[0]) == world);
	CHECK(std::get<0>(result[1]) == hello);
	CHECK(std::get<1>(result[1]) == "World!");
	CHECK(std::get<1>(result[1]) == world);
}

TEST_CASE("SignalFireArgsLValueRef") {
	bool recieved = false;
	int x = 2;
	std::string y = "Test string";
	Signal<int&, std::string&> source;
	source.output([&x, &y, &recieved](int& a, std::string & b) {
		recieved = true;
		CHECK(a == 2);
				CHECK(b == std::string("Test string"));
				CHECK(&a == &x);
				CHECK(&b == &y);
	});
	source.fire(x, y);
	CHECK(recieved);
}

TEST_CASE("SignalFireArgsConstLValueRef") {
	bool recieved = false;
	const int x = 2;
	const std::string y = "Test string";
	Signal<const int&, const std::string&> source;
	source.output([&x, &y, &recieved](const int& a, const std::string & b) {
		recieved = true;
		CHECK(a == 2);
				CHECK(b == std::string("Test string"));
				CHECK(&a == &x);
				CHECK(&b == &y);
	});
	source.fire(x, y);
	CHECK(recieved);
}

TEST_CASE("SignalFireArgsRValueRef") {
	bool recieved = false;
	int x = 2;
	std::string y = "Test string";
	Signal<int&&, std::string&&> source;
	source.output([&x, &y, &recieved](int&& a, std::string && b) {
		recieved = true;
		CHECK(a == 2);
				CHECK(b == std::string("Test string"));
				CHECK(&a == &x);
				CHECK(b.data() == y.data());
	});
	source.fire(std::move(x), std::move(y));
	CHECK(recieved);
}

// --- Return --------------------------------------------------------------------------------------

TEST_CASE("SignalReturnTypeVoidFromDefault") {
	Signal<> source0;
	Signal<int> source1;

	static_assert(std::is_same < decltype(source0.fire()), void>::value, "");
	static_assert(std::is_same < decltype(source1.fire(0)), void>::value, "");
}

TEST_CASE("SignalReturnTypeVoidFromSignature") {
	Signal<void() > source0;
	Signal<void(void) > source1;
	Signal<void(int) > source2;

	static_assert(std::is_same < decltype(source0.fire()), void>::value, "");
	static_assert(std::is_same < decltype(source1.fire()), void>::value, "");
	static_assert(std::is_same < decltype(source2.fire(0)), void>::value, "");
}

TEST_CASE("SignalReturnTypeRTypeFromSignature") {
	Signal<int() > source0;
	Signal<int(void) > source1;
	Signal<int(int) > source2;

	static_assert(std::is_same < decltype(source0.fire()), int>::value, "");
	static_assert(std::is_same < decltype(source1.fire()), int>::value, "");
	static_assert(std::is_same < decltype(source2.fire(0)), int>::value, "");
}

TEST_CASE("SignalReturnValue") {
	Signal<int() > source;
	source.output([] {
		return 42;
	});
	CHECK(source.fire() == 42);
}

// --- Operator() ----------------------------------------------------------------------------------
// --- Accumulator ---------------------------------------------------------------------------------

TEST_CASE("SignalReturnDefaultAccumulation") {
	Signal<int() > source;
	source.output([] {
		return 12;
	});
	source.output([] {
		return 30;
	});
	CHECK(source.fire() == 42);
}

// --- Disconnect ----------------------------------------------------------------------------------
// --- Clear ---------------------------------------------------------------------------------------
// --- DisconnectAllSignal -------------------------------------------------------------------------





// === MultiThread =================================================================================

void foo(Signal<int, libv::MultiThread>* s) {
	for (int i = 0; i < 1000; i++) {
		s->fire(i);
	}
}

TEST_CASE("Signal multi thread stress") {
	Signal<int, libv::MultiThread> source;
	Signal<int, libv::MultiThread> target;

	std::thread t0(foo, &source);
	std::thread t1(foo, &source);

	source.output(target);

	t0.join();
	t1.join();
}
