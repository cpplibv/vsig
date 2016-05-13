// File: signal_test.cpp, Created on 2014. december 8. 2:19, Author: Vader

#include <catch.hpp>

#include "signal_test_util.hpp"
#include <libv/sig/signal.hpp>

#include <iostream>

// -------------------------------------------------------------------------------------------------

bool should_reach = false;

struct Base : libv::Trackable {
	void bar(int) {
		CHECK(should_reach);
	}
};

struct Member {
	void bar(int) {
		CHECK(should_reach);
	}
};

struct Derived : Base {
	void foo(int) {
		CHECK(should_reach);
	}
};

struct BaseOp : libv::Trackable {
	void operator()(int) {
		CHECK(should_reach);
	}
};

struct DerivedOp : BaseOp {
	void operator()(int) {
		CHECK(should_reach);
	}
};

struct MemberOp {
	void operator()(int) {
		CHECK(should_reach);
	}
};

void globalFunctionShouldReachCheck(int) {
	CHECK(should_reach);
}

template <typename Signal>
void fire_signal_helper(Signal& s, int i) {
	s.fire(i);
}

void fire_signal_helper(libv::CapacitiveSignal<int>& s, int i) {
	s.fire(i);
	s.flush();
}

// --- Output --------------------------------------------------------------------------------------

template <template <typename...> class SignalType>
void signal_output_into_everything() {
	SignalType<int> source;

	should_reach = true;
	{
		libv::Signal<int> targetSignal;
		libv::Trackable targetTrackingPoint;
		Base targetBase;
		Derived targetDerived;
		Base& targetPolymorf = targetDerived;
		BaseOp targetBaseOp;
		DerivedOp targetDerivedOp;
		BaseOp& targetPolymorfOp = targetDerivedOp;
		auto targetTrackingPointSP = std::make_shared<int>(0);
		auto targetMemberSP = std::make_shared<Member>();
		auto targetMemberOPSP = std::make_shared<MemberOp>();

		source.output(targetSignal);
		source.output(targetSignal, &libv::Signal<int>::operator());

		source.output(targetBase, &Base::bar);
		source.output(targetDerived, &Base::bar);
		source.output(targetPolymorf, &Base::bar);

		source.output(targetBase, &Derived::bar);
		source.output(targetDerived, &Derived::bar);
		source.output(targetPolymorf, &Derived::bar);

		source.output(targetDerived, &Derived::foo);
		source.output(targetMemberSP, &Member::bar);

		source.output(targetBaseOp, &BaseOp::operator());
		source.output(targetDerivedOp, &BaseOp::operator());
		source.output(targetDerivedOp, &DerivedOp::operator());
		source.output(targetMemberOPSP, &MemberOp::operator());

		source.output(targetBaseOp);
		source.output(targetDerivedOp);
		source.output(targetPolymorfOp);
		source.output(targetMemberOPSP);

		source.output(dummyGlobalFunction<void, int>);
		// Global function without tracking point: No check executed
		source.output([](int) { });
		// Global function without tracking point: No check executed

		source.output(targetTrackingPoint, globalFunctionShouldReachCheck);
		source.output(targetTrackingPoint, [](int) { CHECK(should_reach); });

		source.output(targetTrackingPointSP, [](int) { CHECK(should_reach); });

		CHECK(source.outputSize() == 23u);
		fire_signal_helper(source, 1);
		CHECK(source.outputSize() == 23u);
	}
	should_reach = false;

	fire_signal_helper(source, 2);
	CHECK(source.outputSize() == 2u);
}

TEST_CASE("Signal output into everything") {
	signal_output_into_everything<libv::Signal>();
	signal_output_into_everything<libv::CapacitiveSignal>();
	signal_output_into_everything<libv::SwitchSignal>();
	signal_output_into_everything<libv::HistorySignal>();
	signal_output_into_everything<libv::ConditionalSignal>();
}
