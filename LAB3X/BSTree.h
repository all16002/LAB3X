#ifndef BSTREE_H
#define BSTREE_H

#include <stdio.h>

struct treeNode
{
	int data;
	struct treeNode* left;
	struct treeNode* right;
};

typedef struct treeNode* BSTree;

/* Skapar ett tomt trad */
BSTree emptyTree(void);

/* Returnerar 1 ifall tradet är tomt, 0 annars */
int isEmpty(const BSTree tree);

/* Satter in 'data' sorterat i *tree */
void insertSorted(BSTree* tree, int data);

/* Utskriftsfunktioner*/
void printPreorder(const BSTree tree, FILE *textfile);
void printInorder(const BSTree tree, FILE *textfile);
void printPostorder(const BSTree tree, FILE *textfile);

/* Returnerar 1 om 'data' finns i tree, 0 annars */
int find(const BSTree tree, int data);

/* Tar bort 'data' från tradet om det finns */
void removeElement(BSTree* tree, int data);

/* Returnerar hur manga noder som totalt finns i tradet */
int numberOfNodes(const BSTree tree);

/* Returnerar hur djupt tradet ar */
int depth(const BSTree tree);

/* Returnerar minimidjupet för tradet */
int minDepth(const BSTree tree);

/* Balansera tradet sa att depth(tree) == minDepth(tree) */
void balanceTree(BSTree* tree);

/* Tom tradet och frigor minnet for de olika noderna */
void freeTree(BSTree* tree);

#endif
