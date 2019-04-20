/*
Alexander Meade COP3502C Due 2/12/17 Mastermind Program
*/

#include <stdio.h>
#include <stdlib.h>

int nSlot,nColour,nMove; // globals for the values that will be split between functions
int* correct; //points to amount correct guess
int* incorrect; // points to amount of incorrect guesses
int** guessNum; // points to the guess array

int evaluate(int* combo);//checks perfect nonperfect
int answer(int* combo, int k);//number of combos permuted
int goodMatch(int* board, int* correct); // number of correct matches
int badMatch(int* board, int* correct); // number of total minus correct
int* colourFreq(int* board);// returns frequency of colours on the board
int totalMatch(int* board, int* total); // number of total matches
int minNum(int a, int b); //returns smaller value
int sumMatch(int* colour1, int* colour2);//adds two colours


int main()
{
    int loop, nCases;
    scanf("%d", &nCases);

    for(loop=0;loop<nCases;loop++)
    {
        int i,j;
        scanf("%d%d%d", &nSlot, &nColour, &nMove);
        //allocate space for all we need use calloc to remove any junk variables
        guessNum= calloc(nMove,sizeof(int*));
        correct= calloc(nMove,sizeof(int));
        incorrect= calloc(nMove,sizeof(int));

        for(i=0;i<nMove;i++)
            guessNum[i]= calloc(nSlot,sizeof(int));

        for(i=0;i<nMove;i++) // take in amount of guesses with right and wrong guess
        {
            for(j=0;j<nSlot;j++)
            scanf("%d", &guessNum[i][j]);
            scanf("%d%d", &correct[i], &incorrect[i]);
        }
int* combo = calloc(nSlot,sizeof(int));//combo to fill

printf("%d\n", answer(combo,0)); //recusion to print answer i feel this function doesnt work right

        for(i=0;i<nMove;i++)//free all da callocs
        {
            free(guessNum[i]);
            free(guessNum);
            free(correct);
            free(incorrect);
            free(combo);
        }

    }
}

int answer(int* comboC, int k)//modified permutation function
{
    int i;
    int sum=0;

    if (k == nSlot)
    {
        return evaluate(comboC);
    }


    else
        for(i=0;i<nColour;i++)
        {
            comboC[k]=i;
            sum += answer(comboC, k+1); //recursion pro
        }
    return sum;
}


int evaluate(int* combo)//checks to see perfect and nonperfect mathces
{
    int i;
    for(i=0;i<nMove;i++)
    {
    int perfectMatch = goodMatch(guessNum[i],combo);
    int wrongMatch = badMatch(guessNum[i],combo);

    if((perfectMatch != correct[i]) || (wrongMatch != incorrect[i])) //check if results are the same else its not right
        return 0;
    }

    return 1;
}



int goodMatch(int* board, int* rightColour)// this gives amount of black pegs which are correct colour and spot
{
    int i, nMatch=0;
    for(i=0; i<nSlot;i++)
    {
        if(board[i] == rightColour[i])
            nMatch++;
    }

    return nMatch;

}

int badMatch(int* board, int* rightColour)//total minus good match
{
    return totalMatch(board, rightColour) - goodMatch(board, rightColour);
}

int totalMatch(int* board, int* rightColour) // finds the total number of colours avilable on board and adds them with total correct to return a value
{
    int* boardColours = colourFreq(board);
    int* rightColours = colourFreq(rightColour);

    int result = sumMatch(boardColours,rightColours);

    free(boardColours);
    free(rightColours);

    return result;
}


int* colourFreq(int* board)//frequency colours happen on board
{
 int i;
 int* colours = calloc(nColour,sizeof(int));
 for(i=0;i<nSlot;i++)
        colours[board[i]]++;
    return colours;

}


int sumMatch(int* colour1, int* colour2)  //adds two amounts of colours together
{
    int i, sum = 0;
    for(i=0; i<nColour;i++)
    {
        sum += minNum(colour1[i],colour2[i]);
    }

    return sum;
}

int minNum(int a, int b) //returns minimum number
{
    return (a < b ? a : b);
}

