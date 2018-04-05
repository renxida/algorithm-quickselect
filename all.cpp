#include "all.h"
#include <cstdlib>
#include <ctime> // for performance timing
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>

const int SEED = 5000; // seed for random array

const SelectFunctionPointer selection_functions[] = {alg1::select, alg2::select,
                                                     alg3::select};

/********************UTILITY FUNCTIONS****************/

void swap(int *a, int *b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

int *random_array(int n, int mini, int maxi, int seed = SEED) {
  std::mt19937 generator(seed);
  std::uniform_int_distribution<int> distribution(mini, maxi);
  int *arr = (int *)malloc(n * sizeof(int));
  for (int i = 0; i < n; i++) {
    arr[i] = distribution(generator);
  }
  return arr;
}

struct partition_result partition(int arr[], int left, int right) {
  // do three-way partition
  // returns struct partition_result with fields int a, int b.
  // [left, a) contains elements less than pivot
  // [a, b) contains elements equal to pivot
  // [b, right) contains element larger than pivot)

  int pivot = arr[left];

  int a = left; // index of first element equal to pivot, among
                // already-processed elements
  int b = left; // index of first element larger than pivot, among
                // already-processed.

  int cur = left; // first unprocessed element. Also, last larger + 1
  while (cur < right) {
    if (arr[cur] < pivot) {
      swap(&arr[cur], &arr[a]);
      a++;
    }
    // now arr[cur] is guaranteed to be equal or larger
    if (arr[cur] == pivot) {
      swap(&arr[cur], &arr[b]);
      b++;
    }

    cur++;
  }

  struct partition_result ret;
  ret.a = a;
  ret.b = b;
  return ret;
}

void printArray(int arr[], int size) {
  int i;
  for (i = 0; i < size; i++)
    printf("%d ", arr[i]);
  printf("\n");
}

int *input_array(int size) {
  int *ret = new int[size];
  int i;
  std::cout << "Enter integer array elements, separated by spaces:\n";
  for (i = 0; i < size; i++) {
    std::cin >> ret[i];
  }
  return ret;
}
/**********************UTILITY FUNCTIONS END*************************/

/*********************Algorithm 1:RANDOM**********************************/
namespace alg1 {

int select_rec(int arr[], int left, int right, int k) {

  // do a random swap to get random pivot
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(left, right - 1);
  swap(&arr[left], &arr[distribution(generator)]);

  struct partition_result p = partition(arr, left, right);

  if (p.a <= k && k < p.b) {
    return arr[p.a];
  } else if (k < p.a) {
    // recurse on left half
    return select_rec(arr, left = left, right = p.a, k = k);
  } else if (k >= p.b) {
    // recurse on right half
    return select_rec(arr, left = p.b, right = right, k = k);
  }
}

int select(int arr[], int n, int k) { return select_rec(arr, 0, n, k); }
} // namespace alg1
/*********************END RANDOM******************************/

/*********************S2: deterministic***********************/
namespace alg2 {

int get_pivot(int arr[], int left, int right) {
  // returns pivot of arr[left:right], using medians-of-medians method
  // detailed in https://www.ics.uci.edu/~eppstein/161/960130.html
  int n = right - left;
  if (n <= 10) {
    return alg1::select_rec(arr, left, right, (right + left) / 2);
  }

  int n_groups =
      (n % 5 == 0) ? (n / 5) : (n / 5 + 1); // equivalent to ceil(n/5);

  int *group_medians = (int *)malloc(n_groups * sizeof(int));

  int group_left;
  for (group_left = left; group_left < right; group_left += 5) {
    int group_right = group_left + 5;
    group_right = (group_right < right) ? group_right : right;

    group_medians[(group_left - left) / 5] = alg1::select_rec(
        arr, group_left, group_right, (group_right + group_left) / 2);
  }

  int ret = alg1::select(group_medians, n_groups, n_groups / 2);
  free(group_medians);
  return ret;
}

int select_rec(int arr[], int left, int right, int k) {
  if (right - left < 10) {
    return alg1::select_rec(arr, left, right, k);
  }

  // instead of random pivot, do this:

  int pivot = get_pivot(arr, left, right);

  int idx_pivot = left;
  while (arr[idx_pivot] != pivot) {
    if (idx_pivot >= right) {
      // exit with an error
      throw std::out_of_range("Went out-of-bounds while searching for pivot.");
    }
    ++idx_pivot;
  }

  swap(&arr[left], &arr[idx_pivot]);

  struct partition_result p = partition(arr, left, right);

  if (p.a <= k && k < p.b) {
    return arr[p.a];
  } else if (k < p.a) {
    // recurse on left half
    return select_rec(arr, left, p.a, k = k);
  } else if (k >= p.b) {
    // recurse on right half
    return select_rec(arr, p.b, right, k = k);
  }
}

int select(int arr[], int n, int k) { return select_rec(arr, 0, n, k); }

} // namespace alg2
/*********************END DETERMINISTIC***********************/

/*********************Algorithm 3: QUICKSORT******************/
namespace alg3 {
void quickSort(int arr[], int left, int right) {
  // left: leftest element to toch
  // right: first element not to toch
  // base cases
  if (right - left <= 1) {
    return;
  }

  // do a random swap to ensure that the pivot is random
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(left, right - 1);
  swap(&arr[left], &arr[distribution(generator)]);

  struct partition_result p = partition(arr, left, right);

  quickSort(arr, left, p.a);
  quickSort(arr, p.b, right);
}

int select(int arr[], int n, int k) {
  quickSort(arr, 0, n);
  return arr[k];
}

} // namespace alg3

namespace test_correctness {
/*********************END QUICKSORT***************************/
int correctness_cell(SelectFunctionPointer select, int k) {
  int correctness_arr[] = {7,  2, 4,  6, 9, 11, 2,  6, 10, 6,
                           15, 6, 14, 2, 7, 5,  13, 9, 12, 15};
  int n = 20;
  return select(correctness_arr, n, k);
}

void correctness_row(int k) {

  std::cout << "|" << std::setw(10) << k << "|";
  for (int i = 0; i < 3; i++) {
    std::cout << std::setw(15) << std::fixed << std::setprecision(2)
              << correctness_cell(selection_functions[i], k) << "|";
  }
  std::cout << std::endl;
}

/** driver function **/
void correctness(void) {
  std::cout << "Tabulating correctness for the algorithms." << std::endl;
  std::cout << "\tShowing output: kth smallest in array given in specification"
            << std::endl;
  std::cout << "\tIndexing start at k=0 yielding smallest element."
            << std::endl;
  std::cout << std::endl;
  std::cout << "|" << std::setw(10) << "k"
            << "|";
  for (int i = 0; i < 3; i++) {
    std::cout << std::setw(14) << "algorithm " << i + 1 << "|";
  }
  std::cout << std::endl;
  std::cout << "|:--------:|---------------|---------------|---------------|n";
  for (int k = 0; k < 20; ++k) {
    correctness_row(k);
  }
  std::cout << std::endl;
}

} // namespace test_correctness

namespace test_corner_cases {
void corner_cases() {
  std::cout << "Testing corner-cases for the algorithm." << std::endl;
  // case 1
  std::cout << "\t * selecting from 1-element array\n";
  for (int i = 0; i < 3; ++i) {
    int output;
    int arr[] = {998};
    output = selection_functions[i](arr, 1, 0);
    if (output == 998) {
      std::cout << "\t\t * Success: algorithm " << i + 1 << std::endl;
    } else {
      std::cout << "\t\t * Fail: algorithm " << i + 1 << std::endl;
      std::cout << "\t\t\t * Actual output: " << output << std::endl;
    }
  }
  // case 2
  std::cout << "\t * selecting from sorted length-3 array\n";
  for (int i = 0; i < 3; ++i) {
    int result[3];
    std::cout << "\t\t * algorithm " << i + 1 << " outputted { ";
    for (int j = 0; j < 3; ++j) {
      int arr[] = {1, 2, 3};
      std::cout << selection_functions[i](arr, 3, j) << " ";
    }
    std::cout << "}.\n";
  }

  std::cout << "\t * selecting from reverse-sorted length-3 array\n";
  for (int i = 0; i < 3; ++i) {
    int result[3];
    std::cout << "\t\t * algorithm " << i + 1 << " outputted { ";
    for (int j = 0; j < 3; ++j) {
      int arr[] = {3, 2, 1};
      std::cout << selection_functions[i](arr, 3, j) << " ";
    }
    std::cout << "}.\n";
  }

  // case 3: large array
  std::cout << "\t * selecting from large random array\n";
  std::cout << "\t   (the output should be around 5e6)\n";
  int *arr_large;
  int n_large = 1e7;
  for (int i = 0; i < 3; ++i) {
    arr_large = random_array(n_large, 1, n_large / 100);
    std::cout << "\t\t * algorithm " << i + 1 << " outputted "
              << selection_functions[i](arr_large, n_large, n_large / 2)
              << std::endl;
    free(arr_large);
  }
}
} // namespace test_corner_cases

namespace test_performance {
float performance_cell(SelectFunctionPointer select, int *arr, int n) {
  std::clock_t t0 = std::clock();
  select(arr, n, n / 2);
  return (std::clock() - t0) / (float)(CLOCKS_PER_SEC / 1000);
  return 0;
}

float *performance_row(int n) {
  int *arr;

  std::cout << "|" << std::setw(10) << n << "|";
  for (int i = 0; i < 3; i++) {
    arr = random_array(n, 1, n / 100);
    std::cout << std::setw(15) << std::fixed << std::setprecision(2)
              << performance_cell(selection_functions[i], arr, n) << "|";
    free(arr);
  }
  std::cout << std::endl;
}

int performance() {
  // performance
  std::cout << "Tabulating performances for the algorithms." << std::endl;
  std::cout << "Units are in milliseconds." << std::endl;
  std::cout << std::endl;
  std::cout << "|" << std::setw(10) << "n"
            << "|";
  for (int i = 0; i < 3; i++) {
    std::cout << std::setw(14) << "algorithm " << i + 1 << "|";
  }
  std::cout << std::endl;
  std::cout << "|:--------:|---------------|---------------|---------------|\n";
  float *time_taken;
  performance_row(1e5);
  performance_row(3e5);
  performance_row(1e6);
  performance_row(3e6);
  performance_row(1e7);
  performance_row(3e7);
  performance_row(1e8);
  performance_row(3e8);
  performance_row(1e9);
  performance_row(3e9);  // warning: overflow in implicit constant conversion
  performance_row(1e10); // warning: overflow in implicit constant conversion
  std::cout << std::endl;
  return 0;
}
} // namespace test_performance

int main() {
  setbuf(stdout, NULL); // no buffer to make things look better
  test_correctness::correctness();
  test_corner_cases::corner_cases();
  test_performance::performance();
}