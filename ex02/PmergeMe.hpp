#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>

class PmergeMe {
private:
    std::vector<int> vecData;
    std::deque<int> deqData;
    std::vector<int> original;
    
    void mergeInsertVector(std::vector<int>& arr);
    void binaryInsertVec(std::vector<int>& arr, int value);
    
    void mergeInsertDeque(std::deque<int>& arr);
    void binaryInsertDeq(std::deque<int>& arr, int value);
    
    bool checkNumber(const std::string& str);
    size_t jacobsthal(size_t n);
    void generateJacobsthalSeq(std::vector<size_t>& seq, size_t limit);

public:
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();
    
    void process(int argc, char** argv);
};

#endif
