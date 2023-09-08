#pragma once

#include <vector>
#include <chrono>
#include <cmath>
#include <set>
#include "SharedConstants.h"

long SieveOfAtkin(std::vector<int>& primes, int end = SIEVE_END_VALUE) {
	auto start = std::chrono::high_resolution_clock::now();
	std::vector<bool> sieve(end + 1, false);
	if (end > 2)
		sieve[2] = true;

	if (end > 3)
		sieve[3] = true;

	for (int i = 1; i * i <= end; i++) {
		for (int j = 1; j * j <= end; j++) {
			int n = (4 * i * i) + (j * j);
			if (n <= end && (n % 12 == 1 || n % 12 == 5))
				sieve[n] = !sieve[n];

			n = (3 * i * i) + (j * j);
			if (n <= end && n % 12 == 7)
				sieve[n] = !sieve[n];

			n = (3 * i * i) - (j * j);
			if (i > j && n <= end && n % 12 == 11)
				sieve[n] = !sieve[n];
		}
	}

	for (int i = 5; i * i <= end; i++) {
		if (sieve[i]) {
			for (int j = i * i; j <= end; j += i * i) {
				sieve[j] = false;
			}
		}
	}

	for (int i = 2; i <= end; i++) {
		if (sieve[i])
			primes.push_back(i);
	}

	auto end_time = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(end_time - start).count();
}