// File: type_history_signal_test.cpp, Created on 2016.04.25. 7:09, Author: Vader

#include <catch.hpp>

#include "signal_test_util.hpp"
#include <libv/sig/signal.hpp>

// -------------------------------------------------------------------------------------------------

using libv::HistorySignal;

// === HistorySignal ===============================================================================

TEST_CASE("HistorySignal Test") {
	HistorySignal<int> source;
	SpyResultTypeFor(source) result1;
	SpyResultTypeFor(source) result2;

	SECTION("Connection without history result no output") {
		source.output(spyInto<void, int>(result1));
		CHECK(result1.size() == 0u);
	}

	SECTION("Fire after connection result output from fire") {
		source.output(spyInto<void, int>(result1));
		source.fire(42);
		REQUIRE(result1.size() == 1u);
		CHECK(std::get<0>(result1[0]) == 42);
	}

	SECTION("Fire before connection result output from histroy") {
		source.fire(42);
		source.output(spyInto<void, int>(result1));
		REQUIRE(result1.size() == 1u);
		CHECK(std::get<0>(result1[0]) == 42);
	}

	SECTION("History calls should match the original order") {
		source.fire(40);
		source.fire(41);
		source.fire(42);
		source.output(spyInto<void, int>(result1));
		REQUIRE(result1.size() == 3u);
		CHECK(std::get<0>(result1[0]) == 40);
		CHECK(std::get<0>(result1[1]) == 41);
		CHECK(std::get<0>(result1[2]) == 42);
	}

	SECTION("Connection with cleared history result no output") {
		source.fire(42);
		source.clearHistory();
		source.output(spyInto<void, int>(result1));
		CHECK(result1.size() == 0u);
	}

	SECTION("Multiple connection should be handled per connection") {
		CHECK(result1.size() == 0u);
		CHECK(result2.size() == 0u);

		source.fire(100);
		source.output(spyInto<void, int>(result1));

		REQUIRE(result1.size() == 1u);
		CHECK(std::get<0>(result1[0]) == 100);
		CHECK(result2.size() == 0u);

		source.fire(200);
		source.output(spyInto<void, int>(result2));

		REQUIRE(result1.size() == 2u);
		CHECK(std::get<0>(result1[0]) == 100);
		CHECK(std::get<0>(result1[1]) == 200);
		REQUIRE(result2.size() == 2u);
		CHECK(std::get<0>(result1[0]) == 100);
		CHECK(std::get<0>(result1[1]) == 200);
	}
}