#include "sslkey_auth_idl.h"
#include "ngx_http.h"
ngx_int_t ngx_http_auth_basic_handler_sense_1_wrapper(int *ngx_http_auth_basic_handler_sense_1_ret, ngx_http_request_t *r);

/* 全局变量访问函数声明 */
ngx_str_t* get_ngx_sys_errlist_wrapper();
int set_ngx_sys_errlist_wrapper(ngx_str_t value,int len);

int set_ngx_sys_errlist_len_wrapper(size_t value);
size_t get_ngx_sys_errlist_len_wrapper();

int set_ngx_sys_errlist_data_wrapper(u_char * value);
u_char * get_ngx_sys_errlist_data_wrapper();

ngx_rbtree_t get_ngx_event_timer_rbtree_wrapper();
int set_ngx_event_timer_rbtree_wrapper(ngx_rbtree_t value);

ngx_pid_t get_ngx_pid_wrapper();
int set_ngx_pid_wrapper(ngx_pid_t value);

int set_ngx_use_stderr_wrapper(ngx_uint_t value);
ngx_uint_t get_ngx_use_stderr_wrapper();

ngx_uint_t get_ngx_cached_time_sec_wrapper();
int set_ngx_cached_time_wrapper(ngx_uint_t value);

ngx_msec_t get_ngx_current_msec_wrapper();
int set_ngx_current_msec_wrapper(ngx_msec_t value);

ngx_uint_t get_ngx_cached_time_wrapper();

// static u_char *ngx_sprintf_num(u_char *buf, u_char *last, uint64_t ui64, u_char zero, ngx_uint_t hexadecimal, ngx_uint_t width);
// typedef u_char *(*ngx_sprintf_numFunc)(u_char *buf, u_char *last, uint64_t ui64, u_char zero, ngx_uint_t hexadecimal, ngx_uint_t width);
// static void *ngx_core_module_create_conf(ngx_cycle_t *cycle);
// typedef void *(*ngx_core_module_create_confFunc)(ngx_cycle_t *cycle);
// char *ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_conf_set_flag_slotFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// static char *ngx_set_user(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_set_userFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// static char *ngx_set_worker_processes(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_set_worker_processesFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// static char *ngx_events_block(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_events_blockFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// static void *ngx_event_core_create_conf(ngx_cycle_t *cycle);
// typedef void *(*ngx_event_core_create_confFunc)(ngx_cycle_t *cycle);
// static void *ngx_epoll_create_conf(ngx_cycle_t *cycle);
// typedef void *(*ngx_epoll_create_confFunc)(ngx_cycle_t *cycle);
// static char *ngx_event_connections(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_event_connectionsFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// static char *ngx_event_core_init_conf(ngx_cycle_t *cycle, void *conf);
// typedef char *(*ngx_event_core_init_confFunc)(ngx_cycle_t *cycle, void *conf);
// static char *ngx_epoll_init_conf(ngx_cycle_t *cycle, void *conf);
// typedef char *(*ngx_epoll_init_confFunc)(ngx_cycle_t *cycle, void *conf);
// static char *ngx_http_block(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_http_blockFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// static void *ngx_http_core_create_main_conf(ngx_conf_t *cf);
// typedef void *(*ngx_http_core_create_main_confFunc)(ngx_conf_t *cf);
// static void *ngx_http_core_create_srv_conf(ngx_conf_t *cf);
// typedef void *(*ngx_http_core_create_srv_confFunc)(ngx_conf_t *cf);
// static void *ngx_http_core_create_loc_conf(ngx_conf_t *cf);
// typedef void *(*ngx_http_core_create_loc_confFunc)(ngx_conf_t *cf);
// static void *ngx_http_log_create_main_conf(ngx_conf_t *cf);
// typedef void *(*ngx_http_log_create_main_confFunc)(ngx_conf_t *cf);
// static void *ngx_http_log_create_loc_conf(ngx_conf_t *cf);
// typedef void *(*ngx_http_log_create_loc_confFunc)(ngx_conf_t *cf);
// static void *ngx_http_upstream_create_main_conf(ngx_conf_t *cf);
// typedef void *(*ngx_http_upstream_create_main_confFunc)(ngx_conf_t *cf);
// static void *ngx_http_index_create_loc_conf(ngx_conf_t *cf);
// typedef void *(*ngx_http_index_create_loc_confFunc)(ngx_conf_t *cf);
// static void *ngx_http_mirror_create_loc_conf(ngx_conf_t *cf);
// typedef void *(*ngx_http_mirror_create_loc_confFunc)(ngx_conf_t *cf);
// static void *ngx_http_try_files_create_loc_conf(ngx_conf_t *cf);
// typedef void *(*ngx_http_try_files_create_loc_confFunc)(ngx_conf_t *cf);
// static void *ngx_http_auth_basic_create_loc_conf(ngx_conf_t *cf);
// typedef void *(*ngx_http_auth_basic_create_loc_confFunc)(ngx_conf_t *cf);
// static void *ngx_http_upstream_hash_create_conf(ngx_conf_t *cf);
// typedef void *(*ngx_http_upstream_hash_create_confFunc)(ngx_conf_t *cf);
// static void *ngx_http_upstream_random_create_conf(ngx_conf_t *cf);
// typedef void *(*ngx_http_upstream_random_create_confFunc)(ngx_conf_t *cf);
// static void *ngx_http_upstream_keepalive_create_conf(ngx_conf_t *cf);
// typedef void *(*ngx_http_upstream_keepalive_create_confFunc)(ngx_conf_t *cf);
// static void *ngx_http_headers_create_conf(ngx_conf_t *cf);
// typedef void *(*ngx_http_headers_create_confFunc)(ngx_conf_t *cf);
// static void *ngx_http_copy_filter_create_conf(ngx_conf_t *cf);
// typedef void *(*ngx_http_copy_filter_create_confFunc)(ngx_conf_t *cf);
// static ngx_int_t ngx_http_core_preconfiguration(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_core_preconfigurationFunc)(ngx_conf_t *cf);
// static ngx_int_t ngx_http_upstream_add_variables(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_upstream_add_variablesFunc)(ngx_conf_t *cf);
// char *ngx_conf_include(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_conf_includeFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// static char *ngx_http_core_types(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_http_core_typesFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// static char *ngx_http_core_type(ngx_conf_t *cf, ngx_command_t *dummy, void *conf);
// typedef char *(*ngx_http_core_typeFunc)(ngx_conf_t *cf, ngx_command_t *dummy, void *conf);
// char *ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_conf_set_str_slotFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// static char *ngx_http_core_keepalive(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_http_core_keepaliveFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// static char *ngx_http_core_server(ngx_conf_t *cf, ngx_command_t *cmd, void *dummy);
// typedef char *(*ngx_http_core_serverFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *dummy);
// static char *ngx_http_core_listen(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_http_core_listenFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// static char *ngx_http_core_server_name(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_http_core_server_nameFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// char *ngx_http_set_complex_value_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_http_set_complex_value_slotFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// static char *ngx_http_auth_basic_user_file(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_http_auth_basic_user_fileFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// static char *ngx_http_core_location(ngx_conf_t *cf, ngx_command_t *cmd, void *dummy);
// typedef char *(*ngx_http_core_locationFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *dummy);
// static char *ngx_http_core_root(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_http_core_rootFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// static char *ngx_http_index_set_index(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_http_index_set_indexFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// static char *ngx_http_core_error_page(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// typedef char *(*ngx_http_core_error_pageFunc)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
// static char *ngx_http_core_init_main_conf(ngx_conf_t *cf, void *conf);
// typedef char *(*ngx_http_core_init_main_confFunc)(ngx_conf_t *cf, void *conf);
// static char *ngx_http_core_merge_srv_conf(ngx_conf_t *cf, void *parent, void *child);
// typedef char *(*ngx_http_core_merge_srv_confFunc)(ngx_conf_t *cf, void *parent, void *child);
// static char *ngx_http_core_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);
// typedef char *(*ngx_http_core_merge_loc_confFunc)(ngx_conf_t *cf, void *parent, void *child);
// static char *ngx_http_log_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);
// typedef char *(*ngx_http_log_merge_loc_confFunc)(ngx_conf_t *cf, void *parent, void *child);
// static char *ngx_http_upstream_init_main_conf(ngx_conf_t *cf, void *conf);
// typedef char *(*ngx_http_upstream_init_main_confFunc)(ngx_conf_t *cf, void *conf);
// static char *ngx_http_index_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);
// typedef char *(*ngx_http_index_merge_loc_confFunc)(ngx_conf_t *cf, void *parent, void *child);
// static char *ngx_http_mirror_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);
// typedef char *(*ngx_http_mirror_merge_loc_confFunc)(ngx_conf_t *cf, void *parent, void *child);
// static char *ngx_http_auth_basic_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);
// typedef char *(*ngx_http_auth_basic_merge_loc_confFunc)(ngx_conf_t *cf, void *parent, void *child);
// static char *ngx_http_headers_merge_conf(ngx_conf_t *cf, void *parent, void *child);
// typedef char *(*ngx_http_headers_merge_confFunc)(ngx_conf_t *cf, void *parent, void *child);
// static char *ngx_http_copy_filter_merge_conf(ngx_conf_t *cf, void *parent, void *child);
// typedef char *(*ngx_http_copy_filter_merge_confFunc)(ngx_conf_t *cf, void *parent, void *child);
// static ngx_int_t ngx_http_cmp_locations(const ngx_queue_t *one, const ngx_queue_t *two);
// typedef ngx_int_t (*ngx_http_cmp_locationsFunc)(const ngx_queue_t *one, const ngx_queue_t *two);
// static ngx_int_t ngx_http_core_postconfiguration(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_core_postconfigurationFunc)(ngx_conf_t *cf);
// static ngx_int_t ngx_http_log_init(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_log_initFunc)(ngx_conf_t *cf);
// static ngx_int_t ngx_http_static_init(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_static_initFunc)(ngx_conf_t *cf);
// static ngx_int_t ngx_http_index_init(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_index_initFunc)(ngx_conf_t *cf);
// static ngx_int_t ngx_http_mirror_init(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_mirror_initFunc)(ngx_conf_t *cf);
// static ngx_int_t ngx_http_try_files_init(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_try_files_initFunc)(ngx_conf_t *cf);
// static ngx_int_t ngx_http_auth_basic_init(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_auth_basic_initFunc)(ngx_conf_t *cf);
// static ngx_int_t ngx_http_write_filter_init(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_write_filter_initFunc)(ngx_conf_t *cf);
// static ngx_int_t ngx_http_header_filter_init(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_header_filter_initFunc)(ngx_conf_t *cf);
// static ngx_int_t ngx_http_chunked_filter_init(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_chunked_filter_initFunc)(ngx_conf_t *cf);
// static ngx_int_t ngx_http_range_header_filter_init(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_range_header_filter_initFunc)(ngx_conf_t *cf);
// static ngx_int_t ngx_http_headers_filter_init(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_headers_filter_initFunc)(ngx_conf_t *cf);
// static ngx_int_t ngx_http_copy_filter_init(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_copy_filter_initFunc)(ngx_conf_t *cf);
// static ngx_int_t ngx_http_range_body_filter_init(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_range_body_filter_initFunc)(ngx_conf_t *cf);
// static ngx_int_t ngx_http_not_modified_filter_init(ngx_conf_t *cf);
// typedef ngx_int_t (*ngx_http_not_modified_filter_initFunc)(ngx_conf_t *cf);
// static char *ngx_core_module_init_conf(ngx_cycle_t *cycle, void *conf);
// typedef char *(*ngx_core_module_init_confFunc)(ngx_cycle_t *cycle, void *conf);
// static char *ngx_event_init_conf(ngx_cycle_t *cycle, void *conf);
// typedef char *(*ngx_event_init_confFunc)(ngx_cycle_t *cycle, void *conf);
// static ngx_int_t ngx_event_module_init(ngx_cycle_t *cycle);
// typedef ngx_int_t (*ngx_event_module_initFunc)(ngx_cycle_t *cycle);
// static void ngx_worker_process_cycle(ngx_cycle_t *cycle, void *data);
// typedef void (*ngx_worker_process_cycleFunc)(ngx_cycle_t *cycle, void *data);
// static ngx_int_t ngx_event_process_init(ngx_cycle_t *cycle);
// typedef ngx_int_t (*ngx_event_process_initFunc)(ngx_cycle_t *cycle);
// static ngx_int_t ngx_epoll_init(ngx_cycle_t *cycle, ngx_msec_t timer);
// typedef ngx_int_t (*ngx_epoll_initFunc)(ngx_cycle_t *cycle, ngx_msec_t timer);
// static ngx_int_t ngx_epoll_add_event(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);
// typedef ngx_int_t (*ngx_epoll_add_eventFunc)(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);
// static ngx_int_t ngx_epoll_process_events(ngx_cycle_t *cycle, ngx_msec_t timer, ngx_uint_t flags);
// typedef ngx_int_t (*ngx_epoll_process_eventsFunc)(ngx_cycle_t *cycle, ngx_msec_t timer, ngx_uint_t flags);
// void ngx_event_accept(ngx_event_t *ev);
// typedef void (*ngx_event_acceptFunc)(ngx_event_t *ev);
// void ngx_http_init_connection(ngx_connection_t *c);
// typedef void (*ngx_http_init_connectionFunc)(ngx_connection_t *c);
// static void ngx_http_wait_request_handler(ngx_event_t *rev);
// typedef void (*ngx_http_wait_request_handlerFunc)(ngx_event_t *rev);
// ssize_t ngx_unix_recv(ngx_connection_t *c, u_char *buf, size_t size);
// typedef ssize_t (*ngx_unix_recvFunc)(ngx_connection_t *c, u_char *buf, size_t size);
// static ngx_int_t ngx_http_process_host(ngx_http_request_t *r, ngx_table_elt_t *h, ngx_uint_t offset);
// typedef ngx_int_t (*ngx_http_process_hostFunc)(ngx_http_request_t *r, ngx_table_elt_t *h, ngx_uint_t offset);
// static ngx_int_t ngx_http_process_unique_header_line(ngx_http_request_t *r, ngx_table_elt_t *h, ngx_uint_t offset);
// typedef ngx_int_t (*ngx_http_process_unique_header_lineFunc)(ngx_http_request_t *r, ngx_table_elt_t *h, ngx_uint_t offset);
// static ngx_int_t ngx_http_process_user_agent(ngx_http_request_t *r, ngx_table_elt_t *h, ngx_uint_t offset);
// typedef ngx_int_t (*ngx_http_process_user_agentFunc)(ngx_http_request_t *r, ngx_table_elt_t *h, ngx_uint_t offset);
// ngx_int_t ngx_http_core_find_config_phase(ngx_http_request_t *r, ngx_http_phase_handler_t *ph);
// typedef ngx_int_t (*ngx_http_core_find_config_phaseFunc)(ngx_http_request_t *r, ngx_http_phase_handler_t *ph);
// ngx_int_t ngx_http_core_access_phase(ngx_http_request_t *r, ngx_http_phase_handler_t *ph);
// typedef ngx_int_t (*ngx_http_core_access_phaseFunc)(ngx_http_request_t *r, ngx_http_phase_handler_t *ph);
// static ngx_int_t ngx_http_auth_basic_handler(ngx_http_request_t *r);
// typedef ngx_int_t (*ngx_http_auth_basic_handlerFunc)(ngx_http_request_t *r);
// ngx_int_t ngx_http_core_post_access_phase(ngx_http_request_t *r, ngx_http_phase_handler_t *ph);
// typedef ngx_int_t (*ngx_http_core_post_access_phaseFunc)(ngx_http_request_t *r, ngx_http_phase_handler_t *ph);
// ngx_int_t ngx_http_core_generic_phase(ngx_http_request_t *r, ngx_http_phase_handler_t *ph);
// typedef ngx_int_t (*ngx_http_core_generic_phaseFunc)(ngx_http_request_t *r, ngx_http_phase_handler_t *ph);
// static ngx_int_t ngx_http_try_files_handler(ngx_http_request_t *r);
// typedef ngx_int_t (*ngx_http_try_files_handlerFunc)(ngx_http_request_t *r);
// static ngx_int_t ngx_http_mirror_handler(ngx_http_request_t *r);
// typedef ngx_int_t (*ngx_http_mirror_handlerFunc)(ngx_http_request_t *r);
// ngx_int_t ngx_http_core_content_phase(ngx_http_request_t *r, ngx_http_phase_handler_t *ph);
// typedef ngx_int_t (*ngx_http_core_content_phaseFunc)(ngx_http_request_t *r, ngx_http_phase_handler_t *ph);
// static ngx_int_t ngx_http_index_handler(ngx_http_request_t *r);
// typedef ngx_int_t (*ngx_http_index_handlerFunc)(ngx_http_request_t *r);
// static ngx_int_t ngx_http_static_handler(ngx_http_request_t *r);
// typedef ngx_int_t (*ngx_http_static_handlerFunc)(ngx_http_request_t *r);
// static ngx_int_t ngx_http_not_modified_header_filter(ngx_http_request_t *r);
// typedef ngx_int_t (*ngx_http_not_modified_header_filterFunc)(ngx_http_request_t *r);
// static ngx_int_t ngx_http_headers_filter(ngx_http_request_t *r);
// typedef ngx_int_t (*ngx_http_headers_filterFunc)(ngx_http_request_t *r);
// static ngx_int_t ngx_http_range_header_filter(ngx_http_request_t *r);
// typedef ngx_int_t (*ngx_http_range_header_filterFunc)(ngx_http_request_t *r);
// static ngx_int_t ngx_http_chunked_header_filter(ngx_http_request_t *r);
// typedef ngx_int_t (*ngx_http_chunked_header_filterFunc)(ngx_http_request_t *r);
// static ngx_int_t ngx_http_header_filter(ngx_http_request_t *r);
// typedef ngx_int_t (*ngx_http_header_filterFunc)(ngx_http_request_t *r);
// static ngx_int_t ngx_http_range_body_filter(ngx_http_request_t *r, ngx_chain_t *in);
// typedef ngx_int_t (*ngx_http_range_body_filterFunc)(ngx_http_request_t *r, ngx_chain_t *in);
// static ngx_int_t ngx_http_copy_filter(ngx_http_request_t *r, ngx_chain_t *in);
// typedef ngx_int_t (*ngx_http_copy_filterFunc)(ngx_http_request_t *r, ngx_chain_t *in);
// static ngx_int_t ngx_http_trailers_filter(ngx_http_request_t *r, ngx_chain_t *in);
// typedef ngx_int_t (*ngx_http_trailers_filterFunc)(ngx_http_request_t *r, ngx_chain_t *in);
// static ngx_int_t ngx_http_chunked_body_filter(ngx_http_request_t *r, ngx_chain_t *in);
// typedef ngx_int_t (*ngx_http_chunked_body_filterFunc)(ngx_http_request_t *r, ngx_chain_t *in);
// ngx_int_t ngx_http_write_filter(ngx_http_request_t *r, ngx_chain_t *in);
// typedef ngx_int_t (*ngx_http_write_filterFunc)(ngx_http_request_t *r, ngx_chain_t *in);
// ngx_chain_t *ngx_linux_sendfile_chain(ngx_connection_t *c, ngx_chain_t *in, off_t limit);
// typedef ngx_chain_t *(*ngx_linux_sendfile_chainFunc)(ngx_connection_t *c, ngx_chain_t *in, off_t limit);
// static ngx_int_t ngx_http_log_handler(ngx_http_request_t *r);
// typedef ngx_int_t (*ngx_http_log_handlerFunc)(ngx_http_request_t *r);
// static size_t ngx_http_log_variable_getlen(ngx_http_request_t *r, uintptr_t data);
// typedef size_t (*ngx_http_log_variable_getlenFunc)(ngx_http_request_t *r, uintptr_t data);
// static ngx_int_t ngx_http_variable_remote_addr(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data);
// typedef ngx_int_t (*ngx_http_variable_remote_addrFunc)(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data);
// static ngx_int_t ngx_http_variable_remote_user(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data);
// typedef ngx_int_t (*ngx_http_variable_remote_userFunc)(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data);
// static ngx_int_t ngx_http_variable_request_line(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data);
// typedef ngx_int_t (*ngx_http_variable_request_lineFunc)(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data);
// static ngx_int_t ngx_http_variable_header(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data);
// typedef ngx_int_t (*ngx_http_variable_headerFunc)(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data);
// static u_char *ngx_http_log_variable(ngx_http_request_t *r, u_char *buf, ngx_http_log_op_t *op);
// typedef u_char *(*ngx_http_log_variableFunc)(ngx_http_request_t *r, u_char *buf, ngx_http_log_op_t *op);
// static u_char *ngx_http_log_copy_short(ngx_http_request_t *r, u_char *buf, ngx_http_log_op_t *op);
// typedef u_char *(*ngx_http_log_copy_shortFunc)(ngx_http_request_t *r, u_char *buf, ngx_http_log_op_t *op);
// static u_char *ngx_http_log_time(ngx_http_request_t *r, u_char *buf, ngx_http_log_op_t *op);
// typedef u_char *(*ngx_http_log_timeFunc)(ngx_http_request_t *r, u_char *buf, ngx_http_log_op_t *op);
// static u_char *ngx_http_log_status(ngx_http_request_t *r, u_char *buf, ngx_http_log_op_t *op);
// typedef u_char *(*ngx_http_log_statusFunc)(ngx_http_request_t *r, u_char *buf, ngx_http_log_op_t *op);
// static u_char *ngx_http_log_body_bytes_sent(ngx_http_request_t *r, u_char *buf, ngx_http_log_op_t *op);
// typedef u_char *(*ngx_http_log_body_bytes_sentFunc)(ngx_http_request_t *r, u_char *buf, ngx_http_log_op_t *op);
// void ngx_pool_cleanup_file(void *data);
// typedef void (*ngx_pool_cleanup_fileFunc)(void *data);
// static void ngx_http_keepalive_handler(ngx_event_t *rev);
// typedef void (*ngx_http_keepalive_handlerFunc)(ngx_event_t *rev);
// static ngx_int_t ngx_epoll_del_connection(ngx_connection_t *c, ngx_uint_t flags);
// typedef ngx_int_t (*ngx_epoll_del_connectionFunc)(ngx_connection_t *c, ngx_uint_t flags);
// // static void ngx_signal_handler(int signo, siginfo_t *siginfo, void *ucontext);
// // typedef void (*ngx_signal_handlerFunc)(int signo, siginfo_t *siginfo, void *ucontext);
// static void ngx_conf_flush_files(ngx_cycle_t *cycle);
// typedef void (*ngx_conf_flush_filesFunc)(ngx_cycle_t *cycle);
// static void ngx_cleanup_environment(void *data);
// typedef void (*ngx_cleanup_environmentFunc)(void *data);
// static void ngx_resolver_cleanup(void *data);
// typedef void (*ngx_resolver_cleanupFunc)(void *data);
typedef enum {
    SIG_NGX_SPRINTF_NUMFUNC,
    SIG_NGX_CORE_MODULE_CREATE_CONFFUNC,
    SIG_NGX_CONF_SET_FLAG_SLOTFUNC,
    SIG_NGX_SET_USERFUNC,
    SIG_NGX_SET_WORKER_PROCESSESFUNC,
    SIG_NGX_EVENTS_BLOCKFUNC,
    SIG_NGX_EVENT_CORE_CREATE_CONFFUNC,
    SIG_NGX_EPOLL_CREATE_CONFFUNC,
    SIG_NGX_EVENT_CONNECTIONSFUNC,
    SIG_NGX_EVENT_CORE_INIT_CONFFUNC,
    SIG_NGX_EPOLL_INIT_CONFFUNC,
    SIG_NGX_HTTP_BLOCKFUNC,
    SIG_NGX_HTTP_CORE_CREATE_MAIN_CONFFUNC,
    SIG_NGX_HTTP_CORE_CREATE_SRV_CONFFUNC,
    SIG_NGX_HTTP_CORE_CREATE_LOC_CONFFUNC,
    SIG_NGX_HTTP_LOG_CREATE_MAIN_CONFFUNC,
    SIG_NGX_HTTP_LOG_CREATE_LOC_CONFFUNC,
    SIG_NGX_HTTP_UPSTREAM_CREATE_MAIN_CONFFUNC,
    SIG_NGX_HTTP_INDEX_CREATE_LOC_CONFFUNC,
    SIG_NGX_HTTP_MIRROR_CREATE_LOC_CONFFUNC,
    SIG_NGX_HTTP_TRY_FILES_CREATE_LOC_CONFFUNC,
    SIG_NGX_HTTP_AUTH_BASIC_CREATE_LOC_CONFFUNC,
    SIG_NGX_HTTP_UPSTREAM_HASH_CREATE_CONFFUNC,
    SIG_NGX_HTTP_UPSTREAM_RANDOM_CREATE_CONFFUNC,
    SIG_NGX_HTTP_UPSTREAM_KEEPALIVE_CREATE_CONFFUNC,
    SIG_NGX_HTTP_HEADERS_CREATE_CONFFUNC,
    SIG_NGX_HTTP_COPY_FILTER_CREATE_CONFFUNC,
    SIG_NGX_HTTP_CORE_PRECONFIGURATIONFUNC,
    SIG_NGX_HTTP_UPSTREAM_ADD_VARIABLESFUNC,
    SIG_NGX_CONF_INCLUDEFUNC,
    SIG_NGX_HTTP_CORE_TYPESFUNC,
    SIG_NGX_HTTP_CORE_TYPEFUNC,
    SIG_NGX_CONF_SET_STR_SLOTFUNC,
    SIG_NGX_HTTP_CORE_KEEPALIVEFUNC,
    SIG_NGX_HTTP_CORE_SERVERFUNC,
    SIG_NGX_HTTP_CORE_LISTENFUNC,
    SIG_NGX_HTTP_CORE_SERVER_NAMEFUNC,
    SIG_NGX_HTTP_SET_COMPLEX_VALUE_SLOTFUNC,
    SIG_NGX_HTTP_AUTH_BASIC_USER_FILEFUNC,
    SIG_NGX_HTTP_CORE_LOCATIONFUNC,
    SIG_NGX_HTTP_CORE_ROOTFUNC,
    SIG_NGX_HTTP_INDEX_SET_INDEXFUNC,
    SIG_NGX_HTTP_CORE_ERROR_PAGEFUNC,
    SIG_NGX_HTTP_CORE_INIT_MAIN_CONFFUNC,
    SIG_NGX_HTTP_CORE_MERGE_SRV_CONFFUNC,
    SIG_NGX_HTTP_CORE_MERGE_LOC_CONFFUNC,
    SIG_NGX_HTTP_LOG_MERGE_LOC_CONFFUNC,
    SIG_NGX_HTTP_UPSTREAM_INIT_MAIN_CONFFUNC,
    SIG_NGX_HTTP_INDEX_MERGE_LOC_CONFFUNC,
    SIG_NGX_HTTP_MIRROR_MERGE_LOC_CONFFUNC,
    SIG_NGX_HTTP_AUTH_BASIC_MERGE_LOC_CONFFUNC,
    SIG_NGX_HTTP_HEADERS_MERGE_CONFFUNC,
    SIG_NGX_HTTP_COPY_FILTER_MERGE_CONFFUNC,
    SIG_NGX_HTTP_CMP_LOCATIONSFUNC,
    SIG_NGX_HTTP_CORE_POSTCONFIGURATIONFUNC,
    SIG_NGX_HTTP_LOG_INITFUNC,
    SIG_NGX_HTTP_STATIC_INITFUNC,
    SIG_NGX_HTTP_INDEX_INITFUNC,
    SIG_NGX_HTTP_MIRROR_INITFUNC,
    SIG_NGX_HTTP_TRY_FILES_INITFUNC,
    SIG_NGX_HTTP_AUTH_BASIC_INITFUNC,
    SIG_NGX_HTTP_WRITE_FILTER_INITFUNC,
    SIG_NGX_HTTP_HEADER_FILTER_INITFUNC,
    SIG_NGX_HTTP_CHUNKED_FILTER_INITFUNC,
    SIG_NGX_HTTP_RANGE_HEADER_FILTER_INITFUNC,
    SIG_NGX_HTTP_HEADERS_FILTER_INITFUNC,
    SIG_NGX_HTTP_COPY_FILTER_INITFUNC,
    SIG_NGX_HTTP_RANGE_BODY_FILTER_INITFUNC,
    SIG_NGX_HTTP_NOT_MODIFIED_FILTER_INITFUNC,
    SIG_NGX_CORE_MODULE_INIT_CONFFUNC,
    SIG_NGX_EVENT_INIT_CONFFUNC,
    SIG_NGX_EVENT_MODULE_INITFUNC,
    SIG_NGX_WORKER_PROCESS_CYCLEFUNC,
    SIG_NGX_EVENT_PROCESS_INITFUNC,
    SIG_NGX_EPOLL_INITFUNC,
    SIG_NGX_EPOLL_ADD_EVENTFUNC,
    SIG_NGX_EPOLL_PROCESS_EVENTSFUNC,
    SIG_NGX_EVENT_ACCEPTFUNC,
    SIG_NGX_HTTP_INIT_CONNECTIONFUNC,
    SIG_NGX_HTTP_WAIT_REQUEST_HANDLERFUNC,
    SIG_NGX_UNIX_RECVFUNC,
    SIG_NGX_HTTP_PROCESS_HOSTFUNC,
    SIG_NGX_HTTP_PROCESS_UNIQUE_HEADER_LINEFUNC,
    SIG_NGX_HTTP_PROCESS_USER_AGENTFUNC,
    SIG_NGX_HTTP_CORE_FIND_CONFIG_PHASEFUNC,
    SIG_NGX_HTTP_CORE_ACCESS_PHASEFUNC,
    SIG_NGX_HTTP_AUTH_BASIC_HANDLERFUNC,
    SIG_NGX_HTTP_CORE_POST_ACCESS_PHASEFUNC,
    SIG_NGX_HTTP_CORE_GENERIC_PHASEFUNC,
    SIG_NGX_HTTP_TRY_FILES_HANDLERFUNC,
    SIG_NGX_HTTP_MIRROR_HANDLERFUNC,
    SIG_NGX_HTTP_CORE_CONTENT_PHASEFUNC,
    SIG_NGX_HTTP_INDEX_HANDLERFUNC,
    SIG_NGX_HTTP_STATIC_HANDLERFUNC,
    SIG_NGX_HTTP_NOT_MODIFIED_HEADER_FILTERFUNC,
    SIG_NGX_HTTP_HEADERS_FILTERFUNC,
    SIG_NGX_HTTP_RANGE_HEADER_FILTERFUNC,
    SIG_NGX_HTTP_CHUNKED_HEADER_FILTERFUNC,
    SIG_NGX_HTTP_HEADER_FILTERFUNC,
    SIG_NGX_HTTP_RANGE_BODY_FILTERFUNC,
    SIG_NGX_HTTP_COPY_FILTERFUNC,
    SIG_NGX_HTTP_TRAILERS_FILTERFUNC,
    SIG_NGX_HTTP_CHUNKED_BODY_FILTERFUNC,
    SIG_NGX_HTTP_WRITE_FILTERFUNC,
    SIG_NGX_LINUX_SENDFILE_CHAINFUNC,
    SIG_NGX_HTTP_LOG_HANDLERFUNC,
    SIG_NGX_HTTP_LOG_VARIABLE_GETLENFUNC,
    SIG_NGX_HTTP_VARIABLE_REMOTE_ADDRFUNC,
    SIG_NGX_HTTP_VARIABLE_REMOTE_USERFUNC,
    SIG_NGX_HTTP_VARIABLE_REQUEST_LINEFUNC,
    SIG_NGX_HTTP_VARIABLE_HEADERFUNC,
    SIG_NGX_HTTP_LOG_VARIABLEFUNC,
    SIG_NGX_HTTP_LOG_COPY_SHORTFUNC,
    SIG_NGX_HTTP_LOG_TIMEFUNC,
    SIG_NGX_HTTP_LOG_STATUSFUNC,
    SIG_NGX_HTTP_LOG_BODY_BYTES_SENTFUNC,
    SIG_NGX_POOL_CLEANUP_FILEFUNC,
    SIG_NGX_HTTP_KEEPALIVE_HANDLERFUNC,
    SIG_NGX_EPOLL_DEL_CONNECTIONFUNC,
    SIG_NGX_SIGNAL_HANDLERFUNC,
    SIG_NGX_CONF_FLUSH_FILESFUNC,
    SIG_NGX_CLEANUP_ENVIRONMENTFUNC,
    SIG_NGX_RESOLVER_CLEANUPFUNC,
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
