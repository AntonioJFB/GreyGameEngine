﻿// GreyGameEngine.cpp: define el punto de entrada de la aplicación.
//

#include <cstdint>
#include <iostream>

#include "GreyGameEngine.h"
#include "Utils/MemViwer.hpp"
#include "Manager/EntityManager.hpp"

#include "Metaprogramming/Metaprogramming.hpp"

#include <list>

using namespace std;
using namespace MemViwer;

void seeType(auto) { cout << __FUNCSIG__ << "\n"; }

struct PhysicsComponent
{
	int x {16}, y{16}, z{16};
};

struct RenderComponent
{
	char sprite = '@';
};

struct AIComponent
{
	int patrol[4]{10, 20, 30, 40};
};

struct PlayerTag {};
struct EnemyTag {};
struct ObstacleTag{};

using CMPList = MP::TypeList<PhysicsComponent, RenderComponent, AIComponent>;
using TAGList = MP::TypeList<PlayerTag, EnemyTag, ObstacleTag>;

template<typename CMPs, typename TAGs, std::size_t Capacity=10>
struct GameEngine
{
	using cmps = MP::cmp_traits<CMPs>;
	using tags = MP::tag_traits<TAGs>;

	using data_type = MP::cmp_traits<CMPs>::mask_type;

	template<typename List>
	using to_tuple = MP::replace_t<std::tuple, List>;

	template<typename T>
	using to_slotmap = SlotMap<T, data_type, Capacity>;

	using storage_t = to_tuple<MP::forall_insert_template_t<to_slotmap, CMPs>>;

	storage_t storage_{};

	//Testeando otras cosas de metaprogramming

	using rpl_cont = MP::replace_container_t<std::list, std::vector<int>>;
	rpl_cont rpl_cont_ {};

	using fali = MP::forall_elements_list_insert_t<std::vector, CMPs>;
	fali fali_{};

	using replace = MP::replace_list_t<std::tuple, CMPs>;
	replace repl_{};

	using replace_all_l = MP::replace_all_list_t<std::tuple, SlotMap, CMPs>;
	replace_all_l rplallL_{};

	using newList = MP::add_types_to_list_t<CMPs, PlayerTag>;
	newList newL_{};

	using combList = MP::combine_typelists_t<CMPs, TAGs>;
	combList cmbLst_{};

	using eraseList = MP::erase_first_type_t<CMPs>;
	eraseList ersFrs_{};
};


int main()
{
	using GE = GameEngine<CMPList, TAGList>;
	GE Engine {};

	seeType(Engine.storage_);
	seeType(Engine.rpl_cont_);
	seeType(Engine.fali_);
	seeType(Engine.repl_);
	seeType(Engine.rplallL_);
	seeType(Engine.newL_);
	seeType(Engine.cmbLst_);
	seeType(Engine.ersFrs_);

	static_assert(GE::tags::size() == 3);
	static_assert(GE::tags::id<ObstacleTag>() == 2);
	static_assert(GE::tags::mask<ObstacleTag>() == 0b100);

	static_assert(GE::cmps::id<PhysicsComponent>() == 0);
	static_assert(GE::cmps::mask<PhysicsComponent>() == 0b01);

	static_assert(MP::is_same_v<PhysicsComponent, PhysicsComponent> == true);
	static_assert(MP::is_same_v<PhysicsComponent, RenderComponent> == false);

	static_assert(CMPList::contains<PhysicsComponent>() == true);
	//TODO: A partir de ahora, el EntityManager tiene que recibir una lista de Componentes y una Lista de Tags
	//TODO: A partir de ahora, el ComponentStorage tiene que recibir una lista de Componentes

	//TODO: El EntityManager debería recibir todos los template parameters de configuracion de los SlotMaps, key_types, TagMaks y demás tipos
	//EntityManager<PhysicsComponent, RenderComponent, AIComponent, PlayerTag, EnemyTag, ObstacleTag, 5>EM{5};

	return 0;
}
