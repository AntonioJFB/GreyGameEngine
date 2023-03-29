namespace GreyGameEngine 
{
	template<typename CMPs, size_t Capacity>
	template<typename CMP>
	constexpr auto Entity<CMPs, Capacity>::getComponent() const noexcept
	{
		return std::get<to_key_type<CMP>>(key_storage);
	}

	//=============================================================================

	template<typename CMPs, size_t Capacity>
	template<typename CMP>
	constexpr void Entity<CMPs, Capacity>::addComponent(auto pSlotKey, auto pCMPMask) noexcept
	{
		components_ |= pCMPMask; //TODO: Cuando lo haga con metaprogramming esto deberia cambiar. Debo recibir la Mask como templateParameter

		std::get<to_key_type<CMP>>(key_storage) = pSlotKey;
	}

	//=============================================================================

	template<typename CMPs, size_t Capacity>
	template<typename CMP>
	constexpr auto Entity<CMPs, Capacity>::removeComponent(auto pCMPMask) noexcept
	{
		components_ ^= pCMPMask; //TODO: Cuando lo haga con metaprogramming esto deberia cambiar. Debo recibir la Mask como templateParameter
		return getComponent<CMP>();
	}
}