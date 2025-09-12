#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>

/* Trả về loại file */
const char *get_file_type(mode_t m) {
    if (S_ISREG(m)) return "Regular File";
    if (S_ISDIR(m)) return "Directory";
    if (S_ISLNK(m)) return "Symbolic Link";
    if (S_ISCHR(m)) return "Character Device";
    if (S_ISBLK(m)) return "Block Device";
    if (S_ISFIFO(m)) return "FIFO/Named Pipe";
    if (S_ISSOCK(m)) return "Socket";
    return "Unknown";
}

/* Hiển thị thông tin file */
void print_file_info(const char *path) {
    struct stat st;

    if (lstat(path, &st) == -1) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    printf("File Path     : %s\n", path);
    printf("File Type     : %s\n", get_file_type(st.st_mode));
    printf("Size          : %lld bytes\n", (long long)st.st_size);

    // Thời gian chỉnh sửa cuối
    char ts[64];
    struct tm *lt = localtime(&st.st_mtime);
    if (lt != NULL && strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", lt) != 0) {
        printf("Last Modified : %s\n", ts);
    } else {
        printf("Last Modified : (time format error)\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return EXIT_FAILURE;
    }

    print_file_info(argv[1]);
    return EXIT_SUCCESS;
}
