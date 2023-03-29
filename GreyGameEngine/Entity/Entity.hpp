#pragma once
#include <cstdint>
#include <array>
#include <tuple>

#include "../Utils/DataStructures/SlotMap.h"

#define NON_VALID_ENTITY_ID 0

namespace GreyGameEngine
{
	template<typename CMPs, size_t Capacity>
	class Entity
	{
	public:
		//Friend class EntityManager tyoe
		template <typename CMPs, typename TAGs, size_t Capacity>
		friend struct EntityManager;

		using cmp_info = MP::cmp_traits<CMPs>;

	private:
		using mask_type = cmp_info::mask_type;

		//Type of the key_strage of component Keys
		template <typename T>
		using to_key_type = SlotMap<T, mask_type, Capacity>::key_type;

		template<typename List>
		using to_key_tuple = MP::replace_t<std::tuple, List>;

		//TODO: Probar que funcione
		//Key Storage type
		using key_storageT = to_key_tuple<MP::forall_insert_template_t<to_key_type, CMPs>>;

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
		inline mask_type getID() const noexcept { return ID_; }

		/*
		*@brief Method to set the ID of an Entity
		*/
		inline void setID(mask_type pID) noexcept { ID_ = pID; }

		/*
		*@brief Method to know if an Entity has a component.
		* @param pId: ID of the component to check
		* @return true if has the component. False if not.
		*/
		[[nodiscard]] inline bool hasComponent(auto const pId) const noexcept { return (components_ & pId); }

		/*
		*@brief Method to get the CMPs Mask.
		* @return cmp_mask_type that signals the components that has the Entitiy
		*/
		[[nodiscard]] inline auto getComponentsMask() const noexcept { return components_; }

		//TODO: HasTag
		[[nodiscard]] inline bool hasTag(auto const pTagMask) noexcept { return tags_ & pTagMask; }
		//TODO: GetTags
		[[nodiscard]] inline auto getTagsMask() const noexcept { return tags_; }

	private:
		//Counter of entities and used as ID
		inline static size_t nextID{ 0 };

		//ID of the Entity. Increases automatically when an entity is created.
		size_t ID_{ ++nextID };

		//Components bit mask
		mask_type components_{};

		//Tagas bit mask
		mask_type tags_{};

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
		inline void addTag(mask_type const pTagMask) noexcept { tags_ |= pTagMask; }
		//TODO: AddTags
		// 
		//TODO: RemoveTag
		inline void removeTag(mask_type const pTagMask) noexcept { tags_ ^= pTagMask; }
		//TODO: RemoveTags
		// 

	};
}

#include "Entity.tpp"