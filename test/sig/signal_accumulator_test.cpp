// File: VecTest.cpp, Created on 2014. december 8. 2:19, Author: Vader

#include <catch.hpp>

#include <libv/sig/signal.hpp>

using namespace libv;

// -------------------------------------------------------------------------------------------------

TEST_CASE("Default signal accumulator should be sum") {
	Signal<int()> signal;

	signal.output([] {
		return 2;
	});

	CHECK(signal.fire() == 2);

	signal.output([] {
		return 3;
	});

	CHECK(signal.fire() == 5);

	signal.output([] {
		return 37;
	});

	CHECK(signal.fire() == 42);
}

// -------------------------------------------------------------------------------------------------

TEST_CASE("AccumulatorAnd should logical and shortcut") {
	Signal<bool(), AccumulatorAnd<bool>> signal;
	int callDepth = 0;

	signal.output([&] {
		++callDepth;
		return false;
	});
	signal.output([&] {
		++callDepth;
		return false;
	});
	signal.output([&] {
		++callDepth;
		return false;
	});

	CHECK(signal.fire() == false);
	CHECK(callDepth == 1);
}

TEST_CASE("AccumulatorAnd should logical and visit all") {
	Signal<bool(), AccumulatorAnd<bool>> signal;
	int callDepth = 0;

	signal.output([&] {
		++callDepth;
		return true;
	});
	signal.output([&] {
		++callDepth;
		return true;
	});
	signal.output([&] {
		++callDepth;
		return true;
	});

	CHECK(signal.fire() == true);
	CHECK(callDepth == 3);
}

// -------------------------------------------------------------------------------------------------

TEST_CASE("AccumulatorAnd should logical or visit all") {
	Signal<bool(), AccumulatorOr<bool>> signal;
	int callDepth = 0;

	signal.output([&] {
		++callDepth;
		return false;
	});
	signal.output([&] {
		++callDepth;
		return false;
	});
	signal.output([&] {
		++callDepth;
		return false;
	});

	CHECK(signal.fire() == false);
	CHECK(callDepth == 3);
}

TEST_CASE("AccumulatorAnd should logical or shortcut") {
	Signal<bool(), AccumulatorOr<bool>> signal;
	int callDepth = 0;

	signal.output([&] {
		++callDepth;
		return true;
	});
	signal.output([&] {
		++callDepth;
		return true;
	});
	signal.output([&] {
		++callDepth;
		return true;
	});

	CHECK(signal.fire() == true);
	CHECK(callDepth == 1);
}

// -------------------------------------------------------------------------------------------------

TEST_CASE("AccumulatorLast should visit all and return last") {
	Signal<int(), AccumulatorLast<int>> signal;
	int callDepth = 0;
	int last = 0;

	signal.output([&] {
		++callDepth;
		return last = 1;
	});
	signal.output([&] {
		++callDepth;
		return last = 2;
	});
	signal.output([&] {
		++callDepth;
		return last = 3;
	});

	CHECK(signal.fire() == last);
	CHECK(callDepth == 3);
}
