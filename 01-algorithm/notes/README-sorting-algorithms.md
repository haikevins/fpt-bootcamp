# Sorting Algorithms

> Tài liệu học tập độc lập về **Sorting Algorithms**, trình bày các khái niệm nền tảng, cách phân loại, độ phức tạp, tính ổn định và mã nguồn minh họa bằng ngôn ngữ C.

---

## Mục lục

1. [Sorting là gì?](#1-sorting-là-gì)
2. [Tại sao cần sắp xếp?](#2-tại-sao-cần-sắp-xếp)
3. [Các tiêu chí đánh giá](#3-các-tiêu-chí-đánh-giá)
4. [Phân loại thuật toán sắp xếp](#4-phân-loại-thuật-toán-sắp-xếp)
5. [Bảng so sánh tổng quan](#5-bảng-so-sánh-tổng-quan)
6. [Selection Sort](#6-selection-sort)
7. [Bubble Sort](#7-bubble-sort)
8. [Insertion Sort](#8-insertion-sort)
9. [Merge Sort](#9-merge-sort)
10. [Quick Sort](#10-quick-sort)
11. [Heap Sort](#11-heap-sort)
12. [Cycle Sort](#12-cycle-sort)
13. [Counting Sort](#13-counting-sort)
14. [Radix Sort](#14-radix-sort)
15. [Bucket Sort](#15-bucket-sort)
16. [Pigeonhole Sort](#16-pigeonhole-sort)
17. [Hybrid Sorting Algorithms](#17-hybrid-sorting-algorithms)
18. [Thư viện sắp xếp trong C](#18-thư-viện-sắp-xếp-trong-c)
19. [Cách chọn thuật toán](#19-cách-chọn-thuật-toán)
20. [Ứng dụng của Sorting](#20-ứng-dụng-của-sorting)
21. [Sorting trong lập trình nhúng](#21-sorting-trong-lập-trình-nhúng)
22. [Các lỗi thường gặp](#22-các-lỗi-thường-gặp)
23. [Bài tập luyện tập](#23-bài-tập-luyện-tập)
24. [Tổng kết](#24-tổng-kết)
25. [Tài liệu tham khảo](#25-tài-liệu-tham-khảo)

---

## 1. Sorting là gì?

**Sorting** là quá trình sắp xếp lại các phần tử trong một tập dữ liệu theo một thứ tự xác định.

Ví dụ, mảng ban đầu:

```text
[10, 20, 5, 2]
```

Sắp xếp tăng dần:

```text
[2, 5, 10, 20]
```

Sắp xếp giảm dần:

```text
[20, 10, 5, 2]
```

Ngoài số nguyên, ta có thể sắp xếp:

- Số thực.
- Ký tự.
- Chuỗi.
- Cấu trúc dữ liệu.
- Record trong cơ sở dữ liệu.
- Đối tượng theo một trường khóa.
- Dữ liệu cảm biến theo timestamp.
- Message theo priority.

Ví dụ sắp xếp cấu trúc theo tuổi:

```c
typedef struct
{
    char name[32];
    int age;
} Person;
```

Khóa sắp xếp có thể là:

```text
age
```

---

## 2. Tại sao cần sắp xếp?

Dữ liệu đã sắp xếp giúp nhiều bài toán trở nên dễ hoặc nhanh hơn.

### Tìm kiếm

Binary Search yêu cầu dữ liệu đã sắp xếp:

```text
Search: O(log n)
```

### Loại bỏ phần tử trùng

Sau khi sắp xếp, các phần tử giống nhau thường nằm cạnh nhau.

```text
[1, 1, 2, 2, 2, 5, 8]
```

### Tìm median

Sau khi sắp xếp, median nằm tại vị trí giữa.

### Two Pointers

Nhiều bài toán tổng hai số, ba số hoặc khoảng cách nhỏ nhất được giải hiệu quả hơn sau khi sắp xếp.

### Merge dữ liệu

Hai mảng đã sắp xếp có thể được trộn trong thời gian tuyến tính.

### Lập lịch

Các tác vụ có thể được sắp theo:

- Thời gian bắt đầu.
- Deadline.
- Priority.
- Thời lượng.
- Thời điểm kết thúc.

---

## 3. Các tiêu chí đánh giá

## 3.1. Độ phức tạp thời gian

Một thuật toán thường được đánh giá ở ba trường hợp:

- Best case.
- Average case.
- Worst case.

Ví dụ:

```text
Insertion Sort
Best:    O(n)
Average: O(n²)
Worst:   O(n²)
```

---

## 3.2. Độ phức tạp bộ nhớ

### In-place

Thuật toán chỉ cần một lượng nhỏ bộ nhớ phụ:

```text
O(1)
```

Ví dụ:

- Selection Sort.
- Bubble Sort.
- Insertion Sort.
- Heap Sort.
- Quick Sort theo cách partition tại chỗ, không tính recursion stack.

### Out-of-place

Thuật toán cần thêm vùng nhớ tỷ lệ với số phần tử:

```text
O(n)
```

Ví dụ Merge Sort trên mảng thường cần một mảng phụ.

---

## 3.3. Stable Sort

Một thuật toán **stable** giữ nguyên thứ tự tương đối của các phần tử có cùng khóa.

Ví dụ:

```text
(A, 10), (B, 5), (C, 10)
```

Sau khi sắp theo giá trị tăng dần, một kết quả stable là:

```text
(B, 5), (A, 10), (C, 10)
```

`A` vẫn đứng trước `C` vì ban đầu hai phần tử cùng khóa `10` có thứ tự như vậy.

Stable Sort quan trọng khi sắp xếp nhiều khóa.

Ví dụ:

1. Sắp theo tên.
2. Sau đó sắp stable theo tuổi.

Các phần tử cùng tuổi vẫn giữ thứ tự theo tên.

---

## 3.4. Adaptive Sort

Thuật toán adaptive tận dụng việc dữ liệu đã được sắp một phần.

Ví dụ:

- Insertion Sort.
- TimSort.

Insertion Sort có thể đạt `O(n)` khi mảng gần như đã sắp xếp.

---

## 3.5. Online Sort

Thuật toán online có thể xử lý dữ liệu khi dữ liệu đến dần.

Insertion Sort có thể chèn phần tử mới vào vùng đã sắp xếp.

---

## 3.6. Comparison-Based Sort

Thuật toán xác định thứ tự bằng cách so sánh các phần tử:

```c
if (a[i] < a[j])
```

Ví dụ:

- Selection Sort.
- Bubble Sort.
- Insertion Sort.
- Merge Sort.
- Quick Sort.
- Heap Sort.

Các thuật toán sắp xếp dựa hoàn toàn vào phép so sánh có giới hạn lý thuyết trung bình và worst-case là:

```text
Ω(n log n)
```

đối với mô hình so sánh tổng quát.

---

## 3.7. Non-Comparison Sort

Thuật toán không chỉ dựa vào phép so sánh trực tiếp.

Ví dụ:

- Counting Sort.
- Radix Sort.
- Bucket Sort.
- Pigeonhole Sort.

Những thuật toán này tận dụng đặc điểm của khóa như:

- Phạm vi giá trị nhỏ.
- Chữ số.
- Phân bố dữ liệu.
- Khóa nguyên.

Chúng có thể nhanh hơn `O(n log n)` trong điều kiện phù hợp.

---

## 4. Phân loại thuật toán sắp xếp

## 4.1. Comparison-Based Algorithms

```text
Selection Sort
Bubble Sort
Insertion Sort
Merge Sort
Quick Sort
Heap Sort
Cycle Sort
3-Way Merge Sort
```

---

## 4.2. Non-Comparison-Based Algorithms

```text
Counting Sort
Radix Sort
Bucket Sort
Pigeonhole Sort
```

---

## 4.3. Hybrid Sorting Algorithms

Hybrid Sort kết hợp nhiều thuật toán để tận dụng ưu điểm của từng loại.

```text
IntroSort
TimSort
```

Ví dụ:

- TimSort kết hợp Merge Sort và Insertion Sort.
- IntroSort bắt đầu bằng Quick Sort, chuyển sang Heap Sort khi recursion quá sâu và thường dùng Insertion Sort cho đoạn nhỏ.

---

## 4.4. Internal và External Sorting

### Internal Sorting

Toàn bộ dữ liệu nằm trong RAM.

Ví dụ:

- Sắp xếp một mảng nhỏ.
- Sắp xếp danh sách record trong bộ nhớ.

### External Sorting

Dữ liệu lớn hơn RAM và phải sử dụng bộ nhớ ngoài.

Ví dụ:

- File rất lớn.
- Dữ liệu trên SSD.
- Database table lớn.

External Merge Sort là một kỹ thuật phổ biến:

1. Chia dữ liệu thành các phần vừa RAM.
2. Sắp xếp từng phần.
3. Ghi các phần đã sắp xếp ra file.
4. Merge nhiều phần theo thứ tự.

---

## 5. Bảng so sánh tổng quan

| Thuật toán | Best | Average | Worst | Bộ nhớ phụ | Stable | In-place |
|---|---:|---:|---:|---:|:---:|:---:|
| Selection Sort | `O(n²)` | `O(n²)` | `O(n²)` | `O(1)` | Không mặc định | Có |
| Bubble Sort | `O(n)` | `O(n²)` | `O(n²)` | `O(1)` | Có | Có |
| Insertion Sort | `O(n)` | `O(n²)` | `O(n²)` | `O(1)` | Có | Có |
| Merge Sort | `O(n log n)` | `O(n log n)` | `O(n log n)` | `O(n)` với mảng | Có | Không mặc định |
| Quick Sort | `O(n log n)` | `O(n log n)` | `O(n²)` | `O(log n)` trung bình | Không mặc định | Có |
| Heap Sort | `O(n log n)` | `O(n log n)` | `O(n log n)` | `O(1)` | Không | Có |
| Cycle Sort | `O(n²)` | `O(n²)` | `O(n²)` | `O(1)` | Không | Có |
| Counting Sort | `O(n + k)` | `O(n + k)` | `O(n + k)` | `O(n + k)` | Có nếu triển khai đúng | Không mặc định |
| Radix Sort | `O(d(n + b))` | `O(d(n + b))` | `O(d(n + b))` | `O(n + b)` | Phụ thuộc sort theo chữ số | Không |
| Bucket Sort | `O(n + k)` kỳ vọng | `O(n + k)` kỳ vọng | `O(n²)` | `O(n + k)` | Phụ thuộc triển khai | Không |
| Pigeonhole Sort | `O(n + range)` | `O(n + range)` | `O(n + range)` | `O(range)` | Có thể | Không |

Trong đó:

```text
n     = số phần tử
k     = phạm vi hoặc số bucket
d     = số chữ số
b     = cơ số
range = max - min + 1
```

---

## 6. Selection Sort

## 6.1. Ý tưởng

Chia mảng thành hai phần:

```text
Phần đã sắp xếp | Phần chưa sắp xếp
```

Mỗi vòng lặp:

1. Tìm phần tử nhỏ nhất trong vùng chưa sắp xếp.
2. Đổi chỗ nó với phần tử đầu vùng chưa sắp xếp.
3. Mở rộng vùng đã sắp xếp thêm một phần tử.

Ví dụ:

```text
[64, 25, 12, 22, 11]
```

Tìm nhỏ nhất là `11`, đổi với `64`:

```text
[11, 25, 12, 22, 64]
```

Tiếp tục với phần còn lại:

```text
[11 | 25, 12, 22, 64]
```

---

## 6.2. Mã C

```c
#include <stddef.h>

static void SwapInt(int *a, int *b)
{
    int temporary = *a;
    *a = *b;
    *b = temporary;
}

void SelectionSort(int array[], size_t size)
{
    size_t i;

    if ((array == NULL) || (size < 2U))
    {
        return;
    }

    for (i = 0U; i + 1U < size; i++)
    {
        size_t minimum_index = i;
        size_t j;

        for (j = i + 1U; j < size; j++)
        {
            if (array[j] < array[minimum_index])
            {
                minimum_index = j;
            }
        }

        if (minimum_index != i)
        {
            SwapInt(
                &array[i],
                &array[minimum_index]
            );
        }
    }
}
```

---

## 6.3. Đặc điểm

```text
So sánh: O(n²)
Swap:    O(n)
```

Selection Sort phù hợp khi:

- Mảng nhỏ.
- Chi phí ghi hoặc swap cao.
- Muốn giới hạn số lần swap.
- Cần code đơn giản.

Không phù hợp với dữ liệu lớn.

---

## 7. Bubble Sort

## 7.1. Ý tưởng

So sánh từng cặp phần tử kề nhau.

Nếu sai thứ tự thì đổi chỗ.

Sau mỗi lượt, phần tử lớn nhất chưa sắp xếp sẽ “nổi” về cuối.

Ví dụ:

```text
[5, 1, 4, 2, 8]
```

So sánh `5` và `1`:

```text
[1, 5, 4, 2, 8]
```

So sánh `5` và `4`:

```text
[1, 4, 5, 2, 8]
```

Sau một lượt:

```text
[1, 4, 2, 5, 8]
```

---

## 7.2. Mã C tối ưu dừng sớm

```c
#include <stdbool.h>
#include <stddef.h>

void BubbleSort(int array[], size_t size)
{
    size_t pass;

    if ((array == NULL) || (size < 2U))
    {
        return;
    }

    for (pass = 0U; pass + 1U < size; pass++)
    {
        bool swapped = false;
        size_t index;

        for (index = 0U;
             index + 1U < size - pass;
             index++)
        {
            if (array[index] > array[index + 1U])
            {
                int temporary = array[index];

                array[index] = array[index + 1U];
                array[index + 1U] = temporary;

                swapped = true;
            }
        }

        if (!swapped)
        {
            break;
        }
    }
}
```

---

## 7.3. Đặc điểm

Best case khi mảng đã sắp xếp:

```text
O(n)
```

Average và worst case:

```text
O(n²)
```

Bubble Sort stable nếu chỉ swap khi:

```c
left > right
```

Không swap khi hai giá trị bằng nhau.

---

## 8. Insertion Sort

## 8.1. Ý tưởng

Giống cách sắp bài trên tay.

Tại mỗi bước:

1. Lấy một phần tử làm `key`.
2. Dịch các phần tử lớn hơn `key` sang phải.
3. Chèn `key` vào đúng vị trí.

Ví dụ:

```text
[5, 2, 4, 6, 1, 3]
```

Phần đã sắp xếp ban đầu:

```text
[5]
```

Chèn `2`:

```text
[2, 5]
```

Chèn `4`:

```text
[2, 4, 5]
```

---

## 8.2. Mã C

```c
#include <stddef.h>

void InsertionSort(int array[], size_t size)
{
    size_t index;

    if ((array == NULL) || (size < 2U))
    {
        return;
    }

    for (index = 1U; index < size; index++)
    {
        int key = array[index];
        size_t position = index;

        while ((position > 0U) &&
               (array[position - 1U] > key))
        {
            array[position] =
                array[position - 1U];

            position--;
        }

        array[position] = key;
    }
}
```

---

## 8.3. Đặc điểm

Insertion Sort phù hợp với:

- Mảng nhỏ.
- Dữ liệu gần như đã sắp xếp.
- Hybrid Sort xử lý partition nhỏ.
- Dữ liệu đến dần.

Best case:

```text
O(n)
```

Worst case với mảng giảm dần:

```text
O(n²)
```

Thuật toán stable và in-place.

---

## 9. Merge Sort

## 9.1. Ý tưởng

Merge Sort dùng chiến lược:

```text
Divide and Conquer
```

Các bước:

1. Chia mảng thành hai nửa.
2. Sắp xếp từng nửa.
3. Merge hai nửa đã sắp xếp.

Ví dụ:

```text
[38, 27, 43, 3, 9, 82, 10]
```

Chia:

```text
[38, 27, 43, 3]    [9, 82, 10]
```

Tiếp tục chia cho đến khi mỗi phần có một phần tử.

Sau đó merge ngược lại.

---

## 9.2. Hàm merge

Hai đoạn:

```text
Left:  [3, 27, 38, 43]
Right: [9, 10, 82]
```

Merge:

```text
[3, 9, 10, 27, 38, 43, 82]
```

---

## 9.3. Mã C

```c
#include <stddef.h>
#include <stdlib.h>

static void Merge(
    int array[],
    int temporary[],
    size_t left,
    size_t middle,
    size_t right
)
{
    size_t left_index = left;
    size_t right_index = middle;
    size_t output_index = left;

    while ((left_index < middle) &&
           (right_index < right))
    {
        if (array[left_index] <= array[right_index])
        {
            temporary[output_index] =
                array[left_index];

            left_index++;
        }
        else
        {
            temporary[output_index] =
                array[right_index];

            right_index++;
        }

        output_index++;
    }

    while (left_index < middle)
    {
        temporary[output_index++] =
            array[left_index++];
    }

    while (right_index < right)
    {
        temporary[output_index++] =
            array[right_index++];
    }

    for (output_index = left;
         output_index < right;
         output_index++)
    {
        array[output_index] =
            temporary[output_index];
    }
}

static void MergeSortRange(
    int array[],
    int temporary[],
    size_t left,
    size_t right
)
{
    size_t middle;

    if ((right - left) < 2U)
    {
        return;
    }

    middle = left + (right - left) / 2U;

    MergeSortRange(
        array,
        temporary,
        left,
        middle
    );

    MergeSortRange(
        array,
        temporary,
        middle,
        right
    );

    Merge(
        array,
        temporary,
        left,
        middle,
        right
    );
}

bool MergeSort(int array[], size_t size)
{
    int *temporary;

    if (array == NULL)
    {
        return false;
    }

    if (size < 2U)
    {
        return true;
    }

    temporary =
        (int *)malloc(size * sizeof(int));

    if (temporary == NULL)
    {
        return false;
    }

    MergeSortRange(
        array,
        temporary,
        0U,
        size
    );

    free(temporary);

    return true;
}
```

Cần thêm:

```c
#include <stdbool.h>
```

---

## 9.4. Đặc điểm

Thời gian ở mọi trường hợp:

```text
O(n log n)
```

Merge Sort stable nếu khi hai giá trị bằng nhau, lấy phần tử bên trái trước:

```c
left_value <= right_value
```

Hạn chế chính trên mảng:

```text
Cần O(n) bộ nhớ phụ
```

Merge Sort rất phù hợp với Linked List vì có thể merge bằng cách thay đổi liên kết.

---

## 10. Quick Sort

## 10.1. Ý tưởng

Quick Sort cũng dùng Divide and Conquer.

Các bước:

1. Chọn một phần tử làm `pivot`.
2. Partition:
   - Phần nhỏ hơn hoặc bằng pivot.
   - Pivot.
   - Phần lớn hơn pivot.
3. Sắp xếp đệ quy hai phần.

---

## 10.2. Lomuto Partition

Chọn phần tử cuối làm pivot.

```text
[10, 80, 30, 90, 40, 50, 70]
                              ^
                              |
                            pivot
```

Sau partition:

```text
[10, 30, 40, 50] 70 [90, 80]
```

---

## 10.3. Mã C

```c
#include <stddef.h>

static void QuickSwap(int *left, int *right)
{
    int temporary = *left;
    *left = *right;
    *right = temporary;
}

static size_t Partition(
    int array[],
    size_t low,
    size_t high
)
{
    int pivot = array[high];
    size_t smaller_index = low;
    size_t scan_index;

    for (scan_index = low;
         scan_index < high;
         scan_index++)
    {
        if (array[scan_index] <= pivot)
        {
            QuickSwap(
                &array[smaller_index],
                &array[scan_index]
            );

            smaller_index++;
        }
    }

    QuickSwap(
        &array[smaller_index],
        &array[high]
    );

    return smaller_index;
}

static void QuickSortRange(
    int array[],
    size_t low,
    size_t high
)
{
    size_t pivot_index;

    if (low >= high)
    {
        return;
    }

    pivot_index = Partition(
        array,
        low,
        high
    );

    if (pivot_index > low)
    {
        QuickSortRange(
            array,
            low,
            pivot_index - 1U
        );
    }

    QuickSortRange(
        array,
        pivot_index + 1U,
        high
    );
}

void QuickSort(int array[], size_t size)
{
    if ((array == NULL) || (size < 2U))
    {
        return;
    }

    QuickSortRange(
        array,
        0U,
        size - 1U
    );
}
```

---

## 10.4. Chọn pivot

Các cách chọn:

- Phần tử đầu.
- Phần tử cuối.
- Phần tử giữa.
- Random pivot.
- Median-of-three.

Pivot kém có thể tạo partition rất lệch:

```text
0 phần tử | pivot | n - 1 phần tử
```

Khi đó:

```text
Worst case = O(n²)
Recursion depth = O(n)
```

Random pivot hoặc median-of-three giúp giảm khả năng gặp trường hợp xấu trong thực tế.

---

## 10.5. Đặc điểm

Average case:

```text
O(n log n)
```

Worst case:

```text
O(n²)
```

Quick Sort thường nhanh trong thực tế do:

- Cache locality tốt.
- In-place.
- Constant factor nhỏ.

Quick Sort mặc định không stable.

---

## 11. Heap Sort

## 11.1. Ý tưởng

Heap Sort sử dụng Binary Heap.

Để sắp tăng dần:

1. Xây dựng Max Heap.
2. Phần tử lớn nhất nằm ở root.
3. Đổi root với phần tử cuối.
4. Giảm kích thước heap.
5. Heapify lại.
6. Lặp cho đến khi hoàn tất.

---

## 11.2. Quan hệ chỉ mục

Với Node tại `index`:

```c
left  = 2 * index + 1
right = 2 * index + 2
```

Parent:

```c
parent = (index - 1) / 2
```

---

## 11.3. Mã C

```c
#include <stddef.h>

static void HeapSwap(int *left, int *right)
{
    int temporary = *left;
    *left = *right;
    *right = temporary;
}

static void Heapify(
    int array[],
    size_t heap_size,
    size_t root
)
{
    while (true)
    {
        size_t largest = root;
        size_t left = 2U * root + 1U;
        size_t right = 2U * root + 2U;

        if ((left < heap_size) &&
            (array[left] > array[largest]))
        {
            largest = left;
        }

        if ((right < heap_size) &&
            (array[right] > array[largest]))
        {
            largest = right;
        }

        if (largest == root)
        {
            break;
        }

        HeapSwap(
            &array[root],
            &array[largest]
        );

        root = largest;
    }
}

void HeapSort(int array[], size_t size)
{
    size_t index;

    if ((array == NULL) || (size < 2U))
    {
        return;
    }

    for (index = size / 2U;
         index > 0U;
         index--)
    {
        Heapify(
            array,
            size,
            index - 1U
        );
    }

    for (index = size;
         index > 1U;
         index--)
    {
        HeapSwap(
            &array[0],
            &array[index - 1U]
        );

        Heapify(
            array,
            index - 1U,
            0U
        );
    }
}
```

Cần thêm:

```c
#include <stdbool.h>
```

---

## 11.4. Đặc điểm

Mọi trường hợp:

```text
O(n log n)
```

Bộ nhớ phụ:

```text
O(1)
```

Heap Sort không stable và locality thường kém hơn Quick Sort.

Phù hợp khi cần:

- Worst-case `O(n log n)`.
- Không dùng bộ nhớ phụ lớn.
- Không muốn recursion sâu.

---

## 12. Cycle Sort

## 12.1. Ý tưởng

Cycle Sort đặt mỗi phần tử trực tiếp về vị trí cuối cùng của nó.

Thuật toán cố gắng giảm số lần ghi dữ liệu.

Điều này hữu ích khi thao tác ghi đắt hơn thao tác đọc, ví dụ một số loại bộ nhớ không bay hơi.

---

## 12.2. Đặc điểm

```text
Time:  O(n²)
Space: O(1)
```

Cycle Sort thường có số lần ghi tối thiểu hoặc gần tối thiểu trong nhóm thuật toán so sánh in-place.

Hạn chế:

- Code phức tạp hơn Selection Sort.
- Không stable.
- Không phù hợp với dữ liệu lớn nếu CPU time quan trọng.
- Cần xử lý cẩn thận phần tử trùng.

---

## 13. Counting Sort

## 13.1. Ý tưởng

Counting Sort đếm số lần xuất hiện của từng giá trị.

Phù hợp khi:

- Dữ liệu là số nguyên.
- Phạm vi giá trị không quá lớn.

Ví dụ:

```text
Input:
[4, 2, 2, 8, 3, 3, 1]
```

Count:

```text
Value: 0 1 2 3 4 5 6 7 8
Count: 0 1 2 2 1 0 0 0 1
```

Kết quả:

```text
[1, 2, 2, 3, 3, 4, 8]
```

---

## 13.2. Mã C hỗ trợ số âm

```c
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

bool CountingSort(int array[], size_t size)
{
    int minimum;
    int maximum;
    size_t range;
    size_t *count;
    size_t index;
    size_t output_index = 0U;

    if (array == NULL)
    {
        return false;
    }

    if (size < 2U)
    {
        return true;
    }

    minimum = array[0];
    maximum = array[0];

    for (index = 1U; index < size; index++)
    {
        if (array[index] < minimum)
        {
            minimum = array[index];
        }

        if (array[index] > maximum)
        {
            maximum = array[index];
        }
    }

    if ((long long)maximum -
        (long long)minimum >
        (long long)SIZE_MAX - 1LL)
    {
        return false;
    }

    range =
        (size_t)((long long)maximum -
                 (long long)minimum + 1LL);

    count = (size_t *)calloc(
        range,
        sizeof(size_t)
    );

    if (count == NULL)
    {
        return false;
    }

    for (index = 0U; index < size; index++)
    {
        size_t count_index =
            (size_t)((long long)array[index] -
                     (long long)minimum);

        count[count_index]++;
    }

    for (index = 0U; index < range; index++)
    {
        while (count[index] > 0U)
        {
            array[output_index++] =
                minimum + (int)index;

            count[index]--;
        }
    }

    free(count);

    return true;
}
```

---

## 13.3. Đặc điểm

```text
Time:  O(n + k)
Space: O(k)
```

Trong đó:

```text
k = maximum - minimum + 1
```

Không nên dùng khi:

```text
n nhỏ nhưng phạm vi giá trị cực lớn
```

Ví dụ:

```text
[1, 1_000_000_000]
```

Mảng count sẽ quá lớn.

Bản code đơn giản phía trên không stable vì ghi lại giá trị trực tiếp. Counting Sort stable cần prefix sum và output array.

---

## 14. Radix Sort

## 14.1. Ý tưởng

Radix Sort sắp xếp theo từng chữ số.

Với LSD Radix Sort:

1. Sắp theo hàng đơn vị.
2. Sắp theo hàng chục.
3. Sắp theo hàng trăm.
4. Tiếp tục đến chữ số lớn nhất.

Sort theo từng chữ số phải stable.

Ví dụ:

```text
[170, 45, 75, 90, 802, 24, 2, 66]
```

Sau hàng đơn vị, hàng chục và hàng trăm, mảng trở thành:

```text
[2, 24, 45, 66, 75, 90, 170, 802]
```

---

## 14.2. Mã C cho số không âm

```c
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static bool CountingSortByDigit(
    int array[],
    size_t size,
    int exponent
)
{
    size_t count[10] = {0U};
    int *output;
    size_t index;

    output = (int *)malloc(size * sizeof(int));

    if (output == NULL)
    {
        return false;
    }

    for (index = 0U; index < size; index++)
    {
        int digit =
            (array[index] / exponent) % 10;

        count[digit]++;
    }

    for (index = 1U; index < 10U; index++)
    {
        count[index] += count[index - 1U];
    }

    for (index = size; index > 0U; index--)
    {
        int value = array[index - 1U];
        int digit = (value / exponent) % 10;

        output[count[digit] - 1U] = value;
        count[digit]--;
    }

    for (index = 0U; index < size; index++)
    {
        array[index] = output[index];
    }

    free(output);

    return true;
}

bool RadixSort(int array[], size_t size)
{
    int maximum;
    int exponent;
    size_t index;

    if (array == NULL)
    {
        return false;
    }

    if (size < 2U)
    {
        return true;
    }

    maximum = array[0];

    for (index = 0U; index < size; index++)
    {
        if (array[index] < 0)
        {
            return false;
        }

        if (array[index] > maximum)
        {
            maximum = array[index];
        }
    }

    for (exponent = 1;
         maximum / exponent > 0;
         exponent *= 10)
    {
        if (!CountingSortByDigit(
                array,
                size,
                exponent))
        {
            return false;
        }

        if (exponent > maximum / 10)
        {
            break;
        }
    }

    return true;
}
```

---

## 14.3. Đặc điểm

```text
Time: O(d(n + b))
```

Trong đó:

- `d`: số chữ số.
- `b`: cơ số, ví dụ `10`.

Radix Sort phù hợp với:

- Số nguyên.
- ID có số chữ số giới hạn.
- Chuỗi có độ dài cố định.
- Khóa được chia thành nhiều digit hoặc byte.

---

## 15. Bucket Sort

## 15.1. Ý tưởng

Chia dữ liệu vào nhiều bucket.

```text
Input
  |
  +--> Bucket 0
  +--> Bucket 1
  +--> Bucket 2
  +--> ...
```

Sau đó:

1. Sắp xếp từng bucket.
2. Ghép các bucket theo thứ tự.

Bucket Sort hiệu quả khi dữ liệu phân bố khá đều.

Ví dụ số thực trong khoảng `[0, 1)`:

```text
0.12, 0.91, 0.35, 0.27, 0.64
```

Có thể chia theo khoảng:

```text
Bucket 0: [0.0, 0.2)
Bucket 1: [0.2, 0.4)
Bucket 2: [0.4, 0.6)
Bucket 3: [0.6, 0.8)
Bucket 4: [0.8, 1.0)
```

---

## 15.2. Đặc điểm

Average case với phân bố phù hợp:

```text
O(n + k)
```

Worst case khi tất cả phần tử rơi vào một bucket:

```text
O(n²)
```

Hiệu quả phụ thuộc:

- Cách chọn số bucket.
- Hàm ánh xạ.
- Phân bố dữ liệu.
- Thuật toán dùng trong từng bucket.

---

## 16. Pigeonhole Sort

## 16.1. Ý tưởng

Pigeonhole Sort tạo một “hole” cho mỗi giá trị trong phạm vi.

```text
Hole index = value - minimum
```

Đếm số phần tử ở mỗi hole rồi ghi lại theo thứ tự.

Nó gần với Counting Sort.

Phù hợp khi:

```text
range gần với n
```

---

## 16.2. Độ phức tạp

```text
Time:  O(n + range)
Space: O(range)
```

Không phù hợp khi phạm vi dữ liệu rất lớn so với số phần tử.

---

## 17. Hybrid Sorting Algorithms

## 17.1. IntroSort

IntroSort thường:

1. Bắt đầu bằng Quick Sort.
2. Theo dõi độ sâu recursion.
3. Nếu recursion quá sâu, chuyển sang Heap Sort.
4. Dùng Insertion Sort cho partition nhỏ.

Mục tiêu:

- Tốc độ thực tế tốt của Quick Sort.
- Worst-case `O(n log n)` của Heap Sort.
- Hiệu quả trên đoạn nhỏ của Insertion Sort.

---

## 17.2. TimSort

TimSort tận dụng các đoạn dữ liệu đã có thứ tự, gọi là `run`.

Khái quát:

1. Phát hiện các run tăng hoặc giảm.
2. Đảo run giảm nếu cần.
3. Mở rộng run nhỏ bằng Insertion Sort.
4. Merge các run theo chiến lược nhất định.

TimSort:

- Stable.
- Adaptive.
- Hoạt động tốt với dữ liệu thực tế đã được sắp một phần.

---

## 17.3. 3-Way Partition Quick Sort

Khi mảng có nhiều phần tử trùng, có thể chia thành ba vùng:

```text
< pivot | == pivot | > pivot
```

Điều này tránh xử lý lặp lại một vùng lớn chứa các phần tử bằng pivot.

---

## 18. Thư viện sắp xếp trong C

C Standard Library cung cấp:

```c
qsort()
```

Header:

```c
#include <stdlib.h>
```

Prototype:

```c
void qsort(
    void *base,
    size_t number_of_elements,
    size_t element_size,
    int (*compare)(const void *, const void *)
);
```

---

## 18.1. Sắp xếp số nguyên tăng dần

Không nên viết:

```c
return *(int *)left - *(int *)right;
```

vì phép trừ có thể overflow.

Cách an toàn hơn:

```c
#include <stdlib.h>

static int CompareIntAscending(
    const void *left,
    const void *right
)
{
    int left_value = *(const int *)left;
    int right_value = *(const int *)right;

    return (left_value > right_value) -
           (left_value < right_value);
}

int main(void)
{
    int array[] = {5, 2, 9, 1, 5, 6};
    size_t size = sizeof(array) / sizeof(array[0]);

    qsort(
        array,
        size,
        sizeof(array[0]),
        CompareIntAscending
    );

    return 0;
}
```

---

## 18.2. Sắp xếp giảm dần

```c
static int CompareIntDescending(
    const void *left,
    const void *right
)
{
    int left_value = *(const int *)left;
    int right_value = *(const int *)right;

    return (right_value > left_value) -
           (right_value < left_value);
}
```

---

## 18.3. Sắp xếp cấu trúc

```c
typedef struct
{
    char name[32];
    int score;
} Student;
```

Comparator theo score giảm dần:

```c
static int CompareStudentByScore(
    const void *left,
    const void *right
)
{
    const Student *student_left =
        (const Student *)left;

    const Student *student_right =
        (const Student *)right;

    return (student_right->score >
            student_left->score) -
           (student_right->score <
            student_left->score);
}
```

---

## 18.4. Lưu ý về `qsort()`

- Không giả định thuật toán nội bộ cụ thể.
- C standard không bắt buộc `qsort()` stable.
- Comparator phải nhất quán.
- Comparator trả:
  - Số âm nếu phần tử trái đứng trước.
  - `0` nếu hai khóa bằng nhau.
  - Số dương nếu phần tử trái đứng sau.
- Hàm generic có thể chậm hơn code chuyên biệt do gọi callback nhiều lần.
- Trong embedded, cần kiểm tra implementation của C library về Stack, code size và worst-case behavior.

---

## 19. Cách chọn thuật toán

## 19.1. Mảng rất nhỏ

Ưu tiên:

```text
Insertion Sort
Selection Sort
```

Insertion Sort thường tốt nếu dữ liệu gần như đã sắp xếp.

Selection Sort phù hợp nếu muốn giảm số lần ghi.

---

## 19.2. Dữ liệu gần như đã sắp xếp

Ưu tiên:

```text
Insertion Sort
TimSort
```

---

## 19.3. Cần worst-case `O(n log n)`

Ưu tiên:

```text
Merge Sort
Heap Sort
IntroSort
```

---

## 19.4. Cần stable

Ưu tiên:

```text
Merge Sort
Insertion Sort
Bubble Sort
Counting Sort stable
Radix Sort với digit sort stable
TimSort
```

---

## 19.5. RAM rất hạn chế

Ưu tiên:

```text
Heap Sort
Quick Sort in-place
Insertion Sort
Selection Sort
```

Cần kiểm soát recursion stack nếu dùng Quick Sort.

---

## 19.6. Phạm vi số nguyên nhỏ

Ưu tiên:

```text
Counting Sort
Pigeonhole Sort
```

---

## 19.7. Số nguyên có số chữ số giới hạn

Ưu tiên:

```text
Radix Sort
```

---

## 19.8. Nhiều phần tử trùng

Có thể dùng:

```text
3-Way Quick Sort
Counting Sort
```

---

## 19.9. Linked List

Ưu tiên:

```text
Merge Sort
```

Merge không cần truy cập ngẫu nhiên và có thể thay đổi liên kết Node.

---

## 19.10. Dữ liệu không vừa RAM

Ưu tiên:

```text
External Merge Sort
```

---

## 20. Ứng dụng của Sorting

Sorting được dùng trong:

- Binary Search.
- Tìm median và percentile.
- Loại bỏ phần tử trùng.
- Merge nhiều nguồn dữ liệu.
- Database `ORDER BY`.
- Indexing.
- Data visualization.
- Ranking.
- Scheduling.
- Kruskal Minimum Spanning Tree.
- Closest Pair.
- Interval problems.
- Two Sum và Three Sum.
- Anagram grouping.
- Event timeline.
- Log processing.
- Compression.
- Computational Geometry.

---

## 20.1. Sort rồi Search

```text
Unsorted data
      |
      v
Sort: O(n log n)
      |
      v
Many binary searches: O(log n) each
```

Nếu chỉ tìm một lần, Linear Search có thể đơn giản hơn.

Nếu tìm nhiều lần, chi phí sắp xếp ban đầu có thể được bù lại.

---

## 20.2. Merge Intervals

Các interval thường được sắp theo thời điểm bắt đầu:

```text
[1, 3], [2, 6], [8, 10]
```

Sau đó duyệt một lần để merge.

---

## 20.3. Kruskal Algorithm

Các cạnh được sắp theo trọng số tăng dần.

Sau đó chọn cạnh nhỏ nhất không tạo cycle.

---

## 21. Sorting trong lập trình nhúng

## 21.1. Số lượng phần tử thường nhỏ

Trong firmware, nhiều bài toán chỉ có:

- 8 sensor samples.
- 16 priorities.
- 32 records.
- Một số nhỏ timestamp.

Với kích thước nhỏ, thuật toán đơn giản như Insertion Sort có thể tốt hơn một thuật toán phức tạp.

---

## 21.2. RAM hạn chế

Merge Sort trên mảng cần bộ nhớ phụ.

Nếu:

```text
n = 1000
sizeof(int) = 4 byte
```

Mảng phụ cần khoảng:

```text
4000 byte
```

Trên MCU nhỏ, đây có thể là chi phí lớn.

---

## 21.3. Recursion

Merge Sort và Quick Sort thường được viết đệ quy.

Trong embedded:

- Stack có giới hạn nhỏ.
- Worst-case recursion có thể gây Stack Overflow.
- Coding standard có thể cấm recursion.
- Worst-case Stack usage khó phân tích.

Có thể dùng:

- Phiên bản iterative.
- Heap Sort.
- Insertion Sort.
- Giới hạn recursion depth.
- Manual Stack cố định.

---

## 21.4. Tính xác định

Trong hard real-time, cần quan tâm worst-case:

```text
Quick Sort worst case: O(n²)
Heap Sort worst case:  O(n log n)
Merge Sort worst case: O(n log n)
```

Quick Sort nhanh trung bình nhưng có biến thiên thời gian lớn hơn nếu không kiểm soát pivot và recursion.

---

## 21.5. Stable Sort trong Event và Message

Giả sử message được sắp theo priority.

Hai message cùng priority có thể cần giữ thứ tự thời gian đến.

Khi đó cần Stable Sort hoặc dùng thêm sequence number:

```c
typedef struct
{
    uint8_t priority;
    uint32_t sequence;
} Message;
```

Comparator:

1. Priority cao trước.
2. Nếu bằng priority, sequence nhỏ trước.

---

## 21.6. Flash và EEPROM

Một số bộ nhớ có số chu kỳ ghi giới hạn.

Thuật toán có nhiều swap làm tăng số lần ghi.

Có thể cân nhắc:

- Selection Sort.
- Cycle Sort.
- Sắp xếp trong RAM rồi ghi kết quả một lần.
- Lưu index thay vì di chuyển record lớn.
- Sắp xếp mảng con trỏ.

---

## 21.7. Sắp xếp con trỏ thay vì object lớn

Nếu record lớn:

```c
typedef struct
{
    uint8_t payload[256];
    uint32_t timestamp;
} Record;
```

Swap toàn bộ `Record` tốn nhiều copy.

Có thể tạo:

```c
Record *order[COUNT];
```

Sau đó sắp xếp mảng con trỏ theo `timestamp`.

Ưu điểm:

- Mỗi swap chỉ đổi con trỏ.
- Giảm copy.
- Dữ liệu gốc không bị di chuyển.

---

## 21.8. DMA và vùng nhớ chia sẻ

Không được sắp xếp buffer khi DMA hoặc peripheral đang ghi vào cùng vùng nhớ.

Cần:

- Double Buffer.
- Ownership rõ ràng.
- Critical section.
- Dừng DMA trước khi thay đổi.
- Sắp xếp snapshot riêng.

---

## 22. Các lỗi thường gặp

## 22.1. Sai giới hạn vòng lặp

Sai:

```c
for (i = 0; i < size; i++)
{
    if (array[i] > array[i + 1])
```

Khi:

```text
i == size - 1
```

`array[i + 1]` vượt mảng.

Đúng:

```c
i + 1 < size
```

---

## 22.2. Underflow với `size_t`

Sai:

```c
for (size_t i = size - 1; i >= 0; i--)
```

`size_t` là unsigned nên điều kiện `i >= 0` luôn đúng.

Đúng:

```c
for (size_t i = size; i > 0U; i--)
{
    size_t index = i - 1U;
}
```

---

## 22.3. Tính middle có nguy cơ overflow

Thay vì:

```c
middle = (left + right) / 2;
```

dùng:

```c
middle = left + (right - left) / 2;
```

---

## 22.4. Comparator bằng phép trừ

Sai:

```c
return left_value - right_value;
```

Có thể signed integer overflow.

Đúng:

```c
return (left_value > right_value) -
       (left_value < right_value);
```

---

## 22.5. Mất tính stable

Trong merge, nếu hai khóa bằng nhau mà lấy phần tử bên phải trước, thứ tự ban đầu có thể bị đảo.

Stable merge:

```c
if (left_value <= right_value)
{
    take_left();
}
```

---

## 22.6. Counting Sort với range quá lớn

Phải kiểm tra:

```text
range = maximum - minimum + 1
```

Không nên cấp phát count array nếu range quá lớn so với `n` hoặc RAM.

---

## 22.7. Quick Sort Stack Overflow

Pivot xấu liên tục có thể tạo recursion sâu `O(n)`.

Giải pháp:

- Random pivot.
- Median-of-three.
- Sort partition nhỏ trước.
- Tail recursion elimination.
- Giới hạn recursion.
- IntroSort.

---

## 22.8. Không xử lý phần tử trùng

Một partition không phù hợp có thể hoạt động kém khi nhiều giá trị bằng nhau.

Có thể dùng 3-way partition.

---

## 22.9. Benchmark không công bằng

Khi so sánh thuật toán:

- Dùng cùng dữ liệu đầu vào.
- Copy lại mảng trước mỗi lần chạy.
- Kiểm tra kết quả đã sắp xếp.
- Chạy nhiều lần.
- Tách thời gian tạo dữ liệu.
- Test nhiều phân bố.
- Bật cùng mức compiler optimization.
- Không benchmark cùng lúc với I/O hoặc log.

---

## 22.10. Chọn thuật toán chỉ theo Big-O

Hai thuật toán cùng `O(n log n)` vẫn có thể khác nhau về:

- Cache locality.
- Constant factor.
- Memory allocation.
- Branch prediction.
- Stable requirement.
- Worst-case latency.
- Code size.
- Kích thước đầu vào.

---

## 23. Bài tập luyện tập

## Dễ

1. Kiểm tra mảng đã sắp xếp chưa.
2. Sắp xếp mảng chỉ chứa hai loại giá trị.
3. Sắp xếp chuỗi ký tự.
4. Sắp xếp ma trận.
5. Sắp xếp Linked List.
6. Sắp xếp theo dạng sóng.
7. Merge hai mảng đã sắp xếp.
8. Tìm giao của hai mảng đã sắp xếp.
9. Tìm hợp của hai mảng đã sắp xếp.
10. Two Sum trên mảng đã sắp xếp.
11. Sắp chuỗi theo độ dài.
12. Kiểm tra hai interval có giao nhau không.

---

## Trung bình

1. Sắp xếp theo tần suất.
2. Merge Overlapping Intervals.
3. Minimum Platforms.
4. Chocolate Distribution.
5. Three-Way Partitioning.
6. Sắp xếp mảng `0`, `1`, `2`.
7. Đếm inversion.
8. Tìm phần tử nhỏ thứ `k`.
9. Tìm `k` phần tử nhỏ nhất.
10. Three Sum.
11. Three Sum Closest.
12. Merge `k` sorted arrays.
13. Tìm đoạn con ngắn nhất cần sắp xếp.
14. Sắp xếp mảng gần như đã sắp.
15. Relative Sorting.
16. Minimum Increments to Make Unique.

---

## Khó

1. Merge hai mảng đã sắp xếp không dùng extra space.
2. Top K Frequent Elements.
3. Three Sum Distinct Triplets.
4. Four Sum.
5. Surpasser Count.
6. Minimum swaps để biến đổi mảng.
7. Closest Pair of Points.
8. External Merge Sort.
9. Parallel Merge Sort.
10. IntroSort.
11. TimSort đơn giản.
12. Stable Quick Sort.
13. Sort record lớn bằng mảng index.
14. Lock-free parallel sorting pipeline.
15. Sắp xếp dữ liệu lớn không vừa RAM.

---

## 24. Tổng kết

Sorting là quá trình sắp xếp phần tử theo một thứ tự xác định.

Các nhóm chính:

```text
Comparison-Based
Non-Comparison-Based
Hybrid Sorting
```

Thuật toán cơ bản:

```text
Selection Sort
Bubble Sort
Insertion Sort
Merge Sort
Quick Sort
Heap Sort
```

Thuật toán không dựa hoàn toàn vào so sánh:

```text
Counting Sort
Radix Sort
Bucket Sort
Pigeonhole Sort
```

Không có một thuật toán tốt nhất cho mọi trường hợp.

Cần chọn dựa trên:

1. Số phần tử.
2. Phân bố dữ liệu.
3. Phạm vi khóa.
4. RAM khả dụng.
5. Có cần stable không.
6. Có yêu cầu worst-case không.
7. Dữ liệu có gần như đã sắp không.
8. Dữ liệu có nhiều phần tử trùng không.
9. Có thể sử dụng recursion không.
10. Chi phí đọc, ghi và swap.

Gợi ý nhanh:

```text
Mảng nhỏ                    → Insertion Sort
Gần như đã sắp              → Insertion Sort / TimSort
Stable O(n log n)           → Merge Sort
Average nhanh, in-place     → Quick Sort
Worst O(n log n), O(1) RAM  → Heap Sort
Range số nguyên nhỏ         → Counting Sort
Khóa có số chữ số giới hạn  → Radix Sort
Dữ liệu không vừa RAM       → External Merge Sort
```

---

## 25. Tài liệu tham khảo

README này được biên soạn và diễn đạt lại độc lập dựa trên phạm vi chủ đề được trình bày trong:

- GeeksforGeeks — **Sorting Algorithms**
- GeeksforGeeks — **Introduction to Sorting**
- GeeksforGeeks — **Applications of Sorting**
- GeeksforGeeks — các bài riêng về Selection Sort, Bubble Sort, Insertion Sort, Merge Sort, Quick Sort, Heap Sort, Counting Sort, Radix Sort và Bucket Sort.

Trang chủ đề:

<https://www.geeksforgeeks.org/dsa/sorting-algorithms/>

README không phải là bản sao nguyên văn của nội dung nguồn.
