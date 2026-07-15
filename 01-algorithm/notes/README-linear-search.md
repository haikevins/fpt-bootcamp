# Linear Search trong C

Linear Search, còn gọi là **Sequential Search**, là thuật toán tìm kiếm bằng cách kiểm tra lần lượt từng phần tử cho đến khi:

- Tìm thấy phần tử cần tìm.
- Hoặc đã duyệt hết tập dữ liệu.

Ví dụ:

```text
array  = [10, 50, 30, 70, 80, 20, 90, 40]
target = 30
```

Quá trình:

```text
10 != 30
50 != 30
30 == 30  -> tìm thấy tại index 2
```

Linear Search không yêu cầu dữ liệu phải được sắp xếp. Thuật toán phù hợp với mảng nhỏ, Linked List, dữ liệu thường xuyên thay đổi và các trường hợp chỉ cần một vài truy vấn.

---

## Mục lục

1. [Linear Search là gì?](#1-linear-search-là-gì)
2. [Bài toán cơ bản](#2-bài-toán-cơ-bản)
3. [Nguyên lý hoạt động](#3-nguyên-lý-hoạt-động)
4. [Ví dụ từng bước](#4-ví-dụ-từng-bước)
5. [Cài đặt cơ bản bằng C](#5-cài-đặt-cơ-bản-bằng-c)
6. [Thiết kế API an toàn hơn](#6-thiết-kế-api-an-toàn-hơn)
7. [Độ phức tạp](#7-độ-phức-tạp)
8. [So sánh Linear Search và Binary Search](#8-so-sánh-linear-search-và-binary-search)
9. [Tìm lần xuất hiện đầu tiên](#9-tìm-lần-xuất-hiện-đầu-tiên)
10. [Tìm lần xuất hiện cuối cùng](#10-tìm-lần-xuất-hiện-cuối-cùng)
11. [Đếm số lần xuất hiện](#11-đếm-số-lần-xuất-hiện)
12. [Tìm tất cả vị trí](#12-tìm-tất-cả-vị-trí)
13. [Tìm min, max và phần tử gần nhất](#13-tìm-min-max-và-phần-tử-gần-nhất)
14. [Linear Search theo điều kiện](#14-linear-search-theo-điều-kiện)
15. [Sentinel Linear Search](#15-sentinel-linear-search)
16. [Linear Search đệ quy](#16-linear-search-đệ-quy)
17. [Tìm kiếm trong chuỗi](#17-tìm-kiếm-trong-chuỗi)
18. [Tìm kiếm trên struct](#18-tìm-kiếm-trên-struct)
19. [Generic Linear Search](#19-generic-linear-search)
20. [Tìm kiếm trong Linked List](#20-tìm-kiếm-trong-linked-list)
21. [Tìm kiếm trong ma trận](#21-tìm-kiếm-trong-ma-trận)
22. [Tối ưu khi dữ liệu đã sắp xếp](#22-tối-ưu-khi-dữ-liệu-đã-sắp-xếp)
23. [Ứng dụng của Linear Search](#23-ứng-dụng-của-linear-search)
24. [Linear Search trong embedded](#24-linear-search-trong-embedded)
25. [Bảng command và bảng calibration](#25-bảng-command-và-bảng-calibration)
26. [Interrupt, DMA và concurrency](#26-interrupt-dma-và-concurrency)
27. [Các lỗi thường gặp](#27-các-lỗi-thường-gặp)
28. [Khi nào nên dùng?](#28-khi-nào-nên-dùng)
29. [Khi nào không nên dùng?](#29-khi-nào-không-nên-dùng)
30. [Kiểm thử](#30-kiểm-thử)
31. [Bài tập luyện tập](#31-bài-tập-luyện-tập)
32. [Cấu trúc repository đề xuất](#32-cấu-trúc-repository-đề-xuất)
33. [Tóm tắt](#33-tóm-tắt)

---

# 1. Linear Search là gì?

Linear Search duyệt dữ liệu theo thứ tự tuyến tính:

```text
index:   0    1    2    3    4
array: [10] [20] [30] [40] [50]
         ↓    ↓    ↓
target = 30
```

Các bước:

1. Bắt đầu ở phần tử đầu tiên.
2. So sánh phần tử hiện tại với `target`.
3. Nếu bằng nhau, trả về vị trí.
4. Nếu không bằng, chuyển sang phần tử tiếp theo.
5. Nếu hết dữ liệu mà chưa tìm thấy, trả về trạng thái không tồn tại.

Linear Search còn được gọi là:

```text
Sequential Search
```

vì các phần tử được kiểm tra tuần tự.

---

# 2. Bài toán cơ bản

Cho mảng:

```text
arr[] = [1, 2, 3, 4]
x = 3
```

Kết quả:

```text
2
```

vì `3` nằm tại index `2`.

Ví dụ khác:

```text
arr[] = [10, 8, 30, 4, 5]
x = 5
```

Kết quả:

```text
4
```

Nếu không tồn tại:

```text
arr[] = [10, 8, 30]
x = 6
```

Kết quả:

```text
-1
```

Quy ước phổ biến:

```text
index >= 0  -> tìm thấy
-1          -> không tìm thấy
```

---

# 3. Nguyên lý hoạt động

Pseudo-code:

```text
linear_search(array, length, target):
    for index từ 0 đến length - 1:
        nếu array[index] bằng target:
            trả về index

    trả về NOT_FOUND
```

Không gian tìm kiếm giảm từng phần tử:

```text
n
n - 1
n - 2
...
1
0
```

Thuật toán không sử dụng thông tin về thứ tự tương đối giữa các phần tử.

---

# 4. Ví dụ từng bước

```text
index:  0   1   2   3   4
array: [10,  8, 30,  4,  5]
target = 5
```

| Bước | Index | Giá trị | Kết quả |
|---:|---:|---:|---|
| 1 | 0 | 10 | Không khớp |
| 2 | 1 | 8 | Không khớp |
| 3 | 2 | 30 | Không khớp |
| 4 | 3 | 4 | Không khớp |
| 5 | 4 | 5 | Tìm thấy |

Kết quả:

```text
index = 4
```

Nếu tìm `6`, thuật toán phải kiểm tra đủ năm phần tử trước khi trả về `-1`.

---

# 5. Cài đặt cơ bản bằng C

```c
#include <stdio.h>

int linear_search_basic(
    const int array[],
    int length,
    int target)
{
    int index;

    for (index = 0; index < length; index++)
    {
        if (array[index] == target)
        {
            return index;
        }
    }

    return -1;
}

int main(void)
{
    const int values[] = { 2, 3, 4, 10, 40 };
    const int length =
        (int)(sizeof(values) / sizeof(values[0]));

    const int target = 10;
    const int result =
        linear_search_basic(values, length, target);

    if (result >= 0)
    {
        printf("Found at index %d\n", result);
    }
    else
    {
        printf("Not found\n");
    }

    return 0;
}
```

Kết quả:

```text
Found at index 3
```

---

# 6. Thiết kế API an toàn hơn

Trong C hiện đại:

- Dùng `size_t` cho kích thước và index.
- Dùng `const` nếu hàm không sửa dữ liệu.
- Dùng `ptrdiff_t` nếu muốn trả về `-1`.
- Hoặc dùng `bool` kết hợp output parameter.

## `linear_search.h`

```c
#ifndef LINEAR_SEARCH_H
#define LINEAR_SEARCH_H

#include <stdbool.h>
#include <stddef.h>

ptrdiff_t linear_search_first(
    const int *array,
    size_t length,
    int target);

ptrdiff_t linear_search_last(
    const int *array,
    size_t length,
    int target);

size_t linear_search_count(
    const int *array,
    size_t length,
    int target);

bool linear_search_find(
    const int *array,
    size_t length,
    int target,
    size_t *index);

size_t linear_search_all(
    const int *array,
    size_t length,
    int target,
    size_t *output_indices,
    size_t output_capacity);

#endif
```

## `linear_search.c`

```c
#include "linear_search.h"

ptrdiff_t linear_search_first(
    const int *array,
    size_t length,
    int target)
{
    size_t index;

    if ((array == NULL) && (length > 0U))
    {
        return -1;
    }

    for (index = 0U; index < length; index++)
    {
        if (array[index] == target)
        {
            return (ptrdiff_t)index;
        }
    }

    return -1;
}

bool linear_search_find(
    const int *array,
    size_t length,
    int target,
    size_t *index)
{
    size_t current;

    if (((array == NULL) && (length > 0U)) ||
        (index == NULL))
    {
        return false;
    }

    for (current = 0U; current < length; current++)
    {
        if (array[current] == target)
        {
            *index = current;
            return true;
        }
    }

    return false;
}
```

Ví dụ dùng `bool`:

```c
size_t index;

if (linear_search_find(values, length, target, &index))
{
    printf("Found at index %zu\n", index);
}
```

Cách này không cần dùng một index âm để biểu diễn lỗi.

---

# 7. Độ phức tạp

## Best case

Phần tử cần tìm nằm ở index đầu tiên:

```text
O(1)
```

Số phép so sánh:

```text
1
```

## Worst case

Phần tử:

- Nằm ở vị trí cuối cùng.
- Hoặc không tồn tại.

Thuật toán phải kiểm tra `n` phần tử:

```text
O(n)
```

## Average case

Nếu mục tiêu phân bố đều trên các vị trí:

```text
số phép so sánh trung bình ≈ (n + 1) / 2
```

Do đó:

```text
O(n)
```

## Bộ nhớ phụ

Phiên bản lặp:

```text
O(1)
```

Phiên bản đệ quy:

```text
O(n)
```

trong worst-case do Call Stack.

---

# 8. So sánh Linear Search và Binary Search

| Tiêu chí | Linear Search | Binary Search |
|---|---|---|
| Yêu cầu dữ liệu có thứ tự | Không | Có |
| Best case | `O(1)` | `O(1)` |
| Average case | `O(n)` | `O(log n)` |
| Worst case | `O(n)` | `O(log n)` |
| Phù hợp Linked List | Có | Không hiệu quả |
| Cài đặt | Rất đơn giản | Dễ lỗi biên hơn |
| Dữ liệu nhỏ | Phù hợp | Không tạo nhiều lợi ích |
| Dữ liệu thường xuyên thay đổi | Phù hợp | Phải duy trì thứ tự |
| Nhiều truy vấn | Chậm hơn | Tốt hơn nếu đã sort |

Nếu mảng chưa sort và chỉ tìm một lần:

```text
Linear Search: O(n)
```

Sort rồi Binary Search:

```text
O(n log n) + O(log n)
```

thường không có lợi.

---

# 9. Tìm lần xuất hiện đầu tiên

Duyệt từ trái sang phải và return ngay khi khớp.

```c
ptrdiff_t linear_search_first(
    const int *array,
    size_t length,
    int target)
{
    size_t index;

    if ((array == NULL) && (length > 0U))
    {
        return -1;
    }

    for (index = 0U; index < length; index++)
    {
        if (array[index] == target)
        {
            return (ptrdiff_t)index;
        }
    }

    return -1;
}
```

Ví dụ:

```text
array  = [4, 2, 7, 2, 9]
target = 2
```

Kết quả:

```text
1
```

---

# 10. Tìm lần xuất hiện cuối cùng

Có thể duyệt từ cuối về đầu:

```c
ptrdiff_t linear_search_last(
    const int *array,
    size_t length,
    int target)
{
    size_t index;

    if ((array == NULL) && (length > 0U))
    {
        return -1;
    }

    for (index = length; index > 0U; index--)
    {
        const size_t current = index - 1U;

        if (array[current] == target)
        {
            return (ptrdiff_t)current;
        }
    }

    return -1;
}
```

Không viết:

```c
for (size_t index = length - 1U; index >= 0U; index--)
```

vì `size_t` là kiểu không dấu nên điều kiện `index >= 0U` luôn đúng.

---

# 11. Đếm số lần xuất hiện

```c
size_t linear_search_count(
    const int *array,
    size_t length,
    int target)
{
    size_t count = 0U;
    size_t index;

    if ((array == NULL) && (length > 0U))
    {
        return 0U;
    }

    for (index = 0U; index < length; index++)
    {
        if (array[index] == target)
        {
            count++;
        }
    }

    return count;
}
```

Ví dụ:

```text
array  = [5, 2, 5, 8, 5]
target = 5
count  = 3
```

Hàm phải duyệt toàn bộ mảng nên luôn có thời gian:

```text
O(n)
```

---

# 12. Tìm tất cả vị trí

```c
size_t linear_search_all(
    const int *array,
    size_t length,
    int target,
    size_t *output_indices,
    size_t output_capacity)
{
    size_t found_count = 0U;
    size_t index;

    if (((array == NULL) && (length > 0U)) ||
        ((output_indices == NULL) &&
         (output_capacity > 0U)))
    {
        return 0U;
    }

    for (index = 0U; index < length; index++)
    {
        if (array[index] == target)
        {
            if (found_count < output_capacity)
            {
                output_indices[found_count] = index;
            }

            found_count++;
        }
    }

    return found_count;
}
```

Ví dụ:

```text
array  = [5, 2, 5, 8, 5]
target = 5
```

Kết quả:

```text
indices = [0, 2, 4]
count   = 3
```

Giá trị trả về là tổng số lần xuất hiện, ngay cả khi output buffer không đủ lớn.

---

# 13. Tìm min, max và phần tử gần nhất

## Tìm min và max trong một lượt

```c
#include <stdbool.h>
#include <stddef.h>

bool linear_min_max(
    const int *array,
    size_t length,
    int *minimum,
    int *maximum)
{
    size_t index;
    int current_minimum;
    int current_maximum;

    if ((array == NULL) ||
        (minimum == NULL) ||
        (maximum == NULL) ||
        (length == 0U))
    {
        return false;
    }

    current_minimum = array[0];
    current_maximum = array[0];

    for (index = 1U; index < length; index++)
    {
        if (array[index] < current_minimum)
        {
            current_minimum = array[index];
        }

        if (array[index] > current_maximum)
        {
            current_maximum = array[index];
        }
    }

    *minimum = current_minimum;
    *maximum = current_maximum;

    return true;
}
```

## Tìm phần tử gần target nhất

```c
#include <stdint.h>

ptrdiff_t linear_search_nearest(
    const int *array,
    size_t length,
    int target)
{
    size_t best_index;
    int64_t best_distance;
    size_t index;

    if ((array == NULL) || (length == 0U))
    {
        return -1;
    }

    best_index = 0U;
    best_distance =
        (int64_t)array[0] - (int64_t)target;

    if (best_distance < 0)
    {
        best_distance = -best_distance;
    }

    for (index = 1U; index < length; index++)
    {
        int64_t distance =
            (int64_t)array[index] - (int64_t)target;

        if (distance < 0)
        {
            distance = -distance;
        }

        if (distance < best_distance)
        {
            best_distance = distance;
            best_index = index;
        }
    }

    return (ptrdiff_t)best_index;
}
```

Chuyển sang `int64_t` trước khi trừ để tránh overflow với `INT_MIN` và `INT_MAX`.

---

# 14. Linear Search theo điều kiện

Không phải lúc nào cũng tìm theo phép bằng.

Ví dụ:

- Phần tử đầu tiên lớn hơn `100`.
- Sensor đầu tiên đang lỗi.
- Packet đầu tiên hết timeout.
- Slot đầu tiên đang rảnh.

## Predicate API

```c
typedef bool (*int_predicate_t)(
    int value,
    size_t index,
    void *context);

ptrdiff_t linear_find_if(
    const int *array,
    size_t length,
    int_predicate_t predicate,
    void *context)
{
    size_t index;

    if (((array == NULL) && (length > 0U)) ||
        (predicate == NULL))
    {
        return -1;
    }

    for (index = 0U; index < length; index++)
    {
        if (predicate(array[index], index, context))
        {
            return (ptrdiff_t)index;
        }
    }

    return -1;
}
```

Ví dụ predicate:

```c
typedef struct
{
    int threshold;
} threshold_context_t;

static bool greater_than_threshold(
    int value,
    size_t index,
    void *context)
{
    const threshold_context_t *config =
        (const threshold_context_t *)context;

    (void)index;

    return value > config->threshold;
}
```

---

# 15. Sentinel Linear Search

Sentinel Search đặt `target` tạm thời ở cuối mảng để vòng lặp không phải kiểm tra biên ở mỗi bước.

```c
ptrdiff_t sentinel_linear_search(
    int *array,
    size_t length,
    int target)
{
    int last_value;
    size_t index = 0U;

    if ((array == NULL) || (length == 0U))
    {
        return -1;
    }

    last_value = array[length - 1U];
    array[length - 1U] = target;

    while (array[index] != target)
    {
        index++;
    }

    array[length - 1U] = last_value;

    if ((index < (length - 1U)) ||
        (last_value == target))
    {
        return (ptrdiff_t)index;
    }

    return -1;
}
```

## Ưu điểm

Vòng lặp chỉ kiểm tra:

```c
array[index] != target
```

## Hạn chế

- Tạm thời sửa mảng.
- Không dùng với dữ liệu `const`.
- Không phù hợp Flash/ROM chỉ đọc.
- Không an toàn nếu task khác, ISR hoặc DMA cùng truy cập.
- Lợi ích thực tế có thể rất nhỏ.
- Chỉ nên dùng sau khi benchmark.

---

# 16. Linear Search đệ quy

```c
ptrdiff_t linear_search_recursive(
    const int *array,
    size_t length,
    size_t index,
    int target)
{
    if (((array == NULL) && (length > 0U)) ||
        (index >= length))
    {
        return -1;
    }

    if (array[index] == target)
    {
        return (ptrdiff_t)index;
    }

    return linear_search_recursive(
        array,
        length,
        index + 1U,
        target);
}
```

Độ phức tạp thời gian:

```text
O(n)
```

Bộ nhớ Call Stack:

```text
O(n)
```

Phiên bản đệ quy không có lợi thế so với vòng lặp và không phù hợp với dữ liệu lớn hoặc firmware có Stack nhỏ.

---

# 17. Tìm kiếm trong chuỗi

## Tìm một ký tự

```c
ptrdiff_t linear_search_character(
    const char *text,
    char target)
{
    size_t index;

    if (text == NULL)
    {
        return -1;
    }

    for (index = 0U; text[index] != '\0'; index++)
    {
        if (text[index] == target)
        {
            return (ptrdiff_t)index;
        }
    }

    return -1;
}
```

Thư viện C đã có hàm tương tự:

```c
strchr()
```

## Tìm chuỗi con kiểu đơn giản

Thử khớp pattern tại từng vị trí của text.

Worst-case:

```text
O(n × m)
```

với:

```text
n = độ dài text
m = độ dài pattern
```

Với dữ liệu lớn, cân nhắc:

- KMP.
- Rabin–Karp.
- Boyer–Moore.
- Z Algorithm.

---

# 18. Tìm kiếm trên struct

```c
typedef struct
{
    unsigned int id;
    const char *name;
    float score;
} student_t;
```

## Tìm theo ID

```c
ptrdiff_t find_student_by_id(
    const student_t *students,
    size_t count,
    unsigned int target_id)
{
    size_t index;

    if ((students == NULL) && (count > 0U))
    {
        return -1;
    }

    for (index = 0U; index < count; index++)
    {
        if (students[index].id == target_id)
        {
            return (ptrdiff_t)index;
        }
    }

    return -1;
}
```

## Tìm theo tên

```c
#include <string.h>

ptrdiff_t find_student_by_name(
    const student_t *students,
    size_t count,
    const char *target_name)
{
    size_t index;

    if (((students == NULL) && (count > 0U)) ||
        (target_name == NULL))
    {
        return -1;
    }

    for (index = 0U; index < count; index++)
    {
        if ((students[index].name != NULL) &&
            (strcmp(students[index].name, target_name) == 0))
        {
            return (ptrdiff_t)index;
        }
    }

    return -1;
}
```

Không dùng:

```c
students[index].name == target_name
```

vì đó là so sánh địa chỉ con trỏ, không phải nội dung chuỗi.

---

# 19. Generic Linear Search

```c
typedef bool (*match_function_t)(
    const void *element,
    const void *key,
    void *context);

ptrdiff_t linear_search_generic(
    const void *base,
    size_t count,
    size_t element_size,
    const void *key,
    match_function_t match,
    void *context)
{
    const unsigned char *bytes =
        (const unsigned char *)base;

    size_t index;

    if (((base == NULL) && (count > 0U)) ||
        (element_size == 0U) ||
        (match == NULL))
    {
        return -1;
    }

    for (index = 0U; index < count; index++)
    {
        const void *element =
            &bytes[index * element_size];

        if (match(element, key, context))
        {
            return (ptrdiff_t)index;
        }
    }

    return -1;
}
```

Comparator cho `int`:

```c
static bool int_matches(
    const void *element,
    const void *key,
    void *context)
{
    const int value = *(const int *)element;
    const int target = *(const int *)key;

    (void)context;

    return value == target;
}
```

## Trade-off

Ưu điểm:

- Tái sử dụng cho nhiều kiểu dữ liệu.
- Hỗ trợ predicate tùy biến.

Nhược điểm:

- Mất type safety.
- Function pointer có overhead.
- Khó inline.
- Code phức tạp hơn.

Trong embedded, hàm chuyên biệt cho từng kiểu thường đơn giản và tối ưu hơn.

---

# 20. Tìm kiếm trong Linked List

```c
typedef struct node
{
    int data;
    struct node *next;
} node_t;
```

## Tìm node

```c
node_t *linked_list_find(
    node_t *head,
    int target)
{
    node_t *current = head;

    while (current != NULL)
    {
        if (current->data == target)
        {
            return current;
        }

        current = current->next;
    }

    return NULL;
}
```

## Tìm index logic

```c
ptrdiff_t linked_list_find_index(
    const node_t *head,
    int target)
{
    const node_t *current = head;
    ptrdiff_t index = 0;

    while (current != NULL)
    {
        if (current->data == target)
        {
            return index;
        }

        current = current->next;
        index++;
    }

    return -1;
}
```

Linear Search phù hợp với Linked List vì các node vốn phải được truy cập tuần tự.

---

# 21. Tìm kiếm trong ma trận

```c
bool linear_search_matrix(
    const int *matrix,
    size_t rows,
    size_t columns,
    int target,
    size_t *found_row,
    size_t *found_column)
{
    size_t row;
    size_t column;

    if ((matrix == NULL) ||
        (found_row == NULL) ||
        (found_column == NULL) ||
        (rows == 0U) ||
        (columns == 0U))
    {
        return false;
    }

    for (row = 0U; row < rows; row++)
    {
        for (column = 0U; column < columns; column++)
        {
            if (matrix[(row * columns) + column] == target)
            {
                *found_row = row;
                *found_column = column;
                return true;
            }
        }
    }

    return false;
}
```

Độ phức tạp:

```text
O(rows × columns)
```

Nếu ma trận có thứ tự đặc biệt, có thể dùng thuật toán nhanh hơn.

---

# 22. Tối ưu khi dữ liệu đã sắp xếp

Với mảng tăng dần, có thể dừng khi giá trị hiện tại lớn hơn target:

```c
ptrdiff_t linear_search_sorted(
    const int *array,
    size_t length,
    int target)
{
    size_t index;

    if ((array == NULL) && (length > 0U))
    {
        return -1;
    }

    for (index = 0U; index < length; index++)
    {
        if (array[index] == target)
        {
            return (ptrdiff_t)index;
        }

        if (array[index] > target)
        {
            break;
        }
    }

    return -1;
}
```

Worst-case vẫn là:

```text
O(n)
```

Nếu có nhiều truy vấn trên mảng lớn đã sort, Binary Search phù hợp hơn.

---

# 23. Ứng dụng của Linear Search

Linear Search thường được dùng trong:

- Mảng chưa sắp xếp.
- Tập dữ liệu nhỏ.
- Linked List.
- Tìm kiếm một lần.
- Tìm phần tử đầu tiên thỏa điều kiện.
- Tìm min/max.
- Tìm slot trống.
- Tìm resource đang rảnh.
- Tìm command.
- Tìm sensor theo ID.
- Tìm packet theo sequence number.
- Tìm task theo handle.
- Tìm ký tự hoặc chuỗi.
- Quét vùng nhớ.
- Kiểm tra duplicate.
- Duyệt bảng callback.

---

# 24. Linear Search trong embedded

## Tập dữ liệu nhỏ

Firmware thường có các bảng:

```text
4 command
8 sensor
12 state
16 timer
```

Linear Search có ưu điểm:

- Code nhỏ.
- Dễ hiểu.
- Dễ kiểm thử.
- Không cần sort.
- Không cần RAM phụ.
- Worst-case rõ ràng.

Nếu bảng có tối đa tám phần tử:

```text
Worst-case = 8 phép kiểm tra
```

Đây có thể là lựa chọn tốt hơn một cấu trúc phức tạp.

## Bảng const trong Flash

```c
static const uint16_t supported_ids[] =
{
    0x0101U,
    0x0203U,
    0x1000U,
    0x2001U
};
```

Có thể duyệt trực tiếp mà không cần copy sang RAM.

## Tránh cấp phát động

Linear Search chỉ cần:

```text
O(1)
```

bộ nhớ phụ.

---

# 25. Bảng command và bảng calibration

## Bảng command

```c
typedef void (*command_handler_t)(
    const char *arguments);

typedef struct
{
    const char *name;
    command_handler_t handler;
} command_entry_t;
```

```c
static const command_entry_t command_table[] =
{
    { "help", command_help },
    { "status", command_status },
    { "reset", command_reset },
    { "version", command_version }
};
```

Tìm command:

```c
#include <string.h>

const command_entry_t *command_find(
    const char *name)
{
    size_t index;
    const size_t count =
        sizeof(command_table) /
        sizeof(command_table[0]);

    if (name == NULL)
    {
        return NULL;
    }

    for (index = 0U; index < count; index++)
    {
        if (strcmp(command_table[index].name, name) == 0)
        {
            return &command_table[index];
        }
    }

    return NULL;
}
```

## Bảng calibration

```c
typedef struct
{
    uint16_t raw_value;
    int16_t physical_value;
} calibration_point_t;
```

Với bảng nhỏ, có thể tìm điểm gần nhất bằng Linear Search. Với bảng lớn đã sort, Binary Search kết hợp interpolation tốt hơn.

---

# 26. Interrupt, DMA và concurrency

Linear Search không tự thread-safe.

Nếu ISR, DMA hoặc task khác sửa dữ liệu trong lúc tìm kiếm:

- Kết quả có thể không nhất quán.
- Có thể đọc object đang được cập nhật một phần.
- Chiều dài logic có thể thay đổi.
- Thứ tự dữ liệu có thể bị phá vỡ.

Giải pháp:

- Ownership rõ ràng.
- Snapshot dữ liệu.
- Double buffering.
- Critical Section.
- Mutex.
- Chờ DMA hoàn tất.
- Immutable lookup table.

Không nên thực hiện Linear Search dài trong ISR.

ISR nên:

```text
nhận dữ liệu -> lưu buffer/post event -> thoát
```

Task hoặc main loop thực hiện tìm kiếm.

---

# 27. Các lỗi thường gặp

## Lỗi điều kiện vòng lặp

Sai:

```c
for (index = 0U; index <= length; index++)
```

Đúng:

```c
index < length
```

## Dùng `sizeof` trên con trỏ

Sai:

```c
void function(int *array)
{
    size_t length =
        sizeof(array) / sizeof(array[0]);
}
```

Trong hàm, `array` chỉ là con trỏ. Phải truyền `length` riêng.

## Trả `-1` bằng `size_t`

```c
size_t result = -1;
```

tạo một giá trị rất lớn. Dùng:

- `ptrdiff_t`.
- `bool + output`.
- Hoặc mô tả rõ sentinel `SIZE_MAX`.

## Không dừng khi đã tìm thấy

Nếu cần lần đầu, phải return ngay.

## So sánh chuỗi bằng `==`

Phải dùng:

```c
strcmp()
```

## So sánh số thực bằng `==`

Có thể cần tolerance:

```c
fabs(value - target) <= epsilon
```

## Tràn số khi tính khoảng cách

Chuyển sang kiểu rộng hơn trước khi trừ.

## Sentinel Search sửa dữ liệu dùng chung

Có thể gây race condition hoặc làm task khác đọc dữ liệu giả.

## Đệ quy không cần thiết

Tăng nguy cơ Program Stack Overflow.

## Ghi vượt output buffer

Khi tìm tất cả vị trí, phải kiểm tra capacity.

---

# 28. Khi nào nên dùng?

Nên dùng Linear Search khi:

- Dữ liệu chưa sort.
- Tập dữ liệu nhỏ.
- Chỉ có một hoặc vài truy vấn.
- Dữ liệu thay đổi thường xuyên.
- Dùng Linked List.
- Cần code đơn giản.
- RAM và code size hạn chế.
- Target thường nằm gần đầu.
- Dữ liệu chỉ đọc tuần tự.
- Worst-case `O(n)` vẫn đáp ứng timing.

---

# 29. Khi nào không nên dùng?

Cân nhắc giải pháp khác khi:

- Mảng rất lớn.
- Có nhiều truy vấn lặp lại.
- Dữ liệu đã sort.
- Cần lookup gần `O(1)`.
- ISR không thể chấp nhận quét dài.
- Có thể xây index trước.
- Cần tìm prefix hoặc pattern lớn.

Lựa chọn khác:

- Binary Search.
- Hash Table.
- Direct Address Table.
- Binary Search Tree.
- Trie.
- KMP hoặc Boyer–Moore.
- Bloom Filter.

---

# 30. Kiểm thử

## `test_linear_search.c`

```c
#include <assert.h>
#include <limits.h>

#include "linear_search.h"

static void test_empty_array(void)
{
    assert(linear_search_first(NULL, 0U, 10) == -1);
    assert(linear_search_count(NULL, 0U, 10) == 0U);
}

static void test_single_element(void)
{
    const int array[] = { 10 };

    assert(linear_search_first(array, 1U, 10) == 0);
    assert(linear_search_first(array, 1U, 5) == -1);
    assert(linear_search_last(array, 1U, 10) == 0);
}

static void test_first_and_last(void)
{
    const int array[] = { 4, 2, 7, 2, 9 };

    assert(linear_search_first(array, 5U, 2) == 1);
    assert(linear_search_last(array, 5U, 2) == 3);
}

static void test_not_found(void)
{
    const int array[] = { 10, 8, 30 };

    assert(linear_search_first(array, 3U, 6) == -1);
}

static void test_count(void)
{
    const int array[] = { 5, 2, 5, 8, 5 };

    assert(linear_search_count(array, 5U, 5) == 3U);
    assert(linear_search_count(array, 5U, 7) == 0U);
}

static void test_extreme_values(void)
{
    const int array[] =
    {
        INT_MIN,
        -1,
        0,
        1,
        INT_MAX
    };

    assert(linear_search_first(array, 5U, INT_MIN) == 0);
    assert(linear_search_first(array, 5U, INT_MAX) == 4);
}

static void test_boolean_api(void)
{
    const int array[] = { 2, 4, 6, 8 };
    size_t index = 0U;

    assert(linear_search_find(array, 4U, 6, &index));
    assert(index == 2U);

    assert(!linear_search_find(array, 4U, 7, &index));
}

int main(void)
{
    test_empty_array();
    test_single_element();
    test_first_and_last();
    test_not_found();
    test_count();
    test_extreme_values();
    test_boolean_api();

    return 0;
}
```

Biên dịch:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic \
    -fsanitize=address,undefined \
    src/linear_search.c tests/test_linear_search.c \
    -Iinc -o linear_search_tests
```

Các trường hợp cần kiểm thử:

1. Mảng rỗng.
2. Một phần tử.
3. Target ở đầu.
4. Target ở cuối.
5. Target ở giữa.
6. Target không tồn tại.
7. Nhiều duplicate.
8. Tất cả phần tử bằng nhau.
9. Số âm.
10. `INT_MIN`.
11. `INT_MAX`.
12. Output buffer nhỏ hơn số kết quả.
13. Linked List rỗng.
14. Struct chứa chuỗi `NULL`.
15. Dữ liệu bị cập nhật đồng thời.

---

# 31. Bài tập luyện tập

## Cơ bản

1. Cài Linear Search.
2. Tìm lần xuất hiện đầu tiên.
3. Tìm lần xuất hiện cuối cùng.
4. Đếm số lần xuất hiện.
5. Tìm tất cả index.
6. Tìm min và max.
7. Tìm số chẵn đầu tiên.
8. Tìm số âm đầu tiên.
9. Tìm ký tự trong chuỗi.
10. Tìm student theo ID.
11. Tìm node trong Linked List.
12. Tìm phần tử trong ma trận.

## Trung bình

1. Sentinel Linear Search.
2. Generic Linear Search.
3. Tìm phần tử gần target nhất.
4. Tìm duplicate đầu tiên.
5. Tìm phần tử xuất hiện đúng một lần.
6. Tìm phần tử lớn thứ hai.
7. Naive Pattern Search.
8. Move-to-Front Search.
9. Transpose Search.
10. Tìm command trong command table.
11. Tìm calibration point gần nhất.
12. Tìm slot trống đầu tiên.

## Nâng cao

1. Benchmark Linear Search và Binary Search.
2. Branchless Linear Search.
3. Loop Unrolling.
4. Vectorized Search.
5. Parallel Linear Search.
6. Search dữ liệu DMA bằng snapshot.
7. Tìm nhiều target trong một lượt.
8. Xây Bloom Filter trước Linear Search.
9. Search trên file theo chunk.
10. Chọn cấu trúc lookup theo workload.

---

# 32. Cấu trúc repository đề xuất

```text
linear-search/
├── README.md
├── inc/
│   └── linear_search.h
├── src/
│   ├── linear_search.c
│   ├── sentinel_search.c
│   ├── generic_search.c
│   └── linked_list_search.c
├── examples/
│   ├── basic/
│   │   └── main.c
│   ├── command_table/
│   │   └── command_table.c
│   ├── calibration/
│   │   └── calibration_search.c
│   └── generic/
│       └── generic_search_example.c
├── tests/
│   └── test_linear_search.c
└── CMakeLists.txt
```

## `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.16)

project(linear_search C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(linear_search
    src/linear_search.c
    src/sentinel_search.c
    src/generic_search.c
    src/linked_list_search.c
)

target_include_directories(linear_search
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_compile_options(linear_search
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

add_executable(linear_search_tests
    tests/test_linear_search.c
)

target_link_libraries(linear_search_tests
    PRIVATE
        linear_search
)
```

Build:

```bash
cmake -S . -B build
cmake --build build
./build/linear_search_tests
```

---

# 33. Tóm tắt

Các điểm quan trọng:

- Linear Search còn gọi là Sequential Search.
- Thuật toán kiểm tra lần lượt từng phần tử.
- Không yêu cầu dữ liệu được sắp xếp.
- Best-case là `O(1)`.
- Average và worst-case là `O(n)`.
- Phiên bản lặp dùng `O(1)` bộ nhớ phụ.
- Linear Search phù hợp với mảng nhỏ và Linked List.
- Tìm lần đầu có thể dừng ngay.
- Đếm hoặc tìm tất cả vị trí phải duyệt hết.
- Sentinel Search giảm kiểm tra biên nhưng sửa mảng.
- Generic Search linh hoạt nhưng giảm type safety.
- Truy cập tuần tự có cache locality tốt.
- Trong embedded, thuật toán có code nhỏ và worst-case dễ dự đoán.
- Không nên thực hiện tìm kiếm dài trong ISR.
- Phải bảo vệ dữ liệu nếu ISR, DMA hoặc task khác thay đổi.
- Không dùng `sizeof(pointer)` để tính số phần tử.
- Không so sánh chuỗi bằng `==`.
- Với nhiều truy vấn trên dữ liệu lớn, cân nhắc Binary Search hoặc Hash Table.

---

## Tài liệu tham khảo

README này được tổng hợp và viết lại dựa trên:

- **Linear Search Algorithm**, GeeksforGeeks:  
  https://www.geeksforgeeks.org/dsa/linear-search/

Nội dung không sao chép nguyên văn toàn bộ trang nguồn. Phần giải thích, sơ đồ và mã C được biên soạn theo cấu trúc độc lập, đồng thời bổ sung Sentinel Search, Generic Search, tìm kiếm trên struct, Linked List, ứng dụng embedded và kiểm thử.
