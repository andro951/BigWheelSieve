/*
Author - Isaac Richards
Date - 04SEP23
Description - Prime sieve that aims to create 1 large compositeWheelBasePrime that is an array of numbers which represent the amount that has to be added
	to the current prime number to get the next prime number.
	Each prime starting with 2 is treated as a compositeWheelBasePrime.  It is "spun" to check which values it prevents until the patter it creates repeats.
	This repeating patter is extremely important because it has a flaw.
	The size of this repetition pattern (which I refer to as compositeWheelBasePrime repetition circumference) becomes very large very quickly.
	Each time a compositeWheelBasePrime's repetition pattern is completed, the size of the big compositeWheelBasePrime is multiplied by the next compositeWheelBasePrime's size.
		Start
		Wheel being checked: 1, Big Wheel Circumference: 1
		Wheel being checked: 2, Big Wheel Circumference: 2
		Wheel being checked: 3, Big Wheel Circumference: 6
		Wheel being checked: 5, Big Wheel Circumference: 30
		Wheel being checked: 7, Big Wheel Circumference: 210
		Wheel being checked: 11, Big Wheel Circumference: 2310
		Wheel being checked: 13, Big Wheel Circumference: 30030
		Wheel being checked: 17, Big Wheel Circumference: 510510
		...
		To prevent overflow, I cap the big compositeWheelBasePrime size at the "end" value when the sieve is running since the compositeWheelBasePrime circumference
			is not needed if it is larger than end.
	The first time a prime number is hit, it creates a "stitch".
	Stitches are points in their repetition where prime number will never occur again, so the big compositeWheelBasePrime at that index and the
		previous index need to be combined together or "stitched".
	For example, After the 2 compositeWheelBasePrime is dropped, the big compositeWheelBasePrime is [2, 2, 2] which started at 1, so 1, 3, 5, 7, 9, 11, 13, 15, 17... are numbers the big compositeWheelBasePrime currently thinks are primes.
	The first number that is not a prime is 9.  It is not a coincidence that 9 is 3 * 3 which has not completed it's repetition circumference.
	The 3 compositeWheelBasePrime is dropped after checking the next prime number. which causes it's stitches to be applied to the big compositeWheelBasePrime,
		then the big compositeWheelBasePrime is duplicated 5 times because it's now time to calculate 5's repetition circumference.
	The big compositeWheelBasePrime goes from [2, 2, 2] to [4, 2] from applying the stitch from 3 being hit, then is duplicated 5 times.
	From [4, 2] to [4, 2, 4, 2, 4, 2, 4, 2, 4, 2] which is [4, 2] * 5.
	Counting starts at 1, then add each number in the big compositeWheelBasePrime to see what the next prime is.
	[2, 2, 2] is 1 + 2 + 2 + 2 or 1 + 2 = 3 + 2 = 5 + 2 = 7.  Because the repetition of the pattern 3 makes has a circumference of 6,
		there will be a NON-PRIME number that is divisible by 3, but no previous primes every 6 numbers.
		3 needs to stitch this spot to prevent non-primes from being hit by the big compositeWheelBasePrime.:
		1, 3(div by 3 and no others below 3), 5, 7, 9(div by 3 and no others below 3), 11, 13, 15(div by 3 and no others below 3)
		So the stitch is applied to the first and second number in the big compositeWheelBasePrime so [2, 2, 2] becomes [4, 2] which will make the sequence look like:
		1, 5, 7, 11, 13, 17, 19, 23, 25, 29, 31...
		This looks a lot better, but the 5 compositeWheelBasePrime has a problem.
		Notice with 3, it's repetition circumference is 6, but 5's is 30.
		The first multiple of 3 that is not a multiple of primes below it is 9, which is less than it's repetition circumference.
		However with 5, it's repetition circumference is 30, but the first multiple of 5 that is not a multiple of primes below it is 25.
		25 is less than 30, so it's not taken care of by the stitch created when 5 is hit.
		Another stitch is needed to prevent 25's from being hit by the big compositeWheelBasePrime in the future.
		I say 25's because the patter will repeat in the form of 30n + 25:
		  [6,  4,  2,  4,  2,  4,  2,  4,  2] [6,  4,  2,  4,  2,  4,  2,  4,  2] [6,  4,  2,  4,  2,  4,  2,  4,  2]
		1, 7, 11, 13, 17, 19, 23, 25, 29, 31, 37, 41, 43, 47, 49, 53, 55, 59, 61, 67, 71, 73, 77, 79, 83, 85, 89, 91...
		Notice the 25, 55 and 85 which are all 30n + 25.
		this means that the 5 pattern needs to be stitches where 25 would occur:
		  [6,  4,  2,  4,  2,  4,  6,  2] [6,  4,  2,  4,  2,  4,  6,  2] [6,  4,  2,  4,  2,  4,  6,  2]
		1, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 49, 53, 59, 61, 67, 71, 73, 77, 79, 83, 89, 91...
		Great, now the 5 compositeWheelBasePrime's repetition circumference is taken care of.
		However, as I mentioned before, the repetition circumference grows significantly as the wheels get larger.
		The 7 repetition (size 210) needs to deal with 7 * 7 (49), 7 * 11 (77), 7 * 13 (91), 7 * 17 (119), 7 * 19 (133), 7 * 23 (161), 7 * 29 (203)
		Ok, so we need to multiply the 7 compositeWheelBasePrime by every prime until the product is larger than it's circumference.  Not too bad.
		What about 11?
		The 11 repetition (size 2310) needs to deal with 11 * 11 (121), 11 * 13 (143), 11 * 17 (187), 11 * 19 (209), 11 * 23 (253),
			11 * 29 (319), 11 * 31 (341), 11 * 37 (407), 11 * 41 (451), 11 * 43 (473), 11 * 47 (517), 11 * 53 (583), 11 * 59 (649),
			11 * 61 (671), 11 * 67 (737), 11 * 71 (781), 11 * 73 (803), 11 * 77 (847), 11 * 79 (869), 11 * 83 (913), 11 * 89 (979),
			11 * 91 (1001), 11 * 97 (1067), 11 * 101 (1111), 11 * 103 (1133), 11 * 107 (1177), 11 * 109 (1199), 11 * 113 (1243),
			11 * 119 (1309), 11 * 121 (1331)....... oh no.... That's 11^3.....oh boy.

		So... We need to multiply the current compositeWheelBasePrime by every combination of primes higher than it where the result is less than it's repetition circumference.
		...Awesome....super nice....exactly what I wanted...
		This is not limited to prime^n.  It is every combination of primes where the result is less than the repetition circumference.
			Such as 11 * 13 * 13 or 17 * 17 * 19 * 43

		In it's current state, this program is accurate when end <= 100000.  After that, sets of 4 prime multiples and more are missed.
			Factors of 104329: 17, 17, 19, 19
			Factors of 126293: 17, 17, 19, 23
			Factors of 152881: 17, 17, 23, 23
			Factors of 159239: 17, 17, 19, 29
			Factors of 170221: 17, 17, 19, 31
			Factors of 170867: 17, 19, 23, 23
		Those are the first few.
		More work is needed with the children of composite heels to handles these.
*/

#pragma once

#include <vector>
#include <chrono>
#include <cmath>
#include <set>
#include <map>
#include <queue>
#include "SharedConstants.h"
#include "SimpleSieve.h"
#include "Wheel.h"
#include "Logging.h"
#include "PrimeFactorization.h"

//TODO:
//	Convert stitches to vector.
//	Shift starting point to have 5 or 7 as the starting compositeWheelBasePrime.
//	Get rid of 1 from primes and related.
//	change queue to an actual queue
//
//	calculate all queue values as soon as possible.  Need to figure that part out.
//	Clean up things remaining from using a vector instead of a queue.
//
//	Make stitches pointers or something similar to eliminate the repetitive rolling for stitches.
//	Try ANOTHER COPY/PASTED VERSION that uses an array of bools instead of a queue set.
//	Try ANOTHER COPY/PASTED VERSION that uses a vector of compositeWheelBasePrime objects instead of all the vectors
//	Make a Blocking Wheel Struct/Class for the smallerCompositeWheel vectors
//	Make a Wheel or Prime Struct/Class for the prime vectors

long BigWheelSieve(std::vector<int>& primes, int end = SIEVE_END_VALUE) {
	auto start = std::chrono::high_resolution_clock::now();
	if (end <= 7) {
		//Starts with 2, 3, 5, 7.  Designed to handle a much higher end value than < 7.
		if (end < 7) {
			int lastPrime;
			do {
				primes.pop_back();
				lastPrime = primes.size() > 1 ? primes.back() : 0;
			} while (lastPrime > end && lastPrime > 0);
		}

		auto end_early_time = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(end_early_time - start).count();
	}

	int potentialPrime = 11;
	int bigWheelCircumfrance = 30;
	int nextWheelToDrop = 5;
	int primesNextWheelIndex = 2;
	int bigWheelIndex = 2;
	int bigWheelSize = 10;
	primes = { 2, 3, 5, 7 };
	std::map<int, int> primeIndexes = { { 2, 0 }, { 3, 1 }, { 5, 2 }, {7, 3 } };
	std::vector<int> wheelRepititionCircumfrances = { 2, 6, 30, 210 };//Same size as primes
	std::vector<std::vector<int>> stitches = { {}, {}, { 5 }, { 7 } };//Same size as primes

	std::vector<int> compositeWheels = { 5, 7 };
	std::vector<int> compositeWheelNextBlockingValues = { 25, 49 };//Same size as compositeWheels
	std::vector<int> compositeWheelBasePrimes = { 5, 7 };//Same size as compositeWheels//Could be calculated from primes[compositeWheelToPrimeWheelIndex[compositeWheelIndex]] instead
	std::vector<int> compositeWheelPrimeMultiplierIndexes = { 2, 3 };//Same size as compositeWheels
	std::vector<int> compositeWheelToPrimeWheelIndex = { 2, 3 };//Same size as compositeWheels
	std::vector<int> compositeWheelPrimeStartingIndexs = { 2, 3 };//Same size as compositeWheels
	std::vector<bool> compositeWheelWillHaveChildren = { false, false };//Same size as compositeWheels

	std::vector<int> bigWheel = { 4, 2, 4, 2, 4, 2, 4, 2, 4, 2 };

	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> queue;
	queue.push({ 25, 0 });
	queue.push({ 49, 1 });

	int nextCompositeWheelBlockingValue = 25;



	bool printAll = false;

	bool troubleShootingQue = false;
	std::vector<int> queValuesForTroubleshooting = { };

	bool troubleShootingThreeOrHigherMultipules = false;

	do {
		//Check if the number is the next non-prime that is not taken care of by the big compositeWheelBasePrime.
		if (potentialPrime == nextCompositeWheelBlockingValue) {
			//not prime
			std::pair<int, int> queValue = queue.top();
			queue.pop();
			int compositeWheelIndex = queValue.second;
			int compositeWheel = compositeWheels[compositeWheelIndex];
			int compositeWheelPrimeStartingIndex = compositeWheelPrimeStartingIndexs[compositeWheelIndex];
			int compositeWheelPrimeStartingValue = primes[compositeWheelPrimeStartingIndex];
			int compositeWheelBasePrime = compositeWheelBasePrimes[compositeWheelIndex];
			int compositeWheelBasePrimeIndex = compositeWheelToPrimeWheelIndex[compositeWheelIndex];
			int wheelRepetitionCircumfrance = wheelRepititionCircumfrances[compositeWheelBasePrimeIndex];
			stitches[compositeWheelBasePrimeIndex].push_back(potentialPrime);
			if (compositeWheelWillHaveChildren[compositeWheelIndex]) {
				int newCompositeWheelBlockingValue = potentialPrime * compositeWheelPrimeStartingValue;
				if (wheelRepetitionCircumfrance >= newCompositeWheelBlockingValue) {
					if (troubleShootingThreeOrHigherMultipules) {
						PrintPrimeFactors(potentialPrime);
					}

					int newCompositeWheelIndex = compositeWheels.size();
					compositeWheels.push_back(potentialPrime);
					compositeWheelNextBlockingValues.push_back(newCompositeWheelBlockingValue);
					compositeWheelBasePrimes.push_back(compositeWheelBasePrime);
					compositeWheelToPrimeWheelIndex.push_back(compositeWheelBasePrimeIndex);
					compositeWheelPrimeMultiplierIndexes.push_back(compositeWheelPrimeStartingIndex);
					compositeWheelPrimeStartingIndexs.push_back(compositeWheelPrimeStartingIndex);
					compositeWheelWillHaveChildren.push_back(true);
					queue.push({ newCompositeWheelBlockingValue, newCompositeWheelIndex });
				}
			}

			int nextPrimeMultiplerIndex = ++compositeWheelPrimeMultiplierIndexes[compositeWheelIndex];
			int nextPrimeToMultiply = primes[nextPrimeMultiplerIndex];
			int nextHitValue = compositeWheel * nextPrimeToMultiply;
			bool thisCompositeWheelWillHaveChildren = nextPrimeToMultiply <= compositeWheelPrimeStartingValue;
			compositeWheelWillHaveChildren[compositeWheelIndex] = thisCompositeWheelWillHaveChildren;
			if (nextHitValue < wheelRepetitionCircumfrance) {
				compositeWheelNextBlockingValues[compositeWheelIndex] = nextHitValue;
			}
			else {
				compositeWheelNextBlockingValues[compositeWheelIndex] = 0;
			}

			if (compositeWheelNextBlockingValues[compositeWheelIndex] > 0) {
				queue.push({ nextHitValue, compositeWheelIndex });
			}

			nextCompositeWheelBlockingValue = compositeWheelNextBlockingValues[queue.size() > 0 ? queue.top().second : 0];
		}
		else {
			//Not a hit, so the number is prime.
			primeIndexes.emplace(potentialPrime, primes.size());
			primes.push_back(potentialPrime);

			if (end / potentialPrime >= potentialPrime) {
				int square = potentialPrime * potentialPrime;
				int primeIndex = primes.size() - 1;
				int lastWheelCircumfrance = wheelRepititionCircumfrances[wheelRepititionCircumfrances.size() - 1];
				int wheelRepititionCircumfrance = lastWheelCircumfrance * potentialPrime;
				if (wheelRepititionCircumfrance > end)
					wheelRepititionCircumfrance = end;

				wheelRepititionCircumfrances.push_back(wheelRepititionCircumfrance);

				if (wheelRepititionCircumfrance >= square) {
					int compositeWheelIndex = compositeWheels.size();
					compositeWheels.push_back(potentialPrime);
					compositeWheelNextBlockingValues.push_back(square);
					compositeWheelBasePrimes.push_back(potentialPrime);
					compositeWheelToPrimeWheelIndex.push_back(primeIndex);
					compositeWheelPrimeMultiplierIndexes.push_back(primeIndex);
					compositeWheelPrimeStartingIndexs.push_back(primeIndex);
					compositeWheelWillHaveChildren.push_back(true);

					queue.push({ square, compositeWheelIndex });

					//Add a new smallerCompositeWheel compositeWheelBasePrime for each smallerCompositeWheel below this where smallerCompositeWheel * potentialPrime < the lower one's wheelRepititionCircumfrance.
					//Not needed until 11 * 13, so stop at the index for 11, which is 4.
					for (int smallerPrimeIndex = primeIndex - 1; smallerPrimeIndex > 3; --smallerPrimeIndex) {
						int smallerWheelRepetitionCircumfrance = wheelRepititionCircumfrances[smallerPrimeIndex];
						int smallerPrime = primes[smallerPrimeIndex];
						int smallerCompositeWheel = smallerPrime * potentialPrime;
						int smallerCompositeWheelFirstHit = smallerCompositeWheel * potentialPrime;
						if (smallerCompositeWheelFirstHit > smallerWheelRepetitionCircumfrance) {
							if (smallerWheelRepetitionCircumfrance < end) {
								break;
							}
							else {
								continue;
							}
						}

						int smallercompositeWheelIndex = compositeWheels.size();
						compositeWheels.push_back(smallerCompositeWheel);
						compositeWheelNextBlockingValues.push_back(smallerCompositeWheelFirstHit);
						compositeWheelBasePrimes.push_back(smallerPrime);
						compositeWheelToPrimeWheelIndex.push_back(smallerPrimeIndex);
						compositeWheelPrimeMultiplierIndexes.push_back(primeIndex);
						compositeWheelPrimeStartingIndexs.push_back(primeIndex);
						compositeWheelWillHaveChildren.push_back(true);

						queue.push({ smallerCompositeWheelFirstHit, smallercompositeWheelIndex });
					}
				}

				stitches.push_back(std::vector<int>{ potentialPrime });
			}
		}

		//Check if the bigWheel has reached the end.  If it has, it needs to be sized up by dropping the current compositeWheelBasePrime and working 
		//	on the repetition circumference of the next compositeWheelBasePrime.
		if (++bigWheelIndex == bigWheelSize) {
			if (printAll) {
				PrintVector(primes, "Primes");
				std::cout << "Big Wheel Index: " << bigWheelIndex << std::endl;
				std::cout << "Dropping compositeWheelBasePrime " << nextWheelToDrop << std::endl;
				PrintVector(bigWheel, "Big Wheel before");
			}

			//Do stitches for current compositeWheelBasePrime before sizing up the big compositeWheelBasePrime.
			int tempNum = 1;
			int tempBigWheelIndex = -1;
			if (printAll) {
				PrintVector(stitches[primesNextWheelIndex], "Stitches");
			}

			for (const auto& stitch : stitches[primesNextWheelIndex]) {//TODO: Make stitches pointers/reference to the bigWheel values instead of using tempNum/tempBigWheelIndex
				//Stitch
				while (tempNum < stitch) {
					int numToAdd = bigWheel[++tempBigWheelIndex];
					tempNum += numToAdd;
				}

				int numToAdd2 = bigWheel[++tempBigWheelIndex];
				tempNum += numToAdd2;
				if (tempBigWheelIndex == 1) {
					tempBigWheelIndex = 0;
					bigWheel[0] += bigWheel[1];
					bigWheel.erase(bigWheel.begin() + 1);
					--bigWheelIndex;
				}
				else {
					//Stitch previous 2
					bigWheel[tempBigWheelIndex - 1] += bigWheel[tempBigWheelIndex];
					bigWheel.erase(bigWheel.begin() + tempBigWheelIndex);
					--tempBigWheelIndex;
					--bigWheelIndex;
				}

				--bigWheelSize;
			}

			if (printAll) {
				PrintVector(bigWheel, "Big Wheel after Stitches");
			}

			//Duplicate the big compositeWheelBasePrime x number of times, where x is the next compositeWheelBasePrime being worked on. 2, 6, 30, 210, 2310, 30030, 510510...
			++primesNextWheelIndex;
			nextWheelToDrop = primes[primesNextWheelIndex];
			std::vector<int> bigWheelCopy = bigWheel;
			for (int i = 0; i < nextWheelToDrop - 1; ++i) {
				bigWheel.insert(bigWheel.end(), bigWheelCopy.begin(), bigWheelCopy.end());
			}

			bigWheelSize = bigWheel.size();//TODO: calculate manually.  bigWheelSize *= nextWheelToDrop;
			bigWheelCircumfrance *= nextWheelToDrop;
			if (printAll) {
				PrintVector(bigWheel, "Big Wheel after size up");
				std::cout << std::endl;
			}
		}

		if (troubleShootingQue || printAll) {
			std::cout << "potentialPrime += bigWheel[" << bigWheelIndex << "]; " << potentialPrime << " += " << bigWheel[bigWheelIndex] << " = " << potentialPrime + bigWheel[bigWheelIndex] << ", nextCompositeWheelBlockingValue: " << nextCompositeWheelBlockingValue << std::endl;
			std::cout << "queValues: ";
			bool first = true;
			for (int i = 0; i < queValuesForTroubleshooting.size(); ++i) {
				if (first) {
					first = false;
				}
				else {
					std::cout << ", ";
				}

				int queValue = queValuesForTroubleshooting[i];
				std::cout << queValue;
				if (queValue == 1331)
					std::cout << "(found 1331 at " << i << ")";
			}

			std::cout << std::endl;
		}

		//Big compositeWheelBasePrime spinning.
		potentialPrime += bigWheel[bigWheelIndex];

	} while (potentialPrime < end);

	auto end_time = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start).count();
}