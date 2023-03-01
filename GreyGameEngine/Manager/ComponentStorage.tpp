template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
template<typename CMP>
constexpr auto& ComponentStorage<CMP0, CMP1, CMP2, Capacity>::getComponents() noexcept
{
	return getComponents_impl<CMP>(this);
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
template<typename CMP>
constexpr auto const& ComponentStorage<CMP0, CMP1, CMP2, Capacity>::getComponents() const noexcept
{
	return getComponents_impl<CMP>(this);
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
template<typename CMP>
static constexpr auto& ComponentStorage<CMP0, CMP1, CMP2, Capacity>::getComponents_impl(auto* self) noexcept
{
	return std::get<CMP_Storage_Type<CMP>>(self->components_); //TODO: Aqui por ejemplo podria usar el ID del CMP como TParameter del std::get
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
template<typename CMP>
constexpr CMP& ComponentStorage<CMP0, CMP1, CMP2, Capacity>::getComponent(auto const pKey) noexcept
{
	return getComponent_impl<CMP>(pKey, this);

}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
template<typename CMP>
constexpr CMP const& ComponentStorage<CMP0, CMP1, CMP2, Capacity>::getComponent(auto const pKey) const noexcept
{
	return getComponent_impl<CMP>(pKey, this);
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
template<typename CMP>
static constexpr auto& 
ComponentStorage<CMP0, CMP1, CMP2, Capacity>::getComponent_impl(auto const pKey, auto* self) noexcept
{
	auto& cmps = std::get<CMP_Storage_Type<CMP>>(self->components_);
	return cmps[pKey];
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
template<typename CMP>
constexpr auto ComponentStorage<CMP0, CMP1, CMP2, Capacity>::addComponent(CMP& pCMP) noexcept
{
	return addComponent<CMP>(CMP{pCMP});
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
template<typename CMP>
constexpr auto ComponentStorage<CMP0, CMP1, CMP2, Capacity>::addComponent(CMP&& pCMP) noexcept
{
	auto& cmps = std::get<CMP_Storage_Type<CMP>>(components_);
	return cmps.push_back(pCMP);
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
template<typename CMP, typename... ParametersTypes>
constexpr auto ComponentStorage<CMP0, CMP1, CMP2, Capacity>::addComponent(ParametersTypes&&...parameters) noexcept
{
	auto& cmps = std::get<CMP_Storage_Type<CMP>>(components_);
	return cmps.push_back(CMP{std::forward<ParametersTypes>(parameters)...});
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
template<typename CMP>
constexpr void ComponentStorage<CMP0, CMP1, CMP2, Capacity>::removeComponent(auto const pKey) noexcept
{
	std::get<CMP_Storage_Type<CMP>>(components_).erase(pKey);
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
constexpr void ComponentStorage<CMP0, CMP1, CMP2, Capacity>::clearStorage() noexcept
{ 
	//TODO: Esto es bastante guarro, cuando haga la lista de CMPs tengo que cambiar a hacer un parameter pack 
	//y con la CMPList de metaprogramming
	std::get<0>(components_).clear();
	std::get<1>(components_).clear();
	std::get<2>(components_).clear();
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
template<typename CMP>
constexpr void ComponentStorage<CMP0, CMP1, CMP2, Capacity>::clearComponents() noexcept
{
	std::get<CMP_Storage_Type<CMP>>(components_).clear();
}