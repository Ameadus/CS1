/*
Alexander Meade COP3502C conference.c
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ROOMNAME 19

typedef struct lecStruct{
	int roomNum;
	long time;
	long startTime;
	char name[ROOMNAME +1];
	int used;
} lecStruct;

typedef struct lecHeap{
	char  name[ROOMNAME +1];
	long time;
}lecHeap;

//init functs
lecStruct* createRoom(lecStruct* roomArr,int numRooms, int numLec);
lecHeap* readlecHeap(lecHeap* list, int numLecs);

//room functiosn
lecStruct* giveRoom(lecStruct* room, lecHeap lec, long totalTime, long *time, int filled); //give a lec a room
void printList(lecStruct* lecArr, int meetings); //print the values in arr of lecs

void swapRooms(lecStruct firstRoom, lecStruct secondRoom); //swap rooms
lecStruct* popRoom(lecStruct* room, lecStruct* list, long totalTime, long *time, int *index, int filled);//free a room
lecStruct *changeNodeTimes(lecStruct *room, int time, int filled);//helps change times for pop
lecStruct* clearRooms(lecStruct* room, int numLec, int filled);//empty the halls

//funts relating to heap balance
lecStruct* balanceRooms(lecStruct *room, int filled, int numLec); //balance the heap struct
lecStruct* balanceRoomsR(lecStruct* room, int filled, int numLec); //help balance heap
lecStruct* balanceRoomsA(lecStruct* room, int filled, int numLec); //help balance heap2

//merge sort functs (went unused)
void mergeSort(lecStruct* arr[],int length);
void mergeSortRec(lecStruct* arr[],int low, int high);
void merge(lecStruct* arr[], int start1, int start2, int end2);

int main(void){

	long totalTime, tempTime;
	int numMeetings, tempNumMeetings,totalRooms=0,i,j,place=0,listPlace=0,isCorrect=0;
	lecHeap *list;
	lecStruct *rooms = NULL, *lecArr = NULL;

    //scans and space allocation
	scanf("%d %li", &numMeetings, &totalTime);
	tempTime = totalTime;
	rooms = malloc(numMeetings*sizeof(lecStruct));
	tempNumMeetings = numMeetings;
    lecArr = malloc(numMeetings*sizeof(lecStruct));
	list = (lecHeap*)readlecHeap(list, numMeetings);
	printf("time: %li\n", tempTime);

	for(i = 0; i< numMeetings; i++){
    rooms[i].time = -1;
    rooms[i].startTime = -1;
    }

	int isFound = 0;
	int indexList = 0;
	int roomsFilled = 0;

	rooms = createRoom(rooms, totalRooms, numMeetings);
	totalRooms++;
    printf("created room %d\n",totalRooms);

	//create rooms to fill
	while(tempTime < 0 || tempNumMeetings != 0){
		if(tempTime < 0){
			rooms = createRoom(rooms, totalRooms, numMeetings);
			tempNumMeetings = numMeetings;
			tempTime = totalTime;
			listPlace=0;
			indexList = 0;
			roomsFilled=0;
			totalRooms++;
            printf("created room %d\n",totalRooms);
		}

        //while rooms not all full and theres still meetings give work to solving room number
		while(roomsFilled != totalRooms && tempNumMeetings != 0){
			rooms = giveRoom(rooms, list[listPlace], totalTime, &tempTime, totalRooms);
			tempNumMeetings--;
			roomsFilled++;
			listPlace++;
			isFound =0;
			rooms =	balanceRooms(rooms, roomsFilled/2, numMeetings);
		}

		lecArr = popRoom(rooms, lecArr, totalTime,&tempTime, &indexList, roomsFilled);
	 	rooms =	balanceRooms(rooms, roomsFilled/2, numMeetings);
		roomsFilled--;

		//when a room finishes pop it off
		while(rooms[0].time == 0){
            lecArr = popRoom(rooms, lecArr, totalTime,&tempTime, &indexList, roomsFilled);
	 	    rooms =	balanceRooms(rooms, roomsFilled/2, numMeetings);
		    roomsFilled--;
        }

		printf("outtime: %li\n", tempTime);

        if(tempTime < 0){
            rooms = clearRooms(rooms, numMeetings, roomsFilled);

        }
	}

    //until all rooms are empty
    while(roomsFilled >0)
    {
    	rooms =	balanceRoomsA(rooms, roomsFilled/2, numMeetings);
    	 lecArr = popRoom(rooms, lecArr, totalTime,&tempTime, &indexList, roomsFilled);
	 	 //   rooms =	balanceRoomsA(rooms, roomsFilled/2, numMeetings);
		    roomsFilled--;
    }
    //give the answers
	printf( " \n %d \n", totalRooms);
	printList(lecArr, numMeetings);

}




//this mallocs and scans in values to a lecHeap
lecHeap* readlecHeap(lecHeap* list, int numLecs){
	int i;
	list = malloc(numLecs * sizeof(lecHeap));

	for(i=0;i<numLecs;i++)
		scanf("%s %ld", list[i].name, &list[i].time);
	return list;
}

//this function creates a room
lecStruct* createRoom(lecStruct* roomArr,int numRooms, int numLec){

	roomArr[numRooms].used = 1;
	roomArr[numRooms].time = -1;
	roomArr[numRooms].roomNum = numRooms;
    numRooms++;
	return roomArr;

}

//this fuction will give a conference a room to perform a lecture
lecStruct* giveRoom(lecStruct* room, lecHeap lec, long totalTime, long *time, int filled)
{
    int i;
    for(i=0;i<filled;i++)
    {
	    if(room[i].used && room[i].time == -1)
	    {
		    strcpy(room[i].name, lec.name);

	    	room[i].time = lec.time;
	    	room[i].startTime = totalTime - *time;

	    	break;
	    }
    }
	return room;
}

//pop off first room to open
lecStruct* popRoom(lecStruct* room, lecStruct* list, long totalTime, long* time, int* index, int filled){

	*time -= room[0].time;
	room = changeNodeTimes(room, room->time, filled);
	strcpy(list[*index].name, room[0].name);
	list[*index].time = room[0].startTime;
	list[*index].roomNum = room[0].roomNum;
	*index+=1;

	room[0].time = -1;
	room[0].startTime = -1;
    strcpy(room[0].name, "");

	return list;
}

//change the time for the room
lecStruct* changeNodeTimes(lecStruct* room, int time, int filled){
	int i;
	for(i=0;i<filled;i++){
		if(room[i].used && room[2*i+1].used && room[2*i+1].time != -1)
		{
			room[2*i+1].time -= time;
		}

	    if(room[i].used && room[2*i+2].used && room[2*i+2].time != -1)
		{
			room[2*i+2].time -= time;
		}
	}

	return room;
}

//rebalance the heap function
lecStruct* balanceRooms(lecStruct* room, int filled, int numLec){
	int i;
	lecStruct temp;

	for(i=filled;i>=0;i--)
	{
        if(2*i+1 > numLec || 2*i+2> numLec || i > numLec)
            continue;
		if(room[i].time > room[2*i+1].time && room[2*i+1].time > 0 )
		{

            temp = room[i];
			room[i]= room[2*i+1];
			room[2*i+1] = temp;

		}
		if(room[i].time > room[2*i+2].time && room[2*i+2].time > 0)
		{

            temp = room[i];
			room[i]= room[2*i+2];
			room[2*i+2] = temp;
        }
	}

	return room;
}

//helper function for balanceRooms
lecStruct* balanceRoomsR(lecStruct* room, int filled, int numLec){
	int i;
	lecStruct temp;

	for(i=filled;i>=0;i--)
	{
        if(2*i+1 > numLec ||2*i+2> numLec || i > numLec)
            continue;
		if(room[i].roomNum > room[2*i+1].roomNum && room[2*i+1].used && room[i].used )
		{

            temp = room[i];
			room[i]= room[2*i+1];
			room[2*i+1] = temp;

		}
		if(room[i].roomNum > room[2*i+2].roomNum && room[i].used && room[2*i+2].used)
		{

            temp = room[i];
			room[i]= room[2*i+2];
			room[2*i+2] = temp;
        }
	}

	return room;
}


//another helper function for balanceRooms
lecStruct* balanceRoomsA(lecStruct* room, int filled, int numLec){
	int i;
	lecStruct temp;

	for(i=filled;i>=0;i--)
	{
        if(2*i+1 > numLec ||2*i+2> numLec || i > numLec)
            continue;
		if(room[i].time < 0&& room[2*i+1].time >= 0)
		{

            temp = room[i];
			room[i]= room[2*i+1];
			room[2*i+1] = temp;

		}
	 if(room[i].time < 0 && room[2*i+2].time >= 0 )
		{

            temp = room[i];
			room[i]= room[2*i+2];
			room[2*i+2] = temp;
        }
	}

	return room;
}

//swaps two rooms values
void swapRooms(lecStruct firstRoom, lecStruct secondRoom){
	lecStruct temp;

	strcpy(temp.name,firstRoom.name);
	temp.startTime = firstRoom.startTime;
	temp.time = firstRoom.time;
	temp.roomNum = firstRoom.roomNum;

	strcpy(firstRoom.name,secondRoom.name);
	firstRoom.startTime = secondRoom.startTime;
	firstRoom.time = secondRoom.time;
	firstRoom.roomNum = secondRoom.roomNum;


	strcpy(secondRoom.name,temp.name);
	secondRoom.startTime = temp.startTime;
	secondRoom.time = temp.time;
	secondRoom.roomNum = temp.roomNum;
}

//prints values of lectures
void printList(lecStruct* lecArr, int meetings){
	int i;

	for(i=0;i<meetings;i++)
		printf("%s %d %li\n", lecArr[i].name, lecArr[i].roomNum, lecArr[i].time);
}

//empty a room out ot be reused
lecStruct* clearRooms(lecStruct* room, int numLec, int filled){
    int i;

    for(i=0;i<numLec;i++)
    {
            room[i].time = -1;
            room[i].startTime = -1;
            strcpy(room[i].name, "");
    }
    room = balanceRoomsR(room, filled/2, numLec);
    return room;
}



// Merges arr[start1-start2-1] and arr[start2-end2] into a single sorted array[start1-end2]
void merge(lecStruct* arr[], int start1, int start2, int end2) {
    struct lecStruct** temp = malloc((end2-start1+1)*sizeof(struct lecStruct*));

    // i is left list j right list k is the merge
    int i = start1, j = start2, k;

    //fill merge
    for (k=0; k<=end2-start1; k++) {
        //list 1
        if (i == start2) {
            temp[k] = arr[j];
            j++;
        }
        //list 2
        else if (j == end2+1) {
            temp[k] = arr[i];
            i++;
        }
        //compare to see if i first
        else if (arr[i]->startTime - arr[j]->startTime < 0) {
            temp[k] = arr[i];
            i++;
        }
        //else j goes first
        else {
            temp[k] = arr[j];
            j++;
        }
    }
    //put the pointers back to original arr
    for (k=0; k<=end2-start1; k++)
        arr[k+start1] = temp[k];
}


//wrapper funct
void mergeSort(lecStruct* arr[],int length) {
    mergeSortRec(arr, 0, length-1);
}

//merge sort of vals in the arr between low and high inclusive
void mergeSortRec(lecStruct* arr[],int low, int high) {

    //base case check only sort for >1
    if (low < high) {
        int mid = (low+high)/2;
        mergeSortRec(arr,low,mid);
        mergeSortRec(arr,mid+1,high);
        merge(arr, low, mid+1, high);
    }
}
