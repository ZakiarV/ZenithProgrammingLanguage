/*
This file is for the temporary High Level Assembly Code (HLAC) Compiler C version. Once the C version is complete,
the HLAC Compiler will be rewritten in HLAC itself, and this C version will be removed. The C version serves as a 
reference implementation to ensure that the HLAC Compiler's functionality is correct before transitioning to the HACL implementation.
*/


/*
Just some type definitions tp make life easier.
These types are chosen to match the sizes of the corresponding types in C,
and to be clear about their signedness and size.
*/
typedef unsigned long long u64;
typedef long long i64;
typedef unsigned int u32;
typedef int i32;
typedef unsigned short u16;
typedef short i16;
typedef unsigned char u8;
typedef char i8;
typedef int bool;
#define true 1
#define false 0

/*
System calls are being used directly in this implementation to avoid dependencies 
on the C standard library, which will not be available in the implementation
for a long time.
*/

// System call numbers for x86-64 Linux.
enum {
    SYSCALL_READ  = 0,
    SYSCALL_WRITE = 1,
    SYSCALL_CLOSE = 3,
    SYSCALL_STAT  = 4,
    SYSCALL_FSTAT = 5,
    SYSCALL_MMAP   = 9,
    SYSCALL_MUNMAP = 11,
    SYSCALL_EXIT  = 60,
    SYSCALL_OPENAT = 257,
};

// A simple structure to represent file status, similar to struct stat in sys/stat.h.
typedef struct stat {
    unsigned long st_dev;        // 8 bytes, offset 0
    unsigned long st_ino;        // 8 bytes, offset 8
    unsigned long st_nlink;      // 8 bytes, offset 16

    unsigned int  st_mode;       // 4 bytes, offset 24
    unsigned int  st_uid;        // 4 bytes, offset 28
    unsigned int  st_gid;        // 4 bytes, offset 32
    unsigned int  __pad0;        // 4 bytes, offset 36

    unsigned long st_rdev;       // 8 bytes, offset 40

    long          st_size;       // 8 bytes, offset 48
    long          st_blksize;    // 8 bytes, offset 56
    long          st_blocks;     // 8 bytes, offset 64

    unsigned long st_atime;      // 8 bytes, offset 72
    unsigned long st_atime_nsec; // 8 bytes, offset 80

    unsigned long st_mtime;      // 8 bytes, offset 88
    unsigned long st_mtime_nsec; // 8 bytes, offset 96

    unsigned long st_ctime;      // 8 bytes, offset 104
    unsigned long st_ctime_nsec; // 8 bytes, offset 112

    long          __unused[3];   // 24 bytes, offset 120
} stat_t;


// System call implementations using inline assembly.
static i64 linux_syscall(
    i64 call,
    i64 arg1,
    i64 arg2,
    i64 arg3,
    i64 arg4,
    i64 arg5,
    i64 arg6
) {
    register i64 rax asm("rax") = call;
    register i64 rdi asm("rdi") = arg1;
    register i64 rsi asm("rsi") = arg2;
    register i64 rdx asm("rdx") = arg3;
    register i64 r10 asm("r10") = arg4;
    register i64 r8  asm("r8")  = arg5;
    register i64 r9  asm("r9")  = arg6;

    asm volatile (
        "syscall"
        : "+r"(rax)
        : "r"(rdi), "r"(rsi), "r"(rdx), "r"(r10), "r"(r8), "r"(r9)
        : "rcx", "r11", "memory"
    );
    return rax;
}

// A wrapper function to handle system call errors. If the system call returns a negative value, it will exit with that value as the status code.
i64 syscall_wrapper(i64 call,i64 arg1, i64 arg2, i64 arg3, i64 arg4, i64 arg5, i64 arg6) {
    i64 result = linux_syscall(call, arg1, arg2, arg3, arg4, arg5, arg6);
    if (result < 0) {
        linux_syscall(SYSCALL_EXIT, result, 0, 0, 0, 0, 0);
    }
    return result;
}


// Wrappers for specific system calls.
i64 sys_read(i64 fd, void *buf, i64 count) {
    return syscall_wrapper(SYSCALL_READ, fd, (i64)buf, count, 0, 0, 0);
}

i64 sys_write(i64 fd, const void *buf, i64 count) {
    return syscall_wrapper(SYSCALL_WRITE, fd, (i64)buf, count, 0, 0, 0);
}

i64 sys_exit(i64 status) {
    return syscall_wrapper(SYSCALL_EXIT, status, 0, 0, 0, 0, 0);
}

i64 sys_stat(const char *pathname, stat_t *statbuf) {
    return syscall_wrapper(SYSCALL_STAT, (i64)pathname, (i64)statbuf, 0, 0, 0, 0);
}

i64 sys_openat(i64 dirfd, const char *pathname, i64 flags, i64 mode) {
    return syscall_wrapper(SYSCALL_OPENAT, dirfd, (i64)pathname, flags, mode, 0, 0);
}

i64 sys_fstat(i64 fd, stat_t *statbuf) {
    return syscall_wrapper(SYSCALL_FSTAT, fd, (i64)statbuf, 0, 0, 0, 0);
}

void *sys_mmap(void *addr, i64 length, i64 prot, i64 flags, i64 fd, i64 offset) {
    return (void *)syscall_wrapper(SYSCALL_MMAP, (i64)addr, length, prot, flags, fd, offset);
}

i64 sys_munmap(void *addr, i64 length) {
    return syscall_wrapper(SYSCALL_MUNMAP, (i64)addr, length, 0, 0, 0, 0);
}

i64 sys_close(i64 fd) {
    return syscall_wrapper(SYSCALL_CLOSE, fd, 0, 0, 0, 0, 0);
}

/*
End of system call implementations.
*/




/*
Start 
*/

enum {
    O_RDONLY = 0,
    O_WRONLY = 1,
    O_RDWR   = 2,
    O_CREAT  = 64,
    O_EXCL   = 128,
    O_NOCTTY = 256,
    O_TRUNC  = 512,
    O_APPEND  = 1024,
    O_DIRECTORY = 65536,
    O_NOFOLLOW = 131072,
    O_CLOEXEC  = 524288
};

enum {
    M_OTHER_EXEC  = 1,    // 0o1
    M_OTHER_WRITE = 2,    // 0o2
    M_OTHER_READ  = 4,    // 0o4
    M_GROUP_EXEC  = 8,    // 0o10
    M_GROUP_WRITE = 16,   // 0o20
    M_GROUP_READ  = 32,   // 0o40
    M_OWNER_EXEC   = 64,  // 0o100
    M_OWNER_WRITE  = 128, // 0o200
    M_OWNER_READ   = 256, // 0o400
    M_STICKY      = 512,  // 0o1000
    M_SETGID      = 1024, // 0o2000
    M_SETUID      = 2048  // 0o4000
};

enum {
    PROT_READ  = 1,
    PROT_WRITE = 2,
    PROT_EXEC  = 4,
    PROT_NONE  = 0
};

enum {
    MAP_SHARED    = 1,
    MAP_PRIVATE   = 2,
    MAP_FIXED     = 16,
    MAP_ANONYMOUS = 32
};


typedef enum TokenType {
    TOKEN_FUNCTION,
    TOKEN_END,
    TOKEN_LOCALS,
    TOKEN_ENDLOCALS,
    TOKEN_DATA,
    TOKEN_ENDDATA,
    TOKEN_LABEL,
} TokenType;


typedef struct String {
    char *data;
    i64 length;
} String;


typedef struct ListString {
    String *items;
    i64 count;
} ListString;


typedef struct Token {
    String value;
    TokenType type;
} Token;


typedef struct File {
    char *content;
    i64 size;
} File;


i64 get_strlen(const char *s) {
    i64 len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}


stat_t get_file_stat_by_path(const char *path) {
    stat_t statbuf;
    sys_stat(path, &statbuf);
    return statbuf;
}


stat_t get_file_stat_by_fd(i64 fd) {
    stat_t statbuf;
    sys_fstat(fd, &statbuf);
    return statbuf;
}

bool string_equals(const char *s1, const char *s2) {
    i64 len1 = get_strlen(s1);
    i64 len2 = get_strlen(s2);
    if (len1 != len2) {
        return false;
    }
    for (i64 i = 0; i < len1; i++) {
        if (s1[i] != s2[i]) {
            return false;
        }
    }
    return true;
}


i64 open_file(const char *pathname, i64 flags, i64 mode) {
    return sys_openat(-100 /* AT_FDCWD */, pathname, flags, mode);
}


File read_file(const char *path) {
    File file;
    i64 fd = open_file(path, O_RDONLY, (i64)(M_OWNER_READ | M_GROUP_READ | M_OTHER_READ));
    stat_t statbuf = get_file_stat_by_fd(fd);
    i64 file_size = statbuf.st_size;
    char *file_content = (char *)sys_mmap(0, file_size, (i64)(PROT_READ|PROT_WRITE), (i64)(MAP_PRIVATE|MAP_ANONYMOUS), -1, 0);
    sys_read(fd, file_content, file_size);
    sys_close(fd);
    file.content = file_content;
    file.size = file_size;
    return file;
}


char *file_content_to_cstring(File file) {
    char *cstring = (char *)sys_mmap(0, file.size + 1, (i64)(PROT_READ|PROT_WRITE), (i64)(MAP_PRIVATE|MAP_ANONYMOUS), -1, 0);
    for (i64 i = 0; i < file.size; i++) {
        cstring[i] = file.content[i];
    }
    cstring[file.size] = '\0';
    return cstring;
}


char *string_duplicate(const char *s) {
    i64 len = get_strlen(s);
    char *dup = (char *)sys_mmap(0, len + 1, (i64)(PROT_READ|PROT_WRITE), (i64)(MAP_PRIVATE|MAP_ANONYMOUS), -1, 0);
    for (i64 i = 0; i < len; i++) {
        dup[i] = s[i];
    }
    dup[len] = '\0';
    return dup;
}


ListString split_cstring(const char *s, char* delimiter) {
    i64 count = 1; // At least one token
    for (i64 i = 0; s[i] != '\0'; i++) {
        for (i64 j = 0; delimiter[j] != '\0'; j++) {
            if (s[i] == delimiter[j]) {
                count++;
                break;
            }
        }
    }

    String *strings = (String *)sys_mmap(0, count * sizeof(String), (i64)(PROT_READ|PROT_WRITE), (i64)(MAP_PRIVATE|MAP_ANONYMOUS), -1, 0);
    i64 index = 0;
    i64 start = 0;
    for (i64 i = 0; ; i++) {
        bool is_delimiter = false;
        for (i64 j = 0; delimiter[j] != '\0'; j++) {
            if (s[i] == delimiter[j]) {
                is_delimiter = true;
                break;
            }
        }
        if (s[i] == '\0' || is_delimiter) {
            i64 length = i - start;
            strings[index].data = string_duplicate(s + start);
            strings[index].length = length;
            index++;
            start = i + 1;
        }
        if (s[i] == '\0') {
            break;
        }
    }
    ListString result;
    result.items = strings;
    result.count = index;
    return result;
}


String copy_string(String s) {
    char *data = (char *)sys_mmap(0, s.length + 1, (i64)(PROT_READ|PROT_WRITE), (i64)(MAP_PRIVATE|MAP_ANONYMOUS), -1, 0);
    for (i64 i = 0; i < s.length; i++) {
        data[i] = s.data[i];
    }
    data[s.length] = '\0';
    String result;
    result.data = data;
    result.length = s.length;
    return result;
}


ListString sanitize_list_string(ListString list) {
    i64 count = 0;
    for (i64 i = 0; i < list.count; i++) {
        if (list.items[i].length > 0) {
            count++;
        }
    }

    String *strings = (String *)sys_mmap(0, count * sizeof(String), (i64)(PROT_READ|PROT_WRITE), (i64)(MAP_PRIVATE|MAP_ANONYMOUS), -1, 0);
    i64 index = 0;
    for (i64 i = 0; i < list.count; i++) {
        if (list.items[i].length > 0) {
            strings[index] = copy_string(list.items[i]);
            index++;
        }
    }
    ListString result;
    result.items = strings;
    result.count = index;
    return result;
}


i64 main(i64 argc, char **argv) {
    File file = read_file(argv[1]);

    char *cstring = file_content_to_cstring(file);

    ListString tokens = split_cstring(cstring, " ,\t\n");
    ListString sanitized_tokens = sanitize_list_string(tokens);
    
    sys_munmap(file.content, file.size);
    sys_munmap(cstring, get_strlen(cstring) + 1);
    sys_munmap(tokens.items, tokens.count * sizeof(String));

    for (i64 i = 0; i < sanitized_tokens.count; i++) {
        sys_write(1, sanitized_tokens.items[i].data, sanitized_tokens.items[i].length);
        sys_write(1, "\n", 1);
    }

    for (i64 i = 0; i < sanitized_tokens.count; i++) {
        sys_munmap(sanitized_tokens.items[i].data, sanitized_tokens.items[i].length + 1);
    }

    sys_munmap(sanitized_tokens.items, sanitized_tokens.count * sizeof(String));
    return 0;
}


__attribute__((naked))
void _start(void) {
    // Get the arguments from the stack.
    
    int argc;
    char **argv;

    asm volatile (
        "mov (%%rsp), %0\n"   // argc is at the top of the stack
        "lea 8(%%rsp), %1\n"  // argv starts right after argc
        :
        : "r"(argc), "r"(argv)
    );

    i64 result = main(argc, argv);
    sys_exit(result);

    __builtin_unreachable();
}