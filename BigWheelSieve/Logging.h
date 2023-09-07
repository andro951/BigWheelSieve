#pragma once

#include <vector>
#include <set>
#include <string>
#include "PrimeFactorization.h"

template<typename T>
void PrintVector(std::vector<T>& vec, std::string name = "") {
	if (name != "")
		std::cout << name << ": ";

	bool first = true;
	for (auto& value : vec) {
		if (first) {
			first = false;
		}
		else {
			std::cout << ", ";
		}

		std::cout << value;
	}

	std::cout << std::endl;
}

template<typename T>
void PrintSet(std::set<T>& valueSet, std::string name = "") {
	if (name != "")
		std::cout << name << ": ";

	bool first = true;
	for (auto& value : valueSet) {
		if (first) {
			first = false;
		}
		else {
			std::cout << ", ";
		}

		std::cout << value;
	}

	std::cout << std::endl;
}

void PrintPrimeFactors(int num) {
	std::vector<int> primeFactors = PrimeFactors(num);
	std::string s = "Factors of " + std::to_string(num);
	PrintVector(primeFactors, s);
}

void PrintAllPrimeFactors(std::vector<int> numbers) {
	if (numbers.size() > 0) {
		for (const auto& num : numbers) {
			PrintPrimeFactors(num);
		}

		std::cout << std::endl;
	}
}

template<typename T>
void PrintVectorDiff(std::vector<T>& correct, std::string name1, std::vector<T>& test, std::string name2) {
	int correctIndex = 0;
	int testIndex = 0;
	std::vector<T> missing;
	std::vector<T> extra;
	T correctValue = correct[correctIndex];
	T testValue = test[testIndex];
	int testSize = test.size();
	int correctSize = correct.size();
	while (testIndex < testSize) {
		if (correctValue == testValue) {
			if (++correctIndex < correctSize)
				correctValue = correct[correctIndex];

			//if (testIndex >= testSize - 1) {
			//	missing.push_back(correctValue);
			//	correctValue = correct[++correctIndex];
			//	continue;
			//}
			if (++testIndex < testSize)
				testValue = test[testIndex];
		}
		else if (testValue > correctValue) {
			if (++correctIndex < correctSize) {
				missing.push_back(correctValue);
				correctValue = correct[correctIndex];
			}
			else {
				extra.push_back(testValue);
				if (++testIndex >= testSize)
					continue;

				testValue = test[testIndex];
			}
		}
		else {
			extra.push_back(testValue);
			if (++testIndex >= testSize)
				break;

			testValue = test[testIndex];
		}
	}

	PrintVector(missing, "Missing");
	PrintAllPrimeFactors(missing);

	PrintVector(extra, "Extra");
	PrintAllPrimeFactors(extra);
}