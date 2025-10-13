#include "thttpd_idl.h"
int auth_check2_sense_1_wrapper(int *auth_check2_sense_1_ret, httpd_conn *hc, char *dirname);
void make_log_entry_sense_1_wrapper(char *ru, char url[305], char bytes[40], httpd_conn *hc, struct timeval *nowP);

/* 全局变量访问函数声明 */
int get_str_alloc_count_wrapper();
int set_str_alloc_count_wrapper(int value);
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
int gsetter_str_alloc_count_prefix_wrapper();
