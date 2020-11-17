// Copyright (c) 2014-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "base58.h"

static const char* pszBase58 = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz"; 

std::string EncodeBase58(const uint8_t* pbegin, const uint8_t* pend)
{
	// Skip & count leading zeroes.
	int zeroes = 0;
	int length = 0;
	while (pbegin != pend && *pbegin == 0) {
		pbegin++;
		zeroes++;
	}
	// Allocate enough space in big-endian base58 representation.
	int size = (pend - pbegin) * 138 / 100 + 1; // log(256) / log(58), rounded up.
	std::vector<uint8_t> b58(size);
	
	// Process the bytes.
	while (pbegin != pend) {
		// carry is the current byte value as an int
		int carry = *pbegin;
		int i = 0;
		
		// Apply "b58 = b58 * 256 + ch".
		// Do not advance the iterator through the results vector if carry == 0 or if i is less than length
		//
		// -------------------
		std::vector<uint8_t>::reverse_iterator it;
		for (it = b58.rbegin(); (carry != 0 || i < length) && (it != b58.rend()); it++, i++) {
			carry += 256 * (*it);
			// Set this elemend of the b58 vector to the remainder of 
			*it = carry % 58;
			carry /= 58;
		}

		assert(carry == 0);
		length = i;
		pbegin++;
	}
	// Skip leading zeroes in base58 result.
	std::vector<uint8_t>::iterator it = b58.begin() + (size - length);
	while (it != b58.end() && *it == 0)
		it++;
	// Translate the result into a string.
	std::string str;
	str.reserve(zeroes + (b58.end() - it));
	str.assign(zeroes, '1');
	while (it != b58.end())
		str += pszBase58[*(it++)];
	return str;
}

std::string EncodeBase58(const std::vector<uint8_t>& vch)
{
	return EncodeBase58(vch.data(), vch.data() + vch.size());
}

