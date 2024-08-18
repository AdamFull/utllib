#include "uuid.hpp"

namespace utl
{
	std::string to_string(const guid& g)
	{
		std::ostringstream oss;
		oss << std::hex << std::setfill('0')
			<< std::setw(8) << g.d1 << '-'
			<< std::setw(4) << g.d2 << '-'
			<< std::setw(4) << g.d3 << '-'
			<< std::setw(2) << static_cast<int>(g.d4[0])
			<< std::setw(2) << static_cast<int>(g.d4[1]) << '-';

		for (int i = 2; i < 8; ++i)
			oss << std::setw(2) << static_cast<int>(g.d4[i]);

		return oss.str();
	}

	guid generate_guid()
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<uint32_t> dist32;
		static std::uniform_int_distribution<uint16_t> dist16;
		std::uniform_int_distribution<uint32_t> dist8(0, 0xFF);

		guid result;
		result.d1 = dist32(gen);
		result.d2 = dist16(gen);
		result.d3 = dist16(gen);

		// Setting the UUID version (4) in d3's first nibble
		result.d3 = (result.d3 & 0x0FFF) | 0x4000;

		// Generating d4 and setting the UUID variant (variant 1) in d4[0]'s first two bits
		result.d4[0] = dist8(gen);
		result.d4[1] = dist8(gen);
		result.d4[0] = (result.d4[0] & 0x3F) | 0x80; // Variant 1
		for (int i = 2; i < 8; ++i)
			result.d4[i] = dist8(gen);

		return result;
	}
}