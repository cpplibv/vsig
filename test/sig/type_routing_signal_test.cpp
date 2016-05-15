// File: type_routing_signal_test.cpp, Created on 2016.04.28. 18:31, Author: Vader

#include <catch.hpp>

#include "signal_test_util.hpp"
#include <libv/sig/signal.hpp>

// -------------------------------------------------------------------------------------------------

using libv::RoutingSignal;

// === RoutingSignal ============================================================================

TEST_CASE("RoutingSignal test RoutingFirstArgAsAddress") {
	RoutingSignal<int(int), libv::RoutingFirstArgAsAddress<int>> source;

	SECTION("No output") {
		CHECK(source.fire(3) == 0);
	}
	SECTION("Single match") {
		source.output(1, [](int){ return 11; });
		source.output(2, [](int){ return 12; });
		source.output(3, [](int){ return 13; });

		CHECK(source.fire(3) == 13);
		CHECK(source.fire(3) == 13);
		CHECK(source.fire(2) == 12);
	}
	SECTION("Multiple match") {
		source.output(1, [](int){ return 11; });
		source.output(2, [](int){ return 12; });
		source.output(3, [](int){ return 13; });
		source.output(3, [](int){ return 10; });

		CHECK(source.fire(3) == 23);
		CHECK(source.fire(3) == 23);
		CHECK(source.fire(2) == 12);
	}
}

TEST_CASE("RoutingSignal test RoutingFirstArgAsInRange") {
	RoutingSignal<int(int), libv::AccumulatorSum<int>, libv::RoutingFirstArgAsInRange<int>> source;

	SECTION("No output") {
		CHECK(source.fire(3) == 0);
	}
	SECTION("Single match") {
		source.output(libv::RoutingRangeAddress<int>{1, 10}, [](int){ return 11; });
		source.output(libv::RoutingRangeAddress<int>{5, 15}, [](int){ return 12; });
		source.output(libv::RoutingRangeAddress<int>{10, 20}, [](int){ return 13; });

		CHECK(source.fire(5) == 11);
		CHECK(source.fire(10) == 12);
		CHECK(source.fire(18) == 13);
	}
	SECTION("Multiple match") {
		source.output(libv::RoutingRangeAddress<int>{1, 10}, [](int){ return 11; });
		source.output(libv::RoutingRangeAddress<int>{5, 15}, [](int){ return 12; });
		source.output(libv::RoutingRangeAddress<int>{10, 20}, [](int){ return 13; });

		CHECK(source.fire(6) == 23);
		CHECK(source.fire(12) == 25);
	}
	SECTION("Single match output rematch") {
		source.output(1, 10, [](int){ return 11; });
		source.output(5, 15, [](int){ return 12; });
		source.output(10, 20, [](int){ return 13; });

		CHECK(source.fire(5) == 11);
		CHECK(source.fire(10) == 12);
		CHECK(source.fire(18) == 13);
	}
	SECTION("Multiple match output rematch") {
		source.output(1, 10, [](int){ return 11; });
		source.output(5, 15, [](int){ return 12; });
		source.output(10, 20, [](int){ return 13; });

		CHECK(source.fire(6) == 23);
		CHECK(source.fire(12) == 25);
	}
}
