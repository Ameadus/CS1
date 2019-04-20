/*
Alexander Meade
Recitation program Vacation COP3502
Due 2/22/17
*/

#include <stdio.h>
#include <math.h>
#define MAXRIDE 10

    struct plain//locations on a cartisian plain
    {
    double x;
    double y;
    };

double distanceEq(const struct plain* p1, const struct plain* p2);
double findDistance(int perm[], struct plain ridePosition[] ,int n);
double bestPath(int perm[],int used[],int k, int n, struct plain ridePosition[], int block[][MAXRIDE]);

int main(void)
{
    int nParks,loop;
    scanf("%d", &nParks);
        for(loop=0;loop<nParks;loop++)
        {
            int nRides, nBlockpath,i,j;
            scanf("%d%d", &nRides, &nBlockpath);
            struct plain ridePosition[MAXRIDE];
            for(i=0;i<nRides;i++)//read in the location of the ride
                scanf("%lf %lf", &ridePosition[i].x, &ridePosition[i].y);

            //store blocked paths in a matrix blocked[i][j]
            int blocked[MAXRIDE][MAXRIDE];
            for(i=0;i<MAXRIDE;i++)
                for(j=0;j<MAXRIDE;j++)
                    blocked[i][j]=0; //if this equals 1 the path is considered blocked

            for(i=0;i<nBlockpath;i++)
            {
                int p1,p2;
                scanf("%d%d", &p1, &p2);
                blocked[p1-1][p2-1]= 1; //this path is blocked
                blocked[p2-1][p1-1]= 1; //this makes sure blocked from all direction
            }

            int perm[MAXRIDE], used[MAXRIDE];

            for(i=0;i<nRides;i++)
                used[i]=0; //no rides have been visited yet
            double answer = bestPath(perm, used, 0, nRides, ridePosition, blocked); //find the best path for the given park

            printf("Vacation #%d+1:\n", loop);
                if(answer < 4999999 ) //if less than the arbitraily big number then we are good
                    printf("Jimmy can finish all of the rides in %.3lf seconds.\n",answer+nRides*120); //take the path add the number of rides then multiply for time on each ride to get total time
                else
                    printf("Jimmy should plan this vacation on a different day.\n"); // if bigger than big number then jimmy wont be able to do this
        }

}

double bestPath(int perm[],int used[],int k, int n, struct plain ridePosition[], int block[][MAXRIDE])//function to permutate between all the rides and find shortest distance
{
    if(k == n)
        return findDistance(perm, ridePosition, n);
    else
    {
        int i;
        double best = 5000000;//make a number bigger than all the possible perms
        for(i=0;i<n;i++)
        {
        if (!used[i]) //look for valid paths
            {
        if (k == 0 || block[perm[k-1]][i] == 0)
                {
            perm[k] = i;
            used[i] = 1;
            double path = bestPath(perm, used, k+1, n, ridePosition, block);//recursively look thru all the paths to find the best one
                if(path < best)//if we find a shorter path make it the best one
                    best = path;
                    used[i] = 0;
                }
            }
        }
    return best;
    }
}

double distanceEq(const struct plain* p1, const struct plain* p2)//function to find distance between two points
{
    return sqrt( pow(p1->x - p2->x,2) + pow(p1->y - p2->y,2));
}

double findDistance(int perm[], struct plain ridePosition[] ,int n)//function to find distance to rides
{
    int i;
    struct plain begin; //make the location start at 0,0
    begin.x= 0; // x zero
    begin.y= 0; // y zero
    double distance = distanceEq(&begin,&ridePosition[perm[0]]);// find the distance between the beginning and first ride

    for(i=0;i<n;i++) // loop thru all the rides
         distance = distance + distanceEq(&ridePosition[perm[i-1]],&ridePosition[perm[i]]);
        return distance; // get final distance

}
