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
//	inline operator bool();
//		- only required if operator,(const T&, Accumulator<T>&) would be ill formed (exp: T = void)
//		- returns true on logical shortcut
//	inline T result();
//		- returns the accumulated results
//};

// -------------------------------------------------------------------------------------------------

template <typename T, T initialValue = T{}>
struct Accumulator {
	T _result{initialValue};

	inline T& result() {
		return _result;
	}
};

struct AccumulatorVoid {
	inline bool add() {
		return true;
	}
	inline operator bool() {
		return true;
	}
	inline void result() { }
};

// T and K should be related, CV at least, this cant stay this way... T K for operator, ... nope.
template <typename T, typename Acc>
bool operator,(T val, Acc& acc) {
	return acc.add(val);
}
//template <typename T, typename K, K L>
//bool operator,(T val, ::libv::Accumulator<K, L>& acc) {
//	return acc.add(val);
//}

// -------------------------------------------------------------------------------------------------

template <typename T>
struct AccumulatorSum : public Accumulator<T> {
	inline bool add(const T& value) {
		this->_result += value;
		return true;
	}
};

template <typename T>
struct AccumulatorLast : public Accumulator<T> {
	inline bool add(const T& value) {
		this->_result = value;
		return true;
	}
};

template <typename T>
struct AccumulatorLogicalAnd : public Accumulator<T, true> {
	inline bool add(const T& value) override {
		this->_result = this->_result && value;
		return !this->_result;
	}
};

template <typename T>
struct AccumulatorLogicalOr : public Accumulator<T, false> {
	inline bool add(const T& value) override {
		this->_result = this->_result || value;
		return this->_result;
	}
};

// -------------------------------------------------------------------------------------------------

} //namespace libv
