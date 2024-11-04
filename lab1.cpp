﻿#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <random>
#include "Bigint.h"
#include "Benchmark.h"

//c38027f97d1bc3787fca237180234602e0d61e48fd1b80e8642d181247ff3a1eb6105d541517c202ac9118e369ea32f0ce6aafc8a6fefe2993bec3befca44512
//28a895cfd0200f098e2effaf19391c4a837ff199610c9bd4241b1a226effbc0ba1e4c9f81cd703a4212c02f96767adbc1e336b9d2cc5eccc98bd202bddc8fa5011f2df252fbb1843b3422db8f63129bad292c3b42a2402cf732500d8f0954fdc329c8a197a41fd984ca46b69caa59553ba92bacac9646d1a9c4508c3111f7ebf
//2dc02054ff689a01dc9b1ad05270aa78a271b0c9bf058834f55843a616f85232421f0ce4a84eac70a626ecaa949f174ef2b436a4142e1dcf790dfd232a613e9a
//20736a58fe16964aa40fbafbcb63175b370f70823dd4212ff42bfbc778478b757e15cb8f9eb1bc33ae07f2366f07c6bade3f8cb1745bd0622073c835d9ed17b9897730a542ab333ec3f25e91a36787064d30eb8c5a396d5345f580b8104249d9d90cf6a85a2ce2e09a2ef2df9308cc0c9f57ff7727e6b0f56294dafff4c3b994

std::string generateHexNumber(size_t length) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 15);

    std::ostringstream hexStream;

    for (size_t i = 0; i < length; ++i) {
        int randomValue = dist(gen);
        hexStream << std::hex << std::nouppercase << randomValue;
    }

    return hexStream.str();
}

int main() {
    //BigInt a, b, c, sum, sub, bigmul, div, mod, gsd, test ;
    //std::string n = "a3f7c6b9d5e9f1b3c2d4e5f6a8b9c0d1e3f4a5b6c7d8e9f1b2c3d4e5f6a7b8c9d0e1f2a3b4c5d6e7f8a9b0c1d2e3f4a5b6c7d8e9f1b2c3d4e5f6a7b8";
    //std::string k = "8dc02054ff689a01dc9b1ad05270aa78a271b0c9bf058834f55843a616f85232421f0ce4a84eac70a626ecaa949f174ef2b436a4142e1dcf790dfd232a613e9a";
    //std::string l = "c38027f97d1bc3787fca237180234602e0d61e48fd1b80e8642d181247ff3a1eb6105d541517c202ac9118e369ea32f0ce6aafc8a6fefe2993bec3befca44512";
   /* std::string n = "a3f7c6b9d5e9f1b3c2d4e5f6a8b9c0d1e3f4a5b6c7d8e9f1b2c3d4e5f6a7b8c9d0e1f2a3b4c5d6e7f8a9b0c1d2e3f4a5b6c7d8e9f1b2c3d4e5f6a7b8";
    std::string k = "9e4d3c5f6b8a9c0d1e2f3a4b5c6d7e8f1a2b3c4d5e6f7a8b9c0d1e2f3a4b";
    std::string l = "b0e1d2c3b4a59687b6c5d4e3f2a1b098c7d6e5f4a3b2c1d0e9f8d7c6b5a4";
    std::string m = "fffff";*/
    //a = n;
    //b = k;
    //c = l;

    //sum = a + b;
    //sub = b - a;
    //bigmul = bigmul.multiplyBigInt(a, b);
    //div = div.divide(d, c);
    //mod = mod.modulo(a, b);
    //gsd = gsd.BigIntGSD(a, b);
    //test = test.LongPowerWindow(a, 2);
    //test = test.LongModPowerBarrett(a, b, c);


    std::vector<double> times;  
    double total_time = 0.0;
    size_t length = 512;
    double total = 0.0;
    for (int i = 0; i < 100; ++i) {
        std::string hexNumber1 = generateHexNumber(length);
        std::string hexNumber2 = generateHexNumber(length);
        std::string hexNumber3 = generateHexNumber(length - 10);
        std::cout << "Hex number " << (i + 1) << ": " << hexNumber1 << std::endl;
        std::cout << "Hex number " << (i + 1) << ": " << hexNumber2 << std::endl;
        std::cout << "Hex number " << (i + 1) << ": " << hexNumber3 << std::endl;
        BigInt left, right, res, extra;
        left = hexNumber1;
        right = hexNumber2;
        extra = hexNumber3;
        {
            Benchmark benchmark("Summation time for iteration " + std::to_string(i + 1));
            //res = res.BigIntGSD(left, right);
            res = res.LongModPowerBarrett(extra, left, right);
            double elapsed_time = benchmark.getElapsedTime(); 
            times.push_back(elapsed_time); 
            total_time += elapsed_time;
        }
        std::cout << "Result: " << res.to_hex() << std::endl;
    }

    /*std::cout << "First number: " << (a.to_hex()) << std::endl;
    std::cout << "Second: " << b.to_hex() << std::endl;
    std::cout << "SumResult: " << sum.to_hex() << std::endl;
    std::cout << "SubResult: " << sub.to_hex() << std::endl;
    std::cout << "MulResult: " << bigmul.to_hex() << std::endl;
    std::cout << "DivResult: " << div.to_hex() << std::endl;
    std::cout << "Mod: " << mod.to_hex() << std::endl;*/
    //std::cout << "Test: " << test.to_hex() << std::endl;

    std::cout << "Total time for all iterations: " << total_time << " seconds." << std::endl;

    return 0;
}