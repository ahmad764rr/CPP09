#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class BitcoinExchange
{
private:
    std::map<std::string, float> database;
    
    bool loadDatabase();
    bool isDateValid(const std::string& date);
    bool isValueValid(const std::string& val);
    float findRate(const std::string& date);
    
public:
    BitcoinExchange();
    BitcoinExchange(const BitcoinExchange& src);
    BitcoinExchange& operator=(const BitcoinExchange& src);
    ~BitcoinExchange();
    
    void processFile(const std::string& filename);
};

#endif
