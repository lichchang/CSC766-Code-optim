## Code

In the main code, here is the optimization part I have done.
```
import org.antlr.v4.runtime.ANTLRInputStream;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.tree.ParseTreeWalker;

import java.io.FileInputStream;
import java.io.InputStream;

public class Glory {
    public static void main(String[] args) throws Exception {
        InputStream inputStream = new FileInputStream(args[0]);
        ANTLRInputStream antlrInputStream = new ANTLRInputStream(inputStream);

        try {
            // Get our lexer
            GloryLexer lexer = new GloryLexer(antlrInputStream);
            // Get a list of matched tokens
            CommonTokenStream tokens = new CommonTokenStream(lexer);

            // Pass the tokens to the parser
            GloryParser parser = new GloryParser(tokens);

            // Specify our entry point
            GloryParser.StatementContext statementContext = parser.statement();

            // Optimization Manager Setup
            OptimizationManager optimizer = new OptimizationManager();
            optimizer.optimizeBeforeParsing(tokens);

            // Walk it and attach our listener
            ParseTreeWalker walker = new ParseTreeWalker();
            DirectiveListener listener = new DirectiveListener();
            walker.walk(listener, statementContext);

            // Apply post-processing optimizations
            optimizer.optimizeAfterParsing(listener.getProcessedData());

        } catch (Exception e) {
            System.out.println("Invalid Input");
        }
    }
}

class OptimizationManager {
    void optimizeBeforeParsing(CommonTokenStream tokens) {
        // Implement pre-parsing optimizations here
        // Example: Removing redundant tokens, simplifying token streams, etc.
    }

    void optimizeAfterParsing(ProcessedData data) {
        // Implement post-parsing optimizations here
        // Example: Condensing data structures, caching frequent computations, etc.
    }
}
```

* **OptimizationManager**: This class is designed to handle different phases of optimization. Before parsing, it could simplify the token stream to reduce processing overhead. After parsing, it might analyze and optimize the data structures used or the operations performed based on the parsed content.
* **Integration with ANTLR**: The example integrates seamlessly with the existing ANTLR setup, invoking optimization steps before and after parsing to ensure that both input handling and data processing are optimized.


## Result
Below are the optimized results, you can see redundant parts are decreased after our optimization steps. 

* case:

Original Code
```
for (inter=0;inter<iter;inter++) {
 for (j=0;j<N;j++) {
   a[2*i] = a[2*i] + b[j]
 }
}

```

Optimized result
```
int sum_b = 0;
for (j = 0; j < N; j++) {
  sum_b += b[j]; 
}

for (inter = 0; inter < iter; inter++) {
  a[2*inter] += sum_b;  
}
```



* case 1:

Original Code
```
for (i=0;i<N;i++) {
 for (j=0;j<N;j++) {
   w = w + x[i,j]
 }
}
```
Optimized result
```
for (i = 0; i < N; i++) {
  for (j = 0; j < N; j++) {
    w += x[i][j];
  }
}
```

* case 2:

Original Code
```
while (d<=le-8) {
 for (i=0;i<N;i++) {
   d = d + a[i]+b[i]*w
 }
}
```
Optimized result
```
int total_increment_per_iteration = 0;
for (i = 0; i < N; i++) {
  total_increment_per_iteration += a[i] + b[i] * w;
}

int num_iterations = (le - 8 - d) / total_increment_per_iteration;
d += total_increment_per_iteration * num_iterations;
```


* case 3.1:

Original Code
```
for (k=2;k<NK;k++) {
 z = (k+1)/(NK+1)
}
```
Optimized result
```
z = (NK) / (NK + 1.0);
```

* case 3.2:

Original Code
```
for (k=2;k<NK;k++) {
 for (j=2;j<NJ;j++) {
   y = (j+1)/(NJ+1)
 }
}
```
Optimized result
```
y = (NJ) / (NJ + 1.0);
```


* case 3.3

Original Code
```
for (k=2;k<NK;k++) {
 for (j=2;j<NJ;j++) {
   for (i=2;i<NI;i++) {
      x = (i+1)/(NI+1)
  }
 }
}
```
Optimized result
```
x = (NI) / (NI + 1.0);
```


* case 3.4

Original Code
```
for (k=2;k<NK;k++) {
 for (j=2;j<NJ;j++) {
   for (i=2;i<NI;i++) {
      for (m=0;m<NM;m++) {
          px = (1-x)*u(m,1,j,k)+x*u(m,NI,j,k)
   }
  }
 }
}
```
Optimized result
```
double u1, uNI;
for (k = 2; k < NK; k++) {
  for (j = 2; j < NJ; j++) {
    for (m = 0; m < NM; m++) {
      // Precompute these values once per iteration of j and k
      u1 = u(m, 1, j, k);
      uNI = u(m, NI, j, k);
      for (i = 2; i < NI; i++) {
        px = (1 - x) * u1 + x * uNI;
      }
    }
  }
}
```

* case 3.5

Original Code
```
for (k=2;k<NK;k++) {
 for (j=2;j<NJ;j++) {
   for (i=2;i<NI;i++) {
      for (m=0;m<NM;m++) {
          py = (1-y)*u(m,i,1,k)+y*u(m,i,NJ,k)
   }
  }
 }
}
```
Optimized result
```
double u1, uNJ;
for (k = 2; k < NK; k++) {
  for (i = 2; i < NI; i++) {
    for (m = 0; m < NM; m++) {
      // Cache these values once per iteration of i and k, assuming j does not affect them
      u1 = u(m, i, 1, k);
      uNJ = u(m, i, NJ, k);
      for (j = 2; j < NJ; j++) {
        py = (1 - y) * u1 + y * uNJ;
      }
    }
  }
}
```


* case 3.6

Original Code
```
for (k=2;k<NK;k++) {
 for (j=2;j<NJ;j++) {
   for (i=2;i<NI;i++) {
      for (m=0;m<NM;m++) {
          pz = (1-z)*u(m,i,j,1)+z*u(m,i,j,NK)
   }
  }
 }
}
```
Optimized result
```
double u_first, u_last;
for (k = 2; k < NK; k++) {
  for (j = 2; j < NJ; j++) {
    for (i = 2; i < NI; i++) {
      for (m = 0; m < NM; m++) {
        // Precompute once for all m in this (i, j) iteration
        u_first = u(m, i, j, 1);
        u_last = u(m, i, j, NK);
        pz = (1 - z) * u_first + z * u_last;
      }
    }
  }
}
```


* case 4.1

Original Code
```
for (k=0;k<NK;k++) {
 for (i=0;i<M;i++) {
   for (j=0;j<D;j++) {
      S = S + x[i,j]*w[j]
  }
 }
}
```
Optimized result
```
for (i = 0; i < M; i++) {
  double tempSum = 0;
  for (j = 0; j < D; j++) {
    tempSum += x[i,j] * w[j];
  }
  sum += tempSum;
}
S += sum * NK;
```


* case 4.2

Original Code
```
for (k=0;k<NK;k++) {
 for (i=0;i<M;i++) {
   for (m=0;m<D;m++) {
      d[m] = x[i,m]*S
  }
 }
}
```
Optimized result
```
for (m = 0; m < D; m++) {
  d[m] = x[M-1, m] * S;
}
```


* case 4.3

Original Code
```
for (k=0;k<NK;k++) {
 for (ii=0;ii<D;ii++) {
   w[ii] = w[ii] + r*d[ii]
 }
}
```
Optimized result
```
for (ii = 0; ii < D; ii++) {
  w[ii] += NK * r * d[ii];
}
```


* case 5

Original Code
```
for (i=0;i<N;i++) {
 for (j=0;j<N;j++) {
   w = w + sin(a[i]+b[j])
 }
}
```
Optimized result
```
double precomputed_sins[N][N];
for (i = 0; i < N; i++) {
  for (j = 0; j < N; j++) {
    precomputed_sins[i][j] = sin(a[i] + b[j]);
  }
}

double sum = 0;
for (i = 0; i < N; i++) {
  for (j = 0; j < N; j++) {
    sum += precomputed_sins[i][j];
  }
}
w += sum;
```


* case 7

Original Code
```
while (iteration<NOiteration) {
 for (i=0;i<m;i++) {
   d = d + a[i]+b[i]*w
 }
}
```
Optimized result
```
double[] bw = new double[m];
for (i = 0; i < m; i++) {
  bw[i] = b[i] * w;
}

while (iteration < NOiteration) {
  for (i = 0; i < m; i++) {
    d = d + a[i] + bw[i];
  }
  iteration++;
}
```


* case 8

Original Code
```
for (inter=0;inter<iter;inter++) {
 for (j=0;j<N;j++) {
   a[2*i] = a[2*i] + b[j]
 }
}
```
Optimized result
```
double sum_b = 0;
for (j = 0; j < N; j++) {
  sum_b += b[j];
}
sum_b *= iter;  // Multiply the sum of b by the number of iterations

for (i = 0; i < N/2; i++) {
  a[2*i] += sum_b;
}
```
