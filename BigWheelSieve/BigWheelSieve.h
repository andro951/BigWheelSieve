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
		return std::chrono::duration_cast<std::chrono::microseconds>(end_early_time - start).count();
	}

	int potentialPrime = 11;
	int bigWheelIndex = 2;
	int bigWheelSize = 10;
	int primesNextWheelIndex = 2;
	int nextWheelToDrop = 5;
	std::vector<int> bigWheel = { 4, 2, 4, 2, 4, 2, 4, 2, 4, 2 };

	primes = { 2, 3, 5, 7 };
	std::map<int, int> primeIndexes = { { 2, 0 }, { 3, 1 }, { 5, 2 }, {7, 3 } };
	std::vector<int> wheelRepititionCircumfrances = { 2, 6, 30, 210 };//Same size as primes
	std::vector<std::vector<int>> stitches = { {}, {}, { 5 }, { 7 } };//Same size as primes
	//std::vector<std::vector<int*>> stitches = { {}, {}, { &bigWheel[1] }, { &bigWheel[2] }};//Same size as primes.  Points to the 2nd of the 2 values that need to be stitched

	int nextCompositeWheelBlockingValue = 25;
	std::vector<int> compositeWheels = { 5, 7 };
	std::vector<int> compositeWheelNextBlockingValues = { 25, 49 };//Same size as compositeWheels
	std::vector<int> compositeWheelPrimeMultiplierIndexes = { 2, 3 };//Same size as compositeWheels
	std::vector<int> compositeWheelToPrimeWheelIndex = { 2, 3 };//Same size as compositeWheels
	std::vector<int> compositeWheelPrimeStartingIndexs = { 2, 3 };//Same size as compositeWheels
	std::vector<bool> compositeWheelWillHaveChildren = { false, false };//Same size as compositeWheels

	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> compositeWheelIndexQueue;
	compositeWheelIndexQueue.push({ 25, 0 });
	compositeWheelIndexQueue.push({ 49, 1 });

	long stitchTime = 0;
	long stichEraseTime = 0;

	do {
		//Check if the number is the next composite that isn't skipped by the big wheel.
		if (potentialPrime == nextCompositeWheelBlockingValue) {
			//Composite value hit.  Replace the next composite blocking value with the next composite value from the queue.

			//The compositeWheel stays the same while the primeMultiplierIndex increases so that compositeWheel is multiplied by 
			// every prime >= it's primeStartingIndex.
			//Denoted as compositeWheel * (startingPrime) meaning the compositeWheel will be multiplied by the set of all primes 
			// starting with startingPrime while the product is <= repetition circumference.
			//Example 13 * (13) is 13 * set of prime numbers between 13 and 173.
			//13 * (13)
			//13 *  13
			//13 *  17
			//13 *  19
			//13 *  23
			//...
			//13 *  173
			std::pair<int, int> queValue = compositeWheelIndexQueue.top();
			compositeWheelIndexQueue.pop();
			int compositeWheelIndex = queValue.second;
			int compositeWheel = compositeWheels[compositeWheelIndex];
			int compositeWheelPrimeStartingIndex = compositeWheelPrimeStartingIndexs[compositeWheelIndex];
			int compositeWheelBasePrimeIndex = compositeWheelToPrimeWheelIndex[compositeWheelIndex];
			int wheelRepetitionCircumfrance = wheelRepititionCircumfrances[compositeWheelBasePrimeIndex];
			int currentPrimeMultiplerIndex = compositeWheelPrimeMultiplierIndexes[compositeWheelIndex];
			stitches[compositeWheelBasePrimeIndex].push_back(potentialPrime);

			//Create a Larger Child composite wheel if a previous child attempt first hit wasn't above the repetition circumference.
			//compositeWheel * startingPrime has a child, compositeWheel * startingPrime * (startingPrime).
			//Example: 13 * 13 has a child, 13 * 13 * (13).
			//Putting them all together:
			//Repetition Circumference: 30030
			//13 * (13)
			//13 *  13 -> 13 * 13 * (13)
			//            13 * 13 *  13 -> 13 * 13 * 13 * (13)
			//                             13 * 13 * 13 *  13 "no child"
			//                             13 * 13 * 13 *  17 "37,349 too high, stop"
			// 
			//            13 * 13 *  17 -> 13 * 13 * 17 * (17) "37,349 too high, stop having children"
			//            13 * 13 *  19
			//            13 * 13 *  23
			//            ...
			//            13 * 13 *  173
			//            13 * 13 *  179 "30,251 too high, stop"
			// 
			//13 *  17 -> 13 * 17 * (17)
			//            13 * 17 *  17
			//            13 * 17 *  19
			//            13 * 17 *  23
			//            ...
			//            13 * 17 *  131
			//            13 * 17 *  137 "30,277 too high, stop"
			//
			//13 *  19 -> 13 * 19 * (19)
			//            13 * 19 *  19
			//            13 * 19 *  23
			//            13 * 19 *  29
			//            ...
			//            13 * 19 *  113
			//            13 * 19 *  127 "31,369 too high, stop"
			//..........................
			//13 *  47 -> 13 * 47 * (47)
			//            13 * 47 *  47
			//            13 * 47 *  53 "32,383 too high, stop"
			// 
			//13 *  53 -> 13 * 53 * (53) "32,383 too high, stop having children"
			//13 *  59
			//13 *  61
			//...
			//13 *  2309
			//13 *  2311 "30,043 too high, stop"
			if (compositeWheelWillHaveChildren[compositeWheelIndex]) {
				int compositeWheelPrimeStartingValue = primes[currentPrimeMultiplerIndex];
				int max = wheelRepetitionCircumfrance / compositeWheelPrimeStartingValue;
				if (potentialPrime <= max) {
					int newCompositeWheelBlockingValue = potentialPrime * compositeWheelPrimeStartingValue;
					if (wheelRepetitionCircumfrance >= newCompositeWheelBlockingValue) {
						int newCompositeWheelIndex = compositeWheels.size();
						compositeWheels.push_back(potentialPrime);
						compositeWheelNextBlockingValues.push_back(newCompositeWheelBlockingValue);
						compositeWheelToPrimeWheelIndex.push_back(compositeWheelBasePrimeIndex);
						compositeWheelPrimeMultiplierIndexes.push_back(currentPrimeMultiplerIndex);
						compositeWheelPrimeStartingIndexs.push_back(currentPrimeMultiplerIndex);
						compositeWheelWillHaveChildren.push_back(true);
						compositeWheelIndexQueue.push({ newCompositeWheelBlockingValue, newCompositeWheelIndex });
					}
					else {
						compositeWheelWillHaveChildren[compositeWheelIndex] = false;
					}
				}
			}

			//Check the next composite hit for the composite wheel that was just hit.
			int nextPrimeMultiplerIndex = ++compositeWheelPrimeMultiplierIndexes[compositeWheelIndex];
			int nextPrimeToMultiply = primes[nextPrimeMultiplerIndex];

			//compositeWheelWillHaveChildren[compositeWheelIndex] = thisCompositeWheelWillHaveChildren;
			int max = wheelRepetitionCircumfrance / compositeWheel;
			if (nextPrimeToMultiply <= max) {
				int nextHitValue = compositeWheel * nextPrimeToMultiply;
				compositeWheelNextBlockingValues[compositeWheelIndex] = nextHitValue;
				compositeWheelIndexQueue.push({ nextHitValue, compositeWheelIndex });
			}
				

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
				int wheelRepititionCircumfrance;
				if (lastWheelCircumfrance >= end) {
					wheelRepititionCircumfrance = end;
				}
				else {
					int max = end / lastWheelCircumfrance;
					if (potentialPrime > max) {
						wheelRepititionCircumfrance = end;
					}
					else {
						wheelRepititionCircumfrance = lastWheelCircumfrance * potentialPrime;
						if (wheelRepititionCircumfrance > end)
							wheelRepititionCircumfrance = end;
					}
				}

				//Cap wheelRepititionCircumfrance at end to prevent overflowing.
				wheelRepititionCircumfrances.push_back(wheelRepititionCircumfrance);

				//Add one or more composite wheels based off the current prime.
				if (wheelRepititionCircumfrance >= square) {
					//Create one composite wheel for the prime itself with a first hit value of potentialPrime^2.
					int compositeWheelIndex = compositeWheels.size();
					compositeWheels.push_back(potentialPrime);
					compositeWheelNextBlockingValues.push_back(square);
					compositeWheelToPrimeWheelIndex.push_back(primeIndex);
					compositeWheelPrimeMultiplierIndexes.push_back(primeIndex);
					compositeWheelPrimeStartingIndexs.push_back(primeIndex);
					compositeWheelWillHaveChildren.push_back(true);
					compositeWheelIndexQueue.push({ square, compositeWheelIndex });
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

			auto startStitches = std::chrono::high_resolution_clock::now();
			//for (const auto& stitch : stitches[primesNextWheelIndex]) {//TODO: Make stitches pointers/reference to the bigWheel values instead of using tempNum/tempBigWheelIndex
			std::vector<int>& stitchesForCurrentWheel = stitches[primesNextWheelIndex];
			for (int i = 0; i < stitchesForCurrentWheel.size(); ++i) {
				//Stitch by combining the value at the index of the stitch with the value at the previous index.
				while (tempNum < stitchesForCurrentWheel[i]) {
					int numToAdd = bigWheel[++tempBigWheelIndex];
					tempNum += numToAdd;
				}

				int numToAdd2 = bigWheel[++tempBigWheelIndex];
				tempNum += numToAdd2;
				bigWheel[tempBigWheelIndex - 1] += bigWheel[tempBigWheelIndex];
				auto startErase = std::chrono::high_resolution_clock::now();
				bigWheel.erase(bigWheel.begin() + tempBigWheelIndex);
				auto endErase = std::chrono::high_resolution_clock::now();
				stichEraseTime += std::chrono::duration_cast<std::chrono::microseconds>(endErase - startErase).count();
				--tempBigWheelIndex;
				--bigWheelIndex;
				--bigWheelSize;
			}

			auto endStitches = std::chrono::high_resolution_clock::now();
			stitchTime += std::chrono::duration_cast<std::chrono::microseconds>(endStitches - startStitches).count();

			//Duplicate the big wheel x number of times, where x is the next prime wheel being worked on. 2, 6, 30, 210, 2310, 30030, 510510...
			++primesNextWheelIndex;
			nextWheelToDrop = primes[primesNextWheelIndex];
			std::vector<int> bigWheelCopy = bigWheel;
			for (int i = 0; i < nextWheelToDrop - 1; ++i) {
				bigWheel.insert(bigWheel.end(), bigWheelCopy.begin(), bigWheelCopy.end());
			}

			bigWheelSize = bigWheel.size();//TODO: calculate manually.
		}

		//Spin the big wheel to get the next potential prime.
		potentialPrime += bigWheel[bigWheelIndex];

	} while (potentialPrime < end);

	std::cout << "Stitch time: " << stitchTime << " microseconds" << std::endl;
	std::cout << "Stitch erase time: " << stichEraseTime << " microseconds" << std::endl;

	auto end_time = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(end_time - start).count();
}