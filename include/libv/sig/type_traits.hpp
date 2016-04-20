// File:   type_traits.hpp Author: Vader Created on March 13, 2016, 9:19 PM

#pragma once

#include <type_traits>

namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename...>
struct voider { //CWG 1558
	using type = void;
};

template <typename... Args>
using void_t = typename voider<Args...>::type;

// -------------------------------------------------------------------------------------------------

template <typename T>
using enable_if_t = typename std::enable_if<T::value>::type;

template <typename T>
using disable_if_t = typename std::enable_if<!T::value>::type;

template <typename T>
using remove_reference_t = typename std::remove_reference<T>::type;

template <typename T>
using is_void_t = typename std::is_void<T>::type;

// -------------------------------------------------------------------------------------------------

template <typename... Types>
struct list {
};

template <typename APack, typename BPack>
struct concat_list;

template <typename... AList, typename... BList>
struct concat_list<list<AList...>, list<BList...>> {
	using type = list < AList..., BList...>;
};

// =================================================================================================

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

// -------------------------------------------------------------------------------------------------

template <typename Module, typename Tag> struct is_T1_same : ::std::false_type {};
template <template <typename...> class Module, typename ModuleTag, typename Tag> struct is_T1_same<Module<ModuleTag>, Tag> :
	std::is_same<ModuleTag, Tag>{ };

template <typename T, typename Tag, typename = void> struct is_module : ::std::false_type {};
template <typename T, typename Tag> struct is_module<T, Tag, ::libv::void_t<typename T::module>> :
	is_T1_same<typename T::module, Tag>{ };

template <typename Tag, typename... Moduls>
struct select_helper {
	// NOTE: Moved into here due to aliasing issues, but this is redundant with global is_module
	template <typename T, typename = void> struct is_module : ::std::false_type {};
	template <typename T> struct is_module<T, ::libv::void_t<typename T::module>> :
		is_T1_same<typename T::module, Tag>{ };

	using type = typename ::libv::find_first<
				is_module,
				Moduls...
			>::type;
};

template <typename Tag, typename... Moduls>
using select = typename select_helper<Tag, Moduls...>::type;

// =================================================================================================

namespace tag {

struct accumulator { };
struct thread_policy { };
struct history_size { };

} //namespace tag

template <typename... Tags>
struct tag_type {
};

// -------------------------------------------------------------------------------------------------

template <typename... Moduls>
using select_accumulator = select<tag::accumulator, Moduls...>;
template <typename... Moduls>
using select_thread_policy = select<tag::thread_policy, Moduls...>;
template <typename... Moduls>
using select_history_size = select<tag::history_size, Moduls...>;

// -------------------------------------------------------------------------------------------------

} //namespace libv
