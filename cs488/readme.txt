Contents of zip file:  .
-readme.txt            .
-main.cpp              .
-pt.cpp                .              Project 1 README
-pt.h                  .                 Shane Lopez
-makeG                 .
........................

 For this assignment the goal was to use openGL to draw the author's name,
a simple scene including a house and a tree, and that same scene with polygons
filled in. The only constraint was that nothing other than GLPOINTS was to be 
used as a parameter for drawing.
 In order to achieve these goals it was necessary to create a line function,
which was done using an implementation of Bresenham's midpoint-algorithm 
adapted from the course text, which was modified to handle slopes of absolute 
values greater than 1, as well as vertical lines.
 For purposes of clarity, a point class was created and used to store
coordinate data. This made the passing of information to functions easier, and
made the code easier to understand during the debugging phase.
 Following the completion of the line function, the author's name was drawn
using simple lines, and the house scene was drawn with two rectangles and three
trapezoids. Filling of the latter shapes was achieved by means of a simple 
function, coded specifically for those shapes. The rectangle's upper two points
were sampled for x values, and lines between those x values were drawn with
decreasing y values (from ymax to y min). Filling the trapezoids was a little
trickier, but was achieved using the calculations of the line function to
calculate the x and y intercepts of the outer two edges, the left edge's 
intercepts worked out from the bottom up, and the right edge's intercepts 
worked out from the top down; each of these sets of points were stored in 
arrays, and iterated over in a nested loop to form lines from the bottom up.
 As a general way to get around the long command used to compile the program: 
"g++ -lglut -lGLU -lGL main.cpp pt.cpp -o P1", a make file was made, and to 
compile the program all one needs to type is: "make -f makeG".
 As a general note, this program was built off of the SampleMain code supplied
by Chihua Ma, which was used as a basic setup for openGL, something the author 
is still acquainting himself with. 
