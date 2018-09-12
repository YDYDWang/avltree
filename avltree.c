#include <stdlib.h>
#include <windows.h>

typedef struct _AVL_NODE {
	int val;
	struct _AVL_NODE *parent;
	struct _AVL_NODE *left;
	struct _AVL_NODE *right;
	int height;
} AVL_NODE, *PAVL_NODE;


PAVL_NODE root = NULL;

void rotate_right(PAVL_NODE root, PAVL_NODE pivot) {
	pivot->parent = root->parent;
	root->parent = pivot;
	root->left = pivot->right;
	pivot->right = root;
	root->height = height(root);
	pivot->height = height(pivot);
	if (pivot->parent != NULL) {
		pivot->parent->left = pivot;
	}
}

void rotate_left(PAVL_NODE root, PAVL_NODE pivot) {
	pivot->parent = root->parent;
	root->parent = pivot;
	root->right = pivot->left;
	pivot->left = root;
	root->height = height(root);
	pivot->height = height(pivot);
	if (pivot->parent != NULL) {
		pivot->parent->right = pivot;
	}
}

int balance_factor(PAVL_NODE node) {
	if (node->left == NULL) {
		if (node->right == NULL) {
			return 0;
		} else {
			return (-node->right->height);
		}
	} else {
		if (node->right == NULL) {
			return node->left->height;
		} else {
			return node->left->height - node->right->height;
		}
	}
}

int height(PAVL_NODE node) {
	if (node->left == NULL) {
		if (node->right == NULL) {
			return 1;
		} else {
			return node->right->height + 1;
		}
	} else {
		if (node->right == NULL) {
			return node->left->height + 1;
		} else {
			return  max(node->left->height, node->right->height) + 1;
		}
	}
}

void trace_to_root(PAVL_NODE node) {
	PAVL_NODE n = node;
	while (n != NULL) {
		int factor = balance_factor(n);
		if (factor == 2) {
			if (n->left->val > node->val) {
				rotate_right(n, n->left);
			} else {
				rotate_left(n->left, n->left->right);
				rotate_right(n, n->left);
			}
		} else if (factor == -2) {
			if (n->right->val < node->val) {
				rotate_left(n, n->right);
			} else {
				rotate_right(n->right, n->left->left);
				rotate_left(n, n->right);
			}
		} else if (factor > 2 || factor < -2) {
			printf("[FATAL] Invalid balance factor: %d", factor);
		} else {
			n->height = height(n);
		}
		root = n;
		n = n->parent;
	}
}

BOOL insert(int val) {
	PAVL_NODE node;
	if (init_avl_node(val, &node)) {
		if (root == NULL) {
			root = node;
			return TRUE;
		} else if (search_then_insert(node)) {
			trace_to_root(node);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL search_then_insert(PAVL_NODE node) {
	PAVL_NODE n = root;
	while (TRUE) {
		if (n->val > node->val) {
			if (n->left != NULL) {
				n = n->left;
			} else {
				n->left = node;
				node->parent = n;
				return TRUE;
			}
		}
		else if (n->val < node->val) {
			if (n->right != NULL) {
				n = n->right;
			} else {
				n->right = node;
				node->parent = n;
				return TRUE;
			}
		} else {
			printf("[WARN] Duplicate value: %d was detected", node->val);
			return FALSE;
		}
	}
	return FALSE;
}

BOOL init_avl_node(int val, PAVL_NODE *node) {
	PAVL_NODE n = malloc(sizeof(AVL_NODE));
	if (n == NULL) {
		printf("[FATAL] Fail to allocate memory for AVL_NODE\n");
		return FALSE;
	}
	*node = n;
	n->val = val;
	n->height = 1;
	n->parent = NULL;
	n->left = NULL;
	n->right = NULL;
	return TRUE;
}

PAVL_NODE search(int val) {
	PAVL_NODE n = root;
	while (n != NULL) {
		if (n->val == val) {
			return n;
		} else if (n->val > val) {
			n = n->left;
		} else {
			n = n->right;
		}
	}
	return NULL;
}

int main(void) {
	printf("sizeof(AVL_NODE): %d\n", sizeof(AVL_NODE));
	printf("sizeof(PAVL_NODE): %d\n", sizeof(PAVL_NODE));
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	LARGE_INTEGER start;
	QueryPerformanceCounter(&start);
	for (int i = 0; i < 655360; i++) {
		if (!insert(i)) {
			printf("error at val: %d\n", i);
		}
	}
	LARGE_INTEGER end;
	QueryPerformanceCounter(&end);
	long time = ((end.QuadPart - start.QuadPart) * 1000) / frequency.QuadPart;
	printf("cost: %d(ms)\n", time);

	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
	PAVL_NODE avl_node = search(2);
	QueryPerformanceCounter(&end);
	time = end.QuadPart - start.QuadPart;
	printf("cost: %d(count)\n", time);
	getchar();
}