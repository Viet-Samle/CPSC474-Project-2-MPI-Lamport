#include <iostream>
#include <algorithm>
#include <fstream>
#include "mpi.h"

using namespace std;

#define N 1
#define M 10
#define event_size 2

int main(int argc, char *argv[]) {
  int size, rank;
  char given_events[N][M][event_size];

  int send_array[100];
  fill_n(send_array, 100, -1);

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    string line;
    char event[2];
    ifstream test_file;
    test_file.open("input.txt");
    if (test_file) {
      cout << "Read file" << endl;
    }
    else {
      cout << "File not read" << endl;
      return 0;
    }

    for (int i = 0; i < N; i++) {
      getline(test_file, line);
      //cout << line << endl;
      for (int j = 0; j < M; j++) {
        event[0] = line[j * 2];
        //cout << event[0];
        event[1] = line[j * 2 + 1];
        //cout << event[1];
        for (int k = 0; k < event_size; k++) {
          given_events[i][j][k] = event[k];
        }
      }
    }
    test_file.close();
    // For testing purposes. Printing the given_events
  /*
    for (int i = 0; i < N; i++) {
      cout << endl;
      for (int j = 0; j < M; j++) {
        for (int k = 0; k < event_size; k++) {
          cout << given_events[i][j][k];
        }
      }
    }
  */

  }

  MPI_Finalize();
  return 0;
}
