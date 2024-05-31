The game state is represented as a matrix. The input will be given as a matrix. The matrix elements can belong to the above classes.   

. -> no boundary wall, empty space
X -> Normal block  
Y -> Target block  
W -> Wall  
P -> Spider   
G -> Grasshopper  
S -> Snail   
L -> LadyBug   
B -> Butterfly  
H -> HoneyBee

If the grid is n\*m, then the matrix size is (2n+1)\*(2m+1). The tiles of game are present in (odd, odd) indices. Rest are to store walls.  

Refer sample_image.jpg. The top left of matrix is the west corner of the image grid in game.

The test case files consist of this grid, and the last line contains the number of moves to solve the level as per the original game.

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
- The code is working as a basic BFS as of 0954AM, 31st May.

- Test Results:  <0.4 sec on most examples, and <4.5 secs on all examples.

- Optimisations: The algorithm is a simple BFS over the state space. One extra pruning condition is that if moves < no. of empty targets, return false. This is valid for all levels 1-90.