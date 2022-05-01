# MemoryAllocationSimulation
Demonstration of different memory allocation algorithms


## Compiling
In the linux terminal, go to the directory of the program and run the command "gcc holes.c -o holes"


## Running the program
Run the program using ./holes INPUTFILE
INPUTFILE must be a properly formated file, it is required to run the program.
## Using the simulator

The simulator takes no user input during run time. The simulator uses the following algorithms in order to allocate memory:
First Fit
Best Fit
Worst Fit
Next Fit

The simulator prints when a process is added to the memory, as well as the number of holes, number of processes, memory usage and cumulative memory usage
When the ready queue is empty, the simulator will print the final statistics

## Design
The simulator uses a linked list to create a ready queue of processes
The main memory is set at 128MB and it assumes all processes are an integer value
The main memory is represented by a doubly linked list of processes. Empty memory is stored as emptry process structs
When a process is removed from the memory to create space, holes before and after the process will be merged to create a single hole
After one of the algorithms has run, all variables are reset for the next algorithm


### First fit
- finds first hole of the appropriate size
- adds process to memeory, creates hole if there is remaining room
- if there isn't enough space, remove the oldest process

### Best fit
- first hole of appropriate size is chosen as best fit
- if a more optimal hole is found, that becomes the best fit hole
- adds process to best fit hole
- if there isn't enough space, remove oldest process

### Worst fit
- first hole of appropriate size is chosen as worst fit
- if a less optimal hole is found, that becomes the worst fit hole
- adds process to worst fit hole
- if there isn't enough space, remove oldest process

### Next fit
- starts at last inserted process, searches for a hole that fits
- if no hole is found after that process, a search from the begining of the memeory will look for a hole until the entire memeory is searched
- if no hole is found, remove oldest process

## Limitations

The '!' character cannot represent a process name, it is reserved to represent a hole.

## References

1. C Program of priority queue using linked list
Author: Animesh Shaw
Publication date: September 22, 2012
URL: https://codeitaway.wordpress.com/2012/09/22/c-program-of-priority-queue-using-linked-list/
Code referenced for linked list implementation. Lines: 667-750
2. Program to copy one linked list into another using recursion
Author: Jackson Bouchard 
URL: http://www.dailyfreecode.com/code/copy-linked-list-another-recursion-2824.aspx
Code referenced and modified for copying linked list. Lines: 645-659
