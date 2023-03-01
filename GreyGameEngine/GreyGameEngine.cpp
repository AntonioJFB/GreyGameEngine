// GreyGameEngine.cpp: define el punto de entrada de la aplicación.
//

#include <cstdint>

#include "GreyGameEngine.h"
#include "Utils/MemViwer.hpp"
#include "Manager/EntityManager.hpp"

using namespace std;
using namespace MemViwer;

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

enum Tags : uint8_t
{
	Player = 0, Enemy = 1, Obstacle = 2
};

int main()
{
	//TODO: El EntityManager debería recibir todos los template parameters de configuracion de los SlotMaps, key_types, TagMaks y demás tipos
	EntityManager<PhysicsComponent, RenderComponent, AIComponent, 5>EM{5};

	auto& entity = EM.createEntity();

	/*TODO: La inicializacion de una entidad deberia ir justo despues de su crecion porque, de momento, como uso vector.
	si creo muchas entidades y hace resize, pierdo la referencia a esas entidades y casca el programa.*/

	auto& phycmp = EM.addComponent<PhysicsComponent>(entity, PhysicsComponent{});
	auto& aicmp = EM.addComponent<AIComponent>(entity, AIComponent{});
	auto& rendcmp = EM.addComponent<RenderComponent>(entity, RenderComponent{});
	
	auto& entity2 = EM.createEntity();
	EM.addTag(entity2, Tags::Player);
	auto& entity3 = EM.createEntity();
	EM.addTag(entity3, Tags::Enemy);
	auto& entity4 = EM.createEntity();
	EM.addTag(entity4, Tags::Obstacle);
	EM.addTag(entity4, Tags::Enemy);
	EM.removeTag(entity4, Tags::Obstacle);

	auto& entity5 = EM.createEntity();

	EM.forAll();

	return 0;
}
