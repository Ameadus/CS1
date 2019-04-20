/*
Corn Maze.c by Alexander Meade COP3502
*/
#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 301 //1 more than max size specified
#define ILLEGAL 'X' //cant go to these
#define OUTERBOUND '~' //outside of maze
#define NOEXIT -1 // no exit found

typedef struct cord //cartisian coordinate
{
    int x;
    int y;
}cordM; //to call the struct cord


typedef struct node
{
    cordM* ptr; //point to cordM
    struct node* next; //next node
}nodeM; //to call the struct node

typedef struct queue
{
    nodeM* front; //ptr to front of queue
    nodeM* back; //ptr to back of queue
}queueM; //call to struct queue

//queue functions
void initialize(queueM* qPtr); //initalize queue to null
int enqueue(queueM* qPtr, int x1, int y1); //enqueue item to back
cordM* dequeue(queueM* qPtr); //dequeue an item from front
int empty(queueM* qPtr); //see if queue empty

//functions to solve
cordM* findStart(char maze[][MAXSIZE], int row, int col); //find start point
int breadthFirstSearch(char maze[][MAXSIZE], int row, int col, cordM* start); //must use

int main()
{
    int numCase,loop;
    scanf("%d", &numCase);//number of cases

    for(loop=0;loop<numCase;loop++)//repeat for each case
    {
    int row,col,i,j;
    scanf("%d%d", &row,&col);//size of maze matrix
    char maze[MAXSIZE][MAXSIZE]; //maze space

        for(i=0;i<row;i++)//scan in maze size
            scanf("%s",maze[i]);

    cordM* starting = findStart(maze,row,col); //get starting point
        if(starting != NULL)
            printf("%d\n",breadthFirstSearch(maze,row,col,starting));//find if u can escape or not
    }
    return 0; //was used for testing
}


void initialize(queueM* qPtr) //make both front/back ptrs null
{
     qPtr->front = NULL;
     qPtr->back = NULL;
}


int enqueue(queueM* qPtr, int x1, int y1)
{

    cordM* nextPt = malloc(sizeof(cordM)); //make new point space
    nextPt->x = x1; //x coord
    nextPt->y = y1; //y coord
    nodeM* temp; //the new node

    temp = malloc(sizeof(nodeM)); //make space for a new node


    if (temp != NULL) //make sure allocation works
        {
        //set up node to be stored at the back
        temp->ptr = nextPt;
        temp->next = NULL;

        if (qPtr->back != NULL) //if node not empty we must set back to the temp node
            qPtr->back->next = temp;

        qPtr->back = temp; //reset the back of the new node

        if (qPtr->front == NULL) //if queue was empty set the front of the queue
            qPtr->front = temp;

        return 1;//enqueue success
    }

    else
        return 0; //enqueue fails
}

cordM* dequeue(queueM* qPtr)
{
    nodeM* temp;

    if(qPtr->front == NULL)
        return NULL; //if front is null list is empty return null

    temp = qPtr->front; //temp pointer to store location so we can use old node

    qPtr->front = qPtr->front->next; //make the front the next node

    if (qPtr->front == NULL)
        qPtr->back = NULL; //if dequeue makes the queue empty make sure the back ptr empty also

    cordM* reval = temp->ptr; //store value to return

    free(temp); //free stored val

    return reval; //return dequeue value
}


int empty(queueM* qPtr)
{
    return qPtr->front == NULL; // if the front element is null then its an empty list
}


cordM* findStart(char maze[][MAXSIZE], int row, int col)//finding starting coordinate
{
    int i,j;
    for(i=0;i<row;i++)
        for(j=0;j<col;j++) //look thru all squares for S
            if(maze[i][j]== 'S') //if we find it make a temp cordM and store its x,y as the current i,j index
            {
            cordM* reval = malloc(sizeof(cordM));
            reval->x = i;
            reval->y = j;
            return reval;
            }

    return NULL;//cant find start
}

int breadthFirstSearch(char maze[][MAXSIZE], int row, int col, cordM* startLoc)
{
    //store all distances here initialize to not found.
    int i, j, distance[MAXSIZE][MAXSIZE];
    for (i=0; i<row; i++)
        for (j=0; j<col; j++)
            distance[i][j] = NOEXIT;

    //set up queue.
    queueM queueMaze;
    initialize(&queueMaze);
    int xpos = startLoc->x;
    int ypos = startLoc->y;
    enqueue(&queueMaze, xpos, ypos);
    free(startLoc);

    // Put in first distance.
    distance[xpos][ypos] = 0;

    // Start bfs.
    while (!empty(&queueMaze))
    {

        // Get next item and free memory once its stored
        cordM* nextPos = dequeue(&queueMaze);
        xpos = nextPos->x;
        ypos = nextPos->y;
        free(nextPos);
        int currentPos = distance[xpos][ypos];

        if (maze[xpos][ypos] == OUTERBOUND)
        {
            while (!empty(&queueMaze)) dequeue(&queueMaze);
            return currentPos;//answer
        }

        // Go left
        if (xpos > 0 && maze[xpos-1][ypos] != ILLEGAL && distance[xpos-1][ypos] == NOEXIT)
        {
            distance[xpos-1][ypos] = currentPos+1;
            enqueue(&queueMaze, xpos-1, ypos);
        }

        // Go right
        if (xpos < row-1 && maze[xpos+1][ypos] != ILLEGAL && distance[xpos+1][ypos] == NOEXIT)
        {
            distance[xpos+1][ypos] = currentPos+1;
            enqueue(&queueMaze, xpos+1, ypos);
        }

        // Go up
        if (ypos > 0 && maze[xpos][ypos-1] != ILLEGAL && distance[xpos][ypos-1] == NOEXIT)
        {
            distance[xpos][ypos-1] = currentPos+1;
            enqueue(&queueMaze, xpos, ypos-1);
        }

        // Go down
        if (ypos < col-1 && maze[xpos][ypos+1] != ILLEGAL && distance[xpos][ypos+1] == NOEXIT)
        {
            distance[xpos][ypos+1] = currentPos+1;
            enqueue(&queueMaze, xpos, ypos+1);
        }

    }


    return NOEXIT;

}
