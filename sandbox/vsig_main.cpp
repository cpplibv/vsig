// File: main.cpp, Created on 2014. április 25. 21:23, Author: Vader

#include <iostream>
#include <type_traits>

#include <libv/sig/type_traits.hpp>

// -------------------------------------------------------------------------------------------------

struct A1 {
	using ATag = void;
	void v() {
		std::cout << "A1" << std::endl;
	}
};
struct A2 {
	using ATag = void;
	void v() {
		std::cout << "A2" << std::endl;
	}
};

struct B1 {
	using BTag = void;
	void v() {
		std::cout << "B1" << std::endl;
	}
};
struct B2 {
	using BTag = void;
	void v() {
		std::cout << "B2" << std::endl;
	}
};

// -------------------------------------------------------------------------------------------------

LIBV_IMPLEMENT_HAS_TAG(is_a, ATag)
LIBV_IMPLEMENT_HAS_TAG(is_b, BTag)
LIBV_IMPLEMENT_SELECT_MODUL(select_a, is_a)
LIBV_IMPLEMENT_SELECT_MODUL(select_b, is_b)
LIBV_IMPLEMENT_SELECT_MODUL_OR_DEAFULT(select_a_or, is_a)

// -------------------------------------------------------------------------------------------------

template<typename... Moduls>
struct foo {
	void a() {
		select_a_or<A2, Moduls...> mod;
		mod.v();
	}
	void b() {
		select_b<Moduls...> mod;
		mod.v();
	}
};

// -------------------------------------------------------------------------------------------------

int main(int, char**) {
//	foo<A1, B1, int, void() > f;
//	foo<int, A1, B2, void() > f;
	foo<int, B2, A2, void(), void(int) > f;
//	foo<int, B2, void(), void(int) > f;

	f.a();
	f.b();

	return 0;
}