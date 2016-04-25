// File: type_conditional_signal_test.cpp, Created on 2016.04.25. 7:09, Author: Vader

#include <catch.hpp>

#include "signal_test_util.hpp"
#include <libv/sig/signal.hpp>

// -------------------------------------------------------------------------------------------------

using libv::ConditionalSignal;

// === SwitchSignal ===========================================================================

TEST_CASE("ConditionalSignal Test") {
//	ConditionalSignal<int> source;
//	SpyResultTypeFor(source) result;
//	source.output(spyInto<void, int>(result));
//
//	SECTION("By default firing reaches the output") {
//		source.fire(0);
//		CHECK(result.size() == 1u);
//	}
//
//	SECTION("Output can be disabled") {
//		source.disable();
//		source.fire(0);
//		CHECK(result.size() == 0u);
//	}
//
//	SECTION("Output can be re-enabled") {
//		source.disable();
//		source.enable();
//		source.fire(0);
//		CHECK(result.size() == 1u);
//	}
}
