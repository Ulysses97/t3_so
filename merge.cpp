#include <iostream>
#include <array>
#include <algorithm>
#include <chrono>
#include <fstream>

template <int n>
void merge(std::array<int, n>* arr, int start, int end) {
  int middle = (start+end) / 2;
  int current1 = start;
  int current2 = middle+1;

  std::array<int, n> temp;

  for(int i = 0; i < end-start+1; ++i) {
    if((*arr)[current1] < (*arr)[current2]) {
      if(current1 > middle) {
        temp[i] = (*arr)[current2++];
      }
      else {
        temp[i] = (*arr)[current1++];
      }
    }
    else {
      if(current2 > end) {
        temp[i] = (*arr)[current1++];
      }
      else {
        temp[i] = (*arr)[current2++];
      }
    }
  }

  for(int i = start; i <= end; ++i) {
    (*arr)[i] = temp[i-start];
  }
}

template <int n>
void MergeSort(std::array<int, n>* arr, int start, int end) {
  if(start < end) {
    int middle = (start+end) / 2;
    MergeSort<n>(arr, start, middle);
    MergeSort<n>(arr, middle+1, end);
    merge<n>(arr, start, end);
  }
}

int main() {

  std::ofstream output_data("MergeSortSerial.txt", std::ios::trunc);

  const int min_n = 10000;
  const int max_n = 20000;
  const int step = 500;
  const int reps = 3;

  std::array<int, max_n> arr;

  for(int i = 0; i < max_n; ++i) {
    arr[i] = i+1;
  }

  for(int N = min_n; N <= max_n; N += step) {
    float time_sum = 0;
    float avg_time;

    for(int i = 0; i < reps; ++i) {
      std::random_shuffle(arr.begin(), arr.begin()+N);

      auto start = std::chrono::high_resolution_clock::now();

      MergeSort<max_n>(&arr, 0 , N);

      auto stop = std::chrono::high_resolution_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
      time_sum += duration.count()/1000.0;
    }

    output_data << N << ' ' << (time_sum/reps) << std::endl;
  }

  return 0;
}