// File: main.cpp, Created on 2014. április 25. 21:23, Author: Vader

#include <iostream>
#include <type_traits>

#include <libv/sig/type_traits.hpp>
//#include <libv/sig/signal.hpp>

//// -------------------------------------------------------------------------------------------------
//
//struct A1 {
//	using ATag = void;
//	void v() {
//		std::cout << "A1" << std::endl;
//	}
//};
//struct A2 {
//	using ATag = void;
//	void v() {
//		std::cout << "A2" << std::endl;
//	}
//};
//
//struct B1 {
//	using BTag = void;
//	void v() {
//		std::cout << "B1" << std::endl;
//	}
//};
//struct B2 {
//	using BTag = void;
//	void v() {
//		std::cout << "B2" << std::endl;
//	}
//};
//
//// -------------------------------------------------------------------------------------------------
//
//LIBV_IMPLEMENT_HAS_TAG(is_a, ATag)
//LIBV_IMPLEMENT_HAS_TAG(is_b, BTag)
//LIBV_IMPLEMENT_SELECT_MODUL(select_a, is_a)
//LIBV_IMPLEMENT_SELECT_MODUL(select_b, is_b)
//LIBV_IMPLEMENT_SELECT_MODUL_OR_DEAFULT(select_a_or, is_a)
//
//// -------------------------------------------------------------------------------------------------
//
//template<typename... Moduls>
//struct foo {
//	void a() {
//		select_a_or<A2, Moduls...> mod;
//		mod.v();
//	}
//	void b() {
//		select_b<Moduls...> mod;
//		mod.v();
//	}
//};
//
//// -------------------------------------------------------------------------------------------------
//
//int main(int, char**) {
////	foo<A1, B1, int, void() > f;
////	foo<int, A1, B2, void() > f;
//	foo<int, B2, A2, void(), void(int) > f;
////	foo<int, B2, void(), void(int) > f;
//
//	f.a();
//	f.b();
//
//	return 0;
//}

//using namespace libv;
//
//int main(int, char**) {
//	Signal<> a;
//	Signal<> b;
//
//	a.output(b);
//
//	a.fire();
//
//	std::cout << "xy" << std::endl;
//
//	return 0;
//}

// -------------------------------------------------------------------------------------------------

using namespace libv;

#include <functional>

//template <typename F, typename... Args, typename I = disable_if_t<std::is_void<result_of_t<F>>>>
//inline void add(F&& func, Args&&... args, int /*ignored*/ = 0) {
//	std::cout << "not void" << std::endl;
//	func(std::forward<Args>(args)...);
//}

//template <typename F, typename... Args, typename I = enable_if_t<is_void_t<result_of_t<F>>>>
//inline void add(F&& func, Args&&... args) {
//	std::cout << "c" << std::endl;
//	func(std::forward<Args>(args)...);
//}

struct accumulator_traits {
	template <typename T>
	struct _helper {
		template <typename F, typename... Args>
		static inline bool add(F&& func, Args&&... args) {
			std::cout << "non void" << std::endl;
			func(std::forward<Args>(args)...);
			return true;
		}
	};

};
template <>
struct accumulator_traits::_helper<void> {
	template <typename F, typename... Args>
	static inline bool add(F&& func, Args&&... args) {
		std::cout << "void" << std::endl;
		func(std::forward<Args>(args)...);
		return true;
	}
};

template <typename R, typename... FArgs, typename... Args>
inline bool add(std::function<R(FArgs...)>& func, Args&&... args) {
	return accumulator_traits::_helper<R>::add(func, std::forward<Args>(args)...);
}

int main(int, char**) {
	std::function<int()> f0 = [] { return 42; };
	std::function<void()> f1 = [] { };
	std::function<int(int)> f2 = [] (int) { return 42; };
	std::function<void(int)> f3 = [] (int) { };

	add(f0);
	add(f1);
	add(f2, 2);
	add(f3, 3);

	//	typename std::result_of<decltype(f)>::type fr = f();
	//	std::cout << fr << std::endl;
	std::cout << "-- eof --" << std::endl;

	return 0;
}