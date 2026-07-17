# Tính chu vi của hòn đảo

## Mô tả

Cho một ma trận hai chiều `grid` có kích thước `row × col`, biểu diễn một bản đồ gồm đất và nước:

- `grid[i][j] = 1`: ô đất
- `grid[i][j] = 0`: ô nước

Bản đồ chỉ có duy nhất một hòn đảo và hòn đảo được bao quanh hoàn toàn bởi nước.

Hai ô đất được xem là nối với nhau khi chúng có chung một cạnh theo một trong bốn hướng:

```text
trên, dưới, trái, phải
```

Hãy tính chu vi của hòn đảo.

## Dữ liệu vào

Dòng đầu tiên gồm hai số nguyên:

```text
row col
```

`row` dòng tiếp theo, mỗi dòng gồm `col` giá trị `0` hoặc `1`.

## Dữ liệu ra

In một số nguyên là chu vi của hòn đảo.

## Ví dụ

### Input

```text
4 4
0 1 0 0
1 1 1 0
0 1 0 0
1 1 0 0
```

### Output

```text
16
```

## Gợi ý

Mỗi ô đất ban đầu đóng góp `4` cạnh vào chu vi.

Nếu hai ô đất nằm cạnh nhau thì cạnh chung của chúng không thuộc chu vi. Vì vậy, với mỗi cặp ô đất liền kề, cần loại bỏ `2` cạnh.

Có thể duyệt toàn bộ ma trận và kiểm tra:

- Ô phía trên
- Ô phía dưới
- Ô bên trái
- Ô bên phải

Hoặc chỉ cần kiểm tra hai hướng để tránh đếm lặp:

- Ô phía trên
- Ô bên trái

## Kiến thức sử dụng

- Mảng hai chiều
- Duyệt ma trận
- Kiểm tra biên
- Tính toán theo các ô lân cận

## Gợi ý cấu trúc thư mục

```text
island-perimeter/
├── README.md
└── main.c
```
