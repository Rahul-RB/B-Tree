#include "headers.h"
bTree* createTree(char* fileName,bool mode)
{
	bTree* tree = malloc(sizeof(bTree));
	if(!mode) //new file
    {
        strcpy(tree->fileName,fileName);
        tree->fp = fopen(fileName,"w");
        fclose(tree->fp);
        tree->fp = fopen(fileName,"r+");
        tree->root = 0;
        tree->nextPos = 0;
        return tree;
    }
    FILE *oldFile = fopen("tree.dat", "r");
    fread(tree, sizeof(bTree), 1, oldFile);
    fclose(oldFile);
    tree->fp = fopen(fileName, "r+");
    return tree;
}

bTreeNode* nodeInit(bTreeNode* node,bool isLeaf,bTree* tree)
{
	node->isLeaf = isLeaf;
	node->noOfRecs=0;
	node->pos = tree->nextPos;
	(tree->nextPos)++;
	int i;
	for (int i = 0; i < 2*t; ++i)
	{
		node->children[i] = -1;
	}
	return node;
}

void writeFile(bTree* ptr_tree, bTreeNode* p, int pos) {// pos = -1; use nextPos {
    // printf("w:%d\n", pos);
    if(pos == -1) {
        pos = ptr_tree->nextPos++;
    }
    fseek(ptr_tree->fp, pos * sizeof(bTreeNode), 0);
    fwrite(p, sizeof(bTreeNode), 1, ptr_tree->fp);
    
}

void readFile(bTree* ptr_tree, bTreeNode* p, int pos) {    
    fseek(ptr_tree->fp, pos * sizeof(bTreeNode), 0);
    fread(p, sizeof(bTreeNode), 1, ptr_tree->fp);
}

void enterData(recordNode* record, int id_num, char country[], char grate[], int score, int rate) {
    
    record->key = id_num;
    strcpy(record->country, country);
    strcpy(record->grate, grate);
    record->score = score;
    record->rate = rate;
    
    return;
}
recordNode* getData(char *filepath, int len) {
    
    recordNode *recordArr = malloc(sizeof(recordNode)*len);
    char delim = ','; char line[256];
    int file_no = 0;
    int i,key,score,rate;
    char country[255];
    char grate[255];
    
    FILE *inpFile = fopen(filepath, "r");
    
    while(file_no < len && fscanf(inpFile, "%d,%[^,],%[^,],%d,%d", &key, country, grate, &score, &rate)) {
        enterData(&recordArr[file_no], key, country, grate, score, rate);
        file_no++;
    }

    return recordArr;
}

void splitChild(bTree* tree, bTreeNode* x, int i, bTreeNode* y)
{
	bTreeNode* z = malloc(sizeof(bTreeNode));
	nodeInit(z,y->isLeaf,tree);
	z->noOfRecs = t-1;

	int j;
	for(j=0;j<t-1;j++)
	{
		z->recordArr[j] = y->recordArr[j+t];
	}

	if(!y->isLeaf)
	{
		for(j=0;j<t;j++)
		{
			z->children[j] = y->children[j+t];
		}
	}
	y->noOfRecs = t-1;

	for(j=(x->noOfRecs);j >= i+1;j--)
	{
		x->children[j+1] = x->children[j];
	}
	//linking z
	x->children[i+1] = z->pos;
	for(j=(x->noOfRecs)-1;j >= i;j--)
	{
		x->recordArr[j+1] = x->recordArr[j];
	}
	x->recordArr[i] = y->recordArr[t-1];
	x->noOfRecs++;
	// tree->node[x->pos] = x;
	// tree->node[y->pos] = y;
	// tree->node[z->pos] = z;
    writeFile(tree, x, x->pos);
    writeFile(tree, y, y->pos);
    writeFile(tree, z, z->pos);
	// free(z);
}

void insertNonFull(bTree* tree,bTreeNode* x,recordNode* record)
{	
	int i = (x->noOfRecs)-1;
	if(x->isLeaf == true)
	{
		while((i>=0) && (record->key < x->recordArr[i]->key))
		{
			x->recordArr[i+1] = x->recordArr[i];
			i--;
		}
		x->recordArr[i+1] = record;
		(x->noOfRecs)++;
        writeFile(tree, x, x->pos);
		// tree->node[x->pos]=x;
	}
	else
	{
		while((i>=0) && (record->key < x->recordArr[i]->key))
		{
			i=i-1;
		}
		// bTreeNode* childAtPosi = nodeInit(tree,0,tree->nextPos); //TODO
		bTreeNode* childAtPosi = malloc(sizeof(bTreeNode));
        readFile(tree, childAtPosi, x->children[i+1]);
		// childAtPosi = tree->node[x->children[i+1]]; //doubtful
		if(childAtPosi->noOfRecs == (2*t-1))
		{
			splitChild(tree,x,i+1,childAtPosi);
			if( x->recordArr[i+1]->key < record->key){
				i++;
			}
		}
        readFile(tree, childAtPosi, x->children[i+1]);
		// childAtPosi = tree->node[x->children[i+1]];
		insertNonFull(tree,childAtPosi,record);
		// free(childAtPosi);
	}
}

void insert(bTree* tree,recordNode* record)
{
	if(tree->nextPos == 0) //empty tree, first element.
	{
		tree->root = tree->nextPos;
		bTreeNode* firstNode = malloc(sizeof(bTreeNode));
		nodeInit(firstNode,true,tree);
		firstNode->recordArr[0] = record;
		(firstNode->noOfRecs)++;
        writeFile(tree, firstNode, firstNode->pos);
		// tree->node[firstNode->pos] = firstNode;
		// free(firstNode);
		return;
	}
	else
	{
		// bTreeNode* rootCopy = nodeInit(tree,false);
		bTreeNode* rootCopy = malloc(sizeof(bTreeNode));
        readFile(tree, rootCopy, tree->root);
		// rootCopy = tree->node[tree->root];

		if(rootCopy->noOfRecs == (2*t-1))
		{
			bTreeNode* newRoot = malloc(sizeof(bTreeNode));
			nodeInit(newRoot,false,tree);
			newRoot->children[0] = tree->root;

			splitChild(tree,newRoot,0,rootCopy);

			int i=0;
			if(newRoot->recordArr[0]->key < record->key){
				i++;
			}
			
			bTreeNode* childAtPosi = malloc(sizeof(bTreeNode));
            readFile(tree, childAtPosi, newRoot->children[i]);
			// childAtPosi = tree->node[newRoot->children[i]];//doubtful
			insertNonFull(tree,childAtPosi,record);
			tree->root = newRoot->pos;
            writeFile(tree, rootCopy, rootCopy->pos);
            // tree->node[rootCopy->pos] = rootCopy;

			// free(newRoot);
		}
		else
		{
			insertNonFull(tree,rootCopy,record);
		}
		// free(rootCopy);
	}
}

void traverse(bTree* tree, int root) {
    
    if(-1 == root) {    
        return;
    }
    
    bTreeNode *toPrint = malloc(sizeof(bTreeNode));
    
    readFile(tree, toPrint, root);
    // toPrint = tree->node[root];
    
    dispNode(toPrint);

    for(int i = 0; i < 2*t; i++) {
        traverse(tree, toPrint->children[i]);
    }
    // free(toPrint);
}

void dispNode(bTreeNode* node)
{
	printf("Number of Records:%d\n",node->noOfRecs );
	printf("Position in node:%d\n",node->pos );
	printf("Is leaf?:%d\n",node->isLeaf );
	printf("Keys:\n");
	for(int i = 0; i < node->noOfRecs; i++)
	{
		printf("%d ", node->recordArr[i]->key);
	}
	printf("\n");
	printf("Links:\n");
	for (int i = 0; i < 2*t; ++i)
	{
		printf("%d ",node->children[i] );
	}
	printf("\n");

}

recordNode* searchRecursive(bTree* tree, int key, bTreeNode* root) {
    int i = 0;
    
    while(i < root->noOfRecs && key > root->recordArr[i]->key)
        i++;
    
    
    if(i < root->noOfRecs && key == root->recordArr[i]->key)
        return root->recordArr[i];
    
    
    else if(root->isLeaf) {
        return NULL;
    }
    else {
        bTreeNode* childAtPosi = malloc(sizeof(bTreeNode));
        readFile(tree, childAtPosi, root->children[i]);
        // childAtPosi = tree->node[root->children[i]];
        return searchRecursive(tree, key, childAtPosi);
    }
}

recordNode* search(bTree* tree, int key) {
    
    bTreeNode* root = malloc(sizeof(bTreeNode));
    readFile(tree, root, tree->root);
    // root = tree->node[tree->root];
    return searchRecursive(tree, key, root);
     
}

int findKey(bTreeNode* node, int k) {
    int idx=0;
    while (idx < node->noOfRecs && node->recordArr[idx]->key < k)
        ++idx;
    return idx;
}

// A function to remove the idx-th key from this node - which is a leaf node
bool removeFromLeaf (bTree* tree, bTreeNode *node, int idx) {
 
    // Move all the keys after the idx-th pos one place backward
    for (int i=idx+1; i<node->noOfRecs; ++i)
        node->recordArr[i-1] = node->recordArr[i];
 
    // Reduce the count of keys
    node->noOfRecs--;
 
    return true;
}
 
// A function to remove the idx-th key from this node - which is a non-leaf node
bool removeFromNonLeaf(bTree* tree, bTreeNode *node, int idx) {
 
    int k = node->recordArr[idx]->key;
    bool res;
    
    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));
    
    readFile(tree, child, node->children[idx]);
    // child = tree->node[node->children[idx]];
    readFile(tree, sibling, node->children[idx-1]);
    // sibling = tree->node[node->children[idx-1]];
 
    // If the child that precedes k (C[idx]) has atleast t keys,
    // find the predecessor 'pred' of k in the subtree rooted at
    // C[idx]. Replace k by pred. Recursively delete pred
    // in C[idx]
    if (child->noOfRecs >= t) {
        recordNode* pred = getPred(tree, node, idx);
        node->recordArr[idx] = pred;
        res = removeNode(tree, child, pred);
    }
 
    // If the child C[idx] has less that t keys, examine C[idx+1].
    // If C[idx+1] has atleast t keys, find the successor 'succ' of k in
    // the subtree rooted at C[idx+1]
    // Replace k by succ
    // Recursively delete succ in C[idx+1]
    else if  (sibling->noOfRecs >= t)
    {
        recordNode* succ = getSucc(tree, node, idx);
        node->recordArr[idx] = succ;
        res = removeNode(tree, sibling, succ);
    }
 
    // If both C[idx] and C[idx+1] has less that t keys,merge k and all of C[idx+1]
    // into C[idx]
    // Now C[idx] contains 2t-1 keys
    // Free C[idx+1] and recursively delete k from C[idx]
    else {
        merge(tree, node, idx);
        res = removeNode(tree, child, k);
    }
    
    writeFile(tree, child, child->pos);
    // tree->node[child->pos] = child;
    writeFile(tree, sibling, sibling->pos);
  	// tree->node[sibling->pos] = sibling;  
    return res;
}


bool removeNode(bTree* tree, bTreeNode* node, int k) {
    int idx = findKey(node, k);
    bool res;
    // The key to be removed is present in this node
    if (idx < node->noOfRecs && node->recordArr[idx]->key == k) {
 
        // If the node is a leaf node - removeFromLeaf is called
        // Otherwise, removeFromNonLeaf function is called
        if (node->isLeaf)
            res = removeFromLeaf(tree, node, idx);
        else
            res = removeFromNonLeaf(tree, node, idx);
        
        writeFile(tree, node, node->pos);
        // tree->node[node->pos] = node;
    }
    else {
 
        // If this node is a leaf node, then the key is not present in tree
        if (node->isLeaf) {
            return false;
        }
 
        // The key to be removed is present in the sub-tree rooted with this node
        // The flag indicates whether the key is present in the sub-tree rooted
        // with the last child of this node
        bool flag = ( (idx==node->noOfRecs)? true : false );
 
        // If the child where the key is supposed to exist has less that t keys,
        // we fill that child
        
        bTreeNode *childAtPosi = malloc(sizeof(bTreeNode));
        readFile(tree, childAtPosi, node->children[idx]);
        // childAtPosi = tree->node[node->children[idx]];
        
        if (childAtPosi->noOfRecs < t)
            fill(tree, node, idx);
 
        // If the last child has been merged, it must have merged with the previous
        // child and so we recurse on the (idx-1)th child. Else, we recurse on the
        // (idx)th child which now has atleast t keys
        if (flag && idx > node->noOfRecs) {
            bTreeNode *sibling = malloc(sizeof(bTreeNode));
            readFile(tree, sibling, node->children[idx-1]);
            // sibling = tree->node[node->children[idx-1]];
            removeNode(tree, sibling, k);
            writeFile(tree, sibling, sibling->pos);
            // tree->node[sibling->pos] = sibling;
        }
        else
            removeNode(tree, childAtPosi, k);
        
        writeFile(tree, childAtPosi, childAtPosi->pos);
        // tree->node[childAtPosi->pos] = childAtPosi;
        res = true;
    }
    return res;
}


// A function to get predecessor of keys[idx]
recordNode* getPred(bTree* tree, bTreeNode *node, int idx) {
    // Keep moving to the right most node until we reach a leaf
    
    bTreeNode *curr = malloc(sizeof(bTreeNode));
    readFile(tree, curr, node->children[idx]);
    // curr = tree->node[node->children[idx]];
    while (!curr->isLeaf){
        readFile(tree, curr, curr->children[curr->noOfRecs]);
        // curr = tree->node[curr->children[curr->noOfRecs]];
    }
        
    // Return the last key of the leaf
    // return &(curr->recordArr[curr->n-1]);
    return curr->recordArr[curr->noOfRecs-1];
}
 
recordNode *getSucc(bTree* tree, bTreeNode *node, int idx) {
 
    // Keep moving the left most node starting from C[idx+1] until we reach a leaf
    bTreeNode *curr = malloc(sizeof(bTreeNode));
    readFile(tree, curr, node->children[idx]);
    // curr = tree->node[node->children[idx]];
    while (!curr->isLeaf){
        readFile(tree, curr, curr->children[0]);
    	// curr = tree->node[curr->children[0]];
    }
 
    // Return the first key of the leaf
    // return &(curr->recordArr[0]);
    return curr->recordArr[0];
}
 
// A function to fill child C[idx] which has less than t-1 keys
void fill(bTree* tree, bTreeNode *node, int idx) {
 
    bTreeNode *cPrev = malloc(sizeof(bTreeNode));
    bTreeNode *cSucc = malloc(sizeof(bTreeNode));
    readFile(tree, cPrev, node->children[idx-1]);
    // cPrev = tree->node[node->children[idx-1]];
    readFile(tree, cSucc, node->children[idx+1]);
    // cSucc = tree->node[node->children[idx+1]];
    // If the previous child(C[idx-1]) has more than t-1 keys, borrow a key
    // from that child
    if (idx!=0 && cPrev->noOfRecs>=t)
        borrowFromPrev(tree, node, idx);
 
    // If the next child(C[idx+1]) has more than t-1 keys, borrow a key
    // from that child
    else if (idx!=node->noOfRecs && cSucc->noOfRecs>=t)
        borrowFromNext(tree, node, idx);
 
    // Merge C[idx] with its sibling
    // If C[idx] is the last child, merge it with with its previous sibling
    // Otherwise merge it with its next sibling
    else {
        if (idx != node->noOfRecs)
            merge(tree, node, idx);
        else
            merge(tree, node, idx-1);
    }
    return;
}
 
// A function to borrow a key from C[idx-1] and insert it
// into C[idx]
void borrowFromPrev(bTree* tree, bTreeNode *node, int idx) {
    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));
    
    readFile(tree, child, node->children[idx]);
    // child = tree->node[node->children[idx]];
    readFile(tree, sibling, node->children[idx-1]);
    // sibling = tree->node[node->children[idx-1]];
    // The last key from C[idx-1] goes up to the parent and key[idx-1]
    // from parent is inserted as the first key in C[idx]. Thus, the  loses
    // sibling one key and child gains one key
 
    // Moving all key in C[idx] one step ahead
    for (int i=child->noOfRecs-1; i>=0; --i)
        child->recordArr[i+1] = child->recordArr[i];
 
    // If C[idx] is not a leaf, move all its child pointers one step ahead
    if (!child->isLeaf) {
        for(int i=child->noOfRecs; i>=0; --i)
            child->children[i+1] = child->children[i];
    }
 
    // Setting child's first key equal to keys[idx-1] from the current node
    child->recordArr[0] = node->recordArr[idx-1];
 
    // Moving sibling's last child as C[idx]'s first child
    if (!node->isLeaf)
        child->children[0] = sibling->children[sibling->noOfRecs];
 
    // Moving the key from the sibling to the parent
    // This reduces the number of keys in the sibling
    node->recordArr[idx-1] = sibling->recordArr[sibling->noOfRecs-1];
 
    child->noOfRecs += 1;
    sibling->noOfRecs -= 1;
    
    writeFile(tree, node, node->pos);
    // tree->node[node->pos]=node;
    writeFile(tree, child, child->pos);
    // tree->node[child->pos]=child;
    writeFile(tree, sibling, sibling->pos);
    // tree->node[sibling->pos]=sibling;
    
    return;
}
 
// A function to borrow a key from the C[idx+1] and place
// it in C[idx]
void borrowFromNext(bTree* tree, bTreeNode *node, int idx) {
 
    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));
    
    readFile(tree, child, node->children[idx]);
    // child = tree->node[node->children[idx]];
    readFile(tree, sibling, node->children[idx+1]);
 	// sibling = tree->node[node->children[idx+1]];
    // keys[idx] is inserted as the last key in C[idx]
    child->recordArr[(child->noOfRecs)] = node->recordArr[idx];
 
    // Sibling's first child is inserted as the last child
    // into C[idx]
    if (!(child->isLeaf))
        child->children[(child->noOfRecs)+1] = sibling->children[0];
 
    //The first key from sibling is inserted into keys[idx]
    node->recordArr[idx] = sibling->recordArr[0];
 
    // Moving all keys in sibling one step behind
    for (int i=1; i<sibling->noOfRecs; ++i)
        sibling->recordArr[i-1] = sibling->recordArr[i];
 
    // Moving the child pointers one step behind
    if (!sibling->isLeaf) {
        for(int i=1; i<=sibling->noOfRecs; ++i)
            sibling->children[i-1] = sibling->children[i];
    }
 
    // Increasing and decreasing the key count of C[idx] and C[idx+1]
    // respectively
    child->noOfRecs += 1;
    sibling->noOfRecs -= 1;
    
    writeFile(tree, node, node->pos);
    // tree->node[node->pos]=node;
    writeFile(tree, child, child->pos);
    // tree->node[child->pos]=child;
    writeFile(tree, sibling, sibling->pos);
    // tree->node[sibling->pos]=sibling;

    return;
}
 
// A function to merge C[idx] with C[idx+1]
// C[idx+1] is freed after merging
void merge(bTree* tree, bTreeNode *node, int idx) {
    
    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));
    
    readFile(tree, child, node->children[idx]);
    // child = tree->node[node->children[idx]];
    readFile(tree, sibling, node->children[idx+1]);
 	// sibling = tree->node[node->children[idx+1]];
    // Pulling a key from the current node and inserting it into (t-1)th
    // position of C[idx]
    child->recordArr[t-1] = node->recordArr[idx];
 
    // Copying the keys from C[idx+1] to C[idx] at the end
    for (int i=0; i<sibling->noOfRecs; ++i)
        child->recordArr[i+t] = sibling->recordArr[i];
 
    // Copying the child pointers from C[idx+1] to C[idx]
    if (!child->isLeaf) {
        for(int i=0; i<=sibling->noOfRecs; ++i)
            child->children[i+t] = sibling->children[i];
    }
 
    // Moving all keys after idx in the current node one step before -
    // to fill the gap created by moving keys[idx] to C[idx]
    for (int i=idx+1; i<node->noOfRecs; ++i)
        node->recordArr[i-1] = node->recordArr[i];
 
    // Moving the child pointers after (idx+1) in the current node one
    // step before
    for (int i=idx+2; i<=node->noOfRecs; ++i)
        node->children[i-1] = node->children[i];
 
    // Updating the key count of child and the current node
    child->noOfRecs += sibling->noOfRecs+1;
    node->noOfRecs--;
 
    // Freeing the memory occupied by sibling
    writeFile(tree, node, node->pos);
    // tree->node[node->pos]=node;
    writeFile(tree, child, child->pos);
    // tree->node[child->pos]=child;
    writeFile(tree, sibling, sibling->pos);
    // tree->node[sibling->pos]-sibling;
    return;
}

bool removeFromTree(bTree* tree, int key) {
    
    bTreeNode *root = malloc(sizeof(bTreeNode));
    readFile(tree, root, tree->root);
    // root = tree->node[tree->root];
    return removeNode(tree, root, key);
    
}