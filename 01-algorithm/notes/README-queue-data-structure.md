# Data Structures — Queue

> Tài liệu học tập độc lập về **Queue**, được trình bày theo hướng dễ hiểu và dễ áp dụng bằng ngôn ngữ C.

---

## Mục lục

1. [Queue là gì?](#1-queue-là-gì)
2. [Nguyên tắc FIFO](#2-nguyên-tắc-fifo)
3. [Các thuật ngữ cơ bản](#3-các-thuật-ngữ-cơ-bản)
4. [Các loại Queue](#4-các-loại-queue)
5. [Các thao tác cơ bản](#5-các-thao-tác-cơ-bản)
6. [Minh họa hoạt động](#6-minh-họa-hoạt-động)
7. [Cài đặt Queue tuyến tính bằng mảng](#7-cài-đặt-queue-tuyến-tính-bằng-mảng)
8. [Cài đặt Circular Queue bằng mảng](#8-cài-đặt-circular-queue-bằng-mảng)
9. [Cài đặt Queue bằng Linked List](#9-cài-đặt-queue-bằng-linked-list)
10. [So sánh các cách cài đặt](#10-so-sánh-các-cách-cài-đặt)
11. [Độ phức tạp](#11-độ-phức-tạp)
12. [Ứng dụng của Queue](#12-ứng-dụng-của-queue)
13. [Queue trong lập trình nhúng](#13-queue-trong-lập-trình-nhúng)
14. [Ưu điểm và hạn chế](#14-ưu-điểm-và-hạn-chế)
15. [Các lỗi thường gặp](#15-các-lỗi-thường-gặp)
16. [Bài tập luyện tập](#16-bài-tập-luyện-tập)
17. [Tổng kết](#17-tổng-kết)
18. [Tài liệu tham khảo](#18-tài-liệu-tham-khảo)

---

## 1. Queue là gì?

**Queue** là một cấu trúc dữ liệu tuyến tính hoạt động theo nguyên tắc:

```text
FIFO — First In, First Out
```

Phần tử được đưa vào Queue trước sẽ được lấy ra trước.

Việc thêm và xóa được thực hiện tại hai đầu khác nhau:

```text
Front                                Rear
  |                                    |
  v                                    v
+-------+    +-------+    +-------+    +-------+
|   10  | -> |   20  | -> |   30  | -> |   40  |
+-------+    +-------+    +-------+    +-------+
```

- Phần tử được thêm tại **rear**.
- Phần tử được lấy ra tại **front**.

Queue giống một hàng người chờ phục vụ:

```text
Người đến trước → được phục vụ trước
Người đến sau   → đứng cuối hàng
```

---

## 2. Nguyên tắc FIFO

Giả sử lần lượt đưa các giá trị sau vào Queue:

```text
10, 20, 30
```

Queue trở thành:

```text
Front                  Rear
  |                      |
  v                      v
 10  ->  20  ->  30
```

Khi lấy dữ liệu:

```text
10 được lấy ra trước
20 được lấy ra tiếp theo
30 được lấy ra cuối cùng
```

Trình tự:

```text
Enqueue: 10 -> 20 -> 30
Dequeue: 10 -> 20 -> 30
```

Điểm này khác với Stack:

```text
Stack: Last In, First Out
Queue: First In, First Out
```

---

## 3. Các thuật ngữ cơ bản

## 3.1. Front

`front` biểu diễn vị trí của phần tử sẽ được lấy ra tiếp theo.

```text
Front = phần tử cũ nhất còn tồn tại trong Queue
```

Ví dụ:

```text
Front
  |
  v
 10 -> 20 -> 30
```

Lệnh `dequeue()` sẽ lấy `10`.

---

## 3.2. Rear

`rear` biểu diễn vị trí cuối Queue, nơi phần tử mới được thêm vào.

```text
10 -> 20 -> 30
            ^
            |
           Rear
```

Lệnh:

```c
enqueue(40);
```

tạo ra:

```text
10 -> 20 -> 30 -> 40
```

---

## 3.3. Size

`size` là số phần tử hiện đang tồn tại trong Queue.

```c
size_t size;
```

Ví dụ:

```text
Queue: 10 -> 20 -> 30
Size:  3
```

---

## 3.4. Capacity

`capacity` là số phần tử tối đa Queue có thể chứa.

```c
size_t capacity;
```

Khái niệm này đặc biệt quan trọng với Queue sử dụng mảng cố định.

---

## 3.5. Overflow

**Queue Overflow** xảy ra khi cố thêm phần tử vào một Queue cố định đã đầy.

Ví dụ:

```text
Capacity = 4

Front              Rear
  |                  |
  v                  v
[10][20][30][40]
```

Không thể thêm `50` nếu Queue không hỗ trợ mở rộng.

---

## 3.6. Underflow

**Queue Underflow** xảy ra khi cố lấy phần tử từ Queue đang rỗng.

Các thao tác phải kiểm tra underflow:

- `dequeue()`
- `getFront()`
- `getRear()`

---

## 4. Các loại Queue

## 4.1. Simple Queue

Simple Queue tuân theo FIFO:

- Thêm tại rear.
- Xóa tại front.

```text
Front                       Rear
  |                           |
  v                           v
  A -> B -> C -> D
```

Có thể cài đặt bằng:

- Mảng.
- Circular Array.
- Linked List.

---

## 4.2. Circular Queue

Circular Queue xem mảng như một vòng tròn.

Sau phần tử cuối cùng của mảng, chỉ mục quay về `0`.

```text
0 -> 1 -> 2 -> 3 -> 4
^                   |
|___________________|
```

Chỉ mục được cập nhật bằng:

```c
index = (index + 1U) % capacity;
```

Circular Queue tái sử dụng các ô trống ở đầu mảng sau khi dữ liệu đã được lấy ra.

---

## 4.3. Double-Ended Queue

**Deque** cho phép thêm và xóa tại cả hai đầu.

```text
Push Front  <--- [ Queue ] ---> Push Back
Pop Front   <--- [ Queue ] ---> Pop Back
```

Các thao tác phổ biến:

- `pushFront()`
- `pushBack()`
- `popFront()`
- `popBack()`

Deque có thể hoạt động giống:

- Queue.
- Stack.
- Sliding Window container.

---

## 4.4. Priority Queue

Trong Priority Queue, phần tử được xử lý theo độ ưu tiên thay vì chỉ dựa trên thời điểm thêm.

Ví dụ:

```text
Task A — Priority 2
Task B — Priority 5
Task C — Priority 1
```

Nếu số lớn hơn có độ ưu tiên cao hơn:

```text
Task B được xử lý trước
```

Priority Queue thường được cài đặt bằng:

- Binary Heap.
- Sorted Array.
- Sorted Linked List.
- Balanced Tree.

Priority Queue không phải Queue FIFO thuần túy.

---

## 5. Các thao tác cơ bản

## 5.1. Enqueue

Thêm một phần tử tại cuối Queue.

```c
enqueue(value);
```

Luồng xử lý:

```text
Kiểm tra Queue đầy
        |
        +-- Đầy --> Báo overflow
        |
        v
Ghi value tại rear
        |
        v
Cập nhật rear
        |
        v
Tăng size
```

---

## 5.2. Dequeue

Lấy và xóa phần tử tại đầu Queue.

```c
dequeue(&value);
```

Luồng xử lý:

```text
Kiểm tra Queue rỗng
        |
        +-- Rỗng --> Báo underflow
        |
        v
Đọc phần tử tại front
        |
        v
Cập nhật front
        |
        v
Giảm size
```

---

## 5.3. Get Front

Đọc phần tử đầu Queue nhưng không xóa nó.

```c
getFront(&value);
```

Queue không thay đổi sau thao tác này.

---

## 5.4. Get Rear

Đọc phần tử cuối Queue nhưng không xóa nó.

```c
getRear(&value);
```

---

## 5.5. Is Empty

Kiểm tra Queue có rỗng không.

```c
isEmpty();
```

Nếu quản lý bằng `size`:

```c
size == 0U
```

---

## 5.6. Is Full

Kiểm tra Queue cố định có đầy không.

```c
isFull();
```

Nếu quản lý bằng `size`:

```c
size == capacity
```

---

## 5.7. Size

Trả về số phần tử đang tồn tại.

```c
size();
```

---

## 6. Minh họa hoạt động

Giả sử Queue có capacity bằng 5.

Ban đầu:

```text
Front = 0
Rear  = 0
Size  = 0

[ _ ][ _ ][ _ ][ _ ][ _ ]
```

### Enqueue 10

```text
[10 ][ _ ][ _ ][ _ ][ _ ]
 ^
 |
Front
      ^
      |
     Rear

Size = 1
```

Trong cách quản lý ở README này:

- `front` chỉ phần tử sẽ được đọc.
- `rear` chỉ ô trống sẽ được ghi tiếp theo.

---

### Enqueue 20

```text
[10 ][20 ][ _ ][ _ ][ _ ]
 ^
 |
Front
           ^
           |
          Rear

Size = 2
```

---

### Enqueue 30

```text
[10 ][20 ][30 ][ _ ][ _ ]
 ^
 |
Front
                ^
                |
               Rear

Size = 3
```

---

### Get Front

Giá trị nhận được:

```text
10
```

Queue không thay đổi.

---

### Get Rear

Giá trị nhận được:

```text
30
```

Do `rear` đang chỉ ô trống tiếp theo nên phần tử cuối nằm tại:

```c
(rear + capacity - 1U) % capacity
```

---

### Dequeue

Giá trị lấy ra:

```text
10
```

Sau đó:

```text
[10 ][20 ][30 ][ _ ][ _ ]
      ^
      |
     Front
                ^
                |
               Rear

Size = 2
```

Dữ liệu `10` có thể vẫn còn trong RAM, nhưng không còn thuộc vùng logic hợp lệ của Queue.

---

## 7. Cài đặt Queue tuyến tính bằng mảng

Cách đơn giản nhất dùng hai chỉ mục:

```c
int front;
int rear;
```

Ban đầu:

```c
front = 0;
rear = -1;
```

Enqueue:

```c
rear++;
data[rear] = value;
```

Dequeue:

```c
value = data[front];
front++;
```

### Vấn đề

Giả sử mảng có 5 ô.

Sau khi thêm:

```text
[10][20][30][40][50]
 ^               ^
 |               |
front           rear
```

Lấy ra ba phần tử:

```text
[10][20][30][40][50]
            ^     ^
            |     |
          front  rear
```

Ba ô đầu đã trống về mặt logic:

```text
[ _ ][ _ ][ _ ][40][50]
```

Tuy nhiên, `rear` đã ở cuối mảng.

Nếu chỉ kiểm tra:

```c
rear == capacity - 1
```

Queue bị xem là đầy dù vẫn còn các ô trống ở đầu.

### Các cách khắc phục

1. Dịch toàn bộ phần tử về đầu mảng sau mỗi lần dequeue.
2. Reset chỉ mục khi Queue trở thành rỗng.
3. Sử dụng Circular Queue.

Dịch toàn bộ dữ liệu có thể làm `dequeue()` trở thành:

```text
O(n)
```

Circular Queue là lựa chọn hiệu quả hơn.

---

## 8. Cài đặt Circular Queue bằng mảng

Cấu trúc project:

```text
circular-queue/
├── include/
│   └── queue.h
├── src/
│   ├── queue.c
│   └── main.c
└── README.md
```

---

### 8.1. File `queue.h`

```c
#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct
{
    int *data;
    size_t capacity;

    size_t front;
    size_t rear;
    size_t size;
} Queue;

/**
 * @brief Initialize a queue using caller-provided storage.
 */
bool Queue_Init(
    Queue *queue,
    int *storage,
    size_t capacity
);

/**
 * @brief Insert one value at the rear of the queue.
 */
bool Queue_Enqueue(
    Queue *queue,
    int value
);

/**
 * @brief Remove and return the value at the front.
 */
bool Queue_Dequeue(
    Queue *queue,
    int *value
);

/**
 * @brief Read the front value without removing it.
 */
bool Queue_GetFront(
    const Queue *queue,
    int *value
);

/**
 * @brief Read the rear value without removing it.
 */
bool Queue_GetRear(
    const Queue *queue,
    int *value
);

/**
 * @brief Return true when the queue contains no elements.
 */
bool Queue_IsEmpty(const Queue *queue);

/**
 * @brief Return true when the queue has reached capacity.
 */
bool Queue_IsFull(const Queue *queue);

/**
 * @brief Return the current number of elements.
 */
size_t Queue_Size(const Queue *queue);

/**
 * @brief Return the maximum number of elements.
 */
size_t Queue_Capacity(const Queue *queue);

/**
 * @brief Remove all elements without modifying storage.
 */
void Queue_Clear(Queue *queue);

#endif
```

---

### 8.2. File `queue.c`

```c
#include "queue.h"

bool Queue_Init(
    Queue *queue,
    int *storage,
    size_t capacity
)
{
    if ((queue == NULL) ||
        (storage == NULL) ||
        (capacity == 0U))
    {
        return false;
    }

    queue->data = storage;
    queue->capacity = capacity;

    queue->front = 0U;
    queue->rear = 0U;
    queue->size = 0U;

    return true;
}

bool Queue_IsEmpty(const Queue *queue)
{
    return (queue == NULL) || (queue->size == 0U);
}

bool Queue_IsFull(const Queue *queue)
{
    if (queue == NULL)
    {
        return false;
    }

    return queue->size == queue->capacity;
}

size_t Queue_Size(const Queue *queue)
{
    if (queue == NULL)
    {
        return 0U;
    }

    return queue->size;
}

size_t Queue_Capacity(const Queue *queue)
{
    if (queue == NULL)
    {
        return 0U;
    }

    return queue->capacity;
}

bool Queue_Enqueue(
    Queue *queue,
    int value
)
{
    if ((queue == NULL) || (queue->data == NULL))
    {
        return false;
    }

    if (Queue_IsFull(queue))
    {
        return false;
    }

    queue->data[queue->rear] = value;

    queue->rear =
        (queue->rear + 1U) % queue->capacity;

    queue->size++;

    return true;
}

bool Queue_Dequeue(
    Queue *queue,
    int *value
)
{
    if ((queue == NULL) ||
        (queue->data == NULL) ||
        (value == NULL))
    {
        return false;
    }

    if (Queue_IsEmpty(queue))
    {
        return false;
    }

    *value = queue->data[queue->front];

    queue->front =
        (queue->front + 1U) % queue->capacity;

    queue->size--;

    return true;
}

bool Queue_GetFront(
    const Queue *queue,
    int *value
)
{
    if ((queue == NULL) ||
        (queue->data == NULL) ||
        (value == NULL))
    {
        return false;
    }

    if (Queue_IsEmpty(queue))
    {
        return false;
    }

    *value = queue->data[queue->front];

    return true;
}

bool Queue_GetRear(
    const Queue *queue,
    int *value
)
{
    size_t rear_index;

    if ((queue == NULL) ||
        (queue->data == NULL) ||
        (value == NULL))
    {
        return false;
    }

    if (Queue_IsEmpty(queue))
    {
        return false;
    }

    rear_index =
        (queue->rear + queue->capacity - 1U)
        % queue->capacity;

    *value = queue->data[rear_index];

    return true;
}

void Queue_Clear(Queue *queue)
{
    if (queue == NULL)
    {
        return;
    }

    queue->front = 0U;
    queue->rear = 0U;
    queue->size = 0U;
}
```

---

### 8.3. File `main.c`

```c
#include "queue.h"

#include <stdio.h>

#define QUEUE_CAPACITY 5U

int main(void)
{
    Queue queue;
    int storage[QUEUE_CAPACITY];
    int value;

    if (!Queue_Init(
            &queue,
            storage,
            QUEUE_CAPACITY))
    {
        printf("Queue initialization failed.\n");
        return 1;
    }

    Queue_Enqueue(&queue, 10);
    Queue_Enqueue(&queue, 20);
    Queue_Enqueue(&queue, 30);

    if (Queue_GetFront(&queue, &value))
    {
        printf("Front: %d\n", value);
    }

    if (Queue_GetRear(&queue, &value))
    {
        printf("Rear: %d\n", value);
    }

    while (Queue_Dequeue(&queue, &value))
    {
        printf("Dequeue: %d\n", value);
    }

    return 0;
}
```

Kết quả:

```text
Front: 10
Rear: 30
Dequeue: 10
Dequeue: 20
Dequeue: 30
```

---

### 8.4. Biên dịch

```bash
gcc -std=c11 \
    -Wall \
    -Wextra \
    -Wpedantic \
    -Iinclude \
    src/queue.c \
    src/main.c \
    -o queue_demo
```

Chạy:

```bash
./queue_demo
```

---

### 8.5. Ví dụ quay vòng

Capacity:

```text
5
```

Sau khi enqueue:

```text
[10][20][30][40][ _ ]
 ^
 |
front
                ^
                |
               rear
```

Dequeue hai phần tử:

```text
[10][20][30][40][ _ ]
        ^
        |
       front
                ^
                |
               rear
```

Enqueue `50`:

```text
[10][20][30][40][50]
        ^
        |
       front
 ^
 |
rear
```

`rear` quay lại chỉ mục `0`.

Enqueue `60`:

```text
[60][20][30][40][50]
        ^
        |
       front
     ^
     |
    rear
```

Thứ tự logic của Queue là:

```text
30 -> 40 -> 50 -> 60
```

Dù thứ tự vật lý trong mảng là:

```text
60, 20, 30, 40, 50
```

---

## 9. Cài đặt Queue bằng Linked List

Queue bằng Linked List giữ hai con trỏ:

```text
front → Node đầu
rear  → Node cuối
```

```text
front                                rear
  |                                    |
  v                                    v
+------+-------+    +------+-------+    +------+------+
|  10  | next  | -> |  20  | next  | -> |  30  | NULL |
+------+-------+    +------+-------+    +------+------+
```

- Enqueue: thêm Node sau `rear`.
- Dequeue: xóa Node tại `front`.

Nếu giữ cả `front` và `rear`, hai thao tác đều là `O(1)`.

---

### 9.1. File `linked_queue.h`

```c
#ifndef LINKED_QUEUE_H
#define LINKED_QUEUE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct QueueNode
{
    int data;
    struct QueueNode *next;
} QueueNode;

typedef struct
{
    QueueNode *front;
    QueueNode *rear;
    size_t size;
} LinkedQueue;

void LinkedQueue_Init(LinkedQueue *queue);

bool LinkedQueue_Enqueue(
    LinkedQueue *queue,
    int value
);

bool LinkedQueue_Dequeue(
    LinkedQueue *queue,
    int *value
);

bool LinkedQueue_GetFront(
    const LinkedQueue *queue,
    int *value
);

bool LinkedQueue_GetRear(
    const LinkedQueue *queue,
    int *value
);

bool LinkedQueue_IsEmpty(const LinkedQueue *queue);

size_t LinkedQueue_Size(const LinkedQueue *queue);

void LinkedQueue_Clear(LinkedQueue *queue);

#endif
```

---

### 9.2. File `linked_queue.c`

```c
#include "linked_queue.h"

#include <stdlib.h>

void LinkedQueue_Init(LinkedQueue *queue)
{
    if (queue == NULL)
    {
        return;
    }

    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0U;
}

bool LinkedQueue_IsEmpty(const LinkedQueue *queue)
{
    return (queue == NULL) || (queue->front == NULL);
}

size_t LinkedQueue_Size(const LinkedQueue *queue)
{
    if (queue == NULL)
    {
        return 0U;
    }

    return queue->size;
}

bool LinkedQueue_Enqueue(
    LinkedQueue *queue,
    int value
)
{
    QueueNode *new_node;

    if (queue == NULL)
    {
        return false;
    }

    new_node = (QueueNode *)malloc(sizeof(QueueNode));

    if (new_node == NULL)
    {
        return false;
    }

    new_node->data = value;
    new_node->next = NULL;

    if (queue->rear == NULL)
    {
        queue->front = new_node;
        queue->rear = new_node;
    }
    else
    {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }

    queue->size++;

    return true;
}

bool LinkedQueue_Dequeue(
    LinkedQueue *queue,
    int *value
)
{
    QueueNode *node_to_remove;

    if ((queue == NULL) || (value == NULL))
    {
        return false;
    }

    if (LinkedQueue_IsEmpty(queue))
    {
        return false;
    }

    node_to_remove = queue->front;

    *value = node_to_remove->data;
    queue->front = node_to_remove->next;

    free(node_to_remove);
    queue->size--;

    if (queue->front == NULL)
    {
        queue->rear = NULL;
    }

    return true;
}

bool LinkedQueue_GetFront(
    const LinkedQueue *queue,
    int *value
)
{
    if ((queue == NULL) || (value == NULL))
    {
        return false;
    }

    if (LinkedQueue_IsEmpty(queue))
    {
        return false;
    }

    *value = queue->front->data;

    return true;
}

bool LinkedQueue_GetRear(
    const LinkedQueue *queue,
    int *value
)
{
    if ((queue == NULL) || (value == NULL))
    {
        return false;
    }

    if (LinkedQueue_IsEmpty(queue))
    {
        return false;
    }

    *value = queue->rear->data;

    return true;
}

void LinkedQueue_Clear(LinkedQueue *queue)
{
    QueueNode *current;

    if (queue == NULL)
    {
        return;
    }

    current = queue->front;

    while (current != NULL)
    {
        QueueNode *next_node = current->next;

        free(current);
        current = next_node;
    }

    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0U;
}
```

---

### 9.3. Trường hợp Queue chỉ còn một Node

Trước khi dequeue:

```text
front
  |
  v
+------+------+
|  10  | NULL |
+------+------+
  ^
  |
rear
```

Sau khi xóa:

```text
front = NULL
rear  = NULL
size  = 0
```

Nếu quên đặt `rear = NULL`, `rear` sẽ trở thành dangling pointer.

---

## 10. So sánh các cách cài đặt

| Tiêu chí | Linear Array | Circular Array | Linked List |
|---|---|---|---|
| Bộ nhớ | Mảng liên tiếp | Mảng liên tiếp | Các Node rời nhau |
| Capacity | Cố định | Cố định | Theo bộ nhớ khả dụng |
| Tái sử dụng ô đầu mảng | Không tốt | Có | Không áp dụng |
| Enqueue | `O(1)` | `O(1)` | `O(1)` với rear |
| Dequeue | `O(1)` hoặc `O(n)` nếu dịch | `O(1)` | `O(1)` |
| Bộ nhớ phụ/phần tử | Không | Không | Một con trỏ |
| Cache locality | Tốt | Tốt | Thường kém hơn |
| Cấp phát động | Không | Không | Thường có |
| Phân mảnh Heap | Không | Không | Có thể |
| Phù hợp embedded | Trung bình | Rất phù hợp | Cần cân nhắc |

Trong phần mềm nhúng, Circular Queue thường là lựa chọn tốt vì:

- RAM xác định trước.
- Không cần `malloc()`.
- Enqueue và dequeue là `O(1)`.
- Tái sử dụng toàn bộ mảng.

---

## 11. Độ phức tạp

| Thao tác | Circular Array Queue | Linked Queue |
|---|---:|---:|
| Enqueue | `O(1)` | `O(1)` |
| Dequeue | `O(1)` | `O(1)` |
| Get Front | `O(1)` | `O(1)` |
| Get Rear | `O(1)` | `O(1)` |
| Is Empty | `O(1)` | `O(1)` |
| Is Full | `O(1)` | Không áp dụng trực tiếp |
| Size | `O(1)` | `O(1)` nếu lưu `size` |
| Clear | `O(1)` logic | `O(n)` để giải phóng Node |
| Tìm kiếm | `O(n)` | `O(n)` |

Queue không được thiết kế để truy cập phần tử ở giữa theo chỉ số.

---

## 12. Ứng dụng của Queue

## 12.1. Buffer giữa hai thiết bị có tốc độ khác nhau

Ví dụ:

```text
Thiết bị nhanh → Queue Buffer → Thiết bị chậm
```

Các trường hợp:

- Keyboard và CPU.
- UART và ứng dụng.
- Network interface và protocol stack.
- ADC/DMA và task xử lý.
- Producer và consumer.

Queue hấp thụ sự khác biệt tốc độ trong một khoảng thời gian giới hạn.

---

## 12.2. CPU Scheduling

Các task chờ CPU có thể được lưu trong Ready Queue.

```text
Ready Queue:
Task A -> Task B -> Task C
```

Scheduler lấy task tại front để chạy.

Round-robin scheduler thường dùng cấu trúc vòng:

```text
Task A -> Task B -> Task C
   ^                 |
   |_________________|
```

---

## 12.3. Breadth-First Search

BFS sử dụng Queue để duyệt theo từng lớp.

```text
Enqueue start node

While queue is not empty:
    Dequeue one node
    Visit it
    Enqueue unvisited neighbors
```

Ví dụ cây:

```text
        A
       / \
      B   C
     / \   \
    D   E   F
```

Level-order traversal:

```text
A, B, C, D, E, F
```

---

## 12.4. Level-Order Traversal

Queue lưu các Node của từng tầng trong cây.

```text
Enqueue root
Dequeue node
Enqueue its children
```

---

## 12.5. Printer Queue

Nhiều tài liệu cùng yêu cầu in:

```text
Document A -> Document B -> Document C
```

Máy in xử lý theo thứ tự nhận yêu cầu.

---

## 12.6. Network Packet Queue

Router và switch có thể lưu packet đang chờ truyền.

```text
RX packets -> Queue -> TX interface
```

Khi Queue đầy, hệ thống cần chính sách:

- Drop packet mới.
- Drop packet cũ.
- Ưu tiên packet quan trọng.
- Backpressure.
- Flow control.

---

## 12.7. Message Queue

Task hoặc process có thể trao đổi message qua Queue.

```text
Producer Task
     |
     v
Message Queue
     |
     v
Consumer Task
```

Message Queue giúp tách rời:

- Thời điểm gửi.
- Thời điểm xử lý.
- Tốc độ producer.
- Tốc độ consumer.

---

## 12.8. Event Queue

Trong kiến trúc Event-Driven:

```text
Interrupt/Producer
       |
       v
   Event Queue
       |
       v
Dispatcher/Consumer
```

Event thường được xử lý theo thứ tự đến.

---

## 12.9. Call Center và hàng chờ dịch vụ

Queue phù hợp khi nhiều yêu cầu cùng chờ một resource:

- ATM.
- Quầy bán vé.
- Call center.
- Database connection.
- Thread pool.
- Web server request.
- Shared peripheral.

---

## 12.10. FIFO Page Replacement

Thuật toán FIFO thay thế page cũ nhất trước.

Queue lưu thứ tự page được đưa vào bộ nhớ.

---

## 12.11. Streaming và Pipeline

Dữ liệu đi qua nhiều stage:

```text
Capture Queue
      |
      v
Processing Queue
      |
      v
Encoding Queue
      |
      v
Output Queue
```

Mỗi stage lấy dữ liệu từ Queue đầu vào và đưa kết quả vào Queue đầu ra.

---

## 13. Queue trong lập trình nhúng

## 13.1. UART RX Queue

Interrupt nhận từng byte:

```text
UART RX Interrupt
       |
       v
Read received byte
       |
       v
Enqueue byte
       |
       v
Return from interrupt
```

Vòng lặp chính:

```text
Main loop
   |
   v
Dequeue byte
   |
   v
Parse command or protocol
```

Trong trường hợp byte stream đơn giản, cấu trúc này còn được gọi là Ring Buffer hoặc Circular Buffer.

---

## 13.2. Event Queue

ISR chỉ tạo sự kiện ngắn:

```c
Event event = {
    .signal = EVENT_BUTTON_PRESSED
};

EventQueue_Push(&event_queue, event);
```

Main hoặc task xử lý sau:

```c
while (EventQueue_Pop(&event_queue, &event))
{
    Event_Dispatch(&event);
}
```

Điều này giúp ISR ngắn và tránh xử lý phức tạp trong interrupt context.

---

## 13.3. RTOS Queue

RTOS Queue thường hỗ trợ:

- Gửi từ task.
- Nhận trong task.
- Chờ có timeout.
- Gửi từ ISR bằng API riêng.
- Đánh thức task đang chờ.
- Copy object hoặc truyền con trỏ.

Cần tuân thủ API của RTOS.

Không nên gọi API task thông thường từ ISR nếu RTOS yêu cầu hàm `FromISR`.

---

## 13.4. Queue truyền giá trị

Ví dụ Queue chứa trực tiếp:

```c
typedef struct
{
    uint16_t id;
    uint16_t value;
} SensorSample;
```

Ưu điểm:

- Ownership đơn giản.
- Không cần quản lý con trỏ.
- Không bị use-after-free.

Hạn chế:

- Có thể copy nhiều dữ liệu.
- Queue tốn RAM nếu phần tử lớn.

---

## 13.5. Queue truyền con trỏ

Queue chỉ lưu:

```c
Message *
```

Payload được lưu trong Memory Pool.

Luồng:

```text
Allocate Message
      |
      v
Fill Message
      |
      v
Enqueue pointer
      |
      v
Consumer dequeues pointer
      |
      v
Process Message
      |
      v
Return Message to Pool
```

Ưu điểm:

- Giảm số lần copy.
- Phù hợp zero-copy.

Hạn chế:

- Ownership phức tạp hơn.
- Có nguy cơ double free.
- Có nguy cơ use-after-free.
- Pool có thể hết object.

---

## 13.6. Queue đầy

Firmware phải xác định chính sách khi Queue đầy.

### Từ chối dữ liệu mới

```text
Giữ toàn bộ dữ liệu cũ
Bỏ dữ liệu mới
```

Phù hợp khi thứ tự cũ quan trọng.

### Ghi đè dữ liệu cũ nhất

```text
Bỏ phần tử tại front
Thêm dữ liệu mới tại rear
```

Phù hợp với sensor sample hoặc log gần nhất.

### Chặn task gửi

Trong RTOS:

```text
Task chờ đến khi Queue có chỗ
```

Không áp dụng trong ISR.

### Báo lỗi hoặc backpressure

Producer giảm tốc độ hoặc dừng phát dữ liệu.

---

## 13.7. Queue rỗng

Consumer có thể:

- Trả về ngay.
- Chờ event.
- Chờ semaphore.
- Block với timeout.
- Đi ngủ cho đến khi producer đánh thức.

Trong bare-metal super loop, thường dùng:

```c
while (Queue_Dequeue(&queue, &value))
{
    Process(value);
}
```

---

## 13.8. Truy cập đồng thời

Nếu ISR enqueue và `main()` dequeue, Queue phải được thiết kế an toàn.

Các lựa chọn:

- Critical section.
- Atomic index.
- Single Producer – Single Consumer Ring Buffer.
- Mutex.
- Semaphore.
- API của RTOS.
- Lock-free Queue phù hợp với kiến trúc CPU.

`volatile` không tự động làm Queue thread-safe.

---

## 13.9. Chọn kích thước Queue

Capacity nên dựa trên:

```text
Tốc độ producer
×
Thời gian consumer có thể bị trì hoãn
```

Ví dụ:

```text
UART nhận:          1000 byte/s
Main có thể bận:    50 ms
```

Số byte có thể đến:

```text
1000 × 0.05 = 50 byte
```

Capacity cần lớn hơn 50 byte để có vùng an toàn.

Cần xét thêm:

- Burst dữ liệu.
- Interrupt latency.
- Worst-case task execution.
- DMA block.
- Protocol frame lớn nhất.
- Thời gian disable interrupt.

---

## 13.10. High-Water Mark

Có thể theo dõi số phần tử lớn nhất từng tồn tại:

```c
if (queue->size > queue->maximum_size)
{
    queue->maximum_size = queue->size;
}
```

Giá trị này giúp:

- Phát hiện Queue quá nhỏ.
- Tránh cấp dư RAM.
- Đánh giá tải thực tế.
- Tìm consumer xử lý quá chậm.

---

## 14. Ưu điểm và hạn chế

## 14.1. Ưu điểm

- Mô hình FIFO rõ ràng.
- Enqueue và dequeue có thể đạt `O(1)`.
- Phù hợp producer–consumer.
- Hữu ích trong scheduling.
- Tách rời nơi tạo và nơi xử lý dữ liệu.
- Dễ dùng làm buffer.
- Circular Queue sử dụng RAM hiệu quả.
- Phù hợp BFS và level-order traversal.
- Có thể triển khai tĩnh cho embedded.

---

## 14.2. Hạn chế

- Chỉ truy cập trực tiếp front và rear.
- Tìm kiếm phần tử là `O(n)`.
- Chèn hoặc xóa giữa Queue không hiệu quả.
- Queue dùng mảng có capacity cố định.
- Queue có thể overflow hoặc underflow.
- Linked Queue tốn thêm con trỏ.
- Queue động có thể phân mảnh Heap.
- Queue đầy cần chính sách xử lý rõ ràng.
- Producer nhanh hơn consumer lâu dài vẫn làm Queue đầy.

---

## 15. Các lỗi thường gặp

## 15.1. Không kiểm tra Queue đầy

Sai:

```c
queue->data[queue->rear] = value;
queue->rear++;
```

Nếu Queue đầy, chương trình có thể ghi ra ngoài mảng hoặc ghi đè dữ liệu chưa xử lý.

Đúng:

```c
if (Queue_IsFull(queue))
{
    return false;
}
```

---

## 15.2. Không kiểm tra Queue rỗng

Sai:

```c
*value = queue->data[queue->front];
queue->front++;
```

Nếu Queue rỗng, giá trị đọc không hợp lệ.

---

## 15.3. Không quay vòng chỉ mục

Sai:

```c
queue->rear++;
```

Nếu `rear` đạt `capacity`, lần truy cập tiếp theo nằm ngoài mảng.

Đúng:

```c
queue->rear =
    (queue->rear + 1U) % queue->capacity;
```

---

## 15.4. Không phân biệt Queue đầy và rỗng

Nếu chỉ sử dụng:

```c
front == rear
```

trạng thái này có thể biểu diễn:

- Queue rỗng.
- Queue đầy sau một vòng.

Có ba cách phổ biến:

1. Lưu biến `size`.
2. Hy sinh một ô trong mảng.
3. Lưu cờ `full`.

README sử dụng biến `size`.

---

## 15.5. Sai khi lấy rear

Trong cài đặt mà `rear` chỉ ô trống tiếp theo:

```c
queue->data[queue->rear]
```

không phải phần tử cuối.

Chỉ mục phần tử cuối là:

```c
(queue->rear + queue->capacity - 1U)
% queue->capacity
```

---

## 15.6. Quên cập nhật rear của Linked Queue

Khi xóa Node cuối cùng:

```text
front = NULL
```

phải đặt thêm:

```c
rear = NULL;
```

Nếu không, `rear` trở thành dangling pointer.

---

## 15.7. Linked Queue không giữ rear

Nếu chỉ giữ `front`, mỗi lần enqueue cuối phải duyệt toàn bộ danh sách:

```text
O(n)
```

Giữ con trỏ `rear` giúp enqueue là `O(1)`.

---

## 15.8. Quên giải phóng Linked Queue

Mỗi Node được tạo bằng `malloc()` phải được giải phóng.

```c
LinkedQueue_Clear(&queue);
```

Nếu không, chương trình bị memory leak.

---

## 15.9. Dùng Queue khi cần Priority Queue

FIFO không xử lý độ ưu tiên.

Nếu task quan trọng phải chạy trước, cần:

- Priority Queue.
- Nhiều Queue theo priority.
- Scheduler riêng.

---

## 15.10. Queue chỉ trì hoãn, không giải quyết chênh lệch tốc độ lâu dài

Nếu producer luôn tạo 100 item/s nhưng consumer chỉ xử lý 80 item/s:

```text
Mỗi giây Queue tăng 20 item
```

Dù Queue lớn đến đâu, cuối cùng vẫn đầy.

Cần:

- Tăng tốc consumer.
- Giảm tốc producer.
- Thêm consumer.
- Bỏ dữ liệu theo chính sách.
- Thực hiện backpressure.

---

## 16. Bài tập luyện tập

## Cơ bản

1. Cài đặt Queue bằng mảng tuyến tính.
2. Cài đặt Circular Queue.
3. Cài đặt Queue bằng Linked List.
4. Viết `enqueue`, `dequeue`, `front`, `rear`.
5. Đảo ngược Queue.
6. In toàn bộ Queue mà không làm mất dữ liệu.
7. Tìm một giá trị trong Queue.
8. Tính tổng các phần tử.
9. Xóa toàn bộ Queue.
10. Cài đặt Queue chứa `struct`.

## Trung bình

1. Cài đặt Stack bằng hai Queue.
2. Cài đặt Queue bằng hai Stack.
3. Đảo ngược `k` phần tử đầu.
4. Sinh số nhị phân từ `1` đến `n`.
5. Tìm ký tự không lặp đầu tiên trong stream.
6. Level-order traversal của Binary Tree.
7. BFS cho Graph.
8. Flood Fill bằng Queue.
9. Rotten Oranges.
10. Number of Islands bằng BFS.
11. Minimum Knight Moves.
12. First Negative Number in Every Window.
13. Interleave hai nửa Queue.
14. Cài đặt Deque bằng Circular Array.
15. Cài đặt nhiều Queue trong một mảng.

## Nâng cao

1. Sliding Window Maximum.
2. Shortest Path in Binary Maze.
3. 0–1 BFS bằng Deque.
4. Multi-source BFS.
5. Snake and Ladder shortest path.
6. Minimum Cost Path.
7. Task Scheduler.
8. Design Hit Counter.
9. Moving Average from Data Stream.
10. Circular Tour.
11. LRU Cache kết hợp Hash Table và Deque.
12. Lock-free Single Producer – Single Consumer Queue.
13. Multi-Producer Multi-Consumer Queue.
14. Event Queue kết hợp Memory Pool.
15. Priority Queue bằng Binary Heap.

---

## 17. Tổng kết

Queue là cấu trúc dữ liệu tuyến tính hoạt động theo:

```text
FIFO — First In, First Out
```

Các đầu của Queue:

```text
front → nơi lấy dữ liệu
rear  → nơi thêm dữ liệu
```

Các thao tác chính:

```text
enqueue()  → thêm phần tử tại rear
dequeue()  → lấy phần tử tại front
getFront() → đọc phần tử đầu
getRear()  → đọc phần tử cuối
isEmpty()  → kiểm tra rỗng
isFull()   → kiểm tra đầy
size()     → số phần tử hiện có
```

Với Circular Queue:

```c
next_index = (current_index + 1U) % capacity;
```

Độ phức tạp của các thao tác cơ bản:

```text
Enqueue:   O(1)
Dequeue:   O(1)
Get Front: O(1)
Get Rear:  O(1)
```

Queue được ứng dụng trong:

- Buffer giữa các thiết bị.
- CPU scheduling.
- Breadth-First Search.
- Level-order traversal.
- Printer queue.
- Network packet queue.
- Message queue.
- Event-driven system.
- RTOS.
- UART RX/TX.
- Producer–consumer.
- Streaming pipeline.

Trong embedded:

- Nên ưu tiên Circular Queue dùng mảng cố định.
- Luôn xử lý overflow và underflow.
- Xác định chính sách khi Queue đầy.
- Cân nhắc Queue truyền giá trị hoặc con trỏ.
- Dùng Memory Pool nếu truyền object động.
- Bảo vệ truy cập giữa ISR và main/task.
- Theo dõi high-water mark.
- Chọn capacity theo worst-case load.

---

## 18. Tài liệu tham khảo

README này được biên soạn và diễn đạt lại độc lập dựa trên phạm vi chủ đề được trình bày trong:

- GeeksforGeeks — **Queue Data Structure**:  
  https://www.geeksforgeeks.org/dsa/queue-data-structure/
