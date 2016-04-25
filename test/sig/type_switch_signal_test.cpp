// File: type_switch_signal_test.cpp, Created on 2016.04.25. 7:09, Author: Vader

#include <catch.hpp>

#include "signal_test_util.hpp"
#include <libv/sig/signal.hpp>

// -------------------------------------------------------------------------------------------------

using libv::SwitchSignal;

// === SwitchSignal ===========================================================================

TEST_CASE("SwitchSignal Test") {
	SwitchSignal<int> source;
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
