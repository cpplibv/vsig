// File: thread_policy.hpp Author: Vader Created on March 13, 2016, 2:55 PM

#pragma once

#include <mutex>
#include "tag.hpp"

// -------------------------------------------------------------------------------------------------

namespace libv {

//concept ThreadPolicy {
//	using module = tag_type<tag::thread_policy>;
//	void readLock();
//	void readUnlock();
//	void writeLock();
//	void writeUnlock();
//};

// -------------------------------------------------------------------------------------------------

struct SingleThread {
	using module = tag_type<tag::thread_policy>;
	static constexpr bool ALLOW_DISCONNECT_ON_DTOR = true;
	inline void readLock() { }
	inline void readUnlock() { }
	inline void writeLock() { }
	inline void writeUnlock() { }
};

struct MultiThread {
	using module = tag_type<tag::thread_policy>;
	static constexpr bool ALLOW_DISCONNECT_ON_DTOR = false;

	std::recursive_mutex mutex;
	inline void readLock() {
		mutex.lock();
	}
	inline void readUnlock() {
		mutex.unlock();
	}
	inline void writeLock() {
		mutex.lock();
	}
	inline void writeUnlock() {
		mutex.unlock();
	}
};

// -------------------------------------------------------------------------------------------------

struct adopt_lock_t {
};

static constexpr adopt_lock_t adopt_lock;

template <typename Mutex>
struct read_lock_guard {
	Mutex* mutex = nullptr;
	read_lock_guard(Mutex& mutex) : mutex(&mutex) {
		this->mutex->readLock();
	}
	read_lock_guard(Mutex& mutex, adopt_lock_t) : mutex(&mutex) { }
	read_lock_guard(const read_lock_guard<Mutex>& orig) = delete;
	read_lock_guard(read_lock_guard<Mutex>&& orig) {
		std::swap(mutex, orig.mutex);
	}
	~read_lock_guard() {
		if (mutex)
			mutex->readUnlock();
	}
};

template <typename Mutex>
auto make_read_lock_guard(Mutex& mutex) ->
read_lock_guard<Mutex> {
	return read_lock_guard<Mutex>(mutex);
}
template <typename Mutex>
auto make_read_lock_guard(Mutex& mutex, adopt_lock_t adopt) ->
read_lock_guard<Mutex> {
	return read_lock_guard<Mutex>(mutex, adopt);
}

template <typename Mutex>
struct write_lock_guard {
	Mutex* mutex = nullptr;
	write_lock_guard(Mutex& mutex) : mutex(&mutex) {
		this->mutex->writeLock();
	}
	write_lock_guard(Mutex& mutex, adopt_lock_t) : mutex(&mutex) { }
	write_lock_guard(const write_lock_guard<Mutex>& mutex) = delete;
	write_lock_guard(write_lock_guard<Mutex>&& orig) {
		std::swap(mutex, orig.mutex);
	}
	~write_lock_guard() {
		if (mutex)
			mutex->writeUnlock();
	}
};

template <typename Mutex>
auto make_write_lock_guard(Mutex& mutex) ->
write_lock_guard<Mutex> {
	return write_lock_guard<Mutex>(mutex);
}
template <typename Mutex>
auto make_write_lock_guard(Mutex& mutex, adopt_lock_t adopt) ->
write_lock_guard<Mutex> {
	return write_lock_guard<Mutex>(mutex, adopt);
}

// -------------------------------------------------------------------------------------------------

} //namespace libv