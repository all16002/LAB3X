#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "BSTree.h"

/*Det ar helt tillatet att lagga till egna hjalpfunktioner men de befintliga funktionerna far inte andras*/

/* Statiska hjalpfunktioner anvands av andra funktioner i tradet och ska inte ligga i interfacet (anvandaren behover inte kanna till dessa) */


/* Skapar en tradnod med det givna datat genom att allokera minne for noden. Glom inte att initiera pekarna*/
static struct treeNode* createNode(int data)
{
    struct treeNode* createdNode;

    createdNode = malloc(sizeof(struct treeNode));
    if (createdNode == NULL) // allocation failed! oh no!!!
        return NULL;
    else
    {
        createdNode->left = NULL;
        createdNode->right = NULL;
        createdNode->data = data;
        return createdNode;
    }
}

/* Skriver en del av en sorterad array-version av ett trad */
static int writeArraySegment(const BSTree tree, int* arrayToWrite, int startIndex)
{
    if (!isEmpty(tree))
    {
        int stepsWritten = 0;
        if (tree->left == NULL && tree->right == NULL)
        {
            arrayToWrite[startIndex] = tree->data;
            return ++stepsWritten;
        }
        else
        {
            stepsWritten += writeArraySegment(tree->left, arrayToWrite, startIndex);
            arrayToWrite[startIndex + stepsWritten] = tree->data;
            stepsWritten++;
            stepsWritten += writeArraySegment(tree->right, arrayToWrite, startIndex + stepsWritten);
            return stepsWritten;
        }
    }
    else
        return 0;
}

/* Returnerar en dynamiskt allokerad array som innehaller tradets data sorterat */
static int* writeSortedToArray(const BSTree tree)
{
    /* Skapa en dynamisk array med ratt storlek

       Skriv datat fran tradet sorterat till arrayen (minsta till storsta)
       - till detta kanske du behover en hjalpfunktion */

    int* sortedArray = malloc(sizeof(int) * numberOfNodes(tree));

    if (sortedArray != NULL)
        writeArraySegment(tree, sortedArray, 0);

    return sortedArray;
}

/* Bygger upp ett sorterat, balanserat trad fran en sorterad array */
static void buildTreeSortedFromArray(BSTree* tree, const int arr[], int size)
{
    /* Bygg rekursivt fran mitten.
       Mittenelementet i en delarray skapar rot i deltradet
       Vanster delarray bygger vanster deltrad
       Hoger delarray bygger hoger deltrad*/

    if (size > 0)
    {
        int midIndex = size / 2;

        struct treeNode* createdNode = createNode(arr[midIndex]);

        buildTreeSortedFromArray(&(createdNode->left), arr, midIndex);
        buildTreeSortedFromArray(&(createdNode->right), arr + midIndex + 1, size - midIndex - 1);
        *tree = createdNode;
    }
}

/* Hittar adressen till ett givet varde i tradet */
BSTree* findNodeAddress(BSTree* tree, int data)
{
    if (!isEmpty(*tree))
    {
        struct treeNode* node = *tree;
        if (data == node->data)
            return tree;
        else if (data < node->data)
            return findNodeAddress(&(node->left), data);
        else // if (data > tree-> data)
            return findNodeAddress(&(node->right), data);
    }
    else // if the tree to search is empty, it will not contain the requested data
        return NULL;
}

/* Implementation av tradet, funktionerna i interfacet */

/* Skapar ett tomt trad - denna funktion ar fardig */
BSTree emptyTree(void)
{
    return NULL;
}

/* Returnerar 1 ifall tradet ar tomt, 0 annars */
int isEmpty(const BSTree tree)
{
    return (tree == NULL);
}

/* Satter in 'data' sorterat i *tree
 Post-condition: data finns i tradet*/
void insertSorted(BSTree* tree, int data)
{
    /*Tank pa att tradet kan vara tomt vid insattning
      Du bestammer sjalv hur dubletter ska hanteras, ska de inte accepteras eller
      ska de laggas till vanster/hoger?.
      Post-condition kan verifieras med hjalp av find(...)*/

    struct treeNode* nodeToCheck = *tree;

    if (isEmpty(nodeToCheck))
    { // checking at a) an empty tree or b) an empty leaf. either way, insert data here.
        struct treeNode* nodeToInsert = createNode(data);
        if (nodeToInsert != NULL)
            *tree = nodeToInsert;
        //  else if (nodeToInsert == NULL)
        //        something went wrong!
        return;
    }

    if (data < nodeToCheck->data)
        insertSorted(&(nodeToCheck->left), data);
    else if (data > nodeToCheck->data)
        insertSorted(&(nodeToCheck->right), data);
    //  else if (data == nodeToCheckAt->data)
    //        do nothing
}

/* Utskriftsfunktioner
   Vid anrop: anvand stdio som andra argument for att skriva ut pa skarmen
   Det racker att ni implementerar LR ordningarna*/
void printPreorder(const BSTree tree, FILE *textfile)
{
    if (!isEmpty(tree))
    {
        fprintf(textfile, "%d\n", tree->data);
        printPreorder(tree->left, textfile);
        printPreorder(tree->right, textfile);
    }
}

void printInorder(const BSTree tree, FILE *textfile)
{
    if (!isEmpty(tree))
    {
        printInorder(tree->left, textfile);
        fprintf(textfile, "%d\n", tree->data);
        printInorder(tree->right, textfile);
    }
}

void printPostorder(const BSTree tree, FILE *textfile)
{
    if (!isEmpty(tree))
    {
        printPostorder(tree->left, textfile);
        printPostorder(tree->right, textfile);
        fprintf(textfile, "%d\n", tree->data);
    }
}

/* Returnerar 1 om 'data' finns i tree, 0 annars */
int find(const BSTree tree, int data)
{
    if (findNodeAddress(&tree, data) != NULL)
        return 1;
    else
        return 0;
}

/* Tar bort 'data' fran tradet om det finns */
void removeElement(BSTree* tree, int data)
{
    /* Inget data ska/kan tas bort fran ett tomt trad
    Tre fall: Ett lov (inga barn), ett barn (vanster eller hoger), tva barn

    Glom inte att frigora noden nar den lankats ur tradet*/

    if (isEmpty(*tree))
        return;

    BSTree* nodeAddress = findNodeAddress(tree, data);

    if (!isEmpty(*nodeAddress))
    {
        struct treeNode* nodeToRemove = *nodeAddress;
        if (isEmpty(nodeToRemove->left))
        {
            *nodeAddress = nodeToRemove->right;
            free(nodeToRemove);
        }
        else if (isEmpty(nodeToRemove->right))
        {
            *nodeAddress = nodeToRemove->left;
            free(nodeToRemove);
        }
        else // if (neither left or right children of nodeToRemove are empty)
        {
            struct treeNode* successorNode = nodeToRemove->right;
            while (!isEmpty(successorNode->left))
                successorNode = successorNode->left;
            int successorData = successorNode->data;
            /*if (!isEmpty(successorNode->right))
            {
                BSTree* successorAddress = findNodeAddress(tree, successorData);
                *successorAddress = successorNode->right;
                free(successorNode);
            }*/
            removeElement(tree, successorData);
            nodeToRemove->data = successorData;
        }
    }
    // else if (isEmpty(*nodeAddress)
    //        do nothing;
    
}

/* Returnerar hur manga noder som totalt finns i tradet */
int numberOfNodes(const BSTree tree)
{
    if (!isEmpty(tree))
        return numberOfNodes(tree->left) + numberOfNodes(tree->right) + 1;
    else
        return 0;
}

/* Returnerar hur djupt tradet ar */
int depth(const BSTree tree)
{
    if (!isEmpty(tree))
    {
        int rightDepth = depth(tree->right) + 1;
        int leftDepth = depth(tree->left) + 1;
        return ((rightDepth > leftDepth) ? rightDepth : leftDepth);
    }
    else
        return 0;
}

/* Returnerar minimidjupet for tradet
   Se math.h for anvandbara funktioner*/
int minDepth(const BSTree tree)
{
    if (isEmpty(tree))
        return 0;
    else
    {
        double minDepth = log2(numberOfNodes(tree) + 1);
        
        return (int)ceil(minDepth);
    }
}

/* Balansera tradet sa att depth(tree) == minDepth(tree) */
void balanceTree(BSTree* tree)
{
    /* Forslag pa algoritm:
       - overfor tradet till en dynamiskt allokerad array med writeSortedToArray()
       - tom tradet med freeTree()
       - bygg upp tradet rekursivt fran arrayen med buildTreeSortedFromArray()
       - frigor minne for den dynamiskt allokerade arrayen


       Post-conditions:
       - tree har lika manga noder som tidigare
       - djupet for tradet ar samma som minimumdjupet for tradet */

    int* treeArray = writeSortedToArray(*tree);
    if (treeArray != NULL)
    {
        int treeSize = numberOfNodes(*tree);
        freeTree(tree);
        buildTreeSortedFromArray(tree, treeArray, treeSize);
        free(treeArray);
    }
    // else
    //        memory allocation failed. do nothing.
}

/* Tom tradet och frigor minnet for de olika noderna */
void freeTree(BSTree* tree)
{
    struct treeNode* nodeToFree = *tree;

    if (!isEmpty(nodeToFree))
    {
        freeTree(&(nodeToFree->left));
        freeTree(&(nodeToFree->right));
        nodeToFree->data = NULL;
        free(nodeToFree);
        *tree = NULL;
    }

    assert(isEmpty(*tree));
    // Post-condition: tradet ar tomt
}
