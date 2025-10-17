#include "BitcoinExchange.hpp"
#include <cstdlib>

BitcoinExchange::BitcoinExchange()
{
    if (!loadDatabase())
    {
        std::cerr << "Error: could not open file." << std::endl;
        exit(1);
    }
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& src)
{
    database = src.database;
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& src)
{
    if (this != &src)
        database = src.database;
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}

bool BitcoinExchange::loadDatabase()
{
    std::ifstream file("data.csv");
    if (!file.is_open())
        return false;
    
    std::string line;
    std::getline(file, line); // skip header
    
    while (std::getline(file, line))
    {
        size_t comma = line.find(',');
        if (comma == std::string::npos)
            continue;
        
        std::string date = line.substr(0, comma);
        std::string rate = line.substr(comma + 1);
        database[date] = atof(rate.c_str());
    }
    
    file.close();
    return !database.empty();
}

bool BitcoinExchange::isDateValid(const std::string& date)
{
    if (date.length() != 10)
        return false;
    
    if (date[4] != '-' || date[7] != '-')
        return false;
    
    for (int i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7)
            continue;
        if (date[i] < '0' || date[i] > '9')
            return false;
    }
    
    int year = atoi(date.substr(0, 4).c_str());
    int month = atoi(date.substr(5, 2).c_str());
    int day = atoi(date.substr(8, 2).c_str());
    
    if (year < 2009 || year > 2022)
        return false;
    if (month < 1 || month > 12)
        return false;
    if (day < 1 || day > 31)
        return false;
    
    return true;
}

bool BitcoinExchange::isValueValid(const std::string& val)
{
    if (val.empty())
        return false;
    
    int i = 0;
    if (val[0] == '-' || val[0] == '+')
        i++;
    
    int dotCount = 0;
    int digitCount = 0;
    
    while (i < (int)val.length())
    {
        if (val[i] == '.')
            dotCount++;
        else if (val[i] >= '0' && val[i] <= '9')
            digitCount++;
        else
            return false;
        i++;
    }
    
    return (dotCount <= 1 && digitCount > 0);
}

float BitcoinExchange::findRate(const std::string& date)
{
    std::map<std::string, float>::iterator it = database.lower_bound(date);
    
    if (it != database.end() && it->first == date)
        return it->second;
    
    if (it == database.begin())
        return database.begin()->second;
    
    --it;
    return it->second;
}

void BitcoinExchange::processFile(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    if (!file.is_open())
    {
        std::cerr << "Error: could not open file." << std::endl;
        return;
    }
    
    std::string line;
    std::getline(file, line); // skip header
    
    while (std::getline(file, line))
    {
        size_t pipe = line.find('|');
        if (pipe == std::string::npos)
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }
        
        std::string dateStr = line.substr(0, pipe);
        std::string valStr = line.substr(pipe + 1);
        
        // remove spaces
        while (!dateStr.empty() && dateStr[dateStr.length() - 1] == ' ')
            dateStr.erase(dateStr.length() - 1);
        while (!dateStr.empty() && dateStr[0] == ' ')
            dateStr.erase(0, 1);
        while (!valStr.empty() && valStr[0] == ' ')
            valStr.erase(0, 1);
        while (!valStr.empty() && valStr[valStr.length() - 1] == ' ')
            valStr.erase(valStr.length() - 1);
        
        if (!isDateValid(dateStr))
        {
            std::cerr << "Error: bad input => " << dateStr << std::endl;
            continue;
        }
        
        if (!isValueValid(valStr))
        {
            std::cerr << "Error: bad input => " << valStr << std::endl;
            continue;
        }
        
        float value = atof(valStr.c_str());
        
        if (value < 0)
        {
            std::cerr << "Error: not a positive number." << std::endl;
            continue;
        }
        
        if (value > 1000)
        {
            std::cerr << "Error: too large a number." << std::endl;
            continue;
        }
        
        float rate = findRate(dateStr);
        float result = value * rate;
        
        std::cout << dateStr << " => " << valStr << " = " << result << std::endl;
    }
    
    file.close();
}
