// File: type_capacitive_signal_test.cpp, Created on 2016.04.25. 7:09, Author: Vader

#include <catch.hpp>

#include "signal_test_util.hpp"
#include <libv/sig/signal.hpp>

// -------------------------------------------------------------------------------------------------

using libv::CapacitiveSignal;

// === CapacitiveSignal ============================================================================

TEST_CASE("CapacitiveSignal Test") {
	CapacitiveSignal<int> source;
	SpyResultTypeFor(source) result;
	source.output(spyInto<void, int>(result));

	SECTION("Flushing an empty signal result no output") {
		source.flush();
		CHECK(result.size() == 0u);
	}

	SECTION("Firing then flushing reaches the output") {
		source.fire(0);
		CHECK(result.size() == 0u);

		source.flush();
		CHECK(result.size() == 1u);
	}

	SECTION("Firing multiple time then flushing reaches the output") {
		source.fire(0);
		source.fire(0);
		CHECK(result.size() == 0u);

		source.flush();
		CHECK(result.size() == 2u);
	}
}
