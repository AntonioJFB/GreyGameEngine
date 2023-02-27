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

int main()
{
	//TODO: El EntityManager debería recibir todos los template parameters de configuracion de los SlotMaps, key_types, TagMaks y demás tipos
	EntityManager<PhysicsComponent, RenderComponent, AIComponent, 5>EM{5};

	auto& entity =  EM.createEntity();
	
	auto& cmp = EM.addComponent<PhysicsComponent>(entity, 1, 2, 3);
	auto& cmp1 = EM.addComponent<RenderComponent>(entity, '#');
	auto& cmp2 = EM.addComponent<AIComponent>(entity, AIComponent{}); //TODO: Si no paso ninun parametro para crear el componente no se crea bien el objeto. !!!

	auto& AIcmp = EM.getComponent<AIComponent>(entity);

	auto& AIcmps = EM.getComponents<AIComponent>();

	EM.removeComponents<PhysicsComponent, RenderComponent, AIComponent>(entity);

	EM.addComponent<PhysicsComponent>(entity, PhysicsComponent{});

	return 0;
}
