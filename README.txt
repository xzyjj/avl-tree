	AVL-tree (self-balancing binary search tree) implementation

compile:
	gcc -g -o test avltree.c test_avltree.c
	gcc -g -o test_bench avltree.c xstdlib.c test_bench_avltree.c

licenses:
	GPL-3.0
	LGPL-2.0
