#include "nginx-1.15.5_rpc_wrapper.h"
extern CLIENT *clnt;

extern ngx_module_t  ngx_http_auth_basic_module;
typedef struct {
    ngx_http_complex_value_t  *realm;
    ngx_http_complex_value_t   user_file;
} ngx_http_auth_basic_loc_conf_t;
ngx_int_t ngx_http_auth_basic_handler_sense_1_wrapper(int *ngx_http_auth_basic_handler_sense_1_ret, ngx_http_request_t *r)
{
    ngx_http_auth_basic_handler_sense_1_ret_ *result_rpc;
    ngx_int_t sensefun_ret;
int_ptr ngx_http_auth_basic_handler_sense_1_ret_rpc;
if(ngx_http_auth_basic_handler_sense_1_ret!=NULL) {
ngx_http_auth_basic_handler_sense_1_ret_rpc.int_ptr_len=1;
ngx_http_auth_basic_handler_sense_1_ret_rpc.int_ptr_val=malloc(sizeof(int)*ngx_http_auth_basic_handler_sense_1_ret_rpc.int_ptr_len);
if(ngx_http_auth_basic_handler_sense_1_ret_rpc.int_ptr_val==NULL) {
perror("malloc failed");
exit(1);
}
for(int i0=0;i0<ngx_http_auth_basic_handler_sense_1_ret_rpc.int_ptr_len;i0++) {
ngx_http_auth_basic_handler_sense_1_ret_rpc.int_ptr_val[i0]=ngx_http_auth_basic_handler_sense_1_ret[i0];
	}
}else{ngx_http_auth_basic_handler_sense_1_ret_rpc.int_ptr_len=0;ngx_http_auth_basic_handler_sense_1_ret_rpc.int_ptr_val=NULL;}
ngx_http_request_t_rpc_ptr r_rpc_ptr;
if(r==NULL) {
r_rpc_ptr.ngx_http_request_t_rpc_ptr_len=0;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val=NULL;
} else {
r_rpc_ptr.ngx_http_request_t_rpc_ptr_len=1;
ngx_http_request_t_rpc test_request;
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val=&test_request;
for(int i0=0;i0<r_rpc_ptr.ngx_http_request_t_rpc_ptr_len;i0++) {
r_rpc_ptr.ngx_http_request_t_rpc_ptr_val[i0].signature=r[i0].signature;

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
result_rpc=ngx_http_auth_basic_handler_sense_1_rpc_1(ngx_http_auth_basic_handler_sense_1_ret_rpc,r_rpc_ptr,clnt);
if(result_rpc==(ngx_http_auth_basic_handler_sense_1_ret_ *)NULL) {
clnt_perror(clnt,"call failed");
}
sensefun_ret=result_rpc->ret0;
if(result_rpc->ngx_http_auth_basic_handler_sense_1_ret.int_ptr_val==NULL) {
ngx_http_auth_basic_handler_sense_1_ret=NULL;
} else {
ngx_http_auth_basic_handler_sense_1_ret=(int *)malloc(result_rpc->ngx_http_auth_basic_handler_sense_1_ret.int_ptr_len*sizeof(int));
if(ngx_http_auth_basic_handler_sense_1_ret==NULL) {
perror("malloc failed");
exit(1);
}
for(int i0=0;i0<result_rpc->ngx_http_auth_basic_handler_sense_1_ret.int_ptr_len;i0++) {
ngx_http_auth_basic_handler_sense_1_ret[i0]=result_rpc->ngx_http_auth_basic_handler_sense_1_ret.int_ptr_val[i0];
}
}
    if(result_rpc->r.ngx_http_request_t_rpc_ptr_val==NULL) {
        r=NULL;
    } else {
        r=malloc(result_rpc->r.ngx_http_request_t_rpc_ptr_len*sizeof( ngx_http_request_t));
        if(r==NULL) {
            perror("malloc failed");
            exit(1);
        }
        for(int i0=0;i0<result_rpc->r.ngx_http_request_t_rpc_ptr_len;i0++) {
r[i0].signature=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].signature;

r[i0].lingering_time=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].lingering_time;
r[i0].start_sec=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].start_sec;

r[i0].limit_rate=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].limit_rate;
r[i0].limit_rate_after=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].limit_rate_after;
r[i0].header_size=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].header_size;
r[i0].request_length=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].request_length;

r[i0].count=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].count;
r[i0].subrequests=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].subrequests;
r[i0].blocked=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].blocked;
r[i0].aio=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].aio;
r[i0].http_state=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].http_state;
r[i0].complex_uri=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].complex_uri;
r[i0].quoted_uri=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].quoted_uri;
r[i0].plus_in_uri=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].plus_in_uri;
r[i0].space_in_uri=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].space_in_uri;
r[i0].invalid_header=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].invalid_header;
r[i0].add_uri_to_alias=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].add_uri_to_alias;
r[i0].valid_location=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].valid_location;
r[i0].valid_unparsed_uri=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].valid_unparsed_uri;
r[i0].uri_changed=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].uri_changed;
r[i0].uri_changes=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].uri_changes;
r[i0].request_body_in_single_buf=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].request_body_in_single_buf;
r[i0].request_body_in_file_only=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].request_body_in_file_only;
r[i0].request_body_in_persistent_file=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].request_body_in_persistent_file;
r[i0].request_body_in_clean_file=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].request_body_in_clean_file;
r[i0].request_body_file_group_access=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].request_body_file_group_access;
r[i0].request_body_file_log_level=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].request_body_file_log_level;
r[i0].request_body_no_buffering=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].request_body_no_buffering;
r[i0].subrequest_in_memory=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].subrequest_in_memory;
r[i0].waited=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].waited;
r[i0].cached=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].cached;
r[i0].proxy=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].proxy;
r[i0].bypass_cache=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].bypass_cache;
r[i0].no_cache=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].no_cache;
r[i0].limit_conn_set=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].limit_conn_set;
r[i0].limit_req_set=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].limit_req_set;
r[i0].pipeline=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].pipeline;
r[i0].chunked=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].chunked;
r[i0].header_only=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].header_only;
r[i0].expect_trailers=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].expect_trailers;
r[i0].keepalive=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].keepalive;
r[i0].lingering_close=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].lingering_close;
r[i0].discard_body=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].discard_body;
r[i0].reading_body=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].reading_body;
r[i0].internal=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].internal;
r[i0].error_page=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].error_page;
r[i0].filter_finalize=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].filter_finalize;
r[i0].post_action=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].post_action;
r[i0].request_complete=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].request_complete;
r[i0].request_output=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].request_output;
r[i0].header_sent=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].header_sent;
r[i0].expect_tested=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].expect_tested;
r[i0].root_tested=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].root_tested;
r[i0].done=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].done;
r[i0].logged=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].logged;
r[i0].buffered=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].buffered;
r[i0].main_filter_need_in_memory=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].main_filter_need_in_memory;
r[i0].filter_need_in_memory=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].filter_need_in_memory;
r[i0].filter_need_temporary=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].filter_need_temporary;
r[i0].preserve_body=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].preserve_body;
r[i0].allow_ranges=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].allow_ranges;
r[i0].subrequest_ranges=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].subrequest_ranges;
r[i0].single_range=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].single_range;
r[i0].disable_not_modified=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].disable_not_modified;
r[i0].stat_reading=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].stat_reading;
r[i0].stat_writing=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].stat_writing;
r[i0].stat_processing=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].stat_processing;
r[i0].background=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].background;
r[i0].health_check=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].health_check;

r[i0].http_minor=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].http_minor;
r[i0].http_major=result_rpc->r.ngx_http_request_t_rpc_ptr_val[i0].http_major;
        }
    }
return sensefun_ret;
}

/* 全局变量访问函数 - 客户端Wrapper */
// ngx_str_t* get_ngx_sys_errlist_wrapper()
// {
//     ngx_sys_errlist_ret_t *result_rpc;
//     ngx_str_t* value = {0};
//     // result_rpc = get_ngx_sys_errlist_1(clnt);
//     //     if(result_rpc->value.ngx_str_t_rpc_ptr_val==NULL) {
//     //         value=NULL;
//     //     } else {
//     //         value=malloc(result_rpc->value.ngx_str_t_rpc_ptr_len*sizeof(struct ngx_str_t));
//     //         if(value==NULL) {
//     //             perror("malloc failed");
//     //             exit(1);
//     //         }
//     //         for(int i0=0;i0<result_rpc->value.ngx_str_t_rpc_ptr_len;i0++) {
//     // value[i0].len=result_rpc->value.ngx_str_t_rpc_ptr_val[i0].len;
//     // if (result_rpc->value.ngx_str_t_rpc_ptr_val[i0].data_rpc->int_ptr_val == NULL) {value[i0].data = NULL;}
//     // else {
//     // 	value[i0].data = (int *)malloc(result_rpc->value.ngx_str_t_rpc_ptr_val[i0].data_rpc->int_ptr_len * sizeof(int));
//     // 	if (value[i0].data == NULL) {
//     // 		perror("malloc failed");
//     // 		exit(1);
//     // 	}
//     // 	for (int i = 0; i < result_rpc->value.ngx_str_t_rpc_ptr_val[i0].data_rpc->int_ptr_len; i++) {
//     // 		value[i0].data[i] = result_rpc->value.ngx_str_t_rpc_ptr_val[i0].data_rpc->int_ptr_val[i];
//     // 	}
//     // }
//     //         }
//     //     }
//     return value;
// }

// int set_ngx_sys_errlist_wrapper(ngx_str_t value,int len)
// {
//     void *result;
//     // ngx_str_t_rpc_ptr value_rpc_ptr;
//     // if(value==NULL) {
//     // value_rpc_ptr.ngx_str_t_rpc_ptr_len=0;
//     // value_rpc_ptr.ngx_str_t_rpc_ptr_val=NULL;
//     // } else {
//     // value_rpc_ptr.ngx_str_t_rpc_ptr_len=1;
//     // value_rpc_ptr.ngx_str_t_rpc_ptr_val=malloc(1*sizeof(ngx_str_t_rpc));
//     // if(value_rpc_ptr.ngx_str_t_rpc_ptr_val==NULL) {
//     // perror("malloc failed");
//     // exit(1);
//     // }
//     // for(int i0=0;i0<value_rpc_ptr.ngx_str_t_rpc_ptr_len;i0++) {
//     // value_rpc_ptr.ngx_str_t_rpc_ptr_val[i0].len=value[i0].len;
//     // if (value[i0].data== NULL) {value_rpc_ptr.ngx_str_t_rpc_ptr_val[i0].data_rpc.int_ptr_len = 0;value_rpc_ptr.ngx_str_t_rpc_ptr_val[i0].data_rpc.int_ptr_val = NULL;}
//     // else {value_rpc_ptr.ngx_str_t_rpc_ptr_val[i0].data_rpc.int_ptr_len = 1;value_rpc_ptr.ngx_str_t_rpc_ptr_val[i0].data_rpc.int_ptr_val = malloc(value_rpc_ptr.ngx_str_t_rpc_ptr_val[i0].data_rpc.int_ptr_len * sizeof(int));
//     // 	if (value_rpc_ptr.ngx_str_t_rpc_ptr_val[i0].data_rpc.int_ptr_val == NULL) {
//     // 		perror("malloc failed");
//     // 		exit(1);
//     // 	}
//     // 	for (int i = 0; i < value_rpc_ptr.ngx_str_t_rpc_ptr_val[i0].data_rpc.int_ptr_len; i++) {
//     // 		value_rpc_ptr.ngx_str_t_rpc_ptr_val[i0].data_rpc.int_ptr_val[i] = value[i0].data[i];
//     // 	}
//     // }
//     // }
//     // }
//     // result = set_ngx_sys_errlist_1(value_rpc_ptr, clnt);
//     // if (result == NULL) {
//     //     clnt_perror(clnt, "set call failed");
//     //     return 0;
//     // }
//     return 1;
// }

// size_t get_ngx_sys_errlist_len_wrapper()
// {
//     ngx_sys_errlist_len_ret_t *result;
//     size_t ret_val = 0; // 默认值
//     result = get_ngx_sys_errlist_len_1(clnt);
//     if (result == NULL) {
//         clnt_perror(clnt, "get member call failed");
//         return ret_val;
//     }
//     ret_val = result->value;
//     return ret_val;
// }

// int set_ngx_sys_errlist_len_wrapper(size_t value)
// {
//     void *result;
//     result = set_ngx_sys_errlist_len_1(value, clnt);
//     if (result == NULL) {
//         clnt_perror(clnt, "set member call failed");
//         return 0;
//     }
//     return 1;
// }

// int set_ngx_use_stderr_wrapper(ngx_uint_t value)
// {
//     void *result;
//     // result = set_ngx_use_stderr_1(value, clnt);
//     // if (result == NULL) {
//     //     clnt_perror(clnt, "set member call failed");
//     //     return 0;
//     // }
//     return 1;
// }

// ngx_uint_t get_ngx_cached_time_sec_wrapper()
// {
//     // ngx_cached_time_sec_ret_t *result;
//     ngx_uint_t ret_val = 0; // 默认值
//     // result = get_ngx_cached_time_sec_1(clnt);
//     // if (result == NULL) {
//     //     clnt_perror(clnt, "get member call failed");
//     //     return ret_val;
//     // }
//     // ret_val = result->value;
//     return ret_val;
// }

// ngx_uint_t get_ngx_cached_time_wrapper()
// {
//     // ngx_cached_time_ret_t *result;
//     ngx_uint_t ret_val = 0; // 默认值
//     // result = get_ngx_cached_time_1(clnt);
//     // if (result == NULL) {
//     //     clnt_perror(clnt, "get member call failed");
//     //     return ret_val;
//     // }
//     // ret_val = result->value;
//     return ret_val;
// }

// int set_ngx_current_msec_wrapper(ngx_msec_t value)
// {
//     void *result;
//     // result = set_ngx_current_msec_1(value, clnt);
//     // if (result == NULL) {
//     //     clnt_perror(clnt, "set member call failed");
//     //     return 0;
//     // }
//     return 1;
// }

// ngx_msec_t get_ngx_current_msec_wrapper()
// {
//     // ngx_current_msec_ret_t *result;
//     ngx_msec_t ret_val = 0; // 默认值
//     // result = get_ngx_current_msec_1(clnt);
//     // if (result == NULL) {
//     //     clnt_perror(clnt, "get member call failed");
//     //     return ret_val;
//     // }
//     // ret_val = result->value;
//     return ret_val;
// }

// int set_ngx_cached_time_wrapper(ngx_uint_t value)
// {
//     void *result;
//     // result = set_ngx_cached_time_1(value, clnt);
//     // if (result == NULL) {
//     //     clnt_perror(clnt, "set member call failed");
//     //     return 0;
//     // }
//     return 1;
// }

// ngx_uint_t get_ngx_use_stderr_wrapper()
// {
//     // ngx_use_stderr_ret_t *result;
//     ngx_uint_t ret_val = 0; // 默认值
//     // // result = get_ngx_use_stderr_1(clnt);
//     // if (result == NULL) {
//     //     clnt_perror(clnt, "get member call failed");
//     //     return ret_val;
//     // }
//     // // ret_val = result->value;
//     return ret_val;
// }

// u_char * get_ngx_sys_errlist_data_wrapper()
// {
//     ngx_sys_errlist_data_ret_t *result;
//     u_char * ret_val = NULL;
//     result = get_ngx_sys_errlist_data_1(clnt);
//     if (result == NULL) {
//         clnt_perror(clnt, "get member call failed");
//         return ret_val;
//     }
//     ret_val = result->value;
//     return ret_val;
// }

// int set_ngx_sys_errlist_data_wrapper(u_char * value)
// {
//     void *result;
//     // result = set_ngx_sys_errlist_data_1(value, clnt);
//     if (result == NULL) {
//         clnt_perror(clnt, "set member call failed");
//         return 0;
//     }
//     return 1;
// }

// ngx_rbtree_t get_ngx_event_timer_rbtree_wrapper()
// {
//     ngx_event_timer_rbtree_ret_t *result_rpc;
//     ngx_rbtree_t default_value = {0};
//     result_rpc = get_ngx_event_timer_rbtree_1(clnt);
//     if (result_rpc == NULL) {
//         clnt_perror(clnt, "get call failed");
//         return default_value;
//     }
//     // if (result_rpc->value.root.ngx_rbtree_node_t_rpc_ptr_val == NULL) {default_value.root=NULL;}
//     // else {default_value.root=(ngx_rbtree_node_t *)malloc(result_rpc->value.root.ngx_rbtree_node_t_rpc_ptr_len * sizeof(ngx_rbtree_node_t));
//     // 	if (default_value.root == NULL) {
//     // 		perror("malloc failed");
//     // 		exit(1);
//     // 	}
//     // 	for (int i1 = 0; i1 < result_rpc->value.root.ngx_rbtree_node_t_rpc_ptr_len; i1++) {
//     // if (result_rpc->value.root.ngx_rbtree_node_t_rpc_ptr_val[i1].left.ngx_rbtree_node_t_rpc_ptr_val == NULL) {default_value.root[i1].left=NULL;}
//     // else {default_value.root[i1].left=(ngx_rbtree_node_t *)malloc(result_rpc->value.root.ngx_rbtree_node_t_rpc_ptr_val[i1].left.ngx_rbtree_node_t_rpc_ptr_len * sizeof(ngx_rbtree_node_t));
//     // 	if (default_value.root[i1].left == NULL) {
//     // 		perror("malloc failed");
//     // 		exit(1);
//     // 	}
//     // 	for (int i1 = 0; i1 < result_rpc->value.root.ngx_rbtree_node_t_rpc_ptr_val[i1].left.ngx_rbtree_node_t_rpc_ptr_len; i1++) {
//     // 	}
//     // }
//     // if (result_rpc->value.root.ngx_rbtree_node_t_rpc_ptr_val[i1].right.ngx_rbtree_node_t_rpc_ptr_val == NULL) {default_value.root[i1].right=NULL;}
//     // else {default_value.root[i1].right=(ngx_rbtree_node_t *)malloc(result_rpc->value.root.ngx_rbtree_node_t_rpc_ptr_val[i1].right.ngx_rbtree_node_t_rpc_ptr_len * sizeof(ngx_rbtree_node_t));
//     // 	if (default_value.root[i1].right == NULL) {
//     // 		perror("malloc failed");
//     // 		exit(1);
//     // 	}
//     // 	for (int i1 = 0; i1 < result_rpc->value.root.ngx_rbtree_node_t_rpc_ptr_val[i1].right.ngx_rbtree_node_t_rpc_ptr_len; i1++) {
//     // 	}
//     // }
//     // if (result_rpc->value.root.ngx_rbtree_node_t_rpc_ptr_val[i1].parent.ngx_rbtree_node_t_rpc_ptr_val == NULL) {default_value.root[i1].parent=NULL;}
//     // else {default_value.root[i1].parent=(ngx_rbtree_node_t *)malloc(result_rpc->value.root.ngx_rbtree_node_t_rpc_ptr_val[i1].parent.ngx_rbtree_node_t_rpc_ptr_len * sizeof(ngx_rbtree_node_t));
//     // 	if (default_value.root[i1].parent == NULL) {
//     // 		perror("malloc failed");
//     // 		exit(1);
//     // 	}
//     // 	for (int i1 = 0; i1 < result_rpc->value.root.ngx_rbtree_node_t_rpc_ptr_val[i1].parent.ngx_rbtree_node_t_rpc_ptr_len; i1++) {
//     // 	}
//     // }
//     // default_value.root[i1].color=result_rpc->value.root.ngx_rbtree_node_t_rpc_ptr_val[i1].color;
//     // default_value.root[i1].data=result_rpc->value.root.ngx_rbtree_node_t_rpc_ptr_val[i1].data;
//     // 	}
//     // }
//     // if (result_rpc->value.sentinel.ngx_rbtree_node_t_rpc_ptr_val == NULL) {default_value.sentinel=NULL;}
//     // else {default_value.sentinel=(ngx_rbtree_node_t *)malloc(result_rpc->value.sentinel.ngx_rbtree_node_t_rpc_ptr_len * sizeof(ngx_rbtree_node_t));
//     // 	if (default_value.sentinel == NULL) {
//     // 		perror("malloc failed");
//     // 		exit(1);
//     // 	}
//     // 	for (int i1 = 0; i1 < result_rpc->value.sentinel.ngx_rbtree_node_t_rpc_ptr_len; i1++) {
//     // 	}
//     // }
//     return default_value;
// }

// int set_ngx_event_timer_rbtree_wrapper(ngx_rbtree_t ngx_event_timer_rbtree)
// {
//     void *result;
//     // ngx_rbtree_t_rpc ngx_event_timer_rbtree_rpc;
//     // if (ngx_event_timer_rbtree.root == NULL) {ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_len = 0;ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_val = NULL;}
//     // else {ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_len = 1;ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_val = malloc(ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_len * sizeof(ngx_rbtree_node_t_rpc));
//     // 	if (ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_val == NULL) {
//     // 		perror("malloc failed");
//     // 		exit(1);
//     // 	}
//     // 	for (int i1 = 0; i1 < ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_len; i1++) {
//     // if (ngx_event_timer_rbtree.root[i1].left == NULL) {ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_val[i1].left.ngx_rbtree_node_t_rpc_ptr_len = 0;ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_val[i1].left.ngx_rbtree_node_t_rpc_ptr_val = NULL;}
//     // else {ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_val[i1].left.ngx_rbtree_node_t_rpc_ptr_len = 1;ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_val[i1].left.ngx_rbtree_node_t_rpc_ptr_val = malloc(ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_val[i1].left.ngx_rbtree_node_t_rpc_ptr_len * sizeof(ngx_rbtree_node_t_rpc));
//     // 	if (ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_val[i1].left.ngx_rbtree_node_t_rpc_ptr_val == NULL) {
//     // 		perror("malloc failed");
//     // 		exit(1);
//     // 	}
//     // 	for (int i2 = 0; i2 < ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_val[i1].left.ngx_rbtree_node_t_rpc_ptr_len; i2++) {
//     // 	}
//     // }
//     // if (ngx_event_timer_rbtree.root[i1].right == NULL) {ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_val[i1].right.ngx_rbtree_node_t_rpc_ptr_len = 0;ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_val[i1].right.ngx_rbtree_node_t_rpc_ptr_val = NULL;}
//     // else {ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_val[i1].right.ngx_rbtree_node_t_rpc_ptr_len = 1;ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_val[i1].right.ngx_rbtree_node_t_rpc_ptr_val = malloc(ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_val[i1].right.ngx_rbtree_node_t_rpc_ptr_len * sizeof(ngx_rbtree_node_t_rpc));
//     // 	if (ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_val[i1].right.ngx_rbtree_node_t_rpc_ptr_val == NULL) {
//     // 		perror("malloc failed");
//     // 		exit(1);
//     // 	}
//     // 	for (int i2 = 0; i2 < ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_val[i1].right.ngx_rbtree_node_t_rpc_ptr_len; i2++) {
//     // 	}
//     // }
//     // if (ngx_event_timer_rbtree.root[i1].parent == NULL) {ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_val[i1].parent.ngx_rbtree_node_t_rpc_ptr_len = 0;ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_val[i1].parent.ngx_rbtree_node_t_rpc_ptr_val = NULL;}
//     // else {ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_val[i1].parent.ngx_rbtree_node_t_rpc_ptr_len = 1;ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_val[i1].parent.ngx_rbtree_node_t_rpc_ptr_val = malloc(ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_val[i1].parent.ngx_rbtree_node_t_rpc_ptr_len * sizeof(ngx_rbtree_node_t_rpc));
//     // 	if (ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_val[i1].parent.ngx_rbtree_node_t_rpc_ptr_val == NULL) {
//     // 		perror("malloc failed");
//     // 		exit(1);
//     // 	}
//     // 	for (int i2 = 0; i2 < ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_val[i1].parent.ngx_rbtree_node_t_rpc_ptr_len; i2++) {
//     // 	}
//     // }
//     // ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_val[i1].color=ngx_event_timer_rbtree.root[i1].color;
//     // ngx_event_timer_rbtree_rpc.root.ngx_rbtree_node_t_rpc_ptr_val[i1].data=ngx_event_timer_rbtree.root[i1].data;
//     // 	}
//     // }
//     // if (ngx_event_timer_rbtree.sentinel == NULL) {ngx_event_timer_rbtree_rpc.sentinel.ngx_rbtree_node_t_rpc_ptr_len = 0;ngx_event_timer_rbtree_rpc.sentinel.ngx_rbtree_node_t_rpc_ptr_val = NULL;}
//     // else {ngx_event_timer_rbtree_rpc.sentinel.ngx_rbtree_node_t_rpc_ptr_len = 1;ngx_event_timer_rbtree_rpc.sentinel.ngx_rbtree_node_t_rpc_ptr_val = malloc(ngx_event_timer_rbtree_rpc.sentinel.ngx_rbtree_node_t_rpc_ptr_len * sizeof(ngx_rbtree_node_t_rpc));
//     // 	if (ngx_event_timer_rbtree_rpc.sentinel.ngx_rbtree_node_t_rpc_ptr_val == NULL) {
//     // 		perror("malloc failed");
//     // 		exit(1);
//     // 	}
//     // 	for (int i1 = 0; i1 < ngx_event_timer_rbtree_rpc.sentinel.ngx_rbtree_node_t_rpc_ptr_len; i1++) {
//     // 	}
//     // }
//     // result = set_ngx_event_timer_rbtree_1(ngx_event_timer_rbtree_rpc, clnt);
//     // if (result == NULL) {
//     //     clnt_perror(clnt, "set call failed");
//     //     return 0;
//     // }
//     return 1;
// }

// // FunctionMapEntry function_map[] = {
// //     {"ngx_sprintf_num", (void*)ngx_sprintf_num, SIG_NGX_SPRINTF_NUMFUNC},
// //     {"ngx_core_module_create_conf", (void*)ngx_core_module_create_conf, SIG_NGX_CORE_MODULE_CREATE_CONFFUNC},
// //     {"ngx_conf_set_flag_slot", (void*)ngx_conf_set_flag_slot, SIG_NGX_CONF_SET_FLAG_SLOTFUNC},
// //     {"ngx_set_user", (void*)ngx_set_user, SIG_NGX_SET_USERFUNC},
// //     {"ngx_set_worker_processes", (void*)ngx_set_worker_processes, SIG_NGX_SET_WORKER_PROCESSESFUNC},
// //     {"ngx_events_block", (void*)ngx_events_block, SIG_NGX_EVENTS_BLOCKFUNC},
// //     {"ngx_event_core_create_conf", (void*)ngx_event_core_create_conf, SIG_NGX_EVENT_CORE_CREATE_CONFFUNC},
// //     {"ngx_epoll_create_conf", (void*)ngx_epoll_create_conf, SIG_NGX_EPOLL_CREATE_CONFFUNC},
// //     {"ngx_event_connections", (void*)ngx_event_connections, SIG_NGX_EVENT_CONNECTIONSFUNC},
// //     {"ngx_event_core_init_conf", (void*)ngx_event_core_init_conf, SIG_NGX_EVENT_CORE_INIT_CONFFUNC},
// //     {"ngx_epoll_init_conf", (void*)ngx_epoll_init_conf, SIG_NGX_EPOLL_INIT_CONFFUNC},
// //     {"ngx_http_block", (void*)ngx_http_block, SIG_NGX_HTTP_BLOCKFUNC},
// //     {"ngx_http_core_create_main_conf", (void*)ngx_http_core_create_main_conf, SIG_NGX_HTTP_CORE_CREATE_MAIN_CONFFUNC},
// //     {"ngx_http_core_create_srv_conf", (void*)ngx_http_core_create_srv_conf, SIG_NGX_HTTP_CORE_CREATE_SRV_CONFFUNC},
// //     {"ngx_http_core_create_loc_conf", (void*)ngx_http_core_create_loc_conf, SIG_NGX_HTTP_CORE_CREATE_LOC_CONFFUNC},
// //     {"ngx_http_log_create_main_conf", (void*)ngx_http_log_create_main_conf, SIG_NGX_HTTP_LOG_CREATE_MAIN_CONFFUNC},
// //     {"ngx_http_log_create_loc_conf", (void*)ngx_http_log_create_loc_conf, SIG_NGX_HTTP_LOG_CREATE_LOC_CONFFUNC},
// //     {"ngx_http_upstream_create_main_conf", (void*)ngx_http_upstream_create_main_conf, SIG_NGX_HTTP_UPSTREAM_CREATE_MAIN_CONFFUNC},
// //     {"ngx_http_index_create_loc_conf", (void*)ngx_http_index_create_loc_conf, SIG_NGX_HTTP_INDEX_CREATE_LOC_CONFFUNC},
// //     {"ngx_http_mirror_create_loc_conf", (void*)ngx_http_mirror_create_loc_conf, SIG_NGX_HTTP_MIRROR_CREATE_LOC_CONFFUNC},
// //     {"ngx_http_try_files_create_loc_conf", (void*)ngx_http_try_files_create_loc_conf, SIG_NGX_HTTP_TRY_FILES_CREATE_LOC_CONFFUNC},
// //     {"ngx_http_auth_basic_create_loc_conf", (void*)ngx_http_auth_basic_create_loc_conf, SIG_NGX_HTTP_AUTH_BASIC_CREATE_LOC_CONFFUNC},
// //     {"ngx_http_upstream_hash_create_conf", (void*)ngx_http_upstream_hash_create_conf, SIG_NGX_HTTP_UPSTREAM_HASH_CREATE_CONFFUNC},
// //     {"ngx_http_upstream_random_create_conf", (void*)ngx_http_upstream_random_create_conf, SIG_NGX_HTTP_UPSTREAM_RANDOM_CREATE_CONFFUNC},
// //     {"ngx_http_upstream_keepalive_create_conf", (void*)ngx_http_upstream_keepalive_create_conf, SIG_NGX_HTTP_UPSTREAM_KEEPALIVE_CREATE_CONFFUNC},
// //     {"ngx_http_headers_create_conf", (void*)ngx_http_headers_create_conf, SIG_NGX_HTTP_HEADERS_CREATE_CONFFUNC},
// //     {"ngx_http_copy_filter_create_conf", (void*)ngx_http_copy_filter_create_conf, SIG_NGX_HTTP_COPY_FILTER_CREATE_CONFFUNC},
// //     {"ngx_http_core_preconfiguration", (void*)ngx_http_core_preconfiguration, SIG_NGX_HTTP_CORE_PRECONFIGURATIONFUNC},
// //     {"ngx_http_upstream_add_variables", (void*)ngx_http_upstream_add_variables, SIG_NGX_HTTP_UPSTREAM_ADD_VARIABLESFUNC},
// //     {"ngx_conf_include", (void*)ngx_conf_include, SIG_NGX_CONF_INCLUDEFUNC},
// //     {"ngx_http_core_types", (void*)ngx_http_core_types, SIG_NGX_HTTP_CORE_TYPESFUNC},
// //     {"ngx_http_core_type", (void*)ngx_http_core_type, SIG_NGX_HTTP_CORE_TYPEFUNC},
// //     {"ngx_conf_set_str_slot", (void*)ngx_conf_set_str_slot, SIG_NGX_CONF_SET_STR_SLOTFUNC},
// //     {"ngx_http_core_keepalive", (void*)ngx_http_core_keepalive, SIG_NGX_HTTP_CORE_KEEPALIVEFUNC},
// //     {"ngx_http_core_server", (void*)ngx_http_core_server, SIG_NGX_HTTP_CORE_SERVERFUNC},
// //     {"ngx_http_core_listen", (void*)ngx_http_core_listen, SIG_NGX_HTTP_CORE_LISTENFUNC},
// //     {"ngx_http_core_server_name", (void*)ngx_http_core_server_name, SIG_NGX_HTTP_CORE_SERVER_NAMEFUNC},
// //     {"ngx_http_set_complex_value_slot", (void*)ngx_http_set_complex_value_slot, SIG_NGX_HTTP_SET_COMPLEX_VALUE_SLOTFUNC},
// //     {"ngx_http_auth_basic_user_file", (void*)ngx_http_auth_basic_user_file, SIG_NGX_HTTP_AUTH_BASIC_USER_FILEFUNC},
// //     {"ngx_http_core_location", (void*)ngx_http_core_location, SIG_NGX_HTTP_CORE_LOCATIONFUNC},
// //     {"ngx_http_core_root", (void*)ngx_http_core_root, SIG_NGX_HTTP_CORE_ROOTFUNC},
// //     {"ngx_http_index_set_index", (void*)ngx_http_index_set_index, SIG_NGX_HTTP_INDEX_SET_INDEXFUNC},
// //     {"ngx_http_core_error_page", (void*)ngx_http_core_error_page, SIG_NGX_HTTP_CORE_ERROR_PAGEFUNC},
// //     {"ngx_http_core_init_main_conf", (void*)ngx_http_core_init_main_conf, SIG_NGX_HTTP_CORE_INIT_MAIN_CONFFUNC},
// //     {"ngx_http_core_merge_srv_conf", (void*)ngx_http_core_merge_srv_conf, SIG_NGX_HTTP_CORE_MERGE_SRV_CONFFUNC},
// //     {"ngx_http_core_merge_loc_conf", (void*)ngx_http_core_merge_loc_conf, SIG_NGX_HTTP_CORE_MERGE_LOC_CONFFUNC},
// //     {"ngx_http_log_merge_loc_conf", (void*)ngx_http_log_merge_loc_conf, SIG_NGX_HTTP_LOG_MERGE_LOC_CONFFUNC},
// //     {"ngx_http_upstream_init_main_conf", (void*)ngx_http_upstream_init_main_conf, SIG_NGX_HTTP_UPSTREAM_INIT_MAIN_CONFFUNC},
// //     {"ngx_http_index_merge_loc_conf", (void*)ngx_http_index_merge_loc_conf, SIG_NGX_HTTP_INDEX_MERGE_LOC_CONFFUNC},
// //     {"ngx_http_mirror_merge_loc_conf", (void*)ngx_http_mirror_merge_loc_conf, SIG_NGX_HTTP_MIRROR_MERGE_LOC_CONFFUNC},
// //     {"ngx_http_auth_basic_merge_loc_conf", (void*)ngx_http_auth_basic_merge_loc_conf, SIG_NGX_HTTP_AUTH_BASIC_MERGE_LOC_CONFFUNC},
// //     {"ngx_http_headers_merge_conf", (void*)ngx_http_headers_merge_conf, SIG_NGX_HTTP_HEADERS_MERGE_CONFFUNC},
// //     {"ngx_http_copy_filter_merge_conf", (void*)ngx_http_copy_filter_merge_conf, SIG_NGX_HTTP_COPY_FILTER_MERGE_CONFFUNC},
// //     {"ngx_http_cmp_locations", (void*)ngx_http_cmp_locations, SIG_NGX_HTTP_CMP_LOCATIONSFUNC},
// //     {"ngx_http_core_postconfiguration", (void*)ngx_http_core_postconfiguration, SIG_NGX_HTTP_CORE_POSTCONFIGURATIONFUNC},
// //     {"ngx_http_log_init", (void*)ngx_http_log_init, SIG_NGX_HTTP_LOG_INITFUNC},
// //     {"ngx_http_static_init", (void*)ngx_http_static_init, SIG_NGX_HTTP_STATIC_INITFUNC},
// //     {"ngx_http_index_init", (void*)ngx_http_index_init, SIG_NGX_HTTP_INDEX_INITFUNC},
// //     {"ngx_http_mirror_init", (void*)ngx_http_mirror_init, SIG_NGX_HTTP_MIRROR_INITFUNC},
// //     {"ngx_http_try_files_init", (void*)ngx_http_try_files_init, SIG_NGX_HTTP_TRY_FILES_INITFUNC},
// //     {"ngx_http_auth_basic_init", (void*)ngx_http_auth_basic_init, SIG_NGX_HTTP_AUTH_BASIC_INITFUNC},
// //     {"ngx_http_write_filter_init", (void*)ngx_http_write_filter_init, SIG_NGX_HTTP_WRITE_FILTER_INITFUNC},
// //     {"ngx_http_header_filter_init", (void*)ngx_http_header_filter_init, SIG_NGX_HTTP_HEADER_FILTER_INITFUNC},
// //     {"ngx_http_chunked_filter_init", (void*)ngx_http_chunked_filter_init, SIG_NGX_HTTP_CHUNKED_FILTER_INITFUNC},
// //     {"ngx_http_range_header_filter_init", (void*)ngx_http_range_header_filter_init, SIG_NGX_HTTP_RANGE_HEADER_FILTER_INITFUNC},
// //     {"ngx_http_headers_filter_init", (void*)ngx_http_headers_filter_init, SIG_NGX_HTTP_HEADERS_FILTER_INITFUNC},
// //     {"ngx_http_copy_filter_init", (void*)ngx_http_copy_filter_init, SIG_NGX_HTTP_COPY_FILTER_INITFUNC},
// //     {"ngx_http_range_body_filter_init", (void*)ngx_http_range_body_filter_init, SIG_NGX_HTTP_RANGE_BODY_FILTER_INITFUNC},
// //     {"ngx_http_not_modified_filter_init", (void*)ngx_http_not_modified_filter_init, SIG_NGX_HTTP_NOT_MODIFIED_FILTER_INITFUNC},
// //     {"ngx_core_module_init_conf", (void*)ngx_core_module_init_conf, SIG_NGX_CORE_MODULE_INIT_CONFFUNC},
// //     {"ngx_event_init_conf", (void*)ngx_event_init_conf, SIG_NGX_EVENT_INIT_CONFFUNC},
// //     {"ngx_event_module_init", (void*)ngx_event_module_init, SIG_NGX_EVENT_MODULE_INITFUNC},
// //     {"ngx_worker_process_cycle", (void*)ngx_worker_process_cycle, SIG_NGX_WORKER_PROCESS_CYCLEFUNC},
// //     {"ngx_event_process_init", (void*)ngx_event_process_init, SIG_NGX_EVENT_PROCESS_INITFUNC},
// //     {"ngx_epoll_init", (void*)ngx_epoll_init, SIG_NGX_EPOLL_INITFUNC},
// //     {"ngx_epoll_add_event", (void*)ngx_epoll_add_event, SIG_NGX_EPOLL_ADD_EVENTFUNC},
// //     {"ngx_epoll_process_events", (void*)ngx_epoll_process_events, SIG_NGX_EPOLL_PROCESS_EVENTSFUNC},
// //     {"ngx_event_accept", (void*)ngx_event_accept, SIG_NGX_EVENT_ACCEPTFUNC},
// //     {"ngx_http_init_connection", (void*)ngx_http_init_connection, SIG_NGX_HTTP_INIT_CONNECTIONFUNC},
// //     {"ngx_http_wait_request_handler", (void*)ngx_http_wait_request_handler, SIG_NGX_HTTP_WAIT_REQUEST_HANDLERFUNC},
// //     {"ngx_unix_recv", (void*)ngx_unix_recv, SIG_NGX_UNIX_RECVFUNC},
// //     {"ngx_http_process_host", (void*)ngx_http_process_host, SIG_NGX_HTTP_PROCESS_HOSTFUNC},
// //     {"ngx_http_process_unique_header_line", (void*)ngx_http_process_unique_header_line, SIG_NGX_HTTP_PROCESS_UNIQUE_HEADER_LINEFUNC},
// //     {"ngx_http_process_user_agent", (void*)ngx_http_process_user_agent, SIG_NGX_HTTP_PROCESS_USER_AGENTFUNC},
// //     {"ngx_http_core_find_config_phase", (void*)ngx_http_core_find_config_phase, SIG_NGX_HTTP_CORE_FIND_CONFIG_PHASEFUNC},
// //     {"ngx_http_core_access_phase", (void*)ngx_http_core_access_phase, SIG_NGX_HTTP_CORE_ACCESS_PHASEFUNC},
// //     {"ngx_http_auth_basic_handler", (void*)ngx_http_auth_basic_handler, SIG_NGX_HTTP_AUTH_BASIC_HANDLERFUNC},
// //     {"ngx_http_core_post_access_phase", (void*)ngx_http_core_post_access_phase, SIG_NGX_HTTP_CORE_POST_ACCESS_PHASEFUNC},
// //     {"ngx_http_core_generic_phase", (void*)ngx_http_core_generic_phase, SIG_NGX_HTTP_CORE_GENERIC_PHASEFUNC},
// //     {"ngx_http_try_files_handler", (void*)ngx_http_try_files_handler, SIG_NGX_HTTP_TRY_FILES_HANDLERFUNC},
// //     {"ngx_http_mirror_handler", (void*)ngx_http_mirror_handler, SIG_NGX_HTTP_MIRROR_HANDLERFUNC},
// //     {"ngx_http_core_content_phase", (void*)ngx_http_core_content_phase, SIG_NGX_HTTP_CORE_CONTENT_PHASEFUNC},
// //     {"ngx_http_index_handler", (void*)ngx_http_index_handler, SIG_NGX_HTTP_INDEX_HANDLERFUNC},
// //     {"ngx_http_static_handler", (void*)ngx_http_static_handler, SIG_NGX_HTTP_STATIC_HANDLERFUNC},
// //     {"ngx_http_not_modified_header_filter", (void*)ngx_http_not_modified_header_filter, SIG_NGX_HTTP_NOT_MODIFIED_HEADER_FILTERFUNC},
// //     {"ngx_http_headers_filter", (void*)ngx_http_headers_filter, SIG_NGX_HTTP_HEADERS_FILTERFUNC},
// //     {"ngx_http_range_header_filter", (void*)ngx_http_range_header_filter, SIG_NGX_HTTP_RANGE_HEADER_FILTERFUNC},
// //     {"ngx_http_chunked_header_filter", (void*)ngx_http_chunked_header_filter, SIG_NGX_HTTP_CHUNKED_HEADER_FILTERFUNC},
// //     {"ngx_http_header_filter", (void*)ngx_http_header_filter, SIG_NGX_HTTP_HEADER_FILTERFUNC},
// //     {"ngx_http_range_body_filter", (void*)ngx_http_range_body_filter, SIG_NGX_HTTP_RANGE_BODY_FILTERFUNC},
// //     {"ngx_http_copy_filter", (void*)ngx_http_copy_filter, SIG_NGX_HTTP_COPY_FILTERFUNC},
// //     {"ngx_http_trailers_filter", (void*)ngx_http_trailers_filter, SIG_NGX_HTTP_TRAILERS_FILTERFUNC},
// //     {"ngx_http_chunked_body_filter", (void*)ngx_http_chunked_body_filter, SIG_NGX_HTTP_CHUNKED_BODY_FILTERFUNC},
// //     {"ngx_http_write_filter", (void*)ngx_http_write_filter, SIG_NGX_HTTP_WRITE_FILTERFUNC},
// //     {"ngx_linux_sendfile_chain", (void*)ngx_linux_sendfile_chain, SIG_NGX_LINUX_SENDFILE_CHAINFUNC},
// //     {"ngx_http_log_handler", (void*)ngx_http_log_handler, SIG_NGX_HTTP_LOG_HANDLERFUNC},
// //     {"ngx_http_log_variable_getlen", (void*)ngx_http_log_variable_getlen, SIG_NGX_HTTP_LOG_VARIABLE_GETLENFUNC},
// //     {"ngx_http_variable_remote_addr", (void*)ngx_http_variable_remote_addr, SIG_NGX_HTTP_VARIABLE_REMOTE_ADDRFUNC},
// //     {"ngx_http_variable_remote_user", (void*)ngx_http_variable_remote_user, SIG_NGX_HTTP_VARIABLE_REMOTE_USERFUNC},
// //     {"ngx_http_variable_request_line", (void*)ngx_http_variable_request_line, SIG_NGX_HTTP_VARIABLE_REQUEST_LINEFUNC},
// //     {"ngx_http_variable_header", (void*)ngx_http_variable_header, SIG_NGX_HTTP_VARIABLE_HEADERFUNC},
// //     {"ngx_http_log_variable", (void*)ngx_http_log_variable, SIG_NGX_HTTP_LOG_VARIABLEFUNC},
// //     {"ngx_http_log_copy_short", (void*)ngx_http_log_copy_short, SIG_NGX_HTTP_LOG_COPY_SHORTFUNC},
// //     {"ngx_http_log_time", (void*)ngx_http_log_time, SIG_NGX_HTTP_LOG_TIMEFUNC},
// //     {"ngx_http_log_status", (void*)ngx_http_log_status, SIG_NGX_HTTP_LOG_STATUSFUNC},
// //     {"ngx_http_log_body_bytes_sent", (void*)ngx_http_log_body_bytes_sent, SIG_NGX_HTTP_LOG_BODY_BYTES_SENTFUNC},
// //     {"ngx_pool_cleanup_file", (void*)ngx_pool_cleanup_file, SIG_NGX_POOL_CLEANUP_FILEFUNC},
// //     {"ngx_http_keepalive_handler", (void*)ngx_http_keepalive_handler, SIG_NGX_HTTP_KEEPALIVE_HANDLERFUNC},
// //     {"ngx_epoll_del_connection", (void*)ngx_epoll_del_connection, SIG_NGX_EPOLL_DEL_CONNECTIONFUNC},
// //     {"ngx_signal_handler", (void*)ngx_signal_handler, SIG_NGX_SIGNAL_HANDLERFUNC},
// //     {"ngx_conf_flush_files", (void*)ngx_conf_flush_files, SIG_NGX_CONF_FLUSH_FILESFUNC},
// //     {"ngx_cleanup_environment", (void*)ngx_cleanup_environment, SIG_NGX_CLEANUP_ENVIRONMENTFUNC},
// //     {"ngx_resolver_cleanup", (void*)ngx_resolver_cleanup, SIG_NGX_RESOLVER_CLEANUPFUNC},
// //     {NULL, NULL, SIG_UNKNOWN} // 哨兵值
// // };
// // // --- 查找函数名：根据函数名查找函数条目 ---
// // void* find_function_entry_by_name(const char* func_name) {
// //     for (int i = 0; function_map[i].name != NULL; i++) {
// //         if (strcmp(function_map[i].name, func_name) == 0) {
// //             return &function_map[i].func_ptr_void;
// //         }
// //     }
// //     return NULL; // 未找到
// // }
// // // --- 新增函数：根据函数地址查找函数名 ---
// // const char* find_function_name_by_address(void* func_address) {
// //     for (int i = 0; function_map[i].name != NULL; i++) {
// //         // 直接比较 void* 指针
// //         if (function_map[i].func_ptr_void == func_address) {
// //             return function_map[i].name;
// //         }
// //     }
// //     return NULL; // 未找到对应的函数名
// // }
// char *get_filename_from_fp(FILE *fp) {
//     if (!fp) {                                                                 
//         fprintf(stderr, "Error: FILE pointer is NULL\n");
//         return NULL;
//     }
//     // 获取文件描述符
//     int fd = fileno(fp);
//     if (fd == -1) {
//         fprintf(stderr, "Error: fileno failed");
//         return NULL;
//     }
//     // 构造 /proc/self/fd/<fd> 路径
//     char fd_path[64];   
//     snprintf(fd_path, sizeof(fd_path), "/proc/self/fd/%d", fd);
//     // 分配内存存储文件名
//     char *filename = malloc(PATH_MAX);
//     if (!filename) {
//         fprintf(stderr, "Error: malloc failed");
//         return NULL;
//     }
//     // 读取符号链接目标（文件名）
//     ssize_t len = readlink(fd_path, filename, PATH_MAX - 1);
//     if (len == -1) {
//         fprintf(stderr, "Error: readlink failed for %s", fd_path);
//         free(filename);
//         return NULL;
//     }
//     filename[len] = '\0'; // 确保字符串终止
//     return filename;
// }