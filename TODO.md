// Stack -------------------------------------------------------------------------------------------

Implement more shit (SignalTypes)
-- commit
Trackable and Trackable API
Trackable type ereaser instead of inheritance
This basicly means remove Trackable base class and handle trackable and handle it as concept
-- commit
Use historyMax
-- commit
collapse operator() and output(&O, D::f) onto base. CRTP required
-- commit
API / CompileTime virtual
-- commit

// -------------------------------------------------------------------------------------------------

Rename Accumulator to Combinator?
Add CombinatorHost?

Trackable interface
	could it be non virtual? at least partially?
	unite with weak and shared and observer?
	two headed trackable?

Signal Hub
Signal Proxy
Signal Local Hub
Signal Local Proxy

Signal Connection Marker


http://gcc.godbolt.org/


CTVF compile-time virtual function:
	CRTP and a helper layer with sfiane for correct function overload selection in derived

wrapper identity for nesting template type
	member template type to bypass template template type syntax

traits

implement CTVF

Different int / class / class id:
template<class T> int x;
template<class T> constexpr int* p = &x<T>;

benchmarks:
https://github.com/NoAvailableAlias/signal-slot-benchmarks


add_compile_options(-flto=13)
add_compile_options(-fconcepts)