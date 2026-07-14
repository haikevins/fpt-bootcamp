# Gộp danh sách dự thi

## Mô tả

Một trường tổ chức thi bốn môn:

- Toán
- Văn
- Anh
- Vật lý

Danh sách thí sinh của mỗi môn được quản lý bằng một **Linked List** riêng.

Mỗi node trong danh sách chứa các thông tin:

- `name`: tên thí sinh, ví dụ `NguyenVanA`
- `subject`: tên môn thi, gồm `toan`, `van`, `anh` hoặc `vatly`
- `className`: tên lớp, được biểu diễn bằng một ký tự như `A`, `B`, `C`, `D`
- `next`: con trỏ đến node tiếp theo

Bốn danh sách đầu vào gồm:

- `mathList`: danh sách thi Toán
- `literatureList`: danh sách thi Văn
- `englishList`: danh sách thi Anh
- `physicsList`: danh sách thi Vật lý

Hãy gộp bốn danh sách trên thành một Linked List duy nhất.

## Yêu cầu

1. Các danh sách phải được nối theo đúng thứ tự:

   ```text
   mathList → literatureList → englishList → physicsList
   ```

2. Giữ nguyên thứ tự xuất hiện của thí sinh trong từng danh sách.
3. Tái sử dụng các node đã có, không tạo thêm node mới.

## Dữ liệu vào

Dòng đầu tiên gồm bốn số nguyên, lần lượt là số lượng thí sinh thi:

```text
Toán Văn Anh Vật-lý
```

Các dòng tiếp theo chứa tên và lớp của từng thí sinh.

Thí sinh được nhập lần lượt theo thứ tự:

```text
Toán → Văn → Anh → Vật lý
```

## Dữ liệu ra

In danh sách sau khi gộp theo định dạng:

```text
<Tên-thí-sinh>-<Môn-thi>-<Lớp>
```

## Ví dụ

### Input

```text
2 2 1 2
Bao A
Cuong B
Bao A
Dung C
Bao A
Bao A
Cuong B
```

### Output

```text
Bao-toan-A
Cuong-toan-B
Bao-van-A
Dung-van-C
Bao-anh-A
Bao-vatly-A
Cuong-vatly-B
```

## Kiến thức sử dụng

- Linked List
- Con trỏ
- Nối hai danh sách liên kết
- Duyệt danh sách liên kết

## Gợi ý cấu trúc thư mục

```text
merge-exam-lists/
├── README.md
└── main.c
```
