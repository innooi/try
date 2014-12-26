#include "header.h"
#include "treap.h"

Treap<uint32_t, uint32_t> Tree;

int main() {
	for (uint32_t i = 1; i <= 10; ++i) {
		Tree.insert(i, i);
	}
	//Tree.clear();
	//printf("%d\n", Tree.size());
	//Tree.clear();
	//Tree[5] = 114;
	//printf("%d\n", Tree.search(5));
	//Tree.del(5);
	printf("%d\n", Tree.kth(2));
	Tree._del(Tree.root, 5);
	Tree._del(Tree.root, 6);
	Tree._del(Tree.root, 9);
	printf("%d\n", Tree.size());
	printf("%d\n", Tree.kth(7));
	return 0;
}
