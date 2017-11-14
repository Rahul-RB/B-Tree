#include <time.h>
#include <string.h>
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
	printf("How many records do you want to build from dataset?");
	scanf("%d",&len);

    recordNode *records = getData("data/dataset.csv", len);
    int size = len/(2*t-1);
    bTree* tree = createTree(size);
    
	clock_gettime(CLOCK_MONOTONIC,&start);    
    for(i=0;i<len;i++)
    {
    	insert(tree,&records[i]);
    }
	clock_gettime(CLOCK_MONOTONIC,&end);

    if(!strcmp(argv[1],"-b"))
    {
	    printf("Time Taken to build tree: %f seconds\n",((double)end.tv_sec + 1.0e-9*end.tv_nsec) - ((double)start.tv_sec + 1.0e-9*start.tv_nsec));
    	exit(0);
    }

    if(!strcmp(argv[1],"-s"))
    {
	    printf("Time Taken to build tree: %f seconds\n",((double)end.tv_sec + 1.0e-9*end.tv_nsec) - ((double)start.tv_sec + 1.0e-9*start.tv_nsec));
	    int key;
	    sscanf(argv[2],"%d",&key);
	    recordNode* res = search(tree,key);
    	printf("key\tcountry\tgrate\tscore\trate\n");
        printf("%d\t",res->key );
		printf("%s\t",res->country );
		printf("%s\t",res->grate );
		printf("%d\t",res->score );
		printf("%d\n",res->rate );
    }

    if(!strcmp(argv[1],"-d"))
    {
	    printf("Time Taken to build tree: %f seconds\n",((double)end.tv_sec + 1.0e-9*end.tv_nsec) - ((double)start.tv_sec + 1.0e-9*start.tv_nsec));
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
