#pragma once

namespace MemViwer
{
	constexpr uint8_t defaultWidth{ 16 };

	static size_t calculateLines(size_t const size, size_t const width = defaultWidth) noexcept
	{
		auto const lines{ size / width };
		if ((size & 0xF) != 0)
			return lines + 1;
		return lines;
	}

	static void showMemoryLine(uint8_t const* memPtr, size_t const width) noexcept
	{
		std::printf("%16p ||", (void*)memPtr);
		for (size_t i{ 0 }; i < width; ++i) { std::printf(" %02X", memPtr[i]); }
		std::printf(" || ");
		for (size_t i{ 0 }; i < width; ++i)
		{
			auto c = (memPtr[i] > 31 && memPtr[i] < 128) ? memPtr[i] : '.';
			std::printf("%c", c);
		}
		std::printf("||\n");
	}

	static void showMem(uint8_t const* memPtr, size_t const size, size_t const width = defaultWidth) noexcept
	{
		std::printf("%p\n", (void*)memPtr);
		std::printf("     ADDRESS    || 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
		std::printf("----------------||------------------------------------------------\n");
		auto const lines{ calculateLines(size, width) };
		for (size_t i{ 0 }; i < lines; ++i)
		{
			showMemoryLine(memPtr, width);
			memPtr += width;
		}
		std::printf("----------------||------------------------------------------------\n");
	}

	static void showMemPtr(auto const* ptr, size_t const size) noexcept
	{
		auto* p = reinterpret_cast<uint8_t const*>(ptr);
		std::printf("####### SHOWING POINTER. Size: %zd\n", size);
		showMem(p, size);

	}

	static void showMemObj(auto const& obj) noexcept
	{
		auto* p = reinterpret_cast<uint8_t const*>(&obj);
		std::printf("####### SHOWING OBJECT. Size: %zd\n", sizeof(obj));
		showMem(p, sizeof(obj));

	}
};