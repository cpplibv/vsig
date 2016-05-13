// Stack -------------------------------------------------------------------------------------------

per output store
route address
routing logic
Routing Signal
-- commit
Transform Signal
-- commit
review mutexing the system
reqursive_mutex -> mutex; functionNoLock versions if needs to be
review every mutex read / write: lock / unlock
deadlock multi thread loops on signals
-- commit
Dynamic Accumulator
Accumulator Host
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

full CRPT maybe not necessary, partial CRTP?
partial CRTP output?
partial CRTP fire?

Isnt this partial CRTP is one form of the CTVF?

HEHEHEHEEEe: this is a valid syntax...
typename = typename base_type::template is_acceptable_func<Func>
                    ^^^^^^^^^ dependent scope


simultaneous bidirectional connection may introduce deadlock on current trackable implementation
what makes boost signal2 trackable tick?
http://www.boost.org/doc/libs/1_60_0/boost/signals2/