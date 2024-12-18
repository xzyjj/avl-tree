/* avltree.c - avl-tree (self-balancing binary search tree) implementation */

#include "xstddef.h"
#include "avltree.h"


/* func: avl_change_child - change parent node's child
* @param1: struct avl_root * # avl-tree root node
* @param2: struct avl_node * # parent node
* @param2: struct avl_node * # old node
* @param2: struct avl_node * # new node
* @return: void
*/
void avl_change_child(struct avl_root *root, struct avl_node *parent,
		struct avl_node *old, struct avl_node *new) {
	if (!parent) {
		root->node = new;
	} else if (parent->left == old) {
		parent->left = new;
	} else {
		parent->right = new;
	}
} /* end */

/* func: avl_update_height - update avl-tree node height
* @param1: struct avl_node * # avl-tree node
* @return: void
*/
void avl_update_height(struct avl_node *node) {
	int32 lh = AVL_HEIGHT(node->left);
	int32 rh = AVL_HEIGHT(node->right);

	node->bf = 1 + AVL_MAX(lh, rh);
} /* end */

/* func: avl_balance_factor - get avl-tree balance factor
* @param1: struct avl_node * # avl-tree node
* @return: void
*/
int32 avl_balance_factor(struct avl_node *node) {
	int32 lh = AVL_HEIGHT(node->left);
	int32 rh = AVL_HEIGHT(node->right);

	return lh - rh;
} /* end */

/* func: avl_left_rotate - avl-tree left-rotate
* @param1: struct avl_root * # avl-tree root node
* @param2: struct avl_node * # avl-tree rotate node's parent
* @return: void
*      g           g
*     / \         / \
*    p   u  -->  n   u
*   / \         / \
*  s   n       p   r
*     / \     / \
*    l   r   s   l
*/
void avl_left_rotate(struct avl_root *root, struct avl_node *parent) {
	struct avl_node *gparent = NULL, *node = NULL, *child = NULL;
	gparent = parent->parent;
	node = parent->right;
	child = node->left;

	/* 'left node' becomes 'parent right' */
	parent->right = child;
	if (child)
		child->parent = parent;

	/* 'parent' becomes 'left node' */
	node->left = parent;
	parent->parent = node;

	/* 'node' becomes 'gparent child' */
	avl_change_child(root, gparent, parent, node);
	node->parent = gparent;

	/* update height */
	avl_update_height(parent);
	avl_update_height(node);
} /* end */

/* func: avl_right_rotate - avl-tree right-rotate
* @param1: struct avl_root * # avl-tree root node
* @param2: struct avl_node * # avl-tree rotate node's parent
* @return: void
*       g         g
*      / \       / \
*     p   u --> n   u
*    / \       / \
*   n   s     l   p
*  / \           / \
* l   r         r   s
*/
void avl_right_rotate(struct avl_root *root, struct avl_node *parent) {
	struct avl_node *gparent = NULL, *node = NULL, *child = NULL;
	gparent = parent->parent;
	node = parent->left;
	child = node->right;

	/* 'right node' becomes 'parent left' */
	parent->left = child;
	if (child)
		child->parent = parent;

	/* 'parent' becomes 'right node' */
	node->right = parent;
	parent->parent = node;

	/* 'node' becomes 'gparent child' */
	avl_change_child(root, gparent, parent, node);
	node->parent = gparent;

	/* update height */
	avl_update_height(parent);
	avl_update_height(node);
} /* end */

/* func: avl_balance - fix avl-tree balance
* @param1: struct avl_root * # avl-tree root node
* @param2: struct avl_node * # avl-tree fix node
* @return: void
*/
void avl_balance(struct avl_root *root, struct avl_node *node) {
	struct avl_node *parent = NULL;
	for (int32 bf = 0; node; node = parent) {
		avl_update_height(node);

		parent = node->parent;
		if (!parent)
			break;

		bf = avl_balance_factor(parent);
		if (bf > 1) { /* LL LR */
			/* LR */
			if (avl_balance_factor(parent->left) < 0)
				avl_left_rotate(root, parent->left);
			avl_right_rotate(root, parent);
		} else if (bf < -1) { /* RR RL */
			/* RL */
			if (avl_balance_factor(parent->right) > 0)
				avl_right_rotate(root, parent->right);
			avl_left_rotate(root, parent);
		}
	}
} /* end */

/* func: avl_erase_fix - fix avl-tree erase balance
* @param1: struct avl_root * # avl-tree root node
* @param2: struct avl_node * # avl-tree erase node
* @return: void
*/
void avl_erase_fix(struct avl_root *root, struct avl_node *node) {
	struct avl_node *parent = node->parent, *successor = NULL;
	/*
	*   n          r          r
	*  / \        / \        / \
	* l   r  --> l   n  --> l   r2
	*    / \        / \
	*  nil  r2    nil  r2
	*
	*    n          l2         l2
	*   / \        / \        / \
	*  l   r  --> l   r  --> l   r
	*     / \        / \        / \
	*   l2  r2      n  r2      r3  r2
	*   / \        / \
	* nil  r3    nil  r3
	*/
	if (node->left && node->right) {
		successor = node->right;
		if (successor->left) {
			do {
				parent = successor;
				successor = successor->left;
			} while (successor->left);

			/* 'successor right' becomes 'parent left' */
			parent->left = successor->right;
			if (successor->right)
				parent->left->parent = parent;

			/* 'node right' becomes 'successor right' */
			successor->right = node->right;
			successor->right->parent = successor;
		}

		/* 'node left' becomes 'successor left' */
		successor->left = node->left;
		successor->left->parent = successor;

		/* 'successor' becomes 'gparent child' */
		avl_change_child(root, node->parent, node, successor);
		successor->parent = node->parent;

		if (successor == node->right) {
	        	avl_balance(root, successor->left);
		} else {
	        	avl_balance(root, successor);
		}

		return;
	}
	if (node->left) {
		/* 'node left' becomes 'parent child' */
		avl_change_child(root, parent, node, node->left);
		node->left->parent = parent;

		avl_balance(root, node->left);

		return;
	}
	if (node->right) {
		/* 'node right' becomes 'parent child' */
		avl_change_child(root, parent, node, node->right);
		node->right->parent = parent;

		avl_balance(root, node->right);

		return;
	}

	avl_change_child(root, parent, node, NULL);
	if (parent) {
		 /* sibling node */
		if (parent->left) {
			avl_balance(root, parent->left);
		} else if (parent->right) {
			avl_balance(root, parent->right);
		} else { /* parent */
			avl_balance(root, parent);
		}
	} /* 'parent' is nil == root */
} /* end */
