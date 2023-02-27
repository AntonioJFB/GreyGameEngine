#pragma once
#include <cstdint>
#include <vector>
#include <array>

#include "ComponentStorage.hpp"
#include "../Entity/Entity.hpp"

template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
struct EntityManager
{
	using storage_type = ComponentStorage<CMP0, CMP1, CMP2, Capacity>;
	using Entity_t = Entity<CMP0, CMP1, CMP2, Capacity>;
	using EntitiesStorage_t = std::vector<Entity_t>;

	constexpr explicit EntityManager(const size_t pCapacity = 5) noexcept;

	//CreateEntity
	[[nodiscard]] constexpr Entity_t& createEntity() noexcept;

	//GetEntities
	[[nodiscard]] inline EntitiesStorage_t& getEntities() noexcept { return entities_; }
	[[nodiscard]] inline EntitiesStorage_t const& getEntities() const noexcept { return entities_; }

	//GetEntityByID
	[[nodiscard]] inline Entity_t const* getEntityByID(auto const pID) const noexcept { return GetEntityByID_impl(this, pID); }
	[[nodiscard]] inline Entity_t* getEntityByID(auto const pID) noexcept { return GetEntityByID_impl(this, pID); }

	//AddComponent
	template<typename CMP>
	[[nodiscard]] constexpr CMP& addComponent(Entity_t& pEntity, CMP& pComponent) noexcept;

	template<typename CMP>
	[[nodiscard]] constexpr CMP& addComponent(Entity_t& pEntity, CMP&& pComponent) noexcept;

	template<typename CMP, typename... ParamTypes>
	[[nodiscard]] constexpr CMP& addComponent(Entity_t& pEntity, ParamTypes&&... pParams) noexcept;

	//RemoveComponentFromEntity
	template<typename CMP>
	void constexpr removeComponent(Entity_t& pEntity) noexcept;

	//TODO: RemoveComponentsFromEntity
	template<typename... CMPs>
	void constexpr removeComponents (Entity_t& pEntity) noexcept;

	//Funcion para coger un componente
	template<typename CMP>
	[[nodiscard]] inline CMP& getComponent(Entity_t const& pEntity) noexcept { return getComponent_impl<CMP>(pEntity, this); }

	//Funcion para coger un componente version const --> Tengo que hacer la de impl
	template<typename CMP>
	[[nodiscard]] inline CMP const& getComponent(Entity_t const& pEntity) const noexcept { return getComponent_impl<CMP>(pEntity, this); }

	//Funcion para coger todos los componentes del mismo tipo--> Tengo que hacer la de impl
	template<typename CMP>
	[[nodiscard]] inline auto& getComponents() noexcept { return getComponents_impl<CMP>(this); }
	
	//Funcion para coger todos los componentes del mismo tipo const --> Tengo que hacer la de impl
	template<typename CMP>
	[[nodiscard]] inline auto const& getComponents() const noexcept { return getComponents_impl<CMP>(this); }

	//TODO: SetEntityForDestroy
	//TODO: Update

private:

	EntitiesStorage_t	entities_{}; //TODO: Mas adelante, cuando la cosa ya este furulando, probar a cambiarlo por array
	storage_type		components_{};
	
	//GetEntityByID_impl
	[[nodiscard]] static constexpr auto* GetEntityByID_impl(auto* slef, const auto pID) noexcept;

	//Funcion para coger un componente _impl
	template<typename CMP>
	[[nodiscard]]static constexpr auto& getComponent_impl(Entity_t const& pEntity, auto* self) noexcept;

	//Funcion para coger un componente _impl
	template<typename CMP>
	[[nodiscard]] static constexpr auto& getComponents_impl(auto* self) noexcept;

	//USANDO EL TAG DISPATCHING --> TODO: LUEGO ESTO SE AUTOMATIZA CON METAPROGRAMMING
	[[nodiscard]] inline uint8_t getCMPId(CMP0) const noexcept { return 1; }
	[[nodiscard]] inline uint8_t getCMPId(CMP1) const noexcept { return 2; }
	[[nodiscard]] inline uint8_t getCMPId(CMP2) const noexcept { return 4; }
	
	//TODO: GetComponents_impl
	//TODO: ForAllMatchin_impl
	//TODO: ForAllMatchinPairs_impl
	
	//TODO: IsEntityMatchingTheSignature
	//TODO: RemoveDeadEntities
};

#include "EntityManager.tpp"