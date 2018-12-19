#include <iostream>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include "mpi.h"

using namespace std;

// Do NOT Modify
#define EVENT_SIZE 2

// Change as neccessary
#define INPUT_FILE "input1.txt"
#define N 3     // # processes
#define M 4     // # of events

int main(int argc, char *argv[]) {
  int size, rank;
  char given_events[N+1][M][EVENT_SIZE];

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);     // My process number
  MPI_Comm_size(MPI_COMM_WORLD, &size);     // How many processes there are

  if (rank == 0) {
    string line;
    char event[2];
    ifstream test_file;

    // Read input file
    test_file.open(INPUT_FILE);
    if (test_file) {
      // cout << "Read file" << endl;
    }
    else {
      // cout << "File not read" << endl;
      return 0;
    }

    for (int i = 0; i < 1; i++) {
      for (int j = 0; j < M; j++) {
        event[0] = 'a';
        event[1] = 'a';
        for (int k = 0; k < EVENT_SIZE; k++) {
          given_events[i][j][k] = event[k];
        }
      }
    }

    // cout << "Receive this from the text file" << endl;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < M; j++) {
          for (int k = 0; k < EVENT_SIZE; k++) {
              // cout << given_events[i][j][k];
          }
      }
      // cout << endl;
    }
    // cout << endl;

    for (int i = 1; i < N+1; i++) {
      getline(test_file, line);
      //cout << line << endl;
      for (int j = 0; j < M; j++) {
        event[0] = line[j * 2];
        //cout << event[0];
        event[1] = line[j * 2 + 1];
        //cout << event[1];
        for (int k = 0; k < EVENT_SIZE; k++) {
          given_events[i][j][k] = event[k];
        }
      }
    }
    test_file.close();

    // print array
    // cout << "Receive this from the text file" << endl;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < M; j++) {
          for (int k = 0; k < EVENT_SIZE; k++) {
              // cout << given_events[i][j][k];
          }
      }
      // cout << endl;
    }
    // cout << endl;


    int answers[N][M][EVENT_SIZE];

  }

  MPI_Barrier(MPI_COMM_WORLD);

  int sub_answers [1][M];
  char sub_events [1][M][EVENT_SIZE];

  if (rank == 0) {

    int result = MPI_Scatter(given_events, M * EVENT_SIZE, MPI_CHAR, MPI_IN_PLACE,  0, MPI_CHAR, 0, MPI_COMM_WORLD);
    // cout << "MPI result " << result << endl;
  }
  else {
    int result = MPI_Scatter(NULL, M * EVENT_SIZE, MPI_CHAR, &sub_events,  M * EVENT_SIZE, MPI_CHAR, 0, MPI_COMM_WORLD);
    // cout << "MPI result " << result << endl;
  }
  // MPI_Scatter(given_events, M * EVENT_SIZE, MPI_CHAR, &sub_events, M * EVENT_SIZE, MPI_CHAR, 0, MPI_COMM_WORLD);

  if (rank != 10) {
      // cout << "printing given events..." << endl;
      // cout << "Process: " << rank << endl;
      for (int i = 0; i < 1; i++) {
        for (int j = 0; j < M; j++) {
          for (int k = 0; k < EVENT_SIZE; k++) {
            // cout << sub_events[i][j][k];
          }
        }
      }
      // cout << endl;
  }

  if(rank == 0) {
    MPI_Status status;
    int num_done = 0;
    char recv_buffer[4];

    int send_array[100];
    fill_n(send_array, 100, -1);

    // cout << "process 0 waiting to receive..." << endl;
    while (num_done != N) {
        MPI_Recv(recv_buffer, 4, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        // cout << "received: ";
        for(int i = 0; i < 4; i++) {
            // cout << recv_buffer[i];
        }
        // cout << endl;

        if (recv_buffer[0] == 's') {
            char c = recv_buffer[1];
            int index = atoi(&c);
            char tmp[2] = {recv_buffer[2], recv_buffer[3]};
            int val = atoi(tmp);

            send_array[index] = val;

            // cout << "index: " << index << " value: " << send_array[index] << endl;

            // cout << "current send array ";
            // for (int i = 0; i < 6; i++) {
            //     cout <<
            // }

        } else if (recv_buffer[0] == 'r') {
            char c = recv_buffer[1];
            int index = atoi(&c);
            // int index = recv_buffer[1];
            int send_val = send_array[index];

            // cout << "received index: " << index << endl;

            // Send message and keep going
            // cout << "process: " << rank;
            // cout << " sending msg: " << send_val << endl;

            MPI_Send(&send_val, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);

        } else if (recv_buffer[0] == 'd') {
            num_done++;
        }
    }
  } else {
      int lc_val = 0;
      MPI_Request ireq;
      MPI_Status istatus;

      for (int i = 0; i < M; i++) {
        // cout << "current sub event: " << sub_events[0][i][0] << endl;
        if (sub_events[0][i][0] == 's') {
            sub_answers[0][i] = ++lc_val;

            // Convert lc_val to a 2-char array
            char tmp[3];
            snprintf (tmp, 3, "%02d", lc_val);
            // cout << "THE VALUE OF lc_val is " << tmp << endl;

            // Create send message
            char send_msg[4] = {'s', sub_events[0][i][1], tmp[0], tmp[1]};

            // Send message and keep going
            // cout << "process: " << rank;
            // cout << " sending msg: " << send_msg << endl;
            // MPI_Isend(&send_msg, 4, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &ireq);
            // MPI_Wait(&ireq, &istatus);

            MPI_Send(&send_msg, 4, MPI_CHAR, 0, 0, MPI_COMM_WORLD);

        } else if (sub_events[0][i][0] == 'r') {
            int val = -1;
            int sleep_time = .5 * 1000;
            int num_cycles = 0;

            char send_msg[4] = {'r', sub_events[0][i][1], '!', '!'};

            do {

                MPI_Send(&send_msg, 4, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
                MPI_Recv(&val, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                // cout << "process " << rank << " about to sleep..." << endl;
                usleep(sleep_time);
                num_cycles++;
            } while (val == -1 && num_cycles < 15);

            lc_val = max(lc_val, val);
            sub_answers[0][i] = ++lc_val;

        } else {
            sub_answers[0][i] = ++lc_val;
        }
      }

      // Create send message
      char send_msg[4] = {'d', -10, -10, -10};

      // Send message and keep going
      // cout << "process " << rank << " is done!!!!" << endl;
      MPI_Send(send_msg, 4, MPI_CHAR, 0, 0, MPI_COMM_WORLD);

      // for (int i =0; i < M; i++) {
      //     cout << sub_answers[0][i];
      // }
      // cout << endl;


      cout << "p" << rank - 1 << ": ";
      for (int i = 0; i < M; i++) {
          cout << sub_answers[0][i] << " ";
      }
      cout << endl;
  }

  MPI_Barrier(MPI_COMM_WORLD);

  int all_answers[N][M];

  MPI_Gather(sub_answers, M, MPI_INT, all_answers, N * M, MPI_INT, 0, MPI_COMM_WORLD);
  //
  // if (rank == 0) {
  //     cout << "printing answers..." << endl;
  //     for (int i = 0; i < N; i++) {
  //       cout << endl;
  //       for (int j = 0; j < M; j++) {
  //           cout << all_answers[i][j];
  //       }
  //     }
  // }

  MPI_Finalize();
  return 0;
}
