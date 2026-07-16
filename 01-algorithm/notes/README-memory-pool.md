# Data Structures — Memory Pool và ứng dụng

> Tài liệu này trình bày độc lập về **Memory Pool** theo hướng thực hành bằng ngôn ngữ C và ứng dụng trong phần mềm nhúng.

---

## Mục lục

1. [Đặt vấn đề](#1-đặt-vấn-đề)
2. [Memory Pool là gì?](#2-memory-pool-là-gì)
3. [Cấu trúc của Memory Pool](#3-cấu-trúc-của-memory-pool)
4. [Nguyên lý hoạt động](#4-nguyên-lý-hoạt-động)
5. [Thiết kế Memory Pool](#5-thiết-kế-memory-pool)
6. [Cài đặt bằng C](#6-cài-đặt-bằng-c)
7. [Ví dụ sử dụng](#7-ví-dụ-sử-dụng)
8. [Phân tích mã nguồn](#8-phân-tích-mã-nguồn)
9. [Ứng dụng trong hệ thống nhúng](#9-ứng-dụng-trong-hệ-thống-nhúng)
10. [Critical Section và truy cập đồng thời](#10-critical-section-và-truy-cập-đồng-thời)
11. [Giám sát Memory Pool](#11-giám-sát-memory-pool)
12. [Memory Pool và Heap](#12-memory-pool-và-heap)
13. [Độ phức tạp](#13-độ-phức-tạp)
14. [Ưu điểm và hạn chế](#14-ưu-điểm-và-hạn-chế)
15. [Các lỗi thường gặp](#15-các-lỗi-thường-gặp)
16. [Hướng mở rộng](#16-hướng-mở-rộng)
17. [Tổng kết](#17-tổng-kết)
18. [Tài liệu tham khảo](#18-tài-liệu-tham-khảo)

---

## 1. Đặt vấn đề

Trong quá trình chương trình hoạt động, hệ thống thường cần tạo và hủy các đối tượng tạm thời như:

- Message.
- Event.
- Packet.
- Buffer.
- Command.
- Connection.
- Control block.
- Cấu trúc dữ liệu của ứng dụng.

Cách thông thường trong C là cấp phát trên Heap:

```c
void *object = malloc(object_size);

/* Use object. */

free(object);
```

Cách này linh hoạt nhưng trong phần mềm nhúng và hệ thống thời gian thực có thể phát sinh một số vấn đề:

- Thời gian cấp phát và giải phóng không hoàn toàn cố định.
- Heap có thể bị phân mảnh sau nhiều lần cấp phát và giải phóng.
- Khó dự đoán chính xác lượng bộ nhớ còn lại.
- Có nguy cơ memory leak, double free và use-after-free.
- `malloc()` có thể không phù hợp để gọi trong Interrupt Service Routine.
- Hệ thống có thể thất bại tại thời điểm chạy do Heap không còn vùng nhớ phù hợp.

Một giải pháp thường được sử dụng là chuẩn bị sẵn một vùng nhớ cố định và tái sử dụng các phần tử bên trong vùng nhớ đó.

Giải pháp này được gọi là **Memory Pool**.

---

## 2. Memory Pool là gì?

**Memory Pool** là một kỹ thuật quản lý bộ nhớ trong đó một vùng nhớ được cấp phát trước và chia thành nhiều phần tử có kích thước xác định.

Mỗi phần tử có thể được gọi là:

- Block.
- Chunk.
- Object.
- Package.
- Node.
- Slot.

Khi chương trình cần một đối tượng:

```text
Memory Pool cấp một block đang trống
```

Khi chương trình dùng xong:

```text
Block được trả lại Memory Pool
```

Block không bị hủy khỏi RAM mà được tái sử dụng trong những lần cấp phát sau.

### Mô hình tổng quát

```text
+-------------------------------------------------------+
|                    MEMORY POOL                        |
+------------+------------+------------+----------------+
|  Block 0   |  Block 1   |  Block 2   |    Block N     |
+------------+------------+------------+----------------+
```

Memory Pool thường có:

- Số block cố định.
- Kích thước mỗi block cố định.
- Một cấu trúc theo dõi block nào đang trống.
- API để cấp phát và thu hồi block.

---

## 3. Cấu trúc của Memory Pool

Một cách cài đặt phổ biến là sử dụng:

```text
Static Array + Singly Linked List
```

Mảng tĩnh chứa toàn bộ block:

```c
static PoolBlock pool_storage[POOL_CAPACITY];
```

Các block chưa được sử dụng được nối thành một **free list**.

```text
free_list
    |
    v
+---------+    +---------+    +---------+    +---------+
| Block 0 | -> | Block 1 | -> | Block 2 | -> | Block 3 | -> NULL
+---------+    +---------+    +---------+    +---------+
```

Mỗi block cần có một con trỏ để liên kết với block trống tiếp theo:

```c
typedef struct PoolBlock
{
    struct PoolBlock *next;
    uint8_t payload[POOL_BLOCK_DATA_SIZE];
} PoolBlock;
```

Khi block đang nằm trong free list:

```text
next → block trống tiếp theo
```

Khi block được cấp phát:

```text
payload → vùng dữ liệu ứng dụng có thể sử dụng
```

---

## 4. Nguyên lý hoạt động

## 4.1. Trạng thái ban đầu

Giả sử pool có 4 block:

```text
Index:       0           1           2           3
         +-------+   +-------+   +-------+   +-------+
Pool:    |   B0  |   |   B1  |   |   B2  |   |   B3  |
         +-------+   +-------+   +-------+   +-------+
```

Sau khi khởi tạo, toàn bộ block đều trống:

```text
free_list
    |
    v
   B0 -> B1 -> B2 -> B3 -> NULL
```

Số block trống:

```text
free_count = 4
```

Số block đang sử dụng:

```text
used_count = 0
```

---

## 4.2. Cấp phát block đầu tiên

Khi gọi:

```c
PoolBlock *block = MemoryPool_Allocate(&pool);
```

Pool lấy phần tử đầu của free list:

```text
allocated = B0
```

Sau đó cập nhật free list:

```text
free_list
    |
    v
   B1 -> B2 -> B3 -> NULL
```

Kết quả trả về:

```text
block -> B0
```

---

## 4.3. Cấp phát thêm một block

Lần tiếp theo:

```text
allocated = B1
```

Free list trở thành:

```text
free_list
    |
    v
   B2 -> B3 -> NULL
```

Hai block đang được sử dụng:

```text
B0, B1
```

Hai block còn trống:

```text
B2, B3
```

---

## 4.4. Giải phóng một block

Giả sử `B0` đã được sử dụng xong.

Khi gọi:

```c
MemoryPool_Free(&pool, B0);
```

`B0` được chèn lại vào đầu free list:

```text
B0 -> B2 -> B3 -> NULL
^
|
free_list
```

Thứ tự block trong free list không nhất thiết giống thứ tự ban đầu trong mảng.

Điều quan trọng là:

- Mỗi block trống xuất hiện đúng một lần trong free list.
- Block đang sử dụng không nằm trong free list.
- Không có liên kết bị mất hoặc tạo cycle ngoài ý muốn.

---

## 4.5. Khi Pool hết block

Nếu tất cả block đã được cấp phát:

```text
free_list = NULL
free_count = 0
```

Lần cấp phát tiếp theo phải trả về lỗi:

```c
NULL
```

Chương trình sử dụng Memory Pool phải xử lý tình huống này.

Ví dụ:

```c
PoolBlock *block = MemoryPool_Allocate(&pool);

if (block == NULL)
{
    /* Pool exhausted: drop, retry, log or enter safe state. */
}
```

Không được dereference con trỏ khi kết quả cấp phát là `NULL`.

---

## 5. Thiết kế Memory Pool

Ví dụ trong tài liệu này sử dụng:

```text
Số block:                  16
Dung lượng dữ liệu/block:  32 byte
```

### Cấu trúc một block

```c
typedef struct MemoryPoolBlock
{
    struct MemoryPoolBlock *next;
    uint8_t data[32];
} MemoryPoolBlock;
```

### Cấu trúc quản lý pool

```c
typedef struct
{
    MemoryPoolBlock *free_list;
    MemoryPoolBlock *storage;
    size_t capacity;
    size_t free_count;
    size_t minimum_free_count;
    bool initialized;
} MemoryPool;
```

### Ý nghĩa các trường

#### `free_list`

Trỏ đến block trống đầu tiên.

#### `storage`

Trỏ đến mảng chứa toàn bộ block.

#### `capacity`

Tổng số block trong pool.

#### `free_count`

Số block hiện còn trống.

#### `minimum_free_count`

Số block trống thấp nhất từng ghi nhận.

Giá trị này giúp đánh giá mức sử dụng cao nhất:

```text
maximum_used = capacity - minimum_free_count
```

#### `initialized`

Cho biết pool đã được khởi tạo hay chưa.

---

## 6. Cài đặt bằng C

Cấu trúc project:

```text
memory-pool/
├── include/
│   └── memory_pool.h
├── src/
│   ├── memory_pool.c
│   └── main.c
└── README.md
```

---

### 6.1. File `memory_pool.h`

```c
#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MEMORY_POOL_BLOCK_DATA_SIZE    32U

typedef struct MemoryPoolBlock
{
    struct MemoryPoolBlock *next;
    uint8_t data[MEMORY_POOL_BLOCK_DATA_SIZE];
} MemoryPoolBlock;

typedef struct
{
    MemoryPoolBlock *free_list;
    MemoryPoolBlock *storage;

    size_t capacity;
    size_t free_count;
    size_t minimum_free_count;

    bool initialized;
} MemoryPool;

/**
 * @brief Initialize a pool with an externally supplied block array.
 *
 * @param pool      Pool control structure.
 * @param storage   Array used as pool storage.
 * @param capacity  Number of blocks in storage.
 *
 * @return true on success, false on invalid arguments.
 */
bool MemoryPool_Init(
    MemoryPool *pool,
    MemoryPoolBlock *storage,
    size_t capacity
);

/**
 * @brief Obtain one block from the pool.
 *
 * @return Address of an available block or NULL when no block is available.
 */
MemoryPoolBlock *MemoryPool_Allocate(MemoryPool *pool);

/**
 * @brief Return a block to the pool.
 *
 * @return true on success, false when the pool or block is invalid.
 */
bool MemoryPool_Free(
    MemoryPool *pool,
    MemoryPoolBlock *block
);

/**
 * @brief Check whether an address belongs to this pool.
 */
bool MemoryPool_Owns(
    const MemoryPool *pool,
    const MemoryPoolBlock *block
);

/**
 * @brief Return the number of currently available blocks.
 */
size_t MemoryPool_GetFreeCount(const MemoryPool *pool);

/**
 * @brief Return the number of currently allocated blocks.
 */
size_t MemoryPool_GetUsedCount(const MemoryPool *pool);

/**
 * @brief Return the maximum number of blocks used at the same time.
 */
size_t MemoryPool_GetHighWaterMark(const MemoryPool *pool);

/**
 * @brief Return true when no free block remains.
 */
bool MemoryPool_IsEmpty(const MemoryPool *pool);

/**
 * @brief Reset all blocks to the free state.
 *
 * This function invalidates every block previously returned to users.
 */
bool MemoryPool_Reset(MemoryPool *pool);

#endif
```

---

### 6.2. File `memory_pool.c`

```c
#include "memory_pool.h"

#include <stdint.h>
#include <string.h>

static bool MemoryPool_IsBlockFree(
    const MemoryPool *pool,
    const MemoryPoolBlock *block
)
{
    const MemoryPoolBlock *current;

    if ((pool == NULL) || (block == NULL))
    {
        return false;
    }

    current = pool->free_list;

    while (current != NULL)
    {
        if (current == block)
        {
            return true;
        }

        current = current->next;
    }

    return false;
}

bool MemoryPool_Init(
    MemoryPool *pool,
    MemoryPoolBlock *storage,
    size_t capacity
)
{
    size_t index;

    if ((pool == NULL) || (storage == NULL) || (capacity == 0U))
    {
        return false;
    }

    pool->storage = storage;
    pool->capacity = capacity;
    pool->free_count = capacity;
    pool->minimum_free_count = capacity;
    pool->initialized = true;

    for (index = 0U; index < capacity; index++)
    {
        memset(storage[index].data, 0, sizeof(storage[index].data));

        if ((index + 1U) < capacity)
        {
            storage[index].next = &storage[index + 1U];
        }
        else
        {
            storage[index].next = NULL;
        }
    }

    pool->free_list = &storage[0];

    return true;
}

MemoryPoolBlock *MemoryPool_Allocate(MemoryPool *pool)
{
    MemoryPoolBlock *allocated_block;

    if ((pool == NULL) || (!pool->initialized))
    {
        return NULL;
    }

    if (pool->free_list == NULL)
    {
        return NULL;
    }

    allocated_block = pool->free_list;
    pool->free_list = allocated_block->next;

    allocated_block->next = NULL;

    if (pool->free_count > 0U)
    {
        pool->free_count--;
    }

    if (pool->free_count < pool->minimum_free_count)
    {
        pool->minimum_free_count = pool->free_count;
    }

    return allocated_block;
}

bool MemoryPool_Owns(
    const MemoryPool *pool,
    const MemoryPoolBlock *block
)
{
    uintptr_t pool_begin;
    uintptr_t pool_end;
    uintptr_t block_address;
    uintptr_t offset;

    if ((pool == NULL) ||
        (!pool->initialized) ||
        (pool->storage == NULL) ||
        (block == NULL))
    {
        return false;
    }

    pool_begin = (uintptr_t)&pool->storage[0];
    pool_end =
        (uintptr_t)&pool->storage[pool->capacity];
    block_address = (uintptr_t)block;

    if ((block_address < pool_begin) ||
        (block_address >= pool_end))
    {
        return false;
    }

    offset = block_address - pool_begin;

    return (offset % sizeof(MemoryPoolBlock)) == 0U;
}

bool MemoryPool_Free(
    MemoryPool *pool,
    MemoryPoolBlock *block
)
{
    if ((pool == NULL) ||
        (!pool->initialized) ||
        (block == NULL))
    {
        return false;
    }

    if (!MemoryPool_Owns(pool, block))
    {
        return false;
    }

    /*
     * This traversal detects a simple double-free condition.
     * It can be disabled in performance-critical production builds
     * when object ownership is already guaranteed by design.
     */
    if (MemoryPool_IsBlockFree(pool, block))
    {
        return false;
    }

    memset(block->data, 0, sizeof(block->data));

    block->next = pool->free_list;
    pool->free_list = block;

    if (pool->free_count < pool->capacity)
    {
        pool->free_count++;
    }

    return true;
}

size_t MemoryPool_GetFreeCount(const MemoryPool *pool)
{
    if ((pool == NULL) || (!pool->initialized))
    {
        return 0U;
    }

    return pool->free_count;
}

size_t MemoryPool_GetUsedCount(const MemoryPool *pool)
{
    if ((pool == NULL) || (!pool->initialized))
    {
        return 0U;
    }

    return pool->capacity - pool->free_count;
}

size_t MemoryPool_GetHighWaterMark(const MemoryPool *pool)
{
    if ((pool == NULL) || (!pool->initialized))
    {
        return 0U;
    }

    return pool->capacity - pool->minimum_free_count;
}

bool MemoryPool_IsEmpty(const MemoryPool *pool)
{
    if ((pool == NULL) || (!pool->initialized))
    {
        return true;
    }

    return pool->free_list == NULL;
}

bool MemoryPool_Reset(MemoryPool *pool)
{
    if ((pool == NULL) ||
        (!pool->initialized) ||
        (pool->storage == NULL) ||
        (pool->capacity == 0U))
    {
        return false;
    }

    return MemoryPool_Init(
        pool,
        pool->storage,
        pool->capacity
    );
}
```

---

### 6.3. File `main.c`

```c
#include "memory_pool.h"

#include <stdio.h>
#include <string.h>

#define APP_POOL_CAPACITY    16U

static MemoryPool app_pool;
static MemoryPoolBlock app_pool_storage[APP_POOL_CAPACITY];

int main(void)
{
    MemoryPoolBlock *first;
    MemoryPoolBlock *second;

    if (!MemoryPool_Init(
            &app_pool,
            app_pool_storage,
            APP_POOL_CAPACITY))
    {
        printf("Pool initialization failed.\n");
        return 1;
    }

    first = MemoryPool_Allocate(&app_pool);
    second = MemoryPool_Allocate(&app_pool);

    if ((first == NULL) || (second == NULL))
    {
        printf("Pool allocation failed.\n");
        return 1;
    }

    memcpy(first->data, "MESSAGE_A", sizeof("MESSAGE_A"));
    memcpy(second->data, "MESSAGE_B", sizeof("MESSAGE_B"));

    printf("First block:  %s\n", (char *)first->data);
    printf("Second block: %s\n", (char *)second->data);

    printf(
        "Used: %zu, Free: %zu\n",
        MemoryPool_GetUsedCount(&app_pool),
        MemoryPool_GetFreeCount(&app_pool)
    );

    if (!MemoryPool_Free(&app_pool, first))
    {
        printf("Could not return the first block.\n");
    }

    if (!MemoryPool_Free(&app_pool, second))
    {
        printf("Could not return the second block.\n");
    }

    printf(
        "Used: %zu, Free: %zu\n",
        MemoryPool_GetUsedCount(&app_pool),
        MemoryPool_GetFreeCount(&app_pool)
    );

    printf(
        "Maximum simultaneous usage: %zu\n",
        MemoryPool_GetHighWaterMark(&app_pool)
    );

    return 0;
}
```

---

### 6.4. Biên dịch

```bash
gcc -std=c11 \
    -Wall \
    -Wextra \
    -Wpedantic \
    -Iinclude \
    src/memory_pool.c \
    src/main.c \
    -o memory_pool_demo
```

Chạy:

```bash
./memory_pool_demo
```

Kết quả dự kiến:

```text
First block:  MESSAGE_A
Second block: MESSAGE_B
Used: 2, Free: 14
Used: 0, Free: 16
Maximum simultaneous usage: 2
```

---

## 7. Ví dụ sử dụng

## 7.1. Sử dụng tương tự `malloc()`

Với Heap:

```c
Message *message = malloc(sizeof(Message));

if (message == NULL)
{
    /* Allocation failed. */
}
```

Với Memory Pool:

```c
MemoryPoolBlock *block = MemoryPool_Allocate(&app_pool);

if (block == NULL)
{
    /* No free block remains. */
}
```

Cả hai cách đều trả về địa chỉ vùng nhớ cho người dùng.

Điểm khác biệt là Memory Pool lấy bộ nhớ từ vùng đã được chuẩn bị trước, thay vì yêu cầu Heap tìm một vùng nhớ mới.

---

## 7.2. Lưu một Message

```c
typedef struct
{
    uint16_t signal;
    uint16_t length;
    uint8_t payload[24];
} AppMessage;
```

Kiểm tra kích thước:

```c
_Static_assert(
    sizeof(AppMessage) <= MEMORY_POOL_BLOCK_DATA_SIZE,
    "AppMessage is larger than one pool block"
);
```

Cấp phát:

```c
MemoryPoolBlock *block;
AppMessage *message;

block = MemoryPool_Allocate(&app_pool);

if (block != NULL)
{
    message = (AppMessage *)block->data;

    message->signal = 10U;
    message->length = 3U;
    message->payload[0] = 0x11U;
    message->payload[1] = 0x22U;
    message->payload[2] = 0x33U;
}
```

Giải phóng:

```c
MemoryPool_Free(&app_pool, block);
```

---

## 7.3. Event-Driven System

Định nghĩa Event:

```c
typedef enum
{
    EVENT_NONE = 0,
    EVENT_BUTTON_PRESSED,
    EVENT_UART_RECEIVED,
    EVENT_TIMEOUT
} EventSignal;

typedef struct
{
    EventSignal signal;
    uint16_t length;
    uint8_t payload[24];
} Event;
```

Cấp phát Event:

```c
MemoryPoolBlock *block = MemoryPool_Allocate(&event_pool);

if (block != NULL)
{
    Event *event = (Event *)block->data;

    event->signal = EVENT_BUTTON_PRESSED;
    event->length = 0U;

    EventQueue_Push(block);
}
```

Task xử lý Event:

```c
void EventTask_Run(void)
{
    MemoryPoolBlock *block;

    block = EventQueue_Pop();

    if (block != NULL)
    {
        Event *event = (Event *)block->data;

        Event_Dispatch(event);

        MemoryPool_Free(&event_pool, block);
    }
}
```

Luồng xử lý:

```text
Producer
   |
   v
Allocate block from Pool
   |
   v
Fill Event data
   |
   v
Push block into Event Queue
   |
   v
Consumer receives block
   |
   v
Process Event
   |
   v
Return block to Pool
```

---

## 7.4. Packet Buffer

Ví dụ một packet nhỏ:

```c
typedef struct
{
    uint16_t length;
    uint8_t data[28];
} NetworkPacket;
```

Luồng nhận:

```text
Network ISR/DMA
      |
      v
Get packet block
      |
      v
Store received bytes
      |
      v
Pass block to protocol task
      |
      v
Parse and process packet
      |
      v
Return block to pool
```

Khi packet lớn hơn kích thước block, có thể:

- Tăng kích thước block.
- Dùng nhiều loại pool.
- Ghép nhiều block thành chuỗi.
- Chỉ lưu descriptor trong pool và payload ở buffer riêng.
- Dùng pool dành riêng cho packet lớn.

---

## 8. Phân tích mã nguồn

## 8.1. Khởi tạo free list

```c
for (index = 0U; index < capacity; index++)
{
    if ((index + 1U) < capacity)
    {
        storage[index].next = &storage[index + 1U];
    }
    else
    {
        storage[index].next = NULL;
    }
}
```

Các block được nối theo thứ tự:

```text
storage[0]
    |
    v
storage[1]
    |
    v
storage[2]
    |
    v
...
    |
    v
storage[capacity - 1]
    |
    v
NULL
```

Sau đó:

```c
pool->free_list = &storage[0];
```

---

## 8.2. Cấp phát một block

```c
allocated_block = pool->free_list;
pool->free_list = allocated_block->next;
```

Minh họa:

```text
Trước:

free_list
    |
    v
   B0 -> B1 -> B2 -> NULL
```

Lấy `B0`:

```text
allocated_block -> B0
```

Cập nhật danh sách:

```text
free_list
    |
    v
   B1 -> B2 -> NULL
```

Đây là thao tác xóa Node đầu của Singly Linked List.

---

## 8.3. Giải phóng một block

```c
block->next = pool->free_list;
pool->free_list = block;
```

Minh họa:

```text
Trước:

free_list
    |
    v
   B1 -> B2 -> NULL

block -> B0
```

Sau khi trả `B0`:

```text
free_list
    |
    v
   B0 -> B1 -> B2 -> NULL
```

Đây là thao tác thêm Node vào đầu Singly Linked List.

---

## 8.4. Tại sao thêm và xóa ở đầu free list?

Nếu luôn thao tác ở đầu danh sách:

```text
Allocate: remove front
Free:     insert front
```

Cả hai thao tác không cần duyệt toàn bộ danh sách.

Do đó thời gian cơ bản là:

```text
O(1)
```

---

## 8.5. Kiểm tra block thuộc pool

Không nên nhận mọi địa chỉ từ người dùng rồi đưa vào free list.

Ví dụ nguy hiểm:

```c
int local_variable;
MemoryPool_Free(&pool, (MemoryPoolBlock *)&local_variable);
```

Nếu không kiểm tra, free list có thể chứa địa chỉ không thuộc vùng storage.

Hàm `MemoryPool_Owns()` kiểm tra:

1. Địa chỉ nằm trong khoảng của mảng pool.
2. Địa chỉ trùng đúng vị trí bắt đầu của một block.

---

## 8.6. Phát hiện double free

Double free xảy ra khi cùng một block được trả lại pool nhiều lần:

```c
MemoryPool_Free(&pool, block);
MemoryPool_Free(&pool, block);
```

Nếu không phát hiện, free list có thể tạo cycle:

```text
B0 -> B1 -> B0 -> B1 -> ...
```

Code mẫu duyệt free list trước khi thêm block để phát hiện trường hợp đơn giản này.

Việc kiểm tra có độ phức tạp `O(n)`. Trong hệ thống yêu cầu hiệu năng cao, có thể thay bằng:

- Mảng trạng thái cho từng block.
- Bitmap allocation.
- Magic number.
- Debug build có kiểm tra, release build bỏ kiểm tra.
- Quy tắc ownership chặt chẽ.
- Wrapper object có trường state.

---

## 9. Ứng dụng trong hệ thống nhúng

## 9.1. Hệ thống thời gian thực

Memory Pool giúp hệ thống có một số lượng object đã được xác định trước.

Ví dụ:

```text
Pool có 32 Event
→ Hệ thống không thể có quá 32 Event đang tồn tại đồng thời
```

Giới hạn này cho phép:

- Ước lượng RAM từ lúc compile.
- Phát hiện rõ trường hợp quá tải.
- Tránh phụ thuộc vào trạng thái phân mảnh của Heap.
- Giảm biến thiên thời gian cấp phát.

Memory Pool thường kết hợp với:

- Active Object.
- Event Queue.
- RTOS Queue.
- Message Passing.
- Publish–Subscribe.
- State Machine.
- Timer Event.

---

## 9.2. Giao tiếp giữa các Task

Ví dụ:

```text
Task Sensor
    |
    | Allocate Message
    v
Message Pool
    |
    | Push pointer
    v
Message Queue
    |
    | Pop pointer
    v
Task Communication
    |
    | Process and Free
    v
Message Pool
```

Queue chỉ cần truyền con trỏ:

```c
MemoryPoolBlock *
```

Thay vì copy toàn bộ payload nhiều lần.

Cần đảm bảo chỉ một thành phần sở hữu block tại mỗi thời điểm.

---

## 9.3. Packet Buffer

Các giao thức truyền thông thường phải xử lý nhiều packet:

- Ethernet.
- Wi-Fi.
- BLE.
- TCP/IP.
- CAN Transport Protocol.
- UART framing.
- USB.
- SPI packet.

Buffer Pool có thể chuẩn bị trước một số packet buffer có kích thước cố định.

Ưu điểm:

- Tái sử dụng buffer.
- Không cần cấp phát Heap cho mỗi packet.
- Dễ đặt giới hạn số packet đang chờ.
- Dễ theo dõi mức sử dụng.

---

## 9.4. Driver và DMA

Một driver có thể sử dụng nhiều buffer:

```text
Buffer đang được DMA ghi
Buffer đang chờ xử lý
Buffer đang được ứng dụng đọc
Buffer đang trống
```

Memory Pool có thể quản lý các buffer trống.

Ví dụ:

```text
Free Pool -> DMA RX -> Processing Queue -> Application -> Free Pool
```

---

## 9.5. Object Pool

Nếu ứng dụng thường xuyên tạo cùng một loại object:

```c
typedef struct
{
    uint32_t timestamp;
    uint16_t id;
    uint8_t state;
} DeviceEvent;
```

Có thể tạo pool riêng:

```text
DeviceEvent Pool
```

Pool này chỉ chứa `DeviceEvent`, nhờ đó:

- Không cần ép kiểu payload.
- Kích thước mỗi block đúng bằng object.
- API rõ ràng hơn.
- Giảm lãng phí bộ nhớ.

---

## 9.6. Quản lý bộ nhớ tĩnh giống Heap

Từ ý tưởng:

```text
Static memory + free list
```

có thể xây dựng allocator phức tạp hơn:

- Nhiều pool với nhiều kích thước block.
- Fixed-block allocator.
- Slab allocator.
- Segregated free lists.
- Buddy allocator.
- TLSF.
- Region allocator.

Memory Pool đơn giản là nền tảng để hiểu các cơ chế quản lý bộ nhớ này.

---

## 10. Critical Section và truy cập đồng thời

Các thao tác sau thay đổi free list:

```c
pool->free_list = pool->free_list->next;
```

và:

```c
block->next = pool->free_list;
pool->free_list = block;
```

Nếu hai ngữ cảnh thực thi cùng thao tác tại một thời điểm, danh sách có thể bị hỏng.

Ví dụ:

- ISR và `main()`.
- Hai RTOS Task.
- Hai core.
- Một task và callback.
- Một producer và nhiều consumer.

---

## 10.1. Bare-metal Critical Section

Ví dụ khái quát:

```c
MemoryPoolBlock *MemoryPool_AllocateSafe(MemoryPool *pool)
{
    MemoryPoolBlock *block;

    uint32_t interrupt_state = Platform_EnterCritical();

    block = MemoryPool_Allocate(pool);

    Platform_ExitCritical(interrupt_state);

    return block;
}
```

Giải phóng:

```c
bool MemoryPool_FreeSafe(
    MemoryPool *pool,
    MemoryPoolBlock *block
)
{
    bool result;

    uint32_t interrupt_state = Platform_EnterCritical();

    result = MemoryPool_Free(pool, block);

    Platform_ExitCritical(interrupt_state);

    return result;
}
```

Critical section phải ngắn.

Không nên:

- Xóa dữ liệu lớn trong critical section.
- In log bằng UART trong critical section.
- Chờ mutex hoặc delay trong critical section.
- Thực hiện thuật toán dài trong khi interrupt bị khóa.

---

## 10.2. RTOS Mutex

Trong task context:

```c
Mutex_Lock(&pool_mutex);

block = MemoryPool_Allocate(&pool);

Mutex_Unlock(&pool_mutex);
```

Không nên dùng mutex thông thường trong ISR nếu RTOS không cho phép.

---

## 10.3. API dành cho ISR

Một số hệ thống thiết kế API riêng:

```c
MemoryPool_AllocateFromISR();
MemoryPool_FreeFromISR();
```

API này phải tuân thủ quy tắc của RTOS hoặc MCU đang sử dụng.

---

## 10.4. `volatile` không đủ

Khai báo:

```c
volatile MemoryPoolBlock *free_list;
```

không làm thao tác cập nhật linked list trở thành atomic.

`volatile` chỉ liên quan đến cách compiler truy cập biến. Nó không thay thế:

- Critical section.
- Mutex.
- Atomic operation.
- Spinlock.
- Quy tắc single-owner.

---

## 11. Giám sát Memory Pool

Một pool trong sản phẩm thực tế nên cung cấp thông tin thống kê.

### Số block trống

```c
MemoryPool_GetFreeCount(&pool);
```

### Số block đang dùng

```c
MemoryPool_GetUsedCount(&pool);
```

### Mức sử dụng cao nhất

```c
MemoryPool_GetHighWaterMark(&pool);
```

Ví dụ:

```text
capacity           = 16
minimum_free_count = 3
```

Mức sử dụng đồng thời cao nhất:

```text
16 - 3 = 13 block
```

Tỷ lệ sử dụng cao nhất:

```text
13 / 16 × 100% = 81.25%
```

---

## 11.1. Pool Exhaustion Counter

Có thể thêm:

```c
size_t allocation_failure_count;
```

Mỗi khi pool hết block:

```c
pool->allocation_failure_count++;
```

Thông tin này giúp xác định:

- Pool có quá nhỏ không.
- Consumer có xử lý quá chậm không.
- Có memory leak logic không.
- Hệ thống có bị burst dữ liệu không.

---

## 11.2. Allocation Counter

Có thể theo dõi:

```c
size_t total_allocation_count;
size_t total_free_count;
```

Nếu sau một chu kỳ hoạt động:

```text
total_allocation_count > total_free_count
```

không nhất thiết là lỗi vì có thể vẫn còn block đang sử dụng.

Nhưng phải thỏa mãn:

```text
used_count =
total_allocation_count - total_free_count
```

khi không có reset hoặc lỗi thống kê.

---

## 11.3. Debug State

Trong debug build, mỗi block có thể có trạng thái:

```c
typedef enum
{
    BLOCK_STATE_FREE = 0,
    BLOCK_STATE_ALLOCATED
} BlockState;
```

Hoặc magic number:

```c
#define BLOCK_MAGIC_FREE         0xFEEEFEEEU
#define BLOCK_MAGIC_ALLOCATED    0xA110CA7EU
```

Nhờ đó có thể phát hiện:

- Double free.
- Free block chưa từng cấp phát.
- Ghi tràn làm hỏng metadata.
- Block bị sử dụng sau khi đã trả pool.

---

## 12. Memory Pool và Heap

| Tiêu chí | Memory Pool | Heap với `malloc/free` |
|---|---|---|
| Vùng nhớ | Chuẩn bị trước | Quản lý động |
| Kích thước object | Thường cố định | Linh hoạt |
| Thời gian cấp phát | Dễ dự đoán | Có thể thay đổi |
| Phân mảnh ngoài | Hầu như không với fixed block | Có thể xảy ra |
| Giới hạn số object | Rõ ràng | Phụ thuộc Heap |
| Lãng phí nội bộ | Có thể có | Tùy allocator |
| Giải phóng | Trả block về pool | Trả vùng nhớ về Heap |
| Kiểm soát RAM | Cao | Khó hơn |
| Phù hợp ISR | Chỉ khi thiết kế an toàn | Thường không nên |
| Tính linh hoạt | Thấp hơn | Cao |
| Độ phức tạp triển khai | Thấp với fixed block | Đã có sẵn trong C Library |

---

## 12.1. Internal Fragmentation

Nếu mỗi block có 32 byte nhưng ứng dụng chỉ lưu 5 byte:

```text
Lãng phí trong block = 32 - 5 = 27 byte
```

Đây là **internal fragmentation**.

Có thể giảm bằng cách tạo nhiều pool:

```text
Pool nhỏ:  16 byte/block
Pool vừa:  32 byte/block
Pool lớn: 128 byte/block
```

Allocator chọn pool phù hợp với kích thước yêu cầu.

---

## 12.2. External Fragmentation

Với các block cố định và được tái sử dụng nguyên khối, Memory Pool không phải tìm vùng nhớ liên tiếp có kích thước thay đổi.

Do đó hiện tượng các lỗ trống nhỏ nằm rải rác như Heap thông thường được hạn chế đáng kể.

---

## 12.3. Khi nào vẫn nên dùng Heap?

Heap có thể phù hợp khi:

- Kích thước object thay đổi nhiều.
- Số object khó xác định trước.
- Hệ thống có nhiều RAM.
- Không có yêu cầu thời gian thực nghiêm ngặt.
- Thời gian cấp phát không phải vấn đề.
- Nền tảng có allocator đáng tin cậy.
- Ứng dụng chạy trên PC, Linux hoặc hệ điều hành đầy đủ.

Memory Pool và Heap không loại trừ lẫn nhau. Một hệ thống có thể dùng:

```text
Memory Pool cho object quan trọng/thời gian thực
Heap cho chức năng ít quan trọng hoặc khởi tạo
```

---

## 13. Độ phức tạp

Với free list đơn:

| Thao tác | Độ phức tạp |
|---|---:|
| Khởi tạo pool | `O(n)` |
| Cấp phát block | `O(1)` |
| Trả block | `O(1)` |
| Kiểm tra hết pool | `O(1)` |
| Lấy số block trống | `O(1)` |
| Lấy số block đang dùng | `O(1)` |
| Reset pool | `O(n)` |
| Kiểm tra double free bằng duyệt list | `O(n)` |
| Kiểm tra ownership theo địa chỉ | `O(1)` |

Trong đó `n` là số block trong pool.

### Bộ nhớ

Pool cần:

```text
capacity × sizeof(MemoryPoolBlock)
```

Ngoài dữ liệu ứng dụng, mỗi block cần metadata như con trỏ `next`.

Trên MCU 32-bit:

```text
sizeof(next) = 4 byte
```

Với payload 32 byte, một block có thể cần ít nhất:

```text
4 + 32 = 36 byte
```

Kích thước thực tế có thể lớn hơn do alignment.

---

## 14. Ưu điểm và hạn chế

## 14.1. Ưu điểm

- Bộ nhớ tối đa được biết trước.
- Thời gian cấp phát thường ngắn và dễ dự đoán.
- Cấp phát và giải phóng cơ bản là `O(1)`.
- Tái sử dụng object nhiều lần.
- Hạn chế phân mảnh ngoài.
- Phù hợp với message, event, packet và buffer.
- Dễ theo dõi số block trống và đang dùng.
- Có thể hoạt động mà không cần Heap.
- Có thể dùng trong bare-metal và RTOS.
- Thuận tiện cho thiết kế producer–consumer.

---

## 14.2. Hạn chế

- Số block tối đa cố định.
- Pool có thể hết khi hệ thống bị burst.
- Các block cùng kích thước có thể gây lãng phí RAM.
- Không phù hợp với object có kích thước quá khác nhau.
- Cần quản lý ownership rõ ràng.
- Có thể bị double free hoặc use-after-free.
- Cần bảo vệ khi nhiều ngữ cảnh truy cập.
- Việc chọn capacity quá nhỏ hoặc quá lớn đều gây vấn đề.
- Reset pool có thể làm vô hiệu các con trỏ đang được sử dụng.

---

## 15. Các lỗi thường gặp

## 15.1. Dereference khi Pool rỗng

Sai:

```c
MemoryPoolBlock *block = pool->free_list;

pool->free_list = block->next;
```

Nếu `free_list == NULL`, câu lệnh `block->next` gây lỗi.

Đúng:

```c
if (pool->free_list == NULL)
{
    return NULL;
}
```

---

## 15.2. Không kiểm tra kết quả cấp phát

Sai:

```c
MemoryPoolBlock *block = MemoryPool_Allocate(&pool);
block->data[0] = 10U;
```

Đúng:

```c
MemoryPoolBlock *block = MemoryPool_Allocate(&pool);

if (block != NULL)
{
    block->data[0] = 10U;
}
```

---

## 15.3. Double free

Sai:

```c
MemoryPool_Free(&pool, block);
MemoryPool_Free(&pool, block);
```

Một block chỉ được trả pool đúng một lần sau mỗi lần cấp phát.

Sau khi free, nên xóa con trỏ phía người dùng:

```c
MemoryPool_Free(&pool, block);
block = NULL;
```

---

## 15.4. Use-after-free

Sai:

```c
MemoryPool_Free(&pool, block);

block->data[0] = 0x55U;
```

Sau khi được trả pool, block có thể được cấp cho thành phần khác.

---

## 15.5. Free con trỏ không thuộc Pool

Sai:

```c
MemoryPoolBlock local_block;

MemoryPool_Free(&pool, &local_block);
```

Pool chỉ được nhận lại block do chính pool đó cấp.

---

## 15.6. Tràn payload

Sai:

```c
memcpy(block->data, source, 100U);
```

Trong khi:

```text
sizeof(block->data) = 32
```

Đúng:

```c
if (source_length <= sizeof(block->data))
{
    memcpy(block->data, source, source_length);
}
```

---

## 15.7. Quên trả block

```c
block = MemoryPool_Allocate(&pool);

/* Process data. */

/* Missing MemoryPool_Free(). */
```

Nếu lặp lại, pool sẽ dần hết block dù chương trình không dùng Heap.

Đây là một dạng resource leak.

---

## 15.8. Một block có nhiều owner

Nguy hiểm:

```text
Producer giữ block
Queue giữ block
Consumer giữ block
```

Nếu một thành phần giải phóng trong khi thành phần khác còn sử dụng, xảy ra use-after-free.

Nên quy định:

```text
Mỗi thời điểm chỉ có một owner chịu trách nhiệm giải phóng block
```

Hoặc dùng reference counting nếu thực sự cần chia sẻ.

---

## 15.9. Gọi reset khi còn block đang dùng

```c
MemoryPool_Reset(&pool);
```

sẽ đưa toàn bộ block về free list.

Mọi con trỏ đã cấp phát trước đó trở thành không hợp lệ về mặt ownership, dù địa chỉ vẫn còn tồn tại trong RAM.

Chỉ reset khi chắc chắn không còn người dùng nào giữ block.

---

## 16. Hướng mở rộng

## 16.1. Generic Memory Pool

Thay vì cố định kiểu `MemoryPoolBlock`, có thể nhận:

```text
storage pointer
block size
block count
```

Ví dụ API:

```c
bool GenericPool_Init(
    GenericPool *pool,
    void *storage,
    size_t block_size,
    size_t block_count
);
```

Điều kiện:

```text
block_size >= sizeof(void *)
```

vì mỗi block trống cần chứa liên kết `next`.

---

## 16.2. Nhiều kích thước block

```text
Small Pool  -> 16-byte blocks
Medium Pool -> 64-byte blocks
Large Pool  -> 256-byte blocks
```

Yêu cầu 40 byte sẽ chọn Medium Pool.

Cách này giảm internal fragmentation so với chỉ có một block size lớn.

---

## 16.3. Bitmap Pool

Thay vì free list, dùng bitmap:

```text
0 = free
1 = allocated
```

Ví dụ 8 block:

```text
Bitmap: 0 1 1 0 0 1 0 0
```

Ưu điểm:

- Dễ kiểm tra double free.
- Metadata tách khỏi payload.
- Dễ duyệt trạng thái.

Nhược điểm:

- Tìm block trống có thể phải quét bitmap.
- Cần tối ưu bằng bit operation nếu pool lớn.

---

## 16.4. Reference Counting

Nếu nhiều module cùng sử dụng một block:

```c
typedef struct
{
    uint16_t reference_count;
    uint8_t data[...];
} SharedBlock;
```

Khi chia sẻ:

```c
reference_count++;
```

Khi một owner dùng xong:

```c
reference_count--;
```

Chỉ trả pool khi:

```c
reference_count == 0
```

Cần đồng bộ nếu nhiều task thay đổi reference count.

---

## 16.5. Zero-copy Message Passing

Thay vì copy payload qua nhiều queue:

```text
Producer tạo block
→ Queue truyền con trỏ
→ Consumer đọc trực tiếp block
→ Consumer trả block
```

Điều này giảm:

- Số lần copy.
- Thời gian CPU.
- RAM tạm thời.

Đổi lại, ownership phải được quản lý rất rõ.

---

## 16.6. Typed Pool

Có thể tạo pool chuyên biệt:

```c
typedef struct EventBlock
{
    struct EventBlock *next;
    Event event;
} EventBlock;
```

Ưu điểm:

- Không cần ép kiểu `uint8_t data[]`.
- Compiler kiểm tra kiểu tốt hơn.
- Kích thước block đúng với object.
- API dễ đọc.

Ví dụ:

```c
Event *EventPool_Allocate(void);
void EventPool_Free(Event *event);
```

---

## 17. Tổng kết

Memory Pool là kỹ thuật chuẩn bị trước một vùng nhớ và tái sử dụng các block trong vùng nhớ đó.

Mô hình cơ bản:

```text
Static Array
    +
Singly Linked Free List
```

Khởi tạo:

```text
B0 -> B1 -> B2 -> ... -> BN -> NULL
```

Cấp phát:

```text
Lấy block đầu khỏi free list
```

Giải phóng:

```text
Đưa block trở lại đầu free list
```

Các thao tác cơ bản:

```text
Allocate: O(1)
Free:     O(1)
```

Memory Pool phù hợp với:

- Event-Driven Architecture.
- RTOS Message.
- Packet Buffer.
- DMA Buffer.
- Object Pool.
- Queue truyền con trỏ.
- Hệ thống yêu cầu bộ nhớ xác định trước.
- Ứng dụng không muốn phụ thuộc vào Heap trong run-time.

Khi thiết kế cần chú ý:

1. Xử lý khi pool hết block.
2. Kiểm tra kích thước dữ liệu.
3. Không dereference con trỏ `NULL`.
4. Không double free.
5. Không dùng block sau khi free.
6. Quy định ownership rõ ràng.
7. Bảo vệ free list bằng critical section hoặc mutex.
8. Theo dõi `free_count` và high-water mark.
9. Chọn số block và kích thước block phù hợp.
10. Không reset pool khi còn block đang được sử dụng.

---

## 18. Tài liệu tham khảo

README này được biên soạn và diễn đạt lại độc lập dựa trên chủ đề được trình bày trong bài:

- EPCB — **Data Structures - Memory Pool và ứng dụng**:  
  https://epcb.vn/blogs/ak-embedded-software/pool-memory-va-ung-dung
