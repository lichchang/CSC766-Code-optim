# CSC766-Code-optim

I have optimized five projects: example1, bgd, fuse, priv2, and ssymm. In addition to these, other projects have encountered unexpected errors that prevent even the original versions from executing. In this report, I will summarize the work I have performed on each project and the respective improvements in performance.


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
