#pragma once
#include <cstdint>
#include <array>

#include "../Utils/DataStructures/SlotMap.h"

template<typename CMP0, typename CMP1, typename CMP2, size_t Capacity = 5>
class Entity
{
public:
	template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
	friend class EntityManager;

	using key_storage = SlotMap::key_type;

	//TODO: Default constructor
	//TODO: == operator
	
	//TODO: GetID
	
private:
	inline static size_t nextID {0};

	size_t ID_{++nextID};

	uint8_t components_{};
	uint8_t tags_{};

	std::array<key_storage, Capacity> component_keys_{};

	//TODO: AddComponent
	//TODO: RemoveComponent
	//TODO: GetComponent
	//TODO: GetComponents
	//TODO: RemoveComponent

	//TODO: AddTag
	//TODO: AddTags
	//TODO: RemoveTag
	//TODO: RemoveTags
	//TODO: GetTag
	//TODO: GetTags
};