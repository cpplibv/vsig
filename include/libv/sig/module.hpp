// File: module.hpp, Created on 2014. augusztus 9. 16:07, Author: Vader

#pragma once

#include <atomic>

#include "tag.hpp"
#include "type_traits.hpp"

namespace libv {

// -------------------------------------------------------------------------------------------------

//
//
// =================================================================================================
//     Accumulator Implementations
// =================================================================================================
//
//

//template <typename T>
//concept bool Accumulator() {
//    return requires(T a) {
////      using module = tag_type<tag::accumulator>;
//        {a.add(R1)} -> Boolean;
//        {a.result()} -> R2;
//    };
//}

// Concept Accumulator {
//	using module = tag_type<tag::accumulator>;
//	Accumulator()
//		- Accumulator should be default constructible
//	inline bool add(R1);
//		- returns true on logical shortcut, marking no more call is necessary
//		- R1 will match the callbacks return type
//		- R1 can be void
//	inline R2 result();
//		- returns the accumulated results
//		- R2 is not restricted to match R1
//		- R2 can be void
//};

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
	static_assert(is_module_tag<Accumulator, tag::accumulator>::value,
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

//
//
// =================================================================================================
//     Condition Implementations
// =================================================================================================
//
//

// Concept Condition {
//	using module = tag_type<tag::condition>;
//	Condition()
//		- Condition should be default constructible
//	inline bool check(Args...);
//		- returns true if the input is valid and distribution is desired
//		- at least protected visibility required
//		- Args... will match the received arguments during fire
// };

template <typename... Args>
struct ConditionDynamic {
	using module = tag_type<tag::condition>;
private:
	std::function<bool(Args...)> func;
protected:
	inline bool check(Args... args){
		return func(std::forward<Args>(args)...);
	}
public:
	ConditionDynamic() {
		func = [](Args...){return true;};
	}

	template <typename F>
	void setCondition(F&& func) {
		this->func = std::forward<F>(func);
	}
};

struct ConditionSwitch {
	using module = tag_type<tag::condition>;
private:
	std::atomic<bool> enabled;
protected:
	template <typename... Args>
	inline bool check(Args&&...) {
		return enabled;
	}
public:
	inline void enable() {
		enabled = true;
	}
	inline void disable() {
		enabled = false;
	}
};

template <typename F>
struct ConditionStatic {
	using module = tag_type<tag::condition>;
private:
	F func;
protected:
	ConditionStatic() = default;
	template <typename... Args>
	inline bool check(Args&&... args){
		return func(std::forward<Args>(args)...);
	}
};

struct ConditionTrue {
	using module = tag_type<tag::condition>;
protected:
	ConditionTrue() = default;
	template <typename... Args>
	inline bool check(Args&&... args){
		return true;
	}
};

// -------------------------------------------------------------------------------------------------

} //namespace libv
