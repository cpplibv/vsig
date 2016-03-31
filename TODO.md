// Stack -------------------------------------------------------------------------------------------

API / CompileTime virtual
-- commit
Collapse Signal-s output override
With the previous commit there could be a way to implement it but in more generic based on
void output(RType(Object::*func)(Args...), Derivered* obj)
to
void output(Derivered* obj, RType(Object::*func)(Args...) = &Object::operator())
-- commit
Implement more shit (SignalTypes)
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
elimante multiple realization of the same signal if default parameter is set or not by moduls...
this will may bring back the traits in some form...
-- commit
inline RType fire(Args... args) ... //Where is my template?... NOWHERE! And that should stay in nowhere land...
the conversion is perfect if happens outside of my function call! It will happen. then i dont care where.
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