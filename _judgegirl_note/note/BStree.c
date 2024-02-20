# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> //atoi()


typedef struct Treenode {
	int data;
	struct Treenode* left, * right;
} Node;

Node* genNode(int data, Node* left, Node* right);
Node* insertTree(Node* root, int data);
void printTree(Node* root);
void freeTree(Node* root);

# define key 5
int main() {
	int data;
	Node* root = NULL;

	for (int i = 0; i < key; i++) {
		scanf("%d", &data);
		root = insertTree(root, data); printf("%p\n", root);
	}
	printTree(root);
	freeTree(root);
	return 0;
}

Node* genNode(int data, Node* left, Node* right) {
	Node* node = malloc(sizeof(Node));
	node->data = data;
	node->left = left;
	node->right = right;
	return node;
}

Node* insertTree(Node* root, int data) {
	if (root == NULL)
		return genNode(data, NULL, NULL);
	if (data < root->data)
		root->left = insertTree(root->left, data);
	else
		root->right = insertTree(root->right, data);
	return root;
}

void printTree(Node* root) {
	if (root == NULL) return;
	printTree(root->left);
	printf("(do whatever you want~~) data = %d\n", root->data);
	printTree(root->right);
}

void freeTree(Node* root) {
	if (root == NULL) return;
	freeTree(root->left);
	freeTree(root->right);
	free(root);
}