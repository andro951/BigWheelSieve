#pragma once

#include <vector>
#include <chrono>
#include <cmath>
#include <set>
#include <map>
#include "SharedConstants.h"
#include "Wheel.h"
#include "Logging.h"

//Starting number = wheel * wheel
//Condition to drop wheel: nextValue > end || 

long BigWheelSieve(std::vector<int>& primes, int end = SIEVE_END_VALUE) {
	auto start = std::chrono::high_resolution_clock::now();
	int num = 1;
	int bigWheelCircumfrance = 1;
	int nextWheelToDrop = 1;
	int primesNextWheelIndex = 0;
	int bigWheelIndex = 0;
	int bigWheelSize = 1;
	primes = { 1 };
	//std::vector<int> wheels = { 1 };
	std::vector<int> pseudoPrimeIndexes = { 0 };//Same size as primes
	std::vector<int> wheelsNextPseudoPrime = { 1 };//Same size as primes
	std::vector<int> wheelsNextValues = { 1 };//Same size as primes

	std::vector<int> pseudoPrimes = { 1 };//pseudoPrimes are all prime^n
	std::vector<int> bigWheel = { 1 };

	std::vector<int> que = { };//Continuously grows.  TODO: replace with a queue.
	std::map<int, std::vector<int>> stitches = { };
	int queIndex = 0;
	int wheelsNextValue = 1;

	while (num < end) {
		//std::cout << "num += bigWheel[" << bigWheelIndex << "]; " << num << " += " << bigWheel[bigWheelIndex] << " = " << num + bigWheel[bigWheelIndex] << std::endl;
		num += bigWheel[bigWheelIndex];

		if (num == wheelsNextValue) {
			//TODO: This never triggers currently.
			//not prime
			int wheelIndex = que[queIndex];//1
			int wheel = primes[wheelIndex];//2
			
			stitches[wheel].push_back(num);

			if (wheelsNextPseudoPrime[wheelIndex] == num) {
				pseudoPrimes.push_back(num);
				wheelsNextPseudoPrime[wheelIndex] = num * wheel;
			}

			int nextValue = pseudoPrimes[++pseudoPrimeIndexes[wheelIndex]] * wheel;//3 * 2 = 6
			int i = queIndex + 1;
			while (wheelsNextValues[que[i]] < nextValue) {//TODO: Do binary search.
				i++;
			}

			que.insert(que.begin() + i, wheelIndex);
			wheelsNextValues[wheelIndex] = nextValue;
			
			//Might be needed if wheels are added past the point when they sould have been dropped.
			//while (que[queIndex] < nextWheelToDrop) {
			//	++queIndex;
			//}

			wheelsNextValue = wheelsNextValues[que[++queIndex]];
		}
		else {
			//prime
			primes.push_back(num);
			pseudoPrimes.push_back(num);
			pseudoPrimeIndexes.push_back(pseudoPrimeIndexes.size() - 1);//TODO: replace size() with an integer that tracks size instead.
			int square = num * num;
			if (square < end) {
				int primeIndex = primes.size() - 1;
				wheelsNextPseudoPrime.push_back(square);
				wheelsNextValues.push_back(square);
				que.push_back(primeIndex);
				//if (wheelsNextValue < num)//TODO: remove when wheelsNextValue is set to 2, 3, 5, 7, etc.
				//	wheelsNextValue = square;
				
				stitches.emplace(num, std::vector<int>{ num });// .insert({ num, });
			}
		}

		//Check if the bigWheel needs to be updated and if the 
		if (++bigWheelIndex == bigWheelSize) {
			//PrintVector(primes, "Primes");
			//std::cout << "Big Wheel Index: " << bigWheelIndex << std::endl;
			//std::cout << "Dropping wheel " << nextWheelToDrop << std::endl;
			//PrintVector(bigWheel, "Big Wheel before");
			//Do stitches for current wheel before sizing up the big wheel.
			int tempNum = 1;
			int tempBigWheelIndex = -1;
			//PrintVector(stitches[nextWheelToDrop], "Stitches");
			for (const auto& stitch : stitches[nextWheelToDrop]) {//TODO: Make stitches pointers/reference to the bigWheel values instead of using tempNum/tempBigWheelIndex
				//Stitch
				while (tempNum < stitch) {
					int numToAdd = bigWheel[++tempBigWheelIndex];
					tempNum += numToAdd;
				}
				
				int numToAdd2 = bigWheel[++tempBigWheelIndex];
				tempNum += numToAdd2;
				if (tempBigWheelIndex == 1) {
					tempBigWheelIndex = 0;
					//Stitch end and first
					//TODO: Check this, may have to be swapped
					//bigWheel[bigWheel.size() - 1] += bigWheel[0];//Send first to end
					//bigWheel.erase(bigWheel.begin());//Remove first
					//bigWheel[0] += bigWheel[bigWheel.size() - 1];//Send end to first
					//bigWheel.erase(bigWheel.end() - 1);//Remove end
					bigWheel[0] += bigWheel[1];
					bigWheel.erase(bigWheel.begin() + 1);
					--bigWheelIndex;
				}
				else if (tempBigWheelIndex == 0) {
					//Stitch end and 2nd to end
					int bigWheelEnd = bigWheel.size() - 1;
					bigWheel[bigWheelEnd - 1] += bigWheel[bigWheelEnd];
					bigWheel.erase(bigWheel.end() - 1);
				}
				else {
					//Stitch previous 2
					bigWheel[--tempBigWheelIndex - 1] += bigWheel[tempBigWheelIndex];
					bigWheel.erase(bigWheel.begin() + tempBigWheelIndex);
					--bigWheelIndex;
				}

				--bigWheelSize;
			}

			//PrintVector(bigWheel, "Big Wheel after Stitches");

			++primesNextWheelIndex;
			nextWheelToDrop = primes[primesNextWheelIndex];
			std::vector<int> bigWheelCopy = bigWheel;
			for (int i = 0; i < nextWheelToDrop - 1; ++i) {
				bigWheel.insert(bigWheel.end(), bigWheelCopy.begin(), bigWheelCopy.end());
			}

			bigWheelSize = bigWheel.size();//TODO: calculate manually.  bigWheelSize *= nextWheelToDrop;
			bigWheelCircumfrance *= nextWheelToDrop;
			//PrintVector(bigWheel, "Big Wheel after size up");
			//std::cout << std::endl;
		}
	}

	auto end_time = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start).count();
}