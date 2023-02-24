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
	EntityManager<PhysicsComponent, RenderComponent, AIComponent, 5>EM{5};

	auto& entity =  EM.createEntity();
	PhysicsComponent physcmp{};
	auto& cmp = EM.addComponent<PhysicsComponent>(entity, physcmp);

	return 0;
}
