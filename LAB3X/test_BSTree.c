#include <assert.h>
#include "BSTree.h"
#include <stdio.h>

/*Funktion for att testa ditt trad.
  OBS! Preorder, Inorder och Postorder
       maste testas manuellt via menyn*/
void testTree(BSTree tree);

/*Meny som kan anvandas for att testa enstaka funktioner
  Ni far givetvis skriva en egen meny om ni vill*/
void menuTree(BSTree tree);


int main(void)
{
    BSTree tree = emptyTree();
    
	testTree(tree);
//	menuTree(tree);
    
    return 0;
}


void testTree(BSTree tree)
{
    printf("Starting test\n");
    
    //Tradet ska vara tomt från borjan
    assert(isEmpty(tree));
    //Tester på tomt träd
    assert(!find(tree, 7)); //leta i tomt träd
    removeElement(&tree, 7); //ta bort från tomt träd - programmet ska inte krasha
    assert(depth(tree) == 0 && minDepth(tree) == 0);
    balanceTree(&tree); //balansera ett tomt träd - programmet ska inte krasha
    
    
    // Satt in 7 element i tradet
    int arr[7] = {5,10,1,3,7,19,16}, i;
    for (i = 0; i < 7; i++)
    {
        insertSorted(&tree, arr[i]);
    }
    
    // Verifiera att alla element finns i tradet
    for (i = 0; i < 7; i++)
    {
        assert(find(tree, arr[i]));
    }
    
    assert(numberOfNodes(tree) == 7);
    assert(depth(tree) == 4); // Om vi satter in noderna i denna ordning blir djupet 4
    assert(minDepth(tree) == 3); // Med 7 noder ar minimidjupet 3
    
    removeElement(&tree, 7); //Ta bort ett lov
    removeElement(&tree, 19); // Tar bort ett element med ett barn
    removeElement(&tree, 5); // Tar bort ett element med tva barn
    assert(numberOfNodes(tree) == 4); // Tre noder har blivit borttagna
    // Verifiera att talen blivit borttagna
    assert(!find(tree, 7));
    assert(!find(tree, 19));
    assert(!find(tree, 5));
    
    //ta bort resterande noder i tradet
    removeElement(&tree, 10);
    removeElement(&tree, 1);
    removeElement(&tree, 3);
    removeElement(&tree, 16);
    
    assert(isEmpty(tree));
    
    //Lagg till i ett tomt trad
    insertSorted(&tree, 10);
    assert(find(tree, 10));
    
    // Addera 9 element sa att tradet blir obalanserat
    for (i = 0; i < 9; i++)
        insertSorted(&tree, i+20);
    
    assert(numberOfNodes(tree) == 10);
    
    //Verifiera att tradet ar obalanserat
    assert(depth(tree) != minDepth(tree));
    
    balanceTree(&tree);
    
    printf("Current number of nodes: %d\n", numberOfNodes(tree));

    
    assert(numberOfNodes(tree) == 10); // Verifiera att antalet noder ar detsamma
    assert(depth(tree) == minDepth(tree)); // Verifiera att tradet ar balanserat
    
    // Tom tradet och kontrollera att det ar tomt
    freeTree(&tree);
    assert(isEmpty(tree));
    assert(numberOfNodes(tree) == 0);
    assert(depth(tree) == 0);
    
    printf("Congratulations, your program passet the test\n");
}


void menuTree(BSTree tree)
{
    int choice, data;
    char c;
    
    do
    {
        printf("\n\n--------------MENU--------------\n"
               "1 - Add to tree\n"
               "2 - Remove from tree\n"
               "3 - Print in preorder\n"
               "4 - Print in inorder\n"
               "5 - Print in postorder\n"
               "6 - Number of nodes in tree\n"
               "7 - Depth of tree (actual and theoretical)\n"
               "8 - Is the tree empty?\n"
               "9 - Balance tree\n"
               "10 - Search in tree\n"
               "11 - Free tree\n"
               "12 - End program\n"
               "-----------------------------------\n"
               "Choice: ");
        
        scanf("%d", &choice);
        while((c = getchar()) != '\n' && c != EOF); //Rensar lasbufferten
        
        switch(choice)
        {
            case 1: printf("Data to add: ");
                scanf("%d", &data);
                insertSorted(&tree, data);
                printf("Tree: ");
                printPreorder(tree, stdout);
                break;
            case 2: printf("Data to remove: ");
                scanf("%d", &data);
                removeElement(&tree, data);
                printf("Tree: ");
                printPreorder(tree, stdout);
                break;
            case 3: printf("Tree in preorder: ");
                printPreorder(tree, stdout);
                break;
            case 4: printf("Tree in inorder: ");
                printInorder(tree, stdout);
                break;
            case 5: printf("Tree in postorder: ");
                printPostorder(tree, stdout);
                break;
            case 6: printf("Number of nodes in tree: %d", numberOfNodes(tree));
                break;
            case 7: printf("Actual depth of tree: %d\n", depth(tree));
                printf("Theoretical minimum depth of tree: %d\n", minDepth(tree));
                break;
            case 8: if (isEmpty(tree) == 1)
                printf("The tree is empty\n");
            else
                printf("The tree is not empty\n");
                
                break;
            case 9: balanceTree(&tree);
                break;
            case 10: printf("Data to search for: ");
                scanf("%d", &data);
                if (find(tree, data) == 1)
                    printf("%d was found in the tree", data);
                else
                    printf("%d was not found in the tree", data);
                
                break;
            case 11: freeTree(&tree);
                break;
            case 12: printf("Ending tree menu\n"); break;
            default: printf("Wrong input\n");
        }
        
    }while(choice != 12);
    
    freeTree(&tree);
    
}

