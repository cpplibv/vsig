# VSIG

## Introduction
VSIG is a generalized header-only library implementing a signal-slot like event handling system in
native C++14 with higher level of abstractions and extensibility.
VSIG has no other dependency then C++14 and does not require any additional build step.
The implementation should be portable and warning resistant.

## Motivation
Signal-Slot is a paradigm where Signals (sources of information / events / calls) can be connected
to Slots (drains / callback functions / functions) and if a Signal is notified (fired / called)
every connected Slot will receives it. This paradigm has already been implemented several times
but i found every implementation laking in some aspects. Most of the existing implementation were:
- either uses its own preprocessor (which is a non-standard and adds many dependencies or build steps)
- or does not utilizes modern C++ template features and therefore has some limitation
- or hard or cannot be extended with addition 'external' logic
- or concentrate only on the basic signal forwarding

I also found many trivial problem around event handling such as async calls, conditions or routing
which can be solved by the event handling system itself. The implementation also does not
distinguish Signals and Slots, everything is considered as a Signal thus signals can be chained.
And the library is full of 'proof of concepts' so i learned more implementing it then any other way.

## Build
VSIG is header-only therefore coping the content of the include folder and including it should be sufficient as long as you use C++14.
However when my times allows it there will be complete and full integration as CMake install.

Test can be run by
- `cmake path-to-project-root`
- `make ext`
- `make vsig_test`
- and run the vsig_test in `/bin` directory

## Examples
The following example are based on this skeleton:
```
#include <iostream>
#include <libv/sig/signal.hpp>

int main() {
	// Example code
	return 0;
}
```

#### Hello, World!
The following example writes "Hello, World!" using the library.

1. we create a signal, a signal that takes no arguments and has a void return value.
2. we connect the hello lambda function to the signal using the output method. (Any object is acceptable which has correct function call operator)
3. we use the signal like a function to call the slots, which in turns print "Hello, World!".

```
libv::Signal<> signal;
signal.output([]{ std::cout << "Hello World!" << std::endl; });
signal();
```

#### Signal Parameters and Return Value
The following example shows signals with return value and parameters

1. we create a signal, a signal that takes two int as argument and has an int as return value.
2. we connect a function with int(int, int) signature.
3. we use the signal and we get the result 42.

```
libv::Signal<int(int, int)> signal;
signal.output([](int a, int b) { return a + b; });
std::cout << signal(30, 12) << std::endl;
```

#### Capacitive Signal
The following example TODO

1. we create a capacitive signal, a signal that can store the calls and flush them async.
2. we connect a function.
3. we use the signal multiple time but not receive any output.
4. we flush the signal and receive all three call.

```
libv::CapacitiveSignal<int> signal;
signal.output([](int x){ std::cout << x << ','; });
signal(1); // no output
signal(2); // no output
signal(3); // no output
signal.flush(); // output: 1,2,3,
```

#### Routing Signal
The following example demonstrates the use of the RoutingSignal where we forward the call only to those who match a criteria

1. we create a routing signal with setting the routing condition to match in a range the first parameter of the calls with int(int) signature and summarize the return values.
2. we connect the first function setting its address range to [5,15]
3. we connect the second function and use a shortcut to set address range to [10,20]
4. we use the signal with 6 as parameter what is match only in the first range so its calls the first function and return 1;
5. we use the signal with 12 as parameter what is matching both range therefore calls both function and return 3;

```
libv::RoutingSignal<int(int), libv::AccumulatorSum<int>, libv::RoutingFirstArgAsInRange<int>> signal;
signal.output(libv::RoutingRangeAddress<int>{5, 15}, [](int){ return 1; });
signal.output(10, 20, [](int){ return 2; });
std::cout << signal(6) << std::endl; //output: 1
std::cout << signal(12) << std::endl; //output: 3
```

## Signal Types
#### Signal
Basic Signal type.

Compatible modules:
- call_syntax
- accumulator
- thread_policy

#### CapacitiveSignal
Signal that is capable of storing and async distributing calls.
The regular call simply stores the call arguments (any reference type will be copied into a temporary).
Distributing is possible via `flush` member function.

Compatible modules:
- call_syntax - Restriction: only void return type (there are plans for lifting this restriction)
- thread_policy

#### ConditionalSignal
Signal that is capable of optionally forwarding calls on a per-call condition

Compatible modules:
- call_syntax
- condition
- accumulator
- thread_policy

#### HistorySignal
Signal that is capable of storing the history of previous calls and forwarding it to late subscribers.
The regular call distributes and stores the call arguments (any reference type will be copied into a temporary).
Late subscribers via `output` member function will receive previous calls in a synchronous manner.

Compatible modules:
- call_syntax
- history_size
- thread_policy

#### RoutingSignal
Signal that is capable of optionally forwarding calls on a per-output condition.

Compatible modules:
- call_syntax
- routing_logic
- accumulator
- thread_policy

#### SwitchSignal
Signal that can be disabled or enabled at runtime.

Compatible modules:
- call_syntax
- accumulator
- thread_policy

#### Additional planed Signals
- TransformSignal - Signal that can manipulate the call arguments value, type or number.
- AdaptiveSignal - Transform like signal but with per-output level instead of per-call level transformation.
- PrioritySignal - Capacitive like signal where the stored calls are sorted in priority.
- OutPrioritySignal - Signal where the distribution order is based on output priority.
- UniqueSignal - Capacitive like signal where the stored calls remains unique.
- MergerSignal - Signal that shallows redundant calls.
- RepeaterSignal - Signal that repeats its input multiple times.
- WorkerSignal - Signal with own thread or thread pool for call distribution.

## Modules
Modules are logical extension or modifiers to Signals. Not every module is compatible with every Signal.
Modules can be assigned to a Signal just by putting it into the signal's template parameter list.
Example: Signal<Module1, Module2, Module3>. The order of the Modules does not matter (for more info about this see Aliasing).

#### accumulator
Accumulator or combinator of return values. Merges multiple return values into one return value.
Accumulators can interrupt distribution (useful for logical shortcuts) or can change the return type (useful for not loosing precision).

Examples:
- `AccumulatorAnd<T>` - Evaluate return values as logical AND with shortcut.
- `AccumulatorCounter<T>` - Ignores original return value and returns the count of call delivery.
- `AccumulatorLast<T>` - Returns the last return value.
- `AccumulatorLimiter<T, N>` - Ignores original return value and limit the count of call delivery to N.
- `AccumulatorSum<T>` - Summarizes the return values.

#### call_syntax
Special module. Determines the arguments and the return type.

Example:
- `Signal<>` - void return value with no argument
- `Signal<int>` - void return value with one int as argument
- `Signal<int(double)>` - int return value with one double as argument

#### condition
Determines the pre-condition of call forwarding.

Example:
- `ConditionDynamic<Args...>` - Allows run time condition change. Adds setCondition(F) (F is bool(Args...) like function object).
- `ConditionStatic<F>` - Allows compile time condition change where (F is bool(Args...) like function type).
- `ConditionSwitch` - Adds enable and disable public member functions with the expected behavior into the Signal.

#### history_size
Determines the maximum number of calls stored.

Example:
- `history_size<N>` - sets the maximum number of calls stored to N, 0 means no limit.

#### routing_logic
Determines the routing logic.

Example:
- `RoutingFirstArgAsAddress<T>` - Using the first argument of the call and matches against the output's value
- `RoutingFirstArgAsInRange<T>` - Using the first argument of the call and matches against the output's range

#### thread_policy
Determines the multi thread policy.

Example:
- `SingleThread`
- `MultiThread`

## Multi Thread support
There is full multi-thread support.
Thread policies applied on per-endpoint basis.
Currently the default thread support is Single thread due the nature of event handling.

To access thread-safe version of any signal or endpoint just use `MultiThread` thread_policy module.

Example for Signals:
- `Signal<MultiThread>`
- `Signal<int(int), MultiThread>`
- `CapacitivSignal<int, int, int, MultiThread>`

Example for Trackable:
- `TrackableThread<MultiThread>`

## Aliasing
One of the main unique feature of the library is its aliasing system.

Not a unique problem that in case of big number of template parameters
the order and the default values are becoming a unmanageable or hard enough to not want to manage.

Therefore the library has its own aliasing and template parameter deduction system where
the template parameter order does not matter and every not given template parameter will
be set to its default value.

The deduction pseudo rules are follows:
- Modules are found by `module` member type tag.
- If a Module is not set but required then default value will be used.
- Call syntax are found based on R(Args...) syntax.
- If call syntax is present every other parameter has to be a module.
- If call syntax is not present return type set to void and every non module parameter will be handled as Arguments with respect to order.

If the deduction or parsing of template parameters fail you will most likely receive a static assert error message which tries to help you.

##### Example A:

Consider the following, you want to write a Signal with int return value, no parameter, sum accumulator and in Single thread.
But what would be the correct template parameter order?:

- `Signal<int(), AccumulatorSum<int>, SingleThread>`
- `Signal<int(), SingleThread, AccumulatorSum<int>>`
- `Signal<AccumulatorSum<int>, int(), SingleThread>`
- `Signal<AccumulatorSum<int>, SingleThread, int()>`
- `Signal<SingleThread, int(), AccumulatorSum<int>>`
- `Signal<SingleThread, AccumulatorSum<int>, int()>`

The beauty of the system that every one of the lines above are correct.
Moreover, because in this case the deafult Accumulator and ThreadPolicy would be
`SingleThread` and `AccumulatorSum<int>` the following versions are also correct:

- `Signal<int(), AccumulatorSum<int>>`
- `Signal<AccumulatorSum<int>, int()>`
- `Signal<int(), SingleThread>`
- `Signal<SingleThread, int()>`
- `Signal<int()>`

Every 11 version of this example will result in the exact same type of

`SignalImpl<int(), AccumulatorSum<int>, SingleThread>`

##### Example B:

In most use cases return values are void, so lets see the shortcut to omit call syntax and use hybrid syntax:

- `Signal<void(const MouseEvent&), MultiThread>`
- `Signal<const MouseEvent&, MultiThread>`

Which is also can be written as:

- `Signal<MultiThread, const MouseEvent&>`
- `Signal<MultiThread, void(const MouseEvent&)>`

And all of them would result in: `SignalImpl<void(const MouseEvent&), AccumulatorSum<void>, MultiThread>`

## Performance
TODO: graphs and comparison to std::function, boost::signal2, plain vtble

## When it should be used?
- Whenever you are dealing with a high extensibility requirements in a callback or signal system
- When the focus is on the extensibility and maintainability instead of raw like performance
- When callback are planed to handle non trivial tasks

## When it should **NOT** be used?
- When a single function call would be sufficient
- When raw performance is essential

## Known issues
- Inheritance based modules (Condition Module) has no generalized way for multi-thread safety if it has mutable state.

## Alternatives

Boost.Signals2 - http://www.boost.org/doc/libs/1_61_0/doc/html/signals2.html

QT - http://doc.qt.io/qt-5/signalsandslots.html

libsigc - http://libsigc.sourceforge.net/
