#include <math.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <assert.h>
#include <cstdint>

std::string EncodeBase58(const uint8_t* pbegin, const uint8_t* pend);
std::string EncodeBase58(const std::vector<uint8_t>& inputBuffer);
