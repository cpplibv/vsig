// Stack -------------------------------------------------------------------------------------------

Collapse Signal-s output override
-- commit
History signal output auto-flush
-- commit
Implement SignalAsyncBase. This will reduce redundancy
-- commit
Implement more shit (SignalTypes)
-- commit
Find base classes
-- commit
Accumulator different _real_ return type then given
-- commit
Trackable type ereaser instead of inheritance
This basicly means remove Trackable base class and handle trackable and handle it as concept
-- commit
Use historyMax
-- commit
maybe hide signal "internal" types
-- commit
sort moduls from aliases
-- commit

// -------------------------------------------------------------------------------------------------

Trackable interface
	could it be non virtual?
	unite with weak and shared and observer?

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