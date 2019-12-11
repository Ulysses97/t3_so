#include <iostream>
#include <array>

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

  std::array<int, 8> arr = {5,9,1,3,0,4,3,8};

  MergeSort<8>(&arr, 0, 7);

  for(int i = 0; i < 8; ++i) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}