// File: Accumulator.hpp, Created on 2014. augusztus 9. 16:07, Author: Vader

#pragma once

#include "type_traits.hpp"

namespace libv {

// IDEA: Instead of accumulator_tag, i could use a common empty base class.

// -------------------------------------------------------------------------------------------------

// template <typename T, typename R = T>
// Concept Accumulator {
//	using accumulator_tag = void;
//	Accumulator()
//		- Accumulator should be default constructible
//	inline bool add();
//		- returns true on logical shortcut, marking the last necessary call
//	inline R result();
//		- returns the accumulated results
//};
//
// template <>
// Concept Accumulator<void> {
//	using accumulator_tag = void;
//};

// -------------------------------------------------------------------------------------------------

template <typename T>
struct _accumulator_traits_add_helper {
	template <typename A, typename F, typename... Args>
	static inline bool add(A& acc, F&& func, Args&&... args) {
		return acc.add(func(std::forward<Args>(args)...));
	}
};

template <>
struct _accumulator_traits_add_helper<void> {
	template <typename A, typename F, typename... Args>
	static inline bool add(A& acc, F&& func, Args&&... args) {
		func(std::forward<Args>(args)...);
		return acc.add();
	}
};

template <typename Accumulator>
struct accumulator_traits {
	static_assert(is_module<Accumulator, tag::accumulator>::value,
			"accumulator_traits can only be used with an accumulator.\n"
			"for more information see accumulator concept");
	using result_type = decltype(std::declval<Accumulator>().result());
	static inline Accumulator create() {
		return Accumulator();
	}
	template <typename R, typename... FArgs, typename... Args>
	static inline bool add(Accumulator& acc, std::function<R(FArgs...)>& func, Args&&... args) {
		return _accumulator_traits_add_helper<R>::add(acc, func, std::forward<Args>(args)...);
	}
	static inline result_type result(Accumulator & acc) {
		return acc.result();
	}
};

// -------------------------------------------------------------------------------------------------

struct AccumulatorBase {
	using module = tag_type<tag::accumulator>;
	inline void result() { };
	inline bool add() {
		return false;
	};
};

// =================================================================================================

template <typename T>
struct AccumulatorSum : AccumulatorBase {
	T accumulatedValue;
	inline T result() const {
		return accumulatedValue;
	}
	inline bool add(const T& value) {
		accumulatedValue += value;
		return false;
	}
};

template <>
struct AccumulatorSum<void> : AccumulatorBase {
};

// -------------------------------------------------------------------------------------------------

template <typename T>
struct AccumulatorLast : AccumulatorBase {
	T accumulatedValue{};
	inline T result() const {
		return accumulatedValue;
	}
	inline bool add(const T& value) {
		accumulatedValue = value;
		return false;
	}
};

template <>
struct AccumulatorLast<void> : AccumulatorBase {
};

// -------------------------------------------------------------------------------------------------

template <typename T>
struct AccumulatorAnd : AccumulatorBase {
	T accumulatedValue{true};
	inline T result() const {
		return accumulatedValue;
	}
	inline bool add(const T& value) {
		accumulatedValue = accumulatedValue && value;
		return !accumulatedValue;
	}
};

template <>
struct AccumulatorAnd<void> : AccumulatorBase {
};

// -------------------------------------------------------------------------------------------------

template <typename T>
struct AccumulatorOr : AccumulatorBase {
	T accumulatedValue{false};
	inline T result() const {
		return accumulatedValue;
	}
	inline bool add(const T& value) {
		accumulatedValue = accumulatedValue || value;
		return accumulatedValue;
	}
};

template <>
struct AccumulatorOr<void> : AccumulatorBase {
};

// -------------------------------------------------------------------------------------------------

template <typename T>
struct AccumulatorCounter : AccumulatorBase {
	size_t counter = 0;
	inline size_t result() const {
		return counter;
	}
	inline bool add(const T&) {
		++counter;
		return false;
	}
};

template <>
struct AccumulatorCounter<void> : AccumulatorBase {
	size_t counter = 0;
	inline size_t result() const {
		return counter;
	}
	inline bool add() {
		++counter;
		return false;
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T, size_t N>
struct AccumulatorLimiter : AccumulatorBase {
	size_t counter = 0;
	inline size_t result() const {
		return counter;
	}
	inline bool add(const T&) {
		++counter;
		return counter >= N;
	}
};

template <size_t N>
struct AccumulatorLimiter<void, N> : AccumulatorBase {
	size_t counter = 0;
	inline size_t result() const {
		return counter;
	}
	inline bool add() {
		++counter;
		return counter >= N;
	}
};

} //namespace libv
