#include "thttpd_rpc_wrapper.h"
extern CLIENT *clnt;
int auth_check2_sense_1_wrapper(int *auth_check2_sense_1_ret, httpd_conn *hc, char *dirname)
{
    auth_check2_sense_1_ret_ *result_rpc;
    int sensefun_ret;
int_ptr auth_check2_sense_1_ret_rpc;
if(auth_check2_sense_1_ret!=NULL) {
auth_check2_sense_1_ret_rpc.int_ptr_len=1;
auth_check2_sense_1_ret_rpc.int_ptr_val=malloc(sizeof(int)*auth_check2_sense_1_ret_rpc.int_ptr_len);
if(auth_check2_sense_1_ret_rpc.int_ptr_val==NULL) {
perror("malloc failed");
exit(1);
}
for(int i0=0;i0<auth_check2_sense_1_ret_rpc.int_ptr_len;i0++) {
auth_check2_sense_1_ret_rpc.int_ptr_val[i0]=auth_check2_sense_1_ret[i0];
	}
}else{auth_check2_sense_1_ret_rpc.int_ptr_len=0;auth_check2_sense_1_ret_rpc.int_ptr_val=NULL;}
httpd_conn_rpc_ptr hc_rpc_ptr;
if(hc==NULL) {
hc_rpc_ptr.httpd_conn_rpc_ptr_len=0;
hc_rpc_ptr.httpd_conn_rpc_ptr_val=NULL;
} else {
hc_rpc_ptr.httpd_conn_rpc_ptr_len=1;
hc_rpc_ptr.httpd_conn_rpc_ptr_val=malloc(1*sizeof(httpd_conn_rpc));
if(hc_rpc_ptr.httpd_conn_rpc_ptr_val==NULL) {
perror("malloc failed");
exit(1);
}
for(int i0=0;i0<hc_rpc_ptr.httpd_conn_rpc_ptr_len;i0++) {
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].initialized=hc[i0].initialized;
if (hc[i0].hs == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_len = 1;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_len * sizeof(httpd_server_rpc));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	for (int i1 = 0; i1 < hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_len; i1++) {
if (hc[i0].hs[i1].binding_hostname == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_len = strlen(hc[i0].hs[i1].binding_hostname) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_val, hc[i0].hs[i1].binding_hostname, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_len);
}
if (hc[i0].hs[i1].server_hostname == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_len = strlen(hc[i0].hs[i1].server_hostname) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_val, hc[i0].hs[i1].server_hostname, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_len);
}
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].port=hc[i0].hs[i1].port;
if (hc[i0].hs[i1].cgi_pattern == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_len = strlen(hc[i0].hs[i1].cgi_pattern) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_val, hc[i0].hs[i1].cgi_pattern, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_len);
}
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_limit=hc[i0].hs[i1].cgi_limit;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_count=hc[i0].hs[i1].cgi_count;
if (hc[i0].hs[i1].charset == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_len = strlen(hc[i0].hs[i1].charset) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_val, hc[i0].hs[i1].charset, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_len);
}
if (hc[i0].hs[i1].p3p == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_len = strlen(hc[i0].hs[i1].p3p) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_val, hc[i0].hs[i1].p3p, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_len);
}
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].max_age=hc[i0].hs[i1].max_age;
if (hc[i0].hs[i1].cwd == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_len = strlen(hc[i0].hs[i1].cwd) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_val, hc[i0].hs[i1].cwd, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_len);
}
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].listen4_fd=hc[i0].hs[i1].listen4_fd;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].listen6_fd=hc[i0].hs[i1].listen6_fd;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].no_log=hc[i0].hs[i1].no_log;
if (hc[i0].hs[i1].logfp == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_len = strlen(hc[i0].hs[i1].logfp) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_val, hc[i0].hs[i1].logfp, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_len);
}
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].no_symlink_check=hc[i0].hs[i1].no_symlink_check;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].vhost=hc[i0].hs[i1].vhost;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].global_passwd=hc[i0].hs[i1].global_passwd;
if (hc[i0].hs[i1].url_pattern == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_len = strlen(hc[i0].hs[i1].url_pattern) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_val, hc[i0].hs[i1].url_pattern, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_len);
}
if (hc[i0].hs[i1].local_pattern == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_len = strlen(hc[i0].hs[i1].local_pattern) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_val, hc[i0].hs[i1].local_pattern, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_len);
}
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].no_empty_referrers=hc[i0].hs[i1].no_empty_referrers;
	}
}
if (hc[i0].read_buf == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_len = strlen(hc[i0].read_buf) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_val, hc[i0].read_buf, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_len);
}
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].read_size=hc[i0].read_size;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].read_idx=hc[i0].read_idx;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].checked_idx=hc[i0].checked_idx;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].checked_state=hc[i0].checked_state;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].method=hc[i0].method;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].status=hc[i0].status;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].bytes_to_send=hc[i0].bytes_to_send;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].bytes_sent=hc[i0].bytes_sent;
if (hc[i0].encodedurl == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_len = strlen(hc[i0].encodedurl) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_val, hc[i0].encodedurl, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_len);
}
if (hc[i0].decodedurl == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_len = strlen(hc[i0].decodedurl) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_val, hc[i0].decodedurl, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_len);
}
if (hc[i0].protocol == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_len = strlen(hc[i0].protocol) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_val, hc[i0].protocol, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_len);
}
if (hc[i0].origfilename == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_len = strlen(hc[i0].origfilename) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_val, hc[i0].origfilename, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_len);
}
if (hc[i0].expnfilename == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_len = strlen(hc[i0].expnfilename) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_val, hc[i0].expnfilename, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_len);
}
if (hc[i0].encodings == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_len = strlen(hc[i0].encodings) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_val, hc[i0].encodings, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_len);
}
if (hc[i0].pathinfo == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_len = strlen(hc[i0].pathinfo) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_val, hc[i0].pathinfo, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_len);
}
if (hc[i0].query == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_len = strlen(hc[i0].query) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_val, hc[i0].query, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_len);
}
if (hc[i0].referrer == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_len = strlen(hc[i0].referrer) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_val, hc[i0].referrer, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_len);
}
if (hc[i0].useragent == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_len = strlen(hc[i0].useragent) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_val, hc[i0].useragent, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_len);
}
if (hc[i0].accept == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_len = strlen(hc[i0].accept) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_val, hc[i0].accept, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_len);
}
if (hc[i0].accepte == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_len = strlen(hc[i0].accepte) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_val, hc[i0].accepte, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_len);
}
if (hc[i0].acceptl == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_len = strlen(hc[i0].acceptl) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_val, hc[i0].acceptl, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_len);
}
if (hc[i0].cookie == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_len = strlen(hc[i0].cookie) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_val, hc[i0].cookie, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_len);
}
if (hc[i0].contenttype == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_len = strlen(hc[i0].contenttype) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_val, hc[i0].contenttype, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_len);
}
if (hc[i0].reqhost == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_len = strlen(hc[i0].reqhost) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_val, hc[i0].reqhost, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_len);
}
if (hc[i0].hdrhost == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_len = strlen(hc[i0].hdrhost) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_val, hc[i0].hdrhost, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_len);
}
if (hc[i0].hostdir == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_len = strlen(hc[i0].hostdir) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_val, hc[i0].hostdir, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_len);
}
if (hc[i0].authorization == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_len = strlen(hc[i0].authorization) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_val, hc[i0].authorization, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_len);
}
if (hc[i0].remoteuser == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_len = strlen(hc[i0].remoteuser) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_val, hc[i0].remoteuser, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_len);
}
if (hc[i0].response == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_len = strlen(hc[i0].response) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_val, hc[i0].response, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_len);
}
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].maxdecodedurl=hc[i0].maxdecodedurl;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].maxorigfilename=hc[i0].maxorigfilename;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].maxexpnfilename=hc[i0].maxexpnfilename;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].maxencodings=hc[i0].maxencodings;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].maxpathinfo=hc[i0].maxpathinfo;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].maxquery=hc[i0].maxquery;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].maxaccept=hc[i0].maxaccept;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].maxaccepte=hc[i0].maxaccepte;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].maxreqhost=hc[i0].maxreqhost;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].maxhostdir=hc[i0].maxhostdir;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].maxremoteuser=hc[i0].maxremoteuser;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].maxresponse=hc[i0].maxresponse;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].responselen=hc[i0].responselen;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].if_modified_since=hc[i0].if_modified_since;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].range_if=hc[i0].range_if;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].contentlength=hc[i0].contentlength;
if (hc[i0].type == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_len = strlen(hc[i0].type) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_val, hc[i0].type, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_len);
}
if (hc[i0].hostname == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_len = strlen(hc[i0].hostname) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_val, hc[i0].hostname, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_len);
}
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].mime_flag=hc[i0].mime_flag;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].one_one=hc[i0].one_one;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].got_range=hc[i0].got_range;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].tildemapped=hc[i0].tildemapped;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].first_byte_index=hc[i0].first_byte_index;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].last_byte_index=hc[i0].last_byte_index;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].keep_alive=hc[i0].keep_alive;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].should_linger=hc[i0].should_linger;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].sb.st_dev=hc[i0].sb.st_dev;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].sb.st_ino=hc[i0].sb.st_ino;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].sb.st_mode=hc[i0].sb.st_mode;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].sb.st_nlink=hc[i0].sb.st_nlink;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].sb.st_uid=hc[i0].sb.st_uid;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].sb.st_gid=hc[i0].sb.st_gid;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].sb.st_rdev=hc[i0].sb.st_rdev;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].sb.st_size=hc[i0].sb.st_size;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].sb.st_blksize=hc[i0].sb.st_blksize;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].sb.st_blocks=hc[i0].sb.st_blocks;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].sb.st_atime=hc[i0].sb.st_atime;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].sb.st_mtime=hc[i0].sb.st_mtime;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].sb.st_ctime=hc[i0].sb.st_ctime;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].conn_fd=hc[i0].conn_fd;
if (hc[i0].file_address == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_len = strlen(hc[i0].file_address) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_val, hc[i0].file_address, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_len);
}
}
}
char_ptr dirname_rpc;
if(dirname==NULL) dirname_rpc.charptr.charptr_len=0;else dirname_rpc.charptr.charptr_len=strlen(dirname)+1;
dirname_rpc.charptr.charptr_val=malloc(dirname_rpc.charptr.charptr_len);
if(dirname_rpc.charptr.charptr_val==NULL) {
perror("malloc failed");
exit(1);
}
memcpy(dirname_rpc.charptr.charptr_val,dirname,dirname_rpc.charptr.charptr_len);
result_rpc=auth_check2_sense_1_rpc_1(auth_check2_sense_1_ret_rpc,hc_rpc_ptr,dirname_rpc,clnt);
if(result_rpc==(auth_check2_sense_1_ret_ *)NULL) {
clnt_perror(clnt,"call failed");
}
sensefun_ret=result_rpc->ret0;
if(result_rpc->auth_check2_sense_1_ret.int_ptr_val==NULL) {
auth_check2_sense_1_ret=NULL;
} else {
auth_check2_sense_1_ret=(int *)malloc(result_rpc->auth_check2_sense_1_ret.int_ptr_len*sizeof(int));
if(auth_check2_sense_1_ret==NULL) {
perror("malloc failed");
exit(1);
}
for(int i0=0;i0<result_rpc->auth_check2_sense_1_ret.int_ptr_len;i0++) {
auth_check2_sense_1_ret[i0]=result_rpc->auth_check2_sense_1_ret.int_ptr_val[i0];
}
}
    if(result_rpc->hc.httpd_conn_rpc_ptr_val==NULL) {
        hc=NULL;
    } else {
        hc=malloc(result_rpc->hc.httpd_conn_rpc_ptr_len*sizeof(struct httpd_conn));
        if(hc==NULL) {
            perror("malloc failed");
            exit(1);
        }
        for(int i0=0;i0<result_rpc->hc.httpd_conn_rpc_ptr_len;i0++) {
hc[i0].initialized=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].initialized;
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val == NULL) {hc[i0].hs = NULL;}
else {
	hc[i0].hs = (httpd_server *)malloc(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_len * sizeof(httpd_server));
	if (hc[i0].hs == NULL) {
		perror("malloc failed");
		exit(1);
	}
	for (int i1 = 0; i1 < result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_len; i1++) {
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_val == NULL) {hc[i0].hs[i1].binding_hostname = NULL;}
else {hc[i0].hs[i1].binding_hostname = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_val == NULL) {hc[i0].hs[i1].server_hostname = NULL;}
else {hc[i0].hs[i1].server_hostname = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_val);}
hc[i0].hs[i1].port=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].port;
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_val == NULL) {hc[i0].hs[i1].cgi_pattern = NULL;}
else {hc[i0].hs[i1].cgi_pattern = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_val);}
hc[i0].hs[i1].cgi_limit=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_limit;
hc[i0].hs[i1].cgi_count=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_count;
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_val == NULL) {hc[i0].hs[i1].charset = NULL;}
else {hc[i0].hs[i1].charset = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_val == NULL) {hc[i0].hs[i1].p3p = NULL;}
else {hc[i0].hs[i1].p3p = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_val);}
hc[i0].hs[i1].max_age=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].max_age;
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_val == NULL) {hc[i0].hs[i1].cwd = NULL;}
else {hc[i0].hs[i1].cwd = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_val);}
hc[i0].hs[i1].listen4_fd=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].listen4_fd;
hc[i0].hs[i1].listen6_fd=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].listen6_fd;
hc[i0].hs[i1].no_log=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].no_log;
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_val == NULL) {hc[i0].hs[i1].logfp = NULL;}
else {hc[i0].hs[i1].logfp = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_val);}
hc[i0].hs[i1].no_symlink_check=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].no_symlink_check;
hc[i0].hs[i1].vhost=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].vhost;
hc[i0].hs[i1].global_passwd=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].global_passwd;
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_val == NULL) {hc[i0].hs[i1].url_pattern = NULL;}
else {hc[i0].hs[i1].url_pattern = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_val == NULL) {hc[i0].hs[i1].local_pattern = NULL;}
else {hc[i0].hs[i1].local_pattern = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_val);}
hc[i0].hs[i1].no_empty_referrers=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].no_empty_referrers;
	}
}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_val == NULL) {hc[i0].read_buf = NULL;}
else {hc[i0].read_buf = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_val);}
hc[i0].read_size=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].read_size;
hc[i0].read_idx=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].read_idx;
hc[i0].checked_idx=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].checked_idx;
hc[i0].checked_state=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].checked_state;
hc[i0].method=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].method;
hc[i0].status=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].status;
hc[i0].bytes_to_send=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].bytes_to_send;
hc[i0].bytes_sent=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].bytes_sent;
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_val == NULL) {hc[i0].encodedurl = NULL;}
else {hc[i0].encodedurl = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_val == NULL) {hc[i0].decodedurl = NULL;}
else {hc[i0].decodedurl = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_val == NULL) {hc[i0].protocol = NULL;}
else {hc[i0].protocol = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_val == NULL) {hc[i0].origfilename = NULL;}
else {hc[i0].origfilename = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_val == NULL) {hc[i0].expnfilename = NULL;}
else {hc[i0].expnfilename = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_val == NULL) {hc[i0].encodings = NULL;}
else {hc[i0].encodings = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_val == NULL) {hc[i0].pathinfo = NULL;}
else {hc[i0].pathinfo = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_val == NULL) {hc[i0].query = NULL;}
else {hc[i0].query = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_val == NULL) {hc[i0].referrer = NULL;}
else {hc[i0].referrer = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_val == NULL) {hc[i0].useragent = NULL;}
else {hc[i0].useragent = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_val == NULL) {hc[i0].accept = NULL;}
else {hc[i0].accept = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_val == NULL) {hc[i0].accepte = NULL;}
else {hc[i0].accepte = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_val == NULL) {hc[i0].acceptl = NULL;}
else {hc[i0].acceptl = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_val == NULL) {hc[i0].cookie = NULL;}
else {hc[i0].cookie = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_val == NULL) {hc[i0].contenttype = NULL;}
else {hc[i0].contenttype = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_val == NULL) {hc[i0].reqhost = NULL;}
else {hc[i0].reqhost = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_val == NULL) {hc[i0].hdrhost = NULL;}
else {hc[i0].hdrhost = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_val == NULL) {hc[i0].hostdir = NULL;}
else {hc[i0].hostdir = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_val == NULL) {hc[i0].authorization = NULL;}
else {hc[i0].authorization = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_val == NULL) {hc[i0].remoteuser = NULL;}
else {hc[i0].remoteuser = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_val == NULL) {hc[i0].response = NULL;}
else {hc[i0].response = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_val);}
hc[i0].maxdecodedurl=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].maxdecodedurl;
hc[i0].maxorigfilename=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].maxorigfilename;
hc[i0].maxexpnfilename=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].maxexpnfilename;
hc[i0].maxencodings=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].maxencodings;
hc[i0].maxpathinfo=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].maxpathinfo;
hc[i0].maxquery=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].maxquery;
hc[i0].maxaccept=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].maxaccept;
hc[i0].maxaccepte=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].maxaccepte;
hc[i0].maxreqhost=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].maxreqhost;
hc[i0].maxhostdir=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].maxhostdir;
hc[i0].maxremoteuser=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].maxremoteuser;
hc[i0].maxresponse=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].maxresponse;
hc[i0].responselen=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].responselen;
hc[i0].if_modified_since=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].if_modified_since;
hc[i0].range_if=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].range_if;
hc[i0].contentlength=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].contentlength;
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_val == NULL) {hc[i0].type = NULL;}
else {hc[i0].type = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_val == NULL) {hc[i0].hostname = NULL;}
else {hc[i0].hostname = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_val);}
hc[i0].mime_flag=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].mime_flag;
hc[i0].one_one=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].one_one;
hc[i0].got_range=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].got_range;
hc[i0].tildemapped=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].tildemapped;
hc[i0].first_byte_index=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].first_byte_index;
hc[i0].last_byte_index=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].last_byte_index;
hc[i0].keep_alive=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].keep_alive;
hc[i0].should_linger=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].should_linger;
hc[i0].sb.st_dev=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].sb.st_dev;
hc[i0].sb.st_ino=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].sb.st_ino;
hc[i0].sb.st_mode=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].sb.st_mode;
hc[i0].sb.st_nlink=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].sb.st_nlink;
hc[i0].sb.st_uid=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].sb.st_uid;
hc[i0].sb.st_gid=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].sb.st_gid;
hc[i0].sb.st_rdev=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].sb.st_rdev;
hc[i0].sb.st_size=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].sb.st_size;
hc[i0].sb.st_blksize=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].sb.st_blksize;
hc[i0].sb.st_blocks=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].sb.st_blocks;
hc[i0].sb.st_atim=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].sb.st_atim;
hc[i0].sb.st_mtim=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].sb.st_mtim;
hc[i0].sb.st_ctim=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].sb.st_ctim;
hc[i0].conn_fd=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].conn_fd;
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_val == NULL) {hc[i0].file_address = NULL;}
else {hc[i0].file_address = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_val);}
        }
    }
if(result_rpc->dirname.charptr.charptr_val==NULL) dirname=NULL;else{

dirname=malloc(result_rpc->dirname.charptr.charptr_len);
memcpy(dirname,result_rpc->dirname.charptr.charptr_val,result_rpc->dirname.charptr.charptr_len);
}
return sensefun_ret;
}
void make_log_entry_sense_1_wrapper(char *ru, char url[305], char bytes[40], httpd_conn *hc, struct timeval *nowP)
{
    make_log_entry_sense_1_ret_ *result_rpc;
char_ptr ru_rpc;
if(ru==NULL) ru_rpc.charptr.charptr_len=0;else ru_rpc.charptr.charptr_len=strlen(ru)+1;
ru_rpc.charptr.charptr_val=malloc(ru_rpc.charptr.charptr_len);
if(ru_rpc.charptr.charptr_val==NULL) {
perror("malloc failed");
exit(1);
}
memcpy(ru_rpc.charptr.charptr_val,ru,ru_rpc.charptr.charptr_len);
make_log_entry_sense_1_url url_rpc;
memcpy(url_rpc.url, url, 305);
make_log_entry_sense_1_bytes bytes_rpc;
memcpy(bytes_rpc.bytes, bytes, 40);
httpd_conn_rpc_ptr hc_rpc_ptr;
if(hc==NULL) {
hc_rpc_ptr.httpd_conn_rpc_ptr_len=0;
hc_rpc_ptr.httpd_conn_rpc_ptr_val=NULL;
} else {
hc_rpc_ptr.httpd_conn_rpc_ptr_len=1;
hc_rpc_ptr.httpd_conn_rpc_ptr_val=malloc(1*sizeof(httpd_conn_rpc));
if(hc_rpc_ptr.httpd_conn_rpc_ptr_val==NULL) {
perror("malloc failed");
exit(1);
}
for(int i0=0;i0<hc_rpc_ptr.httpd_conn_rpc_ptr_len;i0++) {
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].initialized=hc[i0].initialized;
if (hc[i0].hs == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_len = 1;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_len * sizeof(httpd_server_rpc));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	for (int i1 = 0; i1 < hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_len; i1++) {
if (hc[i0].hs[i1].binding_hostname == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_len = strlen(hc[i0].hs[i1].binding_hostname) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_val, hc[i0].hs[i1].binding_hostname, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_len);
}
if (hc[i0].hs[i1].server_hostname == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_len = strlen(hc[i0].hs[i1].server_hostname) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_val, hc[i0].hs[i1].server_hostname, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_len);
}
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].port=hc[i0].hs[i1].port;
if (hc[i0].hs[i1].cgi_pattern == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_len = strlen(hc[i0].hs[i1].cgi_pattern) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_val, hc[i0].hs[i1].cgi_pattern, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_len);
}
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_limit=hc[i0].hs[i1].cgi_limit;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_count=hc[i0].hs[i1].cgi_count;
if (hc[i0].hs[i1].charset == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_len = strlen(hc[i0].hs[i1].charset) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_val, hc[i0].hs[i1].charset, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_len);
}
if (hc[i0].hs[i1].p3p == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_len = strlen(hc[i0].hs[i1].p3p) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_val, hc[i0].hs[i1].p3p, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_len);
}
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].max_age=hc[i0].hs[i1].max_age;
if (hc[i0].hs[i1].cwd == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_len = strlen(hc[i0].hs[i1].cwd) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_val, hc[i0].hs[i1].cwd, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_len);
}
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].listen4_fd=hc[i0].hs[i1].listen4_fd;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].listen6_fd=hc[i0].hs[i1].listen6_fd;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].no_log=hc[i0].hs[i1].no_log;
if (hc[i0].hs[i1].logfp == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_len = strlen(hc[i0].hs[i1].logfp) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_val, hc[i0].hs[i1].logfp, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_len);
}
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].no_symlink_check=hc[i0].hs[i1].no_symlink_check;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].vhost=hc[i0].hs[i1].vhost;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].global_passwd=hc[i0].hs[i1].global_passwd;
if (hc[i0].hs[i1].url_pattern == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_len = strlen(hc[i0].hs[i1].url_pattern) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_val, hc[i0].hs[i1].url_pattern, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_len);
}
if (hc[i0].hs[i1].local_pattern == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_len = strlen(hc[i0].hs[i1].local_pattern) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_val, hc[i0].hs[i1].local_pattern, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_len);
}
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].no_empty_referrers=hc[i0].hs[i1].no_empty_referrers;
	}
}
if (hc[i0].read_buf == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_len = strlen(hc[i0].read_buf) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_val, hc[i0].read_buf, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_len);
}
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].read_size=hc[i0].read_size;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].read_idx=hc[i0].read_idx;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].checked_idx=hc[i0].checked_idx;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].checked_state=hc[i0].checked_state;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].method=hc[i0].method;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].status=hc[i0].status;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].bytes_to_send=hc[i0].bytes_to_send;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].bytes_sent=hc[i0].bytes_sent;
if (hc[i0].encodedurl == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_len = strlen(hc[i0].encodedurl) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_val, hc[i0].encodedurl, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_len);
}
if (hc[i0].decodedurl == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_len = strlen(hc[i0].decodedurl) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_val, hc[i0].decodedurl, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_len);
}
if (hc[i0].protocol == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_len = strlen(hc[i0].protocol) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_val, hc[i0].protocol, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_len);
}
if (hc[i0].origfilename == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_len = strlen(hc[i0].origfilename) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_val, hc[i0].origfilename, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_len);
}
if (hc[i0].expnfilename == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_len = strlen(hc[i0].expnfilename) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_val, hc[i0].expnfilename, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_len);
}
if (hc[i0].encodings == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_len = strlen(hc[i0].encodings) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_val, hc[i0].encodings, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_len);
}
if (hc[i0].pathinfo == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_len = strlen(hc[i0].pathinfo) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_val, hc[i0].pathinfo, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_len);
}
if (hc[i0].query == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_len = strlen(hc[i0].query) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_val, hc[i0].query, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_len);
}
if (hc[i0].referrer == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_len = strlen(hc[i0].referrer) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_val, hc[i0].referrer, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_len);
}
if (hc[i0].useragent == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_len = strlen(hc[i0].useragent) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_val, hc[i0].useragent, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_len);
}
if (hc[i0].accept == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_len = strlen(hc[i0].accept) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_val, hc[i0].accept, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_len);
}
if (hc[i0].accepte == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_len = strlen(hc[i0].accepte) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_val, hc[i0].accepte, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_len);
}
if (hc[i0].acceptl == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_len = strlen(hc[i0].acceptl) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_val, hc[i0].acceptl, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_len);
}
if (hc[i0].cookie == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_len = strlen(hc[i0].cookie) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_val, hc[i0].cookie, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_len);
}
if (hc[i0].contenttype == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_len = strlen(hc[i0].contenttype) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_val, hc[i0].contenttype, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_len);
}
if (hc[i0].reqhost == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_len = strlen(hc[i0].reqhost) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_val, hc[i0].reqhost, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_len);
}
if (hc[i0].hdrhost == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_len = strlen(hc[i0].hdrhost) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_val, hc[i0].hdrhost, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_len);
}
if (hc[i0].hostdir == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_len = strlen(hc[i0].hostdir) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_val, hc[i0].hostdir, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_len);
}
if (hc[i0].authorization == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_len = strlen(hc[i0].authorization) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_val, hc[i0].authorization, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_len);
}
if (hc[i0].remoteuser == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_len = strlen(hc[i0].remoteuser) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_val, hc[i0].remoteuser, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_len);
}
if (hc[i0].response == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_len = strlen(hc[i0].response) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_val, hc[i0].response, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_len);
}
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].maxdecodedurl=hc[i0].maxdecodedurl;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].maxorigfilename=hc[i0].maxorigfilename;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].maxexpnfilename=hc[i0].maxexpnfilename;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].maxencodings=hc[i0].maxencodings;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].maxpathinfo=hc[i0].maxpathinfo;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].maxquery=hc[i0].maxquery;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].maxaccept=hc[i0].maxaccept;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].maxaccepte=hc[i0].maxaccepte;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].maxreqhost=hc[i0].maxreqhost;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].maxhostdir=hc[i0].maxhostdir;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].maxremoteuser=hc[i0].maxremoteuser;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].maxresponse=hc[i0].maxresponse;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].responselen=hc[i0].responselen;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].if_modified_since=hc[i0].if_modified_since;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].range_if=hc[i0].range_if;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].contentlength=hc[i0].contentlength;
if (hc[i0].type == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_len = strlen(hc[i0].type) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_val, hc[i0].type, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_len);
}
if (hc[i0].hostname == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_len = strlen(hc[i0].hostname) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_val, hc[i0].hostname, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_len);
}
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].mime_flag=hc[i0].mime_flag;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].one_one=hc[i0].one_one;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].got_range=hc[i0].got_range;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].tildemapped=hc[i0].tildemapped;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].first_byte_index=hc[i0].first_byte_index;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].last_byte_index=hc[i0].last_byte_index;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].keep_alive=hc[i0].keep_alive;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].should_linger=hc[i0].should_linger;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].sb.st_dev=hc[i0].sb.st_dev;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].sb.st_ino=hc[i0].sb.st_ino;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].sb.st_mode=hc[i0].sb.st_mode;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].sb.st_nlink=hc[i0].sb.st_nlink;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].sb.st_uid=hc[i0].sb.st_uid;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].sb.st_gid=hc[i0].sb.st_gid;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].sb.st_rdev=hc[i0].sb.st_rdev;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].sb.st_size=hc[i0].sb.st_size;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].sb.st_blksize=hc[i0].sb.st_blksize;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].sb.st_blocks=hc[i0].sb.st_blocks;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].sb.st_atime=hc[i0].sb.st_atime;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].sb.st_mtime=hc[i0].sb.st_mtime;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].sb.st_ctime=hc[i0].sb.st_ctime;
hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].conn_fd=hc[i0].conn_fd;
if (hc[i0].file_address == NULL) {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_len = 0;hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_val = NULL;}
else {hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_len = strlen(hc[i0].file_address) + 1;

	hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_val = malloc(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_len * sizeof(char));
	if (hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_val, hc[i0].file_address, hc_rpc_ptr.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_len);
}
}
}
struct timeval_rpc_ptr nowP_rpc;
if(nowP==NULL) {
nowP_rpc.timeval_rpc_ptr_len=0;
nowP_rpc.timeval_rpc_ptr_val=NULL;
} else {
nowP_rpc.timeval_rpc_ptr_len=1;
nowP_rpc.timeval_rpc_ptr_val=malloc(sizeof(struct timeval));
if(nowP_rpc.timeval_rpc_ptr_val==NULL) {
perror("malloc failed");
exit(1);
}
memcpy(nowP_rpc.timeval_rpc_ptr_val,nowP,sizeof(struct timeval));
}
result_rpc=make_log_entry_sense_1_rpc_1(ru_rpc,url_rpc,bytes_rpc,hc_rpc_ptr,nowP_rpc,clnt);
if(result_rpc==(make_log_entry_sense_1_ret_ *)NULL) {
clnt_perror(clnt,"call failed");
}
if(result_rpc->ru.charptr.charptr_val==NULL) ru=NULL;else{

ru=malloc(result_rpc->ru.charptr.charptr_len);
memcpy(ru,result_rpc->ru.charptr.charptr_val,result_rpc->ru.charptr.charptr_len);
}
memcpy(url, result_rpc->url.url, 305);
memcpy(bytes, result_rpc->bytes.bytes, 40);
    if(result_rpc->hc.httpd_conn_rpc_ptr_val==NULL) {
        hc=NULL;
    } else {
        hc=malloc(result_rpc->hc.httpd_conn_rpc_ptr_len*sizeof(struct httpd_conn));
        if(hc==NULL) {
            perror("malloc failed");
            exit(1);
        }
        for(int i0=0;i0<result_rpc->hc.httpd_conn_rpc_ptr_len;i0++) {
hc[i0].initialized=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].initialized;
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val == NULL) {hc[i0].hs = NULL;}
else {
	hc[i0].hs = (httpd_server *)malloc(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_len * sizeof(httpd_server));
	if (hc[i0].hs == NULL) {
		perror("malloc failed");
		exit(1);
	}
	for (int i1 = 0; i1 < result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_len; i1++) {
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_val == NULL) {hc[i0].hs[i1].binding_hostname = NULL;}
else {hc[i0].hs[i1].binding_hostname = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_val == NULL) {hc[i0].hs[i1].server_hostname = NULL;}
else {hc[i0].hs[i1].server_hostname = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_val);}
hc[i0].hs[i1].port=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].port;
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_val == NULL) {hc[i0].hs[i1].cgi_pattern = NULL;}
else {hc[i0].hs[i1].cgi_pattern = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_val);}
hc[i0].hs[i1].cgi_limit=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_limit;
hc[i0].hs[i1].cgi_count=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_count;
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_val == NULL) {hc[i0].hs[i1].charset = NULL;}
else {hc[i0].hs[i1].charset = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_val == NULL) {hc[i0].hs[i1].p3p = NULL;}
else {hc[i0].hs[i1].p3p = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_val);}
hc[i0].hs[i1].max_age=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].max_age;
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_val == NULL) {hc[i0].hs[i1].cwd = NULL;}
else {hc[i0].hs[i1].cwd = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_val);}
hc[i0].hs[i1].listen4_fd=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].listen4_fd;
hc[i0].hs[i1].listen6_fd=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].listen6_fd;
hc[i0].hs[i1].no_log=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].no_log;
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_val == NULL) {hc[i0].hs[i1].logfp = NULL;}
else {hc[i0].hs[i1].logfp = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_val);}
hc[i0].hs[i1].no_symlink_check=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].no_symlink_check;
hc[i0].hs[i1].vhost=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].vhost;
hc[i0].hs[i1].global_passwd=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].global_passwd;
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_val == NULL) {hc[i0].hs[i1].url_pattern = NULL;}
else {hc[i0].hs[i1].url_pattern = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_val == NULL) {hc[i0].hs[i1].local_pattern = NULL;}
else {hc[i0].hs[i1].local_pattern = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_val);}
hc[i0].hs[i1].no_empty_referrers=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].no_empty_referrers;
	}
}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_val == NULL) {hc[i0].read_buf = NULL;}
else {hc[i0].read_buf = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_val);}
hc[i0].read_size=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].read_size;
hc[i0].read_idx=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].read_idx;
hc[i0].checked_idx=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].checked_idx;
hc[i0].checked_state=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].checked_state;
hc[i0].method=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].method;
hc[i0].status=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].status;
hc[i0].bytes_to_send=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].bytes_to_send;
hc[i0].bytes_sent=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].bytes_sent;
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_val == NULL) {hc[i0].encodedurl = NULL;}
else {hc[i0].encodedurl = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_val == NULL) {hc[i0].decodedurl = NULL;}
else {hc[i0].decodedurl = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_val == NULL) {hc[i0].protocol = NULL;}
else {hc[i0].protocol = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_val == NULL) {hc[i0].origfilename = NULL;}
else {hc[i0].origfilename = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_val == NULL) {hc[i0].expnfilename = NULL;}
else {hc[i0].expnfilename = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_val == NULL) {hc[i0].encodings = NULL;}
else {hc[i0].encodings = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_val == NULL) {hc[i0].pathinfo = NULL;}
else {hc[i0].pathinfo = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_val == NULL) {hc[i0].query = NULL;}
else {hc[i0].query = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_val == NULL) {hc[i0].referrer = NULL;}
else {hc[i0].referrer = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_val == NULL) {hc[i0].useragent = NULL;}
else {hc[i0].useragent = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_val == NULL) {hc[i0].accept = NULL;}
else {hc[i0].accept = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_val == NULL) {hc[i0].accepte = NULL;}
else {hc[i0].accepte = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_val == NULL) {hc[i0].acceptl = NULL;}
else {hc[i0].acceptl = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_val == NULL) {hc[i0].cookie = NULL;}
else {hc[i0].cookie = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_val == NULL) {hc[i0].contenttype = NULL;}
else {hc[i0].contenttype = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_val == NULL) {hc[i0].reqhost = NULL;}
else {hc[i0].reqhost = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_val == NULL) {hc[i0].hdrhost = NULL;}
else {hc[i0].hdrhost = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_val == NULL) {hc[i0].hostdir = NULL;}
else {hc[i0].hostdir = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_val == NULL) {hc[i0].authorization = NULL;}
else {hc[i0].authorization = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_val == NULL) {hc[i0].remoteuser = NULL;}
else {hc[i0].remoteuser = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_val == NULL) {hc[i0].response = NULL;}
else {hc[i0].response = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_val);}
hc[i0].maxdecodedurl=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].maxdecodedurl;
hc[i0].maxorigfilename=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].maxorigfilename;
hc[i0].maxexpnfilename=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].maxexpnfilename;
hc[i0].maxencodings=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].maxencodings;
hc[i0].maxpathinfo=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].maxpathinfo;
hc[i0].maxquery=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].maxquery;
hc[i0].maxaccept=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].maxaccept;
hc[i0].maxaccepte=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].maxaccepte;
hc[i0].maxreqhost=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].maxreqhost;
hc[i0].maxhostdir=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].maxhostdir;
hc[i0].maxremoteuser=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].maxremoteuser;
hc[i0].maxresponse=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].maxresponse;
hc[i0].responselen=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].responselen;
hc[i0].if_modified_since=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].if_modified_since;
hc[i0].range_if=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].range_if;
hc[i0].contentlength=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].contentlength;
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_val == NULL) {hc[i0].type = NULL;}
else {hc[i0].type = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_val);}
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_val == NULL) {hc[i0].hostname = NULL;}
else {hc[i0].hostname = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_val);}
hc[i0].mime_flag=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].mime_flag;
hc[i0].one_one=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].one_one;
hc[i0].got_range=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].got_range;
hc[i0].tildemapped=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].tildemapped;
hc[i0].first_byte_index=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].first_byte_index;
hc[i0].last_byte_index=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].last_byte_index;
hc[i0].keep_alive=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].keep_alive;
hc[i0].should_linger=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].should_linger;
hc[i0].sb.st_dev=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].sb.st_dev;
hc[i0].sb.st_ino=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].sb.st_ino;
hc[i0].sb.st_mode=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].sb.st_mode;
hc[i0].sb.st_nlink=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].sb.st_nlink;
hc[i0].sb.st_uid=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].sb.st_uid;
hc[i0].sb.st_gid=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].sb.st_gid;
hc[i0].sb.st_rdev=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].sb.st_rdev;
hc[i0].sb.st_size=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].sb.st_size;
hc[i0].sb.st_blksize=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].sb.st_blksize;
hc[i0].sb.st_blocks=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].sb.st_blocks;
hc[i0].sb.st_atim=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].sb.st_atim;
hc[i0].sb.st_mtim=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].sb.st_mtim;
hc[i0].sb.st_ctim=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].sb.st_ctim;
hc[i0].conn_fd=result_rpc->hc.httpd_conn_rpc_ptr_val[i0].conn_fd;
if (result_rpc->hc.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_val == NULL) {hc[i0].file_address = NULL;}
else {hc[i0].file_address = strdup(result_rpc->hc.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_val);}
        }
    }
if(result_rpc->nowP.timeval_rpc_ptr_val==NULL) {
nowP=NULL;
} else {
nowP=malloc(sizeof(struct timeval));
if(nowP==NULL) {
perror("malloc failed");
exit(1);
}
memcpy(nowP,result_rpc->nowP.timeval_rpc_ptr_val,sizeof(struct timeval));
}
}

/* 全局变量访问函数 - 客户端Wrapper */
int get_str_alloc_count_wrapper()
{
    str_alloc_count_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_str_alloc_count_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_str_alloc_count_wrapper(int value)
{
    void *result;
    result = set_str_alloc_count_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

FunctionMapEntry function_map[] = {
    {NULL, NULL, SIG_UNKNOWN} // 哨兵值
};
// --- 查找函数名：根据函数名查找函数条目 ---
void* find_function_entry_by_name(const char* func_name) {
    for (int i = 0; function_map[i].name != NULL; i++) {
        if (strcmp(function_map[i].name, func_name) == 0) {
            return &function_map[i].func_ptr_void;
        }
    }
    return NULL; // 未找到
}
// --- 新增函数：根据函数地址查找函数名 ---
const char* find_function_name_by_address(void* func_address) {
    for (int i = 0; function_map[i].name != NULL; i++) {
        // 直接比较 void* 指针
        if (function_map[i].func_ptr_void == func_address) {
            return function_map[i].name;
        }
    }
    return NULL; // 未找到对应的函数名
}
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
int gsetter_str_alloc_count_prefix_wrapper()
{
   int *result;
   result=gsetter_str_alloc_count_prefix_1(clnt);
   if (result == NULL) 
       clnt_perror(clnt, "call failed");
   return *result;
}
