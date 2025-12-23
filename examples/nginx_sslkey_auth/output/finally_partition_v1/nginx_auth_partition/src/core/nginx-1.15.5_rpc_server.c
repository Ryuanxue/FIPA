#include "nginx-1.15.5_idl.h"
#include "ngx_http.h"

// 定义Nginx auth basic配置结构（与模块中的定义保持一致）
typedef struct {
    ngx_http_complex_value_t  *realm;
    ngx_http_complex_value_t   user_file;
} ngx_http_auth_basic_loc_conf_t;

ngx_int_t ngx_http_auth_basic_handler_sense_1(int *ngx_http_auth_basic_handler_sense_1_ret, ngx_http_request_t *r);

#define SYNC_LOC_CONF(r, clcf, idx, value) \
	do { \
		(r)->loc_conf[(idx)] = (value); \
		if ((clcf) && (clcf)->static_locations && (clcf)->static_locations->inclusive) { \
			(clcf)->static_locations->inclusive->loc_conf[(idx)] = (value); \
		} \
	} while (0)

// 前向声明
// 注意：只声明可以被外部引用的非 static 函数
ngx_int_t ngx_http_rewrite_handler(ngx_http_request_t *r);
ngx_int_t ngx_http_limit_req_handler(ngx_http_request_t *r);
ngx_int_t ngx_http_limit_conn_handler(ngx_http_request_t *r);
typedef ngx_int_t (*ngx_http_output_header_filter_pt)(ngx_http_request_t *r);
ngx_http_output_header_filter_pt  ngx_http_next_header_filter;
ngx_int_t
ngx_http_not_modified_header_filter(ngx_http_request_t *r);
ngx_int_t
ngx_http_headers_filter(ngx_http_request_t *r);
ngx_int_t
ngx_http_static_handler(ngx_http_request_t *r);
ngx_int_t
ngx_http_index_handler(ngx_http_request_t *r);
 ngx_int_t
ngx_http_access_handler(ngx_http_request_t *r);
ngx_int_t
ngx_http_auth_basic_handler(ngx_http_request_t *r);
ngx_int_t
ngx_http_try_files_handler(ngx_http_request_t *r);
ngx_int_t
ngx_http_mirror_handler(ngx_http_request_t *r);
 ngx_int_t
ngx_http_autoindex_handler(ngx_http_request_t *r);

// ⚠️ 不要声明 static 函数！
// ngx_http_access_handler 和 ngx_http_auth_basic_handler 在各自模块中是 static 的
// 在 RPC 模式下，访问控制在客户端完成，服务器端不需要这些 handler

// 外部模块声明
extern ngx_module_t  ngx_http_copy_filter_module;
extern ngx_module_t  ngx_http_range_body_filter_module;
extern ngx_module_t  ngx_http_gzip_filter_module;
extern ngx_module_t  ngx_http_ssi_filter_module;
extern ngx_module_t  ngx_http_charset_filter_module;
extern ngx_module_t  ngx_http_userid_filter_module;
extern ngx_module_t ngx_http_limit_req_module;
extern ngx_module_t  ngx_http_limit_conn_module;
extern ngx_module_t  ngx_http_access_module;
extern ngx_module_t  ngx_http_auth_basic_module;
extern ngx_module_t ngx_http_try_files_module;
extern ngx_module_t  ngx_http_mirror_module;
extern ngx_module_t  ngx_http_index_module;
extern ngx_module_t  ngx_http_headers_filter_module ;
// 定义 ngx_http_rewrite_loc_conf_t 结构
typedef struct {
    ngx_array_t  *codes;        /* uintptr_t */
    ngx_uint_t    stack_size;
    ngx_flag_t    log;
    ngx_flag_t    uninitialized_variable_warn;
} ngx_http_rewrite_loc_conf_t;

typedef struct {
    ngx_http_complex_value_t  *realm;
    ngx_http_complex_value_t   user_file;
} ngx_http_auth_basic_loc_conf_t;

// 定义 ngx_http_limit_req 相关结构
typedef struct {
    ngx_shm_zone_t              *shm_zone;
    /* integer value, 1 corresponds to 0.001 r/s */
    ngx_uint_t                   burst;
    ngx_uint_t                   nodelay; /* unsigned  nodelay:1 */
} ngx_http_limit_req_limit_t;

typedef struct {
    ngx_array_t                  limits;
    ngx_uint_t                   limit_log_level;
    ngx_uint_t                   delay_log_level;
    ngx_uint_t                   status_code;
} ngx_http_limit_req_conf_t;

// 定义 ngx_http_limit_conn 相关结构
typedef struct {
    ngx_shm_zone_t              *shm_zone;
    ngx_uint_t                   conn;
} ngx_http_limit_conn_limit_t;

typedef struct {
    ngx_array_t                  limits;
    ngx_uint_t                   log_level;
    ngx_uint_t                   status_code;
} ngx_http_limit_conn_conf_t;

// 定义 ngx_http_access 相关结构
typedef struct {
    in_addr_t         mask;
    in_addr_t         addr;
    ngx_uint_t        deny;      /* unsigned  deny:1; */
} ngx_http_access_rule_t;

typedef struct {
    ngx_array_t      *rules;     /* array of ngx_http_access_rule_t */
#if (NGX_HAVE_INET6)
    ngx_array_t      *rules6;    /* array of ngx_http_access_rule6_t */
#endif
#if (NGX_HAVE_UNIX_DOMAIN)
    ngx_array_t      *rules_un;  /* array of ngx_http_access_rule_un_t */
#endif
} ngx_http_access_loc_conf_t;

// 定义 ngx_http_try_files 相关结构
typedef struct {
    ngx_array_t           *lengths;
    ngx_array_t           *values;
    ngx_str_t              name;

    unsigned               code:10;
    unsigned               test_dir:1;
} ngx_http_try_file_t;

typedef struct {
    ngx_http_try_file_t   *try_files;
} ngx_http_try_files_loc_conf_t;

// 定义 ngx_http_mirror 相关结构
typedef struct {
    ngx_array_t  *mirror;
    ngx_flag_t    request_body;
} ngx_http_mirror_loc_conf_t;

// 定义 ngx_http_index 相关结构
typedef struct {
    ngx_str_t                name;
    ngx_array_t             *lengths;
    ngx_array_t             *values;
} ngx_http_index_t;

typedef struct {
    ngx_array_t             *indices;    /* array of ngx_http_index_t */
    size_t                   max_index_len;
} ngx_http_index_loc_conf_t;

// long *
// ngx_http_auth_basic_handler_rpc_1_svc( ngx_http_request_t_rpc_ptr arg2,  struct svc_req *rqstp)
// {
// 	static long  result=1;

// 	printf("Server: Entered ngx_http_auth_basic_handler_rpc_1_svc\n");
// 	/*
// 	 * insert server code here
// 	 */

// 	return &result;
// }

// 定义响应结构，用于返回给客户端
typedef struct {
	ngx_uint_t status;           // HTTP 状态码 (200, 404, 403, etc.)
	ngx_str_t  status_line;      // 状态行 ("200 OK", "404 Not Found", etc.)
	ngx_str_t  content_type;     // Content-Type 头
	ngx_str_t  body;             // 响应体内容
	size_t     content_length;   // Content-Length
} ngx_http_response_t;

// void *
// ngx_http_handler_rpc_1_svc(ngx_http_request_t_rpc_ptr arg2,  struct svc_req *rqstp)
// {
// 	static char * result="hello from server";	
// 	printf("Server: Entered ngx_http_handler_rpc_1_svc\n");
// 	return (void *)result;
// }

	


ngx_http_auth_basic_handler_sense_1_ret_ *
ngx_http_auth_basic_handler_sense_1_rpc_1_svc(ngx_http_request_t_rpc_ptr arg2,  struct svc_req *rqstp)
{
static ngx_http_auth_basic_handler_sense_1_ret_ result;
	/*
	 * insert server code here
	 */

	// ========================================
	// 初始化全局 I/O 操作表 ngx_io
	// 根据 GDB 数据：
	// ngx_io = {recv = 0x5555555abd85 <ngx_unix_recv>, 
	//           recv_chain = 0x5555555abf3d <ngx_readv_chain>, 
	//           udp_recv = 0x5555555ac34b <ngx_udp_unix_recv>, 
	//           send = 0x5555555ac407 <ngx_unix_send>, 
	//           udp_send = 0x5555555acc5d <ngx_udp_unix_send>, 
	//           udp_send_chain = 0x5555555acd75 <ngx_udp_unix_sendmsg_chain>, 
	//           send_chain = 0x5555555b43e3 <ngx_linux_sendfile_chain>, 
	//           flags = 1}
	// ========================================
	static int ngx_io_initialized = 0;
	if (!ngx_io_initialized) {
		// 初始化 Linux 平台的 I/O 操作函数
		ngx_io.recv = ngx_unix_recv;
		ngx_io.recv_chain = ngx_readv_chain;
		ngx_io.udp_recv = ngx_udp_unix_recv;
		ngx_io.send = ngx_unix_send;
		ngx_io.udp_send = ngx_udp_unix_send;
		ngx_io.udp_send_chain = ngx_udp_unix_sendmsg_chain;
		
#if (NGX_HAVE_SENDFILE)
		// 使用 Linux sendfile 优化的发送链
		ngx_io.send_chain = ngx_linux_sendfile_chain;
		ngx_io.flags = NGX_IO_SENDFILE;  // flags = 1 (NGX_IO_SENDFILE)
#else
		// 没有 sendfile 支持时使用 writev
		ngx_io.send_chain = ngx_writev_chain;
		ngx_io.flags = 0;
#endif
		
		ngx_io_initialized = 1;
		
		printf("Server: Initialized ngx_io (from GDB data)\n");
		printf("  - recv: %p (ngx_unix_recv)\n", (void*)ngx_io.recv);
		printf("  - recv_chain: %p (ngx_readv_chain)\n", (void*)ngx_io.recv_chain);
		printf("  - udp_recv: %p (ngx_udp_unix_recv)\n", (void*)ngx_io.udp_recv);
		printf("  - send: %p (ngx_unix_send)\n", (void*)ngx_io.send);
		printf("  - udp_send: %p (ngx_udp_unix_send)\n", (void*)ngx_io.udp_send);
		printf("  - udp_send_chain: %p (ngx_udp_unix_sendmsg_chain)\n", (void*)ngx_io.udp_send_chain);
		printf("  - send_chain: %p (ngx_linux_sendfile_chain)\n", (void*)ngx_io.send_chain);
		printf("  - flags: %u (NGX_IO_SENDFILE)\n", ngx_io.flags);
	}

	// 初始化响应结构

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

		// 解封装 uri 字段（请求URI）
		if (rpc_request->uri.data.charptr.charptr_len > 0 &&
			rpc_request->uri.data.charptr.charptr_val != NULL) {
			
			r->uri.len = rpc_request->uri.len;
			r->uri.data = (u_char *)malloc(rpc_request->uri.data.charptr.charptr_len + 1);
			if (r->uri.data == NULL) {
				perror("malloc uri data");
				free(r);
				return &result;
			}
			memcpy(r->uri.data, 
				   rpc_request->uri.data.charptr.charptr_val,
				   rpc_request->uri.data.charptr.charptr_len);
			r->uri.data[rpc_request->uri.data.charptr.charptr_len] = '\0';

			printf("Server: URI: %.*s\n", (int)rpc_request->uri.data.charptr.charptr_len, (char *)r->uri.data);
		} else {
			r->uri.len = 0;
			r->uri.data = NULL;
			printf("Server: No URI data\n");
		}

		
		// 分配 loc_conf 数组内存（8个模块配置指针）
		r->loc_conf = (void **)malloc(64* sizeof(void *));
		if (r->loc_conf == NULL) {
			perror("malloc loc_conf");
			free(r);
			exit(1);
		}
		
		// 初始化前7个配置指针为 NULL
		for (int i = 0; i < 64; i++) {
			r->loc_conf[i] = NULL;
		}

		//loc_conf[34]指向ngx_http_gzip_conf_t结构体，初始化相关字段	
		ngx_http_gzip_filter_module.ctx_index = 34;
		
		// 初始化 ngx_http_gzip_conf_t 结构体
		// 该模块用于压缩 HTTP 响应（gzip 压缩）
		typedef struct {
			ngx_flag_t           enable;                // 是否启用 gzip 压缩
			ngx_flag_t           no_buffer;             // 不使用缓冲区（直接流式压缩）
			ngx_hash_t           types;                 // 需要压缩的 MIME 类型哈希表
			ngx_bufs_t           bufs;                  // 压缩缓冲区配置 {num, size}
			size_t               postpone_gzipping;     // 延迟压缩的字节数阈值
			ngx_int_t            level;                 // 压缩级别 (1-9)
			size_t               wbits;                 // 窗口位数 (zlib window bits)
			size_t               memlevel;              // 内存级别 (zlib mem level)
			ssize_t              min_length;            // 最小压缩长度（小于此值不压缩）
			ngx_array_t         *types_keys;            // MIME 类型键数组
		} ngx_http_gzip_conf_t;
		
		ngx_http_gzip_conf_t *gzip_conf = (ngx_http_gzip_conf_t *)malloc(sizeof(ngx_http_gzip_conf_t));
		if (gzip_conf == NULL) {
			printf("Server: Failed to allocate ngx_http_gzip_conf_t\n");
			free(r->loc_conf);
			free(r);
			return (void *) &result;
		}
		memset(gzip_conf, 0, sizeof(ngx_http_gzip_conf_t));
		
		// 初始化字段（根据 GDB 数据）
		// enable = 0, no_buffer = 0
		gzip_conf->enable = 0;                         // gzip 禁用
		gzip_conf->no_buffer = 0;                      // 使用缓冲区
		
		// types 哈希表（buckets = 0x5555556aa6c8, size = 1）
		gzip_conf->types.size = 1;                     // 哈希表大小为 1
		gzip_conf->types.buckets = (ngx_hash_key_t **)malloc(sizeof(ngx_hash_key_t *) * 1);
		if (gzip_conf->types.buckets != NULL) {
			memset(gzip_conf->types.buckets, 0, sizeof(ngx_hash_key_t *) * 1);
		}
		
		// bufs 缓冲区配置（num = 32, size = 4096）
		gzip_conf->bufs.num = 32;                      // 32 个缓冲区
		gzip_conf->bufs.size = 4096;                   // 每个缓冲区 4KB
		
		// postpone_gzipping = 0
		gzip_conf->postpone_gzipping = 0;              // 不延迟压缩
		
		// level = 1 (压缩级别)
		gzip_conf->level = 1;                          // 压缩级别 1（最快速度）
		
		// wbits = 15 (窗口位数)
		gzip_conf->wbits = 15;                         // zlib 窗口位数 15（默认值）
		
		// memlevel = 8 (内存级别)
		gzip_conf->memlevel = 8;                       // zlib 内存级别 8（默认值）
		
		// min_length = 20 (最小压缩长度)
		gzip_conf->min_length = 20;                    // 小于 20 字节不压缩
		
		// types_keys = 0x0 (NULL)
		gzip_conf->types_keys = NULL;                  // types_keys 数组未设置
		
		// 将配置赋值到 loc_conf[34]
		r->loc_conf[34] = gzip_conf;
		
		printf("Server: Initialized ngx_http_gzip_conf_t at loc_conf[34] (from GDB data)\n");
		printf("  - enable = %d (gzip %s)\n", gzip_conf->enable, gzip_conf->enable ? "enabled" : "disabled");
		printf("  - no_buffer = %d\n", gzip_conf->no_buffer);
		printf("  - types.size = %lu, types.buckets = %p\n", 
			   gzip_conf->types.size, (void*)gzip_conf->types.buckets);
		printf("  - bufs.num = %d, bufs.size = %lu (32 x 4KB)\n", 
			   gzip_conf->bufs.num, gzip_conf->bufs.size);
		printf("  - postpone_gzipping = %lu\n", gzip_conf->postpone_gzipping);
		printf("  - level = %ld (compression level)\n", gzip_conf->level);
		printf("  - wbits = %lu, memlevel = %lu\n", gzip_conf->wbits, gzip_conf->memlevel);
		printf("  - min_length = %ld (minimum size to compress)\n", gzip_conf->min_length);
		printf("  - types_keys = %p\n", (void*)gzip_conf->types_keys);

		//loc_conf[36]指向ngx_http_ssi_loc_conf_t结构体，初始化相关字段r
		ngx_http_ssi_filter_module.ctx_index = 36;
		
		// 初始化 ngx_http_ssi_loc_conf_t 结构体
		// 该模块用于处理 SSI (Server Side Includes) 指令
		typedef struct {
			ngx_flag_t    enable;                       // 是否启用 SSI
			ngx_flag_t    silent_errors;                // 静默错误（不显示错误信息）
			ngx_flag_t    ignore_recycled_buffers;      // 忽略回收的缓冲区
			ngx_flag_t    last_modified;                // 是否设置 Last-Modified 头
			ngx_hash_t    types;                        // 需要处理 SSI 的 MIME 类型哈希表
			size_t        min_file_chunk;               // 最小文件块大小
			size_t        value_len;                    // SSI 变量值的最大长度
			ngx_array_t  *types_keys;                   // MIME 类型键数组
		} ngx_http_ssi_loc_conf_t;
		
		ngx_http_ssi_loc_conf_t *ssi_conf = (ngx_http_ssi_loc_conf_t *)malloc(sizeof(ngx_http_ssi_loc_conf_t));
		if (ssi_conf == NULL) {
			printf("Server: Failed to allocate ngx_http_ssi_loc_conf_t\n");
			free(r->loc_conf);
			free(r);
			return (void *) &result;
		}
		memset(ssi_conf, 0, sizeof(ngx_http_ssi_loc_conf_t));
		
		// 初始化字段（根据 GDB 数据）
		// enable = 0, silent_errors = 0, ignore_recycled_buffers = 0, last_modified = 0
		ssi_conf->enable = 0;                          // SSI 禁用
		ssi_conf->silent_errors = 0;                   // 显示错误
		ssi_conf->ignore_recycled_buffers = 0;         // 不忽略回收的缓冲区
		ssi_conf->last_modified = 0;                   // 不设置 Last-Modified 头
		
		// types 哈希表（buckets = 0x5555556aa948, size = 1）
		ssi_conf->types.size = 1;                      // 哈希表大小为 1
		ssi_conf->types.buckets = (ngx_hash_key_t **)malloc(sizeof(ngx_hash_key_t *) * 1);
		if (ssi_conf->types.buckets != NULL) {
			memset(ssi_conf->types.buckets, 0, sizeof(ngx_hash_key_t *) * 1);
		}
		
		// min_file_chunk = 1024
		ssi_conf->min_file_chunk = 1024;               // 最小文件块大小 1KB
		
		// value_len = 255
		ssi_conf->value_len = 255;                     // SSI 变量值的最大长度 255 字节
		
		// types_keys = 0x0 (NULL)
		ssi_conf->types_keys = NULL;                   // types_keys 数组未设置
		
		// 将配置赋值到 loc_conf[36]
		r->loc_conf[36] = ssi_conf;
		
		printf("Server: Initialized ngx_http_ssi_loc_conf_t at loc_conf[36] (from GDB data)\n");
		printf("  - enable = %d (SSI %s)\n", ssi_conf->enable, ssi_conf->enable ? "enabled" : "disabled");
		printf("  - silent_errors = %d, ignore_recycled_buffers = %d, last_modified = %d\n", 
			   ssi_conf->silent_errors, ssi_conf->ignore_recycled_buffers, ssi_conf->last_modified);
		printf("  - types.size = %lu, types.buckets = %p\n", 
			   ssi_conf->types.size, (void*)ssi_conf->types.buckets);
		printf("  - min_file_chunk = %lu (1KB)\n", ssi_conf->min_file_chunk);
		printf("  - value_len = %lu (255 bytes)\n", ssi_conf->value_len);
		printf("  - types_keys = %p\n", (void*)ssi_conf->types_keys);

		//loc_conf[37]指向ngx_http_charset_loc_conf_t结构体,初始化相关字段
		ngx_http_charset_filter_module.ctx_index = 37;
		
		// 初始化 ngx_http_charset_loc_conf_t 结构体
		// 该模块用于处理字符集转换和设置 Content-Type 的 charset
		typedef struct {
			ngx_int_t                   charset;           // 输出字符集索引
			ngx_int_t                   source_charset;    // 源字符集索引
			ngx_flag_t                  override_charset;  // 是否覆盖已有的字符集
			ngx_hash_t                  types;             // 需要处理的 MIME 类型哈希表
			ngx_array_t                *types_keys;        // MIME 类型键数组
		} ngx_http_charset_loc_conf_t;
		
		ngx_http_charset_loc_conf_t *charset_conf = (ngx_http_charset_loc_conf_t *)malloc(sizeof(ngx_http_charset_loc_conf_t));
		if (charset_conf == NULL) {
			printf("Server: Failed to allocate ngx_http_charset_loc_conf_t\n");
			free(r->loc_conf);
			free(r);
			return (void *) &result;
		}
		memset(charset_conf, 0, sizeof(ngx_http_charset_loc_conf_t));
		
		// 初始化字段（根据 GDB 数据）
		// charset = -2, source_charset = -2, override_charset = 0
		charset_conf->charset = -2;            // NGX_CONF_UNSET (-2) 表示未设置输出字符集
		charset_conf->source_charset = -2;     // NGX_CONF_UNSET (-2) 表示未设置源字符集
		charset_conf->override_charset = 0;    // 不覆盖已有的字符集
		
		// types 哈希表（buckets = 0x5555556aa9d0, size = 4）
		charset_conf->types.size = 4;          // 哈希表大小为 4
		charset_conf->types.buckets = (ngx_hash_elt_t **)malloc(sizeof(ngx_hash_elt_t *) * 4);
		if (charset_conf->types.buckets != NULL) {
			memset(charset_conf->types.buckets, 0, sizeof(ngx_hash_elt_t *) * 4);
		}
		
		// types_keys = 0x0 (NULL)
		charset_conf->types_keys = NULL;       // types_keys 数组未设置
		
		// 将配置赋值到 loc_conf[37]
		r->loc_conf[37] = charset_conf;
		
		printf("Server: Initialized ngx_http_charset_loc_conf_t at loc_conf[37] (from GDB data)\n");
		printf("  - charset = %ld (unset)\n", charset_conf->charset);
		printf("  - source_charset = %ld (unset)\n", charset_conf->source_charset);
		printf("  - override_charset = %d\n", charset_conf->override_charset);
		printf("  - types.size = %lu, types.buckets = %p\n", 
			   charset_conf->types.size, (void*)charset_conf->types.buckets);
		printf("  - types_keys = %p\n", (void*)charset_conf->types_keys);

		//loc_conf[38]指向ngx_http_userid_conf_t结构体,初始化相关字段
		ngx_http_userid_filter_module.ctx_index = 38;
		
		// 初始化 ngx_http_userid_conf_t 结构体
		// 该模块用于管理用户 ID cookie（用于用户跟踪）
		typedef struct {
			ngx_uint_t  enable;      // 是否启用 userid
			ngx_int_t   service;     // 服务 ID
			ngx_str_t   name;        // cookie 名称
			ngx_str_t   domain;      // cookie 域名
			ngx_str_t   path;        // cookie 路径
			ngx_str_t   p3p;         // P3P 隐私策略头
			time_t      expires;     // cookie 过期时间
			u_char      mark;        // 标记字符
		} ngx_http_userid_conf_t;
		
		ngx_http_userid_conf_t *userid_conf = (ngx_http_userid_conf_t *)malloc(sizeof(ngx_http_userid_conf_t));
		if (userid_conf == NULL) {
			printf("Server: Failed to allocate ngx_http_userid_conf_t\n");
			free(r->loc_conf);
			free(r);
			return (void *) &result;
		}
		memset(userid_conf, 0, sizeof(ngx_http_userid_conf_t));
		
		// 初始化字段（根据 GDB 数据）
		// enable = 0, service = -1
		userid_conf->enable = 0;          // userid 禁用
		userid_conf->service = -1;        // 未设置服务 ID (NGX_CONF_UNSET)
		
		// name = {len = 3, data = "uid"}
		userid_conf->name.len = 3;
		userid_conf->name.data = (u_char *)malloc(4);
		if (userid_conf->name.data != NULL) {
			memcpy(userid_conf->name.data, "uid", 3);
			userid_conf->name.data[3] = '\0';
		}
		
		// domain = {len = 0, data = ""}
		userid_conf->domain.len = 0;
		userid_conf->domain.data = (u_char *)malloc(1);
		if (userid_conf->domain.data != NULL) {
			userid_conf->domain.data[0] = '\0';
		}
		
		// path = {len = 8, data = "; path=/"}
		userid_conf->path.len = 8;
		userid_conf->path.data = (u_char *)malloc(9);
		if (userid_conf->path.data != NULL) {
			memcpy(userid_conf->path.data, "; path=/", 8);
			userid_conf->path.data[8] = '\0';
		}
		
		// p3p = {len = 0, data = ""}
		userid_conf->p3p.len = 0;
		userid_conf->p3p.data = (u_char *)malloc(1);
		if (userid_conf->p3p.data != NULL) {
			userid_conf->p3p.data[0] = '\0';
		}
		
		// expires = 0, mark = '\0'
		userid_conf->expires = 0;         // 无过期时间
		userid_conf->mark = '\0';         // 无标记字符
		
		// 将配置赋值到 loc_conf[38]
		r->loc_conf[38] = userid_conf;
		
		printf("Server: Initialized ngx_http_userid_conf_t at loc_conf[38] (from GDB data)\n");
		printf("  - enable = %u (userid %s)\n", userid_conf->enable, userid_conf->enable ? "enabled" : "disabled");
		printf("  - service = %ld\n", userid_conf->service);
		printf("  - name.len = %lu, name.data = \"%.*s\"\n", 
			   userid_conf->name.len, (int)userid_conf->name.len, userid_conf->name.data);
		printf("  - domain.len = %lu, domain.data = \"%.*s\"\n", 
			   userid_conf->domain.len, (int)userid_conf->domain.len, userid_conf->domain.data);
		printf("  - path.len = %lu, path.data = \"%.*s\"\n", 
			   userid_conf->path.len, (int)userid_conf->path.len, userid_conf->path.data);
		printf("  - p3p.len = %lu, p3p.data = \"%.*s\"\n", 
			   userid_conf->p3p.len, (int)userid_conf->p3p.len, userid_conf->p3p.data);
		printf("  - expires = %ld, mark = '%c'\n", userid_conf->expires, userid_conf->mark);

		//loc_conf[39]指向 ngx_http_headers_conf_t 的结构体,初始化expires，headers，trailers字段
		ngx_http_headers_filter_module.ctx_index = 39;
		
		// 初始化 ngx_http_headers_conf_t 结构体
		typedef struct {
			ngx_uint_t                 expires;        // ngx_http_expires_t 枚举
			time_t                     expires_time;
			void                      *expires_value;  // ngx_http_complex_value_t*
			ngx_array_t               *headers;
			ngx_array_t               *trailers;
		} ngx_http_headers_conf_t;
		
		ngx_http_headers_conf_t *headers_conf = (ngx_http_headers_conf_t *)malloc(sizeof(ngx_http_headers_conf_t));
		if (headers_conf == NULL) {
			printf("Server: Failed to allocate ngx_http_headers_conf_t\n");
			free(r->loc_conf);
			free(r);
			return (void *) &result;
		}
		memset(headers_conf, 0, sizeof(ngx_http_headers_conf_t));
		
		// 初始化字段（根据 GDB 数据）
		// expires = NGX_HTTP_EXPIRES_OFF (枚举值 0，表示不设置 Expires 头)
		// expires_time = 0, expires_value = 0x0
		// headers = 0x0, trailers = 0x0
		
		// NGX_HTTP_EXPIRES_OFF = 0 (第一个枚举值)
		headers_conf->expires = 0;          // NGX_HTTP_EXPIRES_OFF - 不设置 Expires 头
		headers_conf->expires_time = 0;     // 无过期时间
		headers_conf->expires_value = NULL; // 无复杂值表达式
		headers_conf->headers = NULL;       // 不设置额外的响应头
		headers_conf->trailers = NULL;      // 不设置 trailers
		
		// 将配置赋值到 loc_conf[39]
		r->loc_conf[39] = headers_conf;
		
		printf("Server: Initialized ngx_http_headers_conf_t at loc_conf[39] (from GDB data)\n");
		printf("  - expires = %u (NGX_HTTP_EXPIRES_OFF)\n", headers_conf->expires);
		printf("  - expires_time = %ld\n", headers_conf->expires_time);
		printf("  - expires_value = %p\n", headers_conf->expires_value);
		printf("  - headers = %p, trailers = %p\n", headers_conf->headers, headers_conf->trailers);


		//loc_conf[40]指向ngx_http_copy_filter_conf_t结构体,初始化相关字段
		
		// 初始化 ngx_http_copy_filter_conf_t 结构体
		// 该模块用于配置输出缓冲区（output buffers）
		typedef struct {
			ngx_bufs_t  bufs;     // 缓冲区配置 {num, size}
		} ngx_http_copy_filter_conf_t;
		
		ngx_http_copy_filter_conf_t *copy_conf = (ngx_http_copy_filter_conf_t *)malloc(sizeof(ngx_http_copy_filter_conf_t));
		if (copy_conf == NULL) {
			printf("Server: Failed to allocate ngx_http_copy_filter_conf_t\n");
			free(r->loc_conf);
			free(r);
			return (void *) &result;
		}
		memset(copy_conf, 0, sizeof(ngx_http_copy_filter_conf_t));
		
		// 初始化字段（根据 GDB 数据）
		// bufs = {num = 2, size = 32768}
		// ngx_bufs_t 包含 num (缓冲区数量) 和 size (每个缓冲区大小)
		copy_conf->bufs.num = 2;       // 2 个输出缓冲区
		copy_conf->bufs.size = 32768;  // 每个缓冲区 32KB
		
		// 将配置赋值到 loc_conf[40]
		r->loc_conf[40] = copy_conf;
		
		printf("Server: Initialized ngx_http_copy_filter_conf_t at loc_conf[40] (from GDB data)\n");
		printf("  - bufs.num = %d (2 output buffers)\n", copy_conf->bufs.num);
		printf("  - bufs.size = %lu (32KB per buffer)\n", copy_conf->bufs.size);

		// loc_conf[0] 指向 ngx_http_core_loc_conf_t 的结构体
		// 在 RPC 模式下，客户端已经完成了 location 匹配并传递了配置
		// 服务器端不需要再创建 static_locations 树，设置为 NULL 防止重复匹配
		ngx_http_core_loc_conf_t *clcf = (ngx_http_core_loc_conf_t *)malloc(sizeof(ngx_http_core_loc_conf_t));
		if (clcf == NULL) {
			printf("Server: Failed to allocate ngx_http_core_loc_conf_t\n");
			free(r->loc_conf);
			free(r);
			return (void *) &result;
		}
		memset(clcf, 0, sizeof(ngx_http_core_loc_conf_t));
		
		// 初始化location名称（从客户端传递的URI中提取，这里简化为根location "/"）
		clcf->name.len = 1;
		clcf->name.data = (u_char *)malloc(2);
		if (clcf->name.data == NULL) {
			printf("Server: Failed to allocate location name\n");
			free(clcf);
			free(r->loc_conf);
			free(r);
			return (void *) &result;
		}
		clcf->name.data[0] = '/';
		clcf->name.data[1] = '\0';
		
		// ⚠️ 关键：RPC 模式下不创建 static_locations 树
		// 原因：
		// 1. 客户端已经完成了 location 匹配
		// 2. 客户端通过 RPC 传递了已匹配的 loc_conf 配置
		// 3. 设置为 NULL 防止服务器端尝试重新匹配（避免死循环）
		clcf->static_locations = NULL;
		
		// ⚠️ 关键：clcf->loc_conf 指向请求的 loc_conf 数组
		// 这使得 clcf->loc_conf 成为一个包含所有模块配置的数组
		// 数组大小为 ngx_http_max_module（约 40-60 个元素）
		// 每个元素指向对应模块的配置结构体，例如：
		//   loc_conf[0] → clcf 自己（ngx_http_core_loc_conf_t）
		//   loc_conf[7] → ngx_http_auth_basic_loc_conf_t
		//   loc_conf[16] → ngx_http_rewrite_loc_conf_t
		//   loc_conf[其他] → 其他模块的配置
		clcf->loc_conf = r->loc_conf;
		
		// 初始化其他必要的字段
		clcf->exact_match = 0;
		clcf->noregex = 0;
		clcf->named = 0;
		clcf->noname = 0;
		clcf->max_ranges = 2147483647;
		clcf->server_tokens = 1;  // NGX_CONF_UNSET (1) - 显示服务器版本信息
		clcf->postpone_output=1460;
	// 	of.read_ahead = clcf->read_ahead;
    // of.directio = clcf->directio;
    // of.valid = clcf->open_file_cache_valid;
    // of.min_uses = clcf->open_file_cache_min_uses;
    // of.errors = clcf->open_file_cache_errors;
    // of.events = clcf->open_file_cache_events
	clcf->open_file_cache = NULL;
	clcf->open_file_cache_valid = 60;
	clcf->open_file_cache_min_uses = 1;
	clcf->open_file_cache_errors = 0;
	clcf->open_file_cache_events = 0;
	clcf->directio = 0;
	clcf->read_ahead = 0;
		
		// 从 RPC 请求中解封装 root 路径
		// 注意：需要客户端在 RPC 请求中包含 core_loc_conf 的 root 字段
		// 这里假设客户端传递了 root 信息（通常是文档根目录路径，如 "/usr/local/nginx/html"）
		// 如果 RPC 结构中没有直接的 core_loc_conf 字段，可能需要修改 .x 文件添加
		
		// 暂时使用默认值，等待客户端传递实际的 root 路径
		// TODO: 修改 multi_idl.x 文件，在 ngx_http_request_t_rpc 中添加 root 字段
		// 或者在 loc_conf 数组中包含 ngx_http_core_loc_conf_t_rpc
		
		// 默认 root 路径（可以从环境变量或配置文件读取）
		// const char *default_root = "/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx/output/muti-partition/nginx-mid/html";
		// clcf->root.len = strlen(default_root);
		// clcf->root.data = (u_char *)malloc(clcf->root.len + 1);
		// if (clcf->root.data != NULL) {
		// 	memcpy(clcf->root.data, default_root, clcf->root.len);
		// 	clcf->root.data[clcf->root.len] = '\0';
		// 	printf("Server: Set root path: %.*s\n", (int)clcf->root.len, clcf->root.data);
		// } else {
		// 	printf("Server: Failed to allocate root path\n");
		// }
		
		// ⚠️ 注意：error_log 字段将在 pool 创建后再初始化（见后文）
		// 因为 error_log 需要指向 r->pool->log，而此时 pool 还未创建
		clcf->error_log = NULL;

		// 初始化 clcf->types_hash 字段
		// types_hash 用于快速查找文件扩展名对应的 MIME 类型
		// 在 RPC 模式下，我们使用简化的初始化（不构建完整的哈希表）
		// size = 98 是 nginx 默认的 types_hash 桶数量（基于常见 MIME 类型数量）
		clcf->types_hash.size = 98;
		
		// 分配哈希桶数组
		// 每个桶是一个指向 ngx_hash_elt_t 的指针
		clcf->types_hash.buckets = (ngx_hash_elt_t **)malloc(98 * sizeof(ngx_hash_elt_t *));
		if (clcf->types_hash.buckets == NULL) {
			printf("Server: Failed to allocate types_hash buckets\n");
			free(clcf->root.data);
			free(clcf);
			free(r->loc_conf);
			free(r);
			return (void *) &result;
		}
		
		// 初始化所有桶为 NULL
		memset(clcf->types_hash.buckets, 0, 98 * sizeof(ngx_hash_elt_t *));
		
		// 初始化相关的哈希配置参数
		clcf->types_hash_max_size = 1024;       // 默认最大哈希大小
		clcf->types_hash_bucket_size = 64;      // 默认桶大小
		
		// ========================================
		// 手动填充常见的 MIME 类型映射
		// ========================================
		// 为了让 ngx_hash_find 能正确工作，我们需要填充 types_hash
		// 这里添加最常见的文件类型映射
		
		// 定义一个简单的映射表
		typedef struct {
			const char *ext;      // 文件扩展名
			const char *type;     // MIME 类型
		} mime_type_t;
		
		static const mime_type_t mime_types[] = {
			{"html", "text/html"},
			{"htm",  "text/html"},
			{"shtml","text/html"},
			{"css",  "text/css"},
			{"xml",  "text/xml"},
			{"gif",  "image/gif"},
			{"jpeg", "image/jpeg"},
			{"jpg",  "image/jpeg"},
			{"js",   "application/javascript"},
			{"txt",  "text/plain"},
			{"png",  "image/png"},
			{"ico",  "image/x-icon"},
			{"json", "application/json"},
			{"pdf",  "application/pdf"},
			{"zip",  "application/zip"},
			{NULL, NULL}  // 结束标记
		};
		
		// 填充哈希表
		int mime_count = 0;
		for (int i = 0; mime_types[i].ext != NULL; i++) {
			size_t ext_len = strlen(mime_types[i].ext);
			size_t type_len = strlen(mime_types[i].type);
			
			// 计算哈希值（使用 nginx 的哈希算法）
			ngx_uint_t hash = 0;
			for (size_t j = 0; j < ext_len; j++) {
				hash = ngx_hash(hash, mime_types[i].ext[j]);
			}
			
			// 计算桶索引
			ngx_uint_t bucket_idx = hash % clcf->types_hash.size;
			
			// 分配 ngx_hash_elt_t 结构
			// 格式: [value指针][key_len(2字节)][key数据]['\0']
			size_t elt_size = sizeof(void*) + sizeof(u_short) + ext_len + 1;
			elt_size = (elt_size + sizeof(void*) - 1) & ~(sizeof(void*) - 1); // 对齐
			
			ngx_hash_elt_t *elt = (ngx_hash_elt_t *)malloc(elt_size + sizeof(ngx_hash_elt_t));
			if (elt == NULL) {
				printf("Server: Failed to allocate hash element for %s\n", mime_types[i].ext);
				continue;
			}
			
			// 分配 MIME 类型字符串
			ngx_str_t *mime_str = (ngx_str_t *)malloc(sizeof(ngx_str_t));
			if (mime_str == NULL) {
				free(elt);
				continue;
			}
			mime_str->len = type_len;
			mime_str->data = (u_char *)malloc(type_len + 1);
			if (mime_str->data == NULL) {
				free(mime_str);
				free(elt);
				continue;
			}
			memcpy(mime_str->data, mime_types[i].type, type_len);
			mime_str->data[type_len] = '\0';
			
			// 填充 ngx_hash_elt_t
			elt->value = mime_str;  // 指向 ngx_str_t 的 MIME 类型
			elt->len = (u_short)ext_len;
			memcpy(elt->name, mime_types[i].ext, ext_len);
			elt->name[ext_len] = '\0';
			
			// 插入到桶中（简单链接，不处理冲突链）
			// 在真实 nginx 中，桶是一个数组，这里简化为单个元素
			if (clcf->types_hash.buckets[bucket_idx] == NULL) {
				clcf->types_hash.buckets[bucket_idx] = elt;
				mime_count++;
			} else {
				// 如果有冲突，跳过（简化处理）
				free(mime_str->data);
				free(mime_str);
				free(elt);
			}
		}
		
		printf("Server: Initialized types_hash with size=%lu, added %d MIME types\n", 
		       clcf->types_hash.size, mime_count);
		
		// ========================================
		// 根据 GDB 调试数据初始化所有非零/非NULL字段
		// ========================================
		
		// 初始化 default_type (默认 MIME 类型)
		const char *default_type_str = "application/octet-stream";
		clcf->default_type.len = strlen(default_type_str);
		clcf->default_type.data = (u_char *)malloc(clcf->default_type.len + 1);
		if (clcf->default_type.data != NULL) {
			memcpy(clcf->default_type.data, default_type_str, clcf->default_type.len);
			clcf->default_type.data[clcf->default_type.len] = '\0';
		}
		
		// 初始化文件大小和超时相关字段
		clcf->client_max_body_size = 1048576;           // 1MB (1024*1024)
		clcf->directio = 9223372036854775807LL;         // NGX_MAX_OFF_T_VALUE (off_t 最大值)
		clcf->directio_alignment = 512;                 // 512 字节对齐
		clcf->client_body_buffer_size = 8192;           // 8KB
		clcf->send_lowat = 0;                           // 不设置发送低水位
		clcf->postpone_output = 1460;                   // 延迟输出阈值 (MTU - headers)
		clcf->limit_rate = 0;                           // 不限速
		clcf->limit_rate_after = 0;                     // 不延迟限速
		clcf->sendfile_max_chunk = 0;                   // 不限制 sendfile 大小
		clcf->read_ahead = 0;                           // 不预读
		clcf->subrequest_output_buffer_size = 4096;     // 4KB 子请求缓冲区
		
		// 初始化超时字段 (毫秒)
		clcf->client_body_timeout = 60000;              // 60秒
		clcf->send_timeout = 60000;                     // 60秒
		clcf->keepalive_timeout = 65000;                // 65秒
		clcf->lingering_time = 30000;                   // 30秒
		clcf->lingering_timeout = 5000;                 // 5秒
		clcf->resolver_timeout = 30000;                 // 30秒
		
		// 初始化 keepalive 相关
		clcf->keepalive_header = 0;                     // 不发送 Keep-Alive 头
		clcf->keepalive_requests = 100;                 // 最多100个请求
		clcf->keepalive_disable = 5;                    // 禁用 MSIE6 的 keepalive
		
		// 初始化其他标志位
		clcf->satisfy = 0;                              // NGX_HTTP_SATISFY_ALL
		clcf->lingering_close = 1;                      // NGX_HTTP_LINGERING_ON
		clcf->if_modified_since = 1;                    // NGX_HTTP_IMS_EXACT
		clcf->max_ranges = 2147483647;                  // INT_MAX
		clcf->client_body_in_file_only = 0;             // 不强制写文件
		clcf->client_body_in_single_buffer = 0;         // 允许多个缓冲区
		clcf->internal = 0;                             // 非内部 location
		clcf->sendfile = 1;                             // 启用 sendfile
		clcf->aio = 0;                                  // 禁用 AIO
		clcf->aio_write = 0;                            // 禁用 AIO write
		clcf->tcp_nopush = 0;                           // 禁用 TCP_NOPUSH
		clcf->tcp_nodelay = 1;                          // 启用 TCP_NODELAY
		clcf->reset_timedout_connection = 0;            // 不重置超时连接
		clcf->absolute_redirect = 1;                    // 使用绝对路径重定向
		clcf->server_name_in_redirect = 0;              // 重定向不使用 server_name
		clcf->port_in_redirect = 1;                     // 重定向包含端口
		clcf->msie_padding = 1;                         // 启用 MSIE padding
		clcf->msie_refresh = 0;                         // 禁用 MSIE refresh
		clcf->log_not_found = 1;                        // 记录 404 错误
		clcf->log_subrequest = 0;                       // 不记录子请求
		clcf->recursive_error_pages = 0;                // 不递归处理错误页
		clcf->server_tokens = 1;                        // 显示服务器版本
		clcf->chunked_transfer_encoding = 1;            // 启用分块传输
		clcf->etag = 1;                                 // 启用 ETag
		
		// 初始化 gzip 相关字段
		clcf->gzip_vary = 0;                            // 不发送 Vary: Accept-Encoding
		clcf->gzip_http_version = 1001;                 // HTTP/1.1 (1*1000 + 1)
		clcf->gzip_proxied = 3;                         // NGX_HTTP_GZIP_PROXIED_EXPIRED | NOCACHE
		clcf->gzip_disable = NULL;                      // 不禁用 gzip
		
		// 初始化符号链接相关
		clcf->disable_symlinks = 0;                     // 允许符号链接
		clcf->disable_symlinks_from = NULL;             // 不限制符号链接路径
		
		// 初始化 open_file_cache 相关
		clcf->open_file_cache = NULL;                   // 不使用文件缓存
		clcf->open_file_cache_valid = 60;               // 60秒有效期
		clcf->open_file_cache_min_uses = 1;             // 最少使用1次
		clcf->open_file_cache_errors = 0;               // 不缓存错误
		clcf->open_file_cache_events = 0;               // 不缓存事件
		
		printf("Server: Initialized all clcf fields based on GDB debug data\n");
		printf("  - client_max_body_size = %ld\n", (long)clcf->client_max_body_size);
		printf("  - default_type = %.*s\n", (int)clcf->default_type.len, clcf->default_type.data);
		printf("  - keepalive_timeout = %ld ms\n", (long)clcf->keepalive_timeout);
		
		// ========================================
		// 初始化需要分配内存的指针字段 (根据 GDB 数据)
		// ========================================
		
		// ========================================
		// 首先创建 inclusive 指向的子 location 配置
		// 根据 GDB 数据分析，inclusive 不是指向 clcf 自己，而是指向一个独立的子 location
		// 对比发现：
		// - r->loc_conf[0] (父): static_locations != NULL, locations != NULL
		// - inclusive (子): static_locations = NULL, locations = NULL (叶子节点)
		// ========================================
		
		ngx_http_core_loc_conf_t *inclusive_clcf = (ngx_http_core_loc_conf_t *)malloc(sizeof(ngx_http_core_loc_conf_t));
		if (inclusive_clcf == NULL) {
			printf("Server: Failed to allocate inclusive_clcf\n");
			return (void *) &result;
		}
		memset(inclusive_clcf, 0, sizeof(ngx_http_core_loc_conf_t));
		
		// 初始化 inclusive location 的字段（根据 GDB 数据）
		// name = {len = 1, data = "/"}
		inclusive_clcf->name.len = 1;
		inclusive_clcf->name.data = (u_char *)malloc(2);
		if (inclusive_clcf->name.data != NULL) {
			inclusive_clcf->name.data[0] = '/';
			inclusive_clcf->name.data[1] = '\0';
		}
		
		// regex = 0x0, noname = 0, lmt_excpt = 0, named = 0
		inclusive_clcf->regex = NULL;
		inclusive_clcf->noname = 0;
		inclusive_clcf->lmt_excpt = 0;
		inclusive_clcf->named = 0;
		
		// exact_match = 0, noregex = 0, auto_redirect = 0
		inclusive_clcf->exact_match = 0;
		inclusive_clcf->noregex = 0;
		inclusive_clcf->auto_redirect = 0;
		
		// gzip_disable_msie6 = 0, gzip_disable_degradation = 0
		inclusive_clcf->gzip_disable_msie6 = 0;
		inclusive_clcf->gzip_disable_degradation = 0;
		
		// ⚠️ 关键：子 location 的 static_locations 和 regex_locations 为 NULL (叶子节点)
		// static_locations = 0x0, regex_locations = 0x0
		inclusive_clcf->static_locations = NULL;
		inclusive_clcf->regex_locations = NULL;
		
		// ⚠️⚠️⚠️ 关键修正：loc_conf 必须是独立的数组！⚠️⚠️⚠️
		// 
		// 原因分析（基于 nginx 源码 ngx_http_core_find_static_location）：
		// 当匹配到 location 时，nginx 执行：
		//     r->loc_conf = node->inclusive->loc_conf;  // 替换请求的配置！
		// 
		// 如果 inclusive_clcf->loc_conf = r->loc_conf（错误做法）：
		//     r->loc_conf = r->loc_conf;  // 自己赋值给自己，没有效果！
		// 
		// 正确做法：inclusive_clcf 必须有独立的 loc_conf 数组
		// GDB 数据也证实：loc_conf = 0x5555556a3bf8（与 r->loc_conf 地址不同）
		
		// 分配独立的 loc_conf 数组
		inclusive_clcf->loc_conf = (void **)malloc(64 * sizeof(void *));
		if (inclusive_clcf->loc_conf == NULL) {
			printf("Server: Failed to allocate inclusive_clcf->loc_conf\n");
			free(inclusive_clcf->name.data);
			free(inclusive_clcf);
			return (void *) &result;
		}
		
		// 初始化所有槽位为 NULL
		for (int i = 0; i < 64; i++) {
			inclusive_clcf->loc_conf[i] = NULL;
		}
		
		// ⚠️ 超级关键：loc_conf[0] 必须指向 inclusive_clcf 自己！
		// 原因：当 nginx 调用 ngx_http_get_module_loc_conf(r, ngx_http_core_module) 时
		// 实际展开为：r->loc_conf[ngx_http_core_module.ctx_index]
		// 即：r->loc_conf[0]
		// 而此时 r->loc_conf 已经被替换为 inclusive_clcf->loc_conf
		// 所以必须让 inclusive_clcf->loc_conf[0] = inclusive_clcf
		inclusive_clcf->loc_conf[0] = inclusive_clcf;
		
		// 继承父 location 的其他模块配置
		// 注意：此时 r->loc_conf[1-63] 中的一些配置已经初始化
		// 子 location 继承这些配置（除非被覆盖）
		for (int i = 1; i < 64; i++) {
			if (r->loc_conf[i] != NULL) {
				inclusive_clcf->loc_conf[i] = r->loc_conf[i];
			}
		}
		
		printf("Server: ⭐ Created independent loc_conf for inclusive_clcf\n");
		printf("  - inclusive_clcf->loc_conf = %p (NEW array, not r->loc_conf!)\n", 
			   (void*)inclusive_clcf->loc_conf);
		printf("  - inclusive_clcf->loc_conf[0] = %p (points to inclusive_clcf)\n", 
			   (void*)inclusive_clcf->loc_conf[0]);
		printf("  - r->loc_conf = %p (will be REPLACED by nginx)\n", 
			   (void*)r->loc_conf);
		printf("  - After match: r->loc_conf will become %p\n", 
			   (void*)inclusive_clcf->loc_conf);
		
		// limit_except = 0, limit_except_loc_conf = 0x0
		inclusive_clcf->limit_except = 0;
		inclusive_clcf->limit_except_loc_conf = NULL;
		
		// handler = 0x0
		inclusive_clcf->handler = NULL;
		
		// alias = 0
		inclusive_clcf->alias = 0;
		
		// root = {len = 87, data = ".../html"}
		// const char *html_root = "/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx/output/muti-partition/html";
		// inclusive_clcf->root.len = strlen(html_root);
		// inclusive_clcf->root.data = (u_char *)malloc(inclusive_clcf->root.len + 1);
		// if (inclusive_clcf->root.data != NULL) {
		// 	strcpy((char *)inclusive_clcf->root.data, html_root);
		// }
		
		// post_action = {len = 0, data = 0x0}
		inclusive_clcf->post_action.len = 0;
		inclusive_clcf->post_action.data = NULL;
		
		// root_lengths = 0x0, root_values = 0x0
		inclusive_clcf->root_lengths = NULL;
		inclusive_clcf->root_values = NULL;
		
		// types = 0x555555699748 (指向 types 数组)
		inclusive_clcf->types = (ngx_array_t *)malloc(sizeof(ngx_array_t));
		if (inclusive_clcf->types != NULL) {
			memset(inclusive_clcf->types, 0, sizeof(ngx_array_t));
			inclusive_clcf->types->elts = NULL;
			inclusive_clcf->types->nelts = 0;
			inclusive_clcf->types->size = sizeof(ngx_str_t);
			inclusive_clcf->types->nalloc = 0;
			inclusive_clcf->types->pool = r->pool;
		}
		
		// types_hash = {buckets = 0x5555556a86a8, size = 98}
		inclusive_clcf->types_hash.size = 98;
		inclusive_clcf->types_hash.buckets = (ngx_hash_elt_t **)malloc(sizeof(ngx_hash_elt_t *) * 98);
		if (inclusive_clcf->types_hash.buckets != NULL) {
			memset(inclusive_clcf->types_hash.buckets, 0, sizeof(ngx_hash_elt_t *) * 98);
		}
		
		// default_type = {len = 24, data = "application/octet-stream"}
		const char *inclusive_default_type_str = "application/octet-stream";
		inclusive_clcf->default_type.len = strlen(inclusive_default_type_str);
		inclusive_clcf->default_type.data = (u_char *)malloc(inclusive_clcf->default_type.len + 1);
		if (inclusive_clcf->default_type.data != NULL) {
			strcpy((char *)inclusive_clcf->default_type.data, inclusive_default_type_str);
		}
		
		// 复制所有数值字段（与父 location 相同）
		inclusive_clcf->client_max_body_size = 1048576;
		inclusive_clcf->directio = 9223372036854775807LL;
		inclusive_clcf->directio_alignment = 512;
		inclusive_clcf->client_body_buffer_size = 8192;
		inclusive_clcf->send_lowat = 0;
		inclusive_clcf->postpone_output = 1460;
		inclusive_clcf->limit_rate = 0;
		inclusive_clcf->limit_rate_after = 0;
		inclusive_clcf->sendfile_max_chunk = 0;
		inclusive_clcf->read_ahead = 0;
		inclusive_clcf->subrequest_output_buffer_size = 4096;
		inclusive_clcf->client_body_timeout = 60000;
		inclusive_clcf->send_timeout = 60000;
		inclusive_clcf->keepalive_timeout = 65000;
		inclusive_clcf->lingering_time = 30000;
		inclusive_clcf->lingering_timeout = 5000;
		inclusive_clcf->resolver_timeout = 30000;
		
		// resolver = 0x5555556a89c8
		inclusive_clcf->resolver = (ngx_resolver_t *)malloc(sizeof(ngx_resolver_t));
		if (inclusive_clcf->resolver != NULL) {
			memset(inclusive_clcf->resolver, 0, sizeof(ngx_resolver_t));
		}
		
		// keepalive 相关
		inclusive_clcf->keepalive_header = 0;
		inclusive_clcf->keepalive_requests = 100;
		inclusive_clcf->keepalive_disable = 5;
		
		// 标志位
		inclusive_clcf->satisfy = 0;
		inclusive_clcf->lingering_close = 1;
		inclusive_clcf->if_modified_since = 1;
		inclusive_clcf->max_ranges = 2147483647;
		inclusive_clcf->client_body_in_file_only = 0;
		inclusive_clcf->client_body_in_single_buffer = 0;
		inclusive_clcf->internal = 0;
		inclusive_clcf->sendfile = 1;
		inclusive_clcf->aio = 0;
		inclusive_clcf->aio_write = 0;
		inclusive_clcf->tcp_nopush = 0;
		inclusive_clcf->tcp_nodelay = 1;
		inclusive_clcf->reset_timedout_connection = 0;
		inclusive_clcf->absolute_redirect = 1;
		inclusive_clcf->server_name_in_redirect = 0;
		inclusive_clcf->port_in_redirect = 1;
		inclusive_clcf->msie_padding = 1;
		inclusive_clcf->msie_refresh = 0;
		inclusive_clcf->log_not_found = 1;
		inclusive_clcf->log_subrequest = 0;
		inclusive_clcf->recursive_error_pages = 0;
		inclusive_clcf->server_tokens = 1;
		inclusive_clcf->chunked_transfer_encoding = 1;
		inclusive_clcf->etag = 1;
		inclusive_clcf->gzip_vary = 0;
		inclusive_clcf->gzip_http_version = 1001;
		inclusive_clcf->gzip_proxied = 3;
		inclusive_clcf->gzip_disable = NULL;
		inclusive_clcf->disable_symlinks = 0;
		inclusive_clcf->disable_symlinks_from = NULL;
		
		// error_pages = 0x5555556a70e0
		inclusive_clcf->error_pages = (ngx_array_t *)malloc(sizeof(ngx_array_t));
		if (inclusive_clcf->error_pages != NULL) {
			memset(inclusive_clcf->error_pages, 0, sizeof(ngx_array_t));
			inclusive_clcf->error_pages->elts = NULL;
			inclusive_clcf->error_pages->nelts = 0;
			inclusive_clcf->error_pages->size = sizeof(void*);
			inclusive_clcf->error_pages->nalloc = 0;
			inclusive_clcf->error_pages->pool = r->pool;
		}
		
		// client_body_temp_path = 0x5555556a8c40
		inclusive_clcf->client_body_temp_path = (ngx_path_t *)malloc(sizeof(ngx_path_t));
		if (inclusive_clcf->client_body_temp_path != NULL) {
			memset(inclusive_clcf->client_body_temp_path, 0, sizeof(ngx_path_t));
			const char *temp_path = "client_body_temp";
			inclusive_clcf->client_body_temp_path->name.len = strlen(temp_path);
			inclusive_clcf->client_body_temp_path->name.data = (u_char *)malloc(inclusive_clcf->client_body_temp_path->name.len + 1);
			if (inclusive_clcf->client_body_temp_path->name.data != NULL) {
				strcpy((char *)inclusive_clcf->client_body_temp_path->name.data, temp_path);
			}
		}
		
		// open_file_cache 相关
		inclusive_clcf->open_file_cache = NULL;
		inclusive_clcf->open_file_cache_valid = 60;
		inclusive_clcf->open_file_cache_min_uses = 1;
		inclusive_clcf->open_file_cache_errors = 0;
		inclusive_clcf->open_file_cache_events = 0;
		
		// ========================================
		// 初始化 error_log（根据 GDB 数据）
		// error_log = {log_level = 4, file = 0x55555568c7a8, ...}
		// ========================================
		inclusive_clcf->error_log = (ngx_log_t *)malloc(sizeof(ngx_log_t));
		if (inclusive_clcf->error_log == NULL) {
			printf("Server: Failed to allocate inclusive_clcf->error_log\n");
			// 清理已分配的资源
			free(inclusive_clcf->loc_conf);
			free(inclusive_clcf->name.data);
			free(inclusive_clcf);
			return (void *) &result;
		}
		memset(inclusive_clcf->error_log, 0, sizeof(ngx_log_t));
		
		// log_level = 4 (NGX_LOG_WARN)
		inclusive_clcf->error_log->log_level = NGX_LOG_WARN;
		
		// file - 分配 ngx_open_file_t 结构
		inclusive_clcf->error_log->file = (ngx_open_file_t *)malloc(sizeof(ngx_open_file_t));
		if (inclusive_clcf->error_log->file != NULL) {
			memset(inclusive_clcf->error_log->file, 0, sizeof(ngx_open_file_t));
			inclusive_clcf->error_log->file->fd = ngx_stderr;
			inclusive_clcf->error_log->file->name.len = 0;
			inclusive_clcf->error_log->file->name.data = NULL;
		}
		
		// 其他字段设为默认值
		inclusive_clcf->error_log->connection = 0;
		inclusive_clcf->error_log->disk_full_time = 0;
		inclusive_clcf->error_log->handler = NULL;
		inclusive_clcf->error_log->data = NULL;
		inclusive_clcf->error_log->writer = NULL;
		inclusive_clcf->error_log->wdata = NULL;
		inclusive_clcf->error_log->action = NULL;
		inclusive_clcf->error_log->next = NULL;
		
		printf("Server: Initialized inclusive_clcf->error_log\n");
		printf("  - log_level: %u (NGX_LOG_WARN)\n", inclusive_clcf->error_log->log_level);
		printf("  - file: %p\n", (void*)inclusive_clcf->error_log->file);
		
		// types_hash 相关
		inclusive_clcf->types_hash_max_size = 1024;
		inclusive_clcf->types_hash_bucket_size = 64;
		
		// ⚠️ 关键：locations = 0x0 (子 location 没有子节点)
		inclusive_clcf->locations = NULL;
		
		printf("Server: Created inclusive sub-location configuration\n");
		printf("  - inclusive_clcf->name: %.*s (len=%lu)\n", 
			   (int)inclusive_clcf->name.len, inclusive_clcf->name.data, inclusive_clcf->name.len);
		printf("  - inclusive_clcf->root: %s (len=%lu)\n", 
			   inclusive_clcf->root.data, inclusive_clcf->root.len);
		printf("  - inclusive_clcf->static_locations: %p (NULL - leaf node)\n", 
			   (void*)inclusive_clcf->static_locations);
		printf("  - inclusive_clcf->locations: %p (NULL - no children)\n", 
			   (void*)inclusive_clcf->locations);
		
		// ========================================
		// 初始化 static_locations (location 树节点)
		// 根据 GDB 数据：
		// static_locations = {left = 0x0, right = 0x0, tree = 0x5555556aab60, exact = 0x0, 
		//                     inclusive = 0x5555556a3d50, auto_redirect = 0, len = 1, name = "/"}
		// ========================================
		
		// ⚠️ 关键修正：name 字段是柔性数组成员 (u_char name[1])
		// 需要分配额外空间来存储完整的名称字符串
		// 对于 name = "/"，len = 1，需要分配：
		// sizeof(ngx_http_location_tree_node_t) - 1 + len = 基础大小 - 1 + 1 = 基础大小
		// 但为了安全，我们额外分配 len 字节
		size_t location_name_len = 1;  // "/" 的长度
		size_t node_size = sizeof(ngx_http_location_tree_node_t) - 1 + location_name_len;
		
		clcf->static_locations = (ngx_http_location_tree_node_t *)malloc(node_size);
		if (clcf->static_locations != NULL) {
			memset(clcf->static_locations, 0, node_size);
			
			// 根据 GDB 数据初始化各字段
			// left = 0x0, right = 0x0 (左右子树为空)
			clcf->static_locations->left = NULL;
			clcf->static_locations->right = NULL;
			
			// tree = 0x5555556aab60 (指向子树，这里简化为 NULL)
			// 在真实 nginx 中，这里会指向嵌套的 location 树
			// 在 RPC 模式下，我们不需要完整的树结构
			clcf->static_locations->tree = NULL;
			
			// exact = 0x0 (精确匹配的 location 为空)
			clcf->static_locations->exact = NULL;
			
			// ⚠️ 关键修正：inclusive 指向独立的子 location 配置，而不是 clcf 自己
			// inclusive = 0x5555556a3d50 (包含匹配的 location，指向 inclusive_clcf)
			clcf->static_locations->inclusive = inclusive_clcf;
			
			// auto_redirect = 0 (不自动重定向)
			clcf->static_locations->auto_redirect = 0;
			
			// len = 1 (location 名称长度为 1，即 "/")
			clcf->static_locations->len = (u_char)location_name_len;
			
			// ⚠️ 关键修正：name 是柔性数组，需要正确复制字符串
			// name = "/" (location 名称)
			// 由于 name 定义为 u_char name[1]，实际可用空间由 malloc 时分配的额外空间决定
			memcpy(clcf->static_locations->name, "/", location_name_len);
			// 不需要 null 终止符，因为 len 字段已经指定了长度
			
			printf("Server: Initialized clcf->static_locations (from GDB data)\n");
			printf("  - node_size: %lu bytes (base + %lu for name)\n", node_size, location_name_len);
			printf("  - left: %p\n", (void*)clcf->static_locations->left);
			printf("  - right: %p\n", (void*)clcf->static_locations->right);
			printf("  - tree: %p\n", (void*)clcf->static_locations->tree);
			printf("  - exact: %p\n", (void*)clcf->static_locations->exact);
			printf("  - inclusive: %p (points to inclusive_clcf, NOT clcf)\n", 
				   (void*)clcf->static_locations->inclusive);
			printf("  - auto_redirect: %u\n", clcf->static_locations->auto_redirect);
			printf("  - len: %u\n", clcf->static_locations->len);
			printf("  - name: %c\n", clcf->static_locations->name[0]);
		}
		
		// 初始化 types 指针 (指向 types 哈希表的键数组)
		clcf->types = (ngx_array_t *)malloc(sizeof(ngx_array_t));
		if (clcf->types != NULL) {
			memset(clcf->types, 0, sizeof(ngx_array_t));
			clcf->types->elts = NULL;
			clcf->types->nelts = 0;
			clcf->types->size = sizeof(ngx_str_t);
			clcf->types->nalloc = 0;
			clcf->types->pool = r->pool;
		}
		
		// 初始化 resolver (DNS 解析器配置)
		clcf->resolver = (ngx_resolver_t *)malloc(sizeof(ngx_resolver_t));
		if (clcf->resolver != NULL) {
			memset(clcf->resolver, 0, sizeof(ngx_resolver_t));
		}
		
		// 初始化 error_pages (错误页面配置数组)
		clcf->error_pages = (ngx_array_t *)malloc(sizeof(ngx_array_t));
		if (clcf->error_pages != NULL) {
			memset(clcf->error_pages, 0, sizeof(ngx_array_t));
			clcf->error_pages->elts = NULL;
			clcf->error_pages->nelts = 0;
			clcf->error_pages->size = sizeof(void*);
			clcf->error_pages->nalloc = 0;
			clcf->error_pages->pool = r->pool;
		}
		
		// 初始化 client_body_temp_path (客户端请求体临时文件路径配置)
		clcf->client_body_temp_path = (ngx_path_t *)malloc(sizeof(ngx_path_t));
		if (clcf->client_body_temp_path != NULL) {
			memset(clcf->client_body_temp_path, 0, sizeof(ngx_path_t));
			// 设置默认临时路径
			const char *temp_path = "client_body_temp";
			clcf->client_body_temp_path->name.len = strlen(temp_path);
			clcf->client_body_temp_path->name.data = (u_char *)malloc(clcf->client_body_temp_path->name.len + 1);
			if (clcf->client_body_temp_path->name.data != NULL) {
				memcpy(clcf->client_body_temp_path->name.data, temp_path, clcf->client_body_temp_path->name.len);
				clcf->client_body_temp_path->name.data[clcf->client_body_temp_path->name.len] = '\0';
			}
			clcf->client_body_temp_path->level[0] = 0;
			clcf->client_body_temp_path->level[1] = 0;
			clcf->client_body_temp_path->level[2] = 0;
			clcf->client_body_temp_path->len = 0;
		}
		
		// 初始化 locations (子 location 配置队列)
		clcf->locations = (ngx_queue_t *)malloc(sizeof(ngx_queue_t));
		if (clcf->locations != NULL) {
			ngx_queue_init(clcf->locations);
		}
		
		printf("Server: Initialized pointer fields (static_locations, types, resolver, error_pages, etc.)\n");
		printf("  - static_locations = %p\n", (void*)clcf->static_locations);
		printf("  - types = %p\n", (void*)clcf->types);
		printf("  - resolver = %p\n", (void*)clcf->resolver);
		printf("  - error_pages = %p\n", (void*)clcf->error_pages);
		printf("  - client_body_temp_path = %p\n", (void*)clcf->client_body_temp_path);
		printf("  - locations = %p\n", (void*)clcf->locations);
		
		// ⚠️ 关键修复：确保 ngx_http_core_module.ctx_index = 0
		// ngx_http_get_module_loc_conf 宏使用 module.ctx_index 来索引 loc_conf 数组
		// 如果 ctx_index 不正确，会导致返回 NULL 或错误的配置
		extern ngx_module_t ngx_http_core_module;
		ngx_http_core_module.ctx_index = 0;
		
		fprintf(stderr, "\n========== VERSION MARKER: 2025-12-21-15:30 ==========\n");
		fprintf(stderr, "Server: Set ngx_http_core_module.ctx_index = 0\n");
		fflush(stderr);
		printf("  - This ensures ngx_http_get_module_loc_conf returns correct clcf\n");
		
		// 将 clcf 赋值给 loc_conf[0] (ngx_http_core_module 的索引是 0)
		r->loc_conf[0] = clcf;
		
		printf("Server: Initialized ngx_http_core_loc_conf_t (RPC mode: static_locations=NULL)\n");
		printf("  - clcf pointer = %p\n", (void*)clcf);
		printf("  - r->loc_conf[0] = %p\n", (void*)r->loc_conf[0]);
		printf("  - clcf->types_hash.size = %lu\n", clcf->types_hash.size);
		printf("  - clcf->types_hash.buckets = %p\n", (void*)clcf->types_hash.buckets);
		printf("  - &clcf->types_hash = %p\n", (void*)&clcf->types_hash);
		printf("  - clcf->loc_conf points to r->loc_conf array (all module configs)\n");
		printf("  - clcf->error_log will be set after pool initialization\n");
		
		//初始化loc_conf[0]指向ngx_http_rewrite_loc_conf_t以及->codes字段
		// 创建 ngx_http_rewrite_loc_conf_t 结构
		ngx_http_rewrite_loc_conf_t *rlcf = (ngx_http_rewrite_loc_conf_t *)malloc(sizeof(ngx_http_rewrite_loc_conf_t));
		if (rlcf == NULL) {
			printf("Server: Failed to allocate ngx_http_rewrite_loc_conf_t\n");
			free(r->loc_conf);
			free(r);
			return (void *) &result;
		}
		memset(rlcf, 0, sizeof(ngx_http_rewrite_loc_conf_t));
		
		// 初始化 codes 数组 (用于存储重写规则的字节码)
		rlcf->codes = NULL;
		// rlcf->codes = (ngx_array_t *)malloc(sizeof(ngx_array_t));
		// if (rlcf->codes == NULL) {
		// 	printf("Server: Failed to allocate codes array\n");
		// 	free(rlcf);
		// 	free(r->loc_conf);
		// 	free(r);
		// 	return (void *) &result;
		// }
		// memset(rlcf->codes, 0, sizeof(ngx_array_t));
		
		// // 初始化 codes 数组的基本属性
		// // 分配数组元素空间 (每个元素是 uintptr_t)
		// rlcf->codes->elts = malloc(10 * sizeof(uintptr_t));
		// if (rlcf->codes->elts == NULL) {
		// 	printf("Server: Failed to allocate codes->elts\n");
		// 	free(rlcf->codes);
		// 	free(rlcf);
		// 	free(r->loc_conf);
		// 	free(r);
		// 	return (void *) &result;
		// }
		// memset(rlcf->codes->elts, 0, 10 * sizeof(uintptr_t));
		
		// rlcf->codes->nelts = 0;           // 当前元素数量
		// rlcf->codes->size = sizeof(uintptr_t);  // 每个元素的大小
		// rlcf->codes->nalloc = 10;         // 分配的元素容量
		// rlcf->codes->pool = r->pool;      // 关联到请求的内存池
		
		// 初始化其他字段
		rlcf->stack_size = 10;            // 堆栈大小
		rlcf->log = 0;                    // 不记录日志
		rlcf->uninitialized_variable_warn = 1;  // 警告未初始化变量
		rlcf->codes = NULL;               // RPC 模式下不使用重写规则

		// 将 rlcf 赋值给 loc_conf[16] 并同步到 inclusive_clcf
		SYNC_LOC_CONF(r, clcf, 16, rlcf);
		
		printf("Server: Initialized ngx_http_rewrite_loc_conf_t at loc_conf[16] (from GDB data)\n");
		printf("  - codes = %p (NULL - no rewrite rules)\n", rlcf->codes);
		printf("  - stack_size = %lu\n", rlcf->stack_size);
		printf("  - log = %d (no logging)\n", rlcf->log);
		printf("  - uninitialized_variable_warn = %d (warn enabled)\n", rlcf->uninitialized_variable_warn);


		// 初始化 loc_conf[9] 指向 ngx_http_access_loc_conf_t 结构体
		// ngx_http_access_loc_conf_t 用于 IP 地址访问控制（allow/deny 指令）
		ngx_http_access_module.ctx_index = 9;
		
		// 创建 ngx_http_access_loc_conf_t 结构
		ngx_http_access_loc_conf_t *alcf = (ngx_http_access_loc_conf_t *)malloc(sizeof(ngx_http_access_loc_conf_t));
		if (alcf == NULL) {
			printf("Server: Failed to allocate ngx_http_access_loc_conf_t\n");
			free(r->loc_conf);
			free(r);
			return (void *) &result;
		}
		memset(alcf, 0, sizeof(ngx_http_access_loc_conf_t));
		
		// 初始化 rules 字段（IPv4 访问控制规则数组）
		// rules 是一个指向 ngx_array_t 的指针，包含 ngx_http_access_rule_t 类型的规则
		// 在 RPC 模式下，访问控制已在客户端完成，服务器端设置为 NULL
		alcf->rules = NULL;
		
#if (NGX_HAVE_INET6)
		// IPv6 访问控制规则（如果支持 IPv6）
		alcf->rules6 = NULL;
#endif
		
#if (NGX_HAVE_UNIX_DOMAIN)
		// Unix domain socket 访问控制规则（如果支持）
		alcf->rules_un = NULL;
#endif
		
		// 使用模块的实际 ctx_index 并同步到 inclusive_clcf
		ngx_uint_t access_ctx_index = ngx_http_access_module.ctx_index;
		SYNC_LOC_CONF(r, clcf, access_ctx_index, alcf);
		
		printf("Server: Initialized ngx_http_access_loc_conf_t at loc_conf[%lu] (from GDB data)\n", access_ctx_index);
		printf("  - rules = %p (NULL - no IPv4 access rules)\n", alcf->rules);
#if (NGX_HAVE_INET6)
		printf("  - rules6 = %p (NULL - no IPv6 access rules)\n", alcf->rules6);
#endif
#if (NGX_HAVE_UNIX_DOMAIN)
		printf("  - rules_un = %p (NULL - no Unix domain access rules)\n", alcf->rules_un);
#endif
		printf("  - Access control done on client side in RPC mode\n");


		// 初始化 loc_conf[10] 指向 ngx_http_limit_conn_conf_t 结构体
		// ngx_http_limit_conn_conf_t 用于限制并发连接数
		ngx_http_limit_conn_module.ctx_index = 10;
		
		// 创建 ngx_http_limit_conn_conf_t 结构
		ngx_http_limit_conn_conf_t *lccf = (ngx_http_limit_conn_conf_t *)malloc(sizeof(ngx_http_limit_conn_conf_t));
		if (lccf == NULL) {
			printf("Server: Failed to allocate ngx_http_limit_conn_conf_t\n");
			free(r->loc_conf);
			free(r);
			return (void *) &result;
		}
		memset(lccf, 0, sizeof(ngx_http_limit_conn_conf_t));
		
		// 初始化 limits 数组字段（根据 GDB 数据）
		// limits = {elts = 0x0, nelts = 0, size = 0, nalloc = 0, pool = 0x0}
		// limits 是一个 ngx_array_t，包含多个 limit_conn 限制规则
		lccf->limits.elts = NULL;        // 数组元素指针（NULL，表示没有连接限制规则）
		lccf->limits.nelts = 0;          // 当前元素数量为 0
		lccf->limits.size = 0;           // 每个元素的大小（未初始化）
		lccf->limits.nalloc = 0;         // 已分配的容量为 0
		lccf->limits.pool = NULL;        // 未关联到内存池
		
		// 初始化其他字段（根据 GDB 数据）
		// log_level = 4 (NGX_LOG_WARN)
		// status_code = 503
		lccf->log_level = 4;             // NGX_LOG_WARN (警告级别)
		lccf->status_code = 503;         // NGX_HTTP_SERVICE_UNAVAILABLE (503)
		
		// 使用模块的实际 ctx_index 并同步到 inclusive_clcf
		ngx_uint_t limit_conn_ctx_index = ngx_http_limit_conn_module.ctx_index;
		SYNC_LOC_CONF(r, clcf, limit_conn_ctx_index, lccf);
		
		printf("Server: Initialized ngx_http_limit_conn_conf_t at loc_conf[%lu] (from GDB data)\n", limit_conn_ctx_index);
		printf("  - limits.elts = %p, nelts = %lu, size = %lu, nalloc = %lu, pool = %p\n",
			   lccf->limits.elts, lccf->limits.nelts, lccf->limits.size, 
			   lccf->limits.nalloc, lccf->limits.pool);
		printf("  - log_level = %lu (NGX_LOG_WARN)\n", lccf->log_level);
		printf("  - status_code = %lu (503 Service Unavailable)\n", lccf->status_code);


		// 初始化 loc_conf[11] 指向 ngx_http_limit_req_conf_t 结构体
		// ngx_http_limit_req_conf_t 用于限制请求速率（rate limiting）
		// 创建 ngx_http_limit_req_conf_t 结构
		ngx_http_limit_req_conf_t *lrcf = (ngx_http_limit_req_conf_t *)malloc(sizeof(ngx_http_limit_req_conf_t));
		if (lrcf == NULL) {
			printf("Server: Failed to allocate ngx_http_limit_req_conf_t\n");
			free(r->loc_conf);
			free(r);
			return (void *) &result;
		}
		memset(lrcf, 0, sizeof(ngx_http_limit_req_conf_t));
		
		// 初始化 limits 数组字段（根据 GDB 数据）
		// limits = {elts = 0x0, nelts = 0, size = 0, nalloc = 0, pool = 0x0}
		// limits 是一个 ngx_array_t，包含多个 limit_req 限制规则
		lrcf->limits.elts = NULL;        // 数组元素指针（NULL，表示没有限速规则）
		lrcf->limits.nelts = 0;          // 当前元素数量为 0
		lrcf->limits.size = 0;           // 每个元素的大小（未初始化）
		lrcf->limits.nalloc = 0;         // 已分配的容量为 0
		lrcf->limits.pool = NULL;        // 未关联到内存池
		
		// 初始化其他字段（根据 GDB 数据）
		// limit_log_level = 4 (NGX_LOG_WARN)
		// delay_log_level = 5 (NGX_LOG_NOTICE)
		// status_code = 503
		lrcf->limit_log_level = 4;       // NGX_LOG_WARN (警告级别)
		lrcf->delay_log_level = 5;       // NGX_LOG_NOTICE (通知级别)
		lrcf->status_code = 503;         // NGX_HTTP_SERVICE_UNAVAILABLE (503)
		
		// ⚠️ 关键修复:使用模块的实际 ctx_index 而不是硬编码的 11,并同步到 inclusive_clcf
		// ngx_http_get_module_loc_conf 宏使用 module.ctx_index 来索引
		// 我们需要确保配置被放在正确的索引位置
		ngx_http_limit_req_module.ctx_index=11;
		ngx_uint_t limit_req_ctx_index = ngx_http_limit_req_module.ctx_index;
		SYNC_LOC_CONF(r, clcf, limit_req_ctx_index, lrcf);
		
		printf("Server: Initialized ngx_http_limit_req_conf_t at loc_conf[%lu] (from GDB data)\n", limit_req_ctx_index);
		printf("  - limits.elts = %p, nelts = %lu, size = %lu, nalloc = %lu, pool = %p\n",
			   lrcf->limits.elts, lrcf->limits.nelts, lrcf->limits.size, 
			   lrcf->limits.nalloc, lrcf->limits.pool);
		printf("  - limit_log_level = %lu (NGX_LOG_WARN)\n", lrcf->limit_log_level);
		printf("  - delay_log_level = %lu (NGX_LOG_NOTICE)\n", lrcf->delay_log_level);
		printf("  - status_code = %lu (503 Service Unavailable)\n", lrcf->status_code);



		ngx_http_auth_basic_module.ctx_index = 8;
		
		// ===============================================
		// 初始化 loc_conf[8] 指向 ngx_http_auth_basic_loc_conf_t
		// 根据 GDB 数据直接初始化（适用于非 RPC 模式或 RPC 数据缺失时）
		// ===============================================
		ngx_http_auth_basic_loc_conf_t *ablcf = (ngx_http_auth_basic_loc_conf_t *)malloc(sizeof(ngx_http_auth_basic_loc_conf_t));
		if (ablcf == NULL) {
			printf("Server: Failed to allocate ngx_http_auth_basic_loc_conf_t\n");
			free(r->loc_conf);
			free(r);
			return (void *) &result;
		}
		memset(ablcf, 0, sizeof(ngx_http_auth_basic_loc_conf_t));
		
		// 初始化 realm 字段（根据 GDB 数据：realm = 0x5555556a3b18）
		ablcf->realm = (ngx_http_complex_value_t *)malloc(sizeof(ngx_http_complex_value_t));
		if (ablcf->realm != NULL) {
			memset(ablcf->realm, 0, sizeof(ngx_http_complex_value_t));
			// realm 的具体值需要从 GDB 中进一步获取，这里先设置为空
			ablcf->realm->value.len = 0;
			ablcf->realm->value.data = NULL;
			ablcf->realm->flushes = NULL;
			ablcf->realm->lengths = NULL;
			ablcf->realm->values = NULL;
		}
		
		// 初始化 user_file 字段（根据 GDB 数据）
		// value = {len = 106, data = "/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx/output/muti-partition/conf/..//auth/.htpasswd"}
		// const char *htpasswd_path = "/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx/output/muti-partition/conf/..//auth/.htpasswd";
		// ablcf->user_file.value.len = 106;
		// ablcf->user_file.value.data = (u_char *)malloc(106 + 1);
		// if (ablcf->user_file.value.data != NULL) {
		// 	memcpy(ablcf->user_file.value.data, htpasswd_path, 106);
		// 	ablcf->user_file.value.data[106] = '\0';
		// }
		ablcf->user_file.flushes = NULL;
		ablcf->user_file.lengths = NULL;
		ablcf->user_file.values = NULL;
		
		// 将配置赋值到 loc_conf[8] 并同步到 inclusive_clcf
		SYNC_LOC_CONF(r, clcf, 8, ablcf);
		
		printf("Server: Initialized ngx_http_auth_basic_loc_conf_t at loc_conf[8] (from GDB data)\n");
		printf("  - realm = %p\n", (void*)ablcf->realm);
		printf("  - user_file.value.len = %lu\n", ablcf->user_file.value.len);
		printf("  - user_file.value.data = \"%.*s\"\n", 
			   (int)ablcf->user_file.value.len, ablcf->user_file.value.data);
		printf("  - user_file.flushes = %p, lengths = %p, values = %p\n",
			   ablcf->user_file.flushes, ablcf->user_file.lengths, ablcf->user_file.values);
		
		// 处理第8个配置（auth_basic 模块配置）- 如果有 RPC 数据则覆盖上面的初始化
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

			// 将本地配置结构体赋值给 request->loc_conf[8] 并同步到 inclusive_clcf
			SYNC_LOC_CONF(r, clcf, 8, local_auth_conf);
		} else {
			SYNC_LOC_CONF(r, clcf, 8, NULL);
		}
	} else {
		SYNC_LOC_CONF(r, clcf, 8, NULL);
	}		// 初始化 loc_conf[5] 指向 ngx_http_index_loc_conf_t 结构体,赋值indices->elts字段，>indices->nelts字段，max_index_len字段
		ngx_http_index_module.ctx_index = 5;
		
		// 创建 ngx_http_index_loc_conf_t 结构
		ngx_http_index_loc_conf_t *ilcf = (ngx_http_index_loc_conf_t *)malloc(sizeof(ngx_http_index_loc_conf_t));
		if (ilcf == NULL) {
			printf("Server: Failed to allocate ngx_http_index_loc_conf_t\n");
			free(r->loc_conf);
			free(r);
			return (void *) &result;
		}
		memset(ilcf, 0, sizeof(ngx_http_index_loc_conf_t));
		
		// 初始化 indices 字段
		// 创建 ngx_array_t 结构并初始化
		ilcf->indices = (ngx_array_t *)malloc(sizeof(ngx_array_t));
		if (ilcf->indices == NULL) {
			printf("Server: Failed to allocate indices array\n");
			free(ilcf);
			free(r->loc_conf);
			free(r);
			return (void *) &result;
		}
		memset(ilcf->indices, 0, sizeof(ngx_array_t));
		
		// 初始化 indices 数组的基本属性
		// 每个元素是 ngx_http_index_t 类型（包含索引文件名和变量信息）
		ilcf->indices->elts = malloc(2 * sizeof(ngx_http_index_t));
		if (ilcf->indices->elts == NULL) {
			printf("Server: Failed to allocate indices elements\n");
			free(ilcf->indices);
			free(ilcf);
			free(r->loc_conf);
			free(r);
			return (void *) &result;
		}
		memset(ilcf->indices->elts, 0, 2 * sizeof(ngx_http_index_t));
		
		// 获取 ngx_http_index_t 数组指针
		ngx_http_index_t *indices_array = (ngx_http_index_t *)ilcf->indices->elts;
		
		// 初始化第一个索引文件：index.html
		// ⚠️ 重要：name.len 必须包含终止符 '\0'（见 ngx_http_index_module.c:184-186 注释）
		indices_array[0].name.len = 11;  // "index.html" + '\0' = 11 字节
		indices_array[0].name.data = (u_char *)malloc(11);
		if (indices_array[0].name.data == NULL) {
			printf("Server: Failed to allocate index.html name\n");
			free(ilcf->indices->elts);
			free(ilcf->indices);
			free(ilcf);
			free(r->loc_conf);
			free(r);
			return (void *) &result;
		}
		// 使用 strcpy 确保包含 '\0' 终止符
		strcpy((char *)indices_array[0].name.data, "index.html");
		// 验证终止符
		if (indices_array[0].name.data[10] != '\0') {
			printf("Server: ERROR - index.html not null-terminated!\n");
		}
		indices_array[0].lengths = NULL;  // 静态文件名，不需要变量
		indices_array[0].values = NULL;
		
		// 初始化第二个索引文件：50x.html
		// ⚠️ 重要：name.len 必须包含终止符 '\0'
		indices_array[1].name.len = 9;  // "50x.html" + '\0' = 9 字节
		indices_array[1].name.data = (u_char *)malloc(9);
		if (indices_array[1].name.data == NULL) {
			printf("Server: Failed to allocate 50x.html name\n");
			free(indices_array[0].name.data);
			free(ilcf->indices->elts);
			free(ilcf->indices);
			free(ilcf);
			free(r->loc_conf);
			free(r);
			return (void *) &result;
		}
		// 使用 strcpy 确保包含 '\0' 终止符
		strcpy((char *)indices_array[1].name.data, "50x.html");
		// 验证终止符
		if (indices_array[1].name.data[8] != '\0') {
			printf("Server: ERROR - 50x.html not null-terminated!\n");
		}
		indices_array[1].lengths = NULL;  // 静态文件名，不需要变量
		indices_array[1].values = NULL;
		
		ilcf->indices->nelts = 2;            // 当前元素数量（2个索引文件）
		ilcf->indices->size = sizeof(ngx_http_index_t);  // 每个元素的大小
		ilcf->indices->nalloc = 2;           // 已分配的容量（2个）
		ilcf->indices->pool = r->pool;       // 关联到请求的内存池
		
		// 初始化 max_index_len 字段
		// max_index_len 记录所有索引文件名中最长的长度（包含 '\0'）
		// "index.html" = 11 字节（含 '\0'），"50x.html" = 9 字节（含 '\0'），最大为 11
		ilcf->max_index_len = 11;
		
		// 使用模块的实际 ctx_index 并同步到 inclusive_clcf
		ngx_uint_t index_ctx_index = ngx_http_index_module.ctx_index;
		SYNC_LOC_CONF(r, clcf, index_ctx_index, ilcf);
		
		printf("Server: Initialized ngx_http_index_loc_conf_t at loc_conf[%lu] (from GDB data)\n", index_ctx_index);
		printf("  - indices = %p (array of index files)\n", (void*)ilcf->indices);
		printf("  - max_index_len = %zu (longest filename: \"index.html\" = 11 bytes)\n", ilcf->max_index_len);
		printf("  - indices->nelts = %lu (number of index files)\n", ilcf->indices->nelts);
		printf("  - Index files configured:\n");
		printf("    [0] index.html (len=%lu", ((ngx_http_index_t *)ilcf->indices->elts)[0].name.len);
		printf(", data=\"%s\"", ((ngx_http_index_t *)ilcf->indices->elts)[0].name.data);
		printf(", null_term=%s)\n", 
			   ((ngx_http_index_t *)ilcf->indices->elts)[0].name.data[10] == '\0' ? "YES" : "NO");
		printf("    [1] 50x.html (len=%lu", ((ngx_http_index_t *)ilcf->indices->elts)[1].name.len);
		printf(", data=\"%s\"", ((ngx_http_index_t *)ilcf->indices->elts)[1].name.data);
		printf(", null_term=%s)\n", 
			   ((ngx_http_index_t *)ilcf->indices->elts)[1].name.data[8] == '\0' ? "YES" : "NO");

		// 初始化 loc_conf[6] 指向 ngx_http_mirror_loc_conf_t 结构体,赋值mirror字段request_body字段
		ngx_http_mirror_module.ctx_index = 6;
		
		// 创建 ngx_http_mirror_loc_conf_t 结构
		ngx_http_mirror_loc_conf_t *mlcf = (ngx_http_mirror_loc_conf_t *)malloc(sizeof(ngx_http_mirror_loc_conf_t));
		if (mlcf == NULL) {
			printf("Server: Failed to allocate ngx_http_mirror_loc_conf_t\n");
			free(r->loc_conf);
			free(r);
			return (void *) &result;
		}
		memset(mlcf, 0, sizeof(ngx_http_mirror_loc_conf_t));
		
		// 初始化字段（根据 GDB 数据）
		// mirror = 0x0 (NULL)
		// request_body = 1
		
		// mirror 是一个 ngx_array_t 指针，包含镜像请求的目标 URI
		// NULL 表示没有配置 mirror 指令
		mlcf->mirror = NULL;
		
		// request_body 是一个标志位，指示是否镜像请求体
		// 0 = 不镜像请求体，1 = 镜像请求体
		// GDB 数据显示值为 1（启用镜像请求体）
		mlcf->request_body = 1;
		
		// 使用模块的实际 ctx_index 并同步到 inclusive_clcf
		ngx_uint_t mirror_ctx_index = ngx_http_mirror_module.ctx_index;
		SYNC_LOC_CONF(r, clcf, mirror_ctx_index, mlcf);
		
		printf("Server: Initialized ngx_http_mirror_loc_conf_t at loc_conf[%lu] (from GDB data)\n", mirror_ctx_index);
		printf("  - mirror = %p (NULL - no mirror destinations configured)\n", mlcf->mirror);
		printf("  - request_body = %lu (1 = mirror request body if mirrors were configured)\n", mlcf->request_body);


		// 初始化 loc_conf[7] 指向 ngx_http_try_files_loc_conf_t 结构体
		// ngx_http_try_files_loc_conf_t 用于 try_files 指令（按顺序尝试多个文件）
		ngx_http_try_files_module.ctx_index = 7;
		
		// 创建 ngx_http_try_files_loc_conf_t 结构
		ngx_http_try_files_loc_conf_t *tlcf = (ngx_http_try_files_loc_conf_t *)malloc(sizeof(ngx_http_try_files_loc_conf_t));
		if (tlcf == NULL) {
			printf("Server: Failed to allocate ngx_http_try_files_loc_conf_t\n");
			free(r->loc_conf);
			free(r);
			return (void *) &result;
		}
		memset(tlcf, 0, sizeof(ngx_http_try_files_loc_conf_t));
		
		// 初始化字段（根据 GDB 数据）
		// try_files = 0x0 (NULL)
		
		// try_files 是一个 ngx_http_try_file_t 指针，包含要尝试的文件列表
		// NULL 表示没有配置 try_files 指令
		tlcf->try_files = NULL;
		
		// 使用模块的实际 ctx_index 并同步到 inclusive_clcf
		ngx_uint_t try_files_ctx_index = ngx_http_try_files_module.ctx_index;
		SYNC_LOC_CONF(r, clcf, try_files_ctx_index, tlcf);
		
		printf("Server: Initialized ngx_http_try_files_loc_conf_t at loc_conf[%lu] (from GDB data)\n", try_files_ctx_index);
		printf("  - try_files = %p (NULL - no try_files directive configured)\n", tlcf->try_files);

		
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
		r->headers_in.if_none_match = NULL;
		r->headers_in.if_modified_since = NULL;
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
				
				// 初始化必要的字段
				r->pool->chain = NULL;
				r->pool->large = NULL;
				r->pool->cleanup = NULL;
				
				// 创建并初始化 log 字段（非常重要！）
				r->pool->log = (ngx_log_t *)malloc(sizeof(ngx_log_t));
				if (r->pool->log == NULL) {
					free(r->pool);
					r->pool = NULL;
					printf("Server: Failed to allocate pool log\n");
					return &result;
				}
				memset(r->pool->log, 0, sizeof(ngx_log_t));
				// 设置基本的日志级别
				r->pool->log->log_level = NGX_LOG_ERR;
				r->pool->log->file = NULL;
				r->pool->log->handler = NULL;
				r->pool->log->data = NULL;
				r->pool->log->action = NULL;
				r->pool->log->next = NULL;				// 恢复池数据
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
				
				// ⚠️ 重要：pool 创建完成后，立即初始化 clcf->error_log
				// 根据 GDB 数据：
				// error_log = {log_level = 4, file = 0x55555568cbb0, connection = 0, 
				//              disk_full_time = 0, handler = 0x0, data = 0x0, 
				//              writer = 0x0, wdata = 0x0, action = 0x0, next = 0x0}
				if (r->loc_conf && r->loc_conf[0]) {
					ngx_http_core_loc_conf_t *clcf_temp = (ngx_http_core_loc_conf_t *)r->loc_conf[0];
					
					// 分配独立的 error_log 结构
					clcf_temp->error_log = (ngx_log_t *)malloc(sizeof(ngx_log_t));
					if (clcf_temp->error_log == NULL) {
						printf("Server: Failed to allocate clcf->error_log\n");
						return &result;
					}
					memset(clcf_temp->error_log, 0, sizeof(ngx_log_t));
					
					// 根据 GDB 数据初始化各字段
					// log_level = 4 (NGX_LOG_WARN)
					clcf_temp->error_log->log_level = NGX_LOG_WARN;  // 4
					
					// file = 0x55555568cbb0 (指向 ngx_open_file_t 结构)
					// 我们使用 r->pool->log->file，如果存在的话
					// 或者分配新的 file 结构
					if (r->pool->log && r->pool->log->file) {
						clcf_temp->error_log->file = r->pool->log->file;
					} else {
						// 分配 ngx_open_file_t 结构
						clcf_temp->error_log->file = (ngx_open_file_t *)malloc(sizeof(ngx_open_file_t));
						if (clcf_temp->error_log->file != NULL) {
							memset(clcf_temp->error_log->file, 0, sizeof(ngx_open_file_t));
							// 设置默认日志文件（可以从配置读取）
							clcf_temp->error_log->file->fd = ngx_stderr;  // 默认输出到 stderr
							clcf_temp->error_log->file->name.len = 0;
							clcf_temp->error_log->file->name.data = NULL;
						}
					}
					
					// connection = 0 (无连接号)
					clcf_temp->error_log->connection = 0;
					
					// disk_full_time = 0
					clcf_temp->error_log->disk_full_time = 0;
					
					// handler = 0x0, data = 0x0
					clcf_temp->error_log->handler = NULL;
					clcf_temp->error_log->data = NULL;
					
					// writer = 0x0, wdata = 0x0
					clcf_temp->error_log->writer = NULL;
					clcf_temp->error_log->wdata = NULL;
					
					// action = 0x0, next = 0x0
					clcf_temp->error_log->action = NULL;
					clcf_temp->error_log->next = NULL;
					
					printf("Server: Initialized clcf->error_log (from GDB data)\n");
					printf("  - log_level: %u (NGX_LOG_WARN)\n", clcf_temp->error_log->log_level);
					printf("  - file: %p\n", (void*)clcf_temp->error_log->file);
					printf("  - connection: %u\n", clcf_temp->error_log->connection);
					printf("  - disk_full_time: %ld\n", (long)clcf_temp->error_log->disk_full_time);
					printf("  - All other pointers: NULL\n");
				}
				
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

	// 初始化 r->connection 结构（必须在调用 ngx_http_handler 之前）
	r->connection = (ngx_connection_t *)malloc(sizeof(ngx_connection_t));
	if (r->connection == NULL) {
		printf("Server: Failed to allocate connection structure\n");
		// 清理已分配的资源
		if (r->pool && r->pool->log) free(r->pool->log);
		if (r->pool) free(r->pool);
		if (r->loc_conf) free(r->loc_conf);
		free(r);
		return (void *) &result;
	}
	memset(r->connection, 0, sizeof(ngx_connection_t));
	
	// ===============================================
	// 根据 GDB 数据初始化 connection 字段
	// ===============================================
	// data = 0x5555556a4c70 (指向请求对象本身)
	r->connection->data = r;
	
	// fd = 3 (文件描述符)
	r->connection->fd = 3;
	
	// recv/send 函数指针（从 ngx_io 全局变量获取）
	r->connection->recv = ngx_io.recv;           // ngx_unix_recv
	r->connection->send = ngx_io.send;           // ngx_unix_send
	r->connection->recv_chain = ngx_io.recv_chain;  // ngx_readv_chain
	r->connection->send_chain = ngx_io.send_chain;  // ngx_linux_sendfile_chain
	
	// sent = 0 (已发送字节数)
	r->connection->sent = 0;
	
	// pool 字段稍后初始化
	// log 字段稍后初始化
	
	// type = 1 (SOCK_STREAM - TCP连接)
	r->connection->type = 1;
	
	// socklen = 16 (sizeof(struct sockaddr_in))
	r->connection->socklen = 16;
	
	// proxy_protocol_addr = {len = 0, data = 0x0}
	r->connection->proxy_protocol_addr.len = 0;
	r->connection->proxy_protocol_addr.data = NULL;
	r->connection->proxy_protocol_port = 0;
	
	// udp = 0x0 (NULL - 这不是UDP连接)
	r->connection->udp = NULL;
	
	// local_socklen = 16
	r->connection->local_socklen = 16;
	
	// number = 1 (连接编号)
	r->connection->number = 1;
	
	// requests = 1 (该连接处理的请求数)
	r->connection->requests = 1;
	
	// buffered = 0 (无缓冲数据)
	r->connection->buffered = 0;
	
	// log_error = 2 (NGX_ERROR_INFO)
	r->connection->log_error = 2;
	
	// 状态标志位（根据GDB数据）
	r->connection->timedout = 0;      // 未超时
	r->connection->error = 0;         // 无错误
	r->connection->destroyed = 0;     // 未销毁
	r->connection->idle = 0;          // 非空闲
	r->connection->reusable = 0;      // 不可复用
	r->connection->close = 0;         // 不关闭
	r->connection->shared = 0;        // 非共享
	r->connection->sendfile = 1;      // 启用 sendfile
	r->connection->sndlowat = 0;      // SO_SNDLOWAT = 0
	r->connection->tcp_nodelay = 0;   // TCP_NODELAY 未设置
	r->connection->tcp_nopush = 2;    // TCP_NOPUSH 状态
	r->connection->need_last_buf = 0; // 不需要最后一个缓冲区

	// 初始化 r->connection->sockaddr 结构
	// sockaddr 用于存储客户端的网络地址信息
	// 在 RPC 模式下，我们创建一个基本的 IPv4 地址结构
	r->connection->sockaddr = (struct sockaddr *)malloc(sizeof(struct sockaddr_in));
	if (r->connection->sockaddr == NULL) {
		printf("Server: Failed to allocate connection sockaddr\n");
		free(r->connection);
		if (r->pool && r->pool->log) free(r->pool->log);
		if (r->pool) free(r->pool);
		if (r->loc_conf) free(r->loc_conf);
		free(r);
		return (void *) &result;
	}
	memset(r->connection->sockaddr, 0, sizeof(struct sockaddr_in));
	
	// 设置地址族为 IPv4
	struct sockaddr_in *sin = (struct sockaddr_in *)r->connection->sockaddr;
	sin->sin_family = AF_INET;
	sin->sin_addr.s_addr = htonl(INADDR_LOOPBACK);  // 127.0.0.1 (本地回环地址)
	sin->sin_port = htons(0);  // 端口设置为 0（由系统分配）
	
	// 设置 socklen (地址结构的长度) - 已在上面设置
	// r->connection->socklen = 16;
	
	// 初始化 addr_text = {len = 9, data = "127.0.0.1"}
	r->connection->addr_text.len = 9;
	r->connection->addr_text.data = (u_char *)malloc(10);  // 9 + '\0'
	if (r->connection->addr_text.data == NULL) {
		printf("Server: Failed to allocate addr_text\n");
		free(r->connection->sockaddr);
		free(r->connection);
		if (r->pool && r->pool->log) free(r->pool->log);
		if (r->pool) free(r->pool);
		if (r->loc_conf) free(r->loc_conf);
		free(r);
		return (void *) &result;
	}
	memcpy(r->connection->addr_text.data, "127.0.0.1", 10);
	
	// 初始化 local_sockaddr (服务端地址)
	r->connection->local_sockaddr = (struct sockaddr *)malloc(sizeof(struct sockaddr_in));
	if (r->connection->local_sockaddr == NULL) {
		printf("Server: Failed to allocate local_sockaddr\n");
		free(r->connection->addr_text.data);
		free(r->connection->sockaddr);
		free(r->connection);
		if (r->pool && r->pool->log) free(r->pool->log);
		if (r->pool) free(r->pool);
		if (r->loc_conf) free(r->loc_conf);
		free(r);
		return (void *) &result;
	}
	memset(r->connection->local_sockaddr, 0, sizeof(struct sockaddr_in));
	struct sockaddr_in *local_sin = (struct sockaddr_in *)r->connection->local_sockaddr;
	local_sin->sin_family = AF_INET;
	local_sin->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	local_sin->sin_port = htons(80);  // 服务端监听 80 端口
	// local_socklen 已在上面设置为 16
	
	printf("Server: Initialized connection->sockaddr (AF_INET, 127.0.0.1)\n");
	printf("  - addr_text = \"%.*s\" (len=%lu)\n", 
		   (int)r->connection->addr_text.len, 
		   r->connection->addr_text.data,
		   r->connection->addr_text.len);
	printf("  - local_sockaddr initialized (port 80)\n");

	// 初始化 r->connection->read 事件结构
	// read 事件用于处理读取操作和超时管理
	r->connection->read = (ngx_event_t *)malloc(sizeof(ngx_event_t));
	if (r->connection->read == NULL) {
		printf("Server: Failed to allocate connection read event\n");
		if (r->connection->sockaddr) free(r->connection->sockaddr);
		free(r->connection);
		if (r->pool && r->pool->log) free(r->pool->log);
		if (r->pool) free(r->pool);
		if (r->loc_conf) free(r->loc_conf);
		free(r);
		return (void *) &result;
	}
	memset(r->connection->read, 0, sizeof(ngx_event_t));
	
	// 初始化 read 事件的关键字段
	r->connection->read->data = r->connection;  // 回指到 connection
	r->connection->read->log = r->pool ? r->pool->log : NULL;
	r->connection->read->timer_set = 0;  // 定时器未设置
	r->connection->read->ready = 1;      // 标记为就绪（RPC 模式下数据已可用）
	r->connection->read->active = 0;     // 事件未激活
	r->connection->read->delayed = 0;    // 未延迟
	r->connection->read->eof = 0;        // 未到达文件结尾
	r->connection->read->error = 0;      // 无错误
	r->connection->read->timedout = 0;   // 未超时
	r->connection->read->pending_eof = 0; // 无挂起的EOF
	
	// 初始化 r->connection->write 事件结构
	r->connection->write = (ngx_event_t *)malloc(sizeof(ngx_event_t));
	if (r->connection->write == NULL) {
		printf("Server: Failed to allocate connection write event\n");
		free(r->connection->read);
		if (r->connection->sockaddr) free(r->connection->sockaddr);
		free(r->connection);
		if (r->pool && r->pool->log) free(r->pool->log);
		if (r->pool) free(r->pool);
		if (r->loc_conf) free(r->loc_conf);
		free(r);
		return (void *) &result;
	}
	memset(r->connection->write, 0, sizeof(ngx_event_t));
	
	// 初始化 write 事件的关键字段
	r->connection->write->data = r->connection;  // 回指到 connection
	r->connection->write->log = r->pool ? r->pool->log : NULL;
	r->connection->write->timer_set = 0;  // 定时器未设置
	r->connection->write->ready = 1;      // 标记为就绪（RPC 模式下可以写入）
	r->connection->write->active = 0;     // 事件未激活
	r->connection->write->delayed = 0;    // 未延迟
	r->connection->write->error = 0;      // 无错误
	r->connection->write->timedout = 0;   // 未超时
	
	// 初始化 listening 指针 (指向监听socket结构)
	// 在 RPC 模式下，我们创建一个简化的 listening 结构
	r->connection->listening = (ngx_listening_t *)malloc(sizeof(ngx_listening_t));
	if (r->connection->listening == NULL) {
		printf("Server: Failed to allocate listening structure\n");
		free(r->connection->write);
		free(r->connection->read);
		free(r->connection->local_sockaddr);
		free(r->connection->addr_text.data);
		free(r->connection->sockaddr);
		free(r->connection);
		if (r->pool && r->pool->log) free(r->pool->log);
		if (r->pool) free(r->pool);
		if (r->loc_conf) free(r->loc_conf);
		free(r);
		return (void *) &result;
	}
	memset(r->connection->listening, 0, sizeof(ngx_listening_t));
	
	// 初始化 pool 指针 (指向内存池)
	r->connection->pool = r->pool;
	
	// 初始化 log 指针 (指向日志对象)
	if (r->pool && r->pool->log) {
		r->connection->log = r->pool->log;
	}
	
	// 初始化 buffer 指针 (接收缓冲区)
	// buffer 是一个 ngx_buf_t 结构，用于存储接收到的数据
	r->connection->buffer = (ngx_buf_t *)malloc(sizeof(ngx_buf_t));
	if (r->connection->buffer == NULL) {
		printf("Server: Failed to allocate connection buffer\n");
		free(r->connection->listening);
		free(r->connection->write);
		free(r->connection->read);
		free(r->connection->local_sockaddr);
		free(r->connection->addr_text.data);
		free(r->connection->sockaddr);
		free(r->connection);
		if (r->pool && r->pool->log) free(r->pool->log);
		if (r->pool) free(r->pool);
		if (r->loc_conf) free(r->loc_conf);
		free(r);
		return (void *) &result;
	}
	memset(r->connection->buffer, 0, sizeof(ngx_buf_t));
	
	// 分配缓冲区数据空间
	size_t conn_buffer_size = 4096;
	r->connection->buffer->start = (u_char *)malloc(conn_buffer_size);
	if (r->connection->buffer->start == NULL) {
		printf("Server: Failed to allocate connection buffer data\n");
		free(r->connection->buffer);
		free(r->connection->listening);
		free(r->connection->write);
		free(r->connection->read);
		free(r->connection->local_sockaddr);
		free(r->connection->addr_text.data);
		free(r->connection->sockaddr);
		free(r->connection);
		if (r->pool && r->pool->log) free(r->pool->log);
		if (r->pool) free(r->pool);
		if (r->loc_conf) free(r->loc_conf);
		free(r);
		return (void *) &result;
	}
	memset(r->connection->buffer->start, 0, conn_buffer_size);
	r->connection->buffer->pos = r->connection->buffer->start;
	r->connection->buffer->last = r->connection->buffer->start;
	r->connection->buffer->end = r->connection->buffer->start + conn_buffer_size;
	r->connection->buffer->temporary = 1;
	r->connection->buffer->memory = 1;
	
	// 初始化 queue 双向链表 (用于连接队列管理)
	// queue = {prev = 0x55555568c830, next = 0x55555568c830}
	// 在 RPC 模式下，我们让 prev 和 next 都指向自己（单节点链表）
	r->connection->queue.prev = &r->connection->queue;
	r->connection->queue.next = &r->connection->queue;
	
	printf("Server: Initialized connection->read and connection->write events\n");
	printf("  - read->timer_set = %u, read->ready = %u\n", 
		   r->connection->read->timer_set, r->connection->read->ready);
	printf("  - write->timer_set = %u, write->ready = %u\n", 
		   r->connection->write->timer_set, r->connection->write->ready);
	printf("Server: Initialized connection additional fields\n");
	printf("  - listening = %p\n", (void*)r->connection->listening);
	printf("  - pool = %p\n", (void*)r->connection->pool);
	printf("  - log = %p\n", (void*)r->connection->log);
	printf("  - buffer = %p (size=%zu)\n", (void*)r->connection->buffer, conn_buffer_size);
	printf("  - queue = {prev=%p, next=%p}\n", 
		   (void*)r->connection->queue.prev, (void*)r->connection->queue.next);
	printf("  - fd = %d, type = %u, sent = %lu\n", 
		   r->connection->fd, r->connection->type, r->connection->sent);
	printf("  - number = %lu, requests = %lu\n", 
		   r->connection->number, r->connection->requests);
	printf("  - flags: sendfile=%u, tcp_nopush=%u, log_error=%u\n",
		   r->connection->sendfile, r->connection->tcp_nopush, r->connection->log_error);


	// 初始化 r->header_in 缓冲区
	// header_in 是一个 ngx_buf_t 结构，用于存储接收到的 HTTP 请求头
	r->header_in = (ngx_buf_t *)malloc(sizeof(ngx_buf_t));
	if (r->header_in == NULL) {
		printf("Server: Failed to allocate header_in buffer\n");
		free(r->connection->write);
		free(r->connection->read);
		if (r->connection->sockaddr) free(r->connection->sockaddr);
		free(r->connection);
		if (r->pool && r->pool->log) free(r->pool->log);
		if (r->pool) free(r->pool);
		if (r->loc_conf) free(r->loc_conf);
		free(r);
		return (void *) &result;
	}
	memset(r->header_in, 0, sizeof(ngx_buf_t));
	
	// 分配缓冲区数据空间（通常为 1024 字节，足够存储请求头）
	size_t header_buffer_size = 1024;
	r->header_in->start = (u_char *)malloc(header_buffer_size);
	if (r->header_in->start == NULL) {
		printf("Server: Failed to allocate header_in buffer data\n");
		free(r->header_in);
		free(r->connection->write);
		free(r->connection->read);
		if (r->connection->sockaddr) free(r->connection->sockaddr);
		free(r->connection);
		if (r->pool && r->pool->log) free(r->pool->log);
		if (r->pool) free(r->pool);
		if (r->loc_conf) free(r->loc_conf);
		free(r);
		return (void *) &result;
	}
	memset(r->header_in->start, 0, header_buffer_size);
	
	// 初始化缓冲区指针
	// pos: 指向当前读取位置（开始时指向缓冲区起始位置）
	// last: 指向已接收数据的末尾（开始时等于 pos，表示没有数据）
	// end: 指向缓冲区末尾
	r->header_in->pos = r->header_in->start;
	r->header_in->last = r->header_in->start;  // RPC 模式下，请求头已在客户端解析
	r->header_in->end = r->header_in->start + header_buffer_size;
	
	// 初始化其他缓冲区字段
	r->header_in->temporary = 1;  // 临时缓冲区
	r->header_in->memory = 1;     // 内存缓冲区
	r->header_in->mmap = 0;       // 不是 mmap 映射
	r->header_in->recycled = 0;   // 不可回收
	r->header_in->in_file = 0;    // 不在文件中
	r->header_in->flush = 0;      // 不需要刷新
	r->header_in->last_buf = 0;   // 不是最后一个缓冲区
	r->header_in->last_in_chain = 0; // 不是链中最后一个
	
	// 文件相关字段（对于内存缓冲区，这些字段为 NULL/0）
	r->header_in->file = NULL;
	r->header_in->file_pos = 0;
	r->header_in->file_last = 0;
	
	// shadow 和 tag 字段（用于缓冲区管理）
	r->header_in->shadow = NULL;
	r->header_in->tag = NULL;
	
	printf("Server: Initialized r->header_in buffer\n");
	printf("  - buffer size = %zu bytes\n", header_buffer_size);
	printf("  - pos = %p, last = %p, end = %p\n", 
		   r->header_in->pos, r->header_in->last, r->header_in->end);
	printf("  - temporary = %u, memory = %u\n", 
		   r->header_in->temporary, r->header_in->memory);
	
	// 初始化 connection 的 log 字段为 pool 的 log
	if (r->pool && r->pool->log) {
		r->connection->log = r->pool->log;
	} else {
		// 如果 pool 没有 log，创建一个临时的
		r->connection->log = (ngx_log_t *)malloc(sizeof(ngx_log_t));
		if (r->connection->log) {
			memset(r->connection->log, 0, sizeof(ngx_log_t));
			r->connection->log->log_level = NGX_LOG_ERR;
		}
	}

	//请在此处初始化r->main_conf
	// main_conf 是一个指向所有 HTTP 模块主配置的指针数组
	// 在 RPC 服务器环境中，我们需要为其分配空间
	// 注意：实际的 nginx 中，main_conf 数组的大小由 ngx_http_max_module 决定
	// 这里我们简化处理，分配一个固定大小的数组
	r->main_conf = (void **)malloc(32 * sizeof(void *));
	if (r->main_conf == NULL) {
		printf("Server: Failed to allocate main_conf array\n");
		// 清理已分配的资源
		if (r->connection) {
			if (r->connection->log && r->connection->log != r->pool->log) {
				free(r->connection->log);
			}
			free(r->connection);
		}
		if (r->pool && r->pool->log) free(r->pool->log);
		if (r->pool) free(r->pool);
		if (r->loc_conf) free(r->loc_conf);
		free(r);
		return (void *) &result;
	}
	
	// 初始化所有指针为 NULL
	for (int i = 0; i < 32; i++) {
		r->main_conf[i] = NULL;
	}
	
	//初始化main_conf的第一个指针指向ngx_http_core_main_conf_t，并且->phase_engine.handlers指向一个数组，且数组中第一个元素的checker赋值为ngx_http_core_rewrite_phase
	// 创建 ngx_http_core_main_conf_t 结构
	ngx_http_core_main_conf_t *cmcf = (ngx_http_core_main_conf_t *)malloc(sizeof(ngx_http_core_main_conf_t));
	if (cmcf == NULL) {
		printf("Server: Failed to allocate ngx_http_core_main_conf_t\n");
		free(r->main_conf);
		if (r->connection) {
			if (r->connection->log && r->connection->log != r->pool->log) {
				free(r->connection->log);
			}
			free(r->connection);
		}
		if (r->pool && r->pool->log) free(r->pool->log);
		if (r->pool) free(r->pool);
		if (r->loc_conf) free(r->loc_conf);
		free(r);
		return (void *) &result;
	}
	memset(cmcf, 0, sizeof(ngx_http_core_main_conf_t));
	
	// ========================================
	// 根据 GDB 数据初始化 ngx_http_core_main_conf_t 的所有字段
	// ========================================
	
	// 1. 初始化 servers 数组 (存储所有虚拟服务器配置)
	// servers = {elts = 0x55555568f000, nelts = 1, size = 8, nalloc = 4, pool = 0x55555568c740}
	cmcf->servers.elts = malloc(4 * 8);  // nalloc=4, size=8
	if (cmcf->servers.elts != NULL) {
		memset(cmcf->servers.elts, 0, 4 * 8);
	}
	cmcf->servers.nelts = 1;             // 1 个服务器配置
	cmcf->servers.size = 8;              // 每个元素 8 字节（指针大小）
	cmcf->servers.nalloc = 4;            // 分配了 4 个槽位
	cmcf->servers.pool = r->pool;        // 关联到请求池
	
	// 2. 初始化 headers_in_hash (请求头哈希表)
	// headers_in_hash = {buckets = 0x5555556abe00, size = 32}
	cmcf->headers_in_hash.size = 32;
	cmcf->headers_in_hash.buckets = (ngx_hash_elt_t **)malloc(sizeof(ngx_hash_elt_t *) * 32);
	if (cmcf->headers_in_hash.buckets != NULL) {
		memset(cmcf->headers_in_hash.buckets, 0, sizeof(ngx_hash_elt_t *) * 32);
	}
	
	// 3. 初始化 variables_hash (变量哈希表)
	// variables_hash = {buckets = 0x5555556ac960, size = 201}
	cmcf->variables_hash.size = 201;
	cmcf->variables_hash.buckets = (ngx_hash_elt_t **)malloc(sizeof(ngx_hash_elt_t *) * 201);
	if (cmcf->variables_hash.buckets != NULL) {
		memset(cmcf->variables_hash.buckets, 0, sizeof(ngx_hash_elt_t *) * 201);
	}
	
	// 4. 初始化 variables 数组 (所有变量的数组)
	// variables = {elts = 0x5555556ac5c0, nelts = 9, size = 56, nalloc = 16, pool = 0x55555568c740}
	cmcf->variables.elts = malloc(16 * 56);  // nalloc=16, size=56
	if (cmcf->variables.elts != NULL) {
		memset(cmcf->variables.elts, 0, 16 * 56);
	}
	cmcf->variables.nelts = 9;           // 9 个变量
	cmcf->variables.size = 56;           // 每个变量 56 字节
	cmcf->variables.nalloc = 16;         // 分配了 16 个槽位
	cmcf->variables.pool = r->pool;
	
	// 5. 初始化 prefix_variables 数组 (前缀变量)
	// prefix_variables = {elts = 0x555555695b98, nelts = 8, size = 56, nalloc = 8, pool = 0x55555568c740}
	cmcf->prefix_variables.elts = malloc(8 * 56);  // nalloc=8, size=56
	if (cmcf->prefix_variables.elts != NULL) {
		memset(cmcf->prefix_variables.elts, 0, 8 * 56);
	}
	cmcf->prefix_variables.nelts = 8;    // 8 个前缀变量
	cmcf->prefix_variables.size = 56;
	cmcf->prefix_variables.nalloc = 8;
	cmcf->prefix_variables.pool = r->pool;
	
	// 6. 初始化其他标量字段
	cmcf->ncaptures = 0;                 // 正则捕获组数量
	cmcf->server_names_hash_max_size = 512;
	cmcf->server_names_hash_bucket_size = 64;
	cmcf->variables_hash_max_size = 1024;
	cmcf->variables_hash_bucket_size = 64;
	cmcf->variables_keys = NULL;         // 变量键（未使用）
	cmcf->ports = NULL;                  // 监听端口列表（将在后面设置）
	
	// 7. 初始化 phases 数组 (11 个阶段的 handler 数组)
	// 每个阶段都有一个 handlers 数组
	// Phase 0: POST_READ - 无 handlers
	cmcf->phases[NGX_HTTP_POST_READ_PHASE].handlers.elts = malloc(1 * 8);
	if (cmcf->phases[NGX_HTTP_POST_READ_PHASE].handlers.elts != NULL) {
		memset(cmcf->phases[NGX_HTTP_POST_READ_PHASE].handlers.elts, 0, 1 * 8);
	}
	cmcf->phases[NGX_HTTP_POST_READ_PHASE].handlers.nelts = 0;
	cmcf->phases[NGX_HTTP_POST_READ_PHASE].handlers.size = 8;
	cmcf->phases[NGX_HTTP_POST_READ_PHASE].handlers.nalloc = 1;
	cmcf->phases[NGX_HTTP_POST_READ_PHASE].handlers.pool = r->pool;
	
	// Phase 1: SERVER_REWRITE - 1 handler
	cmcf->phases[NGX_HTTP_SERVER_REWRITE_PHASE].handlers.elts = malloc(1 * 8);
	if (cmcf->phases[NGX_HTTP_SERVER_REWRITE_PHASE].handlers.elts != NULL) {
		memset(cmcf->phases[NGX_HTTP_SERVER_REWRITE_PHASE].handlers.elts, 0, 1 * 8);
	}
	cmcf->phases[NGX_HTTP_SERVER_REWRITE_PHASE].handlers.nelts = 1;
	cmcf->phases[NGX_HTTP_SERVER_REWRITE_PHASE].handlers.size = 8;
	cmcf->phases[NGX_HTTP_SERVER_REWRITE_PHASE].handlers.nalloc = 1;
	cmcf->phases[NGX_HTTP_SERVER_REWRITE_PHASE].handlers.pool = r->pool;
	
	// Phase 2: FIND_CONFIG - 不可配置，跳过
	cmcf->phases[NGX_HTTP_FIND_CONFIG_PHASE].handlers.elts = NULL;
	cmcf->phases[NGX_HTTP_FIND_CONFIG_PHASE].handlers.nelts = 0;
	cmcf->phases[NGX_HTTP_FIND_CONFIG_PHASE].handlers.size = 0;
	cmcf->phases[NGX_HTTP_FIND_CONFIG_PHASE].handlers.nalloc = 0;
	cmcf->phases[NGX_HTTP_FIND_CONFIG_PHASE].handlers.pool = NULL;
	
	// Phase 3: REWRITE - 1 handler
	cmcf->phases[NGX_HTTP_REWRITE_PHASE].handlers.elts = malloc(1 * 8);
	if (cmcf->phases[NGX_HTTP_REWRITE_PHASE].handlers.elts != NULL) {
		memset(cmcf->phases[NGX_HTTP_REWRITE_PHASE].handlers.elts, 0, 1 * 8);
	}
	cmcf->phases[NGX_HTTP_REWRITE_PHASE].handlers.nelts = 1;
	cmcf->phases[NGX_HTTP_REWRITE_PHASE].handlers.size = 8;
	cmcf->phases[NGX_HTTP_REWRITE_PHASE].handlers.nalloc = 1;
	cmcf->phases[NGX_HTTP_REWRITE_PHASE].handlers.pool = r->pool;
	
	// Phase 4: POST_REWRITE - 不可配置，跳过
	cmcf->phases[NGX_HTTP_POST_REWRITE_PHASE].handlers.elts = NULL;
	cmcf->phases[NGX_HTTP_POST_REWRITE_PHASE].handlers.nelts = 0;
	cmcf->phases[NGX_HTTP_POST_REWRITE_PHASE].handlers.size = 0;
	cmcf->phases[NGX_HTTP_POST_REWRITE_PHASE].handlers.nalloc = 0;
	cmcf->phases[NGX_HTTP_POST_REWRITE_PHASE].handlers.pool = NULL;
	
	// Phase 5: PREACCESS - 2 handlers (limit_conn, limit_req)
	cmcf->phases[NGX_HTTP_PREACCESS_PHASE].handlers.elts = malloc(2 * 8);
	if (cmcf->phases[NGX_HTTP_PREACCESS_PHASE].handlers.elts != NULL) {
		memset(cmcf->phases[NGX_HTTP_PREACCESS_PHASE].handlers.elts, 0, 2 * 8);
	}
	cmcf->phases[NGX_HTTP_PREACCESS_PHASE].handlers.nelts = 2;
	cmcf->phases[NGX_HTTP_PREACCESS_PHASE].handlers.size = 8;
	cmcf->phases[NGX_HTTP_PREACCESS_PHASE].handlers.nalloc = 2;
	cmcf->phases[NGX_HTTP_PREACCESS_PHASE].handlers.pool = r->pool;
	
	// Phase 6: ACCESS - 2 handlers (access, auth_basic)
	cmcf->phases[NGX_HTTP_ACCESS_PHASE].handlers.elts = malloc(2 * 8);
	if (cmcf->phases[NGX_HTTP_ACCESS_PHASE].handlers.elts != NULL) {
		memset(cmcf->phases[NGX_HTTP_ACCESS_PHASE].handlers.elts, 0, 2 * 8);
	}
	cmcf->phases[NGX_HTTP_ACCESS_PHASE].handlers.nelts = 2;
	cmcf->phases[NGX_HTTP_ACCESS_PHASE].handlers.size = 8;
	cmcf->phases[NGX_HTTP_ACCESS_PHASE].handlers.nalloc = 2;
	cmcf->phases[NGX_HTTP_ACCESS_PHASE].handlers.pool = r->pool;
	
	// Phase 7: POST_ACCESS - 不可配置，跳过
	cmcf->phases[NGX_HTTP_POST_ACCESS_PHASE].handlers.elts = NULL;
	cmcf->phases[NGX_HTTP_POST_ACCESS_PHASE].handlers.nelts = 0;
	cmcf->phases[NGX_HTTP_POST_ACCESS_PHASE].handlers.size = 0;
	cmcf->phases[NGX_HTTP_POST_ACCESS_PHASE].handlers.nalloc = 0;
	cmcf->phases[NGX_HTTP_POST_ACCESS_PHASE].handlers.pool = NULL;
	
	// Phase 8: PRECONTENT - 2 handlers (try_files, mirror)
	cmcf->phases[NGX_HTTP_PRECONTENT_PHASE].handlers.elts = malloc(2 * 8);
	if (cmcf->phases[NGX_HTTP_PRECONTENT_PHASE].handlers.elts != NULL) {
		memset(cmcf->phases[NGX_HTTP_PRECONTENT_PHASE].handlers.elts, 0, 2 * 8);
	}
	cmcf->phases[NGX_HTTP_PRECONTENT_PHASE].handlers.nelts = 2;
	cmcf->phases[NGX_HTTP_PRECONTENT_PHASE].handlers.size = 8;
	cmcf->phases[NGX_HTTP_PRECONTENT_PHASE].handlers.nalloc = 2;
	cmcf->phases[NGX_HTTP_PRECONTENT_PHASE].handlers.pool = r->pool;
	
	// Phase 9: CONTENT - 3 handlers (index, autoindex, static)
	cmcf->phases[NGX_HTTP_CONTENT_PHASE].handlers.elts = malloc(4 * 8);
	if (cmcf->phases[NGX_HTTP_CONTENT_PHASE].handlers.elts != NULL) {
		memset(cmcf->phases[NGX_HTTP_CONTENT_PHASE].handlers.elts, 0, 4 * 8);
	}
	cmcf->phases[NGX_HTTP_CONTENT_PHASE].handlers.nelts = 3;
	cmcf->phases[NGX_HTTP_CONTENT_PHASE].handlers.size = 8;
	cmcf->phases[NGX_HTTP_CONTENT_PHASE].handlers.nalloc = 4;
	cmcf->phases[NGX_HTTP_CONTENT_PHASE].handlers.pool = r->pool;
	
	// Phase 10: LOG - 1 handler
	cmcf->phases[NGX_HTTP_LOG_PHASE].handlers.elts = malloc(1 * 8);
	if (cmcf->phases[NGX_HTTP_LOG_PHASE].handlers.elts != NULL) {
		memset(cmcf->phases[NGX_HTTP_LOG_PHASE].handlers.elts, 0, 1 * 8);
	}
	cmcf->phases[NGX_HTTP_LOG_PHASE].handlers.nelts = 1;
	cmcf->phases[NGX_HTTP_LOG_PHASE].handlers.size = 8;
	cmcf->phases[NGX_HTTP_LOG_PHASE].handlers.nalloc = 1;
	cmcf->phases[NGX_HTTP_LOG_PHASE].handlers.pool = r->pool;
	
	printf("Server: Initialized ngx_http_core_main_conf_t fields (from GDB data)\n");
	printf("  - servers: nelts=%lu, nalloc=%lu\n", cmcf->servers.nelts, cmcf->servers.nalloc);
	printf("  - headers_in_hash.size=%lu\n", cmcf->headers_in_hash.size);
	printf("  - variables_hash.size=%lu\n", cmcf->variables_hash.size);
	printf("  - variables: nelts=%lu, nalloc=%lu\n", cmcf->variables.nelts, cmcf->variables.nalloc);
	printf("  - prefix_variables: nelts=%lu, nalloc=%lu\n", 
		   cmcf->prefix_variables.nelts, cmcf->prefix_variables.nalloc);
	printf("  - server_names_hash_max_size=%lu\n", cmcf->server_names_hash_max_size);
	printf("  - variables_hash_max_size=%lu\n", cmcf->variables_hash_max_size);
	
	// 分配 phase_engine.handlers 数组（根据nginx实际需要，分配14个元素的数组）
	// 典型的nginx HTTP请求处理包含以下阶段：
	// 0: POST_READ (optional)
	// 1: SERVER_REWRITE 
	// 2: FIND_CONFIG
	// 3: REWRITE
	// 4: POST_REWRITE
	// 5-7: PREACCESS (可能多个handler)
	// 8-10: ACCESS (可能多个handler，如limit_conn, limit_req, auth_basic)
	// 11: POST_ACCESS
	// 12: CONTENT (try_files, index, static等)
	// 13: 结束标记 (NULL)
	cmcf->phase_engine.handlers = (ngx_http_phase_handler_t *)malloc(sizeof(ngx_http_phase_handler_t) * 14);
	if (cmcf->phase_engine.handlers == NULL) {
		printf("Server: Failed to allocate phase_engine.handlers\n");
		free(cmcf);
		free(r->main_conf);
		if (r->connection) {
			if (r->connection->log && r->connection->log != r->pool->log) {
				free(r->connection->log);
			}
			free(r->connection);
		}
		if (r->pool && r->pool->log) free(r->pool->log);
		if (r->pool) free(r->pool);
		if (r->loc_conf) free(r->loc_conf);
		free(r);
		return (void *) &result;
	}
	memset(cmcf->phase_engine.handlers, 0, sizeof(ngx_http_phase_handler_t) * 14);
	
	// 初始化各个阶段的 handlers
	// 阶段0: POST_READ_PHASE (跳过，通常为空)
	
	// 阶段1: SERVER_REWRITE_PHASE - 服务器级别的rewrite
	cmcf->phase_engine.handlers[0].checker = ngx_http_core_rewrite_phase;
	cmcf->phase_engine.handlers[0].handler = ngx_http_rewrite_handler;
	cmcf->phase_engine.handlers[0].next = 1;
	
	// 阶段2: FIND_CONFIG_PHASE - 查找location配置
	cmcf->phase_engine.handlers[1].checker = ngx_http_core_find_config_phase;
	cmcf->phase_engine.handlers[1].handler = NULL;  // find_config阶段没有handler
	cmcf->phase_engine.handlers[1].next = 2;
	
	// 阶段3: REWRITE_PHASE - location级别的rewrite
	cmcf->phase_engine.handlers[2].checker = ngx_http_core_rewrite_phase;
	cmcf->phase_engine.handlers[2].handler = ngx_http_rewrite_handler;
	cmcf->phase_engine.handlers[2].next = 3;
	
	// 阶段4: POST_REWRITE_PHASE - rewrite后处理
	cmcf->phase_engine.handlers[3].checker = ngx_http_core_post_rewrite_phase;
	cmcf->phase_engine.handlers[3].handler = NULL;
	cmcf->phase_engine.handlers[3].next = 1;  // 指向find_config，用于rewrite后重新查找location
	
	// 阶段5-7: PREACCESS_PHASE - 预访问阶段（limit_conn, limit_req等）
	cmcf->phase_engine.handlers[4].checker = ngx_http_core_generic_phase;
	cmcf->phase_engine.handlers[4].handler = ngx_http_limit_req_handler;  // 实际应该是limit_conn_handler等
	cmcf->phase_engine.handlers[4].next = 5;
	
	cmcf->phase_engine.handlers[5].checker = ngx_http_core_generic_phase;
	cmcf->phase_engine.handlers[5].handler = ngx_http_limit_conn_handler;  // 实际应该是limit_req_handler等
	cmcf->phase_engine.handlers[5].next = 6;
	
	// 阶段8-10: ACCESS_PHASE - 访问控制阶段
	// ⚠️ RPC 模式：访问控制已在客户端完成，服务器端不需要重新执行
	// ngx_http_access_handler 和 ngx_http_auth_basic_handler 是 static 函数，无法引用
	// checker 负责阶段流程控制，handler 设置为 NULL 跳过实际处理
	cmcf->phase_engine.handlers[6].checker = ngx_http_core_access_phase;
	cmcf->phase_engine.handlers[6].handler = ngx_http_access_handler;  // access_handler (RPC 模式下跳过)
	cmcf->phase_engine.handlers[6].next = 7;
	
	cmcf->phase_engine.handlers[7].checker = ngx_http_core_access_phase;
	cmcf->phase_engine.handlers[7].handler = ngx_http_auth_basic_handler;  // auth_basic_handler (RPC 模式下跳过)
	cmcf->phase_engine.handlers[7].next = 8;
	
	cmcf->phase_engine.handlers[8].checker = ngx_http_core_post_access_phase;
	cmcf->phase_engine.handlers[8].handler = NULL;  // 其他 access handler
	cmcf->phase_engine.handlers[8].next = 9;
	
	// 阶段11: POST_ACCESS_PHASE - 访问控制后处理
	cmcf->phase_engine.handlers[9].checker = ngx_http_core_generic_phase;
	cmcf->phase_engine.handlers[9].handler = ngx_http_try_files_handler;
	cmcf->phase_engine.handlers[9].next = 10;
	
	// 阶段12: PRECONTENT_PHASE - 内容处理前阶段
	cmcf->phase_engine.handlers[10].checker = ngx_http_core_generic_phase;
	cmcf->phase_engine.handlers[10].handler = ngx_http_mirror_handler;  // try_files等
	cmcf->phase_engine.handlers[10].next = 11;
	
	// 阶段13: CONTENT_PHASE - 内容处理阶段
	cmcf->phase_engine.handlers[11].checker = ngx_http_core_content_phase;
	cmcf->phase_engine.handlers[11].handler = ngx_http_index_handler;  // 实际应该是index_handler, static_handler等
	cmcf->phase_engine.handlers[11].next = 12;
	
	// 备用content handler
	cmcf->phase_engine.handlers[12].checker = ngx_http_core_content_phase;
	cmcf->phase_engine.handlers[12].handler = ngx_http_autoindex_handler;
	cmcf->phase_engine.handlers[12].next = 13;
	
	// 最后一个元素作为结束标记（全部为NULL）
	cmcf->phase_engine.handlers[13].checker = ngx_http_core_content_phase;
	cmcf->phase_engine.handlers[13].handler = ngx_http_static_handler;
	cmcf->phase_engine.handlers[13].next = 0;
	
	// 初始化 phase_engine 的其他字段
	// server_rewrite_index 指向 SERVER_REWRITE_PHASE 的起始位置（handlers[0]）
	cmcf->phase_engine.server_rewrite_index = 0;
	// location_rewrite_index 指向 REWRITE_PHASE 的起始位置（handlers[2]）
	cmcf->phase_engine.location_rewrite_index = 2;
	
	// 将 cmcf 赋值给 main_conf[0] (假设 ngx_http_core_module.ctx_index = 0)
	r->main_conf[0] = cmcf;
	
	printf("Server: Initialized ngx_http_core_main_conf_t with phase_engine (14 handlers)\n");
	printf("  - server_rewrite_index: %lu\n", cmcf->phase_engine.server_rewrite_index);
	printf("  - location_rewrite_index: %lu\n", cmcf->phase_engine.location_rewrite_index);
	printf("  - Phase handlers: SERVER_REWRITE -> FIND_CONFIG -> REWRITE -> POST_REWRITE -> ");
	printf("PREACCESS -> ACCESS -> POST_ACCESS -> PRECONTENT -> CONTENT\n");
	
	// 同样初始化 srv_conf（服务器级别配置）
	r->srv_conf = (void **)malloc(32 * sizeof(void *));
	if (r->srv_conf == NULL) {
		printf("Server: Failed to allocate srv_conf array\n");
		free(r->main_conf);
		if (r->connection) {
			if (r->connection->log && r->connection->log != r->pool->log) {
				free(r->connection->log);
			}
			free(r->connection);
		}
		if (r->pool && r->pool->log) free(r->pool->log);
		if (r->pool) free(r->pool);
		if (r->loc_conf) free(r->loc_conf);
		free(r);
		return (void *) &result;
	}
	
	// 初始化所有指针为 NULL
	for (int i = 0; i < 32; i++) {
		r->srv_conf[i] = NULL;
	}

	//初始化srv_conf的第一个指针指向ngx_http_core_srv_conf_t,且初始化字段->ctx->loc_conf
	// 创建 ngx_http_core_srv_conf_t 结构
	ngx_http_core_srv_conf_t *cscf = (ngx_http_core_srv_conf_t *)malloc(sizeof(ngx_http_core_srv_conf_t));
	if (cscf == NULL) {
		printf("Server: Failed to allocate ngx_http_core_srv_conf_t\n");
		free(r->srv_conf);
		free(r->main_conf);
		if (r->connection) {
			if (r->connection->log && r->connection->log != r->pool->log) {
				free(r->connection->log);
			}
			free(r->connection);
		}
		if (r->pool && r->pool->log) free(r->pool->log);
		if (r->pool) free(r->pool);
		if (r->loc_conf) free(r->loc_conf);
		free(r);
		return (void *) &result;
	}
	memset(cscf, 0, sizeof(ngx_http_core_srv_conf_t));
	
	// 创建 ctx (ngx_http_conf_ctx_t) 结构
	cscf->ctx = (ngx_http_conf_ctx_t *)malloc(sizeof(ngx_http_conf_ctx_t));
	if (cscf->ctx == NULL) {
		printf("Server: Failed to allocate ngx_http_conf_ctx_t\n");
		free(cscf);
		free(r->srv_conf);
		free(r->main_conf);
		if (r->connection) {
			if (r->connection->log && r->connection->log != r->pool->log) {
				free(r->connection->log);
			}
			free(r->connection);
		}
		if (r->pool && r->pool->log) free(r->pool->log);
		if (r->pool) free(r->pool);
		if (r->loc_conf) free(r->loc_conf);
		free(r);
		return (void *) &result;
	}
	memset(cscf->ctx, 0, sizeof(ngx_http_conf_ctx_t));
	
	// 初始化 ctx 的三个配置数组指针
	// main_conf 指向 r->main_conf (共享主配置)
	cscf->ctx->main_conf = r->main_conf;
	
	// srv_conf 指向 r->srv_conf (共享服务器配置)
	cscf->ctx->srv_conf = r->srv_conf;
	
	// loc_conf 指向 r->loc_conf (使用请求的位置配置)
	cscf->ctx->loc_conf = r->loc_conf;
	
	// ========================================
	// 根据 GDB 数据初始化 ngx_http_core_srv_conf_t 的所有字段
	// ========================================
	
	// 1. 初始化 server_names 数组 (虚拟服务器名称列表)
	// server_names = {elts = 0x555555690ff8, nelts = 1, size = 32, nalloc = 4, pool = 0x555555690750}
	cscf->server_names.elts = malloc(4 * 32);  // nalloc=4, size=32
	if (cscf->server_names.elts != NULL) {
		memset(cscf->server_names.elts, 0, 4 * 32);
	}
	cscf->server_names.nelts = 1;        // 1 个服务器名称
	cscf->server_names.size = 32;        // 每个元素 32 字节
	cscf->server_names.nalloc = 4;       // 分配了 4 个槽位
	cscf->server_names.pool = r->pool;   // 关联到请求池
	
	// 2. ctx 已经在上面初始化
	
	// 3. file_name - 配置文件路径
	// file_name = 0x55555568caab ".../conf/nginx_test.conf"
	// const char *config_path = "/home/raoxue/Desktop/IF-driver-partition/FIPA/examples/nginx/output/muti-partition/conf/nginx_test.conf";
	// cscf->file_name = (u_char *)malloc(strlen(config_path) + 1);
	// if (cscf->file_name != NULL) {
	// 	strcpy((char *)cscf->file_name, config_path);
	// }
	
	// 4. line - 配置文件行号
	cscf->line = 35;
	
	// 5. server_name - 服务器名称
	// server_name = {len = 9, data = 0x5555556a8642 "localhost/..."}
	const char *server_name_str = "localhost";
	cscf->server_name.len = 9;
	cscf->server_name.data = (u_char *)malloc(strlen(server_name_str) + 1);
	if (cscf->server_name.data != NULL) {
		strcpy((char *)cscf->server_name.data, server_name_str);
	}
	
	// 6. connection_pool_size - 连接池大小
	cscf->connection_pool_size = 512;
	
	// 7. request_pool_size - 请求池大小
	cscf->request_pool_size = 4096;
	
	// 8. client_header_buffer_size - 客户端请求头缓冲区大小
	cscf->client_header_buffer_size = 1024;
	
	// 9. large_client_header_buffers - 大请求头缓冲区配置
	// large_client_header_buffers = {num = 4, size = 8192}
	cscf->large_client_header_buffers.num = 4;
	cscf->large_client_header_buffers.size = 8192;
	
	// 10. client_header_timeout - 客户端请求头超时时间（毫秒）
	cscf->client_header_timeout = 60000;  // 60 秒
	
	// 11. ignore_invalid_headers - 是否忽略无效请求头
	cscf->ignore_invalid_headers = 1;
	
	// 12. merge_slashes - 是否合并多个斜杠
	cscf->merge_slashes = 1;
	
	// 13. underscores_in_headers - 是否允许请求头中有下划线
	cscf->underscores_in_headers = 0;
	
	// 14. listen - 是否配置了监听端口
	cscf->listen = 1;
	
	// 15. captures - 正则捕获组数量
	cscf->captures = 0;
	
	// 16. named_locations - 命名 location 数组
	cscf->named_locations = NULL;
	
	printf("Server: Initialized ngx_http_core_srv_conf_t fields (from GDB data)\n");
	printf("  - server_name: %s (len=%lu)\n", cscf->server_name.data, cscf->server_name.len);
	printf("  - connection_pool_size: %lu\n", cscf->connection_pool_size);
	printf("  - request_pool_size: %lu\n", cscf->request_pool_size);
	printf("  - client_header_buffer_size: %lu\n", cscf->client_header_buffer_size);
	printf("  - large_client_header_buffers: num=%lu, size=%lu\n", 
		   cscf->large_client_header_buffers.num, cscf->large_client_header_buffers.size);
	printf("  - client_header_timeout: %lu ms\n", cscf->client_header_timeout);
	printf("  - file_name: %s (line %u)\n", cscf->file_name, cscf->line);
	
	// 将 cscf 赋值给 srv_conf[0] (假设 ngx_http_core_module.ctx_index = 0)
	r->srv_conf[0] = cscf;

	// 初始化 r->main 字段
	// 在 nginx 中，r->main 指向主请求对象
	// 对于主请求，r->main 指向自己；对于子请求，r->main 指向父请求的 main
	// 在 RPC 模式下，我们处理的都是主请求，所以 r->main = r
	r->main = r;
	
	// 初始化 limit_req_set 标志位
	// limit_req_set 是一个位字段，用于标记是否已经被 limit_req 模块处理过
	// 这个字段通过 r->main->limit_req_set 访问（见 ngx_http_limit_req_module.c:227）
	// 在 nginx 正常流程中，这个字段在请求创建时初始化为 0
	// 在 limit_req handler 中会被设置为 1，防止重复处理
	r->limit_req_set = 0;
	
	// 初始化 count 字段（引用计数）
	// count 用于跟踪请求的引用次数，防止过早释放
	// 主请求初始值为 1，子请求创建时会增加父请求的 count
	r->count = 1;

	r->access_code=0;
	r->method=2;
	r->headers_in.content_length_n=-1;
	r->err_status=0;
	r->http_version=1001;

	// ========================================
	// 根据 GDB 数据初始化所有 ngx_http_request_t 字段
	// ========================================
	
	// 1. 基本字段（已从 RPC 恢复或已初始化）
	// r->signature = 1347703880;  // 已初始化
	// r->connection = ...;  // 已初始化
	// r->ctx = ...;  // 将在后面初始化
	// r->main_conf, srv_conf, loc_conf = ...;  // 已初始化
	
	// 2. 事件处理函数
	r->read_event_handler = ngx_http_block_reading;
	r->write_event_handler = NULL;
	
	// 3. cache 和 upstream 相关（RPC 模式下为 NULL）
	r->cache = NULL;
	r->upstream = NULL;
	r->upstream_states = NULL;
	
	// 4. pool 和 header_in 已初始化
	
	// 5. 初始化 headers_in 字段（输入请求头）
	// 初始化 headers_in.headers 链表（根据 GDB 数据）
	// headers = {last = 0x5555556a4ce0, part = {elts = 0x55555569afb0, nelts = 4, next = 0x0}, 
	//            size = 48, nalloc = 20, pool = 0x5555556a4c20}
	
	r->headers_in.headers.part.elts = malloc(20 * 48);  // nalloc=20, size=48
	if (r->headers_in.headers.part.elts == NULL) {
		printf("Server: Failed to allocate headers_in.headers.part.elts\n");
		return (void *) &result;
	}
	memset(r->headers_in.headers.part.elts, 0, 20 * 48);
	
	r->headers_in.headers.part.nelts = 4;      // GDB 显示有 4 个元素（Host, User-Agent, Accept, Authorization）
	r->headers_in.headers.part.next = NULL;
	
	r->headers_in.headers.last = &r->headers_in.headers.part;
	r->headers_in.headers.size = 48;
	r->headers_in.headers.nalloc = 20;
	r->headers_in.headers.pool = r->pool;
	
	printf("Server: Initialized r->headers_in.headers\n");
	printf("  - headers.part.nelts = %lu (Host, User-Agent, Accept, Authorization)\n", 
		   r->headers_in.headers.part.nelts);
	
	// ========================================
	// 根据 GDB 数据初始化 headers_in 的各个字段
	// ========================================
	
	// 现在初始化具体的 header 指针
	// 这些指针将指向 headers.part.elts 数组中的元素
	// 根据 GDB 数据：
	// - host = 0x55555569afb0 (指向 elts[0])
	// - user_agent = 0x55555569b010 (指向 elts[1])
	// - authorization = 0x55555569afe0 (指向 elts[3])
	
	ngx_table_elt_t *headers_array = (ngx_table_elt_t *)r->headers_in.headers.part.elts;
	
	// Host header (elts[0])
	r->headers_in.host = &headers_array[0];
	
	// Connection header (GDB 显示为 NULL)
	r->headers_in.connection = NULL;
	
	// 条件请求相关的 headers (GDB 显示都为 NULL)
	r->headers_in.if_modified_since = NULL;
	r->headers_in.if_unmodified_since = NULL;
	r->headers_in.if_match = NULL;
	r->headers_in.if_none_match = NULL;
	
	// User-Agent header (elts[1])
	r->headers_in.user_agent = &headers_array[1];
	
	// Referer header (GDB 显示为 NULL)
	r->headers_in.referer = NULL;
	
	// Content 相关的 headers (GDB 显示都为 NULL)
	r->headers_in.content_length = NULL;
	r->headers_in.content_range = NULL;
	r->headers_in.content_type = NULL;
	
	// Range 相关的 headers (GDB 显示都为 NULL)
	r->headers_in.range = NULL;
	r->headers_in.if_range = NULL;
	
	// Transfer-Encoding 相关 (GDB 显示都为 NULL)
	r->headers_in.transfer_encoding = NULL;
	r->headers_in.te = NULL;
	r->headers_in.expect = NULL;
	r->headers_in.upgrade = NULL;
	
	// Accept-Encoding (GDB 显示为 NULL)
	r->headers_in.accept_encoding = NULL;
	
	// Via header (GDB 显示为 NULL)
	r->headers_in.via = NULL;
	
	// Authorization header (elts[3])
	// r->headers_in.authorization = &headers_array[3];
	
	// Keep-Alive header (GDB 显示为 NULL)
	r->headers_in.keep_alive = NULL;
	
	// x_forwarded_for 数组 (GDB 显示为空)
	// x_forwarded_for = {elts = 0x0, nelts = 0, size = 0, nalloc = 0, pool = 0x0}
	r->headers_in.x_forwarded_for.elts = NULL;
	r->headers_in.x_forwarded_for.nelts = 0;
	r->headers_in.x_forwarded_for.size = 0;
	r->headers_in.x_forwarded_for.nalloc = 0;
	r->headers_in.x_forwarded_for.pool = NULL;
	
	// user 和 passwd (GDB 显示为空)
	// user = {len = 0, data = 0x0}
	// passwd = {len = 0, data = 0x0}
	r->headers_in.user.len = 0;
	r->headers_in.user.data = NULL;
	r->headers_in.passwd.len = 0;
	r->headers_in.passwd.data = NULL;
	
	// cookies 数组 (GDB 显示为空)
	// cookies = {elts = 0x0, nelts = 0, size = 0, nalloc = 0, pool = 0x0}
	r->headers_in.cookies.elts = NULL;
	r->headers_in.cookies.nelts = 0;
	r->headers_in.cookies.size = 0;
	r->headers_in.cookies.nalloc = 0;
	r->headers_in.cookies.pool = NULL;
	
	// server 字段（从 GDB 数据：len = 9, data = "localhost:8081"）
	// server = {len = 9, data = 0x55555568a646 "localhost:8081"}
	// 注意：GDB 显示 len=9，但实际字符串是 "localhost" (9个字符)
	const char *server_str = "localhost";
	r->headers_in.server.len = 9;
	r->headers_in.server.data = (u_char *)malloc(strlen(server_str) + 1);
	if (r->headers_in.server.data != NULL) {
		strcpy((char *)r->headers_in.server.data, server_str);
	}
	
	// content_length_n (GDB 显示为 -1，已在前面初始化)
	// content_length_n = -1
	
	// keep_alive_n (GDB 显示为 -1)
	// keep_alive_n = -1
	r->headers_in.keep_alive_n = -1;
	
	// connection_type (GDB 显示为 0)
	// connection_type = 0
	r->headers_in.connection_type = 0;
	
	// chunked (GDB 显示为 0)
	// chunked = 0
	r->headers_in.chunked = 0;
	
	// 浏览器类型标志位（从 GDB 数据都为 0）
	// msie = 0, msie6 = 0, opera = 0, gecko = 0, chrome = 0, safari = 0, konqueror = 0
	r->headers_in.msie = 0;
	r->headers_in.msie6 = 0;
	r->headers_in.opera = 0;
	r->headers_in.gecko = 0;
	r->headers_in.chrome = 0;
	r->headers_in.safari = 0;
	r->headers_in.konqueror = 0;
	
	printf("Server: Initialized r->headers_in fields (from GDB data)\n");
	printf("  - host: %p (points to headers[0])\n", (void *)r->headers_in.host);
	printf("  - user_agent: %p (points to headers[1])\n", (void *)r->headers_in.user_agent);
	printf("  - authorization: %p (points to headers[3])\n", (void *)r->headers_in.authorization);
	printf("  - server: %s (len=%lu)\n", r->headers_in.server.data, r->headers_in.server.len);
	printf("  - content_length_n: %ld\n", r->headers_in.content_length_n);
	printf("  - keep_alive_n: %ld\n", r->headers_in.keep_alive_n);
	printf("  - connection_type: %u\n", r->headers_in.connection_type);
	printf("  - chunked: %u\n", r->headers_in.chunked);
	
	// ========================================
	// 6. 根据 GDB 数据初始化 headers_out 字段（输出响应头）
	// ========================================
	// headers_out.headers 和 trailers 将在后面单独初始化
	
	// status 和 status_line (GDB 显示都为 0/NULL)
	// status = 0
	// status_line = {len = 0, data = 0x0}
	r->headers_out.status = 0;
	r->headers_out.status_line.len = 0;
	r->headers_out.status_line.data = NULL;
	
	// 所有 header 指针 (GDB 显示都为 NULL)
	// server = 0x0, date = 0x0, content_length = 0x0
	r->headers_out.server = NULL;
	r->headers_out.date = NULL;
	r->headers_out.content_length = NULL;
	
	// content_encoding = 0x0, location = 0x0, refresh = 0x0
	r->headers_out.content_encoding = NULL;
	r->headers_out.location = NULL;
	r->headers_out.refresh = NULL;
	
	// last_modified = 0x0, content_range = 0x0, accept_ranges = 0x0
	r->headers_out.last_modified = NULL;
	r->headers_out.content_range = NULL;
	r->headers_out.accept_ranges = NULL;
	
	// www_authenticate = 0x0, expires = 0x0, etag = 0x0
	r->headers_out.www_authenticate = NULL;
	r->headers_out.expires = NULL;
	r->headers_out.etag = NULL;
	
	// override_charset = 0x0
	r->headers_out.override_charset = NULL;
	
	// content_type 相关字段 (GDB 显示都为 0/NULL)
	// content_type_len = 0
	// content_type = {len = 0, data = 0x0}
	// charset = {len = 0, data = 0x0}
	// content_type_lowcase = 0x0
	// content_type_hash = 0
	r->headers_out.content_type_len = 0;
	r->headers_out.content_type.len = 0;
	r->headers_out.content_type.data = NULL;
	r->headers_out.charset.len = 0;
	r->headers_out.charset.data = NULL;
	r->headers_out.content_type_lowcase = NULL;
	r->headers_out.content_type_hash = 0;
	
	// cache_control 数组 (GDB 显示为空)
	// cache_control = {elts = 0x0, nelts = 0, size = 0, nalloc = 0, pool = 0x0}
	r->headers_out.cache_control.elts = NULL;
	r->headers_out.cache_control.nelts = 0;
	r->headers_out.cache_control.size = 0;
	r->headers_out.cache_control.nalloc = 0;
	r->headers_out.cache_control.pool = NULL;
	
	// link 数组 (GDB 显示为空)
	// link = {elts = 0x0, nelts = 0, size = 0, nalloc = 0, pool = 0x0}
	r->headers_out.link.elts = NULL;
	r->headers_out.link.nelts = 0;
	r->headers_out.link.size = 0;
	r->headers_out.link.nalloc = 0;
	r->headers_out.link.pool = NULL;
	
	// 数值字段 (从 GDB 数据)
	// content_length_n = -1
	// content_offset = 0
	// date_time = 0
	// last_modified_time = -1
	r->headers_out.content_length_n = -1;
	r->headers_out.content_offset = 0;
	r->headers_out.date_time = 0;
	r->headers_out.last_modified_time = -1;
	
	printf("Server: Initialized r->headers_out fields (from GDB data)\n");
	printf("  - status: %u\n", r->headers_out.status);
	printf("  - content_length_n: %ld\n", r->headers_out.content_length_n);
	printf("  - content_offset: %ld\n", (long)r->headers_out.content_offset);
	printf("  - date_time: %ld\n", (long)r->headers_out.date_time);
	printf("  - last_modified_time: %ld\n", (long)r->headers_out.last_modified_time);
	printf("  - All header pointers: NULL\n");
	printf("  - cache_control.nelts: %lu\n", r->headers_out.cache_control.nelts);
	printf("  - link.nelts: %lu\n", r->headers_out.link.nelts);
	
	// 7. request_body（RPC 模式下为 NULL）
	r->request_body = NULL;
	
	// 8. 时间相关字段（从 GDB 数据）
	r->lingering_time = 0;
	r->start_sec = 1766298734;
	r->start_msec = 142;
	
	// 9. HTTP 请求行和 URI 字段
	// method 已初始化为 2 (GET)
	// http_version 已初始化为 1001 (HTTP/1.1)
	
	// request_line（从 GDB 数据：len = 14, data = "GET / HTTP/1.1\r\nHost"）
	r->request_line.len = 14;
	r->request_line.data = (u_char *)malloc(20 + 1);
	if (r->request_line.data != NULL) {
		memcpy(r->request_line.data, "GET / HTTP/1.1\r\nHost", 20);
		r->request_line.data[20] = '\0';
	}
	
	// uri 已从 RPC 恢复（len = 1, data = "/"）
	
	// args（查询参数，从 GDB 数据为空）
	r->args.len = 0;
	r->args.data = NULL;
	
	// exten（文件扩展名，从 GDB 数据为空）
	r->exten.len = 0;
	r->exten.data = NULL;
	
	// unparsed_uri（原始 URI，从 GDB 数据：len = 1, data = "/"）
	r->unparsed_uri.len = 1;
	r->unparsed_uri.data = (u_char *)malloc(1 + 1);
	if (r->unparsed_uri.data != NULL) {
		r->unparsed_uri.data[0] = '/';
		r->unparsed_uri.data[1] = '\0';
	}
	
	// method_name（从 GDB 数据：len = 3, data = "GET"）
	r->method_name.len = 3;
	r->method_name.data = (u_char *)malloc(3 + 1);
	if (r->method_name.data != NULL) {
		memcpy(r->method_name.data, "GET", 3);
		r->method_name.data[3] = '\0';
	}
	
	// http_protocol（从 GDB 数据：len = 8, data = "HTTP/1.1"）
	r->http_protocol.len = 8;
	r->http_protocol.data = (u_char *)malloc(8 + 1);
	if (r->http_protocol.data != NULL) {
		memcpy(r->http_protocol.data, "HTTP/1.1", 8);
		r->http_protocol.data[8] = '\0';
	}
	
	// schema（协议，从 GDB 数据为空）
	r->schema.len = 0;
	r->schema.data = NULL;
	
	// 10. 子请求和输出链相关字段
	// ===============================================
	// 根据 GDB 数据初始化 r->out 输出链
	// r->out 是一个 ngx_chain_t 链表，包含响应数据
	// ===============================================
	
	// 创建第一个链节点 (响应头缓冲区)
	ngx_chain_t *out_first = (ngx_chain_t *)malloc(sizeof(ngx_chain_t));
	if (out_first == NULL) {
		printf("Server: Failed to allocate first out chain node\n");
		return (void *) &result;
	}
	memset(out_first, 0, sizeof(ngx_chain_t));
	
	// 创建第一个缓冲区 (包含HTTP响应头)
	ngx_buf_t *header_buf = (ngx_buf_t *)malloc(sizeof(ngx_buf_t));
	if (header_buf == NULL) {
		printf("Server: Failed to allocate header buffer\n");
		free(out_first);
		return (void *) &result;
	}
	memset(header_buf, 0, sizeof(ngx_buf_t));
	
	// 响应头内容 (从GDB数据)
	const char *response_header = 
		"HTTP/1.1 200 OK\r\n"
		"Server: nginx/1.15.5\r\n"
		"Date: Sun, 21 Dec 2025 18:31:09 GMT\r\n"
		"Content-Type: text/html\r\n"
		"Content-Length: 18\r\n"
		"Last-Modified: Wed, 24 Sep 2025 17:45:14 GMT\r\n"
		"Connection: keep-alive\r\n"
		"ETag: \"68\"\r\n"
		"Accept-Ranges: bytes\r\n"
		"\r\n";
	
	size_t header_len = strlen(response_header);
	
	// 分配并复制响应头数据
	header_buf->start = (u_char *)malloc(header_len + 16);  // 额外空间
	if (header_buf->start == NULL) {
		printf("Server: Failed to allocate header buffer data\n");
		free(header_buf);
		free(out_first);
		return (void *) &result;
	}
	memcpy(header_buf->start, response_header, header_len);
	
	// 初始化缓冲区指针 (根据GDB数据)
	header_buf->pos = header_buf->start;        // 当前读取位置
	header_buf->last = header_buf->start + header_len;  // 数据末尾
	header_buf->end = header_buf->start + header_len + 16;  // 缓冲区末尾
	
	// 文件相关字段 (根据GDB: file_pos=0, file_last=0, file=0x0)
	header_buf->file_pos = 0;
	header_buf->file_last = 0;
	header_buf->file = NULL;
	
	// 缓冲区标志位 (根据GDB: temporary=1, memory=0, mmap=0...)
	header_buf->temporary = 1;   // 临时缓冲区
	header_buf->memory = 0;      // 不是内存缓冲区标志
	header_buf->mmap = 0;        // 不是mmap映射
	header_buf->recycled = 0;    // 不可回收
	header_buf->in_file = 0;     // 不在文件中
	header_buf->flush = 0;       // 不需要刷新
	header_buf->sync = 0;        // 不需要同步
	header_buf->last_buf = 0;    // 不是最后一个缓冲区
	header_buf->last_in_chain = 0;  // 不是链中最后一个
	header_buf->last_shadow = 0;    // 不是影子缓冲区最后一个
	header_buf->temp_file = 0;      // 不是临时文件
	header_buf->num = 0;
	
	// 其他字段 (根据GDB: tag=0x0, shadow=0x0)
	header_buf->tag = NULL;
	header_buf->shadow = NULL;
	
	// 将缓冲区关联到链节点
	out_first->buf = header_buf;
	
	// 创建第二个链节点 (响应体缓冲区)
	ngx_chain_t *out_second = (ngx_chain_t *)malloc(sizeof(ngx_chain_t));
	if (out_second == NULL) {
		printf("Server: Failed to allocate second out chain node\n");
		free(header_buf->start);
		free(header_buf);
		free(out_first);
		return (void *) &result;
	}
	memset(out_second, 0, sizeof(ngx_chain_t));
	
	// 创建第二个缓冲区 (包含响应体数据)
	ngx_buf_t *body_buf = (ngx_buf_t *)malloc(sizeof(ngx_buf_t));
	if (body_buf == NULL) {
		printf("Server: Failed to allocate body buffer\n");
		free(out_second);
		free(header_buf->start);
		free(header_buf);
		free(out_first);
		return (void *) &result;
	}
	memset(body_buf, 0, sizeof(ngx_buf_t));
	
	// 第二个缓冲区通常是文件缓冲区或包含实际内容
	// 暂时创建一个简单的内存缓冲区
	const char *response_body = "Welcome to nginx!";  // 示例响应体
	size_t body_len = strlen(response_body);
	
	body_buf->start = (u_char *)malloc(body_len + 1);
	if (body_buf->start == NULL) {
		printf("Server: Failed to allocate body buffer data\n");
		free(body_buf);
		free(out_second);
		free(header_buf->start);
		free(header_buf);
		free(out_first);
		return (void *) &result;
	}
	memcpy(body_buf->start, response_body, body_len);
	body_buf->start[body_len] = '\0';
	
	body_buf->pos = body_buf->start;
	body_buf->last = body_buf->start + body_len;
	body_buf->end = body_buf->start + body_len + 1;
	body_buf->file_pos = 0;
	body_buf->file_last = 0;
	body_buf->file = NULL;
	body_buf->temporary = 1;
	body_buf->memory = 1;
	body_buf->mmap = 0;
	body_buf->recycled = 0;
	body_buf->in_file = 0;
	body_buf->flush = 0;
	body_buf->sync = 0;
	body_buf->last_buf = 1;      // 这是最后一个缓冲区
	body_buf->last_in_chain = 1; // 这是链中最后一个
	body_buf->last_shadow = 0;
	body_buf->temp_file = 0;
	body_buf->num = 0;
	body_buf->tag = NULL;
	body_buf->shadow = NULL;
	
	// 将缓冲区关联到第二个链节点
	out_second->buf = body_buf;
	out_second->next = NULL;  // 这是链表末尾
	
	// 连接两个链节点
	out_first->next = NULL;
	
	// 设置 r->out 指向链表头
	r->out = out_first;
	r->out=NULL;
	
	// printf("Server: Initialized r->out output chain (2 nodes)\n");
	// printf("  - out[0]: buf=%p, header_len=%zu, next=%p\n", 
	// 	   (void*)out_first->buf, header_len, (void*)out_first->next);
	// printf("    - pos=%p, last=%p, end=%p\n",
	// 	   (void*)header_buf->pos, (void*)header_buf->last, (void*)header_buf->end);
	// printf("    - temporary=%u, memory=%u, last_buf=%u\n",
	// 	   header_buf->temporary, header_buf->memory, header_buf->last_buf);
	// printf("  - out[1]: buf=%p, body_len=%zu, next=%p\n",
	// 	   (void*)out_second->buf, body_len, (void*)out_second->next);
	// printf("    - last_buf=%u, last_in_chain=%u\n",
	// 	   body_buf->last_buf, body_buf->last_in_chain);
	
	// r->main 已初始化为 r
	r->parent = NULL;
	r->postponed = NULL;
	r->post_subrequest = NULL;
	r->posted_requests = NULL;
	
	// 11. 请求处理阶段和变量
	r->phase_handler = 0;
	r->content_handler = NULL;
	// r->access_code 已初始化为 0
	r->variables = NULL;  // 将在后面初始化
	r->ncaptures = 0;
	r->captures = NULL;
	r->captures_data = NULL;
	
	// 12. 限流和速率控制
	r->limit_rate = 0;
	r->limit_rate_after = 0;
	
	// 13. 请求统计字段
	r->header_size = 0;
	r->request_length = 129;
	// r->err_status 已初始化为 0
	
	// 14. http_connection 和 stream
	r->http_connection = NULL;  // 将根据实际情况设置
	r->stream = NULL;
	
	// 15. 日志处理函数
	// 注意：ngx_http_log_error_handler 在 ngx_http_request.c 中定义为 static
	// 在 RPC 模式下，我们将其设置为 NULL，或者使用 pool->log 的默认 handler
	r->log_handler = NULL;
	
	// 16. cleanup 和引用计数
	r->cleanup = NULL;
	// r->count 已初始化为 1
	
	// 17. 子请求限制
	r->subrequests = 51;
	r->blocked = 0;
	r->aio = 0;
	
	// 18. HTTP 状态和标志位
	r->http_state = 2;
	r->complex_uri = 0;
	r->quoted_uri = 0;
	r->plus_in_uri = 0;
	r->space_in_uri = 0;
	r->invalid_header = 0;
	r->add_uri_to_alias = 0;
	r->valid_location = 0;
	r->valid_unparsed_uri = 1;
	r->uri_changed = 0;
	r->uri_changes = 11;
	
	// 19. request_body 相关标志位
	r->request_body_in_single_buf = 0;
	r->request_body_in_file_only = 0;
	r->request_body_in_persistent_file = 0;
	r->request_body_in_clean_file = 0;
	r->request_body_file_group_access = 0;
	r->request_body_file_log_level = 0;
	r->request_body_no_buffering = 0;
	
	// 20. 其他标志位
	r->subrequest_in_memory = 0;
	r->waited = 0;
	r->cached = 0;
	r->gzip_tested = 0;
	r->gzip_ok = 0;
	r->gzip_vary = 0;
	r->proxy = 0;
	r->bypass_cache = 0;
	r->no_cache = 0;
	r->limit_conn_set = 0;
	// r->limit_req_set 已初始化为 0
	r->pipeline = 0;
	r->chunked = 0;
	r->header_only = 0;
	r->expect_trailers = 0;
	r->keepalive = 0;
	r->lingering_close = 0;
	r->discard_body = 0;
	r->reading_body = 0;
	r->internal = 0;
	r->error_page = 0;
	r->filter_finalize = 0;
	r->post_action = 0;
	r->request_complete = 0;
	r->request_output = 0;
	r->header_sent = 0;
	r->expect_tested = 0;
	r->root_tested = 0;
	r->done = 0;
	r->logged = 0;
	r->buffered = 0;
	r->main_filter_need_in_memory = 0;
	r->filter_need_in_memory = 0;
	r->filter_need_temporary = 0;
	r->preserve_body = 0;
	r->allow_ranges = 0;
	r->subrequest_ranges = 0;
	r->single_range = 0;
	r->disable_not_modified = 0;
	r->stat_reading = 0;
	r->stat_writing = 0;
	r->stat_processing = 0;
	r->background = 0;
	r->health_check = 0;
	
	// 21. HTTP 请求解析状态（从 GDB 数据）
	r->state = 0;
	r->header_hash = 2871506184;
	r->lowcase_index = 6;
	
	// lowcase_header（从 GDB 数据："acceptgention" + 18个'\0'）
	memcpy(r->lowcase_header, "acceptgention", 13);
	memset(r->lowcase_header + 13, 0, 19);
	
	// 22. 请求解析指针（从 GDB 数据）
	// 这些指针指向 header_in 缓冲区中的位置
	// 在 RPC 模式下，我们需要模拟这些指针
	r->header_name_start = NULL;
	r->header_name_end = NULL;
	r->header_start = NULL;
	r->header_end = NULL;
	r->uri_start = NULL;
	r->uri_end = NULL;
	r->uri_ext = NULL;
	r->args_start = NULL;
	r->request_start = NULL;
	r->request_end = NULL;
	r->method_end = NULL;
	r->schema_start = NULL;
	r->schema_end = NULL;
	r->host_start = NULL;
	r->host_end = NULL;
	r->port_start = NULL;
	r->port_end = NULL;
	
	// 23. HTTP 协议版本
	r->http_minor = 1;
	r->http_major = 1;

	
	



	ngx_http_top_header_filter=ngx_http_not_modified_header_filter;
	ngx_http_next_header_filter = ngx_http_headers_filter;

	r->headers_out.server=NULL;
	r->headers_out.date=NULL;
	r->headers_out.content_length=NULL;

	// ===============================================
	// 初始化 r->headers_out.headers 链表
	// ===============================================
	// headers_out.headers 是一个 ngx_list_t 结构，用于存储HTTP响应头
	// 根据提供的结构体信息初始化：
	// headers = {last = 0x..., part = {elts = 0x..., nelts = 0, next = 0x0}, 
	//            size = 48, nalloc = 20, pool = 0x...}
	
	// 初始化 headers_out.headers.part (第一个链表节点)
	r->headers_out.headers.part.elts = malloc(20 * 48);  // nalloc=20, size=48
	if (r->headers_out.headers.part.elts == NULL) {
		return (void *) &result;
	}
	memset(r->headers_out.headers.part.elts, 0, 20 * 48);
	
	r->headers_out.headers.part.nelts = 0;     // 初始元素数量为0
	r->headers_out.headers.part.next = NULL;   // 没有下一个part
	
	// 初始化 headers_out.headers 的其他字段
	r->headers_out.headers.last = &r->headers_out.headers.part;  // last指向第一个part
	r->headers_out.headers.size = 48;          // 每个header元素的大小 (sizeof(ngx_table_elt_t))
	r->headers_out.headers.nalloc = 20;        // 每个part可容纳20个元素
	r->headers_out.headers.pool = r->pool;     // 关联到请求的内存池
	
	

	// ===============================================
	// 初始化 r->headers_out.trailers 链表
	// ===============================================
	// trailers 是一个 ngx_list_t 结构，用于存储 HTTP trailer 头（chunk 编码时使用）
	// 根据 GDB 数据初始化：
	// trailers = {last = 0x5555556a4e90, part = {elts = 0x5555556a55e0, nelts = 0, next = 0x0}, 
	//             size = 48, nalloc = 4, pool = 0x5555556a4c20}
	
	// 初始化 headers_out.trailers.part (第一个链表节点)
	r->headers_out.trailers.part.elts = malloc(4 * 48);  // nalloc=4, size=48
	if (r->headers_out.trailers.part.elts == NULL) {
		printf("Server: Failed to allocate headers_out.trailers.part.elts\n");
		return (void *) &result;
	}
	memset(r->headers_out.trailers.part.elts, 0, 4 * 48);
	
	r->headers_out.trailers.part.nelts = 0;     // 初始元素数量为0
	r->headers_out.trailers.part.next = NULL;   // 没有下一个part
	
	// 初始化 headers_out.trailers 的其他字段
	r->headers_out.trailers.last = &r->headers_out.trailers.part;  // last指向第一个part
	r->headers_out.trailers.size = 48;          // 每个trailer元素的大小 (sizeof(ngx_table_elt_t))
	r->headers_out.trailers.nalloc = 4;         // 每个part可容纳4个元素
	r->headers_out.trailers.pool = r->pool;     // 关联到请求的内存池
	
	;

	// ===============================================
	// 初始化 r->ctx 数组及 r->ctx[41]
	// ===============================================
	//初始化r->ctx,并初始化r->ctx[41]指向类型ngx_http_range_filter_ctx_t的结构体
	
	// ctx 是一个指针数组，每个模块可以在 ctx 中存储自己的上下文数据
	// 数组大小应该足够容纳所有 HTTP 模块（通常是 64 个）
	r->ctx = (void **)malloc(64 * sizeof(void *));
	if (r->ctx == NULL) {
		printf("Server: Failed to allocate r->ctx array\n");
		return (void *) &result;
	}
	
	// 初始化所有指针为 NULL
	for (int i = 0; i < 64; i++) {
		r->ctx[i] = NULL;
	}
	

	// r->ctx[40]指向ngx_output_chain_ctx_t结构体
	ngx_http_copy_filter_module.ctx_index = 40;
	ngx_output_chain_ctx_t *occtx = NULL;
	r->ctx[40] = occtx;
	
	// 设置 ngx_http_range_body_filter_module 的 ctx_index
	ngx_http_range_body_filter_module.ctx_index = 41;
	
	// 初始化 ngx_http_range_filter_ctx_t 结构体
	// 该模块用于处理 HTTP Range 请求（部分内容请求）
	typedef struct {
		off_t        offset;             // 当前偏移量
		ngx_str_t    boundary_header;    // 多部分范围的边界头
		ngx_array_t  ranges;             // 范围数组 (ngx_http_range_t)
	} ngx_http_range_filter_ctx_t;
	
	ngx_http_range_filter_ctx_t *range_ctx = NULL;
	
	r->ctx[41] = range_ctx;


int ngx_http_auth_basic_handler_sense_1_ret;

ngx_int_t sensefun_ret;
sensefun_ret=ngx_http_auth_basic_handler_sense_1(ngx_http_auth_basic_handler_sense_1_ret,r);

result.ret0=sensefun_ret;
    return &result;
}

