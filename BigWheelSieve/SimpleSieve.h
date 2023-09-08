#pragma once

#include <vector>
#include <chrono>
#include <cmath>
#include <set>
#include "SharedConstants.h"

long SimpleSieve(std::vector<int>& primes, int end = SIEVE_END_VALUE) {
	auto start = std::chrono::high_resolution_clock::now();
	std::vector<bool> sieve(end + 1, true);
	sieve[0] = false;
	sieve[1] = false;
	for (int i = 2; i <= std::sqrt(end); ++i) {
		if (sieve[i]) {
			for (int j = i * i; j <= end; j += i) {
				sieve[j] = false;
			}
		}
	}
	for (int i = 0; i <= end; ++i) {
		if (sieve[i]) {
			primes.push_back(i);
		}
	}

	auto end_time = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(end_time - start).count();
}