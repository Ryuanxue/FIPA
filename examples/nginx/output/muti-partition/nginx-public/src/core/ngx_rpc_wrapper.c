
#include "ngx_rpc_wrapper.h"
extern CLIENT *clnt;
extern ngx_module_t  ngx_http_auth_basic_module;
typedef struct {
    ngx_http_complex_value_t  *realm;
    ngx_http_complex_value_t   user_file;
} ngx_http_auth_basic_loc_conf_t;

void ngx_http_handler_wrapper(ngx_http_request_t *r)
{

// const char *htpasswd_path = "/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx/output/muti-partition/conf/..//auth/.htpasswd";
// 		memcpy(ablcf->user_file.value.data, htpasswd_path, 106);
// 		SYNC_LOC_CONF(r, clcf, 8, ablcf);
		
// 		memcpy(clcf->root.data, default_root, clcf->root.len);
// 		r->loc_conf[0] = clcf;
// 		const char *default_root = "/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx/output/muti-partition/nginx-mid/html";
		
// 		const char *html_root = "/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx/output/muti-partition/html";
// 		strcpy((char *)inclusive_clcf->root.data, html_root);
// 		clcf->static_locations->inclusive = inclusive_clcf;
		
// 		const char *config_path = "/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx/output/muti-partition/conf/nginx_test.conf";
// 		strcpy((char *)cscf->file_name, config_path);
// 		r->srv_conf[0] = cscf;

//请输出r->loc_conf[0]->root.data, r->loc_conf[0]->static_locations->inclusive->root.data
//r->loc_conf[8].user_file.value.data,r->srv_conf[0]->file_name,用printf

// 输出 r->loc_conf[0]->root.data
ngx_http_core_loc_conf_t *clcf = (ngx_http_core_loc_conf_t *)r->loc_conf[ngx_http_core_module.ctx_index];
if (clcf && clcf->root.data) {
    printf("=== r->loc_conf[0]->root.data = %s ===\n", clcf->root.data);
} else {
    printf("=== r->loc_conf[0]->root.data is NULL ===\n");
}

// 输出 r->loc_conf[0]->static_locations->inclusive->root.data
if (clcf && clcf->static_locations && clcf->static_locations->inclusive) {
    printf("=== r->loc_conf[0]->static_locations->inclusive->root.data = %s ===\n", 
           clcf->static_locations->inclusive->root.data);
} else {
    printf("=== r->loc_conf[0]->static_locations->inclusive is NULL ===\n");
}

// 输出 r->loc_conf[8]->user_file.value.data
ngx_http_auth_basic_loc_conf_t *alcf_debug = (ngx_http_auth_basic_loc_conf_t *)r->loc_conf[ngx_http_auth_basic_module.ctx_index];
if (alcf_debug && alcf_debug->user_file.value.data) {
    printf("=== r->loc_conf[8]->user_file.value.data = %s ===\n", 
           alcf_debug->user_file.value.data);
} else {
    printf("=== r->loc_conf[8]->user_file.value.data is NULL ===\n");
}


// 输出 r->srv_conf[0]->file_name
ngx_http_core_srv_conf_t *cscf = (ngx_http_core_srv_conf_t *)r->srv_conf[ngx_http_core_module.ctx_index];
if (cscf && cscf->file_name) {
    printf("=== r->srv_conf[0]->file_name = %s ===\n", cscf->file_name);
} else {
    printf("=== r->srv_conf[0]->file_name is NULL ===\n");
}
fflush(stdout);




void *result_rpc;

printf("=== FUNCTION START: ngx_http_handler_wrapper called ===\n");
fflush(stdout);

ngx_http_request_t_rpc_ptr r_rpc_ptr;
if(r==NULL) {
r_rpc_ptr.ngx_http_request_t_rpc_ptr_len=0;

//

r_rpc_ptr.ngx_http_request_t_rpc_ptr_val=NULL;
} else {
r_rpc_ptr.ngx_http_request_t_rpc_ptr_len=1;
ngx_http_request_t_rpc test_request;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val=&test_request;
for(int i0=0;i0<r_rpc_ptr.ngx_http_request_t_rpc_ptr_len;i0++) {
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].signature=r[i0].signature;


//================r->loc_conf中第8个元素的拷贝========================
ngx_http_auth_basic_loc_conf_t *alcf;
alcf = r->loc_conf[ngx_http_auth_basic_module.ctx_index];
printf("Debug: alcf->realm = %p\n", alcf->realm);
// printf("Debug: alcf->user_file = %p\n", alcf->user_file);


memset(&test_request, 0, sizeof(test_request));
test_request.loc_conf.ngx_http_auth_basic_loc_conf_t_rpc_twoptr_len = 8;
// 分配 loc_conf 数组内存
	ngx_http_auth_basic_loc_conf_t_rpc_ptr *loc_conf_array = 
		(ngx_http_auth_basic_loc_conf_t_rpc_ptr*)malloc(8 * sizeof(ngx_http_auth_basic_loc_conf_t_rpc_ptr));
// 前7个数据均为空，最后一个数据为有效数据
	for (int i = 0; i < 7; i++) {
		loc_conf_array[i].ngx_http_auth_basic_loc_conf_t_rpc_ptr_len = 0;
		loc_conf_array[i].ngx_http_auth_basic_loc_conf_t_rpc_ptr_val = NULL;
	}

	// 深度拷贝到 RPC 结构体的第8个元素
	ngx_http_auth_basic_loc_conf_t_rpc auth_basic_conf;
	memset(&auth_basic_conf, 0, sizeof(auth_basic_conf));
	// 拷贝 realm 字段到 RPC 结构体
	auth_basic_conf.realm.ngx_http_complex_value_t_rpc_ptr_len = 1;
	ngx_http_complex_value_t_rpc *realm_rpc = 
		(ngx_http_complex_value_t_rpc *)malloc(sizeof(ngx_http_complex_value_t_rpc));
	if (realm_rpc == NULL) {
		perror("malloc realm_rpc");
		exit(1);
	}
	memset(realm_rpc, 0, sizeof(ngx_http_complex_value_t_rpc));
	realm_rpc->value.len = alcf->realm->value.len;
	realm_rpc->value.data.charptr.charptr_len = alcf->realm->value.len;
	realm_rpc->value.data.charptr.charptr_val = (char *)alcf->realm->value.data;
	auth_basic_conf.realm.ngx_http_complex_value_t_rpc_ptr_val = realm_rpc;

	// 拷贝 user_file 字段到 RPC 结构体
	auth_basic_conf.user_file.value.len = alcf->user_file.value.len;
	auth_basic_conf.user_file.value.data.charptr.charptr_len = alcf->user_file.value.len;
	auth_basic_conf.user_file.value.data.charptr.charptr_val = (char *)alcf->user_file.value.data;

	// 将有效配置分配给第8个元素
	loc_conf_array[7].ngx_http_auth_basic_loc_conf_t_rpc_ptr_len = 1;
	ngx_http_auth_basic_loc_conf_t_rpc *auth_conf_copy = 
		(ngx_http_auth_basic_loc_conf_t_rpc *)malloc(sizeof(ngx_http_auth_basic_loc_conf_t_rpc));
	if (auth_conf_copy == NULL) {
		perror("malloc auth_conf_copy");
		exit(1);
	}
	memcpy(auth_conf_copy, &auth_basic_conf, sizeof(ngx_http_auth_basic_loc_conf_t_rpc));
	loc_conf_array[7].ngx_http_auth_basic_loc_conf_t_rpc_ptr_val = auth_conf_copy;
	// 将数组指针赋值给 test_request
	test_request.loc_conf.ngx_http_auth_basic_loc_conf_t_rpc_twoptr_val = loc_conf_array;
//======================r->loc_conf处理结束=================


//======================r->main_conf中第0个元素的拷贝========================

ngx_http_core_main_conf_t *cmcf;
cmcf = r->main_conf[ngx_http_core_module.ctx_index];
printf("Debug: cmcf->ncaptures = %ld\n", cmcf->ncaptures);

test_request.main_conf.ngx_http_core_main_conf_t_rpc_twoptr_len = 1;
// 分配 main_conf 数组内存
ngx_http_core_main_conf_t_rpc_ptr *main_conf_array = 
	(ngx_http_core_main_conf_t_rpc_ptr*)malloc(1 * sizeof(ngx_http_core_main_conf_t_rpc_ptr));
if (main_conf_array == NULL) {
	perror("malloc main_conf_array");
	exit(1);
}

// 深度拷贝到 RPC 结构体的第0个元素
ngx_http_core_main_conf_t_rpc core_main_conf;
memset(&core_main_conf, 0, sizeof(core_main_conf));
core_main_conf.ncaptures = cmcf->ncaptures;

// 将配置分配给第0个元素
main_conf_array[0].ngx_http_core_main_conf_t_rpc_ptr_len = 1;
ngx_http_core_main_conf_t_rpc *core_main_conf_copy = 
	(ngx_http_core_main_conf_t_rpc *)malloc(sizeof(ngx_http_core_main_conf_t_rpc));
if (core_main_conf_copy == NULL) {
	perror("malloc core_main_conf_copy");
	free(main_conf_array);
	exit(1);
}
memcpy(core_main_conf_copy, &core_main_conf, sizeof(ngx_http_core_main_conf_t_rpc));
main_conf_array[0].ngx_http_core_main_conf_t_rpc_ptr_val = core_main_conf_copy;

// 将数组指针赋值给 test_request
test_request.main_conf.ngx_http_core_main_conf_t_rpc_twoptr_val = main_conf_array;

//======================r->main_conf处理结束=================




	//打包r->headers_in数据
	// 初始化 headers_in 结构体
// 设置 user 字段
test_request.headers_in.user.len = r->headers_in.user.len;
test_request.headers_in.user.data.charptr.charptr_len = r->headers_in.user.len;
test_request.headers_in.user.data.charptr.charptr_val = (char *)r->headers_in.user.data;

// 设置 passwd 字段
test_request.headers_in.passwd.len = r->headers_in.passwd.len;
test_request.headers_in.passwd.data.charptr.charptr_len = r->headers_in.passwd.len;
test_request.headers_in.passwd.data.charptr.charptr_val = (char *)r->headers_in.passwd.data;

// 设置 authorization 字段
test_request.headers_in.authorization.ngx_table_elt_t_rpc_ptr_len = 1;
ngx_table_elt_t_rpc *auth_rpc = (ngx_table_elt_t_rpc *)malloc(sizeof(ngx_table_elt_t_rpc));
if (auth_rpc == NULL) {
	perror("malloc auth_rpc");
	exit(1);
}
memset(auth_rpc, 0, sizeof(ngx_table_elt_t_rpc));

// 拷贝 authorization 头部到 RPC 结构体

// auth_rpc->hash = r->headers_in.authorization->hash;


auth_rpc->key.len = r->headers_in.authorization->key.len;
auth_rpc->key.data.charptr.charptr_len = r->headers_in.authorization->key.len;
auth_rpc->key.data.charptr.charptr_val = (char *)r->headers_in.authorization->key.data;

auth_rpc->value.len = r->headers_in.authorization->value.len;
auth_rpc->value.data.charptr.charptr_len = r->headers_in.authorization->value.len;
auth_rpc->value.data.charptr.charptr_val = (char *)r->headers_in.authorization->value.data;

// auth_rpc->lowcase_key.charptr.charptr_len = r->headers_in.authorization->key.len;
// auth_rpc->lowcase_key.charptr.charptr_val = (char *)r->headers_in.authorization->lowcase_key;

test_request.headers_in.authorization.ngx_table_elt_t_rpc_ptr_val = auth_rpc;


// 将内存池数据打包到 RPC 结构体
if (r->pool != NULL) {
	// 分配 ngx_pool_t_rpc 结构体
	ngx_pool_t_rpc *pool_rpc = (ngx_pool_t_rpc *)malloc(sizeof(ngx_pool_t_rpc));
	if (pool_rpc == NULL) {
		perror("malloc pool_rpc");
		exit(1);
	}
	memset(pool_rpc, 0, sizeof(ngx_pool_t_rpc));
	
	// 拷贝基本池信息
	pool_rpc->max = r->pool->max;
	pool_rpc->pool_id = 1;           // 当前池的ID
	pool_rpc->next_pool_id = 0;      // 没有下一个池
	pool_rpc->current_pool_id = 1;   // 当前池指向自己
// 处理池数据部分
	// 计算池数据大小
	size_t pool_data_size = r->pool->d.end - r->pool->d.last;
	if (pool_data_size > 0) {
		// 拷贝池数据内容到pool_data字段
		pool_rpc->pool_data.pool_data_len = pool_data_size;
		pool_rpc->pool_data.pool_data_val = (char *)malloc(pool_data_size);
		if (pool_rpc->pool_data.pool_data_val == NULL) {
			perror("malloc pool data content");
			free(pool_rpc);
			exit(1);
		}
		memcpy(pool_rpc->pool_data.pool_data_val, r->pool->d.last, pool_data_size);
		
		// 对于偏移量字段，我们不需要实际的偏移数据，只需要记录偏移值
		// 设置空的偏移量数据，避免XDR序列化NULL指针
		pool_rpc->d.last_offset.charptr.charptr_len = 0;
		pool_rpc->d.last_offset.charptr.charptr_val = NULL;
		pool_rpc->d.end_offset.charptr.charptr_len = 0;
		pool_rpc->d.end_offset.charptr.charptr_val = NULL;
	} else {
		pool_rpc->pool_data.pool_data_len = 0;
		pool_rpc->pool_data.pool_data_val = NULL;
		pool_rpc->d.last_offset.charptr.charptr_len = 0;
		pool_rpc->d.last_offset.charptr.charptr_val = NULL;
		pool_rpc->d.end_offset.charptr.charptr_len = 0;
		pool_rpc->d.end_offset.charptr.charptr_val = NULL;
	}
	
	pool_rpc->d.failed = r->pool->d.failed;
	
	// 设置请求的池指针
	test_request.pool.ngx_pool_t_rpc_ptr_len = 1;
	test_request.pool.ngx_pool_t_rpc_ptr_val = pool_rpc;

	printf("Pool data packed successfully\n");
	printf("Pool max size: %zu, failed: %lu, data size: %zu\n", 
		   r->pool->max, r->pool->d.failed, pool_data_size);
} else {
	// 没有池数据
	test_request.pool.ngx_pool_t_rpc_ptr_len = 0;
	test_request.pool.ngx_pool_t_rpc_ptr_val = NULL;
	printf("No pool data to pack\n");
}


//请对r->loc_conf进行处理，它是void**,最终指向的数据是ngx_http_auth_basic_loc_conf_t结构体
//假设r->loc_conf指向的ngx_http_auth_basic_loc_conf_t结构体有8个元素帮我依次打印每个值
printf("=== DEBUG: r[%d].loc_conf analysis ===\n", i0);
printf("r[%d].loc_conf = %p\n", i0, r[i0].loc_conf);

/**
 * 请配置
 * r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].config_path为r->srv_conf[0]->file_name
 * r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].html_root为r->loc_conf[0]->root.data
 * r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].passwd_path为r->loc_conf[8]->user_file.value.data
 */

// 配置 config_path: r->srv_conf[0]->file_name
ngx_http_core_srv_conf_t *cscf_config = (ngx_http_core_srv_conf_t *)r->srv_conf[ngx_http_core_module.ctx_index];
if (cscf_config && cscf_config->file_name) {
    r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].config_path.charptr.charptr_len = strlen((char *)cscf_config->file_name);
    r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].config_path.charptr.charptr_val = (char *)cscf_config->file_name;
    printf("Debug: config_path配置 - %s\n", cscf_config->file_name);
} else {
    r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].config_path.charptr.charptr_len = 0;
    r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].config_path.charptr.charptr_val = NULL;
    printf("Debug: config_path is NULL\n");
}

// 配置 html_root: r->loc_conf[0]->root.data
ngx_http_core_loc_conf_t *clcf_config = (ngx_http_core_loc_conf_t *)r->loc_conf[ngx_http_core_module.ctx_index];
if (clcf_config && clcf_config->root.data) {
    r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].html_root.charptr.charptr_len = clcf_config->root.len;
    r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].html_root.charptr.charptr_val = (char *)clcf_config->root.data;
    printf("Debug: html_root配置 - len=%zu, data=%s\n", clcf_config->root.len, clcf_config->root.data);
} else {
    r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].html_root.charptr.charptr_len = 0;
    r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].html_root.charptr.charptr_val = NULL;
    printf("Debug: html_root is NULL\n");
}

// 配置 passwd_path: r->loc_conf[8]->user_file.value.data
ngx_http_auth_basic_loc_conf_t *alcf_config = (ngx_http_auth_basic_loc_conf_t *)r->loc_conf[ngx_http_auth_basic_module.ctx_index];
if (alcf_config && alcf_config->user_file.value.data) {
    r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].passwd_path.charptr.charptr_len = alcf_config->user_file.value.len;
    r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].passwd_path.charptr.charptr_val = (char *)alcf_config->user_file.value.data;
    printf("Debug: passwd_path配置 - len=%zu, data=%s\n", alcf_config->user_file.value.len, alcf_config->user_file.value.data);
} else {
    r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].passwd_path.charptr.charptr_len = 0;
    r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].passwd_path.charptr.charptr_val = NULL;
    printf("Debug: passwd_path is NULL\n");
}

// 封装 r->uri 字段
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].uri.len = r[i0].uri.len;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].uri.data.charptr.charptr_len = strlen((char *)r[i0].uri.data);
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].uri.data.charptr.charptr_val = (char *)r[i0].uri.data;
printf("Debug: uri封装 - len=%zu, data=%.*s\n", 
	   r[i0].uri.len, (int)r[i0].uri.len, r[i0].uri.data);

r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].count=r[i0].count;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].subrequests=r[i0].subrequests;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].blocked=r[i0].blocked;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].aio=r[i0].aio;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].http_state=r[i0].http_state;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].complex_uri=r[i0].complex_uri;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].quoted_uri=r[i0].quoted_uri;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].plus_in_uri=r[i0].plus_in_uri;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].space_in_uri=r[i0].space_in_uri;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].invalid_header=r[i0].invalid_header;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].add_uri_to_alias=r[i0].add_uri_to_alias;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].valid_location=r[i0].valid_location;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].valid_unparsed_uri=r[i0].valid_unparsed_uri;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].uri_changed=r[i0].uri_changed;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].uri_changes=r[i0].uri_changes;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].request_body_in_single_buf=r[i0].request_body_in_single_buf;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].request_body_in_file_only=r[i0].request_body_in_file_only;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].request_body_in_persistent_file=r[i0].request_body_in_persistent_file;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].request_body_in_clean_file=r[i0].request_body_in_clean_file;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].request_body_file_group_access=r[i0].request_body_file_group_access;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].request_body_file_log_level=r[i0].request_body_file_log_level;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].request_body_no_buffering=r[i0].request_body_no_buffering;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].subrequest_in_memory=r[i0].subrequest_in_memory;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].waited=r[i0].waited;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].cached=r[i0].cached;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].proxy=r[i0].proxy;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].bypass_cache=r[i0].bypass_cache;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].no_cache=r[i0].no_cache;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].limit_conn_set=r[i0].limit_conn_set;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].limit_req_set=r[i0].limit_req_set;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].pipeline=r[i0].pipeline;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].chunked=r[i0].chunked;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].header_only=r[i0].header_only;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].expect_trailers=r[i0].expect_trailers;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].keepalive=r[i0].keepalive;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].lingering_close=r[i0].lingering_close;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].discard_body=r[i0].discard_body;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].reading_body=r[i0].reading_body;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].internal=r[i0].internal;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].error_page=r[i0].error_page;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].filter_finalize=r[i0].filter_finalize;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].post_action=r[i0].post_action;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].request_complete=r[i0].request_complete;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].request_output=r[i0].request_output;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].header_sent=r[i0].header_sent;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].expect_tested=r[i0].expect_tested;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].root_tested=r[i0].root_tested;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].done=r[i0].done;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].logged=r[i0].logged;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].buffered=r[i0].buffered;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].main_filter_need_in_memory=r[i0].main_filter_need_in_memory;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].filter_need_in_memory=r[i0].filter_need_in_memory;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].filter_need_temporary=r[i0].filter_need_temporary;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].preserve_body=r[i0].preserve_body;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].allow_ranges=r[i0].allow_ranges;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].subrequest_ranges=r[i0].subrequest_ranges;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].single_range=r[i0].single_range;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].disable_not_modified=r[i0].disable_not_modified;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].stat_reading=r[i0].stat_reading;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].stat_writing=r[i0].stat_writing;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].stat_processing=r[i0].stat_processing;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].background=r[i0].background;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].health_check=r[i0].health_check;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].http_minor=r[i0].http_minor;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].http_major=r[i0].http_major;
}




}

printf("=== Before RPC call ===\n");
fflush(stdout);

// 写入调试文件
FILE *debug_log = fopen("/tmp/nginx_rpc_debug.log", "a");
if (debug_log) {
    fprintf(debug_log, "=== Before RPC call, clnt=%p ===\n", clnt);
    fflush(debug_log);
    fclose(debug_log);
}

result_rpc=ngx_http_handler_rpc_1(r_rpc_ptr,clnt);

// 写入调试文件
debug_log = fopen("/tmp/nginx_rpc_debug.log", "a");
if (debug_log) {
    fprintf(debug_log, "=== After RPC call, result_rpc=%p ===\n", result_rpc);
    fflush(debug_log);
    fclose(debug_log);
}

printf("=== After RPC call, result_rpc=%p ===\n", result_rpc);
fflush(stdout);

// if (result_rpc==(void *)NULL) {
//     printf("ERROR: RPC call failed\n");
//     fflush(stdout);
//     clnt_perror(clnt,"call failed");
//     return;
// }else{
    printf("SUCCESS: RPC call succeeded\n");
    fflush(stdout);

	//请在此处完成一个使用r->connection和函数ngx_linux_sendfile_chain向客户端发送“hello”的动作

	// 使用 r->connection 和 ngx_linux_sendfile_chain 向客户端发送 "hello"
	// 1. 准备HTTP响应
	static u_char hello_response[] = 
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: 17\r\n"
		"Connection: close\r\n"
		"\r\n"
		"welcome to nginx\n";

	// 2. 从内存池分配缓冲区
	ngx_buf_t *b;
	b = ngx_create_temp_buf(r->pool, sizeof(hello_response) - 1);
	if (b == NULL) {
		printf("ERROR: Failed to create buffer\n");
		return;
	}
	
	// 3. 复制数据到缓冲区
	b->last = ngx_cpymem(b->last, hello_response, sizeof(hello_response) - 1);
	b->last_buf = 1;
	b->last_in_chain = 1;
	
	// 4. 创建chain链表
	ngx_chain_t out;
	out.buf = b;
	out.next = NULL;
	
	// 5. 发送数据
	ngx_chain_t *cl;
	cl = ngx_linux_sendfile_chain(r->connection, &out, 0);
	
	if (cl == NGX_CHAIN_ERROR) {
		printf("ERROR: Failed to send data\n");
	} else {
		printf("SUCCESS: Hello message sent\n");
		fflush(stdout);
	} 
// }



}