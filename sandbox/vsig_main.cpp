// File: main.cpp, Created on 2014. április 25. 21:23, Author: Vader

#include <iostream>
#include <type_traits>

#include <libv/sig/type_traits.hpp>
//#include <libv/sig/signal.hpp>
//#include <libv/sig/accumulator.hpp>

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
//template<typename... Modules>
//struct foo {
//	void a() {
//		select_a_or<A2, Modules...> mod;
//		mod.v();
//	}
//	void b() {
//		select_b<Modules...> mod;
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

//struct accumulator_traits {
//	template <typename T>
//	struct _helper {
//		template <typename F, typename... Args>
//		static inline bool add(F&& func, Args&&... args) {
//			std::cout << "non void" << std::endl;
//			func(std::forward<Args>(args)...);
//			return true;
//		}
//	};
//
//};
//template <>
//struct accumulator_traits::_helper<void> {
//	template <typename F, typename... Args>
//	static inline bool add(F&& func, Args&&... args) {
//		std::cout << "void" << std::endl;
//		func(std::forward<Args>(args)...);
//		return true;
//	}
//};
//
//template <typename R, typename... FArgs, typename... Args>
//inline bool add(std::function<R(FArgs...)>& func, Args&&... args) {
//	return accumulator_traits::_helper<R>::add(func, std::forward<Args>(args)...);
//}
//
//int main(int, char**) {
//	std::function<int()> f0 = [] { return 42; };
//	std::function<void()> f1 = [] { };
//	std::function<int(int)> f2 = [] (int) { return 42; };
//	std::function<void(int)> f3 = [] (int) { };
//
//	add(f0);
//	add(f1);
//	add(f2, 2);
//	add(f3, 3);
//
//	//	typename std::result_of<decltype(f)>::type fr = f();
//	//	std::cout << fr << std::endl;
//	std::cout << "-- eof --" << std::endl;
//
//	return 0;
//}
//
//template <typename T>
//struct X {
//template <typename K>
//struct Y {
//};
//};
//
//template <template <typename...> class M>
//struct O {
////	using type = typename M<void>::Y<void>;
//	template <typename T>
//	using type_help = typename M<void>::Y;
//	using type = type_help<void>;
//};
//struct A1 {
//	using module = tag_type<tag::accumulator>;
//
//	void foo() {
//		std::cout << "A1" << std::endl;
//	}
//};
//struct A2 {
//	using module = tag_type<tag::accumulator>;
//
//	void foo() {
//		std::cout << "A2" << std::endl;
//	}
//};
//struct B1 {
//	using module = tag_type<tag::thread_policy>;
//
//	void foo() {
//		std::cout << "B1" << std::endl;
//	}
//};
//struct C1 {
//	using module = tag_type<tag::history_size>;
//
//	void foo() {
//		std::cout << "C1" << std::endl;
//	}
//};
//
//int main(int, char**) {
////	std::cout << module_filter_helper<tag::accumulator>::type<A1>::value << std::endl;
////	std::cout << module_filter_helper<tag::accumulator>::type<A2>::value << std::endl;
////	std::cout << module_filter_helper<tag::accumulator>::type<B1>::value << std::endl;
////	std::cout << module_filter_helper<tag::accumulator>::type<C1>::value << std::endl;
//
//	//	std::cout << find_first<module_filter_helper<tag::accumulator>, A1>::found::value << std::endl;
//	//	select<tag::accumulator, A1, A2, B1> x;
//	select<tag::history_size, A1<void>, A2<void>, B1<void>, C1<void>> x;
//	x.foo();
//
//	std::cout << "-- eof --" << std::endl;
//
//	return 0;
//}

//// =================================================================================================
//namespace libv {
//
//template <template <typename...> class, typename Signature>
//struct module_filter;
//
//// -------------------------------------------------------------------------------------------------
//struct Tag_A {
//};
//struct Tag_B {
//};
//struct Tag_C {
//};
//
//// -------------------------------------------------------------------------------------------------
//struct Module_A {
//	using module = tag_type<Tag_A>;
//	void print() {
//		std::cout << " - Module_A" << std::endl;
//	}
//};
//struct Module_B {
//	using module = tag_type<Tag_B>;
//	void print() {
//		std::cout << " - Module_B" << std::endl;
//	}
//};
//struct Module_C {
//	using module = tag_type<Tag_C>;
//	void print() {
//		std::cout << " - Module_C" << std::endl;
//	}
//};
//
//// -------------------------------------------------------------------------------------------------
//template <typename S, typename A, typename B>
//struct Signal_A {
//	A a;
//	B b;
//
//	void print() {
//		std::cout << "Signal_A:" << std::endl;
//		a.print();
//		b.print();
//	}
//};
//template <typename R, typename... Args>
//struct module_filter<Signal_A, R(Args...)> {
//	using default_modules = list<Module_A, Module_B>;
//	using parameter_tags = list<Tag_A, Tag_B>;
//};
//
//// ---
//template <typename S, typename C, typename A, typename B>
//struct Signal_B : Signal_A<S, A, B> {
//	C c;
//	void print() {
//		std::cout << "Signal_B:" << std::endl;
//		this->a.print();
//		this->b.print();
//		c.print();
//	}
//};
//template <typename RType, typename... Args>
//struct module_filter<Signal_B, RType(Args...)> {
//	using base_type = module_filter<Signal_A, RType(Args...)>;
//
//	using default_modules = list<Module_C>;
//	using parameter_tags = list<Tag_C>;
//};
//
//// =================================================================================================
//
//template <typename MF, typename = void>
//struct get_module_filter_default_modules {
//	using type = list<>;
//};
//
//template <typename MF>
//struct get_module_filter_default_modules<MF, void_t<typename MF::default_modules>> {
//	using type = typename MF::default_modules;
//};
//
//template <typename MF, typename = void>
//struct get_module_filter_parameter_tags {
//	using type = list<>;
//};
//
//template <typename MF>
//struct get_module_filter_parameter_tags<MF, void_t<typename MF::parameter_tags>> {
//	using type = typename MF::parameter_tags;
//};
//
//// -------------------------------------------------------------------------------------------------
//
//template <typename MF, typename = void>
//struct summarieze_module_filter_default_modules {
//	using type = typename MF::default_modules;
//};
//
//template <typename MF>
//struct summarieze_module_filter_default_modules<MF, void_t<typename MF::base_type>> {
//	using type = typename concat_list<
//				typename get_module_filter_default_modules<MF>::type,
//				typename summarieze_module_filter_default_modules<typename MF::base_type>::type
//			>::type;
//};
//
//
//
//template <typename MF, typename = void>
//struct summarieze_module_filter_parameter_tags {
//	using type = typename MF::parameter_tags;
//};
//
//template <typename MF>
//struct summarieze_module_filter_parameter_tags<MF, void_t<typename MF::base_type>> {
//	using type = typename concat_list<
//				typename get_module_filter_parameter_tags<MF>::type,
//				typename summarieze_module_filter_parameter_tags<typename MF::base_type>::type
//			>::type;
//};
//
//// -------------------------------------------------------------------------------------------------
//
//template <typename Tags, typename Moduls>
//struct foreach_select;
//
//template <typename... Moduls>
//struct foreach_select<list<>, list<Moduls...>> {
//	using type = list<>;
//};
//
//template <typename TagsHead, typename... TagsTail, typename... Moduls>
//struct foreach_select<list<TagsHead, TagsTail...>, list<Moduls...>> {
//	using type = typename append<
//				select<TagsHead, Moduls...>,
//				typename foreach_select<list<TagsTail...>, list<Moduls...>>::type
//			>::type;
//};
//
//// -------------------------------------------------------------------------------------------------
//
//template <template <typename...> class Signal, typename... Args>
//struct process_module_filter;
//
//template <template <typename...> class Signal, typename RType, typename... Args, typename... Modules>
//struct process_module_filter<Signal, RType(Args...), Modules...> {
//	using raw_filter = module_filter<Signal, RType(Args...)>;
//
//	using summariezed_defaults = typename summarieze_module_filter_default_modules<raw_filter>::type;
//	using modules_with_defaults = typename concat_list<list<Modules...>, summariezed_defaults>::type;
//	using summariezed_tags = typename summarieze_module_filter_parameter_tags<raw_filter>::type;
//	using modules_to_be_applied = typename foreach_select<summariezed_tags, modules_with_defaults>::type;
//
//	using type = typename append<RType(Args...), modules_to_be_applied>::type;
//};
//
//// -------------------------------------------------------------------------------------------------
//
//template <template <typename...> class Signal, typename... Modules>
//struct signal_with_filtered_moduls {
//	using default_signature = void(void);
//	using signature = typename select_call_signature<Modules..., default_signature>::type;
//	using final_modul_list = typename process_module_filter<Signal, signature, Modules...>::type;
//	using type = typename apply_modul_pack<Signal, final_modul_list>::type;
//};
//
//// -------------------------------------------------------------------------------------------------
//
//template <typename... Modules>
//using Signal_Alias_A = typename signal_with_filtered_moduls<Signal_A, Modules...>::type;
//template <typename... Modules>
//using Signal_Alias_B = typename signal_with_filtered_moduls<Signal_B, Modules...>::type;
//
//} //namespace libv
//// -------------------------------------------------------------------------------------------------
//
//int main(int, char**) {
//	Signal_Alias_A<Module_B, Module_A> a0;
//	Signal_Alias_A<Module_B> a1;
//	Signal_Alias_B<Module_B, Module_C, Module_A> a2;
//	Signal_Alias_B<Module_A> a3;
//
//	a0.print();
//	a1.print();
//	a2.print();
//	a3.print();
//
//	std::cout << "-- eof --" << std::endl;
//	return 0;
//}

// =================================================================================================

//template <template<typename> class T>
//struct X_HOST {
//	void foo() {
//		std::cout << "X" << std::endl;
//	}
//};
//
//template <typename T, typename K>
//struct TO_WRAP {
//};
//
//template <typename T>
//struct X {
//	template <typename K>
//	struct Y : TO_WRAP<T, K> {
//	};
//};
//
//template <typename L>
//void foo(){
////	template <typename T>
////	using type = typename X<L>::Y;
////	X_HOST<X<L>::Y> x; //bad     ^
////	X_HOST<X<void>::Y> x; //ok   |
//	X_HOST<type> x; // would be ok? if type could be declared there...
//	x.foo();
//}

// =================================================================================================
//
//int main(int, char**) {
//	select<tag::accumulator, A1, A2, B1> x;
//	x.foo();
//
//	std::cout << "-- eof --" << std::endl;
//	return 0;
//}

#include <libv/sig/signal.hpp>

int main(int, char**) {
	libv::Signal<void(int)> signal0;
	libv::Signal<SingleThread, void(int)> signal1;

	signal0.output(signal1);
	signal1.output([](int x) {
		std::cout << x << std::endl;
	});

	signal0.fire(1);
	signal0.fire(2);
	signal0.fire(3);

	std::cout << "-- eof --" << std::endl;
	return 0;
}
