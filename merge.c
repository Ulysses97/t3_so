#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define THREADS_NUM 2

int randInt(int low, int hig) {
  return (rand() % (hig-low+1)) + low;
}

void shuffle_arr(int arr[], int size) {
  int i;
  #pragma omp parallel for
  for(i = 0; i < size; ++i) {
    arr[i] = randInt(0, 1000);
  }
}

void print_arr(int arr[], int size) {
  int i;
  for(i = 0; i < size; ++i) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

void merge(int arr[], int start, int end) {
  int len = end-start+1;
  int* temp = malloc(sizeof(int) * len);

  int middle = (start+end) / 2;
  int currentL = start;
  int currentR = middle+1;

  int i;
  for(i = 0; i < len; ++i) {
    if(arr[currentL] < arr[currentR]) {
      if(currentL > middle) {
        temp[i] = arr[currentR++];
      }
      else {
        temp[i] = arr[currentL++];
      }
    }
    else {
      if(currentR > end) {
        temp[i] = arr[currentL++];
      }
      else {
        temp[i] = arr[currentR++];
      }
    }
  }

  #pragma omp parallel for
  for(i = start; i <= end; ++i) {
    arr[i] = temp[i-start];
  }

  free(temp);
}

void merge_sort_serial(int arr[], int start, int end) {
  if(start < end) {
    int middle = (start+end) / 2;
    merge_sort_serial(arr, start, middle);
    merge_sort_serial(arr, middle+1, end);
    merge(arr, start, end);
  }
}

void merge_sort_parallel(int arr[], int start, int end, int threads) {
  if(threads == 1) {
    merge_sort_serial(arr, start, end);
  }
  else if(threads > 1) {
    if(start < end) {
      int middle = (start+end) / 2;

      #pragma omp parallel sections
      {
        #pragma omp section
          merge_sort_parallel(arr, start, middle, threads/2);
        #pragma omp section
          merge_sort_parallel(arr, middle+1, end, threads-threads/2);
      }
      merge(arr, start, end);
    }
  }
}

int main() {

  srand(time(NULL));

  int min_n = 100000;
  int max_n = 1000000;
  int step = 100000;
  int reps = 3;

  int* arr = malloc(sizeof(int) * max_n);

  int n;
  for(n = min_n; n <= max_n; n += step) {
    double time_sum = 0;

    int i;
    for(i = 0; i < reps; ++i) {
      shuffle_arr(arr, n);

      double start = omp_get_wtime();
      merge_sort_serial(arr, 0, n);
      double stop = omp_get_wtime();
      time_sum += stop-start;
    }
    printf("%d%c %f\n", n, 's', time_sum/reps);
  }

  for(n = min_n; n <= max_n; n += step) {
    double time_sum = 0;

    int i;
    for(i = 0; i < reps; ++i) {
      shuffle_arr(arr, n);

      double start = omp_get_wtime();
      merge_sort_parallel(arr, 0, n, THREADS_NUM);
      double stop = omp_get_wtime();
      time_sum += stop-start;
    }
    printf("%d%c %f\n", n, 'p', time_sum/reps);
  }

  free(arr);

  return 0;
}