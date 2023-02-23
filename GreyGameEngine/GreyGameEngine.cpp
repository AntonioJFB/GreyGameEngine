// GreyGameEngine.cpp: define el punto de entrada de la aplicación.
//

#include <cstdint>

#include "GreyGameEngine.h"
#include "Utils/MemViwer.hpp"
#include "Manager/ComponentStorage.hpp"

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
	ComponentStorage<PhysicsComponent, RenderComponent, AIComponent, 5> cmps {};

	auto key1 = cmps.addComponent<PhysicsComponent>(PhysicsComponent{ 1,2,3 });
	auto key2 = cmps.addComponent<PhysicsComponent>(4,5,6);
	auto key3 = cmps.addComponent<PhysicsComponent>(PhysicsComponent{ 8,9,10 });
	auto key4 = cmps.addComponent<RenderComponent>('a'); 
	auto key5 = cmps.addComponent<RenderComponent>('b');
	auto key6 = cmps.addComponent<AIComponent>(); 
	auto key7 = cmps.addComponent<PhysicsComponent>(PhysicsComponent{ 11,12,13 });
	auto key8 = cmps.addComponent<AIComponent>();

	cmps.removeComponent<PhysicsComponent>(key1);
	cmps.removeComponent<PhysicsComponent>(key2);
	cmps.removeComponent<RenderComponent>(key4);
	cmps.removeComponent<AIComponent>(key8);

	key1 = cmps.addComponent<PhysicsComponent>(PhysicsComponent{ .x = 20, .y = 90, .z = 2 });
	key4 = cmps.addComponent<RenderComponent>(RenderComponent{'#'});
	key8 = cmps.addComponent<AIComponent>(AIComponent{ .patrol = {666, 777, 888, 999} });

	auto const& phyCmp = cmps.getComponent<PhysicsComponent>(key1);
	cout << "PhysCmp: { x: " << phyCmp.x << ", y: " << phyCmp.y << ", z:" << phyCmp.z << " }\n";


	auto& PhysCmps = cmps.getComponents<PhysicsComponent>();

	for (auto& cmp : PhysCmps)
		cout << "PhysCmp: { x: " << cmp.x << ", y: " << cmp.y << ", z:" << cmp.z << " }\n";
	
	auto& RenderCmps = cmps.getComponents<RenderComponent>();

	for (auto& cmp : RenderCmps)
		cout << "RenderCmp: " << cmp.sprite << "\n";


	auto& AIcmps = cmps.getComponents<AIComponent>();

	for (auto& cmp : AIcmps)
		cout << "AICmps: [ " << cmp.patrol[0] << ", " << cmp.patrol[1] << ", " << cmp.patrol[2] << ", " << cmp.patrol[3] << " ]\n";

	return 0;
}
