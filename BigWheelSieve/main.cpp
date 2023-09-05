

#include <iostream>
#include <vector>
#include <set>
#include "SimpleSieve.h"
#include "BigWheelSieve.h"
#include "Logging.h"

int main() {
    std::vector<int> simpleSievePrimes;
    long simpleSieveTime = SimpleSieve(simpleSievePrimes);
    std::cout << "Simple Sieve Time: " << simpleSieveTime << "ms" << std::endl;
    //PrintVector(simpleSievePrimes, "Simple Sieve Primes");

    std::vector<int> bigWheelSievePrimes;
    long bigWheelSieveTime = BigWheelSieve(bigWheelSievePrimes);
    std::cout << "Big Wheel Sieve Time: " << bigWheelSieveTime << "ms" << std::endl;
    //PrintVector(bigWheelSievePrimes, "Big Wheel Sieve Primes");

    PrintVectorDiff(simpleSievePrimes, "Simple Sieve Primes", bigWheelSievePrimes, "Big Wheel Sieve Primes");
}