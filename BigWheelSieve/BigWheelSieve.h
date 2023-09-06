/*

Description - Prime sieve that aims to create 1 large wheel that is an array of numbers which represent the amount that has to be added 
	to the current prime number to get the next prime number.
	Each prime starting with 2 is treated as a wheel.  It is "spun" to check which values it prevents until the patter it creates repeats.
	This repeating patter is extremely important because it has a flaw.  
	The size of this repetition pattern (which I refer to as wheel repetition circumference) becomes very large very quickly.
	Each time a wheel's repetition pattern is completed, the size of the big wheel is multiplied by the next wheel's size.
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
	The first time a prime number is hit, it creates a "stitch".
	Stitches are points in their repetition where prime number will never occur again, so the big wheel at that index and the 
		previous index need to be combined together or "stitched".
	For example, After the 2 wheel is dropped, the big wheel is [2, 2, 2] which started at 1, so 1, 3, 5, 7, 9, 11, 13, 15, 17... are numbers the big wheel currently thinks are primes.
	The first number that is not a prime is 9.  It is not a coincidence that 9 is 3 * 3 which has not completed it's repetition circumference.
	The 3 wheel is dropped after checking the next prime number. which causes it's stitches to be applied to the big wheel, 
		then the big wheel is duplicated 5 times because it's now time to calculate 5's repetition circumference.
	The big wheel goes from [2, 2, 2] to [4, 2] from applying the stitch from 3 being hit, then is duplicated 5 times.
	From [4, 2] to [4, 2, 4, 2, 4, 2, 4, 2, 4, 2] which is [4, 2] * 5.
	Counting starts at 1, then add each number in the big wheel to see what the next prime is.
	[2, 2, 2] is 1 + 2 + 2 + 2 or 1 + 2 = 3 + 2 = 5 + 2 = 7.  Because the repetition of the pattern 3 makes has a circumference of 6, 
		there will be a NON-PRIME number that is divisible by 3, but no previous primes every 6 numbers.  
		3 needs to stitch this spot to prevent non-primes from being hit by the big wheel.:
		1, 3(div by 3 and no others below 3), 5, 7, 9(div by 3 and no others below 3), 11, 13, 15(div by 3 and no others below 3)
		So the stitch is applied to the first and second number in the big wheel so [2, 2, 2] becomes [4, 2] which will make the sequence look like:
		1, 5, 7, 11, 13, 17, 19, 23, 25, 29, 31...
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
		The 7 repetition (size 210) needs to deal with 7 * 7 (49), 7 * 11 (77), 7 * 13 (91), 7 * 17 (119), 7 * 19 (133), 7 * 23 (161), 7 * 29 (203)
		Ok, so we need to multiply the 7 wheel by every prime until the product is larger than it's circumference.  Not too bad.
		What about 11?
		The 11 repetition (size 2310) needs to deal with 11 * 11 (121), 11 * 13 (143), 11 * 17 (187), 11 * 19 (209), 11 * 23 (253), 
			11 * 29 (319), 11 * 31 (341), 11 * 37 (407), 11 * 41 (451), 11 * 43 (473), 11 * 47 (517), 11 * 53 (583), 11 * 59 (649), 
			11 * 61 (671), 11 * 67 (737), 11 * 71 (781), 11 * 73 (803), 11 * 77 (847), 11 * 79 (869), 11 * 83 (913), 11 * 89 (979), 
			11 * 91 (1001), 11 * 97 (1067), 11 * 101 (1111), 11 * 103 (1133), 11 * 107 (1177), 11 * 109 (1199), 11 * 113 (1243),
			11 * 119 (1309), 11 * 121 (1331)....... oh no.... That's 11^3.....oh boy.

		So... We need to multiply the current wheel by every combination of primes higher than it where the result is less than it's repetition circumference.
		...Awesome....super nice....exactly what I wanted...

		
*/

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

//wheelsNextValues need to be made up of the products of all primes that haven't been dropped yet.
//It looks like it is only multiples of primes and primes and pseudo primes.
//Calculate the remainder of the wheels blocking values when their square is hit.
//	This guarantees that the values needed in the range of their repetition circumference have been populated

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
	//std::vector<int> wheelsNextPseudoPrime = { 1 };//Same size as primes //Not needed?

	//std::vector<int> wheelValuePrimeMultiplierIndexs = { 0 };//Same size as primes
	//std::vector<int> wheelsNextValues = { 1 };//Same size as primes
	std::vector<long long> wheelRepititionCircumfrances = { 1 };//Same size as primes
	//std::vector<std::vector<int>> wheelBlockingValues = { { 1 } };//Same size as primes.  Starts with just square then is populated in full up to the repetition circumference (or end) when the square is hit. 
	//std::vector<int> wheelBlockingValues2 = { 1 };//Same size as primes.  Starts with just square.  Replaced by the next number each time it's hit.
	//std::vector<int> wheelBlockingBases = { 1 };//Same size as primes.  Starts with wheel size.  Replaced by wheelSize^n as needed.
	//std::vector<int> wheelBlockingPrimeIndexex = { 0 };//Same size as primes.  Starts with PrimeIndex.  Goes up by 1 each blocking value hit.
	//Does need to use pseudo primes in the blocking value calculations and needs to have them in their own vector so they are separate from primes.

	std::vector<int> pseudoPrimes = { };
	std::vector<int> pseudoPrimeBlockingValues = { };//Same size as pseudoPrimes
	std::vector<int> pseudoPrimeWheelBaseValue = { };//Same size as pseudoPrimes
	std::vector<int> pseudoPrime_PrimeMultiplierIndexes = { };//Same size as pseudoPrimes
	std::vector<int> pseduoPrimeToWheelIndex = { };//Same size as pseudoPrimes
	std::vector<bool> pseduoPrimeFirstHit = { };//Same size as pseudoPrimes

	std::vector<int> bigWheel = { 1 };

	std::vector<int> que = { };//Continuously grows.  TODO: replace with a queue.
	std::map<int, std::vector<int>> stitches = { };//TOSO: replace with a vector.  Can get index instead of doing keys.
	int queIndex = 0;

	int nextPseudoPrimeBlockingValue = 1;
	bool printAll = false;

	while (num < end) {
		if (printAll) {
			std::cout << "num += bigWheel[" << bigWheelIndex << "]; " << num << " += " << bigWheel[bigWheelIndex] << " = " << num + bigWheel[bigWheelIndex] << std::endl;
		}
		//Big wheel spinning.  num is the next potential prime.
		num += bigWheel[bigWheelIndex];

		//Check if the number is the next non-prime that is not taken care of by the wheels.
		if (num == nextPseudoPrimeBlockingValue) {
			//TODO: This never triggers currently.
			//not prime
			//int wheelIndex = que[queIndex];
			//int wheel = primes[wheelIndex];
			int pseudoPrimeIndex = que[queIndex];
			int pseudoPrime = pseudoPrimes[pseudoPrimeIndex];
			int wheel = pseudoPrimeWheelBaseValue[pseudoPrimeIndex];
			int wheelIndex = pseduoPrimeToWheelIndex[pseudoPrimeIndex];
			int wheelRepetitionCircumfrance = wheelRepititionCircumfrances[wheelIndex];
			stitches[wheel].push_back(num);
			if (false && pseduoPrimeFirstHit[pseudoPrimeIndex]) {
				pseduoPrimeFirstHit[pseudoPrimeIndex] = false;

				//num is the pseudo prime.
				int newPseudoPrimeBlockingValue = num * pseudoPrime;
				if (wheelRepetitionCircumfrance > newPseudoPrimeBlockingValue) {
					int newPseudoPrimeIndex = pseudoPrimes.size();
					pseudoPrimes.push_back(num);
					pseudoPrimeBlockingValues.push_back(newPseudoPrimeBlockingValue);
					pseudoPrimeWheelBaseValue.push_back(wheel);
					pseduoPrimeToWheelIndex.push_back(wheelIndex);
					pseudoPrime_PrimeMultiplierIndexes.push_back(wheelIndex);//TODO: replace size() with an integer that tracks size instead.
					pseduoPrimeFirstHit.push_back(true);
					
					int highestValueInQue = que.size() > 1 ? pseudoPrimeBlockingValues[que[que.size() - 1]] : 0;
					if (highestValueInQue > newPseudoPrimeBlockingValue) {
						int i = queIndex + 1;
						int queNextIndex = que[i];
						int currentQueBlockingValue = pseudoPrimeBlockingValues[queNextIndex];
						//int currentQueBlockingValue = wheelBlockingValues[queNextIndex][wheelValuePrimeMultiplierIndexs[queNextIndex]];
						while (currentQueBlockingValue < newPseudoPrimeBlockingValue) {
							queNextIndex = que[++i];
							//currentQueBlockingValue = wheelBlockingValues[queNextIndex][wheelValuePrimeMultiplierIndexs[queNextIndex]];
							currentQueBlockingValue = pseudoPrimeBlockingValues[queNextIndex];
						}

						que.insert(que.begin() + i, pseudoPrimeIndex);
					}
					else {
						que.push_back(pseudoPrimeIndex);
					}
				}
			}
			//if (wheelsNextPseudoPrime[wheelIndex] == num) {
			//	pseudoPrimes.push_back(num);
			//	wheelsNextPseudoPrime[wheelIndex] = num * wheel;
			//}

			//If first hit.
			/*if (wheelValuePrimeMultiplierIndexs[wheelIndex] == 0) {
				//Calculate the remainder of the wheels blocking values when their square is hit.
				//	This guarantees that the values needed in the range of their repetition circumference have been populated
				bool finished = false;
				int wheelRepetitionCircumfrance = wheelRepititionCircumfrances[wheelIndex];
				int base = wheel;
				int nextPrimeMultiplerIndex = wheelIndex + 1;
				int nextHitValue = base * primes[nextPrimeMultiplerIndex];
				while (nextHitValue < wheelRepetitionCircumfrance && nextHitValue < end) {
					do {
						wheelBlockingValues[wheelIndex].push_back(nextHitValue);
						nextHitValue = primes[++nextPrimeMultiplerIndex] * base;
					}
					while (nextHitValue < wheelRepetitionCircumfrance && nextHitValue < end);

					base *= wheel;
					nextHitValue = base * wheel;
					nextPrimeMultiplerIndex = wheelIndex;
				}
			}*///Close, but the repetition circumference gets too high for the current primes found.  Have to do 1 at a time instead.
			int nextPrimeMultiplerIndex = ++pseudoPrime_PrimeMultiplierIndexes[pseudoPrimeIndex];
			int nextHitValue = pseudoPrime * primes[nextPrimeMultiplerIndex];
			if (nextHitValue < wheelRepetitionCircumfrance) {//&& nextHitValue < end) {
				pseudoPrimeBlockingValues[pseudoPrimeIndex] = nextHitValue;
			}
			else {
				pseudoPrimeBlockingValues[pseudoPrimeIndex] = 0;
			}

			if (pseudoPrimeBlockingValues[pseudoPrimeIndex] > 0) {
				int highestValueInQue = que.size() > 1 ? pseudoPrimeBlockingValues[que[que.size() - 1]] : 0;
				if (highestValueInQue > nextHitValue) {
					int i = queIndex + 1;
					int queNextIndex = que[i];
					int currentQueBlockingValue = pseudoPrimeBlockingValues[queNextIndex];
					//int currentQueBlockingValue = wheelBlockingValues[queNextIndex][wheelValuePrimeMultiplierIndexs[queNextIndex]];
					while (currentQueBlockingValue < nextHitValue) {
						queNextIndex = que[++i];
						//currentQueBlockingValue = wheelBlockingValues[queNextIndex][wheelValuePrimeMultiplierIndexs[queNextIndex]];
						currentQueBlockingValue = pseudoPrimeBlockingValues[queNextIndex];
					}
					//while (wheelsNextValues[que[i]] < nextValue) {//TODO: Do binary search.
					//	i++;
					//}

					que.insert(que.begin() + i, pseudoPrimeIndex);
					//que.insert(que.begin() + i, wheelIndex);
					//wheelsNextValues[wheelIndex] = nextHitValue;
				}
				else {
					que.push_back(pseudoPrimeIndex);
				}
			}

			/*int nextWheelValuePrimeMultiplierIndex = ++wheelValuePrimeMultiplierIndexs[wheelIndex];
			bool shouldCheckThisWheelNextValue = wheelBlockingValues[wheelIndex].size() - 1 >= nextWheelValuePrimeMultiplierIndex;
			if (shouldCheckThisWheelNextValue) {
				//int nextValue = pseudoPrimes[++pseudoPrimeIndexes[wheelIndex]] * wheel;
				int nextValue = wheelBlockingValues[wheelIndex][nextWheelValuePrimeMultiplierIndex];
				if (nextValue < wheelRepititionCircumfrances[wheelIndex]) {
					//Add the hit wheel's next value to the que.
					int i = queIndex + 1;
					int queNextIndex = que[i];
					int currentQueBlockingValue = wheelBlockingValues[queNextIndex][wheelValuePrimeMultiplierIndexs[queNextIndex]];
					while (currentQueBlockingValue < nextValue) {
						queNextIndex = que[++i];
						currentQueBlockingValue = wheelBlockingValues[queNextIndex][wheelValuePrimeMultiplierIndexs[queNextIndex]];
					}
					//while (wheelsNextValues[que[i]] < nextValue) {//TODO: Do binary search.
					//	i++;
					//}

					que.insert(que.begin() + i, wheelIndex);
					wheelsNextValues[wheelIndex] = nextValue;
				}
			}*/
			
			//Might be needed if wheels are added past the point when they should have been dropped.
			//while (que[queIndex] < nextWheelToDrop) {
			//	++queIndex;
			//}
			int queNextIndex2 = que.size() - 1 > queIndex ? que[++queIndex] : 0;
			nextPseudoPrimeBlockingValue = pseudoPrimeBlockingValues[queNextIndex2];
			//wheelsNextValue = wheelBlockingValues[queNextIndex2][wheelValuePrimeMultiplierIndexs[queNextIndex2]];
		}
		else {
			//Not a hit, so the number is prime.
			primes.push_back(num);
			int square = num * num;
			if (square < end) {
				int primeIndex = primes.size() - 1;
				//wheelsNextPseudoPrime.push_back(square);
				int lastWheelCircumfrance = wheelRepititionCircumfrances[wheelRepititionCircumfrances.size() - 1];
				int wheelRepititionCircumfrance = lastWheelCircumfrance * num;
				if (wheelRepititionCircumfrance > end)
					wheelRepititionCircumfrance = end;

				wheelRepititionCircumfrances.push_back(wheelRepititionCircumfrance);
				//wheelsNextValues.push_back(square);
				//wheelValuePrimeMultiplierIndexs.push_back(0);
				//wheelBlockingValues.push_back({ square });
				//wheelBlockingValues2.push_back(square);
				if (wheelRepititionCircumfrance > square) {
					int pseudoPrimeIndex = pseudoPrimes.size();
					pseudoPrimes.push_back(num);
					pseudoPrimeBlockingValues.push_back(square);
					pseudoPrimeWheelBaseValue.push_back(num);
					pseduoPrimeToWheelIndex.push_back(primeIndex);
					pseudoPrime_PrimeMultiplierIndexes.push_back(primeIndex);//TODO: replace size() with an integer that tracks size instead.
					pseduoPrimeFirstHit.push_back(true);
					int highestValueInQue = que.size() > 1 ?  pseudoPrimeBlockingValues[que[que.size() - 1]] : 0;
					if (highestValueInQue > square) {
						int i = queIndex + 1;
						int queNextIndex = que[i];
						int currentQueBlockingValue = pseudoPrimeBlockingValues[queNextIndex];
						//int currentQueBlockingValue = wheelBlockingValues[queNextIndex][wheelValuePrimeMultiplierIndexs[queNextIndex]];
						while (currentQueBlockingValue < square) {
							queNextIndex = que[++i];
							//currentQueBlockingValue = wheelBlockingValues[queNextIndex][wheelValuePrimeMultiplierIndexs[queNextIndex]];
							currentQueBlockingValue = pseudoPrimeBlockingValues[queNextIndex];
						}

						que.insert(que.begin() + i, pseudoPrimeIndex);
					}
					else {
						que.push_back(pseudoPrimeIndex);
						if (num > nextPseudoPrimeBlockingValue)//TODO: remove when wheelsNextValue is set to 2, 3, 5, 7, etc.
							nextPseudoPrimeBlockingValue = square;//TODO: remove when wheelsNextValue is set to 2, 3, 5, 7, etc.
					}
				}

				//if (wheelsNextValue < num)//TODO: remove when wheelsNextValue is set to 2, 3, 5, 7, etc.
				//	wheelsNextValue = square;
				
				stitches.emplace(num, std::vector<int>{ num });// .insert({ num, });
			}
		}

		//Check if the bigWheel has reached the end.  If it has, it needs to be sized up by dropping the current wheel and working 
		//	on the repetition circumference of the next wheel.
		if (++bigWheelIndex == bigWheelSize) {
			if (printAll) {
				PrintVector(primes, "Primes");
				std::cout << "Big Wheel Index: " << bigWheelIndex << std::endl;
				std::cout << "Dropping wheel " << nextWheelToDrop << std::endl;
				PrintVector(bigWheel, "Big Wheel before");
			}
			
			//Do stitches for current wheel before sizing up the big wheel.
			int tempNum = 1;
			int tempBigWheelIndex = -1;
			if (printAll) {
				PrintVector(stitches[nextWheelToDrop], "Stitches");
			}
			
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

			//Duplicate the big wheel x number of times, where x is the next wheel being worked on. 2, 6, 30, 210, 2310, 30030, 510510...
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
	}

	primes.erase(primes.begin());
	primes.erase(primes.end() - 1);

	auto end_time = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start).count();
}