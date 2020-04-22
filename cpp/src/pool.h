/**
 * A pool object.
 *
 * @author Dennis Kristiansen
 * @file pool.h
 */

#pragma once

#include <functional>
#include <vector>

/**
 * A generic object pool.
 *
 * Allocates objects when the pool is full. So that creating a new object never
 * fails.
 *
 * @tparam T Type of the object stored in the pool.
 */
template <class T>
class Pool {
  public:
	Pool(size_t size = MIN_SIZE);
	T &  create();
	void map(std::function<bool(T &)> fn);
	size_t size() const { return firstDead; }

  private:
	static const size_t MIN_SIZE = 100;
	static const size_t MAX_SIZE = 1000;

	size_t firstDead = 0;

	T *mem = nullptr;
};

template <class T>
Pool<T>::Pool(size_t size) {
    mem = (T*)malloc(size);
}

template <class T>
T &Pool<T>::create() {
	// Find next unused
	// Tag it as used
	// Return reference to the object

	auto index = firstDead;
	firstDead += 1;

	// Init mem[index]?

	return mem[index];
}

template <class T>
void Pool<T>::map(std::function<bool(T &)> const fn) {
	// Iterate over all the objects in the pool
	// Run function with active objects as param
	// The lambda returns whether or not the object is active
	// If the object is no longer active, swap it with the last active one, and
	// set new firstDead

	// Iterate over pool backwards
	for (size_t i = firstDead; i-- > 0;) {
		if (!fn(mem[i])) {
			std::swap(mem[i], mem[firstDead - 1]);
			firstDead -= 1;
		}
	}
}

void test_object_pool() {
	//    Pool<bool> pool;
	//    bool b = true;
	//    auto fn = [b](bool &state){ state = b; return true; };

	Pool<int> pool;
	auto      fn = [](int &state) {
        state = 2;
        return true;
	};

	for (size_t i = 0; i < 50; i++)
		pool.create() = 0;

	pool.map(fn);
	pool.map([](int &state) {
		std::cout << state << std::endl;
		return true;
	});

	// Should count the number of active objects in the pool, and deactivate them
	int count = 0;
	pool.map([&count](int &state) {
		count++;
		return false;
	});
	std::cout << count << std::endl;

	// The count should now be 0
    count = 0;
    pool.map([&count](int &state) {
      count++;
      return false;
    });
    std::cout << count << std::endl;
}
