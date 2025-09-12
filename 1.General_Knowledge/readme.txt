Hướng dẫn tạo thư viện shared và static và tự động hóa bằng Makefile

Cấu trúc thư mục:
- bin (Binary): Chứa file thực thi chương trình
- inc (Include): Chứa file <file_name>.h
- lib (Library): 
+ shared: Chứa thư viện dạng shared
+ static: Chứa thư viện dạng static

- obj (Object): Chứa file object <file_name>.o
- src (Source): Chứa file nguồn <file_name>.c

Hướng dẫn chạy file Makefile bằng command promt (CMD):
- Tạo ra các object => Tạo ra thư viện shared và staic => Tạo ra chương trình thực thi
với từng thư viện shared và static
----------------------------------
make all
----------------------------------

- Xóa nhanh tất cả các file không cần thiết trước khi đẩy lên github bao gồm các file
object (obj), file thực thi (bin), các file thư viện (lib)
----------------------------------
make clean
----------------------------------

Hướng dẫn chạy file thực thi bằng command promt (CMD):
+ Chạy file "main_shared" (file có được sau khi chạy lệnh "make all") 
----------------------------------------------
LD_LIBRARY_PATH=./lib/shared ./bin/main_shared
----------------------------------------------
Chú thích: Để chạy file có chứa thư viện static thì phải có cờ "LD_LIBRARY_PATH=<directory_share_lib>"
và kèm theo phía sau đó là đường dẫn file thực thi "./bin/main_shared"

+ Chạy file "main_static" (file có được sau khi chạy lệnh "make all")
----------------------------------------------
./bin/main_static
----------------------------------------------