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


char *register_names[] = {
    "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rsp", "rbp", // 8
    "r8",  "r9",  "r10", "r11", "r12", "r13", "r14", "r15", // 8
    "eax", "ebx", "ecx", "edx", "esi", "edi", "esp", "ebp", // 8
    "r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d", // 8
    "ax",  "bx",  "cx",  "dx",  "si",  "di",  "sp",  "bp",  // 8
    "r8w", "r9w", "r10w", "r11w", "r12w", "r13w", "r14w", "r15w", // 8
    "al",  "bl",  "cl",  "dl",  "sil", "dil", "spl", "bpl", // 8
    "r8b", "r9b", "r10b", "r11b", "r12b", "r13b", "r14b", "r15b" // 8
    "ah",  "bh",  "ch",  "dh" // 4
};

i64 num_registers = sizeof(register_names) / sizeof(register_names[0]);

char *instruction_names[] = {
    "mov", "add", "sub", "mul", "div", "mod", "and", "or", "xor", "not",
    "shl", "shr", "sar", "rol", "ror",
    "cmp", "test",
    "jmp", "je", "jne", "jg", "jge", "jl", "jle", "jz", "jnz",
    "syscall"
    // ... (more instructions can be added as needed)
};

i64 num_instructions = sizeof(instruction_names) / sizeof(instruction_names[0]);

enum {
    O_RDONLY = 0,
    O_WRONLY = 1,
    O_RDWR   = 2,
    O_CREAT  = 64,      // 0100
    O_EXCL   = 128,     // 0200
    O_NOCTTY = 256,     // 0400
    O_TRUNC  = 512,     // 01000
    O_APPEND = 1024,    // 02000

    O_DIRECTORY = 65536,  // 0200000
    O_NOFOLLOW  = 131072, // 0400000
    O_CLOEXEC   = 524288  // 02000000
};

enum {
    M_OTHER_EXEC  = 1,     // 0001
    M_OTHER_WRITE = 2,     // 0002
    M_OTHER_READ  = 4,     // 0004

    M_GROUP_EXEC  = 8,     // 0010
    M_GROUP_WRITE = 16,    // 0020
    M_GROUP_READ  = 32,    // 0040

    M_OWNER_EXEC  = 64,    // 0100
    M_OWNER_WRITE = 128,   // 0200
    M_OWNER_READ  = 256,   // 0400

    M_STICKY      = 512,   // 01000
    M_SETGID      = 1024,  // 02000
    M_SETUID      = 2048   // 04000
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


typedef struct String {
    char *data;
    i64 length;
} String;


typedef struct ListString {
    String *items;
    i64 count;
} ListString;


typedef struct File {
    char *content;
    i64 size;
    i64 descriptor;
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


bool is_char_in_string(const char c, String string) {
    for (i64 i = 0; i < string.length; i++) {
        if (string.data[i] == c) {
            return true;
        }
    }
    return false;
}


bool is_string_in_array(const char *s, char **array, i64 array_length) {
    for (i64 i = 0; i < array_length; i++) {
        if (string_equals(s, array[i])) {
            return true;
        }
    }
    return false;
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
    file.content = file_content;
    file.size = file_size;
    file.descriptor = fd;
    return file;
}


File create_file(const char *path, i64 flags, i64 mode) {
    File file;
    i64 fd = open_file(path, flags, mode);
    file.content = 0;
    file.size = 0;
    file.descriptor = fd;
    return file;
}


i64 write_file(File file, const char *content, i64 size) {
    return sys_write(file.descriptor, content, size);
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


char *int_cstring(i64 integer) {
    char buffer[21];
    i8 index = 0;
    while (integer > 0) {
        buffer[index++] = '0' + (integer % 10);
        integer /= 10;
    }
    // Reverse the buffer to get the correct order.
    char temp[21];
    for (i8 i = 0; i < index; i++) {
        temp[i] = buffer[index - i - 1];
    }
    temp[index] = '\0';
    return string_duplicate(temp);
}


i64 cstring_int(const char *s) {
    if (get_strlen(s) == 0) {
        return 0;
    }
    if (get_strlen(s) > 20) {
        return 0; // Too long to be a valid integer
    }
    i64 result = 0;
    for (i64 i = 0; s[i] != '\0'; i++) {
        if (s[i] < '0' || s[i] > '9') {
            return 0; // Invalid character
        }
        result = result * 10 + (s[i] - '0');
    }
    return result;
}


bool is_cstring_integer(const char *s) {
    char *hexdigits = "0123456789abcdefABCDEF";
    char *octdigits = "01234567";
    String hex_digits = {
        .data = hexdigits,
        .length = get_strlen(hexdigits)
    };
    String oct_digits = {
        .data = octdigits,
        .length = get_strlen(octdigits)
    };
    
    if (get_strlen(s) == 0) {
        return false;
    }
    if (get_strlen(s) >= 3) {
        if (s[1]=='x'||s[1]=='X') {
            for (i64 i = 2; s[i] != '\0'; i++) {
                if (!is_char_in_string(s[i], hex_digits)) {
                    return false;
                }
            }
        } else if (s[1]=='o'||s[1]=='O'||s[1]=='q'||s[1]=='Q') {
            for (i64 i = 2; s[i] != '\0'; i++) {
                if (!is_char_in_string(s[i], oct_digits)) {
                    return false;
                }
            }
        } else {
            for (i64 i = 0; s[i] != '\0'; i++) {
                if (s[i] < '0' || s[i] > '9') {
                    return false; // Invalid character
                }
            }
        }
    } else {
        for (i64 i = 0; s[i] != '\0'; i++) {
            if (s[i] < '0' || s[i] > '9') {
                return false; // Invalid character
            }
        }
    }
    return true;
}


void print_integer(i64 integer) {
    char *cstring = int_cstring(integer);
    i64 length = get_strlen(cstring);
    sys_write(1, cstring, length);
    sys_munmap(cstring, length + 1);
}


void print_char_integer(char character) {
    i64 char_ = (i64)character;
    print_integer(char_);
    sys_write(1, "\n", 1);
}


void free_string(String s) {
    sys_munmap(s.data, s.length + 1);
}


void free_list_string(ListString list) {
    for (i64 i = 0; i < list.count; i++) {
        free_string(list.items[i]);
    }
    sys_munmap(list.items, list.count * sizeof(String));
}


void free_file(File file) {
    sys_munmap(file.content, file.size + 1);
    sys_close(file.descriptor);
}


void free_cstring(char *cstring) {
    i64 length = get_strlen(cstring);
    sys_munmap(cstring, length + 1);
}


char *string_uppercase(const char *s) {
    i64 len = get_strlen(s);
    char *upper = (char *)sys_mmap(0, len + 1, (i64)(PROT_READ|PROT_WRITE), (i64)(MAP_PRIVATE|MAP_ANONYMOUS), -1, 0);
    for (i64 i = 0; i < len; i++) {
        if (s[i] >= 'a' && s[i] <= 'z') {
            upper[i] = s[i] - ('a' - 'A');
        } else {
            upper[i] = s[i];
        }
    }
    upper[len] = '\0';
    return upper;
}


char *string_lowercase(const char *s) {
    i64 len = get_strlen(s);
    char *lower = (char *)sys_mmap(0, len + 1, (i64)(PROT_READ|PROT_WRITE), (i64)(MAP_PRIVATE|MAP_ANONYMOUS), -1, 0);
    for (i64 i = 0; i < len; i++) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            lower[i] = s[i] + ('a' - 'A');
        } else {
            lower[i] = s[i];
        }
    }
    lower[len] = '\0';
    return lower;
}


typedef enum TokenType {
    TOKEN_REGISTER,
    TOKEN_INSTRUCTION,
    TOKEN_COLON,
    TOKEN_SEMICOLON,
    TOKEN_SYMBOL,
    TOKEN_MEMORY_OPERAND,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_LABEL,
    TOKEN_FUNCTION,
    TOKEN_END,
    TOKEN_INTEGER,
    TOKEN_LOCALS,
    TOKEN_ENDLOCALS,
    TOKEN_TEXT,
    TOKEN_DATA,
    TOKEN_ENDTEXT,
    TOKEN_ENDDATA,
    TOKEN_SIZE,
    TOKEN_WHERE,
    TOKEN_STACK,
    TOKEN_REGISTER_LOC,
    TOKEN_QWORD,
    TOKEN_DWORD,
    TOKEN_WORD,
    TOKEN_BYTE,
    TOKEN_EXTERN,
    TOKEN_EXPORT,
    TOKEN_SAVE_REGS,
    TOKEN_RESTORE_REGS,
    TOKEN_DB,
    TOKEN_DW,
    TOKEN_DD,
    TOKEN_DQ,
    TOKEN_DEFINITION,
    TOKEN_ENDDEFINITION,
    TOKEN_STRUCT_DEF,
    TOKEN_ENDSTRUCT_DEF,
    TOKEN_STRUCT,
    TOKEN_ENDSTRUCT,
    TOKEN_FIELD,
     // ... (more token types can be added as needed)
} TokenType;


typedef struct Token {
    TokenType type;
    String value;
} Token;


typedef struct TokenList {
    Token *items;
    i64 count;
} TokenList;


void free_token(Token token) {
    free_string(token.value);
}


void free_token_list(TokenList list) {
    for (i64 i = 0; i < list.count; i++) {
        free_token(list.items[i]);
    }
    sys_munmap(list.items, list.count * sizeof(Token));
}


void print_token(Token token) {
    char *type_str;
    switch (token.type) {
        case TOKEN_REGISTER: type_str = "REGISTER"; break;
        case TOKEN_INSTRUCTION: type_str = "INSTRUCTION"; break;
        case TOKEN_COLON: type_str = "COLON"; break;
        case TOKEN_SEMICOLON: type_str = "SEMICOLON"; break;
        case TOKEN_SYMBOL: type_str = "SYMBOL"; break;
        case TOKEN_MEMORY_OPERAND: type_str = "MEMORY_OPERAND"; break;
        case TOKEN_LBRACKET: type_str = "LBRACKET"; break;
        case TOKEN_RBRACKET: type_str = "RBRACKET"; break;
        case TOKEN_LABEL: type_str = "LABEL"; break;
        case TOKEN_FUNCTION: type_str = "FUNCTION"; break;
        case TOKEN_END: type_str = "END"; break;
        case TOKEN_INTEGER: type_str = "INTEGER"; break;
        case TOKEN_LOCALS: type_str = "LOCALS"; break;
        case TOKEN_ENDLOCALS: type_str = "ENDLOCALS"; break;
        case TOKEN_TEXT: type_str = "TEXT"; break;
        case TOKEN_DATA: type_str = "DATA"; break;
        case TOKEN_ENDTEXT: type_str = "ENDTEXT"; break;
        case TOKEN_ENDDATA: type_str = "ENDDATA"; break;
        case TOKEN_SIZE: type_str = "SIZE"; break;
        case TOKEN_WHERE: type_str = "WHERE"; break;
        case TOKEN_STACK: type_str = "STACK"; break;
        case TOKEN_REGISTER_LOC: type_str = "REGISTER_LOC"; break;
        case TOKEN_QWORD: type_str = "QWORD"; break;
        case TOKEN_DWORD: type_str = "DWORD"; break;
        case TOKEN_WORD: type_str = "WORD"; break;
        case TOKEN_BYTE: type_str = "BYTE"; break;
        case TOKEN_EXTERN: type_str = "EXTERN"; break;
        case TOKEN_EXPORT: type_str = "EXPORT"; break;
        case TOKEN_SAVE_REGS: type_str = "SAVE_REGS"; break;
        case TOKEN_RESTORE_REGS: type_str = "RESTORE_REGS"; break;
        case TOKEN_DB: type_str = "DB"; break;
        case TOKEN_DW: type_str = "DW"; break;
        case TOKEN_DD: type_str = "DD"; break;
        case TOKEN_DQ: type_str = "DQ"; break;
        case TOKEN_DEFINITION: type_str = "DEFINITION"; break;
        case TOKEN_ENDDEFINITION: type_str = "ENDDEFINTION"; break;
        case TOKEN_STRUCT_DEF: type_str = "STRUCT_DEF"; break;
        case TOKEN_ENDSTRUCT_DEF: type_str = "ENDSTRUCT_DEF"; break;
        case TOKEN_STRUCT: type_str = "STRUCT"; break;
        case TOKEN_ENDSTRUCT: type_str = "ENDSTRUCT"; break;
        case TOKEN_FIELD: type_str = "FIELD"; break;
        default: type_str = "UNKNOWN";
    }
    sys_write(1, "[", 1);
    sys_write(1, type_str, get_strlen(type_str));
    sys_write(1, ": ", 2);
    sys_write(1, token.value.data, token.value.length);
    sys_write(1, "]\n", 2);
}


TokenList tokenize_file(File file) {
    char *cstring = file_content_to_cstring(file);
    ListString unsanitized_strings = split_cstring(cstring, " ,\n\r\t");
    ListString strings = sanitize_list_string(unsanitized_strings);
    free_list_string(unsanitized_strings);
    free_cstring(cstring);

    Token *tokens = (Token *)sys_mmap(0, strings.count * sizeof(Token), (i64)(PROT_READ|PROT_WRITE), (i64)(MAP_PRIVATE|MAP_ANONYMOUS), -1, 0);
    for (i64 i = 0; i < strings.count; i++) {
        Token token;
        if (is_string_in_array(strings.items[i].data, register_names, num_registers)) {
            token.type = TOKEN_REGISTER;
        } else if (is_string_in_array(strings.items[i].data, instruction_names, num_instructions)) {
            token.type = TOKEN_INSTRUCTION;
        } else if (string_equals(strings.items[i].data, ":")) {
            token.type = TOKEN_COLON;
        } else if (string_equals(strings.items[i].data, ";")) {
            token.type = TOKEN_SEMICOLON;
        } else if (string_equals(strings.items[i].data, "[")) {
            token.type = TOKEN_LBRACKET;
        } else if (string_equals(strings.items[i].data, "]")) {
            token.type = TOKEN_RBRACKET;
        } else if (is_cstring_integer(strings.items[i].data)) {
            token.type = TOKEN_INTEGER;
        } else if (string_equals(strings.items[i].data, "extern")) {
            token.type = TOKEN_EXTERN;
        } else if (string_equals(strings.items[i].data, "export")) {
            token.type = TOKEN_EXPORT;
        } else if (string_equals(strings.items[i].data, "locals")) {
            token.type = TOKEN_LOCALS;
        } else if (string_equals(strings.items[i].data, "endlocals")) {
            token.type = TOKEN_ENDLOCALS;
        } else if (string_equals(strings.items[i].data, "TEXT")) {
            token.type = TOKEN_TEXT;
        } else if (string_equals(strings.items[i].data, "DATA")) {
            token.type = TOKEN_DATA;
        } else if (string_equals(strings.items[i].data, "ENDTEXT")) {
            token.type = TOKEN_ENDTEXT;
        } else if (string_equals(strings.items[i].data, "ENDDATA")) {
            token.type = TOKEN_ENDDATA;
        } else if (string_equals(strings.items[i].data, "size")) {
            token.type = TOKEN_SIZE;
        } else if (string_equals(strings.items[i].data, "where")) {
            token.type = TOKEN_WHERE;
        } else if (string_equals(strings.items[i].data, "stack")) {
            token.type = TOKEN_STACK;
        } else if (string_equals(strings.items[i].data, "register")) {
            token.type = TOKEN_REGISTER_LOC;
        } else if (string_equals(string_uppercase(strings.items[i].data), "QWORD")) {
            token.type = TOKEN_QWORD;
        } else if (string_equals(string_uppercase(strings.items[i].data), "DWORD")) {
            token.type = TOKEN_DWORD;
        } else if (string_equals(string_uppercase(strings.items[i].data), "WORD")) {
            token.type = TOKEN_WORD;
        } else if (string_equals(string_uppercase(strings.items[i].data), "BYTE")) {
            token.type = TOKEN_BYTE;
        } else if (string_equals(strings.items[i].data, "end")) {
            token.type = TOKEN_END;
        } else if (string_equals(strings.items[i].data, "function")) {
            token.type = TOKEN_FUNCTION;
        } else if (string_equals(strings.items[i].data, "label")) {
            token.type = TOKEN_LABEL;
        } else if (string_equals(strings.items[i].data, "save_registers")) {
            token.type = TOKEN_SAVE_REGS;
        } else if (string_equals(strings.items[i].data, "restore_registers")) {
            token.type = TOKEN_RESTORE_REGS;
        } else if (string_equals(strings.items[i].data, "db")) {
            token.type = TOKEN_DB;
        } else if (string_equals(strings.items[i].data, "dw")) {
            token.type = TOKEN_DW;
        } else if (string_equals(strings.items[i].data, "dd")) {
            token.type = TOKEN_DD;
        } else if (string_equals(strings.items[i].data, "dq")) {
            token.type = TOKEN_DQ;
        } else if (strings.items[i].data[0] == '[' && strings.items[i].data[strings.items[i].length - 1] == ']') {
            token.type = TOKEN_MEMORY_OPERAND;
        } else if (string_equals(strings.items[i].data, "DEFINITION")) {
            token.type = TOKEN_DEFINITION;
        } else if (string_equals(strings.items[i].data, "ENDDEFINITION")) {
            token.type = TOKEN_ENDDEFINITION;
        } else if (string_equals(strings.items[i].data, "STRUCT_DEF")) {
            token.type = TOKEN_STRUCT_DEF;
        } else if (string_equals(strings.items[i].data, "ENDSTRUCT_DEF")) {
            token.type = TOKEN_ENDSTRUCT_DEF;
        } else if (string_equals(strings.items[i].data, "struct")) {
            token.type = TOKEN_STRUCT;
        } else if (string_equals(strings.items[i].data, "endstruct")) {
            token.type = TOKEN_ENDSTRUCT;
        } else if (string_equals(strings.items[i].data, "field")) {
            token.type = TOKEN_FIELD;
        } else {
            token.type = TOKEN_SYMBOL; // Default to symbol for now
        }

        token.value = copy_string(strings.items[i]);
        tokens[i] = token;
    }

    TokenList result;
    result.items = tokens;
    result.count = strings.count;
    return result;
}


i64 main(i64 argc, char **argv) {
    File file = read_file(argv[1]);
    File output_file = create_file(argv[2], (i64)(O_RDWR | O_CREAT | O_TRUNC), (i64)(M_OWNER_READ | M_GROUP_READ | M_OTHER_READ | M_OWNER_WRITE | M_GROUP_WRITE | M_OTHER_WRITE));

    TokenList tokens = tokenize_file(file);

    for (i64 i = 0; i < tokens.count; i++) {
        print_token(tokens.items[i]);
    }

    free_file(file);
    free_file(output_file);
    free_token_list(tokens);
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