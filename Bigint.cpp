#include "Bigint.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <vector>

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

    std::string rev = reverseBlocks(hex_str);
    
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
    /**/
    return (reverseBlocksBack(result));
}

//operations
BigInt BigInt::longAdd(const BigInt& left, const BigInt& right) {
    BigInt result, helper, helperr;
    helper = left;
    helperr = right;
    size_t _count = count_check(left, right);

    result.resize(_count);

    helper.resize(_count);
    helperr.resize(_count);

    uint64_t carry = 0; 

    for (size_t i = 0; i < _count; ++i) {
        uint64_t sum = static_cast<uint64_t>(helper.num[i]) + helperr.num[i] + carry; 

        if (sum >= int32_size) { 
            carry = 1;
            result.num[i] = static_cast<uint32_t>(sum % int32_size); 
        }
        else {
            carry = 0; 
            result.num[i] = static_cast<uint32_t>(sum); 
        }
    }
    return result;
}

BigInt BigInt::longSub(const BigInt& left, const BigInt& right) {
    BigInt result;
    BigInt minuend = left; 
    BigInt subtrahend = right;
    size_t _count = count_check(left, right);

    minuend.resize(_count);
    subtrahend.resize(_count);
    result.resize(_count);

    int64_t borrow = 0;

    for (size_t i = 0; i < _count; ++i) {
        int64_t diff = static_cast<int64_t>(minuend.num[i]) - subtrahend.num[i] - borrow;

        if (diff < 0) {  
            diff += int32_size;  
            borrow = 1;  
        }
        else {
            borrow = 0;  
        }

        result.num[i] = static_cast<uint32_t>(diff);
    }

    if (borrow > 0) {
        std::cerr << "Warning: Result is negative!" << std::endl;
    }
    return result;
}

BigInt BigInt::multiplyDigitByBigInt(const BigInt& number, uint32_t digit) {
    BigInt result;
    if (digit == 0) {
        return result;
    }
 
    result.resize(MaxCount);
    BigInt minuend = number;
    minuend.resize(MaxCount);

    uint64_t carry = 0;

    for (size_t i = 0; i < MaxCount; ++i) {
        uint64_t product = static_cast<uint64_t>(minuend.num[i]) * digit + carry;

        result.num[i] = static_cast<uint32_t>(product);

        carry = product >> 32;
    }

    if (carry > 0) {
        result.resize(result.count + 1); 
        result.num[result.count - 1] = static_cast<uint32_t>(carry);
    }

    return result;
}

BigInt BigInt::multiplyBigInt(const BigInt& left, const BigInt& right) {
    BigInt result;
    result.resize(left.count + right.count);

    uint64_t carry = 0;

    for (size_t i = 0; i < left.count; ++i) {
        carry = 0; 
        for (size_t j = 0; j < right.count; ++j) {
            uint64_t product = static_cast<uint64_t>(left.num[i]) * right.num[j] + result.num[i + j] + carry;
            result.num[i + j] = static_cast<uint32_t>(product);
            carry = product >> 32;
        }
        result.num[i + right.count] = static_cast<uint32_t>(carry);
    }
    return result;
}

//all reverse realization))
    std::string reverseBlocks(const std::string& input) {
        std::string result = input;
        size_t length = result.length();

        if (length % 8 != 0) {
            size_t padding = 8 - (length % 8);

            result.insert(0, padding, '0');
        }
        size_t blockSize = 8;
        std::vector<std::string> blocks;
        for (size_t i = 0; i < result.length(); i += blockSize) {
            blocks.push_back(result.substr(i, blockSize));
        }
        std::reverse(blocks.begin(), blocks.end());
        std::string reversed;
        for (const auto& block : blocks) {
            reversed += block;
        }
        return reversed;
    }
    std::string reverseBlocksBack(const std::string& input) {
        size_t blockSize = 8;
        std::vector<std::string> blocks;

        for (size_t i = 0; i < input.length(); i += blockSize) {
            blocks.push_back(input.substr(i, blockSize));
        }

        std::reverse(blocks.begin(), blocks.end());

        std::string result;
        for (const auto& block : blocks) {
            result += block;
        }

        size_t firstNonZero = result.find_first_not_of('0');
        if (firstNonZero == std::string::npos) {
            return "0";
        }
        return result.substr(firstNonZero);
    }

    //other functions
    

    size_t count_check(const BigInt& left, const BigInt& right) {
        if (left.count > right.count) {
            return left.count;
        }
        else { return right.count; }
    }

    void BigInt::resize(size_t newCount) {
        if (this->count == newCount) {
            return; 
        }

        uint32_t* newNum = new uint32_t[newCount];

        for (size_t i = 0;  i < std::min(count, newCount); ++i) {
            newNum[i] = num[i];
        }

        for (size_t i = count; i < newCount; ++i) {
            newNum[i] = 0;
        }

        delete[] num;
 
        num = newNum;
        count = newCount;
    }

    ////////////
    size_t BigInt::bitLength() const {
        size_t bitLength = 0;

        for (size_t i = count - 1; i >= 0; --i) { 
            if (num[i] != 0) {
                for (int j = 31; j >= 0; --j) {
                    if (num[i] & (1 << j)) {
                        bitLength = i * 32 + j + 1;
                        return bitLength;
                    }
                }
            }
        }

        return bitLength; 
    }

    BigInt BigInt::shiftBitsToHigh(size_t n) const {
        BigInt result(*this);

        size_t chunkShift = n / 32;
        size_t bitShift = n % 32;    

        result.resize(count + chunkShift);

        if (chunkShift > 0) {
            for (size_t i = count; i-- > 0;) {
                result.num[i + chunkShift] = result.num[i];
            }

            for (size_t i = 0; i < chunkShift; ++i) {
                result.num[i] = 0;
            }
        }

        if (bitShift > 0) {
            uint32_t carry = 0; 
            for (size_t i = 0; i < count + chunkShift; ++i) {
                uint32_t current = result.num[i];
                result.num[i] = (current << bitShift) | carry; 
                carry = current >> (32 - bitShift);
            }

            if (carry > 0) {
                result.resize(result.count + 1);
                result.num[result.count - 1] = carry;
            }
        }

        return result;
    }

    bool BigInt::operator>=(const BigInt& other) const {
        if (count != other.count) {
            return count > other.count;
        }

        for (size_t i = count - 1 ; i >= 0; --i) {
            if (this->num[i] != other.num[i]) {
                return this->num[i] > other.num[i];
            }
        }

        return true;
    }

    bool BigInt::operator==(const BigInt& other) const {
        if (count != other.count) {
            return false;
        }

        for (size_t i = 0; i < count; ++i) {
            if (num[i] != other.num[i]) {
                return false;
            }
        }

        return true;
    }

    BigInt BigInt::operator-(const BigInt& other) const {
        

        BigInt result = *this;
        uint32_t borrow = 0;

        for (size_t i = 0; i < count; ++i) {
            uint64_t diff = static_cast<uint64_t>(result.num[i]) - other.num[i] - borrow;
            if (result.num[i] < other.num[i] + borrow) {
                borrow = 1;
            }
            else {
                borrow = 0;
            }
            result.num[i] = static_cast<uint32_t>(diff);
        }

        return result;
    }

    BigInt BigInt::divide(const BigInt& A, const BigInt& B) {
        /*if (B.to_hex() == "0") {
            throw std::invalid_argument("Division by zero!");
        }*/

        BigInt Q, C;
        BigInt R = A; 

        size_t k = B.bitLength();
        size_t t;

        if (R >= B) {
            t = R.bitLength(); 

            C = B.shiftBitsToHigh(t - k); 
            std::cout << C.to_hex() << std::endl;
            if (R < C) {
                t = t - 1; 
                C = B.shiftBitsToHigh(t - k);  
            }

            R = R.longSub(R, C);
            BigInt shiftValue ;
            shiftValue = "1";
            shiftValue.resize(count + t - k);
            shiftValue = shiftValue << (t - k); 
            Q = Q.longAdd(Q, shiftValue); 
        }

        /*remainder = R; */
        return Q;
    }

    bool BigInt::operator<(const BigInt& other) const {
        if (count != other.count) {
            return count < other.count;
        }

        for (size_t i = count - 1; i >= 0; --i) {
            if (num[i] != other.num[i]) {
                return num[i] < other.num[i];
            }
        }

        return false;
    }

    BigInt BigInt::operator<<(unsigned long long shift) const {
        BigInt result = *this;

        size_t blockShift = shift / 32;
        size_t bitShift = shift % 32;    

        result.resize(count + blockShift);

        for (size_t i = count; i-- > 0;) { 
            if (i >= blockShift) {
                result.num[i] = result.num[i - blockShift];
            }
            else {
                result.num[i] = 0;
            }
        }

        if (bitShift > 0) {
            uint32_t carry = 0;
            for (size_t i = 0; i < result.count; ++i) {
                uint64_t temp = (static_cast<uint64_t>(result.num[i]) << bitShift) | carry;
                carry = static_cast<uint32_t>(temp >> 32);
                result.num[i] = static_cast<uint32_t>(temp); 
            }

            if (carry > 0) {
                result.resize(result.count + 1);
                result.num[result.count - 1] = carry;
            }
        }

        return result;
    }
