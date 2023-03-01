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
	//Friend class EntityManager tyoe
	template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
	friend struct EntityManager;

private:
	//Type of the key_strage of component Keys
	template <typename T>
	using key_type = SlotMap<T, size_t, Capacity>::key_type; //TODO: El segundo parametro deberia ser del mismo tipo que el de los slotmaps de del componentSotrage. Deberia recibirlo por template parameter (?)

	//TODO: Cuando vea la metaprogramming, tengo que modificarlo
	//Key Storage type
	using key_storageT = std::tuple<key_type<CMP0>, key_type<CMP1>, key_type<CMP2>>;

public:
	/*
	*@brief Default constructor of Entity
	*/
	constexpr explicit Entity() = default;
	
	/*
	*@brief Operator ==. Compares if the memory address of this is iqual to the one of the objesct passed by parameter
	* @param pOther Reference to other entity to compare with this
	* @return true if both memory addresses are the same. False if not.
	*/
	[[nodiscard]] inline bool operator==(Entity const& pOther) const noexcept { return this == &pOther; }
	
	/*
	*@brief Method to get the ID of an Entity
	*/
	inline size_t getID() const noexcept { return ID_; }

	/*
	*@brief Method to set the ID of an Entity
	*/
	inline void setID(size_t pID) noexcept { ID_ = pID; }

	/*
	*@brief Method to know if an Entity has a component.
	* @param pId: ID of the component to check
	* @return true if has the component. False if not.
	*/
	[[nodiscard]] inline bool hasComponent(auto const pId) const noexcept { return (components_ & pId); }

private:
	//Counter of entities and used as ID
	inline static size_t nextID {0};

	//ID of the Entity. Increases automatically when an entity is created.
	size_t ID_{++nextID};

	//Components bit mask
	using cmp_mask_type = uint8_t;
	uint8_t components_{};
	
	//Tagas bit mask
	using tag_mask_type = uint8_t;
	uint8_t tags_{};

	//Storage of Component keys
	key_storageT key_storage{};
	
private:
	/*
	*@brief Method to get the key of the component of type CMP
	* @tparam CMP type of the component
	* @return Key to access the component.
	*/
	template<typename CMP>
	[[nodiscard]] constexpr auto getComponent() const noexcept;
	
	/*
	*@brief Method to get all the component keys
	* @return const reference to the key storage
	*/
	[[nodiscard]] inline auto const& getComponentKeys() const noexcept { return key_storage; }

	/*
	*@brief Method to add a component to an Entity.
	* @tparam CMP type of the component
	* @param pSlotKey Key to add to component keys
	* @param pCMPId ID of the component to add to the mask
	*/
	template<typename CMP>
	[[nodiscard]] constexpr void addComponent(auto pSlotKey, auto pCMPMask) noexcept;

	/*
	*@brief Method to remove a component from an Entity.
	* @tparam CMP type of the component
	* @param pCMPId ID of the component to be removed form the mask
	*/
	template<typename CMP>
	auto constexpr removeComponent(auto pCMPMask) noexcept;

	//TODO: Todo lo relacionado con los tags esta cogido con pinzas porque lo voy a cambiar con metaprogramming
	//TODO: AddTag
	inline void addTag(tag_mask_type const pTagMask) noexcept { tags_ |= pTagMask; }
	//TODO: AddTags
	// 
	//TODO: RemoveTag
	inline void removeTag(tag_mask_type const pTagMask) noexcept { tags_ ^= pTagMask; }
	//TODO: RemoveTags
	// 
	//TODO: HasTag
	[[nodiscard]] inline bool hasTag(tag_mask_type const pTagMask) noexcept { tags_& pTagMask; }
	//TODO: GetTags
	[[nodiscard]] inline tag_mask_type getTags() const noexcept { return tags_; }
};

#include "Entity.tpp"