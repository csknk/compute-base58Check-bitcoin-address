Compute Bitcoin Base58Check Address
===================================
Bitcoin addresses are computed from an initial public key or redeem script in the case of Pay To Public Key Hash (P2PKH) and Pay-To-Script-Hash (P2SH) respectively.

The address is computed by: 

1: SHA256 hashing the input bytes
2: RIPEMD160 hashing the result of step 1
3. Prepending the network byte to the hash computed in step 2
4. Performing SHA256 twice on the concatenated (network byte || RIPEMD160 hash) from step 3 - the checksum is the first four bytes of the result
5. Appending the 4 byte checksum to the concatenated (network byte || RIPEMD160 hash), resulting in 25 bytes
6. Base58 encoding the result


	
