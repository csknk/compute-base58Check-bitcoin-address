#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include "utilities.h"
#include "base58.h"

using namespace utilities;
int main(int argc, char *argv[])
{
	uint8_t networkByte = 0x00;
	if (argc == 2 && std::string(argv[1]) == "-regtest") {
		networkByte = 0xc4;
	}
	std::string hexstring;
	std::cout << "Enter a hexstring representation of a public key,"
		"or a hexstring representation of a P2SH multisig redeem script:\n";
	std::cin >> hexstring;
	Bytes input;
	hexstringToBytes(hexstring, input);

	// Bytes is a std::vector<uint8_t>. Initialise to the exact required length.
	// If the container is too big it will contain zero bytes that will get hashed!
	Bytes initialSHA256(SHA256_DIGEST_LENGTH);
	
	// Note that sha256() expects a pointer as the first argument.
	// Alternative: sha256(&input[0], size, initialSHA256);
	sha256(input.data(), input.size(), initialSHA256);
	std::cout << "\nFirst SHA256 hash:\t";
	printHex(initialSHA256);
	
	Bytes ripemd160Hash(RIPEMD160_DIGEST_LENGTH);
	ripemd160(&initialSHA256[0], initialSHA256.size(), ripemd160Hash);
	std::cout << "First RIPEMD160 Hash:\t";
	printHex(ripemd160Hash);

	// Prepend the version byte
	ripemd160Hash.insert(std::begin(ripemd160Hash), networkByte);

	// Perform SHA256 twice  - checksum is the first four bytes of the result.
	Bytes checksumHash(SHA256_DIGEST_LENGTH);
	sha256(&ripemd160Hash[0], ripemd160Hash.size(), checksumHash);
	sha256(&checksumHash[0], checksumHash.size(), checksumHash);
	std::cout << "Checksum hashed twice:\t";
	printHex(checksumHash);

	// Get first 4 bytes - the checksum
	Bytes::const_iterator first = checksumHash.begin();
	Bytes::const_iterator last = first + 4;

	// Append checksum to ripemd160Hash
	ripemd160Hash.insert(ripemd160Hash.end(), first, last);
	std::cout << "Checksum appended:\t";
	printHex(ripemd160Hash);
	
	std::string b58 = EncodeBase58(ripemd160Hash);
	std::cout << "Base58 encoded:\t\t";
	std::cout << b58 << '\n';

	return 0;
}
