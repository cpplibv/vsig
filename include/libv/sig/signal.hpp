// File: Signal.hpp, Created on 2014. május 14. 20:05, Author: Vader

#pragma once

#define LIBV_SIGNAL_VERSION 2016030100

// TODO P1: Use templated containers
// TODO P1: Use ordered small vector for Signal-Slot-Trackable
// TODO P1: Rework Signal-Slot multithreading with shared_reqursive_mutex
// 			http://en.cppreference.com/w/cpp/thread/shared_mutex
//			Or with lock free way, or both...
// TODO P1: Rework connection and trackable objects
// TODO P1: RType not 100% the return type of the fire! Use accumulator's chose
//			This is a very important note.
// TODO P2: Branch signals into two: sync and async signals, this will ease my pain about them...
// TODO P2: Forbid && move / review std::forward
// TODO P2: Change from trackable to shared or weak ptr
// 			signal target variable ami egy shared_ptr és a signal kap róla
// 			egy weak_ptr-t így amikor külden rá infot csak kidobja magából, ígykönnyen
// 			lehet "lokális változót" asnyc módosítani
// TODO P2: Add "Signal Hub" approach where signals are light weight and a main hub stores
//			everything and member signal "markers" (or what should i call them...) are just an
//			accessors to the hub wrapping the necessary minimal information.
//			SignalHub is a trackable
//			Both method should be kept
// TODO P3: Remove output overload based on SignalType and change to concept on operator() and Trackable
// TODO P3: Decide the fate of input functions. Could be implemented with CRTP...
// IDEA: ...and this CRTP could be used for disabling default functions... or not.
// TODO P3: Remove reference, remove constness from results type?
// TODO P3: In Signal Hub-Proxy a connection can be represented with only 1 ptr where the last last
//			bit of the address can represent the direction!
// TODO P3: Give static assert for "typos" like: Signal<int, AccumulatorSum<int>, SingleThread>
// TODO P3: Add precise assertions for correct module number: 0-1 acc, 0-1 thread, 0-1 callsyntax
// TODO P5: Implement position independent call syntax
// TODO P5: Fully generalize modules (including remove macros?, or improving them)

// TODO P5: AdaptivSignal [in/out]put (same, generic lambda...)
// TODO P5: RoutingSignal [set/get]Condition(SignalRouter)
// TODO P5: PrioritySignal - Modified capacitiv where the storage is a priority que
// 			May consider a "predicate" function for generating priority
// TODO P5: UniqueSignal - Modified capacitiv where the storage is unique
// 			May consider a "compare" function for determining uniqueness
//			May consider merging and collapsing awaiting events buy a new one
//			This would be through some template extensibility...
// TODO P5: MoveSignal - allows && move as arg, but only has one output
// TODO P5: ConditionalSignal - Forward the call only if the predicate function allows it
// TODO P5: HistorySignal - Stores and forward calls but also distribute them to late subscribers.
//			Also a template size_t N for limit max call memory number
// TODO P5: TransformSignal - Manipulating the arguments flowing through it using a
// 			manipulator function. Similar to std::transform.
// TODO P5: AsnycSignal - Put the fire method and the arguments into a worker
// 			thread que to call this signal in async mode. (template executor?)
// 			Note: the main task of this signal is to use the executor for callbacks
// TODO P5: SnycSignal - Put the fire method and the arguments into a worker
// 			thread que to call this signal in sync mode. (template executor?)
// 			Note: the main task of this signal is to use the executor for callbacks

// TODO P5: Another raw idea for light weight signals: SignalPtr which is only a pointer to the real
//			signal. It could also store some additional information such as FilterSignalPtr could
//			store the filter information that will be applied to the output. (Component::onClick
//			could be a FilterSignalPtr that points to the UI signal and stores the boundaries)
//			Or would a Signal Tunnel be a better name?

// TODO PD: Documentation of Signal-Slot. The boost version may be relevant
// 			http://www.boost.org/doc/libs/1_59_0/doc/html/signals2/reference.html
// TODO PD: Turotial of Signal-Slot. The boost version may be relevant
// 			http://www.boost.org/doc/libs/1_59_0/doc/html/signals2/tutorial.html

// IDEA: Make a connection object to trackables and move thread policy into it (?)
// FACT: Signal ctor needs to be able to handle an adaptor?
// FACT: It is possible to do adaptiv slots! it is very possible generic lambda?
// NAME: Hub: SignalHub, SignalHost
// NAME: Proxy: SignalProxy, SignalMarker

#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <vector>

#include "accumulator.hpp"
#include "trackable.hpp"
#include "thread_policy.hpp"
#include "type_traits.hpp"

namespace libv {

// === SignalBase ==================================================================================

template <typename Accumulator, typename ThreadPolicy, typename RType, typename... Args>
class SignalBaseImpl : public TrackableBase {
public:
	using signal_tag = void;
protected:
	ThreadPolicy threadPolicy;
	mutable std::recursive_mutex mutex;
	std::multiset<TrackableBase*> inputs;
	std::multimap<TrackableBase*, std::function<RType(Args...) >> outputs;

protected:
	virtual void connect(TrackableBase* ptr, bool reflect = true) override {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		if (reflect)
			ptr->connect(this, false);
	}
	virtual void disconnect(TrackableBase* ptr, bool reflect = true) override {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		outputs.erase(ptr);
		inputs.erase(ptr);

		if (reflect && ptr)
			ptr->disconnect(this, false);
	}
	// ---------------------------------------------------------------------------------------------
public:
	SignalBaseImpl() = default;
	SignalBaseImpl(const SignalBaseImpl<Accumulator, ThreadPolicy, RType, Args...>& other) = delete;

	// ---------------------------------------------------------------------------------------------
protected:
	template <typename R2 = RType, typename = enable_if_t<std::is_void<R2>>>
	RType fireImpl(Args... args) {
		for (auto& output : outputs) {
			output.second(std::forward<Args>(args)...);
		}
	}
	template <typename R2 = RType, typename = disable_if_t<std::is_void<R2>>>
	RType fireImpl(Args... args, int /*ignored*/ = 0) {
		auto accumulator = Accumulator();
		for (auto& output : outputs) {
			if (!accumulator.add(output.second(std::forward<Args>(args)...)))
				return accumulator.result();
		}
		return accumulator.result();
	}

	// ---------------------------------------------------------------------------------------------
public:
	void clearInput() {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		while (!inputs.empty())
			disconnect(*inputs.begin());
	}
	void clearOutput() {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		while (!outputs.empty())
			disconnect(outputs.begin()->first);
	}
	inline size_t inputSize() const {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		return inputs.size();
	}
	inline size_t outputSize() const {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		return outputs.size();
	}

	// ---------------------------------------------------------------------------------------------
//	template<typename SignalType, typename = enable_if_t<is_signal<SignalType>>>
//	inline void input(SignalType& sig) {
//		sig.output(this);
//	}
//	template<typename SignalType, typename = enable_if_t<is_signal<SignalType>>>
//	inline void input(SignalType* const sig) {
//		sig->output(this);
//	}

	// ---------------------------------------------------------------------------------------------
	inline void output(const std::function<RType(Args...)>& func) {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		outputs.emplace(nullptr, func);
	}
	template<typename Object, typename Derivered>
	inline void output(RType(Object::*func)(Args...), Derivered& obj) {
		output(func, &obj);
	}
	template<typename Object, typename Derivered>
	void output(RType(Object::*func)(Args...), Derivered* obj) {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		static_assert(std::is_base_of<TrackableBase, Object>::value,
				"Object type has to be Derived from TrackableBase "
				"(You may want to consider inheriting from libv::Trackable).");
		outputs.emplace(obj, [obj, func](Args... args) {
			(obj->*func)(std::forward<Args>(args)...);
		});
		static_cast<TrackableBase*> (obj)->connect(this, true);

		//TODO P4: enforce Deriver base of Object
	}
	template<typename SignalType, typename = enable_if_t<is_signal<SignalType>>>
	inline void output(SignalType& slot) {
		this->output(&slot);
	}
	template<typename SignalType, typename = enable_if_t<is_signal<SignalType>>>
	inline void output(SignalType* const slot) {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		this->output(&SignalType::fire, slot);
		slot->inputs.emplace(this);
	}

	// ---------------------------------------------------------------------------------------------
	virtual ~SignalBaseImpl() {
		clearInput();
		clearOutput();
	}
};

// === Signal ======================================================================================

template <typename Accumulator, typename ThreadPolicy, typename RType, typename... Args>
class SignalImpl : public SignalBaseImpl<Accumulator, ThreadPolicy, RType, Args...> {
	using SelfType = SignalImpl<Accumulator, ThreadPolicy, RType, Args...>;
public:
	SignalImpl() = default;
	SignalImpl(const SelfType& other) = delete;
public:
	inline RType fire(Args... args) {
		std::lock_guard<std::recursive_mutex> thread_guard(this->mutex);
		return this->fireImpl(std::forward<Args>(args)...);
	}
	inline RType operator()(Args... args) {
		std::lock_guard<std::recursive_mutex> thread_guard(this->mutex);
		return this->fireImpl(std::forward<Args>(args)...);
	}
};

// === SwitchSignal ================================================================================

template <typename Accumulator, typename ThreadPolicy, typename RType, typename... Args>
struct SwitchSignalImpl : public SignalBaseImpl<Accumulator, ThreadPolicy, RType, Args...> {
	bool enabled = true;
public:
	inline void enable() {
		std::lock_guard<std::recursive_mutex> thread_guard(this->mutex);
		enabled = true;
	}
	inline void disable() {
		std::lock_guard<std::recursive_mutex> thread_guard(this->mutex);
		enabled = false;
	}
	inline RType fire(Args... args) {
		std::lock_guard<std::recursive_mutex> thread_guard(this->mutex);
		if (enabled)
			return this->fireImpl(std::forward<Args>(args)...);
		return RType();
	}
};

// === CapacitivSignal =============================================================================

template <typename Accumulator, typename ThreadPolicy, typename RType, typename... Args>
class CapacitivSignalImpl : public SignalBaseImpl<Accumulator, ThreadPolicy, void, Args...> {
private:
	std::vector<std::tuple<typename std::remove_reference<Args>::type...>> argQue;
private:
	template<std::size_t... Is>
	inline void flushHelper(std::index_sequence<Is...>) {
		for (auto& item : argQue) {
			this->fireImpl(std::get<Is>(item)...);
		}
		argQue.clear();
	}
public:
	inline void fire(Args... args) {
		std::lock_guard<std::recursive_mutex> thread_guard(this->mutex);
		argQue.emplace_back(args...);
	}
	inline void flush() {
		std::lock_guard<std::recursive_mutex> thread_guard(this->mutex);
		flushHelper(std::index_sequence_for < Args...>{});
	}
};

// === HistorySignal ===============================================================================

template <typename Accumulator, typename ThreadPolicy, typename RType, typename... Args>
class HistorySignalImpl : public SignalBaseImpl<Accumulator, ThreadPolicy, void, Args...> {
private:
	std::vector<std::tuple<typename std::remove_reference<Args>::type...>> history;
private:
	template<typename F, std::size_t... Is>
	inline void flushHelper(F&& func, std::index_sequence<Is...>) {
		for (auto& item : history) {
			func(std::get<Is>(item)...);
		}
	}
public:
	// TODO P5: History signal output auto-flush
	//	template<typename... Args>
	//	void output(Args... args) {
	//		flushHelper(std::index_sequence_for<Args...>{});
	//	}
	inline size_t historySize() const {
		std::lock_guard<std::recursive_mutex> thread_guard(this->mutex);
		return history.size();
	}
	inline void fire(Args... args) {
		std::lock_guard<std::recursive_mutex> thread_guard(this->mutex);
		history.emplace_back(args...);
		this->fireImpl(args...);
	}
	inline void clearHistory() {
		std::lock_guard<std::recursive_mutex> thread_guard(this->mutex);
		history.clear();
	}
};

// === Aliases =====================================================================================

template<template <typename...> class, typename...>
struct SignalAliasHelper;

template<template <typename...> class Signal,
		typename R, typename... Args,
		typename... DefaultModuls,
		typename... Moduls>
struct SignalAliasHelper<
		Signal, R(Args...),
		deafult<DefaultModuls...>,
		Moduls...> {
	using type = Signal<
			select_accumulator_or<select_accumulator<DefaultModuls...>, Moduls...>,
			select_thread_policy_or<select_thread_policy<DefaultModuls...>, Moduls...>,
			R, Args...>;
};

template<template <typename...> class Signal, typename... Types>
using SignalAliasHelper_t = typename SignalAliasHelper<Signal, Types...>::type;

template<typename SignalBase>
struct SignalAliasHelperUsingCtor : SignalBase {
	using SignalBase::SignalBase;
};

template<template <typename...> class Signal, typename... Types>
using SignalAliasHelperUsingCtor_t =
		SignalAliasHelperUsingCtor<SignalAliasHelper_t<Signal, Types...>>;

// Signal ------------------------------------------------------------------------------------------

template<typename... Args>
struct Signal : Signal<void(Args...)> {
};

template<typename R, typename... Args, typename... Moduls>
struct Signal<R(Args...), Moduls...> :
	SignalAliasHelperUsingCtor_t<
			SignalImpl, R(Args...),
			deafult<AccumulatorSum<R>, SingleThread>,
			Moduls...> {
};

// CapacitivSignal ---------------------------------------------------------------------------------

template<typename... Args>
struct CapacitivSignal : CapacitivSignal<void(Args...)> {
};

template<typename R, typename... Args, typename... Moduls>
struct CapacitivSignal<R(Args...), Moduls...> :
	SignalAliasHelperUsingCtor_t<
			CapacitivSignalImpl, R(Args...),
			deafult<AccumulatorSum<R>, SingleThread>,
			Moduls...> {
	// TODO P4: R should always be void. Assert it.
};

// SwitchSignal ------------------------------------------------------------------------------------

template<typename... Args>
struct SwitchSignal : SwitchSignal<void(Args...)> {
};

template<typename R, typename... Args, typename... Moduls>
struct SwitchSignal<R(Args...), Moduls...> :
	SignalAliasHelperUsingCtor_t<
			SwitchSignalImpl, R(Args...),
			deafult<AccumulatorSum<R>, SingleThread>,
			Moduls...> {
};

// HistorySignal -----------------------------------------------------------------------------------

template<typename... Args>
struct HistorySignal : HistorySignal<void(Args...)> {
};

template<typename R, typename... Args, typename... Moduls>
struct HistorySignal<R(Args...), Moduls...> :
	SignalAliasHelperUsingCtor_t<
			HistorySignalImpl, R(Args...),
			deafult<AccumulatorSum<R>, SingleThread>,
			Moduls...> {
	// TODO P4: R should always be void. Assert it.
};

// -------------------------------------------------------------------------------------------------

} //namespace libv