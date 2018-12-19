#include "mpi.h"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unistd.h>

using namespace std;

// Do NOT Modify
#define EVENT_SIZE 2

// Change as neccessary
#define INPUT_FILE "input1.txt"
#define N 3 // # processes
#define M 4 // # of events

int main(int argc, char *argv[]) {
  // Declare an initialize MPI variables
  int size, rank;
  char given_events[N + 1][M][EVENT_SIZE];

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); // My MPI process number
  MPI_Comm_size(MPI_COMM_WORLD, &size); // Total number of MPI processes

  // Process 0 reads the data
  if (rank == 0) {
    string line;
    char event[2];
    ifstream test_file;

    // Open file
    test_file.open(INPUT_FILE);
    if (!test_file) {
      return 0;
    }

    // Insert and extra row becuase MPI_Scatter skips the first one
    for (int i = 0; i < 1; i++) {
      for (int j = 0; j < M; j++) {
        event[0] = 'a';
        event[1] = 'a';
        for (int k = 0; k < EVENT_SIZE; k++) {
          given_events[i][j][k] = event[k];
        }
      }
    }

    // Read data
    for (int i = 1; i < N + 1; i++) {
      getline(test_file, line);

      for (int j = 0; j < M; j++) {
        event[0] = line[j * 2];
        event[1] = line[j * 2 + 1];
        for (int k = 0; k < EVENT_SIZE; k++) {
          given_events[i][j][k] = event[k];
        }
      }
    }
    test_file.close();
  }

  MPI_Barrier(MPI_COMM_WORLD);

  int sub_answers[1][M];
  char sub_events[1][M][EVENT_SIZE];

  // Scatter the data to process other than 0
  if (rank == 0) {
    int result = MPI_Scatter(given_events, M * EVENT_SIZE, MPI_CHAR,
                             MPI_IN_PLACE, 0, MPI_CHAR, 0, MPI_COMM_WORLD);
  } else {
    int result = MPI_Scatter(NULL, M * EVENT_SIZE, MPI_CHAR, &sub_events,
                             M * EVENT_SIZE, MPI_CHAR, 0, MPI_COMM_WORLD);
  }

  // Process 0 manage send_array
  if (rank == 0) {
    MPI_Status status;
    int num_done = 0;
    char recv_buffer[4];

    int send_array[100];
    fill_n(send_array, 100, -1);

    // Run until all Lamport processes have completed
    while (num_done != N) {
      MPI_Recv(recv_buffer, 4, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG,
               MPI_COMM_WORLD, &status);

      // Handle a send message
      if (recv_buffer[0] == 's') {
        // Convert chars to ints
        char c = recv_buffer[1];
        int index = atoi(&c);
        char tmp[2] = {recv_buffer[2], recv_buffer[3]};
        int val = atoi(tmp);

        // Update send_array
        send_array[index] = val;

        // Handle a receive message
      } else if (recv_buffer[0] == 'r') {
        // Convert chars to ints
        char c = recv_buffer[1];
        int index = atoi(&c);
        int send_val = send_array[index];

        // Send the lc val that is stored
        // Will send -1 when we haven't received a matching send event
        // The -1 comes from the initialization of send_array
        MPI_Send(&send_val, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);

        // Handle a done message
      } else if (recv_buffer[0] == 'd') {
        num_done++;
      }
    }

    // All processes except 0 handle an array
  } else {
    int lc_val = 0;
    MPI_Request ireq;
    MPI_Status istatus;

    for (int i = 0; i < M; i++) {

      // Handle a send event
      if (sub_events[0][i][0] == 's') {
        sub_answers[0][i] = ++lc_val;

        // Zero-pad the current lc value
        char tmp[3];
        snprintf(tmp, 3, "%02d", lc_val);

        // Create send message
        char send_msg[4] = {'s', sub_events[0][i][1], tmp[0], tmp[1]};

        MPI_Send(&send_msg, 4, MPI_CHAR, 0, 0, MPI_COMM_WORLD);

        // Handle a receive event
      } else if (sub_events[0][i][0] == 'r') {
        int val = -1;
        int sleep_time = .5 * 1000;
        int num_cycles = 0;

        // Create receive message
        char send_msg[4] = {'r', sub_events[0][i][1], '!', '!'};

        // Keep sending the message until you get a valid lc value
        do {
          MPI_Send(&send_msg, 4, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
          MPI_Recv(&val, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD,
                   MPI_STATUS_IGNORE);

          // Sleep to not overwhelm process 0
          usleep(sleep_time);

          // Don't run indefinitely
          num_cycles++;
        } while (val == -1 && num_cycles < 15);

        // Update answers array with valid lc value
        lc_val = max(lc_val, val);
        sub_answers[0][i] = ++lc_val;

      } else {
        sub_answers[0][i] = ++lc_val;
      }
    }

    // Create done message
    char send_msg[4] = {'d', -10, -10, -10};

    MPI_Send(send_msg, 4, MPI_CHAR, 0, 0, MPI_COMM_WORLD);

    // Print the lc vaules for the completed Lamport process
    cout << "p" << rank - 1 << ": ";
    for (int i = 0; i < M; i++) {
      cout << sub_answers[0][i] << " ";
    }
    cout << endl;
  }

  MPI_Finalize();
  return 0;
}
