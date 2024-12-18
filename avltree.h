/* avltree.h - avl-tree (self-balancing binary search tree) definitions */
#ifndef DS_AVLTREE_H
#define DS_AVLTREE_H

#include "xstddef.h"


struct avl_node {
	int32 bf;
	struct avl_node *parent, *left, *right;
};

struct avl_root {
	struct avl_node *node;
};

#undef AVL_HEIGHT
#define AVL_HEIGHT(x) ((x) ? (x)->bf : 0)
#undef AVL_MAX
#define AVL_MAX(x1, x2) (((x1) > (x2)) ? x1 : x2)

void avl_change_child(struct avl_root *root, struct avl_node *parent,
		struct avl_node *old, struct avl_node *new);
void avl_update_height(struct avl_node *node);
int32 avl_balance_factor(struct avl_node *node);
void avl_left_rotate(struct avl_root *root, struct avl_node *parent);
void avl_right_rotate(struct avl_root *root, struct avl_node *parent);
void avl_balance(struct avl_root *root, struct avl_node *node);
void avl_erase_fix(struct avl_root *root, struct avl_node *node);


#endif
