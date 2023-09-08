

#include <iostream>
#include <vector>
#include <set>
#include "SimpleSieve.h"
#include "SieveOfSundaram.h"
#include "SieveOfAtkin.h"
#include "BigWheelSieve.h"
#include "Logging.h"

int main() {
    /*
    std::vector<int> simpleSievePrimes;
    long simpleSieveTime = SimpleSieve(simpleSievePrimes);
    std::cout << "Eratosthenes Sieve Time: " << simpleSieveTime << "ms" << ", primesCount: " << simpleSievePrimes.size() << std::endl;
    //PrintVector(simpleSievePrimes, "Simple Sieve Primes");

    std::vector<int> sundaramPrimes;
    long sundaramTime = SieveOfSundaram(sundaramPrimes);
    std::cout << "Sundaram Sieve Time: " << sundaramTime << "ms" << ", primesCount: " << sundaramPrimes.size() << std::endl;
    //PrintVector(sundaramPrimes, "Sundaram Sieve Primes");
    */
    std::vector<int> atkinPrimes;
    long atkinTime = SieveOfAtkin(atkinPrimes);
    std::cout << "Atkin Sieve Time: " << atkinTime << "ms" << ", primesCount: " << atkinPrimes.size() << std::endl;
    //PrintVector(atkinPrimes, "Atkin Sieve Primes");

    std::vector<int> bigWheelSievePrimes;
    long bigWheelSieveTime = BigWheelSieve(bigWheelSievePrimes);
    std::cout << "Big Wheel Sieve Time: " << bigWheelSieveTime << "ms" << ", primesCount: " << bigWheelSievePrimes.size() << std::endl;
    //PrintVector(bigWheelSievePrimes, "Big Wheel Sieve Primes");

    PrintVectorDiff(atkinPrimes, "Simple Sieve Primes", bigWheelSievePrimes, "Big Wheel Sieve Primes");
}