#include "nginx-1.15.5_idl.h"
#include "ngx_http.h"

// 定义Nginx auth basic配置结构（与模块中的定义保持一致）
typedef struct {
    ngx_http_complex_value_t  *realm;
    ngx_http_complex_value_t   user_file;
} ngx_http_auth_basic_loc_conf_t;

ngx_int_t ngx_http_auth_basic_handler_sense_1(int *ngx_http_auth_basic_handler_sense_1_ret, ngx_http_request_t *r);

ngx_http_auth_basic_handler_sense_1_ret_ *ngx_http_auth_basic_handler_sense_1_rpc_1_svc(int_ptr arg1,ngx_http_request_t_rpc_ptr arg2,struct svc_req *rqstp)
{
    printf("RPC service called\n");
	static ngx_http_auth_basic_handler_sense_1_ret_ result;
int *ngx_http_auth_basic_handler_sense_1_ret=NULL;
if(arg1.int_ptr_val!=NULL) {
ngx_http_auth_basic_handler_sense_1_ret=malloc(arg1.int_ptr_len*sizeof(int));
if(ngx_http_auth_basic_handler_sense_1_ret==NULL) {
perror("malloc failed");
exit(1);
}
for(int i0=0;i0<arg1.int_ptr_len;i0++) {
ngx_http_auth_basic_handler_sense_1_ret[i0]=arg1.int_ptr_val[i0];
	}
}
ngx_http_request_t *r=NULL;
r = (ngx_http_request_t *)malloc(sizeof(ngx_http_request_t));
	if (r == NULL) {
		perror("malloc");
		exit(1);
	}

    // 检查 arg2 是否有有效数据
	if (arg2.ngx_http_request_t_rpc_ptr_len > 0 && arg2.ngx_http_request_t_rpc_ptr_val != NULL) {
		ngx_http_request_t_rpc *rpc_request = arg2.ngx_http_request_t_rpc_ptr_val;

// 从 RPC 结构体恢复基本字段到本地 request 结构体
		r->signature = rpc_request->signature;
		
		// 分配 loc_conf 数组内存（8个模块配置指针）
		r->loc_conf = (void **)malloc(8 * sizeof(void *));
		if (r->loc_conf == NULL) {
			perror("malloc loc_conf");
			free(r);
			exit(1);
		}
		
		// 初始化前7个配置指针为 NULL
		for (int i = 0; i < 7; i++) {
			r->loc_conf[i] = NULL;
		}
		
		// 处理第8个配置（auth_basic 模块配置）
		if (rpc_request->loc_conf.ngx_http_auth_basic_loc_conf_t_rpc_twoptr_len >= 8 &&
			rpc_request->loc_conf.ngx_http_auth_basic_loc_conf_t_rpc_twoptr_val != NULL) {
			
			ngx_http_auth_basic_loc_conf_t_rpc_ptr *loc_conf_array = 
				rpc_request->loc_conf.ngx_http_auth_basic_loc_conf_t_rpc_twoptr_val;
			
			// 检查第8个元素是否有有效配置
			if (loc_conf_array[7].ngx_http_auth_basic_loc_conf_t_rpc_ptr_len > 0 &&
				loc_conf_array[7].ngx_http_auth_basic_loc_conf_t_rpc_ptr_val != NULL) {
				
				ngx_http_auth_basic_loc_conf_t_rpc *auth_conf = 
					loc_conf_array[7].ngx_http_auth_basic_loc_conf_t_rpc_ptr_val;
				
				// 创建本地的 auth_basic 配置结构体
				ngx_http_auth_basic_loc_conf_t *local_auth_conf = 
					(ngx_http_auth_basic_loc_conf_t *)malloc(sizeof(ngx_http_auth_basic_loc_conf_t));
				if (local_auth_conf == NULL) {
					perror("malloc local_auth_conf");
					exit(1);
				}
				memset(local_auth_conf, 0, sizeof(ngx_http_auth_basic_loc_conf_t));
				
				// 恢复 realm 字段
				if (auth_conf->realm.ngx_http_complex_value_t_rpc_ptr_len > 0 &&
					auth_conf->realm.ngx_http_complex_value_t_rpc_ptr_val != NULL) {
					
					ngx_http_complex_value_t_rpc *realm_value = 
						auth_conf->realm.ngx_http_complex_value_t_rpc_ptr_val;
					
					// 创建本地的 realm 结构体
					local_auth_conf->realm = (ngx_http_complex_value_t *)malloc(sizeof(ngx_http_complex_value_t));
					if (local_auth_conf->realm == NULL) {
						perror("malloc realm");
						free(local_auth_conf);
						exit(1);
					}
					memset(local_auth_conf->realm, 0, sizeof(ngx_http_complex_value_t));
					
					// 深拷贝 realm 字符串数据
					if (realm_value->value.data.charptr.charptr_len > 0 &&
						realm_value->value.data.charptr.charptr_val != NULL) {
						
						local_auth_conf->realm->value.len = realm_value->value.len;
						local_auth_conf->realm->value.data = (u_char *)malloc(realm_value->value.len + 1);
						if (local_auth_conf->realm->value.data == NULL) {
							perror("malloc realm data");
							free(local_auth_conf->realm);
							free(local_auth_conf);
							exit(1);
						}
						memcpy(local_auth_conf->realm->value.data, 
							   realm_value->value.data.charptr.charptr_val, 
							   realm_value->value.len);
						local_auth_conf->realm->value.data[realm_value->value.len] = '\0';
						
						printf("  Realm: %.*s\n", 
							realm_value->value.len,
							realm_value->value.data.charptr.charptr_val);
					}
					
					local_auth_conf->realm->flushes = NULL;
					local_auth_conf->realm->lengths = NULL;
					local_auth_conf->realm->values = NULL;
				} else {
					local_auth_conf->realm = NULL;
				}
				
				// 恢复 user_file 字段
				if (auth_conf->user_file.value.data.charptr.charptr_len > 0 &&
					auth_conf->user_file.value.data.charptr.charptr_val != NULL) {
					
					local_auth_conf->user_file.value.len = auth_conf->user_file.value.len;
					local_auth_conf->user_file.value.data = (u_char *)malloc(auth_conf->user_file.value.len + 1);
					if (local_auth_conf->user_file.value.data == NULL) {
						perror("malloc user_file data");
						if (local_auth_conf->realm) {
							free(local_auth_conf->realm->value.data);
							free(local_auth_conf->realm);
						}
						free(local_auth_conf);
						exit(1);
					}
					memcpy(local_auth_conf->user_file.value.data,
						   auth_conf->user_file.value.data.charptr.charptr_val,
						   auth_conf->user_file.value.len);
					local_auth_conf->user_file.value.data[auth_conf->user_file.value.len] = '\0';
					
					printf("  User file: %.*s\n",
						auth_conf->user_file.value.len,
						auth_conf->user_file.value.data.charptr.charptr_val);
				}else {
					// 如果没有user_file数据，确保字段被正确初始化为NULL
					local_auth_conf->user_file.value.len = 0;
					local_auth_conf->user_file.value.data = NULL;
				}
				
				local_auth_conf->user_file.flushes = NULL;
				local_auth_conf->user_file.lengths = NULL;
				local_auth_conf->user_file.values = NULL;
				
				// 将本地配置结构体赋值给 request->loc_conf[7]
				r->loc_conf[7] = local_auth_conf;
			} else {
				r->loc_conf[7] = NULL;
			}
		} else {
			r->loc_conf[7] = NULL;
		}
		
		printf("Request restored successfully. Signature: 0x%x\n", r->signature);
	
        // 解封装并深度拷贝 headers_in 字段
		// 1. 拷贝 user 字段
		if (rpc_request->headers_in.user.data.charptr.charptr_len > 0 &&
			rpc_request->headers_in.user.data.charptr.charptr_val != NULL) {
			
			r->headers_in.user.len = rpc_request->headers_in.user.len;
			r->headers_in.user.data = (u_char *)malloc(r->headers_in.user.len + 1);
			if (r->headers_in.user.data == NULL) {
				perror("malloc user data");
				free(r);
				result.ret0 = -1;
				return &result;
			}
			memcpy(r->headers_in.user.data, 
				   rpc_request->headers_in.user.data.charptr.charptr_val,
				   r->headers_in.user.len);
			r->headers_in.user.data[r->headers_in.user.len] = '\0';
			
			printf("User: %.*s\n", (int)r->headers_in.user.len, (char *)r->headers_in.user.data);
		} else {
			r->headers_in.user.len = 0;
			r->headers_in.user.data = NULL;
		}
		
		// 2. 拷贝 passwd 字段
		if (rpc_request->headers_in.passwd.data.charptr.charptr_len > 0 &&
			rpc_request->headers_in.passwd.data.charptr.charptr_val != NULL) {
			
			r->headers_in.passwd.len = rpc_request->headers_in.passwd.len;
			r->headers_in.passwd.data = (u_char *)malloc(r->headers_in.passwd.len + 1);
			if (r->headers_in.passwd.data == NULL) {
				perror("malloc passwd data");
				if (r->headers_in.user.data) free(r->headers_in.user.data);
				free(r);
				result.ret0 = -1;
				return &result;
			}
			memcpy(r->headers_in.passwd.data,
				   rpc_request->headers_in.passwd.data.charptr.charptr_val,
				   r->headers_in.passwd.len);
			r->headers_in.passwd.data[r->headers_in.passwd.len] = '\0';
			
			printf("Password: %.*s\n", (int)r->headers_in.passwd.len, (char *)r->headers_in.passwd.data);
		} else {
			r->headers_in.passwd.len = 0;
			r->headers_in.passwd.data = NULL;
		}
		
		// 3. 拷贝 authorization 头部字段
		if (rpc_request->headers_in.authorization.ngx_table_elt_t_rpc_ptr_len > 0 &&
			rpc_request->headers_in.authorization.ngx_table_elt_t_rpc_ptr_val != NULL) {
			
			ngx_table_elt_t_rpc *auth_rpc = rpc_request->headers_in.authorization.ngx_table_elt_t_rpc_ptr_val;
			
			// 分配本地 authorization 结构体
			r->headers_in.authorization = (ngx_table_elt_t *)malloc(sizeof(ngx_table_elt_t));
			if (r->headers_in.authorization == NULL) {
				perror("malloc authorization");
				if (r->headers_in.passwd.data) free(r->headers_in.passwd.data);
				if (r->headers_in.user.data) free(r->headers_in.user.data);
				free(r);
				result.ret0 = -1;
				return &result;
			}
			memset(r->headers_in.authorization, 0, sizeof(ngx_table_elt_t));
			
			// 拷贝 hash 值
			r->headers_in.authorization->hash = auth_rpc->hash;
			
			// 深拷贝 key 字段
			if (auth_rpc->key.data.charptr.charptr_len > 0 &&
				auth_rpc->key.data.charptr.charptr_val != NULL) {
				
				r->headers_in.authorization->key.len = auth_rpc->key.len;
				r->headers_in.authorization->key.data = (u_char *)malloc(r->headers_in.authorization->key.len + 1);
				if (r->headers_in.authorization->key.data == NULL) {
					perror("malloc auth key");
					free(r->headers_in.authorization);
					if (r->headers_in.passwd.data) free(r->headers_in.passwd.data);
					if (r->headers_in.user.data) free(r->headers_in.user.data);
					free(r);
					result.ret0 = -1;
					return &result;
				}
				memcpy(r->headers_in.authorization->key.data,
					   auth_rpc->key.data.charptr.charptr_val,
					   r->headers_in.authorization->key.len);
				r->headers_in.authorization->key.data[r->headers_in.authorization->key.len] = '\0';
			}
			
			// 深拷贝 value 字段
			if (auth_rpc->value.data.charptr.charptr_len > 0 &&
				auth_rpc->value.data.charptr.charptr_val != NULL) {
				
				r->headers_in.authorization->value.len = auth_rpc->value.len;
				r->headers_in.authorization->value.data = (u_char *)malloc(r->headers_in.authorization->value.len + 1);
				if (r->headers_in.authorization->value.data == NULL) {
					perror("malloc auth value");
					if (r->headers_in.authorization->key.data) free(r->headers_in.authorization->key.data);
					free(r->headers_in.authorization);
					if (r->headers_in.passwd.data) free(r->headers_in.passwd.data);
					if (r->headers_in.user.data) free(r->headers_in.user.data);
					free(r);
					result.ret0 = -1;
					return &result;
				}
				memcpy(r->headers_in.authorization->value.data,
					   auth_rpc->value.data.charptr.charptr_val,
					   r->headers_in.authorization->value.len);
				r->headers_in.authorization->value.data[r->headers_in.authorization->value.len] = '\0';
				
				printf("Authorization: %.*s\n", 
					   (int)r->headers_in.authorization->value.len,
					   (char *)r->headers_in.authorization->value.data);
			}
			
			// 深拷贝 lowcase_key 字段
			if (auth_rpc->lowcase_key.charptr.charptr_len > 0 &&
				auth_rpc->lowcase_key.charptr.charptr_val != NULL) {
				
				r->headers_in.authorization->lowcase_key = (u_char *)malloc(auth_rpc->lowcase_key.charptr.charptr_len + 1);
				if (r->headers_in.authorization->lowcase_key == NULL) {
					perror("malloc lowcase_key");
					if (r->headers_in.authorization->value.data) free(r->headers_in.authorization->value.data);
					if (r->headers_in.authorization->key.data) free(r->headers_in.authorization->key.data);
					free(r->headers_in.authorization);
					if (r->headers_in.passwd.data) free(r->headers_in.passwd.data);
					if (r->headers_in.user.data) free(r->headers_in.user.data);
					free(r);
					result.ret0 = -1;
					return &result;
				}
				memcpy(r->headers_in.authorization->lowcase_key,
					   auth_rpc->lowcase_key.charptr.charptr_val,
					   auth_rpc->lowcase_key.charptr.charptr_len);
				r->headers_in.authorization->lowcase_key[auth_rpc->lowcase_key.charptr.charptr_len] = '\0';
			} else {
				r->headers_in.authorization->lowcase_key = NULL;
			} 
			
		} else {
			r->headers_in.authorization = NULL;
		}
		printf("Successfully unpacked request headers\n");


        // 解包内存池数据
		if (rpc_request->pool.ngx_pool_t_rpc_ptr_len > 0 && 
			rpc_request->pool.ngx_pool_t_rpc_ptr_val != NULL) {
			
			ngx_pool_t_rpc *pool_rpc = rpc_request->pool.ngx_pool_t_rpc_ptr_val;
			
			// 创建本地内存池
			r->pool = (ngx_pool_t *)malloc(sizeof(ngx_pool_t));
			if (r->pool != NULL) {
				memset(r->pool, 0, sizeof(ngx_pool_t));
				
				// 恢复池的基本信息
				r->pool->max = pool_rpc->max;
				r->pool->current = r->pool; // 指向自己
				r->pool->d.failed = pool_rpc->d.failed;
				
				// 恢复池数据
				if (pool_rpc->pool_data.pool_data_len > 0 && pool_rpc->pool_data.pool_data_val != NULL) {
					// 分配池数据空间
					size_t pool_data_size = pool_rpc->pool_data.pool_data_len;
					r->pool->d.last = (u_char *)malloc(pool_data_size);
					
					if (r->pool->d.last != NULL) {
						// 拷贝池数据
						memcpy(r->pool->d.last, pool_rpc->pool_data.pool_data_val, pool_data_size);
						r->pool->d.end = r->pool->d.last + pool_data_size;
						
						printf("Server: Pool data unpacked successfully\n");
						printf("Server: Pool max: %zu, failed: %lu, data size: %zu\n", 
							   r->pool->max, r->pool->d.failed, pool_data_size);
					} else {
						printf("Server: Failed to allocate pool data\n");
					}
				} else {
					// 没有池数据内容
					r->pool->d.last = NULL;
					r->pool->d.end = NULL;
					printf("Server: No pool data content to unpack\n");
				}
				
				// 初始化其他池字段
				r->pool->d.next = NULL;
				
			} else {
				printf("Server: Failed to allocate local pool\n");
			}
		} else {
			r->pool = NULL;
			printf("Server: No pool data received\n");
		}
    
    
    } else {
		printf("No valid request data received in arg2\n");
		r->signature = 0;
		r->loc_conf = NULL;
	}

ngx_int_t sensefun_ret;
sensefun_ret=ngx_http_auth_basic_handler_sense_1(ngx_http_auth_basic_handler_sense_1_ret,r);

result.ret0=sensefun_ret;
    return &result;
}

void *set_ngx_sys_errlist_1_svc(ngx_str_t_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
//     ngx_str_t *local_ngx_sys_errlist=NULL;
//     if(arg1.ngx_str_t_rpc_ptr_val!=NULL) {
//     local_ngx_sys_errlist=( ngx_str_t *)malloc(arg1.ngx_str_t_rpc_ptr_len*sizeof( ngx_str_t *));
//     if(local_ngx_sys_errlist==NULL) {
//     perror("malloc failed");
//     exit(1);
//     }
//     for(int i0=0;i0<arg1.ngx_str_t_rpc_ptr_len;i0++) {
//     local_ngx_sys_errlist[i0].len=arg1.ngx_str_t_rpc_ptr_val[i0].len;
//     if (arg1.ngx_str_t_rpc_ptr_val[i0].data_rpc.int_ptr_val == NULL) {local_ngx_sys_errlist[i0].data=NULL;}
//     else {local_ngx_sys_errlist[i0].data=(int *)malloc(arg1.ngx_str_t_rpc_ptr_val[i0].data_rpc.int_ptr_len * sizeof(int));
//     	if (local_ngx_sys_errlist[i0].data == NULL) {
//     		perror("malloc failed");
//     		exit(1);
//     	}
//     	for (int i = 0; i < arg1.ngx_str_t_rpc_ptr_val[i0].data_rpc.int_ptr_len; i++) {
//     		local_ngx_sys_errlist[i0].data[i] = arg1.ngx_str_t_rpc_ptr_val[i0].data_rpc.int_ptr_val[i];
//     	}
//     }
//     	}
//     }
//     memcpy(&ngx_sys_errlist, &local_ngx_sys_errlist, sizeof(ngx_str_t));
    return (void *)&result;
}

extern ngx_str_t  *ngx_sys_errlist;
ngx_sys_errlist_len_ret_t *get_ngx_sys_errlist_len_1_svc(struct svc_req *rqstp)
{
    static ngx_sys_errlist_len_ret_t result;
    result.value = ngx_sys_errlist->data;
    return &result;
}

void *set_ngx_sys_errlist_len_1_svc(long value, struct svc_req *rqstp)
{
    static int result = 1;
    ngx_sys_errlist->len = value;
    return (void *)&result;
}


ngx_sys_errlist_ret_t* get_ngx_sys_errlist_1_svc(struct svc_req *rqstp)
{
    static ngx_sys_errlist_ret_t result;
    // result.len = ngx_sys_errlist->len;
    // result.data = ngx_sys_errlist->data;
    return &result;
}

ngx_sys_errlist_data_ret_t *get_ngx_sys_errlist_data_1_svc(struct svc_req *rqstp)
{
    static ngx_sys_errlist_data_ret_t result;
    result.value = ngx_sys_errlist->data; // 警告：仅传递指针值
    return &result;
}

void *set_ngx_sys_errlist_data_1_svc(char_ptr  value, struct svc_req *rqstp)
{
    static int result = 1;
    // ngx_sys_errlist->data = value;
    return (void *)&result;
}

ngx_event_timer_rbtree_ret_t *get_ngx_event_timer_rbtree_1_svc(struct svc_req *rqstp)
{
    static ngx_event_timer_rbtree_ret_t result;
    return &result;
}

void *set_ngx_event_timer_rbtree_1_svc(ngx_rbtree_t_rpc value, struct svc_req *rqstp)
{
    static int result = 1;
    ngx_rbtree_t_rpc arg1;
    return (void *)&result;
}

// typedef u_char *(*ngx_sprintf_numFunc)(u_char *buf, u_char *last, uint64_t ui64, u_char zero, ngx_uint_t hexadecimal, ngx_uint_t width);
// typedef void *(*ngx_core_module_create_confFunc)(ngx_cycle_t *cycle);
// typedef char *(*ngx_conf_set_flag_slotFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_set_userFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_set_worker_processesFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_events_blockFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef void *(*ngx_event_core_create_confFunc)(ngx_cycle_t *cycle);
// typedef void *(*ngx_epoll_create_confFunc)(ngx_cycle_t *cycle);
// typedef char *(*ngx_event_connectionsFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_event_core_init_confFunc)(ngx_cycle_t *cycle, void *conf);
// typedef char *(*ngx_epoll_init_confFunc)(ngx_cycle_t *cycle, void *conf);
// typedef char *(*ngx_http_blockFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef void *(*ngx_http_core_create_main_confFunc)(ngx_conf_t *cf);
// typedef void *(*ngx_http_core_create_srv_confFunc)(ngx_conf_t *cf);
// typedef void *(*ngx_http_core_create_loc_confFunc)(ngx_conf_t *cf);
// typedef void *(*ngx_http_log_create_main_confFunc)(ngx_conf_t *cf);
// typedef void *(*ngx_http_log_create_loc_confFunc)(ngx_conf_t *cf);
// typedef void *(*ngx_http_upstream_create_main_confFunc)(ngx_conf_t *cf);
// typedef void *(*ngx_http_index_create_loc_confFunc)(ngx_conf_t *cf);
// typedef void *(*ngx_http_mirror_create_loc_confFunc)(ngx_conf_t *cf);
// typedef void *(*ngx_http_try_files_create_loc_confFunc)(ngx_conf_t *cf);
// typedef void *(*ngx_http_auth_basic_create_loc_confFunc)(ngx_conf_t *cf);
// typedef void *(*ngx_http_upstream_hash_create_confFunc)(ngx_conf_t *cf);
// typedef void *(*ngx_http_upstream_random_create_confFunc)(ngx_conf_t *cf);
// typedef void *(*ngx_http_upstream_keepalive_create_confFunc)(ngx_conf_t *cf);
// typedef void *(*ngx_http_headers_create_confFunc)(ngx_conf_t *cf);
// typedef void *(*ngx_http_copy_filter_create_confFunc)(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_core_preconfigurationFunc)(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_upstream_add_variablesFunc)(ngx_conf_t *cf);
// typedef char *(*ngx_conf_includeFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_http_core_typesFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_http_core_typeFunc)(ngx_conf_t *cf, ngx_command_t *dummy, void *conf);
// typedef char *(*ngx_conf_set_str_slotFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_http_core_keepaliveFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_http_core_serverFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *dummy);
// typedef char *(*ngx_http_core_listenFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_http_core_server_nameFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_http_set_complex_value_slotFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_http_auth_basic_user_fileFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_http_core_locationFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *dummy);
// typedef char *(*ngx_http_core_rootFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_http_index_set_indexFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_http_core_error_pageFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_http_core_init_main_confFunc)(ngx_conf_t *cf, void *conf);
// typedef char *(*ngx_http_core_merge_srv_confFunc)(ngx_conf_t *cf, void *parent, void *child);
// typedef char *(*ngx_http_core_merge_loc_confFunc)(ngx_conf_t *cf, void *parent, void *child);
// typedef char *(*ngx_http_log_merge_loc_confFunc)(ngx_conf_t *cf, void *parent, void *child);
// typedef char *(*ngx_http_upstream_init_main_confFunc)(ngx_conf_t *cf, void *conf);
// typedef char *(*ngx_http_index_merge_loc_confFunc)(ngx_conf_t *cf, void *parent, void *child);
// typedef char *(*ngx_http_mirror_merge_loc_confFunc)(ngx_conf_t *cf, void *parent, void *child);
// typedef char *(*ngx_http_auth_basic_merge_loc_confFunc)(ngx_conf_t *cf, void *parent, void *child);
// typedef char *(*ngx_http_headers_merge_confFunc)(ngx_conf_t *cf, void *parent, void *child);
// typedef char *(*ngx_http_copy_filter_merge_confFunc)(ngx_conf_t *cf, void *parent, void *child);
// typedef ngx_int_t (*ngx_http_cmp_locationsFunc)(const ngx_queue_t *one, const ngx_queue_t *two);
// typedef ngx_int_t (*ngx_http_core_postconfigurationFunc)(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_log_initFunc)(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_static_initFunc)(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_index_initFunc)(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_mirror_initFunc)(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_try_files_initFunc)(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_auth_basic_initFunc)(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_write_filter_initFunc)(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_header_filter_initFunc)(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_chunked_filter_initFunc)(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_range_header_filter_initFunc)(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_headers_filter_initFunc)(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_copy_filter_initFunc)(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_range_body_filter_initFunc)(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_not_modified_filter_initFunc)(ngx_conf_t *cf);
// typedef char *(*ngx_core_module_init_confFunc)(ngx_cycle_t *cycle, void *conf);
// typedef char *(*ngx_event_init_confFunc)(ngx_cycle_t *cycle, void *conf);
// typedef ngx_int_t (*ngx_event_module_initFunc)(ngx_cycle_t *cycle);
// typedef void (*ngx_worker_process_cycleFunc)(ngx_cycle_t *cycle, void *data);
// typedef ngx_int_t (*ngx_event_process_initFunc)(ngx_cycle_t *cycle);
// typedef ngx_int_t (*ngx_epoll_initFunc)(ngx_cycle_t *cycle, ngx_msec_t timer);
// typedef ngx_int_t (*ngx_epoll_add_eventFunc)(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);
// typedef ngx_int_t (*ngx_epoll_process_eventsFunc)(ngx_cycle_t *cycle, ngx_msec_t timer, ngx_uint_t flags);
// typedef void (*ngx_event_acceptFunc)(ngx_event_t *ev);
// typedef void (*ngx_http_init_connectionFunc)(ngx_connection_t *c);
// typedef void (*ngx_http_wait_request_handlerFunc)(ngx_event_t *rev);
// typedef ssize_t (*ngx_unix_recvFunc)(ngx_connection_t *c, u_char *buf, size_t size);
// typedef ngx_int_t (*ngx_http_process_hostFunc)(ngx_http_request_t *r, ngx_table_elt_t *h, ngx_uint_t offset);
// typedef ngx_int_t (*ngx_http_process_unique_header_lineFunc)(ngx_http_request_t *r, ngx_table_elt_t *h, ngx_uint_t offset);
// typedef ngx_int_t (*ngx_http_process_user_agentFunc)(ngx_http_request_t *r, ngx_table_elt_t *h, ngx_uint_t offset);
// typedef ngx_int_t (*ngx_http_core_find_config_phaseFunc)(ngx_http_request_t *r, ngx_http_phase_handler_t *ph);
// typedef ngx_int_t (*ngx_http_core_access_phaseFunc)(ngx_http_request_t *r, ngx_http_phase_handler_t *ph);
// typedef ngx_int_t (*ngx_http_auth_basic_handlerFunc)(ngx_http_request_t *r);
// typedef ngx_int_t (*ngx_http_core_post_access_phaseFunc)(ngx_http_request_t *r, ngx_http_phase_handler_t *ph);
// typedef ngx_int_t (*ngx_http_core_generic_phaseFunc)(ngx_http_request_t *r, ngx_http_phase_handler_t *ph);
// typedef ngx_int_t (*ngx_http_try_files_handlerFunc)(ngx_http_request_t *r);
// typedef ngx_int_t (*ngx_http_mirror_handlerFunc)(ngx_http_request_t *r);
// typedef ngx_int_t (*ngx_http_core_content_phaseFunc)(ngx_http_request_t *r, ngx_http_phase_handler_t *ph);
// typedef ngx_int_t (*ngx_http_index_handlerFunc)(ngx_http_request_t *r);
// typedef ngx_int_t (*ngx_http_static_handlerFunc)(ngx_http_request_t *r);
// typedef ngx_int_t (*ngx_http_not_modified_header_filterFunc)(ngx_http_request_t *r);
// typedef ngx_int_t (*ngx_http_headers_filterFunc)(ngx_http_request_t *r);
// typedef ngx_int_t (*ngx_http_range_header_filterFunc)(ngx_http_request_t *r);
// typedef ngx_int_t (*ngx_http_chunked_header_filterFunc)(ngx_http_request_t *r);
// typedef ngx_int_t (*ngx_http_header_filterFunc)(ngx_http_request_t *r);
// typedef ngx_int_t (*ngx_http_range_body_filterFunc)(ngx_http_request_t *r, ngx_chain_t *in);
// typedef ngx_int_t (*ngx_http_copy_filterFunc)(ngx_http_request_t *r, ngx_chain_t *in);
// typedef ngx_int_t (*ngx_http_trailers_filterFunc)(ngx_http_request_t *r, ngx_chain_t *in);
// typedef ngx_int_t (*ngx_http_chunked_body_filterFunc)(ngx_http_request_t *r, ngx_chain_t *in);
// typedef ngx_int_t (*ngx_http_write_filterFunc)(ngx_http_request_t *r, ngx_chain_t *in);
// typedef ngx_chain_t *(*ngx_linux_sendfile_chainFunc)(ngx_connection_t *c, ngx_chain_t *in, off_t limit);
// typedef ngx_int_t (*ngx_http_log_handlerFunc)(ngx_http_request_t *r);
// typedef size_t (*ngx_http_log_variable_getlenFunc)(ngx_http_request_t *r, uintptr_t data);
// typedef ngx_int_t (*ngx_http_variable_remote_addrFunc)(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data);
// typedef ngx_int_t (*ngx_http_variable_remote_userFunc)(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data);
// typedef ngx_int_t (*ngx_http_variable_request_lineFunc)(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data);
// typedef ngx_int_t (*ngx_http_variable_headerFunc)(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data);
// typedef u_char *(*ngx_http_log_variableFunc)(ngx_http_request_t *r, u_char *buf, ngx_http_log_op_t *op);
// typedef u_char *(*ngx_http_log_copy_shortFunc)(ngx_http_request_t *r, u_char *buf, ngx_http_log_op_t *op);
// typedef u_char *(*ngx_http_log_timeFunc)(ngx_http_request_t *r, u_char *buf, ngx_http_log_op_t *op);
// typedef u_char *(*ngx_http_log_statusFunc)(ngx_http_request_t *r, u_char *buf, ngx_http_log_op_t *op);
// typedef u_char *(*ngx_http_log_body_bytes_sentFunc)(ngx_http_request_t *r, u_char *buf, ngx_http_log_op_t *op);
// typedef void (*ngx_pool_cleanup_fileFunc)(void *data);
// typedef void (*ngx_http_keepalive_handlerFunc)(ngx_event_t *rev);
// typedef ngx_int_t (*ngx_epoll_del_connectionFunc)(ngx_connection_t *c, ngx_uint_t flags);
// typedef void (*ngx_signal_handlerFunc)(int signo, siginfo_t *siginfo, void *ucontext);
// typedef void (*ngx_conf_flush_filesFunc)(ngx_cycle_t *cycle);
// typedef void (*ngx_cleanup_environmentFunc)(void *data);
// typedef void (*ngx_resolver_cleanupFunc)(void *data);
// typedef enum {
//     SIG_NGX_SPRINTF_NUMFUNC,
//     SIG_NGX_CORE_MODULE_CREATE_CONFFUNC,
//     SIG_NGX_CONF_SET_FLAG_SLOTFUNC,
//     SIG_NGX_SET_USERFUNC,
//     SIG_NGX_SET_WORKER_PROCESSESFUNC,
//     SIG_NGX_EVENTS_BLOCKFUNC,
//     SIG_NGX_EVENT_CORE_CREATE_CONFFUNC,
//     SIG_NGX_EPOLL_CREATE_CONFFUNC,
//     SIG_NGX_EVENT_CONNECTIONSFUNC,
//     SIG_NGX_EVENT_CORE_INIT_CONFFUNC,
//     SIG_NGX_EPOLL_INIT_CONFFUNC,
//     SIG_NGX_HTTP_BLOCKFUNC,
//     SIG_NGX_HTTP_CORE_CREATE_MAIN_CONFFUNC,
//     SIG_NGX_HTTP_CORE_CREATE_SRV_CONFFUNC,
//     SIG_NGX_HTTP_CORE_CREATE_LOC_CONFFUNC,
//     SIG_NGX_HTTP_LOG_CREATE_MAIN_CONFFUNC,
//     SIG_NGX_HTTP_LOG_CREATE_LOC_CONFFUNC,
//     SIG_NGX_HTTP_UPSTREAM_CREATE_MAIN_CONFFUNC,
//     SIG_NGX_HTTP_INDEX_CREATE_LOC_CONFFUNC,
//     SIG_NGX_HTTP_MIRROR_CREATE_LOC_CONFFUNC,
//     SIG_NGX_HTTP_TRY_FILES_CREATE_LOC_CONFFUNC,
//     SIG_NGX_HTTP_AUTH_BASIC_CREATE_LOC_CONFFUNC,
//     SIG_NGX_HTTP_UPSTREAM_HASH_CREATE_CONFFUNC,
//     SIG_NGX_HTTP_UPSTREAM_RANDOM_CREATE_CONFFUNC,
//     SIG_NGX_HTTP_UPSTREAM_KEEPALIVE_CREATE_CONFFUNC,
//     SIG_NGX_HTTP_HEADERS_CREATE_CONFFUNC,
//     SIG_NGX_HTTP_COPY_FILTER_CREATE_CONFFUNC,
//     SIG_NGX_HTTP_CORE_PRECONFIGURATIONFUNC,
//     SIG_NGX_HTTP_UPSTREAM_ADD_VARIABLESFUNC,
//     SIG_NGX_CONF_INCLUDEFUNC,
//     SIG_NGX_HTTP_CORE_TYPESFUNC,
//     SIG_NGX_HTTP_CORE_TYPEFUNC,
//     SIG_NGX_CONF_SET_STR_SLOTFUNC,
//     SIG_NGX_HTTP_CORE_KEEPALIVEFUNC,
//     SIG_NGX_HTTP_CORE_SERVERFUNC,
//     SIG_NGX_HTTP_CORE_LISTENFUNC,
//     SIG_NGX_HTTP_CORE_SERVER_NAMEFUNC,
//     SIG_NGX_HTTP_SET_COMPLEX_VALUE_SLOTFUNC,
//     SIG_NGX_HTTP_AUTH_BASIC_USER_FILEFUNC,
//     SIG_NGX_HTTP_CORE_LOCATIONFUNC,
//     SIG_NGX_HTTP_CORE_ROOTFUNC,
//     SIG_NGX_HTTP_INDEX_SET_INDEXFUNC,
//     SIG_NGX_HTTP_CORE_ERROR_PAGEFUNC,
//     SIG_NGX_HTTP_CORE_INIT_MAIN_CONFFUNC,
//     SIG_NGX_HTTP_CORE_MERGE_SRV_CONFFUNC,
//     SIG_NGX_HTTP_CORE_MERGE_LOC_CONFFUNC,
//     SIG_NGX_HTTP_LOG_MERGE_LOC_CONFFUNC,
//     SIG_NGX_HTTP_UPSTREAM_INIT_MAIN_CONFFUNC,
//     SIG_NGX_HTTP_INDEX_MERGE_LOC_CONFFUNC,
//     SIG_NGX_HTTP_MIRROR_MERGE_LOC_CONFFUNC,
//     SIG_NGX_HTTP_AUTH_BASIC_MERGE_LOC_CONFFUNC,
//     SIG_NGX_HTTP_HEADERS_MERGE_CONFFUNC,
//     SIG_NGX_HTTP_COPY_FILTER_MERGE_CONFFUNC,
//     SIG_NGX_HTTP_CMP_LOCATIONSFUNC,
//     SIG_NGX_HTTP_CORE_POSTCONFIGURATIONFUNC,
//     SIG_NGX_HTTP_LOG_INITFUNC,
//     SIG_NGX_HTTP_STATIC_INITFUNC,
//     SIG_NGX_HTTP_INDEX_INITFUNC,
//     SIG_NGX_HTTP_MIRROR_INITFUNC,
//     SIG_NGX_HTTP_TRY_FILES_INITFUNC,
//     SIG_NGX_HTTP_AUTH_BASIC_INITFUNC,
//     SIG_NGX_HTTP_WRITE_FILTER_INITFUNC,
//     SIG_NGX_HTTP_HEADER_FILTER_INITFUNC,
//     SIG_NGX_HTTP_CHUNKED_FILTER_INITFUNC,
//     SIG_NGX_HTTP_RANGE_HEADER_FILTER_INITFUNC,
//     SIG_NGX_HTTP_HEADERS_FILTER_INITFUNC,
//     SIG_NGX_HTTP_COPY_FILTER_INITFUNC,
//     SIG_NGX_HTTP_RANGE_BODY_FILTER_INITFUNC,
//     SIG_NGX_HTTP_NOT_MODIFIED_FILTER_INITFUNC,
//     SIG_NGX_CORE_MODULE_INIT_CONFFUNC,
//     SIG_NGX_EVENT_INIT_CONFFUNC,
//     SIG_NGX_EVENT_MODULE_INITFUNC,
//     SIG_NGX_WORKER_PROCESS_CYCLEFUNC,
//     SIG_NGX_EVENT_PROCESS_INITFUNC,
//     SIG_NGX_EPOLL_INITFUNC,
//     SIG_NGX_EPOLL_ADD_EVENTFUNC,
//     SIG_NGX_EPOLL_PROCESS_EVENTSFUNC,
//     SIG_NGX_EVENT_ACCEPTFUNC,
//     SIG_NGX_HTTP_INIT_CONNECTIONFUNC,
//     SIG_NGX_HTTP_WAIT_REQUEST_HANDLERFUNC,
//     SIG_NGX_UNIX_RECVFUNC,
//     SIG_NGX_HTTP_PROCESS_HOSTFUNC,
//     SIG_NGX_HTTP_PROCESS_UNIQUE_HEADER_LINEFUNC,
//     SIG_NGX_HTTP_PROCESS_USER_AGENTFUNC,
//     SIG_NGX_HTTP_CORE_FIND_CONFIG_PHASEFUNC,
//     SIG_NGX_HTTP_CORE_ACCESS_PHASEFUNC,
//     SIG_NGX_HTTP_AUTH_BASIC_HANDLERFUNC,
//     SIG_NGX_HTTP_CORE_POST_ACCESS_PHASEFUNC,
//     SIG_NGX_HTTP_CORE_GENERIC_PHASEFUNC,
//     SIG_NGX_HTTP_TRY_FILES_HANDLERFUNC,
//     SIG_NGX_HTTP_MIRROR_HANDLERFUNC,
//     SIG_NGX_HTTP_CORE_CONTENT_PHASEFUNC,
//     SIG_NGX_HTTP_INDEX_HANDLERFUNC,
//     SIG_NGX_HTTP_STATIC_HANDLERFUNC,
//     SIG_NGX_HTTP_NOT_MODIFIED_HEADER_FILTERFUNC,
//     SIG_NGX_HTTP_HEADERS_FILTERFUNC,
//     SIG_NGX_HTTP_RANGE_HEADER_FILTERFUNC,
//     SIG_NGX_HTTP_CHUNKED_HEADER_FILTERFUNC,
//     SIG_NGX_HTTP_HEADER_FILTERFUNC,
//     SIG_NGX_HTTP_RANGE_BODY_FILTERFUNC,
//     SIG_NGX_HTTP_COPY_FILTERFUNC,
//     SIG_NGX_HTTP_TRAILERS_FILTERFUNC,
//     SIG_NGX_HTTP_CHUNKED_BODY_FILTERFUNC,
//     SIG_NGX_HTTP_WRITE_FILTERFUNC,
//     SIG_NGX_LINUX_SENDFILE_CHAINFUNC,
//     SIG_NGX_HTTP_LOG_HANDLERFUNC,
//     SIG_NGX_HTTP_LOG_VARIABLE_GETLENFUNC,
//     SIG_NGX_HTTP_VARIABLE_REMOTE_ADDRFUNC,
//     SIG_NGX_HTTP_VARIABLE_REMOTE_USERFUNC,
//     SIG_NGX_HTTP_VARIABLE_REQUEST_LINEFUNC,
//     SIG_NGX_HTTP_VARIABLE_HEADERFUNC,
//     SIG_NGX_HTTP_LOG_VARIABLEFUNC,
//     SIG_NGX_HTTP_LOG_COPY_SHORTFUNC,
//     SIG_NGX_HTTP_LOG_TIMEFUNC,
//     SIG_NGX_HTTP_LOG_STATUSFUNC,
//     SIG_NGX_HTTP_LOG_BODY_BYTES_SENTFUNC,
//     SIG_NGX_POOL_CLEANUP_FILEFUNC,
//     SIG_NGX_HTTP_KEEPALIVE_HANDLERFUNC,
//     SIG_NGX_EPOLL_DEL_CONNECTIONFUNC,
//     SIG_NGX_SIGNAL_HANDLERFUNC,
//     SIG_NGX_CONF_FLUSH_FILESFUNC,
//     SIG_NGX_CLEANUP_ENVIRONMENTFUNC,
//     SIG_NGX_RESOLVER_CLEANUPFUNC,
//     SIG_UNKNOWN
// } FunctionSignature;
// typedef struct {
//     const char *name;         // 函数名
//     void *func_ptr_void;      // 存储为 void* 的函数地址
//     FunctionSignature signature; // 函数签名标识符
// } FunctionMapEntry;
// FunctionMapEntry function_map[] = {
//     {"ngx_sprintf_num", (void*)ngx_sprintf_num, SIG_NGX_SPRINTF_NUMFUNC},
//     {"ngx_core_module_create_conf", (void*)ngx_core_module_create_conf, SIG_NGX_CORE_MODULE_CREATE_CONFFUNC},
//     {"ngx_conf_set_flag_slot", (void*)ngx_conf_set_flag_slot, SIG_NGX_CONF_SET_FLAG_SLOTFUNC},
//     {"ngx_set_user", (void*)ngx_set_user, SIG_NGX_SET_USERFUNC},
//     {"ngx_set_worker_processes", (void*)ngx_set_worker_processes, SIG_NGX_SET_WORKER_PROCESSESFUNC},
//     {"ngx_events_block", (void*)ngx_events_block, SIG_NGX_EVENTS_BLOCKFUNC},
//     {"ngx_event_core_create_conf", (void*)ngx_event_core_create_conf, SIG_NGX_EVENT_CORE_CREATE_CONFFUNC},
//     {"ngx_epoll_create_conf", (void*)ngx_epoll_create_conf, SIG_NGX_EPOLL_CREATE_CONFFUNC},
//     {"ngx_event_connections", (void*)ngx_event_connections, SIG_NGX_EVENT_CONNECTIONSFUNC},
//     {"ngx_event_core_init_conf", (void*)ngx_event_core_init_conf, SIG_NGX_EVENT_CORE_INIT_CONFFUNC},
//     {"ngx_epoll_init_conf", (void*)ngx_epoll_init_conf, SIG_NGX_EPOLL_INIT_CONFFUNC},
//     {"ngx_http_block", (void*)ngx_http_block, SIG_NGX_HTTP_BLOCKFUNC},
//     {"ngx_http_core_create_main_conf", (void*)ngx_http_core_create_main_conf, SIG_NGX_HTTP_CORE_CREATE_MAIN_CONFFUNC},
//     {"ngx_http_core_create_srv_conf", (void*)ngx_http_core_create_srv_conf, SIG_NGX_HTTP_CORE_CREATE_SRV_CONFFUNC},
//     {"ngx_http_core_create_loc_conf", (void*)ngx_http_core_create_loc_conf, SIG_NGX_HTTP_CORE_CREATE_LOC_CONFFUNC},
//     {"ngx_http_log_create_main_conf", (void*)ngx_http_log_create_main_conf, SIG_NGX_HTTP_LOG_CREATE_MAIN_CONFFUNC},
//     {"ngx_http_log_create_loc_conf", (void*)ngx_http_log_create_loc_conf, SIG_NGX_HTTP_LOG_CREATE_LOC_CONFFUNC},
//     {"ngx_http_upstream_create_main_conf", (void*)ngx_http_upstream_create_main_conf, SIG_NGX_HTTP_UPSTREAM_CREATE_MAIN_CONFFUNC},
//     {"ngx_http_index_create_loc_conf", (void*)ngx_http_index_create_loc_conf, SIG_NGX_HTTP_INDEX_CREATE_LOC_CONFFUNC},
//     {"ngx_http_mirror_create_loc_conf", (void*)ngx_http_mirror_create_loc_conf, SIG_NGX_HTTP_MIRROR_CREATE_LOC_CONFFUNC},
//     {"ngx_http_try_files_create_loc_conf", (void*)ngx_http_try_files_create_loc_conf, SIG_NGX_HTTP_TRY_FILES_CREATE_LOC_CONFFUNC},
//     {"ngx_http_auth_basic_create_loc_conf", (void*)ngx_http_auth_basic_create_loc_conf, SIG_NGX_HTTP_AUTH_BASIC_CREATE_LOC_CONFFUNC},
//     {"ngx_http_upstream_hash_create_conf", (void*)ngx_http_upstream_hash_create_conf, SIG_NGX_HTTP_UPSTREAM_HASH_CREATE_CONFFUNC},
//     {"ngx_http_upstream_random_create_conf", (void*)ngx_http_upstream_random_create_conf, SIG_NGX_HTTP_UPSTREAM_RANDOM_CREATE_CONFFUNC},
//     {"ngx_http_upstream_keepalive_create_conf", (void*)ngx_http_upstream_keepalive_create_conf, SIG_NGX_HTTP_UPSTREAM_KEEPALIVE_CREATE_CONFFUNC},
//     {"ngx_http_headers_create_conf", (void*)ngx_http_headers_create_conf, SIG_NGX_HTTP_HEADERS_CREATE_CONFFUNC},
//     {"ngx_http_copy_filter_create_conf", (void*)ngx_http_copy_filter_create_conf, SIG_NGX_HTTP_COPY_FILTER_CREATE_CONFFUNC},
//     {"ngx_http_core_preconfiguration", (void*)ngx_http_core_preconfiguration, SIG_NGX_HTTP_CORE_PRECONFIGURATIONFUNC},
//     {"ngx_http_upstream_add_variables", (void*)ngx_http_upstream_add_variables, SIG_NGX_HTTP_UPSTREAM_ADD_VARIABLESFUNC},
//     {"ngx_conf_include", (void*)ngx_conf_include, SIG_NGX_CONF_INCLUDEFUNC},
//     {"ngx_http_core_types", (void*)ngx_http_core_types, SIG_NGX_HTTP_CORE_TYPESFUNC},
//     {"ngx_http_core_type", (void*)ngx_http_core_type, SIG_NGX_HTTP_CORE_TYPEFUNC},
//     {"ngx_conf_set_str_slot", (void*)ngx_conf_set_str_slot, SIG_NGX_CONF_SET_STR_SLOTFUNC},
//     {"ngx_http_core_keepalive", (void*)ngx_http_core_keepalive, SIG_NGX_HTTP_CORE_KEEPALIVEFUNC},
//     {"ngx_http_core_server", (void*)ngx_http_core_server, SIG_NGX_HTTP_CORE_SERVERFUNC},
//     {"ngx_http_core_listen", (void*)ngx_http_core_listen, SIG_NGX_HTTP_CORE_LISTENFUNC},
//     {"ngx_http_core_server_name", (void*)ngx_http_core_server_name, SIG_NGX_HTTP_CORE_SERVER_NAMEFUNC},
//     {"ngx_http_set_complex_value_slot", (void*)ngx_http_set_complex_value_slot, SIG_NGX_HTTP_SET_COMPLEX_VALUE_SLOTFUNC},
//     {"ngx_http_auth_basic_user_file", (void*)ngx_http_auth_basic_user_file, SIG_NGX_HTTP_AUTH_BASIC_USER_FILEFUNC},
//     {"ngx_http_core_location", (void*)ngx_http_core_location, SIG_NGX_HTTP_CORE_LOCATIONFUNC},
//     {"ngx_http_core_root", (void*)ngx_http_core_root, SIG_NGX_HTTP_CORE_ROOTFUNC},
//     {"ngx_http_index_set_index", (void*)ngx_http_index_set_index, SIG_NGX_HTTP_INDEX_SET_INDEXFUNC},
//     {"ngx_http_core_error_page", (void*)ngx_http_core_error_page, SIG_NGX_HTTP_CORE_ERROR_PAGEFUNC},
//     {"ngx_http_core_init_main_conf", (void*)ngx_http_core_init_main_conf, SIG_NGX_HTTP_CORE_INIT_MAIN_CONFFUNC},
//     {"ngx_http_core_merge_srv_conf", (void*)ngx_http_core_merge_srv_conf, SIG_NGX_HTTP_CORE_MERGE_SRV_CONFFUNC},
//     {"ngx_http_core_merge_loc_conf", (void*)ngx_http_core_merge_loc_conf, SIG_NGX_HTTP_CORE_MERGE_LOC_CONFFUNC},
//     {"ngx_http_log_merge_loc_conf", (void*)ngx_http_log_merge_loc_conf, SIG_NGX_HTTP_LOG_MERGE_LOC_CONFFUNC},
//     {"ngx_http_upstream_init_main_conf", (void*)ngx_http_upstream_init_main_conf, SIG_NGX_HTTP_UPSTREAM_INIT_MAIN_CONFFUNC},
//     {"ngx_http_index_merge_loc_conf", (void*)ngx_http_index_merge_loc_conf, SIG_NGX_HTTP_INDEX_MERGE_LOC_CONFFUNC},
//     {"ngx_http_mirror_merge_loc_conf", (void*)ngx_http_mirror_merge_loc_conf, SIG_NGX_HTTP_MIRROR_MERGE_LOC_CONFFUNC},
//     {"ngx_http_auth_basic_merge_loc_conf", (void*)ngx_http_auth_basic_merge_loc_conf, SIG_NGX_HTTP_AUTH_BASIC_MERGE_LOC_CONFFUNC},
//     {"ngx_http_headers_merge_conf", (void*)ngx_http_headers_merge_conf, SIG_NGX_HTTP_HEADERS_MERGE_CONFFUNC},
//     {"ngx_http_copy_filter_merge_conf", (void*)ngx_http_copy_filter_merge_conf, SIG_NGX_HTTP_COPY_FILTER_MERGE_CONFFUNC},
//     {"ngx_http_cmp_locations", (void*)ngx_http_cmp_locations, SIG_NGX_HTTP_CMP_LOCATIONSFUNC},
//     {"ngx_http_core_postconfiguration", (void*)ngx_http_core_postconfiguration, SIG_NGX_HTTP_CORE_POSTCONFIGURATIONFUNC},
//     {"ngx_http_log_init", (void*)ngx_http_log_init, SIG_NGX_HTTP_LOG_INITFUNC},
//     {"ngx_http_static_init", (void*)ngx_http_static_init, SIG_NGX_HTTP_STATIC_INITFUNC},
//     {"ngx_http_index_init", (void*)ngx_http_index_init, SIG_NGX_HTTP_INDEX_INITFUNC},
//     {"ngx_http_mirror_init", (void*)ngx_http_mirror_init, SIG_NGX_HTTP_MIRROR_INITFUNC},
//     {"ngx_http_try_files_init", (void*)ngx_http_try_files_init, SIG_NGX_HTTP_TRY_FILES_INITFUNC},
//     {"ngx_http_auth_basic_init", (void*)ngx_http_auth_basic_init, SIG_NGX_HTTP_AUTH_BASIC_INITFUNC},
//     {"ngx_http_write_filter_init", (void*)ngx_http_write_filter_init, SIG_NGX_HTTP_WRITE_FILTER_INITFUNC},
//     {"ngx_http_header_filter_init", (void*)ngx_http_header_filter_init, SIG_NGX_HTTP_HEADER_FILTER_INITFUNC},
//     {"ngx_http_chunked_filter_init", (void*)ngx_http_chunked_filter_init, SIG_NGX_HTTP_CHUNKED_FILTER_INITFUNC},
//     {"ngx_http_range_header_filter_init", (void*)ngx_http_range_header_filter_init, SIG_NGX_HTTP_RANGE_HEADER_FILTER_INITFUNC},
//     {"ngx_http_headers_filter_init", (void*)ngx_http_headers_filter_init, SIG_NGX_HTTP_HEADERS_FILTER_INITFUNC},
//     {"ngx_http_copy_filter_init", (void*)ngx_http_copy_filter_init, SIG_NGX_HTTP_COPY_FILTER_INITFUNC},
//     {"ngx_http_range_body_filter_init", (void*)ngx_http_range_body_filter_init, SIG_NGX_HTTP_RANGE_BODY_FILTER_INITFUNC},
//     {"ngx_http_not_modified_filter_init", (void*)ngx_http_not_modified_filter_init, SIG_NGX_HTTP_NOT_MODIFIED_FILTER_INITFUNC},
//     {"ngx_core_module_init_conf", (void*)ngx_core_module_init_conf, SIG_NGX_CORE_MODULE_INIT_CONFFUNC},
//     {"ngx_event_init_conf", (void*)ngx_event_init_conf, SIG_NGX_EVENT_INIT_CONFFUNC},
//     {"ngx_event_module_init", (void*)ngx_event_module_init, SIG_NGX_EVENT_MODULE_INITFUNC},
//     {"ngx_worker_process_cycle", (void*)ngx_worker_process_cycle, SIG_NGX_WORKER_PROCESS_CYCLEFUNC},
//     {"ngx_event_process_init", (void*)ngx_event_process_init, SIG_NGX_EVENT_PROCESS_INITFUNC},
//     {"ngx_epoll_init", (void*)ngx_epoll_init, SIG_NGX_EPOLL_INITFUNC},
//     {"ngx_epoll_add_event", (void*)ngx_epoll_add_event, SIG_NGX_EPOLL_ADD_EVENTFUNC},
//     {"ngx_epoll_process_events", (void*)ngx_epoll_process_events, SIG_NGX_EPOLL_PROCESS_EVENTSFUNC},
//     {"ngx_event_accept", (void*)ngx_event_accept, SIG_NGX_EVENT_ACCEPTFUNC},
//     {"ngx_http_init_connection", (void*)ngx_http_init_connection, SIG_NGX_HTTP_INIT_CONNECTIONFUNC},
//     {"ngx_http_wait_request_handler", (void*)ngx_http_wait_request_handler, SIG_NGX_HTTP_WAIT_REQUEST_HANDLERFUNC},
//     {"ngx_unix_recv", (void*)ngx_unix_recv, SIG_NGX_UNIX_RECVFUNC},
//     {"ngx_http_process_host", (void*)ngx_http_process_host, SIG_NGX_HTTP_PROCESS_HOSTFUNC},
//     {"ngx_http_process_unique_header_line", (void*)ngx_http_process_unique_header_line, SIG_NGX_HTTP_PROCESS_UNIQUE_HEADER_LINEFUNC},
//     {"ngx_http_process_user_agent", (void*)ngx_http_process_user_agent, SIG_NGX_HTTP_PROCESS_USER_AGENTFUNC},
//     {"ngx_http_core_find_config_phase", (void*)ngx_http_core_find_config_phase, SIG_NGX_HTTP_CORE_FIND_CONFIG_PHASEFUNC},
//     {"ngx_http_core_access_phase", (void*)ngx_http_core_access_phase, SIG_NGX_HTTP_CORE_ACCESS_PHASEFUNC},
//     {"ngx_http_auth_basic_handler", (void*)ngx_http_auth_basic_handler, SIG_NGX_HTTP_AUTH_BASIC_HANDLERFUNC},
//     {"ngx_http_core_post_access_phase", (void*)ngx_http_core_post_access_phase, SIG_NGX_HTTP_CORE_POST_ACCESS_PHASEFUNC},
//     {"ngx_http_core_generic_phase", (void*)ngx_http_core_generic_phase, SIG_NGX_HTTP_CORE_GENERIC_PHASEFUNC},
//     {"ngx_http_try_files_handler", (void*)ngx_http_try_files_handler, SIG_NGX_HTTP_TRY_FILES_HANDLERFUNC},
//     {"ngx_http_mirror_handler", (void*)ngx_http_mirror_handler, SIG_NGX_HTTP_MIRROR_HANDLERFUNC},
//     {"ngx_http_core_content_phase", (void*)ngx_http_core_content_phase, SIG_NGX_HTTP_CORE_CONTENT_PHASEFUNC},
//     {"ngx_http_index_handler", (void*)ngx_http_index_handler, SIG_NGX_HTTP_INDEX_HANDLERFUNC},
//     {"ngx_http_static_handler", (void*)ngx_http_static_handler, SIG_NGX_HTTP_STATIC_HANDLERFUNC},
//     {"ngx_http_not_modified_header_filter", (void*)ngx_http_not_modified_header_filter, SIG_NGX_HTTP_NOT_MODIFIED_HEADER_FILTERFUNC},
//     {"ngx_http_headers_filter", (void*)ngx_http_headers_filter, SIG_NGX_HTTP_HEADERS_FILTERFUNC},
//     {"ngx_http_range_header_filter", (void*)ngx_http_range_header_filter, SIG_NGX_HTTP_RANGE_HEADER_FILTERFUNC},
//     {"ngx_http_chunked_header_filter", (void*)ngx_http_chunked_header_filter, SIG_NGX_HTTP_CHUNKED_HEADER_FILTERFUNC},
//     {"ngx_http_header_filter", (void*)ngx_http_header_filter, SIG_NGX_HTTP_HEADER_FILTERFUNC},
//     {"ngx_http_range_body_filter", (void*)ngx_http_range_body_filter, SIG_NGX_HTTP_RANGE_BODY_FILTERFUNC},
//     {"ngx_http_copy_filter", (void*)ngx_http_copy_filter, SIG_NGX_HTTP_COPY_FILTERFUNC},
//     {"ngx_http_trailers_filter", (void*)ngx_http_trailers_filter, SIG_NGX_HTTP_TRAILERS_FILTERFUNC},
//     {"ngx_http_chunked_body_filter", (void*)ngx_http_chunked_body_filter, SIG_NGX_HTTP_CHUNKED_BODY_FILTERFUNC},
//     {"ngx_http_write_filter", (void*)ngx_http_write_filter, SIG_NGX_HTTP_WRITE_FILTERFUNC},
//     {"ngx_linux_sendfile_chain", (void*)ngx_linux_sendfile_chain, SIG_NGX_LINUX_SENDFILE_CHAINFUNC},
//     {"ngx_http_log_handler", (void*)ngx_http_log_handler, SIG_NGX_HTTP_LOG_HANDLERFUNC},
//     {"ngx_http_log_variable_getlen", (void*)ngx_http_log_variable_getlen, SIG_NGX_HTTP_LOG_VARIABLE_GETLENFUNC},
//     {"ngx_http_variable_remote_addr", (void*)ngx_http_variable_remote_addr, SIG_NGX_HTTP_VARIABLE_REMOTE_ADDRFUNC},
//     {"ngx_http_variable_remote_user", (void*)ngx_http_variable_remote_user, SIG_NGX_HTTP_VARIABLE_REMOTE_USERFUNC},
//     {"ngx_http_variable_request_line", (void*)ngx_http_variable_request_line, SIG_NGX_HTTP_VARIABLE_REQUEST_LINEFUNC},
//     {"ngx_http_variable_header", (void*)ngx_http_variable_header, SIG_NGX_HTTP_VARIABLE_HEADERFUNC},
//     {"ngx_http_log_variable", (void*)ngx_http_log_variable, SIG_NGX_HTTP_LOG_VARIABLEFUNC},
//     {"ngx_http_log_copy_short", (void*)ngx_http_log_copy_short, SIG_NGX_HTTP_LOG_COPY_SHORTFUNC},
//     {"ngx_http_log_time", (void*)ngx_http_log_time, SIG_NGX_HTTP_LOG_TIMEFUNC},
//     {"ngx_http_log_status", (void*)ngx_http_log_status, SIG_NGX_HTTP_LOG_STATUSFUNC},
//     {"ngx_http_log_body_bytes_sent", (void*)ngx_http_log_body_bytes_sent, SIG_NGX_HTTP_LOG_BODY_BYTES_SENTFUNC},
//     {"ngx_pool_cleanup_file", (void*)ngx_pool_cleanup_file, SIG_NGX_POOL_CLEANUP_FILEFUNC},
//     {"ngx_http_keepalive_handler", (void*)ngx_http_keepalive_handler, SIG_NGX_HTTP_KEEPALIVE_HANDLERFUNC},
//     {"ngx_epoll_del_connection", (void*)ngx_epoll_del_connection, SIG_NGX_EPOLL_DEL_CONNECTIONFUNC},
//     {"ngx_signal_handler", (void*)ngx_signal_handler, SIG_NGX_SIGNAL_HANDLERFUNC},
//     {"ngx_conf_flush_files", (void*)ngx_conf_flush_files, SIG_NGX_CONF_FLUSH_FILESFUNC},
//     {"ngx_cleanup_environment", (void*)ngx_cleanup_environment, SIG_NGX_CLEANUP_ENVIRONMENTFUNC},
//     {"ngx_resolver_cleanup", (void*)ngx_resolver_cleanup, SIG_NGX_RESOLVER_CLEANUPFUNC},
//     {NULL, NULL, SIG_UNKNOWN} // 哨兵值
// };
// // --- 查找函数名：根据函数名查找函数条目 ---
// void* find_function_entry_by_name(const char* func_name) {
//     for (int i = 0; function_map[i].name != NULL; i++) {
//         if (strcmp(function_map[i].name, func_name) == 0) {
//             return &function_map[i].func_ptr_void;
//         }
//     }
//     return NULL; // 未找到
// }
// // --- 新增函数：根据函数地址查找函数名 ---
// const char* find_function_name_by_address(void* func_address) {
//     for (int i = 0; function_map[i].name != NULL; i++) {
//         // 直接比较 void* 指针
//         if (function_map[i].func_ptr_void == func_address) {
//             return function_map[i].name;
//         }
//     }
//     return NULL; // 未找到对应的函数名
// }
char *get_filename_from_fp(FILE *fp) {
    if (!fp) {                                                                 
        fprintf(stderr, "Error: FILE pointer is NULL\n");
        return NULL;
    }
    // 获取文件描述符
    int fd = fileno(fp);
    if (fd == -1) {
        fprintf(stderr, "Error: fileno failed");
        return NULL;
    }
    // 构造 /proc/self/fd/<fd> 路径
    char fd_path[64];   
    snprintf(fd_path, sizeof(fd_path), "/proc/self/fd/%d", fd);
    // 分配内存存储文件名
    char *filename = malloc(PATH_MAX);
    if (!filename) {
        fprintf(stderr, "Error: malloc failed");
        return NULL;
    }
    // 读取符号链接目标（文件名）
    ssize_t len = readlink(fd_path, filename, PATH_MAX - 1);
    if (len == -1) {
        fprintf(stderr, "Error: readlink failed for %s", fd_path);
        free(filename);
        return NULL;
    }
    filename[len] = '\0'; // 确保字符串终止
    return filename;
}