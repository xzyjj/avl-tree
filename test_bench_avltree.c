#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "xstddef.h"
#include "xstdlib.h"
#include "avltree.h"


struct Test_avl {
	int32 key;
	char buf[128];

	struct avl_node avl;
};

#define AVLTREE_BENCHTEST_SIZE (4 << 19)
#define AVLTREE_TYPE struct Test_avl
#define AVLTREE_NODE_MEMBER avl
#define AVLTREE_KEY_TYPE int32
#define AVLTREE_KEY_MEMBER key

void avl_each(struct avl_node *node, int n, int c) {
	if (node) {
		avl_each(node->right, n + 4, 0);

		for (int i = 0; i < n; i++)
				printf(" ");

		AVLTREE_TYPE *avl = container_of(node, AVLTREE_TYPE,
			AVLTREE_NODE_MEMBER);
		if (c == -1) {
			printf("=%d (%d)(p: %p, l: %p, r: %p, n: %p)\n", avl->AVLTREE_KEY_MEMBER, node->bf,
				node->parent, node->left, node->right, node);
		} else if (c == 0) {
			printf("/%d (%d)(p: %p, l: %p, r: %p, n: %p)\n", avl->AVLTREE_KEY_MEMBER, node->bf,
				node->parent, node->left, node->right, node);
		} else {
			printf("\\%d (%d)(p: %p, l: %p, r: %p, n: %p)\n", avl->AVLTREE_KEY_MEMBER, node->bf,
				node->parent, node->left, node->right, node);
		}

		avl_each(node->left, n + 4, 1);
	} else {
		for (int i = 0; i < n; i++)
			printf(" ");
		if (c == -1) {
			printf("=Nil\n");
		} else if (c == 0) {
			printf("/Nil\n");
		} else {
			printf("\\Nil\n");
		}
	}
}

static AVLTREE_TYPE *avl_insert(struct avl_root *root, AVLTREE_TYPE *new) {
	struct avl_node **link = &root->node, *parent = NULL, *node = NULL;
	AVLTREE_KEY_TYPE key = 0, key_new = new->AVLTREE_KEY_MEMBER;
	while (*link) {
		parent = *link;
		key = container_of(parent, AVLTREE_TYPE,
			AVLTREE_NODE_MEMBER)->AVLTREE_KEY_MEMBER;
		if (key_new == key)
			return NULL;
		if (key_new < key) {
			link = &parent->left;
		} else {
			link = &parent->right;
		}
	}

	node = &new->AVLTREE_NODE_MEMBER;
	node->left = node->right = NULL;
	node->parent = parent;
	node->bf = 1;
	*link = node;

	avl_balance(root, node);

	return new;
}

static AVLTREE_TYPE *avl_erase(struct avl_root *root, AVLTREE_KEY_TYPE key_old) {
	struct avl_node **link = &root->node, *tmp = NULL, *node = NULL;
	AVLTREE_KEY_TYPE key = 0;
	while (*link) {
		tmp = *link;
		key = container_of(tmp, AVLTREE_TYPE,
			AVLTREE_NODE_MEMBER)->AVLTREE_KEY_MEMBER;
		if (key_old == key) {
			node = tmp;
			break;
		}

		if (key_old < key) {
			link = &tmp->left;
		} else {
			link = &tmp->right;
		}
	}
	if (!node)
		return NULL;

	avl_erase_fix(root, node);

	return container_of(tmp, AVLTREE_TYPE, AVLTREE_NODE_MEMBER);
}

void avl_search(struct avl_root *root, AVLTREE_KEY_TYPE key_se) {
	struct avl_node **link = &root->node, *tmp = NULL, *node = NULL;
	AVLTREE_KEY_TYPE key = 0;
	while (*link) {
		tmp = *link;
		key = container_of(tmp, AVLTREE_TYPE,
			AVLTREE_NODE_MEMBER)->AVLTREE_KEY_MEMBER;
		if (key_se == key) {
			node = tmp;
			break;
		}

		if (key_se < key) {
			link = &tmp->left;
		} else {
			link = &tmp->right;
		}
	}

	printf("key: %d, node: %p\n", key_se, node);
}

int main() {
	struct avl_root root = { NULL };
	struct Test_avl *avl = NULL, *bavl = malloc(
		sizeof(struct Test_avl) * AVLTREE_BENCHTEST_SIZE);
	clock_t start, end;
	double time;

	RANDOM_TYPE0_NEW(rand, 123456);
	for (int i = 0; i < AVLTREE_BENCHTEST_SIZE; i++) {
		XSYMBOL(random_r)(&rand, &(bavl[i].key));
	}

	/* insert */
	start = clock();
	for (int i = 0; i < AVLTREE_BENCHTEST_SIZE; i++) {
		avl_insert(&root, &bavl[i]);
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("insert: %d, time: %.6fs, height: %d\n", AVLTREE_BENCHTEST_SIZE, time,
		root.node ? root.node->bf : 0);

	/* erase */
	start = clock();
	for (int i = 0; i < 100000; i++) {
		avl_erase(&root, bavl[i].key);
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("erase: %d, time: %.6fs, height: %d\n", 100000, time,
		root.node ? root.node->bf : 0);

	/* search */
	start = clock();
	for (int i = 0; i < 10; i++) {
		avl_search(&root, bavl[99999 + i].key);
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("search: %d, time: %.6fs, height: %d\n", 10, time,
		root.node ? root.node->bf : 0);

	/* erase */
	start = clock();
	for (int i = 0; i < AVLTREE_BENCHTEST_SIZE; i++) {
		avl_erase(&root, bavl[i].key);
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("erase: %d, time: %.6fs, height: %d\n", AVLTREE_BENCHTEST_SIZE, time,
		root.node ? root.node->bf : 0);

	return 0;
}
