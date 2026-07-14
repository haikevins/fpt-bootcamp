# Tính thời gian đàn kiến rơi khỏi tấm ván

## Mô tả

Cho một tấm ván dài `n` đơn vị. Có nhiều con kiến đang di chuyển trên tấm ván, mỗi con có vận tốc:

```text
1 đơn vị/giây
```

Một số con kiến di chuyển sang trái, các con còn lại di chuyển sang phải.

Khi hai con kiến đi ngược chiều gặp nhau, chúng lập tức đổi hướng và không mất thêm thời gian.

Khi một con kiến đi đến một trong hai đầu tấm ván, nó rơi khỏi ván ngay lập tức.

Hãy xác định thời điểm con kiến cuối cùng rơi khỏi tấm ván.

## Dữ liệu vào

- Dòng đầu tiên chứa số nguyên `n`, là chiều dài của tấm ván.
- Dòng thứ hai chứa vị trí ban đầu của các con kiến đang di chuyển từ phải sang trái.
- Dòng thứ ba chứa vị trí ban đầu của các con kiến đang di chuyển từ trái sang phải.

Các vị trí trên cùng một dòng được phân tách bằng dấu cách.

## Dữ liệu ra

In thời điểm con kiến cuối cùng rơi khỏi tấm ván.

## Ví dụ

### Input

```text
4
4 3
0 1
```

### Output

```text
4
```

## Gợi ý

Khi hai con kiến gặp nhau và đổi hướng, có thể xem như chúng đi xuyên qua nhau.

Vì vậy, không cần mô phỏng từng lần va chạm.

Thời gian để một con kiến rơi khỏi ván được tính như sau:

- Kiến đi sang trái tại vị trí `x`:

  ```text
  time = x
  ```

- Kiến đi sang phải tại vị trí `x`:

  ```text
  time = n - x
  ```

Kết quả là thời gian lớn nhất trong số tất cả các con kiến.

## Kiến thức sử dụng

- Mô phỏng
- Tư duy quy đổi chuyển động
- Tìm giá trị lớn nhất
- Duyệt mảng

## Gợi ý cấu trúc thư mục

```text
last-ant-time/
├── README.md
└── main.c
```
