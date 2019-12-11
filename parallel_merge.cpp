#include <iostream>
#include <array>
#include <algorithm>
#include <omp.h>

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

  #pragma omp parallel for
  for(int i = start; i <= end; ++i) {
    (*arr)[i] = temp[i-start];
  }
}

template <int n>
void MergeSort(std::array<int, n>* arr, int start, int end) {
  if(start < end) {
    int middle = (start+end) / 2;
    #pragma omp parallel sections
    {
      { MergeSort<n>(arr, start, middle); }
      #pragma omp section
      { MergeSort<n>(arr, middle+1, end); }
    }
    merge<n>(arr, start, end);
  }
}

int main() {

  const int N = 100;

  std::array<int, N> arr;

  for(int i = 0; i < N; ++i) {
    arr[i] = i+1;
  }

  std::random_shuffle(arr.begin(), arr.end());

  for(int i = 0; i < N; ++i) {
    std::cout << arr[i] << " ";
  }

  MergeSort<N>(&arr, 0, N-1);

  for(int i = 0; i < N; ++i) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}