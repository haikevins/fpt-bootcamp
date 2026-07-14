# Ring Buffer trong lập trình nhúng

Ring Buffer, còn được gọi là **Circular Buffer** hoặc **bộ đệm vòng**, là một cấu trúc dữ liệu FIFO có vùng nhớ cố định. Khi con trỏ đọc hoặc ghi đi đến cuối vùng nhớ, nó quay lại vị trí đầu tiên thay vì tiếp tục mở rộng bộ nhớ.

Ring Buffer đặc biệt phù hợp với hệ thống nhúng vì:

- Không cần cấp phát động trong quá trình chạy.
- Thời gian thêm và lấy dữ liệu là `O(1)`.
- Kích thước bộ nhớ được xác định trước.
- Phù hợp với dữ liệu đến không đồng bộ.
- Dễ dùng để tách phần thu thập dữ liệu khỏi phần xử lý dữ liệu.

Các ứng dụng thường gặp:

- Nhận dữ liệu UART, SPI hoặc USB.
- Lưu hàng đợi sự kiện giữa ISR và vòng lặp chính.
- Lưu các mẫu ADC gần nhất.
- Buffer âm thanh hoặc dữ liệu streaming.
- Lưu log trước khi truyền ra cổng debug.
- Xây dựng command-line parser.
- Giao tiếp giữa các task trong hệ thống thời gian thực.

> Tài liệu này tập trung vào Ring Buffer được hiện thực bằng mảng tĩnh trong ngôn ngữ C.

---

## Mục lục

1. [Nguyên lý FIFO](#1-nguyên-lý-fifo)
2. [Cấu trúc của Ring Buffer](#2-cấu-trúc-của-ring-buffer)
3. [Cơ chế quay vòng](#3-cơ-chế-quay-vòng)
4. [Trạng thái rỗng và đầy](#4-trạng-thái-rỗng-và-đầy)
5. [Chính sách khi buffer đầy](#5-chính-sách-khi-buffer-đầy)
6. [Độ phức tạp](#6-độ-phức-tạp)
7. [Thiết kế API](#7-thiết-kế-api)
8. [Mã nguồn hoàn chỉnh](#8-mã-nguồn-hoàn-chỉnh)
9. [Ví dụ sử dụng](#9-ví-dụ-sử-dụng)
10. [Ứng dụng với UART](#10-ứng-dụng-với-uart)
11. [Ứng dụng lọc dữ liệu ADC](#11-ứng-dụng-lọc-dữ-liệu-adc)
12. [Sử dụng với Interrupt và RTOS](#12-sử-dụng-với-interrupt-và-rtos)
13. [Các lỗi thường gặp](#13-các-lỗi-thường-gặp)
14. [Tối ưu cho vi điều khiển](#14-tối-ưu-cho-vi-điều-khiển)
15. [Kiểm thử đề xuất](#15-kiểm-thử-đề-xuất)
16. [Ring Buffer bằng mảng và Linked List](#16-ring-buffer-bằng-mảng-và-linked-list)
17. [Ring Buffer và Queue khác nhau thế nào?](#17-ring-buffer-và-queue-khác-nhau-thế-nào)
18. [Khi nào không nên dùng Ring Buffer?](#18-khi-nào-không-nên-dùng-ring-buffer)
19. [Cấu trúc thư mục đề xuất](#19-cấu-trúc-thư-mục-đề-xuất)
20. [Tóm tắt](#20-tóm-tắt)

---

## 1. Nguyên lý FIFO

Ring Buffer hoạt động theo nguyên tắc:

```text
First In, First Out
```

Phần tử được thêm vào trước sẽ được lấy ra trước.

Ví dụ:

```text
Put:  A
Put:  B
Put:  C

Get:  A
Get:  B
Get:  C
```

Ring Buffer khác mảng tuyến tính ở chỗ vùng nhớ được xem như một vòng tròn.

```text
index:    0    1    2    3    4
        +----+----+----+----+----+
buffer: |    |    |    |    |    |
        +----+----+----+----+----+
          ^                   |
          |___________________|
                quay vòng
```

Khi chỉ mục đạt đến cuối mảng, chỉ mục tiếp theo được đưa về `0`.

---

## 2. Cấu trúc của Ring Buffer

Một Ring Buffer cơ bản thường có các thành phần sau:

| Thành phần | Ý nghĩa |
|---|---|
| `buffer` | Con trỏ đến vùng nhớ lưu dữ liệu |
| `capacity` | Số phần tử tối đa có thể lưu |
| `head` | Vị trí của phần tử cũ nhất, sẽ được đọc tiếp theo |
| `tail` | Vị trí sẽ ghi phần tử mới tiếp theo |
| `count` | Số phần tử hợp lệ hiện có |
| `overwrite` | Cho phép ghi đè dữ liệu cũ khi đầy hay không |

Quy ước được sử dụng trong tài liệu này:

```text
head = vị trí đọc tiếp theo
tail = vị trí ghi tiếp theo
```

Khi khởi tạo:

```text
head  = 0
tail  = 0
count = 0
```

Buffer đang rỗng.

---

## 3. Cơ chế quay vòng

Chỉ mục tiếp theo có thể được tính bằng phép chia lấy dư:

```c
next_index = (current_index + 1U) % capacity;
```

Ví dụ với `capacity = 5`:

```text
0 → 1 → 2 → 3 → 4 → 0 → 1 → ...
```

### Quá trình thêm dữ liệu

Giả sử buffer có 5 phần tử:

```text
head = 0
tail = 0
count = 0
```

Thêm `A`:

```text
        head
          |
          v
        +---+---+---+---+---+
buffer: | A |   |   |   |   |
        +---+---+---+---+---+
              ^
              |
             tail

count = 1
```

Thêm tiếp `B` và `C`:

```text
        head
          |
          v
        +---+---+---+---+---+
buffer: | A | B | C |   |   |
        +---+---+---+---+---+
                      ^
                      |
                     tail

count = 3
```

### Quá trình lấy dữ liệu

Lấy một phần tử sẽ trả về `A`, sau đó `head` dịch sang vị trí tiếp theo:

```text
            head
              |
              v
        +---+---+---+---+---+
buffer: | A | B | C |   |   |
        +---+---+---+---+---+
                      ^
                      |
                     tail

count = 2
```

Giá trị `A` có thể vẫn còn trong vùng nhớ vật lý, nhưng nó không còn được xem là dữ liệu hợp lệ vì đã nằm ngoài phạm vi logic của Ring Buffer.

---

## 4. Trạng thái rỗng và đầy

Khi sử dụng biến `count`, trạng thái của buffer được xác định rõ ràng:

```c
empty = (count == 0U);
full  = (count == capacity);
```

### Buffer rỗng

```text
count = 0
```

Không được lấy dữ liệu vì không có phần tử hợp lệ.

### Buffer đầy

```text
count = capacity
```

Không còn vị trí trống. Hành vi tiếp theo phụ thuộc vào chính sách được lựa chọn.

### Tại sao không chỉ dùng `head == tail`?

Nếu chỉ lưu `head` và `tail`, biểu thức:

```c
head == tail
```

có thể biểu diễn cả hai trường hợp:

- Buffer rỗng.
- Buffer đầy sau khi con trỏ ghi quay hết một vòng.

Có ba cách phổ biến để giải quyết sự mơ hồ này:

1. Dùng thêm biến `count`.
2. Dùng thêm cờ `is_full`.
3. Luôn để trống một ô, khi đó sức chứa thực tế chỉ còn `capacity - 1`.

Tài liệu này sử dụng biến `count` vì dễ hiểu và cho phép dùng toàn bộ vùng nhớ.

---

## 5. Chính sách khi buffer đầy

Có hai chính sách phổ biến.

### 5.1. Từ chối dữ liệu mới

Khi buffer đã đầy, hàm ghi trả về lỗi:

```text
RB_FULL
```

Ưu điểm:

- Không làm mất dữ liệu cũ.
- Phù hợp với command, packet hoặc dữ liệu không được phép bỏ.

Nhược điểm:

- Phía gửi dữ liệu phải xử lý trạng thái đầy.
- Có thể mất dữ liệu mới nếu producer nhanh hơn consumer.

### 5.2. Ghi đè phần tử cũ nhất

Dữ liệu mới được ghi vào vị trí hiện tại của `tail`. Đồng thời `head` cũng được đẩy sang vị trí tiếp theo để loại bỏ phần tử cũ nhất.

Ưu điểm:

- Buffer luôn nhận được dữ liệu mới.
- Luôn giữ lại `N` phần tử gần nhất.
- Phù hợp với log, dữ liệu cảm biến hoặc cửa sổ lọc.

Nhược điểm:

- Dữ liệu chưa được xử lý có thể bị mất.
- Cần xác định rõ ứng dụng có chấp nhận mất dữ liệu cũ hay không.

Trong mã nguồn bên dưới, chính sách này được cấu hình bằng biến:

```c
overwrite
```

---

## 6. Độ phức tạp

| Thao tác | Độ phức tạp thời gian |
|---|---|
| Khởi tạo | `O(1)` |
| Thêm một phần tử | `O(1)` |
| Lấy một phần tử | `O(1)` |
| Xem phần tử đầu | `O(1)` |
| Kiểm tra rỗng/đầy | `O(1)` |
| Xóa toàn bộ dữ liệu logic | `O(1)` |
| Đọc/ghi `n` phần tử | `O(n)` |

Dung lượng bộ nhớ:

```text
O(capacity)
```

---

## 7. Thiết kế API

API được chia thành các nhóm:

### Khởi tạo và đặt lại

```c
rb_status_t rb_init(...);
void rb_reset(...);
```

### Kiểm tra trạng thái

```c
bool rb_is_empty(...);
bool rb_is_full(...);
size_t rb_size(...);
size_t rb_capacity(...);
size_t rb_free_space(...);
```

### Thêm và lấy dữ liệu

```c
rb_status_t rb_push(...);
rb_status_t rb_pop(...);
rb_status_t rb_peek(...);
```

### Thao tác theo khối

```c
size_t rb_write(...);
size_t rb_read(...);
```

Các hàm trả về mã trạng thái thay vì âm thầm bỏ qua lỗi.

---

## 8. Mã nguồn hoàn chỉnh

### `ring_buffer.h`

```c
#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum
{
    RB_OK = 0,
    RB_EMPTY,
    RB_FULL,
    RB_INVALID_ARGUMENT
} rb_status_t;

typedef struct
{
    uint8_t *buffer;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t count;
    bool overwrite;
} ring_buffer_t;

/**
 * @brief Initialize a byte-oriented ring buffer.
 *
 * @param rb         Ring buffer object.
 * @param storage    User-provided storage array.
 * @param capacity   Number of bytes in storage.
 * @param overwrite  true: overwrite oldest byte when full.
 *                   false: reject new byte when full.
 *
 * @return RB_OK on success, otherwise RB_INVALID_ARGUMENT.
 */
rb_status_t rb_init(ring_buffer_t *rb,
                    uint8_t *storage,
                    size_t capacity,
                    bool overwrite);

/**
 * @brief Remove all logical data from the buffer.
 *
 * The underlying memory is not cleared.
 */
void rb_reset(ring_buffer_t *rb);

bool rb_is_empty(const ring_buffer_t *rb);
bool rb_is_full(const ring_buffer_t *rb);

size_t rb_size(const ring_buffer_t *rb);
size_t rb_capacity(const ring_buffer_t *rb);
size_t rb_free_space(const ring_buffer_t *rb);

rb_status_t rb_push(ring_buffer_t *rb, uint8_t data);
rb_status_t rb_pop(ring_buffer_t *rb, uint8_t *data);
rb_status_t rb_peek(const ring_buffer_t *rb, uint8_t *data);

size_t rb_write(ring_buffer_t *rb,
                const uint8_t *data,
                size_t length);

size_t rb_read(ring_buffer_t *rb,
               uint8_t *data,
               size_t length);

#endif
```

### `ring_buffer.c`

```c
#include "ring_buffer.h"

static bool rb_is_valid(const ring_buffer_t *rb)
{
    return (rb != NULL) &&
           (rb->buffer != NULL) &&
           (rb->capacity > 0U);
}

static size_t rb_next_index(const ring_buffer_t *rb, size_t index)
{
    index++;

    if (index >= rb->capacity)
    {
        index = 0U;
    }

    return index;
}

rb_status_t rb_init(ring_buffer_t *rb,
                    uint8_t *storage,
                    size_t capacity,
                    bool overwrite)
{
    if ((rb == NULL) || (storage == NULL) || (capacity == 0U))
    {
        return RB_INVALID_ARGUMENT;
    }

    rb->buffer = storage;
    rb->capacity = capacity;
    rb->head = 0U;
    rb->tail = 0U;
    rb->count = 0U;
    rb->overwrite = overwrite;

    return RB_OK;
}

void rb_reset(ring_buffer_t *rb)
{
    if (!rb_is_valid(rb))
    {
        return;
    }

    rb->head = 0U;
    rb->tail = 0U;
    rb->count = 0U;
}

bool rb_is_empty(const ring_buffer_t *rb)
{
    return !rb_is_valid(rb) || (rb->count == 0U);
}

bool rb_is_full(const ring_buffer_t *rb)
{
    return rb_is_valid(rb) && (rb->count == rb->capacity);
}

size_t rb_size(const ring_buffer_t *rb)
{
    return rb_is_valid(rb) ? rb->count : 0U;
}

size_t rb_capacity(const ring_buffer_t *rb)
{
    return rb_is_valid(rb) ? rb->capacity : 0U;
}

size_t rb_free_space(const ring_buffer_t *rb)
{
    return rb_is_valid(rb) ? (rb->capacity - rb->count) : 0U;
}

rb_status_t rb_push(ring_buffer_t *rb, uint8_t data)
{
    if (!rb_is_valid(rb))
    {
        return RB_INVALID_ARGUMENT;
    }

    if (rb_is_full(rb))
    {
        if (!rb->overwrite)
        {
            return RB_FULL;
        }

        /*
         * The new byte replaces the oldest byte.
         * Advance head to point to the new oldest byte.
         */
        rb->buffer[rb->tail] = data;
        rb->tail = rb_next_index(rb, rb->tail);
        rb->head = rb_next_index(rb, rb->head);

        return RB_OK;
    }

    rb->buffer[rb->tail] = data;
    rb->tail = rb_next_index(rb, rb->tail);
    rb->count++;

    return RB_OK;
}

rb_status_t rb_pop(ring_buffer_t *rb, uint8_t *data)
{
    if (!rb_is_valid(rb) || (data == NULL))
    {
        return RB_INVALID_ARGUMENT;
    }

    if (rb_is_empty(rb))
    {
        return RB_EMPTY;
    }

    *data = rb->buffer[rb->head];
    rb->head = rb_next_index(rb, rb->head);
    rb->count--;

    return RB_OK;
}

rb_status_t rb_peek(const ring_buffer_t *rb, uint8_t *data)
{
    if (!rb_is_valid(rb) || (data == NULL))
    {
        return RB_INVALID_ARGUMENT;
    }

    if (rb_is_empty(rb))
    {
        return RB_EMPTY;
    }

    *data = rb->buffer[rb->head];

    return RB_OK;
}

size_t rb_write(ring_buffer_t *rb,
                const uint8_t *data,
                size_t length)
{
    size_t written = 0U;

    if (!rb_is_valid(rb) || ((data == NULL) && (length > 0U)))
    {
        return 0U;
    }

    while (written < length)
    {
        const rb_status_t status = rb_push(rb, data[written]);

        if (status != RB_OK)
        {
            break;
        }

        written++;
    }

    return written;
}

size_t rb_read(ring_buffer_t *rb,
               uint8_t *data,
               size_t length)
{
    size_t read_count = 0U;

    if (!rb_is_valid(rb) || ((data == NULL) && (length > 0U)))
    {
        return 0U;
    }

    while (read_count < length)
    {
        const rb_status_t status = rb_pop(rb, &data[read_count]);

        if (status != RB_OK)
        {
            break;
        }

        read_count++;
    }

    return read_count;
}
```

### Tại sao không dùng `%` trong `rb_next_index()`?

Phép toán sau hoàn toàn hợp lệ:

```c
(index + 1U) % capacity
```

Tuy nhiên, trên một số vi điều khiển không có phần cứng chia, phép `%` có thể tạo ra nhiều lệnh hơn. So sánh và đưa chỉ mục về `0` thường đơn giản và dễ dự đoán hơn:

```c
index++;

if (index >= capacity)
{
    index = 0U;
}
```

Trình biên dịch hiện đại có thể tự tối ưu, vì vậy nên đo đạc trước khi kết luận một cách triển khai nhanh hơn.

---

## 9. Ví dụ sử dụng

### `main.c`

```c
#include <stdio.h>

#include "ring_buffer.h"

#define BUFFER_SIZE 5U

int main(void)
{
    uint8_t storage[BUFFER_SIZE];
    ring_buffer_t rb;
    uint8_t value;

    if (rb_init(&rb, storage, BUFFER_SIZE, true) != RB_OK)
    {
        return 1;
    }

    rb_push(&rb, 'A');
    rb_push(&rb, 'B');
    rb_push(&rb, 'C');

    while (rb_pop(&rb, &value) == RB_OK)
    {
        printf("%c\n", (char)value);
    }

    return 0;
}
```

Kết quả:

```text
A
B
C
```

### Kiểm tra cơ chế ghi đè

```c
#include <stdio.h>

#include "ring_buffer.h"

#define BUFFER_SIZE 4U

int main(void)
{
    uint8_t storage[BUFFER_SIZE];
    ring_buffer_t rb;
    uint8_t value;

    rb_init(&rb, storage, BUFFER_SIZE, true);

    rb_push(&rb, 1U);
    rb_push(&rb, 2U);
    rb_push(&rb, 3U);
    rb_push(&rb, 4U);

    /* Buffer is full. The oldest value, 1, will be overwritten. */
    rb_push(&rb, 5U);

    while (rb_pop(&rb, &value) == RB_OK)
    {
        printf("%u ", (unsigned int)value);
    }

    return 0;
}
```

Kết quả:

```text
2 3 4 5
```

---

## 10. Ứng dụng với UART

Một ứng dụng phổ biến là nhận từng byte trong ngắt UART và xử lý dữ liệu ở vòng lặp chính.

### Luồng dữ liệu

```text
UART peripheral
      |
      | RX interrupt
      v
+-------------------+
| UART RX ISR       |
| Read one byte     |
| Push to RB        |
+-------------------+
      |
      v
+-------------------+
| Ring Buffer       |
+-------------------+
      |
      | Pop bytes
      v
+-------------------+
| Main loop/parser  |
| Process command   |
+-------------------+
```

Mục tiêu là giữ ISR ngắn:

```text
ISR chỉ nhận byte và đưa vào buffer.
Phần phân tích command được thực hiện bên ngoài ISR.
```

### Ví dụ với STM32

Tên thanh ghi và cờ ngắt thay đổi tùy theo dòng STM32 hoặc thư viện được sử dụng. Đoạn mã dưới đây minh họa nguyên lý:

```c
#include "ring_buffer.h"

#define UART_RX_BUFFER_SIZE 128U

static uint8_t uart_rx_storage[UART_RX_BUFFER_SIZE];
static ring_buffer_t uart_rx_buffer;

void app_init(void)
{
    (void)rb_init(&uart_rx_buffer,
                  uart_rx_storage,
                  UART_RX_BUFFER_SIZE,
                  false);
}

void USART1_IRQHandler(void)
{
    if (uart_rx_data_is_available())
    {
        const uint8_t received_byte = uart_rx_read_byte();

        if (rb_push(&uart_rx_buffer, received_byte) != RB_OK)
        {
            /*
             * Overflow handling options:
             * - increment an error counter;
             * - discard the byte;
             * - set an overflow event;
             * - reset the current frame.
             */
        }
    }
}

void app_process(void)
{
    uint8_t byte;

    while (rb_pop(&uart_rx_buffer, &byte) == RB_OK)
    {
        command_parser_process_byte(byte);
    }
}
```

### Nhận command kết thúc bằng ký tự xuống dòng

```c
#include <stddef.h>
#include <stdint.h>

#define COMMAND_MAX_LENGTH 64U

static char command[COMMAND_MAX_LENGTH];
static size_t command_length;

static void command_parser_process_byte(uint8_t byte)
{
    if ((byte == '\r') || (byte == '\n'))
    {
        if (command_length > 0U)
        {
            command[command_length] = '\0';
            command_execute(command);
            command_length = 0U;
        }

        return;
    }

    if (command_length < (COMMAND_MAX_LENGTH - 1U))
    {
        command[command_length] = (char)byte;
        command_length++;
    }
    else
    {
        /* The command is too long. Drop the current command safely. */
        command_length = 0U;
    }
}
```

### Khi nào nên ghi đè dữ liệu UART?

Thông thường, UART dùng để nhận command hoặc frame giao thức thì **không nên ghi đè**. Nếu một byte bị mất, toàn bộ frame có thể sai.

Nên dùng:

```c
overwrite = false;
```

Sau đó phát hiện `RB_FULL` và xử lý overflow rõ ràng.

Chính sách ghi đè phù hợp hơn khi chỉ cần dữ liệu mới nhất, ví dụ:

- Log chẩn đoán không quan trọng.
- Dữ liệu cảm biến liên tục.
- Telemetry có thể chấp nhận bỏ mẫu cũ.

---

## 11. Ứng dụng lọc dữ liệu ADC

Ring Buffer có thể lưu `N` mẫu ADC gần nhất để tính trung bình trượt.

Ví dụ với cửa sổ 10 mẫu:

```text
sample[0], sample[1], ..., sample[9]
```

Khi có mẫu mới:

1. Loại ảnh hưởng của mẫu cũ nhất khỏi tổng.
2. Ghi mẫu mới vào vị trí đó.
3. Cộng mẫu mới vào tổng.
4. Di chuyển chỉ mục sang vị trí tiếp theo.

Cách này không cần tính lại tổng của toàn bộ mảng sau mỗi mẫu.

```c
#include <stddef.h>
#include <stdint.h>

#define ADC_WINDOW_SIZE 10U

typedef struct
{
    uint16_t samples[ADC_WINDOW_SIZE];
    size_t index;
    size_t count;
    uint32_t sum;
} adc_moving_average_t;

void adc_filter_init(adc_moving_average_t *filter)
{
    size_t i;

    if (filter == NULL)
    {
        return;
    }

    for (i = 0U; i < ADC_WINDOW_SIZE; i++)
    {
        filter->samples[i] = 0U;
    }

    filter->index = 0U;
    filter->count = 0U;
    filter->sum = 0U;
}

uint16_t adc_filter_update(adc_moving_average_t *filter,
                           uint16_t new_sample)
{
    if (filter == NULL)
    {
        return 0U;
    }

    if (filter->count < ADC_WINDOW_SIZE)
    {
        filter->count++;
    }
    else
    {
        filter->sum -= filter->samples[filter->index];
    }

    filter->samples[filter->index] = new_sample;
    filter->sum += new_sample;

    filter->index++;

    if (filter->index >= ADC_WINDOW_SIZE)
    {
        filter->index = 0U;
    }

    return (uint16_t)(filter->sum / filter->count);
}
```

Độ phức tạp cho mỗi mẫu:

```text
O(1)
```

Nếu mỗi lần đều cộng lại toàn bộ `N` mẫu, độ phức tạp sẽ là:

```text
O(N)
```

---

## 12. Sử dụng với Interrupt và RTOS

### Ring Buffer không tự động thread-safe

Mã nguồn cơ bản ở trên không tự bảo vệ khi có nhiều execution context cùng truy cập, chẳng hạn:

- ISR ghi, `main()` đọc.
- Hai ISR cùng ghi.
- Nhiều task cùng đọc hoặc ghi.
- Một task ghi trong khi task khác reset buffer.

Một thao tác `push` hoặc `pop` thay đổi nhiều trường:

```text
buffer
head hoặc tail
count
```

Nếu bị ngắt giữa chừng, trạng thái có thể không nhất quán.

### Trường hợp ISR producer và main consumer

Có thể bảo vệ phần truy cập bằng critical section ngắn.

Ví dụ khái niệm:

```c
bool uart_buffer_pop_safe(uint8_t *data)
{
    rb_status_t status;

    ENTER_CRITICAL_SECTION();
    status = rb_pop(&uart_rx_buffer, data);
    EXIT_CRITICAL_SECTION();

    return status == RB_OK;
}
```

Critical section chỉ nên bao quanh thao tác rất ngắn. Không đặt các thao tác chậm như:

- `printf`.
- Chờ UART truyền.
- Delay.
- Cấp phát bộ nhớ.
- Phân tích command dài.

bên trong critical section.

### Với FreeRTOS

Tùy mục tiêu, có thể cân nhắc:

- FreeRTOS Queue.
- Stream Buffer.
- Message Buffer.
- Mutex.
- Critical section.
- Task notification.

Ring Buffer tự viết vẫn hữu ích khi:

- Cần API rất nhỏ.
- Cần kiểm soát chính xác bộ nhớ.
- Không muốn phụ thuộc RTOS.
- Chỉ có một producer và một consumer.
- Cần tối ưu riêng cho driver.

### Single Producer, Single Consumer

Một Ring Buffer có thể được thiết kế lock-free cho mô hình:

```text
một producer + một consumer
```

nhưng phải tuân thủ chặt chẽ:

- Producer chỉ cập nhật chỉ mục ghi.
- Consumer chỉ cập nhật chỉ mục đọc.
- Có quy tắc memory ordering phù hợp.
- Không dùng chung biến `count` theo cách không nguyên tử.
- Kiểu dữ liệu chỉ mục phải được CPU đọc/ghi nguyên tử.
- Compiler và CPU không được phép sắp xếp lại thao tác ngoài dự kiến.

Vì vậy, không nên tự nhận mã nguồn là lock-free chỉ vì nó không dùng mutex.

---

## 13. Các lỗi thường gặp

### 13.1. Nhầm ý nghĩa của `head` và `tail`

Cần quy định ngay từ đầu:

```text
head đọc
tail ghi
```

Không thay đổi quy ước giữa các hàm.

### 13.2. Không phân biệt được buffer rỗng và đầy

Dùng `head == tail` mà không có `count`, cờ `full` hoặc một ô dự phòng sẽ tạo trạng thái mơ hồ.

### 13.3. Ghi đè nhưng không thông báo

Nếu ứng dụng không chấp nhận mất dữ liệu, hàm `push` phải trả về `RB_FULL`.

Không nên ghi đè âm thầm.

### 13.4. Cho rằng `pop` sẽ xóa byte khỏi RAM

`pop` chỉ làm dữ liệu không còn hợp lệ về mặt logic. Byte cũ vẫn có thể còn trong mảng cho đến khi bị ghi đè.

Nếu buffer chứa dữ liệu nhạy cảm, có thể cần xóa vùng nhớ một cách chủ động.

### 13.5. Dùng Ring Buffer byte cho packet nhưng không có ranh giới frame

Ring Buffer byte chỉ lưu luồng byte:

```text
A B C D E F ...
```

Nó không tự biết đâu là đầu và cuối packet.

Cần thêm một trong các cơ chế:

- Ký tự phân cách.
- Trường độ dài.
- Header cố định.
- State machine parser.
- CRC/checksum.
- Ring Buffer của các object packet thay vì byte.

### 13.6. Không kiểm tra `capacity == 0`

Phép modulo hoặc quay vòng với sức chứa bằng `0` là không hợp lệ.

### 13.7. Tràn biến `count`

Luôn duy trì điều kiện bất biến:

```text
0 <= count <= capacity
```

### 13.8. Gọi hàm xử lý nặng trong ISR

ISR nên chỉ thực hiện công việc tối thiểu:

```text
đọc dữ liệu → lưu buffer → báo sự kiện → thoát ISR
```

### 13.9. Dùng bộ nhớ động không cần thiết

Trong embedded, nên ưu tiên:

```c
static uint8_t storage[BUFFER_SIZE];
```

thay vì gọi `malloc()` trong quá trình chạy.

### 13.10. Không kiểm thử lúc chỉ mục quay vòng

Nhiều lỗi chỉ xuất hiện khi:

```text
tail đi từ capacity - 1 về 0
head đi từ capacity - 1 về 0
```

---

## 14. Tối ưu cho vi điều khiển

### 14.1. Dùng sức chứa là lũy thừa của hai

Nếu:

```text
capacity = 2^n
```

có thể tính chỉ mục bằng phép AND:

```c
next_index = (index + 1U) & (capacity - 1U);
```

Ví dụ:

```text
capacity = 128
mask     = 127
```

Điều kiện bắt buộc:

```c
(capacity & (capacity - 1U)) == 0U
```

Không dùng kỹ thuật này nếu `capacity` không phải lũy thừa của hai.

### 14.2. Chọn kiểu chỉ mục phù hợp

Nếu buffer nhỏ hơn `256` phần tử, có thể dùng `uint8_t`.

Nếu nhỏ hơn `65536` phần tử, có thể dùng `uint16_t`.

Tuy nhiên, nên cân nhắc:

- Kích thước word tự nhiên của CPU.
- Tính nguyên tử khi truy cập từ ISR.
- Khả năng mở rộng.
- Cảnh báo chuyển kiểu.

`size_t` thường thuận tiện và đúng với kích thước mảng trong C.

### 14.3. Không copy nếu không cần thiết

Một thiết kế nâng cao có thể cung cấp con trỏ đến vùng dữ liệu liên tục trong buffer để DMA hoặc driver xử lý trực tiếp.

Tuy nhiên, khi dữ liệu đi qua cuối mảng, vùng dữ liệu logic có thể bị chia thành hai đoạn:

```text
[head ... capacity - 1] + [0 ... tail - 1]
```

API zero-copy cần quản lý hai đoạn này cẩn thận.

### 14.4. Kết hợp DMA

Trong UART RX dùng DMA circular mode, DMA tự ghi dữ liệu vào một mảng vòng. Phần mềm theo dõi vị trí ghi hiện tại của DMA và xử lý phần dữ liệu mới.

Cần xử lý các sự kiện như:

- Half-transfer.
- Transfer-complete.
- UART idle line.
- DMA write index.
- Cache coherency trên MCU có data cache.

### 14.5. Dùng `volatile` đúng mục đích

`volatile` có thể ngăn compiler loại bỏ hoặc cache một số lần truy cập, nhưng:

```text
volatile không tạo atomicity
volatile không thay thế mutex
volatile không tự tạo memory barrier đầy đủ
```

Không nên thêm `volatile` vào mọi biến và cho rằng Ring Buffer đã an toàn với interrupt.

---

## 15. Kiểm thử đề xuất

Một Ring Buffer nên được kiểm thử ít nhất các trường hợp sau:

1. Khởi tạo với tham số hợp lệ.
2. Khởi tạo với con trỏ `NULL`.
3. Khởi tạo với `capacity = 0`.
4. `pop` khi buffer rỗng.
5. Thêm một phần tử rồi lấy ra.
6. Thêm đến khi đầy.
7. Ghi thêm khi đầy với `overwrite = false`.
8. Ghi thêm khi đầy với `overwrite = true`.
9. `head` quay từ cuối về đầu.
10. `tail` quay từ cuối về đầu.
11. Ghi và đọc nhiều vòng liên tiếp.
12. `peek` không làm thay đổi `count`.
13. Đọc theo khối nhiều hơn số phần tử hiện có.
14. Ghi theo khối nhiều hơn vùng trống.
15. Dữ liệu lấy ra đúng thứ tự FIFO.

### Ví dụ test đơn giản bằng `assert`

```c
#include <assert.h>
#include <stdint.h>

#include "ring_buffer.h"

static void test_fifo_order(void)
{
    uint8_t storage[3];
    ring_buffer_t rb;
    uint8_t value;

    assert(rb_init(&rb, storage, 3U, false) == RB_OK);
    assert(rb_is_empty(&rb));

    assert(rb_push(&rb, 10U) == RB_OK);
    assert(rb_push(&rb, 20U) == RB_OK);
    assert(rb_push(&rb, 30U) == RB_OK);
    assert(rb_is_full(&rb));

    assert(rb_pop(&rb, &value) == RB_OK);
    assert(value == 10U);

    assert(rb_pop(&rb, &value) == RB_OK);
    assert(value == 20U);

    assert(rb_pop(&rb, &value) == RB_OK);
    assert(value == 30U);

    assert(rb_pop(&rb, &value) == RB_EMPTY);
}

static void test_overwrite(void)
{
    uint8_t storage[3];
    ring_buffer_t rb;
    uint8_t value;

    assert(rb_init(&rb, storage, 3U, true) == RB_OK);

    assert(rb_push(&rb, 1U) == RB_OK);
    assert(rb_push(&rb, 2U) == RB_OK);
    assert(rb_push(&rb, 3U) == RB_OK);
    assert(rb_push(&rb, 4U) == RB_OK);

    assert(rb_pop(&rb, &value) == RB_OK);
    assert(value == 2U);

    assert(rb_pop(&rb, &value) == RB_OK);
    assert(value == 3U);

    assert(rb_pop(&rb, &value) == RB_OK);
    assert(value == 4U);
}

int main(void)
{
    test_fifo_order();
    test_overwrite();

    return 0;
}
```

---

## 16. Ring Buffer bằng mảng và Linked List

Ring Buffer có thể được xây dựng bằng mảng hoặc danh sách liên kết.

### Mảng

Ưu điểm:

- Bộ nhớ liên tục.
- Truy cập nhanh.
- Ít metadata.
- Cache-friendly.
- Dễ xác định dung lượng.
- Phù hợp với hệ thống không dùng heap.

Nhược điểm:

- Dung lượng thường cố định.
- Cần xử lý chỉ mục quay vòng.

### Linked List

Ưu điểm:

- Có thể mở rộng linh hoạt.
- Không yêu cầu vùng nhớ liên tục.

Nhược điểm:

- Mỗi node cần thêm con trỏ.
- Có thể phân mảnh heap.
- Thời gian cấp phát khó dự đoán.
- Quản lý lỗi cấp phát phức tạp.
- Không phù hợp với nhiều hệ thống real-time nhỏ.

Trong phần lớn firmware vi điều khiển, mảng tĩnh thường là lựa chọn đơn giản và dễ kiểm soát hơn.

---

## 17. Ring Buffer và Queue khác nhau thế nào?

Ring Buffer là một cách hiện thực Queue bằng vùng nhớ vòng cố định.

Một Queue tổng quát có thể:

- Dùng mảng.
- Dùng Linked List.
- Cấp phát động.
- Có kích thước thay đổi.

Ring Buffer thường nhấn mạnh:

- Kích thước cố định.
- Chỉ mục quay vòng.
- Không di chuyển toàn bộ dữ liệu sau mỗi lần lấy.
- Có thể hỗ trợ chính sách ghi đè dữ liệu cũ.

Không phải mọi Queue đều là Ring Buffer, nhưng Ring Buffer thường được dùng như một Queue FIFO.

---

## 18. Khi nào không nên dùng Ring Buffer?

Không nên dùng Ring Buffer byte đơn giản khi:

- Mỗi phần tử có kích thước thay đổi và không có framing.
- Không được phép mất bất kỳ dữ liệu nào nhưng không có flow control.
- Producer có thể nhanh hơn consumer trong thời gian không giới hạn.
- Cần tìm kiếm hoặc xóa phần tử ở giữa thường xuyên.
- Nhiều producer và consumer truy cập đồng thời nhưng không có cơ chế đồng bộ.
- Cần ưu tiên phần tử theo mức độ quan trọng.

Có thể cân nhắc:

- Message Queue.
- Priority Queue.
- Linked List.
- Memory Pool.
- FreeRTOS Stream Buffer hoặc Message Buffer.
- Giao thức có flow control.

---

## 19. Cấu trúc thư mục đề xuất

```text
ring-buffer/
├── README.md
├── inc/
│   └── ring_buffer.h
├── src/
│   └── ring_buffer.c
├── examples/
│   ├── basic/
│   │   └── main.c
│   ├── uart_rx/
│   │   └── uart_rx_example.c
│   └── adc_filter/
│       └── adc_filter_example.c
└── tests/
    └── test_ring_buffer.c
```

---

## 20. Tóm tắt

Ring Buffer là một cấu trúc dữ liệu quan trọng trong embedded software vì nó giải quyết tốt bài toán producer và consumer chạy ở tốc độ khác nhau.

Các điểm cần nhớ:

- Ring Buffer hoạt động theo FIFO.
- `head` trỏ đến phần tử sẽ đọc tiếp theo.
- `tail` trỏ đến vị trí sẽ ghi tiếp theo.
- Chỉ mục quay lại `0` khi đi qua cuối mảng.
- Biến `count` giúp phân biệt trạng thái rỗng và đầy.
- Cần xác định rõ chính sách khi đầy: từ chối hay ghi đè.
- `push`, `pop`, `peek` đều có thể thực hiện trong `O(1)`.
- Không nên mặc định Ring Buffer là thread-safe.
- Với UART command, thường nên từ chối byte mới khi đầy.
- Với dữ liệu cảm biến, ghi đè dữ liệu cũ có thể là lựa chọn phù hợp.
- Cần kiểm thử kỹ các trường hợp chỉ mục quay vòng và buffer overflow.

---

## Tài liệu tham khảo

Nội dung được biên soạn lại và mở rộng từ bài viết:

- **Data Structures - Ring buffer và ứng dụng**, AK Embedded Software, EPCB:  
  https://epcb.vn/blogs/ak-embedded-software/data-structures-ring-buffer-va-ung-dung

Phần mã nguồn trong README này được viết lại theo một API độc lập, có bổ sung kiểm tra lỗi, chính sách overflow, thao tác theo khối, ví dụ UART, lọc ADC và lưu ý đồng bộ.