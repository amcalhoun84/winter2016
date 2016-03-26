#include<stdio.h>
#include<stdlib.h>
#include "bst.h"
#include "structs.h"

/* Example main file to begin exercising your tree */

/*
Functions to test

struct Node *_addNode(struct Node *cur, TYPE val)
int containsBSTree(struct BSTree *tree, TYPE val)
TYPE _leftMost(struct Node *cur)
struct Node *_removeLeftMost(struct Node *cur)
struct Node *_removeNode(struct Node *cur, TYPE val)
*/


struct Node {
	TYPE         val;
	struct Node *left;
	struct Node *right;
};

struct BSTree {
	struct Node *root;
	int          cnt;
};

TYPE _leftMost(struct Node *cur);
struct Node *_removeLeftMost(struct Node *cur);
struct Node *_removeNode(struct Node *cur, TYPE val);

struct BSTree *buildBSTTree() {
    /*     50
         13  110
        10 
    */
    struct BSTree *tree	= newBSTree();		
		
	/*Create value of the type of data that you want to store*/
	struct data *myData1 = (struct data *) malloc(sizeof(struct data));
	struct data *myData2 = (struct data *) malloc(sizeof(struct data));
	struct data *myData3 = (struct data *) malloc(sizeof(struct data));
	struct data *myData4 = (struct data *) malloc(sizeof(struct data));
		
	myData1->number = 50;
	myData1->name = "rooty";
	myData2->number = 13;
	myData2->name = "lefty";
	myData3->number = 110;
	myData3->name = "righty";
	myData4->number = 10;
	myData4->name = "lefty of lefty";
	
	/*add the values to BST*/
	addBSTree(tree, myData1);
	addBSTree(tree, myData2);
	addBSTree(tree, myData3);
	addBSTree(tree, myData4);
    
    return tree;
}


void testAddNode() {
    struct BSTree *tree	= newBSTree();
    
    struct data myData1;
	struct data myData2;
	struct data myData3;
	struct data myData4;
		
	myData1.number = 50;
	myData1.name = "rooty";
    addBSTree(tree, &myData1);
    
    
    if (compare(tree->root->val, (TYPE *) &myData1) != 0) {
        printf("addNode() test: FAIL to insert 50 as root\n");
        return;
    }
    else if (tree->cnt != 1) {
        printf("addNode() test: FAIL to increase count when inserting 50 as root\n");
        return;
    }
    else printf("addNode() test: PASS when adding 50 as root\n");
    
    
	myData2.number = 13;
	myData2.name = "lefty";
    addBSTree(tree, &myData2);
    
    
    if (compare(tree->root->left->val, (TYPE *) &myData2) != 0) {
        printf("addNode() test: FAIL to insert 13 as left child of root\n");
        return;
    }
    else if (tree->cnt != 2) {
        printf("addNode() test: FAIL to increase count when inserting 13 as left of root\n");
        return;
    }
    else printf("addNode() test: PASS when adding 13 as left of root\n");
    
    
	myData3.number = 110;
	myData3.name = "righty";
    addBSTree(tree, &myData3);
        
        
    if (compare(tree->root->right->val, (TYPE *) &myData3) != 0) {
        printf("addNode() test: FAIL to insert 110 as right child of root\n");
        return;
    }
    else if (tree->cnt != 3) {
        printf("addNode() test: FAIL to increase count when inserting 110 as right of root\n");
        return;
    }
    else printf("addNode() test: PASS when adding 110 as right of root\n");
    
    
	myData4.number = 10;
	myData4.name = "righty of lefty";
	addBSTree(tree, &myData4);
    
    if (compare(tree->root->left->left->val, (TYPE *) &myData4) != 0) {
        printf("addNode() test: FAIL to insert 10 as left child of left of root\n");
        return;
    }
    else if (tree->cnt != 4) {
        printf("addNode() test: FAIL to increase count when inserting 10 as left of left of root\n");
        return;
    }
    else printf("addNode() test: PASS when adding 10 as left of left of root\n");
}


void testContainsBSTree() {
    struct BSTree *tree = buildBSTTree();
    
    struct data myData1;
	struct data myData2;
	struct data myData3;
	struct data myData4;
	struct data myData5;
	
	myData1.number = 50;
	myData1.name = "rooty";
	myData2.number = 13;
	myData2.name = "lefty";
	myData3.number = 110;
	myData3.name = "righty";
	myData4.number = 10;
	myData4.name = "lefty of lefty";
    myData5.number = 111;
	myData5.name = "not in tree";
    
    
    if (containsBSTree(tree, &myData1))
        printf("containsBSTree(): PASS when test containing 50 as root\n");
    else
        printf("containsBSTree(): FAIL when test containing 50 as root\n");
        
    if (containsBSTree(tree, &myData2))
        printf("containsBSTree(): PASS when test containing 13 as left of root\n");
    else
        printf("containsBSTree(): FAIL when test containing 13 as left of root\n");
        
        
    if (containsBSTree(tree, &myData3))
        printf("containsBSTree(): PASS when test containing 110 as right of root\n");
    else
        printf("containsBSTree(): FAIL when test containing 110 as right of root\n");
        
    if (containsBSTree(tree, &myData4))
        printf("containsBSTree(): PASS when test containing 10 as left of left of root\n");
    else
        printf("containsBSTree(): FAIL when test containing 10 as left of left of root\n");
        
    if (!containsBSTree(tree, &myData5))
        printf("containsBSTree(): PASS when test containing 111, which is not in the tree\n");
    else
        printf("containsBSTree(): FAIL when test containing 111, which shouldn't be in the tree.\n");
}


void testLeftMost() {
    struct BSTree *tree = buildBSTTree();
    
    struct data myData1;
	struct data myData2;
	struct data myData3;
	struct data myData4;
		
	myData1.number = 50;
	myData1.name = "rooty";
	myData2.number = 13;
	myData2.name = "lefty";
	myData3.number = 110;
	myData3.name = "righty";
	myData4.number = 10;
	myData4.name = "lefty of lefty";
    
    if (compare(_leftMost(tree->root), &myData4) == 0)
        printf("_leftMost(): PASS left most of root\n");
    else
        printf("_leftMost(): FAIL left most of root\n");
    
    if (compare(_leftMost(tree->root->left), &myData4) == 0)
        printf("_leftMost(): PASS left most of left of root\n");
    else
        printf("_leftMost(): FAIL left most of left of root\n");
        
    if (compare(_leftMost(tree->root->left->left), &myData4) == 0)
        printf("_leftMost(): PASS left most of left of left of root\n");
    else
        printf("_leftMost(): FAIL left most of left of left of root\n");
    
    if (compare(_leftMost(tree->root->right), &myData3) == 0)
        printf("_leftMost(): PASS left most of right of root\n");
    else
        printf("_leftMost(): FAIL left most of right of root\n");
}


void testRemoveLeftMost() {
    struct BSTree *tree = buildBSTTree();
    struct Node *cur;
    
    cur = _removeLeftMost(tree->root);
    if (cur == tree->root)
        printf("_removeLeftMost: PASS removing leftmost of root 1st try\n");
    else 
        printf("_removeLeftMost: FAIL removing leftmost of root 1st try\n");
    
    cur = _removeLeftMost(tree->root->right);
    if (cur == NULL)
        printf("_removeLeftMost: PASS removing leftmost of right of root 1st try\n");
    else 
        printf("_removeLeftMost: FAIL removing leftmost of right of root 1st try\n");
        
    
    cur = _removeLeftMost(tree->root);
    if (cur == tree->root)
        printf("_removeLeftMost: PASS removing leftmost of root 2st try\n");
    else 
        printf("_removeLeftMost: FAIL removing leftmost of root 2st try\n");
    
}


void testRemoveNode() {
    struct BSTree *tree = buildBSTTree();
    struct Node *cur;
    struct data myData1;
	struct data myData2;
	struct data myData3;
	struct data myData4;
		
	myData1.number = 50;
	myData1.name = "rooty";
	myData2.number = 13;
	myData2.name = "lefty";
	myData3.number = 110;
	myData3.name = "righty";
	myData4.number = 10;
	myData4.name = "lefty of lefty";
    
    _removeNode(tree->root, &myData4);
    if (compare(tree->root->val, &myData1) == 0 && tree->root->left->left == NULL)
        printf("_removeNode(): PASS remove left of left of root 1st try\n");
    else
        printf("_removeNode(): FAIL remove left of left of root 1st try\n");
        
    _removeNode(tree->root, &myData3);
    if (compare(tree->root->val, &myData1) == 0 && tree->root->right == NULL)
        printf("_removeNode(): PASS remove right of root 2st try\n");
    else
        printf("_removeNode(): FAIL remove right of root 2st try\n");
    
    _removeNode(tree->root, &myData2);
    if (compare(tree->root->val, &myData1) == 0 && tree->root->left == 0)
        printf("_removeNode(): PASS remove left of root 3st try\n");
    else
        printf("_removeNode(): FAIL remove left of root 3st try\n");
        
    cur = _removeNode(tree->root, &myData1);
    if (cur == NULL)
        printf("_removeNode(): PASS remove root 4st try\n");
    else
        printf("_removeNode(): FAIL remove root 4st try\n");
        
}


int main(int argc, char *argv[])
{	
    testAddNode();
    testContainsBSTree();
    testLeftMost();
    testRemoveLeftMost();
    testRemoveNode();
    return 0;
}



