

#include "./bst.h"
#include <iostream>
//#include "../../rational/rational/rational.h"

int main() {
	//swe2::bst <swe2::rational <int>> t;
	swe2::bst <int> t{10, 3, 6, 9, 5, 1, 15, 21, 12, 17};
	std::cout << t.size() << "\n";


	swe2::bst <int> emptyTree{};
	std::cout << emptyTree.empty() << "\n";
}