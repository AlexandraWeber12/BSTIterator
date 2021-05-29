

#include "./bst.h"
#include "../../rational/rational/rational.h"

int main() {
	swe2::bst <swe2::rational <int>> t;

	t.insert(10);
	t.insert(3);
	t.insert(6);
	t.insert(9);
	t.insert(5);
	t.insert(1);
	t.insert(15);
	t.insert(21);
	t.insert(12);
	t.insert(17);
}