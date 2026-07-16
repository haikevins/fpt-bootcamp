# Data Structures — Danh sách liên kết đơn (Singly Linked List)

> Tài liệu học tập độc lập về Linked List, được tổng hợp theo hướng dễ áp dụng trong C và lập trình nhúng.

---

## Mục lục

1. [Linked List là gì?](#1-linked-list-là-gì)
2. [Linked List và Array khác nhau như thế nào?](#2-linked-list-và-array-khác-nhau-như-thế-nào)
3. [Cấu tạo của một Node](#3-cấu-tạo-của-một-node)
4. [Cấu trúc của Singly Linked List](#4-cấu-trúc-của-singly-linked-list)
5. [Các loại Linked List](#5-các-loại-linked-list)
6. [Các thao tác cơ bản](#6-các-thao-tác-cơ-bản)
7. [Cài đặt Singly Linked List bằng C](#7-cài-đặt-singly-linked-list-bằng-c)
8. [Ví dụ chạy chương trình](#8-ví-dụ-chạy-chương-trình)
9. [Phân tích từng thao tác](#9-phân-tích-từng-thao-tác)
10. [Độ phức tạp](#10-độ-phức-tạp)
11. [Ứng dụng của Linked List](#11-ứng-dụng-của-linked-list)
12. [Linked List trong lập trình nhúng](#12-linked-list-trong-lập-trình-nhúng)
13. [Các lỗi thường gặp](#13-các-lỗi-thường-gặp)
14. [Bài tập luyện tập](#14-bài-tập-luyện-tập)
15. [Tổng kết](#15-tổng-kết)
16. [Tài liệu tham khảo](#16-tài-liệu-tham-khảo)

---

## 1. Linked List là gì?

**Linked List** là một cấu trúc dữ liệu tuyến tính gồm nhiều phần tử gọi là **Node**.

Các Node không bắt buộc nằm liên tiếp nhau trong bộ nhớ. Thay vào đó, mỗi Node giữ địa chỉ của Node tiếp theo để tạo thành một chuỗi liên kết.

```text
head
  |
  v
+------+-------+    +------+-------+    +------+-------+
| data | next  | -> | data | next  | -> | data | NULL  |
+------+-------+    +------+-------+    +------+-------+
```

Trong **Singly Linked List**, mỗi Node chỉ biết Node đứng ngay sau nó.

Một danh sách có thể dài ra khi thêm Node mới và ngắn lại khi một Node bị xóa.

### Đặc điểm chính

- Các phần tử không cần nằm liên tiếp trong RAM.
- Kích thước có thể thay đổi trong khi chương trình chạy.
- Thêm hoặc xóa Node không yêu cầu dịch chuyển toàn bộ dữ liệu còn lại.
- Muốn truy cập một Node phải duyệt từ `head`.
- Không hỗ trợ truy cập ngẫu nhiên trực tiếp bằng chỉ số như Array.

---

## 2. Linked List và Array khác nhau như thế nào?

### Array

Array lưu các phần tử trong một vùng nhớ liên tiếp:

```text
Index:     0       1       2       3
        +-----+ +-----+ +-----+ +-----+
Array:  | 10  | | 20  | | 30  | | 40  |
        +-----+ +-----+ +-----+ +-----+
```

Ta có thể truy cập trực tiếp:

```c
array[2]
```

### Linked List

Các Node có thể nằm ở những vị trí khác nhau trong bộ nhớ:

```text
0x1000             0x2300             0x1810
+------+-------+   +------+-------+   +------+-------+
|  10  | 0x2300|-> |  20  | 0x1810|-> |  30  | NULL  |
+------+-------+   +------+-------+   +------+-------+
```

Để đến Node thứ ba, chương trình phải đi qua Node thứ nhất và Node thứ hai.

### Bảng so sánh

| Tiêu chí | Array | Linked List |
|---|---|---|
| Vị trí trong bộ nhớ | Liên tiếp | Không nhất thiết liên tiếp |
| Kích thước | Thường xác định trước | Có thể thay đổi động |
| Truy cập theo chỉ số | Nhanh, `O(1)` | Không trực tiếp, thường `O(n)` |
| Thêm đầu danh sách | Phải dịch phần tử, `O(n)` | `O(1)` |
| Xóa đầu danh sách | Phải dịch phần tử, `O(n)` | `O(1)` |
| Thêm cuối | `O(1)` nếu còn chỗ | `O(1)` nếu có `tail`, nếu không là `O(n)` |
| Bộ nhớ phụ | Không cần con trỏ cho từng phần tử | Mỗi Node cần thêm con trỏ |
| Cache locality | Tốt | Thường kém hơn |
| Cấp phát động | Không bắt buộc | Thường dùng, nhưng có thể thay bằng memory pool |

### Khi nào nên dùng Array?

- Biết trước số phần tử tối đa.
- Cần truy cập nhanh theo chỉ số.
- RAM hạn chế và không muốn tốn thêm bộ nhớ cho con trỏ.
- Cần tận dụng cache hoặc DMA.
- Dữ liệu ít thêm, xóa ở giữa.

### Khi nào nên dùng Linked List?

- Số phần tử thay đổi khi chương trình chạy.
- Thường xuyên thêm hoặc xóa phần tử.
- Không cần truy cập ngẫu nhiên theo chỉ số.
- Mỗi phần tử có vòng đời độc lập.
- Muốn tổ chức queue, stack, event list hoặc memory pool.

---

## 3. Cấu tạo của một Node

Một Node của Singly Linked List gồm hai thành phần:

1. **Data**: dữ liệu của Node.
2. **Next**: con trỏ đến Node kế tiếp.

```c
typedef struct Node
{
    int data;
    struct Node *next;
} Node;
```

### Giải thích

```c
int data;
```

Lưu giá trị của Node. Kiểu `int` có thể được thay bằng:

- `uint8_t`
- `uint16_t`
- `float`
- một `struct`
- một con trỏ `void *`
- kiểu dữ liệu riêng của ứng dụng

```c
struct Node *next;
```

Lưu địa chỉ của Node tiếp theo.

Node cuối cùng có:

```c
next == NULL
```

---

## 4. Cấu trúc của Singly Linked List

Ta có thể quản lý danh sách chỉ bằng `head`:

```c
typedef struct
{
    Node *head;
} SinglyLinkedList;
```

Tuy nhiên, nếu thường xuyên thêm vào cuối, nên giữ thêm `tail`:

```c
typedef struct
{
    Node *head;
    Node *tail;
    size_t size;
} SinglyLinkedList;
```

### Ý nghĩa

```text
head → Node đầu tiên
tail → Node cuối cùng
size → số Node hiện có
```

Danh sách rỗng:

```text
head = NULL
tail = NULL
size = 0
```

Danh sách có một Node:

```text
head
  |
  v
+------+------+
|  10  | NULL |
+------+------+
  ^
  |
tail
```

Khi chỉ có một phần tử:

```c
head == tail
```

---

## 5. Các loại Linked List

## 5.1. Singly Linked List

Mỗi Node chỉ trỏ đến Node tiếp theo.

```text
A -> B -> C -> NULL
```

Chỉ duyệt thuận từ đầu đến cuối.

---

## 5.2. Doubly Linked List

Mỗi Node có hai liên kết:

- `next`: Node tiếp theo.
- `prev`: Node phía trước.

```text
NULL <- A <-> B <-> C -> NULL
```

Có thể duyệt theo cả hai chiều nhưng mỗi Node tốn thêm bộ nhớ cho con trỏ `prev`.

---

## 5.3. Circular Singly Linked List

Node cuối trỏ lại Node đầu:

```text
     +------------------+
     |                  |
     v                  |
A -> B -> C ------------+
```

Không có Node nào có `next == NULL`.

Phù hợp với:

- Round-robin scheduler.
- Danh sách tác vụ tuần hoàn.
- Quản lý lượt chơi.
- Bộ lập lịch dạng vòng.

---

## 5.4. Circular Doubly Linked List

Node đầu và Node cuối liên kết với nhau theo cả hai chiều.

```text
+-----------------------+
|                       v
A <-> B <-> C <---------+
^                       |
+-----------------------+
```

Thường được sử dụng khi cần:

- Chèn, xóa nhanh ở nhiều vị trí.
- Duyệt hai chiều.
- Danh sách quản lý task hoặc resource phức tạp.

---

## 6. Các thao tác cơ bản

Một Singly Linked List thường cần các thao tác:

- Khởi tạo danh sách.
- Kiểm tra danh sách rỗng.
- Tạo Node.
- Thêm vào đầu.
- Thêm vào cuối.
- Chèn sau một Node.
- Tìm kiếm theo giá trị.
- Xóa Node đầu.
- Xóa theo giá trị.
- Duyệt danh sách.
- Đếm số Node.
- Xóa toàn bộ danh sách.

---

## 7. Cài đặt Singly Linked List bằng C

Cấu trúc project:

```text
singly-linked-list/
├── include/
│   └── singly_linked_list.h
├── src/
│   ├── singly_linked_list.c
│   └── main.c
└── README.md
```

---

### 7.1. File `singly_linked_list.h`

```c
#ifndef SINGLY_LINKED_LIST_H
#define SINGLY_LINKED_LIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct SllNode
{
    int data;
    struct SllNode *next;
} SllNode;

typedef struct
{
    SllNode *head;
    SllNode *tail;
    size_t size;
} SinglyLinkedList;

/**
 * @brief Initialize an empty singly linked list.
 */
void Sll_Init(SinglyLinkedList *list);

/**
 * @brief Return true when the list has no nodes.
 */
bool Sll_IsEmpty(const SinglyLinkedList *list);

/**
 * @brief Return the current number of nodes.
 */
size_t Sll_Size(const SinglyLinkedList *list);

/**
 * @brief Insert a new value at the beginning of the list.
 */
bool Sll_PushFront(SinglyLinkedList *list, int value);

/**
 * @brief Insert a new value at the end of the list.
 */
bool Sll_PushBack(SinglyLinkedList *list, int value);

/**
 * @brief Insert a value immediately after the specified node.
 */
bool Sll_InsertAfter(
    SinglyLinkedList *list,
    SllNode *position,
    int value
);

/**
 * @brief Return the first node whose data equals value.
 */
SllNode *Sll_Find(
    const SinglyLinkedList *list,
    int value
);

/**
 * @brief Remove the first node and optionally return its value.
 */
bool Sll_PopFront(
    SinglyLinkedList *list,
    int *removed_value
);

/**
 * @brief Remove the first node whose data equals value.
 */
bool Sll_RemoveFirst(
    SinglyLinkedList *list,
    int value
);

/**
 * @brief Print all values from head to tail.
 */
void Sll_Print(const SinglyLinkedList *list);

/**
 * @brief Free all nodes and reset the list.
 */
void Sll_Clear(SinglyLinkedList *list);

#endif
```

---

### 7.2. File `singly_linked_list.c`

```c
#include "singly_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

static SllNode *Sll_CreateNode(int value)
{
    SllNode *node = (SllNode *)malloc(sizeof(SllNode));

    if (node == NULL)
    {
        return NULL;
    }

    node->data = value;
    node->next = NULL;

    return node;
}

void Sll_Init(SinglyLinkedList *list)
{
    if (list == NULL)
    {
        return;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0U;
}

bool Sll_IsEmpty(const SinglyLinkedList *list)
{
    return (list == NULL) || (list->head == NULL);
}

size_t Sll_Size(const SinglyLinkedList *list)
{
    if (list == NULL)
    {
        return 0U;
    }

    return list->size;
}

bool Sll_PushFront(SinglyLinkedList *list, int value)
{
    SllNode *new_node;

    if (list == NULL)
    {
        return false;
    }

    new_node = Sll_CreateNode(value);

    if (new_node == NULL)
    {
        return false;
    }

    new_node->next = list->head;
    list->head = new_node;

    if (list->tail == NULL)
    {
        list->tail = new_node;
    }

    list->size++;

    return true;
}

bool Sll_PushBack(SinglyLinkedList *list, int value)
{
    SllNode *new_node;

    if (list == NULL)
    {
        return false;
    }

    new_node = Sll_CreateNode(value);

    if (new_node == NULL)
    {
        return false;
    }

    if (list->tail == NULL)
    {
        list->head = new_node;
        list->tail = new_node;
    }
    else
    {
        list->tail->next = new_node;
        list->tail = new_node;
    }

    list->size++;

    return true;
}

bool Sll_InsertAfter(
    SinglyLinkedList *list,
    SllNode *position,
    int value
)
{
    SllNode *new_node;

    if ((list == NULL) || (position == NULL))
    {
        return false;
    }

    new_node = Sll_CreateNode(value);

    if (new_node == NULL)
    {
        return false;
    }

    new_node->next = position->next;
    position->next = new_node;

    if (list->tail == position)
    {
        list->tail = new_node;
    }

    list->size++;

    return true;
}

SllNode *Sll_Find(
    const SinglyLinkedList *list,
    int value
)
{
    SllNode *current;

    if (list == NULL)
    {
        return NULL;
    }

    current = list->head;

    while (current != NULL)
    {
        if (current->data == value)
        {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

bool Sll_PopFront(
    SinglyLinkedList *list,
    int *removed_value
)
{
    SllNode *node_to_remove;

    if ((list == NULL) || (list->head == NULL))
    {
        return false;
    }

    node_to_remove = list->head;

    if (removed_value != NULL)
    {
        *removed_value = node_to_remove->data;
    }

    list->head = node_to_remove->next;
    free(node_to_remove);

    list->size--;

    if (list->head == NULL)
    {
        list->tail = NULL;
    }

    return true;
}

bool Sll_RemoveFirst(
    SinglyLinkedList *list,
    int value
)
{
    SllNode *previous;
    SllNode *current;

    if ((list == NULL) || (list->head == NULL))
    {
        return false;
    }

    previous = NULL;
    current = list->head;

    while (current != NULL)
    {
        if (current->data == value)
        {
            if (previous == NULL)
            {
                list->head = current->next;
            }
            else
            {
                previous->next = current->next;
            }

            if (list->tail == current)
            {
                list->tail = previous;
            }

            free(current);
            list->size--;

            if (list->head == NULL)
            {
                list->tail = NULL;
            }

            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;
}

void Sll_Print(const SinglyLinkedList *list)
{
    const SllNode *current;

    if (list == NULL)
    {
        printf("List is invalid.\n");
        return;
    }

    current = list->head;

    printf("HEAD -> ");

    while (current != NULL)
    {
        printf("%d -> ", current->data);
        current = current->next;
    }

    printf("NULL\n");
}

void Sll_Clear(SinglyLinkedList *list)
{
    SllNode *current;

    if (list == NULL)
    {
        return;
    }

    current = list->head;

    while (current != NULL)
    {
        SllNode *next_node = current->next;

        free(current);
        current = next_node;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0U;
}
```

---

### 7.3. File `main.c`

```c
#include "singly_linked_list.h"

#include <stdio.h>

int main(void)
{
    SinglyLinkedList list;
    SllNode *node;
    int removed_value;

    Sll_Init(&list);

    Sll_PushBack(&list, 10);
    Sll_PushBack(&list, 20);
    Sll_PushBack(&list, 30);

    printf("After push back:\n");
    Sll_Print(&list);

    Sll_PushFront(&list, 5);

    printf("After push front:\n");
    Sll_Print(&list);

    node = Sll_Find(&list, 20);

    if (node != NULL)
    {
        Sll_InsertAfter(&list, node, 25);
    }

    printf("After inserting 25 after 20:\n");
    Sll_Print(&list);

    if (Sll_PopFront(&list, &removed_value))
    {
        printf("Removed from front: %d\n", removed_value);
    }

    printf("After pop front:\n");
    Sll_Print(&list);

    if (Sll_RemoveFirst(&list, 25))
    {
        printf("Removed value 25.\n");
    }

    printf("Final list:\n");
    Sll_Print(&list);

    printf("List size: %zu\n", Sll_Size(&list));

    Sll_Clear(&list);

    return 0;
}
```

---

### 7.4. Biên dịch

```bash
gcc -std=c11 \
    -Wall \
    -Wextra \
    -Wpedantic \
    -Iinclude \
    src/singly_linked_list.c \
    src/main.c \
    -o linked_list_demo
```

Chạy:

```bash
./linked_list_demo
```

---

## 8. Ví dụ chạy chương trình

Kết quả dự kiến:

```text
After push back:
HEAD -> 10 -> 20 -> 30 -> NULL

After push front:
HEAD -> 5 -> 10 -> 20 -> 30 -> NULL

After inserting 25 after 20:
HEAD -> 5 -> 10 -> 20 -> 25 -> 30 -> NULL

Removed from front: 5

After pop front:
HEAD -> 10 -> 20 -> 25 -> 30 -> NULL

Removed value 25.

Final list:
HEAD -> 10 -> 20 -> 30 -> NULL

List size: 3
```

---

## 9. Phân tích từng thao tác

## 9.1. Khởi tạo danh sách

```c
void Sll_Init(SinglyLinkedList *list)
{
    list->head = NULL;
    list->tail = NULL;
    list->size = 0U;
}
```

Sau khi khởi tạo:

```text
head -> NULL
tail -> NULL
size = 0
```

---

## 9.2. Tạo Node mới

```c
SllNode *node = malloc(sizeof(SllNode));
```

`malloc()` yêu cầu một vùng nhớ có kích thước đủ chứa `SllNode`.

Nếu thành công:

```c
node->data = value;
node->next = NULL;
```

Nếu thất bại:

```c
node == NULL
```

Chương trình phải kiểm tra kết quả của `malloc()` trước khi sử dụng Node.

---

## 9.3. Thêm vào đầu

Danh sách ban đầu:

```text
head
  |
  v
  10 -> 20 -> 30 -> NULL
```

Cần thêm `5`.

### Bước 1: Tạo Node

```text
new_node
   |
   v
   5 -> NULL
```

### Bước 2: Cho Node mới trỏ đến Node đầu cũ

```c
new_node->next = list->head;
```

```text
5 -> 10 -> 20 -> 30 -> NULL
```

### Bước 3: Cập nhật `head`

```c
list->head = new_node;
```

Kết quả:

```text
head
  |
  v
  5 -> 10 -> 20 -> 30 -> NULL
```

Thao tác này không phụ thuộc số Node nên có độ phức tạp `O(1)`.

---

## 9.4. Thêm vào cuối

Nếu danh sách rỗng:

```c
list->head = new_node;
list->tail = new_node;
```

Nếu danh sách không rỗng:

```c
list->tail->next = new_node;
list->tail = new_node;
```

Minh họa:

```text
Trước:

head                       tail
  |                          |
  v                          v
 10 -> 20 -> 30 -> NULL

Node mới:

 40 -> NULL
```

Liên kết Node cuối cũ với Node mới:

```text
10 -> 20 -> 30 -> 40 -> NULL
```

Cập nhật `tail`:

```text
head                             tail
  |                                |
  v                                v
 10 -> 20 -> 30 -> 40 -> NULL
```

Vì danh sách có `tail`, thao tác thêm cuối là `O(1)`.

---

## 9.5. Chèn sau một Node

Giả sử cần chèn `25` sau `20`:

```text
10 -> 20 -> 30 -> NULL
```

Thứ tự cập nhật phải là:

```c
new_node->next = position->next;
position->next = new_node;
```

Kết quả:

```text
10 -> 20 -> 25 -> 30 -> NULL
```

Không nên đảo ngược hai câu lệnh trên vì có thể làm mất liên kết đến phần còn lại của danh sách.

---

## 9.6. Tìm kiếm

```c
current = list->head;

while (current != NULL)
{
    if (current->data == value)
    {
        return current;
    }

    current = current->next;
}
```

Danh sách được duyệt tuần tự từ `head`.

Trong trường hợp xấu nhất, phải kiểm tra toàn bộ `n` Node:

```text
O(n)
```

---

## 9.7. Xóa Node đầu

Danh sách ban đầu:

```text
head
  |
  v
 10 -> 20 -> 30 -> NULL
```

Lưu Node cần xóa:

```c
node_to_remove = list->head;
```

Di chuyển `head`:

```c
list->head = node_to_remove->next;
```

Giải phóng Node cũ:

```c
free(node_to_remove);
```

Kết quả:

```text
head
  |
  v
 20 -> 30 -> NULL
```

Nếu danh sách trở thành rỗng thì phải đặt:

```c
list->tail = NULL;
```

---

## 9.8. Xóa theo giá trị

Để xóa một Node ở giữa, cần giữ hai con trỏ:

```text
previous -> Node phía trước
current  -> Node đang kiểm tra
```

Ví dụ xóa `20`:

```text
10 -> 20 -> 30 -> NULL
^     ^
|     |
prev  current
```

Cho Node trước bỏ qua Node cần xóa:

```c
previous->next = current->next;
```

Kết quả:

```text
10 ------> 30 -> NULL
```

Sau đó:

```c
free(current);
```

---

## 9.9. Xóa toàn bộ danh sách

Sai:

```c
free(current);
current = current->next;
```

Sau `free(current)`, không được tiếp tục truy cập `current->next`.

Đúng:

```c
while (current != NULL)
{
    SllNode *next_node = current->next;

    free(current);
    current = next_node;
}
```

Phải lưu địa chỉ Node tiếp theo trước khi giải phóng Node hiện tại.

---

## 10. Độ phức tạp

Giả sử danh sách có `n` Node và có cả `head` lẫn `tail`.

| Thao tác | Độ phức tạp |
|---|---:|
| Kiểm tra rỗng | `O(1)` |
| Lấy kích thước khi có biến `size` | `O(1)` |
| Truy cập Node đầu | `O(1)` |
| Truy cập Node cuối qua `tail` | `O(1)` |
| Thêm đầu | `O(1)` |
| Thêm cuối qua `tail` | `O(1)` |
| Thêm cuối khi không có `tail` | `O(n)` |
| Tìm theo giá trị | `O(n)` |
| Truy cập Node thứ `i` | `O(n)` |
| Xóa đầu | `O(1)` |
| Xóa theo giá trị | `O(n)` |
| Xóa sau một Node đã biết | `O(1)` |
| Duyệt toàn bộ | `O(n)` |
| Xóa toàn bộ danh sách | `O(n)` |

### Độ phức tạp bộ nhớ

Mỗi Node cần:

```text
sizeof(data) + sizeof(next pointer)
```

Ví dụ trên MCU 32-bit:

```text
int      = 4 byte
pointer  = 4 byte
```

Một Node có thể cần ít nhất khoảng 8 byte, chưa tính alignment hoặc metadata của heap allocator.

---

## 11. Ứng dụng của Linked List

Linked List thường được dùng để xây dựng:

### Stack

Thêm và lấy phần tử tại đầu danh sách:

```text
push  -> push front
pop   -> pop front
```

Cả hai thao tác đều `O(1)`.

---

### Queue

```text
enqueue -> push back
dequeue -> pop front
```

Nếu có `tail`, cả hai thao tác đều `O(1)`.

---

### Deque

Doubly Linked List phù hợp hơn vì cần thêm và xóa ở cả hai đầu.

---

### Hash Table với Separate Chaining

Mỗi bucket có thể là một Linked List:

```text
bucket[0] -> Node -> Node -> NULL
bucket[1] -> NULL
bucket[2] -> Node -> NULL
```

Khi nhiều key có cùng index, chúng được lưu trong cùng một danh sách.

---

### Graph Adjacency List

Mỗi đỉnh của đồ thị có một danh sách các đỉnh kề:

```text
A -> B -> C
B -> A -> D
C -> A
D -> B
```

---

### Memory Allocator

Các vùng nhớ trống có thể được quản lý bằng danh sách:

```text
free block -> free block -> free block
```

Các hàm cấp phát bộ nhớ thường cần cấu trúc dữ liệu để theo dõi block đang rảnh và block đã sử dụng.

---

### RTOS

Linked List thường xuất hiện trong:

- Ready list.
- Delayed task list.
- Timer list.
- Event waiting list.
- Queue quản lý message hoặc task.
- Danh sách resource.

---

### LRU Cache

Có thể kết hợp:

```text
Hash Table + Doubly Linked List
```

Hash Table giúp tìm nhanh, Doubly Linked List giúp di chuyển hoặc xóa Node nhanh.

---

### Browser History

Doubly Linked List phù hợp vì người dùng cần di chuyển:

```text
Back <-> Current <-> Forward
```

---

## 12. Linked List trong lập trình nhúng

Linked List hữu ích trong embedded, nhưng cần cân nhắc kỹ cách quản lý bộ nhớ.

## 12.1. Hạn chế của `malloc()` và `free()`

Cấp phát động có thể gây:

- Phân mảnh heap.
- Thời gian thực thi khó dự đoán.
- Hết heap trong lúc hệ thống đang chạy.
- Lỗi double free.
- Memory leak.
- Khó chứng minh tính xác định trong hệ thống real-time.

Với firmware nhỏ hoặc yêu cầu độ tin cậy cao, nên cân nhắc:

- Cấp phát toàn bộ Node khi khởi động.
- Dùng Memory Pool.
- Dùng mảng Node cố định.
- Không gọi `malloc()` trong ISR.
- Không cấp phát động sau giai đoạn initialization.

---

## 12.2. Linked List sử dụng Memory Pool

Thay vì lấy từng Node từ heap, ta chuẩn bị trước một mảng:

```c
#define NODE_POOL_SIZE 16U

static SllNode node_pool[NODE_POOL_SIZE];
```

Các Node chưa dùng được đưa vào một free list:

```text
free_head
   |
   v
Node -> Node -> Node -> NULL
```

Khi cần Node:

```text
lấy Node đầu khỏi free list
```

Khi xóa Node:

```text
trả Node về free list
```

Ưu điểm:

- Không phân mảnh heap.
- Bộ nhớ tối đa được biết trước.
- Thời gian cấp phát dễ dự đoán.
- Phù hợp với firmware real-time hơn.

Nhược điểm:

- Số Node tối đa cố định.
- Phải xử lý trường hợp pool hết Node.

---

## 12.3. Không thao tác phức tạp trong ISR

Không nên:

```c
void UART_IRQHandler(void)
{
    Sll_PushBack(&list, value); /* May call malloc(). */
}
```

Lý do:

- `malloc()` có thể không reentrant.
- Thời gian cấp phát không xác định.
- ISR có thể kéo dài.
- Có thể xung đột với code đang thao tác heap.

Thay vào đó:

- Dùng Ring Buffer cho byte UART.
- Dùng queue tĩnh.
- Dùng memory pool có thao tác nguyên tử.
- Đẩy sự kiện tối giản rồi xử lý trong `main()` hoặc task.

---

## 12.4. Truy cập đồng thời

Nếu một task thêm Node và task khác xóa Node, danh sách có thể bị hỏng.

Có thể cần:

- Critical section.
- Mutex.
- Scheduler lock.
- Tắt interrupt trong đoạn rất ngắn.
- Single Producer – Single Consumer design.
- API riêng của RTOS.

`volatile` không làm Linked List trở thành thread-safe.

---

## 12.5. Quyền sở hữu dữ liệu

Cần quy định rõ:

- Ai tạo Node?
- Ai có quyền xóa Node?
- Khi xóa Node có phải giải phóng `data` không?
- `data` là dữ liệu copy hay chỉ là con trỏ?
- Một Node có được nằm trong hai danh sách cùng lúc không?

Ví dụ Node chứa con trỏ:

```c
typedef struct MessageNode
{
    uint8_t *payload;
    size_t length;
    struct MessageNode *next;
} MessageNode;
```

Khi xóa Node, cần biết `payload` được quản lý bởi:

- Node.
- Module khác.
- Buffer tĩnh.
- Memory pool riêng.

Không xác định rõ ownership dễ gây memory leak hoặc double free.

---

## 13. Các lỗi thường gặp

## 13.1. Không kiểm tra kết quả `malloc()`

Sai:

```c
SllNode *node = malloc(sizeof(SllNode));
node->data = value;
```

Đúng:

```c
SllNode *node = malloc(sizeof(SllNode));

if (node == NULL)
{
    return false;
}
```

---

## 13.2. Làm mất phần còn lại của danh sách

Sai khi chèn sau:

```c
position->next = new_node;
new_node->next = position->next;
```

Sau dòng đầu, liên kết cũ đã bị ghi mất.

Đúng:

```c
new_node->next = position->next;
position->next = new_node;
```

---

## 13.3. Use-after-free

Sai:

```c
free(current);
current = current->next;
```

Đúng:

```c
SllNode *next_node = current->next;
free(current);
current = next_node;
```

---

## 13.4. Không cập nhật `tail`

Khi xóa Node cuối:

```text
10 -> 20 -> 30 -> NULL
            ^
            |
           tail
```

Nếu `30` bị xóa, phải cập nhật:

```text
tail -> 20
```

Nếu không, `tail` trở thành dangling pointer.

---

## 13.5. Không xử lý danh sách rỗng

Trước khi đọc:

```c
list->head->data
```

phải chắc chắn:

```c
list != NULL
list->head != NULL
```

---

## 13.6. Memory leak

Mỗi Node được tạo bằng `malloc()` phải được giải phóng bằng `free()` khi không còn sử dụng.

```c
Sll_Clear(&list);
```

nên được gọi trước khi kết thúc hoặc khởi tạo lại danh sách.

---

## 13.7. Tạo vòng lặp ngoài ý muốn

Nếu một Node trỏ ngược đến Node trước:

```text
10 -> 20 -> 30
      ^     |
      |_____|
```

Vòng lặp duyệt:

```c
while (current != NULL)
```

sẽ không bao giờ kết thúc.

Có thể dùng thuật toán Floyd để phát hiện cycle:

```text
slow đi 1 bước
fast đi 2 bước
```

Nếu `slow == fast`, danh sách có cycle.

---

## 14. Bài tập luyện tập

## Cơ bản

1. Đếm số Node mà không sử dụng biến `size`.
2. In toàn bộ danh sách.
3. Tìm Node theo giá trị.
4. Thêm Node vào đầu.
5. Thêm Node vào cuối.
6. Xóa Node đầu.
7. Xóa Node cuối.
8. Xóa Node theo vị trí.
9. Xóa Node theo giá trị.
10. Lấy Node thứ `n` từ đầu.
11. Lấy Node thứ `n` từ cuối.

## Trung bình

1. Đảo ngược Singly Linked List.
2. Tìm Node giữa danh sách.
3. Phát hiện cycle.
4. Tính chiều dài cycle.
5. Xóa Node trùng trong danh sách đã sắp xếp.
6. Xóa Node trùng trong danh sách chưa sắp xếp.
7. Gộp hai danh sách đã sắp xếp.
8. Tách danh sách thành hai nửa.
9. Xoay danh sách sang trái hoặc phải.
10. Đổi chỗ từng cặp Node.
11. Phân tách Node chẵn và lẻ.

## Nâng cao

1. Tìm giao điểm của hai Linked List.
2. Đảo danh sách theo từng nhóm `k` Node.
3. Gộp `k` danh sách đã sắp xếp.
4. Cài đặt LRU Cache.
5. Clone danh sách có con trỏ ngẫu nhiên.
6. Sắp xếp Linked List bằng Merge Sort.
7. Chuyển Binary Tree thành Doubly Linked List.
8. Thiết kế Memory Pool dùng free list.
9. Cài đặt intrusive linked list cho embedded.
10. Xây dựng task list đơn giản cho scheduler.

---

## 15. Tổng kết

Các điểm quan trọng cần nhớ:

```text
Node = data + next
```

```text
head → Node đầu
tail → Node cuối
```

Node cuối có:

```c
next == NULL
```

Ưu điểm nổi bật:

- Thêm và xóa linh hoạt.
- Không cần vùng nhớ liên tiếp.
- Thêm đầu là `O(1)`.
- Thêm cuối là `O(1)` khi có `tail`.
- Có thể dùng để xây dựng nhiều cấu trúc dữ liệu khác.

Hạn chế:

- Không truy cập trực tiếp theo chỉ số.
- Tốn thêm bộ nhớ cho con trỏ.
- Cache locality không tốt bằng Array.
- Dễ gặp lỗi con trỏ.
- Cấp phát động có thể không phù hợp với hệ thống real-time.

Trong embedded, Linked List nên được sử dụng cùng với:

- Quy tắc ownership rõ ràng.
- Kiểm tra con trỏ đầy đủ.
- Memory pool nếu cần tính xác định.
- Cơ chế đồng bộ khi có nhiều ngữ cảnh truy cập.
- Chính sách giới hạn số Node.

---

## 16. Tài liệu tham khảo

README này được biên soạn và diễn đạt lại độc lập dựa trên chủ đề được trình bày trong bài:

- EPCB — **Data Structures - Danh sách liên kết đơn (Linked List)**:  
  https://epcb.vn/blogs/ak-embedded-software/data-structures-danh-sach-lien-ket-don-linked-list
- GeeksforGeeks — **Linked List Data Structure**:  
  https://www.geeksforgeeks.org/dsa/linked-list-data-structure/
