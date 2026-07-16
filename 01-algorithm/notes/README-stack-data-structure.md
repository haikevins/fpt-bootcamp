# Data Structures — Stack

> Tài liệu học tập độc lập về **Stack**, được trình bày theo hướng dễ hiểu và dễ áp dụng bằng ngôn ngữ C.

---

## Mục lục

1. [Stack là gì?](#1-stack-là-gì)
2. [Nguyên tắc LIFO](#2-nguyên-tắc-lifo)
3. [Các thuật ngữ cơ bản](#3-các-thuật-ngữ-cơ-bản)
4. [Các loại Stack](#4-các-loại-stack)
5. [Các thao tác cơ bản](#5-các-thao-tác-cơ-bản)
6. [Minh họa hoạt động](#6-minh-họa-hoạt-động)
7. [Cài đặt Stack bằng mảng](#7-cài-đặt-stack-bằng-mảng)
8. [Cài đặt Stack bằng Linked List](#8-cài-đặt-stack-bằng-linked-list)
9. [So sánh hai cách cài đặt](#9-so-sánh-hai-cách-cài-đặt)
10. [Độ phức tạp](#10-độ-phức-tạp)
11. [Ứng dụng của Stack](#11-ứng-dụng-của-stack)
12. [Stack trong bộ nhớ chương trình](#12-stack-trong-bộ-nhớ-chương-trình)
13. [Stack trong lập trình nhúng](#13-stack-trong-lập-trình-nhúng)
14. [Ưu điểm và hạn chế](#14-ưu-điểm-và-hạn-chế)
15. [Các lỗi thường gặp](#15-các-lỗi-thường-gặp)
16. [Bài tập luyện tập](#16-bài-tập-luyện-tập)
17. [Tổng kết](#17-tổng-kết)
18. [Tài liệu tham khảo](#18-tài-liệu-tham-khảo)

---

## 1. Stack là gì?

**Stack** là một cấu trúc dữ liệu tuyến tính, trong đó việc thêm và lấy phần tử chỉ diễn ra tại một đầu duy nhất, gọi là **top**.

Stack hoạt động theo nguyên tắc:

```text
LIFO — Last In, First Out
```

Có nghĩa là:

```text
Phần tử được đưa vào sau cùng
sẽ được lấy ra đầu tiên.
```

Ví dụ trực quan là một chồng đĩa:

```text
       +-------+
Top -> | Plate |
       +-------+
       | Plate |
       +-------+
       | Plate |
       +-------+
```

Ta chỉ có thể:

- Đặt một chiếc đĩa mới lên trên cùng.
- Lấy chiếc đĩa trên cùng ra trước.

---

## 2. Nguyên tắc LIFO

Giả sử lần lượt thêm ba giá trị:

```text
10, 20, 30
```

Thứ tự trong Stack:

```text
Top
 |
 v
30
20
10
```

Khi lấy phần tử ra:

```text
30 được lấy ra trước
20 được lấy ra tiếp theo
10 được lấy ra cuối cùng
```

Trình tự:

```text
Push: 10 -> 20 -> 30
Pop:  30 -> 20 -> 10
```

Stack đôi khi cũng được mô tả bằng thuật ngữ:

```text
FILO — First In, Last Out
```

Phần tử được đưa vào đầu tiên sẽ bị lấy ra cuối cùng.

---

## 3. Các thuật ngữ cơ bản

## 3.1. Top

`top` biểu diễn vị trí của phần tử trên cùng.

Với Stack dùng mảng:

```c
int top;
```

Khi Stack rỗng:

```c
top == -1
```

Khi có một phần tử:

```c
top == 0
```

---

## 3.2. Capacity

`capacity` là số phần tử tối đa Stack có thể chứa.

```c
size_t capacity;
```

Ví dụ:

```text
capacity = 8
```

Stack chỉ lưu tối đa 8 phần tử nếu dùng mảng kích thước cố định.

---

## 3.3. Size

`size` là số phần tử đang tồn tại trong Stack.

Với Stack dùng chỉ số `top`:

```c
size = top + 1
```

---

## 3.4. Overflow

**Stack Overflow** xảy ra khi cố thêm phần tử vào một Stack cố định đã đầy.

```text
top == capacity - 1
```

Ví dụ:

```text
Capacity = 4

Top
 |
 v
40
30
20
10
```

Không thể `push(50)` nếu không mở rộng vùng lưu trữ.

---

## 3.5. Underflow

**Stack Underflow** xảy ra khi cố lấy phần tử từ Stack đang rỗng.

```text
top == -1
```

Các thao tác cần kiểm tra underflow:

- `pop()`
- `peek()`
- `top()`

---

## 4. Các loại Stack

## 4.1. Fixed-size Stack

Fixed-size Stack có sức chứa xác định trước.

Thường được cài đặt bằng:

```text
Static Array
```

Ví dụ:

```c
#define STACK_CAPACITY 16U

int data[STACK_CAPACITY];
```

Ưu điểm:

- Bộ nhớ được xác định trước.
- Không cần cấp phát động.
- Dễ sử dụng trong embedded.
- Push và pop có thời gian cố định.

Hạn chế:

- Có thể overflow.
- Có thể lãng phí RAM nếu capacity quá lớn.

---

## 4.2. Dynamic Stack

Dynamic Stack có thể tăng hoặc giảm kích thước khi chương trình chạy.

Có thể cài đặt bằng:

- Linked List.
- Dynamic Array.
- Resizable container.

Ưu điểm:

- Không bị giới hạn bởi một capacity nhỏ cố định.
- Linh hoạt khi số lượng phần tử thay đổi.

Hạn chế:

- Phụ thuộc bộ nhớ động.
- Có thể hết Heap.
- Có nguy cơ phân mảnh bộ nhớ.
- Khó dự đoán thời gian cấp phát.
- Không phải lúc nào cũng phù hợp với embedded real-time.

---

## 5. Các thao tác cơ bản

## 5.1. Push

Thêm một phần tử lên đỉnh Stack.

```c
push(value);
```

Luồng xử lý:

```text
Kiểm tra Stack đầy
        |
        +-- Đầy --> Báo overflow
        |
        v
Tăng top
        |
        v
Ghi value vào data[top]
```

---

## 5.2. Pop

Lấy và xóa phần tử trên cùng.

```c
pop(&value);
```

Luồng xử lý:

```text
Kiểm tra Stack rỗng
        |
        +-- Rỗng --> Báo underflow
        |
        v
Đọc data[top]
        |
        v
Giảm top
```

---

## 5.3. Peek

Đọc phần tử trên cùng nhưng không xóa nó.

```c
peek(&value);
```

Stack không thay đổi sau thao tác này.

---

## 5.4. IsEmpty

Kiểm tra Stack có rỗng không.

```c
is_empty();
```

Với mảng:

```c
top == -1
```

---

## 5.5. IsFull

Kiểm tra Stack cố định có đầy không.

```c
is_full();
```

Với mảng:

```c
top == capacity - 1
```

---

## 5.6. Size

Trả về số phần tử hiện có.

```c
size();
```

Với `top` bắt đầu từ `-1`:

```c
size = (size_t)(top + 1)
```

---

## 6. Minh họa hoạt động

Stack ban đầu:

```text
Top = -1

[ _ ][ _ ][ _ ][ _ ][ _ ]
```

### Push 10

```text
Top = 0

[10 ][ _ ][ _ ][ _ ][ _ ]
 ^
 |
Top
```

### Push 20

```text
Top = 1

[10 ][20 ][ _ ][ _ ][ _ ]
      ^
      |
     Top
```

### Push 30

```text
Top = 2

[10 ][20 ][30 ][ _ ][ _ ]
           ^
           |
          Top
```

### Peek

```text
Giá trị nhận được: 30
```

Stack vẫn giữ nguyên:

```text
[10 ][20 ][30 ][ _ ][ _ ]
           ^
           |
          Top
```

### Pop

Giá trị được lấy ra:

```text
30
```

Sau đó:

```text
Top = 1

[10 ][20 ][30 ][ _ ][ _ ]
      ^
      |
     Top
```

Giá trị `30` có thể vẫn còn vật lý trong mảng, nhưng không còn thuộc vùng dữ liệu hợp lệ của Stack.

---

## 7. Cài đặt Stack bằng mảng

Cấu trúc project:

```text
array-stack/
├── include/
│   └── stack.h
├── src/
│   ├── stack.c
│   └── main.c
└── README.md
```

---

### 7.1. File `stack.h`

```c
#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stddef.h>

typedef struct
{
    int *data;
    size_t capacity;
    int top;
} Stack;

/**
 * @brief Initialize a stack using caller-provided storage.
 */
bool Stack_Init(
    Stack *stack,
    int *storage,
    size_t capacity
);

/**
 * @brief Push one value onto the stack.
 */
bool Stack_Push(
    Stack *stack,
    int value
);

/**
 * @brief Remove and return the top value.
 */
bool Stack_Pop(
    Stack *stack,
    int *value
);

/**
 * @brief Read the top value without removing it.
 */
bool Stack_Peek(
    const Stack *stack,
    int *value
);

/**
 * @brief Return true when the stack contains no elements.
 */
bool Stack_IsEmpty(const Stack *stack);

/**
 * @brief Return true when the stack has reached capacity.
 */
bool Stack_IsFull(const Stack *stack);

/**
 * @brief Return the current number of elements.
 */
size_t Stack_Size(const Stack *stack);

/**
 * @brief Return the maximum number of elements.
 */
size_t Stack_Capacity(const Stack *stack);

/**
 * @brief Remove all elements without modifying storage.
 */
void Stack_Clear(Stack *stack);

#endif
```

---

### 7.2. File `stack.c`

```c
#include "stack.h"

#include <limits.h>

bool Stack_Init(
    Stack *stack,
    int *storage,
    size_t capacity
)
{
    if ((stack == NULL) ||
        (storage == NULL) ||
        (capacity == 0U) ||
        (capacity > (size_t)INT_MAX))
    {
        return false;
    }

    stack->data = storage;
    stack->capacity = capacity;
    stack->top = -1;

    return true;
}

bool Stack_IsEmpty(const Stack *stack)
{
    return (stack == NULL) || (stack->top < 0);
}

bool Stack_IsFull(const Stack *stack)
{
    if ((stack == NULL) || (stack->capacity == 0U))
    {
        return false;
    }

    return stack->top == (int)(stack->capacity - 1U);
}

size_t Stack_Size(const Stack *stack)
{
    if (Stack_IsEmpty(stack))
    {
        return 0U;
    }

    return (size_t)(stack->top + 1);
}

size_t Stack_Capacity(const Stack *stack)
{
    if (stack == NULL)
    {
        return 0U;
    }

    return stack->capacity;
}

bool Stack_Push(
    Stack *stack,
    int value
)
{
    if ((stack == NULL) || (stack->data == NULL))
    {
        return false;
    }

    if (Stack_IsFull(stack))
    {
        return false;
    }

    stack->top++;
    stack->data[stack->top] = value;

    return true;
}

bool Stack_Pop(
    Stack *stack,
    int *value
)
{
    if ((stack == NULL) ||
        (stack->data == NULL) ||
        (value == NULL))
    {
        return false;
    }

    if (Stack_IsEmpty(stack))
    {
        return false;
    }

    *value = stack->data[stack->top];
    stack->top--;

    return true;
}

bool Stack_Peek(
    const Stack *stack,
    int *value
)
{
    if ((stack == NULL) ||
        (stack->data == NULL) ||
        (value == NULL))
    {
        return false;
    }

    if (Stack_IsEmpty(stack))
    {
        return false;
    }

    *value = stack->data[stack->top];

    return true;
}

void Stack_Clear(Stack *stack)
{
    if (stack == NULL)
    {
        return;
    }

    stack->top = -1;
}
```

---

### 7.3. File `main.c`

```c
#include "stack.h"

#include <stdio.h>

#define STACK_CAPACITY 5U

int main(void)
{
    Stack stack;
    int storage[STACK_CAPACITY];
    int value;

    if (!Stack_Init(
            &stack,
            storage,
            STACK_CAPACITY))
    {
        printf("Stack initialization failed.\n");
        return 1;
    }

    Stack_Push(&stack, 10);
    Stack_Push(&stack, 20);
    Stack_Push(&stack, 30);

    if (Stack_Peek(&stack, &value))
    {
        printf("Top: %d\n", value);
    }

    while (Stack_Pop(&stack, &value))
    {
        printf("Pop: %d\n", value);
    }

    return 0;
}
```

Kết quả:

```text
Top: 30
Pop: 30
Pop: 20
Pop: 10
```

---

### 7.4. Biên dịch

```bash
gcc -std=c11 \
    -Wall \
    -Wextra \
    -Wpedantic \
    -Iinclude \
    src/stack.c \
    src/main.c \
    -o stack_demo
```

Chạy:

```bash
./stack_demo
```

---

## 8. Cài đặt Stack bằng Linked List

Với Linked List, Node đầu tiên được dùng làm `top`.

```text
top
 |
 v
+------+-------+    +------+-------+    +------+------+
|  30  | next  | -> |  20  | next  | -> |  10  | NULL |
+------+-------+    +------+-------+    +------+------+
```

### Push

Thêm Node mới vào đầu danh sách.

```text
O(1)
```

### Pop

Xóa Node đầu danh sách.

```text
O(1)
```

---

### 8.1. File `linked_stack.h`

```c
#ifndef LINKED_STACK_H
#define LINKED_STACK_H

#include <stdbool.h>
#include <stddef.h>

typedef struct StackNode
{
    int data;
    struct StackNode *next;
} StackNode;

typedef struct
{
    StackNode *top;
    size_t size;
} LinkedStack;

void LinkedStack_Init(LinkedStack *stack);

bool LinkedStack_Push(
    LinkedStack *stack,
    int value
);

bool LinkedStack_Pop(
    LinkedStack *stack,
    int *value
);

bool LinkedStack_Peek(
    const LinkedStack *stack,
    int *value
);

bool LinkedStack_IsEmpty(const LinkedStack *stack);

size_t LinkedStack_Size(const LinkedStack *stack);

void LinkedStack_Clear(LinkedStack *stack);

#endif
```

---

### 8.2. File `linked_stack.c`

```c
#include "linked_stack.h"

#include <stdlib.h>

void LinkedStack_Init(LinkedStack *stack)
{
    if (stack == NULL)
    {
        return;
    }

    stack->top = NULL;
    stack->size = 0U;
}

bool LinkedStack_IsEmpty(const LinkedStack *stack)
{
    return (stack == NULL) || (stack->top == NULL);
}

size_t LinkedStack_Size(const LinkedStack *stack)
{
    if (stack == NULL)
    {
        return 0U;
    }

    return stack->size;
}

bool LinkedStack_Push(
    LinkedStack *stack,
    int value
)
{
    StackNode *new_node;

    if (stack == NULL)
    {
        return false;
    }

    new_node = (StackNode *)malloc(sizeof(StackNode));

    if (new_node == NULL)
    {
        return false;
    }

    new_node->data = value;
    new_node->next = stack->top;

    stack->top = new_node;
    stack->size++;

    return true;
}

bool LinkedStack_Pop(
    LinkedStack *stack,
    int *value
)
{
    StackNode *node_to_remove;

    if ((stack == NULL) || (value == NULL))
    {
        return false;
    }

    if (LinkedStack_IsEmpty(stack))
    {
        return false;
    }

    node_to_remove = stack->top;

    *value = node_to_remove->data;
    stack->top = node_to_remove->next;

    free(node_to_remove);
    stack->size--;

    return true;
}

bool LinkedStack_Peek(
    const LinkedStack *stack,
    int *value
)
{
    if ((stack == NULL) || (value == NULL))
    {
        return false;
    }

    if (LinkedStack_IsEmpty(stack))
    {
        return false;
    }

    *value = stack->top->data;

    return true;
}

void LinkedStack_Clear(LinkedStack *stack)
{
    StackNode *current;

    if (stack == NULL)
    {
        return;
    }

    current = stack->top;

    while (current != NULL)
    {
        StackNode *next_node = current->next;

        free(current);
        current = next_node;
    }

    stack->top = NULL;
    stack->size = 0U;
}
```

---

### 8.3. Lưu ý

Linked Stack không có overflow theo capacity cố định, nhưng vẫn có thể thất bại khi:

```c
malloc() == NULL
```

Do đó, không nên hiểu Dynamic Stack là có kích thước vô hạn.

Giới hạn thực tế vẫn là:

- Dung lượng Heap.
- Số Node tối đa.
- Chính sách bộ nhớ của hệ thống.

---

## 9. So sánh hai cách cài đặt

| Tiêu chí | Array Stack | Linked Stack |
|---|---|---|
| Cách lưu | Mảng liên tiếp | Các Node liên kết |
| Capacity | Thường cố định | Tăng theo bộ nhớ khả dụng |
| Push | `O(1)` | `O(1)` |
| Pop | `O(1)` | `O(1)` |
| Peek | `O(1)` | `O(1)` |
| Bộ nhớ phụ mỗi phần tử | Không | Một con trỏ `next` |
| Cache locality | Tốt | Thường kém hơn |
| Cấp phát động | Không bắt buộc | Thường có |
| Overflow | Khi đầy | Khi không cấp phát được Node |
| Phân mảnh Heap | Không | Có thể |
| Phù hợp embedded | Rất phù hợp | Chỉ nên dùng khi quản lý bộ nhớ rõ ràng |

Trong phần mềm nhúng, Array Stack thường dễ kiểm soát hơn.

Nếu cần Dynamic Stack nhưng không muốn dùng Heap, có thể dùng:

```text
Linked List + Memory Pool
```

---

## 10. Độ phức tạp

| Thao tác | Array Stack | Linked Stack |
|---|---:|---:|
| Push | `O(1)` | `O(1)` |
| Pop | `O(1)` | `O(1)` |
| Peek | `O(1)` | `O(1)` |
| IsEmpty | `O(1)` | `O(1)` |
| IsFull | `O(1)` | Không áp dụng trực tiếp |
| Size | `O(1)` | `O(1)` nếu lưu biến `size` |
| Clear | `O(1)` logic | `O(n)` để giải phóng Node |
| Tìm kiếm phần tử | `O(n)` | `O(n)` |

Stack không được thiết kế để tìm kiếm hoặc truy cập phần tử ở giữa.

---

## 11. Ứng dụng của Stack

## 11.1. Function Call Stack

Khi một hàm gọi hàm khác, hệ thống cần lưu trạng thái của hàm hiện tại.

Khái quát:

```text
main()
  |
  v
FunctionA()
  |
  v
FunctionB()
```

Call Stack:

```text
Top
 |
 v
FunctionB frame
FunctionA frame
main frame
```

Khi `FunctionB()` kết thúc:

```text
Pop FunctionB frame
```

Chương trình quay lại đúng vị trí trong `FunctionA()`.

---

## 11.2. Recursion

Mỗi lời gọi đệ quy tạo thêm một stack frame.

Ví dụ:

```c
int factorial(int n)
{
    if (n <= 1)
    {
        return 1;
    }

    return n * factorial(n - 1);
}
```

Với:

```c
factorial(4)
```

Call Stack phát triển:

```text
factorial(4)
factorial(3)
factorial(2)
factorial(1)
```

Sau khi gặp điều kiện dừng, các frame được lấy ra theo thứ tự ngược lại.

Nếu recursion quá sâu, có thể gây:

```text
Call Stack Overflow
```

---

## 11.3. Kiểm tra dấu ngoặc hợp lệ

Chuỗi:

```text
({[]})
```

Quy tắc:

- Gặp dấu mở: push vào Stack.
- Gặp dấu đóng: so sánh với phần tử top.
- Nếu khớp: pop.
- Kết thúc chuỗi, Stack phải rỗng.

Ví dụ:

```c
bool IsMatchingPair(char opening, char closing)
{
    return ((opening == '(') && (closing == ')')) ||
           ((opening == '[') && (closing == ']')) ||
           ((opening == '{') && (closing == '}'));
}
```

---

## 11.4. Expression Evaluation

Stack được dùng để xử lý:

- Infix.
- Prefix.
- Postfix.
- Chuyển đổi biểu thức.
- Thứ tự ưu tiên toán tử.

Ví dụ postfix:

```text
3 4 + 5 *
```

Các bước:

```text
Push 3
Push 4
Pop 4 và 3
Tính 3 + 4 = 7
Push 7
Push 5
Pop 5 và 7
Tính 7 * 5 = 35
```

Kết quả:

```text
35
```

---

## 11.5. Undo và Redo

Ứng dụng chỉnh sửa có thể dùng hai Stack:

```text
Undo Stack
Redo Stack
```

Khi thực hiện thao tác mới:

```text
Push thao tác vào Undo Stack
Clear Redo Stack
```

Khi Undo:

```text
Pop từ Undo Stack
Thực hiện thao tác ngược
Push vào Redo Stack
```

---

## 11.6. Browser History

Hai Stack có thể biểu diễn:

```text
Back history
Forward history
```

Khi người dùng quay lại trang trước, trang hiện tại được đưa vào Forward Stack.

---

## 11.7. Depth-First Search

DFS có thể dùng:

- Recursion.
- Stack tường minh.

Ví dụ:

```text
Push start node
While stack not empty:
    Pop node
    Visit node
    Push unvisited neighbors
```

---

## 11.8. Backtracking

Stack phù hợp với các bài toán cần quay lại trạng thái trước:

- Mê cung.
- N-Queens.
- Sudoku.
- Sinh hoán vị.
- Tìm đường.
- Parser.
- State exploration.

---

## 11.9. Monotonic Stack

Monotonic Stack duy trì các phần tử theo thứ tự tăng hoặc giảm.

Ứng dụng:

- Next Greater Element.
- Previous Greater Element.
- Next Smaller Element.
- Previous Smaller Element.
- Stock Span.
- Largest Rectangle in Histogram.
- Trapping Rain Water.
- Maximum Rectangle.

---

## 11.10. Đảo ngược dữ liệu

Do tính chất LIFO, Stack có thể đảo ngược:

- Chuỗi.
- Mảng.
- Linked List.
- Thứ tự thao tác.
- Các phần tử trong Queue.

---

## 12. Stack trong bộ nhớ chương trình

Cần phân biệt hai khái niệm:

### Stack Data Structure

Là cấu trúc dữ liệu do lập trình viên thiết kế.

Ví dụ:

```c
Stack stack;
```

### Call Stack / Runtime Stack

Là vùng bộ nhớ được hệ thống dùng cho:

- Địa chỉ trở về.
- Tham số hàm.
- Biến cục bộ.
- Thanh ghi được lưu.
- Stack frame.
- Context của lời gọi hàm.

Hai khái niệm cùng dùng nguyên tắc LIFO nhưng không hoàn toàn giống nhau.

---

## 12.1. Biến cục bộ

Ví dụ:

```c
void Function(void)
{
    int value = 10;
    char buffer[128];
}
```

Tùy compiler, ABI và tối ưu hóa, `value` và `buffer` có thể được đặt trong stack frame.

Khi hàm kết thúc, vùng nhớ frame được thu hồi tự động.

---

## 12.2. Stack Overflow của chương trình

Call Stack Overflow có thể xảy ra do:

- Recursion không có điều kiện dừng.
- Recursion quá sâu.
- Mảng cục bộ quá lớn.
- Nhiều hàm lồng nhau.
- Stack của RTOS task quá nhỏ.
- Interrupt nesting sâu.
- Context lưu quá nhiều dữ liệu.

Ví dụ nguy hiểm trong embedded:

```c
void Process(void)
{
    uint8_t large_buffer[8192];
}
```

Trên MCU có ít RAM, biến cục bộ lớn có thể làm tràn Stack.

---

## 13. Stack trong lập trình nhúng

## 13.1. Ưu tiên Stack cố định

Trong firmware, Stack dùng mảng tĩnh có các lợi ích:

- RAM tối đa biết trước.
- Không dùng `malloc()`.
- Không bị phân mảnh Heap.
- Push/pop có thời gian dễ dự đoán.
- Dễ kiểm thử overflow và underflow.

---

## 13.2. Không nhầm với MSP và PSP

Trên ARM Cortex-M, CPU có thể có:

- Main Stack Pointer — MSP.
- Process Stack Pointer — PSP.

Đây là stack của CPU và context thực thi, không phải một Stack container thông thường.

---

## 13.3. Stack của RTOS Task

Mỗi task thường có vùng Stack riêng:

```text
Task A Stack
Task B Stack
Task C Stack
```

Stack phải đủ cho:

- Biến cục bộ.
- Chuỗi lời gọi hàm.
- Context switch.
- Interrupt hoặc exception theo thiết kế RTOS.
- Thư viện được gọi.
- Worst-case execution path.

---

## 13.4. High-Water Mark

RTOS thường cung cấp cách kiểm tra lượng Stack tối thiểu còn trống trong quá trình chạy.

Mục tiêu:

- Phát hiện Stack quá nhỏ.
- Tránh cấp dư RAM.
- Đánh giá worst-case thực tế.

Không nên chỉ dựa vào một bài test ngắn. Cần kiểm thử các đường chạy xấu nhất.

---

## 13.5. ISR và Stack

Khi vào interrupt, CPU cần lưu context.

Interrupt lồng nhau có thể làm tăng mức sử dụng Stack.

Các ISR nên:

- Ngắn.
- Tránh biến cục bộ lớn.
- Tránh recursion.
- Tránh gọi chuỗi hàm sâu.
- Tránh `printf()` nếu không cần thiết.
- Tránh cấp phát động.

---

## 13.6. Stack Container dùng giữa ISR và Main

Một Stack thông thường không tự động thread-safe.

Nếu ISR và `main()` cùng thay đổi `top`, có thể xảy ra race condition.

Cần:

- Critical section.
- Tắt interrupt trong đoạn rất ngắn.
- Atomic operation.
- Quy tắc single-producer/single-consumer phù hợp.
- Cơ chế đồng bộ của RTOS.

`volatile` không thay thế cơ chế đồng bộ.

---

## 14. Ưu điểm và hạn chế

## 14.1. Ưu điểm

- Cấu trúc đơn giản.
- Push và pop nhanh.
- Các thao tác chính thường là `O(1)`.
- Phù hợp với LIFO.
- Hữu ích trong recursion và parsing.
- Dễ cài đặt bằng mảng hoặc Linked List.
- Array Stack có cache locality tốt.
- Dễ dùng để đảo ngược thứ tự dữ liệu.

---

## 14.2. Hạn chế

- Chỉ truy cập trực tiếp phần tử top.
- Khó truy cập hoặc sửa phần tử ở giữa.
- Fixed Stack có thể overflow.
- Pop từ Stack rỗng gây underflow.
- Linked Stack tốn thêm bộ nhớ cho con trỏ.
- Dynamic Stack có thể thất bại khi hết Heap.
- Không phù hợp nếu cần FIFO.
- Call Stack có thể tràn khi recursion sâu.

---

## 15. Các lỗi thường gặp

## 15.1. Không kiểm tra overflow

Sai:

```c
stack->data[++stack->top] = value;
```

Nếu Stack đầy, câu lệnh ghi ra ngoài mảng.

Đúng:

```c
if (Stack_IsFull(stack))
{
    return false;
}
```

---

## 15.2. Không kiểm tra underflow

Sai:

```c
*value = stack->data[stack->top--];
```

Nếu:

```c
top == -1
```

chương trình truy cập chỉ số âm.

---

## 15.3. Khởi tạo `top` sai

Đúng:

```c
top = -1;
```

Nếu khởi tạo:

```c
top = 0;
```

phải định nghĩa lại toàn bộ ý nghĩa của `top` và điều kiện rỗng.

Không nên trộn hai quy ước.

---

## 15.4. Tăng `top` sai thứ tự

Với quy ước:

```c
top = -1
```

Cách đúng:

```c
stack->top++;
stack->data[stack->top] = value;
```

Hoặc:

```c
stack->data[++stack->top] = value;
```

---

## 15.5. Giảm `top` trước khi đọc

Sai:

```c
stack->top--;
*value = stack->data[stack->top];
```

Cách này trả về phần tử bên dưới top cũ.

Đúng:

```c
*value = stack->data[stack->top];
stack->top--;
```

---

## 15.6. Dùng giá trị đặc biệt để báo lỗi

Ví dụ:

```c
return -1;
```

có thể gây mơ hồ nếu `-1` là dữ liệu hợp lệ.

Tốt hơn:

```c
bool Stack_Pop(Stack *stack, int *value);
```

Giá trị trả về cho biết thành công hay thất bại.

---

## 15.7. Quên giải phóng Linked Stack

Mỗi Node được tạo bằng `malloc()` phải được giải phóng.

```c
LinkedStack_Clear(&stack);
```

Nếu không, chương trình bị memory leak.

---

## 15.8. Recursion vô hạn

Sai:

```c
void Recursive(void)
{
    Recursive();
}
```

Mỗi lời gọi thêm một stack frame cho đến khi tràn Call Stack.

---

## 16. Bài tập luyện tập

## Cơ bản

1. Cài đặt Stack bằng mảng.
2. Cài đặt Stack bằng Linked List.
3. Viết `push`, `pop`, `peek`, `isEmpty`, `size`.
4. Đảo ngược chuỗi bằng Stack.
5. Đảo ngược mảng bằng Stack.
6. Kiểm tra dấu ngoặc hợp lệ.
7. Chuyển số thập phân sang nhị phân.
8. Kiểm tra chuỗi palindrome bằng Stack.
9. Xóa phần tử giữa Stack.
10. Sắp xếp Stack.

## Trung bình

1. Cài đặt Queue bằng hai Stack.
2. Cài đặt Stack bằng hai Queue.
3. Chuyển Infix sang Postfix.
4. Chuyển Infix sang Prefix.
5. Tính biểu thức Postfix.
6. Tính biểu thức Prefix.
7. Next Greater Element.
8. Previous Greater Element.
9. Next Smaller Element.
10. Stock Span.
11. Simplify Path.
12. Decode String.
13. Remove Adjacent Duplicates.
14. Asteroid Collision.
15. Daily Temperatures.

## Nâng cao

1. Largest Rectangle in Histogram.
2. Maximal Rectangle.
3. Trapping Rain Water bằng Stack.
4. Min Stack với `getMin()` là `O(1)`.
5. Max Stack.
6. Frequency Stack.
7. Longest Valid Parentheses.
8. Basic Calculator.
9. Parse Boolean Expression.
10. Iterative DFS.
11. Tarjan-style algorithms sử dụng Stack.
12. Thiết kế Undo/Redo system.
13. Cài đặt nhiều Stack trong một mảng.
14. Cài đặt Stack lock-free.
15. Dùng Memory Pool cho Linked Stack trong embedded.

---

## 17. Tổng kết

Stack là cấu trúc dữ liệu tuyến tính hoạt động theo:

```text
LIFO — Last In, First Out
```

Các thao tác chính:

```text
push()    → thêm phần tử lên top
pop()     → lấy và xóa phần tử top
peek()    → đọc phần tử top
isEmpty() → kiểm tra rỗng
isFull()  → kiểm tra đầy
size()    → lấy số phần tử
```

Với Array Stack:

```text
Empty: top == -1
Full:  top == capacity - 1
Size:  top + 1
```

Độ phức tạp của các thao tác cơ bản:

```text
Push: O(1)
Pop:  O(1)
Peek: O(1)
```

Stack được ứng dụng trong:

- Function call.
- Recursion.
- Expression evaluation.
- Syntax parsing.
- Undo/redo.
- Browser history.
- DFS.
- Backtracking.
- Monotonic Stack.
- Quản lý context trong hệ thống.

Trong embedded:

- Nên ưu tiên Stack dùng mảng cố định.
- Luôn kiểm tra overflow và underflow.
- Tránh biến cục bộ quá lớn.
- Theo dõi Stack của từng RTOS task.
- Hạn chế recursion sâu.
- Không coi `volatile` là cơ chế đồng bộ.
- Dùng Memory Pool nếu cần Dynamic Stack có giới hạn xác định.

---

## 18. Tài liệu tham khảo

README này được biên soạn và diễn đạt lại độc lập dựa trên chủ đề được trình bày trong:

- GeeksforGeeks — **Stack Data Structure**:  
  https://www.geeksforgeeks.org/dsa/stack-data-structure/
