#include <cassert>
#include <cstdio>

namespace GreyGameEngine
{
	template<typename CMPs, typename TAGs, size_t Capacity>
	constexpr EntityManager<CMPs, TAGs, Capacity>::EntityManager(const size_t pCapacity) noexcept
	{
		entities_.reserve(pCapacity);
	}

	//=============================================================================

	template<typename CMPs, typename TAGs, size_t Capacity>
	constexpr typename EntityManager<CMPs, TAGs, Capacity>::Entity_t&
	EntityManager<CMPs, TAGs, Capacity>::createEntity() noexcept
	{
		return entities_.emplace_back();
	}

	//=============================================================================

	template<typename CMPs, typename TAGs, size_t Capacity>
	constexpr auto*
	EntityManager<CMPs, TAGs, Capacity>::GetEntityByID_impl(auto* self, const auto pID) noexcept
	{
		assert(("There aren't entities!", self->entities_.size()));

		decltype(&self->entities_[0]) entity{ nullptr };

		for (auto& e : self->entities_)
		{
			if (e.getID() == pID)
				return &e;
		}

		return entity;
	}

	//=============================================================================

	template<typename CMPs, typename TAGs, size_t Capacity>
	template <typename CMP>
	constexpr CMP&
	EntityManager<CMPs, TAGs, Capacity>::addComponent(Entity_t& pEntity, CMP& pComponent) noexcept
	{
		return addComponent<CMP>(pEntity, CMP{ pComponent });
	}

	//=============================================================================

	template<typename CMPs, typename TAGs, size_t Capacity>
	template <typename CMP, typename... ParamTypes>
	constexpr CMP&
	EntityManager<CMPs, TAGs, Capacity>::addComponent(Entity_t& pEntity, ParamTypes&&... pParams) noexcept
	{
		return addComponent<CMP>(pEntity, CMP{ std::forward<ParamTypes>(pParams)... });

		//TODO: Otra opcion a parte podria ser hacer como abajo y pasar los params... para la funcion del componentStorage que tmb recibe los params asi
	}

	//=============================================================================

	template<typename CMPs, typename TAGs, size_t Capacity>
	template <typename CMP>
	constexpr CMP&
	EntityManager<CMPs, TAGs, Capacity>::addComponent(Entity_t& pEntity, CMP&& pComponent) noexcept
	{
		//1.- Deberia comprobar si la entidad ya tiene ese componente
		auto cmpMask = cmp_info::template mask<CMP>();

		if (pEntity.hasComponent(cmpMask))
		{
			//2.- Si lo tiene, deberia devolver el que tiene
			auto key = pEntity.getComponent<CMP>();
			return components_.getComponent<CMP>(key);
		}

		//3.- Si no lo tiene deberia crearlo y devolverlo
		auto key = components_.addComponent<CMP>(pComponent);

		pEntity.addComponent<CMP>(key, cmpMask);

		return components_.getComponent<CMP>(key);
	}

	//=============================================================================

	template<typename CMPs, typename TAGs, size_t Capacity>
	template <typename CMP>
	constexpr void
	EntityManager<CMPs, TAGs, Capacity>::removeComponent(Entity_t& pEntity) noexcept
	{
		auto cmpMask = cmp_info::template mask<CMP>(); 

		if(pEntity.hasComponent(cmpMask))
		{
			auto key = pEntity.removeComponent<CMP>(cmpMask);
			components_.removeComponent<CMP>(key);
		}
	}

	//=============================================================================

	template<typename CMPs, typename TAGs, size_t Capacity>
	template <typename... Ts>
	constexpr void
	EntityManager<CMPs, TAGs, Capacity>::removeComponents(Entity_t& pEntity) noexcept
	{
		(removeComponent<Ts>(pEntity), ...);
	}

	//=============================================================================

	template<typename CMPs, typename TAGs, size_t Capacity>
	template <typename CMP>
	constexpr bool
	EntityManager<CMPs, TAGs, Capacity>::hasComponent(Entity_t& pEntity) const noexcept
	{
		auto cmpMask = cmp_info::template mask<CMP>();
		return pEntity.hasComponent(cmpMask);
	}

	//=============================================================================

	template<typename CMPs, typename TAGs, size_t Capacity>
	template <typename CMP>
	constexpr auto&
	EntityManager<CMPs, TAGs, Capacity>::getComponent_impl(Entity_t const& pEntity, auto* self) noexcept
	{
		auto key = pEntity.getComponent<CMP>();
		return self->components_.getComponent<CMP>(key);
	}

	//=============================================================================

	template<typename CMPs, typename TAGs, size_t Capacity>
	template <typename CMP>
	constexpr auto&
	EntityManager<CMPs, TAGs, Capacity>::getComponents_impl(auto* self) noexcept
	{
		return self->components_.getComponents<CMP>();
	}

	//=============================================================================

	template<typename CMPs, typename TAGs, size_t Capacity>
	template <typename TAG>
	constexpr void
	EntityManager<CMPs, TAGs, Capacity>::addTag(Entity_t& pEntity) noexcept
	{
		auto tagMask = tag_info::template mask<TAG>();
		pEntity.addTag(tagMask);
	}

	//=============================================================================

	template<typename CMPs, typename TAGs, size_t Capacity>
	template <typename TAG>
	constexpr void
	EntityManager<CMPs, TAGs, Capacity>::removeTag(Entity_t& pEntity) noexcept
	{
		auto tagMask = tag_info::template mask<TAG>();
		pEntity.removeTag(tagMask);
	}

	//=============================================================================

	template<typename CMPs, typename TAGs, size_t Capacity>
	template <typename TAG>
	constexpr bool
	EntityManager<CMPs, TAGs, Capacity>::hasTag(Entity_t& pEntity) noexcept
	{
		auto tagMask = tag_info::template mask<TAG>();
		return pEntity.hasTag(tagMask);

	}

	//=============================================================================

	template<typename CMPs, typename TAGs, size_t Capacity>
	constexpr void
	EntityManager<CMPs, TAGs, Capacity>::checkDeadEntities() noexcept
	{
		bool thereAreDeadEntities{ false };

		for (auto& e : entities_)
		{
			if (e.getID() == NON_VALID_ENTITY_ID)
			{
				thereAreDeadEntities = true;
				removeComponents<CMPs>(e); //TODO: Comprobar que esto funciona
			}
		}

		if (thereAreDeadEntities)
			removeDeadEntities();
	}

	//=============================================================================

	template<typename CMPs, typename TAGs, size_t Capacity>
	constexpr void
	EntityManager<CMPs, TAGs, Capacity>::removeDeadEntities() noexcept
	{
		entities_.erase(std::remove_if(entities_.begin(), entities_.end(),
			[](Entity_t const& e) {
				return e.getID() == NON_VALID_ENTITY_ID;
			}),
			entities_.end());
	}

	//=============================================================================

	template<typename CMPs, typename TAGs, size_t Capacity>
	constexpr void
	EntityManager<CMPs, TAGs, Capacity>::forAll() noexcept
	{
		//SI le pongo una captura, el lambda ya no es convertible a puntero a funcion.
		//Un lambda  es un struct con el operador llamada, si le pongo captura va a tener variables miembro.
		//Si no le pongo captura, el objeto solo tiene la funcion y eso si que es convertible a puntero a funcion.
		//Un struct que lleva variables miembro no lo puedo convertir a funcion. Pero un struct que solo lleva
		//una funcion si qu elo puedo convertir a funcion.

		//TODO: Esto es una implementacion de juguete
		forAll_impl([](Entity_t& e) {
			std::cout << "Entity_" << e.getID() << "\n";
			std::cout << "Components: " << e.getComponentsMask() << "\n";
			std::cout << "Tags:" << e.getTagsMask() << "\n";
			});
	}

	//=============================================================================

	template<typename CMPs, typename TAGs, size_t Capacity>
	constexpr void
	EntityManager<CMPs, TAGs, Capacity>::forAll_impl(TypeProcessFunc process) noexcept
	{
		//TODO: Esto es una implementacion de juguete

		for (auto& e : entities_)
			process(e);
	}
}