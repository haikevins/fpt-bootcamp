# Dijkstra's Shortest Path Algorithm

> Tài liệu học tập độc lập về **Dijkstra's Algorithm**, trình bày nguyên lý tham lam, thao tác relaxation, cách khôi phục đường đi và mã nguồn C hoàn chỉnh.

---

## Mục lục

1. [Dijkstra là gì?](#1-dijkstra-là-gì)
2. [Bài toán Single-Source Shortest Path](#2-bài-toán-single-source-shortest-path)
3. [Điều kiện áp dụng](#3-điều-kiện-áp-dụng)
4. [Tính chất đường đi ngắn nhất](#4-tính-chất-đường-đi-ngắn-nhất)
5. [Ý tưởng tham lam](#5-ý-tưởng-tham-lam)
6. [Các cấu trúc dữ liệu cần dùng](#6-các-cấu-trúc-dữ-liệu-cần-dùng)
7. [Relaxation là gì?](#7-relaxation-là-gì)
8. [Các bước của thuật toán](#8-các-bước-của-thuật-toán)
9. [Ví dụ từng bước](#9-ví-dụ-từng-bước)
10. [Pseudocode](#10-pseudocode)
11. [Cài đặt bằng Adjacency Matrix](#11-cài-đặt-bằng-adjacency-matrix)
12. [Mã C hoàn chỉnh](#12-mã-c-hoàn-chỉnh)
13. [Khôi phục đường đi ngắn nhất](#13-khôi-phục-đường-đi-ngắn-nhất)
14. [Kết quả chạy thử](#14-kết-quả-chạy-thử)
15. [Dijkstra với Adjacency List và Min Heap](#15-dijkstra-với-adjacency-list-và-min-heap)
16. [Độ phức tạp](#16-độ-phức-tạp)
17. [Tại sao Dijkstra không dùng được với cạnh âm?](#17-tại-sao-dijkstra-không-dùng-được-với-cạnh-âm)
18. [Graph không liên thông](#18-graph-không-liên-thông)
19. [So sánh với các thuật toán khác](#19-so-sánh-với-các-thuật-toán-khác)
20. [Ứng dụng](#20-ứng-dụng)
21. [Dijkstra trong lập trình nhúng](#21-dijkstra-trong-lập-trình-nhúng)
22. [Các lỗi thường gặp](#22-các-lỗi-thường-gặp)
23. [Bài tập luyện tập](#23-bài-tập-luyện-tập)
24. [Tổng kết](#24-tổng-kết)
25. [Tài liệu tham khảo](#25-tài-liệu-tham-khảo)

---

## 1. Dijkstra là gì?

**Dijkstra's Algorithm** là thuật toán tìm đường đi ngắn nhất từ một đỉnh nguồn đến các đỉnh còn lại trong Graph có trọng số không âm.

Bài toán:

```text
Input:
- Một weighted graph G = (V, E)
- Một source vertex S

Output:
- Khoảng cách ngắn nhất từ S đến mọi vertex
- Có thể lưu thêm predecessor để khôi phục đường đi
```

Ví dụ:

```text
A --4-- B
|       /|
1     2  5
|   /    |
C --1-- D
```

Nếu bắt đầu từ `A`, Dijkstra xác định:

```text
distance[A] = 0
distance[C] = 1
distance[D] = 2
distance[B] = 3
```

Đường ngắn nhất từ `A` đến `B`:

```text
A → C → D → B
```

với tổng trọng số:

```text
1 + 1 + 1 = 3
```

---

## 2. Bài toán Single-Source Shortest Path

Dijkstra giải bài toán:

```text
Single-Source Shortest Path
```

Nghĩa là:

```text
Một source
→ khoảng cách ngắn nhất đến tất cả vertex khác
```

Thuật toán không trực tiếp giải:

```text
All-Pairs Shortest Path
```

Muốn tìm đường ngắn nhất giữa mọi cặp đỉnh, có thể:

- Chạy Dijkstra từ từng đỉnh.
- Dùng Floyd–Warshall.
- Dùng Johnson's Algorithm với Graph phù hợp.

---

## 3. Điều kiện áp dụng

Dijkstra hoạt động đúng khi:

```text
Mọi edge weight >= 0
```

Có thể có cạnh trọng số `0`.

Không được có cạnh âm:

```text
weight < 0
```

Dijkstra có thể dùng cho:

- Directed Graph.
- Undirected Graph.
- Connected Graph.
- Disconnected Graph.
- Sparse Graph.
- Dense Graph.

Với Graph không liên thông, một số đỉnh có khoảng cách:

```text
INF
```

nghĩa là không thể đi tới từ source.

---

## 4. Tính chất đường đi ngắn nhất

Nếu đường đi ngắn nhất từ `A` đến `D` là:

```text
A → B → C → D
```

thì các đường con cũng phải là đường ngắn nhất:

```text
A → B
A → B → C
B → C → D
C → D
```

Nếu tồn tại một đường ngắn hơn từ `B` đến `D`, ta có thể thay đoạn:

```text
B → C → D
```

bằng đường ngắn hơn đó.

Khi ấy đường từ `A` đến `D` ban đầu không còn là đường ngắn nhất, tạo ra mâu thuẫn.

Tính chất này cho phép xây dựng lời giải từng bước.

---

## 5. Ý tưởng tham lam

Dijkstra là một **Greedy Algorithm**.

Ở mỗi vòng lặp:

1. Chọn đỉnh chưa xử lý có `distance` nhỏ nhất.
2. Xem khoảng cách đó là đã được xác nhận.
3. Dùng đỉnh vừa chọn để cập nhật các hàng xóm.
4. Không quay lại thay đổi khoảng cách của đỉnh đã xác nhận.

Quyết định tham lam:

```text
Đỉnh chưa xử lý gần source nhất
sẽ được xử lý tiếp theo.
```

Điều này chỉ an toàn vì không có cạnh âm.

---

## 6. Các cấu trúc dữ liệu cần dùng

## 6.1. Distance Array

```c
distance[v]
```

Lưu khoảng cách tốt nhất hiện biết từ source đến `v`.

Khởi tạo:

```text
distance[source] = 0
distance[v]      = INF, với v != source
```

---

## 6.2. Previous hoặc Parent Array

```c
previous[v]
```

Lưu đỉnh đứng trước `v` trên đường ngắn nhất hiện tại.

Ví dụ:

```text
previous[D] = C
previous[C] = A
```

Có thể khôi phục:

```text
A → C → D
```

---

## 6.3. Visited hoặc Settled Array

```c
visited[v]
```

Ý nghĩa:

```text
false → khoảng cách chưa được xác nhận cuối cùng
true  → khoảng cách ngắn nhất đã được xác nhận
```

---

## 6.4. Min Priority Queue

Với Graph lớn, dùng Min Heap để lấy nhanh đỉnh có khoảng cách nhỏ nhất.

Key:

```text
distance[vertex]
```

Thao tác quan trọng:

- Insert.
- Extract Min.
- Decrease Key.

---

## 7. Relaxation là gì?

**Relaxation** là thao tác kiểm tra xem đi qua một đỉnh trung gian có tạo ra đường ngắn hơn hay không.

Xét cạnh:

```text
u → v
```

có trọng số:

```text
weight(u, v)
```

Khoảng cách mới:

```text
candidate = distance[u] + weight(u, v)
```

Nếu:

```text
candidate < distance[v]
```

thì cập nhật:

```c
distance[v] = candidate;
previous[v] = u;
```

### Ví dụ

Hiện tại:

```text
distance[u] = 5
weight(u,v) = 3
distance[v] = 12
```

Đường mới:

```text
5 + 3 = 8
```

Vì:

```text
8 < 12
```

cập nhật:

```text
distance[v] = 8
previous[v] = u
```

Nếu candidate không nhỏ hơn, giữ nguyên giá trị cũ.

---

## 8. Các bước của thuật toán

Giả sử Graph có `V` đỉnh và source là `S`.

### Bước 1: Khởi tạo

```text
distance[S] = 0

Với mọi v != S:
    distance[v] = INF

visited[v] = false
previous[v] = NONE
```

### Bước 2: Chọn đỉnh gần nhất

Tìm đỉnh `u` thỏa:

```text
visited[u] == false
```

và có:

```text
distance[u] nhỏ nhất
```

### Bước 3: Xác nhận đỉnh

```c
visited[u] = true;
```

### Bước 4: Relax các cạnh đi ra từ u

Với từng hàng xóm `v`:

```text
candidate = distance[u] + weight(u,v)
```

Nếu ngắn hơn:

```text
distance[v] = candidate
previous[v] = u
```

### Bước 5: Lặp lại

Kết thúc khi:

- Mọi đỉnh reachable đã được visited.
- Không còn đỉnh chưa visited có distance hữu hạn.
- Hoặc đỉnh đích đã được xác nhận nếu chỉ tìm một destination.

---

## 9. Ví dụ từng bước

Xét Graph vô hướng:

```text
         4
    0 -------- 1
    |          | \
  1 |        1 |  \ 7
    |          |   \
    2 -------- 3 --- 4
         2       3
```

Các cạnh:

```text
0 — 1 : 4
0 — 2 : 1
2 — 1 : 2
1 — 3 : 1
2 — 3 : 5
3 — 4 : 3
1 — 4 : 7
```

Source:

```text
0
```

---

### Khởi tạo

| Vertex | Distance | Previous | Visited |
|---:|---:|---:|:---:|
| 0 | 0 | — | No |
| 1 | INF | — | No |
| 2 | INF | — | No |
| 3 | INF | — | No |
| 4 | INF | — | No |

---

### Vòng 1: Chọn vertex 0

```text
distance[0] = 0
```

Relax:

```text
0 → 1: 0 + 4 = 4
0 → 2: 0 + 1 = 1
```

| Vertex | Distance | Previous |
|---:|---:|---:|
| 0 | 0 | — |
| 1 | 4 | 0 |
| 2 | 1 | 0 |
| 3 | INF | — |
| 4 | INF | — |

Đánh dấu:

```text
visited[0] = true
```

---

### Vòng 2: Chọn vertex 2

Đỉnh chưa visited gần nhất:

```text
distance[2] = 1
```

Relax:

```text
2 → 1: 1 + 2 = 3
```

`3 < 4`, cập nhật:

```text
distance[1] = 3
previous[1] = 2
```

```text
2 → 3: 1 + 5 = 6
```

| Vertex | Distance | Previous |
|---:|---:|---:|
| 0 | 0 | — |
| 1 | 3 | 2 |
| 2 | 1 | 0 |
| 3 | 6 | 2 |
| 4 | INF | — |

---

### Vòng 3: Chọn vertex 1

```text
distance[1] = 3
```

Relax:

```text
1 → 3: 3 + 1 = 4
```

`4 < 6`, cập nhật:

```text
distance[3] = 4
previous[3] = 1
```

```text
1 → 4: 3 + 7 = 10
```

| Vertex | Distance | Previous |
|---:|---:|---:|
| 0 | 0 | — |
| 1 | 3 | 2 |
| 2 | 1 | 0 |
| 3 | 4 | 1 |
| 4 | 10 | 1 |

---

### Vòng 4: Chọn vertex 3

```text
distance[3] = 4
```

Relax:

```text
3 → 4: 4 + 3 = 7
```

`7 < 10`, cập nhật:

```text
distance[4] = 7
previous[4] = 3
```

---

### Kết quả

| Vertex | Shortest Distance | Previous |
|---:|---:|---:|
| 0 | 0 | — |
| 1 | 3 | 2 |
| 2 | 1 | 0 |
| 3 | 4 | 1 |
| 4 | 7 | 3 |

Đường đến `4` được lần ngược:

```text
4 ← 3 ← 1 ← 2 ← 0
```

Đảo lại:

```text
0 → 2 → 1 → 3 → 4
```

Tổng trọng số:

```text
1 + 2 + 1 + 3 = 7
```

---

## 10. Pseudocode

```text
Dijkstra(Graph, Source):

    for each vertex v:
        distance[v] = INF
        previous[v] = NONE
        visited[v]  = false

    distance[Source] = 0

    repeat:

        u = unvisited vertex with minimum distance

        if u does not exist:
            break

        visited[u] = true

        for each neighbor v of u:

            if visited[v] == false:

                candidate =
                    distance[u] + weight(u, v)

                if candidate < distance[v]:

                    distance[v] = candidate
                    previous[v] = u

    return distance, previous
```

Với Min Priority Queue:

```text
distance[source] = 0
push(source, 0)

while priority queue not empty:

    (u, queued_distance) = extract_min()

    if queued_distance != distance[u]:
        continue

    for each edge u -> v:

        candidate = distance[u] + weight

        if candidate < distance[v]:

            distance[v] = candidate
            previous[v] = u
            push(v, candidate)
```

Cách push bản ghi mới và bỏ qua bản ghi cũ giúp tránh phải cài đặt Decrease Key phức tạp.

---

## 11. Cài đặt bằng Adjacency Matrix

Adjacency Matrix:

```c
graph[u][v]
```

Lưu trọng số cạnh từ `u` đến `v`.

Trong thiết kế dưới đây:

```text
DIJKSTRA_NO_EDGE → không có cạnh
0                → cạnh có trọng số 0 vẫn hợp lệ
```

Không nên dùng `0` để biểu diễn “không có cạnh” nếu Graph có thể chứa zero-weight edge.

### Chọn đỉnh nhỏ nhất

Với bản đơn giản, mỗi vòng quét toàn bộ `V` đỉnh:

```text
O(V)
```

Thực hiện tối đa `V` vòng:

```text
O(V²)
```

Bản này phù hợp khi:

- Graph nhỏ.
- Graph dày.
- Muốn code đơn giản.
- Muốn dùng vùng nhớ tĩnh.
- Embedded có số đỉnh cố định.

---

## 12. Mã C hoàn chỉnh

Cấu trúc project:

```text
dijkstra/
├── include/
│   └── dijkstra.h
├── src/
│   ├── dijkstra.c
│   └── main.c
└── README.md
```

---

### 12.1. File `dijkstra.h`

```c
#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define DIJKSTRA_MAX_VERTICES  16U
#define DIJKSTRA_NO_EDGE       UINT32_MAX
#define DIJKSTRA_INFINITY      UINT32_MAX

typedef struct
{
    size_t vertex_count;

    uint32_t weight
        [DIJKSTRA_MAX_VERTICES]
        [DIJKSTRA_MAX_VERTICES];
} WeightedGraph;

typedef struct
{
    uint32_t distance[DIJKSTRA_MAX_VERTICES];
    size_t previous[DIJKSTRA_MAX_VERTICES];
    bool reachable[DIJKSTRA_MAX_VERTICES];
} DijkstraResult;

/**
 * @brief Initialize a graph with no edges.
 */
bool WeightedGraph_Init(
    WeightedGraph *graph,
    size_t vertex_count
);

/**
 * @brief Add or update one directed edge.
 */
bool WeightedGraph_AddDirectedEdge(
    WeightedGraph *graph,
    size_t source,
    size_t destination,
    uint32_t weight
);

/**
 * @brief Add or update one undirected edge.
 */
bool WeightedGraph_AddUndirectedEdge(
    WeightedGraph *graph,
    size_t first,
    size_t second,
    uint32_t weight
);

/**
 * @brief Compute shortest paths from one source.
 */
bool Dijkstra_Compute(
    const WeightedGraph *graph,
    size_t source,
    DijkstraResult *result
);

/**
 * @brief Reconstruct a source-to-target path.
 *
 * @return Number of vertices in the path.
 *         Returns 0 if the target is unreachable
 *         or the output buffer is too small.
 */
size_t Dijkstra_ReconstructPath(
    const DijkstraResult *result,
    size_t vertex_count,
    size_t source,
    size_t target,
    size_t output_path[],
    size_t output_capacity
);

#endif
```

---

### 12.2. File `dijkstra.c`

```c
#include "dijkstra.h"

static bool FindNearestUnvisited(
    const uint32_t distance[],
    const bool visited[],
    size_t vertex_count,
    size_t *vertex
)
{
    uint32_t minimum_distance = DIJKSTRA_INFINITY;
    size_t best_vertex = 0U;
    bool found = false;
    size_t index;

    if ((distance == NULL) ||
        (visited == NULL) ||
        (vertex == NULL))
    {
        return false;
    }

    for (index = 0U;
         index < vertex_count;
         index++)
    {
        if ((!visited[index]) &&
            (distance[index] < minimum_distance))
        {
            minimum_distance = distance[index];
            best_vertex = index;
            found = true;
        }
    }

    if (!found)
    {
        return false;
    }

    *vertex = best_vertex;

    return true;
}

bool WeightedGraph_Init(
    WeightedGraph *graph,
    size_t vertex_count
)
{
    size_t row;
    size_t column;

    if ((graph == NULL) ||
        (vertex_count == 0U) ||
        (vertex_count > DIJKSTRA_MAX_VERTICES))
    {
        return false;
    }

    graph->vertex_count = vertex_count;

    for (row = 0U;
         row < DIJKSTRA_MAX_VERTICES;
         row++)
    {
        for (column = 0U;
             column < DIJKSTRA_MAX_VERTICES;
             column++)
        {
            graph->weight[row][column] =
                DIJKSTRA_NO_EDGE;
        }
    }

    for (row = 0U; row < vertex_count; row++)
    {
        graph->weight[row][row] = 0U;
    }

    return true;
}

bool WeightedGraph_AddDirectedEdge(
    WeightedGraph *graph,
    size_t source,
    size_t destination,
    uint32_t weight
)
{
    if ((graph == NULL) ||
        (source >= graph->vertex_count) ||
        (destination >= graph->vertex_count) ||
        (weight == DIJKSTRA_NO_EDGE))
    {
        return false;
    }

    graph->weight[source][destination] = weight;

    return true;
}

bool WeightedGraph_AddUndirectedEdge(
    WeightedGraph *graph,
    size_t first,
    size_t second,
    uint32_t weight
)
{
    if (!WeightedGraph_AddDirectedEdge(
            graph,
            first,
            second,
            weight))
    {
        return false;
    }

    if (!WeightedGraph_AddDirectedEdge(
            graph,
            second,
            first,
            weight))
    {
        return false;
    }

    return true;
}

bool Dijkstra_Compute(
    const WeightedGraph *graph,
    size_t source,
    DijkstraResult *result
)
{
    bool visited[DIJKSTRA_MAX_VERTICES] = {false};
    size_t vertex;
    size_t iteration;

    if ((graph == NULL) ||
        (result == NULL) ||
        (source >= graph->vertex_count))
    {
        return false;
    }

    for (vertex = 0U;
         vertex < graph->vertex_count;
         vertex++)
    {
        result->distance[vertex] =
            DIJKSTRA_INFINITY;

        result->previous[vertex] =
            DIJKSTRA_MAX_VERTICES;

        result->reachable[vertex] = false;
    }

    result->distance[source] = 0U;
    result->previous[source] = source;
    result->reachable[source] = true;

    for (iteration = 0U;
         iteration < graph->vertex_count;
         iteration++)
    {
        size_t current;

        if (!FindNearestUnvisited(
                result->distance,
                visited,
                graph->vertex_count,
                &current))
        {
            /*
             * No additional reachable vertex remains.
             */
            break;
        }

        visited[current] = true;

        for (vertex = 0U;
             vertex < graph->vertex_count;
             vertex++)
        {
            uint32_t edge_weight =
                graph->weight[current][vertex];

            uint32_t candidate;

            if (visited[vertex])
            {
                continue;
            }

            if (edge_weight == DIJKSTRA_NO_EDGE)
            {
                continue;
            }

            if (result->distance[current] ==
                DIJKSTRA_INFINITY)
            {
                continue;
            }

            /*
             * Prevent unsigned integer overflow.
             */
            if (result->distance[current] >
                UINT32_MAX - edge_weight)
            {
                continue;
            }

            candidate =
                result->distance[current] +
                edge_weight;

            if (candidate < result->distance[vertex])
            {
                result->distance[vertex] = candidate;
                result->previous[vertex] = current;
                result->reachable[vertex] = true;
            }
        }
    }

    return true;
}

size_t Dijkstra_ReconstructPath(
    const DijkstraResult *result,
    size_t vertex_count,
    size_t source,
    size_t target,
    size_t output_path[],
    size_t output_capacity
)
{
    size_t reverse_path[DIJKSTRA_MAX_VERTICES];
    size_t reverse_length = 0U;
    size_t current;
    size_t index;

    if ((result == NULL) ||
        (output_path == NULL) ||
        (vertex_count == 0U) ||
        (vertex_count > DIJKSTRA_MAX_VERTICES) ||
        (source >= vertex_count) ||
        (target >= vertex_count) ||
        (!result->reachable[target]))
    {
        return 0U;
    }

    current = target;

    while (true)
    {
        if (reverse_length >= vertex_count)
        {
            /*
             * Invalid predecessor chain or cycle.
             */
            return 0U;
        }

        reverse_path[reverse_length] = current;
        reverse_length++;

        if (current == source)
        {
            break;
        }

        current = result->previous[current];

        if (current >= vertex_count)
        {
            return 0U;
        }
    }

    if (reverse_length > output_capacity)
    {
        return 0U;
    }

    for (index = 0U;
         index < reverse_length;
         index++)
    {
        output_path[index] =
            reverse_path[reverse_length - 1U - index];
    }

    return reverse_length;
}
```

---

### 12.3. File `main.c`

```c
#include "dijkstra.h"

#include <inttypes.h>
#include <stdio.h>

static void PrintDistances(
    const DijkstraResult *result,
    size_t vertex_count,
    size_t source
)
{
    size_t vertex;

    for (vertex = 0U;
         vertex < vertex_count;
         vertex++)
    {
        printf(
            "Distance from %zu to %zu: ",
            source,
            vertex
        );

        if (!result->reachable[vertex])
        {
            printf("unreachable\n");
        }
        else
        {
            printf(
                "%" PRIu32 "\n",
                result->distance[vertex]
            );
        }
    }
}

static void PrintPath(
    const DijkstraResult *result,
    size_t vertex_count,
    size_t source,
    size_t target
)
{
    size_t path[DIJKSTRA_MAX_VERTICES];
    size_t length;
    size_t index;

    length = Dijkstra_ReconstructPath(
        result,
        vertex_count,
        source,
        target,
        path,
        DIJKSTRA_MAX_VERTICES
    );

    printf(
        "Path from %zu to %zu: ",
        source,
        target
    );

    if (length == 0U)
    {
        printf("unreachable\n");
        return;
    }

    for (index = 0U; index < length; index++)
    {
        printf("%zu", path[index]);

        if ((index + 1U) < length)
        {
            printf(" -> ");
        }
    }

    printf("\n");
}

int main(void)
{
    WeightedGraph graph;
    DijkstraResult result;
    const size_t source = 0U;

    if (!WeightedGraph_Init(&graph, 5U))
    {
        printf("Could not initialize graph.\n");
        return 1;
    }

    WeightedGraph_AddUndirectedEdge(
        &graph, 0U, 1U, 4U
    );

    WeightedGraph_AddUndirectedEdge(
        &graph, 0U, 2U, 1U
    );

    WeightedGraph_AddUndirectedEdge(
        &graph, 2U, 1U, 2U
    );

    WeightedGraph_AddUndirectedEdge(
        &graph, 1U, 3U, 1U
    );

    WeightedGraph_AddUndirectedEdge(
        &graph, 2U, 3U, 5U
    );

    WeightedGraph_AddUndirectedEdge(
        &graph, 3U, 4U, 3U
    );

    WeightedGraph_AddUndirectedEdge(
        &graph, 1U, 4U, 7U
    );

    if (!Dijkstra_Compute(
            &graph,
            source,
            &result))
    {
        printf("Dijkstra failed.\n");
        return 1;
    }

    PrintDistances(
        &result,
        graph.vertex_count,
        source
    );

    printf("\n");

    PrintPath(
        &result,
        graph.vertex_count,
        source,
        4U
    );

    return 0;
}
```

---

### 12.4. Biên dịch

```bash
gcc -std=c11 \
    -Wall \
    -Wextra \
    -Wpedantic \
    -Iinclude \
    src/dijkstra.c \
    src/main.c \
    -o dijkstra_demo
```

Chạy:

```bash
./dijkstra_demo
```

---

## 13. Khôi phục đường đi ngắn nhất

Sau khi thuật toán hoàn tất:

```c
previous[v]
```

cho biết đỉnh trước `v`.

Ví dụ:

```text
previous[4] = 3
previous[3] = 1
previous[1] = 2
previous[2] = 0
```

Lần ngược:

```text
4 ← 3 ← 1 ← 2 ← 0
```

Sau đó đảo thứ tự:

```text
0 → 2 → 1 → 3 → 4
```

### Tại sao cần một mảng tạm?

Predecessor chain đi từ target về source.

Mảng tạm hoạt động giống Stack:

```text
Push target
Push previous[target]
...
Push source
```

Sau đó đọc ngược để nhận đường đi từ source đến target.

### Trường hợp không thể tới

Nếu:

```text
reachable[target] == false
```

không tồn tại đường đi từ source đến target.

---

## 14. Kết quả chạy thử

Kết quả dự kiến:

```text
Distance from 0 to 0: 0
Distance from 0 to 1: 3
Distance from 0 to 2: 1
Distance from 0 to 3: 4
Distance from 0 to 4: 7

Path from 0 to 4: 0 -> 2 -> 1 -> 3 -> 4
```

---

## 15. Dijkstra với Adjacency List và Min Heap

Với Sparse Graph, Adjacency Matrix gây lãng phí:

```text
Memory = O(V²)
```

Adjacency List chỉ lưu các cạnh tồn tại:

```text
Memory = O(V + E)
```

Min Heap lưu các cặp:

```c
typedef struct
{
    size_t vertex;
    uint32_t distance;
} HeapEntry;
```

Root luôn có distance nhỏ nhất.

### Lazy Deletion

Thay vì Decrease Key, mỗi lần distance tốt hơn:

```text
Push một entry mới
```

Khi pop:

```c
if (entry.distance != distance[entry.vertex])
{
    bỏ qua entry cũ
}
```

Pseudo-code:

```text
push(source, 0)

while heap not empty:

    entry = extract_min()

    u = entry.vertex

    if entry.distance != distance[u]:
        continue

    for each edge (u, v, weight):

        candidate = distance[u] + weight

        if candidate < distance[v]:

            distance[v] = candidate
            previous[v] = u
            push(v, candidate)
```

### Ưu điểm

- Không cần indexed heap.
- Code dễ hơn Decrease Key.
- Hiệu quả tốt trong thực tế.

### Hạn chế

- Một vertex có thể có nhiều entry trong Heap.
- Heap có thể chứa nhiều bản ghi cũ.
- Cần capacity lớn hơn `V` nếu dùng vùng nhớ cố định.

---

## 16. Độ phức tạp

Ký hiệu:

```text
V = số vertex
E = số edge
```

### Adjacency Matrix + Linear Scan

Mỗi lần chọn đỉnh nhỏ nhất:

```text
O(V)
```

Lặp tối đa `V` lần:

```text
O(V²)
```

Relax hàng xóm qua một hàng matrix:

```text
O(V)
```

Tổng:

```text
Time  = O(V²)
Space = O(V²)
```

nếu tính cả Matrix.

Bộ nhớ phụ của thuật toán:

```text
O(V)
```

---

### Adjacency List + Binary Min Heap

Các thao tác Heap:

```text
Extract Min → O(log V)
Relax/Push  → O(log V)
```

Tổng thường được viết:

```text
O((V + E) log V)
```

Với connected graph, thường rút gọn:

```text
O(E log V)
```

Bộ nhớ:

```text
O(V + E)
```

cho Graph và các cấu trúc thuật toán.

---

### Fibonacci Heap

Về lý thuyết:

```text
O(E + V log V)
```

nhờ Decrease Key amortized tốt.

Implementation phức tạp và Binary Heap thường thực tế hơn.

---

## 17. Tại sao Dijkstra không dùng được với cạnh âm?

Xét Graph:

```text
S --2--> A
S --5--> B
B --(-10)--> A
```

Ban đầu:

```text
distance[A] = 2
distance[B] = 5
```

Dijkstra chọn `A` trước vì:

```text
2 < 5
```

và xác nhận khoảng cách `2`.

Nhưng sau khi đi qua `B`:

```text
S → B → A
= 5 + (-10)
= -5
```

Đường này ngắn hơn `2`.

Quyết định tham lam trước đó trở thành sai.

Dijkstra giả định:

```text
Khi chọn đỉnh chưa xử lý có distance nhỏ nhất,
không có đường đi trong tương lai làm distance đó nhỏ hơn.
```

Cạnh âm phá vỡ giả định này.

### Thuật toán thay thế

- Bellman–Ford: hỗ trợ cạnh âm và phát hiện negative cycle.
- Floyd–Warshall: all-pairs, hỗ trợ cạnh âm nếu không có negative cycle phù hợp.
- Johnson: all-pairs cho sparse graph có thể chứa cạnh âm nhưng không có negative cycle.

---

## 18. Graph không liên thông

Nếu một đỉnh không thể tới từ source:

```text
distance[v] = INF
reachable[v] = false
```

Trong vòng chọn đỉnh nhỏ nhất, nếu không còn đỉnh chưa visited có distance hữu hạn:

```text
Dừng thuật toán
```

Không nên tiếp tục với một index chưa được khởi tạo.

Ví dụ:

```text
Component 1: 0 — 1 — 2
Component 2: 3 — 4
```

Chạy từ `0`:

```text
0, 1, 2 → reachable
3, 4    → unreachable
```

---

## 19. So sánh với các thuật toán khác

| Thuật toán | Loại Graph | Trọng số âm | Mục tiêu | Độ phức tạp điển hình |
|---|---|:---:|---|---:|
| BFS | Unweighted/equal weight | Không áp dụng | Single-source | `O(V + E)` |
| Dijkstra | Non-negative weights | Không | Single-source | `O(E log V)` |
| Bellman–Ford | Weighted | Có | Single-source | `O(VE)` |
| Floyd–Warshall | Weighted | Có, không negative cycle | All-pairs | `O(V³)` |
| A* | Non-negative cost + heuristic | Không | Source-to-target | Phụ thuộc heuristic |
| DAG Shortest Path | Directed Acyclic Graph | Có thể | Single-source | `O(V + E)` |

### Dijkstra và BFS

Nếu mọi cạnh có cùng trọng số:

```text
weight = 1
```

BFS là lựa chọn đơn giản hơn và nhanh hơn.

### Dijkstra và Prim

Hai thuật toán đều có thể dùng Min Heap nhưng mục tiêu khác nhau.

Dijkstra:

```text
Tối thiểu tổng khoảng cách từ source đến từng vertex
```

Prim:

```text
Tạo Minimum Spanning Tree có tổng trọng số cạnh nhỏ nhất
```

Shortest Path Tree và Minimum Spanning Tree không phải cùng một khái niệm.

---

## 20. Ứng dụng

Dijkstra được sử dụng trong:

- Tìm đường ngắn nhất.
- Hệ thống bản đồ.
- Routing trong network.
- Telephone network.
- Social network.
- Robot path planning.
- Game AI.
- Logistics.
- Packet routing.
- Dependency cost.
- Geographic Information Systems.
- Service discovery.
- Transit planning.

---

## 20.1. Bản đồ

Vertex:

```text
Giao lộ hoặc địa điểm
```

Edge:

```text
Đường nối
```

Weight:

- Khoảng cách.
- Thời gian.
- Chi phí.
- Năng lượng.
- Toll fee.

Nếu weight thay đổi theo thời gian thực, Graph phải được cập nhật trước khi chạy lại thuật toán.

---

## 20.2. Network Routing

Vertex:

```text
Router
```

Edge:

```text
Network link
```

Weight có thể biểu diễn:

- Latency.
- Hop cost.
- Bandwidth cost.
- Congestion metric.

Link-state routing protocols sử dụng ý tưởng shortest path trên topology đã biết.

---

## 20.3. Social Network

Vertex:

```text
User
```

Edge:

```text
Relationship
```

Weight có thể là:

- Mức độ liên hệ.
- Chi phí kết nối.
- Khoảng cách xã hội.
- Độ tin cậy.

Nếu mọi cạnh bằng nhau, BFS thường phù hợp hơn.

---

## 20.4. Robot và Game

Graph có thể biểu diễn:

- Grid.
- Navigation mesh.
- Waypoint network.
- State transition.

Weight có thể là:

- Khoảng cách.
- Năng lượng.
- Độ nguy hiểm.
- Thời gian di chuyển.

---

## 21. Dijkstra trong lập trình nhúng

## 21.1. Giới hạn số đỉnh

Trong firmware, nên đặt giới hạn compile-time:

```c
#define MAX_VERTICES 16U
```

Nhờ đó biết trước RAM cho:

- Adjacency Matrix.
- Distance.
- Previous.
- Visited.
- Heap hoặc Queue.

---

## 21.2. Adjacency Matrix cho Graph nhỏ

Với:

```text
V = 16
uint16_t weight
```

Matrix cần:

```text
16 × 16 × 2 = 512 byte
```

Ưu điểm:

- Không dùng con trỏ.
- Không cần cấp phát động.
- Truy cập cạnh `O(1)`.
- Code đơn giản.

Nếu Graph rất thưa, Adjacency List tốn ít RAM hơn.

---

## 21.3. Chọn kiểu dữ liệu Weight

Cần tính đường đi lớn nhất có thể.

Nếu:

```text
maximum edge weight = W
maximum path edges  = V - 1
```

Upper bound gần đúng:

```text
(V - 1) × W
```

Chọn:

- `uint16_t`
- `uint32_t`
- `uint64_t`

phù hợp.

Luôn kiểm tra overflow:

```c
if (distance[u] <= UINT32_MAX - weight)
{
    candidate = distance[u] + weight;
}
```

---

## 21.4. Không dùng giá trị cạnh bằng 0 để biểu diễn No Edge

Cạnh trọng số `0` là hợp lệ.

Nên dùng:

```c
UINT32_MAX
```

hoặc một bitmap riêng để đánh dấu cạnh tồn tại.

---

## 21.5. Thời gian thực thi

Matrix implementation:

```text
O(V²)
```

Với `V` nhỏ và cố định, worst-case dễ tính.

Min Heap implementation:

```text
O(E log V)
```

nhanh hơn với Graph thưa nhưng code và memory management phức tạp hơn.

---

## 21.6. Không chạy thuật toán lớn trong ISR

Dijkstra có thể thực hiện nhiều vòng lặp và relaxation.

ISR nên:

1. Ghi nhận thay đổi Graph hoặc yêu cầu route.
2. Đặt event.
3. Để main loop hoặc RTOS task chạy thuật toán.

---

## 21.7. Incremental Execution

Nếu không thể chạy hết trong một chu kỳ:

```text
Mỗi tick xử lý một vertex hoặc K edges
```

Lưu trạng thái:

- Current phase.
- Distance.
- Visited.
- Current vertex.
- Neighbor index.
- Priority Queue.

Đây là cách biến thuật toán thành state machine không blocking.

---

## 21.8. Graph thay đổi đồng thời

Nếu DMA, ISR hoặc task khác sửa weight trong lúc Dijkstra chạy, kết quả có thể không nhất quán.

Cần:

- Snapshot.
- Double Buffer.
- Mutex.
- Critical section ngắn.
- Version number.
- Single-owner task.

---

## 22. Các lỗi thường gặp

## 22.1. Dùng Dijkstra với cạnh âm

Kết quả có thể sai dù chương trình vẫn chạy.

---

## 22.2. Nhầm Dijkstra với Minimum Spanning Tree

Dijkstra tạo Shortest Path Tree từ source.

Prim/Kruskal tạo Minimum Spanning Tree.

---

## 22.3. Dùng 0 làm No Edge

Điều này làm mất khả năng biểu diễn zero-weight edge.

---

## 22.4. Cộng INF với Weight

Sai:

```c
candidate = distance[u] + weight;
```

khi:

```text
distance[u] == INF
```

Có thể overflow và tạo số nhỏ giả.

Phải kiểm tra trước.

---

## 22.5. Integer Overflow

Ngay cả khi distance hữu hạn:

```text
distance[u] + weight
```

vẫn có thể vượt giới hạn kiểu.

---

## 22.6. Không xử lý Graph không liên thông

Hàm chọn minimum có thể không tìm thấy vertex.

Không được dùng một `next_vertex` chưa khởi tạo.

---

## 22.7. Không cập nhật Previous

Nếu chỉ cập nhật `distance`, ta biết độ dài nhưng không khôi phục được đường đi.

---

## 22.8. Đánh dấu Visited quá sớm

Chỉ xác nhận vertex sau khi nó được chọn là đỉnh chưa visited có distance nhỏ nhất.

Không đánh dấu ngay lần đầu phát hiện như BFS.

---

## 22.9. Min Heap chứa bản ghi cũ

Với Lazy Deletion, phải bỏ qua entry nếu:

```c
entry.distance != distance[entry.vertex]
```

Nếu không, có thể xử lý lặp nhiều lần hoặc sai logic settled.

---

## 22.10. Priority Queue sắp sai chiều

Dijkstra cần:

```text
Min Priority Queue
```

Không phải Max Heap.

---

## 22.11. Không dừng sớm đúng cách

Nếu chỉ tìm đến một target, có thể dừng khi target được extract/chọn làm minimum.

Không nên dừng ngay lần đầu target được relaxation vì distance vẫn có thể được cải thiện.

---

## 22.12. Previous Chain bị lỗi

Khi reconstruct:

- Giới hạn số bước tối đa bằng `V`.
- Kiểm tra index.
- Xử lý target unreachable.
- Tránh vòng lặp vô hạn do dữ liệu hỏng.

---

## 23. Bài tập luyện tập

### Cơ bản

1. Dijkstra bằng Adjacency Matrix.
2. In distance từ source.
3. Lưu previous.
4. Khôi phục đường đi.
5. Xử lý Graph không liên thông.
6. Directed Graph.
7. Undirected Graph.
8. Zero-weight edge.
9. Chỉ tìm một target.
10. Kiểm tra input có cạnh âm.

### Trung bình

1. Dijkstra bằng Adjacency List.
2. Dijkstra bằng Binary Min Heap.
3. Lazy Deletion.
4. Indexed Min Heap với Decrease Key.
5. Network Delay Time.
6. Path With Minimum Effort.
7. Cheapest Flights với giới hạn stop.
8. Minimum Cost Path trên grid.
9. Shortest Path trong weighted maze.
10. K shortest distances đơn giản.
11. Count number of shortest paths.
12. Multi-source Dijkstra.

### Nâng cao

1. Bidirectional Dijkstra.
2. A* Search.
3. Johnson's Algorithm.
4. Dijkstra với state expansion.
5. Shortest path có resource constraint.
6. Dynamic graph shortest path.
7. Time-dependent edge weights.
8. Dijkstra với radix heap.
9. External-memory graph.
10. Incremental Dijkstra trong embedded.
11. Route cache.
12. Parallel shortest path.

---

## 24. Tổng kết

Dijkstra tìm đường đi ngắn nhất từ một source trong Graph có trọng số không âm.

Các cấu trúc chính:

```text
distance[]
previous[]
visited[]
Min Priority Queue hoặc linear scan
```

Relaxation:

```text
candidate = distance[u] + weight(u,v)

if candidate < distance[v]:
    distance[v] = candidate
    previous[v] = u
```

Quyết định tham lam:

```text
Luôn chọn vertex chưa xử lý có distance nhỏ nhất.
```

Độ phức tạp:

```text
Adjacency Matrix + Linear Scan:
O(V²)

Adjacency List + Binary Min Heap:
O((V + E) log V)
thường viết O(E log V)
```

Các điểm quan trọng:

1. Không có cạnh âm.
2. Không cộng khi distance là INF.
3. Kiểm tra integer overflow.
4. Lưu previous để reconstruct path.
5. Xử lý đỉnh unreachable.
6. Zero-weight edge là hợp lệ.
7. Không nhầm Shortest Path Tree với Minimum Spanning Tree.
8. Embedded nên chọn cấu trúc theo giới hạn RAM và worst-case.

---

## 25. Tài liệu tham khảo

README này được biên soạn và diễn đạt lại độc lập dựa trên phạm vi chủ đề được trình bày trong:

- Programiz — **Dijkstra's Algorithm**

README không phải là bản sao nguyên văn của nội dung nguồn.
