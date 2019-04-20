/*
    This code belongs to Alexander Meade COP3502
    This is the word search program #1
 DONT FORGET FREES()
 Program outline:
 store dictionary text in a malloc array
 scan and create grids
 scan and create size of grids then enter strings of letters using malloc
 compare the dictionary array and grid array using binary search and traverse function
 make sure that program doesnt go out of array bounds

 print all results found

*/


#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 256
#define LOWER 4
#define UPPER 19


const int SIZE = 8; // 8 directions
//Up Left Right Down. Up-left, Up-Right,Down-Left,Down-Right
const int DX[] = {-1,0,0,1,-1,-1,1,1};
const int DY[] = {0,-1,1,0,-1,1,-1,1};

char** readList(char fileName[]); //opens and stores dictionary to array this works
int wordSearch(char** wordList, char* Word); //searches for matches problem here?
int onGrid( int x, int y); //keep movement on grid this works
char* getString(char** grid, int row1, int lines1, int movement, int size); //moves to find word on grid maybe here?

int row,lines,wordsCount; //globals



int main () //issue must be in the main
{
    char** masterList = readList("dictionary.txt");
    int gridNum,loop;

    //printf("How many grids would you like?\n");
    scanf("%d", &gridNum);

 for(loop=1; loop <= gridNum ; loop++)
 {
     int i,j,k,length;

    printf("Words Found Grid #%d:\n",loop);
    //printf("How big would you like the grid?\n");
    scanf("%d%d", &row, &lines);

    char** grid = malloc(sizeof(char*)*row );

        for(i=0; i<row; i++)
    {
        grid[i] = malloc(sizeof(char)*(lines+1));
        scanf("%s", grid[i]); // scans the user input strings
    }
            for (i=0; i<row; i++)
            {
                for (j=0; j<lines; j++)
                {
                    for (k=0; k<SIZE; k++) //use to move around array by element like in egn3211
                        for (length=LOWER; length<=UPPER; length++) //words from size 4 to 19
                    {
                            char* result = getString(grid,i,j,k,length);

                            if (result != NULL)
                            {
                                if(wordSearch(masterList,result))
                                    printf("%s\n", result);
                                free(result);
                            }
                    }
                }
            }

free(grid[i]);
free(grid);
free(masterList[i]);
free(masterList);





 }


    }




char** readList(char fileName[]) //reads the dictionary file
{

  FILE* wordBank = fopen(fileName, "r");   // open dictionary for read

    int i;

    fscanf(wordBank, "%d", &wordsCount); // see how many words in dictionary text


    char** bigList = malloc( sizeof(char*)*wordsCount);

    for (i = 0; i < wordsCount; i++){

      // Allocate the maximum amount of space for this individual word.
      bigList[i] = malloc(sizeof(char)*(MAXLEN+1));
      //puts everything in bigList to be compared
      fscanf(wordBank, "%s", bigList[i]);
    }

    fclose(wordBank);    // close dictionary
    return bigList;
  }


int wordSearch(char** wordList, char* Word) //string search binary function I feel the issue is here
 {

    int bot = 0, top = wordsCount-1;

    while (bot <= top)
        {

        int mid = (bot+top)/2;
        int match = strcmp(Word,wordList[mid]);

            if (match == 0)

                return 1;

            else if (match < 0)

                    top = mid-1;

            else

            bot = mid+1;

        }
 }

int onGrid( int x, int y)
{

 //checks to make sure movement wont go off grid in x or y direction
    return x >= 0 && x < row && y >= 0 && y < lines;

}

char* getString(char** grid, int row1, int lines1, int movement, int size)
{
  int i;
  if(!onGrid(row1 + (size-1)*DX[movement], lines1 + (size-1)*DY[movement]))
    return NULL;

    char* result = malloc(sizeof(char)*(size+1));

    for(i=0; i<size; i++)
        result[i] = grid[row1+i*DX[movement]][lines1+i*DY[movement]];
        result[size] = '\0';
        return result;
}
