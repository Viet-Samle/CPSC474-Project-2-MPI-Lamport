# Lamport's Logical Clock implemented using MPI
</br>
Psudo code will look something like: 

```
string array[n][m]
int array [n][m + 1] // Will be the array holding the final solution, with element 0 holding the value of the process's current LC value 
int send array[100]  // Some arbritrary number that's larger than the actual number of send events
initialize mpi with rank and size
scatter the string array, n number of processes, each process will have string array[rank]
scatter the int array
Read the string array and for each send, record it in the send array 
MPI_Barrier
Calculate value of each event's LC values into the int array.
MPI_Barrier
MPI_Gather for the int array
Have process 0 print the int array
MPI_Finalize
```
</br>
Questions to ask the Professor
</br>
If we're not modifying the string array, do we even need to scatter it? It can be maintained as a shared array that every process can read.
Asking because it seems like scattering a string array with mpi_scatter seems finicky at best and a nightmare at worst.
</br>
</br>
Right now this is assuming that we have an n number of processes, which makes sense because Lamport's is separated into processes. 
</br>
Is it ok if we assume/force this project to have the same number of processes as the n value?
</br>
When we're trying to run it with `mpirun -n __ ./a.out` is it ok if we have a predetermined value in the __ space or would you want something more dynamic?
</br>
