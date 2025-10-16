/* 
 * 修复后的 struct addrinfo 的正确IDL表示
 * 
 * 问题分析：
 * 原始结构体包含自引用指针 struct addrinfo *ai_next，
 * 直接转换为 struct addrinfo_rpc ai_next 会导致循环依赖编译错误。
 * 
 * 解决方案：
 * 1. 使用前向声明避免循环依赖
 * 2. 将自引用指针转换为指针类型而不是值类型
 * 3. 在深度复制时将链表转换为数组传输，接收端重构为链表
 */

/* 前向声明 - 避免循环依赖 */
struct addrinfo_rpc;

/* sockaddr的RPC表示 */
struct sockaddr_rpc {
    int sa_family;
    opaque sa_data<14>;  /* 简化处理，使用opaque数组 */
};
typedef struct sockaddr_rpc sockaddr_rpc;

/* sockaddr指针的RPC表示 */
typedef sockaddr_rpc sockaddr_rpc_ptr<>;

/* char*指针的RPC表示 */
struct char_ptr {
    opaque charptr<>;
};
typedef struct char_ptr char_ptr;

/* 自引用指针的RPC表示 - 关键修复点 */
typedef struct addrinfo_rpc *addrinfo_rpc_ptr;

/* 或者使用数组方式避免递归 */
typedef struct addrinfo_rpc addrinfo_rpc_array<>;

/* addrinfo的正确RPC定义 */
struct addrinfo_rpc {
    int ai_flags;
    int ai_family;
    int ai_socktype;
    int ai_protocol;
    int ai_addrlen;
    sockaddr_rpc_ptr ai_addr;      /* 使用指针包装 */
    char_ptr ai_canonname;         /* 字符串指针 */
    addrinfo_rpc_ptr ai_next;      /* **修复**：使用指针类型而不是值类型 */
};
typedef struct addrinfo_rpc addrinfo_rpc;

/* 
 * 深度复制策略说明：
 * 
 * 客户端编组（链表 -> 数组）：
 * 1. 遍历链表计算长度 n
 * 2. 分配大小为 n 的 addrinfo_rpc 数组
 * 3. 逐个复制链表节点到数组中（忽略ai_next指针）
 * 4. 传输数组和长度信息
 * 
 * 服务端解组（数组 -> 链表）：
 * 1. 接收数组和长度信息
 * 2. 为每个数组元素分配新的 addrinfo 节点
 * 3. 复制数据并重新链接 ai_next 指针
 * 4. 返回重构后的链表头指针
 * 
 * 优势：
 * - 避免了IDL中的循环依赖
 * - 支持任意长度的链表
 * - 内存管理清晰明确
 * 
 * 局限性：
 * - 需要人工实现编组/解组逻辑
 * - 无法自动处理循环链表
 * - 性能开销（需要两次遍历）
 */

/* RPC程序定义 */
program ADDRINFO_RPC {
    version ADDRINFO_RPC_VERS {
        addrinfo_rpc_array get_addrinfo(char_ptr hostname, char_ptr service) = 1;
        void free_addrinfo(addrinfo_rpc_array addrinfo_list) = 2;
    } = 1;
} = 0x20000001;
