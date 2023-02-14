// GreyGameEngine.cpp: define el punto de entrada de la aplicación.
//

#include <cstdint>
#include <cstdio>
#include <array>
#include <vector>
#include<iostream>

#include "GreyGameEngine.h"
#include "Utils/MemViwer.hpp"
#include "Utils/DataStructures/SlotMap.h"


using namespace std;
using namespace MemViwer;

struct PhysicsComponent
{
	int a {16};
};

int main()
{
	/*std::vector<int> a{1,2,4,8,16,32};
	showMemObj(a);
	showMemPtr(&a[0], 24);

	a.push_back(64);

	showMemObj(a);
	showMemPtr(&a[0], 48);*/

	SlotMap<PhysicsComponent> PhysicsSlotMap{};
	PhysicsComponent phyCmp {1};
	auto key1 = PhysicsSlotMap.push_back(phyCmp);
	auto key2 = PhysicsSlotMap.push_back(PhysicsComponent{2});
	auto key3 = PhysicsSlotMap.push_back(PhysicsComponent{3});
	auto& cmp1 = PhysicsSlotMap.erase(key2);
	auto key4 = PhysicsSlotMap.push_back(PhysicsComponent{4});

	showMemObj(PhysicsSlotMap);
	
	auto const& cmp2 = PhysicsSlotMap[key1];

	for (auto const& cmp : PhysicsSlotMap)
	{
		std::cout << "PhysCMP_" << cmp.a << "\n";
	}
	return 0;
}
