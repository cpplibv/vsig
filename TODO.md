// Stack -------------------------------------------------------------------------------------------

make sure of fix ordering of moduls (make_traits<...>::type)
-- commit
Implement SignalAsyncBase. This will reduce redundancy
-- commit
Implement more shit (SignalTypes)
-- commit
Find base classes
-- commit
Accumulator different _real_ return type then given
-- commit
Collapse Signal-s output override
-- commit
Trackable type ereaser instead of inheritance
This basicly means remove Trackable base class and handle trackable and handle it as concept
-- commit
Use historyMax
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
