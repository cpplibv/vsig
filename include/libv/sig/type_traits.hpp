// File:   type_traits.hpp Author: Vader Created on March 13, 2016, 9:19 PM

#pragma once

#include <type_traits>

#include "tag.hpp"

namespace libv {

// core --------------------------------------------------------------------------------------------

template <typename...>
struct voider { //CWG 1558
	using type = void;
};

template <typename... Args>
using void_t = typename voider<Args...>::type;

template <typename... T>
struct always_true : std::true_type {
};

template <typename... T>
struct always_false : std::false_type {
};

// std shortcuts -----------------------------------------------------------------------------------

template <typename T>
using enable_if_t = typename std::enable_if<T::value>::type;

template <typename T>
using disable_if_t = typename std::enable_if<!T::value>::type;

template <typename T>
using remove_reference_t = typename std::remove_reference<T>::type;

template <typename T>
using is_void_t = typename std::is_void<T>::type;

// list --------------------------------------------------------------------------------------------

template <typename... Types>
struct list {
};

// list_concat

template <typename APack, typename BPack>
struct list_concat;

template <typename... AList, typename... BList>
struct list_concat<list<AList...>, list<BList...>> {
	using type = list<AList..., BList...>;
};

template <typename A, typename B>
using list_concat_t = typename list_concat<A, B>::type;

// list_append

template <typename Element, typename... List>
struct list_append;

template <typename Element, typename... List>
struct list_append<Element, list<List...>> {
	using type = list<Element, List...>;
};

template <typename Element, typename... List>
struct list_append<list<List...>, Element> {
	using type = list<List..., Element>;
};

template <typename A, typename B>
using list_append_t = typename list_append<A, B>::type;

// list_size

template <typename List>
struct list_size;

template <typename... List>
struct list_size<list<List...>> {
	static constexpr size_t value = sizeof...(List);
};

template <typename List>
constexpr size_t list_size_v = list_size<List>::value;

// list_search

template <typename Element, typename List>
struct list_search;

template <typename Element>
struct list_search<Element, list<>> : std::false_type {
};

template <typename Element, typename... List>
struct list_search<Element, list<Element, List...>> : std::true_type {
};

template <typename Element, typename Head, typename... List>
struct list_search<Element, list<Head, List...>> : list_search<Element, list<List...>> {
};

// to list_to_signature ----------------------------------------------------------------------------

template <typename List>
struct list_to_signature;

template <typename RType, typename... Args>
struct list_to_signature<list<RType, Args...>> {
	using type = RType(Args...);
};

// map ---------------------------------------------------------------------------------------------

template <typename... T>
struct bag;



// find_first --------------------------------------------------------------------------------------

template <template <typename...> class, typename...>
struct find_first;

template <template <typename...> class Predicate>
struct find_first<Predicate> {
	using type = void;
	using found = std::false_type;
};

template <template <typename...> class Predicate, typename Head, typename... Tail>
struct find_first<Predicate, Head, Tail...> {
	using type = typename
			std::conditional<
				Predicate<Head>::value,
				Head,
				typename find_first<Predicate, Tail...>::type
			>::type;
	using found = typename
			std::conditional<
				Predicate<Head>::value,
				std::true_type,
				typename find_first<Predicate, Tail...>::found
			>::type;
};

// module ------------------------------------------------------------------------------------------

template <typename Module> struct get_module_tag;
template <template <typename...> class Module, typename Tag>
struct get_module_tag<Module<Tag>> {
	using type = Tag;
};

template <typename Module, typename Tag> struct is_t1_same : ::std::false_type {};
template <template <typename...> class Module, typename ModuleTag, typename Tag> struct is_t1_same<Module<ModuleTag>, Tag> :
	std::is_same<ModuleTag, Tag>{ };

template <typename T, typename = void> struct is_module : std::false_type {};
template <typename T> struct is_module<T, void_t<typename T::module>> : std::true_type {};

template <typename T, typename Tag, typename = void> struct is_module_tag : std::false_type {};
template <typename T, typename Tag> struct is_module_tag<T, Tag, void_t<typename T::module>> :
	is_t1_same<typename T::module, Tag>{ };

template <typename Tag>
struct is_module_bound_tag {
	template <typename T>
	using type = is_module_tag<T, Tag>;
};

template <typename Tag, typename... Modules>
using select = typename ::libv::find_first<is_module_bound_tag<Tag>::template type, Modules...>::type;

// is call signature -------------------------------------------------------------------------------

template <typename T>
struct is_call_signature : std::false_type {
};

template <typename RType, typename... Args>
struct is_call_signature<RType(Args...)> : std::true_type {
};

// count call signature ---------------------------------------------------------------------------

template <size_t N = 0, typename... Args>
struct count_call_signature_helper;

template <size_t N>
struct count_call_signature_helper<N> {
	static constexpr size_t value = N;
};

template <size_t N, typename Head, typename... Tail>
struct count_call_signature_helper<N, Head, Tail...> : count_call_signature_helper<N, Tail...> {
};

template <size_t N, typename RType, typename... Args, typename... Tail>
struct count_call_signature_helper<N, RType(Args...), Tail...> : count_call_signature_helper<N + 1, Tail...> {
};

template <typename... Args>
struct count_call_signature : count_call_signature_helper<0, Args...> {
};

// select call signature ---------------------------------------------------------------------------

template <typename...>
struct select_call_signature;

template <typename RType, typename... Args, typename... Tail>
struct select_call_signature<RType(Args...), Tail...> {
	using type = RType(Args...);
};

template <typename Head, typename... Tail>
struct select_call_signature<Head, Tail...> {
	using type = typename select_call_signature<Tail...>::type;
};

// search call signature ---------------------------------------------------------------------------

template <typename...>
struct search_call_signature;

template <>
struct search_call_signature<> : std::false_type {};

template <typename RType, typename... Args, typename... Tail>
struct search_call_signature<RType(Args...), Tail...> : std::true_type {};

template <typename Head, typename... Tail>
struct search_call_signature<Head, Tail...> : search_call_signature<Tail...> {};

// is_signal ---------------------------------------------------------------------------------------

template <typename SignalType>
using is_signal = is_module_tag<SignalType, tag::signal>;

// -------------------------------------------------------------------------------------------------

} //namespace libv
