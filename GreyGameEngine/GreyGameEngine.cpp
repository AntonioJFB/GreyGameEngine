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

	auto& PhysCmps = cmps.getComponents<PhysicsComponent>();

	for (auto& cmp : PhysCmps)
		cout << "PhysCmp: { x: " << cmp.x << ", y: " << cmp.y << ", z:" << cmp.z << " }\n";

	return 0;
}
