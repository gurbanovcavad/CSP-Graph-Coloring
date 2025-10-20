# CSP-Graph-Coloring
## To run the code:
```
g++ csp.cpp
./a.exe < input.txt (Get-Content input.txt | ./a.exe)
```
## Input format:
* colors: colors=k
    > colors=3
* edges: u,v
    > 1,2
## Implementation choices:
* storing the input:
    > To hold edges: I use an array of sets because when it comes to checking if a node is adjacent with x node, searching in a set is more efficient than an array. Additionally, using set ensures that there is no duplicate edges. 

    > To hold domains: I also use an array of sets for domains. This is also because of efficiency, especially we frequently need to check if there is a particular color of a variable. 

    > To keep track of prunes: I use a vector of arrays of size 2 called trail. 
    
    > LCV function: I create a vector of arrays which hold the number of nodes that are affected and a color, and then I sort them by using a custom comparator. After that, I add sorted colors to a vector and return it.

    > Revise function: I hold a variable called ok to check if the program removes any color so that the program can know if it should add current edges' neigboring edges. I have a vector called a which holds the colors which have to be removed from the domain of x. After that, I check colors one-by-one if we color x with color u and we will not have any colors available in y's domain, then it means that we have to add that color to a.
    
    > FirstAc3 function: it is the function that we call before backtracking to check if we have a solution.

    > Main function: the program gets the input in form of lines and break into parts. Then, the program creates domains for every single node and fills it with the colors {1,...,k}. I have a variable called ok which checks if there is a self-loop. The program checks ok and calls firstAc3(); and after that, it calls backtracking function and prints if there is a solution.

    > Isolated nodes: backtracking function runs calls mrv which has a loop from node 1 to node n, and this loop ensures that there will be no uncolored nodes left. 
## Output format:
* The result:   
    > SOLUTION: {1: 1, 2: 2, 3: 3, 4: 1, ..., n: 5}

    > failure