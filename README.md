# ⚡ FPS Boost Toolkit

> Một công cụ mã nguồn mở siêu nhẹ được viết bằng **C++**, giúp tối ưu hóa hệ thống Windows, tinh chỉnh cấu hình và tối ưu hiệu năng chơi game.

---

## 📑 Mục lục
- [Tính năng](#-tính-năng)
- [Yêu cầu hệ thống](#-yêu-cầu-hệ-thống)
- [Hướng dẫn Build & Cài đặt](#-hướng-dẫn-build--cài-đặt)
- [Giấy phép](#-giấy-phép)

---

## 🚀 Tính năng

| Tính năng | Mô tả |
| :--- | :--- |
| **Lightweight C++** | Được viết hoàn toàn bằng C++, tiêu tốn ít hơn 15MB RAM khi chạy ngầm. |
| **Config & FastFlag** | Tùy chỉnh nhanh các thông số ẩn để tối ưu FPS cho game. |
| **System Cleaner** | Tự động dọn dẹp file tạm (Temp) và chuyển đổi Windows Power Plan sang High Performance. |
| **Safe & Backup** | Tích hợp tính năng sao lưu và khôi phục cài đặt gốc chỉ với 1 cú click. |

---

## 💻 Yêu cầu hệ thống

- **Hệ điều hành:** Windows 10 / 11 (64-bit)
- **Compiler:** Visual Studio 2022 (MSVC) hoặc MinGW-w64
- **Build Tool:** CMake 3.16 trở lên

---

## ⚙️ Hướng dẫn Build từ Mã nguồn

```bash
# 1. Clone repository về máy
git clone [https://github.com/iamatomic04/FPS-Boost-Toolkit.git](https://github.com/iamatomic04/FPS-Boost-Toolkit.git)
cd FPS-Boost-Toolkit

# 2. Tạo thư mục build
mkdir build && cd build

# 3. Cấu hình và biên dịch bằng CMake
cmake ..
cmake --build . --config Release