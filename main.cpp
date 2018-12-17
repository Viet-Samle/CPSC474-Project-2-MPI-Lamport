#include <iostream>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include "mpi.h"

using namespace std;

#define N 2     // # processes
#define M 4    // # of events
#define EVENT_SIZE 2

int main(int argc, char *argv[]) {
  int size, rank;
  char given_events[N][M][EVENT_SIZE];

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);     // My process number
  MPI_Comm_size(MPI_COMM_WORLD, &size);     // How many processes there are

  if (rank == 0) {
    string line;
    char event[2];
    ifstream test_file;

    // Read input file
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
        for (int k = 0; k < EVENT_SIZE; k++) {
          given_events[i][j][k] = event[k];
        }
      }
    }
    test_file.close();


    int answers[N][M][EVENT_SIZE];
  }

  MPI_Barrier(MPI_COMM_WORLD);
  // Scatter the array
  int sub_answers[1][M];
  char sub_events[1][M][2];
  MPI_Scatter(given_events, M * EVENT_SIZE, MPI_CHAR, sub_events, M * EVENT_SIZE, MPI_CHAR, 0, MPI_COMM_WORLD);

  // if (rank != 0) {
  //     cout << "printing given events..." << endl;
  //     for (int i = 0; i < 1; i++) {
  //       cout << endl;
  //       for (int j = 0; j < M; j++) {
  //         for (int k = 0; k < EVENT_SIZE; k++) {
  //           cout << sub_events[i][j][k];
  //         }
  //       }
  //     }
  //     cout << endl;
  // }

  if(rank == 0) {
    MPI_Status status;
    int num_done = 0;
    char recv_buffer[4];

    int send_array[100];
    fill_n(send_array, 100, -1);

    cout << "process 0 waiting to receive..." << endl;
    while (num_done != N) {
        MPI_Recv(recv_buffer, 4, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        cout << "received: ";
        for(int i = 0; i < 4; i++) {
            cout << recv_buffer;
        }
        cout << endl;

        if (recv_buffer[0] == 's') {
            int index = atoi(&recv_buffer[1]);
            char tmp[2] = {recv_buffer[2], recv_buffer[3]};
            int val = atoi(tmp);

            send_array[index] = val;

        } else if (recv_buffer[0] == 'r') {
            int index = recv_buffer[1];
            int send_val = send_array[index];

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
        if (sub_events[1][i][0] == 's') {
            sub_answers[0][i] = ++lc_val;

            // Convert lc_val to a 2-char array
            char tmp[3];
            snprintf (tmp, 3, "%02d", lc_val);

            // Create send message
            char send_msg[4] = {'s', sub_events[1][i][1], tmp[0], tmp[1]};

            // Send message and keep going
            cout << "process: " << rank;
            cout << " sending msg: " << send_msg << endl;
            MPI_Isend(send_msg, 4, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &ireq);
            MPI_Wait(&ireq, &istatus);

        } else if (sub_events[0][i][0] == 'r') {
            int val = -1;
            int sleep_time = .5 * 1000;

            char send_msg[4] = {'r', sub_events[1][i][1], -100, -100};

            do {

                MPI_Send(&send_msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                MPI_Recv(&val, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                cout << "process " << rank << " about to sleep..." << endl;
                usleep(sleep_time);
            } while (val == -1);


            sub_answers[0][i] = max(lc_val++, val) + 1;

        } else {
            sub_answers[0][i] = ++lc_val;
        }
      }

      // Create send message
      char send_msg[4] = {'d', 0, 0, 0};

      // Send message and keep going
      cout << "process " << rank << " is done!!!!" << endl;
      MPI_Send(send_msg, 4, MPI_CHAR, 0, 0, MPI_COMM_WORLD);

      for (int i =0; i < M; i++) {
          cout << sub_answers[0][i];
      }
      cout << endl;


  }

  MPI_Barrier(MPI_COMM_WORLD);

  int all_answers[N][M];

  MPI_Gather(sub_answers, M, MPI_INT, all_answers, N * M, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0) {
      cout << "printing answers..." << endl;
      for (int i = 0; i < N; i++) {
        cout << endl;
        for (int j = 0; j < M; j++) {
            cout << all_answers[i][j];
        }
      }
  }

  MPI_Finalize();
  return 0;
}
