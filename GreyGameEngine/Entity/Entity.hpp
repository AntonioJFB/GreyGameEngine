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

private:
	template <typename T>
	using key_type = SlotMap<T, size_t, Capacity>::key_type; //TODO: El segundo parametro deberia ser del mismo tipo que el de los slotmaps de del componentSotrage. Deberia recibirlo por template parameter (?)

	//TODO: Cuando vea la metaprogramming, tengo que modificarlo
	using key_storageT = std::tuple<key_type<CMP0>, key_type<CMP1>, key_type<CMP2>>;

public:
	//Default constructor
	constexpr explicit Entity() = default;
	
	// == operator
	[[nodiscard]] inline bool operator==(Entity const& pOther) const noexcept { return this == &pOther; }
	
	//GetID
	inline size_t getID() const noexcept { return ID_; }

	//HasComponent
	[[nodiscard]] inline bool hasComponent(auto const pId) const noexcept { return (components_ & pId); }

private:
	inline static size_t nextID {0};

	size_t ID_{++nextID};

	uint8_t components_{};
	uint8_t tags_{};

	key_storageT key_storage{};
	
private:
	//GetComponent --> Me devuelve la key
	template<typename CMP>
	[[nodiscard]] constexpr auto getComponent() const noexcept;
	
	//TODO: GetComponents
	[[nodiscard]] constexpr auto const& getComponentKeys() const noexcept;

	//AddComponent --> Recibe la key como parametro y de momento el ID del component para añadirlo a la mascara
	template<typename CMP>
	[[nodiscard]] constexpr void addComponent(auto pSlotKey, auto pCMPId) noexcept;

	//RemoveComponent
	template<typename CMP>
	auto constexpr removeComponent(auto pCMPId) noexcept;

	//TODO: AddTag
	//TODO: AddTags
	//TODO: RemoveTag
	//TODO: RemoveTags
	//TODO: GetTag
	//TODO: GetTags
};

#include "Entity.tpp"