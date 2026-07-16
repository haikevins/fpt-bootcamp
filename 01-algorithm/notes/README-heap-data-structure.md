# Heap Data Structure

> Tài liệu học tập độc lập về **Heap Data Structure**, trình bày Binary Heap, Min Heap, Max Heap, các thao tác cơ bản, Priority Queue, Heap Sort và mã nguồn C hoàn chỉnh theo hướng phù hợp với lập trình nhúng.

---

## Mục lục

1. [Heap là gì?](#1-heap-là-gì)
2. [Không nhầm Heap Data Structure với Heap Memory](#2-không-nhầm-heap-data-structure-với-heap-memory)
3. [Complete Binary Tree](#3-complete-binary-tree)
4. [Heap Property](#4-heap-property)
5. [Min Heap và Max Heap](#5-min-heap-và-max-heap)
6. [Biểu diễn Binary Heap bằng mảng](#6-biểu-diễn-binary-heap-bằng-mảng)
7. [Chiều cao của Heap](#7-chiều-cao-của-heap)
8. [Các thao tác cơ bản](#8-các-thao-tác-cơ-bản)
9. [Sift Up](#9-sift-up)
10. [Sift Down và Heapify](#10-sift-down-và-heapify)
11. [Cài đặt Min Heap bằng C](#11-cài-đặt-min-heap-bằng-c)
12. [Ví dụ chạy chương trình](#12-ví-dụ-chạy-chương-trình)
13. [Build Heap từ một mảng](#13-build-heap-từ-một-mảng)
14. [Heap Sort](#14-heap-sort)
15. [Priority Queue](#15-priority-queue)
16. [Độ phức tạp](#16-độ-phức-tạp)
17. [Kiểm tra một mảng có phải Heap](#17-kiểm-tra-một-mảng-có-phải-heap)
18. [Tìm phần tử lớn hoặc nhỏ thứ K](#18-tìm-phần-tử-lớn-hoặc-nhỏ-thứ-k)
19. [Các ứng dụng của Heap](#19-các-ứng-dụng-của-heap)
20. [Các loại Heap khác](#20-các-loại-heap-khác)
21. [Heap trong lập trình nhúng](#21-heap-trong-lập-trình-nhúng)
22. [Ưu điểm và hạn chế](#22-ưu-điểm-và-hạn-chế)
23. [Các lỗi thường gặp](#23-các-lỗi-thường-gặp)
24. [Bài tập luyện tập](#24-bài-tập-luyện-tập)
25. [Tổng kết](#25-tổng-kết)
26. [Tài liệu tham khảo](#26-tài-liệu-tham-khảo)

---

## 1. Heap là gì?

**Heap** là một cấu trúc dữ liệu dạng cây, thường được triển khai dưới dạng **Binary Heap**.

Binary Heap phải thỏa mãn hai điều kiện:

1. Có hình dạng của một **Complete Binary Tree**.
2. Thỏa mãn **Heap Property**.

Heap cho phép truy cập nhanh phần tử nhỏ nhất hoặc lớn nhất tại Node gốc:

```text
Min Heap → phần tử nhỏ nhất nằm ở root
Max Heap → phần tử lớn nhất nằm ở root
```

Heap thường được sử dụng để xây dựng:

- Priority Queue.
- Heap Sort.
- Dijkstra.
- Prim.
- Huffman Coding.
- Task Scheduler.
- Top K.
- Median của luồng dữ liệu.

---

## 2. Không nhầm Heap Data Structure với Heap Memory

Từ **heap** có thể chỉ hai khái niệm khác nhau.

### Heap Data Structure

Là cấu trúc dữ liệu được mô tả trong README này:

```text
Complete Binary Tree + Heap Property
```

### Heap Memory

Là vùng bộ nhớ động mà chương trình thường sử dụng thông qua:

```c
malloc();
calloc();
realloc();
free();
```

Hai khái niệm không giống nhau.

Một Binary Heap có thể được lưu:

- Trong mảng tĩnh.
- Trong mảng cấp phát động.
- Trong vùng Stack.
- Trong Heap Memory.
- Trong bộ nhớ do người dùng cung cấp.

Ví dụ sau sử dụng mảng tĩnh nhưng vẫn là Heap Data Structure:

```c
int storage[32];
MinHeap heap;
```

---

## 3. Complete Binary Tree

**Complete Binary Tree** là cây nhị phân có đặc điểm:

- Tất cả các tầng, trừ tầng cuối, được lấp đầy.
- Tầng cuối được điền từ trái sang phải.
- Không có khoảng trống nằm giữa các Node ở tầng cuối.

Ví dụ hợp lệ:

```text
            10
          /    \
        20      30
       /  \    /
     40   50  60
```

Các Node ở tầng cuối được điền từ trái sang phải.

Ví dụ không phải Complete Binary Tree:

```text
            10
          /    \
        20      30
          \    /
          50  60
```

Node `20` không có con trái nhưng lại có con phải.

### Vì sao hình dạng complete quan trọng?

Complete Binary Tree có thể được ánh xạ liên tiếp vào một mảng mà không cần lưu con trỏ:

```text
Tree level order → Array
```

Điều này làm Heap:

- Tiết kiệm bộ nhớ.
- Có cache locality tốt.
- Dễ triển khai.
- Không cần `left` và `right` pointer trong mỗi Node.

---

## 4. Heap Property

Heap Property mô tả quan hệ giữa một Node và các Node con trực tiếp.

### Min Heap Property

```text
parent <= left_child
parent <= right_child
```

Điều này đúng tại mọi Node trong Heap.

### Max Heap Property

```text
parent >= left_child
parent >= right_child
```

Điều này cũng đúng tại mọi Node.

Heap Property chỉ đảm bảo quan hệ giữa cha và con.

Nó không đảm bảo toàn bộ mảng được sắp xếp.

Ví dụ Min Heap hợp lệ:

```text
            2
          /   \
         5     3
        / \   / \
       9  8  7  6
```

Mảng level-order:

```text
[2, 5, 3, 9, 8, 7, 6]
```

Mảng này không tăng dần, nhưng vẫn là Min Heap.

---

## 5. Min Heap và Max Heap

## 5.1. Min Heap

Trong Min Heap:

```text
Giá trị của parent <= giá trị của các child
```

Root luôn là phần tử nhỏ nhất:

```text
heap[0] = minimum
```

Ví dụ:

```text
            1
          /   \
         3     2
        / \   / \
       7  8  5  4
```

Ứng dụng:

- Min Priority Queue.
- Dijkstra.
- Prim.
- Merge K Sorted Lists.
- K phần tử lớn nhất bằng Min Heap kích thước K.
- Job có deadline hoặc cost nhỏ nhất.

---

## 5.2. Max Heap

Trong Max Heap:

```text
Giá trị của parent >= giá trị của các child
```

Root luôn là phần tử lớn nhất:

```text
heap[0] = maximum
```

Ví dụ:

```text
            90
          /    \
        70      80
       /  \    / \
     30   60  50  40
```

Ứng dụng:

- Max Priority Queue.
- Heap Sort tăng dần.
- K phần tử nhỏ nhất bằng Max Heap kích thước K.
- Task có priority cao nhất.
- Theo dõi giá trị lớn nhất động.

---

## 6. Biểu diễn Binary Heap bằng mảng

Heap thường được lưu theo thứ tự **level-order**.

Ví dụ:

```text
            10
          /    \
        20      30
       /  \    / \
     40   50  60  70
```

Mảng:

```text
Index: [ 0,  1,  2,  3,  4,  5,  6]
Value: [10, 20, 30, 40, 50, 60, 70]
```

Với Node tại index `i`:

### Parent

```c
parent = (i - 1U) / 2U;
```

Chỉ hợp lệ khi:

```text
i > 0
```

### Left Child

```c
left = 2U * i + 1U;
```

### Right Child

```c
right = 2U * i + 2U;
```

### Ví dụ

Với:

```text
i = 2
```

```text
parent = 0
left   = 5
right  = 6
```

```text
array[2] = 30
array[0] = 10
array[5] = 60
array[6] = 70
```

---

## 6.1. Leaf Node

Với Heap có `n` phần tử:

```text
Các Node từ index n / 2 đến n - 1 là leaf.
```

Node nội bộ cuối cùng:

```text
index = n / 2 - 1
```

Điều này quan trọng khi Build Heap:

```c
for (i = size / 2U; i > 0U; i--)
{
    HeapifyDown(i - 1U);
}
```

---

## 7. Chiều cao của Heap

Binary Heap là Complete Binary Tree nên chiều cao là:

```text
O(log n)
```

Cụ thể, với `n > 0`:

```text
height = floor(log2(n))
```

Ví dụ:

| Số Node | Chiều cao |
|---:|---:|
| 1 | 0 |
| 2–3 | 1 |
| 4–7 | 2 |
| 8–15 | 3 |
| 16–31 | 4 |

Các thao tác di chuyển từ Node lên root hoặc từ root xuống leaf chỉ đi tối đa theo chiều cao:

```text
O(log n)
```

---

## 8. Các thao tác cơ bản

Một Binary Heap thường hỗ trợ:

- Initialize.
- Is Empty.
- Is Full.
- Peek root.
- Insert.
- Extract root.
- Sift Up.
- Sift Down.
- Change Key.
- Delete at index.
- Build Heap.
- Clear.
- Validate Heap.

Với Min Heap:

```text
Peek Root    → lấy minimum
Extract Root → xóa và trả minimum
```

Với Max Heap:

```text
Peek Root    → lấy maximum
Extract Root → xóa và trả maximum
```

---

## 9. Sift Up

**Sift Up**, còn gọi là:

- Bubble Up.
- Percolate Up.
- Heapify Up.

Được sử dụng sau khi chèn phần tử mới hoặc giảm key trong Min Heap.

### Ví dụ chèn `2` vào Min Heap

Ban đầu:

```text
            5
          /   \
         8     7
        / \
       9  10
```

Mảng:

```text
[5, 8, 7, 9, 10]
```

Chèn `2` vào cuối để giữ Complete Tree:

```text
[5, 8, 7, 9, 10, 2]
```

Cây:

```text
            5
          /   \
         8     7
        / \   /
       9 10  2
```

So sánh `2` với parent `7`:

```text
2 < 7 → swap
```

```text
[5, 8, 2, 9, 10, 7]
```

Tiếp tục so sánh với root `5`:

```text
2 < 5 → swap
```

Kết quả:

```text
[2, 8, 5, 9, 10, 7]
```

Min Heap được khôi phục.

### Pseudocode

```text
SiftUp(index):

    while index > 0:

        parent = (index - 1) / 2

        if heap[parent] <= heap[index]:
            stop

        swap(heap[parent], heap[index])
        index = parent
```

---

## 10. Sift Down và Heapify

**Sift Down**, còn gọi là:

- Bubble Down.
- Percolate Down.
- Heapify Down.
- Min Heapify hoặc Max Heapify.

Được dùng sau khi:

- Xóa root.
- Tăng key trong Min Heap.
- Giảm key trong Max Heap.
- Build Heap.
- Heap Sort.

### Ví dụ Extract Min

Min Heap:

```text
[2, 5, 3, 9, 8, 7, 6]
```

Root:

```text
2
```

Đưa phần tử cuối `6` lên root:

```text
[6, 5, 3, 9, 8, 7]
```

So sánh root với hai child:

```text
5 và 3
```

Child nhỏ nhất là `3`.

Swap:

```text
[3, 5, 6, 9, 8, 7]
```

Heap đã hợp lệ.

### Pseudocode

```text
SiftDown(index):

    loop:

        left     = 2 * index + 1
        right    = 2 * index + 2
        smallest = index

        if left tồn tại và heap[left] < heap[smallest]:
            smallest = left

        if right tồn tại và heap[right] < heap[smallest]:
            smallest = right

        if smallest == index:
            stop

        swap(heap[index], heap[smallest])
        index = smallest
```

---

## 11. Cài đặt Min Heap bằng C

Cấu trúc project:

```text
min-heap/
├── include/
│   └── min_heap.h
├── src/
│   ├── min_heap.c
│   └── main.c
└── README.md
```

Phiên bản dưới đây sử dụng vùng nhớ do người gọi cung cấp:

- Không gọi `malloc()`.
- Capacity được xác định trước.
- Phù hợp với embedded.
- API trả `bool` để báo lỗi rõ ràng.

---

### 11.1. File `min_heap.h`

```c
#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <stdbool.h>
#include <stddef.h>

typedef struct
{
    int *data;
    size_t capacity;
    size_t size;
} MinHeap;

/**
 * @brief Initialize a min heap with caller-provided storage.
 */
bool MinHeap_Init(
    MinHeap *heap,
    int storage[],
    size_t capacity
);

/**
 * @brief Return true if the heap has no elements.
 */
bool MinHeap_IsEmpty(const MinHeap *heap);

/**
 * @brief Return true if the heap reached capacity.
 */
bool MinHeap_IsFull(const MinHeap *heap);

/**
 * @brief Return the number of stored elements.
 */
size_t MinHeap_Size(const MinHeap *heap);

/**
 * @brief Return the minimum value without removing it.
 */
bool MinHeap_Peek(
    const MinHeap *heap,
    int *value
);

/**
 * @brief Insert a new value.
 */
bool MinHeap_Insert(
    MinHeap *heap,
    int value
);

/**
 * @brief Remove and return the minimum value.
 */
bool MinHeap_ExtractMin(
    MinHeap *heap,
    int *value
);

/**
 * @brief Replace the value at an index and repair the heap.
 */
bool MinHeap_ChangeKey(
    MinHeap *heap,
    size_t index,
    int new_value
);

/**
 * @brief Delete the value stored at an index.
 */
bool MinHeap_DeleteAt(
    MinHeap *heap,
    size_t index,
    int *removed_value
);

/**
 * @brief Build a heap by copying values into heap storage.
 */
bool MinHeap_Build(
    MinHeap *heap,
    const int values[],
    size_t count
);

/**
 * @brief Verify the min-heap property.
 */
bool MinHeap_IsValid(const MinHeap *heap);

/**
 * @brief Remove all logical elements.
 */
void MinHeap_Clear(MinHeap *heap);

#endif
```

---

### 11.2. File `min_heap.c`

```c
#include "min_heap.h"

static size_t ParentIndex(size_t index)
{
    return (index - 1U) / 2U;
}

static size_t LeftChildIndex(size_t index)
{
    return 2U * index + 1U;
}

static size_t RightChildIndex(size_t index)
{
    return 2U * index + 2U;
}

static void SwapInt(int *left, int *right)
{
    int temporary = *left;
    *left = *right;
    *right = temporary;
}

static void SiftUp(
    MinHeap *heap,
    size_t index
)
{
    while (index > 0U)
    {
        size_t parent = ParentIndex(index);

        if (heap->data[parent] <= heap->data[index])
        {
            break;
        }

        SwapInt(
            &heap->data[parent],
            &heap->data[index]
        );

        index = parent;
    }
}

static void SiftDown(
    MinHeap *heap,
    size_t index
)
{
    while (true)
    {
        size_t smallest = index;
        size_t left = LeftChildIndex(index);
        size_t right = RightChildIndex(index);

        if ((left < heap->size) &&
            (heap->data[left] <
             heap->data[smallest]))
        {
            smallest = left;
        }

        if ((right < heap->size) &&
            (heap->data[right] <
             heap->data[smallest]))
        {
            smallest = right;
        }

        if (smallest == index)
        {
            break;
        }

        SwapInt(
            &heap->data[index],
            &heap->data[smallest]
        );

        index = smallest;
    }
}

bool MinHeap_Init(
    MinHeap *heap,
    int storage[],
    size_t capacity
)
{
    if ((heap == NULL) ||
        (storage == NULL) ||
        (capacity == 0U))
    {
        return false;
    }

    heap->data = storage;
    heap->capacity = capacity;
    heap->size = 0U;

    return true;
}

bool MinHeap_IsEmpty(const MinHeap *heap)
{
    return (heap == NULL) || (heap->size == 0U);
}

bool MinHeap_IsFull(const MinHeap *heap)
{
    if (heap == NULL)
    {
        return false;
    }

    return heap->size == heap->capacity;
}

size_t MinHeap_Size(const MinHeap *heap)
{
    if (heap == NULL)
    {
        return 0U;
    }

    return heap->size;
}

bool MinHeap_Peek(
    const MinHeap *heap,
    int *value
)
{
    if ((heap == NULL) ||
        (value == NULL) ||
        (heap->data == NULL) ||
        (heap->size == 0U))
    {
        return false;
    }

    *value = heap->data[0];

    return true;
}

bool MinHeap_Insert(
    MinHeap *heap,
    int value
)
{
    size_t inserted_index;

    if ((heap == NULL) || (heap->data == NULL))
    {
        return false;
    }

    if (MinHeap_IsFull(heap))
    {
        return false;
    }

    inserted_index = heap->size;

    heap->data[inserted_index] = value;
    heap->size++;

    SiftUp(heap, inserted_index);

    return true;
}

bool MinHeap_ExtractMin(
    MinHeap *heap,
    int *value
)
{
    if ((heap == NULL) ||
        (heap->data == NULL) ||
        (value == NULL) ||
        (heap->size == 0U))
    {
        return false;
    }

    *value = heap->data[0];

    heap->size--;

    if (heap->size > 0U)
    {
        heap->data[0] = heap->data[heap->size];
        SiftDown(heap, 0U);
    }

    return true;
}

bool MinHeap_ChangeKey(
    MinHeap *heap,
    size_t index,
    int new_value
)
{
    int old_value;

    if ((heap == NULL) ||
        (heap->data == NULL) ||
        (index >= heap->size))
    {
        return false;
    }

    old_value = heap->data[index];
    heap->data[index] = new_value;

    if (new_value < old_value)
    {
        SiftUp(heap, index);
    }
    else if (new_value > old_value)
    {
        SiftDown(heap, index);
    }

    return true;
}

bool MinHeap_DeleteAt(
    MinHeap *heap,
    size_t index,
    int *removed_value
)
{
    int replacement;
    int deleted;

    if ((heap == NULL) ||
        (heap->data == NULL) ||
        (index >= heap->size))
    {
        return false;
    }

    deleted = heap->data[index];

    heap->size--;

    if (index < heap->size)
    {
        replacement = heap->data[heap->size];
        heap->data[index] = replacement;

        if ((index > 0U) &&
            (heap->data[index] <
             heap->data[ParentIndex(index)]))
        {
            SiftUp(heap, index);
        }
        else
        {
            SiftDown(heap, index);
        }
    }

    if (removed_value != NULL)
    {
        *removed_value = deleted;
    }

    return true;
}

bool MinHeap_Build(
    MinHeap *heap,
    const int values[],
    size_t count
)
{
    size_t index;

    if ((heap == NULL) ||
        (heap->data == NULL) ||
        (values == NULL) ||
        (count > heap->capacity))
    {
        return false;
    }

    for (index = 0U; index < count; index++)
    {
        heap->data[index] = values[index];
    }

    heap->size = count;

    for (index = heap->size / 2U;
         index > 0U;
         index--)
    {
        SiftDown(heap, index - 1U);
    }

    return true;
}

bool MinHeap_IsValid(const MinHeap *heap)
{
    size_t parent;

    if ((heap == NULL) || (heap->data == NULL))
    {
        return false;
    }

    for (parent = 0U;
         parent < heap->size / 2U;
         parent++)
    {
        size_t left = LeftChildIndex(parent);
        size_t right = RightChildIndex(parent);

        if ((left < heap->size) &&
            (heap->data[parent] > heap->data[left]))
        {
            return false;
        }

        if ((right < heap->size) &&
            (heap->data[parent] > heap->data[right]))
        {
            return false;
        }
    }

    return true;
}

void MinHeap_Clear(MinHeap *heap)
{
    if (heap == NULL)
    {
        return;
    }

    heap->size = 0U;
}
```

---

### 11.3. File `main.c`

```c
#include "min_heap.h"

#include <stdio.h>

#define HEAP_CAPACITY 16U

static void PrintHeap(const MinHeap *heap)
{
    size_t index;

    printf("Heap:");

    for (index = 0U;
         index < MinHeap_Size(heap);
         index++)
    {
        printf(" %d", heap->data[index]);
    }

    printf("\n");
}

int main(void)
{
    MinHeap heap;
    int storage[HEAP_CAPACITY];
    int value;

    if (!MinHeap_Init(
            &heap,
            storage,
            HEAP_CAPACITY))
    {
        printf("Heap initialization failed.\n");
        return 1;
    }

    MinHeap_Insert(&heap, 3);
    MinHeap_Insert(&heap, 2);
    MinHeap_Insert(&heap, 15);
    MinHeap_Insert(&heap, 5);
    MinHeap_Insert(&heap, 4);
    MinHeap_Insert(&heap, 45);

    PrintHeap(&heap);

    if (MinHeap_Peek(&heap, &value))
    {
        printf("Minimum: %d\n", value);
    }

    if (MinHeap_ExtractMin(&heap, &value))
    {
        printf("Extracted: %d\n", value);
    }

    PrintHeap(&heap);

    if (MinHeap_ChangeKey(&heap, 2U, 1))
    {
        printf("Changed index 2 to 1.\n");
    }

    PrintHeap(&heap);

    if (MinHeap_DeleteAt(&heap, 1U, &value))
    {
        printf("Deleted: %d\n", value);
    }

    PrintHeap(&heap);

    printf(
        "Heap valid: %s\n",
        MinHeap_IsValid(&heap) ? "yes" : "no"
    );

    return 0;
}
```

---

### 11.4. Biên dịch

```bash
gcc -std=c11 \
    -Wall \
    -Wextra \
    -Wpedantic \
    -Iinclude \
    src/min_heap.c \
    src/main.c \
    -o min_heap_demo
```

Chạy:

```bash
./min_heap_demo
```

---

## 12. Ví dụ chạy chương trình

Một kết quả có thể là:

```text
Heap: 2 3 4 5 15 45
Minimum: 2
Extracted: 2
Heap: 3 5 4 45 15
Changed index 2 to 1.
Heap: 1 5 3 45 15
Deleted: 5
Heap: 1 15 3 45
Heap valid: yes
```

Cấu trúc mảng có thể khác nhưng vẫn hợp lệ nếu Heap Property được giữ.

Ví dụ hai Min Heap hợp lệ:

```text
[1, 3, 2, 7, 6, 5, 4]
```

và:

```text
[1, 2, 3, 4, 5, 6, 7]
```

Heap không yêu cầu một biểu diễn duy nhất.

---

## 13. Build Heap từ một mảng

Có hai cách tạo Heap từ `n` phần tử.

## 13.1. Chèn từng phần tử

```text
Bắt đầu Heap rỗng
→ Insert phần tử thứ nhất
→ Insert phần tử thứ hai
→ ...
```

Mỗi Insert:

```text
O(log n)
```

Tổng:

```text
O(n log n)
```

---

## 13.2. Bottom-Up Heap Construction

Copy toàn bộ dữ liệu vào mảng Heap.

Sau đó Sift Down từ Node nội bộ cuối cùng về root:

```c
for (index = size / 2U;
     index > 0U;
     index--)
{
    SiftDown(heap, index - 1U);
}
```

Độ phức tạp:

```text
O(n)
```

### Vì sao không phải O(n log n)?

Không phải mọi Node đều đi xuống toàn bộ chiều cao.

- Khoảng một nửa Node là leaf: không cần Heapify.
- Khoảng một phần tư Node chỉ cách leaf một tầng.
- Chỉ rất ít Node ở gần root cần đi sâu.

Tổng công việc hội tụ về:

```text
O(n)
```

---

## 14. Heap Sort

Heap Sort là thuật toán sắp xếp sử dụng Binary Heap.

Để sắp xếp tăng dần:

1. Build Max Heap.
2. Root là phần tử lớn nhất.
3. Swap root với phần tử cuối vùng chưa sắp.
4. Giảm heap size.
5. Max Heapify root.
6. Lặp lại.

### Mã C

```c
#include <stddef.h>

static void HeapSortSwap(int *left, int *right)
{
    int temporary = *left;
    *left = *right;
    *right = temporary;
}

static void MaxHeapify(
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

        HeapSortSwap(
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
        MaxHeapify(
            array,
            size,
            index - 1U
        );
    }

    for (index = size;
         index > 1U;
         index--)
    {
        HeapSortSwap(
            &array[0],
            &array[index - 1U]
        );

        MaxHeapify(
            array,
            index - 1U,
            0U
        );
    }
}
```

### Đặc điểm

```text
Best:    O(n log n)
Average: O(n log n)
Worst:   O(n log n)
Space:   O(1)
Stable:  No
```

Heap Sort có worst-case tốt nhưng thường có cache locality kém hơn Quick Sort.

---

## 15. Priority Queue

Priority Queue là cấu trúc trong đó phần tử được xử lý theo priority.

Không nhất thiết tuân theo FIFO.

Ví dụ:

```text
Task A: priority 2
Task B: priority 7
Task C: priority 4
```

Với Max Priority Queue:

```text
Task B được lấy trước
```

Binary Heap là cách triển khai phổ biến vì:

```text
Peek priority cao nhất/thấp nhất → O(1)
Insert                          → O(log n)
Extract                         → O(log n)
```

---

## 15.1. Priority Queue chứa cấu trúc

```c
typedef struct
{
    uint32_t task_id;
    uint8_t priority;
    uint32_t sequence;
} TaskEntry;
```

Comparator có thể ưu tiên:

1. Priority cao hơn.
2. Nếu cùng priority, sequence nhỏ hơn được xử lý trước.

Lưu ý:

```text
Binary Heap thông thường không stable.
```

Muốn giữ FIFO giữa các phần tử cùng priority, cần thêm `sequence`.

---

## 16. Độ phức tạp

| Thao tác | Độ phức tạp |
|---|---:|
| Peek root | `O(1)` |
| Insert | `O(log n)` |
| Extract root | `O(log n)` |
| Sift Up | `O(log n)` |
| Sift Down | `O(log n)` |
| Change Key | `O(log n)` |
| Delete at known index | `O(log n)` |
| Build Heap bottom-up | `O(n)` |
| Search arbitrary value | `O(n)` |
| Validate Heap | `O(n)` |
| Heap Sort | `O(n log n)` |

### Tìm một giá trị bất kỳ

Heap chỉ đảm bảo cha nhỏ hơn hoặc lớn hơn con.

Nó không cho phép Binary Search.

Để tìm một giá trị bất kỳ trong Heap, worst case vẫn là:

```text
O(n)
```

---

## 17. Kiểm tra một mảng có phải Heap

### Kiểm tra Min Heap

Với mỗi Node nội bộ:

```text
parent <= left
parent <= right
```

```c
#include <stdbool.h>
#include <stddef.h>

bool IsMinHeap(
    const int array[],
    size_t size
)
{
    size_t parent;

    if (array == NULL)
    {
        return false;
    }

    for (parent = 0U;
         parent < size / 2U;
         parent++)
    {
        size_t left = 2U * parent + 1U;
        size_t right = 2U * parent + 2U;

        if ((left < size) &&
            (array[parent] > array[left]))
        {
            return false;
        }

        if ((right < size) &&
            (array[parent] > array[right]))
        {
            return false;
        }
    }

    return true;
}
```

Độ phức tạp:

```text
O(n)
```

---

## 18. Tìm phần tử lớn hoặc nhỏ thứ K

## 18.1. K-th Smallest

Một cách:

1. Build Min Heap từ toàn bộ dữ liệu.
2. Extract Min `k` lần.

Độ phức tạp:

```text
Build:      O(n)
K extracts: O(k log n)
Total:      O(n + k log n)
```

---

## 18.2. K phần tử lớn nhất

Dùng Min Heap kích thước `k`.

1. Thêm `k` phần tử đầu.
2. Với mỗi phần tử còn lại:
   - Nếu lớn hơn root, thay root.
3. Heap chứa `k` phần tử lớn nhất.

Độ phức tạp:

```text
O(n log k)
```

Bộ nhớ:

```text
O(k)
```

---

## 18.3. K-th Largest trong Stream

Duy trì Min Heap kích thước `k`.

Sau mỗi phần tử mới:

```text
Nếu Heap chưa đủ k:
    Insert

Nếu value > root:
    Replace root
```

Root là phần tử lớn thứ `k` trong dữ liệu đã nhận.

---

## 19. Các ứng dụng của Heap

## 19.1. Priority Queue

Heap cung cấp truy cập nhanh phần tử có priority cao nhất hoặc thấp nhất.

Ứng dụng:

- Task scheduling.
- Event scheduling.
- Interrupt work queue.
- Job processing.
- Packet scheduling.
- Patient triage.

---

## 19.2. Heap Sort

Heap Sort đảm bảo:

```text
O(n log n)
```

ở worst case và chỉ dùng bộ nhớ phụ `O(1)`.

---

## 19.3. Dijkstra

Dijkstra thường sử dụng Min Priority Queue để chọn đỉnh có khoảng cách tạm thời nhỏ nhất.

```text
Extract Min → O(log V)
Decrease Key → O(log V)
```

---

## 19.4. Prim

Prim dùng Min Heap để chọn cạnh hoặc đỉnh có key nhỏ nhất khi xây Minimum Spanning Tree.

---

## 19.5. A* Search

A* có thể dùng Min Heap theo:

```text
f(n) = g(n) + h(n)
```

Node có `f` nhỏ nhất được xử lý trước.

---

## 19.6. Huffman Coding

Huffman Coding:

1. Đưa tần suất ký tự vào Min Heap.
2. Extract hai Node nhỏ nhất.
3. Ghép thành Node mới.
4. Insert lại Heap.
5. Lặp đến khi còn một cây.

---

## 19.7. Merge K Sorted Arrays hoặc Lists

Đưa phần tử đầu của mỗi nguồn vào Min Heap.

Mỗi lần:

1. Extract phần tử nhỏ nhất.
2. Thêm phần tử kế tiếp của cùng nguồn.

Độ phức tạp với tổng `n` phần tử và `k` nguồn:

```text
O(n log k)
```

---

## 19.8. Nearly Sorted Array

Nếu mỗi phần tử lệch vị trí đúng tối đa `k`, dùng Min Heap kích thước `k + 1`.

Độ phức tạp:

```text
O(n log k)
```

---

## 19.9. Median của Data Stream

Dùng hai Heap:

```text
Max Heap → nửa nhỏ
Min Heap → nửa lớn
```

Cân bằng kích thước hai Heap.

Median:

- Root của một Heap nếu tổng số phần tử lẻ.
- Trung bình hai root nếu chẵn.

---

## 19.10. Load Balancing

Min Heap có thể lưu server theo tải hiện tại.

Server có load thấp nhất nằm tại root.

---

## 19.11. Resource Allocation

Heap có thể chọn:

- Memory block phù hợp theo cost.
- CPU slot.
- Resource có thời gian hoàn thành sớm.
- Request có deadline gần nhất.

---

## 19.12. Order Statistics

Heap hỗ trợ:

- K-th largest.
- K-th smallest.
- Top K frequent.
- K closest points.
- K maximum sum combinations.

---

## 19.13. Event Simulation

Các event được sắp theo timestamp.

Min Heap root là event xảy ra sớm nhất.

```text
Extract earliest event
→ xử lý
→ thêm event mới
```

---

## 20. Các loại Heap khác

## 20.1. Binary Heap

Mỗi Node tối đa hai child.

Phổ biến nhất vì đơn giản và hiệu quả.

---

## 20.2. D-ary hoặc K-ary Heap

Mỗi Node có `d` child.

Công thức với index `i` tùy cách đánh số.

Ưu điểm:

- Chiều cao thấp hơn Binary Heap.
- Giảm số bước Sift Up.

Hạn chế:

- Sift Down phải so sánh nhiều child hơn.

---

## 20.3. Binomial Heap

Là tập hợp các Binomial Tree.

Hỗ trợ merge/union Heap hiệu quả hơn Binary Heap trong một số bài toán.

---

## 20.4. Fibonacci Heap

Có amortized complexity tốt cho:

- Insert.
- Decrease Key.
- Meld.

Thường được nhắc trong phân tích lý thuyết của Dijkstra và Prim.

Implementation phức tạp hơn Binary Heap.

---

## 20.5. Leftist Heap

Heap dựa trên cây, được thiết kế để merge nhanh.

---

## 20.6. Pairing Heap

Cấu trúc heap tree tương đối đơn giản với hiệu năng thực tế tốt cho nhiều workload.

---

## 20.7. Min-Max Heap

Hỗ trợ truy cập cả minimum và maximum hiệu quả.

---

## 21. Heap trong lập trình nhúng

## 21.1. Dùng vùng nhớ tĩnh

Trong embedded, nên cân nhắc:

```c
#define MAX_EVENTS 32U

static Event heap_storage[MAX_EVENTS];
```

Không cần gọi `malloc()` trong runtime.

Ưu điểm:

- RAM tối đa biết trước.
- Không phân mảnh Heap Memory.
- Không thất bại do allocator.
- Dễ phân tích worst case.

---

## 21.2. Event Scheduler

Min Heap có thể sắp event theo deadline:

```c
typedef struct
{
    uint32_t deadline;
    uint16_t signal;
} TimedEvent;
```

Root:

```text
Event có deadline gần nhất
```

Timer ISR chỉ nên đánh thức scheduler hoặc đặt flag.

Không nên thực hiện nhiều thao tác Heap phức tạp trong ISR nếu thời gian interrupt bị giới hạn nghiêm ngặt.

---

## 21.3. Priority Task Queue

Max Heap có thể lưu task theo priority.

Nếu cùng priority, thêm sequence counter để duy trì thứ tự:

```c
typedef struct
{
    uint8_t priority;
    uint32_t sequence;
    TaskCallback callback;
} ScheduledTask;
```

---

## 21.4. Bảo vệ truy cập đồng thời

Nếu ISR và main/task cùng sửa Heap, các thao tác nhiều bước như swap và cập nhật size không tự động atomic.

Cần:

- Critical section.
- Mutex.
- Scheduler lock.
- Single-owner design.
- Message Queue đưa yêu cầu sang một task quản lý Heap.

`volatile` không đủ để làm Heap thread-safe.

---

## 21.5. Không dùng recursion nếu Stack nhỏ

Sift Down có thể viết đệ quy hoặc lặp.

Bản lặp trong README:

- Không dùng Call Stack bổ sung.
- Dễ phân tích RAM.
- Phù hợp với coding rule cấm recursion.

---

## 21.6. Xác định Worst-Case

Với capacity cố định `N`:

```text
Sift Up tối đa floor(log2(N)) bước
Sift Down tối đa floor(log2(N)) bước
```

Điều này thuận lợi cho hệ thống real-time.

---

## 21.7. Không dùng Heap khi cấu trúc đơn giản hơn đủ tốt

Nếu chỉ có 4 task cố định, Linear Search tìm priority cao nhất có thể đơn giản hơn Heap.

Heap hữu ích khi:

- Dữ liệu thay đổi thường xuyên.
- Cần nhiều Insert và Extract.
- Số phần tử đủ lớn.
- Cần thời gian `O(log n)` thay vì `O(n)`.

---

## 22. Ưu điểm và hạn chế

## 22.1. Ưu điểm

- Peek minimum hoặc maximum là `O(1)`.
- Insert là `O(log n)`.
- Extract root là `O(log n)`.
- Build Heap là `O(n)`.
- Lưu bằng mảng, không cần pointer mỗi Node.
- Cache locality tốt hơn tree pointer-based.
- Priority Queue hiệu quả.
- Worst-case rõ ràng.
- Có thể triển khai hoàn toàn tĩnh.
- Hữu ích trong nhiều Graph Algorithm.

---

## 22.2. Hạn chế

- Không hỗ trợ tìm kiếm arbitrary value nhanh.
- Không giữ toàn bộ dữ liệu theo thứ tự.
- Không stable mặc định.
- Xóa theo giá trị cần biết index hoặc tìm `O(n)`.
- Capacity cố định có thể overflow.
- Dynamic Heap có thể cần resize.
- Heap Sort thường không nhanh bằng Quick Sort trong thực tế.
- Thay đổi key sai hướng có thể phá Heap Property.
- Truy cập đồng thời cần đồng bộ.

---

## 23. Các lỗi thường gặp

## 23.1. Nhầm Heap là Sorted Array

Min Heap:

```text
[1, 4, 2, 9, 7, 5, 3]
```

vẫn hợp lệ dù:

```text
4 > 2
```

vì `4` và `2` là sibling, không phải parent-child.

---

## 23.2. Gọi Parent của root

Công thức:

```c
(index - 1U) / 2U
```

không được gọi khi:

```text
index == 0
```

vì unsigned underflow.

---

## 23.3. Không kiểm tra child tồn tại

Phải kiểm tra:

```c
left < heap->size
right < heap->size
```

trước khi đọc mảng.

---

## 23.4. Insert nhưng không Sift Up

Thêm cuối chỉ giữ Complete Tree, chưa đảm bảo Heap Property.

---

## 23.5. Extract root nhưng không Sift Down

Đưa phần tử cuối lên root có thể phá Heap Property.

---

## 23.6. Chọn sai child khi Sift Down

Trong Min Heap phải swap với child nhỏ nhất.

Không phải luôn swap với left child.

---

## 23.7. Delete bằng giá trị sentinel

Một cách phổ biến là:

```text
Decrease key thành -∞
Extract Min
```

Nhưng dùng `INT_MIN` có thể không phù hợp nếu:

- `INT_MIN` là dữ liệu hợp lệ.
- Heap chứa kiểu khác.
- Comparator phức tạp.

Cách `DeleteAt` bằng phần tử cuối tổng quát và an toàn hơn.

---

## 23.8. Change Key chỉ sửa một hướng

Trong Min Heap:

```text
new < old → Sift Up
new > old → Sift Down
```

Chỉ gọi một hướng cố định có thể không sửa đúng Heap.

---

## 23.9. Nhầm Build Heap là O(n log n)

Bottom-up Build Heap là:

```text
O(n)
```

Chèn từng phần tử mới là:

```text
O(n log n)
```

---

## 23.10. Không kiểm tra capacity

Với Heap tĩnh:

```c
if (heap->size == heap->capacity)
{
    return false;
}
```

Nếu không, Insert sẽ ghi ngoài mảng.

---

## 23.11. Comparator không nhất quán

Nếu Heap chứa `struct`, comparator phải tạo thứ tự nhất quán.

Không nên để kết quả phụ thuộc trạng thái thay đổi ngoài Heap mà không re-heapify.

---

## 23.12. Giữ index bên ngoài nhưng không cập nhật sau swap

Nếu module khác lưu vị trí một object trong Heap, mọi swap phải cập nhật index mapping.

Điều này quan trọng với:

- Decrease Key theo object ID.
- Dijkstra.
- Indexed Priority Queue.
- Scheduler hủy event theo handle.

---

## 24. Bài tập luyện tập

### Cơ bản

1. Cài đặt Min Heap.
2. Cài đặt Max Heap.
3. Insert một key.
4. Extract Min.
5. Extract Max.
6. Peek root.
7. Change Key.
8. Delete tại index.
9. Build Heap.
10. Kiểm tra mảng có phải Min Heap.
11. Kiểm tra mảng có phải Max Heap.
12. Tính chiều cao Heap.
13. Heap Sort tăng dần.
14. Heap Sort giảm dần.

### Trung bình

1. K-th Smallest.
2. K-th Largest.
3. K phần tử lớn nhất.
4. K phần tử nhỏ nhất.
5. K phần tử gần nhất.
6. Sort Nearly Sorted Array.
7. Merge K Sorted Arrays.
8. Merge K Sorted Linked Lists.
9. Connect Ropes with Minimum Cost.
10. Top K Frequent Elements.
11. K-th Largest in a Stream.
12. K Closest Points to Origin.
13. Convert Min Heap to Max Heap.
14. Running Median.
15. Huffman Coding.
16. Maximum product of three elements.

### Nâng cao

1. Dijkstra với Indexed Min Heap.
2. Prim với Min Heap.
3. A* Search.
4. Median từ Data Stream bằng hai Heap.
5. K-th Largest Subarray Sum.
6. Skyline Problem.
7. Range Covering K Sorted Lists.
8. Merge two Binary Max Heaps.
9. Min-Max Heap.
10. Binomial Heap.
11. Fibonacci Heap.
12. Leftist Heap.
13. Pairing Heap.
14. Lock-free Priority Queue.
15. Static Event Scheduler cho embedded.

---

## 25. Tổng kết

Heap Data Structure thường là:

```text
Complete Binary Tree
        +
Heap Property
```

Hai loại phổ biến:

```text
Min Heap → root nhỏ nhất
Max Heap → root lớn nhất
```

Biểu diễn bằng mảng:

```c
parent = (i - 1) / 2;
left   = 2 * i + 1;
right  = 2 * i + 2;
```

Các thao tác chính:

```text
Peek Root    → O(1)
Insert       → O(log n)
Extract Root → O(log n)
Change Key   → O(log n)
Delete Index → O(log n)
Build Heap   → O(n)
Search       → O(n)
```

Heap được ứng dụng trong:

- Priority Queue.
- Heap Sort.
- Dijkstra.
- Prim.
- A*.
- Huffman Coding.
- Merge K Sorted Sources.
- Top K.
- Running Median.
- Event Scheduling.
- Job Scheduling.
- Load Balancing.

Trong embedded:

- Nên ưu tiên mảng tĩnh.
- Kiểm tra overflow và underflow.
- Có thể dùng Sift Down dạng lặp.
- Bảo vệ khi ISR và task cùng truy cập.
- Thêm sequence nếu cần stable priority.
- Không nhầm Heap Data Structure với Heap Memory.

---

## 26. Tài liệu tham khảo

README này được biên soạn và diễn đạt lại độc lập dựa trên phạm vi chủ đề được trình bày trong:

- GeeksforGeeks — **Heap Data Structure**:  
  https://www.geeksforgeeks.org/dsa/heap-data-structure/
