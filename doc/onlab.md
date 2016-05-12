Ignore this file.
This document is not intended to be read in any way until properly formated.
Currently this is a random merge of notes that could be in the final version.

Wild Cards to be replaced: <<what>>
Wild Cards to be reviewed: ((what))



# VSIG

### Introduction
VSIG egy signal-slot (jelzés-foglalat) elvű generikus esemélykezelő rendszer nativ C++ header-only implementációja.
Használatával lehetőség van függvény hivás tetszőleges számosságú továbbítására egyébb
eseménykezelési .
A könyvtár erősen alapoz a modern C++11 erősen tipusus rendszerére, és annak kiaknázására.
Observer pattern
A könyvtár <<N>> signál tipust különböztett meg, melyek magasabb szintű absztrakciókat kinálnak.

---

5 line
what it is? template, generic, native c++, sig-slot, higher abstractions, warning resistance, header only

### Motivation
10 line
difference from alternatives, why it is worth the implementation: signal types
signal-signal-slot

### Terminology
Signal (Szignál vagy Jelzés) - A Signal egy
Node -
Slot (Foglalat) -
trackable (követhető) -
connection (kapcsolat) - signálok
namespaces
callable
life time,
observer_ptr?
accumulators
thread policy

### Lifetime / valid call
A probléma:
Objektumok közötti kapcsolatok mentén történt hivások nincsenek tekintettel az objektumok élettartalmára
//Több objektum között kialakított aktív kapcsolat esetén, a kapcsolatot még bármely fél megszünése előtt meg kell szüntetni.
//Ennek 3 módja lehet:
//((- A kapcsolat sziguran írányitott, tehát nem fordulhat elő hogy a hivott a hívó előtt megszünjön))
//- A hivott sosem szünik meg: ebben az esetben nincs szükség különösebben beavatkozni
//- A hivás ideje alatt a hivott garantáltan nem tud megszünni (weak_ptr - shared_ptr használata)
//- A hivás előtt megszüntetjük a kapcsolatot

Egyszerüsitve a problémát, tekintsük az alábbi fogalmakat:
Passziv kapcsolat: két objektum közti kapcsolat, de nincs hivás a kapcsolat mentén.
Aktiv kapcsolat: két objektum közti kapcsolat és az aktuális pillanatban hivás van jelen.
Ahoz, hogy egy kapcsolat mentén történt hivás biztonságát garantálni tudjuk az alábbi pontokat kell garantálni:
- Objektumok megszünése aktiv módban nem lehetséges:
		Tehát vagy az objektum élettartalma meghosszabodik vagy a megszünése blokkol ameddig létezik aktiv kapcsolata.
- Objektumok megszünése csak passziv módban lehetséges:
		Garantálja, hogy a kapcsolat nem tud újra aktív módba lépni.


- Egy signal elpusztulása csak akkor lehetséges ha minden kapcsolatát megszüntette.
Megoldás:
...


### Compile-Time / Run-Time
Concept, compile time cost, runtime cost

### Install / Integration / Build / CMake
compiler support, C++14, C++11
Még a legerőssebb warrning szinteken is warrning nélkül fordul. (Tesztelve: -Wall -Wextra -Wconversion -Wpedantic)

### Examples

### Usage
#### Aliasing
Három rétegű implementáció:
Implementáció: SignalImpl
Modul / traits selection: Signal<SingleThread> === SignalImpl<void(void), SingleThread>
Sugar coating: Signal<int> === Signal<void(int)>

### Project hierarchy
bin
build
cmake
doc
example
include
lib
sandbox
src
test

### Formating conventions
5 line
new style formating

### Main body
20 line / signal type
	pro, con, performance, usage, example, interactions
...
...

### Signal
### Signal Hub
### Accumulator
dynamic -> static, reason, pro / con

### Single / Multi Thread support

### Tests

### Performance
graphs and comparison to std::function, boost::signal2, plain vtble

### What it should be used for?
15 line
not free abstraction
extensibility

### What it should not be used for?
8 line
where it is not

### Alternatives
at least 5 lib, with pros and cons and link to there page
Boost
QT
other open src lib

### Links, materials
5 line

All rights reserved.