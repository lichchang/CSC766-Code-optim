#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <sys/time.h>

#define O 10
#define V 10


double X[V][V][V][O][O][O];
double T2[V][V][O][O];
double O1[V][O][O][O];
double O2[V][V][V][O];

#define TIME
#ifdef TIME
#define IF_TIME(foo) foo;
#else
#define IF_TIME(foo)
#endif

void init_array()
{
    int a, b, c, e, i, j, k, m;

    // Precompute inverses for frequently used divisors
    double invO[O];
    for(i = 0; i < O; i++) invO[i] = 1.0 / (i + 1);

    for(a = 0; a < V; a++)
        for(b = 0; b < V; b++)
            for(c = 0; c < V; c++)
                for(i = 0; i < O; i++)
                    for(j = 0; j < O; j++)
                        for(k = 0; k < O; k++)
                            X[a][b][c][i][j][k] = a * b * c * invO[i] * invO[j] * invO[k];

    for(a = 0; a < V; a++)
        for(b = 0; b < V; b++)
            for(k = 0; k < O; k++)
                for(m = 0; m < O; m++)
                    T2[a][b][k][m] = a * b * invO[m] * invO[k];

    for(c = 0; c < V; c++)
        for(m = 0; m < O; m++)
            for(i = 0; i < O; i++)
                for(j = 0; j < O; j++)
                    O1[c][m][i][j] = c * m * invO[i] * invO[j];

    for(a = 0; a < V; a++)
        for(b = 0; b < V; b++)
            for(e = 0; e < V; e++)
                for(k = 0; k < O; k++)
                    O2[a][b][e][k] = a * b * invO[e] * invO[k];
}



void print_array()
{
  int a, b, c, i, j, k;
  for(a=0;a<V;a++)
    for(b=0;b<V;b++)
      for(c=0;c<V;c++)
        for(i=0;i<O;i++)
          for(j=0;j<O;j++)
            for(k=0;k<O;k++)
              fprintf(stderr, "%lf ", X[a][b][c][i][j][k]);
}

double rtclock()
{
  struct timezone Tzp;
  struct timeval Tp;
  int stat;
  stat = gettimeofday (&Tp, &Tzp);
  if (stat != 0) printf("Error return from gettimeofday: %d",stat);
  return(Tp.tv_sec + Tp.tv_usec*1.0e-6);
}
double t_start, t_end;

int main()
{
  double s;

  init_array();



//original code 
  IF_TIME(t_start = rtclock());

#pragma scop
int a, b, c, e, i, j, k;
for (a = 0; a < V; a++)
  for (b = 0; b < V; b++)
    for (c = 0; c < V; c++)
      for (i = 0; i < O; i++)
        for (j = 0; j < O; j++)
          for (k = 0; k < O; k++) {
            for (e = 0; e < V; e++) {
              double temp = T2[c][e][i][j] * O2[a][b][e][k];
              for (int m = 0; m < 1000; m++) // This inner loop might be redundant
                X[a][b][c][i][j][k] += temp;
            }
          }
 
#pragma endscop
  
  IF_TIME(t_end = rtclock());
  IF_TIME(printf("time %0.6lfs\n", t_end - t_start));

  double rst_org = X[V-1][V-1][V-1][O-1][O-1][O-1];
    
#ifdef TEST
  print_array();
#endif
  printf("result: %f\n", rst_org);
  return 0;
}
