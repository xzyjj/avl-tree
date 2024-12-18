#include <stdio.h>
#include <stdlib.h>
#include "xstddef.h"
#include "avltree.h"


struct Test_avl {
	int32 key;
	char buf[4096];

	struct avl_node avl;
};

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

AVLTREE_TYPE *avl_insert(struct avl_root *root, AVLTREE_TYPE *new) {
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

AVLTREE_TYPE *avl_erase(struct avl_root *root, AVLTREE_KEY_TYPE key_old) {
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

int main() {
	struct avl_root root = { NULL };
	struct Test_avl *avl = NULL;

	int r, t = 0;
	for (int i = 0; i < 1024; i++) {
e:
		if (t == 0) {
			printf("insert> ");
		} else {
			printf("erase> ");
		}

		fflush(stdout);
		scanf("%d", &r);
		if (r == -9990) {
			t = 0;
			goto e;
		} else if (r == -9999) {
			t = 1;
			goto e;
		} else if (r == -99999) {
			break;
		}

		if (t == 0) {
			avl = malloc(sizeof(struct Test_avl));
			avl->key = r;
			avl_insert(&root, avl);

			avl_each(root.node, 0, -1);
		} else {
			void *p = avl_erase(&root, r);
			if (p)
				free(p);

			avl_each(root.node, 0, -1);
		}
	}

	return 0;
}
