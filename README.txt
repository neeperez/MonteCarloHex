Graph.cpp: The Graph ADT that is used to represent the hex board. I've removed a lot of the code that was created for the previous assignment
(mainly because its long and ugly) and added the MonteCarlo() method along with a helper function.

Graph.hpp: This file stores the prototypes for the methods used in Graph.cpp.

main.cpp: The main client file that allows the player to input the desired size of the board as well as play against the Monte Carlo based AI.

***** NOTE *****
If you do decide to play on an 11x11 board, the wait for the AI to respond is not too long (<20 seconds), but since you're grading you probably 
want to set it to a smaller size.

Makefile: This file compiles the code necessary to run the main client. All that you would need to do is type 'make' into the command line and 
the Makefile will generate an executable file called 'main', which will be used to run the program.
