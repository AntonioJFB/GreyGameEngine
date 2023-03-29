namespace GreyGameEngine {
	template <typename CMPs, size_t Capacity>
	template<typename CMP>
	constexpr auto& ComponentStorage<CMPs, Capacity>::getComponents() noexcept
	{
		return getComponents_impl<CMP>(this);
	}

	//=============================================================================

	template <typename CMPs, size_t Capacity>
	template<typename CMP>
	constexpr auto const& ComponentStorage<CMPs, Capacity>::getComponents() const noexcept
	{
		return getComponents_impl<CMP>(this);
	}

	//=============================================================================

	template <typename CMPs, size_t Capacity>
	template<typename CMP>
	static constexpr auto& ComponentStorage<CMPs, Capacity>::getComponents_impl(auto* self) noexcept
	{
		return std::get<cmp_info::id<CMP>()>(self->components_);
	}

	//=============================================================================

	template <typename CMPs, size_t Capacity>
	template<typename CMP>
	constexpr CMP& ComponentStorage<CMPs, Capacity>::getComponent(auto const pKey) noexcept
	{
		return getComponent_impl<CMP>(pKey, this);

	}

	//=============================================================================

	template <typename CMPs, size_t Capacity>
	template<typename CMP>
	constexpr CMP const& ComponentStorage<CMPs, Capacity>::getComponent(auto const pKey) const noexcept
	{
		return getComponent_impl<CMP>(pKey, this);
	}

	//=============================================================================

	template <typename CMPs, size_t Capacity>
	template<typename CMP>
	static constexpr auto&
		ComponentStorage<CMPs, Capacity>::getComponent_impl(auto const pKey, auto* self) noexcept
	{
		auto& cmps = std::get<cmp_info::id<CMP>()>(self->components_);
		return cmps[pKey];
	}

	//=============================================================================

	template <typename CMPs, size_t Capacity>
	template<typename CMP>
	constexpr auto ComponentStorage<CMPs, Capacity>::addComponent(CMP& pCMP) noexcept
	{
		return addComponent<CMP>(CMP{ pCMP });
	}

	//=============================================================================

	template <typename CMPs, size_t Capacity>
	template<typename CMP>
	constexpr auto ComponentStorage<CMPs, Capacity>::addComponent(CMP&& pCMP) noexcept
	{
		auto& cmps = std::get<cmp_info::id<CMP>()>(components_);
		return cmps.push_back(pCMP);
	}

	//=============================================================================

	template <typename CMPs, size_t Capacity>
	template<typename CMP, typename... ParametersTypes>
	constexpr auto ComponentStorage<CMPs, Capacity>::addComponent(ParametersTypes&&...parameters) noexcept
	{
		auto& cmps = std::get<cmp_info::id<CMP>()>(components_);
		return cmps.push_back(CMP{ std::forward<ParametersTypes>(parameters)... });
	}

	//=============================================================================

	template <typename CMPs, size_t Capacity>
	template<typename CMP>
	constexpr void ComponentStorage<CMPs, Capacity>::removeComponent(auto const pKey) noexcept
	{
		std::get<cmp_info::id<CMP>()>(components_).erase(pKey);
	}

	//=============================================================================

	template <typename CMPs, size_t Capacity>
	constexpr void ComponentStorage<CMPs, Capacity>::clearStorage() noexcept
	{
		std::get<cmp_info::id<CMPs...>()>(components_).clear();
	}

	//=============================================================================

	template <typename CMPs, size_t Capacity>
	template<typename CMP>
	constexpr void ComponentStorage<CMPs, Capacity>::clearComponents() noexcept
	{
		std::get<cmp_info::id<CMP>()>(components_).clear();
	}
}