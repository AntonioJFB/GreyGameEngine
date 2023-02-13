// GreyGameEngine.cpp: define el punto de entrada de la aplicación.
//

#include "GreyGameEngine.h"
#include "EntityMan.hpp"

using namespace std;

int main()
{
	EntityManager::Entity entity1 {};

	cout << "First Entity: " << entity1.ID_ << endl;
	return 0;
}
