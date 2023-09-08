#pragma once

#include <vector>
#include <chrono>
#include <cmath>
#include "SharedConstants.h"

long SieveOfSundaram(std::vector<int>& primes, int end = SIEVE_END_VALUE) {
	auto start = std::chrono::high_resolution_clock::now();
	long long numOddsToCheck = (end - 1) / 2;
	std::vector<bool> marked(numOddsToCheck + 1, false);//Contains all odd numbers from 1 to end;
	for (int i = 1; i <= numOddsToCheck; ++i) {
		int j = i;
		//(2 * i + 1) * (2 * j + 1) = 2 * 2 * i * j + 2 * i + 2 * j + 1 = 2 (i + j + 2 * i * j) + 1
		//Odd numbers are 2n + 1, so we have 2n + 1 where n = i + j + 2 * i * j
		//This converts each odd number to an index in the array.
		long long nextMarked = static_cast<long long>(i) + static_cast<long long>(j) + 2 * static_cast<long long>(i) * static_cast<long long>(j);
		while (nextMarked <= numOddsToCheck) {
			marked[nextMarked] = true;
			++j;
			nextMarked = static_cast<long long>(i) + static_cast<long long>(j) + 2 * static_cast<long long>(i) * static_cast<long long>(j);
		}
	}

	if (end >= 2)
		primes.push_back(2);

	for (int i = 1; i <= numOddsToCheck; ++i) {
		//Convert back from index to each odd number.
		if (!marked[i]) {
			primes.push_back(2 * i + 1);
		}
	}

	auto end_time = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(end_time - start).count();
}