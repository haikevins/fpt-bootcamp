# Memory Pool trong C và lập trình nhúng

Memory Pool là kỹ thuật quản lý bộ nhớ trong đó một vùng nhớ có kích thước xác định được chuẩn bị trước, chia thành nhiều block và tái sử dụng trong suốt thời gian chạy của chương trình.

Thay vì gọi `malloc()` mỗi khi cần tạo một Message, Event, Packet hoặc Buffer, chương trình lấy một block đang rảnh từ Pool. Khi dùng xong, block được trả lại Pool để phục vụ lần cấp phát tiếp theo.

```text
Khởi tạo:

free_list
    |
    v
+---------+    +---------+    +---------+    +---------+
| Block 0 | -> | Block 1 | -> | Block 2 | -> | Block 3 | -> NULL
+---------+    +---------+    +---------+    +---------+

Sau hai lần cấp phát:

Allocated: Block 0, Block 1

free_list
    |
    v
+---------+    +---------+
| Block 2 | -> | Block 3 | -> NULL
+---------+    +---------+

Sau khi trả Block 0:

free_list
    |
    v
+---------+    +---------+    +---------+
| Block 0 | -> | Block 2 | -> | Block 3 | -> NULL
+---------+    +---------+    +---------+
```

Memory Pool đặc biệt phù hợp với firmware, hệ thống Event-Driven và RTOS vì thời gian cấp phát có thể dự đoán, dung lượng tối đa được biết trước và không xảy ra phân mảnh ngoài bên trong Pool có kích thước block cố định.

---

## Mục lục

1. [Vấn đề của Heap trong hệ thống nhúng](#1-vấn-đề-của-heap-trong-hệ-thống-nhúng)
2. [Memory Pool là gì?](#2-memory-pool-là-gì)
3. [Cấu trúc của Fixed-Block Memory Pool](#3-cấu-trúc-của-fixed-block-memory-pool)
4. [Cơ chế hoạt động](#4-cơ-chế-hoạt-động)
5. [Độ phức tạp và đặc tính bộ nhớ](#5-độ-phức-tạp-và-đặc-tính-bộ-nhớ)
6. [Thiết kế API](#6-thiết-kế-api)
7. [Mã nguồn hoàn chỉnh](#7-mã-nguồn-hoàn-chỉnh)
8. [Ví dụ sử dụng cơ bản](#8-ví-dụ-sử-dụng-cơ-bản)
9. [Ứng dụng Event Pool](#9-ứng-dụng-event-pool)
10. [Ứng dụng Packet Buffer Pool](#10-ứng-dụng-packet-buffer-pool)
11. [Memory Pool cho Linked List](#11-memory-pool-cho-linked-list)
12. [Sử dụng với Interrupt và RTOS](#12-sử-dụng-với-interrupt-và-rtos)
13. [Alignment và kích thước block](#13-alignment-và-kích-thước-block)
14. [Fragmentation](#14-fragmentation)
15. [Các lỗi thường gặp](#15-các-lỗi-thường-gặp)
16. [Giám sát và kiểm thử](#16-giám-sát-và-kiểm-thử)
17. [Các biến thể của Memory Pool](#17-các-biến-thể-của-memory-pool)
18. [Khi nào nên và không nên dùng](#18-khi-nào-nên-và-không-nên-dùng)
19. [Cấu trúc repository đề xuất](#19-cấu-trúc-repository-đề-xuất)
20. [Tóm tắt](#20-tóm-tắt)

---

## 1. Vấn đề của Heap trong hệ thống nhúng

Trong C, bộ nhớ động thường được cấp phát và giải phóng bằng:

```c
void *memory = malloc(size);
free(memory);
```

Cách này thuận tiện nhưng có thể gây ra một số vấn đề trong firmware chạy lâu hoặc có yêu cầu thời gian thực.

### 1.1. Thời gian cấp phát không cố định

Allocator phải tìm vùng nhớ phù hợp, tách block, cập nhật metadata hoặc gộp các vùng trống. Thời gian thực thi có thể thay đổi tùy trạng thái Heap.

Điều này gây khó khăn khi cần xác định:

```text
Worst-Case Execution Time
```

### 1.2. Phân mảnh bộ nhớ

Sau nhiều lần cấp phát và giải phóng các block có kích thước khác nhau, Heap có thể còn đủ tổng số byte trống nhưng không có một vùng liên tục đủ lớn cho yêu cầu mới.

```text
Heap:

+------+----+--------+---+------+
| used |free|  used  |free| used |
+------+----+--------+---+------+
```

Tổng vùng trống có thể lớn nhưng bị chia thành nhiều đoạn nhỏ.

### 1.3. Khó dự đoán giới hạn sử dụng

Một lỗi logic có thể liên tục gọi `malloc()` mà không `free()`, dẫn đến Memory Leak và cuối cùng làm cạn Heap.

### 1.4. Khó dùng an toàn trong ISR

Nhiều allocator:

- Không reentrant.
- Dùng lock nội bộ.
- Có thời gian thực thi không xác định.
- Không được thiết kế để gọi trong Interrupt Service Routine.

### 1.5. Lỗi quản lý vòng đời

Các lỗi phổ biến gồm:

- Memory Leak.
- Double Free.
- Use-After-Free.
- Heap Corruption.
- Ghi vượt kích thước block.

Memory Pool không tự loại bỏ mọi lỗi trên, nhưng giới hạn rõ vùng nhớ và số block giúp hệ thống dễ kiểm soát hơn.

---

## 2. Memory Pool là gì?

Memory Pool chuẩn bị trước một vùng nhớ gồm nhiều block.

Ví dụ:

```text
Số block:       16
Payload/block:  32 byte
Tổng payload:   512 byte
```

Mỗi block có hai trạng thái logic:

```text
FREE    : đang nằm trong free list
IN USE  : đã được cấp cho ứng dụng
```

Các thao tác chính:

```c
memory_pool_init();
memory_pool_alloc();
memory_pool_free();
```

### Quy trình sử dụng

```text
1. Khởi tạo Pool
2. Lấy một block rảnh
3. Ghi dữ liệu vào block
4. Truyền quyền sở hữu cho thành phần xử lý
5. Trả block về Pool khi dùng xong
```

### Fixed-Block Memory Pool

Tài liệu này sử dụng loại đơn giản và phổ biến nhất:

```text
Tất cả block có cùng kích thước
```

Ưu điểm:

- Cấp phát `O(1)`.
- Giải phóng `O(1)`.
- Không cần tìm block theo kích thước.
- Không có phân mảnh ngoài bên trong Pool.
- Dễ dùng với mảng tĩnh.

Nhược điểm:

- Yêu cầu nhỏ vẫn chiếm toàn bộ một block.
- Không thể cấp phát object lớn hơn payload của block.
- Số object đồng thời bị giới hạn bởi số block.

---

## 3. Cấu trúc của Fixed-Block Memory Pool

Một Pool hoàn chỉnh trong tài liệu này gồm:

```c
typedef struct
{
    memory_pool_block_t blocks[MEMORY_POOL_BLOCK_COUNT];
    memory_pool_block_t *free_list;
    size_t free_count;
    size_t minimum_free_count;
    bool initialized;
    bool in_use[MEMORY_POOL_BLOCK_COUNT];
} memory_pool_t;
```

### `blocks`

Mảng tĩnh chứa toàn bộ block:

```c
memory_pool_block_t blocks[MEMORY_POOL_BLOCK_COUNT];
```

Vùng nhớ này được tạo ngay khi object Pool được tạo. Không cần gọi `malloc()` trong quá trình chạy.

### `free_list`

Trỏ đến block rảnh đầu tiên:

```c
memory_pool_block_t *free_list;
```

Các block rảnh được nối với nhau bằng Singly Linked List.

### `free_count`

Số block đang rảnh:

```c
size_t free_count;
```

Số block đang dùng:

```text
used_count = block_count - free_count
```

### `minimum_free_count`

Giá trị nhỏ nhất của `free_count` từng ghi nhận:

```c
size_t minimum_free_count;
```

Đây là High-Water Mark theo hướng sử dụng:

```text
maximum_used = block_count - minimum_free_count
```

Thông tin này giúp chọn lại kích thước Pool dựa trên dữ liệu thực tế.

### `in_use`

Mảng trạng thái dùng để:

- Phát hiện Double Free.
- Kiểm tra block có thực sự đang được cấp phát.
- Hỗ trợ debug.

Trong bản tối giản để tiết kiệm RAM, có thể loại bỏ trường này nếu hệ thống đã kiểm soát ownership chặt chẽ.

---

## 4. Cơ chế hoạt động

## 4.1. Khởi tạo

Mỗi block được nối đến block tiếp theo:

```c
blocks[0].next = &blocks[1];
blocks[1].next = &blocks[2];
blocks[2].next = &blocks[3];
blocks[3].next = NULL;
```

Sau đó:

```c
free_list = &blocks[0];
```

Sơ đồ:

```text
free_list
    |
    v
[0] -> [1] -> [2] -> [3] -> NULL
```

## 4.2. Cấp phát

Lấy block đầu free list:

```c
block = pool->free_list;
```

Dịch free list sang block tiếp theo:

```c
pool->free_list = block->next;
```

Tách block khỏi danh sách:

```c
block->next = NULL;
```

Sau một lần cấp phát:

```text
Allocated: [0]

free_list
    |
    v
[1] -> [2] -> [3] -> NULL
```

## 4.3. Giải phóng

Đưa block trở lại đầu free list:

```c
block->next = pool->free_list;
pool->free_list = block;
```

Ví dụ trả block `[0]`:

```text
free_list
    |
    v
[0] -> [1] -> [2] -> [3] -> NULL
```

Cả cấp phát và giải phóng đều chỉ thay đổi một số con trỏ cố định.

## 4.4. Thứ tự block sau nhiều lần sử dụng

Free list không cần giữ thứ tự tăng dần theo index.

```text
[5] -> [1] -> [12] -> [3] -> NULL
```

Điều quan trọng là:

- Mỗi block rảnh xuất hiện đúng một lần.
- Block đang sử dụng không nằm trong free list.
- Free list không có chu trình.
- `free_count` phản ánh đúng số block rảnh.

---

## 5. Độ phức tạp và đặc tính bộ nhớ

| Thao tác | Độ phức tạp |
|---|---:|
| Khởi tạo | `O(N)` |
| Cấp phát một block | `O(1)` |
| Giải phóng một block | `O(1)` |
| Kiểm tra số block rảnh | `O(1)` |
| Kiểm tra Pool rỗng/đầy | `O(1)` |
| Xóa payload khi cấp phát | `O(B)` |
| Kiểm tra toàn bộ Pool | `O(N)` |

Trong đó:

```text
N = số block
B = số byte payload của một block
```

Dung lượng tĩnh xấp xỉ:

```text
N × sizeof(memory_pool_block_t)
+ N × sizeof(bool)
+ metadata của Pool
```

Nếu bật xóa payload bằng `memset()` sau mỗi lần cấp phát, thao tác cấp phát không còn thuần `O(1)` theo số byte, mà là `O(B)`. Với block có kích thước cố định, thời gian vẫn có giới hạn trên rõ ràng.

---

## 6. Thiết kế API

API được chia thành các nhóm.

### Khởi tạo

```c
memory_pool_status_t memory_pool_init(memory_pool_t *pool);
```

### Cấp phát và giải phóng

```c
void *memory_pool_alloc(memory_pool_t *pool);
memory_pool_status_t memory_pool_free(memory_pool_t *pool, void *memory);
```

### Thông tin trạng thái

```c
size_t memory_pool_free_count(const memory_pool_t *pool);
size_t memory_pool_used_count(const memory_pool_t *pool);
size_t memory_pool_maximum_used(const memory_pool_t *pool);

bool memory_pool_is_empty(const memory_pool_t *pool);
bool memory_pool_is_full(const memory_pool_t *pool);
```

Quy ước:

```text
Pool empty: không còn block rảnh
Pool full:  tất cả block đều rảnh
```

Tên gọi này được xét theo “Pool chứa block rảnh”, không phải theo queue dữ liệu của ứng dụng.

### Mã trạng thái

```c
typedef enum
{
    MEMORY_POOL_OK = 0,
    MEMORY_POOL_INVALID_ARGUMENT,
    MEMORY_POOL_NOT_INITIALIZED,
    MEMORY_POOL_EXHAUSTED,
    MEMORY_POOL_POINTER_OUT_OF_RANGE,
    MEMORY_POOL_POINTER_MISALIGNED,
    MEMORY_POOL_DOUBLE_FREE
} memory_pool_status_t;
```

---

## 7. Mã nguồn hoàn chỉnh

### `memory_pool.h`

```c
#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*
 * Adjust these values for the application.
 */
#define MEMORY_POOL_BLOCK_COUNT   16U
#define MEMORY_POOL_PAYLOAD_SIZE  32U

typedef enum
{
    MEMORY_POOL_OK = 0,
    MEMORY_POOL_INVALID_ARGUMENT,
    MEMORY_POOL_NOT_INITIALIZED,
    MEMORY_POOL_EXHAUSTED,
    MEMORY_POOL_POINTER_OUT_OF_RANGE,
    MEMORY_POOL_POINTER_MISALIGNED,
    MEMORY_POOL_DOUBLE_FREE
} memory_pool_status_t;

/*
 * max_align_t ensures the returned payload is suitably aligned for
 * ordinary scalar types and structures supported by the implementation.
 */
typedef union
{
    max_align_t alignment;
    uint8_t bytes[MEMORY_POOL_PAYLOAD_SIZE];
} memory_pool_payload_t;

typedef struct memory_pool_block
{
    struct memory_pool_block *next;
    memory_pool_payload_t payload;
} memory_pool_block_t;

typedef struct
{
    memory_pool_block_t blocks[MEMORY_POOL_BLOCK_COUNT];
    memory_pool_block_t *free_list;

    size_t free_count;
    size_t minimum_free_count;

    bool initialized;
    bool in_use[MEMORY_POOL_BLOCK_COUNT];
} memory_pool_t;

memory_pool_status_t memory_pool_init(memory_pool_t *pool);

void *memory_pool_alloc(memory_pool_t *pool);

memory_pool_status_t memory_pool_free(memory_pool_t *pool,
                                      void *memory);

size_t memory_pool_capacity(const memory_pool_t *pool);
size_t memory_pool_free_count(const memory_pool_t *pool);
size_t memory_pool_used_count(const memory_pool_t *pool);
size_t memory_pool_maximum_used(const memory_pool_t *pool);

bool memory_pool_is_empty(const memory_pool_t *pool);
bool memory_pool_is_full(const memory_pool_t *pool);

bool memory_pool_owns(const memory_pool_t *pool,
                      const void *memory);

#endif
```

### `memory_pool.c`

```c
#include "memory_pool.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

static bool memory_pool_is_valid(const memory_pool_t *pool)
{
    return (pool != NULL) && pool->initialized;
}

static memory_pool_status_t memory_pool_get_block_index(
    const memory_pool_t *pool,
    const void *memory,
    size_t *index)
{
    const uintptr_t pool_begin =
        (uintptr_t)(const void *)&pool->blocks[0];

    const uintptr_t pool_end =
        (uintptr_t)(const void *)&pool->blocks[MEMORY_POOL_BLOCK_COUNT];

    const uintptr_t payload_address =
        (uintptr_t)memory;

    const size_t payload_offset =
        offsetof(memory_pool_block_t, payload);

    uintptr_t block_address;
    uintptr_t offset;

    if ((pool == NULL) || (memory == NULL) || (index == NULL))
    {
        return MEMORY_POOL_INVALID_ARGUMENT;
    }

    if (payload_address < payload_offset)
    {
        return MEMORY_POOL_POINTER_OUT_OF_RANGE;
    }

    block_address = payload_address - payload_offset;

    if ((block_address < pool_begin) || (block_address >= pool_end))
    {
        return MEMORY_POOL_POINTER_OUT_OF_RANGE;
    }

    offset = block_address - pool_begin;

    if ((offset % sizeof(memory_pool_block_t)) != 0U)
    {
        return MEMORY_POOL_POINTER_MISALIGNED;
    }

    *index = (size_t)(offset / sizeof(memory_pool_block_t));

    if (*index >= MEMORY_POOL_BLOCK_COUNT)
    {
        return MEMORY_POOL_POINTER_OUT_OF_RANGE;
    }

    /*
     * Only a pointer to the beginning of payload is accepted.
     */
    if (memory != (const void *)&pool->blocks[*index].payload)
    {
        return MEMORY_POOL_POINTER_MISALIGNED;
    }

    return MEMORY_POOL_OK;
}

memory_pool_status_t memory_pool_init(memory_pool_t *pool)
{
    size_t index;

    if (pool == NULL)
    {
        return MEMORY_POOL_INVALID_ARGUMENT;
    }

    for (index = 0U; index < MEMORY_POOL_BLOCK_COUNT; index++)
    {
        if (index < (MEMORY_POOL_BLOCK_COUNT - 1U))
        {
            pool->blocks[index].next = &pool->blocks[index + 1U];
        }
        else
        {
            pool->blocks[index].next = NULL;
        }

        pool->in_use[index] = false;
    }

    pool->free_list = &pool->blocks[0];
    pool->free_count = MEMORY_POOL_BLOCK_COUNT;
    pool->minimum_free_count = MEMORY_POOL_BLOCK_COUNT;
    pool->initialized = true;

    return MEMORY_POOL_OK;
}

void *memory_pool_alloc(memory_pool_t *pool)
{
    memory_pool_block_t *block;
    size_t index;

    if (!memory_pool_is_valid(pool))
    {
        return NULL;
    }

    block = pool->free_list;

    if (block == NULL)
    {
        return NULL;
    }

    pool->free_list = block->next;
    block->next = NULL;

    index = (size_t)(block - &pool->blocks[0]);
    pool->in_use[index] = true;

    pool->free_count--;

    if (pool->free_count < pool->minimum_free_count)
    {
        pool->minimum_free_count = pool->free_count;
    }

    /*
     * Optional: clear old data before giving the block to the caller.
     * Remove this memset if allocation latency is more important.
     */
    (void)memset(block->payload.bytes,
                 0,
                 sizeof(block->payload.bytes));

    return (void *)&block->payload;
}

memory_pool_status_t memory_pool_free(memory_pool_t *pool,
                                      void *memory)
{
    memory_pool_status_t status;
    memory_pool_block_t *block;
    size_t index;

    if (pool == NULL)
    {
        return MEMORY_POOL_INVALID_ARGUMENT;
    }

    if (!pool->initialized)
    {
        return MEMORY_POOL_NOT_INITIALIZED;
    }

    status = memory_pool_get_block_index(pool, memory, &index);

    if (status != MEMORY_POOL_OK)
    {
        return status;
    }

    if (!pool->in_use[index])
    {
        return MEMORY_POOL_DOUBLE_FREE;
    }

    block = &pool->blocks[index];

    pool->in_use[index] = false;

    block->next = pool->free_list;
    pool->free_list = block;

    pool->free_count++;

    return MEMORY_POOL_OK;
}

size_t memory_pool_capacity(const memory_pool_t *pool)
{
    return memory_pool_is_valid(pool)
               ? MEMORY_POOL_BLOCK_COUNT
               : 0U;
}

size_t memory_pool_free_count(const memory_pool_t *pool)
{
    return memory_pool_is_valid(pool)
               ? pool->free_count
               : 0U;
}

size_t memory_pool_used_count(const memory_pool_t *pool)
{
    return memory_pool_is_valid(pool)
               ? (MEMORY_POOL_BLOCK_COUNT - pool->free_count)
               : 0U;
}

size_t memory_pool_maximum_used(const memory_pool_t *pool)
{
    return memory_pool_is_valid(pool)
               ? (MEMORY_POOL_BLOCK_COUNT - pool->minimum_free_count)
               : 0U;
}

bool memory_pool_is_empty(const memory_pool_t *pool)
{
    return memory_pool_is_valid(pool) &&
           (pool->free_count == 0U);
}

bool memory_pool_is_full(const memory_pool_t *pool)
{
    return memory_pool_is_valid(pool) &&
           (pool->free_count == MEMORY_POOL_BLOCK_COUNT);
}

bool memory_pool_owns(const memory_pool_t *pool,
                      const void *memory)
{
    size_t index;

    if (!memory_pool_is_valid(pool))
    {
        return false;
    }

    return memory_pool_get_block_index(pool,
                                       memory,
                                       &index) == MEMORY_POOL_OK;
}
```

### Điểm khác biệt so với bản tối giản

Bản triển khai trên bổ sung:

- Kiểm tra Pool đã khởi tạo.
- Kiểm tra con trỏ có thuộc Pool hay không.
- Kiểm tra con trỏ có trỏ đúng đầu payload hay không.
- Phát hiện Double Free.
- Theo dõi số block đang dùng.
- Theo dõi mức sử dụng lớn nhất.
- Bảo đảm alignment bằng `max_align_t`.
- Xóa dữ liệu cũ trước khi trả block cho caller.

Trong bản production tối ưu RAM, có thể tắt một số cơ chế debug bằng macro biên dịch.

---

## 8. Ví dụ sử dụng cơ bản

### `main.c`

```c
#include <stdio.h>
#include <string.h>

#include "memory_pool.h"

typedef struct
{
    uint16_t id;
    uint16_t length;
    uint8_t data[16];
} message_t;

int main(void)
{
    memory_pool_t pool;
    message_t *message;
    memory_pool_status_t status;

    status = memory_pool_init(&pool);

    if (status != MEMORY_POOL_OK)
    {
        return 1;
    }

    /*
     * Compile-time protection: the object must fit in one pool block.
     */
    _Static_assert(sizeof(message_t) <= MEMORY_POOL_PAYLOAD_SIZE,
                   "message_t is larger than one pool block");

    message = (message_t *)memory_pool_alloc(&pool);

    if (message == NULL)
    {
        printf("Pool exhausted\n");
        return 1;
    }

    message->id = 100U;
    message->length = 5U;
    (void)memcpy(message->data, "HELLO", message->length);

    printf("Message ID: %u\n", (unsigned int)message->id);
    printf("Free blocks: %zu\n", memory_pool_free_count(&pool));
    printf("Used blocks: %zu\n", memory_pool_used_count(&pool));

    status = memory_pool_free(&pool, message);

    if (status != MEMORY_POOL_OK)
    {
        printf("Free failed: %d\n", (int)status);
        return 1;
    }

    printf("Free blocks after release: %zu\n",
           memory_pool_free_count(&pool));

    return 0;
}
```

### Kết quả dự kiến

```text
Message ID: 100
Free blocks: 15
Used blocks: 1
Free blocks after release: 16
```

### Biên dịch bằng GCC

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic \
    src/memory_pool.c examples/basic/main.c \
    -Iinc -o memory_pool_demo
```

---

## 9. Ứng dụng Event Pool

Trong kiến trúc Event-Driven, một producer tạo Event và gửi cho consumer xử lý.

```text
Button ISR / UART ISR / Timer
             |
             | allocate event
             v
       +-------------+
       | Event Pool  |
       +-------------+
             |
             | post pointer
             v
       +-------------+
       | Event Queue |
       +-------------+
             |
             | get event
             v
       +-------------+
       | Active      |
       | Object/Task |
       +-------------+
             |
             | free event
             v
       +-------------+
       | Event Pool  |
       +-------------+
```

### Định nghĩa Event

```c
typedef enum
{
    EVENT_NONE = 0,
    EVENT_BUTTON_PRESSED,
    EVENT_UART_RECEIVED,
    EVENT_TIMEOUT
} event_signal_t;

typedef struct
{
    event_signal_t signal;
    uint32_t parameter;
} event_t;
```

Kiểm tra kích thước:

```c
_Static_assert(sizeof(event_t) <= MEMORY_POOL_PAYLOAD_SIZE,
               "event_t does not fit in the pool");
```

### Tạo Event

```c
event_t *event_create(memory_pool_t *pool,
                      event_signal_t signal,
                      uint32_t parameter)
{
    event_t *event = (event_t *)memory_pool_alloc(pool);

    if (event == NULL)
    {
        return NULL;
    }

    event->signal = signal;
    event->parameter = parameter;

    return event;
}
```

### Xử lý và trả Event

```c
void event_dispatch(memory_pool_t *pool, event_t *event)
{
    if ((pool == NULL) || (event == NULL))
    {
        return;
    }

    switch (event->signal)
    {
        case EVENT_BUTTON_PRESSED:
            app_handle_button(event->parameter);
            break;

        case EVENT_UART_RECEIVED:
            app_handle_uart(event->parameter);
            break;

        case EVENT_TIMEOUT:
            app_handle_timeout(event->parameter);
            break;

        default:
            break;
    }

    (void)memory_pool_free(pool, event);
}
```

### Ownership của Event

Phải xác định rõ thành phần nào có trách nhiệm giải phóng.

Một quy tắc đơn giản:

```text
Producer sở hữu Event trước khi post.
Queue/Consumer sở hữu Event sau khi post thành công.
Consumer trả Event về Pool sau khi xử lý.
```

Nếu post thất bại:

```c
event_t *event = event_create(&event_pool,
                              EVENT_BUTTON_PRESSED,
                              button_id);

if (event != NULL)
{
    if (!event_queue_post(event))
    {
        /*
         * Queue did not accept ownership.
         */
        (void)memory_pool_free(&event_pool, event);
    }
}
```

Không rõ ownership là nguyên nhân phổ biến gây Memory Leak hoặc Double Free.

---

## 10. Ứng dụng Packet Buffer Pool

Các stack Ethernet, Wi-Fi, BLE, CAN gateway hoặc TCP/IP thường cần nhiều buffer có vòng đời ngắn.

```c
typedef struct
{
    uint16_t length;
    uint16_t interface_id;
    uint8_t payload[28];
} packet_t;
```

Nếu payload lớn hơn block của Pool hiện tại, cần:

- Tăng `MEMORY_POOL_PAYLOAD_SIZE`.
- Tạo Pool riêng cho Packet.
- Dùng nhiều size class.
- Dùng chain nhiều block.
- Chỉ cấp phát descriptor, còn payload nằm trong DMA buffer khác.

### Ví dụ Packet Pool riêng

```c
#define PACKET_POOL_COUNT         8U
#define PACKET_PAYLOAD_CAPACITY   128U

typedef struct packet_block
{
    struct packet_block *next;

    uint16_t length;
    uint8_t payload[PACKET_PAYLOAD_CAPACITY];
} packet_block_t;

typedef struct
{
    packet_block_t storage[PACKET_POOL_COUNT];
    packet_block_t *free_list;
} packet_pool_t;
```

### Luồng nhận packet

```text
Peripheral/DMA
      |
      v
Allocate packet block
      |
      v
Copy or reference payload
      |
      v
Post packet pointer to protocol task
      |
      v
Process packet
      |
      v
Return packet block to Pool
```

### Khi Pool hết block

Các chiến lược có thể dùng:

- Drop packet mới.
- Drop packet cũ ít quan trọng.
- Tăng bộ đếm overflow.
- Tạm dừng RX nếu phần cứng hỗ trợ flow control.
- Báo lỗi hệ thống.
- Dùng backpressure.
- Tăng Pool sau khi đo High-Water Mark.

Không nên âm thầm tiếp tục dereference con trỏ `NULL`.

---

## 11. Memory Pool cho Linked List

Memory Pool thường được dùng để cấp node cho Linked List mà không cần Heap.

```c
typedef struct Node
{
    int data;
    struct Node *next;
} Node;
```

Kiểm tra kích thước:

```c
_Static_assert(sizeof(Node) <= MEMORY_POOL_PAYLOAD_SIZE,
               "Node does not fit in the pool");
```

### Tạo node

```c
Node *node_create(memory_pool_t *pool, int value)
{
    Node *node = (Node *)memory_pool_alloc(pool);

    if (node == NULL)
    {
        return NULL;
    }

    node->data = value;
    node->next = NULL;

    return node;
}
```

### Xóa node

```c
memory_pool_status_t node_destroy(memory_pool_t *pool,
                                  Node *node)
{
    return memory_pool_free(pool, node);
}
```

### Lợi ích

- Không phân mảnh Heap.
- Giới hạn node tối đa rõ ràng.
- `allocate/free` trong `O(1)`.
- Dễ theo dõi số node còn lại.
- Phù hợp Event Queue, Timer List và danh sách Packet Descriptor.

---

## 12. Sử dụng với Interrupt và RTOS

Memory Pool trong ví dụ không tự thread-safe.

Các trường bị thay đổi trong một lần cấp phát:

```text
free_list
in_use[index]
free_count
minimum_free_count
```

Nếu hai execution context cùng thao tác, free list có thể bị hỏng.

## 12.1. Critical Section

Ví dụ khái niệm:

```c
void *memory_pool_alloc_safe(memory_pool_t *pool)
{
    void *memory;

    ENTER_CRITICAL_SECTION();
    memory = memory_pool_alloc(pool);
    EXIT_CRITICAL_SECTION();

    return memory;
}
```

```c
memory_pool_status_t memory_pool_free_safe(memory_pool_t *pool,
                                           void *memory)
{
    memory_pool_status_t status;

    ENTER_CRITICAL_SECTION();
    status = memory_pool_free(pool, memory);
    EXIT_CRITICAL_SECTION();

    return status;
}
```

Critical section chỉ nên bao quanh thao tác trên Pool.

Không thực hiện bên trong critical section:

- `printf()`.
- Delay.
- Truyền UART blocking.
- Xử lý packet dài.
- Gọi callback không kiểm soát.
- Chờ semaphore.

## 12.2. ISR cấp phát, Task giải phóng

Mô hình này có hai context cùng thay đổi free list, nên vẫn cần đồng bộ phù hợp.

Các lựa chọn:

- Tắt ngắt trong một đoạn rất ngắn.
- Dùng primitive `FromISR` của RTOS.
- Dùng Pool riêng cho ISR.
- Dùng Single-Producer/Single-Consumer Queue để chuyển index.
- Thiết kế lock-free có memory ordering rõ ràng.

## 12.3. Với FreeRTOS

Có thể cân nhắc các cơ chế có sẵn:

- Static Queue.
- Queue.
- Stream Buffer.
- Message Buffer.
- Fixed-size object pool do ứng dụng quản lý.
- Heap scheme phù hợp của FreeRTOS.

Memory Pool tự viết hữu ích khi:

- Muốn cấp phát object cố định.
- Muốn kiểm soát chính xác layout.
- Muốn đo mức sử dụng.
- Muốn tránh copy payload lớn.
- Muốn dùng cùng API cả khi có và không có RTOS.

## 12.4. `volatile` không thay thế đồng bộ

`volatile` không tự tạo:

- Atomicity.
- Critical Section.
- Mutex.
- Memory ownership.
- Memory barrier đầy đủ.
- Bảo vệ khỏi Double Free.

Không nên chỉ thêm `volatile` vào `free_list` và kết luận Pool đã an toàn.

---

## 13. Alignment và kích thước block

## 13.1. Tại sao alignment quan trọng?

Một object như:

```c
typedef struct
{
    uint32_t timestamp;
    double value;
} sample_t;
```

có thể yêu cầu địa chỉ được căn chỉnh theo 4, 8 hoặc số byte khác tùy kiến trúc.

Nếu trả về một địa chỉ không đủ alignment, hệ thống có thể:

- Chạy chậm hơn.
- Gây Bus Fault.
- Có hành vi không xác định theo C.
- Không thể dùng cho DMA.

Bản triển khai dùng:

```c
typedef union
{
    max_align_t alignment;
    uint8_t bytes[MEMORY_POOL_PAYLOAD_SIZE];
} memory_pool_payload_t;
```

để payload được căn chỉnh phù hợp với các kiểu dữ liệu thông thường.

## 13.2. Kiểm tra object có vừa block

Dùng `_Static_assert`:

```c
_Static_assert(sizeof(event_t) <= MEMORY_POOL_PAYLOAD_SIZE,
               "event_t is too large");
```

Nếu object có yêu cầu alignment đặc biệt, cũng cần kiểm tra:

```c
_Static_assert(_Alignof(event_t) <= _Alignof(memory_pool_payload_t),
               "event_t alignment is not supported");
```

## 13.3. Alignment cho DMA

DMA có thể yêu cầu:

- Địa chỉ căn 4, 8, 16 hoặc 32 byte.
- Buffer nằm trong vùng RAM cụ thể.
- Không nằm trong CCM/DTCM tùy MCU.
- Cache maintenance trên Cortex-M7 hoặc MCU có D-Cache.
- Section linker riêng.

Ví dụ GCC:

```c
__attribute__((aligned(32)))
static memory_pool_t dma_pool;
```

Hoặc đặt vào section:

```c
__attribute__((section(".dma_buffer")))
static memory_pool_t dma_pool;
```

Yêu cầu cụ thể phụ thuộc MCU, DMA và linker script.

---

## 14. Fragmentation

## 14.1. External Fragmentation

Fixed-block Pool không có phân mảnh ngoài bên trong Pool vì mọi block có cùng kích thước.

Bất kỳ block rảnh nào cũng đáp ứng một yêu cầu hợp lệ.

## 14.2. Internal Fragmentation

Nếu payload là 32 byte nhưng object chỉ cần 5 byte:

```text
Sử dụng: 5 byte
Lãng phí nội bộ: 27 byte
```

Đây là Internal Fragmentation.

## 14.3. Giảm Internal Fragmentation bằng Size Class

Có thể tạo nhiều Pool:

```text
Pool nhỏ:   16 byte
Pool vừa:   32 byte
Pool lớn:  128 byte
```

Yêu cầu được chuyển đến Pool nhỏ nhất đủ chứa object.

```text
5 byte   -> Pool 16
24 byte  -> Pool 32
90 byte  -> Pool 128
```

Cách này gần với Slab Allocator hoặc Segregated Free Lists.

Đổi lại:

- Code phức tạp hơn.
- Có thể một Pool hết block trong khi Pool khác còn trống.
- Cần chính sách chọn Pool.
- Cần lưu thông tin Pool sở hữu block.

---

## 15. Các lỗi thường gặp

## 15.1. Không xử lý khi Pool cạn

Sai:

```c
event_t *event = memory_pool_alloc(&pool);
event->signal = EVENT_TIMEOUT;
```

Nếu Pool hết block, `event == NULL`.

Đúng:

```c
event_t *event = memory_pool_alloc(&pool);

if (event == NULL)
{
    handle_pool_exhaustion();
    return;
}
```

## 15.2. Trả con trỏ không thuộc Pool

```c
int local_value;
memory_pool_free(&pool, &local_value);
```

Bản triển khai an toàn trả về:

```text
MEMORY_POOL_POINTER_OUT_OF_RANGE
```

## 15.3. Trả con trỏ nằm giữa payload

Sai:

```c
uint8_t *memory = memory_pool_alloc(&pool);
memory_pool_free(&pool, &memory[4]);
```

Chỉ con trỏ đúng đầu payload mới hợp lệ.

## 15.4. Double Free

```c
memory_pool_free(&pool, event);
memory_pool_free(&pool, event);
```

Nếu không phát hiện, cùng một block có thể xuất hiện hai lần trong free list:

```text
Block A -> ... -> Block A -> ...
```

Điều này có thể làm hai lần cấp phát sau trả về cùng một vùng nhớ.

## 15.5. Use-After-Free

Sai:

```c
memory_pool_free(&pool, event);
event->signal = EVENT_TIMEOUT;
```

Sau `free`, quyền sở hữu đã trả cho Pool. Block có thể được cấp cho thành phần khác.

## 15.6. Object lớn hơn block

Ép kiểu không làm object tự vừa vào block.

```c
large_packet_t *packet =
    (large_packet_t *)memory_pool_alloc(&pool);
```

Nếu `sizeof(large_packet_t) > MEMORY_POOL_PAYLOAD_SIZE`, thao tác ghi sẽ phá hỏng block kế tiếp hoặc metadata.

Luôn dùng `_Static_assert`.

## 15.7. Không đồng bộ giữa ISR và Task

Free list là Linked List dùng chung. Cập nhật không nguyên tử có thể làm mất block hoặc tạo chu trình.

## 15.8. Khởi tạo lại khi còn object đang dùng

```c
memory_pool_init(&pool);
```

trong khi caller vẫn giữ con trỏ cũ sẽ đưa mọi block về trạng thái free, tạo alias và hỏng ownership.

Chỉ reinitialize Pool khi chắc chắn không còn object đang dùng.

## 15.9. Ghi vượt payload

Memory Pool chỉ quản lý block, không tự biết caller ghi bao nhiêu byte.

```c
memcpy(memory, source, length);
```

phải bảo đảm:

```c
length <= MEMORY_POOL_PAYLOAD_SIZE
```

## 15.10. Xóa dữ liệu nhạy cảm không đầy đủ

Nếu block chứa key hoặc credential, có thể cần xóa dữ liệu khi giải phóng thay vì khi cấp phát.

Lưu ý compiler có thể tối ưu bỏ một số lần `memset()` nếu kết quả không được quan sát. Với dữ liệu bảo mật, dùng hàm xóa an toàn do nền tảng cung cấp.

---

## 16. Giám sát và kiểm thử

## 16.1. Các chỉ số nên theo dõi

- Tổng số block.
- Số block đang rảnh.
- Số block đang dùng.
- Mức sử dụng lớn nhất.
- Số lần cấp phát thất bại.
- Số Double Free.
- Số con trỏ không hợp lệ.
- Thời gian giữ block trung bình nếu cần.
- Owner hiện tại trong bản debug.

Có thể bổ sung:

```c
typedef struct
{
    size_t allocation_count;
    size_t free_count_total;
    size_t allocation_failure_count;
    size_t invalid_free_count;
    size_t double_free_count;
} memory_pool_statistics_t;
```

## 16.2. Unit Test

### `test_memory_pool.c`

```c
#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include "memory_pool.h"

static void test_initial_state(void)
{
    memory_pool_t pool;

    assert(memory_pool_init(&pool) == MEMORY_POOL_OK);
    assert(memory_pool_capacity(&pool) == MEMORY_POOL_BLOCK_COUNT);
    assert(memory_pool_free_count(&pool) == MEMORY_POOL_BLOCK_COUNT);
    assert(memory_pool_used_count(&pool) == 0U);
    assert(memory_pool_is_full(&pool));
    assert(!memory_pool_is_empty(&pool));
}

static void test_allocate_and_free(void)
{
    memory_pool_t pool;
    void *memory;

    assert(memory_pool_init(&pool) == MEMORY_POOL_OK);

    memory = memory_pool_alloc(&pool);

    assert(memory != NULL);
    assert(memory_pool_owns(&pool, memory));
    assert(memory_pool_free_count(&pool) ==
           (MEMORY_POOL_BLOCK_COUNT - 1U));
    assert(memory_pool_used_count(&pool) == 1U);

    assert(memory_pool_free(&pool, memory) == MEMORY_POOL_OK);
    assert(memory_pool_free_count(&pool) == MEMORY_POOL_BLOCK_COUNT);
}

static void test_exhaustion(void)
{
    memory_pool_t pool;
    void *objects[MEMORY_POOL_BLOCK_COUNT];
    size_t index;

    assert(memory_pool_init(&pool) == MEMORY_POOL_OK);

    for (index = 0U; index < MEMORY_POOL_BLOCK_COUNT; index++)
    {
        objects[index] = memory_pool_alloc(&pool);
        assert(objects[index] != NULL);
    }

    assert(memory_pool_is_empty(&pool));
    assert(memory_pool_alloc(&pool) == NULL);
    assert(memory_pool_maximum_used(&pool) ==
           MEMORY_POOL_BLOCK_COUNT);

    for (index = 0U; index < MEMORY_POOL_BLOCK_COUNT; index++)
    {
        assert(memory_pool_free(&pool, objects[index]) ==
               MEMORY_POOL_OK);
    }

    assert(memory_pool_is_full(&pool));
}

static void test_double_free(void)
{
    memory_pool_t pool;
    void *memory;

    assert(memory_pool_init(&pool) == MEMORY_POOL_OK);

    memory = memory_pool_alloc(&pool);
    assert(memory != NULL);

    assert(memory_pool_free(&pool, memory) == MEMORY_POOL_OK);
    assert(memory_pool_free(&pool, memory) ==
           MEMORY_POOL_DOUBLE_FREE);
}

static void test_invalid_pointer(void)
{
    memory_pool_t pool;
    uint32_t external_object = 0U;

    assert(memory_pool_init(&pool) == MEMORY_POOL_OK);

    assert(memory_pool_free(&pool, &external_object) ==
           MEMORY_POOL_POINTER_OUT_OF_RANGE);
}

static void test_reuse(void)
{
    memory_pool_t pool;
    void *first;
    void *second;

    assert(memory_pool_init(&pool) == MEMORY_POOL_OK);

    first = memory_pool_alloc(&pool);
    assert(first != NULL);

    assert(memory_pool_free(&pool, first) == MEMORY_POOL_OK);

    second = memory_pool_alloc(&pool);
    assert(second == first);
}

int main(void)
{
    test_initial_state();
    test_allocate_and_free();
    test_exhaustion();
    test_double_free();
    test_invalid_pointer();
    test_reuse();

    return 0;
}
```

### Biên dịch test

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic \
    -fsanitize=address,undefined \
    src/memory_pool.c tests/test_memory_pool.c \
    -Iinc -o memory_pool_tests
```

Chạy:

```bash
./memory_pool_tests
```

## 16.3. Các trường hợp cần kiểm thử thêm

1. Khởi tạo với `NULL`.
2. Cấp phát trước khi khởi tạo.
3. Giải phóng `NULL`.
4. Cấp phát hết toàn bộ Pool.
5. Giải phóng theo thứ tự khác thứ tự cấp phát.
6. Cấp phát và giải phóng hàng nghìn vòng.
7. Double Free.
8. Con trỏ ngoài Pool.
9. Con trỏ nằm giữa payload.
10. Alignment của object.
11. High-Water Mark.
12. Truy cập đồng thời giữa các context.
13. Pool reset khi còn object đang dùng.
14. Payload lớn nhất bằng đúng block size.
15. Build trên MCU 32-bit và host 64-bit.

---

## 17. Các biến thể của Memory Pool

## 17.1. Object Pool

Mỗi block chứa đúng một kiểu object:

```text
Event Pool
Timer Pool
Packet Pool
Node Pool
```

Ưu điểm:

- API rõ ràng.
- Không cần lưu kích thước yêu cầu.
- Có thể gọi constructor/init riêng.
- Dễ kiểm tra kiểu trong bản debug.

## 17.2. Buffer Pool

Mỗi block là một buffer byte có cùng kích thước.

Ứng dụng:

- UART frame.
- CAN-TP payload.
- Ethernet packet.
- Audio chunk.
- Camera line buffer.

## 17.3. Pool theo nhiều Size Class

```text
16-byte Pool
32-byte Pool
64-byte Pool
128-byte Pool
```

Giảm lãng phí nội bộ nhưng tăng độ phức tạp.

## 17.4. Bitmap Pool

Không dùng free list; dùng bitmap để đánh dấu block rảnh.

```text
0 = free
1 = used
```

Ưu điểm:

- Metadata nhỏ.
- Dễ kiểm tra Double Free.
- Dễ snapshot trạng thái.

Nhược điểm:

- Tìm bit rảnh có thể không còn `O(1)` nếu không có cấu trúc hỗ trợ.
- Có thể cần thao tác bit nguyên tử.

## 17.5. Lock-Free Pool

Có thể xây dựng free list lock-free bằng atomic compare-and-swap.

Tuy nhiên cần xử lý:

- ABA problem.
- Memory ordering.
- Pointer width.
- ISR priority.
- Atomic support của MCU.
- Reclamation và ownership.

Không nên tự viết lock-free allocator cho production nếu chưa có kiểm thử concurrency nghiêm ngặt.

## 17.6. Arena Allocator

Arena cấp phát tuần tự từ một vùng nhớ và giải phóng toàn bộ cùng lúc.

```text
allocate -> allocate -> allocate -> reset all
```

Khác Memory Pool:

- Arena không thường giải phóng từng object.
- Phù hợp các object có cùng vòng đời.
- Cấp phát rất nhanh.
- Không tái sử dụng từng block riêng lẻ trước khi reset.

## 17.7. Buddy Allocator

Buddy Allocator chia vùng nhớ thành các block lũy thừa của hai và có thể gộp các buddy khi giải phóng.

Phù hợp yêu cầu kích thước biến đổi hơn Fixed-Block Pool, nhưng phức tạp hơn và vẫn có Internal Fragmentation.

---

## 18. Khi nào nên và không nên dùng

## Nên dùng khi

- Object có kích thước cố định hoặc một vài size class.
- Số object đồng thời có giới hạn rõ.
- Cần thời gian cấp phát có giới hạn trên.
- Firmware chạy lâu.
- Muốn tránh phân mảnh Heap.
- Cần cấp Event, Message, Packet hoặc Node.
- Cần theo dõi mức sử dụng.
- Hệ thống Event-Driven hoặc RTOS.
- Muốn tránh `malloc()` sau startup.

## Không nên dùng khi

- Kích thước yêu cầu thay đổi rất lớn.
- Không thể xác định số object tối đa.
- Object quá lớn và hiếm khi dùng.
- Cần resize block thường xuyên.
- Vòng đời object không rõ và ownership lộn xộn.
- Một mảng tĩnh đơn giản đã đáp ứng yêu cầu.
- Stack allocation đủ dùng.
- Hệ điều hành đã cung cấp allocator phù hợp và không có yêu cầu real-time chặt.

## So sánh nhanh

| Tiêu chí | Heap | Fixed-Block Pool | Stack |
|---|---|---|---|
| Kích thước object | Linh hoạt | Cố định | Cố định khi vào scope |
| Thời gian cấp phát | Có thể biến đổi | Thường `O(1)` | Rất nhanh |
| Giải phóng riêng lẻ | Có | Có | Không |
| Phân mảnh ngoài | Có thể có | Không trong Pool | Không |
| Số object tối đa | Theo Heap | Biết trước | Theo Stack |
| Dùng trong ISR | Thường không nên | Có thể nếu đồng bộ đúng | Biến cục bộ nhỏ |
| Kiểm soát bộ nhớ | Trung bình | Cao | Cao |
| Phù hợp real-time | Tùy allocator | Tốt | Tốt |

---

## 19. Cấu trúc repository đề xuất

```text
fixed-block-memory-pool/
├── README.md
├── inc/
│   └── memory_pool.h
├── src/
│   └── memory_pool.c
├── examples/
│   ├── basic/
│   │   └── main.c
│   ├── event_pool/
│   │   └── event_pool_example.c
│   ├── packet_pool/
│   │   └── packet_pool_example.c
│   └── linked_list_node_pool/
│       └── node_pool_example.c
├── tests/
│   └── test_memory_pool.c
└── CMakeLists.txt
```

### `CMakeLists.txt` tối giản

```cmake
cmake_minimum_required(VERSION 3.16)

project(fixed_block_memory_pool C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(memory_pool
    src/memory_pool.c
)

target_include_directories(memory_pool
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_compile_options(memory_pool
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

add_executable(memory_pool_tests
    tests/test_memory_pool.c
)

target_link_libraries(memory_pool_tests
    PRIVATE
        memory_pool
)
```

Build:

```bash
cmake -S . -B build
cmake --build build
./build/memory_pool_tests
```

---

## 20. Tóm tắt

Các điểm quan trọng cần nhớ:

- Memory Pool chuẩn bị trước một vùng nhớ cố định.
- Vùng nhớ được chia thành nhiều block để tái sử dụng.
- Fixed-Block Pool thường quản lý block rảnh bằng free list.
- Khởi tạo nối các block thành Singly Linked List.
- Cấp phát lấy block đầu free list.
- Giải phóng đưa block trở lại free list.
- Cấp phát và giải phóng có thể thực hiện trong `O(1)`.
- Pool không có phân mảnh ngoài bên trong vùng block cố định.
- Vẫn có Internal Fragmentation nếu object nhỏ hơn block.
- Cần xử lý rõ trường hợp Pool cạn.
- Cần kiểm tra object không lớn hơn payload.
- Cần bảo đảm alignment.
- Cần xác định ownership của Event, Message và Packet.
- Pool không tự thread-safe.
- Thao tác giữa ISR và Task phải được đồng bộ.
- Không dùng `volatile` thay cho critical section.
- Nên theo dõi `free_count` và High-Water Mark.
- Bản debug nên phát hiện con trỏ sai và Double Free.
- Với nhiều kích thước object, có thể dùng nhiều size class.
- Memory Pool đặc biệt phù hợp Event-Driven, RTOS, Packet Buffer và Linked List Node.

---

## Tài liệu tham khảo

README này được biên soạn lại và mở rộng dựa trên chủ đề:

- [Data Structures - Memory Pool và ứng dụng — AK Embedded Software, EPCB](https://epcb.vn/blogs/ak-embedded-software/pool-memory-va-ung-dung)

Nội dung không sao chép nguyên văn bài viết. Phần mã nguồn được thiết kế lại thành một thư viện Fixed-Block Memory Pool độc lập, có bổ sung kiểm tra lỗi, alignment, Double Free detection, thống kê mức sử dụng, unit test và các lưu ý dành cho ISR/RTOS.
