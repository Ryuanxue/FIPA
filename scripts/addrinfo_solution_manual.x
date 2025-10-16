/* 
 * 手动处理自引用结构体的Sun RPC IDL解决方案
 * 
 * 问题: struct addrinfo 包含自引用指针 ai_next
 * 解决: 使用数组方式传输链表，避免循环引用
 */

/* sockaddr相关结构体 */
struct sockaddr_rpc {
    unsigned short sa_family;
    opaque sa_data<14>;
};

typedef struct sockaddr_rpc sockaddr_rpc_ptr<>;

/* 字符串指针类型 */
struct char_ptr {
    opaque charptr<>;
};
typedef struct char_ptr char_ptr;

/* 
 * 方案1: 扁平化的addrinfo结构 - 移除自引用指针
 * 适用于单个addrinfo对象的传输
 */
struct addrinfo_single_rpc {
    int ai_flags;
    int ai_family;
    int ai_socktype;
    int ai_protocol;
    int ai_addrlen;
    sockaddr_rpc_ptr ai_addr;
    char_ptr ai_canonname;
    /* 注意: 移除了ai_next字段 */
};

/* 
 * 方案2: 数组化的addrinfo结构 - 将链表转换为数组
 * 适用于传输整个addrinfo链表
 */
struct addrinfo_array_rpc {
    struct addrinfo_single_rpc addrinfo_list<>;  /* 动态数组 */
};

/* 函数返回结构体 */
struct getaddrinfo_ret_ {
    int ret0;                           /* 返回值 */
    addrinfo_array_rpc result_list;     /* 结果链表数组化 */
};

/* RPC程序定义 */
program RPCTELNET {
    version RPCTELNETVERS {
        getaddrinfo_ret_ getaddrinfo_rpc(
            char_ptr node,
            char_ptr service,
            addrinfo_single_rpc hints
        ) = 1;
    } = 1;
} = 0x20000001;
