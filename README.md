# ⚡ FPS Boost Toolkit

> Một công cụ tối ưu hóa hệ thống Windows siêu nhẹ viết bằng **C++17**, giúp tăng cường hiệu năng chơi game, chuyển đổi chế độ điện năng (Power Plan) và dọn dẹp file tạm an toàn mà không làm ảnh hưởng đến hệ thống.

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue?style=flat-square&logo=cplusplus)
![Platform](https://img.shields.io/badge/Platform-Windows-0078D6?style=flat-square&logo=windows)
![License](https://img.shields.io/badge/License-MIT-green?style=flat-square)

---

## 📑 Mục lục
- [Tính năng chính](#-tính-năng-chính)
- [Cơ chế hoạt động & An toàn](#-cơ-chế-hoạt-động--an-toàn)
- [Yêu cầu hệ thống](#-yêu-cầu-hệ-thống)
- [Hướng dẫn sử dụng](#-hướng-dẫn-sử-dụng)
- [Hướng dẫn Build từ Mã nguồn](#-hướng-dẫn-build-từ-mã-nguồn)
- [Lưu ý quan trọng](#-lưu-ý-quan-trọng)
- [Giấy phép](#-giấy-phép)

---

## 🚀 Tính năng chính

| Tính năng | Mô tả chi tiết |
| :--- | :--- |
| **1. Show Current Plan** | Kiểm tra nhanh chế độ điện năng đang được kích hoạt trên hệ thống Windows. |
| **2. High Performance** | Kích hoạt chế độ hiệu năng cao, ép xung nhịp CPU tối đa để giảm giật lag khi chơi game. |
| **3. Ultimate Performance** | Mở khóa chế độ hiệu năng cực đại dành riêng cho các dòng máy cấu hình cao. |
| **4. Restore Balanced** | Khôi phục lại chế độ điện năng Cân bằng (Balanced) mặc định khi không chơi game. |
| **5. Clean User Temp** | Dọn dẹp thư mục file tạm cá nhân (`%TEMP%`), tự động bỏ qua file đang bị ứng dụng chiếm dụng. |
| **6. Clean Windows Temp** | Dọn dẹp thư mục file tạm hệ thống (yêu cầu quyền Administrator). |

---

## 🛡️ Cơ chế hoạt động & An toàn

- **Chống giả mạo lệnh (`powercfg`):** Sử dụng đường dẫn hệ thống tuyệt đối (`%SystemRoot%\System32\powercfg.exe`) để ngăn chặn tuyệt đối rủi ro chạy nhầm các file thực thi giả mạo đặt tại thư mục tải về (Downloads).
- **Bảo vệ thư mục cá nhân:** Tool kiểm tra kỹ biến môi trường `TEMP/TMP`. Nếu không tồn tại, chương trình sẽ **từ chối dọn dẹp** thay vì xóa nhầm sang thư mục người dùng (`Documents`, `Desktop`,...).
- **Không ép buộc xóa:** Các tệp tin đang mở hoặc bị khóa bởi ứng dụng khác sẽ được bỏ qua an toàn (`skipped`), không làm treo hay crash ứng dụng.

---

## 💻 Yêu cầu hệ thống

- **Hệ điều hành:** Windows 10 / 11 (64-bit)
- **Quyền hạn:** Khuyến nghị chạy dưới quyền **Administrator** (Run as Administrator) để công cụ có đầy đủ quyền áp dụng Power Plan và xóa file tạm hệ thống.

---

## 📥 Hướng dẫn sử dụng

Bạn có thể lựa chọn 1 trong 2 cách sử dụng dưới đây:

### 🌟 Cách 1: Tải file `.exe` có sẵn (Khuyên dùng - Nhanh nhất)
1. Truy cập vào mục **[Releases](https://github.com/iamatomic04/FPS-Boost-Toolkit/releases)** ở cột bên phải trang repo này.
2. Tải về file thực thi **`FPS-Boost-Toolkit.exe`** mới nhất từ bản **v1.0.0**.
3. Nhấp chuột phải vào file `FPS-Boost-Toolkit.exe` và chọn **Run as administrator** (Chạy dưới quyền Quản trị viên).
4. Chọn các số tương ứng trên menu giao diện Console để thực hiện tối ưu hóa hệ thống.

---

### 🛠️ Cách 2: Tự biên dịch từ Mã nguồn (Dành cho Developer)

#### 1. Trên Visual Studio 2022 (MSVC)
1. Mở Project Properties trong Visual Studio.
2. Vào **C/C++** ➔ **Language** ➔ Mục **C++ Language Standard** chọn **ISO C++17 (/std:c++17)**.
3. Chọn cấu hình **Release** và tiến hành Build (`Ctrl + Shift + B`).

#### 2. Trên MinGW (Dòng lệnh GCC)
Mở terminal tại thư mục chứa mã nguồn và chạy lệnh biên dịch:
```bash
g++ -std=c++17 -O2 main.cpp -o FPS-Boost-Toolkit.exe