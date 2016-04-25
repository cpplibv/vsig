// File:   thread_policy.hpp Author: Vader Created on March 13, 2016, 2:55 PM

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

	inline void readLock() { }
	inline void readUnlock() { }
	inline void writeLock() { }
	inline void writeUnlock() { }
};

struct MultiThread {
	using module = tag_type<tag::thread_policy>;

	std::mutex mutex;
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

//struct SharedMultiThread {
//	inline void readLock() { }
//	inline void readUnlock() { }
//	inline void writeLock() { }
//	inline void writeUnlock() { }
//};

// -------------------------------------------------------------------------------------------------

template <typename Mutex>
struct read_lock_guard {
	Mutex& mutex;
	read_lock_guard(Mutex& mutex) : mutex(mutex) {
		mutex->readLock();
	}
	~read_lock_guard() {
		mutex->readUnlock();
	}
};

template <typename Mutex>
struct write_lock_guard {
	Mutex& mutex;
	write_lock_guard(Mutex& mutex) : mutex(mutex) {
		mutex->writeLock();
	}
	~write_lock_guard() {
		mutex->writeUnlock();
	}
};

// -------------------------------------------------------------------------------------------------

} //namespace libv