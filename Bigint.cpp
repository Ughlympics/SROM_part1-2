#include "Bigint.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <sstream>

const uint16_t MaxCount = 64;
const uint16_t MaxSize = 512;
const int64_t int32_size = 4294967296;

//default constructors
BigInt::BigInt() : count{ 1 }, size{ 1 } {
	num = new uint32_t[size];
	for (int i = 0; i < size; ++i)
		num[i] = 0;
}

BigInt::BigInt(uint32_t _default_num, size_t _chunks, size_t _size) : count{ _chunks }, size{ _size } {
    num = new uint32_t[count];

    for (size_t i = 0; i < count - 1; ++i) {
        num[i] = 0; 
    }

    num[count - 1] = _default_num;
}

BigInt::BigInt(const BigInt& copy) : count{ copy.count }, size{ copy.size }
{
    num = new uint32_t[count];
    for (int i = 0; i < count; ++i)
        num[i] = copy.num[i];
}

//overloaded operators
BigInt& BigInt::operator=(const std::string& hex_str) {
    if (hex_str.length() > MaxSize + 2) {
        throw std::length_error("Error: String exceeds maximum length");
    }

    /*if (hex_str.length() <= 2 || hex_str[0] != '0' || hex_str[1] != 'x' ||
        !std::all_of(hex_str.begin() + 2, hex_str.end(), [](char c) -> bool {
            return (std::isalpha(c) && std::tolower(c) < 'g') || std::isdigit(c);
            }))
    {
        return *this;
    }*/
    size_t length = hex_str.length();
    size_t newCount = (length / 8) + 1 ;

    if (count < newCount) {
        count = newCount;
        delete[] num;
        num = new uint32_t[count]; 
    }

    for (uint16_t i = 0; i < count; ++i) {
        num[i] = 0; 
    }

    std::string rev = reverseString(hex_str);
    
    size = rev.length();

    for (size_t i = 0; i < size ; i += 8) {
        std::string hexChunk = rev.substr(i, 8);

        num[i / 8] = static_cast<uint32_t>(std::stoul(hexChunk, nullptr, 16));
    }

    return *this;
}

BigInt& BigInt::operator=(const BigInt& copy)
{
    if (num)
        delete[] num;

    count = copy.count;
    num = new uint32_t[count];
    for (int i = 0; i < count; ++i)
        num[i] = copy.num[i];

    return *this;
}

//print function
std::string BigInt::to_hex() const {
    if (count == 0 || num == nullptr) {
        return "0x0";
    }

    std::string result = "";

    for (size_t i = 0; i < count; ++i) {
        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(8) << num[i];
        result += ss.str(); 
    }
    
    return removeLeadingZeros(reverseString(result));
}

//operations
BigInt BigInt::longAdd(const BigInt& left, const BigInt& right) {
    BigInt result, helper, helperr;
    helper = left;
    helperr = right;

    result.resize(MaxCount);

    helper.resize(MaxCount);
    helperr.resize(MaxCount);

    uint64_t carry = 0; 

    for (size_t i = 0; i < MaxCount; ++i) {
        uint64_t sum = static_cast<uint64_t>(helper.num[i]) + helperr.num[i] + carry; 

        if (sum >= int32_size) { 
            carry = sum / int32_size;
            result.num[i] = static_cast<uint32_t>(sum % int32_size); 
        }
        else {
            carry = 0; 
            result.num[i] = static_cast<uint32_t>(sum); 
        }
    }

    if (carry > 0) {
        result.resize(MaxCount + 1); 
        result.num[MaxCount] = static_cast<uint32_t>(carry); 
    }

    return result;
}

//close functions 
    std::string reverseString(const std::string & input) {
        if (input.length() < 2) {
            return input;
        }
        std::string reversed = input;
        std::reverse(reversed.begin(), reversed.end());
        return reversed;
    }


    std::string removeLeadingZeros(const std::string& input) {
        if (input.empty()) {
            return "0";
        }

        size_t n = 0;  
        size_t k = input.length();

        while (n < k && input[n] == '0') {
            n++;
        }

        if (n == k) {
            return "0";
        }

        std::string result = input.substr(n);

        if (result.length() > 8) {
            std::string block = result.substr(0, 8);
            std::string rest = result.substr(8);  

            size_t lastNonZero = block.find_last_not_of('0');
            if (lastNonZero != std::string::npos) {
                block = block.substr(0, lastNonZero + 1);
            }

            return block + rest;
        }

        return result;
    }
    

    size_t size_check(const BigInt& left, const BigInt& right) {
        if (left.size > right.size) {
            return left.size;
        }
        else { return right.size; }
    }

    void BigInt::resize(size_t newCount) {
        if (count == newCount) {
            return; 
        }

        uint32_t* newNum = new uint32_t[newCount];

        for (size_t i = 0; i < count + 1; ++i) {
            newNum[i] = num[i];
        }

        for (size_t i = count; i < newCount; ++i) {
            newNum[i] = 0;
        }

        delete[] num;

        num = newNum;
        count = newCount;
    }

