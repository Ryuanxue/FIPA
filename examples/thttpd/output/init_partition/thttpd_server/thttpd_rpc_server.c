#include "thttpd_idl.h"
auth_check2_sense_1_ret_ *auth_check2_sense_1_rpc_1_svc(int_ptr arg1,httpd_conn_rpc_ptr arg2,char_ptr arg3,struct svc_req *rqstp)
{
	static auth_check2_sense_1_ret_ result;
int *auth_check2_sense_1_ret=NULL;
if(arg1.int_ptr_val!=NULL) {
auth_check2_sense_1_ret=malloc(arg1.int_ptr_len*sizeof(int));
if(auth_check2_sense_1_ret==NULL) {
perror("malloc failed");
exit(1);
}
for(int i0=0;i0<arg1.int_ptr_len;i0++) {
auth_check2_sense_1_ret[i0]=arg1.int_ptr_val[i0];
	}
}
httpd_conn *hc=NULL;
if(arg2.httpd_conn_rpc_ptr_val!=NULL) {
hc=( httpd_conn *)malloc(arg2.httpd_conn_rpc_ptr_len*sizeof( httpd_conn *));
if(hc==NULL) {
perror("malloc failed");
exit(1);
}
for(int i0=0;i0<arg2.httpd_conn_rpc_ptr_len;i0++) {
hc[i0].initialized=arg2.httpd_conn_rpc_ptr_val[i0].initialized;
if (arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val == NULL) {hc[i0].hs=NULL;}
else {hc[i0].hs=(httpd_server *)malloc(arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_len * sizeof(httpd_server));
	if (hc[i0].hs == NULL) {
		perror("malloc failed");
		exit(1);
	}
	for (int i1 = 0; i1 < arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_len; i1++) {
if (arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_val == NULL) {hc[i0].hs[i1].binding_hostname=NULL;}

else {hc[i0].hs[i1].binding_hostname=malloc(arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_len);

memcpy(hc[i0].hs[i1].binding_hostname,arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_len);

}

if (arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_val == NULL) {hc[i0].hs[i1].server_hostname=NULL;}

else {hc[i0].hs[i1].server_hostname=malloc(arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_len);

memcpy(hc[i0].hs[i1].server_hostname,arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_len);

}

hc[i0].hs[i1].port=arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].port;
if (arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_val == NULL) {hc[i0].hs[i1].cgi_pattern=NULL;}

else {hc[i0].hs[i1].cgi_pattern=malloc(arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_len);

memcpy(hc[i0].hs[i1].cgi_pattern,arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_len);

}

hc[i0].hs[i1].cgi_limit=arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_limit;
hc[i0].hs[i1].cgi_count=arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_count;
if (arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_val == NULL) {hc[i0].hs[i1].charset=NULL;}

else {hc[i0].hs[i1].charset=malloc(arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_len);

memcpy(hc[i0].hs[i1].charset,arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_len);

}

if (arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_val == NULL) {hc[i0].hs[i1].p3p=NULL;}

else {hc[i0].hs[i1].p3p=malloc(arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_len);

memcpy(hc[i0].hs[i1].p3p,arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_len);

}

hc[i0].hs[i1].max_age=arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].max_age;
if (arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_val == NULL) {hc[i0].hs[i1].cwd=NULL;}

else {hc[i0].hs[i1].cwd=malloc(arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_len);

memcpy(hc[i0].hs[i1].cwd,arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_len);

}

hc[i0].hs[i1].listen4_fd=arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].listen4_fd;
hc[i0].hs[i1].listen6_fd=arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].listen6_fd;
hc[i0].hs[i1].no_log=arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].no_log;
if (arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_val == NULL) {hc[i0].hs[i1].logfp=NULL;}

else {hc[i0].hs[i1].logfp=malloc(arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_len);

memcpy(hc[i0].hs[i1].logfp,arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_len);

}

hc[i0].hs[i1].no_symlink_check=arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].no_symlink_check;
hc[i0].hs[i1].vhost=arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].vhost;
hc[i0].hs[i1].global_passwd=arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].global_passwd;
if (arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_val == NULL) {hc[i0].hs[i1].url_pattern=NULL;}

else {hc[i0].hs[i1].url_pattern=malloc(arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_len);

memcpy(hc[i0].hs[i1].url_pattern,arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_len);

}

if (arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_val == NULL) {hc[i0].hs[i1].local_pattern=NULL;}

else {hc[i0].hs[i1].local_pattern=malloc(arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_len);

memcpy(hc[i0].hs[i1].local_pattern,arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_len);

}

hc[i0].hs[i1].no_empty_referrers=arg2.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].no_empty_referrers;
	}
}
if (arg2.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_val == NULL) {hc[i0].read_buf=NULL;}

else {hc[i0].read_buf=malloc(arg2.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_len);

memcpy(hc[i0].read_buf,arg2.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_len);

}

hc[i0].read_size=arg2.httpd_conn_rpc_ptr_val[i0].read_size;
hc[i0].read_idx=arg2.httpd_conn_rpc_ptr_val[i0].read_idx;
hc[i0].checked_idx=arg2.httpd_conn_rpc_ptr_val[i0].checked_idx;
hc[i0].checked_state=arg2.httpd_conn_rpc_ptr_val[i0].checked_state;
hc[i0].method=arg2.httpd_conn_rpc_ptr_val[i0].method;
hc[i0].status=arg2.httpd_conn_rpc_ptr_val[i0].status;
hc[i0].bytes_to_send=arg2.httpd_conn_rpc_ptr_val[i0].bytes_to_send;
hc[i0].bytes_sent=arg2.httpd_conn_rpc_ptr_val[i0].bytes_sent;
if (arg2.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_val == NULL) {hc[i0].encodedurl=NULL;}

else {hc[i0].encodedurl=malloc(arg2.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_len);

memcpy(hc[i0].encodedurl,arg2.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_len);

}

if (arg2.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_val == NULL) {hc[i0].decodedurl=NULL;}

else {hc[i0].decodedurl=malloc(arg2.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_len);

memcpy(hc[i0].decodedurl,arg2.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_len);

}

if (arg2.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_val == NULL) {hc[i0].protocol=NULL;}

else {hc[i0].protocol=malloc(arg2.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_len);

memcpy(hc[i0].protocol,arg2.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_len);

}

if (arg2.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_val == NULL) {hc[i0].origfilename=NULL;}

else {hc[i0].origfilename=malloc(arg2.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_len);

memcpy(hc[i0].origfilename,arg2.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_len);

}

if (arg2.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_val == NULL) {hc[i0].expnfilename=NULL;}

else {hc[i0].expnfilename=malloc(arg2.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_len);

memcpy(hc[i0].expnfilename,arg2.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_len);

}

if (arg2.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_val == NULL) {hc[i0].encodings=NULL;}

else {hc[i0].encodings=malloc(arg2.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_len);

memcpy(hc[i0].encodings,arg2.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_len);

}

if (arg2.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_val == NULL) {hc[i0].pathinfo=NULL;}

else {hc[i0].pathinfo=malloc(arg2.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_len);

memcpy(hc[i0].pathinfo,arg2.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_len);

}

if (arg2.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_val == NULL) {hc[i0].query=NULL;}

else {hc[i0].query=malloc(arg2.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_len);

memcpy(hc[i0].query,arg2.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_len);

}

if (arg2.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_val == NULL) {hc[i0].referrer=NULL;}

else {hc[i0].referrer=malloc(arg2.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_len);

memcpy(hc[i0].referrer,arg2.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_len);

}

if (arg2.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_val == NULL) {hc[i0].useragent=NULL;}

else {hc[i0].useragent=malloc(arg2.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_len);

memcpy(hc[i0].useragent,arg2.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_len);

}

if (arg2.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_val == NULL) {hc[i0].accept=NULL;}

else {hc[i0].accept=malloc(arg2.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_len);

memcpy(hc[i0].accept,arg2.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_len);

}

if (arg2.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_val == NULL) {hc[i0].accepte=NULL;}

else {hc[i0].accepte=malloc(arg2.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_len);

memcpy(hc[i0].accepte,arg2.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_len);

}

if (arg2.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_val == NULL) {hc[i0].acceptl=NULL;}

else {hc[i0].acceptl=malloc(arg2.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_len);

memcpy(hc[i0].acceptl,arg2.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_len);

}

if (arg2.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_val == NULL) {hc[i0].cookie=NULL;}

else {hc[i0].cookie=malloc(arg2.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_len);

memcpy(hc[i0].cookie,arg2.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_len);

}

if (arg2.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_val == NULL) {hc[i0].contenttype=NULL;}

else {hc[i0].contenttype=malloc(arg2.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_len);

memcpy(hc[i0].contenttype,arg2.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_len);

}

if (arg2.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_val == NULL) {hc[i0].reqhost=NULL;}

else {hc[i0].reqhost=malloc(arg2.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_len);

memcpy(hc[i0].reqhost,arg2.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_len);

}

if (arg2.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_val == NULL) {hc[i0].hdrhost=NULL;}

else {hc[i0].hdrhost=malloc(arg2.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_len);

memcpy(hc[i0].hdrhost,arg2.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_len);

}

if (arg2.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_val == NULL) {hc[i0].hostdir=NULL;}

else {hc[i0].hostdir=malloc(arg2.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_len);

memcpy(hc[i0].hostdir,arg2.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_len);

}

if (arg2.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_val == NULL) {hc[i0].authorization=NULL;}

else {hc[i0].authorization=malloc(arg2.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_len);

memcpy(hc[i0].authorization,arg2.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_len);

}

if (arg2.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_val == NULL) {hc[i0].remoteuser=NULL;}

else {hc[i0].remoteuser=malloc(arg2.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_len);

memcpy(hc[i0].remoteuser,arg2.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_len);

}

if (arg2.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_val == NULL) {hc[i0].response=NULL;}

else {hc[i0].response=malloc(arg2.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_len);

memcpy(hc[i0].response,arg2.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_len);

}

hc[i0].maxdecodedurl=arg2.httpd_conn_rpc_ptr_val[i0].maxdecodedurl;
hc[i0].maxorigfilename=arg2.httpd_conn_rpc_ptr_val[i0].maxorigfilename;
hc[i0].maxexpnfilename=arg2.httpd_conn_rpc_ptr_val[i0].maxexpnfilename;
hc[i0].maxencodings=arg2.httpd_conn_rpc_ptr_val[i0].maxencodings;
hc[i0].maxpathinfo=arg2.httpd_conn_rpc_ptr_val[i0].maxpathinfo;
hc[i0].maxquery=arg2.httpd_conn_rpc_ptr_val[i0].maxquery;
hc[i0].maxaccept=arg2.httpd_conn_rpc_ptr_val[i0].maxaccept;
hc[i0].maxaccepte=arg2.httpd_conn_rpc_ptr_val[i0].maxaccepte;
hc[i0].maxreqhost=arg2.httpd_conn_rpc_ptr_val[i0].maxreqhost;
hc[i0].maxhostdir=arg2.httpd_conn_rpc_ptr_val[i0].maxhostdir;
hc[i0].maxremoteuser=arg2.httpd_conn_rpc_ptr_val[i0].maxremoteuser;
hc[i0].maxresponse=arg2.httpd_conn_rpc_ptr_val[i0].maxresponse;
hc[i0].responselen=arg2.httpd_conn_rpc_ptr_val[i0].responselen;
hc[i0].if_modified_since=arg2.httpd_conn_rpc_ptr_val[i0].if_modified_since;
hc[i0].range_if=arg2.httpd_conn_rpc_ptr_val[i0].range_if;
hc[i0].contentlength=arg2.httpd_conn_rpc_ptr_val[i0].contentlength;
if (arg2.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_val == NULL) {hc[i0].type=NULL;}

else {hc[i0].type=malloc(arg2.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_len);

memcpy(hc[i0].type,arg2.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_len);

}

if (arg2.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_val == NULL) {hc[i0].hostname=NULL;}

else {hc[i0].hostname=malloc(arg2.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_len);

memcpy(hc[i0].hostname,arg2.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_len);

}

hc[i0].mime_flag=arg2.httpd_conn_rpc_ptr_val[i0].mime_flag;
hc[i0].one_one=arg2.httpd_conn_rpc_ptr_val[i0].one_one;
hc[i0].got_range=arg2.httpd_conn_rpc_ptr_val[i0].got_range;
hc[i0].tildemapped=arg2.httpd_conn_rpc_ptr_val[i0].tildemapped;
hc[i0].first_byte_index=arg2.httpd_conn_rpc_ptr_val[i0].first_byte_index;
hc[i0].last_byte_index=arg2.httpd_conn_rpc_ptr_val[i0].last_byte_index;
hc[i0].keep_alive=arg2.httpd_conn_rpc_ptr_val[i0].keep_alive;
hc[i0].should_linger=arg2.httpd_conn_rpc_ptr_val[i0].should_linger;
hc[i0].sb.st_dev=arg2.httpd_conn_rpc_ptr_val[i0].sb.st_dev;
hc[i0].sb.st_ino=arg2.httpd_conn_rpc_ptr_val[i0].sb.st_ino;
hc[i0].sb.st_mode=arg2.httpd_conn_rpc_ptr_val[i0].sb.st_mode;
hc[i0].sb.st_nlink=arg2.httpd_conn_rpc_ptr_val[i0].sb.st_nlink;
hc[i0].sb.st_uid=arg2.httpd_conn_rpc_ptr_val[i0].sb.st_uid;
hc[i0].sb.st_gid=arg2.httpd_conn_rpc_ptr_val[i0].sb.st_gid;
hc[i0].sb.st_rdev=arg2.httpd_conn_rpc_ptr_val[i0].sb.st_rdev;
hc[i0].sb.st_size=arg2.httpd_conn_rpc_ptr_val[i0].sb.st_size;
hc[i0].sb.st_blksize=arg2.httpd_conn_rpc_ptr_val[i0].sb.st_blksize;
hc[i0].sb.st_blocks=arg2.httpd_conn_rpc_ptr_val[i0].sb.st_blocks;
hc[i0].sb.st_atime=arg2.httpd_conn_rpc_ptr_val[i0].sb.st_atime;
hc[i0].sb.st_mtime=arg2.httpd_conn_rpc_ptr_val[i0].sb.st_mtime;
hc[i0].sb.st_ctime=arg2.httpd_conn_rpc_ptr_val[i0].sb.st_ctime;
hc[i0].conn_fd=arg2.httpd_conn_rpc_ptr_val[i0].conn_fd;
if (arg2.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_val == NULL) {hc[i0].file_address=NULL;}

else {hc[i0].file_address=malloc(arg2.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_len);

memcpy(hc[i0].file_address,arg2.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_val,arg2.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_len);

}

	}
}
char *dirname=NULL;
if(arg3.charptr.charptr_val!=NULL) {
dirname=arg3.charptr.charptr_val;
}
int sensefun_ret;
sensefun_ret=auth_check2_sense_1(auth_check2_sense_1_ret,hc,dirname);
result.ret0=sensefun_ret;
if(auth_check2_sense_1_ret==NULL) {result.auth_check2_sense_1_ret.int_ptr_len=0;result.auth_check2_sense_1_ret.int_ptr_val=NULL;}
else {
	result.auth_check2_sense_1_ret.int_ptr_len=1;
	result.auth_check2_sense_1_ret.int_ptr_val=malloc(sizeof(int)*result.auth_check2_sense_1_ret.int_ptr_len);
	if(result.auth_check2_sense_1_ret.int_ptr_val==NULL) {
		perror("malloc failed");
		exit(1);
	}
	for(int i0=0;i0<result.auth_check2_sense_1_ret.int_ptr_len;i0++) {
		result.auth_check2_sense_1_ret.int_ptr_val[i0]=auth_check2_sense_1_ret[i0];
	}
}
    if(hc==NULL) {
        result.hc.httpd_conn_rpc_ptr_len=0;
        result.hc.httpd_conn_rpc_ptr_val=NULL;
    } else {
        result.hc.httpd_conn_rpc_ptr_len=1;
        result.hc.httpd_conn_rpc_ptr_val=malloc(1*sizeof(httpd_conn_rpc));
        if(result.hc.httpd_conn_rpc_ptr_val==NULL) {
            perror("malloc failed");
            exit(1);
        }
        for(int i0=0;i0<result.hc.httpd_conn_rpc_ptr_len;i0++) {
result.hc.httpd_conn_rpc_ptr_val[i0].initialized=hc[i0].initialized;
if (hc[i0].hs == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val = NULL;}
else {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_len = 1;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val = malloc(result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_len * sizeof(httpd_server_rpc));
	if (result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val == NULL) {perror("malloc failed");exit(1);}
	for (int i1 = 0; i1 < result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_len; i1++) {
if (hc[i0].hs->binding_hostname == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_len = strlen(hc[i0].hs->binding_hostname)+1;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_val = hc[i0].hs->binding_hostname;}

if (hc[i0].hs->server_hostname == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_len = strlen(hc[i0].hs->server_hostname)+1;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_val = hc[i0].hs->server_hostname;}

result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].port=hc[i0].hs->port;
if (hc[i0].hs->cgi_pattern == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_len = strlen(hc[i0].hs->cgi_pattern)+1;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_val = hc[i0].hs->cgi_pattern;}

result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_limit=hc[i0].hs->cgi_limit;
result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_count=hc[i0].hs->cgi_count;
if (hc[i0].hs->charset == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_len = strlen(hc[i0].hs->charset)+1;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_val = hc[i0].hs->charset;}

if (hc[i0].hs->p3p == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_len = strlen(hc[i0].hs->p3p)+1;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_val = hc[i0].hs->p3p;}

result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].max_age=hc[i0].hs->max_age;
if (hc[i0].hs->cwd == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_len = strlen(hc[i0].hs->cwd)+1;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_val = hc[i0].hs->cwd;}

result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].listen4_fd=hc[i0].hs->listen4_fd;
result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].listen6_fd=hc[i0].hs->listen6_fd;
result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].no_log=hc[i0].hs->no_log;
if (hc[i0].hs->logfp == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_len = strlen(hc[i0].hs->logfp)+1;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_val = hc[i0].hs->logfp;}

result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].no_symlink_check=hc[i0].hs->no_symlink_check;
result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].vhost=hc[i0].hs->vhost;
result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].global_passwd=hc[i0].hs->global_passwd;
if (hc[i0].hs->url_pattern == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_len = strlen(hc[i0].hs->url_pattern)+1;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_val = hc[i0].hs->url_pattern;}

if (hc[i0].hs->local_pattern == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_len = strlen(hc[i0].hs->local_pattern)+1;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_val = hc[i0].hs->local_pattern;}

result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].no_empty_referrers=hc[i0].hs->no_empty_referrers;
	}
}
if (hc[i0].read_buf == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_len = strlen(hc[i0].read_buf)+1;result.hc.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_val = hc[i0].read_buf;}

result.hc.httpd_conn_rpc_ptr_val[i0].read_size=hc[i0].read_size;
result.hc.httpd_conn_rpc_ptr_val[i0].read_idx=hc[i0].read_idx;
result.hc.httpd_conn_rpc_ptr_val[i0].checked_idx=hc[i0].checked_idx;
result.hc.httpd_conn_rpc_ptr_val[i0].checked_state=hc[i0].checked_state;
result.hc.httpd_conn_rpc_ptr_val[i0].method=hc[i0].method;
result.hc.httpd_conn_rpc_ptr_val[i0].status=hc[i0].status;
result.hc.httpd_conn_rpc_ptr_val[i0].bytes_to_send=hc[i0].bytes_to_send;
result.hc.httpd_conn_rpc_ptr_val[i0].bytes_sent=hc[i0].bytes_sent;
if (hc[i0].encodedurl == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_len = strlen(hc[i0].encodedurl)+1;result.hc.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_val = hc[i0].encodedurl;}

if (hc[i0].decodedurl == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_len = strlen(hc[i0].decodedurl)+1;result.hc.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_val = hc[i0].decodedurl;}

if (hc[i0].protocol == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_len = strlen(hc[i0].protocol)+1;result.hc.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_val = hc[i0].protocol;}

if (hc[i0].origfilename == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_len = strlen(hc[i0].origfilename)+1;result.hc.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_val = hc[i0].origfilename;}

if (hc[i0].expnfilename == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_len = strlen(hc[i0].expnfilename)+1;result.hc.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_val = hc[i0].expnfilename;}

if (hc[i0].encodings == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_len = strlen(hc[i0].encodings)+1;result.hc.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_val = hc[i0].encodings;}

if (hc[i0].pathinfo == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_len = strlen(hc[i0].pathinfo)+1;result.hc.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_val = hc[i0].pathinfo;}

if (hc[i0].query == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_len = strlen(hc[i0].query)+1;result.hc.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_val = hc[i0].query;}

if (hc[i0].referrer == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_len = strlen(hc[i0].referrer)+1;result.hc.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_val = hc[i0].referrer;}

if (hc[i0].useragent == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_len = strlen(hc[i0].useragent)+1;result.hc.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_val = hc[i0].useragent;}

if (hc[i0].accept == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_len = strlen(hc[i0].accept)+1;result.hc.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_val = hc[i0].accept;}

if (hc[i0].accepte == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_len = strlen(hc[i0].accepte)+1;result.hc.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_val = hc[i0].accepte;}

if (hc[i0].acceptl == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_len = strlen(hc[i0].acceptl)+1;result.hc.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_val = hc[i0].acceptl;}

if (hc[i0].cookie == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_len = strlen(hc[i0].cookie)+1;result.hc.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_val = hc[i0].cookie;}

if (hc[i0].contenttype == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_len = strlen(hc[i0].contenttype)+1;result.hc.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_val = hc[i0].contenttype;}

if (hc[i0].reqhost == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_len = strlen(hc[i0].reqhost)+1;result.hc.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_val = hc[i0].reqhost;}

if (hc[i0].hdrhost == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_len = strlen(hc[i0].hdrhost)+1;result.hc.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_val = hc[i0].hdrhost;}

if (hc[i0].hostdir == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_len = strlen(hc[i0].hostdir)+1;result.hc.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_val = hc[i0].hostdir;}

if (hc[i0].authorization == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_len = strlen(hc[i0].authorization)+1;result.hc.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_val = hc[i0].authorization;}

if (hc[i0].remoteuser == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_len = strlen(hc[i0].remoteuser)+1;result.hc.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_val = hc[i0].remoteuser;}

if (hc[i0].response == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_len = strlen(hc[i0].response)+1;result.hc.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_val = hc[i0].response;}

result.hc.httpd_conn_rpc_ptr_val[i0].maxdecodedurl=hc[i0].maxdecodedurl;
result.hc.httpd_conn_rpc_ptr_val[i0].maxorigfilename=hc[i0].maxorigfilename;
result.hc.httpd_conn_rpc_ptr_val[i0].maxexpnfilename=hc[i0].maxexpnfilename;
result.hc.httpd_conn_rpc_ptr_val[i0].maxencodings=hc[i0].maxencodings;
result.hc.httpd_conn_rpc_ptr_val[i0].maxpathinfo=hc[i0].maxpathinfo;
result.hc.httpd_conn_rpc_ptr_val[i0].maxquery=hc[i0].maxquery;
result.hc.httpd_conn_rpc_ptr_val[i0].maxaccept=hc[i0].maxaccept;
result.hc.httpd_conn_rpc_ptr_val[i0].maxaccepte=hc[i0].maxaccepte;
result.hc.httpd_conn_rpc_ptr_val[i0].maxreqhost=hc[i0].maxreqhost;
result.hc.httpd_conn_rpc_ptr_val[i0].maxhostdir=hc[i0].maxhostdir;
result.hc.httpd_conn_rpc_ptr_val[i0].maxremoteuser=hc[i0].maxremoteuser;
result.hc.httpd_conn_rpc_ptr_val[i0].maxresponse=hc[i0].maxresponse;
result.hc.httpd_conn_rpc_ptr_val[i0].responselen=hc[i0].responselen;
result.hc.httpd_conn_rpc_ptr_val[i0].if_modified_since=hc[i0].if_modified_since;
result.hc.httpd_conn_rpc_ptr_val[i0].range_if=hc[i0].range_if;
result.hc.httpd_conn_rpc_ptr_val[i0].contentlength=hc[i0].contentlength;
if (hc[i0].type == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_len = strlen(hc[i0].type)+1;result.hc.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_val = hc[i0].type;}

if (hc[i0].hostname == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_len = strlen(hc[i0].hostname)+1;result.hc.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_val = hc[i0].hostname;}

result.hc.httpd_conn_rpc_ptr_val[i0].mime_flag=hc[i0].mime_flag;
result.hc.httpd_conn_rpc_ptr_val[i0].one_one=hc[i0].one_one;
result.hc.httpd_conn_rpc_ptr_val[i0].got_range=hc[i0].got_range;
result.hc.httpd_conn_rpc_ptr_val[i0].tildemapped=hc[i0].tildemapped;
result.hc.httpd_conn_rpc_ptr_val[i0].first_byte_index=hc[i0].first_byte_index;
result.hc.httpd_conn_rpc_ptr_val[i0].last_byte_index=hc[i0].last_byte_index;
result.hc.httpd_conn_rpc_ptr_val[i0].keep_alive=hc[i0].keep_alive;
result.hc.httpd_conn_rpc_ptr_val[i0].should_linger=hc[i0].should_linger;
result.hc.httpd_conn_rpc_ptr_val[i0].sb.st_dev=hc[i0].sb.st_dev;
result.hc.httpd_conn_rpc_ptr_val[i0].sb.st_ino=hc[i0].sb.st_ino;
result.hc.httpd_conn_rpc_ptr_val[i0].sb.st_mode=hc[i0].sb.st_mode;
result.hc.httpd_conn_rpc_ptr_val[i0].sb.st_nlink=hc[i0].sb.st_nlink;
result.hc.httpd_conn_rpc_ptr_val[i0].sb.st_uid=hc[i0].sb.st_uid;
result.hc.httpd_conn_rpc_ptr_val[i0].sb.st_gid=hc[i0].sb.st_gid;
result.hc.httpd_conn_rpc_ptr_val[i0].sb.st_rdev=hc[i0].sb.st_rdev;
result.hc.httpd_conn_rpc_ptr_val[i0].sb.st_size=hc[i0].sb.st_size;
result.hc.httpd_conn_rpc_ptr_val[i0].sb.st_blksize=hc[i0].sb.st_blksize;
result.hc.httpd_conn_rpc_ptr_val[i0].sb.st_blocks=hc[i0].sb.st_blocks;
result.hc.httpd_conn_rpc_ptr_val[i0].sb.st_atim=hc[i0].sb.st_atime;
result.hc.httpd_conn_rpc_ptr_val[i0].sb.st_mtim=hc[i0].sb.st_mtime;
result.hc.httpd_conn_rpc_ptr_val[i0].sb.st_ctim=hc[i0].sb.st_ctime;
result.hc.httpd_conn_rpc_ptr_val[i0].conn_fd=hc[i0].conn_fd;
if (hc[i0].file_address == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_len = strlen(hc[i0].file_address)+1;result.hc.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_val = hc[i0].file_address;}

        }
    }
if(dirname==NULL) result.dirname.charptr.charptr_len=0;else result.dirname.charptr.charptr_len=strlen(dirname)+1;
result.dirname.charptr.charptr_val=malloc(result.dirname.charptr.charptr_len);
if(result.dirname.charptr.charptr_val==NULL) {
perror("malloc failed");
exit(1);
}
memcpy(result.dirname.charptr.charptr_val,dirname,result.dirname.charptr.charptr_len);
return &result;
}
make_log_entry_sense_1_ret_ *make_log_entry_sense_1_rpc_1_svc(char_ptr arg1,make_log_entry_sense_1_url arg2,make_log_entry_sense_1_bytes arg3,httpd_conn_rpc_ptr arg4, timeval_rpc_ptr arg5,struct svc_req *rqstp)
{
	static make_log_entry_sense_1_ret_ result;
char *ru=NULL;
if(arg1.charptr.charptr_val!=NULL) {
ru=arg1.charptr.charptr_val;
}
char url[305];
memcpy(url, arg2.url, 305);
char bytes[40];
memcpy(bytes, arg3.bytes, 40);
httpd_conn *hc=NULL;
if(arg4.httpd_conn_rpc_ptr_val!=NULL) {
hc=( httpd_conn *)malloc(arg4.httpd_conn_rpc_ptr_len*sizeof( httpd_conn *));
if(hc==NULL) {
perror("malloc failed");
exit(1);
}
for(int i0=0;i0<arg4.httpd_conn_rpc_ptr_len;i0++) {
hc[i0].initialized=arg4.httpd_conn_rpc_ptr_val[i0].initialized;
if (arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val == NULL) {hc[i0].hs=NULL;}
else {hc[i0].hs=(httpd_server *)malloc(arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_len * sizeof(httpd_server));
	if (hc[i0].hs == NULL) {
		perror("malloc failed");
		exit(1);
	}
	for (int i1 = 0; i1 < arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_len; i1++) {
if (arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_val == NULL) {hc[i0].hs[i1].binding_hostname=NULL;}

else {hc[i0].hs[i1].binding_hostname=malloc(arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_len);

memcpy(hc[i0].hs[i1].binding_hostname,arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_len);

}

if (arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_val == NULL) {hc[i0].hs[i1].server_hostname=NULL;}

else {hc[i0].hs[i1].server_hostname=malloc(arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_len);

memcpy(hc[i0].hs[i1].server_hostname,arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_len);

}

hc[i0].hs[i1].port=arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].port;
if (arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_val == NULL) {hc[i0].hs[i1].cgi_pattern=NULL;}

else {hc[i0].hs[i1].cgi_pattern=malloc(arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_len);

memcpy(hc[i0].hs[i1].cgi_pattern,arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_len);

}

hc[i0].hs[i1].cgi_limit=arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_limit;
hc[i0].hs[i1].cgi_count=arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_count;
if (arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_val == NULL) {hc[i0].hs[i1].charset=NULL;}

else {hc[i0].hs[i1].charset=malloc(arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_len);

memcpy(hc[i0].hs[i1].charset,arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_len);

}

if (arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_val == NULL) {hc[i0].hs[i1].p3p=NULL;}

else {hc[i0].hs[i1].p3p=malloc(arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_len);

memcpy(hc[i0].hs[i1].p3p,arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_len);

}

hc[i0].hs[i1].max_age=arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].max_age;
if (arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_val == NULL) {hc[i0].hs[i1].cwd=NULL;}

else {hc[i0].hs[i1].cwd=malloc(arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_len);

memcpy(hc[i0].hs[i1].cwd,arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_len);

}

hc[i0].hs[i1].listen4_fd=arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].listen4_fd;
hc[i0].hs[i1].listen6_fd=arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].listen6_fd;
hc[i0].hs[i1].no_log=arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].no_log;
if (arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_val == NULL) {hc[i0].hs[i1].logfp=NULL;}

else {hc[i0].hs[i1].logfp=malloc(arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_len);

memcpy(hc[i0].hs[i1].logfp,arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_len);

}

hc[i0].hs[i1].no_symlink_check=arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].no_symlink_check;
hc[i0].hs[i1].vhost=arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].vhost;
hc[i0].hs[i1].global_passwd=arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].global_passwd;
if (arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_val == NULL) {hc[i0].hs[i1].url_pattern=NULL;}

else {hc[i0].hs[i1].url_pattern=malloc(arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_len);

memcpy(hc[i0].hs[i1].url_pattern,arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_len);

}

if (arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_val == NULL) {hc[i0].hs[i1].local_pattern=NULL;}

else {hc[i0].hs[i1].local_pattern=malloc(arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_len);

memcpy(hc[i0].hs[i1].local_pattern,arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_len);

}

hc[i0].hs[i1].no_empty_referrers=arg4.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].no_empty_referrers;
	}
}
if (arg4.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_val == NULL) {hc[i0].read_buf=NULL;}

else {hc[i0].read_buf=malloc(arg4.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_len);

memcpy(hc[i0].read_buf,arg4.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_len);

}

hc[i0].read_size=arg4.httpd_conn_rpc_ptr_val[i0].read_size;
hc[i0].read_idx=arg4.httpd_conn_rpc_ptr_val[i0].read_idx;
hc[i0].checked_idx=arg4.httpd_conn_rpc_ptr_val[i0].checked_idx;
hc[i0].checked_state=arg4.httpd_conn_rpc_ptr_val[i0].checked_state;
hc[i0].method=arg4.httpd_conn_rpc_ptr_val[i0].method;
hc[i0].status=arg4.httpd_conn_rpc_ptr_val[i0].status;
hc[i0].bytes_to_send=arg4.httpd_conn_rpc_ptr_val[i0].bytes_to_send;
hc[i0].bytes_sent=arg4.httpd_conn_rpc_ptr_val[i0].bytes_sent;
if (arg4.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_val == NULL) {hc[i0].encodedurl=NULL;}

else {hc[i0].encodedurl=malloc(arg4.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_len);

memcpy(hc[i0].encodedurl,arg4.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_len);

}

if (arg4.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_val == NULL) {hc[i0].decodedurl=NULL;}

else {hc[i0].decodedurl=malloc(arg4.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_len);

memcpy(hc[i0].decodedurl,arg4.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_len);

}

if (arg4.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_val == NULL) {hc[i0].protocol=NULL;}

else {hc[i0].protocol=malloc(arg4.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_len);

memcpy(hc[i0].protocol,arg4.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_len);

}

if (arg4.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_val == NULL) {hc[i0].origfilename=NULL;}

else {hc[i0].origfilename=malloc(arg4.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_len);

memcpy(hc[i0].origfilename,arg4.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_len);

}

if (arg4.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_val == NULL) {hc[i0].expnfilename=NULL;}

else {hc[i0].expnfilename=malloc(arg4.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_len);

memcpy(hc[i0].expnfilename,arg4.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_len);

}

if (arg4.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_val == NULL) {hc[i0].encodings=NULL;}

else {hc[i0].encodings=malloc(arg4.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_len);

memcpy(hc[i0].encodings,arg4.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_len);

}

if (arg4.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_val == NULL) {hc[i0].pathinfo=NULL;}

else {hc[i0].pathinfo=malloc(arg4.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_len);

memcpy(hc[i0].pathinfo,arg4.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_len);

}

if (arg4.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_val == NULL) {hc[i0].query=NULL;}

else {hc[i0].query=malloc(arg4.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_len);

memcpy(hc[i0].query,arg4.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_len);

}

if (arg4.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_val == NULL) {hc[i0].referrer=NULL;}

else {hc[i0].referrer=malloc(arg4.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_len);

memcpy(hc[i0].referrer,arg4.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_len);

}

if (arg4.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_val == NULL) {hc[i0].useragent=NULL;}

else {hc[i0].useragent=malloc(arg4.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_len);

memcpy(hc[i0].useragent,arg4.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_len);

}

if (arg4.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_val == NULL) {hc[i0].accept=NULL;}

else {hc[i0].accept=malloc(arg4.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_len);

memcpy(hc[i0].accept,arg4.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_len);

}

if (arg4.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_val == NULL) {hc[i0].accepte=NULL;}

else {hc[i0].accepte=malloc(arg4.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_len);

memcpy(hc[i0].accepte,arg4.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_len);

}

if (arg4.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_val == NULL) {hc[i0].acceptl=NULL;}

else {hc[i0].acceptl=malloc(arg4.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_len);

memcpy(hc[i0].acceptl,arg4.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_len);

}

if (arg4.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_val == NULL) {hc[i0].cookie=NULL;}

else {hc[i0].cookie=malloc(arg4.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_len);

memcpy(hc[i0].cookie,arg4.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_len);

}

if (arg4.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_val == NULL) {hc[i0].contenttype=NULL;}

else {hc[i0].contenttype=malloc(arg4.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_len);

memcpy(hc[i0].contenttype,arg4.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_len);

}

if (arg4.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_val == NULL) {hc[i0].reqhost=NULL;}

else {hc[i0].reqhost=malloc(arg4.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_len);

memcpy(hc[i0].reqhost,arg4.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_len);

}

if (arg4.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_val == NULL) {hc[i0].hdrhost=NULL;}

else {hc[i0].hdrhost=malloc(arg4.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_len);

memcpy(hc[i0].hdrhost,arg4.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_len);

}

if (arg4.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_val == NULL) {hc[i0].hostdir=NULL;}

else {hc[i0].hostdir=malloc(arg4.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_len);

memcpy(hc[i0].hostdir,arg4.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_len);

}

if (arg4.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_val == NULL) {hc[i0].authorization=NULL;}

else {hc[i0].authorization=malloc(arg4.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_len);

memcpy(hc[i0].authorization,arg4.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_len);

}

if (arg4.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_val == NULL) {hc[i0].remoteuser=NULL;}

else {hc[i0].remoteuser=malloc(arg4.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_len);

memcpy(hc[i0].remoteuser,arg4.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_len);

}

if (arg4.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_val == NULL) {hc[i0].response=NULL;}

else {hc[i0].response=malloc(arg4.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_len);

memcpy(hc[i0].response,arg4.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_len);

}

hc[i0].maxdecodedurl=arg4.httpd_conn_rpc_ptr_val[i0].maxdecodedurl;
hc[i0].maxorigfilename=arg4.httpd_conn_rpc_ptr_val[i0].maxorigfilename;
hc[i0].maxexpnfilename=arg4.httpd_conn_rpc_ptr_val[i0].maxexpnfilename;
hc[i0].maxencodings=arg4.httpd_conn_rpc_ptr_val[i0].maxencodings;
hc[i0].maxpathinfo=arg4.httpd_conn_rpc_ptr_val[i0].maxpathinfo;
hc[i0].maxquery=arg4.httpd_conn_rpc_ptr_val[i0].maxquery;
hc[i0].maxaccept=arg4.httpd_conn_rpc_ptr_val[i0].maxaccept;
hc[i0].maxaccepte=arg4.httpd_conn_rpc_ptr_val[i0].maxaccepte;
hc[i0].maxreqhost=arg4.httpd_conn_rpc_ptr_val[i0].maxreqhost;
hc[i0].maxhostdir=arg4.httpd_conn_rpc_ptr_val[i0].maxhostdir;
hc[i0].maxremoteuser=arg4.httpd_conn_rpc_ptr_val[i0].maxremoteuser;
hc[i0].maxresponse=arg4.httpd_conn_rpc_ptr_val[i0].maxresponse;
hc[i0].responselen=arg4.httpd_conn_rpc_ptr_val[i0].responselen;
hc[i0].if_modified_since=arg4.httpd_conn_rpc_ptr_val[i0].if_modified_since;
hc[i0].range_if=arg4.httpd_conn_rpc_ptr_val[i0].range_if;
hc[i0].contentlength=arg4.httpd_conn_rpc_ptr_val[i0].contentlength;
if (arg4.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_val == NULL) {hc[i0].type=NULL;}

else {hc[i0].type=malloc(arg4.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_len);

memcpy(hc[i0].type,arg4.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_len);

}

if (arg4.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_val == NULL) {hc[i0].hostname=NULL;}

else {hc[i0].hostname=malloc(arg4.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_len);

memcpy(hc[i0].hostname,arg4.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_len);

}

hc[i0].mime_flag=arg4.httpd_conn_rpc_ptr_val[i0].mime_flag;
hc[i0].one_one=arg4.httpd_conn_rpc_ptr_val[i0].one_one;
hc[i0].got_range=arg4.httpd_conn_rpc_ptr_val[i0].got_range;
hc[i0].tildemapped=arg4.httpd_conn_rpc_ptr_val[i0].tildemapped;
hc[i0].first_byte_index=arg4.httpd_conn_rpc_ptr_val[i0].first_byte_index;
hc[i0].last_byte_index=arg4.httpd_conn_rpc_ptr_val[i0].last_byte_index;
hc[i0].keep_alive=arg4.httpd_conn_rpc_ptr_val[i0].keep_alive;
hc[i0].should_linger=arg4.httpd_conn_rpc_ptr_val[i0].should_linger;
hc[i0].sb.st_dev=arg4.httpd_conn_rpc_ptr_val[i0].sb.st_dev;
hc[i0].sb.st_ino=arg4.httpd_conn_rpc_ptr_val[i0].sb.st_ino;
hc[i0].sb.st_mode=arg4.httpd_conn_rpc_ptr_val[i0].sb.st_mode;
hc[i0].sb.st_nlink=arg4.httpd_conn_rpc_ptr_val[i0].sb.st_nlink;
hc[i0].sb.st_uid=arg4.httpd_conn_rpc_ptr_val[i0].sb.st_uid;
hc[i0].sb.st_gid=arg4.httpd_conn_rpc_ptr_val[i0].sb.st_gid;
hc[i0].sb.st_rdev=arg4.httpd_conn_rpc_ptr_val[i0].sb.st_rdev;
hc[i0].sb.st_size=arg4.httpd_conn_rpc_ptr_val[i0].sb.st_size;
hc[i0].sb.st_blksize=arg4.httpd_conn_rpc_ptr_val[i0].sb.st_blksize;
hc[i0].sb.st_blocks=arg4.httpd_conn_rpc_ptr_val[i0].sb.st_blocks;
hc[i0].sb.st_atime=arg4.httpd_conn_rpc_ptr_val[i0].sb.st_atime;
hc[i0].sb.st_mtime=arg4.httpd_conn_rpc_ptr_val[i0].sb.st_mtime;
hc[i0].sb.st_ctime=arg4.httpd_conn_rpc_ptr_val[i0].sb.st_ctime;
hc[i0].conn_fd=arg4.httpd_conn_rpc_ptr_val[i0].conn_fd;
if (arg4.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_val == NULL) {hc[i0].file_address=NULL;}

else {hc[i0].file_address=malloc(arg4.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_len);

memcpy(hc[i0].file_address,arg4.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_val,arg4.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_len);

}

	}
}
struct timeval *nowP=NULL;
if(arg5.timeval_rpc_ptr_val!=NULL) {
nowP=malloc(sizeof(struct timeval));
if(nowP==NULL) {
perror("malloc failed");
exit(1);
}
memcpy(nowP,arg5.timeval_rpc_ptr_val,sizeof(struct timeval));
}
make_log_entry_sense_1(ru,url,bytes,hc,nowP);
if(ru==NULL) result.ru.charptr.charptr_len=0;else result.ru.charptr.charptr_len=strlen(ru)+1;
result.ru.charptr.charptr_val=malloc(result.ru.charptr.charptr_len);
if(result.ru.charptr.charptr_val==NULL) {
perror("malloc failed");
exit(1);
}
memcpy(result.ru.charptr.charptr_val,ru,result.ru.charptr.charptr_len);
memcpy(result.url.url, url, 305);
memcpy(result.bytes.bytes, bytes, 40);
    if(hc==NULL) {
        result.hc.httpd_conn_rpc_ptr_len=0;
        result.hc.httpd_conn_rpc_ptr_val=NULL;
    } else {
        result.hc.httpd_conn_rpc_ptr_len=1;
        result.hc.httpd_conn_rpc_ptr_val=malloc(1*sizeof(httpd_conn_rpc));
        if(result.hc.httpd_conn_rpc_ptr_val==NULL) {
            perror("malloc failed");
            exit(1);
        }
        for(int i0=0;i0<result.hc.httpd_conn_rpc_ptr_len;i0++) {
result.hc.httpd_conn_rpc_ptr_val[i0].initialized=hc[i0].initialized;
if (hc[i0].hs == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val = NULL;}
else {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_len = 1;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val = malloc(result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_len * sizeof(httpd_server_rpc));
	if (result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val == NULL) {perror("malloc failed");exit(1);}
	for (int i1 = 0; i1 < result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_len; i1++) {
if (hc[i0].hs->binding_hostname == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_len = strlen(hc[i0].hs->binding_hostname)+1;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].binding_hostname.charptr.charptr_val = hc[i0].hs->binding_hostname;}

if (hc[i0].hs->server_hostname == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_len = strlen(hc[i0].hs->server_hostname)+1;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].server_hostname.charptr.charptr_val = hc[i0].hs->server_hostname;}

result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].port=hc[i0].hs->port;
if (hc[i0].hs->cgi_pattern == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_len = strlen(hc[i0].hs->cgi_pattern)+1;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_pattern.charptr.charptr_val = hc[i0].hs->cgi_pattern;}

result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_limit=hc[i0].hs->cgi_limit;
result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cgi_count=hc[i0].hs->cgi_count;
if (hc[i0].hs->charset == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_len = strlen(hc[i0].hs->charset)+1;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].charset.charptr.charptr_val = hc[i0].hs->charset;}

if (hc[i0].hs->p3p == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_len = strlen(hc[i0].hs->p3p)+1;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].p3p.charptr.charptr_val = hc[i0].hs->p3p;}

result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].max_age=hc[i0].hs->max_age;
if (hc[i0].hs->cwd == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_len = strlen(hc[i0].hs->cwd)+1;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].cwd.charptr.charptr_val = hc[i0].hs->cwd;}

result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].listen4_fd=hc[i0].hs->listen4_fd;
result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].listen6_fd=hc[i0].hs->listen6_fd;
result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].no_log=hc[i0].hs->no_log;
if (hc[i0].hs->logfp == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_len = strlen(hc[i0].hs->logfp)+1;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].logfp.charptr.charptr_val = hc[i0].hs->logfp;}

result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].no_symlink_check=hc[i0].hs->no_symlink_check;
result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].vhost=hc[i0].hs->vhost;
result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].global_passwd=hc[i0].hs->global_passwd;
if (hc[i0].hs->url_pattern == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_len = strlen(hc[i0].hs->url_pattern)+1;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].url_pattern.charptr.charptr_val = hc[i0].hs->url_pattern;}

if (hc[i0].hs->local_pattern == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_len = strlen(hc[i0].hs->local_pattern)+1;result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].local_pattern.charptr.charptr_val = hc[i0].hs->local_pattern;}

result.hc.httpd_conn_rpc_ptr_val[i0].hs.httpd_server_rpc_ptr_val[i1].no_empty_referrers=hc[i0].hs->no_empty_referrers;
	}
}
if (hc[i0].read_buf == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_len = strlen(hc[i0].read_buf)+1;result.hc.httpd_conn_rpc_ptr_val[i0].read_buf.charptr.charptr_val = hc[i0].read_buf;}

result.hc.httpd_conn_rpc_ptr_val[i0].read_size=hc[i0].read_size;
result.hc.httpd_conn_rpc_ptr_val[i0].read_idx=hc[i0].read_idx;
result.hc.httpd_conn_rpc_ptr_val[i0].checked_idx=hc[i0].checked_idx;
result.hc.httpd_conn_rpc_ptr_val[i0].checked_state=hc[i0].checked_state;
result.hc.httpd_conn_rpc_ptr_val[i0].method=hc[i0].method;
result.hc.httpd_conn_rpc_ptr_val[i0].status=hc[i0].status;
result.hc.httpd_conn_rpc_ptr_val[i0].bytes_to_send=hc[i0].bytes_to_send;
result.hc.httpd_conn_rpc_ptr_val[i0].bytes_sent=hc[i0].bytes_sent;
if (hc[i0].encodedurl == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_len = strlen(hc[i0].encodedurl)+1;result.hc.httpd_conn_rpc_ptr_val[i0].encodedurl.charptr.charptr_val = hc[i0].encodedurl;}

if (hc[i0].decodedurl == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_len = strlen(hc[i0].decodedurl)+1;result.hc.httpd_conn_rpc_ptr_val[i0].decodedurl.charptr.charptr_val = hc[i0].decodedurl;}

if (hc[i0].protocol == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_len = strlen(hc[i0].protocol)+1;result.hc.httpd_conn_rpc_ptr_val[i0].protocol.charptr.charptr_val = hc[i0].protocol;}

if (hc[i0].origfilename == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_len = strlen(hc[i0].origfilename)+1;result.hc.httpd_conn_rpc_ptr_val[i0].origfilename.charptr.charptr_val = hc[i0].origfilename;}

if (hc[i0].expnfilename == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_len = strlen(hc[i0].expnfilename)+1;result.hc.httpd_conn_rpc_ptr_val[i0].expnfilename.charptr.charptr_val = hc[i0].expnfilename;}

if (hc[i0].encodings == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_len = strlen(hc[i0].encodings)+1;result.hc.httpd_conn_rpc_ptr_val[i0].encodings.charptr.charptr_val = hc[i0].encodings;}

if (hc[i0].pathinfo == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_len = strlen(hc[i0].pathinfo)+1;result.hc.httpd_conn_rpc_ptr_val[i0].pathinfo.charptr.charptr_val = hc[i0].pathinfo;}

if (hc[i0].query == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_len = strlen(hc[i0].query)+1;result.hc.httpd_conn_rpc_ptr_val[i0].query.charptr.charptr_val = hc[i0].query;}

if (hc[i0].referrer == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_len = strlen(hc[i0].referrer)+1;result.hc.httpd_conn_rpc_ptr_val[i0].referrer.charptr.charptr_val = hc[i0].referrer;}

if (hc[i0].useragent == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_len = strlen(hc[i0].useragent)+1;result.hc.httpd_conn_rpc_ptr_val[i0].useragent.charptr.charptr_val = hc[i0].useragent;}

if (hc[i0].accept == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_len = strlen(hc[i0].accept)+1;result.hc.httpd_conn_rpc_ptr_val[i0].accept.charptr.charptr_val = hc[i0].accept;}

if (hc[i0].accepte == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_len = strlen(hc[i0].accepte)+1;result.hc.httpd_conn_rpc_ptr_val[i0].accepte.charptr.charptr_val = hc[i0].accepte;}

if (hc[i0].acceptl == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_len = strlen(hc[i0].acceptl)+1;result.hc.httpd_conn_rpc_ptr_val[i0].acceptl.charptr.charptr_val = hc[i0].acceptl;}

if (hc[i0].cookie == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_len = strlen(hc[i0].cookie)+1;result.hc.httpd_conn_rpc_ptr_val[i0].cookie.charptr.charptr_val = hc[i0].cookie;}

if (hc[i0].contenttype == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_len = strlen(hc[i0].contenttype)+1;result.hc.httpd_conn_rpc_ptr_val[i0].contenttype.charptr.charptr_val = hc[i0].contenttype;}

if (hc[i0].reqhost == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_len = strlen(hc[i0].reqhost)+1;result.hc.httpd_conn_rpc_ptr_val[i0].reqhost.charptr.charptr_val = hc[i0].reqhost;}

if (hc[i0].hdrhost == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_len = strlen(hc[i0].hdrhost)+1;result.hc.httpd_conn_rpc_ptr_val[i0].hdrhost.charptr.charptr_val = hc[i0].hdrhost;}

if (hc[i0].hostdir == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_len = strlen(hc[i0].hostdir)+1;result.hc.httpd_conn_rpc_ptr_val[i0].hostdir.charptr.charptr_val = hc[i0].hostdir;}

if (hc[i0].authorization == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_len = strlen(hc[i0].authorization)+1;result.hc.httpd_conn_rpc_ptr_val[i0].authorization.charptr.charptr_val = hc[i0].authorization;}

if (hc[i0].remoteuser == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_len = strlen(hc[i0].remoteuser)+1;result.hc.httpd_conn_rpc_ptr_val[i0].remoteuser.charptr.charptr_val = hc[i0].remoteuser;}

if (hc[i0].response == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_len = strlen(hc[i0].response)+1;result.hc.httpd_conn_rpc_ptr_val[i0].response.charptr.charptr_val = hc[i0].response;}

result.hc.httpd_conn_rpc_ptr_val[i0].maxdecodedurl=hc[i0].maxdecodedurl;
result.hc.httpd_conn_rpc_ptr_val[i0].maxorigfilename=hc[i0].maxorigfilename;
result.hc.httpd_conn_rpc_ptr_val[i0].maxexpnfilename=hc[i0].maxexpnfilename;
result.hc.httpd_conn_rpc_ptr_val[i0].maxencodings=hc[i0].maxencodings;
result.hc.httpd_conn_rpc_ptr_val[i0].maxpathinfo=hc[i0].maxpathinfo;
result.hc.httpd_conn_rpc_ptr_val[i0].maxquery=hc[i0].maxquery;
result.hc.httpd_conn_rpc_ptr_val[i0].maxaccept=hc[i0].maxaccept;
result.hc.httpd_conn_rpc_ptr_val[i0].maxaccepte=hc[i0].maxaccepte;
result.hc.httpd_conn_rpc_ptr_val[i0].maxreqhost=hc[i0].maxreqhost;
result.hc.httpd_conn_rpc_ptr_val[i0].maxhostdir=hc[i0].maxhostdir;
result.hc.httpd_conn_rpc_ptr_val[i0].maxremoteuser=hc[i0].maxremoteuser;
result.hc.httpd_conn_rpc_ptr_val[i0].maxresponse=hc[i0].maxresponse;
result.hc.httpd_conn_rpc_ptr_val[i0].responselen=hc[i0].responselen;
result.hc.httpd_conn_rpc_ptr_val[i0].if_modified_since=hc[i0].if_modified_since;
result.hc.httpd_conn_rpc_ptr_val[i0].range_if=hc[i0].range_if;
result.hc.httpd_conn_rpc_ptr_val[i0].contentlength=hc[i0].contentlength;
if (hc[i0].type == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_len = strlen(hc[i0].type)+1;result.hc.httpd_conn_rpc_ptr_val[i0].type.charptr.charptr_val = hc[i0].type;}

if (hc[i0].hostname == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_len = strlen(hc[i0].hostname)+1;result.hc.httpd_conn_rpc_ptr_val[i0].hostname.charptr.charptr_val = hc[i0].hostname;}

result.hc.httpd_conn_rpc_ptr_val[i0].mime_flag=hc[i0].mime_flag;
result.hc.httpd_conn_rpc_ptr_val[i0].one_one=hc[i0].one_one;
result.hc.httpd_conn_rpc_ptr_val[i0].got_range=hc[i0].got_range;
result.hc.httpd_conn_rpc_ptr_val[i0].tildemapped=hc[i0].tildemapped;
result.hc.httpd_conn_rpc_ptr_val[i0].first_byte_index=hc[i0].first_byte_index;
result.hc.httpd_conn_rpc_ptr_val[i0].last_byte_index=hc[i0].last_byte_index;
result.hc.httpd_conn_rpc_ptr_val[i0].keep_alive=hc[i0].keep_alive;
result.hc.httpd_conn_rpc_ptr_val[i0].should_linger=hc[i0].should_linger;
result.hc.httpd_conn_rpc_ptr_val[i0].sb.st_dev=hc[i0].sb.st_dev;
result.hc.httpd_conn_rpc_ptr_val[i0].sb.st_ino=hc[i0].sb.st_ino;
result.hc.httpd_conn_rpc_ptr_val[i0].sb.st_mode=hc[i0].sb.st_mode;
result.hc.httpd_conn_rpc_ptr_val[i0].sb.st_nlink=hc[i0].sb.st_nlink;
result.hc.httpd_conn_rpc_ptr_val[i0].sb.st_uid=hc[i0].sb.st_uid;
result.hc.httpd_conn_rpc_ptr_val[i0].sb.st_gid=hc[i0].sb.st_gid;
result.hc.httpd_conn_rpc_ptr_val[i0].sb.st_rdev=hc[i0].sb.st_rdev;
result.hc.httpd_conn_rpc_ptr_val[i0].sb.st_size=hc[i0].sb.st_size;
result.hc.httpd_conn_rpc_ptr_val[i0].sb.st_blksize=hc[i0].sb.st_blksize;
result.hc.httpd_conn_rpc_ptr_val[i0].sb.st_blocks=hc[i0].sb.st_blocks;
result.hc.httpd_conn_rpc_ptr_val[i0].sb.st_atim=hc[i0].sb.st_atime;
result.hc.httpd_conn_rpc_ptr_val[i0].sb.st_mtim=hc[i0].sb.st_mtime;
result.hc.httpd_conn_rpc_ptr_val[i0].sb.st_ctim=hc[i0].sb.st_ctime;
result.hc.httpd_conn_rpc_ptr_val[i0].conn_fd=hc[i0].conn_fd;
if (hc[i0].file_address == NULL) {result.hc.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_len = 0;result.hc.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_val = NULL;}

else {result.hc.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_len = strlen(hc[i0].file_address)+1;result.hc.httpd_conn_rpc_ptr_val[i0].file_address.charptr.charptr_val = hc[i0].file_address;}

        }
    }
if(nowP==NULL) {
result.nowP.timeval_rpc_ptr_len=0;
result.nowP.timeval_rpc_ptr_val=NULL;
} else {
result.nowP.timeval_rpc_ptr_len=1;
result.nowP.timeval_rpc_ptr_val=malloc(sizeof(struct timeval));
if(result.nowP.timeval_rpc_ptr_val==NULL) {
perror("malloc failed");
exit(1);
}
memcpy(result.nowP.timeval_rpc_ptr_val,nowP,sizeof(struct timeval));
}
return &result;
}

/* 全局变量访问函数 - 服务端 */
str_alloc_count_ret_t *get_str_alloc_count_1_svc(struct svc_req *rqstp)
{
    static str_alloc_count_ret_t result;
    result.value = str_alloc_count;
    return &result;
}

void *set_str_alloc_count_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1; // 成功标志
    str_alloc_count = value;
    return (void *)&result;
}

typedef enum {
    SIG_UNKNOWN
} FunctionSignature;
typedef struct {
    const char *name;         // 函数名
    void *func_ptr_void;      // 存储为 void* 的函数地址
    FunctionSignature signature; // 函数签名标识符
} FunctionMapEntry;
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
int *gsetter_str_alloc_count_prefix_1_svc(struct svc_req *rqstp)
{
    ++str_alloc_count;
    return &str_alloc_count;
}
