// File: Accumulator.hpp, Created on 2014. augusztus 9. 16:07, Author: Vader

#pragma once

namespace libv {

// IDEA: Instead of accumulator_tag, i could use a common empty base class.

// -------------------------------------------------------------------------------------------------

// template <typename T>
// Concept Accumulator {
//	using accumulator_tag = void;
//	Accumulator()
//		- Accumulator should be default constructible
//	inline bool add();
//		- returns true on logical shortcut
//	inline T result();
//		- returns the accumulated results
//};
//
// template <>
// Concept Accumulator<void> {
//	using accumulator_tag = void;
//};

// -------------------------------------------------------------------------------------------------

struct AccumulatorVoid {
	using accumulator_tag = void;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
struct AccumulatorSum {
	using accumulator_tag = void;
	T accumulatedValue;
	T result() const {
		return accumulatedValue;
	}
	inline bool add(const T& value) {
		accumulatedValue += value;
		return true;
	}
};

template <>
struct AccumulatorSum<void> {
	using accumulator_tag = void;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
struct AccumulatorLast {
	using accumulator_tag = void;
	T accumulatedValue{};
	T result() const {
		return accumulatedValue;
	}
	inline bool add(const T& value) {
		accumulatedValue = value;
		return true;
	}
};

template <>
struct AccumulatorLast<void> {
	using accumulator_tag = void;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
struct AccumulatorAnd {
	using accumulator_tag = void;
	T accumulatedValue{true};
	T result() const {
		return accumulatedValue;
	}
	inline bool add(const T& value) {
		accumulatedValue = accumulatedValue && value;
		return accumulatedValue;
	}
};

template <>
struct AccumulatorAnd<void> {
	using accumulator_tag = void;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
struct AccumulatorOr {
	using accumulator_tag = void;
	T accumulatedValue{false};
	T result() const {
		return accumulatedValue;
	}
	inline bool add(const T& value) {
		accumulatedValue = accumulatedValue || value;
		return !accumulatedValue;
	}
};

template <>
struct AccumulatorOr<void> {
	using accumulator_tag = void;
};

// -------------------------------------------------------------------------------------------------

} //namespace libv
