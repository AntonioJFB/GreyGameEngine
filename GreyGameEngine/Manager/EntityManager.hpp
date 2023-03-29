#pragma once
#include <cstdint>
#include <vector>
#include <array>

#include "../Metaprogramming/Metaprogramming.hpp"
#include "ComponentStorage.hpp"
#include "../Entity/Entity.hpp"

namespace GreyGameEngine
{
	template <typename CMPs, typename TAGs, size_t Capacity = 10>
	struct EntityManager
	{
		//Type used for storage the components
		using storage_type = ComponentStorage<CMPs, Capacity>;

		//Type of the Entities
		using Entity_t = Entity<CMPs, Capacity>;

		//Type of the Storage for the entities
		using EntitiesStorage_t = std::vector<Entity_t>;

		//Type traits for the Components List and Tags List
		using cmp_info = MP::cmp_traits<CMPs>;
		using tag_info = MP::tag_traits<TAGs>;

		/*
		* @brief Constructor of the EntitytManager. Reserves the initial size for the vector of entities.
		* @param pCapacity: Initial number of entities.
		*/
		constexpr explicit EntityManager(const size_t pCapacity = 5) noexcept;

		/*
		* @brief Method to create a new Entity and add it to the Entity Storage.
		* @return reference to the created Entity
		*/
		[[nodiscard]] constexpr Entity_t& createEntity() noexcept;

		/*
		* @brief Method to get a Reference to the Storage of Entities.
		* @return reference to the Storafge of Entities.
		*/
		[[nodiscard]] inline EntitiesStorage_t& getEntities() noexcept { return entities_; }

		/*
		* @brief Method to get a const Reference to the Storage of Entities.
		* @return const reference to the Storafge of Entities.
		*/
		[[nodiscard]] inline EntitiesStorage_t const& getEntities() const noexcept { return entities_; }

		/*
		* @brief Method to get a Reference to an Entity.
		* @param pID: ID of the entity you want to get
		* @return reference to the Entity.
		*/
		[[nodiscard]] inline Entity_t const* getEntityByID(auto const pID) const noexcept { return GetEntityByID_impl(this, pID); }

		/*
		* @brief Method to get a const Reference to an Entity.
		* @param pID: ID of the entity you want to get
		* @return const reference to the Entity.
		*/
		[[nodiscard]] inline Entity_t* getEntityByID(auto const pID) noexcept { return GetEntityByID_impl(this, pID); }

		/*
		* @brief Method to add a component to an Entity
		* @tparam CMP: type of the component to add
		* @param pEntity: Reference to the Entity that will have the component
		* @param pCmp: L Reference to the component to add
		* @return Reference to the new component
		*/
		template<typename CMP>
		[[nodiscard]] constexpr CMP& addComponent(Entity_t& pEntity, CMP& pComponent) noexcept;

		/*
		* @brief Method to add a component to an Entity
		* @tparam CMP: type of the component to add
		* @param pEntity: Reference to the Entity that will have the component
		* @param pCmp: Temporal Reference to the component to add
		* @return Reference to the new component
		*/
		template<typename CMP>
		[[nodiscard]] constexpr CMP& addComponent(Entity_t& pEntity, CMP&& pComponent) noexcept;

		/*
		* @brief Method to add a component to an Entity
		* @param pEntity: Reference to the Entity that will have the component
		* @tparam CMP: type of the component to add, ParamterTypes...: Types of the parameter pack
		* @param parameters: Parameter pack with the necessary parameters to create the Component
		* @return Reference to the new component
		*/
		template<typename CMP, typename... ParamTypes>
		[[nodiscard]] constexpr CMP& addComponent(Entity_t& pEntity, ParamTypes&&... pParams) noexcept;

		/*
		* @brief Method to remove a component from an Entity
		* @tparam CMP: type of the component to remove
		* @param pEntity: Reference to the Entity to remove the component
		*/
		template<typename CMP>
		void constexpr removeComponent(Entity_t& pEntity) noexcept;

		/*
		* @brief Method to remove a series of components from an Entity
		* @tparam CMPs: types of the components to remove
		* @param pEntity: Reference to the Entity to remove the components
		*/
		template<typename... CMPs>
		void constexpr removeComponents(Entity_t& pEntity) noexcept;

		/*
		* @brief Method to get a component from an Entity
		* @tparam CMP: type of the component to get
		* @param pEntity: Reference to the Entity that has the component
		* @return Reference to the component
		*/
		template<typename CMP>
		[[nodiscard]] inline CMP& getComponent(Entity_t const& pEntity) noexcept { return getComponent_impl<CMP>(pEntity, this); }

		/*
		* @brief Method to get a component from an Entity
		* @tparam CMP: type of the component to get
		* @param pEntity: Reference to the Entity that has the component
		* @return Const Reference to the component
		*/
		template<typename CMP>
		[[nodiscard]] inline CMP const& getComponent(Entity_t const& pEntity) const noexcept { return getComponent_impl<CMP>(pEntity, this); }

		/*
		* @brief Method to get all the components of type CMP
		* @tparam CMP: type of the components to get
		* @return Reference to the SlotMap of components
		*/
		template<typename CMP>
		[[nodiscard]] inline auto& getComponents() noexcept { return getComponents_impl<CMP>(this); }

		/*
		* @brief Method to get all the components of type CMP
		* @tparam CMP: type of the components to get
		* @return Const Reference to the SlotMap of components
		*/
		template<typename CMP>
		[[nodiscard]] inline auto const& getComponents() const noexcept { return getComponents_impl<CMP>(this); }

		//TODO: HasComponent
		template<typename CMP>
		[[nodiscard]] constexpr bool hasComponent(Entity_t& pEntity) const noexcept;

		//TODO: Todo lo relacionado con los Tags está cogido con pinzas porque lo voy a cambiar con metaprogramming
		//TODO: AddTag
		template <typename... Ts>
		constexpr void addTags(Entity_t& pEntity) noexcept;
		//TODO: RemoveTag 
		//TODO: Podria hacer RemoveTags
		template <typename... Ts>
		constexpr void removeTags(Entity_t& pEntity) noexcept;
		//TODO: HasTag
		template <typename... Ts>
		constexpr bool hasTags(Entity_t& pEntity) noexcept;


		//TODO: Todo lo del forAll lo tengo que hacer bien una vez tenga sistemas y metaprogramming funcionando
		//TODO: Forall
		constexpr void forAll() noexcept;
		//TODO: ForAllMatching
		//TODO: ForAllMatchingPairs

		/*
		* @brief Method that checks if there are entities marked as dead.
		* If an Entity is marked with an invalid ID, it erases all the components of the entity.
		* If there are Dead entities, it calls removeDeadEntites.
		*/
		void constexpr checkDeadEntities() noexcept;

		/*
		* @brief Method that sets to an invalid ID the ID of an entity to mark it as dead.
		* @param pEntity: Reference to the Entity to be marked as dead
		*/
		constexpr void setEntitiyForDestroy(Entity_t& pEntity) noexcept { pEntity.setID(NON_VALID_ENTITY_ID); }

	private:

		//Vector of entities
		EntitiesStorage_t	entities_{};
		/*TODO: El problema de usar vector, es que si se hace resize y tengo guardada una referencia a una entidad,
				esa referencia deja de ser valida porque los objetos se han movido de sitio.
				Si borro entidades y hago un shrink to fit, los objetos tambien se van a mover y si tengo guardadas referencias
				a esos objetos a lo mejor me las cargo tmb.
				Llevar cuidado y no guardarme referencias de objetos sueltos por ahi porque pueden cambiar.
				-------------------------------------------
				El problema de usar array es que no puedo hacer resize si llega un momento en el que necesito mas entidades.
				Si lo hago con array, a la hora de borrar y añadir nuevas entidades necesito un control de posiciones libres y demas
				con indicadores de tamanyo y proxima posicion libre. Tipo como con el slotmap.
		*/

		//Components storage
		storage_type		components_{};

		/*
		* @brief Implementation Method to get an Entity given an ID
		* @param self: Pointer to this to know if it is const or not
		* @param pID: ID of the entity to get
		* @return Const/Or no const Reference to the Entity
		*/
		[[nodiscard]] static constexpr auto* GetEntityByID_impl(auto* self, const auto pID) noexcept;

		/*
		* @brief Implementation Method to get a Component given an Entity
		* @tparam CMP Type of the CMP to get
		* @param pEntity: ID of the entity to get
		* @param self: Pointer to this to know if it is const or not
		* @return Const/Or no const Reference to the Component
		*/
		template<typename CMP>
		[[nodiscard]] static constexpr auto& getComponent_impl(Entity_t const& pEntity, auto* self) noexcept;

		/*
		* @brief Implementation Method to get all Components given an Entity
		* @tparam CMP Type of the CMPs to get
		* @param self: Pointer to this to know if it is const or not
		* @return Const/Or no const Reference to the SlotMap of Components
		*/
		template<typename CMP>
		[[nodiscard]] static constexpr auto& getComponents_impl(auto* self) noexcept;


		//USANDO EL TAG DISPATCHING --> TODO: LUEGO ESTO SE AUTOMATIZA CON METAPROGRAMMING
		//TODO: Esto a lo mejor deberia moverlo al component storage. De momento lo dejo aqui
		//TODO: SI quiero acceder a los elementos del component estorage haciendo uso del ID estas funciones y las dos publicas de arriba deberian ser static.

		//TODO: Creo que ya no me hace falta implementar el getId ni el getMask porque uso los traits y la MP

		//TODO: ForAll_impl
		using TypeProcessFunc = void(*)(Entity_t&);
		constexpr void forAll_impl(TypeProcessFunc process) noexcept;
		//TODO: ForAllMatchin_impl
		//TODO: ForAllMatchinPairs_impl

		//TODO: IsEntityMatchingTheSignature

		/*
		* @brief Method that erases all the Dead Entities from entities_
		*/
		constexpr void removeDeadEntities() noexcept;
	};
}

#include "EntityManager.tpp"