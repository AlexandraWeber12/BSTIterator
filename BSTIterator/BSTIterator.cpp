

#include "./bst.h"
#include "vld.h"
#include <iostream>
#include <vector>
//#include "../../rational/rational/rational.h"

int main() {
	//swe2::bst <swe2::rational <int>> t;
	swe2::bst<int> t{10, 3, 6, 9, 5, 1, 15, 21, 12, 17};
	swe2::bst<int> t1{ 1 };
	std::cout << t.size() << "\n";

	t.apply([](int value) {
		std::cout << value << "\n";
		
		});

	//t.insert(9);

	auto iterator = t.begin();
	iterator++;
	//const int& value = *iterator;
		
	/*for (auto it = t.begin(); it != t.end(); ++it)
	{
		std::cout << *(it) << "\n";
	}*/


	//auto result = t1 <=> t1;

	//t.swap(t1);



	std::cout << t << "\n";

	////*iterator = 2;
	////value = 2;

	//t.apply([](int value) {
	//	std::cout << value << "\n";

		//});

	//swe2::bst<int> t2{t};

	//swe2::bst<int> t3;
	//t3 = t2;

	//swe2::bst <int> emptyTree{};
	//std::cout << emptyTree.empty() << "\n";
}