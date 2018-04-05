#ifndef XREN_MEDIAN_H
#define XREN_MEDIAN_H

namespace alg1{
int select_rec(int arr[], int left, int right, int k);
int select(int arr[], int n, int k);
}

namespace alg2{
int get_pivot(int arr[], int left, int right);
int select_rec(int arr[], int left, int right, int k);
int select(int arr[], int n, int k);
}

namespace alg3 {
void quickSort(int arr[], int left, int right);
int select(int arr[], int n, int k);
}


/* type definitions */
struct partition_result {
  // represents partition result;
  // arr[0:a] are less than pivot
  // arr[a:b] are equal to pivot
  // arr[b:n] are greater than pivot
  int a;
  int b;
};

typedef int (*SelectFunctionPointer)(int *, int, int);
/* end type defs */

#endif