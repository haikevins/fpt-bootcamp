# Linear Search

> Tài liệu học tập độc lập về **Linear Search**, trình bày nguyên lý, độ phức tạp, các biến thể thường gặp và cách cài đặt bằng ngôn ngữ C.

---

## Mục lục

1. [Linear Search là gì?](#1-linear-search-là-gì)
2. [Ý tưởng cốt lõi](#2-ý-tưởng-cốt-lõi)
3. [Minh họa từng bước](#3-minh-họa-từng-bước)
4. [Cài đặt cơ bản bằng C](#4-cài-đặt-cơ-bản-bằng-c)
5. [Độ phức tạp](#5-độ-phức-tạp)
6. [Tìm lần xuất hiện đầu tiên](#6-tìm-lần-xuất-hiện-đầu-tiên)
7. [Tìm lần xuất hiện cuối cùng](#7-tìm-lần-xuất-hiện-cuối-cùng)
8. [Tìm tất cả vị trí xuất hiện](#8-tìm-tất-cả-vị-trí-xuất-hiện)
9. [Đếm số lần xuất hiện](#9-đếm-số-lần-xuất-hiện)
10. [Tìm phần tử nhỏ nhất và lớn nhất](#10-tìm-phần-tử-nhỏ-nhất-và-lớn-nhất)
11. [Linear Search trên chuỗi](#11-linear-search-trên-chuỗi)
12. [Linear Search trên Linked List](#12-linear-search-trên-linked-list)
13. [Linear Search với cấu trúc dữ liệu](#13-linear-search-với-cấu-trúc-dữ-liệu)
14. [Sentinel Linear Search](#14-sentinel-linear-search)
15. [So sánh Linear Search và Binary Search](#15-so-sánh-linear-search-và-binary-search)
16. [Khi nào nên dùng Linear Search?](#16-khi-nào-nên-dùng-linear-search)
17. [Ứng dụng](#17-ứng-dụng)
18. [Linear Search trong lập trình nhúng](#18-linear-search-trong-lập-trình-nhúng)
19. [Ưu điểm và hạn chế](#19-ưu-điểm-và-hạn-chế)
20. [Các lỗi thường gặp](#20-các-lỗi-thường-gặp)
21. [Bài tập luyện tập](#21-bài-tập-luyện-tập)
22. [Tổng kết](#22-tổng-kết)
23. [Tài liệu tham khảo](#23-tài-liệu-tham-khảo)

---

## 1. Linear Search là gì?

**Linear Search**, còn gọi là **Sequential Search**, là thuật toán tìm kiếm bằng cách kiểm tra từng phần tử theo thứ tự từ đầu đến cuối.

Với mảng:

```text
[10, 50, 30, 70, 80, 20, 90, 40]
```

Cần tìm:

```text
target = 30
```

Thuật toán kiểm tra:

```text
10  → không bằng 30
50  → không bằng 30
30  → tìm thấy
```

Kết quả:

```text
index = 2
```

Nếu duyệt hết mảng mà không tìm thấy, thuật toán trả về một giá trị báo lỗi, thường là:

```text
-1
```

---

## 2. Ý tưởng cốt lõi

Linear Search thực hiện các bước:

1. Bắt đầu từ phần tử đầu tiên.
2. So sánh phần tử hiện tại với `target`.
3. Nếu bằng nhau, trả về vị trí hiện tại.
4. Nếu khác nhau, chuyển sang phần tử kế tiếp.
5. Nếu đã duyệt hết dữ liệu, trả về “không tìm thấy”.

Pseudo-code:

```text
for mỗi phần tử trong dữ liệu:
    nếu phần tử hiện tại == target:
        trả về vị trí hiện tại

trả về -1
```

Linear Search không yêu cầu dữ liệu phải được sắp xếp.

---

## 3. Minh họa từng bước

Mảng:

```text
Index:   0   1   2   3   4
Array:  [2,  3,  4, 10, 40]
```

Cần tìm:

```text
target = 10
```

### Bước 1

```text
index = 0
array[0] = 2
2 != 10
```

Tiếp tục.

### Bước 2

```text
index = 1
array[1] = 3
3 != 10
```

Tiếp tục.

### Bước 3

```text
index = 2
array[2] = 4
4 != 10
```

Tiếp tục.

### Bước 4

```text
index = 3
array[3] = 10
10 == 10
```

Tìm thấy:

```text
return 3
```

Thuật toán không cần kiểm tra phần tử tại index `4` vì đáp án đã được tìm thấy.

---

## 4. Cài đặt cơ bản bằng C

### 4.1. Hàm tìm kiếm

```c
#include <stddef.h>

int LinearSearch(
    const int array[],
    size_t size,
    int target
)
{
    size_t index;

    if ((array == NULL) || (size == 0U))
    {
        return -1;
    }

    for (index = 0U; index < size; index++)
    {
        if (array[index] == target)
        {
            return (int)index;
        }
    }

    return -1;
}
```

### 4.2. Chương trình hoàn chỉnh

```c
#include <stddef.h>
#include <stdio.h>

int LinearSearch(
    const int array[],
    size_t size,
    int target
)
{
    size_t index;

    if ((array == NULL) || (size == 0U))
    {
        return -1;
    }

    for (index = 0U; index < size; index++)
    {
        if (array[index] == target)
        {
            return (int)index;
        }
    }

    return -1;
}

int main(void)
{
    const int array[] = {2, 3, 4, 10, 40};

    const size_t size =
        sizeof(array) / sizeof(array[0]);

    const int target = 10;

    int result = LinearSearch(
        array,
        size,
        target
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

### 4.3. Biên dịch

```bash
gcc -std=c11 \
    -Wall \
    -Wextra \
    -Wpedantic \
    main.c \
    -o linear_search_demo
```

Chạy:

```bash
./linear_search_demo
```

---

## 5. Độ phức tạp

## 5.1. Best case

Phần tử cần tìm nằm ngay đầu mảng:

```text
[target, ..., ..., ...]
```

Chỉ cần một lần so sánh:

```text
O(1)
```

---

## 5.2. Worst case

Hai trường hợp:

- Target nằm ở cuối.
- Target không tồn tại.

Thuật toán phải kiểm tra toàn bộ `n` phần tử:

```text
O(n)
```

---

## 5.3. Average case

Trung bình, thuật toán kiểm tra khoảng một nửa số phần tử:

```text
n / 2
```

Bỏ hằng số trong Big-O:

```text
O(n)
```

---

## 5.4. Bộ nhớ phụ

Thuật toán chỉ dùng một biến chỉ mục:

```text
O(1)
```

---

## 5.5. Bảng tổng hợp

| Trường hợp | Độ phức tạp |
|---|---:|
| Best case | `O(1)` |
| Average case | `O(n)` |
| Worst case | `O(n)` |
| Auxiliary space | `O(1)` |

---

## 6. Tìm lần xuất hiện đầu tiên

Mảng:

```text
[4, 2, 7, 2, 9, 2]
```

Tìm:

```text
target = 2
```

Hàm Linear Search cơ bản trả về ngay khi gặp phần tử đầu tiên:

```text
index = 1
```

```c
int FindFirstOccurrence(
    const int array[],
    size_t size,
    int target
)
{
    size_t index;

    if (array == NULL)
    {
        return -1;
    }

    for (index = 0U; index < size; index++)
    {
        if (array[index] == target)
        {
            return (int)index;
        }
    }

    return -1;
}
```

---

## 7. Tìm lần xuất hiện cuối cùng

Có thể duyệt từ cuối về đầu.

```c
#include <stddef.h>

int FindLastOccurrence(
    const int array[],
    size_t size,
    int target
)
{
    size_t index;

    if ((array == NULL) || (size == 0U))
    {
        return -1;
    }

    for (index = size; index > 0U; index--)
    {
        size_t current = index - 1U;

        if (array[current] == target)
        {
            return (int)current;
        }
    }

    return -1;
}
```

Ví dụ:

```text
Array:  [4, 2, 7, 2, 9, 2]
Target: 2
```

Kết quả:

```text
index = 5
```

Không nên viết:

```c
for (size_t index = size - 1U;
     index >= 0U;
     index--)
```

Vì `size_t` là kiểu unsigned nên điều kiện:

```text
index >= 0
```

luôn đúng.

---

## 8. Tìm tất cả vị trí xuất hiện

Hàm ghi các index tìm được vào một mảng đầu ra.

```c
#include <stddef.h>

size_t FindAllOccurrences(
    const int array[],
    size_t size,
    int target,
    size_t output_indices[],
    size_t output_capacity
)
{
    size_t found_count = 0U;
    size_t index;

    if ((array == NULL) ||
        (output_indices == NULL))
    {
        return 0U;
    }

    for (index = 0U; index < size; index++)
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

```c
const int array[] = {4, 2, 7, 2, 9, 2};
size_t indices[6];

size_t count = FindAllOccurrences(
    array,
    6U,
    2,
    indices,
    6U
);
```

Kết quả:

```text
count = 3
indices = [1, 3, 5]
```

Nếu `found_count` lớn hơn `output_capacity`, hàm vẫn có thể trả tổng số lần xuất hiện, nhưng chỉ ghi được số index vừa với output buffer.

---

## 9. Đếm số lần xuất hiện

Không cần lưu các index nếu chỉ cần số lượng.

```c
#include <stddef.h>

size_t CountOccurrences(
    const int array[],
    size_t size,
    int target
)
{
    size_t count = 0U;
    size_t index;

    if (array == NULL)
    {
        return 0U;
    }

    for (index = 0U; index < size; index++)
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
Array:  [4, 2, 7, 2, 9, 2]
Target: 2
```

Kết quả:

```text
3
```

---

## 10. Tìm phần tử nhỏ nhất và lớn nhất

Việc tìm minimum hoặc maximum cũng là một dạng duyệt tuyến tính.

### 10.1. Tìm minimum

```c
#include <stdbool.h>
#include <stddef.h>

bool FindMinimum(
    const int array[],
    size_t size,
    int *minimum
)
{
    size_t index;

    if ((array == NULL) ||
        (minimum == NULL) ||
        (size == 0U))
    {
        return false;
    }

    *minimum = array[0];

    for (index = 1U; index < size; index++)
    {
        if (array[index] < *minimum)
        {
            *minimum = array[index];
        }
    }

    return true;
}
```

### 10.2. Tìm maximum

```c
bool FindMaximum(
    const int array[],
    size_t size,
    int *maximum
)
{
    size_t index;

    if ((array == NULL) ||
        (maximum == NULL) ||
        (size == 0U))
    {
        return false;
    }

    *maximum = array[0];

    for (index = 1U; index < size; index++)
    {
        if (array[index] > *maximum)
        {
            *maximum = array[index];
        }
    }

    return true;
}
```

Cả hai đều có:

```text
Time:  O(n)
Space: O(1)
```

---

## 11. Linear Search trên chuỗi

## 11.1. Tìm một ký tự

```c
#include <stddef.h>

int FindCharacter(
    const char string[],
    char target
)
{
    size_t index = 0U;

    if (string == NULL)
    {
        return -1;
    }

    while (string[index] != '\0')
    {
        if (string[index] == target)
        {
            return (int)index;
        }

        index++;
    }

    return -1;
}
```

Ví dụ:

```text
String: "embedded"
Target: 'd'
```

Kết quả:

```text
index = 4
```

---

## 11.2. Tìm chuỗi con đơn giản

Cách brute-force kiểm tra từng vị trí bắt đầu.

```c
#include <stddef.h>

int FindSubstring(
    const char text[],
    const char pattern[]
)
{
    size_t start;

    if ((text == NULL) || (pattern == NULL))
    {
        return -1;
    }

    if (pattern[0] == '\0')
    {
        return 0;
    }

    for (start = 0U; text[start] != '\0'; start++)
    {
        size_t offset = 0U;

        while ((pattern[offset] != '\0') &&
               (text[start + offset] != '\0') &&
               (text[start + offset] ==
                pattern[offset]))
        {
            offset++;
        }

        if (pattern[offset] == '\0')
        {
            return (int)start;
        }
    }

    return -1;
}
```

Nếu:

```text
n = độ dài text
m = độ dài pattern
```

Worst case:

```text
O(n × m)
```

---

## 12. Linear Search trên Linked List

Linked List không hỗ trợ truy cập ngẫu nhiên theo index.

Do đó, tìm kiếm thường được thực hiện bằng cách duyệt từng Node.

```c
typedef struct Node
{
    int data;
    struct Node *next;
} Node;
```

### Tìm Node

```c
Node *LinkedList_Find(
    Node *head,
    int target
)
{
    Node *current = head;

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

### Tìm vị trí

```c
int LinkedList_FindIndex(
    const Node *head,
    int target
)
{
    const Node *current = head;
    int index = 0;

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

Độ phức tạp:

```text
O(n)
```

Binary Search không mang lại lợi thế rõ ràng trên Singly Linked List vì việc truy cập Node giữa không phải `O(1)`.

---

## 13. Linear Search với cấu trúc dữ liệu

Ví dụ:

```c
typedef struct
{
    uint16_t id;
    const char *name;
} Device;
```

### Tìm theo ID

```c
#include <stddef.h>
#include <stdint.h>

int FindDeviceById(
    const Device devices[],
    size_t count,
    uint16_t id
)
{
    size_t index;

    if (devices == NULL)
    {
        return -1;
    }

    for (index = 0U; index < count; index++)
    {
        if (devices[index].id == id)
        {
            return (int)index;
        }
    }

    return -1;
}
```

### Tìm theo tên

```c
#include <string.h>

int FindDeviceByName(
    const Device devices[],
    size_t count,
    const char *name
)
{
    size_t index;

    if ((devices == NULL) || (name == NULL))
    {
        return -1;
    }

    for (index = 0U; index < count; index++)
    {
        if ((devices[index].name != NULL) &&
            (strcmp(devices[index].name, name) == 0))
        {
            return (int)index;
        }
    }

    return -1;
}
```

---

## 14. Sentinel Linear Search

Sentinel Search giảm một phép kiểm tra giới hạn trong mỗi vòng lặp bằng cách đặt `target` ở cuối mảng.

### Ý tưởng

Mảng:

```text
[a0, a1, a2, ..., an-1]
```

Lưu giá trị cuối:

```text
last = array[n - 1]
```

Tạm đặt:

```text
array[n - 1] = target
```

Duyệt cho đến khi gặp target mà không cần kiểm tra `index < size` trong mỗi vòng.

Sau đó khôi phục phần tử cuối.

### Mã C

```c
#include <stddef.h>

int SentinelLinearSearch(
    int array[],
    size_t size,
    int target
)
{
    int last;
    size_t index = 0U;

    if ((array == NULL) || (size == 0U))
    {
        return -1;
    }

    last = array[size - 1U];
    array[size - 1U] = target;

    while (array[index] != target)
    {
        index++;
    }

    array[size - 1U] = last;

    if ((index < size - 1U) || (last == target))
    {
        return (int)index;
    }

    return -1;
}
```

### Lưu ý

Sentinel Search:

- Tạm thời sửa dữ liệu đầu vào.
- Không dùng được nếu mảng là `const`.
- Không an toàn nếu ISR hoặc task khác đọc mảng cùng lúc.
- Lợi ích hiệu năng phụ thuộc compiler và CPU.
- Linear Search thông thường thường rõ ràng và an toàn hơn.

---

## 15. So sánh Linear Search và Binary Search

| Tiêu chí | Linear Search | Binary Search |
|---|---|---|
| Dữ liệu cần sắp xếp | Không | Có |
| Truy cập tuần tự | Phù hợp | Không phải lựa chọn tối ưu |
| Best case | `O(1)` | `O(1)` |
| Average case | `O(n)` | `O(log n)` |
| Worst case | `O(n)` | `O(log n)` |
| Bộ nhớ bản lặp | `O(1)` | `O(1)` |
| Cài đặt | Rất đơn giản | Dễ sai biên |
| Linked List | Phù hợp | Thường không hiệu quả |
| Dữ liệu nhỏ | Thường phù hợp | Có thể không cần thiết |
| Dữ liệu thay đổi liên tục | Không cần duy trì sort | Phải duy trì thứ tự |

### Sort rồi Binary Search

Nếu mảng chưa sắp xếp và chỉ tìm một lần:

```text
Sort:          O(n log n)
Binary Search: O(log n)
```

Tổng vẫn lớn hơn Linear Search:

```text
O(n)
```

Sort trước có lợi khi:

- Có nhiều truy vấn tìm kiếm.
- Dữ liệu ít thay đổi.
- Thứ tự đã cần cho thao tác khác.

---

## 16. Khi nào nên dùng Linear Search?

Linear Search phù hợp khi:

### Dữ liệu chưa sắp xếp

```text
[7, 2, 9, 1, 5]
```

Không cần sắp xếp trước.

### Tập dữ liệu nhỏ

Với vài phần tử, chi phí xây dựng cấu trúc phức tạp có thể không cần thiết.

### Chỉ tìm kiếm một lần

Không đáng sort toàn bộ dữ liệu chỉ để thực hiện một truy vấn.

### Dữ liệu nằm trong Linked List

Duyệt tuần tự là cách tự nhiên.

### Dữ liệu thay đổi thường xuyên

Không cần duy trì thứ tự sau mỗi lần thêm hoặc xóa.

### Cần tìm theo điều kiện phức tạp

Ví dụ:

```text
Tìm sensor đầu tiên đang lỗi
Tìm task đầu tiên đã quá deadline
Tìm packet đầu tiên có CRC sai
```

Điều kiện không nhất thiết là phép so sánh bằng đơn giản.

---

## 17. Ứng dụng

Linear Search được dùng trong:

- Tìm phần tử trong mảng chưa sắp xếp.
- Tìm Node trong Linked List.
- Tìm ký tự trong chuỗi.
- Tìm record theo ID hoặc trạng thái.
- Kiểm tra dữ liệu đầu vào.
- Tìm minimum và maximum.
- Tìm phần tử đầu tiên thỏa predicate.
- Duyệt log.
- Tìm command trong bảng nhỏ.
- Tìm thiết bị trong danh sách cấu hình.
- Quét buffer.
- Kiểm tra packet hoặc frame.
- Thuật toán brute force.

---

## 17.1. Tìm phần tử đầu tiên thỏa điều kiện

Ví dụ tìm số dương đầu tiên:

```c
int FindFirstPositive(
    const int array[],
    size_t size
)
{
    size_t index;

    if (array == NULL)
    {
        return -1;
    }

    for (index = 0U; index < size; index++)
    {
        if (array[index] > 0)
        {
            return (int)index;
        }
    }

    return -1;
}
```

---

## 17.2. Tìm phần tử theo predicate

C có thể truyền callback.

```c
#include <stdbool.h>
#include <stddef.h>

typedef bool (*IntPredicate)(int value);

int FindFirstMatching(
    const int array[],
    size_t size,
    IntPredicate predicate
)
{
    size_t index;

    if ((array == NULL) || (predicate == NULL))
    {
        return -1;
    }

    for (index = 0U; index < size; index++)
    {
        if (predicate(array[index]))
        {
            return (int)index;
        }
    }

    return -1;
}
```

Ví dụ predicate:

```c
static bool IsEven(int value)
{
    return (value % 2) == 0;
}
```

---

## 18. Linear Search trong lập trình nhúng

## 18.1. Bảng cấu hình nhỏ

Firmware thường có bảng nhỏ:

```c
typedef struct
{
    uint16_t parameter_id;
    uint32_t default_value;
} ParameterEntry;
```

Nếu chỉ có 8 hoặc 16 phần tử, Linear Search có thể đơn giản và đủ nhanh.

---

## 18.2. Tìm command handler

```c
typedef void (*CommandHandler)(void);

typedef struct
{
    uint16_t command_id;
    CommandHandler handler;
} CommandEntry;
```

```c
CommandHandler FindCommandHandler(
    const CommandEntry table[],
    size_t count,
    uint16_t command_id
)
{
    size_t index;

    if (table == NULL)
    {
        return NULL;
    }

    for (index = 0U; index < count; index++)
    {
        if (table[index].command_id == command_id)
        {
            return table[index].handler;
        }
    }

    return NULL;
}
```

---

## 18.3. Quét buffer UART

Ví dụ tìm ký tự kết thúc dòng:

```c
int FindLineEnding(
    const char buffer[],
    size_t length
)
{
    size_t index;

    if (buffer == NULL)
    {
        return -1;
    }

    for (index = 0U; index < length; index++)
    {
        if ((buffer[index] == '\r') ||
            (buffer[index] == '\n'))
        {
            return (int)index;
        }
    }

    return -1;
}
```

---

## 18.4. Tính xác định thời gian

Worst case:

```text
n lần so sánh
```

Nếu `n` được giới hạn trước, thời gian thực thi tối đa dễ ước lượng.

Ví dụ:

```text
Bảng có tối đa 16 phần tử
→ tối đa 16 lần so sánh
```

Điều này có thể chấp nhận được trong hệ thống real-time nhỏ.

---

## 18.5. Dữ liệu trong Flash

Có thể duyệt bảng hằng nằm trong Flash hoặc ROM.

```c
static const CommandEntry command_table[] = {
    {0x01U, HandleStart},
    {0x02U, HandleStop},
    {0x03U, HandleReset}
};
```

Linear Search:

- Không cần RAM phụ.
- Không cần thay đổi dữ liệu.
- Không yêu cầu bảng đã sắp xếp.

---

## 18.6. Truy cập đồng thời

Nếu DMA hoặc ISR thay đổi mảng trong khi hàm đang tìm kiếm, kết quả có thể không nhất quán.

Cần cân nhắc:

- Snapshot.
- Double Buffer.
- Critical section.
- Ownership rõ ràng.
- Dữ liệu immutable.
- Version counter.

---

## 18.7. Cache và truy cập tuần tự

Linear Search đọc dữ liệu theo thứ tự liên tiếp:

```text
array[0], array[1], array[2], ...
```

Cách truy cập này thường thân thiện với cache và prefetch hơn truy cập ngẫu nhiên.

Với dữ liệu nhỏ, hiệu năng thực tế có thể rất tốt dù Big-O là `O(n)`.

---

## 19. Ưu điểm và hạn chế

## 19.1. Ưu điểm

- Rất dễ hiểu.
- Dễ cài đặt.
- Không cần dữ liệu đã sắp xếp.
- Không cần bộ nhớ phụ đáng kể.
- Hoạt động trên Array và Linked List.
- Có thể dừng ngay khi tìm thấy.
- Phù hợp với dữ liệu nhỏ.
- Dễ áp dụng với predicate phức tạp.
- Truy cập bộ nhớ tuần tự.
- Worst-case dễ xác định khi kích thước có giới hạn.

---

## 19.2. Hạn chế

- Worst case là `O(n)`.
- Chậm với tập dữ liệu lớn.
- Không tận dụng được thứ tự của mảng đã sắp xếp.
- Nhiều truy vấn lặp lại có thể tốn nhiều CPU.
- Tìm kiếm nhiều lần trên cùng dữ liệu có thể nên dùng:
  - Binary Search.
  - Hash Table.
  - Tree.
  - Index riêng.

---

## 20. Các lỗi thường gặp

## 20.1. Duyệt vượt giới hạn

Sai:

```c
for (index = 0U; index <= size; index++)
```

Index hợp lệ cuối cùng là:

```text
size - 1
```

Đúng:

```c
index < size
```

---

## 20.2. Không xử lý con trỏ NULL

Sai:

```c
if (array[0] == target)
```

khi `array == NULL`.

Nên kiểm tra:

```c
if (array == NULL)
{
    return -1;
}
```

---

## 20.3. Trả về kiểu unsigned nhưng dùng `-1`

Sai về mặt giao diện:

```c
size_t LinearSearch(...);

return -1;
```

`-1` sẽ được chuyển thành một số unsigned rất lớn.

Có thể dùng:

```c
int
```

nếu số phần tử không vượt `INT_MAX`.

Hoặc dùng API an toàn hơn:

```c
bool LinearSearch(
    const int array[],
    size_t size,
    int target,
    size_t *index
);
```

---

## 20.4. Không dừng sau khi tìm thấy

Nếu chỉ cần lần xuất hiện đầu tiên:

```c
return (int)index;
```

ngay khi tìm thấy.

Nếu tiếp tục duyệt, hàm có thể vô tình trả lần xuất hiện cuối cùng hoặc tốn thời gian không cần thiết.

---

## 20.5. So sánh chuỗi bằng `==`

Sai:

```c
if (devices[index].name == name)
```

Cách này so sánh địa chỉ con trỏ.

Đúng:

```c
strcmp(devices[index].name, name) == 0
```

---

## 20.6. Sentinel Search trên dữ liệu chỉ đọc

Không được tạm sửa:

```c
const int array[]
```

hoặc bảng nằm trong Flash chỉ đọc.

---

## 20.7. Dùng Linear Search cho quá nhiều truy vấn

Nếu có:

```text
n phần tử
q truy vấn
```

Linear Search có thể tốn:

```text
O(n × q)
```

Nếu dữ liệu phù hợp, có thể:

- Sort một lần rồi Binary Search.
- Xây Hash Table.
- Dùng Tree hoặc index.

---

## 20.8. Không xét chi phí comparator

Tìm kiếm theo chuỗi dài hoặc cấu trúc phức tạp có thể làm mỗi phép so sánh tốn nhiều thời gian.

Ví dụ:

```c
strcmp()
```

bản thân có thể cần duyệt nhiều ký tự.

Tổng chi phí không chỉ phụ thuộc số phần tử, mà còn phụ thuộc chi phí kiểm tra mỗi phần tử.

---

## 21. Bài tập luyện tập

### Cơ bản

1. Tìm một số trong mảng.
2. Tìm lần xuất hiện đầu tiên.
3. Tìm lần xuất hiện cuối cùng.
4. Đếm số lần xuất hiện.
5. Tìm tất cả index.
6. Tìm minimum.
7. Tìm maximum.
8. Tìm ký tự trong chuỗi.
9. Tìm số chẵn đầu tiên.
10. Tìm số âm đầu tiên.
11. Tìm phần tử lớn hơn một ngưỡng.
12. Kiểm tra mảng có chứa target không.

### Trung bình

1. Tìm phần tử lớn thứ hai.
2. Tìm phần tử nhỏ thứ hai.
3. Tìm hai phần tử có tổng bằng target.
4. Tìm phần tử không lặp đầu tiên.
5. Tìm phần tử lặp đầu tiên.
6. Tìm chuỗi con bằng brute force.
7. Tìm record theo nhiều trường.
8. Tìm Node trong Linked List.
9. Tìm phần tử gần target nhất.
10. Tìm khoảng liên tiếp thỏa điều kiện.
11. Sentinel Linear Search.
12. Tìm bằng predicate callback.

### Nâng cao

1. So sánh benchmark Linear và Binary Search.
2. Tối ưu tìm kiếm bằng SIMD.
3. Tìm kiếm song song trên mảng lớn.
4. Xây Hash Table thay Linear Search.
5. Tìm mẫu bằng KMP và so sánh brute force.
6. Thiết kế lookup table cho firmware.
7. Tìm command handler với nhiều chiến lược.
8. Xây API generic dùng `void *` và comparator.
9. Duyệt buffer DMA an toàn.
10. Thiết kế Linear Search có giới hạn thời gian.

---

## 22. Tổng kết

Linear Search kiểm tra từng phần tử theo thứ tự cho đến khi:

```text
Tìm thấy target
hoặc
Duyệt hết dữ liệu
```

Template cơ bản:

```c
for (index = 0U; index < size; index++)
{
    if (array[index] == target)
    {
        return (int)index;
    }
}

return -1;
```

Độ phức tạp:

```text
Best:    O(1)
Average: O(n)
Worst:   O(n)
Space:   O(1)
```

Linear Search phù hợp khi:

- Dữ liệu chưa sắp xếp.
- Tập dữ liệu nhỏ.
- Chỉ có ít truy vấn.
- Dữ liệu nằm trong Linked List.
- Điều kiện tìm kiếm phức tạp.
- Muốn giải pháp đơn giản và dễ kiểm chứng.

Các biến thể quan trọng:

- First Occurrence.
- Last Occurrence.
- Find All.
- Count Occurrences.
- Predicate Search.
- Sentinel Search.
- Search trên String.
- Search trên Linked List.
- Search trong bảng cấu hình hoặc command.

Không nên chỉ nhìn vào Big-O. Với mảng nhỏ và truy cập tuần tự, Linear Search có thể đơn giản, rõ ràng và đủ nhanh.

---

## 23. Tài liệu tham khảo

README này được biên soạn và diễn đạt lại độc lập dựa trên phạm vi chủ đề được trình bày trong:

- GeeksforGeeks — **Linear Search Algorithm**:  
  https://www.geeksforgeeks.org/dsa/linear-search/
