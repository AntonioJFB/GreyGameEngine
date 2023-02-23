#pragma once
#include <cstdint>
#include <array>

#include "ComponentStorage.hpp"
#include "../Entity/Entity.hpp"

template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity = 5>
struct EntityManager
{
	using storage_type = ComponentStorage<CMP0, CMP1, CMP2, Capacity>;

	//Funcion para coger un componente
	template<typename CMP>
	[[nodiscard]] constexpr CMP& getComponent(Entity&) noexcept;

	//Funcion para coger un componente version const --> Tengo que hacer la de impl
	template<typename CMP>
	[[nodiscard]] constexpr CMP const& getComponent(Entity const&) const noexcept;


	//Funcion para saber si tiene un componente --> TODO: Luego evolucionar a hasComponents con un parameter pack
	template<typename CMP>
	[[nodiscard]] constexpr bool hasComponent() const noexcept;

	//Funcion para saber si tiene un componente --> TODO: Luego evolucionar a hasTags con un parameter pack
	template<typename TAG>
	[[nodiscard]] constexpr bool hasTag() const noexcept;

	//Funcion para coger el ID
	[[nodiscard]] inline size_t getID(Entity const& pEntity) const noexcept { return pEntity.ID_; }

	//USANDO EL TAG DISPATCHING --> TODO: LUEGO ESTO SE AUTOMATIZA CON METAPROGRAMMING
	[[nodiscard]] inline uint8_t getCMPId(CMP0) const noexcept { return 0 };
	[[nodiscard]] inline uint8_t getCMPId(CMP1) const noexcept { return 1 };
	[[nodiscard]] inline uint8_t getCMPId(CMP2) const noexcept { return 2 };

	//TODO: CreateEntity
	//TODO: SetEntityID
	//TODO: GetEntities
	//TODO: GetEntityByID
	//TODO: GetEntityByName
	//TODO: SetEntityForDestroy
	//TODO: Update



private:

	std::array<Entity, Capacity> entities_{};
	storage_type components_{};
	

	
	//TODO: GetComponent_impl
	//TODO: GetComponents_impl
	//TODO: ForAllMatchin_impl
	//TODO: ForAllMatchinPairs_impl
	//TODO: IsEntityMatchingTheSignature
	//TODO: GetEntityByID_impl
	//TODO: GetEntityByName_impl
	//TODO: RemoveComponentFromEntity
	//TODO: RemoveComponentsFromEntity
	//TODO: RemoveDeadEntities





};

#include "EntityManager.tpp"