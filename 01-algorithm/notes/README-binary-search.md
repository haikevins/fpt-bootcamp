# Binary Search

> Tài liệu học tập độc lập về **Binary Search**, trình bày nguyên lý, cách cài đặt bằng C, các biến thể quan trọng và những lỗi thường gặp.

---

## Mục lục

1. [Binary Search là gì?](#1-binary-search-là-gì)
2. [Điều kiện áp dụng](#2-điều-kiện-áp-dụng)
3. [Ý tưởng cốt lõi](#3-ý-tưởng-cốt-lõi)
4. [Minh họa từng bước](#4-minh-họa-từng-bước)
5. [Binary Search dạng lặp](#5-binary-search-dạng-lặp)
6. [Binary Search dạng đệ quy](#6-binary-search-dạng-đệ-quy)
7. [Độ phức tạp](#7-độ-phức-tạp)
8. [Tại sao tính `mid` theo công thức an toàn?](#8-tại-sao-tính-mid-theo-công-thức-an-toàn)
9. [Các quy ước khoảng tìm kiếm](#9-các-quy-ước-khoảng-tìm-kiếm)
10. [Tìm lần xuất hiện đầu tiên](#10-tìm-lần-xuất-hiện-đầu-tiên)
11. [Tìm lần xuất hiện cuối cùng](#11-tìm-lần-xuất-hiện-cuối-cùng)
12. [Lower Bound](#12-lower-bound)
13. [Upper Bound](#13-upper-bound)
14. [Đếm số lần xuất hiện](#14-đếm-số-lần-xuất-hiện)
15. [Tìm phần tử gần nhất](#15-tìm-phần-tử-gần-nhất)
16. [Binary Search trên mảng giảm dần](#16-binary-search-trên-mảng-giảm-dần)
17. [Binary Search trên mảng xoay](#17-binary-search-trên-mảng-xoay)
18. [Tìm căn bậc hai nguyên](#18-tìm-căn-bậc-hai-nguyên)
19. [Binary Search trong không gian không giới hạn](#19-binary-search-trong-không-gian-không-giới-hạn)
20. [Binary Search on Answer](#20-binary-search-on-answer)
21. [Binary Search với số thực](#21-binary-search-với-số-thực)
22. [Ứng dụng](#22-ứng-dụng)
23. [Binary Search trong lập trình nhúng](#23-binary-search-trong-lập-trình-nhúng)
24. [Lỗi thường gặp](#24-lỗi-thường-gặp)
25. [Bài tập luyện tập](#25-bài-tập-luyện-tập)
26. [Tổng kết](#26-tổng-kết)
27. [Tài liệu tham khảo](#27-tài-liệu-tham-khảo)

---

## 1. Binary Search là gì?

**Binary Search** là thuật toán tìm kiếm trên một không gian đã có thứ tự hoặc có tính đơn điệu.

Ở mỗi bước, thuật toán:

1. Chọn phần tử ở giữa.
2. So sánh phần tử giữa với giá trị cần tìm.
3. Loại bỏ một nửa không thể chứa đáp án.
4. Tiếp tục trên nửa còn lại.

Vì không gian tìm kiếm giảm còn một nửa sau mỗi bước, thời gian tìm kiếm là:

```text
O(log n)
```

Ví dụ mảng đã sắp xếp:

```text
[2, 5, 8, 12, 16, 23, 38, 56, 72, 91]
```

Tìm:

```text
target = 23
```

---

## 2. Điều kiện áp dụng

Binary Search thường được áp dụng khi thỏa mãn hai điều kiện quan trọng.

### 2.1. Dữ liệu có thứ tự

Ví dụ tăng dần:

```text
[2, 5, 8, 12, 16, 23, 38]
```

Hoặc giảm dần:

```text
[38, 23, 16, 12, 8, 5, 2]
```

Nếu dữ liệu chưa sắp xếp:

```text
[12, 2, 38, 5, 23, 8]
```

việc loại bỏ một nửa dựa trên giá trị giữa không còn chính xác.

### 2.2. Có thể truy cập phần tử giữa hiệu quả

Với mảng:

```c
array[mid]
```

có thời gian truy cập:

```text
O(1)
```

Với Singly Linked List, muốn đến Node thứ `mid` phải duyệt từ đầu:

```text
O(mid)
```

Do đó Binary Search không hiệu quả trên Linked List thông thường.

### 2.3. Không gian đáp án có tính đơn điệu

Binary Search không chỉ dùng để tìm một giá trị trong mảng.

Nó còn áp dụng khi một hàm kiểm tra có dạng:

```text
false false false true true true
```

hoặc:

```text
true true true false false false
```

Ví dụ:

```text
Có thể hoàn thành công việc trong T giờ không?
```

Nếu làm được trong `T`, thường cũng làm được trong mọi thời gian lớn hơn `T`.

---

## 3. Ý tưởng cốt lõi

Giả sử khoảng tìm kiếm là:

```text
low ... high
```

Tính vị trí giữa:

```c
mid = low + (high - low) / 2;
```

So sánh:

```text
array[mid] == target
array[mid] < target
array[mid] > target
```

Với mảng tăng dần:

```text
Nếu array[mid] == target:
    tìm thấy đáp án

Nếu array[mid] < target:
    target chỉ có thể nằm bên phải
    low = mid + 1

Nếu array[mid] > target:
    target chỉ có thể nằm bên trái
    high = mid - 1
```

Thuật toán kết thúc khi:

```text
low > high
```

Khi đó không gian tìm kiếm đã rỗng.

---

## 4. Minh họa từng bước

Mảng:

```text
Index:   0   1   2    3    4    5    6    7    8    9
Array:  [2,  5,  8,  12,  16,  23,  38,  56,  72,  91]
```

Tìm:

```text
target = 23
```

### Bước 1

```text
low  = 0
high = 9
mid  = 0 + (9 - 0) / 2 = 4
```

```text
array[4] = 16
```

Vì:

```text
16 < 23
```

loại bỏ nửa trái và phần tử giữa:

```text
low = mid + 1 = 5
```

Khoảng còn lại:

```text
[23, 38, 56, 72, 91]
```

### Bước 2

```text
low  = 5
high = 9
mid  = 5 + (9 - 5) / 2 = 7
```

```text
array[7] = 56
```

Vì:

```text
56 > 23
```

chỉ giữ nửa trái:

```text
high = mid - 1 = 6
```

Khoảng còn lại:

```text
[23, 38]
```

### Bước 3

```text
low  = 5
high = 6
mid  = 5 + (6 - 5) / 2 = 5
```

```text
array[5] = 23
```

Tìm thấy:

```text
index = 5
```

---

## 5. Binary Search dạng lặp

### 5.1. Mã C

```c
#include <stddef.h>

int BinarySearch(
    const int array[],
    size_t size,
    int target
)
{
    size_t low = 0U;
    size_t high = size;

    if ((array == NULL) || (size == 0U))
    {
        return -1;
    }

    /*
     * Search interval: [low, high)
     * high is one position after the final valid index.
     */
    while (low < high)
    {
        size_t mid = low + (high - low) / 2U;

        if (array[mid] == target)
        {
            return (int)mid;
        }

        if (array[mid] < target)
        {
            low = mid + 1U;
        }
        else
        {
            high = mid;
        }
    }

    return -1;
}
```

Cách trên sử dụng khoảng nửa mở:

```text
[low, high)
```

Ban đầu:

```text
low  = 0
high = size
```

Điều kiện lặp:

```c
low < high
```

### 5.2. Ví dụ sử dụng

```c
#include <stdio.h>

int main(void)
{
    const int array[] = {2, 3, 4, 10, 40};
    const size_t size =
        sizeof(array) / sizeof(array[0]);

    int result = BinarySearch(
        array,
        size,
        10
    );

    if (result >= 0)
    {
        printf(
            "Element found at index %d\n",
            result
        );
    }
    else
    {
        printf("Element not found\n");
    }

    return 0;
}
```

Kết quả:

```text
Element found at index 3
```

---

## 6. Binary Search dạng đệ quy

### 6.1. Mã C

```c
#include <stddef.h>

static int BinarySearchRecursiveRange(
    const int array[],
    size_t low,
    size_t high,
    int target
)
{
    size_t mid;

    if (low >= high)
    {
        return -1;
    }

    mid = low + (high - low) / 2U;

    if (array[mid] == target)
    {
        return (int)mid;
    }

    if (array[mid] < target)
    {
        return BinarySearchRecursiveRange(
            array,
            mid + 1U,
            high,
            target
        );
    }

    return BinarySearchRecursiveRange(
        array,
        low,
        mid,
        target
    );
}

int BinarySearchRecursive(
    const int array[],
    size_t size,
    int target
)
{
    if ((array == NULL) || (size == 0U))
    {
        return -1;
    }

    return BinarySearchRecursiveRange(
        array,
        0U,
        size,
        target
    );
}
```

### 6.2. Đặc điểm

Mỗi lời gọi chỉ tiếp tục với một nửa:

```text
n
n / 2
n / 4
n / 8
...
1
```

Số lời gọi xấp xỉ:

```text
log₂(n)
```

Bản đệ quy dễ thể hiện ý tưởng, nhưng cần thêm Call Stack.

---

## 7. Độ phức tạp

### Thời gian

| Trường hợp | Độ phức tạp |
|---|---:|
| Best case | `O(1)` |
| Average case | `O(log n)` |
| Worst case | `O(log n)` |

Best case xảy ra khi:

```text
target == array[mid]
```

ngay ở lần so sánh đầu tiên.

### Bộ nhớ phụ

Bản lặp:

```text
O(1)
```

Bản đệ quy:

```text
O(log n)
```

do Call Stack.

### Số bước tối đa

Với `n` phần tử, số lần chia đôi gần bằng:

```text
⌊log₂(n)⌋ + 1
```

Ví dụ:

```text
n = 1,000,000
log₂(n) ≈ 19.93
```

Chỉ cần khoảng 20 lần so sánh trong trường hợp điển hình.

---

## 8. Tại sao tính `mid` theo công thức an toàn?

Công thức trực tiếp:

```c
mid = (low + high) / 2;
```

có thể overflow nếu:

```text
low + high > giá trị tối đa của kiểu dữ liệu
```

Công thức an toàn hơn:

```c
mid = low + (high - low) / 2;
```

Về toán học, hai công thức tương đương:

```text
low + (high - low) / 2
= low + high/2 - low/2
≈ (low + high) / 2
```

Nhưng biểu thức:

```text
high - low
```

không lớn hơn độ dài khoảng tìm kiếm.

Với khoảng nửa mở `[low, high)`, cần đảm bảo:

```text
low <= high
```

để phép trừ unsigned không bị underflow.

---

## 9. Các quy ước khoảng tìm kiếm

Có hai quy ước phổ biến.

## 9.1. Khoảng đóng `[low, high]`

Hai đầu đều thuộc vùng tìm kiếm.

Khởi tạo:

```c
low = 0;
high = size - 1;
```

Điều kiện:

```c
while (low <= high)
```

Cập nhật:

```c
low = mid + 1;
high = mid - 1;
```

Cách này cần cẩn thận với `size_t`, vì:

```c
mid - 1
```

có thể underflow khi `mid == 0`.

---

## 9.2. Khoảng nửa mở `[low, high)`

`low` thuộc khoảng, `high` không thuộc khoảng.

Khởi tạo:

```c
low = 0;
high = size;
```

Điều kiện:

```c
while (low < high)
```

Nửa trái:

```text
[low, mid)
```

Nửa phải:

```text
[mid + 1, high)
```

Hoặc với lower bound:

```text
[low, mid)
[mid, high)
```

Khoảng nửa mở thường thuận tiện khi dùng `size_t`.

Điều quan trọng nhất là không trộn hai quy ước trong cùng một hàm.

---

## 10. Tìm lần xuất hiện đầu tiên

Mảng có phần tử trùng:

```text
[1, 2, 2, 2, 2, 3, 4]
```

Tìm `2`.

Binary Search thông thường có thể trả bất kỳ index nào trong:

```text
1, 2, 3, 4
```

Muốn tìm lần xuất hiện đầu tiên, khi gặp target vẫn tiếp tục tìm bên trái.

### Mã C

```c
#include <stddef.h>

int BinarySearchFirst(
    const int array[],
    size_t size,
    int target
)
{
    size_t low = 0U;
    size_t high = size;
    int result = -1;

    if (array == NULL)
    {
        return -1;
    }

    while (low < high)
    {
        size_t mid = low + (high - low) / 2U;

        if (array[mid] >= target)
        {
            if (array[mid] == target)
            {
                result = (int)mid;
            }

            high = mid;
        }
        else
        {
            low = mid + 1U;
        }
    }

    return result;
}
```

Ý tưởng:

```text
Nếu array[mid] >= target:
    đáp án có thể là mid hoặc nằm bên trái
```

---

## 11. Tìm lần xuất hiện cuối cùng

Khi gặp target, tiếp tục tìm bên phải.

```c
#include <stddef.h>

int BinarySearchLast(
    const int array[],
    size_t size,
    int target
)
{
    size_t low = 0U;
    size_t high = size;
    int result = -1;

    if (array == NULL)
    {
        return -1;
    }

    while (low < high)
    {
        size_t mid = low + (high - low) / 2U;

        if (array[mid] <= target)
        {
            if (array[mid] == target)
            {
                result = (int)mid;
            }

            low = mid + 1U;
        }
        else
        {
            high = mid;
        }
    }

    return result;
}
```

---

## 12. Lower Bound

**Lower Bound** là vị trí đầu tiên có giá trị:

```text
>= target
```

Ví dụ:

```text
array  = [1, 2, 2, 2, 4, 7]
target = 2
```

Kết quả:

```text
index = 1
```

Với:

```text
target = 3
```

Kết quả:

```text
index = 4
```

vì `array[4] = 4` là phần tử đầu tiên không nhỏ hơn `3`.

### Mã C

```c
#include <stddef.h>

size_t LowerBound(
    const int array[],
    size_t size,
    int target
)
{
    size_t low = 0U;
    size_t high = size;

    if (array == NULL)
    {
        return 0U;
    }

    while (low < high)
    {
        size_t mid = low + (high - low) / 2U;

        if (array[mid] < target)
        {
            low = mid + 1U;
        }
        else
        {
            high = mid;
        }
    }

    return low;
}
```

Kết quả có thể bằng:

```text
size
```

Điều này có nghĩa là không có phần tử nào `>= target`.

---

## 13. Upper Bound

**Upper Bound** là vị trí đầu tiên có giá trị:

```text
> target
```

Ví dụ:

```text
array  = [1, 2, 2, 2, 4, 7]
target = 2
```

Kết quả:

```text
index = 4
```

### Mã C

```c
#include <stddef.h>

size_t UpperBound(
    const int array[],
    size_t size,
    int target
)
{
    size_t low = 0U;
    size_t high = size;

    if (array == NULL)
    {
        return 0U;
    }

    while (low < high)
    {
        size_t mid = low + (high - low) / 2U;

        if (array[mid] <= target)
        {
            low = mid + 1U;
        }
        else
        {
            high = mid;
        }
    }

    return low;
}
```

---

## 14. Đếm số lần xuất hiện

Số phần tử bằng `target`:

```text
upper_bound(target) - lower_bound(target)
```

### Mã C

```c
#include <stddef.h>

size_t CountOccurrences(
    const int array[],
    size_t size,
    int target
)
{
    size_t first;
    size_t after_last;

    if (array == NULL)
    {
        return 0U;
    }

    first = LowerBound(
        array,
        size,
        target
    );

    after_last = UpperBound(
        array,
        size,
        target
    );

    return after_last - first;
}
```

Ví dụ:

```text
array  = [1, 2, 2, 2, 4]
target = 2
```

```text
lower_bound = 1
upper_bound = 4
count       = 4 - 1 = 3
```

---

## 15. Tìm phần tử gần nhất

Sau khi tìm lower bound:

```text
position = phần tử đầu tiên >= target
```

Hai ứng viên gần nhất là:

```text
array[position]
array[position - 1]
```

nếu các index tồn tại.

### Mã C

```c
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>

bool FindClosest(
    const int array[],
    size_t size,
    int target,
    int *closest
)
{
    size_t position;

    if ((array == NULL) ||
        (closest == NULL) ||
        (size == 0U))
    {
        return false;
    }

    position = LowerBound(
        array,
        size,
        target
    );

    if (position == 0U)
    {
        *closest = array[0];
        return true;
    }

    if (position == size)
    {
        *closest = array[size - 1U];
        return true;
    }

    {
        long long left_distance =
            llabs((long long)array[position - 1U] -
                  (long long)target);

        long long right_distance =
            llabs((long long)array[position] -
                  (long long)target);

        if (left_distance <= right_distance)
        {
            *closest = array[position - 1U];
        }
        else
        {
            *closest = array[position];
        }
    }

    return true;
}
```

Cần thêm:

```c
#include <stdbool.h>
```

---

## 16. Binary Search trên mảng giảm dần

Với mảng:

```text
[100, 80, 60, 40, 20]
```

Quy tắc cập nhật bị đảo.

```c
#include <stddef.h>

int BinarySearchDescending(
    const int array[],
    size_t size,
    int target
)
{
    size_t low = 0U;
    size_t high = size;

    if ((array == NULL) || (size == 0U))
    {
        return -1;
    }

    while (low < high)
    {
        size_t mid = low + (high - low) / 2U;

        if (array[mid] == target)
        {
            return (int)mid;
        }

        if (array[mid] > target)
        {
            low = mid + 1U;
        }
        else
        {
            high = mid;
        }
    }

    return -1;
}
```

Vì mảng giảm dần:

```text
array[mid] > target
```

nghĩa là target nằm bên phải.

---

## 17. Binary Search trên mảng xoay

Mảng tăng dần bị xoay:

```text
[4, 5, 6, 7, 0, 1, 2]
```

Mỗi lần xét `mid`, ít nhất một nửa vẫn được sắp xếp.

### Mã C không có phần tử trùng

```c
#include <stddef.h>

int SearchRotatedArray(
    const int array[],
    size_t size,
    int target
)
{
    size_t low = 0U;
    size_t high = size;

    if ((array == NULL) || (size == 0U))
    {
        return -1;
    }

    while (low < high)
    {
        size_t mid = low + (high - low) / 2U;

        if (array[mid] == target)
        {
            return (int)mid;
        }

        if (array[low] <= array[mid])
        {
            /*
             * Left half [low, mid] is sorted.
             */
            if ((array[low] <= target) &&
                (target < array[mid]))
            {
                high = mid;
            }
            else
            {
                low = mid + 1U;
            }
        }
        else
        {
            /*
             * Right half [mid, high) is sorted.
             */
            if ((array[mid] < target) &&
                (target <= array[high - 1U]))
            {
                low = mid + 1U;
            }
            else
            {
                high = mid;
            }
        }
    }

    return -1;
}
```

Khi có nhiều phần tử trùng, việc xác định nửa nào được sắp xếp có thể mơ hồ và worst case có thể giảm xuống:

```text
O(n)
```

---

## 18. Tìm căn bậc hai nguyên

Tìm:

```text
floor(sqrt(x))
```

Ví dụ:

```text
x = 8
sqrt(8) ≈ 2.828
kết quả = 2
```

Không nên kiểm tra:

```c
mid * mid <= x
```

với kiểu nhỏ vì phép nhân có thể overflow.

Dùng:

```c
mid <= x / mid
```

### Mã C

```c
#include <stdint.h>

uint32_t IntegerSqrt(uint32_t value)
{
    uint32_t low = 0U;
    uint32_t high = value;
    uint32_t answer = 0U;

    while (low <= high)
    {
        uint32_t mid =
            low + (high - low) / 2U;

        if ((mid == 0U) ||
            (mid <= value / mid))
        {
            answer = mid;
            low = mid + 1U;
        }
        else
        {
            high = mid - 1U;
        }
    }

    return answer;
}
```

Có thể giảm khoảng ban đầu:

```text
high = min(value, 65535)
```

cho `uint32_t`, nhưng bản trên dễ hiểu hơn.

---

## 19. Binary Search trong không gian không giới hạn

Giả sử không biết trước giới hạn phải.

Ví dụ:

```text
Tìm target trong một dãy tăng có thể đọc bằng hàm GetValue(index)
```

### Bước 1: Mở rộng phạm vi theo cấp số nhân

```text
high = 1
2
4
8
16
32
...
```

cho đến khi:

```text
GetValue(high) >= target
```

### Bước 2: Binary Search trong khoảng đã tìm được

```text
[high / 2, high]
```

Pseudo-code:

```c
size_t high = 1U;

while (GetValue(high) < target)
{
    high *= 2U;
}

size_t low = high / 2U;

/* Binary search in [low, high]. */
```

Cần kiểm tra overflow của `high *= 2`.

Kỹ thuật này thường được gọi là:

- Exponential Search.
- Unbounded Binary Search.

---

## 20. Binary Search on Answer

Đây là biến thể rất quan trọng.

Ta không tìm một phần tử trong mảng, mà tìm giá trị nhỏ nhất hoặc lớn nhất thỏa mãn một điều kiện đơn điệu.

### Mẫu bài toán

```text
Tìm giá trị nhỏ nhất x sao cho Feasible(x) == true
```

Dạng điều kiện:

```text
x:        0 1 2 3 4 5 6 7
Feasible: F F F F T T T T
```

Đáp án là biên chuyển:

```text
false -> true
```

### Template tìm giá trị nhỏ nhất thỏa mãn

```c
long long FindMinimumFeasible(
    long long low,
    long long high
)
{
    while (low < high)
    {
        long long mid =
            low + (high - low) / 2LL;

        if (Feasible(mid))
        {
            high = mid;
        }
        else
        {
            low = mid + 1LL;
        }
    }

    return low;
}
```

### Ví dụ ứng dụng

- Thời gian nhỏ nhất để hoàn thành công việc.
- Tốc độ nhỏ nhất để xử lý hết dữ liệu.
- Capacity nhỏ nhất của xe hoặc bộ nhớ.
- Khoảng cách lớn nhất vẫn thỏa điều kiện.
- Số ngày tối thiểu để tạo đủ sản phẩm.
- Căn bậc hai nguyên.
- Aggressive Cows.
- Book Allocation.
- Painter Partition.
- Koko Eating Bananas.

### Điều kiện quan trọng

Hàm:

```c
Feasible(x)
```

phải đơn điệu.

Nếu kết quả có dạng:

```text
false true false true
```

Binary Search thông thường không áp dụng được.

---

## 21. Binary Search với số thực

Với số thực, thường không dùng điều kiện:

```text
low <= high
```

Mà lặp theo:

- Số vòng cố định.
- Sai số `epsilon`.

### Ví dụ tìm căn bậc hai

```c
double SquareRootDouble(double value)
{
    double low = 0.0;
    double high = (value >= 1.0) ? value : 1.0;

    for (int iteration = 0;
         iteration < 100;
         iteration++)
    {
        double mid = low + (high - low) / 2.0;

        if (mid * mid <= value)
        {
            low = mid;
        }
        else
        {
            high = mid;
        }
    }

    return low;
}
```

Lặp cố định giúp tránh vòng lặp không kết thúc do sai số floating-point.

---

## 22. Ứng dụng

Binary Search được sử dụng trong:

- Tìm kiếm trên mảng đã sắp xếp.
- Tìm lần xuất hiện đầu tiên và cuối cùng.
- Lower bound và upper bound.
- Tìm phần tử gần nhất.
- Tìm kiếm trong mảng xoay.
- Tìm peak.
- Tìm căn bậc hai.
- Tối ưu hóa trên không gian đơn điệu.
- Database index.
- B-tree và các cấu trúc index.
- `git bisect`.
- Lookup trong bảng định tuyến hoặc symbol table đã sắp xếp.
- Scheduling.
- Computational geometry.
- Machine learning threshold tuning.
- Competitive programming.

### `git bisect`

Ý tưởng:

```text
Một commit cũ hoạt động tốt
Một commit mới bị lỗi
```

Mỗi bước kiểm tra commit ở giữa và loại bỏ một nửa khoảng commit có thể chứa lỗi.

---

## 23. Binary Search trong lập trình nhúng

## 23.1. Lookup Table

Firmware thường có bảng hiệu chỉnh đã sắp xếp:

```c
typedef struct
{
    uint16_t adc;
    int16_t temperature;
} CalibrationPoint;
```

Có thể Binary Search theo `adc` để tìm hai điểm gần nhất rồi nội suy.

---

## 23.2. Bảng mã lỗi hoặc command

Nếu command ID được sắp xếp:

```c
typedef struct
{
    uint16_t command_id;
    void (*handler)(void);
} CommandEntry;
```

Binary Search giúp tìm handler trong:

```text
O(log n)
```

---

## 23.3. Flash và ROM

Bảng tra cứu có thể đặt trong Flash/ROM.

Binary Search chỉ đọc:

- Không cần thay đổi dữ liệu.
- Không cần RAM phụ lớn.
- Phù hợp với bảng hằng.

Tuy nhiên cần cân nhắc:

- Flash wait state.
- Cache.
- Alignment.
- Chi phí truy cập ngẫu nhiên.

---

## 23.4. Tránh recursion khi Stack nhỏ

Bản lặp thường được ưu tiên trong embedded:

```text
Auxiliary space = O(1)
```

Bản đệ quy cần:

```text
O(log n) stack frames
```

Mặc dù độ sâu nhỏ, nhiều tiêu chuẩn dự án vẫn hạn chế recursion.

---

## 23.5. Dữ liệu phải không thay đổi trong lúc tìm kiếm

Nếu ISR hoặc DMA thay đổi mảng trong khi Binary Search đang chạy, giả định “mảng đã sắp xếp” có thể bị phá vỡ.

Cần:

- Snapshot.
- Double Buffer.
- Critical section.
- Ownership rõ ràng.
- Chỉ tìm trên vùng dữ liệu immutable.

---

## 23.6. Worst-case dễ dự đoán

Binary Search có worst-case:

```text
O(log n)
```

Điều này hữu ích với hệ thống real-time khi số phần tử và thời gian mỗi lần đọc đã biết.

---

## 24. Lỗi thường gặp

## 24.1. Dùng Binary Search trên mảng chưa sắp xếp

Sai:

```text
[7, 2, 9, 1, 5]
```

Binary Search có thể bỏ qua vùng chứa target.

---

## 24.2. Tính `mid` có nguy cơ overflow

Không nên:

```c
mid = (low + high) / 2;
```

Nên:

```c
mid = low + (high - low) / 2;
```

---

## 24.3. Không loại bỏ `mid`

Sai:

```c
low = mid;
```

trong một số template có thể làm khoảng không giảm.

Ví dụ:

```text
low = 4
high = 5
mid = 4
```

Nếu tiếp tục:

```text
low = mid = 4
```

vòng lặp không tiến triển.

Với tìm kiếm chính xác thường dùng:

```c
low = mid + 1;
```

---

## 24.4. Trộn khoảng đóng và nửa mở

Ví dụ sai:

```c
high = size;
while (low <= high)
```

`high == size` không phải index hợp lệ.

Phải chọn một trong hai:

```text
[0, size - 1] với low <= high
```

hoặc:

```text
[0, size) với low < high
```

---

## 24.5. Underflow với `size_t`

Sai:

```c
high = mid - 1U;
```

khi:

```text
mid == 0
```

Kết quả trở thành một số unsigned rất lớn.

Dùng khoảng nửa mở giúp tránh nhiều trường hợp này:

```c
high = mid;
```

---

## 24.6. Không xử lý mảng rỗng

Nếu dùng:

```c
high = size - 1U;
```

với:

```text
size = 0
```

sẽ underflow.

Luôn kiểm tra:

```c
if ((array == NULL) || (size == 0U))
{
    return -1;
}
```

---

## 24.7. Mong đợi index đầu tiên khi có phần tử trùng

Binary Search cơ bản chỉ đảm bảo tìm được một phần tử bằng target.

Muốn index đầu hoặc cuối phải dùng biến thể riêng.

---

## 24.8. Comparator hoặc predicate không đơn điệu

Binary Search on Answer chỉ đúng nếu predicate có một biên chuyển rõ ràng.

Sai:

```text
true false true false
```

---

## 24.9. Overflow trong điều kiện kiểm tra

Ví dụ căn bậc hai:

```c
mid * mid <= value
```

có thể overflow.

Dùng:

```c
mid <= value / mid
```

khi `mid != 0`.

---

## 24.10. Sort rồi Binary Search cho một truy vấn duy nhất

Nếu dữ liệu chưa sắp xếp:

```text
Sort:          O(n log n)
Binary Search: O(log n)
```

Trong khi Linear Search một lần:

```text
O(n)
```

Sort trước chỉ có lợi khi:

- Cần nhiều truy vấn.
- Dữ liệu còn được dùng cho thao tác khác.
- Dữ liệu có thể được giữ ở trạng thái đã sắp xếp.

---

## 25. Bài tập luyện tập

### Cơ bản

1. Binary Search dạng lặp.
2. Binary Search dạng đệ quy.
3. Tìm lần xuất hiện đầu tiên.
4. Tìm lần xuất hiện cuối cùng.
5. Đếm số lần xuất hiện.
6. Lower Bound.
7. Upper Bound.
8. Search Insert Position.
9. Floor của một giá trị.
10. Ceil của một giá trị.
11. Tìm phần tử gần nhất.
12. Tìm target trong mảng giảm dần.

### Trung bình

1. Integer Square Root.
2. Search in Rotated Sorted Array.
3. Find Minimum in Rotated Sorted Array.
4. Find Peak Element.
5. Count `1` trong sorted binary array.
6. Tìm phần tử duy nhất trong mảng cặp.
7. First Bad Version.
8. Search in Nearly Sorted Array.
9. Exponential Search.
10. Infinite Sorted Array Search.
11. Koko Eating Bananas.
12. Minimum Days to Make Bouquets.
13. Capacity to Ship Packages.
14. Aggressive Cows.
15. Book Allocation.

### Nâng cao

1. Median of Two Sorted Arrays.
2. K-th Element of Two Sorted Arrays.
3. Split Array Largest Sum.
4. Painter Partition.
5. Allocate Minimum Pages.
6. Maximum Minimum Distance.
7. Binary Search trên ma trận.
8. Binary Search trên đáp án số thực.
9. Parametric Search.
10. Parallel Binary Search.
11. Fractional Cascading.
12. Binary Search trên segment có predicate tùy chỉnh.

---

## 26. Tổng kết

Binary Search hoạt động bằng cách liên tục loại bỏ một nửa không gian tìm kiếm.

Điều kiện chính:

```text
Dữ liệu đã sắp xếp
hoặc
Predicate có tính đơn điệu
```

Template tìm kiếm chính xác:

```text
Tính mid
So sánh với target
Loại bỏ một nửa
Lặp đến khi tìm thấy hoặc khoảng rỗng
```

Độ phức tạp:

```text
Best:    O(1)
Average: O(log n)
Worst:   O(log n)
```

Bộ nhớ:

```text
Iterative: O(1)
Recursive: O(log n)
```

Công thức `mid` nên dùng:

```c
mid = low + (high - low) / 2;
```

Các biến thể cần nắm:

- Exact Search.
- First Occurrence.
- Last Occurrence.
- Lower Bound.
- Upper Bound.
- Rotated Array Search.
- Integer Square Root.
- Unbounded Search.
- Binary Search on Answer.
- Floating-point Binary Search.

Điểm quan trọng nhất không phải là học thuộc một đoạn code, mà là xác định rõ:

1. Khoảng tìm kiếm biểu diễn điều gì?
2. `low` và `high` có thuộc khoảng không?
3. Predicate hoặc dữ liệu có đơn điệu không?
4. Khi nào loại bỏ được `mid`?
5. Vòng lặp có luôn làm khoảng nhỏ lại không?
6. Giá trị trả về là index, vị trí chèn hay biên chuyển?

---

## 27. Tài liệu tham khảo

README này được biên soạn và diễn đạt lại độc lập dựa trên phạm vi chủ đề được trình bày trong:

- GeeksforGeeks — **Binary Search**:  
  https://www.geeksforgeeks.org/dsa/binary-search/>
