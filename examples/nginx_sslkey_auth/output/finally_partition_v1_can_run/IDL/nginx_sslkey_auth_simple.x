typedef int int_ptr<>;
struct char_ptr{opaque charptr<>;};
typedef struct char_ptr char_ptr;
struct ngx_str_t_rpc {
 int len;
char_ptr data;
};

struct ngx_open_file_t_rpc{
int fd;
ngx_str_t_rpc name;
char_ptr flush;
/*opaque data<>;*/
};
typedef struct ngx_open_file_t_rpc ngx_open_file_t_rpc;
typedef ngx_open_file_t_rpc ngx_open_file_t_rpc_ptr<>;
struct ngx_log_t_rpc{
unsigned long int log_level;
ngx_open_file_t_rpc_ptr file;
unsigned long connection;
int disk_full_time;
char_ptr handler;
/*opaque data<>;*/
char_ptr writer;
/*opaque wdata<>;*/
char_ptr action;
struct  ngx_log_t_rpc * next;
};
typedef struct ngx_log_t_rpc ngx_log_t_rpc;
typedef ngx_log_t_rpc ngx_log_t_rpc_ptr<>;
struct ngx_rbtree_node_t_rpc {
unsigned long int key;
struct  ngx_rbtree_node_t_rpc * left;
struct  ngx_rbtree_node_t_rpc * right;
struct  ngx_rbtree_node_t_rpc * parent;
int color;
int data;
};

struct ngx_queue_t_rpc {
struct  ngx_queue_t_rpc * prev;
struct  ngx_queue_t_rpc * next;
};

struct ngx_event_t_rpc{
/*opaque data<>;*/
unsigned write;
unsigned accept;
unsigned instance;
unsigned active;
unsigned disabled;
unsigned ready;
unsigned oneshot;
unsigned complete;
unsigned eof;
unsigned error;
unsigned timedout;
unsigned timer_set;
unsigned delayed;
unsigned deferred_accept;
unsigned pending_eof;
unsigned posted;
unsigned closed;
unsigned channel;
unsigned resolver;
unsigned cancelable;
unsigned available;
char_ptr handler;
unsigned long int index;
ngx_log_t_rpc_ptr log;
ngx_rbtree_node_t_rpc timer;
ngx_queue_t_rpc queue;
};
typedef struct ngx_event_t_rpc ngx_event_t_rpc;
typedef ngx_event_t_rpc ngx_event_t_rpc_ptr<>;
struct sockaddr_rpc {
int sa_family;
char sa_data[14];
};

typedef sockaddr_rpc sockaddr_rpc_ptr<>;

typedef struct sockaddr_rpc_ptr sockaddr_rpc_ptr;

struct ngx_rbtree_t_rpc {
char_ptr insert;
};

struct ngx_listening_t_rpc{
int fd;
sockaddr_rpc sockaddr;
int socklen;
 int addr_text_max_len;
ngx_str_t_rpc addr_text;
int type;
int backlog;
int rcvbuf;
int sndbuf;
int keepidle;
int keepintvl;
int keepcnt;
char_ptr handler;
/*opaque servers<>;*/
ngx_log_t_rpc log;
ngx_log_t_rpc_ptr logp;
 int pool_size;
 int post_accept_buffer_size;
unsigned long int post_accept_timeout;
struct  ngx_listening_t_rpc * previous;
ngx_rbtree_t_rpc rbtree;
ngx_rbtree_node_t_rpc sentinel;
unsigned long int worker;
unsigned open;
unsigned remain;
unsigned ignore;
unsigned bound;
unsigned inherited;
unsigned nonblocking_accept;
unsigned listen;
unsigned nonblocking;
unsigned shared;
unsigned addr_ntop;
unsigned wildcard;
unsigned ipv6only;
unsigned reuseport;
unsigned add_reuseport;
unsigned keepalive;
unsigned deferred_accept;
unsigned delete_deferred;
unsigned add_deferred;
int fastopen;
};
typedef struct ngx_listening_t_rpc ngx_listening_t_rpc;
typedef ngx_listening_t_rpc ngx_listening_t_rpc_ptr<>;

struct ngx_file_info_t_rpc {
/*opaque tempname<>;*/
char_ptr tempname;
};

struct ngx_file_t_rpc{
int fd;
ngx_str_t_rpc name;
ngx_file_info_t_rpc info;
int offset;
int sys_offset;
ngx_log_t_rpc_ptr log;
unsigned valid_info;
unsigned directio;
};
typedef struct ngx_file_t_rpc ngx_file_t_rpc;
typedef ngx_file_t_rpc ngx_file_t_rpc_ptr<>;
struct ngx_buf_t_rpc{
char_ptr pos;
char_ptr last;
int file_pos;
int file_last;
char_ptr start;
char_ptr end;
/*opaque tag<>;*/
ngx_file_t_rpc_ptr file;
struct  ngx_buf_t_rpc * shadow;
unsigned temporary;
unsigned memory;
unsigned mmap;
unsigned recycled;
unsigned in_file;
unsigned flush;
unsigned sync;
unsigned last_buf;
unsigned last_in_chain;
unsigned last_shadow;
unsigned temp_file;
int num;
};
typedef struct ngx_buf_t_rpc ngx_buf_t_rpc;
typedef ngx_buf_t_rpc ngx_buf_t_rpc_ptr<>;
struct ngx_chain_t_rpc{
ngx_buf_t_rpc_ptr buf;
struct  ngx_chain_t_rpc * next;
};
typedef struct ngx_chain_t_rpc ngx_chain_t_rpc;
typedef ngx_chain_t_rpc ngx_chain_t_rpc_ptr<>;
struct ngx_pool_large_t_rpc{
struct  ngx_pool_large_t_rpc * next;
/*opaque alloc<>;*/
};
typedef struct ngx_pool_large_t_rpc ngx_pool_large_t_rpc;
typedef ngx_pool_large_t_rpc ngx_pool_large_t_rpc_ptr<>;
struct ngx_pool_cleanup_t_rpc{
char_ptr handler;
/*opaque data<>;*/
struct  ngx_pool_cleanup_t_rpc * next;
};
typedef struct ngx_pool_cleanup_t_rpc ngx_pool_cleanup_t_rpc;
typedef ngx_pool_cleanup_t_rpc ngx_pool_cleanup_t_rpc_ptr<>;


struct ngx_udp_connection_t_rpc{
ngx_rbtree_node_t_rpc node;
ngx_buf_t_rpc_ptr buffer;
};
typedef struct ngx_udp_connection_t_rpc ngx_udp_connection_t_rpc;
typedef ngx_udp_connection_t_rpc ngx_udp_connection_t_rpc_ptr<>;
struct ngx_connection_t_rpc{
/*opaque data<>;*/
ngx_event_t_rpc_ptr read;
ngx_event_t_rpc_ptr write;
int fd;
char_ptr recv;
char_ptr send;
char_ptr recv_chain;
char_ptr send_chain;
ngx_listening_t_rpc_ptr listening;
int sent;
ngx_log_t_rpc_ptr log;
int type;
sockaddr_rpc sockaddr;
int socklen;
ngx_str_t_rpc addr_text;
ngx_str_t_rpc proxy_protocol_addr;
uint16_t proxy_protocol_port;
ngx_udp_connection_t_rpc_ptr udp;
sockaddr_rpc local_sockaddr;
int local_socklen;
ngx_buf_t_rpc_ptr buffer;
ngx_queue_t_rpc queue;
unsigned long number;
unsigned long int requests;
unsigned buffered;
unsigned log_error;
unsigned timedout;
unsigned error;
unsigned destroyed;
unsigned idle;
unsigned reusable;
unsigned close;
unsigned shared;
unsigned sendfile;
unsigned sndlowat;
unsigned tcp_nodelay;
unsigned tcp_nopush;
unsigned need_last_buf;
};
typedef struct ngx_connection_t_rpc ngx_connection_t_rpc;
typedef ngx_connection_t_rpc ngx_connection_t_rpc_ptr<>;
struct TwoCharPtr{char_ptr twocharptr<>;};
typedef struct TwoCharPtr TwoCharPtr;
struct ngx_array_t_rpc {
/*opaque elts<>;*/
unsigned long int nelts;
 int size;
unsigned long int nalloc;
};

struct ngx_http_file_cache_sh_t_rpc{
ngx_rbtree_t_rpc rbtree;
ngx_rbtree_node_t_rpc sentinel;
ngx_queue_t_rpc queue;
unsigned long int cold;
unsigned long int loading;
int size;
unsigned long int count;
unsigned long int watermark;
};
typedef struct ngx_http_file_cache_sh_t_rpc ngx_http_file_cache_sh_t_rpc;
typedef ngx_http_file_cache_sh_t_rpc ngx_http_file_cache_sh_t_rpc_ptr<>;
struct ngx_shmtx_sh_t_rpc {
unsigned long int lock;
unsigned long int wait;
};

struct ngx_slab_page_t_rpc{
int slab;
struct  ngx_slab_page_t_rpc * next;
int prev;
};
typedef struct ngx_slab_page_t_rpc ngx_slab_page_t_rpc;
typedef ngx_slab_page_t_rpc ngx_slab_page_t_rpc_ptr<>;
struct ngx_shmtx_t_rpc {
char_ptr lock;
char_ptr wait;
unsigned long int semaphore;
int sem;
unsigned long int spin;
};

struct ngx_slab_pool_t_rpc{
ngx_shmtx_sh_t_rpc lock;
 int min_size;
 int min_shift;
ngx_slab_page_t_rpc_ptr pages;
ngx_slab_page_t_rpc_ptr last;
ngx_slab_page_t_rpc free;
unsigned long int pfree;
char_ptr start;
char_ptr end;
ngx_shmtx_t_rpc mutex;
char_ptr log_ctx;
int zero;
unsigned log_nomem;
/*opaque data<>;*/
/*opaque addr<>;*/
};
typedef struct ngx_slab_pool_t_rpc ngx_slab_pool_t_rpc;
typedef ngx_slab_pool_t_rpc ngx_slab_pool_t_rpc_ptr<>;
struct ngx_path_t_rpc{
ngx_str_t_rpc name;
 int len;
char_ptr manager;
char_ptr purger;
char_ptr loader;
/*opaque data<>;*/
char_ptr conf_file;
unsigned long int line;
};
typedef struct ngx_path_t_rpc ngx_path_t_rpc;
typedef ngx_path_t_rpc ngx_path_t_rpc_ptr<>;
struct ngx_shm_t_rpc {
char_ptr addr;
 int size;
ngx_str_t_rpc name;
ngx_log_t_rpc_ptr log;
unsigned long int exists;
};

struct ngx_shm_zone_t_rpc{
/*opaque data<>;*/
ngx_shm_t_rpc shm;
char_ptr init;
/*opaque tag<>;*/
/*opaque sync<>;*/
unsigned long int noreuse;
};
typedef struct ngx_shm_zone_t_rpc ngx_shm_zone_t_rpc;
typedef ngx_shm_zone_t_rpc ngx_shm_zone_t_rpc_ptr<>;
struct ngx_http_file_cache_t_rpc{
ngx_http_file_cache_sh_t_rpc_ptr sh;
ngx_slab_pool_t_rpc_ptr shpool;
ngx_path_t_rpc_ptr path;
int max_size;
 int bsize;
int inactive;
int fail_time;
unsigned long int files;
unsigned long int loader_files;
unsigned long int last;
unsigned long int loader_sleep;
unsigned long int loader_threshold;
unsigned long int manager_files;
unsigned long int manager_sleep;
unsigned long int manager_threshold;
ngx_shm_zone_t_rpc_ptr shm_zone;
unsigned long int use_temp_path;
};
typedef struct ngx_http_file_cache_t_rpc ngx_http_file_cache_t_rpc;
typedef ngx_http_file_cache_t_rpc ngx_http_file_cache_t_rpc_ptr<>;
struct ngx_http_file_cache_node_t_rpc{
ngx_rbtree_node_t_rpc node;
ngx_queue_t_rpc queue;
unsigned count;
unsigned uses;
unsigned valid_msec;
unsigned error;
unsigned exists;
unsigned updating;
unsigned deleting;
unsigned purged;
int expire;
int valid_sec;
 int body_start;
int fs_size;
unsigned long int lock_time;
};
typedef struct ngx_http_file_cache_node_t_rpc ngx_http_file_cache_node_t_rpc;
typedef ngx_http_file_cache_node_t_rpc ngx_http_file_cache_node_t_rpc_ptr<>;
struct ngx_http_cache_t_rpc{
ngx_file_t_rpc file;
ngx_array_t_rpc keys;
int crc32;
int valid_sec;
int updating_sec;
int error_sec;
int last_modified;
int date;
ngx_str_t_rpc etag;
ngx_str_t_rpc vary;
 int header_start;
 int body_start;
int length;
int fs_size;
unsigned long int min_uses;
unsigned long int error;
unsigned long int valid_msec;
unsigned long int vary_tag;
ngx_buf_t_rpc_ptr buf;
ngx_http_file_cache_t_rpc_ptr file_cache;
ngx_http_file_cache_node_t_rpc_ptr node;
unsigned long int lock_timeout;
unsigned long int lock_age;
unsigned long int lock_time;
unsigned long int wait_time;
ngx_event_t_rpc wait_event;
unsigned lock;
unsigned waiting;
unsigned updated;
unsigned updating;
unsigned exists;
unsigned temp_file;
unsigned purged;
unsigned reading;
unsigned secondary;
unsigned background;
unsigned stale_updating;
unsigned stale_error;
};
typedef struct ngx_http_cache_t_rpc ngx_http_cache_t_rpc;
typedef ngx_http_cache_t_rpc ngx_http_cache_t_rpc_ptr<>;
struct ngx_addr_t_rpc{
sockaddr_rpc sockaddr;
int socklen;
ngx_str_t_rpc name;
};
typedef struct ngx_addr_t_rpc ngx_addr_t_rpc;
typedef ngx_addr_t_rpc ngx_addr_t_rpc_ptr<>;
struct ngx_peer_connection_t_rpc {
ngx_connection_t_rpc_ptr connection;
sockaddr_rpc sockaddr;
int socklen;
unsigned long int tries;
unsigned long int start_time;
char_ptr get;
char_ptr free;
char_ptr notify;
/*opaque data<>;*/
ngx_addr_t_rpc_ptr local;
int type;
int rcvbuf;
ngx_log_t_rpc_ptr log;
unsigned cached;
unsigned transparent;
unsigned log_error;
};

struct ngx_bufs_t_rpc {
long int num;
 int size;
};

struct ngx_temp_file_t_rpc{
ngx_file_t_rpc file;
int offset;
struct  ngx_path_t_rpc * path;
char_ptr warn;
unsigned long int access;
unsigned log_level;
unsigned persistent;
unsigned clean;
unsigned thread_write;
};
typedef struct ngx_temp_file_t_rpc ngx_temp_file_t_rpc;
typedef ngx_temp_file_t_rpc ngx_temp_file_t_rpc_ptr<>;
struct ngx_event_pipe_t_rpc{
ngx_connection_t_rpc_ptr upstream;
ngx_connection_t_rpc_ptr downstream;
ngx_chain_t_rpc_ptr free_raw_bufs;
ngx_chain_t_rpc_ptr in;
ngx_chain_t_rpc_ptr writing;
ngx_chain_t_rpc_ptr out;
ngx_chain_t_rpc_ptr free;
ngx_chain_t_rpc_ptr busy;
char_ptr input_filter;
/*opaque input_ctx<>;*/
char_ptr output_filter;
/*opaque output_ctx<>;*/
unsigned read;
unsigned cacheable;
unsigned single_buf;
unsigned free_bufs;
unsigned upstream_done;
unsigned upstream_error;
unsigned upstream_eof;
unsigned upstream_blocked;
unsigned downstream_done;
unsigned downstream_error;
unsigned cyclic_temp_file;
unsigned aio;
long int allocated;
ngx_bufs_t_rpc bufs;
/*opaque tag<>;*/
int busy_size;
int read_length;
int length;
int max_temp_file_size;
int temp_file_write_size;
unsigned long int read_timeout;
unsigned long int send_timeout;
int send_lowat;
ngx_log_t_rpc_ptr log;
ngx_chain_t_rpc_ptr preread_bufs;
 int preread_size;
ngx_buf_t_rpc_ptr buf_to_file;
 int limit_rate;
int start_sec;
ngx_temp_file_t_rpc_ptr temp_file;
int num;
};
typedef struct ngx_event_pipe_t_rpc ngx_event_pipe_t_rpc;
typedef ngx_event_pipe_t_rpc ngx_event_pipe_t_rpc_ptr<>;
struct ngx_output_chain_ctx_t_rpc {
ngx_buf_t_rpc_ptr buf;
ngx_chain_t_rpc_ptr in;
ngx_chain_t_rpc_ptr free;
ngx_chain_t_rpc_ptr busy;
unsigned sendfile;
unsigned directio;
unsigned unaligned;
unsigned need_in_memory;
unsigned need_in_temp;
unsigned aio;
int alignment;
long int allocated;
ngx_bufs_t_rpc bufs;
/*opaque tag<>;*/
char_ptr output_filter;
/*opaque filter_ctx<>;*/
};

struct ngx_chain_writer_ctx_t_rpc {
ngx_chain_t_rpc_ptr out;
ngx_connection_t_rpc_ptr connection;
int limit;
};

struct ngx_http_upstream_peer_t_rpc {
char_ptr init_upstream;
char_ptr init;
/*opaque data<>;*/
};

struct ngx_http_upstream_srv_conf_t_rpc{
ngx_http_upstream_peer_t_rpc peer;
TwoCharPtr srv_conf;
unsigned long int flags;
ngx_str_t_rpc host;
char_ptr file_name;
unsigned long int line;
uint16_t port;
unsigned long int no_port;
ngx_shm_zone_t_rpc_ptr shm_zone;
};
typedef struct ngx_http_upstream_srv_conf_t_rpc ngx_http_upstream_srv_conf_t_rpc;
typedef ngx_http_upstream_srv_conf_t_rpc ngx_http_upstream_srv_conf_t_rpc_ptr<>;
struct ngx_hash_elt_t_rpc {
/*opaque value<>;*/
int len;
};

typedef ngx_hash_elt_t_rpc ngx_hash_elt_t_rpc_ptr<>;

typedef struct ngx_hash_elt_t_rpc_ptr ngx_hash_elt_t_rpc_ptr;

typedef struct ngx_hash_elt_t_rpc_ptr ngx_hash_elt_t_rpc_twoptr<>;

struct ngx_hash_t_rpc {
ngx_hash_elt_t_rpc_twoptr buckets;
unsigned long int size;
};

struct ngx_http_upstream_conf_t_rpc{
ngx_http_upstream_srv_conf_t_rpc_ptr upstream;
unsigned long int connect_timeout;
unsigned long int send_timeout;
unsigned long int read_timeout;
unsigned long int next_upstream_timeout;
 int send_lowat;
 int buffer_size;
 int limit_rate;
 int busy_buffers_size;
 int max_temp_file_size;
 int temp_file_write_size;
 int busy_buffers_size_conf;
 int max_temp_file_size_conf;
 int temp_file_write_size_conf;
ngx_bufs_t_rpc bufs;
unsigned long int ignore_headers;
unsigned long int next_upstream;
unsigned long int store_access;
unsigned long int next_upstream_tries;
long int buffering;
long int request_buffering;
long int pass_request_headers;
long int pass_request_body;
long int ignore_client_abort;
long int intercept_errors;
long int cyclic_temp_file;
long int force_ranges;
struct  ngx_path_t_rpc * temp_path;
ngx_hash_t_rpc hide_headers_hash;
struct  ngx_array_t_rpc * hide_headers;
struct  ngx_array_t_rpc * pass_headers;
ngx_shm_zone_t_rpc_ptr cache_zone;
struct  ngx_http_complex_value_t_rpc * cache_value;
unsigned long int cache_min_uses;
unsigned long int cache_use_stale;
unsigned long int cache_methods;
int cache_max_range_offset;
long int cache_lock;
unsigned long int cache_lock_timeout;
unsigned long int cache_lock_age;
long int cache_revalidate;
long int cache_convert_head;
long int cache_background_update;
struct  ngx_array_t_rpc * cache_valid;
struct  ngx_array_t_rpc * cache_bypass;
struct  ngx_array_t_rpc * cache_purge;
struct  ngx_array_t_rpc * no_cache;
struct  ngx_array_t_rpc * store_lengths;
struct  ngx_array_t_rpc * store_values;
unsigned intercept_404;
unsigned change_buffering;
unsigned pass_trailers;
unsigned preserve_output;
ngx_str_t_rpc module;
};
typedef struct ngx_http_upstream_conf_t_rpc ngx_http_upstream_conf_t_rpc;
typedef ngx_http_upstream_conf_t_rpc ngx_http_upstream_conf_t_rpc_ptr<>;
struct ngx_list_part_t_rpc{
/*opaque elts<>;*/
unsigned long int nelts;
struct  ngx_list_part_t_rpc * next;
};
typedef struct ngx_list_part_t_rpc ngx_list_part_t_rpc;
typedef ngx_list_part_t_rpc ngx_list_part_t_rpc_ptr<>;
struct ngx_list_t_rpc {
ngx_list_part_t_rpc_ptr last;
ngx_list_part_t_rpc part;
 int size;
unsigned long int nalloc;
};

struct ngx_http_upstream_headers_in_t_rpc {
ngx_list_t_rpc headers;
ngx_list_t_rpc trailers;
unsigned long int status_n;
ngx_str_t_rpc status_line;
ngx_array_t_rpc cache_control;
ngx_array_t_rpc cookies;
int content_length_n;
int last_modified_time;
unsigned connection_close;
unsigned chunked;
};

struct ngx_resolver_t_rpc{
ngx_event_t_rpc_ptr event;
/*opaque dummy<>;*/
ngx_log_t_rpc_ptr log;
long int ident;
ngx_array_t_rpc connections;
unsigned long int last_connection;
ngx_rbtree_t_rpc name_rbtree;
ngx_rbtree_node_t_rpc name_sentinel;
ngx_rbtree_t_rpc srv_rbtree;
ngx_rbtree_node_t_rpc srv_sentinel;
ngx_rbtree_t_rpc addr_rbtree;
ngx_rbtree_node_t_rpc addr_sentinel;
ngx_queue_t_rpc name_resend_queue;
ngx_queue_t_rpc srv_resend_queue;
ngx_queue_t_rpc addr_resend_queue;
ngx_queue_t_rpc name_expire_queue;
ngx_queue_t_rpc srv_expire_queue;
ngx_queue_t_rpc addr_expire_queue;
unsigned long int ipv6;
ngx_rbtree_t_rpc addr6_rbtree;
ngx_rbtree_node_t_rpc addr6_sentinel;
ngx_queue_t_rpc addr6_resend_queue;
ngx_queue_t_rpc addr6_expire_queue;
int resend_timeout;
int tcp_timeout;
int expire;
int valid;
unsigned long int log_level;
};
typedef struct ngx_resolver_t_rpc ngx_resolver_t_rpc;
typedef ngx_resolver_t_rpc ngx_resolver_t_rpc_ptr<>;
struct ngx_resolver_node_t_rpc{
ngx_rbtree_node_t_rpc node;
ngx_queue_t_rpc queue;
char_ptr name;
int nlen;
int qlen;
char_ptr query;
char_ptr query6;
int code;
int naddrs;
int nsrvs;
int cnlen;
int naddrs6;
int expire;
int valid;
int ttl;
unsigned tcp;
unsigned tcp6;
unsigned long int last_connection;
};
typedef struct ngx_resolver_node_t_rpc ngx_resolver_node_t_rpc;
typedef ngx_resolver_node_t_rpc ngx_resolver_node_t_rpc_ptr<>;
struct ngx_resolver_addr_t_rpc{
sockaddr_rpc sockaddr;
int socklen;
ngx_str_t_rpc name;
int priority;
int weight;
};
typedef struct ngx_resolver_addr_t_rpc ngx_resolver_addr_t_rpc;
typedef ngx_resolver_addr_t_rpc ngx_resolver_addr_t_rpc_ptr<>;
struct ngx_resolver_srv_name_t_rpc{
ngx_str_t_rpc name;
int priority;
int weight;
int port;
long int state;
unsigned long int naddrs;
struct  ngx_addr_t_rpc * addrs;
};
typedef struct ngx_resolver_srv_name_t_rpc ngx_resolver_srv_name_t_rpc;
typedef ngx_resolver_srv_name_t_rpc ngx_resolver_srv_name_t_rpc_ptr<>;
struct ngx_resolver_ctx_t_rpc{
struct  ngx_resolver_ctx_t_rpc * next;
ngx_resolver_t_rpc_ptr resolver;
ngx_resolver_node_t_rpc_ptr node;
long int ident;
long int state;
ngx_str_t_rpc name;
ngx_str_t_rpc service;
int valid;
unsigned long int naddrs;
ngx_resolver_addr_t_rpc_ptr addrs;
ngx_resolver_addr_t_rpc addr;
unsigned long int count;
unsigned long int nsrvs;
ngx_resolver_srv_name_t_rpc_ptr srvs;
char_ptr handler;
/*opaque data<>;*/
unsigned long int timeout;
unsigned quick;
unsigned async;
unsigned cancelable;
unsigned long int recursion;
ngx_event_t_rpc_ptr event;
};
typedef struct ngx_resolver_ctx_t_rpc ngx_resolver_ctx_t_rpc;
typedef ngx_resolver_ctx_t_rpc ngx_resolver_ctx_t_rpc_ptr<>;
struct ngx_http_upstream_resolved_t_rpc{
ngx_str_t_rpc host;
uint16_t port;
unsigned long int no_port;
unsigned long int naddrs;
struct  ngx_resolver_addr_t_rpc * addrs;
sockaddr_rpc sockaddr;
int socklen;
ngx_str_t_rpc name;
ngx_resolver_ctx_t_rpc_ptr ctx;
};
typedef struct ngx_http_upstream_resolved_t_rpc ngx_http_upstream_resolved_t_rpc;
typedef ngx_http_upstream_resolved_t_rpc ngx_http_upstream_resolved_t_rpc_ptr<>;
struct ngx_http_upstream_state_t_rpc{
unsigned long int status;
unsigned long int response_time;
unsigned long int connect_time;
unsigned long int header_time;
unsigned long int queue_time;
int response_length;
int bytes_received;
struct  ngx_str_t_rpc * peer;
};
typedef struct ngx_http_upstream_state_t_rpc ngx_http_upstream_state_t_rpc;
typedef ngx_http_upstream_state_t_rpc ngx_http_upstream_state_t_rpc_ptr<>;
struct ngx_http_upstream_t_rpc{
char_ptr read_event_handler;
char_ptr write_event_handler;
ngx_peer_connection_t_rpc peer;
ngx_event_pipe_t_rpc_ptr pipe;
ngx_chain_t_rpc_ptr request_bufs;
ngx_output_chain_ctx_t_rpc output;
ngx_chain_writer_ctx_t_rpc writer;
ngx_http_upstream_conf_t_rpc_ptr conf;
ngx_http_upstream_srv_conf_t_rpc_ptr upstream;
ngx_http_upstream_headers_in_t_rpc headers_in;
ngx_http_upstream_resolved_t_rpc_ptr resolved;
ngx_buf_t_rpc from_client;
ngx_buf_t_rpc buffer;
int length;
ngx_chain_t_rpc_ptr out_bufs;
ngx_chain_t_rpc_ptr busy_bufs;
ngx_chain_t_rpc_ptr free_bufs;
char_ptr input_filter_init;
char_ptr input_filter;
/*opaque input_filter_ctx<>;*/
char_ptr create_key;
char_ptr create_request;
char_ptr reinit_request;
char_ptr process_header;
char_ptr abort_request;
char_ptr finalize_request;
char_ptr rewrite_redirect;
char_ptr rewrite_cookie;
unsigned long int timeout;
ngx_http_upstream_state_t_rpc_ptr state;
ngx_str_t_rpc method;
ngx_str_t_rpc schema;
ngx_str_t_rpc uri;
char_ptr cleanup;
unsigned store;
unsigned cacheable;
unsigned accel;
unsigned ssl;
unsigned cache_status;
unsigned buffering;
unsigned keepalive;
unsigned upgrade;
unsigned request_sent;
unsigned request_body_sent;
unsigned request_body_blocked;
unsigned header_sent;
};
typedef struct ngx_http_upstream_t_rpc ngx_http_upstream_t_rpc;
typedef ngx_http_upstream_t_rpc ngx_http_upstream_t_rpc_ptr<>;

struct ngx_table_elt_t_rpc {
    unsigned long int        hash;
    ngx_str_t_rpc         key;
    ngx_str_t_rpc         value;
    char_ptr           lowcase_key;
} ;
typedef struct ngx_table_elt_t_rpc ngx_table_elt_t_rpc;
typedef ngx_table_elt_t_rpc ngx_table_elt_t_rpc_ptr<>;

struct ngx_http_headers_in_t_rpc {
/*ngx_list_t_rpc headers;*/
ngx_str_t_rpc user;
ngx_str_t_rpc passwd;
 ngx_table_elt_t_rpc_ptr authorization;
/**ngx_array_t_rpc cookies;
ngx_str_t_rpc server;
int content_length_n;
int keep_alive_n;
unsigned connection_type;
unsigned chunked;
unsigned msie;
unsigned msie6;
unsigned opera;
unsigned gecko;
unsigned chrome;
unsigned safari;
unsigned konqueror;*/
};

struct ngx_http_headers_out_t_rpc {
ngx_list_t_rpc headers;
ngx_list_t_rpc trailers;
unsigned long int status;
ngx_str_t_rpc status_line;
struct  ngx_str_t_rpc * override_charset;
 int content_type_len;
ngx_str_t_rpc content_type;
ngx_str_t_rpc charset;
char_ptr content_type_lowcase;
unsigned long int content_type_hash;
ngx_array_t_rpc cache_control;
ngx_array_t_rpc link;
int content_length_n;
int content_offset;
int date_time;
int last_modified_time;
};

struct ngx_http_chunked_t_rpc{
unsigned long int state;
int size;
int length;
};
typedef struct ngx_http_chunked_t_rpc ngx_http_chunked_t_rpc;
typedef ngx_http_chunked_t_rpc ngx_http_chunked_t_rpc_ptr<>;
struct ngx_http_request_body_t_rpc{
struct  ngx_temp_file_t_rpc * temp_file;
ngx_chain_t_rpc_ptr bufs;
ngx_buf_t_rpc_ptr buf;
int rest;
int received;
ngx_chain_t_rpc_ptr free;
ngx_chain_t_rpc_ptr busy;
ngx_http_chunked_t_rpc_ptr chunked;
char_ptr post_handler;
};
typedef struct ngx_http_request_body_t_rpc ngx_http_request_body_t_rpc;
typedef ngx_http_request_body_t_rpc ngx_http_request_body_t_rpc_ptr<>;
struct ngx_http_postponed_request_t_rpc{
ngx_chain_t_rpc_ptr out;
struct  ngx_http_postponed_request_t_rpc * next;
};
typedef struct ngx_http_postponed_request_t_rpc ngx_http_postponed_request_t_rpc;
typedef ngx_http_postponed_request_t_rpc ngx_http_postponed_request_t_rpc_ptr<>;
struct ngx_http_post_subrequest_t_rpc{
char_ptr handler;
/*opaque data<>;*/
};
typedef struct ngx_http_post_subrequest_t_rpc ngx_http_post_subrequest_t_rpc;
typedef ngx_http_post_subrequest_t_rpc ngx_http_post_subrequest_t_rpc_ptr<>;
struct ngx_http_posted_request_t_rpc{
struct  ngx_http_posted_request_t_rpc * next;
};
typedef struct ngx_http_posted_request_t_rpc ngx_http_posted_request_t_rpc;
typedef ngx_http_posted_request_t_rpc ngx_http_posted_request_t_rpc_ptr<>;
/** struct ngx_http_complex_value_t_rpc{
ngx_str_t_rpc value;
char_ptr flushes;
opaque lengths<>;
opaque values<>;
};
typedef struct ngx_http_complex_value_t_rpc ngx_http_complex_value_t_rpc;
typedef ngx_http_complex_value_t_rpc ngx_http_complex_value_t_rpc_ptr<>;*/

struct ngx_http_complex_value_t_rpc{
ngx_str_t_rpc value;
int_ptr flushes;
char_ptr lengths;
char_ptr values;
};
typedef ngx_http_complex_value_t_rpc ngx_http_complex_value_t_rpc_ptr<>;
struct ngx_open_file_cache_t_rpc{
ngx_rbtree_t_rpc rbtree;
ngx_rbtree_node_t_rpc sentinel;
ngx_queue_t_rpc expire_queue;
unsigned long int current;
unsigned long int max;
int inactive;
};
typedef struct ngx_open_file_cache_t_rpc ngx_open_file_cache_t_rpc;
typedef ngx_open_file_cache_t_rpc ngx_open_file_cache_t_rpc_ptr<>;
struct ngx_http_core_loc_conf_t_rpc{
ngx_str_t_rpc name;
unsigned noname;
unsigned lmt_excpt;
unsigned named;
unsigned exact_match;
unsigned noregex;
unsigned auto_redirect;
TwoCharPtr loc_conf;
int limit_except;
TwoCharPtr limit_except_loc_conf;
char_ptr handler;
 int alias;
ngx_str_t_rpc root;
ngx_str_t_rpc post_action;
ngx_hash_t_rpc types_hash;
ngx_str_t_rpc default_type;
int client_max_body_size;
int directio;
int directio_alignment;
 int client_body_buffer_size;
 int send_lowat;
 int postpone_output;
 int limit_rate;
 int limit_rate_after;
 int sendfile_max_chunk;
 int read_ahead;
 int subrequest_output_buffer_size;
unsigned long int client_body_timeout;
unsigned long int send_timeout;
unsigned long int keepalive_timeout;
unsigned long int lingering_time;
unsigned long int lingering_timeout;
unsigned long int resolver_timeout;
ngx_resolver_t_rpc_ptr resolver;
int keepalive_header;
unsigned long int keepalive_requests;
unsigned long int keepalive_disable;
unsigned long int satisfy;
unsigned long int lingering_close;
unsigned long int if_modified_since;
unsigned long int max_ranges;
unsigned long int client_body_in_file_only;
long int client_body_in_single_buffer;
long int internal;
long int sendfile;
long int aio;
long int aio_write;
long int tcp_nopush;
long int tcp_nodelay;
long int reset_timedout_connection;
long int absolute_redirect;
long int server_name_in_redirect;
long int port_in_redirect;
long int msie_padding;
long int msie_refresh;
long int log_not_found;
long int log_subrequest;
long int recursive_error_pages;
unsigned long int server_tokens;
long int chunked_transfer_encoding;
long int etag;
unsigned long int disable_symlinks;
ngx_http_complex_value_t_rpc_ptr disable_symlinks_from;
ngx_path_t_rpc_ptr client_body_temp_path;
ngx_open_file_cache_t_rpc_ptr open_file_cache;
int open_file_cache_valid;
unsigned long int open_file_cache_min_uses;
long int open_file_cache_errors;
long int open_file_cache_events;
ngx_log_t_rpc_ptr error_log;
unsigned long int types_hash_max_size;
unsigned long int types_hash_bucket_size;
};
typedef struct ngx_http_core_loc_conf_t_rpc ngx_http_core_loc_conf_t_rpc;
typedef ngx_http_core_loc_conf_t_rpc ngx_http_core_loc_conf_t_rpc_ptr<>;
struct ngx_http_location_tree_node_t_rpc{
struct  ngx_http_location_tree_node_t_rpc * left;
struct  ngx_http_location_tree_node_t_rpc * right;
struct  ngx_http_location_tree_node_t_rpc * tree;
ngx_http_core_loc_conf_t_rpc_ptr exact;
ngx_http_core_loc_conf_t_rpc_ptr inclusive;
int auto_redirect;
int len;
};
typedef struct ngx_http_location_tree_node_t_rpc ngx_http_location_tree_node_t_rpc;
typedef ngx_http_location_tree_node_t_rpc ngx_http_location_tree_node_t_rpc_ptr<>;
struct ngx_http_core_loc_conf_s_rpc {
ngx_str_t_rpc name;
unsigned noname;
unsigned lmt_excpt;
unsigned named;
unsigned exact_match;
unsigned noregex;
unsigned auto_redirect;
ngx_http_location_tree_node_t_rpc_ptr static_locations;
TwoCharPtr loc_conf;
int limit_except;
TwoCharPtr limit_except_loc_conf;
char_ptr handler;
 int alias;
ngx_str_t_rpc root;
ngx_str_t_rpc post_action;
ngx_hash_t_rpc types_hash;
ngx_str_t_rpc default_type;
int client_max_body_size;
int directio;
int directio_alignment;
 int client_body_buffer_size;
 int send_lowat;
 int postpone_output;
 int limit_rate;
 int limit_rate_after;
 int sendfile_max_chunk;
 int read_ahead;
 int subrequest_output_buffer_size;
unsigned long int client_body_timeout;
unsigned long int send_timeout;
unsigned long int keepalive_timeout;
unsigned long int lingering_time;
unsigned long int lingering_timeout;
unsigned long int resolver_timeout;
ngx_resolver_t_rpc_ptr resolver;
int keepalive_header;
unsigned long int keepalive_requests;
unsigned long int keepalive_disable;
unsigned long int satisfy;
unsigned long int lingering_close;
unsigned long int if_modified_since;
unsigned long int max_ranges;
unsigned long int client_body_in_file_only;
long int client_body_in_single_buffer;
long int internal;
long int sendfile;
long int aio;
long int aio_write;
long int tcp_nopush;
long int tcp_nodelay;
long int reset_timedout_connection;
long int absolute_redirect;
long int server_name_in_redirect;
long int port_in_redirect;
long int msie_padding;
long int msie_refresh;
long int log_not_found;
long int log_subrequest;
long int recursive_error_pages;
unsigned long int server_tokens;
long int chunked_transfer_encoding;
long int etag;
unsigned long int disable_symlinks;
ngx_http_complex_value_t_rpc_ptr disable_symlinks_from;
ngx_path_t_rpc_ptr client_body_temp_path;
ngx_open_file_cache_t_rpc_ptr open_file_cache;
int open_file_cache_valid;
unsigned long int open_file_cache_min_uses;
long int open_file_cache_errors;
long int open_file_cache_events;
ngx_log_t_rpc_ptr error_log;
unsigned long int types_hash_max_size;
unsigned long int types_hash_bucket_size;
};

typedef ngx_http_core_loc_conf_s_rpc ngx_http_core_loc_conf_s_rpc_ptr<>;

typedef struct ngx_http_core_loc_conf_s_rpc_ptr ngx_http_core_loc_conf_s_rpc_ptr;

typedef struct ngx_http_core_loc_conf_s_rpc_ptr ngx_http_core_loc_conf_s_rpc_twoptr<>;

struct ngx_http_core_srv_conf_t_rpc{
ngx_array_t_rpc server_names;
char_ptr file_name;
unsigned long int line;
ngx_str_t_rpc server_name;
 int connection_pool_size;
 int request_pool_size;
 int client_header_buffer_size;
ngx_bufs_t_rpc large_client_header_buffers;
unsigned long int client_header_timeout;
long int ignore_invalid_headers;
long int merge_slashes;
long int underscores_in_headers;
unsigned listen;
ngx_http_core_loc_conf_s_rpc_twoptr named_locations;
};
typedef struct ngx_http_core_srv_conf_t_rpc ngx_http_core_srv_conf_t_rpc;
typedef ngx_http_core_srv_conf_t_rpc ngx_http_core_srv_conf_t_rpc_ptr<>;
struct ngx_hash_combined_t_rpc {
ngx_hash_t_rpc hash;
};

struct ngx_http_virtual_names_t_rpc{
ngx_hash_combined_t_rpc names;
unsigned long int nregex;
};
typedef struct ngx_http_virtual_names_t_rpc ngx_http_virtual_names_t_rpc;
typedef ngx_http_virtual_names_t_rpc ngx_http_virtual_names_t_rpc_ptr<>;
struct ngx_http_addr_conf_t_rpc{
ngx_http_core_srv_conf_t_rpc_ptr default_server;
ngx_http_virtual_names_t_rpc_ptr virtual_names;
unsigned ssl;
unsigned http2;
unsigned proxy_protocol;
};
typedef struct ngx_http_addr_conf_t_rpc ngx_http_addr_conf_t_rpc;
typedef ngx_http_addr_conf_t_rpc ngx_http_addr_conf_t_rpc_ptr<>;
struct ngx_http_connection_t_rpc{
ngx_http_addr_conf_t_rpc_ptr addr_conf;
ngx_chain_t_rpc_ptr busy;
long int nbusy;
ngx_chain_t_rpc_ptr free;
unsigned ssl;
unsigned proxy_protocol;
};
typedef struct ngx_http_connection_t_rpc ngx_http_connection_t_rpc;
typedef ngx_http_connection_t_rpc ngx_http_connection_t_rpc_ptr<>;
struct ngx_http_v2_stream_t_rpc{
/*opaque tempname<>;*/
char_ptr tempname;
};
typedef struct ngx_http_v2_stream_t_rpc ngx_http_v2_stream_t_rpc;
typedef ngx_http_v2_stream_t_rpc ngx_http_v2_stream_t_rpc_ptr<>;
struct ngx_http_cleanup_t_rpc{
char_ptr handler;
/*opaque data<>;*/
struct  ngx_http_cleanup_t_rpc * next;
};
typedef struct ngx_http_cleanup_t_rpc ngx_http_cleanup_t_rpc;
typedef ngx_http_cleanup_t_rpc ngx_http_cleanup_t_rpc_ptr<>;



struct ngx_http_auth_basic_loc_conf_t_rpc{
 ngx_http_complex_value_t_rpc_ptr realm;
ngx_http_complex_value_t_rpc user_file;
};
typedef ngx_http_auth_basic_loc_conf_t_rpc ngx_http_auth_basic_loc_conf_t_rpc_ptr<>;
typedef ngx_http_auth_basic_loc_conf_t_rpc_ptr ngx_http_auth_basic_loc_conf_t_rpc_twoptr<>;

/* 内存池数据结构 */
struct ngx_pool_data_t_rpc {
    char_ptr last_offset;    /* 存储偏移量而不是直接指针 */
    char_ptr end_offset;     /* 存储偏移量而不是直接指针 */
    unsigned long int failed;
};
typedef struct ngx_pool_data_t_rpc ngx_pool_data_t_rpc;

/* 内存池结构体（避免循环引用） */
struct ngx_pool_t_rpc {
    ngx_pool_data_t_rpc d;
    unsigned long int max;           /* 最大块大小 */
    unsigned long int pool_id;       /* 池的唯一标识 */
    unsigned long int next_pool_id;  /* 下一个池的ID（0表示没有） */
    unsigned long int current_pool_id; /* 当前池的ID */
    opaque pool_data<>;              /* 实际的内存池数据 */
};

typedef struct ngx_pool_t_rpc ngx_pool_t_rpc;
typedef ngx_pool_t_rpc ngx_pool_t_rpc_ptr<>;

struct ngx_http_request_t_rpc{
int signature;
/* ngx_connection_t_rpc_ptr connection; */
/* TwoCharPtr ctx; */
/* TwoCharPtr main_conf; */
/* TwoCharPtr srv_conf; */
ngx_http_auth_basic_loc_conf_t_rpc_twoptr loc_conf;
/** char_ptr read_event_handler;
char_ptr write_event_handler;
ngx_http_cache_t_rpc_ptr cache;
ngx_http_upstream_t_rpc_ptr upstream;
ngx_buf_t_rpc_ptr header_in;*/
ngx_http_headers_in_t_rpc headers_in;
ngx_pool_t_rpc_ptr pool;  /* 新增：内存池字段 */
/**ngx_http_headers_out_t_rpc headers_out;
ngx_http_request_body_t_rpc_ptr request_body; */
int lingering_time;
int start_sec;
unsigned long int start_msec;
unsigned long int method;
unsigned long int http_version;
ngx_str_t_rpc request_line;
ngx_str_t_rpc uri;
ngx_str_t_rpc args;
ngx_str_t_rpc exten;
ngx_str_t_rpc unparsed_uri;
ngx_str_t_rpc method_name;
ngx_str_t_rpc http_protocol;
ngx_str_t_rpc schema;
/** ngx_chain_t_rpc_ptr out;
struct  ngx_http_request_t_rpc * main;
struct  ngx_http_request_t_rpc * parent;
ngx_http_postponed_request_t_rpc_ptr postponed;
ngx_http_post_subrequest_t_rpc_ptr post_subrequest;
ngx_http_posted_request_t_rpc_ptr posted_requests; */
long int phase_handler;
/* char_ptr content_handler; */
unsigned long int access_code;
/* char_ptr variables; */
 int limit_rate;
 int limit_rate_after;
 int header_size;
int request_length;
unsigned long int err_status;
/**ngx_http_connection_t_rpc_ptr http_connection;
ngx_http_v2_stream_t_rpc_ptr stream;
char_ptr log_handler;
ngx_http_cleanup_t_rpc_ptr cleanup; */
unsigned count;
unsigned subrequests;
unsigned blocked;
unsigned aio;
unsigned http_state;
unsigned complex_uri;
unsigned quoted_uri;
unsigned plus_in_uri;
unsigned space_in_uri;
unsigned invalid_header;
unsigned add_uri_to_alias;
unsigned valid_location;
unsigned valid_unparsed_uri;
unsigned uri_changed;
unsigned uri_changes;
unsigned request_body_in_single_buf;
unsigned request_body_in_file_only;
unsigned request_body_in_persistent_file;
unsigned request_body_in_clean_file;
unsigned request_body_file_group_access;
unsigned request_body_file_log_level;
unsigned request_body_no_buffering;
unsigned subrequest_in_memory;
unsigned waited;
unsigned cached;
unsigned proxy;
unsigned bypass_cache;
unsigned no_cache;
unsigned limit_conn_set;
unsigned limit_req_set;
unsigned pipeline;
unsigned chunked;
unsigned header_only;
unsigned expect_trailers;
unsigned keepalive;
unsigned lingering_close;
unsigned discard_body;
unsigned reading_body;
unsigned internal;
unsigned error_page;
unsigned filter_finalize;
unsigned post_action;
unsigned request_complete;
unsigned request_output;
unsigned header_sent;
unsigned expect_tested;
unsigned root_tested;
unsigned done;
unsigned logged;
unsigned buffered;
unsigned main_filter_need_in_memory;
unsigned filter_need_in_memory;
unsigned filter_need_temporary;
unsigned preserve_body;
unsigned allow_ranges;
unsigned subrequest_ranges;
unsigned single_range;
unsigned disable_not_modified;
unsigned stat_reading;
unsigned stat_writing;
unsigned stat_processing;
unsigned background;
unsigned health_check;
unsigned long int state;
unsigned long int header_hash;
unsigned long int lowcase_index;
char_ptr header_name_start;
char_ptr header_name_end;
char_ptr header_start;
char_ptr header_end;
char_ptr uri_start;
char_ptr uri_end;
char_ptr uri_ext;
char_ptr args_start;
char_ptr request_start;
char_ptr request_end;
char_ptr method_end;
char_ptr schema_start;
char_ptr schema_end;
char_ptr host_start;
char_ptr host_end;
char_ptr port_start;
char_ptr port_end;
unsigned http_minor;
unsigned http_major;
};
typedef struct ngx_http_request_t_rpc ngx_http_request_t_rpc;
typedef ngx_http_request_t_rpc ngx_http_request_t_rpc_ptr<>;
struct ngx_http_auth_basic_handler_sense_1_ret_{
long int ret0;
int_ptr ngx_http_auth_basic_handler_sense_1_ret;
ngx_http_request_t_rpc_ptr r;
};
typedef struct ngx_http_auth_basic_handler_sense_1_ret_ ngx_http_auth_basic_handler_sense_1_ret_;

struct ngx_event_timer_rbtree_ret_t { ngx_rbtree_t_rpc value; };
typedef struct ngx_event_timer_rbtree_ret_t ngx_event_timer_rbtree_ret_t;
typedef ngx_str_t_rpc ngx_str_t_rpc_ptr<>;typedef struct ngx_str_t_rpc_ptr ngx_str_t_rpc_ptr;
struct ngx_sys_errlist_ret_t { ngx_str_t_rpc_ptr value; };
typedef struct ngx_sys_errlist_ret_t ngx_sys_errlist_ret_t;
struct ngx_sys_errlist_len_ret_t { long int value; };
typedef struct ngx_sys_errlist_len_ret_t ngx_sys_errlist_len_ret_t;
struct ngx_sys_errlist_data_ret_t { u_char * value; };
typedef struct ngx_sys_errlist_data_ret_t ngx_sys_errlist_data_ret_t;

program RPCNGINX {
   version RPCNGINXTVERS {
       ngx_http_auth_basic_handler_sense_1_ret_  ngx_http_auth_basic_handler_sense_1_rpc( int_ptr, ngx_http_request_t_rpc_ptr)=1;
   } = 1;
} = 0x20000001;