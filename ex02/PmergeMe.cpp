#include "PmergeMe.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& other) {
    vec = other.vec;
    deq = other.deq;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
        vec = other.vec;
        deq = other.deq;
    }
    return *this;
}

PmergeMe::~PmergeMe() {}

bool PmergeMe::isValidNumber(const std::string& str) {
    if (str.empty())
        return false;
    
    size_t i = 0;
    if (str[0] == '+')
        i = 1;
    
    if (i >= str.length())
        return false;
    
    while (i < str.length()) {
        if (str[i] < '0' || str[i] > '9')
            return false;
        i++;
    }
    return true;
}

void PmergeMe::insertIntoVector(std::vector<int>& sorted, int value) {
    // Binary search for insertion point
    size_t left = 0;
    size_t right = sorted.size();
    
    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (sorted[mid] < value)
            left = mid + 1;
        else
            right = mid;
    }
    
    sorted.insert(sorted.begin() + left, value);
}

void PmergeMe::sortVector(std::vector<int>& arr) {
    size_t n = arr.size();
    if (n <= 1)
        return;
    
    // Use simple insertion sort for small arrays
    if (n <= 16) {
        for (size_t i = 1; i < n; i++) {
            int key = arr[i];
            int j = i - 1;
            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
        return;
    }
    
    // Step 1: Create pairs and track pairings
    std::vector<std::pair<int, int> > pairs;
    int straggler = -1;
    bool hasStraggler = (n % 2 == 1);
    
    if (hasStraggler)
        straggler = arr[n - 1];
    
    // Make pairs, keep larger first
    for (size_t i = 0; i < n - (hasStraggler ? 1 : 0); i += 2) {
        if (arr[i] > arr[i + 1])
            pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
        else
            pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
    }
    
    if (pairs.empty()) {
        if (hasStraggler)
            arr[0] = straggler;
        return;
    }
    
    // Step 2: Extract larger elements and sort them recursively
    std::vector<int> larger;
    std::vector<int> smaller;
    
    for (size_t i = 0; i < pairs.size(); i++) {
        larger.push_back(pairs[i].first);
        smaller.push_back(pairs[i].second);
    }
    
    sortVector(larger);
    
    // Step 3: Build result - first smaller element, then all larger elements
    std::vector<int> result;
    if (!smaller.empty())
        result.push_back(smaller[0]);
    
    for (size_t i = 0; i < larger.size(); i++)
        result.push_back(larger[i]);
    
    // Step 4: Insert remaining smaller elements in order
    for (size_t i = 1; i < smaller.size(); i++)
        insertIntoVector(result, smaller[i]);
    
    // Step 5: Insert straggler if exists
    if (hasStraggler)
        insertIntoVector(result, straggler);
    
    arr = result;
}

void PmergeMe::insertIntoDeque(std::deque<int>& sorted, int value) {
    // Binary search for insertion point
    size_t left = 0;
    size_t right = sorted.size();
    
    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (sorted[mid] < value)
            left = mid + 1;
        else
            right = mid;
    }
    
    sorted.insert(sorted.begin() + left, value);
}

void PmergeMe::sortDeque(std::deque<int>& arr) {
    size_t n = arr.size();
    if (n <= 1)
        return;
    
    // Use simple insertion sort for small arrays
    if (n <= 16) {
        for (size_t i = 1; i < n; i++) {
            int key = arr[i];
            int j = i - 1;
            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
        return;
    }
    
    // Step 1: Create pairs and track pairings
    std::vector<std::pair<int, int> > pairs;
    int straggler = -1;
    bool hasStraggler = (n % 2 == 1);
    
    if (hasStraggler)
        straggler = arr[n - 1];
    
    // Make pairs, keep larger first
    for (size_t i = 0; i < n - (hasStraggler ? 1 : 0); i += 2) {
        if (arr[i] > arr[i + 1])
            pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
        else
            pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
    }
    
    if (pairs.empty()) {
        if (hasStraggler)
            arr[0] = straggler;
        return;
    }
    
    // Step 2: Extract larger elements and sort them recursively
    std::deque<int> larger;
    std::deque<int> smaller;
    
    for (size_t i = 0; i < pairs.size(); i++) {
        larger.push_back(pairs[i].first);
        smaller.push_back(pairs[i].second);
    }
    
    sortDeque(larger);
    
    // Step 3: Build result - first smaller element, then all larger elements
    std::deque<int> result;
    if (!smaller.empty())
        result.push_back(smaller[0]);
    
    for (size_t i = 0; i < larger.size(); i++)
        result.push_back(larger[i]);
    
    // Step 4: Insert remaining smaller elements in order
    for (size_t i = 1; i < smaller.size(); i++)
        insertIntoDeque(result, smaller[i]);
    
    // Step 5: Insert straggler if exists
    if (hasStraggler)
        insertIntoDeque(result, straggler);
    
    arr = result;
}

void PmergeMe::run(int argc, char** argv) {
    // Parse and validate input
    std::vector<int> original;
    
    for (int i = 1; i < argc; i++) {
        std::string str = argv[i];
        
        if (!isValidNumber(str)) {
            std::cerr << "Error" << std::endl;
            exit(1);
        }
        
        long num = std::atol(str.c_str());
        if (num < 0 || num > 2147483647) {
            std::cerr << "Error" << std::endl;
            exit(1);
        }
        
        vec.push_back(static_cast<int>(num));
        deq.push_back(static_cast<int>(num));
        original.push_back(static_cast<int>(num));
    }
    
    // Print before
    std::cout << "Before: ";
    for (size_t i = 0; i < original.size(); i++) {
        std::cout << original[i];
        if (i < original.size() - 1)
            std::cout << " ";
    }
    std::cout << std::endl;
    
    // Sort with vector
    clock_t start = clock();
    sortVector(vec);
    clock_t end = clock();
    double vecTime = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000;
    
    // Sort with deque
    start = clock();
    sortDeque(deq);
    end = clock();
    double deqTime = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000;
    
    // Print after
    std::cout << "After:  ";
    for (size_t i = 0; i < vec.size(); i++) {
        std::cout << vec[i];
        if (i < vec.size() - 1)
            std::cout << " ";
    }
    std::cout << std::endl;
    
    // Print timing
    std::cout << "Time to process a range of " << vec.size() 
              << " elements with std::vector : " 
              << std::fixed << std::setprecision(5) << vecTime << " us" << std::endl;
    
    std::cout << "Time to process a range of " << deq.size() 
              << " elements with std::deque : " 
              << std::fixed << std::setprecision(5) << deqTime << " us" << std::endl;
}
