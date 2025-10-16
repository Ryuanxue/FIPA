/*
 * 手动处理自引用结构体的深度复制实现
 * 
 * 这个文件展示了如何在RPC wrapper中实现
 * struct addrinfo链表到数组的转换
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include "telnet_idl.h"  /* 由rpcgen生成的头文件 */

/* 
 * 将struct addrinfo链表转换为RPC可传输的数组结构
 */
addrinfo_array_rpc* convert_addrinfo_to_rpc_array(struct addrinfo *ai_list) {
    if (!ai_list) return NULL;
    
    // 首先统计链表长度
    int count = 0;
    struct addrinfo *current = ai_list;
    while (current) {
        count++;
        current = current->ai_next;
    }
    
    // 分配RPC结构体内存
    addrinfo_array_rpc *rpc_array = malloc(sizeof(addrinfo_array_rpc));
    if (!rpc_array) return NULL;
    
    // 分配数组内存
    rpc_array->addrinfo_list.addrinfo_list_len = count;
    rpc_array->addrinfo_list.addrinfo_list_val = 
        malloc(count * sizeof(struct addrinfo_single_rpc));
    
    if (!rpc_array->addrinfo_list.addrinfo_list_val) {
        free(rpc_array);
        return NULL;
    }
    
    // 转换每个链表节点到数组元素
    current = ai_list;
    for (int i = 0; i < count; i++) {
        struct addrinfo_single_rpc *rpc_item = 
            &rpc_array->addrinfo_list.addrinfo_list_val[i];
        
        // 复制基本字段
        rpc_item->ai_flags = current->ai_flags;
        rpc_item->ai_family = current->ai_family;
        rpc_item->ai_socktype = current->ai_socktype;
        rpc_item->ai_protocol = current->ai_protocol;
        rpc_item->ai_addrlen = current->ai_addrlen;
        
        // 处理sockaddr结构体指针
        if (current->ai_addr) {
            rpc_item->ai_addr.sockaddr_rpc_ptr_len = 1;
            rpc_item->ai_addr.sockaddr_rpc_ptr_val = 
                malloc(sizeof(struct sockaddr_rpc));
            
            if (rpc_item->ai_addr.sockaddr_rpc_ptr_val) {
                rpc_item->ai_addr.sockaddr_rpc_ptr_val->sa_family = 
                    current->ai_addr->sa_family;
                
                // 复制sa_data (最多14字节)
                rpc_item->ai_addr.sockaddr_rpc_ptr_val->sa_data.sa_data_len = 
                    (current->ai_addrlen > 14) ? 14 : current->ai_addrlen - 2;
                rpc_item->ai_addr.sockaddr_rpc_ptr_val->sa_data.sa_data_val = 
                    malloc(rpc_item->ai_addr.sockaddr_rpc_ptr_val->sa_data.sa_data_len);
                
                if (rpc_item->ai_addr.sockaddr_rpc_ptr_val->sa_data.sa_data_val) {
                    memcpy(rpc_item->ai_addr.sockaddr_rpc_ptr_val->sa_data.sa_data_val,
                           current->ai_addr->sa_data,
                           rpc_item->ai_addr.sockaddr_rpc_ptr_val->sa_data.sa_data_len);
                }
            }
        } else {
            rpc_item->ai_addr.sockaddr_rpc_ptr_len = 0;
            rpc_item->ai_addr.sockaddr_rpc_ptr_val = NULL;
        }
        
        // 处理ai_canonname字符串
        if (current->ai_canonname) {
            int len = strlen(current->ai_canonname);
            rpc_item->ai_canonname.charptr.charptr_len = len;
            rpc_item->ai_canonname.charptr.charptr_val = malloc(len);
            if (rpc_item->ai_canonname.charptr.charptr_val) {
                memcpy(rpc_item->ai_canonname.charptr.charptr_val, 
                       current->ai_canonname, len);
            }
        } else {
            rpc_item->ai_canonname.charptr.charptr_len = 0;
            rpc_item->ai_canonname.charptr.charptr_val = NULL;
        }
        
        current = current->ai_next;
    }
    
    return rpc_array;
}

/* 
 * 将RPC数组结构转换回struct addrinfo链表
 */
struct addrinfo* convert_rpc_array_to_addrinfo(addrinfo_array_rpc *rpc_array) {
    if (!rpc_array || rpc_array->addrinfo_list.addrinfo_list_len == 0) {
        return NULL;
    }
    
    struct addrinfo *head = NULL, *prev = NULL;
    
    for (int i = 0; i < rpc_array->addrinfo_list.addrinfo_list_len; i++) {
        struct addrinfo_single_rpc *rpc_item = 
            &rpc_array->addrinfo_list.addrinfo_list_val[i];
        
        // 分配新的addrinfo节点
        struct addrinfo *ai = malloc(sizeof(struct addrinfo));
        if (!ai) break;
        
        // 复制基本字段
        ai->ai_flags = rpc_item->ai_flags;
        ai->ai_family = rpc_item->ai_family;
        ai->ai_socktype = rpc_item->ai_socktype;
        ai->ai_protocol = rpc_item->ai_protocol;
        ai->ai_addrlen = rpc_item->ai_addrlen;
        ai->ai_next = NULL;
        
        // 恢复sockaddr结构体
        if (rpc_item->ai_addr.sockaddr_rpc_ptr_len > 0 && 
            rpc_item->ai_addr.sockaddr_rpc_ptr_val) {
            
            ai->ai_addr = malloc(sizeof(struct sockaddr));
            if (ai->ai_addr) {
                ai->ai_addr->sa_family = 
                    rpc_item->ai_addr.sockaddr_rpc_ptr_val->sa_family;
                
                if (rpc_item->ai_addr.sockaddr_rpc_ptr_val->sa_data.sa_data_len > 0) {
                    memcpy(ai->ai_addr->sa_data,
                           rpc_item->ai_addr.sockaddr_rpc_ptr_val->sa_data.sa_data_val,
                           rpc_item->ai_addr.sockaddr_rpc_ptr_val->sa_data.sa_data_len);
                }
            }
        } else {
            ai->ai_addr = NULL;
        }
        
        // 恢复ai_canonname字符串
        if (rpc_item->ai_canonname.charptr.charptr_len > 0 && 
            rpc_item->ai_canonname.charptr.charptr_val) {
            
            ai->ai_canonname = malloc(rpc_item->ai_canonname.charptr.charptr_len + 1);
            if (ai->ai_canonname) {
                memcpy(ai->ai_canonname, 
                       rpc_item->ai_canonname.charptr.charptr_val,
                       rpc_item->ai_canonname.charptr.charptr_len);
                ai->ai_canonname[rpc_item->ai_canonname.charptr.charptr_len] = '\0';
            }
        } else {
            ai->ai_canonname = NULL;
        }
        
        // 链接到链表
        if (!head) {
            head = ai;
        } else {
            prev->ai_next = ai;
        }
        prev = ai;
    }
    
    return head;
}

/* 
 * RPC wrapper函数示例
 */
getaddrinfo_ret_* getaddrinfo_rpc_wrapper(char *node, char *service, 
                                          struct addrinfo *hints) {
    getaddrinfo_ret_ *result = malloc(sizeof(getaddrinfo_ret_));
    if (!result) return NULL;
    
    // 调用原始getaddrinfo函数
    struct addrinfo *ai_result = NULL;
    result->ret0 = getaddrinfo(node, service, hints, &ai_result);
    
    if (result->ret0 == 0 && ai_result) {
        // 成功时转换结果
        addrinfo_array_rpc *rpc_array = convert_addrinfo_to_rpc_array(ai_result);
        if (rpc_array) {
            result->result_list = *rpc_array;
            free(rpc_array);  // 只释放容器，不释放内容
        } else {
            // 转换失败
            result->result_list.addrinfo_list.addrinfo_list_len = 0;
            result->result_list.addrinfo_list.addrinfo_list_val = NULL;
        }
        
        // 释放原始结果
        freeaddrinfo(ai_result);
    } else {
        // 失败时返回空数组
        result->result_list.addrinfo_list.addrinfo_list_len = 0;
        result->result_list.addrinfo_list.addrinfo_list_val = NULL;
    }
    
    return result;
}

/*
 * 使用示例和测试代码
 */
void test_addrinfo_conversion() {
    printf("=== 测试struct addrinfo链表到数组的转换 ===\n");
    
    // 模拟getaddrinfo调用
    struct addrinfo hints = {0};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    struct addrinfo *result = NULL;
    int ret = getaddrinfo("www.example.com", "80", &hints, &result);
    
    if (ret == 0 && result) {
        printf("原始链表包含 %d 个节点\n", count_addrinfo_nodes(result));
        
        // 转换为RPC数组格式
        addrinfo_array_rpc *rpc_array = convert_addrinfo_to_rpc_array(result);
        if (rpc_array) {
            printf("转换为数组: %u 个元素\n", 
                   rpc_array->addrinfo_list.addrinfo_list_len);
            
            // 转换回链表格式
            struct addrinfo *restored = convert_rpc_array_to_addrinfo(rpc_array);
            if (restored) {
                printf("恢复链表包含 %d 个节点\n", count_addrinfo_nodes(restored));
                freeaddrinfo(restored);
            }
            
            // 清理RPC数组内存 (这里需要实现complete cleanup函数)
            // cleanup_rpc_array(rpc_array);
        }
        
        freeaddrinfo(result);
    } else {
        printf("getaddrinfo失败: %s\n", gai_strerror(ret));
    }
}

int count_addrinfo_nodes(struct addrinfo *ai) {
    int count = 0;
    while (ai) {
        count++;
        ai = ai->ai_next;
    }
    return count;
}
