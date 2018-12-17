# Lamport's Logical Clock implemented using MPI
</br>
Group Members: </br>
Barry Ibarra </br>
Viet Le </br>
Daniel Bravo </br>

</br>
Psudo code will look something like: 

```
char all_events[n][m][2]
Scatter to every process but 0
if (rank == 0)
  MPI_Status status
  read from file input into char 
  bool all_done = false
  while (!all_done)
    MPI_Receive(... MPI_ANY_SOURCE... &status
    if (received message is a send)
      write the value into the send_array
     else if (received message is a receive)
      MPI_Send(.... status.MPI_SOURCE...)
     else if (some condition to signal all processes are done)
      all_done = true

else 
  // This is every other process. They process the events one by one

MPI_Barrier()
MPI_Gather(...) // Gathering the int array
if (rank == 0)
  Print the array containing the LC values
MPI_Finalize()
```
