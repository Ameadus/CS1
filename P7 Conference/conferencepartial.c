/*
Alexander Meade COP3502C conference.c
*/
#include <stdio.h>
#include <stdlib.h>
#define MAXHEAP 100000

struct heapStruct{
    int* heaparray;
    int capacity;
    int size;
}heap;

//heap/priority queue functions
void swap(struct heapStruct *h, int index1, int index2);
int minimum(int a, int indexa, int b, int indexb);
void percolateDown(struct heapStruct *h, int index);
void percolateUp(struct heapStruct *h, int index);
int insert(struct heapStruct *h, int value);
struct heapStruct* initHeap();
int removeMin(struct heapStruct *h);
void heapSort(int values[], int length);
struct heapStruct * initHeapfromArray(int* values, int length);
void heapify(struct heapStruct *h);
void freeHeap(struct heapStruct *h);

int simulate (int* talkTimes, int length, int numRooms);
int binSearch(int* times, int length, int maxTime);
void heapSort(int values[], int length); //

int main(void){
    int numLecs,maxTime,i;
    int* times;
    struct heapStruct* solution;
    scanf("%d %d", &numLecs, &maxTime);
    for(i=0; i<numLecs; i++ ){
        scanf("%d", times);
        printf("%d", binSearch(times,numLecs,maxTime));

    }
}



int simulate (int* talkTimes, int length, int numRooms){
    int totalTime=0,i,firstDone=0;
    struct heapStruct* myHeap;
    initHeap(myHeap);
    for(i=0;i<length;i++)
        insert(myHeap,numRooms);
    for(i=0;i<length;i++){
        firstDone = removeMin(myHeap);
        totalTime += insert(myHeap,firstDone) ;
    }
        freeHeap(myHeap);
        return totalTime;
}

int binSearch(int* times, int length, int maxTime){
    int low = 1, high = length , minRooms = 1;
    while(low <= high){
    minRooms = simulate (times,high,minRooms);
    int mid = (low+high+1)/2;
    if( mid > maxTime)
        low = mid+1;
    else  high = mid;
    }
}

//make a heap malloc space and set to null
struct heapStruct* initHeap() {
    struct heapStruct* h;
    h = (struct heapStruct*)(malloc(sizeof(struct heapStruct)));
    h->capacity = MAXHEAP;
    h->heaparray = (int*)malloc(sizeof(int)*(MAXHEAP+1));
    h->size = 0;
    return h;
}

void percolateDown(struct heapStruct *h, int index) {
    int min;
    // Only try to percolate down internal nodes.
    if ((2*index+1) <= h->size) {

        // Find the minimum value of the two children of this node.
        min = minimum(h->heaparray[2*index], 2*index, h->heaparray[2*index+1], 2*index+1);
        //if current bigger than min we need to bump down
        if (h->heaparray[index] > h->heaparray[min]) {
            swap(h, index, min);
            percolateDown(h, min);//recc down to bottom
        }
    }

    // Case where our current element has exactly one child, a left child.
    else if (h->size == 2*index) {

        // Here we only compare the current item to its only child.
        // Clearly, no recursive call is needed since the child of this node
        // is a leaf.
        if (h->heaparray[index] > h->heaparray[2*index])
            swap(h, index, 2*index);
    }
}

//runs perc up
void percolateUp(struct heapStruct *h, int index) {

    //root test
    if (index > 1) {
        if (h->heaparray[index/2] > h->heaparray[index]) { //test if bigger than parent
            //promote the node and check if need to do again
            swap(h, index, index/2);
            percolateUp(h, index/2);
        }
    }
}


int insert(struct heapStruct *h, int value) {
    int* temp;
    int* throwaway;
    int i;

    //make space if full
    if (h->size == h->capacity) {
        //make space for array
        h->heaparray = (int*)realloc(h->heaparray, sizeof(int)*(2*h->capacity+1));
        //mem check
        if (h->heaparray == NULL) return 0;
        //double the mem
        h->capacity *= 2;
    }
    //adjust values to keep track and then move inserted val
    h->size++;
    h->heaparray[h->size] = value;
    percolateUp(h, h->size);
}

int removeMin(struct heapStruct *h) {

    int retval;

    //check for populated list
    if (h->size > 0) {

        // This is where the minimum is stored.
        retval = h->heaparray[1];

        // Copy the last value into this top slot.
        h->heaparray[1] = h->heaparray[h->size];

        // Our heap will have one fewer items.
        h->size--;

        // Need to let this value move down to its rightful spot in the heap.
        percolateDown(h, 1);

        // Now we can return our value.
        return retval;
    }
    else
        return -1; //failure case
}

// Swaps the values stored in the heap pointed to by h in index1 and index2.
void swap(struct heapStruct *h, int index1, int index2) {
    int temp = h->heaparray[index1];
    h->heaparray[index1] = h->heaparray[index2];
    h->heaparray[index2] = temp;
}

// Returns indexa if a < b, and returns indexb otherwise.
int minimum(int a, int indexa, int b, int indexb) {
    if (a < b)
        return indexa; //if min is A
    else
        return indexb; //if min is B
}

//prints the heaps values
void printHeap(struct heapStruct *h) {
    int i;

    for (i=1; i<=h->size; i++)
        printf("%d ", h->heaparray[i]);
    printf("\n");
}

void heapify(struct heapStruct *h) {

    int i;

    // We form a heap by just running percolateDown on the first half of the
    // elements, in reverse order.
    for (i=h->size/2; i>0; i--)
        percolateDown(h, i);
}

struct heapStruct* initHeapfromArray(int* values, int length) {

    int i;
    struct heapStruct* h;

    h = (struct heapStruct*)(malloc(sizeof(struct heapStruct)));
    // We allocate one extra slot, since slot 0 stays unused.
    h->heaparray = (int*)malloc(sizeof(int)*(length+1));

    // Just copy the values into our array.
    for (i=1; i<=length; i++)
        h->heaparray[i] = values[i-1];

    // This is the number of values we copied.
    h->size = length;

    // This takes our random values and rearranges them into a heap.
    heapify(h);
    return h;
}

//heap sortin
void heapSort(int values[], int length) {

     struct heapStruct *h;
     int i;

     // Create a heap from the array of values.
     h =  initHeapfromArray(values, length);
     length = h->size;

     // Remove these values from the heap one by one and store them back in the
     // original array.
     for (i=0; i<length; i++) {
         values[i] = removeMin(h);
     }
}

//free my heap up
void freeHeap(struct heapStruct *h) {
     free(h->heaparray);
     free(h);
}
