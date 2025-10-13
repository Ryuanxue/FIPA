#include "mavlink-server_idl.h"
void receive_heartbeat_sense_1_wrapper(uint8_t buf[2041], ssize_t recsize);

/* 全局变量访问函数声明 */
typedef enum {
    SIG_UNKNOWN
} FunctionSignature;
typedef struct {
    const char *name;         // 函数名
    void *func_ptr_void;      // 存储为 void* 的函数地址
    FunctionSignature signature; // 函数签名标识符
} FunctionMapEntry;
extern FunctionMapEntry function_map[];
void* find_function_ptr_by_name(const char* func_name);
const char* find_function_name_by_address(void* func_address);char *get_filename_from_fp(FILE *fp);
