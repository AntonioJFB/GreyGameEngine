#pragma once

#include <array>

template <typename CMP_T, typename DATA_T = uint8_t, size_t Capacity = 10>
struct SlotMap
{
	//using iterator	= CMP_T*; //En windows no puede hacer la conversión de interator a CMP_T*
	//using citerator	= CMP_T const*;

	using iterator = std::_Array_iterator<CMP_T, Capacity>;
	using citerator = std::_Array_const_iterator<CMP_T, Capacity>;

	//Struct used for access the Components
	using key_type = struct {
		DATA_T id_;
		DATA_T gen_;
	};

	//Constructor of the SlotMap
	explicit constexpr SlotMap() noexcept;

private:
	//Number of actions done with the SlotMap
	DATA_T generation_{};
	//Next free slot
	DATA_T freeList_{};
	//Number of elements in the SlotMap
	DATA_T size_{};

	//Array that keeps the keys to the components
	std::array<key_type,	Capacity>	slots_{};
	//Array with the components
	std::array<CMP_T,		Capacity>	data_{};
	//Control array
	std::array<DATA_T,		Capacity>	erase_{};

public:

	//=========================================================================
	//PUBLIC METHODS 
	//=========================================================================

	/**
	* @brief Method used to insert an element in the slotMap.
	*
	* @param p_data (Reference) Data element that is going to be saved in the data_ array
	* @return Returns the key generated to manage the slot added.
	*/
	[[nodiscard]] constexpr key_type const push_back(CMP_T const& pCMP);

	/**
	* @brief Method used to insert an element in the slotMap.
	*
	* @param p_data (Temporal value) Data element that is going to be saved in the data_ array
	* @return Returns the key generated to manage the slot added.
	*/
	[[nodiscard]] constexpr key_type const push_back(CMP_T&& pCMP);

	/**
	* @brief Method used to erase an slot element from the slotMap
	*
	* @param p_key Key used to find the slot we want to erase.
	* @return Returns true if the delete action is correct, false otherwise.
	*/
	[[nodiscard]] constexpr CMP_T const& erase(auto const pKey) noexcept;

	/**
	* @brief Method used to clear the slots_ array and initilize again its slots.
	*
	*/
	constexpr void clear() noexcept;

	/**
	* @brief Returns a const referece of the component (or value) saved in the data_ array
	* managed by the slot refferred by the key used as parameter.
	* This method will be called when the SlotMap is declared const in its instantiation.
	*
	* @param p_key Key used to find the slot we want
	* @return constexpr value_type const&
	*/
	[[nodiscard]] constexpr CMP_T const& operator[](auto pKey) const noexcept;

	/**
	* @brief Returns a referece of the component (or value) saved in the data_ array
	* managed by the slot refferred by the key used as parameter.
	*
	* @param p_key Key used to find the slot we want
	* @return constexpr value_type&
	*/
	[[nodiscard]] constexpr CMP_T& operator[](auto pKey) noexcept;

	/**
	* @brief Iterator to the first element of the data_ array
	*
	* @return constexpr iterator
	*/
	[[nodiscard]] constexpr iterator begin() noexcept;

	/**
	* @brief Const Iterator to the first element of the data_ array
	*
	* @return constexpr citerator
	*/
	[[nodiscard]] constexpr citerator begin() const  noexcept;

	/**
	* @brief Const iterator to the first element of the data_ array
	*
	* @return constexpr citerator
	*/
	[[nodiscard]] constexpr citerator cbegin() const noexcept;

	/**
	* @brief Iterator to the last element of the data_ array
	*
	* @return constexpr iterator
	*/
	[[nodiscard]] constexpr iterator end() noexcept;

	/**
	* @brief Iterator to the last element of the data_ array
	*
	* @return constexpr citerator
	*/
	[[nodiscard]] constexpr citerator end() const noexcept;

	/**
	* @brief Const iterator to the last element of the data_ array
	*
	* @return constexpr citerator
	*/
	[[nodiscard]] constexpr citerator cend() const noexcept;

	/**
	* @brief Returns the generation_y of the slotMap
	*
	* @return constexpr size_t
	*/
	[[nodiscard]] inline DATA_T generation() const noexcept { return generation_; }

	/**
	* @brief Returns the number of elements in the slotMap
	*
	* @return constexpr index_type
	*/
	[[nodiscard]] inline DATA_T size() const noexcept { return size_; }

	/**
	* @brief Returns the capacity of the slotMap
	*
	* @return constexpr size_t
	*/
	[[nodiscard]] inline DATA_T capacity() const noexcept { return Capacity; }

	/**
	* @brief Returns the FreeList_ of the slotMap
	*
	* @return constexpr size_t
	*/
	[[nodiscard]] inline DATA_T FreeList() const noexcept { return freeList_; }

private:
	
	//=========================================================================
	//PRIVATE METHODS 
	//=========================================================================

	/**
	* @brief IniInitializes the slotMap with the provided Capacity.
	* Initializes the keys of each slot.
	*
	*/
	constexpr void initialize() noexcept;

	/**
	* @brief Method that reserves and initializes a slot.
	* The slot id and gen are initilized.
	* The freelist_, size_ and generation_ are updated.
	*
	* @return Returns the position where the new slot is allocated in the slots_ array.
	*/
	[[nodiscard]] constexpr auto const allocate() noexcept;

	/**
	* @brief When deleting an slot, this method copies the last element
	* in the data_ and erase_ arrays to the first free spot if the erased element wasn't
	* the last in these arrays.
	* It also updates de the id of the slot affected by this resort.
	*
	* @param p_id Position of the data_ and erase_ arrays that are going to be updated
	*/
	constexpr void copyLast(DATA_T const pID) noexcept;
	
	/**
	* @brief Checks if a key is valid.
	*
	* @param p_key Key to be checked
	* @return Returns true if is valid, false otherwise.
	*/
	[[nodiscard]] constexpr bool isValid(auto const pKey) noexcept;
};

#include "SlotMap.tpp"