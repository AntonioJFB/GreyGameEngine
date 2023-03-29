// GreyGameEngine.cpp: define el punto de entrada de la aplicación.
//

#include <cstdint>
#include <iostream>
#include <string>

#include "GreyGameEngine.h"
#include "Utils/MemViwer.hpp"
#include "Manager/EntityManager.hpp"

#include "Metaprogramming/Metaprogramming.hpp"

using namespace MemViwer;

void seeType(auto) { std::cout << __FUNCSIG__ << "\n"; }

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
};


int main()
{
	//TODO: A partir de ahora, el EntityManager tiene que recibir una lista de Componentes y una Lista de Tags
	//TODO: A partir de ahora, el ComponentStorage tiene que recibir una lista de Componentes

	//TODO: El EntityManager debería recibir todos los template parameters de configuracion de los SlotMaps, key_types, TagMaks y demás tipos
	//EntityManager<PhysicsComponent, RenderComponent, AIComponent, PlayerTag, EnemyTag, ObstacleTag, 5>EM{5};

	using GameEngine = GreyGameEngine::EntityManager<CMPList, TAGList, 10>;
	GameEngine GE{};

	auto& entity = GE.createEntity();
	auto& phy_cmp = GE.addComponent<PhysicsComponent>(entity, 10, 20, 30);
	auto& rend_cmp = GE.addComponent<RenderComponent>(entity);
	auto& ai_cmp = GE.addComponent<AIComponent>(entity);

	auto& phys_cmps = GE.getComponents<PhysicsComponent>();

	for(auto const& cmp : phys_cmps)
	{
		std::cout << cmp.x << cmp.y << cmp.z << "\n";
	}

	auto& rend_cmps = GE.getComponents<RenderComponent>();

	for (auto const& cmp : rend_cmps)
	{
		std::cout << cmp.sprite << "\n";
	}

	auto& ai_cmps = GE.getComponents<AIComponent>();

	for (auto const& cmp : ai_cmps)
	{
		std::cout << cmp.patrol[0]<< "\n";
	}

	phy_cmp = GE.getComponent<PhysicsComponent>(entity);
	std::cout << phy_cmp.x << phy_cmp.y << phy_cmp.z << "\n";

	auto* e = GE.getEntityByID(1);
	std::cout << std::to_string( e->getID() ) << "\n";

	auto& entities = GE.getEntities();
	
	GE.addTags < PlayerTag, ObstacleTag > (entity);

	GE.setEntitiyForDestroy(entity);
	
	//GE.checkDeadEntities();

	return 0;
}
