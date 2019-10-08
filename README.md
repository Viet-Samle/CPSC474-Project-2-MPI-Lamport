# Lamport's Logical Clock Calculate implemented using MPI

## Group Members:
* Barry Ibarra
* Viet Le
* Daniel Bravo

## Course
CPSC 474 - Distributed Computing  
Prof. Doina Bein  
California State University, Fullerton

## Background
The goal of this project was to implement Lamport's Logical Clock Calculate using MPI. The idea was to improve the performance of the original algorithm. Since most of the work done was parallelizable, this was a perfect fit for MPI. _[Project 1 - Lamport Implementation](https://github.com/Viet-Samle/Lamport-s-Logical-Clock)_

## Implementation

### Pseudocode
```
If rank 0:
  Read input from file
  Send data to other processes with mpi_scatter
Else:
  Receive data from zero with mpi_scatter
If rank 0:
  while (all processes are not complete)
      wait to receive data
      parse message for requested send or receive event
          If send event
            Record send value in send array
          If receive event
            Send logical clock value for requested index
Else
  For (each event in scattered data)
    If (event is send)
      Send message to p0 to record in send array
    	Increment process LC value
    If (event is receive)
    	Send message to p0 to request send event value
    	Receive message from p0
  	    If (send event value == -1)
          Sleep and keep sending until send event != -1
        Else
        	Increment process LC value
  Print your processes' LC values
```

## Getting Started

### Files
* main.cpp
* input1.txt
* input2.txt
* input3.txt
* output1.txt
* output2.txt
* output3.txt
* run\*

\* not required

### Prerequisites
You will need [OpenMPI](https://www.open-mpi.org/software/ompi/v2.0/) installed on your machine.  
You can download it here: https://www.open-mpi.org/software/ompi/v2.0/  
Additionally, [here](https://stackoverflow.com/questions/42703861/how-to-use-mpi-on-mac-os-x) is some helpful instructions for installing it.

_Note: it may take up to 10 minutes to install_

### Usage
_Note: this was tested on MacOS running 10.14.1 and mpirun (Open MPI) 2.0.4_

Navigate to the folder that contains the files. The program defaults to reading input from file `input1.txt`. If you want to change the input file to one of the others or to a custom file, make sure to update lines `14-16` in `main.cpp` accordingly.

For convenience a `run` script was included and can be used as follows:

```
bash run
```
Alternatively, you compile and run the files manually:
```
mpic++ -o test main.cpp

mpirun -n 4 ./test

```
***Note:*** _the arg for `-n` must be 1 more than the number of processes in the input file._

## TODOs
* Take file name as input
* Take number of processes and number of events/process as input
* Gather at the end and have process 0 print results
* Ability to parse NULL events
* Print processes in order
* Implementation of verify algorithm


## Bugs
None (that we're aware of)
