// posix_shm_producer.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // O_* constants
#include <sys/mman.h>   // shm_open, mmap
#include <sys/stat.h>   // mode constants
#include <unistd.h>     // ftruncate, close
#include <string.h>

#define SHM_NAME        "/my_shared_mem"
#define SHARED_MEM_SIZE sizeof(Product) 

typedef struct {
    int id;
    char name[50];
    float price;
} Product;

int main() {
    int shm_fd;
    Product *shared_product;

    // 1. Tạo vùng nhớ chia sẻ (đọc/ghi, tạo mới nếu chưa có)
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        exit(EXIT_FAILURE);
    }

    // 2. Cấp kích thước cho vùng nhớ
    if (ftruncate(shm_fd, SHARED_MEM_SIZE) == -1) {
        perror("ftruncate failed");
        exit(EXIT_FAILURE);
    }

    // 3. Ánh xạ vùng nhớ vào không gian địa chỉ tiến trình
    shared_product = mmap(0, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_product == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    // 4. Ghi dữ liệu sản phẩm vào vùng nhớ chia sẻ
    shared_product->id = 101;
    strcpy(shared_product->name, "Laptop Dell XPS 13");
    shared_product->price = 25999.99;

    printf("Producer: Đã ghi thông tin sản phẩm vào bộ nhớ chia sẻ.\n");
    printf("  ID: %d\n  Tên: %s\n  Giá: %.2f\n", 
           shared_product->id, shared_product->name, shared_product->price);

    // 5. Giữ tiến trình để consumer có thể đọc
    printf("\nNhấn Enter để thoát và xóa vùng nhớ chia sẻ...");
    getchar();

    // 6. Giải phóng
    munmap(shared_product, SHARED_MEM_SIZE);
    close(shm_fd);
    shm_unlink(SHM_NAME);  // Xóa vùng nhớ chia sẻ (sau khi dùng xong)

    return 0;
}
