#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
// 1 , AFG , DIST , 93 , 18
struct rec
{
	int key;
	// rest
	char country[5];
	char grate[10];
	int score;
	int rate;
};
typedef struct rec recordNode;

struct bTreeNode
{
	bool isLeaf; //leaf or not?, if its leaf it's 1, else 0.
	int pos; //tells where in node array is this treeNode present.
	int noOfRecs;
	recordNode* recordArr[2 * t - 1];
	int children[2 * t]; // links
};
typedef struct bTreeNode bTreeNode;

struct tree
{
	char fileName[20];
	FILE* fp;
	int root;
	int nextPos;
};
typedef struct tree bTree;

bTree* createTree(char* fileName,bool mode);
bTreeNode* nodeInit(bTreeNode* node,bool isLeaf,bTree* tree);
void insert(bTree* tree,recordNode* record);
void delete(bTree* tree,int key);
void traverse(bTree* tree, int root);
void dispNode(bTreeNode* node);
void writeFile(bTree* ptr_tree, bTreeNode* p, int pos);
void readFile(bTree* ptr_tree, bTreeNode* p, int pos);


void enterData(recordNode* record, int id_num, char country[], char Grate[], int Score, int Rate);
recordNode* getData(char *filepath, int len);
recordNode* search(bTree* tree, int key);
recordNode* searchRecursive(bTree* tree, int key, bTreeNode* root);
bool removeFromTree(bTree* tree, int key);
void merge(bTree* tree, bTreeNode *node, int idx);
void borrowFromNext(bTree* tree, bTreeNode *node, int idx);
void borrowFromPrev(bTree* tree, bTreeNode *node, int idx);
void fill(bTree* tree, bTreeNode *node, int idx);
recordNode *getSucc(bTree* tree, bTreeNode *node, int idx);
recordNode *getPred(bTree* tree, bTreeNode *node, int idx);
bool removeFromNonLeaf(bTree* tree, bTreeNode *node, int idx);
bool removeFromLeaf (bTree* tree, bTreeNode *node, int idx);
bool removeNode(bTree* tree, bTreeNode* node, int k);
int findKey(bTreeNode* node, int k);