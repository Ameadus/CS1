/*
Alexander Meade
COP3502 Closest Birthday Program
Due 2/26/17
*/

#include <stdio.h>
#include <stdlib.h>

#define MAXPEOPLE 1000
#define DAYSYEAR 365
#define MAXCHAR 30 //one more for longest name in case of null char

struct student//info needed for each student birthday
{
    int month; //
    int day;
    int dayNum;
    char firstN[MAXCHAR]; //all capital letters
    char lastN[MAXCHAR]; //all capital letters
};

char monthN[][MAXCHAR] = {"JANUARY","FEBUARY","MARCH","APRIL","MAY","JUNE","JULY","AUGUST","SEPTEMBER","OCTOBER","NOVEMBER","DECEMBER"};//months from input in all capital letters

int leapYear[]={0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};//sum of days in leap year before 366
int regYear[]={0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};//sum of days in a normal year before 365

//functions to take in data
int readInfo(struct student* theclass[], int studentNum);//read it in
void infoAmount(struct student* theclass[], int studentNum, int leapBabby);
int findMonth(char* monthIn); //find the month they born
void setdayN(struct student* theclass[],int length, int freq[]); //get the day

//quick sort related functions
void quickSortStudent(struct student* theclass[],int low, int high); //long way quick sort
void quickSort(struct student* theclass[],int length); //easier call of quick sort
int partition(struct student* theclass[], int low, int high); //partition for quick sort
void swap(struct student* theclass[], int i1, int i2); //index swaps
int studentcomp(struct student *first, struct student *second); //differentiate between students

//functions to find answers
void solve(struct student* theclass[],int length, int loc, int leapBabby); //actually find the people who have close bdays
int findStudent(struct student* theclass[], int length, char* firstName, char* lastName); //find student index spot

int main()
{
    int i, j, numClass,studentNum;
    scanf("%d", &numClass);
    for (i=1; i<= numClass; i++)
        {
        scanf("%d", &studentNum);
        struct student* theclass[MAXPEOPLE];
        int leap = readInfo(theclass, studentNum);//set to 1 if someone born feb 29th

        if (leap)
            setdayN(theclass, studentNum, leapYear);
        else
            setdayN(theclass, studentNum, regYear);


        quickSort(theclass, studentNum);
        printf("Class #%d:\n", i);
        infoAmount(theclass, studentNum, leap);
    }

 return 0;
}

int findMonth(char* monthIn)//read numeric values of month
{
    int i;
    for(i=1;i<13;i++)
        if(strcmp(monthIn,monthN[i])== 0)
            return i;
    return 12;//invalid set to end or december
}

void setdayN(struct student* theclass[],int length, int freq[])//day numbers of each student in the class for each month
{
    int i;
    for (i=0; i<length; i++)
        theclass[i]->dayNum = freq[theclass[i]->month] + theclass[i]->day;
}

void quickSortStudent(struct student* theclass[],int low, int high) //i chose quick sort for my sort
{
    if (low < high)
    {
        int mid = partition(theclass,low,high);
        quickSortStudent(theclass,low,mid-1);
        quickSortStudent(theclass,mid+1,high);
    }
}

int studentcomp(struct student* first, struct student* second)//compares numerical difference between 1st and 2nd student used to help find bday
{

    int diff = first->dayNum - second->dayNum; //the difference between the student's birthday

    if (diff)
        return diff;

    int lastnamecomp = strcmp(first->lastN, second->lastN);//compares last name for tie break

    if (lastnamecomp)//tie break
        return lastnamecomp;

    return strcmp(first->firstN, second->firstN);//they have to have diff first names so we good
}

int partition(struct student* theclass[], int low, int high)//paratition function for the quick sort edited to work with pointer to struct
{
    struct student* temp;
    int i, lowpos;

    if (low == high) return low;//

    // Pick a random partition element and swap it into index low.
    i = low + rand()%(high-low+1);
    temp = theclass[i];
    theclass[i] = theclass[low];
    theclass[low] = temp;

	lowpos = low; //stores index

	low++;//update index

	while (low <= high) //run until the low and high cross
    {
        //cycle thru the low side
        while (low <= high && studentcomp(theclass[low], theclass[lowpos]) <= 0 )
            low++;
        //cycle thru high side
		while (high >= low && studentcomp(theclass[high], theclass[lowpos]) > 0 )
            high--;

		//values on wrong side so swap
		if (low < high)
		   swap(theclass, low, high);
	}

	swap(theclass, lowpos, high);//swap partition to correct spot

	return high;//return high index
}

void swap(struct student* theclass[], int i1, int i2)//swaps the index value of items in the struct
{
     struct student* temp = theclass[i1];
     theclass[i1] = theclass[i2];
     theclass[i2] = temp;
}


int readInfo(struct student* theclass[], int studentNum)
{
    int i,leap=0;
    for(i=0;i<studentNum;i++)
    {

    theclass[i]= (struct student*)(malloc(sizeof(struct student)));//makes space for each student in class
    scanf("%s%s", theclass[i]->firstN, theclass[i]->lastN);//scan first and last name strings


    char temp[MAXCHAR];

    scanf("%s",temp);
    theclass[i]->month = findMonth(temp);

    int year; //need to read in day and year but we dont care for year just the day
    scanf("%d%d", &theclass[i]->day, &year); //read in day and year ignore year scan val

    if(theclass[i]->month==1 && theclass[i]->day==29)//if the kids born on febuaray 29th then set leap year to true
        leap=1;
    }
     return leap;
}

int findStudent(struct student* theclass[], int length, char* firstName, char* lastName)//returns position of student with name first last
{
    int i;
    for(i=0;i<length;i++)
        if (strcmp(theclass[i]->firstN, firstName) == 0 && strcmp(theclass[i]->lastN, lastName) == 0)
        return i;//return said student

}

void infoAmount(struct student* theclass[], int studentNum, int leapBabby)//leapBabby is to store if the kid is leap year bday or not
{
    int i, infoNum;

    scanf("%d", &infoNum);
    for(i=0;i<infoNum;i++)
    {
        char first[MAXCHAR];
        char last[MAXCHAR];
        scanf("%s%s", first, last);
        int loc = findStudent(theclass,studentNum,first,last); //store location of the found student
        solve(theclass,studentNum,loc,leapBabby);//solve it
    }


}

void solve(struct student* theclass[],int length, int loc, int leapBabby)//find the students
{
    int prev,next;
    if(loc == 0)//wrap to last name on list
    {
        next = theclass[loc+1]->dayNum - theclass[loc]->dayNum;
        prev = DAYSYEAR + leapBabby + theclass[loc]->dayNum - theclass[length-1]->dayNum;
    }

    else if(loc== length-1)//wrap to first name on list
            {
            next = DAYSYEAR + leapBabby + theclass[0]->dayNum - theclass[loc]->dayNum;
            prev = theclass[loc]->dayNum - theclass[loc-1]->dayNum;
            }
    else//standard case
    {
        next = theclass[loc+1]->dayNum - theclass[loc]->dayNum;
        prev = theclass[loc]->dayNum - theclass[loc-1]->dayNum;
    }

    if (DAYSYEAR+leapBabby - next < next)
        next = DAYSYEAR+leapBabby - next;
    if (DAYSYEAR+leapBabby - prev < prev)
        prev = DAYSYEAR+leapBabby - prev;


    int locNext = (loc+1)%length;
    int locPrev = (loc-1+loc)%length;


    if (next <= prev)
        printf("%s %s has the closest birthday to %s %s\n", theclass[locNext]->firstN,theclass[locNext]->lastN, theclass[loc]->firstN, theclass[loc]->lastN);
    else
        printf("%s %s has the closest birthday to %s %s\n", theclass[locPrev]->firstN,theclass[locPrev]->lastN, theclass[loc]->firstN, theclass[loc]->lastN);

}

void quickSort(struct student* theclass[],int length)//simplfy quick sort input
{
    quickSortStudent(theclass, 0, length-1);
}
