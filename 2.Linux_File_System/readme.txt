File Info Viewer  

WHAT (Chương trình này làm gì?)  
- Đây là một chương trình C dùng để lấy thông tin của file trên hệ thống.  
- Nó sử dụng lstat() để đọc metadata và in ra:  
  + Đường dẫn file  
  + Loại file (regular file, directory, symbolic link, socket, v.v.)  
  + Kích thước file (bytes)  
  + Thời gian chỉnh sửa gần nhất  

WHY (Tại sao cần chương trình này?)  
- Hỗ trợ kiểm tra nhanh thông tin file, thay vì dùng lệnh ls -l hoặc stat.  
- Dễ tích hợp vào các công cụ quản lý file, giám sát hệ thống, hoặc dự án bảo mật.  
- Là ví dụ học tập tốt về cách dùng struct stat, lstat() và xử lý thời gian trong C.  

WHERE (Chạy ở đâu?)  
- Chạy trên Linux/Unix environment (Ubuntu, Debian, CentOS, macOS, …).  
- Yêu cầu compiler hỗ trợ chuẩn C (GCC/Clang).  

WHEN (Khi nào dùng?)  
- Khi muốn kiểm tra nhanh loại file và metadata.  
- Khi viết tool quản lý file tự động.  
- Khi debug các vấn đề liên quan đến file system.  

WHO (Ai nên dùng?)  
- Sinh viên CNTT: học cách thao tác với hệ thống file trong C.  
- Lập trình viên hệ thống: cần tích hợp kiểm tra file trong các tool/script.  
- Người quản trị hệ thống (SysAdmin): muốn nhanh chóng kiểm tra thông tin file mà không cần nhiều lệnh phức tạp.  

HOW (Cách sử dụng?)  
1. Biên dịch:  
   gcc file_info.c -o file_info  

2. Chạy chương trình:  
   ./file_info <file_path>  

3. Ví dụ:  
   ./file_info /etc/passwd  

   Kết quả mẫu:  
   File Path     : /etc/passwd  
   File Type     : Regular File  
   Size          : 3056 bytes  
   Last Modified : 2025-09-29 12:45:10  
