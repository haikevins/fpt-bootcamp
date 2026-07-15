# Singly Linked List trong C và lập trình nhúng

Linked List, hay **danh sách liên kết**, là cấu trúc dữ liệu tuyến tính gồm nhiều phần tử gọi là **node**. Các node không cần nằm liên tiếp trong bộ nhớ như mảng; mỗi node lưu dữ liệu và địa chỉ của node kế tiếp.

Tài liệu này trình bày kiến thức nền tảng, các thao tác phổ biến, mã nguồn C hoàn chỉnh và những lưu ý khi áp dụng Linked List trên vi điều khiển, bare-metal hoặc RTOS.

> Nội dung được tổng hợp và viết lại dựa trên hai tài liệu tham khảo ở cuối README. Đây không phải bản sao nguyên văn của các bài viết gốc.

---

## Mục lục

1. [Linked List là gì?](#1-linked-list-là-gì)
2. [Cấu tạo của node](#2-cấu-tạo-của-node)
3. [Head, tail và size](#3-head-tail-và-size)
4. [So sánh Linked List và Array](#4-so-sánh-linked-list-và-array)
5. [Các loại Linked List](#5-các-loại-linked-list)
6. [Độ phức tạp](#6-độ-phức-tạp)
7. [Mã nguồn hoàn chỉnh](#7-mã-nguồn-hoàn-chỉnh)
8. [Ví dụ sử dụng](#8-ví-dụ-sử-dụng)
9. [Giải thích các thao tác](#9-giải-thích-các-thao-tác)
10. [Các thuật toán quan trọng](#10-các-thuật-toán-quan-trọng)
11. [Ứng dụng trong embedded](#11-ứng-dụng-trong-embedded)
12. [Heap và Memory Pool](#12-heap-và-memory-pool)
13. [Interrupt và RTOS](#13-interrupt-và-rtos)
14. [Các lỗi thường gặp](#14-các-lỗi-thường-gặp)
15. [Kiểm thử](#15-kiểm-thử)
16. [Bài tập luyện tập](#16-bài-tập-luyện-tập)
17. [Cấu trúc repository](#17-cấu-trúc-repository)
18. [Tóm tắt](#18-tóm-tắt)

---

## 1. Linked List là gì?

Linked List là danh sách trong đó các phần tử được liên kết với nhau bằng con trỏ.

```text
head
 |
 v
+------+-------+    +------+-------+    +------+-------+
| data | next  | -> | data | next  | -> | data | NULL  |
+------+-------+    +------+-------+    +------+-------+
```

Mỗi phần tử trong danh sách được gọi là một **node**. Node đầu tiên được truy cập thông qua `head`. Từ `head`, chương trình đi theo các con trỏ `next` để duyệt toàn bộ danh sách.

Các node không bắt buộc nằm cạnh nhau trong RAM:

```text
0x20000020: Node A
0x20000134: Node B
0x20000088: Node C
```

Chúng vẫn tạo thành một danh sách hợp lệ nếu:

```text
Node A.next = &Node B
Node B.next = &Node C
Node C.next = NULL
```

Linked List hữu ích khi số phần tử thay đổi trong lúc chương trình chạy và ứng dụng thường xuyên thêm hoặc xóa phần tử.

---

## 2. Cấu tạo của node

Ví dụ node lưu một số nguyên:

```c
typedef struct Node
{
    int data;
    struct Node *next;
} Node;
```

### `data`

Lưu dữ liệu của node:

```c
int data;
```

Kiểu dữ liệu có thể thay đổi theo ứng dụng:

```c
uint8_t data;
uint16_t data;
float data;
sensor_data_t data;
event_t data;
```

Ví dụ node lưu sự kiện:

```c
typedef struct
{
    uint16_t signal;
    uint32_t parameter;
} event_t;

typedef struct EventNode
{
    event_t data;
    struct EventNode *next;
} EventNode;
```

### `next`

Lưu địa chỉ node tiếp theo:

```c
struct Node *next;
```

Node cuối danh sách phải có:

```c
node->next = NULL;
```

---

## 3. Head, tail và size

Một Singly Linked List có thể được quản lý chỉ bằng `head`, nhưng thêm `tail` và `size` giúp nhiều thao tác hiệu quả và rõ ràng hơn.

```c
typedef struct
{
    Node *head;
    Node *tail;
    size_t size;
} LinkedList;
```

### `head`

Trỏ đến node đầu tiên.

```text
head == NULL
```

nghĩa là danh sách rỗng.

### `tail`

Trỏ đến node cuối cùng. Khi danh sách không rỗng:

```c
list->tail->next == NULL;
```

Nhờ `tail`, thao tác thêm cuối có thể thực hiện trong `O(1)`.

### `size`

Lưu số node hiện có, giúp lấy độ dài danh sách trong `O(1)`.

Danh sách rỗng hợp lệ phải thỏa mãn:

```c
list->head == NULL;
list->tail == NULL;
list->size == 0U;
```

---

## 4. So sánh Linked List và Array

| Tiêu chí | Array | Linked List |
|---|---|---|
| Vị trí bộ nhớ | Liên tiếp | Không cần liên tiếp |
| Truy cập phần tử thứ `i` | `O(1)` | `O(n)` |
| Thêm đầu | Thường `O(n)` | `O(1)` |
| Xóa đầu | Thường `O(n)` | `O(1)` |
| Thêm cuối | `O(1)` nếu còn chỗ | `O(1)` nếu có `tail` |
| Xóa cuối | Tùy cách dùng | `O(n)` với singly list |
| Bộ nhớ phụ | Không có con trỏ/node | Có ít nhất một con trỏ/node |
| Cache locality | Tốt | Thường kém hơn |
| Kích thước | Thường cố định | Có thể thay đổi động |
| Dùng với DMA | Thuận lợi | Không thuận lợi |
| Nguy cơ phân mảnh | Không với mảng tĩnh | Có thể xảy ra khi dùng heap |

### Nên dùng Array khi

- Cần truy cập nhanh theo index.
- Số phần tử đã biết trước.
- Cần vùng nhớ liên tục.
- Dùng DMA.
- Muốn cache locality tốt.
- Muốn tránh cấp phát động.

### Nên dùng Linked List khi

- Thường xuyên thêm hoặc xóa node.
- Không biết trước số phần tử.
- Không cần truy cập ngẫu nhiên theo index.
- Cần nối hoặc tách danh sách mà không copy payload.
- Các object đã tồn tại độc lập trong bộ nhớ.

> Linked List không mặc định nhanh hơn Array. Nó chỉ tốt hơn khi mẫu truy cập và yêu cầu bộ nhớ phù hợp.

---

## 5. Các loại Linked List

### 5.1. Singly Linked List

Mỗi node chỉ trỏ đến node tiếp theo:

```text
A -> B -> C -> NULL
```

Chỉ duyệt được từ đầu đến cuối.

### 5.2. Doubly Linked List

Mỗi node có cả `next` và `prev`:

```text
NULL <- A <-> B <-> C -> NULL
```

```c
typedef struct Node
{
    int data;
    struct Node *prev;
    struct Node *next;
} Node;
```

Ưu điểm:

- Duyệt hai chiều.
- Xóa node đã biết trong `O(1)`.
- Xóa cuối trong `O(1)` nếu có `tail`.

Nhược điểm:

- Tốn thêm bộ nhớ cho `prev`.
- Phải cập nhật nhiều liên kết hơn.

### 5.3. Circular Singly Linked List

Node cuối trỏ lại node đầu:

```text
A -> B -> C
^         |
|_________|
```

```c
tail->next = head;
```

Ứng dụng:

- Round-robin scheduler.
- Danh sách task tuần hoàn.
- Playlist lặp.

### 5.4. Circular Doubly Linked List

Node đầu và cuối liên kết hai chiều với nhau. Cấu trúc này thường được dùng trong kernel, scheduler và intrusive list.

---

## 6. Độ phức tạp

Giả sử danh sách có `head`, `tail` và `size`:

| Thao tác | Độ phức tạp |
|---|---|
| Kiểm tra rỗng | `O(1)` |
| Lấy số node | `O(1)` |
| Truy cập theo index | `O(n)` |
| Tìm theo giá trị | `O(n)` |
| Thêm đầu | `O(1)` |
| Thêm cuối | `O(1)` |
| Thêm sau node đã biết | `O(1)` |
| Thêm tại index | `O(n)` |
| Xóa đầu | `O(1)` |
| Xóa cuối | `O(n)` |
| Xóa theo giá trị | `O(n)` |
| Đảo ngược | `O(n)` |
| Xóa toàn bộ | `O(n)` |

Xóa cuối Singly Linked List là `O(n)` vì `tail` không biết node đứng trước nó. Chương trình phải duyệt từ `head` để tìm `previous` sao cho:

```c
previous->next == list->tail;
```

---

## 7. Mã nguồn hoàn chỉnh

### `linked_list.h`

```c
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct Node
{
    int data;
    struct Node *next;
} Node;

typedef struct
{
    Node *head;
    Node *tail;
    size_t size;
} LinkedList;

typedef enum
{
    LIST_OK = 0,
    LIST_EMPTY,
    LIST_NOT_FOUND,
    LIST_OUT_OF_RANGE,
    LIST_ALLOCATION_FAILED,
    LIST_INVALID_ARGUMENT
} list_status_t;

void list_init(LinkedList *list);
bool list_is_empty(const LinkedList *list);
size_t list_size(const LinkedList *list);

list_status_t list_push_front(LinkedList *list, int value);
list_status_t list_push_back(LinkedList *list, int value);
list_status_t list_insert_at(LinkedList *list, size_t index, int value);

list_status_t list_pop_front(LinkedList *list, int *value);
list_status_t list_pop_back(LinkedList *list, int *value);
list_status_t list_remove_at(LinkedList *list, size_t index, int *value);
list_status_t list_remove_value(LinkedList *list, int value);

list_status_t list_front(const LinkedList *list, int *value);
list_status_t list_back(const LinkedList *list, int *value);
list_status_t list_get(const LinkedList *list, size_t index, int *value);

Node *list_find(const LinkedList *list, int value);
Node *list_middle(const LinkedList *list);

void list_reverse(LinkedList *list);
bool list_has_cycle(const LinkedList *list);
void list_print(const LinkedList *list);
void list_clear(LinkedList *list);

#endif
```

### `linked_list.c`

```c
#include "linked_list.h"

#include <stdio.h>
#include <stdlib.h>

static Node *node_create(int value)
{
    Node *node = malloc(sizeof(*node));

    if (node == NULL)
    {
        return NULL;
    }

    node->data = value;
    node->next = NULL;

    return node;
}

void list_init(LinkedList *list)
{
    if (list == NULL)
    {
        return;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0U;
}

bool list_is_empty(const LinkedList *list)
{
    return (list == NULL) || (list->head == NULL);
}

size_t list_size(const LinkedList *list)
{
    return (list != NULL) ? list->size : 0U;
}

list_status_t list_push_front(LinkedList *list, int value)
{
    Node *new_node;

    if (list == NULL)
    {
        return LIST_INVALID_ARGUMENT;
    }

    new_node = node_create(value);

    if (new_node == NULL)
    {
        return LIST_ALLOCATION_FAILED;
    }

    new_node->next = list->head;
    list->head = new_node;

    if (list->tail == NULL)
    {
        list->tail = new_node;
    }

    list->size++;
    return LIST_OK;
}

list_status_t list_push_back(LinkedList *list, int value)
{
    Node *new_node;

    if (list == NULL)
    {
        return LIST_INVALID_ARGUMENT;
    }

    new_node = node_create(value);

    if (new_node == NULL)
    {
        return LIST_ALLOCATION_FAILED;
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
    return LIST_OK;
}

list_status_t list_insert_at(LinkedList *list, size_t index, int value)
{
    Node *previous;
    Node *new_node;
    size_t position;

    if (list == NULL)
    {
        return LIST_INVALID_ARGUMENT;
    }

    if (index > list->size)
    {
        return LIST_OUT_OF_RANGE;
    }

    if (index == 0U)
    {
        return list_push_front(list, value);
    }

    if (index == list->size)
    {
        return list_push_back(list, value);
    }

    previous = list->head;

    for (position = 0U; position < (index - 1U); position++)
    {
        previous = previous->next;
    }

    new_node = node_create(value);

    if (new_node == NULL)
    {
        return LIST_ALLOCATION_FAILED;
    }

    new_node->next = previous->next;
    previous->next = new_node;
    list->size++;

    return LIST_OK;
}

list_status_t list_pop_front(LinkedList *list, int *value)
{
    Node *old_head;

    if ((list == NULL) || (value == NULL))
    {
        return LIST_INVALID_ARGUMENT;
    }

    if (list->head == NULL)
    {
        return LIST_EMPTY;
    }

    old_head = list->head;
    *value = old_head->data;
    list->head = old_head->next;
    list->size--;

    if (list->head == NULL)
    {
        list->tail = NULL;
    }

    free(old_head);
    return LIST_OK;
}

list_status_t list_pop_back(LinkedList *list, int *value)
{
    Node *previous;

    if ((list == NULL) || (value == NULL))
    {
        return LIST_INVALID_ARGUMENT;
    }

    if (list->tail == NULL)
    {
        return LIST_EMPTY;
    }

    if (list->head == list->tail)
    {
        *value = list->head->data;
        free(list->head);
        list->head = NULL;
        list->tail = NULL;
        list->size = 0U;
        return LIST_OK;
    }

    previous = list->head;

    while (previous->next != list->tail)
    {
        previous = previous->next;
    }

    *value = list->tail->data;
    free(list->tail);
    list->tail = previous;
    list->tail->next = NULL;
    list->size--;

    return LIST_OK;
}

list_status_t list_remove_at(LinkedList *list, size_t index, int *value)
{
    Node *previous;
    Node *target;
    size_t position;

    if ((list == NULL) || (value == NULL))
    {
        return LIST_INVALID_ARGUMENT;
    }

    if (index >= list->size)
    {
        return LIST_OUT_OF_RANGE;
    }

    if (index == 0U)
    {
        return list_pop_front(list, value);
    }

    if (index == (list->size - 1U))
    {
        return list_pop_back(list, value);
    }

    previous = list->head;

    for (position = 0U; position < (index - 1U); position++)
    {
        previous = previous->next;
    }

    target = previous->next;
    *value = target->data;
    previous->next = target->next;
    free(target);
    list->size--;

    return LIST_OK;
}

list_status_t list_remove_value(LinkedList *list, int value)
{
    Node *previous;
    Node *current;

    if (list == NULL)
    {
        return LIST_INVALID_ARGUMENT;
    }

    if (list->head == NULL)
    {
        return LIST_EMPTY;
    }

    if (list->head->data == value)
    {
        int removed_value;
        return list_pop_front(list, &removed_value);
    }

    previous = list->head;
    current = list->head->next;

    while (current != NULL)
    {
        if (current->data == value)
        {
            previous->next = current->next;

            if (current == list->tail)
            {
                list->tail = previous;
            }

            free(current);
            list->size--;
            return LIST_OK;
        }

        previous = current;
        current = current->next;
    }

    return LIST_NOT_FOUND;
}

list_status_t list_front(const LinkedList *list, int *value)
{
    if ((list == NULL) || (value == NULL))
    {
        return LIST_INVALID_ARGUMENT;
    }

    if (list->head == NULL)
    {
        return LIST_EMPTY;
    }

    *value = list->head->data;
    return LIST_OK;
}

list_status_t list_back(const LinkedList *list, int *value)
{
    if ((list == NULL) || (value == NULL))
    {
        return LIST_INVALID_ARGUMENT;
    }

    if (list->tail == NULL)
    {
        return LIST_EMPTY;
    }

    *value = list->tail->data;
    return LIST_OK;
}

list_status_t list_get(const LinkedList *list, size_t index, int *value)
{
    const Node *current;
    size_t position;

    if ((list == NULL) || (value == NULL))
    {
        return LIST_INVALID_ARGUMENT;
    }

    if (index >= list->size)
    {
        return LIST_OUT_OF_RANGE;
    }

    current = list->head;

    for (position = 0U; position < index; position++)
    {
        current = current->next;
    }

    *value = current->data;
    return LIST_OK;
}

Node *list_find(const LinkedList *list, int value)
{
    Node *current;

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

Node *list_middle(const LinkedList *list)
{
    Node *slow;
    Node *fast;

    if ((list == NULL) || (list->head == NULL))
    {
        return NULL;
    }

    slow = list->head;
    fast = list->head;

    while ((fast != NULL) && (fast->next != NULL))
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}

void list_reverse(LinkedList *list)
{
    Node *previous;
    Node *current;
    Node *next_node;

    if ((list == NULL) || (list->head == NULL))
    {
        return;
    }

    list->tail = list->head;
    previous = NULL;
    current = list->head;

    while (current != NULL)
    {
        next_node = current->next;
        current->next = previous;
        previous = current;
        current = next_node;
    }

    list->head = previous;
}

bool list_has_cycle(const LinkedList *list)
{
    const Node *slow;
    const Node *fast;

    if (list == NULL)
    {
        return false;
    }

    slow = list->head;
    fast = list->head;

    while ((fast != NULL) && (fast->next != NULL))
    {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast)
        {
            return true;
        }
    }

    return false;
}

void list_print(const LinkedList *list)
{
    const Node *current;

    if (list == NULL)
    {
        return;
    }

    current = list->head;

    while (current != NULL)
    {
        printf("%d", current->data);

        if (current->next != NULL)
        {
            printf(" -> ");
        }

        current = current->next;
    }

    printf(" -> NULL\n");
}

void list_clear(LinkedList *list)
{
    Node *current;

    if (list == NULL)
    {
        return;
    }

    current = list->head;

    while (current != NULL)
    {
        Node *next_node = current->next;
        free(current);
        current = next_node;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0U;
}
```

---

## 8. Ví dụ sử dụng

### `main.c`

```c
#include <stdio.h>

#include "linked_list.h"

int main(void)
{
    LinkedList list;
    int removed_value;

    list_init(&list);

    (void)list_push_back(&list, 10);
    (void)list_push_back(&list, 20);
    (void)list_push_front(&list, 5);
    (void)list_insert_at(&list, 2U, 15);

    printf("List: ");
    list_print(&list);
    printf("Size: %zu\n", list_size(&list));

    if (list_remove_value(&list, 15) == LIST_OK)
    {
        printf("Removed value 15\n");
    }

    if (list_pop_front(&list, &removed_value) == LIST_OK)
    {
        printf("Pop front: %d\n", removed_value);
    }

    printf("Before reverse: ");
    list_print(&list);

    list_reverse(&list);

    printf("After reverse: ");
    list_print(&list);

    list_clear(&list);
    return 0;
}
```

Kết quả:

```text
List: 5 -> 10 -> 15 -> 20 -> NULL
Size: 4
Removed value 15
Pop front: 5
Before reverse: 10 -> 20 -> NULL
After reverse: 20 -> 10 -> NULL
```

Biên dịch:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic \
    src/linked_list.c examples/main.c \
    -Iinc -o linked_list_demo
```

---

## 9. Giải thích các thao tác

### 9.1. Thêm vào đầu

Trước:

```text
head
 |
 v
A -> B -> C -> NULL
```

Tạo node `X`, sau đó:

```c
new_node->next = list->head;
list->head = new_node;
```

Sau:

```text
head
 |
 v
X -> A -> B -> C -> NULL
```

Nếu danh sách ban đầu rỗng, `tail` cũng phải trỏ đến node mới.

Độ phức tạp: `O(1)`.

### 9.2. Thêm vào cuối

Khi có `tail`:

```c
list->tail->next = new_node;
list->tail = new_node;
```

Độ phức tạp: `O(1)`.

Nếu không lưu `tail`, phải duyệt từ `head` đến node cuối, mất `O(n)`.

### 9.3. Chèn tại một vị trí

Để chèn `X` giữa `B` và `C`:

```text
A -> B -> C -> D
```

phải gán theo đúng thứ tự:

```c
new_node->next = previous->next;
previous->next = new_node;
```

Kết quả:

```text
A -> B -> X -> C -> D
```

Nếu đảo thứ tự hai lệnh trên, chương trình có thể làm mất liên kết đến phần còn lại hoặc tạo self-loop.

### 9.4. Xóa đầu

```c
old_head = list->head;
list->head = old_head->next;
free(old_head);
```

Nếu node bị xóa là node duy nhất, phải đặt cả `head` và `tail` về `NULL`.

### 9.5. Xóa node ở giữa

Muốn xóa `C`:

```text
A -> B -> C -> D -> NULL
```

Thực hiện:

```c
previous->next = target->next;
free(target);
```

Kết quả:

```text
A -> B -> D -> NULL
```

### 9.6. Tìm kiếm

Duyệt từ `head` đến khi tìm thấy giá trị hoặc gặp `NULL`:

```c
Node *current = list->head;

while (current != NULL)
{
    if (current->data == value)
    {
        return current;
    }

    current = current->next;
}
```

Trường hợp xấu nhất: `O(n)`.

Linked List không phù hợp với binary search vì không thể truy cập trực tiếp node giữa trong `O(1)`.

---

## 10. Các thuật toán quan trọng

### 10.1. Đảo ngược danh sách

Ban đầu:

```text
A -> B -> C -> NULL
```

Kết quả:

```text
C -> B -> A -> NULL
```

Dùng ba con trỏ:

```c
Node *previous = NULL;
Node *current = list->head;
Node *next_node;
```

Trong mỗi vòng lặp:

```c
next_node = current->next;
current->next = previous;
previous = current;
current = next_node;
```

Độ phức tạp:

```text
Time:  O(n)
Space: O(1)
```

### 10.2. Tìm node giữa

Dùng hai con trỏ:

- `slow`: mỗi vòng đi một node.
- `fast`: mỗi vòng đi hai node.

```c
while ((fast != NULL) && (fast->next != NULL))
{
    slow = slow->next;
    fast = fast->next->next;
}
```

Khi `fast` đến cuối, `slow` nằm ở giữa.

Với số node chẵn, cách viết trên trả về node giữa thứ hai.

### 10.3. Phát hiện chu trình

Danh sách có chu trình:

```text
A -> B -> C -> D
     ^         |
     |_________|
```

Thuật toán Floyd dùng `slow` và `fast`. Nếu hai con trỏ gặp nhau thì danh sách có chu trình.

```c
slow = slow->next;
fast = fast->next->next;

if (slow == fast)
{
    return true;
}
```

Phát hiện chu trình quan trọng vì các hàm duyệt đến `NULL` như `list_print()` hoặc `list_clear()` sẽ chạy vô hạn nếu list bị nối sai.

### 10.4. Tìm node thứ `n` từ cuối

Đưa con trỏ `fast` đi trước `n` bước, sau đó cho `slow` và `fast` cùng tiến. Khi `fast` đến cuối, `slow` nằm tại node thứ `n` từ cuối.

Độ phức tạp:

```text
Time:  O(n)
Space: O(1)
```

### 10.5. Gộp hai danh sách tăng dần

Dùng một dummy node để đơn giản hóa xử lý đầu danh sách:

```c
Node dummy = {0, NULL};
Node *tail = &dummy;

while ((a != NULL) && (b != NULL))
{
    if (a->data <= b->data)
    {
        tail->next = a;
        a = a->next;
    }
    else
    {
        tail->next = b;
        b = b->next;
    }

    tail = tail->next;
}

tail->next = (a != NULL) ? a : b;
return dummy.next;
```

Không cần tạo node mới nếu tái sử dụng node của hai danh sách đầu vào.

---

## 11. Ứng dụng trong embedded

Linked List thường xuất hiện trong firmware, framework event-driven và RTOS.

### 11.1. Event Queue

Mỗi node lưu một sự kiện:

```c
typedef struct
{
    uint16_t signal;
    uint32_t parameter;
} event_t;
```

```text
EVENT_UART_RX -> EVENT_TIMEOUT -> EVENT_BUTTON -> NULL
```

### 11.2. Software Timer

Node timer có thể lưu:

- Tick hết hạn.
- Callback.
- Context.
- Liên kết đến timer kế tiếp.

```c
typedef void (*timer_callback_t)(void *context);

typedef struct TimerNode
{
    uint32_t expiry_tick;
    timer_callback_t callback;
    void *context;
    struct TimerNode *next;
} TimerNode;
```

Danh sách có thể được sắp xếp theo thời điểm hết hạn.

### 11.3. Memory Allocator

Allocator có thể quản lý các block trống bằng một free list:

```text
free block -> free block -> free block -> NULL
```

`malloc()` lấy một block phù hợp khỏi danh sách; `free()` trả block về free list.

### 11.4. Scheduler và RTOS

Linked List có thể quản lý:

- Ready task list.
- Delayed task list.
- Blocked task list.
- Timer list.
- Event waiting list.
- Danh sách tài nguyên.

### 11.5. Driver registry

```text
UART1 -> SPI1 -> I2C1 -> SENSOR1 -> NULL
```

Mỗi driver hoặc thiết bị được đăng ký thành một node.

### 11.6. Buffer descriptor

Thay vì copy payload lớn, list có thể nối các descriptor:

```c
typedef struct BufferNode
{
    uint8_t *data;
    size_t length;
    struct BufferNode *next;
} BufferNode;
```

Ứng dụng:

- Packet chain.
- Scatter-gather I/O.
- Danh sách frame chờ truyền.
- Audio block queue.

### 11.7. Intrusive List

Trong intrusive list, liên kết được đặt trực tiếp trong object:

```c
typedef struct Task
{
    uint8_t priority;
    uint32_t timeout;
    struct Task *next;
} Task;
```

Ưu điểm:

- Không cần cấp một wrapper node riêng.
- Tránh copy payload.
- Phù hợp object có vòng đời do ứng dụng quản lý.

Nhược điểm:

- Object và list bị phụ thuộc cấu trúc.
- Một object muốn nằm trong nhiều list phải có nhiều trường liên kết.

---

## 12. Heap và Memory Pool

### 12.1. Cấp phát bằng Heap

```c
Node *node = malloc(sizeof(*node));
```

Ưu điểm:

- Dễ triển khai.
- Số node thay đổi linh hoạt.

Nhược điểm trong embedded:

- Có thể phân mảnh heap.
- Thời gian cấp phát không luôn cố định.
- Có thể thất bại sau thời gian chạy dài.
- Khó phân tích worst-case execution time.
- Có nguy cơ memory leak và double free.
- Không phù hợp với nhiều ISR.

### 12.2. Memory Pool tĩnh

Tạo sẵn số lượng node cố định:

```c
#define NODE_POOL_SIZE 16U

static Node node_pool[NODE_POOL_SIZE];
```

Các node rảnh được nối thành free list.

```c
typedef struct
{
    Node nodes[NODE_POOL_SIZE];
    Node *free_head;
} NodePool;
```

Khởi tạo:

```c
void node_pool_init(NodePool *pool)
{
    size_t i;

    if (pool == NULL)
    {
        return;
    }

    for (i = 0U; i < (NODE_POOL_SIZE - 1U); i++)
    {
        pool->nodes[i].next = &pool->nodes[i + 1U];
    }

    pool->nodes[NODE_POOL_SIZE - 1U].next = NULL;
    pool->free_head = &pool->nodes[0];
}
```

Cấp node:

```c
Node *node_pool_allocate(NodePool *pool)
{
    Node *node;

    if ((pool == NULL) || (pool->free_head == NULL))
    {
        return NULL;
    }

    node = pool->free_head;
    pool->free_head = node->next;
    node->next = NULL;

    return node;
}
```

Trả node:

```c
void node_pool_free(NodePool *pool, Node *node)
{
    if ((pool == NULL) || (node == NULL))
    {
        return;
    }

    node->next = pool->free_head;
    pool->free_head = node;
}
```

Ưu điểm:

- Không phân mảnh heap.
- Allocate/free `O(1)`.
- Biết trước bộ nhớ tối đa.
- Thời gian thực thi dễ dự đoán hơn.

Nhược điểm:

- Số node tối đa cố định.
- Phải xử lý trường hợp hết node.
- Các node trong pool thường có cùng kích thước.

| Trường hợp | Lựa chọn phù hợp |
|---|---|
| Học DSA trên PC | `malloc()` / `free()` |
| Firmware nhỏ | Memory Pool tĩnh |
| Hard real-time | Ưu tiên pool tĩnh |
| Số phần tử tối đa biết trước | Mảng hoặc pool |
| Payload liên tục cho DMA | Mảng/Ring Buffer |

---

## 13. Interrupt và RTOS

### Linked List không tự thread-safe

Một thao tác thêm hoặc xóa có thể thay đổi nhiều thành phần:

```text
head
tail
previous->next
size
```

Nếu context khác chen vào giữa thao tác, danh sách có thể bị hỏng.

### Không nên gọi `malloc()` trong ISR

Vì:

- Allocator có thể không reentrant.
- Thời gian thực thi khó dự đoán.
- Có thể cần lock.
- ISR phải ngắn.

### Critical Section

```c
ENTER_CRITICAL_SECTION();
status = list_push_back(&event_list, event);
EXIT_CRITICAL_SECTION();
```

Chỉ bảo vệ phần cập nhật liên kết. Không thực hiện `printf()`, delay, truyền blocking hoặc callback dài trong critical section.

### Khi dùng FreeRTOS

Trước khi tự viết list cho việc truyền dữ liệu giữa task, nên cân nhắc:

- Queue.
- Stream Buffer.
- Message Buffer.
- Semaphore.
- Mutex.
- Task Notification.

Linked List tự viết phù hợp khi cần intrusive list, danh sách timer, registry hoặc cấu trúc object đặc thù.

### `volatile` không thay thế đồng bộ

`volatile` không cung cấp:

- Atomicity.
- Mutex.
- Critical section.
- Bảo vệ khỏi race condition.
- Quản lý vòng đời node.

---

## 14. Các lỗi thường gặp

### 14.1. Mất phần còn lại của danh sách

Sai:

```c
current->next = new_node;
new_node->next = current->next;
```

Sau lệnh đầu, `current->next` đã là `new_node`, nên lệnh tiếp theo làm `new_node` trỏ đến chính nó.

Đúng:

```c
new_node->next = current->next;
current->next = new_node;
```

### 14.2. Quên xử lý danh sách rỗng

Sai:

```c
list->tail->next = new_node;
```

Nếu `tail == NULL`, chương trình dereference con trỏ `NULL`.

### 14.3. Quên cập nhật `tail`

Khi xóa node cuối:

```c
list->tail = previous;
list->tail->next = NULL;
```

Khi xóa node duy nhất:

```c
list->head = NULL;
list->tail = NULL;
```

### 14.4. Use-after-free

Sai:

```c
free(node);
node = node->next;
```

Đúng:

```c
Node *next_node = node->next;
free(node);
node = next_node;
```

### 14.5. Memory leak

Mọi node tạo bằng `malloc()` phải được giải phóng. Trước khi kết thúc hoặc tái khởi tạo list, gọi:

```c
list_clear(&list);
```

### 14.6. Double free

Một node chỉ được giải phóng đúng một lần. Không được giữ con trỏ cũ và `free()` lại sau khi node đã bị xóa khỏi danh sách.

### 14.7. Trả về địa chỉ biến cục bộ

Sai:

```c
Node *create_node(int value)
{
    Node node;
    return &node;
}
```

`node` nằm trên stack và hết vòng đời khi hàm kết thúc.

### 14.8. Không kiểm tra `malloc()`

```c
Node *node = malloc(sizeof(*node));

if (node == NULL)
{
    return LIST_ALLOCATION_FAILED;
}
```

### 14.9. Sai điều kiện duyệt

Duyệt toàn bộ node:

```c
while (current != NULL)
```

Nếu dùng `while (current->next != NULL)`, node cuối sẽ không được xử lý.

### 14.10. Vô tình tạo chu trình

Gán sai `next` có thể làm list không còn kết thúc bằng `NULL`. Các vòng lặp duyệt sẽ chạy vô hạn.

### 14.11. Duyệt theo index tạo `O(n²)`

Không nên:

```c
for (size_t i = 0U; i < list_size(&list); i++)
{
    list_get(&list, i, &value);
}
```

Mỗi `list_get()` lại duyệt từ đầu. Hãy duyệt bằng con trỏ để đạt `O(n)`.

### 14.12. Metadata lớn hơn payload

Trên MCU 32-bit, node chứa `uint8_t` và một con trỏ có thể chiếm 8 byte do alignment:

```c
typedef struct Node
{
    uint8_t data;
    struct Node *next;
} Node;
```

Trong trường hợp này, Linked List có overhead lớn. Mảng hoặc Ring Buffer có thể phù hợp hơn.

---

## 15. Kiểm thử

### `test_linked_list.c`

```c
#include <assert.h>

#include "linked_list.h"

static void test_empty_list(void)
{
    LinkedList list;
    int value;

    list_init(&list);

    assert(list_is_empty(&list));
    assert(list_size(&list) == 0U);
    assert(list_pop_front(&list, &value) == LIST_EMPTY);
}

static void test_push_and_get(void)
{
    LinkedList list;
    int value;

    list_init(&list);

    assert(list_push_back(&list, 10) == LIST_OK);
    assert(list_push_back(&list, 20) == LIST_OK);
    assert(list_push_front(&list, 5) == LIST_OK);
    assert(list_size(&list) == 3U);

    assert(list_get(&list, 0U, &value) == LIST_OK);
    assert(value == 5);

    assert(list_get(&list, 2U, &value) == LIST_OK);
    assert(value == 20);

    list_clear(&list);
}

static void test_reverse(void)
{
    LinkedList list;
    int value;

    list_init(&list);

    assert(list_push_back(&list, 1) == LIST_OK);
    assert(list_push_back(&list, 2) == LIST_OK);
    assert(list_push_back(&list, 3) == LIST_OK);

    list_reverse(&list);

    assert(list_get(&list, 0U, &value) == LIST_OK);
    assert(value == 3);

    assert(list_get(&list, 2U, &value) == LIST_OK);
    assert(value == 1);

    list_clear(&list);
}

int main(void)
{
    test_empty_list();
    test_push_and_get();
    test_reverse();
    return 0;
}
```

Biên dịch với sanitizer:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic \
    -fsanitize=address,undefined \
    src/linked_list.c tests/test_linked_list.c \
    -Iinc -o linked_list_tests
```

Các trường hợp nên kiểm thử:

1. Danh sách rỗng.
2. Danh sách một node.
3. Thêm đầu và thêm cuối nhiều lần.
4. Chèn tại index `0`, giữa và `size`.
5. Index vượt phạm vi.
6. Xóa đầu, cuối và node duy nhất.
7. Xóa giá trị không tồn tại.
8. Đảo ngược list rỗng, một node và nhiều node.
9. Tìm node giữa với số node chẵn và lẻ.
10. Xóa toàn bộ và kiểm tra memory leak.
11. Giả lập cấp phát thất bại.
12. Kiểm tra `tail->next == NULL` sau mỗi thao tác.

---

## 16. Bài tập luyện tập

### Cơ bản

1. Đếm số node mà không dùng `size`.
2. Tính tổng giá trị.
3. Tìm giá trị lớn nhất và nhỏ nhất.
4. Đếm số lần một giá trị xuất hiện.
5. Chèn node vào danh sách tăng dần.
6. Xóa tất cả node có một giá trị cho trước.
7. Lấy node thứ `n` từ đầu.
8. In danh sách ngược bằng đệ quy.

### Trung bình

1. Đảo ngược danh sách.
2. Tìm node giữa.
3. Tìm node thứ `n` từ cuối.
4. Xóa node thứ `n` từ cuối.
5. Loại bỏ node trùng.
6. Gộp hai danh sách tăng dần.
7. Tách danh sách thành hai nửa.
8. Kiểm tra palindrome.
9. Xoay danh sách `k` vị trí.
10. Đổi từng cặp node.

### Nâng cao

1. Phát hiện chu trình.
2. Tìm node bắt đầu chu trình.
3. Tìm độ dài chu trình.
4. Tìm giao điểm hai Linked List.
5. Merge Sort cho Linked List.
6. Gộp `k` danh sách tăng dần.
7. Clone list có random pointer.
8. Cài đặt LRU Cache.
9. Reverse theo nhóm `k` node.
10. Thiết kế intrusive list cho scheduler.

---

## 17. Cấu trúc repository

```text
singly-linked-list/
├── README.md
├── inc/
│   └── linked_list.h
├── src/
│   └── linked_list.c
├── examples/
│   └── main.c
├── tests/
│   └── test_linked_list.c
└── memory-pool/
    ├── node_pool.h
    └── node_pool.c
```

---

## 18. Tóm tắt

- Linked List gồm các node liên kết bằng con trỏ.
- Singly Linked List chỉ duyệt từ đầu đến cuối.
- `head` trỏ đến node đầu tiên.
- `tail` trỏ đến node cuối cùng.
- Node cuối phải có `next == NULL`.
- Thêm hoặc xóa đầu có độ phức tạp `O(1)`.
- Thêm cuối là `O(1)` nếu có `tail`.
- Xóa cuối Singly Linked List là `O(n)`.
- Truy cập theo index và tìm kiếm là `O(n)`.
- Phải cập nhật đồng bộ `head`, `tail` và `size`.
- Phải kiểm tra kết quả `malloc()`.
- Mọi node cấp phát động phải được `free()` đúng một lần.
- Linked List không tự thread-safe.
- Trong embedded, Memory Pool thường dễ dự đoán hơn heap.
- Không nên dùng Linked List khi mảng hoặc Ring Buffer phù hợp hơn.

---

## Tài liệu tham khảo

README được tổng hợp và viết lại dựa trên các chủ đề từ:

- **Data Structures - Danh sách liên kết đơn (Linked List)**, AK Embedded Software, EPCB:  
  https://epcb.vn/blogs/ak-embedded-software/data-structures-danh-sach-lien-ket-don-linked-list

- **Linked List Data Structure**, GeeksforGeeks:  
  https://www.geeksforgeeks.org/dsa/linked-list-data-structure/

Phần mã nguồn được biên soạn độc lập và bổ sung API có mã lỗi, quản lý `head/tail/size`, thuật toán đảo danh sách, tìm node giữa, phát hiện chu trình, Memory Pool, kiểm thử và các lưu ý dành cho embedded/RTOS.