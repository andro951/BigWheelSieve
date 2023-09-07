#pragma once

#include <vector>
#include <string>

std::vector<int> PrimeFactors(int n) {
	std::vector<int> primes;
	int end = sqrt(n) + 1;
	SieveOfSundaram(primes, end);
	std::vector<int> factors;
	int primeIndex = 0;
	int prime = primes[primeIndex];
	while (prime * prime <= n) {
		while (n % prime == 0) {
			factors.push_back(prime);
			n /= prime;
		}

		if (++primeIndex < primes.size()) {
			prime = primes[primeIndex];
		}
		else {
			break;
		}
	}

	if (n > 1)
		factors.push_back(n);

	return factors;
}