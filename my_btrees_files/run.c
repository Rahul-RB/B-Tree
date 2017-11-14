#include <time.h>
#include "headers.h"
int main(int argc, char const *argv[])
{
	struct timespec start={0,0};
    struct timespec end={0,0};

	if(argc==1)
	{
		printf("Please enter one of the options below:\n");
		printf("Commands:\n");
		printf("./run [OPTIONS]\n");
		printf("-b [KEY] -->for building a tree.\n");
		printf("-s [KEY] -->for searching a [KEY].\n");
		printf("-d [KEY] -->for deleting a [KEY].\n");
		printf("Exiting now\n");
		exit(0);
	}
	
	int len,i;
	double timeToBuild;
	double timeToSearch;
    printf("How many records do you want to build from dataset?");
	scanf("%d",&len);

    recordNode *records = getData("data/dataset.csv", len);
    bTree* tree = createTree("tree.dat",false);
    
	clock_gettime(CLOCK_MONOTONIC,&start);    
    for(i=0;i<len;i++)
    {
    	insert(tree,&records[i]);
    }
	clock_gettime(CLOCK_MONOTONIC,&end);
	timeToBuild = ((double)end.tv_sec + 1.0e-9*end.tv_nsec) - ((double)start.tv_sec + 1.0e-9*start.tv_nsec);
	if(!strcmp(argv[1],"-b"))
    {
    	printf("Time Taken to build tree: %f seconds\n",timeToBuild);
    	exit(0);
    }

    if(!strcmp(argv[1],"-s"))
    {
	    // printf("Time Taken to build tree: %f seconds\n",timeToBuild);
	    int key;
	    sscanf(argv[2],"%d",&key);
	    // bTree* tree = createTree("tree.dat",true);
		clock_gettime(CLOCK_MONOTONIC,&start);    
	    recordNode* res = search(tree,key);
		clock_gettime(CLOCK_MONOTONIC,&end);
		timeToSearch = ((double)end.tv_sec + 1.0e-9*end.tv_nsec) - ((double)start.tv_sec + 1.0e-9*start.tv_nsec);
	    printf("Time Taken to search: %f seconds\n",timeToSearch);
    	printf("key\tcountry\tgrate\tscore\trate\n");
        printf("%d\t",res->key );
		printf("%s\t",res->country );
		printf("%s\t",res->grate );
		printf("%d\t",res->score );
		printf("%d\n",res->rate );
    }

    if(!strcmp(argv[1],"-d"))
    {
	    printf("Time Taken to build tree: %f seconds\n",timeToBuild);
	    int key;
	    sscanf(argv[2],"%d",&key);
	    bool res = removeFromTree(tree,key);
    	if(res)
    	{
    		printf("Successfull Deletion.\n");
    	}
    	else
    	{
    		printf("Deletion not successful.\n");
    	}
    }

}	
