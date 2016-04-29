// File: SignalTestUtil.hpp, Created on 2015. január 26. 17:15, Author: Vader

#pragma once

#include <libv/sig/signal.hpp>

#include <functional>
#include <vector>

#define SpyResultTypeFor(SIGNAL) typename SpyResultFor<decltype(SIGNAL)>::type

// -------------------------------------------------------------------------------------------------

template <typename>
struct call_signature_to_arg_vector;

template <typename RType, typename... Args>
struct call_signature_to_arg_vector<RType(Args...)> {
	using type = std::vector<std::tuple<typename std::remove_reference<Args>::type...>>;
};

// -------------------------------------------------------------------------------------------------

template<typename...> struct SpyResultFor;

template<template <typename...> class S, typename... Moduls>
struct SpyResultFor<S<Moduls...>> {
	using signature = typename libv::select_call_signature<Moduls...>::type;
	using type = typename call_signature_to_arg_vector<signature>::type;
};

// -------------------------------------------------------------------------------------------------

template<typename R, typename... Args, typename T>
std::function<R(Args...)> spyInto(T& result) {
	return [&result](Args... args) {
		result.emplace_back(args...);
	};
}

// --- Dummy targets -------------------------------------------------------------------------------
template<typename R = void, typename... Args>
R dummyGlobalFunction(Args...) { }

template<typename R = void, typename... Args>
struct dummyType : public libv::Trackable {
	R memberFunction(Args...) { }
	using libv::Trackable::connectionCount;
	static R staticFunction(Args...) { }
};

// -------------------------------------------------------------------------------------------------

template <template <typename...> class F, typename Source, typename Target>
void test_type_permutator_2() {
	F<Source, Source>()();
	F<Source, Target>()();
	F<Target, Target>()();
	F<Target, Source>()();
}
