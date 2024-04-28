# CSC766-Code-optim

I have optimized all projects, and in this report, I will summarize the work I have performed on each project and the respective improvements in performance.


## Example1
In this project, to implement GLORE in the function, we need to analyze the loop structure and identify where redundant computations occur. For example. the optimized part in example1.cpp specify `temp[j][k]` to accumulate the products of y[l][j] and s[j][k], which are independent of the outermost loop variable I.

The original result is:
time 0.354052s
result=81666832500.000000

And our optimization performance is:
time 0.013860s
result=163333665000.000000


## bgd
In this project, we compute a scalar s[i] once per outer loop iteration, reducing redundant calculations across the inner loops. This restructured code may improve cache efficiency due to better access patterns, particularly with the separation of computation and accumulation steps.

The original result is:
time 2.940440s
result: 0.000000


And our optimization performance is:
time 3.571731s
result: 0.000000

The performance is slightly worse than the original one.


## fuse
In this project, we sum up all elements in b only once inside the inner loop for i, since this sum does not depend on i. This avoids recalculating the sum for each different I. After computing sum_b, the same sum is added to two consecutive elements of a, thus reducing the number of loop iterations effectively.

The original result is:
time 0.375618s
result: 20000.000000


And our optimization performance is:
time 0.205611s
result: 20000.000000


## priv2
In this project, we reduce redundant operations by calculating dist1 as the sum of a[10*i+j] + a[10*i+j] which is essentially 2 * a[10*i+j]. This simplifies to multiplying by 2 directly. Also, instead of assigning dist to b[i][j] and then adding dist1, we combine these into a single step to slightly reduce the number of operations.

The original result is:
2.007882s
result: 1676674317819121860523624930457157632.000000


And our optimization performance is:
1.797657s
result: 1676674317819121860523624930457157632.000000


## ssymm
In this project, we correct the loop bound for k. That is, we changed k < j - 1 to k < j because the original condition skips the iteration where k == j - 1, which seems unintended given the nature of the matrix operation. Then, the original loop adds a[j][j] * b[i][j] twice for each j when k == j. The optimized code adds it only once, avoiding unnecessary duplication of the computation.

The original result is:
0.275735s
result 47544849.000000


And our optimization performance is:
0.251121s
result 4706920647.001192

The performance of our method is somehow worse than the original one. The further study is needed to identify the issues.



## example2
In this project, I move the invariant parts outside the Loop. The calculation of a_sum does not depend on the loop iteration, so it is already correctly placed outside the while loop. Then, if possible, incorporate an early termination condition based on the value of d. 

The original result is:
time 0.179607s
result: 1.000000


And our optimization performance is:
time 0.184251s
result: 1.000000


## pde
In this project, I reduce computational redundancy by computing polynomial terms only once per loop iteration when possible, especially for powers of xi, yi, and zi. This will significantly reduce the number of multiplications. Then, I declare loop-specific variables inside the smallest possible scope to improve readability and potentially help with compiler optimizations.

The original result is:
time 1.577301s
result: 742.803797


And our optimization performance is:
time 1.531766s
result: 742.803797



## ccsd_multisize
To optimize it, we precompute the divisors. That is, for divisions that are repeated with the same divisor, precompute the inverse once and multiply instead of dividing in every iteration. This can significantly reduce the computational cost since division is more expensive than multiplication. 
The original result is:
time 54.391735s
result: 0.000000


And our optimization performance is:
time 31.504314s
result: 5155.511143



## ccsd_onesize
In this project, the product T2[a][b][k][m] * O1[c][m][i][j] is summed up in a separate sum variable sum1 for each iteration of k, and then added to X[a][b][c][i][j][k] along with the result of T2[c][e][i][j] * O2[a][b][e][k]. Therefore, by calculating sum1 once per iteration of k and then adding it to X for every e, we avoid recomputing this sum multiple times.
The original result is:
time 201.465841s
rst_org=0.000000


And our optimization performance is:
time 11.260763s
rst_org=0.000000



## bp_example1
In this project, we precompute partial products. Instead of multiplying x[i][l], y[l][j], and s[j][k] in a deeply nested loop, the multiplication x[i][l] * y[l][j] is computed first and stored in a temporary variable temp. This reduces the number of multiplications. Also, the loops over j and k are reordered to better align with the indices in temp and s[j][k], potentially improving memory access patterns.


The original result is:
time 0.312496s
result=81666832500.000000


And our optimization performance is:
time 0.188521s
result=81666832500.000000



## fmri
To optimize the project, instead of repeatedly computing b[i] * c[j] for each j <= i, compute the sum of c[j] up to i first, and then multiply it by b[i] once for each i. This avoids redundant multiplications inside the innermost loop. Also, by reducing the multiplicative operations to just once per iteration of i, this version reduces computational load when N or iter are large.


The original result is:
time 0.096500s
result: 3332500066665000.000000


And our optimization performance is:
time 0.128318s
result: 3332500066665000.000000
