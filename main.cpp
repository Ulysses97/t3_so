#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <stdlib.h>

using namespace std;

class Matrix {
  private:
    vector<vector<float>> matrix;
    int n;
    float det;

    int get_max_abs(vector<float> v, int j) {
      int index = j;

      for(int i = j; i < this->n; ++i) {
        if(abs(v[i]) > abs(v[index])) index = i;
      }

      return index;
    }

    void swap_cols(int u, int v) {
      for(int i = 0; i < this->n; ++i) {
        float aux = this->matrix[i][u];
        this->matrix[i][u] = this->matrix[i][v];
        this->matrix[i][v] = aux;
      }
      this->det *= -1.0;
    }

    void elim(float m, int u, int v) {
      for(int i = 0; i < this->n; ++i) {
        this->matrix[i][v] -= m * this->matrix[i][u];
      }
    }

    void gauss_elim() {
      for(int i = 0; i < this->n; ++i) {
        if(this->matrix[i][i] == 0.0) {
          int index_max = this->get_max_abs(matrix[i], i);
          if(index_max != i) this->swap_cols(index_max, i);
        }

        float pivot = this->matrix[i][i];

        for(int j = i+1; j < this->n; ++j) {
          float m = this->matrix[i][j] / pivot;
          this->elim(m, i, j);
        }
      }
    }

  public:
    Matrix(int n) {
      this->matrix.resize(n, vector<float>(n,0));
      this->n = n;
      this->det = 1.0;

      // random number generator
      default_random_engine rng(chrono::system_clock::now().time_since_epoch().count());
      uniform_real_distribution<float> dist(-1,1);

      for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
          this->matrix[i][j] = dist(rng);
        }
      }
    }

    float determinant() {
      this->gauss_elim();

      for(int i = 0; i < this->n; ++i) {
        det *= this->matrix[i][i];
      }

      return det;
    }

    void print() {
      for(int i = 0; i < this->n; ++i) {
        for(int j = 0; j < this->n; j++) {
          cout << this->matrix[i][j] << ' ';
        }
        cout << endl;
      }
    }
};

int main(int argc, char** argv) {

  if(argc < 2) {
    cerr << "Uso: ./main N" << endl;
    return 1;
  }

  const int N = atoi(argv[1]);

  auto start = chrono::high_resolution_clock::now();

  Matrix* matrix = new Matrix(N);

  cout << matrix->determinant() << endl;

  auto stop = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

  cout << endl << duration.count()/1000.0 << " ms" << endl;

  return 0;
}