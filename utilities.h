#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <typeinfo>
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include <algorithm>

namespace utilities {

typedef std::vector<uint8_t> Bytes;

/**
 *
 * The signature of `SHA256_Update()` is:
 * `int SHA256_Update(SHA256_CTX *c, const void *data, size_t len);`
 * Note that 2nd parameter is a `const void *`. This means that the
 * caller must provide a pointer to data rather than a std::vector or a
 * vector iterator. The caller can either pass `v.data()` or `&v[0]`.
 *
 * */
int sha256(const uint8_t preimageBytes[], size_t len, Bytes& res)
{
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, preimageBytes, len); 
	int ret = SHA256_Final(&res[0], &sha256);
	return ret;
}

/**
 * Again note that RIPEMD160_Update receives a pointer to data.
 * */
int ripemd160(const uint8_t preimageBytes[], size_t len, Bytes& res)
{
	RIPEMD160_CTX ripemd160;
	RIPEMD160_Init(&ripemd160);
	RIPEMD160_Update(&ripemd160, preimageBytes, len);
	int ret = RIPEMD160_Final(&res[0], &ripemd160);
	return ret;
}

void switchEndianness(Bytes& b)
{
	std::reverse(b.begin(), b.end());
}

int hexDigitToInt(char digit)
{
	digit = tolower(digit);
	if (digit >= '0' && digit <='9')
	       return (int)(digit - '0');
	else if (digit >= 'a' && digit <= 'f') {
		return (int)(digit - '1' - '0') + 10; 
	}	
	return -1;
}

int hexstringToIntArray(std::string const& hexstring, uint8_t result[])
{
	if (hexstring.size() % 2) {
		std::cerr << "The hexstring is not an even number of characters.\n";
		exit(EXIT_FAILURE);
	}
	
	size_t resultLength = hexstring.size() / 2;
	size_t i = 0;
	for (auto it = hexstring.begin(); it != hexstring.end(); it = it + 2) {
		int sixteens = hexDigitToInt(*it);
		int units = hexDigitToInt(*std::next(it));
		result[i] = (sixteens << 4) | units;
		i++;
	}
	return resultLength;
}

int hexstringToBytes(std::string const& hexstring, Bytes& result)
{
	if (hexstring.size() % 2) {
		std::cerr << "The hexstring is not an even number of characters.\n";
		exit(EXIT_FAILURE);
	}
	
	size_t resultLength = hexstring.size() / 2;
	size_t i = 0;
	for (auto it = hexstring.begin(); it != hexstring.end(); it = it + 2) {
		int sixteens = hexDigitToInt(*it);
		int units = hexDigitToInt(*std::next(it));
		result.push_back((sixteens << 4) | units);
		i++;
	}
	return resultLength;
}
	
	
/**
 * Print a string as hexadecimal values.
 *
 * */ 
void printToHex(std::string s)
{
	std::cout << "printToHex() for " << s << ": ";
	for(size_t i = 0; i < s.size(); i++) {
		std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)s[i];
	}
	std::cout << '\n';
}

/**
 * Print a string as hexadecimal values using printf()
 *
 * */ 
void printToHexCStyle(std::string s)
{
	std::cout << "printToHex() for " << s << ": ";
	for(size_t i = 0; i < s.size(); i++) {
		printf("%02hhx", s[i]);
	}
	std::cout << '\n';
}

/**
 *
 * return a hex string representation of the value of an integer
 *
 * */ 
std::string intToHexString1(int num)
{
	std::stringstream ss;
	ss << std::hex << num;
	return ss.str();
}

/**
 * Convert an int to a hexadecimal string, using arithmetic.
 *
 * */
std::string intToHexString2(int num)
{
	const char hexChars[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	std::string result;
	while (num) {
		char tmp = hexChars[num % 16];
		result.insert(result.begin(), tmp);
		num /= 16;
	}
	return result;
}

/**
 * Build a hex string representation of bytes.
 * 
 * */
void charToHexString(const char& c, std::string& s)
{
	std::stringstream ss;
	ss << std::setfill('0') << std::setw(2) << std::hex << (0xff & (int)c);
	s.append(ss.str());
}

template <typename T>
void printHex(T input)
{
	for (auto& el : input)
		std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)el;
	std::cout << '\n';
}

} // utilities
#endif
