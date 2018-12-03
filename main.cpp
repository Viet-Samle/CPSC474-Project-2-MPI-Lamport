#include <iostream>
#include <algorithm>

using namespace std;

#define N 10
#define M 10

int main(int argc, char *argv[]) {
  string given_events[N][M];

  int send_array[100];
  fill_n(send_array, 100, -1);

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
        given_events[i][j] = "NULL";
      }
    }
  }

  cout << "Enter the input for the logical clock" << endl;
  for (int i = 0; i < N; i++) {
    cout << "Enter the events for process " << i << endl;
    for (int j = 0; j < M; j++) {
      cin >> input;
      if (input != "NULL") {
          given_events[i][j] = input;
      }
    }
  }


}
