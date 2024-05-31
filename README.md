. -> no boundary wall  
X -> Normal block  
Y -> Target block  
W -> Wall  
P -> Spider   
G -> Grasshopper  
S -> Snail   
L -> LadyBug   
F -> Butterfly  

The input will be given as a matrix. The matrix elements can belong to the above classes. If the grid is n\*m, then the 
matrix size is (2n+1)\*(2m+1).

Refer sample image. The top left of matrix is the west corner of the image.

Sample Encoding:  
...........  
.....Y...Y.  
...........  
.G.GWSWPWY.  
...........  
.......X...  
...........  
.......Y...  
.......W...  

## Updates:
- The code is working as a basic DFS as of 0954AM, 31st May.

- Test Results:  <0.4 sec on most examples, and <4.5 secs on all examples.

- Optimisations: The algorithm is a simple BFS over the state space. One extra pruning condition is that if moves < no. of empty targets, return false. This is valid for all levels 1-90.