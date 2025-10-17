#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& other) {
    vecData = other.vecData;
    deqData = other.deqData;
    original = other.original;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
        vecData = other.vecData;
        deqData = other.deqData;
        original = other.original;
    }
    return *this;
}

PmergeMe::~PmergeMe() {}

size_t PmergeMe::jacobsthal(size_t n) {
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;
    
    size_t prev = 0;
    size_t curr = 1;
    
    for (size_t i = 2; i <= n; i++) {
        size_t next = curr + 2 * prev;
        prev = curr;
        curr = next;
    }
    return curr;
}

void PmergeMe::generateJacobsthalSeq(std::vector<size_t>& seq, size_t limit) {
    if (limit == 0)
        return;
    
    size_t k = 3;
    while (true) {
        size_t jk = jacobsthal(k);
        if (jk >= limit)
            break;
        seq.push_back(jk);
        k++;
    }
}

bool PmergeMe::checkNumber(const std::string& str) {
    if (str.empty())
        return false;
    
    size_t start = 0;
    if (str[0] == '+')
        start = 1;
    
    if (start >= str.length())
        return false;
    
    for (size_t i = start; i < str.length(); i++) {
        if (str[i] < '0' || str[i] > '9')
            return false;
    }
    return true;
}

void PmergeMe::binaryInsertVec(std::vector<int>& arr, int value) {
    size_t left = 0;
    size_t right = arr.size();
    
    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (arr[mid] < value)
            left = mid + 1;
        else
            right = mid;
    }
    
    arr.insert(arr.begin() + left, value);
}

void PmergeMe::mergeInsertVector(std::vector<int>& arr) {
    size_t n = arr.size();
    if (n <= 1)
        return;
    
    if (n <= 16) {
        for (size_t i = 1; i < n; i++) {
            int key = arr[i];
            int j = static_cast<int>(i) - 1;
            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
        return;
    }
    
    std::vector<std::pair<int, int> > pairs;
    int unpaired = -1;
    bool hasUnpaired = (n % 2 == 1);
    
    if (hasUnpaired)
        unpaired = arr[n - 1];
    
    for (size_t i = 0; i < n - (hasUnpaired ? 1 : 0); i += 2) {
        if (arr[i] > arr[i + 1])
            pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
        else
            pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
    }
    
    std::vector<std::pair<int, int> > sortedPairs;
    std::vector<int> larger;
    for (size_t i = 0; i < pairs.size(); i++)
        larger.push_back(pairs[i].first);
    
    mergeInsertVector(larger);
    
    for (size_t i = 0; i < larger.size(); i++) {
        for (size_t j = 0; j < pairs.size(); j++) {
            if (pairs[j].first == larger[i]) {
                sortedPairs.push_back(pairs[j]);
                break;
            }
        }
    }
    
    std::vector<int> chain;
    if (!sortedPairs.empty())
        chain.push_back(sortedPairs[0].second);
    
    for (size_t i = 0; i < larger.size(); i++)
        chain.push_back(larger[i]);
    
    std::vector<size_t> jacobSeq;
    generateJacobsthalSeq(jacobSeq, sortedPairs.size());
    
    std::vector<bool> inserted(sortedPairs.size(), false);
    if (!sortedPairs.empty())
        inserted[0] = true;
    
    for (size_t i = 0; i < jacobSeq.size(); i++) {
        size_t jIdx = jacobSeq[i];
        if (jIdx < sortedPairs.size() && !inserted[jIdx]) {
            binaryInsertVec(chain, sortedPairs[jIdx].second);
            inserted[jIdx] = true;
        }
        
        if (i + 1 < jacobSeq.size()) {
            size_t prevJ = jacobSeq[i];
            size_t nextJ = jacobSeq[i + 1];
            for (size_t j = prevJ - 1; j > 0 && j >= nextJ; j--) {
                if (j < sortedPairs.size() && !inserted[j]) {
                    binaryInsertVec(chain, sortedPairs[j].second);
                    inserted[j] = true;
                }
            }
        }
    }
    
    for (size_t i = 1; i < sortedPairs.size(); i++) {
        if (!inserted[i]) {
            binaryInsertVec(chain, sortedPairs[i].second);
            inserted[i] = true;
        }
    }
    
    if (hasUnpaired)
        binaryInsertVec(chain, unpaired);
    
    arr = chain;
}

void PmergeMe::binaryInsertDeq(std::deque<int>& arr, int value) {
    size_t left = 0;
    size_t right = arr.size();
    
    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (arr[mid] < value)
            left = mid + 1;
        else
            right = mid;
    }
    
    arr.insert(arr.begin() + left, value);
}

void PmergeMe::mergeInsertDeque(std::deque<int>& arr) {
    size_t n = arr.size();
    if (n <= 1)
        return;
    
    if (n <= 16) {
        for (size_t i = 1; i < n; i++) {
            int key = arr[i];
            int j = static_cast<int>(i) - 1;
            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
        return;
    }
    
    std::vector<std::pair<int, int> > pairs;
    int unpaired = -1;
    bool hasUnpaired = (n % 2 == 1);
    
    if (hasUnpaired)
        unpaired = arr[n - 1];
    
    for (size_t i = 0; i < n - (hasUnpaired ? 1 : 0); i += 2) {
        if (arr[i] > arr[i + 1])
            pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
        else
            pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
    }
    
    std::vector<std::pair<int, int> > sortedPairs;
    std::deque<int> larger;
    for (size_t i = 0; i < pairs.size(); i++)
        larger.push_back(pairs[i].first);
    
    mergeInsertDeque(larger);
    
    for (size_t i = 0; i < larger.size(); i++) {
        for (size_t j = 0; j < pairs.size(); j++) {
            if (pairs[j].first == larger[i]) {
                sortedPairs.push_back(pairs[j]);
                break;
            }
        }
    }
    
    std::deque<int> chain;
    if (!sortedPairs.empty())
        chain.push_back(sortedPairs[0].second);
    
    for (size_t i = 0; i < larger.size(); i++)
        chain.push_back(larger[i]);
    
    std::vector<size_t> jacobSeq;
    generateJacobsthalSeq(jacobSeq, sortedPairs.size());
    
    std::vector<bool> inserted(sortedPairs.size(), false);
    if (!sortedPairs.empty())
        inserted[0] = true;
    
    for (size_t i = 0; i < jacobSeq.size(); i++) {
        size_t jIdx = jacobSeq[i];
        if (jIdx < sortedPairs.size() && !inserted[jIdx]) {
            binaryInsertDeq(chain, sortedPairs[jIdx].second);
            inserted[jIdx] = true;
        }
        
        if (i + 1 < jacobSeq.size()) {
            size_t prevJ = jacobSeq[i];
            size_t nextJ = jacobSeq[i + 1];
            for (size_t j = prevJ - 1; j > 0 && j >= nextJ; j--) {
                if (j < sortedPairs.size() && !inserted[j]) {
                    binaryInsertDeq(chain, sortedPairs[j].second);
                    inserted[j] = true;
                }
            }
        }
    }
    
    for (size_t i = 1; i < sortedPairs.size(); i++) {
        if (!inserted[i]) {
            binaryInsertDeq(chain, sortedPairs[i].second);
            inserted[i] = true;
        }
    }
    
    if (hasUnpaired)
        binaryInsertDeq(chain, unpaired);
    
    arr = chain;
}

void PmergeMe::process(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        std::string str = argv[i];
        
        if (!checkNumber(str)) {
            std::cerr << "Error" << std::endl;
            exit(1);
        }
        
        long num = std::atol(str.c_str());
        if (num < 0 || num > 2147483647) {
            std::cerr << "Error" << std::endl;
            exit(1);
        }
        
        int value = static_cast<int>(num);
        vecData.push_back(value);
        deqData.push_back(value);
        original.push_back(value);
    }
    
    std::cout << "Before: ";
    for (size_t i = 0; i < original.size(); i++) {
        std::cout << original[i];
        if (i < original.size() - 1)
            std::cout << " ";
    }
    std::cout << std::endl;
    
    clock_t start = clock();
    mergeInsertVector(vecData);
    clock_t end = clock();
    double vecTime = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000;
    
    start = clock();
    mergeInsertDeque(deqData);
    end = clock();
    double deqTime = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000;
    
    std::cout << "After:  ";
    for (size_t i = 0; i < vecData.size(); i++) {
        std::cout << vecData[i];
        if (i < vecData.size() - 1)
            std::cout << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Time to process a range of " << vecData.size() 
              << " elements with std::vector : " 
              << std::fixed << std::setprecision(5) << vecTime << " us" << std::endl;
    
    std::cout << "Time to process a range of " << deqData.size() 
              << " elements with std::deque : " 
              << std::fixed << std::setprecision(5) << deqTime << " us" << std::endl;
}
