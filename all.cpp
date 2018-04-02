#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <random>

#include <ctime> // for performance timing

const int SEED = 5000; // seed for random array

namespace alg3 {
void quickSort(int arr[], int left, int right);
}

/********************UTILITY FUNCTIONS****************/
struct partition_result {
  // represents partition result;
  // arr[0:a] are less than pivot
  // arr[a:b] are equal to pivot
  // arr[b:n] are greater than pivot
  int a;
  int b;
};

void swap(int *a, int *b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

int* random_array(int n, int mini, int maxi, int seed = SEED){
  std::mt19937 generator(seed);
  std::uniform_int_distribution<int> distribution(mini, maxi);
  int * arr = (int*)malloc(n * sizeof(int));
  int i;
  for (i = 0; i < n; i++) {
    arr[i] = distribution(generator);
  }
  return arr;
}

struct partition_result partition(int arr[], int left, int right) {
  // do three-way partition:
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
int select(int arr[], int left,
           int right); // need function prototype for recursion
int select_rec(int arr[], int left, int right,
               int k); // need function prototype for recursion

int get_pivot(int arr[], int left, int right) {
  // returns pivot of arr[left:right], using medians-of-medians method
  // detailed in https://www.ics.uci.edu/~eppstein/161/960130.html
  int n = right - left;
  if (n <= 10) {
    return alg1::select_rec(arr, left, right, (right + left) / 2);
  }

  int n_groups =
      (n % 5 == 0) ? (n / 5) : (n / 5 + 1); // equivalent to ceil(n/5);

  int * group_medians = (int*)malloc(n_groups * sizeof(int));

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

/*********************END QUICKSORT***************************/

/** begin code for testing**/
void correctness_1(int (*select)(int *, int, int)) {
  // takes pointer *select as argument;
  // tests select(arr, n, n/2) and prints output to screen
  // if nothing goes wrong, this should be the median
  // begin testcase 1
  const int n = 20;
  int arr[] = {7,  2, 4,  6, 9, 11, 2,  6, 10, 6,
               15, 6, 14, 2, 7, 5,  13, 9, 12, 15};
  std::cout << "\tCase 1: length-20 array given in specification" << std::endl;
  const int median_1 = select(arr, n, n / 2);
  std::cout << "\t\tMedian: " << median_1 << std::endl << std::endl;
}

void correctness_2(int (*select)(int *, int, int)) {
  const int n = 10000;
  int *arr;
  arr = random_array(n, 1, n/100);
  std::cout << "\tCase 2: " << n << " random ints" << std::endl;
  const int median_2 = select(arr, n, n / 2);
  std::cout << "\t\tMedian: " << median_2 << std::endl << std::endl;
  free(arr);
}
/** end code for testing **/

void test_partition() {
  int arr1[] = {4, 6, 14, 12};
  int n1 = 4;
  std::cout << "\tTesting partition on array\n\t";
  printArray(arr1, n1);
  struct partition_result ret1 = partition(arr1, 0, n1);
  std::cout << "\ta: " << ret1.a << std::endl;
  std::cout << "\tb: " << ret1.b << std::endl;
  printArray(arr1, n1);
}

/** driver function **/
void test_correctness(void) {
  std::cout << "Testing algorithm 1:randomized quickselect for correctness\n\n";
  correctness_1(alg1::select);
  correctness_2(alg1::select);
  /**/
  std::cout << "Testing algorithm 2: deterministic quickselect\n\n";
  correctness_1(alg2::select);
  correctness_2(alg2::select);
  /**/
  std::cout << "Testing algorithm 3:quicksort-select.\n\n";
  correctness_1(alg3::select);
  correctness_2(alg3::select);
}

typedef int (*SelectFunctionPointer)(int *, int, int);

float performance(SelectFunctionPointer select, int* arr, int n){
    std::clock_t t0 = std::clock();
    select(arr, n, n/2);
    return (std::clock() - t0) / (float)(CLOCKS_PER_SEC / 1000);
    return 0;
}

float* test_performance(int n){
  int * arr;

  SelectFunctionPointer funs[] = {alg1::select, alg2::select, alg3::select};

  std::cout << std::setw(10) << n;
  for(int i=0; i<3; i++){
    arr = random_array(n, 1, n/100);
    std::cout << std::setw(15) << performance(funs[i], arr, n);
    free(arr);
  }
  std::cout << std::endl;
  
}

int main(){
  test_correctness();
  // performance
  std::cout << "Tabulating performances for the algorithms." << std::endl;
  std::cout << "Units are in milliseconds." << std::endl;
  std::cout << std::setw(10) << "n";
  int i;
  for(i = 0; i < 3; i ++){
    std::cout << std::setw(15) << "algorithm " << i;
  }
  std::cout << std::endl;
  float* time_taken;
  test_performance(  1e5);
  test_performance(  3e5);
  test_performance(  1e6);
  test_performance(  3e6);
  test_performance(  1e7);
  test_performance(1.5e7);

  return 0;
}