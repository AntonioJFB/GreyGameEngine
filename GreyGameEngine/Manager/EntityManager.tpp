#include <cassert>

template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
constexpr EntityManager<CMP0, CMP1, CMP2, Capacity>::EntityManager(const size_t pCapacity) noexcept
{
	entities_.reserve(pCapacity);
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
constexpr typename EntityManager<CMP0, CMP1, CMP2, Capacity>::Entity_t& 
EntityManager<CMP0, CMP1, CMP2, Capacity>::createEntity() noexcept
{
	return entities_.emplace_back();
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
constexpr auto*
EntityManager<CMP0, CMP1, CMP2, Capacity>::GetEntityByID_impl(auto* self, const auto pID) noexcept
{
	assert(("There aren't entities!", self->entities_.size()));

	decltype(&self->entities_[0]) entity {nullptr};

	for(auto& e : self->entities_)
	{
		if(e.getID() == pID)
			return &e;
	}

	return entity;
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
template <typename CMP>
constexpr CMP& 
EntityManager<CMP0, CMP1, CMP2, Capacity>::addComponent(Entity_t& pEntity, CMP& pComponent) noexcept
{
	return addComponent<CMP>(pEntity, CMP{pComponent});
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
template <typename CMP, typename... ParamTypes>
constexpr CMP&
EntityManager<CMP0, CMP1, CMP2, Capacity>::addComponent(Entity_t& pEntity, ParamTypes&&... pParams) noexcept
{
	return addComponent<CMP>(pEntity, CMP{ std::forward<ParamTypes>(pParams)... });

	//TODO: Otra opcion a parte podria ser hacer como abajo y pasar los params... para la funcion del componentStorage que tmb recibe los params asi
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
template <typename CMP>
constexpr CMP&
EntityManager<CMP0, CMP1, CMP2, Capacity>::addComponent(Entity_t& pEntity, CMP&& pComponent) noexcept
{
	//1.- Deberia comprobar si la entidad ya tiene ese componente
	if(pEntity.hasComponent(getCMPId(CMP{})))
	{
		//2.- Si lo tiene, deberia devolver el que tiene
		auto key = pEntity.getComponent<CMP>();
		return components_.getComponent<CMP>(key);
	}
	
	//3.- Si no lo tiene deberia crearlo y devolverlo
	auto key = components_.addComponent<CMP>(pComponent);

	pEntity.addComponent<CMP>(key, getCMPId(CMP{})); //TODO: Tengo error de linker aqui. No se por que

	return components_.getComponent<CMP>(key);
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
template <typename CMP>
constexpr void
EntityManager<CMP0, CMP1, CMP2, Capacity>::removeComponent(Entity_t& pEntity) noexcept
{
	assert(("The entity doesn't have this component!", pEntity.hasComponent(getCMPId(CMP{}))));

	auto key = pEntity.removeComponent<CMP>(getCMPId(CMP{}));
	components_.removeComponent<CMP>(key);
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
template <typename... CMPs>
constexpr void
EntityManager<CMP0, CMP1, CMP2, Capacity>::removeComponents(Entity_t& pEntity) noexcept
{
	(removeComponent<CMPs>(pEntity), ...);
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
template <typename CMP>
constexpr auto&
EntityManager<CMP0, CMP1, CMP2, Capacity>::getComponent_impl(Entity_t const& pEntity, auto* self) noexcept
{
	auto key = pEntity.getComponent<CMP>();
	return self->components_.getComponent<CMP>(key);
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
template <typename CMP>
constexpr auto&
EntityManager<CMP0, CMP1, CMP2, Capacity>::getComponents_impl(auto* self) noexcept
{
	return self->components_.getComponents<CMP>();
}