/*
 * =====================================================================================
 *
 *       Filename:  example4.cpp
 *
 *    Description:  the fourth examples from Xipeng Shen
 *
 *        Version:  1.0
 *        Created:  04/21/2015 09:46:10 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yufei Ding, yding8@ncsu.edu
 *   Organization:  North Carolina State University
 *
 * =====================================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

typedef long LARGE_INTEGER;

#define xn 200
#define yn 200
#define zn 200
double u[6][xn][yn][zn];
double d[6][xn];
double c[6][14];
#define TIME

#ifdef TIME
#define IF_TIME(foo) foo;
#else
#define IF_TIME(foo)
#endif

//array initialization
void init_array()
{
  for(int k = 0; k < zn; k++){
    for(int j = 0; j < yn; j++){
      for(int i = 0; i < xn; i++){
        for(int m = 0; m < 6; m++){
          u[m][i][j][k] = i + j + k + m;
        }
      }
    }
  }


  for(int m = 0; m < 6; m++){
    for(int h = 0; h < 14; h++){
       c[m][h] = m + h;
    }
  }
}


//define timer for performance measurement
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


//example4
double example4() {
    for (int k = 2; k < zn-1; k++) {
        double zi = ((double)(k-1))/zn;
        double zi2 = zi * zi;
        double zi3 = zi2 * zi;
        for (int j = 2; j < yn-1; j++) {
            double yi = ((double)(j-1))/yn;
            double yi2 = yi * yi;
            double yi3 = yi2 * yi;
            for (int i = 2; i < xn-1; i++) {
                double xi = ((double)(i-1))/xn;
                double xi2 = xi * xi;
                double xi3 = xi2 * xi;
                for (int m = 0; m < 6; m++) {
                    d[m][i] =   c[m][2] * xi
                                + c[m][3] * yi
                                + c[m][4] * zi
                                + c[m][5] * xi2
                                + c[m][6] * yi2
                                + c[m][7] * zi2
                                + c[m][8] * xi3
                                + c[m][9] * yi3
                                + c[m][10] * zi3
                                + c[m][11] * xi2 * xi2
                                + c[m][12] * yi2 * yi2
                                + c[m][13] * xi * yi * zi2;
                }
                for (int m = 0; m < 6; m++) {
                    u[m][i][j][k] += d[m][i];
                }
            }
        }
    }
    return u[5][xn-2][yn-2][zn-2];
}

int main()
{
  init_array();

  IF_TIME(t_start = rtclock());
  double rst_org = example4();
  IF_TIME(t_end = rtclock());
  IF_TIME(printf("time %0.6lfs\n", t_end - t_start));

  printf("result: %f\n", rst_org);
  return 0;
}

