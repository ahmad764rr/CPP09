#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>

class PmergeMe {
private:
    std::vector<int> vec;
    std::deque<int> deq;
    
    // Vector sorting
    void sortVector(std::vector<int>& arr);
    void insertIntoVector(std::vector<int>& sorted, int value);
    
    // Deque sorting
    void sortDeque(std::deque<int>& arr);
    void insertIntoDeque(std::deque<int>& sorted, int value);
    
    // Validation
    bool isValidNumber(const std::string& str);

public:
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();
    
    void run(int argc, char** argv);
};

#endif
