// File: Signal.hpp, Created on 2014. május 14. 20:05, Author: Vader

#pragma once

#define LIBV_SIGNAL_VERSION 2016041100

// TODO P1: Use templated containers
// TODO P1: Use ordered small vector for Signal-Slot-Trackable
// TODO P1: Rework Signal-Slot multithreading with shared_reqursive_mutex
// 			http://en.cppreference.com/w/cpp/thread/shared_mutex
//			Or with lock free way, or both...
// TODO P1: Rework connection and trackable objects
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
// TODO P3: Improve module filter to warn about:
//				- unused parameters (only 0..1 occurrence per tag or call syntax are allowed)
//				- mixed syntax Signal<int, AccumulatorSum<int>, SingleThread>
// TODO P3: Improve module filter to handle only modules without call syntax as modules
//			So change decider condition from has call syntax to all module
// TODO P4: Current accumulator rename to "combined result"? and add a bigger encapsulation
//			which can provide (through inheritance?) runtime signal non-fire-local store and logic
// TODO P5: HistorySignal:
//				- Dynamic history size
//				- Circular buffer
//				- Output auto-flush thread safety
// TODO P5: Thread safety of modules: exposed function due to inheritance are the danger
//			But, do i really considering cross module operations for solving this?

// TODO P4: ConditionalSignal - Forward the call only if the predicate function allows it
// TODO P4: RoutingSignal [set/get]Condition(SignalRouter)

// TODO P5: AdaptivSignal [in/out]put (same, generic lambda...)
// TODO P5: PrioritySignal - Modified capacitive where the storage is a priority que
// 			May consider a "predicate" function for generating priority
// TODO P5: UniqueSignal - Modified capacitive where the storage is unique
// 			May consider a "compare" function for determining uniqueness
//			May consider merging and collapsing awaiting events buy a new one
//			This would be through some template extensibility...
// TODO P5: MoveSignal - allows && move as arg, but only has one output
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

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <vector>

#include "module.hpp"
#include "tag.hpp"
#include "thread_policy.hpp"
#include "trackable.hpp"
#include "type_traits.hpp"

namespace libv {

// =================================================================================================

template <template <typename...> class, typename Signature>
struct module_filter;

// -------------------------------------------------------------------------------------------------

template <template <typename...> class Signal, typename ModulesList, typename = void>
struct apply_modul_pack {
	static_assert(always_false<ModulesList>::value, ""
			"\n\tInvalid combination of signal parameters / modules."
			"\n\tCheck template parameters of the current apply_modul_pack type:"
			"\n\t\tSignal - Signal type which will get your modules after filter"
			"\n\t\tlist<Modules...> - The list of the template parameters that will be used"
			"\n\t\tvoid - <<implementation detail>>"
			"\n\tNote - The parameter list is already filtered and processed"
			"\n\tNote - Only the first module per type will be applied"
			"\n\tNote - Default values applied if there was not provided one");
};

template <template <typename...> class Signal, typename... Modules>
struct apply_modul_pack<Signal, list<Modules...>, void_t<Signal<Modules...>>> {
	using type = Signal<Modules...>;
};

// -------------------------------------------------------------------------------------------------

template <typename MF, typename = void>
struct get_module_filter_default_modules {
	using type = list<>;
};

template <typename MF>
struct get_module_filter_default_modules<MF, void_t<typename MF::default_modules>> {
	using type = typename MF::default_modules;
};

template <typename MF, typename = void>
struct get_module_filter_parameter_tags {
	using type = list<>;
};

template <typename MF>
struct get_module_filter_parameter_tags<MF, void_t<typename MF::parameter_tags>> {
	using type = typename MF::parameter_tags;
};

// -------------------------------------------------------------------------------------------------

template <typename MF, typename = void>
struct summarieze_module_filter_default_modules {
	using type = typename MF::default_modules;
};

template <typename MF>
struct summarieze_module_filter_default_modules<MF, void_t<typename MF::base_type>> {
	using type = typename concat_list<
				typename get_module_filter_default_modules<MF>::type,
				typename summarieze_module_filter_default_modules<typename MF::base_type>::type
			>::type;
};

template <typename MF, typename = void>
struct summarieze_module_filter_parameter_tags {
	using type = typename MF::parameter_tags;
};

template <typename MF>
struct summarieze_module_filter_parameter_tags<MF, void_t<typename MF::base_type>> {
	using type = typename concat_list<
				typename get_module_filter_parameter_tags<MF>::type,
				typename summarieze_module_filter_parameter_tags<typename MF::base_type>::type
			>::type;
};

// -------------------------------------------------------------------------------------------------

template <typename Tags, typename Modules>
struct foreach_select;

template <typename... Modules>
struct foreach_select<list<>, list<Modules...>> {
	using type = list<>;
};

template <typename TagsHead, typename... TagsTail, typename... Modules>
struct foreach_select<list<TagsHead, TagsTail...>, list<Modules...>> {
	using type = typename append<
				select<TagsHead, Modules...>,
				typename foreach_select<list<TagsTail...>, list<Modules...>>::type
			>::type;
};

// -------------------------------------------------------------------------------------------------

template <template <typename...> class Signal, typename... Args>
struct process_module_filter;

template <template <typename...> class Signal, typename RType, typename... Args, typename... Modules>
struct process_module_filter<Signal, RType(Args...), list<Modules...>> {
	using raw_filter = module_filter<Signal, RType(Args...)>;

	using summariezed_defaults = typename summarieze_module_filter_default_modules<raw_filter>::type;
	using modules_with_defaults = typename concat_list<list<Modules...>, summariezed_defaults>::type;
	using summariezed_tags = typename summarieze_module_filter_parameter_tags<raw_filter>::type;
	using modules_to_be_applied = typename foreach_select<summariezed_tags, modules_with_defaults>::type;

	using type = typename append<RType(Args...), modules_to_be_applied>::type;
};

// -------------------------------------------------------------------------------------------------

template <template <typename...> class Signal, typename... Modules>
struct signal_with_filtered_moduls {
	using signature = typename std::conditional<
				search_call_signature<Modules...>::value,
				typename select_call_signature<Modules..., void(void)>::type,
				void(Modules...)
			>::type;
	using moduls = typename std::conditional<
				search_call_signature<Modules...>::value,
				list<Modules...>,
				list<>
			>::type;
	using final_modul_list = typename process_module_filter<Signal, signature, moduls>::type;
	using type = typename apply_modul_pack<Signal, final_modul_list>::type;
};

// -------------------------------------------------------------------------------------------------

template <template <typename...> class Signal, typename... Modules>
using signal_alias = typename signal_with_filtered_moduls<Signal, Modules...>::type;

// -------------------------------------------------------------------------------------------------

//
//
// =================================================================================================
//     Signal Implementations
// =================================================================================================
//
//

// === SignalBase ==================================================================================

template <typename...>
class SignalBaseImpl;

template <typename R, typename... Args>
struct module_filter<SignalBaseImpl, R(Args...)> {
	using default_modules = list<
			AccumulatorSum<R>,
			SingleThread>;
	using parameter_tags = list<
			tag::accumulator,
			tag::thread_policy>;
};

// -------------------------------------------------------------------------------------------------

template <typename...>
class SignalBaseImpl;

template <typename RType, typename... Args, typename Accumulator, typename ThreadPolicy>
class SignalBaseImpl<RType(Args...), Accumulator, ThreadPolicy> : public TrackableBase {
public:
	using this_type = SignalBaseImpl<RType(Args...), Accumulator, ThreadPolicy>;

	using accumulator = accumulator_traits<Accumulator>;
	using thread_policy = ThreadPolicy;

	using result_type = typename accumulator::result_type;

protected:
	thread_policy threadPolicy;
	mutable std::recursive_mutex mutex;
	std::multiset<TrackableBase*> inputs;
	std::multimap<TrackableBase*, std::function<RType(Args...) >> outputs;

protected:
	virtual void connect(TrackableBase* ptr, bool reflect = true) override {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);

		if (reflect) {
			inputs.emplace(ptr);
			ptr->connect(this, false);
		}
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
	SignalBaseImpl(const this_type& other) = delete;

	// ---------------------------------------------------------------------------------------------
protected:
	result_type fireImpl(Args... args) {
		auto acc = accumulator::create();
		for (auto& output : outputs) {
			if (accumulator::add(acc, output.second, std::forward<Args>(args)...))
				return accumulator::result(acc);
		}
		return accumulator::result(acc);
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
//	template <typename SignalType, typename = enable_if_t<is_signal<SignalType>>>
//	inline void input(SignalType& sig) {
//		sig.output(this);
//	}
//	template <typename SignalType, typename = enable_if_t<is_signal<SignalType>>>
//	inline void input(SignalType* const sig) {
//		sig->output(this);
//	}

	// ---------------------------------------------------------------------------------------------
	template <typename Func>
	inline void output(Func&& func) {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		outputs.emplace(nullptr, std::forward<Func>(func));
		//TODO P4: static assert for callable RType(Args...)
		//TODO P4: handle if F is derived from Trackable
		//TODO P4: sfiane for std function?, ehh... this overload will need sfiane if i collapse
		//		output signal into this...
	}
	template <typename Derivered, typename Object = Derivered>
	inline void output(Derivered& obj, RType(Object::*func)(Args...) = &Object::operator()) {
		output(&obj, func);
	}
	template <typename Derivered, typename Object = Derivered>
	inline void output(Derivered* obj, RType(Object::*func)(Args...) = &Object::operator()) {
		std::lock_guard<std::recursive_mutex> thread_guard(mutex);
		static_assert(std::is_base_of<TrackableBase, Object>::value,
				"Object type has to be Derived from TrackableBase "
				"(You may want to consider inheriting from libv::Trackable).");
		static_assert(std::is_base_of<Object, Derivered>::value,
				"Member function has to be the derived member's function as Object");
		//TODO P4: enforce Deriver base of Object
		//		for this start with some unit tests: base/base derived/base base/derived derived/derived

		outputs.emplace(obj, [obj, func](Args... args) {
			(obj->*func)(std::forward<Args>(args)...);
		});
		static_cast<TrackableBase*> (obj)->connect(this, true);
	}

	// ---------------------------------------------------------------------------------------------
	virtual ~SignalBaseImpl() {
		clearInput();
		clearOutput();
	}
};

// === Signal ======================================================================================

template <typename...>
class SignalImpl;

// -------------------------------------------------------------------------------------------------

template <typename R, typename... Args>
struct module_filter<SignalImpl, R(Args...)> {
	using base_type = module_filter<SignalBaseImpl, R(Args...)>;
};

// -------------------------------------------------------------------------------------------------

template <typename RType, typename... Args, typename... Modules>
class SignalImpl<RType(Args...), Modules...>
		: public SignalBaseImpl<RType(Args...), Modules...> {
public:
	using base_type = SignalBaseImpl<RType(Args...), Modules...>;
	using this_type = SignalImpl<RType(Args...), Modules...>;

public:
	SignalImpl() = default;
	SignalImpl(const this_type& other) = delete;
public:
	inline typename base_type::result_type fire(Args... args) {
		std::lock_guard<std::recursive_mutex> thread_guard(this->mutex);
		return this->fireImpl(std::forward<Args>(args)...);
	}
	inline typename base_type::result_type operator()(Args... args) {
		fire(std::forward<Args>(args)...);
	}
};

// === CapacitiveSignal ============================================================================

template <typename...>
class CapacitiveSignalImpl;

// -------------------------------------------------------------------------------------------------

template <typename R, typename... Args>
struct module_filter<CapacitiveSignalImpl, R(Args...)> {
	using base_type = module_filter<SignalBaseImpl, R(Args...)>;
};

// -------------------------------------------------------------------------------------------------

template <typename RType, typename... Args, typename... Modules>
class CapacitiveSignalImpl<RType(Args...), Modules...>
		: public SignalBaseImpl<RType(Args...), Modules...> {
public:
	using base_type = SignalBaseImpl<RType(Args...), Modules...>;
	using this_type = CapacitiveSignalImpl<RType(Args...), Modules...>;
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
	inline void operator()(Args... args) {
		fire(std::forward<Args>(args)...);
	}
	inline void flush() {
		std::lock_guard<std::recursive_mutex> thread_guard(this->mutex);
		flushHelper(std::index_sequence_for < Args...>{});
	}
};

// === ConditionalSignal ===========================================================================

template <typename...>
class ConditionalSignalImpl;

// -------------------------------------------------------------------------------------------------

template <typename R, typename... Args>
struct module_filter<ConditionalSignalImpl, R(Args...)> {
	using base_type = module_filter<SignalBaseImpl, R(Args...)>;

	using default_modules = list<ConditionDynamic<Args...>>;
	using parameter_tags = list<tag::condition>;
};

// -------------------------------------------------------------------------------------------------

template <typename RType, typename... Args, typename Condition, typename... Modules>
class ConditionalSignalImpl<RType(Args...), Condition, Modules...>
		: public SignalBaseImpl<RType(Args...), Modules...>, public Condition {
public:
	using base_type = SignalBaseImpl<RType(Args...), Modules...>;
	using this_type = ConditionalSignalImpl<RType(Args...), Condition, Modules...>;
private:
	bool enabled = true;
public:
	inline typename base_type::result_type fire(Args... args) {
		std::lock_guard<std::recursive_mutex> thread_guard(this->mutex);
		if (Condition::check(std::forward<Args>(args)...)) {
			return this->fireImpl(std::forward<Args>(args)...);
		} else {
			auto acc = base_type::accumulator::create();
			return base_type::accumulator::result(acc);
		}
	}
	inline typename base_type::result_type operator()(Args... args) {
		return fire(std::forward<Args>(args)...);
	}
};

// === HistorySignal ===============================================================================

template <typename...>
class HistorySignalImpl;

template <size_t N>
struct history_size {
	using module = tag_type<tag::history_size>;
	static constexpr size_t value = N;
};

// -------------------------------------------------------------------------------------------------

template <typename R, typename... Args>
struct module_filter<HistorySignalImpl, R(Args...)> {
	using base_type = module_filter<SignalBaseImpl, R(Args...)>;

	using default_modules = list<history_size<0>>;
	using parameter_tags = list<tag::history_size>;
};

// -------------------------------------------------------------------------------------------------

template <typename RType, typename... Args, typename HistorySize, typename... Modules>
class HistorySignalImpl<RType(Args...), HistorySize, Modules...>
		: public SignalBaseImpl<RType(Args...), Modules...> {
public:
	using base_type = SignalBaseImpl<RType(Args...), Modules...>;
	using this_type = HistorySignalImpl<RType(Args...), HistorySize, Modules...>;

	using history_size = HistorySize;
	static constexpr size_t historySizeMax = history_size::value;
private:
	std::vector<std::tuple<typename std::remove_reference<Args>::type...>> history;
private:
	template <typename F, std::size_t... Is>
	inline void flushHelper(F&& func, std::index_sequence<Is...>) {
		for (auto& item : history) {
			func(std::get<Is>(item)...);
		}
	}
public:
	template <typename F>
	inline void output(F&& func) {
		flushHelper(func, std::index_sequence_for<Args...>{});
		base_type::output(std::forward<F>(func));
	}
	template <typename Derivered, typename Object = Derivered>
	inline void output(Derivered& obj, RType(Object::*func)(Args...) = &Object::operator()) {
		output(&obj, func);
	}
	template <typename Derivered, typename Object = Derivered>
	inline void output(Derivered* obj, RType(Object::*func)(Args...) = &Object::operator()) {
		flushHelper([=](Args... args){
			(obj->*func)(args...);
		}, std::index_sequence_for<Args...>{});
		base_type::output(obj, func);
	}
	inline size_t historySize() const {
		std::lock_guard<std::recursive_mutex> thread_guard(this->mutex);
		return history.size();
	}
	inline size_t historyMax() const {
		return historySizeMax;
	}
	inline void fire(Args... args) {
		std::lock_guard<std::recursive_mutex> thread_guard(this->mutex);
		history.emplace_back(args...);
		this->fireImpl(args...);
	}
	inline void operator()(Args... args) {
		fire(std::forward<Args>(args)...);
	}
	inline void clearHistory() {
		std::lock_guard<std::recursive_mutex> thread_guard(this->mutex);
		history.clear();
	}
};

// === SwitchSignal ================================================================================

template <typename...>
class SwitchSignalImpl;

// -------------------------------------------------------------------------------------------------

template <typename R, typename... Args>
struct module_filter<SwitchSignalImpl, R(Args...)> {
	using base_type = module_filter<ConditionalSignalImpl, R(Args...)>;

	using default_modules = list<ConditionSwitch>;
};

// -------------------------------------------------------------------------------------------------

template <typename RType, typename... Args, typename... Modules>
class SwitchSignalImpl<RType(Args...), Modules...>
		: public ConditionalSignalImpl<RType(Args...), Modules...> {
};

// === Aliases =====================================================================================

template <typename... Args>
using Signal = signal_alias<SignalImpl, Args...>;

template <typename... Args>
using CapacitiveSignal = signal_alias<CapacitiveSignalImpl, Args...>;

template <typename... Args>
using ConditionalSignal = signal_alias<ConditionalSignalImpl, Args...>;

template <typename... Args>
using HistorySignal = signal_alias<HistorySignalImpl, Args...>;

template <typename... Args>
using SwitchSignal = signal_alias<SwitchSignalImpl, Args...>;

// -------------------------------------------------------------------------------------------------

} //namespace libv