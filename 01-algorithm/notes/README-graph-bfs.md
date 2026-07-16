# Breadth-First Search (BFS) for Graphs

> Tài liệu học tập độc lập về **Breadth-First Search**, trình bày nguyên lý duyệt đồ thị theo chiều rộng, cấu trúc Queue, mã nguồn C hoàn chỉnh, độ phức tạp và các ứng dụng.

---

## Mục lục

1. [BFS là gì?](#1-bfs-là-gì)
2. [Ý tưởng cốt lõi](#2-ý-tưởng-cốt-lõi)
3. [Tại sao BFS cần Queue?](#3-tại-sao-bfs-cần-queue)
4. [Trạng thái Visited](#4-trạng-thái-visited)
5. [Các bước của thuật toán](#5-các-bước-của-thuật-toán)
6. [Ví dụ từng bước](#6-ví-dụ-từng-bước)
7. [Pseudocode](#7-pseudocode)
8. [Biểu diễn Graph bằng Adjacency List](#8-biểu-diễn-graph-bằng-adjacency-list)
9. [Cài đặt Queue bằng Circular Array](#9-cài-đặt-queue-bằng-circular-array)
10. [Cài đặt BFS bằng C](#10-cài-đặt-bfs-bằng-c)
11. [Chạy chương trình](#11-chạy-chương-trình)
12. [BFS cho Graph không liên thông](#12-bfs-cho-graph-không-liên-thông)
13. [Tính khoảng cách ngắn nhất](#13-tính-khoảng-cách-ngắn-nhất)
14. [Khôi phục đường đi](#14-khôi-phục-đường-đi)
15. [Độ phức tạp](#15-độ-phức-tạp)
16. [BFS với Adjacency Matrix](#16-bfs-với-adjacency-matrix)
17. [So sánh BFS và DFS](#17-so-sánh-bfs-và-dfs)
18. [Ứng dụng của BFS](#18-ứng-dụng-của-bfs)
19. [BFS trong lập trình nhúng](#19-bfs-trong-lập-trình-nhúng)
20. [Các lỗi thường gặp](#20-các-lỗi-thường-gặp)
21. [Bài tập luyện tập](#21-bài-tập-luyện-tập)
22. [Tổng kết](#22-tổng-kết)
23. [Tài liệu tham khảo](#23-tài-liệu-tham-khảo)

---

## 1. BFS là gì?

**Breadth-First Search**, viết tắt là **BFS**, là thuật toán duyệt hoặc tìm kiếm trên Graph và Tree theo từng tầng.

Thuật toán bắt đầu tại một đỉnh nguồn, sau đó:

1. Thăm đỉnh nguồn.
2. Thăm toàn bộ đỉnh kề trực tiếp.
3. Thăm các đỉnh cách nguồn hai cạnh.
4. Tiếp tục theo từng lớp cho đến khi không còn đỉnh cần xử lý.

Ví dụ:

```text
            0
          /   \
         1     2
        / \     \
       3   4     5
```

BFS bắt đầu từ `0` có thể cho thứ tự:

```text
0, 1, 2, 3, 4, 5
```

Các đỉnh được xử lý theo khoảng cách tăng dần tính từ đỉnh bắt đầu.

---

## 2. Ý tưởng cốt lõi

BFS sử dụng hai thành phần quan trọng:

```text
Visited Array
      +
Queue
```

### `visited`

Cho biết một đỉnh đã được phát hiện hay chưa.

```text
visited[v] = false → chưa được phát hiện
visited[v] = true  → đã được phát hiện
```

### `queue`

Lưu các đỉnh đã được phát hiện nhưng chưa xử lý xong toàn bộ hàng xóm.

```text
Front                               Rear
  |                                   |
  v                                   v
[ vertex ][ vertex ][ vertex ][ vertex ]
```

BFS thực hiện:

```text
Lấy đỉnh ở đầu Queue
→ xử lý đỉnh
→ thêm các hàng xóm chưa visited vào cuối Queue
```

---

## 3. Tại sao BFS cần Queue?

Queue hoạt động theo nguyên tắc:

```text
FIFO — First In, First Out
```

Đỉnh được phát hiện trước sẽ được xử lý trước.

Giả sử từ đỉnh `0`, ta phát hiện:

```text
1, 2, 3
```

Queue:

```text
Front           Rear
  |               |
  v               v
[1][2][3]
```

BFS xử lý `1` trước, rồi `2`, rồi `3`.

Nhờ đó, các đỉnh cùng tầng được xử lý trước khi chuyển sang tầng sâu hơn.

Nếu dùng Stack, thứ tự xử lý sẽ đi sâu theo một nhánh trước, gần với DFS.

---

## 4. Trạng thái Visited

Graph có thể chứa cycle:

```text
0 → 1
↑   |
|   v
3 ← 2
```

Nếu không có `visited`, BFS có thể lặp vô hạn:

```text
0 → 1 → 2 → 3 → 0 → 1 → ...
```

Vì vậy, mỗi đỉnh cần được đánh dấu ngay khi được đưa vào Queue:

```c
visited[neighbor] = true;
Queue_Enqueue(&queue, neighbor);
```

Không nên chờ đến lúc dequeue mới đánh dấu.

Nếu đánh dấu quá muộn, cùng một đỉnh có thể bị nhiều đỉnh khác enqueue nhiều lần.

---

## 5. Các bước của thuật toán

Giả sử bắt đầu tại đỉnh `start`.

### Bước 1

Khởi tạo:

```text
visited của mọi đỉnh = false
Queue rỗng
```

### Bước 2

Đánh dấu đỉnh nguồn:

```text
visited[start] = true
```

Đưa nó vào Queue:

```text
enqueue(start)
```

### Bước 3

Trong khi Queue chưa rỗng:

1. Lấy đỉnh đầu Queue.
2. Xử lý hoặc in đỉnh đó.
3. Duyệt toàn bộ hàng xóm.
4. Với mỗi hàng xóm chưa visited:
   - Đánh dấu visited.
   - Enqueue hàng xóm.

### Bước 4

Kết thúc khi Queue rỗng.

---

## 6. Ví dụ từng bước

Xét Graph vô hướng:

```text
        0
       / \
      1---2
     / \ /
    3---4
```

Các cạnh:

```text
0 — 1
0 — 2
1 — 2
1 — 3
1 — 4
2 — 4
3 — 4
```

Bắt đầu từ:

```text
start = 0
```

### Trạng thái ban đầu

```text
visited = [false, false, false, false, false]
queue   = []
```

Đánh dấu và enqueue `0`:

```text
visited = [true, false, false, false, false]
queue   = [0]
```

---

### Xử lý đỉnh 0

Dequeue:

```text
current = 0
queue   = []
```

Hàng xóm của `0`:

```text
1, 2
```

Đánh dấu và enqueue:

```text
visited = [true, true, true, false, false]
queue   = [1, 2]
```

Thứ tự đã thăm:

```text
0
```

---

### Xử lý đỉnh 1

Dequeue:

```text
current = 1
queue   = [2]
```

Hàng xóm:

```text
0, 2, 3, 4
```

`0` và `2` đã visited.

Thêm `3`, `4`:

```text
visited = [true, true, true, true, true]
queue   = [2, 3, 4]
```

Thứ tự:

```text
0, 1
```

---

### Xử lý đỉnh 2

Dequeue:

```text
current = 2
queue   = [3, 4]
```

Hàng xóm của `2` đều đã visited.

```text
queue = [3, 4]
```

Thứ tự:

```text
0, 1, 2
```

---

### Xử lý đỉnh 3

Dequeue:

```text
current = 3
queue   = [4]
```

Các hàng xóm đã visited.

Thứ tự:

```text
0, 1, 2, 3
```

---

### Xử lý đỉnh 4

Dequeue:

```text
current = 4
queue   = []
```

Không còn hàng xóm chưa visited.

Thứ tự BFS cuối cùng:

```text
0, 1, 2, 3, 4
```

> Thứ tự chính xác có thể khác nếu thứ tự các hàng xóm trong Adjacency List khác nhau. Tuy nhiên, các đỉnh vẫn được duyệt theo từng tầng.

---

## 7. Pseudocode

```text
BFS(graph, start):

    tạo Queue Q

    với mỗi vertex:
        visited[vertex] = false

    visited[start] = true
    enqueue(Q, start)

    while Q không rỗng:

        current = dequeue(Q)
        xử lý current

        for mỗi neighbor của current:

            if visited[neighbor] == false:

                visited[neighbor] = true
                enqueue(Q, neighbor)
```

BFS cho toàn bộ Graph không liên thông:

```text
for mỗi vertex:

    if vertex chưa visited:
        BFS từ vertex đó
```

---

## 8. Biểu diễn Graph bằng Adjacency List

Graph có `V` đỉnh.

Mỗi đỉnh giữ một danh sách các đỉnh kề.

```text
0 → 1 → 2
1 → 0 → 2 → 3 → 4
2 → 0 → 1 → 4
3 → 1 → 4
4 → 1 → 2 → 3
```

### Node của danh sách kề

```c
typedef struct AdjNode
{
    size_t vertex;
    struct AdjNode *next;
} AdjNode;
```

### Graph

```c
typedef struct
{
    size_t vertex_count;
    AdjNode **adjacency;
} Graph;
```

`adjacency[v]` trỏ đến danh sách hàng xóm của đỉnh `v`.

---

## 9. Cài đặt Queue bằng Circular Array

BFS có tối đa `V` đỉnh trong Queue tại một thời điểm nếu mỗi đỉnh chỉ được enqueue một lần.

Do đó capacity có thể là:

```text
vertex_count
```

### Cấu trúc Queue

```c
typedef struct
{
    size_t *data;
    size_t capacity;
    size_t front;
    size_t rear;
    size_t size;
} VertexQueue;
```

### Hàm khởi tạo

```c
static bool Queue_Init(
    VertexQueue *queue,
    size_t capacity
)
{
    if ((queue == NULL) || (capacity == 0U))
    {
        return false;
    }

    queue->data =
        (size_t *)malloc(capacity * sizeof(size_t));

    if (queue->data == NULL)
    {
        return false;
    }

    queue->capacity = capacity;
    queue->front = 0U;
    queue->rear = 0U;
    queue->size = 0U;

    return true;
}
```

### Enqueue

```c
static bool Queue_Enqueue(
    VertexQueue *queue,
    size_t vertex
)
{
    if ((queue == NULL) ||
        (queue->data == NULL) ||
        (queue->size == queue->capacity))
    {
        return false;
    }

    queue->data[queue->rear] = vertex;

    queue->rear =
        (queue->rear + 1U) % queue->capacity;

    queue->size++;

    return true;
}
```

### Dequeue

```c
static bool Queue_Dequeue(
    VertexQueue *queue,
    size_t *vertex
)
{
    if ((queue == NULL) ||
        (queue->data == NULL) ||
        (vertex == NULL) ||
        (queue->size == 0U))
    {
        return false;
    }

    *vertex = queue->data[queue->front];

    queue->front =
        (queue->front + 1U) % queue->capacity;

    queue->size--;

    return true;
}
```

---

## 10. Cài đặt BFS bằng C

Cấu trúc project:

```text
graph-bfs/
├── include/
│   └── graph.h
├── src/
│   ├── graph.c
│   └── main.c
└── README.md
```

---

### 10.1. File `graph.h`

```c
#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include <stddef.h>

typedef struct AdjNode
{
    size_t vertex;
    struct AdjNode *next;
} AdjNode;

typedef struct
{
    size_t vertex_count;
    AdjNode **adjacency;
} Graph;

/**
 * @brief Create a graph with vertex IDs from 0 to vertex_count - 1.
 */
Graph *Graph_Create(size_t vertex_count);

/**
 * @brief Add one directed edge: source -> destination.
 */
bool Graph_AddDirectedEdge(
    Graph *graph,
    size_t source,
    size_t destination
);

/**
 * @brief Add one undirected edge: source <-> destination.
 */
bool Graph_AddUndirectedEdge(
    Graph *graph,
    size_t source,
    size_t destination
);

/**
 * @brief Print the adjacency list.
 */
void Graph_Print(const Graph *graph);

/**
 * @brief Perform BFS from one start vertex.
 */
bool Graph_BFS(
    const Graph *graph,
    size_t start_vertex
);

/**
 * @brief Perform BFS over every connected component.
 */
bool Graph_BFSAll(const Graph *graph);

/**
 * @brief Free every node and the graph object.
 */
void Graph_Destroy(Graph *graph);

#endif
```

---

### 10.2. File `graph.c`

```c
#include "graph.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    size_t *data;
    size_t capacity;
    size_t front;
    size_t rear;
    size_t size;
} VertexQueue;

static bool Queue_Init(
    VertexQueue *queue,
    size_t capacity
)
{
    if ((queue == NULL) || (capacity == 0U))
    {
        return false;
    }

    queue->data =
        (size_t *)malloc(capacity * sizeof(size_t));

    if (queue->data == NULL)
    {
        return false;
    }

    queue->capacity = capacity;
    queue->front = 0U;
    queue->rear = 0U;
    queue->size = 0U;

    return true;
}

static void Queue_Destroy(VertexQueue *queue)
{
    if (queue == NULL)
    {
        return;
    }

    free(queue->data);

    queue->data = NULL;
    queue->capacity = 0U;
    queue->front = 0U;
    queue->rear = 0U;
    queue->size = 0U;
}

static bool Queue_IsEmpty(const VertexQueue *queue)
{
    return (queue == NULL) || (queue->size == 0U);
}

static bool Queue_Enqueue(
    VertexQueue *queue,
    size_t vertex
)
{
    if ((queue == NULL) ||
        (queue->data == NULL) ||
        (queue->size == queue->capacity))
    {
        return false;
    }

    queue->data[queue->rear] = vertex;

    queue->rear =
        (queue->rear + 1U) % queue->capacity;

    queue->size++;

    return true;
}

static bool Queue_Dequeue(
    VertexQueue *queue,
    size_t *vertex
)
{
    if ((queue == NULL) ||
        (queue->data == NULL) ||
        (vertex == NULL) ||
        Queue_IsEmpty(queue))
    {
        return false;
    }

    *vertex = queue->data[queue->front];

    queue->front =
        (queue->front + 1U) % queue->capacity;

    queue->size--;

    return true;
}

static AdjNode *AdjNode_Create(size_t vertex)
{
    AdjNode *node =
        (AdjNode *)malloc(sizeof(AdjNode));

    if (node == NULL)
    {
        return NULL;
    }

    node->vertex = vertex;
    node->next = NULL;

    return node;
}

Graph *Graph_Create(size_t vertex_count)
{
    Graph *graph;

    if (vertex_count == 0U)
    {
        return NULL;
    }

    graph = (Graph *)malloc(sizeof(Graph));

    if (graph == NULL)
    {
        return NULL;
    }

    graph->adjacency = (AdjNode **)calloc(
        vertex_count,
        sizeof(AdjNode *)
    );

    if (graph->adjacency == NULL)
    {
        free(graph);
        return NULL;
    }

    graph->vertex_count = vertex_count;

    return graph;
}

bool Graph_AddDirectedEdge(
    Graph *graph,
    size_t source,
    size_t destination
)
{
    AdjNode *node;

    if ((graph == NULL) ||
        (source >= graph->vertex_count) ||
        (destination >= graph->vertex_count))
    {
        return false;
    }

    node = AdjNode_Create(destination);

    if (node == NULL)
    {
        return false;
    }

    node->next = graph->adjacency[source];
    graph->adjacency[source] = node;

    return true;
}

bool Graph_AddUndirectedEdge(
    Graph *graph,
    size_t source,
    size_t destination
)
{
    if (!Graph_AddDirectedEdge(
            graph,
            source,
            destination))
    {
        return false;
    }

    if (!Graph_AddDirectedEdge(
            graph,
            destination,
            source))
    {
        /*
         * For a production library, roll back the first insertion.
         * This example reports failure to keep the main idea focused.
         */
        return false;
    }

    return true;
}

void Graph_Print(const Graph *graph)
{
    size_t vertex;

    if (graph == NULL)
    {
        return;
    }

    for (vertex = 0U;
         vertex < graph->vertex_count;
         vertex++)
    {
        const AdjNode *current =
            graph->adjacency[vertex];

        printf("%zu:", vertex);

        while (current != NULL)
        {
            printf(" %zu", current->vertex);
            current = current->next;
        }

        printf("\n");
    }
}

static bool Graph_BFSComponent(
    const Graph *graph,
    size_t start_vertex,
    bool visited[]
)
{
    VertexQueue queue;
    size_t current_vertex;

    if ((graph == NULL) ||
        (visited == NULL) ||
        (start_vertex >= graph->vertex_count))
    {
        return false;
    }

    if (!Queue_Init(
            &queue,
            graph->vertex_count))
    {
        return false;
    }

    visited[start_vertex] = true;

    if (!Queue_Enqueue(
            &queue,
            start_vertex))
    {
        Queue_Destroy(&queue);
        return false;
    }

    while (Queue_Dequeue(
        &queue,
        &current_vertex))
    {
        const AdjNode *neighbor =
            graph->adjacency[current_vertex];

        printf("%zu ", current_vertex);

        while (neighbor != NULL)
        {
            size_t adjacent_vertex =
                neighbor->vertex;

            if (!visited[adjacent_vertex])
            {
                visited[adjacent_vertex] = true;

                if (!Queue_Enqueue(
                        &queue,
                        adjacent_vertex))
                {
                    Queue_Destroy(&queue);
                    return false;
                }
            }

            neighbor = neighbor->next;
        }
    }

    Queue_Destroy(&queue);

    return true;
}

bool Graph_BFS(
    const Graph *graph,
    size_t start_vertex
)
{
    bool *visited;
    bool success;

    if ((graph == NULL) ||
        (start_vertex >= graph->vertex_count))
    {
        return false;
    }

    visited = (bool *)calloc(
        graph->vertex_count,
        sizeof(bool)
    );

    if (visited == NULL)
    {
        return false;
    }

    success = Graph_BFSComponent(
        graph,
        start_vertex,
        visited
    );

    free(visited);

    return success;
}

bool Graph_BFSAll(const Graph *graph)
{
    bool *visited;
    size_t vertex;

    if (graph == NULL)
    {
        return false;
    }

    visited = (bool *)calloc(
        graph->vertex_count,
        sizeof(bool)
    );

    if (visited == NULL)
    {
        return false;
    }

    for (vertex = 0U;
         vertex < graph->vertex_count;
         vertex++)
    {
        if (!visited[vertex])
        {
            printf("[component from %zu] ", vertex);

            if (!Graph_BFSComponent(
                    graph,
                    vertex,
                    visited))
            {
                free(visited);
                return false;
            }

            printf("\n");
        }
    }

    free(visited);

    return true;
}

void Graph_Destroy(Graph *graph)
{
    size_t vertex;

    if (graph == NULL)
    {
        return;
    }

    for (vertex = 0U;
         vertex < graph->vertex_count;
         vertex++)
    {
        AdjNode *current =
            graph->adjacency[vertex];

        while (current != NULL)
        {
            AdjNode *next_node = current->next;

            free(current);
            current = next_node;
        }
    }

    free(graph->adjacency);
    free(graph);
}
```

---

### 10.3. File `main.c`

```c
#include "graph.h"

#include <stdio.h>

int main(void)
{
    Graph *graph = Graph_Create(6U);

    if (graph == NULL)
    {
        printf("Could not create graph.\n");
        return 1;
    }

    if (!Graph_AddUndirectedEdge(graph, 0U, 1U) ||
        !Graph_AddUndirectedEdge(graph, 0U, 2U) ||
        !Graph_AddUndirectedEdge(graph, 1U, 2U) ||
        !Graph_AddUndirectedEdge(graph, 1U, 3U) ||
        !Graph_AddUndirectedEdge(graph, 1U, 4U) ||
        !Graph_AddUndirectedEdge(graph, 2U, 4U) ||
        !Graph_AddUndirectedEdge(graph, 3U, 4U))
    {
        printf("Could not add graph edge.\n");
        Graph_Destroy(graph);
        return 1;
    }

    printf("Adjacency list:\n");
    Graph_Print(graph);

    printf("\nBFS starting from vertex 0:\n");

    if (!Graph_BFS(graph, 0U))
    {
        printf("BFS failed.\n");
        Graph_Destroy(graph);
        return 1;
    }

    printf("\n\nBFS over every component:\n");

    if (!Graph_BFSAll(graph))
    {
        printf("BFS-all failed.\n");
        Graph_Destroy(graph);
        return 1;
    }

    Graph_Destroy(graph);

    return 0;
}
```

---

## 11. Chạy chương trình

Biên dịch:

```bash
gcc -std=c11 \
    -Wall \
    -Wextra \
    -Wpedantic \
    -Iinclude \
    src/graph.c \
    src/main.c \
    -o graph_bfs_demo
```

Chạy:

```bash
./graph_bfs_demo
```

Kết quả có thể là:

```text
Adjacency list:
0: 2 1
1: 4 3 2 0
2: 4 1 0
3: 4 1
4: 3 2 1
5:

BFS starting from vertex 0:
0 2 1 4 3

BFS over every component:
[component from 0] 0 2 1 4 3
[component from 5] 5
```

Do Node mới được chèn ở đầu Adjacency List, thứ tự hàng xóm bị đảo so với thứ tự gọi `Graph_AddUndirectedEdge()`.

Các thứ tự BFS sau đều có thể hợp lệ tùy thứ tự hàng xóm:

```text
0 1 2 3 4
```

hoặc:

```text
0 2 1 4 3
```

---

## 12. BFS cho Graph không liên thông

Graph không liên thông có nhiều thành phần:

```text
Component 1: 0 — 1 — 2

Component 2: 3 — 4

Component 3: 5
```

Chạy BFS chỉ từ `0` sẽ không thăm:

```text
3, 4, 5
```

Để duyệt toàn bộ Graph:

```c
for (vertex = 0U;
     vertex < graph->vertex_count;
     vertex++)
{
    if (!visited[vertex])
    {
        BFS từ vertex;
    }
}
```

Mỗi lần bắt đầu BFS mới tương ứng với một connected component mới.

Ta cũng có thể dùng cách này để đếm số connected component:

```text
component_count++
```

mỗi khi gặp một đỉnh chưa visited trong vòng lặp ngoài.

---

## 13. Tính khoảng cách ngắn nhất

Trong Graph không trọng số, BFS tìm đường đi có số cạnh ít nhất từ nguồn đến mọi đỉnh có thể tới được.

Khởi tạo:

```text
distance[start] = 0
```

Khi phát hiện neighbor từ current:

```text
distance[neighbor] = distance[current] + 1
```

### Hàm minh họa

```c
#include <limits.h>

bool Graph_BFSDistance(
    const Graph *graph,
    size_t start,
    size_t distance[]
)
{
    bool *visited;
    VertexQueue queue;
    size_t vertex;

    if ((graph == NULL) ||
        (distance == NULL) ||
        (start >= graph->vertex_count))
    {
        return false;
    }

    visited = (bool *)calloc(
        graph->vertex_count,
        sizeof(bool)
    );

    if (visited == NULL)
    {
        return false;
    }

    if (!Queue_Init(
            &queue,
            graph->vertex_count))
    {
        free(visited);
        return false;
    }

    for (vertex = 0U;
         vertex < graph->vertex_count;
         vertex++)
    {
        distance[vertex] = SIZE_MAX;
    }

    visited[start] = true;
    distance[start] = 0U;
    Queue_Enqueue(&queue, start);

    while (Queue_Dequeue(&queue, &vertex))
    {
        const AdjNode *neighbor =
            graph->adjacency[vertex];

        while (neighbor != NULL)
        {
            size_t adjacent = neighbor->vertex;

            if (!visited[adjacent])
            {
                visited[adjacent] = true;

                distance[adjacent] =
                    distance[vertex] + 1U;

                Queue_Enqueue(
                    &queue,
                    adjacent
                );
            }

            neighbor = neighbor->next;
        }
    }

    Queue_Destroy(&queue);
    free(visited);

    return true;
}
```

Nếu:

```text
distance[v] == SIZE_MAX
```

thì `v` không thể đi tới từ `start`.

---

## 14. Khôi phục đường đi

Ngoài `distance`, lưu thêm:

```text
parent[neighbor] = current
```

Khi cần khôi phục đường đi từ `start` đến `target`:

```text
target
→ parent[target]
→ parent[parent[target]]
→ ...
→ start
```

Chuỗi này đang đi ngược, nên có thể dùng Stack để đảo lại.

Ví dụ:

```text
parent[4] = 2
parent[2] = 0
```

Đường ngược:

```text
4 → 2 → 0
```

Đường đúng:

```text
0 → 2 → 4
```

### Quy tắc

Khi phát hiện đỉnh mới:

```c
parent[adjacent] = current;
```

Đỉnh nguồn có thể dùng:

```c
parent[start] = start;
```

Hoặc một giá trị sentinel:

```text
SIZE_MAX
```

---

## 15. Độ phức tạp

Giả sử:

```text
V = số đỉnh
E = số cạnh
```

### Với Adjacency List

Mỗi đỉnh được enqueue và dequeue tối đa một lần:

```text
O(V)
```

Mỗi cạnh được duyệt qua danh sách kề.

Graph có hướng:

```text
O(E)
```

Graph vô hướng lưu mỗi cạnh hai lần:

```text
2E
```

Bỏ hằng số:

```text
O(E)
```

Tổng:

```text
O(V + E)
```

### Bộ nhớ

- `visited`: `O(V)`
- Queue: `O(V)`
- Adjacency List: `O(V + E)`

Bộ nhớ phụ riêng của BFS:

```text
O(V)
```

---

## 16. BFS với Adjacency Matrix

Adjacency Matrix:

```text
matrix[u][v] = 1 → có cạnh
matrix[u][v] = 0 → không có cạnh
```

Ví dụ:

```text
    0 1 2 3
0 [ 0 1 1 0 ]
1 [ 1 0 1 1 ]
2 [ 1 1 0 0 ]
3 [ 0 1 0 0 ]
```

Để tìm hàng xóm của một đỉnh, phải quét toàn bộ hàng:

```c
for (neighbor = 0U;
     neighbor < vertex_count;
     neighbor++)
{
    if (matrix[current][neighbor] != 0)
    {
        /* Edge exists. */
    }
}
```

Độ phức tạp:

```text
O(V²)
```

### So sánh

| Tiêu chí | Adjacency List | Adjacency Matrix |
|---|---:|---:|
| Bộ nhớ | `O(V + E)` | `O(V²)` |
| Duyệt hàng xóm | `O(degree(v))` | `O(V)` |
| Kiểm tra một cạnh | Thường `O(degree(v))` | `O(1)` |
| Graph thưa | Phù hợp | Lãng phí |
| Graph dày | Phù hợp | Có thể đơn giản hơn |
| Embedded kích thước nhỏ cố định | Cần quản lý Node | Dễ dùng mảng tĩnh |

---

## 17. So sánh BFS và DFS

| Tiêu chí | BFS | DFS |
|---|---|---|
| Cấu trúc chính | Queue | Stack hoặc recursion |
| Chiến lược | Theo tầng | Đi sâu theo nhánh |
| Đường ngắn nhất không trọng số | Có | Không đảm bảo |
| Bộ nhớ worst case | `O(V)` | `O(V)` |
| Phát hiện connected component | Có | Có |
| Phù hợp tìm mục tiêu gần nguồn | Tốt | Có thể đi quá sâu |
| Topological sort | Không trực tiếp, dùng Kahn | Có thể dùng DFS |
| Maze path bất kỳ | Có | Có |
| Maze shortest path | Có | Không đảm bảo |

### BFS

```text
Start
  |
  +--> mọi Node khoảng cách 1
  |
  +--> mọi Node khoảng cách 2
  |
  +--> mọi Node khoảng cách 3
```

### DFS

```text
Start
  |
  +--> đi sâu hết một nhánh
  |
  +--> backtrack
  |
  +--> sang nhánh khác
```

---

## 18. Ứng dụng của BFS

## 18.1. Đường đi ngắn nhất trong Graph không trọng số

BFS tìm đường dùng ít cạnh nhất.

Ví dụ:

- Mê cung mỗi bước có cùng chi phí.
- Bàn cờ.
- Chuyển đổi trạng thái.
- Social network hop count.

---

## 18.2. GPS và bản đồ dạng lưới

Nếu mọi bước di chuyển có cùng chi phí, BFS có thể tìm đường ngắn nhất trên grid.

Nếu cạnh có trọng số khác nhau, cần thuật toán như Dijkstra.

---

## 18.3. Level-Order Traversal

Tree được duyệt theo từng tầng.

```text
Level 0: root
Level 1: children
Level 2: grandchildren
```

---

## 18.4. Connected Components

Chạy BFS từ mỗi đỉnh chưa visited để phân tách các vùng liên thông.

---

## 18.5. Cycle Detection trong Graph vô hướng

Khi BFS, lưu `parent`.

Nếu gặp một neighbor đã visited và neighbor không phải parent của current, Graph chứa cycle.

---

## 18.6. Bipartite Graph

Tô màu theo hai nhóm:

```text
color[start] = 0
color[neighbor] = 1 - color[current]
```

Nếu hai đỉnh nối nhau có cùng màu, Graph không bipartite.

---

## 18.7. Ford–Fulkerson / Edmonds–Karp

BFS có thể tìm augmenting path trong residual graph.

Phiên bản Ford–Fulkerson dùng BFS thường được gọi là Edmonds–Karp.

---

## 18.8. Web Crawling và Search Index

BFS có thể duyệt các trang hoặc liên kết theo độ sâu tăng dần.

Cần bổ sung:

- Giới hạn depth.
- Deduplication.
- Rate limiting.
- Queue bền vững.
- Chính sách ưu tiên.

---

## 18.9. Social Network

Tìm:

- Người cách nhau một quan hệ.
- Bạn của bạn.
- Số bước kết nối.
- Người gần nhất thỏa điều kiện.

---

## 18.10. Broadcast trong Network

Một Node gửi thông tin đến toàn bộ Node có thể tới được theo từng hop.

---

## 18.11. Minimum Spanning Tree trên Graph không trọng số

BFS tạo một BFS tree bao phủ connected component.

BFS tree không phải lời giải MST tổng quát cho Graph có trọng số khác nhau.

Nếu mọi cạnh có cùng trọng số, mọi spanning tree có cùng tổng trọng số theo số cạnh.

---

## 19. BFS trong lập trình nhúng

## 19.1. Tránh cấp phát động

Mã desktop thường dùng:

```c
malloc()
free()
```

Firmware có thể dùng mảng tĩnh:

```c
#define MAX_VERTICES 32U

static bool visited[MAX_VERTICES];
static size_t queue_storage[MAX_VERTICES];
static uint8_t adjacency[MAX_VERTICES][MAX_VERTICES];
```

Ưu điểm:

- RAM tối đa biết trước.
- Không phân mảnh Heap.
- Thời gian dễ dự đoán.
- Không có lỗi cấp phát tại runtime.

---

## 19.2. Graph kích thước cố định

Ví dụ hệ thống có tối đa 16 trạng thái:

```text
State Transition Graph
```

Adjacency Matrix 16 × 16 bằng `uint8_t` cần:

```text
256 byte
```

Nếu dùng bitset:

```text
16 × 16 bit = 32 byte
```

---

## 19.3. Queue phải đủ lớn

Mỗi đỉnh chỉ enqueue một lần nếu đánh dấu đúng thời điểm.

Do đó:

```text
queue_capacity >= vertex_count
```

là đủ cho BFS chuẩn.

Nếu enqueue lặp do đánh dấu sai thời điểm, Queue có thể overflow.

---

## 19.4. Không xử lý BFS trong ISR dài

BFS có thể duyệt toàn bộ Graph:

```text
O(V + E)
```

Không nên chạy một BFS lớn trực tiếp trong interrupt.

ISR nên:

1. Ghi nhận event.
2. Đưa event vào Queue.
3. Đánh thức task hoặc main loop.
4. Xử lý BFS ngoài ISR.

---

## 19.5. Bounded Execution

Nếu cần giới hạn thời gian mỗi chu kỳ, có thể chia BFS thành nhiều bước.

Mỗi lần gọi:

```text
xử lý tối đa K đỉnh
```

Trạng thái Queue và `visited` được giữ lại cho lần sau.

Đây là dạng incremental BFS.

---

## 19.6. Grid và Robot

BFS có thể dùng cho robot trên lưới nhỏ:

```text
0 = ô trống
1 = vật cản
```

Mỗi cell là một vertex.

Hàng xóm:

```text
up
down
left
right
```

Cần tính RAM cho:

- Queue.
- Visited.
- Distance.
- Parent.
- Grid.

---

## 19.7. Timestamp Visited

Nếu chạy BFS nhiều lần, thay vì xóa toàn bộ `visited[]`, có thể dùng:

```c
uint16_t visit_generation[MAX_VERTICES];
uint16_t current_generation;
```

Đỉnh được xem là visited khi:

```c
visit_generation[v] == current_generation
```

Mỗi lần BFS:

```c
current_generation++;
```

Cần xử lý khi counter overflow.

---

## 20. Các lỗi thường gặp

## 20.1. Đánh dấu visited quá muộn

Sai:

```c
enqueue(neighbor);

/* Later, when dequeued: */
visited[neighbor] = true;
```

Một đỉnh có thể được enqueue nhiều lần.

Đúng:

```c
visited[neighbor] = true;
enqueue(neighbor);
```

---

## 20.2. Không kiểm tra Queue overflow

Queue cố định phải kiểm tra capacity.

```c
if (queue->size == queue->capacity)
{
    return false;
}
```

---

## 20.3. Chỉ BFS từ một đỉnh rồi cho rằng đã duyệt toàn Graph

Graph không liên thông cần vòng lặp ngoài.

---

## 20.4. Không kiểm tra vertex ID

Sai:

```c
adjacency[source]
```

khi:

```text
source >= vertex_count
```

Luôn xác thực `source` và `destination`.

---

## 20.5. Quên thêm cạnh ngược cho Graph vô hướng

Với cạnh:

```text
u — v
```

Adjacency List cần:

```text
u -> v
v -> u
```

Graph có hướng chỉ thêm:

```text
u -> v
```

---

## 20.6. Memory Leak

Mỗi `AdjNode` tạo bằng `malloc()` phải được giải phóng.

Queue và mảng `visited` cũng phải được free.

---

## 20.7. Nhầm BFS luôn có một thứ tự duy nhất

Thứ tự BFS phụ thuộc vào thứ tự hàng xóm.

Các kết quả khác nhau vẫn có thể đúng.

---

## 20.8. Dùng BFS cho Graph có trọng số tùy ý

BFS chỉ đảm bảo đường ngắn nhất theo số cạnh hoặc khi mọi cạnh có cùng trọng số.

Với trọng số không âm khác nhau:

```text
Dijkstra
```

Với cạnh trọng số `0` hoặc `1`:

```text
0–1 BFS với Deque
```

---

## 20.9. Nhầm discovered và processed

Một đỉnh có thể:

1. Chưa phát hiện.
2. Đã enqueue nhưng chưa dequeue.
3. Đã dequeue và xử lý.

`visited` trong BFS phổ biến thường có nghĩa là:

```text
đã discovered
```

không nhất thiết đã xử lý xong.

---

## 21. Bài tập luyện tập

### Cơ bản

1. BFS bằng Adjacency Matrix.
2. BFS bằng Adjacency List.
3. In thứ tự duyệt.
4. Đếm số đỉnh reachable.
5. Duyệt toàn bộ Graph không liên thông.
6. Đếm connected components.
7. Tính distance từ một source.
8. Tìm một target bằng BFS.
9. Level-order traversal Binary Tree.
10. BFS trên grid.

### Trung bình

1. Khôi phục shortest path.
2. Kiểm tra bipartite graph.
3. Cycle detection trong Graph vô hướng.
4. Number of Islands.
5. Flood Fill.
6. Rotten Oranges.
7. Shortest Path in Binary Matrix.
8. Word Ladder.
9. Minimum Knight Moves.
10. Multi-source BFS.
11. Nearest `1` for Every Cell.
12. Open the Lock.

### Nâng cao

1. 0–1 BFS.
2. Bidirectional BFS.
3. Edmonds–Karp Maximum Flow.
4. Kahn Topological Sort.
5. Minimum Genetic Mutation.
6. State-space BFS.
7. Parallel BFS.
8. External-memory BFS.
9. Incremental BFS trong firmware.
10. BFS với Memory Pool và Queue tĩnh.
11. BFS trên compressed graph.
12. BFS với bitset adjacency.

---

## 22. Tổng kết

BFS duyệt Graph theo từng tầng.

Cấu trúc chính:

```text
Visited Array
      +
FIFO Queue
```

Quy trình:

```text
Đánh dấu start
→ Enqueue start
→ Dequeue một đỉnh
→ Thăm các hàng xóm chưa visited
→ Đánh dấu và enqueue hàng xóm
→ Lặp đến khi Queue rỗng
```

Độ phức tạp với Adjacency List:

```text
Time:  O(V + E)
Space: O(V)
```

BFS phù hợp với:

- Duyệt theo tầng.
- Shortest path trong Graph không trọng số.
- Connected components.
- Cycle detection.
- Bipartite checking.
- Grid pathfinding.
- Network hop.
- Level-order traversal.
- Ford–Fulkerson dạng Edmonds–Karp.

Các điểm quan trọng:

1. Đánh dấu visited trước khi enqueue.
2. Queue cần đủ capacity.
3. Graph không liên thông cần vòng lặp ngoài.
4. Thứ tự BFS phụ thuộc thứ tự hàng xóm.
5. BFS không thay thế Dijkstra cho trọng số tùy ý.
6. Embedded nên cân nhắc mảng tĩnh và giới hạn worst case.
7. Phải giải phóng Graph, Queue và visited nếu dùng cấp phát động.

---

## 23. Tài liệu tham khảo

README này được biên soạn và diễn đạt lại độc lập dựa trên phạm vi chủ đề được trình bày trong:

- Programiz — **Breadth First Search**:  
  https://www.programiz.com/dsa/graph-bfs
