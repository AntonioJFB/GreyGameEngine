#pragma once
#include <cstdint>
#include <array>
#include <tuple>

#include "../Utils/DataStructures/SlotMap.h"

#define NON_VALID_ENTITY_ID 0

template<typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
class Entity
{
public:
	template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
	friend struct EntityManager;

	template <typename T>
	using key_type = SlotMap<T, uint8_t, Capacity>::key_type;

	//TODO: Cuando vea la metaprogramming, tengo que modificarlo
	using key_storageT = std::tuple<key_type<CMP0>, key_type<CMP1>, key_type<CMP2>>;

	//Default constructor
	constexpr explicit Entity() = default;
	
	// == operator
	inline bool operator==(Entity const& pOther) const noexcept { return this == &pOther; }
	
	//GetID
	inline size_t getID() const noexcept { return ID_; }

	//HasComponent
	[[nodiscard]] inline bool hasComponent(auto const pId) const noexcept { return (components_ & pId); }

	//GetComponent --> Me devuelve la key
	template<typename CMP>
	[[nodiscard]] constexpr auto getComponent() const noexcept;

	//AddComponent --> Recibe la key comop parametro y de momento el ID del component para añadirlo a la mascara
	template<typename CMP>
	[[nodiscard]] constexpr void addComponent(auto pSlotKey, auto pCMPId) noexcept;

private:
	inline static size_t nextID {0};

	size_t ID_{++nextID};

	uint8_t components_{};
	uint8_t tags_{};

	key_storageT key_storage{};

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