// File: Accumulator.hpp, Created on 2014. augusztus 9. 16:07, Author: Vader

#pragma once

namespace libv {

// -------------------------------------------------------------------------------------------------

// template <typename T>
// Concept Accumulator {
//	Accumulator()
//		- Accumulator should be default constructible
//	inline bool add();
//		- returns true on logical shortcut
//	inline T result();
//		- returns the accumulated results
//};

// -------------------------------------------------------------------------------------------------

struct AccumulatorVoid {
};

template <typename T>
struct AccumulatorSum {
	T accumulatedValue;
	T result() const {
		return accumulatedValue;
	}
	inline bool add(const T& value) {
		accumulatedValue += value;
		return true;
	}
};

template <typename T>
struct AccumulatorLast {
	T accumulatedValue{};
	T result() const {
		return accumulatedValue;
	}
	inline bool add(const T& value) {
		accumulatedValue = value;
		return true;
	}
};

template <typename T>
struct AccumulatorLogicalAnd {
	T accumulatedValue{true};
	T result() const {
		return accumulatedValue;
	}
	inline bool add(const T& value) {
		accumulatedValue = accumulatedValue && value;
		return accumulatedValue;
	}
};

template <typename T>
struct AccumulatorLogicalOr {
	T accumulatedValue{false};
	T result() const {
		return accumulatedValue;
	}
	inline bool add(const T& value) {
		accumulatedValue = accumulatedValue || value;
		return !accumulatedValue;
	}
};

// -------------------------------------------------------------------------------------------------

} //namespace libv
