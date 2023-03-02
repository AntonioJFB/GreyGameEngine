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

struct PlayerTag {};
struct EnemyTag {};
struct ObstacleTag{};


int main()
{
	//TODO: El EntityManager debería recibir todos los template parameters de configuracion de los SlotMaps, key_types, TagMaks y demás tipos
	EntityManager<PhysicsComponent, RenderComponent, AIComponent, PlayerTag, EnemyTag, ObstacleTag, 5>EM{5};

	auto& entity = EM.createEntity();

	/*TODO: La inicializacion de una entidad deberia ir justo despues de su crecion porque, de momento, como uso vector.
	si creo muchas entidades y hace resize, pierdo la referencia a esas entidades y casca el programa.*/

	auto& phycmp = EM.addComponent<PhysicsComponent>(entity, PhysicsComponent{});
	auto& aicmp = EM.addComponent<AIComponent>(entity, AIComponent{});
	auto& rendcmp = EM.addComponent<RenderComponent>(entity, RenderComponent{});

	auto hasPhys = EM.hasComponent<PhysicsComponent>(entity);
	cout << "Entity1 has PhysicsComponent: " << hasPhys << "\n";

	auto& AIcmp = EM.getComponent<AIComponent>(entity);
	cout << "Entity1 AICmp patrol 0: { " << AIcmp.patrol[0] << " }\n";

	auto const& renderCMPs = EM.getComponents<RenderComponent>();

	EM.removeComponent<RenderComponent>(entity);
	auto hasRender = EM.hasComponent<RenderComponent>(entity);
	cout << "Entity1 has RenderComponent: " << hasRender << "\n";

	EM.addTag<PlayerTag>(entity);

	bool hasPlayerTag = EM.hasTag<PlayerTag>(entity);
	cout << "Entity1 has PlayerTag: " << hasPlayerTag << "\n";

	EM.removeTag<PlayerTag>(entity);

	EM.hasTag<PlayerTag>(entity);

	hasPlayerTag = EM.hasTag<PlayerTag>(entity);
	cout << "Entity1 has PlayerTag: " << hasPlayerTag << "\n";

	auto const& entities = EM.getEntities();
	for (auto const& e : entities)
		cout << "EntityID_: " << e.getID() << "\n";

	auto* entitiy1cp = EM.getEntityByID(entity.getID());
	if(entitiy1cp)
		cout << "EntityID_: " << entitiy1cp->getID() << "\n";


	/*auto& entity2 = EM.createEntity();
	auto& entity3 = EM.createEntity();
	auto& entity4 = EM.createEntity();
	auto& entity5 = EM.createEntity();*/

	EM.forAll();

	return 0;
}
