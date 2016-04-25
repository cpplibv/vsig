// File: type_conditional_signal_test.cpp, Created on 2016.04.25. 7:09, Author: Vader

#include <catch.hpp>

#include "signal_test_util.hpp"
#include <libv/sig/signal.hpp>

// -------------------------------------------------------------------------------------------------

using libv::ConditionalSignal;

// === ConditionalSignal ===========================================================================

struct TestConditionAbove50 {
	bool operator()(int x) {
		return x > 50;
	}
};

TEST_CASE("ConditionalSignal Test") {
	using ConditionStaticAbove50 = libv::ConditionStatic<TestConditionAbove50>;

	ConditionalSignal<void(int), ConditionStaticAbove50> source;
	SpyResultTypeFor(source) result;
	source.output(spyInto<void, int>(result));

	SECTION("Condition is met on every output") {
		source.fire(40);
		source.fire(60);
		REQUIRE(result.size() == 1u);
		CHECK(std::get<0>(result[0]) == 60);
	}
}

TEST_CASE("ConditionalSignal Test with DynamicCondition") {
	ConditionalSignal<void(int), libv::ConditionDynamic<int>> source;
	SpyResultTypeFor(source) result;
	source.output(spyInto<void, int>(result));

	SECTION("By default firing reaches the output") {
		source.fire(0);
		source.fire(50);
		source.fire(100);
		CHECK(result.size() == 3u);
	}

	SECTION("Filter can be set") {
		source.setCondition(
				[](int x) {
					return x > 20; }
		);
		source.fire(0);
		source.fire(50);
		source.fire(100);
		REQUIRE(result.size() == 2u);
		CHECK(std::get<0>(result[0]) == 50);
		CHECK(std::get<0>(result[1]) == 100);
	}
}

TEST_CASE("ConditionalSignal Test with multi arg DynamicCondition") {
	ConditionalSignal<void(int, int, double), libv::ConditionDynamic<int, int, double>> source;
	SpyResultTypeFor(source) result;
	source.output(spyInto<void, int, int, double>(result));

	SECTION("By default firing reaches the output") {
		source.fire(51, 20, 4.14);
		source.fire(50, 21, 3.14);
		CHECK(result.size() == 2u);
	}

	SECTION("Filter can be ser") {
		source.setCondition(
				[](int x, int y, double s) {
					return (x > 50 || y > 21) && s > 3; }
		);
		source.fire(51, 20, 4.14);
		source.fire(50, 21, 3.14);
		source.fire(49, 22, 2.14);

		REQUIRE(result.size() == 1u);
		CHECK(std::get<0>(result[0]) == 51);
		CHECK(std::get<1>(result[0]) == 20);
		CHECK(std::get<2>(result[0]) == 4.14);
	}
}

TEST_CASE("ConditionalSignal Test ConditionSwitch") {
	ConditionalSignal<void(int), libv::ConditionSwitch> source;
	SpyResultTypeFor(source) result;
	source.output(spyInto<void, int>(result));

	SECTION("By default firing reaches the output") {
		source.fire(0);
		CHECK(result.size() == 1u);
	}

	SECTION("Output can be disabled") {
		source.disable();
		source.fire(0);
		CHECK(result.size() == 0u);
	}

	SECTION("Output can be re-enabled") {
		source.disable();
		source.enable();
		source.fire(0);
		CHECK(result.size() == 1u);
	}
}
