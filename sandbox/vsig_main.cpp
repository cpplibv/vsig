// File: main.cpp, Created on 2014. április 25. 21:23, Author: Vader

template<typename R>
struct foo {
	static R f() {
		return R{};
	}
};

template<>
struct foo<void> {
	static void f() { }
};


template <typename T, T initialValue = T{}>
struct Accumulator {
	T _result{initialValue};
	inline bool add(const T& value) {
		_result += value;
		return true;
	}
	inline T& result() {
		return _result;
	}
};

struct AccumulatorVoid {
	inline bool add() {
		return true;
	}
	inline operator bool() {
		return true;
	}
	inline void result() { }
};

template <typename T, T V>
bool operator,(const T& val, Accumulator<T, V>& acc) {
	return acc.add(val);
}

template<typename R, typename Accum>
R main_v() {
	auto acc = Accum{};
	{
		if (!(foo<R>::f(), acc))
			return acc.result();
	}
	return acc.result();
}

int main(int, char**) {
	main_v<void, AccumulatorVoid>();
	main_v<int, Accumulator<int, 0>>();
	return 0;
}

