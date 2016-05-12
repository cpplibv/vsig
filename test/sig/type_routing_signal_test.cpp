// File: type_routing_signal_test.cpp, Created on 2016.04.28. 18:31, Author: Vader

#include <catch.hpp>

#include "signal_test_util.hpp"
#include <libv/sig/signal.hpp>

// -------------------------------------------------------------------------------------------------

using libv::RoutingSignal;

// === RoutingSignal ============================================================================

TEST_CASE("RoutingSignal Test") {
	RoutingSignal<int, libv::RouteAddress<int>> source;

//	SpyResultTypeFor(source) result;
//	source.output(spyInto<void, int>(result));
//
//	SpyResultTypeFor(source) result;
//	source.output(spyInto<void, int>(result));
//
//	SECTION("Flushing an empty signal result no output") {
//		CHECK(result.size() == 0u);
//	}
}
