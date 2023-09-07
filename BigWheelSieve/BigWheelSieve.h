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
/*
#pragma once

#include <vector>
#include <chrono>
#include <map>
#include <queue>
#include "SharedConstants.h"
#include "SimpleSieve.h"
#include "Logging.h"
#include "PrimeFactorization.h"

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
	int bigWheelIndex = 2;
	int bigWheelSize = 10;
	int primesNextWheelIndex = 2;
	int nextWheelToDrop = 5;
	std::vector<int> bigWheel = { 4, 2, 4, 2, 4, 2, 4, 2, 4, 2 };

	primes = { 2, 3, 5, 7 };
	std::map<int, int> primeIndexes = { { 2, 0 }, { 3, 1 }, { 5, 2 }, {7, 3 } };
	std::vector<int> wheelRepititionCircumfrances = { 2, 6, 30, 210 };//Same size as primes
	std::vector<std::vector<int>> stitches = { {}, {}, { 5 }, { 7 } };//Same size as primes

	int nextCompositeWheelBlockingValue = 25;
	std::vector<int> compositeWheels = { 5, 7 };
	std::vector<int> compositeWheelNextBlockingValues = { 25, 49 };//Same size as compositeWheels
	std::vector<int> compositeWheelBasePrimes = { 5, 7 };//Same size as compositeWheels//Could be calculated from primes[compositeWheelToPrimeWheelIndex[compositeWheelIndex]] instead
	std::vector<int> compositeWheelPrimeMultiplierIndexes = { 2, 3 };//Same size as compositeWheels
	std::vector<int> compositeWheelToPrimeWheelIndex = { 2, 3 };//Same size as compositeWheels
	std::vector<int> compositeWheelPrimeStartingIndexs = { 2, 3 };//Same size as compositeWheels
	std::vector<bool> compositeWheelWillHaveChildren = { false, false };//Same size as compositeWheels

	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> compositeWheelIndexQueue;
	compositeWheelIndexQueue.push({ 25, 0 });
	compositeWheelIndexQueue.push({ 49, 1 });

	do {
		//Check if the number is the next non-prime that is not taken care of by the big compositeWheelBasePrime.
		if (potentialPrime == nextCompositeWheelBlockingValue) {
			//not prime
			std::pair<int, int> queValue = compositeWheelIndexQueue.top();
			compositeWheelIndexQueue.pop();
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
					compositeWheelIndexQueue.push({ newCompositeWheelBlockingValue, newCompositeWheelIndex });
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
				compositeWheelIndexQueue.push({ nextHitValue, compositeWheelIndex });
			}

			nextCompositeWheelBlockingValue = compositeWheelNextBlockingValues[compositeWheelIndexQueue.size() > 0 ? compositeWheelIndexQueue.top().second : 0];
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

					compositeWheelIndexQueue.push({ square, compositeWheelIndex });

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

						compositeWheelIndexQueue.push({ smallerCompositeWheelFirstHit, smallercompositeWheelIndex });
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
*/

/*
Author - Isaac Richards
Date - 04SEP23
Description -
	There are 2 major parts of this sieve that work together, a large wheel that continuously gets larger and a queue for composite checks.
	The goal is to create 1 large wheel (a vector of integers) made up of small numbers that are used to get prime numbers.
		The values are added to the current prime to get the next prime. Example: Big wheel value of 4 when current prime is 13, 13 + 4 = 17.
	
	Each prime (lowest to highest) is treated as a wheel.  The lowest wheel imprints onto the big wheel to prevent all future multiples 
		of the wheel's size from occurring every again.  Example: after the 5 wheel is finished, no multiples of 5 will ever need to be 
		checked again.  It does this by going over the previous wheel's pattern multiple times so that all combinations of itself and 
		the primes before it are covered.

	This repeating patter is extremely important because it has a flaw, it becomes way too large very quickly, and only the smallest 
		wheel can imprint onto the big wheel or the repetition pattern would be ruined.

	Each time a wheel's repetition pattern is completed, the size of the big wheel is multiplied by the next wheel's size.
		Start
		Wheel being checked: 1, Big Wheel Circumference: 1
		Wheel being checked: 2, Big Wheel Circumference: 2 (x2)
		Wheel being checked: 3, Big Wheel Circumference: 6 (x3)
		Wheel being checked: 5, Big Wheel Circumference: 30 (x5)
		Wheel being checked: 7, Big Wheel Circumference: 210 (x7)
		Wheel being checked: 11, Big Wheel Circumference: 2310 (x11)
		Wheel being checked: 13, Big Wheel Circumference: 30030 (x13)
		Wheel being checked: 17, Big Wheel Circumference: 510510 (x17)
		...
	(Note: The current version of the sieve starts with 5 as the lowest wheel to prevent differences for the 1, 2 and 3 wheels)
	To prevent overflow, the repetition circumferences are reduced to the "end" value if they are above it to prevent overflow.
		The repetition circumferences never need to be larger than the end value because the sieve will never need to check past end.

	The first time a prime number is hit, it creates a "stitch". Stitches are points in their repetition where a prime number will 
		never occur again, so the big wheel at that index and the previous index need to be combined together or "stitched".

	For example, After the 2 wheel is dropped, the big wheel is [2, 2, 2].  Counting starts at one, then the big wheel values are applied.
	So, 1, 3, 5, 7, 9, 11, 13, 15, 17... are numbers the big wheel currently thinks are primes because of the [2, 2, 2] pattern.
	The first number that is not a prime is 9.  It is not a coincidence that 9 is 3 * 3 and 3 has not completed it's repetition circumference.
	The 3 wheel is dropped after checking the next prime number. which causes it's stitches to be applied to the big wheel,
		then the big wheel is duplicated 5 times because it's now time to calculate 5's repetition circumference.

	The big wheel goes from [2, 2, 2] to [4, 2] by applying the stitch from 3 being hit, then is duplicated 5 times.
	From [4, 2] to [4, 2, 4, 2, 4, 2, 4, 2, 4, 2] which is [4, 2] * 5.
	Counting starts at 1, then add each number in the big wheel to see what the next prime is.
	[2, 2, 2] is 1 + 2 + 2 + 2 or 1 + 2 = 3 + 2 = 5 + 2 = 7.  Because the repetition of the pattern 3 makes has a circumference of 6,
		there will be a NON-PRIME number that is divisible by 3, but no previous primes every 6 numbers.
		3 needs to stitch this spot to prevent non-primes from being hit by the big wheel.
		This is what the pattern looks like before 3 applies it's stitch:
		1, 3(div by 3 and no others below 3), 5, 7, 9(div by 3 and no others below 3), 11, 13, 15(div by 3 and no others below 3)
		The non-primes are occurring every 6 numbers after 3: 3, 9, 15, 21, 27...
		So the stitch is applied to the first and second number in the big wheel so [2, 2, 2] becomes [4, 2] which will make the sequence 
		look like: 1, 5, 7, 11, 13, 17, 19, 23, 25, 29, 31...
		This looks a lot better, but the 5 wheel has a problem.
		Notice with 3, it's repetition circumference is 6, but 5's is 30.
		The first multiple of 3 that is not a multiple of primes below it is 9, which is less than it's repetition circumference.
		However with 5, it's repetition circumference is 30, but the first multiple of 5 that is not a multiple of primes below it is 25.
		25 is less than 30, so it's not taken care of by the stitch created when 5 is hit.
		Another stitch is needed to prevent 25's from being hit by the big wheel in the future.
		I say 25's because the patter will repeat in the form of 30n + 25:
		  [6,  4,  2,  4,  2,  4,  2,  4,  2] [6,  4,  2,  4,  2,  4,  2,  4,  2] [6,  4,  2,  4,  2,  4,  2,  4,  2]
		1, 7, 11, 13, 17, 19, 23, 25, 29, 31, 37, 41, 43, 47, 49, 53, 55, 59, 61, 67, 71, 73, 77, 79, 83, 85, 89, 91...
		Notice the 25, 55 and 85 which are all 30n + 25.
		this means that the 5 pattern needs to be stitches where 25 would occur:
		  [6,  4,  2,  4,  2,  4,  6,  2] [6,  4,  2,  4,  2,  4,  6,  2] [6,  4,  2,  4,  2,  4,  6,  2]
		1, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 49, 53, 59, 61, 67, 71, 73, 77, 79, 83, 89, 91...
		Great, now the 5 wheel's repetition circumference is taken care of.
		However, as I mentioned before, the repetition circumference grows significantly as the wheels get larger.
		The 7 repetition (size 210) needs to deal with:
			7 * 7 (49), 7 * 11 (77), 7 * 13 (91), 7 * 17 (119), 7 * 19 (133), 7 * 23 (161), 7 * 29 (203)

		Ok, so we need to multiply the 7 wheel by every prime until the product is larger than it's circumference.  Not too bad.
		What about 11?
		The 11 repetition (size 2310) needs to deal with 11 * 11 (121), 11 * 13 (143), 11 * 17 (187), 11 * 19 (209), 11 * 23 (253),
			11 * 29 (319), 11 * 31 (341), 11 * 37 (407), 11 * 41 (451), 11 * 43 (473), 11 * 47 (517), 11 * 53 (583), 11 * 59 (649),
			11 * 61 (671), 11 * 67 (737), 11 * 71 (781), 11 * 73 (803), 11 * 77 (847), 11 * 79 (869), 11 * 83 (913), 11 * 89 (979),
			11 * 91 (1001), 11 * 97 (1067), 11 * 101 (1111), 11 * 103 (1133), 11 * 107 (1177), 11 * 109 (1199), 11 * 113 (1243),
			11 * 119 (1309), 11 * 121 (1331)....... oh no.... That's 11^3.....oh boy.

		So... We need to multiply the current wheel by every combination of primes higher than it where the result is less than it's 
		repetition circumference.
		...Awesome....super nice....exactly what I wanted...
		This is not limited to prime^n.  It is every combination of primes where the result is less than the repetition circumference.
			Such as 11 * 13 * 13 or 17 * 17 * 19 * 43

		In it's current state, this program is accurate when end is <= 100000.  After that, sets of 4 prime multiples and more are missed.
			Factors of 104329: 17, 17, 19, 19
			Factors of 126293: 17, 17, 19, 23
			Factors of 152881: 17, 17, 23, 23
			Factors of 159239: 17, 17, 19, 29
			Factors of 170221: 17, 17, 19, 31
			Factors of 170867: 17, 19, 23, 23
		Those are the first few that are missed.
		More work is needed with the children of composite wheels to handles these, but good enough to share for now.
*/

#pragma once

#include <vector>
#include <chrono>
#include <map>
#include <queue>
#include "SharedConstants.h"
#include "SimpleSieve.h"
#include "Logging.h"
#include "PrimeFactorization.h"

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
	int bigWheelIndex = 2;
	int bigWheelSize = 10;
	int primesNextWheelIndex = 2;
	int nextWheelToDrop = 5;
	std::vector<int> bigWheel = { 4, 2, 4, 2, 4, 2, 4, 2, 4, 2 };

	primes = { 2, 3, 5, 7 };
	std::map<int, int> primeIndexes = { { 2, 0 }, { 3, 1 }, { 5, 2 }, {7, 3 } };
	std::vector<int> wheelRepititionCircumfrances = { 2, 6, 30, 210 };//Same size as primes
	std::vector<std::vector<int>> stitches = { {}, {}, { 5 }, { 7 } };//Same size as primes

	int nextCompositeWheelBlockingValue = 25;
	std::vector<int> compositeWheels = { 5, 7 };
	std::vector<int> compositeWheelNextBlockingValues = { 25, 49 };//Same size as compositeWheels
	std::vector<int> compositeWheelBasePrimes = { 5, 7 };//Same size as compositeWheels//Could be calculated from primes[compositeWheelToPrimeWheelIndex[compositeWheelIndex]] instead
	std::vector<int> compositeWheelPrimeMultiplierIndexes = { 2, 3 };//Same size as compositeWheels
	std::vector<int> compositeWheelToPrimeWheelIndex = { 2, 3 };//Same size as compositeWheels
	std::vector<int> compositeWheelPrimeStartingIndexs = { 2, 3 };//Same size as compositeWheels
	std::vector<bool> compositeWheelWillHaveChildren = { false, false };//Same size as compositeWheels

	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> compositeWheelIndexQueue;
	compositeWheelIndexQueue.push({ 25, 0 });
	compositeWheelIndexQueue.push({ 49, 1 });

	do {
		//Check if the number is the next composite that isn't skipped by the big wheel.
		if (potentialPrime == nextCompositeWheelBlockingValue) {
			//Composite value hit.  Replace the next composite blocking value with the next composite value from the queue.
			std::pair<int, int> queValue = compositeWheelIndexQueue.top();
			compositeWheelIndexQueue.pop();
			int compositeWheelIndex = queValue.second;
			int compositeWheel = compositeWheels[compositeWheelIndex];
			int compositeWheelPrimeStartingIndex = compositeWheelPrimeStartingIndexs[compositeWheelIndex];
			int compositeWheelPrimeStartingValue = primes[compositeWheelPrimeStartingIndex];
			int compositeWheelBasePrimeIndex = compositeWheelToPrimeWheelIndex[compositeWheelIndex];
			int wheelRepetitionCircumfrance = wheelRepititionCircumfrances[compositeWheelBasePrimeIndex];
			stitches[compositeWheelBasePrimeIndex].push_back(potentialPrime);

			//Create a Larger Child composite wheel if allowed.
			//Example: potentialPrime = 143, startingPrime = 13, create a child that will have hits of 143 * 13, 143 * 17, 143 * 19...
			if (compositeWheelWillHaveChildren[compositeWheelIndex]) {
				int newCompositeWheelBlockingValue = potentialPrime * compositeWheelPrimeStartingValue;
				if (wheelRepetitionCircumfrance >= newCompositeWheelBlockingValue) {
					int newCompositeWheelIndex = compositeWheels.size();
					compositeWheels.push_back(potentialPrime);
					compositeWheelNextBlockingValues.push_back(newCompositeWheelBlockingValue);
					compositeWheelBasePrimes.push_back(compositeWheelBasePrimes[compositeWheelIndex]);
					compositeWheelToPrimeWheelIndex.push_back(compositeWheelBasePrimeIndex);
					compositeWheelPrimeMultiplierIndexes.push_back(compositeWheelPrimeStartingIndex);
					compositeWheelPrimeStartingIndexs.push_back(compositeWheelPrimeStartingIndex);
					compositeWheelWillHaveChildren.push_back(true);
					compositeWheelIndexQueue.push({ newCompositeWheelBlockingValue, newCompositeWheelIndex });
				}
			}

			//Check the next composite hit for the composite wheel that was just hit.
			int nextPrimeMultiplerIndex = ++compositeWheelPrimeMultiplierIndexes[compositeWheelIndex];
			int nextPrimeToMultiply = primes[nextPrimeMultiplerIndex];
			int nextHitValue = compositeWheel * nextPrimeToMultiply;

			//Composite wheels create children each time they hit a composite value if the multiple doesn't exceed the 
			//	compositeWheelPrimeStartingIndex.
			//Example: potentialPrime = 121, basePrime = 11, multiple = 11, startingPrime = 11 Fine because multiple <= startingPrime.
			//Example: potentialPrime = 121, basePrime = 11, multiple = 13, startingPrime = 11 not fine, stop having children.
			bool thisCompositeWheelWillHaveChildren = nextPrimeToMultiply <= compositeWheelPrimeStartingValue;
			compositeWheelWillHaveChildren[compositeWheelIndex] = thisCompositeWheelWillHaveChildren;
			if (nextHitValue < wheelRepetitionCircumfrance) {
				compositeWheelNextBlockingValues[compositeWheelIndex] = nextHitValue;
			}
			else {
				compositeWheelNextBlockingValues[compositeWheelIndex] = 0;
			}

			//Stop using the composite wheel if it has no more hits in the desired range/repetition circumference.
			if (compositeWheelNextBlockingValues[compositeWheelIndex] > 0)
				compositeWheelIndexQueue.push({ nextHitValue, compositeWheelIndex });

			nextCompositeWheelBlockingValue = compositeWheelNextBlockingValues[compositeWheelIndexQueue.size() > 0 ? compositeWheelIndexQueue.top().second : 0];
		}
		else {
			//Not a composite hit, so the number is prime.
			primeIndexes.emplace(potentialPrime, primes.size());
			primes.push_back(potentialPrime);

			//Check if the first important blocking value, potentialPrime^2 is in the desired range.
			if (end / potentialPrime >= potentialPrime) {//Same as if (potentialPrime * potentialPrime < end), but prevents overflow.
				//Prime data
				int square = potentialPrime * potentialPrime;
				int primeIndex = primes.size() - 1;
				int lastWheelCircumfrance = wheelRepititionCircumfrances[wheelRepititionCircumfrances.size() - 1];
				int wheelRepititionCircumfrance = lastWheelCircumfrance * potentialPrime;
				//Cap wheelRepititionCircumfrance at end to prevent overflowing.
				wheelRepititionCircumfrances.push_back(wheelRepititionCircumfrance > end ? end : wheelRepititionCircumfrance);

				//Add one or more composite wheels based off the current prime.
				if (wheelRepititionCircumfrance >= square) {
					//Create one composite wheel for the prime itself with a first hit value of potentialPrime^2.
					int compositeWheelIndex = compositeWheels.size();
					compositeWheels.push_back(potentialPrime);
					compositeWheelNextBlockingValues.push_back(square);
					compositeWheelBasePrimes.push_back(potentialPrime);
					compositeWheelToPrimeWheelIndex.push_back(primeIndex);
					compositeWheelPrimeMultiplierIndexes.push_back(primeIndex);
					compositeWheelPrimeStartingIndexs.push_back(primeIndex);
					compositeWheelWillHaveChildren.push_back(true);
					compositeWheelIndexQueue.push({ square, compositeWheelIndex });

					//Add a new smaller composite wheel for each prime below this where smallerPrime * potentialPrime is less than
					//  the smaller prime's wheelRepititionCircumfrance.
					//Not needed below 11 * 13, so stop after the index for 11, which is 4.
					for (int smallerPrimeIndex = primeIndex - 1; smallerPrimeIndex > 3; --smallerPrimeIndex) {
						int smallerWheelRepetitionCircumfrance = wheelRepititionCircumfrances[smallerPrimeIndex];
						int smallerPrime = primes[smallerPrimeIndex];
						int smallerCompositeWheel = smallerPrime * potentialPrime;
						int smallerCompositeWheelFirstHit = smallerCompositeWheel * potentialPrime;

						//Check if the smaller composite wheel's first hit is in the desired range.
						if (smallerCompositeWheelFirstHit > smallerWheelRepetitionCircumfrance) {
							//smallerWheelRepetitionCircumfrance values are capped at end, so need to continue checking all of them 
							//	that are equal to end and only continue if they are equal.
							if (smallerWheelRepetitionCircumfrance < end) {
								//If out of repetition circumference, and repetition circumference is less than end, stop checking 
								//	smaller primes.
								break;
							}
							else {
								continue;
							}
						}

						//Create the smaller composite wheel with a first hit value of smallerPrime * potentialPrime^2.
						int smallercompositeWheelIndex = compositeWheels.size();
						compositeWheels.push_back(smallerCompositeWheel);
						compositeWheelNextBlockingValues.push_back(smallerCompositeWheelFirstHit);
						compositeWheelBasePrimes.push_back(smallerPrime);
						compositeWheelToPrimeWheelIndex.push_back(smallerPrimeIndex);
						compositeWheelPrimeMultiplierIndexes.push_back(primeIndex);
						compositeWheelPrimeStartingIndexs.push_back(primeIndex);
						compositeWheelWillHaveChildren.push_back(true);
						compositeWheelIndexQueue.push({ smallerCompositeWheelFirstHit, smallercompositeWheelIndex });
					}
				}

				//Add the first stitch for the prime value being hit.
				stitches.push_back(std::vector<int>{ potentialPrime });
			}
		}

		//Check if the bigWheel has reached the end of it's current size.
		//If it has, it needs to be sized up by dropping the current wheel and working 
		//	on the repetition circumference of the next prime wheel.
		if (++bigWheelIndex == bigWheelSize) {

			//Do stitches for current wheel before sizing up the big wheel.
			int tempNum = 1;
			int tempBigWheelIndex = -1;
			for (const auto& stitch : stitches[primesNextWheelIndex]) {//TODO: Make stitches pointers/reference to the bigWheel values instead of using tempNum/tempBigWheelIndex
				//Stitch by combining the value at the index of the stitch with the value at the previous index.
				while (tempNum < stitch) {
					int numToAdd = bigWheel[++tempBigWheelIndex];
					tempNum += numToAdd;
				}

				int numToAdd2 = bigWheel[++tempBigWheelIndex];
				tempNum += numToAdd2;
				bigWheel[tempBigWheelIndex - 1] += bigWheel[tempBigWheelIndex];
				bigWheel.erase(bigWheel.begin() + tempBigWheelIndex);
				--tempBigWheelIndex;
				--bigWheelIndex;
				--bigWheelSize;
			}

			//Duplicate the big wheel x number of times, where x is the next prime wheel being worked on. 2, 6, 30, 210, 2310, 30030, 510510...
			++primesNextWheelIndex;
			nextWheelToDrop = primes[primesNextWheelIndex];
			std::vector<int> bigWheelCopy = bigWheel;
			for (int i = 0; i < nextWheelToDrop - 1; ++i) {
				bigWheel.insert(bigWheel.end(), bigWheelCopy.begin(), bigWheelCopy.end());
			}

			bigWheelSize = bigWheel.size();//TODO: calculate manually.
			bigWheelCircumfrance *= nextWheelToDrop;
		}

		//Spin the big wheel to get the next potential prime.
		potentialPrime += bigWheel[bigWheelIndex];

	} while (potentialPrime < end);

	auto end_time = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start).count();
}