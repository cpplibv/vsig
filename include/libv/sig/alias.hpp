// File:   meta.hpp Author: Vader Created on 2016. július 11., 10:58

#pragma once

#include "type_traits.hpp"

namespace libv {

// -------------------------------------------------------------------------------------------------

template <template <typename...> class, typename Signature>
struct module_filter;

// -------------------------------------------------------------------------------------------------

template <template <typename...> class Signal, typename ModulesList, typename = void>
struct apply_module_pack {
	static_assert(always_false<ModulesList>::value, ""
			"\n\tFailed to instantiate signal."
			"\n\tInvalid combination of signal parameters / modules."
			"\n\tCheck template parameters of the current apply_module_pack type:"
			"\n\t\tSignal - Signal type which will get your modules after filter"
			"\n\t\tlist<Modules...> - The list of the template parameters that will be used"
			"\n\t\tvoid - <<implementation detail>>"
			"\n\tNote - The parameter list is already filtered and processed"
			"\n\tNote - Only the first module per type will be applied"
			"\n\tNote - Default values applied if there was not provided one");
};

template <template <typename...> class Signal, typename... Modules>
struct apply_module_pack<Signal, list<Modules...>, void_t<Signal<Modules...>>> {
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
	using type = list_concat_t<
			typename get_module_filter_default_modules<MF>::type,
			typename summarieze_module_filter_default_modules<typename MF::base_type>::type
		>;
};

template <typename MF, typename = void>
struct summarieze_module_filter_parameter_tags {
	using type = typename MF::parameter_tags;
};

template <typename MF>
struct summarieze_module_filter_parameter_tags<MF, void_t<typename MF::base_type>> {
	using type = list_concat_t<
			typename get_module_filter_parameter_tags<MF>::type,
			typename summarieze_module_filter_parameter_tags<typename MF::base_type>::type
		>;
};

// -------------------------------------------------------------------------------------------------

template <typename Tags, typename Modules>
struct foreach_select;

template <typename... Modules>
struct foreach_select<list<>, list<Modules...>>
{
	using type = list<>;
};

template <typename TagsHead, typename... TagsTail, typename... Modules>
struct foreach_select<list<TagsHead, TagsTail...>, list<Modules...>>
{
	using type = list_append_t <
			select<TagsHead, Modules...>,
			typename foreach_select < list < TagsTail...>, list < Modules...>>::type
		>;
};

// -------------------------------------------------------------------------------------------------

template <template <typename...> class Signal, typename... Args>
struct process_module_filter;

template <template <typename...> class Signal, typename RType, typename... Args, typename... Modules>
struct process_module_filter<Signal, RType(Args...), list<Modules...>>
{
	using raw_filter = module_filter<Signal, RType(Args...)>;

	using summariezed_defaults = typename summarieze_module_filter_default_modules<raw_filter>::type;
	using modules_with_defaults = list_concat_t<list<Modules...>, summariezed_defaults>;
	using summariezed_tags = typename summarieze_module_filter_parameter_tags<raw_filter>::type;
	using modules_to_be_applied = typename foreach_select<summariezed_tags, modules_with_defaults>::type;

	using type = list_append_t < RType(Args...), modules_to_be_applied>;
};

// -------------------------------------------------------------------------------------------------

template <typename... Args>
struct split_modules;

template <>
struct split_modules<> {
	using arguments = list<>;
	using modules = list<>;
};

template <typename Head, typename... Args>
struct split_modules<Head, Args...> {
	using arguments = typename std::conditional<
			!is_module<Head>::value && !is_call_signature<Head>::value,
			list_append_t<Head, typename split_modules<Args...>::arguments>,
			typename split_modules<Args...>::arguments
			>::type;
	using modules = typename std::conditional<
			is_module<Head>::value && !is_call_signature<Head>::value,
			list_append_t<Head, typename split_modules<Args...>::modules>,
			typename split_modules<Args...>::modules
			>::type;
};

// -------------------------------------------------------------------------------------------------

template <typename List, typename Acc>
struct check_module_count_helper;

template <typename AccList>
struct check_module_count_helper<list<>, AccList> {
	static constexpr bool value = true;
};

template <typename Head, typename... Tail, typename AccList>
struct check_module_count_helper<list<Head, Tail...>, AccList> {
	static constexpr bool value = std::conditional<
			list_search<typename get_module_tag<typename Head::module>::type, AccList>::value,
			std::false_type,
			check_module_count_helper<list<Tail...>,
			list_append_t<typename get_module_tag<typename Head::module>::type, AccList>>
			>::type::value;
};

template <typename List>
struct check_module_count {
	static constexpr bool value = check_module_count_helper<List, list<>>::value;
};

// -------------------------------------------------------------------------------------------------

template <template <typename...> class Signal, typename... Args>
struct signal_arg_parse {
	using signature = typename std::conditional<
			search_call_signature<Args...>::value,
			typename select_call_signature<Args..., void(void) >::type,
			typename list_to_signature<
			list_append_t<void, typename split_modules<Args...>::arguments>
			>::type
			>::type;
	using modules = typename split_modules<Args...>::modules;

	static_assert(
			!(search_call_signature<Args...>::value &&
			(list_size_v<typename split_modules<Args...>::arguments> > 0)),
			"\n\tInvalid combination of signal parameters / modules:"
			"\n\t\tRedundant function argument parameter as,"
			"\n\t\tboth a function call signature and at least one non module parameter present."
			"\n\tTip - If you want to add new custom modules make sure you added the module meta tag."
			);
	static_assert(
			!(count_call_signature<Args...>::value > 1),
			"\n\tInvalid combination of signal parameters / modules:"
			"\n\t\tMultiple call signature. Only ONE or ZERO call signature can be present."
			);
	static_assert(
			(check_module_count<modules>::value),
			"\n\tInvalid combination of signal parameters / modules:"
			"\n\t\tMultiple parameters / modules with the same meta tag (same type)."
			);
};

template <template <typename...> class Signal, typename... RawArgs>
struct signal_alias {
	using parsed_signature = typename signal_arg_parse<Signal, RawArgs...>::signature;
	using parsed_modules = typename signal_arg_parse<Signal, RawArgs...>::modules;

	using modules = typename process_module_filter<Signal, parsed_signature, parsed_modules>::type;
	using type = typename apply_module_pack<Signal, modules>::type;
};

// -------------------------------------------------------------------------------------------------

template <template <typename...> class Signal, typename... RawArgs>
using signal_alias_t = typename signal_alias<Signal, RawArgs...>::type;

} //namespace libv