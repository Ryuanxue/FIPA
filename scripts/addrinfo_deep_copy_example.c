/*
 * 自引用结构体的深度复制实现示例
 * 以 struct addrinfo 为例展示如何正确处理链表结构的RPC传输
 */

#include "addrinfo_fixed_example.h"
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

/* ========================================
 * 客户端编组函数：将链表转换为数组
 * ======================================== */

/**
 * 计算链表长度
 */
static int count_addrinfo_list(struct addrinfo *head) {
    int count = 0;
    struct addrinfo *current = head;
    while (current != NULL) {
        count++;
        current = current->ai_next;
    }
    return count;
}

/**
 * 编组单个 addrinfo 节点
 */
static void marshal_single_addrinfo(struct addrinfo *src, addrinfo_rpc *dst) {
    if (!src || !dst) return;
    
    // 复制基本字段
    dst->ai_flags = src->ai_flags;
    dst->ai_family = src->ai_family;
    dst->ai_socktype = src->ai_socktype;
    dst->ai_protocol = src->ai_protocol;
    dst->ai_addrlen = src->ai_addrlen;
    
    // 处理 sockaddr 指针
    if (src->ai_addr) {
        dst->ai_addr.sockaddr_rpc_ptr_len = 1;
        dst->ai_addr.sockaddr_rpc_ptr_val = malloc(sizeof(sockaddr_rpc));
        if (dst->ai_addr.sockaddr_rpc_ptr_val) {
            dst->ai_addr.sockaddr_rpc_ptr_val->sa_family = src->ai_addr->sa_family;
            // 复制 sa_data（简化处理）
            dst->ai_addr.sockaddr_rpc_ptr_val->sa_data.sa_data_len = 14;
            dst->ai_addr.sockaddr_rpc_ptr_val->sa_data.sa_data_val = malloc(14);
            if (dst->ai_addr.sockaddr_rpc_ptr_val->sa_data.sa_data_val) {
                memcpy(dst->ai_addr.sockaddr_rpc_ptr_val->sa_data.sa_data_val, 
                       src->ai_addr->sa_data, 14);
            }
        }
    } else {
        dst->ai_addr.sockaddr_rpc_ptr_len = 0;
        dst->ai_addr.sockaddr_rpc_ptr_val = NULL;
    }
    
    // 处理 canonname 字符串指针
    if (src->ai_canonname) {
        size_t len = strlen(src->ai_canonname) + 1;
        dst->ai_canonname.charptr.charptr_len = len;
        dst->ai_canonname.charptr.charptr_val = malloc(len);
        if (dst->ai_canonname.charptr.charptr_val) {
            strcpy((char*)dst->ai_canonname.charptr.charptr_val, src->ai_canonname);
        }
    } else {
        dst->ai_canonname.charptr.charptr_len = 0;
        dst->ai_canonname.charptr.charptr_val = NULL;
    }
    
    // **关键点**：不处理 ai_next，因为我们将链表转换为数组
    dst->ai_next = NULL;
}

/**
 * 客户端包装函数：编组整个链表
 */
addrinfo_rpc_array marshal_addrinfo_list(struct addrinfo *head) {
    addrinfo_rpc_array result = {0};
    
    if (!head) {
        return result;
    }
    
    // 计算链表长度
    int count = count_addrinfo_list(head);
    
    // 分配RPC数组
    result.addrinfo_rpc_array_len = count;
    result.addrinfo_rpc_array_val = malloc(count * sizeof(addrinfo_rpc));
    
    if (!result.addrinfo_rpc_array_val) {
        result.addrinfo_rpc_array_len = 0;
        return result;
    }
    
    // 逐个编组链表节点
    struct addrinfo *current = head;
    for (int i = 0; i < count && current; i++) {
        marshal_single_addrinfo(current, &result.addrinfo_rpc_array_val[i]);
        current = current->ai_next;
    }
    
    return result;
}

/* ========================================
 * 服务端解组函数：将数组重构为链表
 * ======================================== */

/**
 * 解组单个 addrinfo 节点
 */
static struct addrinfo* unmarshal_single_addrinfo(addrinfo_rpc *src) {
    if (!src) return NULL;
    
    struct addrinfo *dst = malloc(sizeof(struct addrinfo));
    if (!dst) return NULL;
    
    memset(dst, 0, sizeof(struct addrinfo));
    
    // 复制基本字段
    dst->ai_flags = src->ai_flags;
    dst->ai_family = src->ai_family;
    dst->ai_socktype = src->ai_socktype;
    dst->ai_protocol = src->ai_protocol;
    dst->ai_addrlen = src->ai_addrlen;
    
    // 处理 sockaddr 指针
    if (src->ai_addr.sockaddr_rpc_ptr_len > 0 && src->ai_addr.sockaddr_rpc_ptr_val) {
        dst->ai_addr = malloc(sizeof(struct sockaddr));
        if (dst->ai_addr) {
            dst->ai_addr->sa_family = src->ai_addr.sockaddr_rpc_ptr_val->sa_family;
            // 复制 sa_data
            if (src->ai_addr.sockaddr_rpc_ptr_val->sa_data.sa_data_len > 0) {
                memcpy(dst->ai_addr->sa_data, 
                       src->ai_addr.sockaddr_rpc_ptr_val->sa_data.sa_data_val,
                       min(14, src->ai_addr.sockaddr_rpc_ptr_val->sa_data.sa_data_len));
            }
        }
    }
    
    // 处理 canonname 字符串指针
    if (src->ai_canonname.charptr.charptr_len > 0 && src->ai_canonname.charptr.charptr_val) {
        dst->ai_canonname = malloc(src->ai_canonname.charptr.charptr_len);
        if (dst->ai_canonname) {
            memcpy(dst->ai_canonname, src->ai_canonname.charptr.charptr_val, 
                   src->ai_canonname.charptr.charptr_len);
        }
    }
    
    // ai_next 将在重构链表时设置
    dst->ai_next = NULL;
    
    return dst;
}

/**
 * 服务端包装函数：解组数组并重构链表
 */
struct addrinfo* unmarshal_addrinfo_list(addrinfo_rpc_array src) {
    if (src.addrinfo_rpc_array_len == 0 || !src.addrinfo_rpc_array_val) {
        return NULL;
    }
    
    struct addrinfo *head = NULL;
    struct addrinfo *prev = NULL;
    
    // 逐个解组并重构链表
    for (int i = 0; i < src.addrinfo_rpc_array_len; i++) {
        struct addrinfo *node = unmarshal_single_addrinfo(&src.addrinfo_rpc_array_val[i]);
        if (!node) {
            // 出错时清理已分配的内存
            // TODO: 实现清理函数
            continue;
        }
        
        if (!head) {
            head = node;  // 第一个节点
        } else {
            prev->ai_next = node;  // 链接到前一个节点
        }
        prev = node;
    }
    
    return head;
}

/* ========================================
 * 内存管理函数
 * ======================================== */

/**
 * 释放编组后的RPC数组内存
 */
void free_marshaled_addrinfo(addrinfo_rpc_array *rpc_array) {
    if (!rpc_array || !rpc_array->addrinfo_rpc_array_val) return;
    
    for (int i = 0; i < rpc_array->addrinfo_rpc_array_len; i++) {
        addrinfo_rpc *item = &rpc_array->addrinfo_rpc_array_val[i];
        
        // 释放 sockaddr
        if (item->ai_addr.sockaddr_rpc_ptr_val) {
            if (item->ai_addr.sockaddr_rpc_ptr_val->sa_data.sa_data_val) {
                free(item->ai_addr.sockaddr_rpc_ptr_val->sa_data.sa_data_val);
            }
            free(item->ai_addr.sockaddr_rpc_ptr_val);
        }
        
        // 释放 canonname
        if (item->ai_canonname.charptr.charptr_val) {
            free(item->ai_canonname.charptr.charptr_val);
        }
    }
    
    free(rpc_array->addrinfo_rpc_array_val);
    rpc_array->addrinfo_rpc_array_val = NULL;
    rpc_array->addrinfo_rpc_array_len = 0;
}

/**
 * 释放解组后的addrinfo链表
 */
void free_unmarshaled_addrinfo(struct addrinfo *head) {
    struct addrinfo *current = head;
    while (current) {
        struct addrinfo *next = current->ai_next;
        
        if (current->ai_addr) {
            free(current->ai_addr);
        }
        if (current->ai_canonname) {
            free(current->ai_canonname);
        }
        free(current);
        
        current = next;
    }
}

/* ========================================
 * 使用示例
 * ======================================== */

/**
 * 客户端调用示例
 */
struct addrinfo* client_getaddrinfo_wrapper(const char* hostname, const char* service) {
    // 1. 准备RPC参数
    char_ptr hostname_rpc = {0};
    char_ptr service_rpc = {0};
    
    if (hostname) {
        size_t len = strlen(hostname) + 1;
        hostname_rpc.charptr.charptr_len = len;
        hostname_rpc.charptr.charptr_val = (u_char*)strdup(hostname);
    }
    
    if (service) {
        size_t len = strlen(service) + 1;
        service_rpc.charptr.charptr_len = len;
        service_rpc.charptr.charptr_val = (u_char*)strdup(service);
    }
    
    // 2. 调用RPC
    addrinfo_rpc_array *result_rpc = get_addrinfo_1(hostname_rpc, service_rpc, clnt);
    
    // 3. 检查RPC调用结果
    if (!result_rpc) {
        clnt_perror(clnt, "getaddrinfo RPC call failed");
        return NULL;
    }
    
    // 4. 解组RPC结果为链表
    struct addrinfo *result = unmarshal_addrinfo_list(*result_rpc);
    
    // 5. 清理临时内存
    if (hostname_rpc.charptr.charptr_val) free(hostname_rpc.charptr.charptr_val);
    if (service_rpc.charptr.charptr_val) free(service_rpc.charptr.charptr_val);
    
    return result;
}

/**
 * 服务端实现示例
 */
addrinfo_rpc_array* get_addrinfo_1_svc(char_ptr hostname, char_ptr service, struct svc_req *rqstp) {
    static addrinfo_rpc_array result;
    
    // 1. 解组RPC参数
    char *hostname_str = NULL;
    char *service_str = NULL;
    
    if (hostname.charptr.charptr_len > 0 && hostname.charptr.charptr_val) {
        hostname_str = malloc(hostname.charptr.charptr_len);
        memcpy(hostname_str, hostname.charptr.charptr_val, hostname.charptr.charptr_len);
        hostname_str[hostname.charptr.charptr_len - 1] = '\0';  // 确保null终止
    }
    
    if (service.charptr.charptr_len > 0 && service.charptr.charptr_val) {
        service_str = malloc(service.charptr.charptr_len);
        memcpy(service_str, service.charptr.charptr_val, service.charptr.charptr_len);
        service_str[service.charptr.charptr_len - 1] = '\0';  // 确保null终止
    }
    
    // 2. 调用真正的系统函数
    struct addrinfo *addrinfo_list = NULL;
    int ret = getaddrinfo(hostname_str, service_str, NULL, &addrinfo_list);
    
    // 3. 处理调用结果
    if (ret != 0) {
        // 错误处理
        result.addrinfo_rpc_array_len = 0;
        result.addrinfo_rpc_array_val = NULL;
    } else {
        // 4. 编组结果为RPC数组
        result = marshal_addrinfo_list(addrinfo_list);
        
        // 5. 释放原始链表（已复制到RPC结构中）
        if (addrinfo_list) {
            freeaddrinfo(addrinfo_list);
        }
    }
    
    // 6. 清理临时内存
    if (hostname_str) free(hostname_str);
    if (service_str) free(service_str);
    
    return &result;
}
