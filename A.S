#include<asm/unistd_64.h>

/*
#define __NR_read 0
#define __NR_write 1
#define __NR_open 2
#define __NR_close 3
#define __NR_fstat 5
#define __NR_mmap 9
#define __NR_munmap 11
#define __NR_exit 60
*/

#define S(n) .globl n;n:;mov %rcx,%r10;mov $__NR_##n,%rax;syscall;ret;
S(read)S(write)S(open)S(close)S(fstat)S(mmap)S(munmap)S(exit)

