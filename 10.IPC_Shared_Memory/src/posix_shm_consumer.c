// posix_shm_consumer.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // O_* constants
#include <sys/mman.h>   // shm_open, mmap
#include <sys/stat.h>   // mode constants
#include <unistd.h>     // close
#include <string.h>

#define SHM_NAME "/my_shared_mem"
#define SHARED_MEM_SIZE sizeof(Product) 

typedef struct {
    int id;
    char name[50];
    float price;
} Product;

int main() {
    int shm_fd;
    Product *shared_product;

    // 1. Mở lại vùng nhớ chia sẻ (được tạo bởi producer)
    shm_fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed (hãy chạy producer trước)");
        exit(EXIT_FAILURE);
    }

    // 2. Ánh xạ vào không gian tiến trình (chỉ đọc)
    shared_product = mmap(NULL, SHARED_MEM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shared_product == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    // 3. Đọc và hiển thị dữ liệu
    printf("Consumer: Đã đọc thông tin sản phẩm từ bộ nhớ chia sẻ.\n");
    printf("  ID: %d\n  Tên: %s\n  Giá: %.2f\n", 
           shared_product->id, shared_product->name, shared_product->price);

    // 4. Giải phóng
    munmap(shared_product, SHARED_MEM_SIZE);
    close(shm_fd);

    return 0;
}
