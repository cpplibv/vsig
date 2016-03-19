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

// -------------------------------------------------------------------------------------------------

template <typename... Type>
struct deafults {
};

template <typename... Types>
struct pack {
};

// -------------------------------------------------------------------------------------------------

template <template<typename...> class, typename...>
struct find_first;

template <template<typename...> class Predicate>
struct find_first<Predicate> {
	using type = void;
	using found = std::false_type;
};

template <template<typename...> class Predicate, typename Head, typename... Tail>
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

#define LIBV_IMPLEMENT_HAS_TAG(Name, Tag) \
template <typename T, typename = void> struct Name : std::false_type {}; \
template <typename T> struct Name<T, ::libv::void_t<typename T::Tag>> : std::true_type { };

// -------------------------------------------------------------------------------------------------

#define LIBV_IMPLEMENT_SELECT_MODUL(Name, Tag) \
template <typename... Moduls> using Name = typename ::libv::find_first<Tag, Moduls...>::type;

// -------------------------------------------------------------------------------------------------

#define LIBV_IMPLEMENT_SELECT_MODUL_OR_DEAFULT(Name, Tag) \
template <typename Default, typename... Moduls> \
using Name = typename std::conditional< \
	::libv::find_first<Tag, Moduls...>::found::value, \
	typename ::libv::find_first<Tag, Moduls...>::type, \
	Default \
>::type;

// -------------------------------------------------------------------------------------------------

LIBV_IMPLEMENT_HAS_TAG(is_signal, signal_tag)

LIBV_IMPLEMENT_HAS_TAG(is_accumulator, accumulator_tag)
LIBV_IMPLEMENT_SELECT_MODUL(select_accumulator, is_accumulator)
LIBV_IMPLEMENT_SELECT_MODUL_OR_DEAFULT(select_accumulator_or, is_accumulator)

LIBV_IMPLEMENT_HAS_TAG(is_thread_policy, thread_policy_tag)
LIBV_IMPLEMENT_SELECT_MODUL(select_thread_policy, is_thread_policy)
LIBV_IMPLEMENT_SELECT_MODUL_OR_DEAFULT(select_thread_policy_or, is_thread_policy)

// -------------------------------------------------------------------------------------------------

} //namespace libv