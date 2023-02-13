// GreyGameEngine.cpp: define el punto de entrada de la aplicación.
//

#include <cstdint>
#include <cstdio>
#include <array>
#include <vector>
#include<iostream>

#include "GreyGameEngine.h"
#include "Utils/MemViwer.hpp"

using namespace std;
using namespace MemViwer;

int main()
{
	std::vector<int> a {1,2,4,8,16,32};
	showMemObj(a);
	showMemPtr(&a[0], 24);

	a.push_back(64);

	showMemObj(a);
	showMemPtr(&a[0], 48);
	
	return 0;
}
