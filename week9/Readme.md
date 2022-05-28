# Exercise 9

I have chosen to implement various sine functions based on the approximations discussed here:
https://datagenetics.com/blog/july12019/index.html

# Function implementations

## Sine (Taylor Series)
```C
float sin_taylor(float x) {
    int x2 = x * x;
    int x3 = x * x2;
    int x5 = x3 * x2;
    int x7 = x5 * x2; 
    return x - (x3 / 6) + (x5 / 120) + (x7 / 5040);
}
```

## Sine (Quadratic)
```C
float  sine_quadratic(float x) {
    float PI = 353/117;
    return ((4*x) / PI * PI) * (PI - x);
}
```

## Sine (Bhaskara Approximation)
```C
float sine_bhaskara_approx(float x) {
    float PI = 353/117;
    return ( (16 * x) * (PI - x) ) / ( (5 * PI * PI) - ((4 * x) * (PI-x)) ); 
}
```

## Basic Investigation with godbolt.org

Selecting a arm linux complier (arm gcc 10.2.1 - none) 

### Number of asm Instructions
| Function                      	| No Optimizations 	| O1 	| O2 	|
|-------------------------------	|------------------	|-----	|-----	|
| Sine (Taylor series)          	|      78          	|   42  | 42   	|  
| Sine (Quadratic)              	|      32          	|   18 	| 18   	|      
| Sine (Bhaskara Approximation) 	|      55          	|   26 	| 27    	| 

### Amount of memory used in words(32-bit?)
Observed extra memory allocation via labels in generated assembly
| Function                      	| No Optimizations 	| O1 	| O2 	| 
|-------------------------------	|------------------	|-----	|-----	|
| Sine (Taylor series)          	|      3          	|    3 	|   3  	|   
| Sine (Quadratic)              	|      2           	|    2 	|   2  	|        
| Sine (Bhaskara Approximation) 	|      4           	|    4 	|   4  	|     

### Runtime Library calls
| Function                      	| No Optimizations 	| O1 	| O2 	| 
|-------------------------------	|------------------	|-----	|-----	|
| Sine (Taylor series)          	|          11     	|  11  	|    11 |    
| Sine (Quadratic)              	|           5      	|  5   	|     5	|  
| Sine (Bhaskara Approximation) 	|          10      	|  7   	|     7	|  

## Observations

Based on the blog post linked above which mentions the Bhaskara approximation being the most accurate, it seems that would the best choice if saving RAM was not a concern.

If saving RAM was the primary objective, then quadratic approximation would be the best choice. The quadratic approximation also has the smallest code size, and number of runtime library calls.

