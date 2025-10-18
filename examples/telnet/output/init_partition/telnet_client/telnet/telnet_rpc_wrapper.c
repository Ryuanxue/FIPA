#include "telnet_rpc_wrapper.h"
extern CLIENT *clnt;
int TerminalRead_wrapper(char *buf, int n)
{
    TerminalRead_ret_ *result_rpc;
    int sensefun_ret;
char_ptr buf_rpc;
if(buf==NULL) buf_rpc.charptr.charptr_len=0;else buf_rpc.charptr.charptr_len=strlen(buf)+1;
buf_rpc.charptr.charptr_val=malloc(buf_rpc.charptr.charptr_len);
if(buf_rpc.charptr.charptr_val==NULL) {
perror("malloc failed");
exit(1);
}
memcpy(buf_rpc.charptr.charptr_val,buf,buf_rpc.charptr.charptr_len);
int n_rpc;
n_rpc=n;
result_rpc=TerminalRead_rpc_1(buf_rpc,n_rpc,clnt);
if(result_rpc==(TerminalRead_ret_ *)NULL) {
clnt_perror(clnt,"call failed");
}
sensefun_ret=result_rpc->ret0;
if(result_rpc->buf.charptr.charptr_val==NULL) buf=NULL;else{

buf=malloc(result_rpc->buf.charptr.charptr_len);
memcpy(buf,result_rpc->buf.charptr.charptr_val,result_rpc->buf.charptr.charptr_len);
}
return sensefun_ret;
}
int tn_sense_1_wrapper(int *tn_sense_1_ret, struct addrinfo *result, struct addrinfo *aip, const int on, int *err, char *hostp, char *user)
{
    tn_sense_1_ret_ *result_rpc;
    int sensefun_ret;
int_ptr tn_sense_1_ret_rpc;
if(tn_sense_1_ret!=NULL) {
tn_sense_1_ret_rpc.int_ptr_len=1;
tn_sense_1_ret_rpc.int_ptr_val=malloc(sizeof(int)*tn_sense_1_ret_rpc.int_ptr_len);
if(tn_sense_1_ret_rpc.int_ptr_val==NULL) {
perror("malloc failed");
exit(1);
}
for(int i0=0;i0<tn_sense_1_ret_rpc.int_ptr_len;i0++) {
tn_sense_1_ret_rpc.int_ptr_val[i0]=tn_sense_1_ret[i0];
	}
}else{tn_sense_1_ret_rpc.int_ptr_len=0;tn_sense_1_ret_rpc.int_ptr_val=NULL;}
addrinfo_rpc_ptr result_rpc_ptr;
if(result==NULL) {
result_rpc_ptr.addrinfo_rpc_ptr_len=0;
result_rpc_ptr.addrinfo_rpc_ptr_val=NULL;
} else {
result_rpc_ptr.addrinfo_rpc_ptr_len=1;
result_rpc_ptr.addrinfo_rpc_ptr_val=malloc(1*sizeof(addrinfo_rpc));
if(result_rpc_ptr.addrinfo_rpc_ptr_val==NULL) {
perror("malloc failed");
exit(1);
}
for(int i0=0;i0<result_rpc_ptr.addrinfo_rpc_ptr_len;i0++) {
result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_flags=result[i0].ai_flags;
result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_family=result[i0].ai_family;
result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_socktype=result[i0].ai_socktype;
result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_protocol=result[i0].ai_protocol;
result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_addrlen=result[i0].ai_addrlen;
if (result[i0].ai_addr == NULL) {result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_addr.sockaddr_rpc_ptr_len = 0;result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_addr.sockaddr_rpc_ptr_val = NULL;}
else {result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_addr.sockaddr_rpc_ptr_len = 1;result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_addr.sockaddr_rpc_ptr_val = malloc(result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_addr.sockaddr_rpc_ptr_len * sizeof(sockaddr_rpc));
	if (result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_addr.sockaddr_rpc_ptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	for (int i1 = 0; i1 < result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_addr.sockaddr_rpc_ptr_len; i1++) {
result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_addr.sockaddr_rpc_ptr_val[i1].sa_family=result[i0].ai_addr[i1].sa_family;
	}
}
if (result[i0].ai_canonname == NULL) {result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_canonname.charptr.charptr_len = 0;result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_canonname.charptr.charptr_val = NULL;}
else {result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_canonname.charptr.charptr_len = strlen(result[i0].ai_canonname) + 1;

	result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_canonname.charptr.charptr_val = malloc(result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_canonname.charptr.charptr_len * sizeof(char));
	if (result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_canonname.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_canonname.charptr.charptr_val, result[i0].ai_canonname, result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_canonname.charptr.charptr_len);
}
if (result[i0].ai_next == NULL) {result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_next.addrinfo_rpc_ptr_len = 0;result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_next.addrinfo_rpc_ptr_val = NULL;}
else {result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_next.addrinfo_rpc_ptr_len = 1;result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_next.addrinfo_rpc_ptr_val = malloc(result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_next.addrinfo_rpc_ptr_len * sizeof(addrinfo_rpc));
	if (result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_next.addrinfo_rpc_ptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	for (int i1 = 0; i1 < result_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_next.addrinfo_rpc_ptr_len; i1++) {
	}
}
}
}
addrinfo_rpc_ptr aip_rpc_ptr;
if(aip==NULL) {
aip_rpc_ptr.addrinfo_rpc_ptr_len=0;
aip_rpc_ptr.addrinfo_rpc_ptr_val=NULL;
} else {
aip_rpc_ptr.addrinfo_rpc_ptr_len=1;
aip_rpc_ptr.addrinfo_rpc_ptr_val=malloc(1*sizeof(addrinfo_rpc));
if(aip_rpc_ptr.addrinfo_rpc_ptr_val==NULL) {
perror("malloc failed");
exit(1);
}
for(int i0=0;i0<aip_rpc_ptr.addrinfo_rpc_ptr_len;i0++) {
aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_flags=aip[i0].ai_flags;
aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_family=aip[i0].ai_family;
aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_socktype=aip[i0].ai_socktype;
aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_protocol=aip[i0].ai_protocol;
aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_addrlen=aip[i0].ai_addrlen;
if (aip[i0].ai_addr == NULL) {aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_addr.sockaddr_rpc_ptr_len = 0;aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_addr.sockaddr_rpc_ptr_val = NULL;}
else {aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_addr.sockaddr_rpc_ptr_len = 1;aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_addr.sockaddr_rpc_ptr_val = malloc(aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_addr.sockaddr_rpc_ptr_len * sizeof(sockaddr_rpc));
	if (aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_addr.sockaddr_rpc_ptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	for (int i1 = 0; i1 < aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_addr.sockaddr_rpc_ptr_len; i1++) {
aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_addr.sockaddr_rpc_ptr_val[i1].sa_family=aip[i0].ai_addr[i1].sa_family;
	}
}
if (aip[i0].ai_canonname == NULL) {aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_canonname.charptr.charptr_len = 0;aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_canonname.charptr.charptr_val = NULL;}
else {aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_canonname.charptr.charptr_len = strlen(aip[i0].ai_canonname) + 1;

	aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_canonname.charptr.charptr_val = malloc(aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_canonname.charptr.charptr_len * sizeof(char));
	if (aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_canonname.charptr.charptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	memcpy(aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_canonname.charptr.charptr_val, aip[i0].ai_canonname, aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_canonname.charptr.charptr_len);
}
if (aip[i0].ai_next == NULL) {aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_next.addrinfo_rpc_ptr_len = 0;aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_next.addrinfo_rpc_ptr_val = NULL;}
else {aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_next.addrinfo_rpc_ptr_len = 1;aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_next.addrinfo_rpc_ptr_val = malloc(aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_next.addrinfo_rpc_ptr_len * sizeof(addrinfo_rpc));
	if (aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_next.addrinfo_rpc_ptr_val == NULL) {
		perror("malloc failed");
		exit(1);
	}
	for (int i1 = 0; i1 < aip_rpc_ptr.addrinfo_rpc_ptr_val[i0].ai_next.addrinfo_rpc_ptr_len; i1++) {
	}
}
}
}
int on_rpc;
on_rpc=on;
int_ptr err_rpc;
if(err!=NULL) {
err_rpc.int_ptr_len=1;
err_rpc.int_ptr_val=malloc(sizeof(int)*err_rpc.int_ptr_len);
if(err_rpc.int_ptr_val==NULL) {
perror("malloc failed");
exit(1);
}
for(int i0=0;i0<err_rpc.int_ptr_len;i0++) {
err_rpc.int_ptr_val[i0]=err[i0];
	}
}else{err_rpc.int_ptr_len=0;err_rpc.int_ptr_val=NULL;}
char_ptr hostp_rpc;
if(hostp==NULL) hostp_rpc.charptr.charptr_len=0;else hostp_rpc.charptr.charptr_len=strlen(hostp)+1;
hostp_rpc.charptr.charptr_val=malloc(hostp_rpc.charptr.charptr_len);
if(hostp_rpc.charptr.charptr_val==NULL) {
perror("malloc failed");
exit(1);
}
memcpy(hostp_rpc.charptr.charptr_val,hostp,hostp_rpc.charptr.charptr_len);
char_ptr user_rpc;
if(user==NULL) user_rpc.charptr.charptr_len=0;else user_rpc.charptr.charptr_len=strlen(user)+1;
user_rpc.charptr.charptr_val=malloc(user_rpc.charptr.charptr_len);
if(user_rpc.charptr.charptr_val==NULL) {
perror("malloc failed");
exit(1);
}
memcpy(user_rpc.charptr.charptr_val,user,user_rpc.charptr.charptr_len);
result_rpc=tn_sense_1_rpc_1(tn_sense_1_ret_rpc,result_rpc_ptr,aip_rpc_ptr,on_rpc,err_rpc,hostp_rpc,user_rpc,clnt);
if(result_rpc==(tn_sense_1_ret_ *)NULL) {
clnt_perror(clnt,"call failed");
}
sensefun_ret=result_rpc->ret0;
if(result_rpc->tn_sense_1_ret.int_ptr_val==NULL) {
tn_sense_1_ret=NULL;
} else {
tn_sense_1_ret=(int *)malloc(result_rpc->tn_sense_1_ret.int_ptr_len*sizeof(int));
if(tn_sense_1_ret==NULL) {
perror("malloc failed");
exit(1);
}
for(int i0=0;i0<result_rpc->tn_sense_1_ret.int_ptr_len;i0++) {
tn_sense_1_ret[i0]=result_rpc->tn_sense_1_ret.int_ptr_val[i0];
}
}
if(result_rpc->err.int_ptr_val==NULL) {
err=NULL;
} else {
err=(int *)malloc(result_rpc->err.int_ptr_len*sizeof(int));
if(err==NULL) {
perror("malloc failed");
exit(1);
}
for(int i0=0;i0<result_rpc->err.int_ptr_len;i0++) {
err[i0]=result_rpc->err.int_ptr_val[i0];
}
}
if(result_rpc->hostp.charptr.charptr_val==NULL) hostp=NULL;else{

hostp=malloc(result_rpc->hostp.charptr.charptr_len);
memcpy(hostp,result_rpc->hostp.charptr.charptr_val,result_rpc->hostp.charptr.charptr_len);
}
if(result_rpc->user.charptr.charptr_val==NULL) user=NULL;else{

user=malloc(result_rpc->user.charptr.charptr_len);
memcpy(user,result_rpc->user.charptr.charptr_val,result_rpc->user.charptr.charptr_len);
}
return sensefun_ret;
}
int get_tout_wrapper()
{
    tout_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_tout_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_tout_wrapper(int value)
{
    void *result;
    result = set_tout_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

unsigned char* get_ttyibuf_wrapper()
{
    ttyibuf_array *result;
    static unsigned char local_copy[1024];
    result = get_ttyibuf_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        memset(local_copy, 0, sizeof(local_copy));
        return local_copy;
    }
    memcpy(local_copy, result->ttyibuf, sizeof(local_copy));
    return local_copy;
}

int set_ttyibuf_wrapper(const unsigned char *value)
{
    ttyibuf_array param;
    void *result;
    if (value == NULL) {
        memset(param.ttyibuf, 0, sizeof(param.ttyibuf));
    } else {
        memcpy(param.ttyibuf, value, sizeof(param.ttyibuf));
    }
    result = set_ttyibuf_1(param, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        return 0;
    }
    return 1;
}

unsigned char* get_str_lm_wrapper()
{
    str_lm_array *result;
    static unsigned char local_copy[1];
    result = get_str_lm_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        memset(local_copy, 0, sizeof(local_copy));
        return local_copy;
    }
    memcpy(local_copy, result->str_lm, sizeof(local_copy));
    return local_copy;
}

int set_str_lm_wrapper(const unsigned char *value)
{
    str_lm_array param;
    void *result;
    if (value == NULL) {
        memset(param.str_lm, 0, sizeof(param.str_lm));
    } else {
        memcpy(param.str_lm, value, sizeof(param.str_lm));
    }
    result = set_str_lm_1(param, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        return 0;
    }
    return 1;
}

int get_local_wrapper()
{
    local_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_local_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_local_wrapper(int value)
{
    void *result;
    result = set_local_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

int get_telnetport_wrapper()
{
    telnetport_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_telnetport_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_telnetport_wrapper(int value)
{
    void *result;
    result = set_telnetport_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

unsigned char* get_slc_import_val_wrapper()
{
    slc_import_val_array *result;
    static unsigned char local_copy[1];
    result = get_slc_import_val_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        memset(local_copy, 0, sizeof(local_copy));
        return local_copy;
    }
    memcpy(local_copy, result->slc_import_val, sizeof(local_copy));
    return local_copy;
}

int set_slc_import_val_wrapper(const unsigned char *value)
{
    slc_import_val_array param;
    void *result;
    if (value == NULL) {
        memset(param.slc_import_val, 0, sizeof(param.slc_import_val));
    } else {
        memcpy(param.slc_import_val, value, sizeof(param.slc_import_val));
    }
    result = set_slc_import_val_1(param, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        return 0;
    }
    return 1;
}

char* get_will_wont_resp_wrapper()
{
    will_wont_resp_array *result;
    static char local_copy[256];
    result = get_will_wont_resp_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        memset(local_copy, 0, 256);
        return local_copy;
    }
    strncpy(local_copy, result->will_wont_resp, 256);
    local_copy[256 - 1] = '\0'; // 确保字符串正确终止
    return local_copy;
}

int set_will_wont_resp_wrapper(const char *value)
{
    will_wont_resp_array param;
    void *result;
    if (value == NULL) {
        param.will_wont_resp[0] = '\0';
    } else {
        strncpy(param.will_wont_resp, value, 256);
        param.will_wont_resp[256 - 1] = '\0';
    }
    result = set_will_wont_resp_1(param, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        return 0;
    }
    return 1;
}

int get_eight_wrapper()
{
    eight_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_eight_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_eight_wrapper(int value)
{
    void *result;
    result = set_eight_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

int get_debug_wrapper()
{
    debug_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_debug_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_debug_wrapper(int value)
{
    void *result;
    result = set_debug_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

Clocks get_clocks_wrapper()
{
    clocks_ret_t *result_rpc;
    Clocks default_value = {0};
    result_rpc = get_clocks_1(clnt);
    if (result_rpc == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    default_value.system=result_rpc->value.system;
    default_value.echotoggle=result_rpc->value.echotoggle;
    default_value.modenegotiated=result_rpc->value.modenegotiated;
    default_value.didnetreceive=result_rpc->value.didnetreceive;
    default_value.gotDM=result_rpc->value.gotDM;
    return default_value;
}

int set_clocks_wrapper(Clocks clocks)
{
    void *result;
    Clocks_rpc clocks_rpc;
    clocks_rpc.system=clocks.system;
    clocks_rpc.echotoggle=clocks.echotoggle;
    clocks_rpc.modenegotiated=clocks.modenegotiated;
    clocks_rpc.didnetreceive=clocks.didnetreceive;
    clocks_rpc.gotDM=clocks.gotDM;
    result = set_clocks_1(clocks_rpc, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

int get_clocks_system_wrapper()
{
    clocks_system_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_clocks_system_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_clocks_system_wrapper(int value)
{
    void *result;
    result = set_clocks_system_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_clocks_echotoggle_wrapper()
{
    clocks_echotoggle_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_clocks_echotoggle_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_clocks_echotoggle_wrapper(int value)
{
    void *result;
    result = set_clocks_echotoggle_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_clocks_modenegotiated_wrapper()
{
    clocks_modenegotiated_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_clocks_modenegotiated_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_clocks_modenegotiated_wrapper(int value)
{
    void *result;
    result = set_clocks_modenegotiated_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_clocks_didnetreceive_wrapper()
{
    clocks_didnetreceive_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_clocks_didnetreceive_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_clocks_didnetreceive_wrapper(int value)
{
    void *result;
    result = set_clocks_didnetreceive_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_clocks_gotDM_wrapper()
{
    clocks_gotDM_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_clocks_gotDM_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_clocks_gotDM_wrapper(int value)
{
    void *result;
    result = set_clocks_gotDM_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char* get_do_dont_resp_wrapper()
{
    do_dont_resp_array *result;
    static char local_copy[256];
    result = get_do_dont_resp_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        memset(local_copy, 0, 256);
        return local_copy;
    }
    strncpy(local_copy, result->do_dont_resp, 256);
    local_copy[256 - 1] = '\0'; // 确保字符串正确终止
    return local_copy;
}

int set_do_dont_resp_wrapper(const char *value)
{
    do_dont_resp_array param;
    void *result;
    if (value == NULL) {
        param.do_dont_resp[0] = '\0';
    } else {
        strncpy(param.do_dont_resp, value, 256);
        param.do_dont_resp[256 - 1] = '\0';
    }
    result = set_do_dont_resp_1(param, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        return 0;
    }
    return 1;
}

unsigned char* get_slc_import_def_wrapper()
{
    slc_import_def_array *result;
    static unsigned char local_copy[1];
    result = get_slc_import_def_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        memset(local_copy, 0, sizeof(local_copy));
        return local_copy;
    }
    memcpy(local_copy, result->slc_import_def, sizeof(local_copy));
    return local_copy;
}

int set_slc_import_def_wrapper(const unsigned char *value)
{
    slc_import_def_array param;
    void *result;
    if (value == NULL) {
        memset(param.slc_import_def, 0, sizeof(param.slc_import_def));
    } else {
        memcpy(param.slc_import_def, value, sizeof(param.slc_import_def));
    }
    result = set_slc_import_def_1(param, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        return 0;
    }
    return 1;
}

fd_set get_ibits_wrapper()
{
    ibits_ret_t *result_rpc;
    fd_set default_value = {0};
    result_rpc = get_ibits_1(clnt);
    if (result_rpc == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return default_value;
}

int set_ibits_wrapper(fd_set ibits)
{
    void *result;
    fd_set_rpc ibits_rpc;
    result = set_ibits_1(ibits_rpc, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

char* get_prompt_wrapper()
{
    char_ptr *result;
    char *ret_val = NULL;
    result = get_prompt_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        return NULL;
    }
    if (result->charptr.charptr_val != NULL) {
        ret_val=malloc(result->charptr.charptr_len);
        memcpy(ret_val, result->charptr.charptr_val, result->charptr.charptr_len);
    }
    return ret_val;
}

void set_prompt_wrapper(const char *value)
{
    void *result;
    char_ptr rpc_value;
    if (value == NULL) {
        rpc_value.charptr.charptr_len = 0;
        rpc_value.charptr.charptr_val = NULL;
    } else {
        rpc_value.charptr.charptr_len = strlen(value) + 1;
        rpc_value.charptr.charptr_val = (char*)value;
    }
    result = set_prompt_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
    }
}

int get_skiprc_wrapper()
{
    skiprc_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_skiprc_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_skiprc_wrapper(int value)
{
    void *result;
    result = set_skiprc_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

int get_net_wrapper()
{
    net_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_net_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_net_wrapper(int value)
{
    void *result;
    result = set_net_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

unsigned char* get_str_lm_mode_wrapper()
{
    str_lm_mode_array *result;
    static unsigned char local_copy[1];
    result = get_str_lm_mode_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        memset(local_copy, 0, sizeof(local_copy));
        return local_copy;
    }
    memcpy(local_copy, result->str_lm_mode, sizeof(local_copy));
    return local_copy;
}

int set_str_lm_mode_wrapper(const unsigned char *value)
{
    str_lm_mode_array param;
    void *result;
    if (value == NULL) {
        memset(param.str_lm_mode, 0, sizeof(param.str_lm_mode));
    } else {
        memcpy(param.str_lm_mode, value, sizeof(param.str_lm_mode));
    }
    result = set_str_lm_mode_1(param, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        return 0;
    }
    return 1;
}

int get_autologin_wrapper()
{
    autologin_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_autologin_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_autologin_wrapper(int value)
{
    void *result;
    result = set_autologin_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

char* get_rcname_wrapper()
{
    char_ptr *result;
    char *ret_val = NULL;
    result = get_rcname_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        return NULL;
    }
    if (result->charptr.charptr_val != NULL) {
        ret_val=malloc(result->charptr.charptr_len);
        memcpy(ret_val, result->charptr.charptr_val, result->charptr.charptr_len);
    }
    return ret_val;
}

void set_rcname_wrapper(const char *value)
{
    void *result;
    char_ptr rpc_value;
    if (value == NULL) {
        rpc_value.charptr.charptr_len = 0;
        rpc_value.charptr.charptr_val = NULL;
    } else {
        rpc_value.charptr.charptr_len = strlen(value) + 1;
        rpc_value.charptr.charptr_val = (char*)value;
    }
    result = set_rcname_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
    }
}

fd_set get_obits_wrapper()
{
    obits_ret_t *result_rpc;
    fd_set default_value = {0};
    result_rpc = get_obits_1(clnt);
    if (result_rpc == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return default_value;
}

int set_obits_wrapper(fd_set obits)
{
    void *result;
    fd_set_rpc obits_rpc;
    result = set_obits_1(obits_rpc, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

char* get_termbuf_wrapper()
{
    termbuf_array *result;
    static char local_copy[2048];
    result = get_termbuf_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        memset(local_copy, 0, 2048);
        return local_copy;
    }
    strncpy(local_copy, result->termbuf, 2048);
    local_copy[2048 - 1] = '\0'; // 确保字符串正确终止
    return local_copy;
}

int set_termbuf_wrapper(const char *value)
{
    termbuf_array param;
    void *result;
    if (value == NULL) {
        param.termbuf[0] = '\0';
    } else {
        strncpy(param.termbuf, value, 2048);
        param.termbuf[2048 - 1] = '\0';
    }
    result = set_termbuf_1(param, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        return 0;
    }
    return 1;
}

Ring get_ttyiring_wrapper()
{
    ttyiring_ret_t *result_rpc;
    Ring default_value = {0};
    result_rpc = get_ttyiring_1(clnt);
    if (result_rpc == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    if (result_rpc->value.consume_rpc.unsigned_ptr_val == NULL) {default_value.consume=NULL;}
    else {default_value.consume=(unsigned *)malloc(result_rpc->value.consume_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (default_value.consume == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < result_rpc->value.consume_rpc.unsigned_ptr_len; i++) {
    		default_value.consume[i] = result_rpc->value.consume_rpc.unsigned_ptr_val[i];
    	}
    }
    if (result_rpc->value.supply_rpc.unsigned_ptr_val == NULL) {default_value.supply=NULL;}
    else {default_value.supply=(unsigned *)malloc(result_rpc->value.supply_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (default_value.supply == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < result_rpc->value.supply_rpc.unsigned_ptr_len; i++) {
    		default_value.supply[i] = result_rpc->value.supply_rpc.unsigned_ptr_val[i];
    	}
    }
    if (result_rpc->value.bottom_rpc.unsigned_ptr_val == NULL) {default_value.bottom=NULL;}
    else {default_value.bottom=(unsigned *)malloc(result_rpc->value.bottom_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (default_value.bottom == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < result_rpc->value.bottom_rpc.unsigned_ptr_len; i++) {
    		default_value.bottom[i] = result_rpc->value.bottom_rpc.unsigned_ptr_val[i];
    	}
    }
    if (result_rpc->value.top_rpc.unsigned_ptr_val == NULL) {default_value.top=NULL;}
    else {default_value.top=(unsigned *)malloc(result_rpc->value.top_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (default_value.top == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < result_rpc->value.top_rpc.unsigned_ptr_len; i++) {
    		default_value.top[i] = result_rpc->value.top_rpc.unsigned_ptr_val[i];
    	}
    }
    if (result_rpc->value.mark_rpc.unsigned_ptr_val == NULL) {default_value.mark=NULL;}
    else {default_value.mark=(unsigned *)malloc(result_rpc->value.mark_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (default_value.mark == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < result_rpc->value.mark_rpc.unsigned_ptr_len; i++) {
    		default_value.mark[i] = result_rpc->value.mark_rpc.unsigned_ptr_val[i];
    	}
    }
    default_value.size=result_rpc->value.size;
    default_value.consumetime=result_rpc->value.consumetime;
    default_value.supplytime=result_rpc->value.supplytime;
    return default_value;
}

int set_ttyiring_wrapper(Ring ttyiring)
{
    void *result;
    Ring_rpc ttyiring_rpc;
    if (ttyiring.consume== NULL) {ttyiring_rpc.consume_rpc.unsigned_ptr_len = 0;ttyiring_rpc.consume_rpc.unsigned_ptr_val = NULL;}
    else {ttyiring_rpc.consume_rpc.unsigned_ptr_len = 1;ttyiring_rpc.consume_rpc.unsigned_ptr_val = malloc(ttyiring_rpc.consume_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (ttyiring_rpc.consume_rpc.unsigned_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < ttyiring_rpc.consume_rpc.unsigned_ptr_len; i++) {
    		ttyiring_rpc.consume_rpc.unsigned_ptr_val[i] = ttyiring.consume[i];
    	}
    }
    if (ttyiring.supply== NULL) {ttyiring_rpc.supply_rpc.unsigned_ptr_len = 0;ttyiring_rpc.supply_rpc.unsigned_ptr_val = NULL;}
    else {ttyiring_rpc.supply_rpc.unsigned_ptr_len = 1;ttyiring_rpc.supply_rpc.unsigned_ptr_val = malloc(ttyiring_rpc.supply_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (ttyiring_rpc.supply_rpc.unsigned_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < ttyiring_rpc.supply_rpc.unsigned_ptr_len; i++) {
    		ttyiring_rpc.supply_rpc.unsigned_ptr_val[i] = ttyiring.supply[i];
    	}
    }
    if (ttyiring.bottom== NULL) {ttyiring_rpc.bottom_rpc.unsigned_ptr_len = 0;ttyiring_rpc.bottom_rpc.unsigned_ptr_val = NULL;}
    else {ttyiring_rpc.bottom_rpc.unsigned_ptr_len = 1;ttyiring_rpc.bottom_rpc.unsigned_ptr_val = malloc(ttyiring_rpc.bottom_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (ttyiring_rpc.bottom_rpc.unsigned_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < ttyiring_rpc.bottom_rpc.unsigned_ptr_len; i++) {
    		ttyiring_rpc.bottom_rpc.unsigned_ptr_val[i] = ttyiring.bottom[i];
    	}
    }
    if (ttyiring.top== NULL) {ttyiring_rpc.top_rpc.unsigned_ptr_len = 0;ttyiring_rpc.top_rpc.unsigned_ptr_val = NULL;}
    else {ttyiring_rpc.top_rpc.unsigned_ptr_len = 1;ttyiring_rpc.top_rpc.unsigned_ptr_val = malloc(ttyiring_rpc.top_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (ttyiring_rpc.top_rpc.unsigned_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < ttyiring_rpc.top_rpc.unsigned_ptr_len; i++) {
    		ttyiring_rpc.top_rpc.unsigned_ptr_val[i] = ttyiring.top[i];
    	}
    }
    if (ttyiring.mark== NULL) {ttyiring_rpc.mark_rpc.unsigned_ptr_len = 0;ttyiring_rpc.mark_rpc.unsigned_ptr_val = NULL;}
    else {ttyiring_rpc.mark_rpc.unsigned_ptr_len = 1;ttyiring_rpc.mark_rpc.unsigned_ptr_val = malloc(ttyiring_rpc.mark_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (ttyiring_rpc.mark_rpc.unsigned_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < ttyiring_rpc.mark_rpc.unsigned_ptr_len; i++) {
    		ttyiring_rpc.mark_rpc.unsigned_ptr_val[i] = ttyiring.mark[i];
    	}
    }
    ttyiring_rpc.size=ttyiring.size;
    ttyiring_rpc.consumetime=ttyiring.consumetime;
    ttyiring_rpc.supplytime=ttyiring.supplytime;
    result = set_ttyiring_1(ttyiring_rpc, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

unsigned char * get_ttyiring_consume_wrapper()
{
    ttyiring_consume_ret_t *result;
    unsigned char * ret_val = NULL;
    result = get_ttyiring_consume_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_ttyiring_consume_wrapper(unsigned char * value)
{
    void *result;
    result = set_ttyiring_consume_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned char * get_ttyiring_supply_wrapper()
{
    ttyiring_supply_ret_t *result;
    unsigned char * ret_val = NULL;
    result = get_ttyiring_supply_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_ttyiring_supply_wrapper(unsigned char * value)
{
    void *result;
    result = set_ttyiring_supply_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned char * get_ttyiring_bottom_wrapper()
{
    ttyiring_bottom_ret_t *result;
    unsigned char * ret_val = NULL;
    result = get_ttyiring_bottom_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_ttyiring_bottom_wrapper(unsigned char * value)
{
    void *result;
    result = set_ttyiring_bottom_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned char * get_ttyiring_top_wrapper()
{
    ttyiring_top_ret_t *result;
    unsigned char * ret_val = NULL;
    result = get_ttyiring_top_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_ttyiring_top_wrapper(unsigned char * value)
{
    void *result;
    result = set_ttyiring_top_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned char * get_ttyiring_mark_wrapper()
{
    ttyiring_mark_ret_t *result;
    unsigned char * ret_val = NULL;
    result = get_ttyiring_mark_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_ttyiring_mark_wrapper(unsigned char * value)
{
    void *result;
    result = set_ttyiring_mark_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_ttyiring_size_wrapper()
{
    ttyiring_size_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_ttyiring_size_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_ttyiring_size_wrapper(int value)
{
    void *result;
    result = set_ttyiring_size_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned long get_ttyiring_consumetime_wrapper()
{
    ttyiring_consumetime_ret_t *result;
    unsigned long ret_val = 0; // 默认值
    result = get_ttyiring_consumetime_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_ttyiring_consumetime_wrapper(unsigned long value)
{
    void *result;
    result = set_ttyiring_consumetime_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned long get_ttyiring_supplytime_wrapper()
{
    ttyiring_supplytime_ret_t *result;
    unsigned long ret_val = 0; // 默认值
    result = get_ttyiring_supplytime_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_ttyiring_supplytime_wrapper(unsigned long value)
{
    void *result;
    result = set_ttyiring_supplytime_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned char* get_netibuf_wrapper()
{
    netibuf_array *result;
    static unsigned char local_copy[1024];
    result = get_netibuf_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        memset(local_copy, 0, sizeof(local_copy));
        return local_copy;
    }
    memcpy(local_copy, result->netibuf, sizeof(local_copy));
    return local_copy;
}

int set_netibuf_wrapper(const unsigned char *value)
{
    netibuf_array param;
    void *result;
    if (value == NULL) {
        memset(param.netibuf, 0, sizeof(param.netibuf));
    } else {
        memcpy(param.netibuf, value, sizeof(param.netibuf));
    }
    result = set_netibuf_1(param, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        return 0;
    }
    return 1;
}

unsigned char* get_ttyobuf_wrapper()
{
    ttyobuf_array *result;
    static unsigned char local_copy[2048];
    result = get_ttyobuf_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        memset(local_copy, 0, sizeof(local_copy));
        return local_copy;
    }
    memcpy(local_copy, result->ttyobuf, sizeof(local_copy));
    return local_copy;
}

int set_ttyobuf_wrapper(const unsigned char *value)
{
    ttyobuf_array param;
    void *result;
    if (value == NULL) {
        memset(param.ttyobuf, 0, sizeof(param.ttyobuf));
    } else {
        memcpy(param.ttyobuf, value, sizeof(param.ttyobuf));
    }
    result = set_ttyobuf_1(param, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        return 0;
    }
    return 1;
}

Ring get_ttyoring_wrapper()
{
    ttyoring_ret_t *result_rpc;
    Ring default_value = {0};
    result_rpc = get_ttyoring_1(clnt);
    if (result_rpc == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    if (result_rpc->value.consume_rpc.unsigned_ptr_val == NULL) {default_value.consume=NULL;}
    else {default_value.consume=(unsigned *)malloc(result_rpc->value.consume_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (default_value.consume == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < result_rpc->value.consume_rpc.unsigned_ptr_len; i++) {
    		default_value.consume[i] = result_rpc->value.consume_rpc.unsigned_ptr_val[i];
    	}
    }
    if (result_rpc->value.supply_rpc.unsigned_ptr_val == NULL) {default_value.supply=NULL;}
    else {default_value.supply=(unsigned *)malloc(result_rpc->value.supply_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (default_value.supply == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < result_rpc->value.supply_rpc.unsigned_ptr_len; i++) {
    		default_value.supply[i] = result_rpc->value.supply_rpc.unsigned_ptr_val[i];
    	}
    }
    if (result_rpc->value.bottom_rpc.unsigned_ptr_val == NULL) {default_value.bottom=NULL;}
    else {default_value.bottom=(unsigned *)malloc(result_rpc->value.bottom_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (default_value.bottom == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < result_rpc->value.bottom_rpc.unsigned_ptr_len; i++) {
    		default_value.bottom[i] = result_rpc->value.bottom_rpc.unsigned_ptr_val[i];
    	}
    }
    if (result_rpc->value.top_rpc.unsigned_ptr_val == NULL) {default_value.top=NULL;}
    else {default_value.top=(unsigned *)malloc(result_rpc->value.top_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (default_value.top == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < result_rpc->value.top_rpc.unsigned_ptr_len; i++) {
    		default_value.top[i] = result_rpc->value.top_rpc.unsigned_ptr_val[i];
    	}
    }
    if (result_rpc->value.mark_rpc.unsigned_ptr_val == NULL) {default_value.mark=NULL;}
    else {default_value.mark=(unsigned *)malloc(result_rpc->value.mark_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (default_value.mark == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < result_rpc->value.mark_rpc.unsigned_ptr_len; i++) {
    		default_value.mark[i] = result_rpc->value.mark_rpc.unsigned_ptr_val[i];
    	}
    }
    default_value.size=result_rpc->value.size;
    default_value.consumetime=result_rpc->value.consumetime;
    default_value.supplytime=result_rpc->value.supplytime;
    return default_value;
}

int set_ttyoring_wrapper(Ring ttyoring)
{
    void *result;
    Ring_rpc ttyoring_rpc;
    if (ttyoring.consume== NULL) {ttyoring_rpc.consume_rpc.unsigned_ptr_len = 0;ttyoring_rpc.consume_rpc.unsigned_ptr_val = NULL;}
    else {ttyoring_rpc.consume_rpc.unsigned_ptr_len = 1;ttyoring_rpc.consume_rpc.unsigned_ptr_val = malloc(ttyoring_rpc.consume_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (ttyoring_rpc.consume_rpc.unsigned_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < ttyoring_rpc.consume_rpc.unsigned_ptr_len; i++) {
    		ttyoring_rpc.consume_rpc.unsigned_ptr_val[i] = ttyoring.consume[i];
    	}
    }
    if (ttyoring.supply== NULL) {ttyoring_rpc.supply_rpc.unsigned_ptr_len = 0;ttyoring_rpc.supply_rpc.unsigned_ptr_val = NULL;}
    else {ttyoring_rpc.supply_rpc.unsigned_ptr_len = 1;ttyoring_rpc.supply_rpc.unsigned_ptr_val = malloc(ttyoring_rpc.supply_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (ttyoring_rpc.supply_rpc.unsigned_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < ttyoring_rpc.supply_rpc.unsigned_ptr_len; i++) {
    		ttyoring_rpc.supply_rpc.unsigned_ptr_val[i] = ttyoring.supply[i];
    	}
    }
    if (ttyoring.bottom== NULL) {ttyoring_rpc.bottom_rpc.unsigned_ptr_len = 0;ttyoring_rpc.bottom_rpc.unsigned_ptr_val = NULL;}
    else {ttyoring_rpc.bottom_rpc.unsigned_ptr_len = 1;ttyoring_rpc.bottom_rpc.unsigned_ptr_val = malloc(ttyoring_rpc.bottom_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (ttyoring_rpc.bottom_rpc.unsigned_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < ttyoring_rpc.bottom_rpc.unsigned_ptr_len; i++) {
    		ttyoring_rpc.bottom_rpc.unsigned_ptr_val[i] = ttyoring.bottom[i];
    	}
    }
    if (ttyoring.top== NULL) {ttyoring_rpc.top_rpc.unsigned_ptr_len = 0;ttyoring_rpc.top_rpc.unsigned_ptr_val = NULL;}
    else {ttyoring_rpc.top_rpc.unsigned_ptr_len = 1;ttyoring_rpc.top_rpc.unsigned_ptr_val = malloc(ttyoring_rpc.top_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (ttyoring_rpc.top_rpc.unsigned_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < ttyoring_rpc.top_rpc.unsigned_ptr_len; i++) {
    		ttyoring_rpc.top_rpc.unsigned_ptr_val[i] = ttyoring.top[i];
    	}
    }
    if (ttyoring.mark== NULL) {ttyoring_rpc.mark_rpc.unsigned_ptr_len = 0;ttyoring_rpc.mark_rpc.unsigned_ptr_val = NULL;}
    else {ttyoring_rpc.mark_rpc.unsigned_ptr_len = 1;ttyoring_rpc.mark_rpc.unsigned_ptr_val = malloc(ttyoring_rpc.mark_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (ttyoring_rpc.mark_rpc.unsigned_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < ttyoring_rpc.mark_rpc.unsigned_ptr_len; i++) {
    		ttyoring_rpc.mark_rpc.unsigned_ptr_val[i] = ttyoring.mark[i];
    	}
    }
    ttyoring_rpc.size=ttyoring.size;
    ttyoring_rpc.consumetime=ttyoring.consumetime;
    ttyoring_rpc.supplytime=ttyoring.supplytime;
    result = set_ttyoring_1(ttyoring_rpc, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

unsigned char * get_ttyoring_consume_wrapper()
{
    ttyoring_consume_ret_t *result;
    unsigned char * ret_val = NULL;
    result = get_ttyoring_consume_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_ttyoring_consume_wrapper(unsigned char * value)
{
    void *result;
    result = set_ttyoring_consume_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned char * get_ttyoring_supply_wrapper()
{
    ttyoring_supply_ret_t *result;
    unsigned char * ret_val = NULL;
    result = get_ttyoring_supply_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_ttyoring_supply_wrapper(unsigned char * value)
{
    void *result;
    result = set_ttyoring_supply_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned char * get_ttyoring_bottom_wrapper()
{
    ttyoring_bottom_ret_t *result;
    unsigned char * ret_val = NULL;
    result = get_ttyoring_bottom_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_ttyoring_bottom_wrapper(unsigned char * value)
{
    void *result;
    result = set_ttyoring_bottom_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned char * get_ttyoring_top_wrapper()
{
    ttyoring_top_ret_t *result;
    unsigned char * ret_val = NULL;
    result = get_ttyoring_top_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_ttyoring_top_wrapper(unsigned char * value)
{
    void *result;
    result = set_ttyoring_top_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned char * get_ttyoring_mark_wrapper()
{
    ttyoring_mark_ret_t *result;
    unsigned char * ret_val = NULL;
    result = get_ttyoring_mark_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_ttyoring_mark_wrapper(unsigned char * value)
{
    void *result;
    result = set_ttyoring_mark_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_ttyoring_size_wrapper()
{
    ttyoring_size_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_ttyoring_size_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_ttyoring_size_wrapper(int value)
{
    void *result;
    result = set_ttyoring_size_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned long get_ttyoring_consumetime_wrapper()
{
    ttyoring_consumetime_ret_t *result;
    unsigned long ret_val = 0; // 默认值
    result = get_ttyoring_consumetime_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_ttyoring_consumetime_wrapper(unsigned long value)
{
    void *result;
    result = set_ttyoring_consumetime_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned long get_ttyoring_supplytime_wrapper()
{
    ttyoring_supplytime_ret_t *result;
    unsigned long ret_val = 0; // 默认值
    result = get_ttyoring_supplytime_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_ttyoring_supplytime_wrapper(unsigned long value)
{
    void *result;
    result = set_ttyoring_supplytime_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_donebinarytoggle_wrapper()
{
    donebinarytoggle_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_donebinarytoggle_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_donebinarytoggle_wrapper(int value)
{
    void *result;
    result = set_donebinarytoggle_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

char* get_user_wrapper()
{
    char_ptr *result;
    char *ret_val = NULL;
    result = get_user_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        return NULL;
    }
    if (result->charptr.charptr_val != NULL) {
        ret_val=malloc(result->charptr.charptr_len);
        memcpy(ret_val, result->charptr.charptr_val, result->charptr.charptr_len);
    }
    return ret_val;
}

void set_user_wrapper(const char *value)
{
    void *result;
    char_ptr rpc_value;
    if (value == NULL) {
        rpc_value.charptr.charptr_len = 0;
        rpc_value.charptr.charptr_val = NULL;
    } else {
        rpc_value.charptr.charptr_len = strlen(value) + 1;
        rpc_value.charptr.charptr_val = (char*)value;
    }
    result = set_user_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
    }
}

unsigned long get_ring_clock_wrapper()
{
    ring_clock_ret_t *result;
    unsigned long default_value = {0}; // 初始化默认值
    result = get_ring_clock_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_ring_clock_wrapper(unsigned long value)
{
    void *result;
    result = set_ring_clock_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

int get_tin_wrapper()
{
    tin_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_tin_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_tin_wrapper(int value)
{
    void *result;
    result = set_tin_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

char** get_telopts_wrapper()
{
    telopts_array *result;
    static char *local_copy[41];
    result = get_telopts_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        memset(local_copy, 0, sizeof(local_copy));
        return local_copy;
    }
    for (int i = 0; i < 41; i++) {
        if (result->telopts[i].charptr.charptr_val != NULL) {
            local_copy[i] = malloc(result->telopts[i].charptr.charptr_len);
            if (local_copy[i] != NULL) {
                memcpy(local_copy[i], result->telopts[i].charptr.charptr_val, result->telopts[i].charptr.charptr_len);
            }
        } else {
            local_copy[i] = NULL;
        }
    }
    return local_copy;
}

int set_telopts_wrapper(const char **value)
{
    telopts_array param;
    void *result;
    for (int i = 0; i < 41; i++) {
        if (value == NULL || value[i] == NULL) {
            param.telopts[i].charptr.charptr_len = 0;
            param.telopts[i].charptr.charptr_val = NULL;
        } else {
            int len = strlen(value[i]) + 1;
            param.telopts[i].charptr.charptr_len = len;
            param.telopts[i].charptr.charptr_val = malloc(len);
            if (param.telopts[i].charptr.charptr_val != NULL) {
                memcpy(param.telopts[i].charptr.charptr_val, value[i], len);
            }
        }
    }
    result = set_telopts_1(param, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        return 0;
    }
    return 1;
}

int get_want_status_response_wrapper()
{
    want_status_response_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_want_status_response_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_want_status_response_wrapper(int value)
{
    void *result;
    result = set_want_status_response_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

unsigned char* get_netobuf_wrapper()
{
    netobuf_array *result;
    static unsigned char local_copy[2048];
    result = get_netobuf_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        memset(local_copy, 0, sizeof(local_copy));
        return local_copy;
    }
    memcpy(local_copy, result->netobuf, sizeof(local_copy));
    return local_copy;
}

int set_netobuf_wrapper(const unsigned char *value)
{
    netobuf_array param;
    void *result;
    if (value == NULL) {
        memset(param.netobuf, 0, sizeof(param.netobuf));
    } else {
        memcpy(param.netobuf, value, sizeof(param.netobuf));
    }
    result = set_netobuf_1(param, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        return 0;
    }
    return 1;
}

int get_restartany_wrapper()
{
    restartany_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_restartany_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_restartany_wrapper(int value)
{
    void *result;
    result = set_restartany_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

int get_localflow_wrapper()
{
    localflow_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_localflow_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_localflow_wrapper(int value)
{
    void *result;
    result = set_localflow_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

char** get_margv_wrapper()
{
    margv_array *result;
    static char *local_copy[20];
    result = get_margv_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        memset(local_copy, 0, sizeof(local_copy));
        return local_copy;
    }
    for (int i = 0; i < 20; i++) {
        if (result->margv[i].charptr.charptr_val != NULL) {
            local_copy[i] = malloc(result->margv[i].charptr.charptr_len);
            if (local_copy[i] != NULL) {
                memcpy(local_copy[i], result->margv[i].charptr.charptr_val, result->margv[i].charptr.charptr_len);
            }
        } else {
            local_copy[i] = NULL;
        }
    }
    return local_copy;
}

int set_margv_wrapper(const char **value)
{
    margv_array param;
    void *result;
    for (int i = 0; i < 20; i++) {
        if (value == NULL || value[i] == NULL) {
            param.margv[i].charptr.charptr_len = 0;
            param.margv[i].charptr.charptr_val = NULL;
        } else {
            int len = strlen(value[i]) + 1;
            param.margv[i].charptr.charptr_len = len;
            param.margv[i].charptr.charptr_val = malloc(len);
            if (param.margv[i].charptr.charptr_val != NULL) {
                memcpy(param.margv[i].charptr.charptr_val, value[i], len);
            }
        }
    }
    result = set_margv_1(param, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        return 0;
    }
    return 1;
}

int get_SYNCHing_wrapper()
{
    SYNCHing_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_SYNCHing_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_SYNCHing_wrapper(int value)
{
    void *result;
    result = set_SYNCHing_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

int get_telrcv_state_wrapper()
{
    telrcv_state_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_telrcv_state_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_telrcv_state_wrapper(int value)
{
    void *result;
    result = set_telrcv_state_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

int get_In3270_wrapper()
{
    In3270_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_In3270_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_In3270_wrapper(int value)
{
    void *result;
    result = set_In3270_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

Ring get_netoring_wrapper()
{
    netoring_ret_t *result_rpc;
    Ring default_value = {0};
    result_rpc = get_netoring_1(clnt);
    if (result_rpc == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    if (result_rpc->value.consume_rpc.unsigned_ptr_val == NULL) {default_value.consume=NULL;}
    else {default_value.consume=(unsigned *)malloc(result_rpc->value.consume_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (default_value.consume == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < result_rpc->value.consume_rpc.unsigned_ptr_len; i++) {
    		default_value.consume[i] = result_rpc->value.consume_rpc.unsigned_ptr_val[i];
    	}
    }
    if (result_rpc->value.supply_rpc.unsigned_ptr_val == NULL) {default_value.supply=NULL;}
    else {default_value.supply=(unsigned *)malloc(result_rpc->value.supply_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (default_value.supply == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < result_rpc->value.supply_rpc.unsigned_ptr_len; i++) {
    		default_value.supply[i] = result_rpc->value.supply_rpc.unsigned_ptr_val[i];
    	}
    }
    if (result_rpc->value.bottom_rpc.unsigned_ptr_val == NULL) {default_value.bottom=NULL;}
    else {default_value.bottom=(unsigned *)malloc(result_rpc->value.bottom_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (default_value.bottom == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < result_rpc->value.bottom_rpc.unsigned_ptr_len; i++) {
    		default_value.bottom[i] = result_rpc->value.bottom_rpc.unsigned_ptr_val[i];
    	}
    }
    if (result_rpc->value.top_rpc.unsigned_ptr_val == NULL) {default_value.top=NULL;}
    else {default_value.top=(unsigned *)malloc(result_rpc->value.top_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (default_value.top == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < result_rpc->value.top_rpc.unsigned_ptr_len; i++) {
    		default_value.top[i] = result_rpc->value.top_rpc.unsigned_ptr_val[i];
    	}
    }
    if (result_rpc->value.mark_rpc.unsigned_ptr_val == NULL) {default_value.mark=NULL;}
    else {default_value.mark=(unsigned *)malloc(result_rpc->value.mark_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (default_value.mark == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < result_rpc->value.mark_rpc.unsigned_ptr_len; i++) {
    		default_value.mark[i] = result_rpc->value.mark_rpc.unsigned_ptr_val[i];
    	}
    }
    default_value.size=result_rpc->value.size;
    default_value.consumetime=result_rpc->value.consumetime;
    default_value.supplytime=result_rpc->value.supplytime;
    return default_value;
}

int set_netoring_wrapper(Ring netoring)
{
    void *result;
    Ring_rpc netoring_rpc;
    if (netoring.consume== NULL) {netoring_rpc.consume_rpc.unsigned_ptr_len = 0;netoring_rpc.consume_rpc.unsigned_ptr_val = NULL;}
    else {netoring_rpc.consume_rpc.unsigned_ptr_len = 1;netoring_rpc.consume_rpc.unsigned_ptr_val = malloc(netoring_rpc.consume_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (netoring_rpc.consume_rpc.unsigned_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < netoring_rpc.consume_rpc.unsigned_ptr_len; i++) {
    		netoring_rpc.consume_rpc.unsigned_ptr_val[i] = netoring.consume[i];
    	}
    }
    if (netoring.supply== NULL) {netoring_rpc.supply_rpc.unsigned_ptr_len = 0;netoring_rpc.supply_rpc.unsigned_ptr_val = NULL;}
    else {netoring_rpc.supply_rpc.unsigned_ptr_len = 1;netoring_rpc.supply_rpc.unsigned_ptr_val = malloc(netoring_rpc.supply_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (netoring_rpc.supply_rpc.unsigned_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < netoring_rpc.supply_rpc.unsigned_ptr_len; i++) {
    		netoring_rpc.supply_rpc.unsigned_ptr_val[i] = netoring.supply[i];
    	}
    }
    if (netoring.bottom== NULL) {netoring_rpc.bottom_rpc.unsigned_ptr_len = 0;netoring_rpc.bottom_rpc.unsigned_ptr_val = NULL;}
    else {netoring_rpc.bottom_rpc.unsigned_ptr_len = 1;netoring_rpc.bottom_rpc.unsigned_ptr_val = malloc(netoring_rpc.bottom_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (netoring_rpc.bottom_rpc.unsigned_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < netoring_rpc.bottom_rpc.unsigned_ptr_len; i++) {
    		netoring_rpc.bottom_rpc.unsigned_ptr_val[i] = netoring.bottom[i];
    	}
    }
    if (netoring.top== NULL) {netoring_rpc.top_rpc.unsigned_ptr_len = 0;netoring_rpc.top_rpc.unsigned_ptr_val = NULL;}
    else {netoring_rpc.top_rpc.unsigned_ptr_len = 1;netoring_rpc.top_rpc.unsigned_ptr_val = malloc(netoring_rpc.top_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (netoring_rpc.top_rpc.unsigned_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < netoring_rpc.top_rpc.unsigned_ptr_len; i++) {
    		netoring_rpc.top_rpc.unsigned_ptr_val[i] = netoring.top[i];
    	}
    }
    if (netoring.mark== NULL) {netoring_rpc.mark_rpc.unsigned_ptr_len = 0;netoring_rpc.mark_rpc.unsigned_ptr_val = NULL;}
    else {netoring_rpc.mark_rpc.unsigned_ptr_len = 1;netoring_rpc.mark_rpc.unsigned_ptr_val = malloc(netoring_rpc.mark_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (netoring_rpc.mark_rpc.unsigned_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < netoring_rpc.mark_rpc.unsigned_ptr_len; i++) {
    		netoring_rpc.mark_rpc.unsigned_ptr_val[i] = netoring.mark[i];
    	}
    }
    netoring_rpc.size=netoring.size;
    netoring_rpc.consumetime=netoring.consumetime;
    netoring_rpc.supplytime=netoring.supplytime;
    result = set_netoring_1(netoring_rpc, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

unsigned char * get_netoring_consume_wrapper()
{
    netoring_consume_ret_t *result;
    unsigned char * ret_val = NULL;
    result = get_netoring_consume_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_netoring_consume_wrapper(unsigned char * value)
{
    void *result;
    result = set_netoring_consume_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned char * get_netoring_supply_wrapper()
{
    netoring_supply_ret_t *result;
    unsigned char * ret_val = NULL;
    result = get_netoring_supply_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_netoring_supply_wrapper(unsigned char * value)
{
    void *result;
    result = set_netoring_supply_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned char * get_netoring_bottom_wrapper()
{
    netoring_bottom_ret_t *result;
    unsigned char * ret_val = NULL;
    result = get_netoring_bottom_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_netoring_bottom_wrapper(unsigned char * value)
{
    void *result;
    result = set_netoring_bottom_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned char * get_netoring_top_wrapper()
{
    netoring_top_ret_t *result;
    unsigned char * ret_val = NULL;
    result = get_netoring_top_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_netoring_top_wrapper(unsigned char * value)
{
    void *result;
    result = set_netoring_top_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned char * get_netoring_mark_wrapper()
{
    netoring_mark_ret_t *result;
    unsigned char * ret_val = NULL;
    result = get_netoring_mark_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_netoring_mark_wrapper(unsigned char * value)
{
    void *result;
    result = set_netoring_mark_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_netoring_size_wrapper()
{
    netoring_size_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_netoring_size_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_netoring_size_wrapper(int value)
{
    void *result;
    result = set_netoring_size_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned long get_netoring_consumetime_wrapper()
{
    netoring_consumetime_ret_t *result;
    unsigned long ret_val = 0; // 默认值
    result = get_netoring_consumetime_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_netoring_consumetime_wrapper(unsigned long value)
{
    void *result;
    result = set_netoring_consumetime_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned long get_netoring_supplytime_wrapper()
{
    netoring_supplytime_ret_t *result;
    unsigned long ret_val = 0; // 默认值
    result = get_netoring_supplytime_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_netoring_supplytime_wrapper(unsigned long value)
{
    void *result;
    result = set_netoring_supplytime_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char* get_options_wrapper()
{
    options_array *result;
    static char local_copy[1];
    result = get_options_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        memset(local_copy, 0, 1);
        return local_copy;
    }
    strncpy(local_copy, result->options, 1);
    local_copy[1 - 1] = '\0'; // 确保字符串正确终止
    return local_copy;
}

int set_options_wrapper(const char *value)
{
    options_array param;
    void *result;
    if (value == NULL) {
        param.options[0] = '\0';
    } else {
        strncpy(param.options, value, 1);
        param.options[1 - 1] = '\0';
    }
    result = set_options_1(param, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        return 0;
    }
    return 1;
}

int get_resettermname_wrapper()
{
    resettermname_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_resettermname_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_resettermname_wrapper(int value)
{
    void *result;
    result = set_resettermname_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

char* get_saveline_wrapper()
{
    saveline_array *result;
    static char local_copy[256];
    result = get_saveline_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        memset(local_copy, 0, 256);
        return local_copy;
    }
    strncpy(local_copy, result->saveline, 256);
    local_copy[256 - 1] = '\0'; // 确保字符串正确终止
    return local_copy;
}

int set_saveline_wrapper(const char *value)
{
    saveline_array param;
    void *result;
    if (value == NULL) {
        param.saveline[0] = '\0';
    } else {
        strncpy(param.saveline, value, 256);
        param.saveline[256 - 1] = '\0';
    }
    result = set_saveline_1(param, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        return 0;
    }
    return 1;
}

FILE* get_NetTrace_wrapper()
{
    char_ptr *result;
    FILE *ret_val = NULL;
    result = get_NetTrace_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        return NULL;
    }
    if (result->charptr.charptr_val != NULL) {
        if (strcmp(result->charptr.charptr_val, "stdin") == 0) {
            ret_val = stdin;
        } else if (strcmp(result->charptr.charptr_val, "stdout") == 0) {
            ret_val = stdout;
        } else if (strcmp(result->charptr.charptr_val, "stderr") == 0) {
            ret_val = stderr;
        } else {
            ret_val = fopen(result->charptr.charptr_val, "r+");
            if (ret_val == NULL) {
                perror("fopen failed");
                return NULL;
            }
        }
    }
    return ret_val;
}

void set_NetTrace_wrapper(FILE *file)
{
    void *result;
    char_ptr rpc_value;
    char *filename=get_filename_from_fp(file);
    if (filename == NULL) {
        rpc_value.charptr.charptr_len = 0;
        rpc_value.charptr.charptr_val = NULL;
    } else {
        rpc_value.charptr.charptr_len = strlen(filename) + 1;
        rpc_value.charptr.charptr_val = filename;
    }
    result = set_NetTrace_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
    }
}

char* get_hostname_wrapper()
{
    char_ptr *result;
    char *ret_val = NULL;
    result = get_hostname_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        return NULL;
    }
    if (result->charptr.charptr_val != NULL) {
        ret_val=malloc(result->charptr.charptr_len);
        memcpy(ret_val, result->charptr.charptr_val, result->charptr.charptr_len);
    }
    return ret_val;
}

void set_hostname_wrapper(const char *value)
{
    void *result;
    char_ptr rpc_value;
    if (value == NULL) {
        rpc_value.charptr.charptr_len = 0;
        rpc_value.charptr.charptr_val = NULL;
    } else {
        rpc_value.charptr.charptr_len = strlen(value) + 1;
        rpc_value.charptr.charptr_val = (char*)value;
    }
    result = set_hostname_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
    }
}

int get_slc_mode_wrapper()
{
    slc_mode_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_slc_mode_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_slc_mode_wrapper(int value)
{
    void *result;
    result = set_slc_mode_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

Ring get_netiring_wrapper()
{
    netiring_ret_t *result_rpc;
    Ring default_value = {0};
    result_rpc = get_netiring_1(clnt);
    if (result_rpc == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    if (result_rpc->value.consume_rpc.unsigned_ptr_val == NULL) {default_value.consume=NULL;}
    else {default_value.consume=(unsigned *)malloc(result_rpc->value.consume_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (default_value.consume == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < result_rpc->value.consume_rpc.unsigned_ptr_len; i++) {
    		default_value.consume[i] = result_rpc->value.consume_rpc.unsigned_ptr_val[i];
    	}
    }
    if (result_rpc->value.supply_rpc.unsigned_ptr_val == NULL) {default_value.supply=NULL;}
    else {default_value.supply=(unsigned *)malloc(result_rpc->value.supply_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (default_value.supply == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < result_rpc->value.supply_rpc.unsigned_ptr_len; i++) {
    		default_value.supply[i] = result_rpc->value.supply_rpc.unsigned_ptr_val[i];
    	}
    }
    if (result_rpc->value.bottom_rpc.unsigned_ptr_val == NULL) {default_value.bottom=NULL;}
    else {default_value.bottom=(unsigned *)malloc(result_rpc->value.bottom_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (default_value.bottom == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < result_rpc->value.bottom_rpc.unsigned_ptr_len; i++) {
    		default_value.bottom[i] = result_rpc->value.bottom_rpc.unsigned_ptr_val[i];
    	}
    }
    if (result_rpc->value.top_rpc.unsigned_ptr_val == NULL) {default_value.top=NULL;}
    else {default_value.top=(unsigned *)malloc(result_rpc->value.top_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (default_value.top == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < result_rpc->value.top_rpc.unsigned_ptr_len; i++) {
    		default_value.top[i] = result_rpc->value.top_rpc.unsigned_ptr_val[i];
    	}
    }
    if (result_rpc->value.mark_rpc.unsigned_ptr_val == NULL) {default_value.mark=NULL;}
    else {default_value.mark=(unsigned *)malloc(result_rpc->value.mark_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (default_value.mark == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < result_rpc->value.mark_rpc.unsigned_ptr_len; i++) {
    		default_value.mark[i] = result_rpc->value.mark_rpc.unsigned_ptr_val[i];
    	}
    }
    default_value.size=result_rpc->value.size;
    default_value.consumetime=result_rpc->value.consumetime;
    default_value.supplytime=result_rpc->value.supplytime;
    return default_value;
}

int set_netiring_wrapper(Ring netiring)
{
    void *result;
    Ring_rpc netiring_rpc;
    if (netiring.consume== NULL) {netiring_rpc.consume_rpc.unsigned_ptr_len = 0;netiring_rpc.consume_rpc.unsigned_ptr_val = NULL;}
    else {netiring_rpc.consume_rpc.unsigned_ptr_len = 1;netiring_rpc.consume_rpc.unsigned_ptr_val = malloc(netiring_rpc.consume_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (netiring_rpc.consume_rpc.unsigned_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < netiring_rpc.consume_rpc.unsigned_ptr_len; i++) {
    		netiring_rpc.consume_rpc.unsigned_ptr_val[i] = netiring.consume[i];
    	}
    }
    if (netiring.supply== NULL) {netiring_rpc.supply_rpc.unsigned_ptr_len = 0;netiring_rpc.supply_rpc.unsigned_ptr_val = NULL;}
    else {netiring_rpc.supply_rpc.unsigned_ptr_len = 1;netiring_rpc.supply_rpc.unsigned_ptr_val = malloc(netiring_rpc.supply_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (netiring_rpc.supply_rpc.unsigned_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < netiring_rpc.supply_rpc.unsigned_ptr_len; i++) {
    		netiring_rpc.supply_rpc.unsigned_ptr_val[i] = netiring.supply[i];
    	}
    }
    if (netiring.bottom== NULL) {netiring_rpc.bottom_rpc.unsigned_ptr_len = 0;netiring_rpc.bottom_rpc.unsigned_ptr_val = NULL;}
    else {netiring_rpc.bottom_rpc.unsigned_ptr_len = 1;netiring_rpc.bottom_rpc.unsigned_ptr_val = malloc(netiring_rpc.bottom_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (netiring_rpc.bottom_rpc.unsigned_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < netiring_rpc.bottom_rpc.unsigned_ptr_len; i++) {
    		netiring_rpc.bottom_rpc.unsigned_ptr_val[i] = netiring.bottom[i];
    	}
    }
    if (netiring.top== NULL) {netiring_rpc.top_rpc.unsigned_ptr_len = 0;netiring_rpc.top_rpc.unsigned_ptr_val = NULL;}
    else {netiring_rpc.top_rpc.unsigned_ptr_len = 1;netiring_rpc.top_rpc.unsigned_ptr_val = malloc(netiring_rpc.top_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (netiring_rpc.top_rpc.unsigned_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < netiring_rpc.top_rpc.unsigned_ptr_len; i++) {
    		netiring_rpc.top_rpc.unsigned_ptr_val[i] = netiring.top[i];
    	}
    }
    if (netiring.mark== NULL) {netiring_rpc.mark_rpc.unsigned_ptr_len = 0;netiring_rpc.mark_rpc.unsigned_ptr_val = NULL;}
    else {netiring_rpc.mark_rpc.unsigned_ptr_len = 1;netiring_rpc.mark_rpc.unsigned_ptr_val = malloc(netiring_rpc.mark_rpc.unsigned_ptr_len * sizeof(unsigned));
    	if (netiring_rpc.mark_rpc.unsigned_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < netiring_rpc.mark_rpc.unsigned_ptr_len; i++) {
    		netiring_rpc.mark_rpc.unsigned_ptr_val[i] = netiring.mark[i];
    	}
    }
    netiring_rpc.size=netiring.size;
    netiring_rpc.consumetime=netiring.consumetime;
    netiring_rpc.supplytime=netiring.supplytime;
    result = set_netiring_1(netiring_rpc, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

unsigned char * get_netiring_consume_wrapper()
{
    netiring_consume_ret_t *result;
    unsigned char * ret_val = NULL;
    result = get_netiring_consume_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_netiring_consume_wrapper(unsigned char * value)
{
    void *result;
    result = set_netiring_consume_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned char * get_netiring_supply_wrapper()
{
    netiring_supply_ret_t *result;
    unsigned char * ret_val = NULL;
    result = get_netiring_supply_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_netiring_supply_wrapper(unsigned char * value)
{
    void *result;
    result = set_netiring_supply_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned char * get_netiring_bottom_wrapper()
{
    netiring_bottom_ret_t *result;
    unsigned char * ret_val = NULL;
    result = get_netiring_bottom_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_netiring_bottom_wrapper(unsigned char * value)
{
    void *result;
    result = set_netiring_bottom_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned char * get_netiring_top_wrapper()
{
    netiring_top_ret_t *result;
    unsigned char * ret_val = NULL;
    result = get_netiring_top_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_netiring_top_wrapper(unsigned char * value)
{
    void *result;
    result = set_netiring_top_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned char * get_netiring_mark_wrapper()
{
    netiring_mark_ret_t *result;
    unsigned char * ret_val = NULL;
    result = get_netiring_mark_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_netiring_mark_wrapper(unsigned char * value)
{
    void *result;
    result = set_netiring_mark_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_netiring_size_wrapper()
{
    netiring_size_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_netiring_size_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_netiring_size_wrapper(int value)
{
    void *result;
    result = set_netiring_size_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned long get_netiring_consumetime_wrapper()
{
    netiring_consumetime_ret_t *result;
    unsigned long ret_val = 0; // 默认值
    result = get_netiring_consumetime_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_netiring_consumetime_wrapper(unsigned long value)
{
    void *result;
    result = set_netiring_consumetime_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned long get_netiring_supplytime_wrapper()
{
    netiring_supplytime_ret_t *result;
    unsigned long ret_val = 0; // 默认值
    result = get_netiring_supplytime_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_netiring_supplytime_wrapper(unsigned long value)
{
    void *result;
    result = set_netiring_supplytime_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_linemode_wrapper()
{
    linemode_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_linemode_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_linemode_wrapper(int value)
{
    void *result;
    result = set_linemode_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

int get_margc_wrapper()
{
    margc_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_margc_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_margc_wrapper(int value)
{
    void *result;
    result = set_margc_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

int get_crmod_wrapper()
{
    crmod_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_crmod_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_crmod_wrapper(int value)
{
    void *result;
    result = set_crmod_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

int get_ISend_wrapper()
{
    ISend_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_ISend_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_ISend_wrapper(int value)
{
    void *result;
    result = set_ISend_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

struct termios get_old_tc_wrapper()
{
    old_tc_ret_t *result_rpc;
    struct termios default_value = {0};
    result_rpc = get_old_tc_1(clnt);
    if (result_rpc == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    default_value.c_iflag=result_rpc->value.c_iflag;
    default_value.c_oflag=result_rpc->value.c_oflag;
    default_value.c_cflag=result_rpc->value.c_cflag;
    default_value.c_lflag=result_rpc->value.c_lflag;
    default_value.c_line=result_rpc->value.c_line;
    default_value.c_ispeed=result_rpc->value.c_ispeed;
    default_value.c_ospeed=result_rpc->value.c_ospeed;
    return default_value;
}

int set_old_tc_wrapper(struct termios old_tc)
{
    void *result;
    termios_rpc old_tc_rpc;
    old_tc_rpc.c_iflag=old_tc.c_iflag;
    old_tc_rpc.c_oflag=old_tc.c_oflag;
    old_tc_rpc.c_cflag=old_tc.c_cflag;
    old_tc_rpc.c_lflag=old_tc.c_lflag;
    old_tc_rpc.c_line=old_tc.c_line;
    old_tc_rpc.c_ispeed=old_tc.c_ispeed;
    old_tc_rpc.c_ospeed=old_tc.c_ospeed;
    result = set_old_tc_1(old_tc_rpc, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

unsigned int get_old_tc_c_iflag_wrapper()
{
    old_tc_c_iflag_ret_t *result;
    unsigned int ret_val = 0; // 默认值
    result = get_old_tc_c_iflag_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_old_tc_c_iflag_wrapper(unsigned int value)
{
    void *result;
    result = set_old_tc_c_iflag_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned int get_old_tc_c_oflag_wrapper()
{
    old_tc_c_oflag_ret_t *result;
    unsigned int ret_val = 0; // 默认值
    result = get_old_tc_c_oflag_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_old_tc_c_oflag_wrapper(unsigned int value)
{
    void *result;
    result = set_old_tc_c_oflag_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned int get_old_tc_c_cflag_wrapper()
{
    old_tc_c_cflag_ret_t *result;
    unsigned int ret_val = 0; // 默认值
    result = get_old_tc_c_cflag_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_old_tc_c_cflag_wrapper(unsigned int value)
{
    void *result;
    result = set_old_tc_c_cflag_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned int get_old_tc_c_lflag_wrapper()
{
    old_tc_c_lflag_ret_t *result;
    unsigned int ret_val = 0; // 默认值
    result = get_old_tc_c_lflag_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_old_tc_c_lflag_wrapper(unsigned int value)
{
    void *result;
    result = set_old_tc_c_lflag_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned char get_old_tc_c_line_wrapper()
{
    old_tc_c_line_ret_t *result;
    unsigned char ret_val = 0; // 默认值
    result = get_old_tc_c_line_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_old_tc_c_line_wrapper(unsigned char value)
{
    void *result;
    result = set_old_tc_c_line_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned int get_old_tc_c_ispeed_wrapper()
{
    old_tc_c_ispeed_ret_t *result;
    unsigned int ret_val = 0; // 默认值
    result = get_old_tc_c_ispeed_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_old_tc_c_ispeed_wrapper(unsigned int value)
{
    void *result;
    result = set_old_tc_c_ispeed_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned int get_old_tc_c_ospeed_wrapper()
{
    old_tc_c_ospeed_ret_t *result;
    unsigned int ret_val = 0; // 默认值
    result = get_old_tc_c_ospeed_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_old_tc_c_ospeed_wrapper(unsigned int value)
{
    void *result;
    result = set_old_tc_c_ospeed_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_connected_wrapper()
{
    connected_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_connected_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_connected_wrapper(int value)
{
    void *result;
    result = set_connected_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

int get_localchars_wrapper()
{
    localchars_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_localchars_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_localchars_wrapper(int value)
{
    void *result;
    result = set_localchars_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

int get_kludgelinemode_wrapper()
{
    kludgelinemode_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_kludgelinemode_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_kludgelinemode_wrapper(int value)
{
    void *result;
    result = set_kludgelinemode_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

char* get_line_wrapper()
{
    line_array *result;
    static char local_copy[256];
    result = get_line_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        memset(local_copy, 0, 256);
        return local_copy;
    }
    strncpy(local_copy, result->line, 256);
    local_copy[256 - 1] = '\0'; // 确保字符串正确终止
    return local_copy;
}

int set_line_wrapper(const char *value)
{
    line_array param;
    void *result;
    if (value == NULL) {
        param.line[0] = '\0';
    } else {
        strncpy(param.line, value, 256);
        param.line[256 - 1] = '\0';
    }
    result = set_line_1(param, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        return 0;
    }
    return 1;
}

struct termios get_new_tc_wrapper()
{
    new_tc_ret_t *result_rpc;
    struct termios default_value = {0};
    result_rpc = get_new_tc_1(clnt);
    if (result_rpc == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    default_value.c_iflag=result_rpc->value.c_iflag;
    default_value.c_oflag=result_rpc->value.c_oflag;
    default_value.c_cflag=result_rpc->value.c_cflag;
    default_value.c_lflag=result_rpc->value.c_lflag;
    default_value.c_line=result_rpc->value.c_line;
    default_value.c_ispeed=result_rpc->value.c_ispeed;
    default_value.c_ospeed=result_rpc->value.c_ospeed;
    return default_value;
}

int set_new_tc_wrapper(struct termios new_tc)
{
    void *result;
    termios_rpc new_tc_rpc;
    new_tc_rpc.c_iflag=new_tc.c_iflag;
    new_tc_rpc.c_oflag=new_tc.c_oflag;
    new_tc_rpc.c_cflag=new_tc.c_cflag;
    new_tc_rpc.c_lflag=new_tc.c_lflag;
    new_tc_rpc.c_line=new_tc.c_line;
    new_tc_rpc.c_ispeed=new_tc.c_ispeed;
    new_tc_rpc.c_ospeed=new_tc.c_ospeed;
    result = set_new_tc_1(new_tc_rpc, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

unsigned int get_new_tc_c_iflag_wrapper()
{
    new_tc_c_iflag_ret_t *result;
    unsigned int ret_val = 0; // 默认值
    result = get_new_tc_c_iflag_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_new_tc_c_iflag_wrapper(unsigned int value)
{
    void *result;
    result = set_new_tc_c_iflag_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned int get_new_tc_c_oflag_wrapper()
{
    new_tc_c_oflag_ret_t *result;
    unsigned int ret_val = 0; // 默认值
    result = get_new_tc_c_oflag_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_new_tc_c_oflag_wrapper(unsigned int value)
{
    void *result;
    result = set_new_tc_c_oflag_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned int get_new_tc_c_cflag_wrapper()
{
    new_tc_c_cflag_ret_t *result;
    unsigned int ret_val = 0; // 默认值
    result = get_new_tc_c_cflag_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_new_tc_c_cflag_wrapper(unsigned int value)
{
    void *result;
    result = set_new_tc_c_cflag_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned int get_new_tc_c_lflag_wrapper()
{
    new_tc_c_lflag_ret_t *result;
    unsigned int ret_val = 0; // 默认值
    result = get_new_tc_c_lflag_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_new_tc_c_lflag_wrapper(unsigned int value)
{
    void *result;
    result = set_new_tc_c_lflag_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned char get_new_tc_c_line_wrapper()
{
    new_tc_c_line_ret_t *result;
    unsigned char ret_val = 0; // 默认值
    result = get_new_tc_c_line_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_new_tc_c_line_wrapper(unsigned char value)
{
    void *result;
    result = set_new_tc_c_line_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned int get_new_tc_c_ispeed_wrapper()
{
    new_tc_c_ispeed_ret_t *result;
    unsigned int ret_val = 0; // 默认值
    result = get_new_tc_c_ispeed_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_new_tc_c_ispeed_wrapper(unsigned int value)
{
    void *result;
    result = set_new_tc_c_ispeed_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

unsigned int get_new_tc_c_ospeed_wrapper()
{
    new_tc_c_ospeed_ret_t *result;
    unsigned int ret_val = 0; // 默认值
    result = get_new_tc_c_ospeed_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_new_tc_c_ospeed_wrapper(unsigned int value)
{
    void *result;
    result = set_new_tc_c_ospeed_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_autoflush_wrapper()
{
    autoflush_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_autoflush_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_autoflush_wrapper(int value)
{
    void *result;
    result = set_autoflush_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

unsigned char* get_slc_reply_wrapper()
{
    slc_reply_array *result;
    static unsigned char local_copy[128];
    result = get_slc_reply_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        memset(local_copy, 0, sizeof(local_copy));
        return local_copy;
    }
    memcpy(local_copy, result->slc_reply, sizeof(local_copy));
    return local_copy;
}

int set_slc_reply_wrapper(const unsigned char *value)
{
    slc_reply_array param;
    void *result;
    if (value == NULL) {
        memset(param.slc_reply, 0, sizeof(param.slc_reply));
    } else {
        memcpy(param.slc_reply, value, sizeof(param.slc_reply));
    }
    result = set_slc_reply_1(param, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        return 0;
    }
    return 1;
}

int get_flushout_wrapper()
{
    flushout_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_flushout_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_flushout_wrapper(int value)
{
    void *result;
    result = set_flushout_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

int get_flushline_wrapper()
{
    flushline_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_flushline_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_flushline_wrapper(int value)
{
    void *result;
    result = set_flushline_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

int get_bol_wrapper()
{
    bol_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_bol_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_bol_wrapper(int value)
{
    void *result;
    result = set_bol_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

fd_set get_xbits_wrapper()
{
    xbits_ret_t *result_rpc;
    fd_set default_value = {0};
    result_rpc = get_xbits_1(clnt);
    if (result_rpc == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return default_value;
}

int set_xbits_wrapper(fd_set xbits)
{
    void *result;
    fd_set_rpc xbits_rpc;
    result = set_xbits_1(xbits_rpc, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

int get_globalmode_wrapper()
{
    globalmode_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_globalmode_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_globalmode_wrapper(int value)
{
    void *result;
    result = set_globalmode_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

unsigned char* get_subbuffer_wrapper()
{
    subbuffer_array *result;
    static unsigned char local_copy[256];
    result = get_subbuffer_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        memset(local_copy, 0, sizeof(local_copy));
        return local_copy;
    }
    memcpy(local_copy, result->subbuffer, sizeof(local_copy));
    return local_copy;
}

int set_subbuffer_wrapper(const unsigned char *value)
{
    subbuffer_array param;
    void *result;
    if (value == NULL) {
        memset(param.subbuffer, 0, sizeof(param.subbuffer));
    } else {
        memcpy(param.subbuffer, value, sizeof(param.subbuffer));
    }
    result = set_subbuffer_1(param, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        return 0;
    }
    return 1;
}

int get_dontlecho_wrapper()
{
    dontlecho_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_dontlecho_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_dontlecho_wrapper(int value)
{
    void *result;
    result = set_dontlecho_1(value, clnt);
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
unsigned long gsetter_ring_clock_prefix_wrapper()
{
   unsigned long *result;
   result=gsetter_ring_clock_prefix_1(clnt);
   if (result == NULL) 
       clnt_perror(clnt, "call failed");
   return *result;
}
int gsetter_want_status_response_prefix_wrapper()
{
   int *result;
   result=gsetter_want_status_response_prefix_1(clnt);
   if (result == NULL) 
       clnt_perror(clnt, "call failed");
   return *result;
}
char* gsetter_subpointer_postfix_wrapper()
{
   char_ptr *result;
   result=gsetter_subpointer_postfix_1(clnt);
   if (result == NULL)
       clnt_perror(clnt, "call failed");
   else{
       if (result->charptr.charptr_val == NULL) {
           result->charptr.charptr_len = 0;
           result->charptr.charptr_val = NULL;
       }else {
           result->charptr.charptr_len = strlen(result->charptr.charptr_val);
           result->charptr.charptr_val = malloc(result->charptr.charptr_len + 1);
           memcpy(result->charptr.charptr_val, subpointer, result->charptr.charptr_len + 1);
       }
   }
   return result->charptr.charptr_val;
}
char* gsetter_slc_replyp_postfix_wrapper()
{
   char_ptr *result;
   result=gsetter_slc_replyp_postfix_1(clnt);
   if (result == NULL)
       clnt_perror(clnt, "call failed");
   else{
       if (result->charptr.charptr_val == NULL) {
           result->charptr.charptr_len = 0;
           result->charptr.charptr_val = NULL;
       }else {
           result->charptr.charptr_len = strlen(result->charptr.charptr_val);
           result->charptr.charptr_val = malloc(result->charptr.charptr_len + 1);
           memcpy(result->charptr.charptr_val, slc_replyp, result->charptr.charptr_len + 1);
       }
   }
   return result->charptr.charptr_val;
}
char* gsetter_opt_replyp_postfix_wrapper()
{
   char_ptr *result;
   result=gsetter_opt_replyp_postfix_1(clnt);
   if (result == NULL)
       clnt_perror(clnt, "call failed");
   else{
       if (result->charptr.charptr_val == NULL) {
           result->charptr.charptr_len = 0;
           result->charptr.charptr_val = NULL;
       }else {
           result->charptr.charptr_len = strlen(result->charptr.charptr_val);
           result->charptr.charptr_val = malloc(result->charptr.charptr_len + 1);
           memcpy(result->charptr.charptr_val, opt_replyp, result->charptr.charptr_len + 1);
       }
   }
   return result->charptr.charptr_val;
}
void gsetter_prompt_prefix_wrapper()
{
    gsetter_prompt_prefix_1(clnt);
int gsetter_margc_postfix_wrapper()
{
   int *result;
   result=gsetter_margc_postfix_1(clnt);
   if (result == NULL) 
       clnt_perror(clnt, "call failed");
   return *result;
}
