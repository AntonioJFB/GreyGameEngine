#pragma once

#include <array>

template <typename CMP, typename key_type, typename Capacity = 100>
struct SlotMap
{
	constexpr explicit SlotMap() noexcept = default;

	struct Key {
		key_type id_;
		key_type gen_;
	};

	key_type generation_{};
	key_type freeList_{};

	std::array<Key, Capacity> slots_{};
	std::array<CMP, Capacity> data_{};
	std::array<Key, Capacity> erase_{};
};