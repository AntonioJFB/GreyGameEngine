// GreyGameEngine.cpp: define el punto de entrada de la aplicación.
//

#include <cstdint>
#include <iostream>

#include "GreyGameEngine.h"
#include "Utils/MemViwer.hpp"
#include "Manager/EntityManager.hpp"

#include "Metaprogramming/Metaprogramming.hpp"

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

template<typename CMPs, typename TAGs>
struct GameEngine
{
	using cmps = MP::cmp_traits<CMPs>;
	using tags = MP::tag_traits<TAGs>;
};


int main()
{
	using GE = GameEngine<CMPList, TAGList>;
	GE Engine {};

	static_assert(GE::tags::size() == 3);
	static_assert(GE::tags::id<ObstacleTag>() == 1);
	static_assert(GE::tags::mask<ObstacleTag>() == 0b10);

	static_assert(GE::cmps::id<PhysicsComponent>() == 1);
	static_assert(GE::cmps::mask<PhysicsComponent>() == 0b10);

	static_assert(MP::is_same_v<PhysicsComponent, PhysicsComponent> == true);
	static_assert(MP::is_same_v<PhysicsComponent, RenderComponent> == false);

	static_assert(CMPList::contains<PhysicsComponent>() == true);
	//TODO: A partir de ahora, el EntityManager tiene que recibir una lista de Componentes y una Lista de Tags
	//TODO: A partir de ahora, el ComponentStorage tiene que recibir una lista de Componentes

	//TODO: El EntityManager debería recibir todos los template parameters de configuracion de los SlotMaps, key_types, TagMaks y demás tipos
	//EntityManager<PhysicsComponent, RenderComponent, AIComponent, PlayerTag, EnemyTag, ObstacleTag, 5>EM{5};

	return 0;
}
