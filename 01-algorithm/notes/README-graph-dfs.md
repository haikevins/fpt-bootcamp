# Depth-First Search (DFS) for Graphs

> Tài liệu học tập độc lập về **Depth-First Search**, trình bày nguyên lý duyệt đồ thị theo chiều sâu, cách dùng recursion hoặc Stack, mã nguồn C hoàn chỉnh, độ phức tạp và các ứng dụng quan trọng.

---

## Mục lục

1. [DFS là gì?](#1-dfs-là-gì)
2. [Ý tưởng cốt lõi](#2-ý-tưởng-cốt-lõi)
3. [Tại sao DFS cần Visited?](#3-tại-sao-dfs-cần-visited)
4. [DFS dùng Recursion và Stack](#4-dfs-dùng-recursion-và-stack)
5. [Các bước của thuật toán](#5-các-bước-của-thuật-toán)
6. [Ví dụ từng bước](#6-ví-dụ-từng-bước)
7. [Pseudocode](#7-pseudocode)
8. [Biểu diễn Graph bằng Adjacency List](#8-biểu-diễn-graph-bằng-adjacency-list)
9. [Cài đặt DFS đệ quy bằng C](#9-cài-đặt-dfs-đệ-quy-bằng-c)
10. [Cài đặt DFS lặp bằng Stack](#10-cài-đặt-dfs-lặp-bằng-stack)
11. [Chạy chương trình](#11-chạy-chương-trình)
12. [DFS cho Graph không liên thông](#12-dfs-cho-graph-không-liên-thông)
13. [DFS Tree và Parent](#13-dfs-tree-và-parent)
14. [Phát hiện Cycle](#14-phát-hiện-cycle)
15. [Độ phức tạp](#15-độ-phức-tạp)
16. [DFS với Adjacency Matrix](#16-dfs-với-adjacency-matrix)
17. [So sánh DFS và BFS](#17-so-sánh-dfs-và-bfs)
18. [Ứng dụng của DFS](#18-ứng-dụng-của-dfs)
19. [DFS trong lập trình nhúng](#19-dfs-trong-lập-trình-nhúng)
20. [Các lỗi thường gặp](#20-các-lỗi-thường-gặp)
21. [Bài tập luyện tập](#21-bài-tập-luyện-tập)
22. [Tổng kết](#22-tổng-kết)
23. [Tài liệu tham khảo](#23-tài-liệu-tham-khảo)

---

## 1. DFS là gì?

**Depth-First Search**, viết tắt là **DFS**, là thuật toán duyệt Graph hoặc Tree bằng cách đi sâu nhất có thể theo một nhánh trước khi quay lui.

Ví dụ:

```text
            0
          /   \
         1     2
        / \     \
       3   4     5
```

Một thứ tự DFS có thể là:

```text
0, 1, 3, 4, 2, 5
```

Luồng suy nghĩ:

```text
Bắt đầu ở 0
→ đi sang 1
→ đi sâu xuống 3
→ 3 không còn nhánh mới
→ quay lui về 1
→ đi sang 4
→ quay lui về 0
→ đi sang 2
→ đi sang 5
```

DFS không nhất thiết có một thứ tự duy nhất. Kết quả phụ thuộc vào thứ tự các hàng xóm trong Graph.

---

## 2. Ý tưởng cốt lõi

DFS sử dụng:

```text
Visited Array
      +
Call Stack hoặc Stack tường minh
```

Mỗi lần đến một đỉnh:

1. Đánh dấu đỉnh là đã visited.
2. Xử lý đỉnh.
3. Chọn một hàng xóm chưa visited.
4. Đi sâu vào hàng xóm đó.
5. Khi không còn hàng xóm chưa visited, quay lui.

Khác với BFS:

```text
BFS → duyệt theo tầng
DFS → đi sâu theo nhánh
```

---

## 3. Tại sao DFS cần Visited?

Graph có thể chứa cycle:

```text
0 → 1
↑   |
|   v
3 ← 2
```

Nếu không có `visited`, DFS có thể lặp:

```text
0 → 1 → 2 → 3 → 0 → ...
```

Do đó, ngay khi bắt đầu xử lý một đỉnh:

```c
visited[vertex] = true;
```

Mọi lần gặp lại đỉnh này đều phải bỏ qua:

```c
if (!visited[neighbor])
{
    DFS(neighbor);
}
```

---

## 4. DFS dùng Recursion và Stack

DFS có hai cách triển khai phổ biến.

## 4.1. Recursion

Recursion dùng Call Stack của chương trình.

```c
DFS(vertex)
{
    mark vertex visited

    for each neighbor:
        if neighbor not visited:
            DFS(neighbor)
}
```

Call Stack tự lưu:

- Đỉnh hiện tại.
- Vị trí đang duyệt trong danh sách hàng xóm.
- Điểm quay lại sau lời gọi con.

---

## 4.2. Stack tường minh

Không dùng recursion, ta tự tạo Stack:

```text
Push start vertex

while Stack không rỗng:
    Pop một đỉnh
    Nếu chưa visited:
        Đánh dấu visited
        Push các hàng xóm chưa visited
```

Cách này hữu ích khi:

- Không muốn dùng recursion.
- Stack của MCU hoặc RTOS task nhỏ.
- Cần kiểm soát memory rõ ràng.
- Coding standard cấm recursion.

---

## 5. Các bước của thuật toán

Giả sử bắt đầu từ `start`.

### Bước 1

Khởi tạo:

```text
visited[v] = false
```

cho mọi đỉnh.

### Bước 2

Gọi:

```text
DFS(start)
```

### Bước 3

Trong `DFS(vertex)`:

```text
visited[vertex] = true
xử lý vertex
```

### Bước 4

Duyệt hàng xóm:

```text
for each neighbor:
    if neighbor chưa visited:
        DFS(neighbor)
```

### Bước 5

Khi một đỉnh không còn hàng xóm chưa visited, hàm trả về và quay lại đỉnh cha.

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

Giả sử thứ tự hàng xóm được duyệt là:

```text
0: 1, 2
1: 0, 2, 3, 4
2: 0, 1, 4
3: 1, 4
4: 1, 2, 3
```

Bắt đầu từ:

```text
start = 0
```

### Thăm 0

```text
visited = [true, false, false, false, false]
order   = [0]
```

Hàng xóm đầu tiên chưa visited:

```text
1
```

Đi sâu vào `1`.

---

### Thăm 1

```text
visited = [true, true, false, false, false]
order   = [0, 1]
```

Hàng xóm:

```text
0 → đã visited
2 → chưa visited
```

Đi sâu vào `2`.

---

### Thăm 2

```text
visited = [true, true, true, false, false]
order   = [0, 1, 2]
```

Hàng xóm:

```text
0 → visited
1 → visited
4 → chưa visited
```

Đi sâu vào `4`.

---

### Thăm 4

```text
visited = [true, true, true, false, true]
order   = [0, 1, 2, 4]
```

Hàng xóm:

```text
1 → visited
2 → visited
3 → chưa visited
```

Đi sâu vào `3`.

---

### Thăm 3

```text
visited = [true, true, true, true, true]
order   = [0, 1, 2, 4, 3]
```

Mọi hàng xóm đã visited.

Quay lui:

```text
3 → 4 → 2 → 1 → 0
```

Thứ tự DFS:

```text
0, 1, 2, 4, 3
```

> Nếu thứ tự hàng xóm khác, DFS có thể cho một thứ tự khác nhưng vẫn hợp lệ.

---

## 7. Pseudocode

### 7.1. DFS đệ quy

```text
DFS(graph, vertex):

    visited[vertex] = true
    process(vertex)

    for each neighbor of vertex:

        if visited[neighbor] == false:

            DFS(graph, neighbor)
```

### 7.2. Duyệt toàn bộ Graph

```text
for each vertex:

    visited[vertex] = false

for each vertex:

    if visited[vertex] == false:

        DFS(graph, vertex)
```

Vòng lặp ngoài là cần thiết vì Graph có thể không liên thông.

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

### Node

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

## 9. Cài đặt DFS đệ quy bằng C

Cấu trúc project:

```text
graph-dfs/
├── include/
│   └── graph.h
├── src/
│   ├── graph.c
│   └── main.c
└── README.md
```

---

### 9.1. File `graph.h`

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

Graph *Graph_Create(size_t vertex_count);

bool Graph_AddDirectedEdge(
    Graph *graph,
    size_t source,
    size_t destination
);

bool Graph_AddUndirectedEdge(
    Graph *graph,
    size_t source,
    size_t destination
);

void Graph_Print(const Graph *graph);

bool Graph_DFSRecursive(
    const Graph *graph,
    size_t start_vertex
);

bool Graph_DFSIterative(
    const Graph *graph,
    size_t start_vertex
);

bool Graph_DFSAll(const Graph *graph);

void Graph_Destroy(Graph *graph);

#endif
```

---

### 9.2. File `graph.c`

```c
#include "graph.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    size_t *data;
    size_t capacity;
    size_t size;
} VertexStack;

static bool Stack_Init(
    VertexStack *stack,
    size_t capacity
)
{
    if ((stack == NULL) || (capacity == 0U))
    {
        return false;
    }

    stack->data =
        (size_t *)malloc(capacity * sizeof(size_t));

    if (stack->data == NULL)
    {
        return false;
    }

    stack->capacity = capacity;
    stack->size = 0U;

    return true;
}

static void Stack_Destroy(VertexStack *stack)
{
    if (stack == NULL)
    {
        return;
    }

    free(stack->data);

    stack->data = NULL;
    stack->capacity = 0U;
    stack->size = 0U;
}

static bool Stack_Push(
    VertexStack *stack,
    size_t vertex
)
{
    if ((stack == NULL) ||
        (stack->data == NULL) ||
        (stack->size == stack->capacity))
    {
        return false;
    }

    stack->data[stack->size] = vertex;
    stack->size++;

    return true;
}

static bool Stack_Pop(
    VertexStack *stack,
    size_t *vertex
)
{
    if ((stack == NULL) ||
        (stack->data == NULL) ||
        (vertex == NULL) ||
        (stack->size == 0U))
    {
        return false;
    }

    stack->size--;
    *vertex = stack->data[stack->size];

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
         * Production code should roll back
         * the first insertion on failure.
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

static void DFSRecursiveVisit(
    const Graph *graph,
    size_t vertex,
    bool visited[]
)
{
    const AdjNode *neighbor;

    visited[vertex] = true;

    printf("%zu ", vertex);

    neighbor = graph->adjacency[vertex];

    while (neighbor != NULL)
    {
        size_t adjacent = neighbor->vertex;

        if (!visited[adjacent])
        {
            DFSRecursiveVisit(
                graph,
                adjacent,
                visited
            );
        }

        neighbor = neighbor->next;
    }
}

bool Graph_DFSRecursive(
    const Graph *graph,
    size_t start_vertex
)
{
    bool *visited;

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

    DFSRecursiveVisit(
        graph,
        start_vertex,
        visited
    );

    free(visited);

    return true;
}

bool Graph_DFSIterative(
    const Graph *graph,
    size_t start_vertex
)
{
    bool *visited;
    VertexStack stack;
    size_t current;

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

    if (!Stack_Init(
            &stack,
            graph->vertex_count))
    {
        free(visited);
        return false;
    }

    if (!Stack_Push(
            &stack,
            start_vertex))
    {
        Stack_Destroy(&stack);
        free(visited);
        return false;
    }

    while (Stack_Pop(&stack, &current))
    {
        const AdjNode *neighbor;

        if (visited[current])
        {
            continue;
        }

        visited[current] = true;
        printf("%zu ", current);

        neighbor = graph->adjacency[current];

        while (neighbor != NULL)
        {
            size_t adjacent = neighbor->vertex;

            if (!visited[adjacent])
            {
                if (!Stack_Push(
                        &stack,
                        adjacent))
                {
                    Stack_Destroy(&stack);
                    free(visited);
                    return false;
                }
            }

            neighbor = neighbor->next;
        }
    }

    Stack_Destroy(&stack);
    free(visited);

    return true;
}

bool Graph_DFSAll(const Graph *graph)
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

            DFSRecursiveVisit(
                graph,
                vertex,
                visited
            );

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

### 9.3. File `main.c`

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

    printf("\nRecursive DFS from vertex 0:\n");

    if (!Graph_DFSRecursive(graph, 0U))
    {
        printf("Recursive DFS failed.\n");
        Graph_Destroy(graph);
        return 1;
    }

    printf("\n\nIterative DFS from vertex 0:\n");

    if (!Graph_DFSIterative(graph, 0U))
    {
        printf("Iterative DFS failed.\n");
        Graph_Destroy(graph);
        return 1;
    }

    printf("\n\nDFS over every component:\n");

    if (!Graph_DFSAll(graph))
    {
        printf("DFS-all failed.\n");
        Graph_Destroy(graph);
        return 1;
    }

    Graph_Destroy(graph);

    return 0;
}
```

---

## 10. Cài đặt DFS lặp bằng Stack

Bản lặp dùng:

```text
Explicit Stack
```

Quy trình:

```text
Push start

while Stack không rỗng:
    current = Pop

    nếu current chưa visited:
        đánh dấu visited
        xử lý current
        Push các neighbor chưa visited
```

### Đánh dấu khi push hay khi pop?

Có hai lựa chọn.

#### Đánh dấu khi pop

```c
if (visited[current])
{
    continue;
}

visited[current] = true;
```

Ưu điểm:

- Code đơn giản.
- Có thể giữ thứ tự gần giống một số mô hình recursive.

Hạn chế:

- Một đỉnh có thể bị push nhiều lần.

#### Đánh dấu khi push

```c
visited[neighbor] = true;
Stack_Push(neighbor);
```

Ưu điểm:

- Mỗi đỉnh chỉ push một lần.
- Stack capacity bằng `V` là đủ.

Hạn chế:

- Cần đánh dấu `start` trước khi push.
- Thứ tự phải được thiết kế cẩn thận nếu muốn giống recursive DFS.

Trong firmware có Stack cố định, đánh dấu khi push thường dễ kiểm soát capacity hơn.

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
    -o graph_dfs_demo
```

Chạy:

```bash
./graph_dfs_demo
```

Một kết quả có thể là:

```text
Adjacency list:
0: 2 1
1: 4 3 2 0
2: 4 1 0
3: 4 1
4: 3 2 1
5:

Recursive DFS from vertex 0:
0 2 4 3 1

Iterative DFS from vertex 0:
0 1 3 4 2

DFS over every component:
[component from 0] 0 2 4 3 1
[component from 5] 5
```

Recursive và iterative DFS có thể cho thứ tự khác nhau do:

- Thứ tự hàng xóm.
- Thứ tự push vào Stack.
- Stack là LIFO.

Cả hai vẫn hợp lệ nếu mỗi đỉnh reachable được thăm đúng một lần.

---

## 12. DFS cho Graph không liên thông

Graph:

```text
Component 1:
0 — 1 — 2

Component 2:
3 — 4

Component 3:
5
```

DFS từ `0` chỉ thăm:

```text
0, 1, 2
```

Để duyệt toàn bộ Graph:

```c
for (vertex = 0U;
     vertex < graph->vertex_count;
     vertex++)
{
    if (!visited[vertex])
    {
        DFSRecursiveVisit(
            graph,
            vertex,
            visited
        );
    }
}
```

Mỗi lần gọi DFS mới từ một đỉnh chưa visited biểu diễn một connected component mới.

Đếm component:

```c
component_count++;
```

---

## 13. DFS Tree và Parent

Trong quá trình DFS, khi đi từ:

```text
current → neighbor
```

lần đầu tiên, ta có thể lưu:

```c
parent[neighbor] = current;
```

Các cạnh này tạo thành một **DFS Tree** hoặc **DFS Forest** nếu Graph không liên thông.

Ví dụ:

```text
parent[0] = SIZE_MAX
parent[1] = 0
parent[2] = 1
parent[4] = 2
parent[3] = 4
```

DFS Tree:

```text
0
|
1
|
2
|
4
|
3
```

Parent hữu ích để:

- Khôi phục đường đi.
- Phát hiện cycle trong Graph vô hướng.
- Phân tích cấu trúc traversal.
- Tạo spanning forest.

---

## 14. Phát hiện Cycle

## 14.1. Graph vô hướng

Trong Graph vô hướng, nếu từ `current` gặp một neighbor:

```text
đã visited
và
neighbor != parent[current]
```

thì có cycle.

Pseudo-code:

```text
DFS(current, parent):

    visited[current] = true

    for each neighbor:

        if neighbor chưa visited:

            if DFS(neighbor, current) phát hiện cycle:
                return true

        else if neighbor != parent:
            return true

    return false
```

---

## 14.2. Graph có hướng

Cần phân biệt ba trạng thái:

```text
WHITE → chưa thăm
GRAY  → đang nằm trên recursion stack
BLACK → đã xử lý xong
```

Nếu gặp cạnh:

```text
current → neighbor
```

và:

```text
color[neighbor] == GRAY
```

thì có back edge và Graph chứa cycle.

---

## 15. Độ phức tạp

Giả sử:

```text
V = số đỉnh
E = số cạnh
```

### Adjacency List

Mỗi đỉnh được đánh dấu visited một lần:

```text
O(V)
```

Mỗi cạnh được xét trong danh sách kề:

```text
O(E)
```

Tổng:

```text
O(V + E)
```

### Bộ nhớ phụ

- `visited`: `O(V)`
- Recursion Stack hoặc explicit Stack: `O(V)` trong worst case.

Do đó:

```text
Space = O(V)
```

### Recursion depth

Worst case Graph dạng chuỗi:

```text
0 → 1 → 2 → 3 → ... → V-1
```

Độ sâu recursion:

```text
O(V)
```

---

## 16. DFS với Adjacency Matrix

Adjacency Matrix:

```text
matrix[u][v] = 1 → có cạnh
matrix[u][v] = 0 → không có cạnh
```

Để tìm hàng xóm của `current`, phải quét toàn bộ hàng:

```c
for (neighbor = 0U;
     neighbor < vertex_count;
     neighbor++)
{
    if ((matrix[current][neighbor] != 0) &&
        (!visited[neighbor]))
    {
        DFS(neighbor);
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
| Kiểm tra cạnh | Thường `O(degree(v))` | `O(1)` |
| Graph thưa | Tốt | Lãng phí |
| Graph dày | Tốt | Có thể đơn giản |
| Mảng tĩnh embedded | Cần quản lý list | Dễ kiểm soát |

---

## 17. So sánh DFS và BFS

| Tiêu chí | DFS | BFS |
|---|---|---|
| Cấu trúc chính | Stack hoặc recursion | Queue |
| Chiến lược | Đi sâu trước | Theo tầng |
| Shortest path không trọng số | Không đảm bảo | Có |
| Connected components | Có | Có |
| Cycle detection | Rất phù hợp | Có |
| Topological sort | Phù hợp | Kahn dùng BFS |
| Strongly connected components | Có | Không phải cách chuẩn |
| Backtracking | Rất phù hợp | Không tự nhiên |
| Memory trên Graph rộng | Có thể thấp hơn BFS | Có thể lớn |
| Memory trên Graph sâu | Recursion có thể sâu | Queue có thể nhỏ hơn |

### DFS

```text
Đi sâu một nhánh
→ hết đường
→ quay lui
```

### BFS

```text
Xử lý toàn bộ tầng hiện tại
→ chuyển sang tầng tiếp theo
```

---

## 18. Ứng dụng của DFS

## 18.1. Tìm đường đi

DFS có thể tìm một đường đi bất kỳ từ source đến target.

Không đảm bảo đó là đường ngắn nhất.

---

## 18.2. Connected Components

Mỗi lần bắt đầu DFS từ một đỉnh chưa visited là một component mới.

---

## 18.3. Cycle Detection

DFS rất phù hợp để phát hiện:

- Cycle trong Graph vô hướng.
- Back edge trong Graph có hướng.

---

## 18.4. Topological Sorting

Với Directed Acyclic Graph:

1. DFS một đỉnh.
2. Sau khi xử lý toàn bộ neighbor, đưa đỉnh vào Stack.
3. Đảo thứ tự hoàn tất.

---

## 18.5. Strongly Connected Components

DFS được sử dụng trong:

- Kosaraju Algorithm.
- Tarjan Algorithm.

---

## 18.6. Kiểm tra Bipartite

Có thể tô màu Graph bằng DFS:

```text
color[neighbor] = 1 - color[current]
```

Nếu hai đỉnh nối nhau có cùng màu, Graph không bipartite.

---

## 18.7. Maze và Backtracking

DFS phù hợp với:

- Mê cung.
- N-Queens.
- Sudoku.
- Sinh permutation.
- State-space exploration.
- Puzzle.

---

## 18.8. Articulation Point và Bridge

DFS timestamp hỗ trợ tìm:

- Articulation point.
- Bridge.
- Biconnected component.

Các biến thường dùng:

```text
discovery_time
low_link
parent
```

---

## 18.9. Spanning Tree

DFS tạo DFS spanning tree cho mỗi connected component.

---

## 18.10. Dependency Analysis

Có thể dùng DFS để:

- Phát hiện dependency cycle.
- Xác định thứ tự build.
- Duyệt cây module.
- Kiểm tra import graph.

---

## 19. DFS trong lập trình nhúng

## 19.1. Hạn chế Recursion

Firmware có Stack nhỏ.

Graph dạng chuỗi có thể tạo:

```text
recursion depth = V
```

Nếu mỗi stack frame dùng nhiều byte, nguy cơ Stack Overflow tăng nhanh.

Giải pháp:

- Dùng iterative DFS.
- Dùng Stack tĩnh.
- Giới hạn số đỉnh.
- Đo high-water mark.
- Không đặt buffer lớn trong hàm recursive.

---

## 19.2. Stack tĩnh

```c
#define MAX_VERTICES 32U

static size_t stack_storage[MAX_VERTICES];
static bool visited[MAX_VERTICES];
```

Không cần `malloc()`.

Ưu điểm:

- RAM xác định trước.
- Không phân mảnh Heap.
- Dễ kiểm tra overflow.
- Thời gian dễ dự đoán.

---

## 19.3. Graph tĩnh

Adjacency Matrix:

```c
static uint8_t adjacency[MAX_VERTICES][MAX_VERTICES];
```

Hoặc adjacency list bằng mảng index thay con trỏ.

Ví dụ:

```c
typedef struct
{
    uint8_t destination;
    int16_t next_index;
} EdgeNode;
```

Cách này tránh pointer và dynamic allocation.

---

## 19.4. Không chạy DFS lớn trong ISR

DFS có worst-case:

```text
O(V + E)
```

Không nên duyệt Graph lớn trong interrupt.

ISR nên:

1. Ghi nhận sự kiện.
2. Đưa event vào Queue.
3. Để main hoặc task chạy DFS.

---

## 19.5. Incremental DFS

Nếu cần chia nhỏ thời gian xử lý:

```text
Mỗi chu kỳ xử lý tối đa K đỉnh
```

Giữ lại:

- Explicit Stack.
- Visited array.
- Trạng thái traversal.

Điều này phù hợp super loop hoặc cooperative scheduler.

---

## 19.6. State Machine Graph

DFS có thể dùng để kiểm tra:

- State unreachable.
- Transition cycle.
- Dependency loop.
- Tất cả trạng thái có thể tới từ initial state hay không.

---

## 19.7. Bounded Worst Case

Nếu:

```text
V <= 16
E <= 32
```

ta có thể ước lượng số thao tác tối đa.

Điều này hữu ích trong hệ thống real-time.

---

## 20. Các lỗi thường gặp

## 20.1. Không đánh dấu visited trước khi recursion

Sai:

```c
for each neighbor:
    if !visited[neighbor]:
        DFS(neighbor)

visited[current] = true;
```

Cycle có thể làm recursion vô hạn.

Đúng:

```c
visited[current] = true;
```

trước khi đi vào neighbor.

---

## 20.2. Quên xử lý Graph không liên thông

DFS từ một source chỉ thăm component chứa source.

---

## 20.3. Stack Overflow do recursion sâu

Graph dạng chain có thể gây hàng nghìn lời gọi lồng nhau.

Dùng iterative DFS khi depth lớn hoặc không kiểm soát được.

---

## 20.4. Không kiểm tra Stack overflow

Explicit Stack cố định phải kiểm tra:

```c
stack->size == stack->capacity
```

---

## 20.5. Nhầm Graph có hướng và vô hướng

Graph vô hướng:

```text
u → v
v → u
```

Graph có hướng:

```text
u → v
```

Không tự thêm cạnh ngược nếu Graph là directed.

---

## 20.6. Memory Leak

Phải giải phóng:

- Mọi AdjNode.
- Mảng adjacency.
- Visited.
- Explicit Stack.
- Graph object.

---

## 20.7. Nhầm DFS đảm bảo shortest path

DFS tìm một đường, không đảm bảo ít cạnh nhất.

Graph không trọng số cần shortest path:

```text
BFS
```

---

## 20.8. Thứ tự iterative khác recursive

Nếu muốn iterative DFS giống recursive, phải push neighbor theo thứ tự ngược.

Ví dụ recursive duyệt:

```text
1, 2, 3
```

Stack nên push:

```text
3, 2, 1
```

để `1` được pop trước.

---

## 20.9. Đánh dấu visited quá muộn trong iterative DFS

Nếu chỉ đánh dấu khi pop, một đỉnh có thể push nhiều lần.

Điều này không sai nếu có kiểm tra lúc pop, nhưng làm tăng nhu cầu Stack.

---

## 20.10. Dùng DFS cho Graph thay đổi đồng thời

Nếu ISR, DMA hoặc task khác thay đổi adjacency trong khi DFS chạy, traversal có thể:

- Bỏ sót cạnh.
- Đọc con trỏ không hợp lệ.
- Lặp sai.
- Crash.

Cần snapshot, lock hoặc ownership rõ ràng.

---

## 21. Bài tập luyện tập

### Cơ bản

1. DFS đệ quy bằng Adjacency Matrix.
2. DFS đệ quy bằng Adjacency List.
3. DFS lặp bằng Stack.
4. In thứ tự traversal.
5. Đếm số đỉnh reachable.
6. Đếm connected components.
7. Tìm target bằng DFS.
8. Khôi phục một đường đi.
9. Duyệt toàn Graph không liên thông.
10. DFS trên Tree.

### Trung bình

1. Cycle detection Graph vô hướng.
2. Cycle detection Graph có hướng.
3. Bipartite checking.
4. Topological sort.
5. Number of Islands.
6. Flood Fill bằng DFS.
7. Maze path.
8. Count paths.
9. Clone Graph.
10. Surrounded Regions.
11. Evaluate Division.
12. Word Search.

### Nâng cao

1. Kosaraju SCC.
2. Tarjan SCC.
3. Articulation Points.
4. Bridges.
5. Biconnected Components.
6. Euler Tour.
7. Heavy-Light Decomposition.
8. DFS timestamp.
9. Iterative Tarjan.
10. Incremental DFS trong firmware.
11. DFS trên compressed adjacency.
12. Parallel DFS.

---

## 22. Tổng kết

DFS duyệt Graph bằng cách đi sâu theo một nhánh trước khi quay lui.

Cấu trúc chính:

```text
Visited Array
      +
Recursion hoặc Explicit Stack
```

Quy trình:

```text
Đánh dấu vertex
→ xử lý vertex
→ chọn neighbor chưa visited
→ đi sâu vào neighbor
→ hết neighbor thì quay lui
```

Độ phức tạp với Adjacency List:

```text
Time:  O(V + E)
Space: O(V)
```

DFS phù hợp với:

- Tìm một đường đi.
- Connected components.
- Cycle detection.
- Topological sorting.
- Strongly connected components.
- Bipartite checking.
- Maze và backtracking.
- Articulation point và bridge.
- Dependency analysis.

Các điểm cần nhớ:

1. Đánh dấu visited trước khi đi sâu.
2. Graph không liên thông cần vòng lặp ngoài.
3. Recursive DFS dùng Call Stack.
4. Iterative DFS dùng Stack tường minh.
5. DFS không đảm bảo shortest path.
6. Thứ tự traversal phụ thuộc thứ tự hàng xóm.
7. Embedded nên cân nhắc Stack tĩnh và tránh recursion sâu.
8. Phải giải phóng tài nguyên nếu dùng cấp phát động.

---

## 23. Tài liệu tham khảo

README này được biên soạn và diễn đạt lại độc lập dựa trên phạm vi chủ đề được trình bày trong:

- Programiz — **Depth First Search (DFS)**

Trang nguồn:

<https://www.programiz.com/dsa/graph-dfs>

README không phải là bản sao nguyên văn của nội dung nguồn.
