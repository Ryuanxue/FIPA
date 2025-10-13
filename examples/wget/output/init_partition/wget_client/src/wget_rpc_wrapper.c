#include "wget_rpc_wrapper.h"
extern CLIENT *clnt;
struct file_memory *wget_read_file_wrapper(const char *file)
{
    wget_read_file_ret_ *result_rpc;
    struct file_memory * sensefun_ret;
char_ptr file_rpc;
if(file==NULL) file_rpc.charptr.charptr_len=0;else file_rpc.charptr.charptr_len=strlen(file)+1;
file_rpc.charptr.charptr_val=malloc(file_rpc.charptr.charptr_len);
if(file_rpc.charptr.charptr_val==NULL) {
perror("malloc failed");
exit(1);
}
memcpy(file_rpc.charptr.charptr_val,file,file_rpc.charptr.charptr_len);
result_rpc=wget_read_file_rpc_1(file_rpc,clnt);
if(result_rpc==(wget_read_file_ret_ *)NULL) {
clnt_perror(clnt,"call failed");
}
if (result_rpc->ret0.file_memoryrpc_ptr_val == NULL) {sensefun_ret = NULL;}
else {
	sensefun_ret = (file_memory *)malloc(result_rpc->ret0.file_memoryrpc_ptr_len * sizeof(file_memory));
	if (sensefun_ret == NULL) {
		perror("malloc failed");
		exit(1);
	}
	for (int i = 0; i < result_rpc->ret0.file_memoryrpc_ptr_len; i++) {
if (result_rpc->ret0.file_memoryrpc_ptr_val[i]->content.charptr.charptr_val == NULL) {sensefun_ret[i]->content = NULL;}
else {sensefun_ret[i]->content = strdup(result_rpc->ret0.file_memoryrpc_ptr_val[i]->content.charptr.charptr_val);}
sensefun_ret[i]->length=result_rpc->ret0.file_memoryrpc_ptr_val[i]->length;
sensefun_ret[i]->mmap_p=result_rpc->ret0.file_memoryrpc_ptr_val[i]->mmap_p;
if(result_rpc->file.charptr.charptr_val==NULL) file=NULL;else{

file=malloc(result_rpc->file.charptr.charptr_len);
memcpy(file,result_rpc->file.charptr.charptr_val,result_rpc->file.charptr.charptr_len);
}
return sensefun_ret;
}
static int sock_read_wrapper(int fd, char *buf, int bufsize)
{
    sock_read_ret_ *result_rpc;
    int sensefun_ret;
int fd_rpc;
fd_rpc=fd;
char_ptr buf_rpc;
if(buf==NULL) buf_rpc.charptr.charptr_len=0;else buf_rpc.charptr.charptr_len=strlen(buf)+1;
buf_rpc.charptr.charptr_val=malloc(buf_rpc.charptr.charptr_len);
if(buf_rpc.charptr.charptr_val==NULL) {
perror("malloc failed");
exit(1);
}
memcpy(buf_rpc.charptr.charptr_val,buf,buf_rpc.charptr.charptr_len);
int bufsize_rpc;
bufsize_rpc=bufsize;
result_rpc=sock_read_rpc_1(fd_rpc,buf_rpc,bufsize_rpc,clnt);
if(result_rpc==(sock_read_ret_ *)NULL) {
clnt_perror(clnt,"call failed");
}
sensefun_ret=result_rpc->ret0;
if(result_rpc->buf.charptr.charptr_val==NULL) buf=NULL;else{

buf=malloc(result_rpc->buf.charptr.charptr_len);
memcpy(buf,result_rpc->buf.charptr.charptr_val,result_rpc->buf.charptr.charptr_len);
}
return sensefun_ret;
}
void main_sense_1_wrapper(char **url, char **t, int nurl, struct ptimer *timer, double start_time)
{
    main_sense_1_ret_ *result_rpc;
TwoCharPtr url_rpc;
if(url==NULL) {
url_rpc.twocharptr.twocharptr_len=0;
url_rpc.twocharptr.twocharptr_val=NULL;
} else {
url_rpc.twocharptr.twocharptr_len=1;
url_rpc.twocharptr.twocharptr_val=malloc(1*sizeof(char_ptr));
if(url_rpc.twocharptr.twocharptr_val==NULL) {
perror("malloc failed");
exit(1);
}
for(int i=0;i<url_rpc.twocharptr.twocharptr_len;i++) {
if(url[i]==NULL) {
url_rpc.twocharptr.twocharptr_val[i].charptr.charptr_len=0;
url_rpc.twocharptr.twocharptr_val[i].charptr.charptr_val=NULL;
} else {
url_rpc.twocharptr.twocharptr_val[i].charptr.charptr_len=strlen(url[i])+1;
url_rpc.twocharptr.twocharptr_val[i].charptr.charptr_val=malloc(url_rpc.twocharptr.twocharptr_val[i].charptr.charptr_len*sizeof(char));
if(url_rpc.twocharptr.twocharptr_val[i].charptr.charptr_val==NULL) {
perror("malloc failed");
exit(1);
}
memcpy(url_rpc.twocharptr.twocharptr_val[i].charptr.charptr_val,url[i],url_rpc.twocharptr.twocharptr_val[i].charptr.charptr_len);
			}
		}
	}
TwoCharPtr t_rpc;
if(t==NULL) {
t_rpc.twocharptr.twocharptr_len=0;
t_rpc.twocharptr.twocharptr_val=NULL;
} else {
t_rpc.twocharptr.twocharptr_len=1;
t_rpc.twocharptr.twocharptr_val=malloc(1*sizeof(char_ptr));
if(t_rpc.twocharptr.twocharptr_val==NULL) {
perror("malloc failed");
exit(1);
}
for(int i=0;i<t_rpc.twocharptr.twocharptr_len;i++) {
if(t[i]==NULL) {
t_rpc.twocharptr.twocharptr_val[i].charptr.charptr_len=0;
t_rpc.twocharptr.twocharptr_val[i].charptr.charptr_val=NULL;
} else {
t_rpc.twocharptr.twocharptr_val[i].charptr.charptr_len=strlen(t[i])+1;
t_rpc.twocharptr.twocharptr_val[i].charptr.charptr_val=malloc(t_rpc.twocharptr.twocharptr_val[i].charptr.charptr_len*sizeof(char));
if(t_rpc.twocharptr.twocharptr_val[i].charptr.charptr_val==NULL) {
perror("malloc failed");
exit(1);
}
memcpy(t_rpc.twocharptr.twocharptr_val[i].charptr.charptr_val,t[i],t_rpc.twocharptr.twocharptr_val[i].charptr.charptr_len);
			}
		}
	}
int nurl_rpc;
nurl_rpc=nurl;
ptimer_rpc_ptr timer_rpc_ptr;
if(timer==NULL) {
timer_rpc_ptr.ptimer_rpc_ptr_len=0;
timer_rpc_ptr.ptimer_rpc_ptr_val=NULL;
} else {
timer_rpc_ptr.ptimer_rpc_ptr_len=1;
timer_rpc_ptr.ptimer_rpc_ptr_val=malloc(1*sizeof(ptimer_rpc));
if(timer_rpc_ptr.ptimer_rpc_ptr_val==NULL) {
perror("malloc failed");
exit(1);
}
for(int i0=0;i0<timer_rpc_ptr.ptimer_rpc_ptr_len;i0++) {
timer_rpc_ptr.ptimer_rpc_ptr_val[i0].elapsed_last=timer[i0].elapsed_last;
timer_rpc_ptr.ptimer_rpc_ptr_val[i0].elapsed_pre_start=timer[i0].elapsed_pre_start;
}
}
double start_time_rpc;
start_time_rpc=start_time;
result_rpc=main_sense_1_rpc_1(url_rpc,t_rpc,nurl_rpc,timer_rpc_ptr,start_time_rpc,clnt);
if(result_rpc==(main_sense_1_ret_ *)NULL) {
clnt_perror(clnt,"call failed");
}
    if(result_rpc->url.twocharptr.twocharptr_val==NULL) {
        url=NULL;
    } else {
        url=(char **)malloc(result_rpc->url.twocharptr.twocharptr_len*sizeof(char *));
        if(url==NULL) {
            perror("malloc failed");
            exit(1);
        }
        for(int i=0;i<result_rpc->url.twocharptr.twocharptr_len;i++) {
            if(result_rpc->url.twocharptr.twocharptr_val[i].charptr.charptr_val==NULL) {
                url[i]=NULL;
            } else {
                url[i]=malloc(result_rpc->url.twocharptr.twocharptr_val[i].charptr.charptr_len*sizeof(char));
            if(url[i]==NULL) {
                perror("malloc failed");
                exit(1);
            }
                memcpy(url[i],result_rpc->url.twocharptr.twocharptr_val[i].charptr.charptr_val,result_rpc->url.twocharptr.twocharptr_val[i].charptr.charptr_len);
        }
    }
    if(result_rpc->t.twocharptr.twocharptr_val==NULL) {
        t=NULL;
    } else {
        t=(char **)malloc(result_rpc->t.twocharptr.twocharptr_len*sizeof(char *));
        if(t==NULL) {
            perror("malloc failed");
            exit(1);
        }
        for(int i=0;i<result_rpc->t.twocharptr.twocharptr_len;i++) {
            if(result_rpc->t.twocharptr.twocharptr_val[i].charptr.charptr_val==NULL) {
                t[i]=NULL;
            } else {
                t[i]=malloc(result_rpc->t.twocharptr.twocharptr_val[i].charptr.charptr_len*sizeof(char));
            if(t[i]==NULL) {
                perror("malloc failed");
                exit(1);
            }
                memcpy(t[i],result_rpc->t.twocharptr.twocharptr_val[i].charptr.charptr_val,result_rpc->t.twocharptr.twocharptr_val[i].charptr.charptr_len);
        }
    }
    if(result_rpc->timer.ptimer_rpc_ptr_val==NULL) {
        timer=NULL;
    } else {
        timer=malloc(result_rpc->timer.ptimer_rpc_ptr_len*sizeof(struct ptimer));
        if(timer==NULL) {
            perror("malloc failed");
            exit(1);
        }
        for(int i0=0;i0<result_rpc->timer.ptimer_rpc_ptr_len;i0++) {
timer[i0].elapsed_last=result_rpc->timer.ptimer_rpc_ptr_val[i0].elapsed_last;
timer[i0].elapsed_pre_start=result_rpc->timer.ptimer_rpc_ptr_val[i0].elapsed_pre_start;
        }
    }
}

/* 全局变量访问函数 - 客户端Wrapper */
struct iri get_dummy_iri_wrapper()
{
    dummy_iri_ret_t *result_rpc;
    struct iri default_value = {0};
    result_rpc = get_dummy_iri_1(clnt);
    if (result_rpc == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    if (result_rpc->value.uri_encoding.charptr.charptr_val == NULL) {default_value.uri_encoding=NULL;}

    else {default_value.uri_encoding=malloc(result_rpc->value.uri_encoding.charptr.charptr_len);

    memcpy(default_value.uri_encoding,result_rpc->value.uri_encoding.charptr.charptr_val,result_rpc->value.uri_encoding.charptr.charptr_len);

    }

    if (result_rpc->value.content_encoding.charptr.charptr_val == NULL) {default_value.content_encoding=NULL;}

    else {default_value.content_encoding=malloc(result_rpc->value.content_encoding.charptr.charptr_len);

    memcpy(default_value.content_encoding,result_rpc->value.content_encoding.charptr.charptr_val,result_rpc->value.content_encoding.charptr.charptr_len);

    }

    if (result_rpc->value.orig_url.charptr.charptr_val == NULL) {default_value.orig_url=NULL;}

    else {default_value.orig_url=malloc(result_rpc->value.orig_url.charptr.charptr_len);

    memcpy(default_value.orig_url,result_rpc->value.orig_url.charptr.charptr_val,result_rpc->value.orig_url.charptr.charptr_len);

    }

    default_value.utf8_encode=result_rpc->value.utf8_encode;
    return default_value;
}

int set_dummy_iri_wrapper(struct iri dummy_iri)
{
    void *result;
    iri_rpc dummy_iri_rpc;
    if (dummy_iri.uri_encoding == NULL) {dummy_iri_rpc.uri_encoding.charptr.charptr_len = 0;dummy_iri_rpc.uri_encoding.charptr.charptr_val = NULL;}
    else {dummy_iri_rpc.uri_encoding.charptr.charptr_len = strlen(dummy_iri.uri_encoding) + 1;

    	dummy_iri_rpc.uri_encoding.charptr.charptr_val = malloc(dummy_iri_rpc.uri_encoding.charptr.charptr_len * sizeof(char));
    	if (dummy_iri_rpc.uri_encoding.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(dummy_iri_rpc.uri_encoding.charptr.charptr_val, dummy_iri.uri_encoding, dummy_iri_rpc.uri_encoding.charptr.charptr_len);
    }
    if (dummy_iri.content_encoding == NULL) {dummy_iri_rpc.content_encoding.charptr.charptr_len = 0;dummy_iri_rpc.content_encoding.charptr.charptr_val = NULL;}
    else {dummy_iri_rpc.content_encoding.charptr.charptr_len = strlen(dummy_iri.content_encoding) + 1;

    	dummy_iri_rpc.content_encoding.charptr.charptr_val = malloc(dummy_iri_rpc.content_encoding.charptr.charptr_len * sizeof(char));
    	if (dummy_iri_rpc.content_encoding.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(dummy_iri_rpc.content_encoding.charptr.charptr_val, dummy_iri.content_encoding, dummy_iri_rpc.content_encoding.charptr.charptr_len);
    }
    if (dummy_iri.orig_url == NULL) {dummy_iri_rpc.orig_url.charptr.charptr_len = 0;dummy_iri_rpc.orig_url.charptr.charptr_val = NULL;}
    else {dummy_iri_rpc.orig_url.charptr.charptr_len = strlen(dummy_iri.orig_url) + 1;

    	dummy_iri_rpc.orig_url.charptr.charptr_val = malloc(dummy_iri_rpc.orig_url.charptr.charptr_len * sizeof(char));
    	if (dummy_iri_rpc.orig_url.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(dummy_iri_rpc.orig_url.charptr.charptr_val, dummy_iri.orig_url, dummy_iri_rpc.orig_url.charptr.charptr_len);
    }
    dummy_iri_rpc.utf8_encode=dummy_iri.utf8_encode;
    result = set_dummy_iri_1(dummy_iri_rpc, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

char * get_dummy_iri_uri_encoding_wrapper()
{
    dummy_iri_uri_encoding_ret_t *result;
    char * ret_val = NULL;
    result = get_dummy_iri_uri_encoding_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_dummy_iri_uri_encoding_wrapper(const char *value)
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
    result = set_dummy_iri_uri_encoding_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_dummy_iri_content_encoding_wrapper()
{
    dummy_iri_content_encoding_ret_t *result;
    char * ret_val = NULL;
    result = get_dummy_iri_content_encoding_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_dummy_iri_content_encoding_wrapper(const char *value)
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
    result = set_dummy_iri_content_encoding_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_dummy_iri_orig_url_wrapper()
{
    dummy_iri_orig_url_ret_t *result;
    char * ret_val = NULL;
    result = get_dummy_iri_orig_url_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_dummy_iri_orig_url_wrapper(const char *value)
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
    result = set_dummy_iri_orig_url_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_dummy_iri_utf8_encode_wrapper()
{
    dummy_iri_utf8_encode_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_dummy_iri_utf8_encode_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_dummy_iri_utf8_encode_wrapper(bool value)
{
    void *result;
    result = set_dummy_iri_utf8_encode_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

struct cookie_jar* get_wget_cookie_jar_wrapper()
{
    wget_cookie_jar_ret_t *result_rpc;
    struct cookie_jar* value = {0};
    result_rpc = get_wget_cookie_jar_1(clnt);
        if(result_rpc->value.cookie_jar_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.cookie_jar_rpc_ptr_len*sizeof(struct cookie_jar));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.cookie_jar_rpc_ptr_len;i0++) {
    if (result_rpc->value.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val == NULL) {value[i0].chains = NULL;}
    else {
    	value[i0].chains = (struct hash_table *)malloc(result_rpc->value.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_len * sizeof( struct hash_table));
    	if (value[i0].chains == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < result_rpc->value.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_len; i1++) {
    if (result_rpc->value.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val == NULL) {value[i0].chains[i1].cells = NULL;}
    else {
    	value[i0].chains[i1].cells = (struct cell *)malloc(result_rpc->value.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_len * sizeof( struct cell));
    	if (value[i0].chains[i1].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < result_rpc->value.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_len; i1++) {
    if (result_rpc->value.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {value[i0].chains[i1].cells[i1].key = NULL;}
    else {value[i0].chains[i1].cells[i1].key = (char *)malloc(result_rpc->value.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].key.key_len * sizeof(char));
    	if (value[i0].chains[i1].cells[i1].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].chains[i1].cells[i1].key, result_rpc->value.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].key.key_val, result_rpc->value.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].key.key_len);
    }
    if (result_rpc->value.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {value[i0].chains[i1].cells[i1].value = NULL;}
    else {value[i0].chains[i1].cells[i1].value = (char *)malloc(result_rpc->value.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].value.value_len * sizeof(char));
    	if (value[i0].chains[i1].cells[i1].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].chains[i1].cells[i1].value, result_rpc->value.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].value.value_val, result_rpc->value.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].value.value_len);
    }
    	}
    }
    value[i0].chains[i1].size=result_rpc->value.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].size;
    value[i0].chains[i1].count=result_rpc->value.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].count;
    value[i0].chains[i1].resize_threshold=result_rpc->value.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].resize_threshold;
    value[i0].chains[i1].prime_offset=result_rpc->value.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].prime_offset;
    	}
    }
    value[i0].cookie_count=result_rpc->value.cookie_jar_rpc_ptr_val[i0].cookie_count;
            }
        }
    return value;
}

int set_wget_cookie_jar_wrapper(struct cookie_jar* value,int len)
{
    void *result;
    cookie_jar_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.cookie_jar_rpc_ptr_len=0;
    value_rpc_ptr.cookie_jar_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.cookie_jar_rpc_ptr_len=1;
    value_rpc_ptr.cookie_jar_rpc_ptr_val=malloc(1*sizeof(cookie_jar_rpc));
    if(value_rpc_ptr.cookie_jar_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.cookie_jar_rpc_ptr_len;i0++) {
    if (value[i0].chains == NULL) {value_rpc_ptr.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_len = 0;value_rpc_ptr.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val = NULL;}
    else {value_rpc_ptr.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_len = 1;value_rpc_ptr.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val = malloc(value_rpc_ptr.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_len * sizeof(hash_table_rpc));
    	if (value_rpc_ptr.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < value_rpc_ptr.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_len; i1++) {
    if (value[i0].chains[i1].cells == NULL) {value_rpc_ptr.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_len = 0;value_rpc_ptr.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val = NULL;}
    else {value_rpc_ptr.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_len = 1;value_rpc_ptr.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val = malloc(value_rpc_ptr.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_len * sizeof(cell_rpc));
    	if (value_rpc_ptr.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i2 = 0; i2 < value_rpc_ptr.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_len; i2++) {
    	}
    }
    value_rpc_ptr.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].size=value[i0].chains[i1].size;
    value_rpc_ptr.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].count=value[i0].chains[i1].count;
    value_rpc_ptr.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].resize_threshold=value[i0].chains[i1].resize_threshold;
    value_rpc_ptr.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].prime_offset=value[i0].chains[i1].prime_offset;
    	}
    }
    value_rpc_ptr.cookie_jar_rpc_ptr_val[i0].cookie_count=value[i0].cookie_count;
    }
    }
    result = set_wget_cookie_jar_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

struct hash_table* get_wget_cookie_jar_chains_wrapper()
{
    wget_cookie_jar_chains_ret_t *result_rpc;
    struct hash_table* value = {0};
    result_rpc = get_wget_cookie_jar_chains_1(clnt);
        if(result_rpc->value.hash_table_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.hash_table_rpc_ptr_len*sizeof(struct hash_table));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.hash_table_rpc_ptr_len;i0++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {value[i0].cells = NULL;}
    else {
    	value[i0].cells = (struct cell *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof( struct cell));
    	if (value[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {value[i0].cells[i1].key = NULL;}
    else {value[i0].cells[i1].key = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len * sizeof(char));
    	if (value[i0].cells[i1].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].key, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);
    }
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {value[i0].cells[i1].value = NULL;}
    else {value[i0].cells[i1].value = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len * sizeof(char));
    	if (value[i0].cells[i1].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].value, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);
    }
    	}
    }
    value[i0].size=result_rpc->value.hash_table_rpc_ptr_val[i0].size;
    value[i0].count=result_rpc->value.hash_table_rpc_ptr_val[i0].count;
    value[i0].resize_threshold=result_rpc->value.hash_table_rpc_ptr_val[i0].resize_threshold;
    value[i0].prime_offset=result_rpc->value.hash_table_rpc_ptr_val[i0].prime_offset;
            }
        }
    return value;
}

int set_wget_cookie_jar_chains_wrapper(struct hash_table* value,int len)
{
    void *result;
    hash_table_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.hash_table_rpc_ptr_len=0;
    value_rpc_ptr.hash_table_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.hash_table_rpc_ptr_len=1;
    value_rpc_ptr.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
    if(value_rpc_ptr.hash_table_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.hash_table_rpc_ptr_len;i0++) {
    if (value[i0].cells == NULL) {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}
    else {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));
    	if (value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    	}
    }
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].size=value[i0].size;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].count=value[i0].count;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].resize_threshold=value[i0].resize_threshold;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].prime_offset=value[i0].prime_offset;
    }
    }
    result = set_wget_cookie_jar_chains_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void * get_wget_cookie_jar_chains_hash_function_wrapper()
{
    char_ptr *result;
    result = get_wget_cookie_jar_chains_hash_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_wget_cookie_jar_chains_hash_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_wget_cookie_jar_chains_hash_function_1(value, clnt);
}

void * get_wget_cookie_jar_chains_test_function_wrapper()
{
    char_ptr *result;
    result = get_wget_cookie_jar_chains_test_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_wget_cookie_jar_chains_test_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_wget_cookie_jar_chains_test_function_1(value, clnt);
}

struct cell* get_wget_cookie_jar_chains_cells_wrapper()
{
    wget_cookie_jar_chains_cells_ret_t *result_rpc;
    struct cell* value = {0};
    result_rpc = get_wget_cookie_jar_chains_cells_1(clnt);
        if(result_rpc->value.cell_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.cell_rpc_ptr_len*sizeof(struct cell));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.cell_rpc_ptr_len;i0++) {
    if (result_rpc->value.cell_rpc_ptr_val[i0].key.key_val == NULL) {value[i0].key = NULL;}
    else {value[i0].key = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].key.key_len * sizeof(char));
    	if (value[i0].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].key, result_rpc->value.cell_rpc_ptr_val[i0].key.key_val, result_rpc->value.cell_rpc_ptr_val[i0].key.key_len);
    }
    if (result_rpc->value.cell_rpc_ptr_val[i0].value.value_val == NULL) {value[i0].value = NULL;}
    else {value[i0].value = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].value.value_len * sizeof(char));
    	if (value[i0].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].value, result_rpc->value.cell_rpc_ptr_val[i0].value.value_val, result_rpc->value.cell_rpc_ptr_val[i0].value.value_len);
    }
            }
        }
    return value;
}

int set_wget_cookie_jar_chains_cells_wrapper(struct cell* value,int len)
{
    void *result;
    cell_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.cell_rpc_ptr_len=0;
    value_rpc_ptr.cell_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.cell_rpc_ptr_len=1;
    value_rpc_ptr.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
    if(value_rpc_ptr.cell_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.cell_rpc_ptr_len;i0++) {
    }
    }
    result = set_wget_cookie_jar_chains_cells_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void* get_wget_cookie_jar_chains_cells_key_wrapper(size_t *out_size)
{
    wget_cookie_jar_chains_cells_key_ret_t *result;
    void *ret_val = NULL;
    result = get_wget_cookie_jar_chains_cells_key_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_wget_cookie_jar_chains_cells_key_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_wget_cookie_jar_chains_cells_key_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

void* get_wget_cookie_jar_chains_cells_value_wrapper(size_t *out_size)
{
    wget_cookie_jar_chains_cells_value_ret_t *result;
    void *ret_val = NULL;
    result = get_wget_cookie_jar_chains_cells_value_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_wget_cookie_jar_chains_cells_value_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_wget_cookie_jar_chains_cells_value_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_wget_cookie_jar_chains_size_wrapper()
{
    wget_cookie_jar_chains_size_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_wget_cookie_jar_chains_size_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_wget_cookie_jar_chains_size_wrapper(int value)
{
    void *result;
    result = set_wget_cookie_jar_chains_size_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_wget_cookie_jar_chains_count_wrapper()
{
    wget_cookie_jar_chains_count_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_wget_cookie_jar_chains_count_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_wget_cookie_jar_chains_count_wrapper(int value)
{
    void *result;
    result = set_wget_cookie_jar_chains_count_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_wget_cookie_jar_chains_resize_threshold_wrapper()
{
    wget_cookie_jar_chains_resize_threshold_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_wget_cookie_jar_chains_resize_threshold_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_wget_cookie_jar_chains_resize_threshold_wrapper(int value)
{
    void *result;
    result = set_wget_cookie_jar_chains_resize_threshold_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_wget_cookie_jar_chains_prime_offset_wrapper()
{
    wget_cookie_jar_chains_prime_offset_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_wget_cookie_jar_chains_prime_offset_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_wget_cookie_jar_chains_prime_offset_wrapper(int value)
{
    void *result;
    result = set_wget_cookie_jar_chains_prime_offset_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_wget_cookie_jar_cookie_count_wrapper()
{
    wget_cookie_jar_cookie_count_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_wget_cookie_jar_cookie_count_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_wget_cookie_jar_cookie_count_wrapper(int value)
{
    void *result;
    result = set_wget_cookie_jar_cookie_count_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

struct hash_table* get_dl_file_url_map_wrapper()
{
    dl_file_url_map_ret_t *result_rpc;
    struct hash_table* value = {0};
    result_rpc = get_dl_file_url_map_1(clnt);
        if(result_rpc->value.hash_table_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.hash_table_rpc_ptr_len*sizeof(struct hash_table));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.hash_table_rpc_ptr_len;i0++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {value[i0].cells = NULL;}
    else {
    	value[i0].cells = (struct cell *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof( struct cell));
    	if (value[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {value[i0].cells[i1].key = NULL;}
    else {value[i0].cells[i1].key = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len * sizeof(char));
    	if (value[i0].cells[i1].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].key, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);
    }
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {value[i0].cells[i1].value = NULL;}
    else {value[i0].cells[i1].value = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len * sizeof(char));
    	if (value[i0].cells[i1].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].value, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);
    }
    	}
    }
    value[i0].size=result_rpc->value.hash_table_rpc_ptr_val[i0].size;
    value[i0].count=result_rpc->value.hash_table_rpc_ptr_val[i0].count;
    value[i0].resize_threshold=result_rpc->value.hash_table_rpc_ptr_val[i0].resize_threshold;
    value[i0].prime_offset=result_rpc->value.hash_table_rpc_ptr_val[i0].prime_offset;
            }
        }
    return value;
}

int set_dl_file_url_map_wrapper(struct hash_table* value,int len)
{
    void *result;
    hash_table_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.hash_table_rpc_ptr_len=0;
    value_rpc_ptr.hash_table_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.hash_table_rpc_ptr_len=1;
    value_rpc_ptr.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
    if(value_rpc_ptr.hash_table_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.hash_table_rpc_ptr_len;i0++) {
    if (value[i0].cells == NULL) {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}
    else {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));
    	if (value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    	}
    }
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].size=value[i0].size;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].count=value[i0].count;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].resize_threshold=value[i0].resize_threshold;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].prime_offset=value[i0].prime_offset;
    }
    }
    result = set_dl_file_url_map_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void * get_dl_file_url_map_hash_function_wrapper()
{
    char_ptr *result;
    result = get_dl_file_url_map_hash_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_dl_file_url_map_hash_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_dl_file_url_map_hash_function_1(value, clnt);
}

void * get_dl_file_url_map_test_function_wrapper()
{
    char_ptr *result;
    result = get_dl_file_url_map_test_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_dl_file_url_map_test_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_dl_file_url_map_test_function_1(value, clnt);
}

struct cell* get_dl_file_url_map_cells_wrapper()
{
    dl_file_url_map_cells_ret_t *result_rpc;
    struct cell* value = {0};
    result_rpc = get_dl_file_url_map_cells_1(clnt);
        if(result_rpc->value.cell_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.cell_rpc_ptr_len*sizeof(struct cell));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.cell_rpc_ptr_len;i0++) {
    if (result_rpc->value.cell_rpc_ptr_val[i0].key.key_val == NULL) {value[i0].key = NULL;}
    else {value[i0].key = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].key.key_len * sizeof(char));
    	if (value[i0].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].key, result_rpc->value.cell_rpc_ptr_val[i0].key.key_val, result_rpc->value.cell_rpc_ptr_val[i0].key.key_len);
    }
    if (result_rpc->value.cell_rpc_ptr_val[i0].value.value_val == NULL) {value[i0].value = NULL;}
    else {value[i0].value = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].value.value_len * sizeof(char));
    	if (value[i0].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].value, result_rpc->value.cell_rpc_ptr_val[i0].value.value_val, result_rpc->value.cell_rpc_ptr_val[i0].value.value_len);
    }
            }
        }
    return value;
}

int set_dl_file_url_map_cells_wrapper(struct cell* value,int len)
{
    void *result;
    cell_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.cell_rpc_ptr_len=0;
    value_rpc_ptr.cell_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.cell_rpc_ptr_len=1;
    value_rpc_ptr.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
    if(value_rpc_ptr.cell_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.cell_rpc_ptr_len;i0++) {
    }
    }
    result = set_dl_file_url_map_cells_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void* get_dl_file_url_map_cells_key_wrapper(size_t *out_size)
{
    dl_file_url_map_cells_key_ret_t *result;
    void *ret_val = NULL;
    result = get_dl_file_url_map_cells_key_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_dl_file_url_map_cells_key_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_dl_file_url_map_cells_key_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

void* get_dl_file_url_map_cells_value_wrapper(size_t *out_size)
{
    dl_file_url_map_cells_value_ret_t *result;
    void *ret_val = NULL;
    result = get_dl_file_url_map_cells_value_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_dl_file_url_map_cells_value_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_dl_file_url_map_cells_value_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_dl_file_url_map_size_wrapper()
{
    dl_file_url_map_size_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_dl_file_url_map_size_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_dl_file_url_map_size_wrapper(int value)
{
    void *result;
    result = set_dl_file_url_map_size_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_dl_file_url_map_count_wrapper()
{
    dl_file_url_map_count_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_dl_file_url_map_count_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_dl_file_url_map_count_wrapper(int value)
{
    void *result;
    result = set_dl_file_url_map_count_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_dl_file_url_map_resize_threshold_wrapper()
{
    dl_file_url_map_resize_threshold_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_dl_file_url_map_resize_threshold_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_dl_file_url_map_resize_threshold_wrapper(int value)
{
    void *result;
    result = set_dl_file_url_map_resize_threshold_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_dl_file_url_map_prime_offset_wrapper()
{
    dl_file_url_map_prime_offset_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_dl_file_url_map_prime_offset_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_dl_file_url_map_prime_offset_wrapper(int value)
{
    void *result;
    result = set_dl_file_url_map_prime_offset_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

struct hash_table* get_downloaded_css_set_wrapper()
{
    downloaded_css_set_ret_t *result_rpc;
    struct hash_table* value = {0};
    result_rpc = get_downloaded_css_set_1(clnt);
        if(result_rpc->value.hash_table_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.hash_table_rpc_ptr_len*sizeof(struct hash_table));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.hash_table_rpc_ptr_len;i0++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {value[i0].cells = NULL;}
    else {
    	value[i0].cells = (struct cell *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof( struct cell));
    	if (value[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {value[i0].cells[i1].key = NULL;}
    else {value[i0].cells[i1].key = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len * sizeof(char));
    	if (value[i0].cells[i1].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].key, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);
    }
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {value[i0].cells[i1].value = NULL;}
    else {value[i0].cells[i1].value = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len * sizeof(char));
    	if (value[i0].cells[i1].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].value, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);
    }
    	}
    }
    value[i0].size=result_rpc->value.hash_table_rpc_ptr_val[i0].size;
    value[i0].count=result_rpc->value.hash_table_rpc_ptr_val[i0].count;
    value[i0].resize_threshold=result_rpc->value.hash_table_rpc_ptr_val[i0].resize_threshold;
    value[i0].prime_offset=result_rpc->value.hash_table_rpc_ptr_val[i0].prime_offset;
            }
        }
    return value;
}

int set_downloaded_css_set_wrapper(struct hash_table* value,int len)
{
    void *result;
    hash_table_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.hash_table_rpc_ptr_len=0;
    value_rpc_ptr.hash_table_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.hash_table_rpc_ptr_len=1;
    value_rpc_ptr.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
    if(value_rpc_ptr.hash_table_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.hash_table_rpc_ptr_len;i0++) {
    if (value[i0].cells == NULL) {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}
    else {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));
    	if (value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    	}
    }
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].size=value[i0].size;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].count=value[i0].count;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].resize_threshold=value[i0].resize_threshold;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].prime_offset=value[i0].prime_offset;
    }
    }
    result = set_downloaded_css_set_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void * get_downloaded_css_set_hash_function_wrapper()
{
    char_ptr *result;
    result = get_downloaded_css_set_hash_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_downloaded_css_set_hash_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_downloaded_css_set_hash_function_1(value, clnt);
}

void * get_downloaded_css_set_test_function_wrapper()
{
    char_ptr *result;
    result = get_downloaded_css_set_test_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_downloaded_css_set_test_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_downloaded_css_set_test_function_1(value, clnt);
}

struct cell* get_downloaded_css_set_cells_wrapper()
{
    downloaded_css_set_cells_ret_t *result_rpc;
    struct cell* value = {0};
    result_rpc = get_downloaded_css_set_cells_1(clnt);
        if(result_rpc->value.cell_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.cell_rpc_ptr_len*sizeof(struct cell));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.cell_rpc_ptr_len;i0++) {
    if (result_rpc->value.cell_rpc_ptr_val[i0].key.key_val == NULL) {value[i0].key = NULL;}
    else {value[i0].key = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].key.key_len * sizeof(char));
    	if (value[i0].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].key, result_rpc->value.cell_rpc_ptr_val[i0].key.key_val, result_rpc->value.cell_rpc_ptr_val[i0].key.key_len);
    }
    if (result_rpc->value.cell_rpc_ptr_val[i0].value.value_val == NULL) {value[i0].value = NULL;}
    else {value[i0].value = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].value.value_len * sizeof(char));
    	if (value[i0].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].value, result_rpc->value.cell_rpc_ptr_val[i0].value.value_val, result_rpc->value.cell_rpc_ptr_val[i0].value.value_len);
    }
            }
        }
    return value;
}

int set_downloaded_css_set_cells_wrapper(struct cell* value,int len)
{
    void *result;
    cell_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.cell_rpc_ptr_len=0;
    value_rpc_ptr.cell_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.cell_rpc_ptr_len=1;
    value_rpc_ptr.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
    if(value_rpc_ptr.cell_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.cell_rpc_ptr_len;i0++) {
    }
    }
    result = set_downloaded_css_set_cells_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void* get_downloaded_css_set_cells_key_wrapper(size_t *out_size)
{
    downloaded_css_set_cells_key_ret_t *result;
    void *ret_val = NULL;
    result = get_downloaded_css_set_cells_key_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_downloaded_css_set_cells_key_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_downloaded_css_set_cells_key_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

void* get_downloaded_css_set_cells_value_wrapper(size_t *out_size)
{
    downloaded_css_set_cells_value_ret_t *result;
    void *ret_val = NULL;
    result = get_downloaded_css_set_cells_value_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_downloaded_css_set_cells_value_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_downloaded_css_set_cells_value_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_downloaded_css_set_size_wrapper()
{
    downloaded_css_set_size_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_downloaded_css_set_size_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_downloaded_css_set_size_wrapper(int value)
{
    void *result;
    result = set_downloaded_css_set_size_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_downloaded_css_set_count_wrapper()
{
    downloaded_css_set_count_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_downloaded_css_set_count_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_downloaded_css_set_count_wrapper(int value)
{
    void *result;
    result = set_downloaded_css_set_count_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_downloaded_css_set_resize_threshold_wrapper()
{
    downloaded_css_set_resize_threshold_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_downloaded_css_set_resize_threshold_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_downloaded_css_set_resize_threshold_wrapper(int value)
{
    void *result;
    result = set_downloaded_css_set_resize_threshold_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_downloaded_css_set_prime_offset_wrapper()
{
    downloaded_css_set_prime_offset_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_downloaded_css_set_prime_offset_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_downloaded_css_set_prime_offset_wrapper(int value)
{
    void *result;
    result = set_downloaded_css_set_prime_offset_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char* get_warc_current_filename_wrapper()
{
    char_ptr *result;
    char *ret_val = NULL;
    result = get_warc_current_filename_1(clnt);
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

void set_warc_current_filename_wrapper(const char *value)
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
    result = set_warc_current_filename_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
    }
}

struct hash_table* get_dl_url_file_map_wrapper()
{
    dl_url_file_map_ret_t *result_rpc;
    struct hash_table* value = {0};
    result_rpc = get_dl_url_file_map_1(clnt);
        if(result_rpc->value.hash_table_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.hash_table_rpc_ptr_len*sizeof(struct hash_table));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.hash_table_rpc_ptr_len;i0++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {value[i0].cells = NULL;}
    else {
    	value[i0].cells = (struct cell *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof( struct cell));
    	if (value[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {value[i0].cells[i1].key = NULL;}
    else {value[i0].cells[i1].key = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len * sizeof(char));
    	if (value[i0].cells[i1].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].key, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);
    }
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {value[i0].cells[i1].value = NULL;}
    else {value[i0].cells[i1].value = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len * sizeof(char));
    	if (value[i0].cells[i1].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].value, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);
    }
    	}
    }
    value[i0].size=result_rpc->value.hash_table_rpc_ptr_val[i0].size;
    value[i0].count=result_rpc->value.hash_table_rpc_ptr_val[i0].count;
    value[i0].resize_threshold=result_rpc->value.hash_table_rpc_ptr_val[i0].resize_threshold;
    value[i0].prime_offset=result_rpc->value.hash_table_rpc_ptr_val[i0].prime_offset;
            }
        }
    return value;
}

int set_dl_url_file_map_wrapper(struct hash_table* value,int len)
{
    void *result;
    hash_table_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.hash_table_rpc_ptr_len=0;
    value_rpc_ptr.hash_table_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.hash_table_rpc_ptr_len=1;
    value_rpc_ptr.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
    if(value_rpc_ptr.hash_table_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.hash_table_rpc_ptr_len;i0++) {
    if (value[i0].cells == NULL) {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}
    else {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));
    	if (value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    	}
    }
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].size=value[i0].size;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].count=value[i0].count;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].resize_threshold=value[i0].resize_threshold;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].prime_offset=value[i0].prime_offset;
    }
    }
    result = set_dl_url_file_map_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void * get_dl_url_file_map_hash_function_wrapper()
{
    char_ptr *result;
    result = get_dl_url_file_map_hash_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_dl_url_file_map_hash_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_dl_url_file_map_hash_function_1(value, clnt);
}

void * get_dl_url_file_map_test_function_wrapper()
{
    char_ptr *result;
    result = get_dl_url_file_map_test_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_dl_url_file_map_test_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_dl_url_file_map_test_function_1(value, clnt);
}

struct cell* get_dl_url_file_map_cells_wrapper()
{
    dl_url_file_map_cells_ret_t *result_rpc;
    struct cell* value = {0};
    result_rpc = get_dl_url_file_map_cells_1(clnt);
        if(result_rpc->value.cell_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.cell_rpc_ptr_len*sizeof(struct cell));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.cell_rpc_ptr_len;i0++) {
    if (result_rpc->value.cell_rpc_ptr_val[i0].key.key_val == NULL) {value[i0].key = NULL;}
    else {value[i0].key = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].key.key_len * sizeof(char));
    	if (value[i0].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].key, result_rpc->value.cell_rpc_ptr_val[i0].key.key_val, result_rpc->value.cell_rpc_ptr_val[i0].key.key_len);
    }
    if (result_rpc->value.cell_rpc_ptr_val[i0].value.value_val == NULL) {value[i0].value = NULL;}
    else {value[i0].value = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].value.value_len * sizeof(char));
    	if (value[i0].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].value, result_rpc->value.cell_rpc_ptr_val[i0].value.value_val, result_rpc->value.cell_rpc_ptr_val[i0].value.value_len);
    }
            }
        }
    return value;
}

int set_dl_url_file_map_cells_wrapper(struct cell* value,int len)
{
    void *result;
    cell_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.cell_rpc_ptr_len=0;
    value_rpc_ptr.cell_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.cell_rpc_ptr_len=1;
    value_rpc_ptr.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
    if(value_rpc_ptr.cell_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.cell_rpc_ptr_len;i0++) {
    }
    }
    result = set_dl_url_file_map_cells_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void* get_dl_url_file_map_cells_key_wrapper(size_t *out_size)
{
    dl_url_file_map_cells_key_ret_t *result;
    void *ret_val = NULL;
    result = get_dl_url_file_map_cells_key_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_dl_url_file_map_cells_key_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_dl_url_file_map_cells_key_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

void* get_dl_url_file_map_cells_value_wrapper(size_t *out_size)
{
    dl_url_file_map_cells_value_ret_t *result;
    void *ret_val = NULL;
    result = get_dl_url_file_map_cells_value_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_dl_url_file_map_cells_value_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_dl_url_file_map_cells_value_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_dl_url_file_map_size_wrapper()
{
    dl_url_file_map_size_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_dl_url_file_map_size_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_dl_url_file_map_size_wrapper(int value)
{
    void *result;
    result = set_dl_url_file_map_size_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_dl_url_file_map_count_wrapper()
{
    dl_url_file_map_count_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_dl_url_file_map_count_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_dl_url_file_map_count_wrapper(int value)
{
    void *result;
    result = set_dl_url_file_map_count_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_dl_url_file_map_resize_threshold_wrapper()
{
    dl_url_file_map_resize_threshold_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_dl_url_file_map_resize_threshold_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_dl_url_file_map_resize_threshold_wrapper(int value)
{
    void *result;
    result = set_dl_url_file_map_resize_threshold_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_dl_url_file_map_prime_offset_wrapper()
{
    dl_url_file_map_prime_offset_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_dl_url_file_map_prime_offset_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_dl_url_file_map_prime_offset_wrapper(int value)
{
    void *result;
    result = set_dl_url_file_map_prime_offset_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

FILE* get_warc_log_fp_wrapper()
{
    char_ptr *result;
    FILE *ret_val = NULL;
    result = get_warc_log_fp_1(clnt);
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

void set_warc_log_fp_wrapper(FILE *file)
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
    result = set_warc_log_fp_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
    }
}

int get_final_exit_status_wrapper()
{
    final_exit_status_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_final_exit_status_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_final_exit_status_wrapper(int value)
{
    void *result;
    result = set_final_exit_status_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

struct hash_table* get_converted_files_wrapper()
{
    converted_files_ret_t *result_rpc;
    struct hash_table* value = {0};
    result_rpc = get_converted_files_1(clnt);
        if(result_rpc->value.hash_table_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.hash_table_rpc_ptr_len*sizeof(struct hash_table));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.hash_table_rpc_ptr_len;i0++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {value[i0].cells = NULL;}
    else {
    	value[i0].cells = (struct cell *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof( struct cell));
    	if (value[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {value[i0].cells[i1].key = NULL;}
    else {value[i0].cells[i1].key = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len * sizeof(char));
    	if (value[i0].cells[i1].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].key, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);
    }
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {value[i0].cells[i1].value = NULL;}
    else {value[i0].cells[i1].value = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len * sizeof(char));
    	if (value[i0].cells[i1].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].value, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);
    }
    	}
    }
    value[i0].size=result_rpc->value.hash_table_rpc_ptr_val[i0].size;
    value[i0].count=result_rpc->value.hash_table_rpc_ptr_val[i0].count;
    value[i0].resize_threshold=result_rpc->value.hash_table_rpc_ptr_val[i0].resize_threshold;
    value[i0].prime_offset=result_rpc->value.hash_table_rpc_ptr_val[i0].prime_offset;
            }
        }
    return value;
}

int set_converted_files_wrapper(struct hash_table* value,int len)
{
    void *result;
    hash_table_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.hash_table_rpc_ptr_len=0;
    value_rpc_ptr.hash_table_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.hash_table_rpc_ptr_len=1;
    value_rpc_ptr.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
    if(value_rpc_ptr.hash_table_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.hash_table_rpc_ptr_len;i0++) {
    if (value[i0].cells == NULL) {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}
    else {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));
    	if (value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    	}
    }
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].size=value[i0].size;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].count=value[i0].count;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].resize_threshold=value[i0].resize_threshold;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].prime_offset=value[i0].prime_offset;
    }
    }
    result = set_converted_files_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void * get_converted_files_hash_function_wrapper()
{
    char_ptr *result;
    result = get_converted_files_hash_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_converted_files_hash_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_converted_files_hash_function_1(value, clnt);
}

void * get_converted_files_test_function_wrapper()
{
    char_ptr *result;
    result = get_converted_files_test_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_converted_files_test_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_converted_files_test_function_1(value, clnt);
}

struct cell* get_converted_files_cells_wrapper()
{
    converted_files_cells_ret_t *result_rpc;
    struct cell* value = {0};
    result_rpc = get_converted_files_cells_1(clnt);
        if(result_rpc->value.cell_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.cell_rpc_ptr_len*sizeof(struct cell));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.cell_rpc_ptr_len;i0++) {
    if (result_rpc->value.cell_rpc_ptr_val[i0].key.key_val == NULL) {value[i0].key = NULL;}
    else {value[i0].key = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].key.key_len * sizeof(char));
    	if (value[i0].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].key, result_rpc->value.cell_rpc_ptr_val[i0].key.key_val, result_rpc->value.cell_rpc_ptr_val[i0].key.key_len);
    }
    if (result_rpc->value.cell_rpc_ptr_val[i0].value.value_val == NULL) {value[i0].value = NULL;}
    else {value[i0].value = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].value.value_len * sizeof(char));
    	if (value[i0].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].value, result_rpc->value.cell_rpc_ptr_val[i0].value.value_val, result_rpc->value.cell_rpc_ptr_val[i0].value.value_len);
    }
            }
        }
    return value;
}

int set_converted_files_cells_wrapper(struct cell* value,int len)
{
    void *result;
    cell_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.cell_rpc_ptr_len=0;
    value_rpc_ptr.cell_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.cell_rpc_ptr_len=1;
    value_rpc_ptr.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
    if(value_rpc_ptr.cell_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.cell_rpc_ptr_len;i0++) {
    }
    }
    result = set_converted_files_cells_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void* get_converted_files_cells_key_wrapper(size_t *out_size)
{
    converted_files_cells_key_ret_t *result;
    void *ret_val = NULL;
    result = get_converted_files_cells_key_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_converted_files_cells_key_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_converted_files_cells_key_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

void* get_converted_files_cells_value_wrapper(size_t *out_size)
{
    converted_files_cells_value_ret_t *result;
    void *ret_val = NULL;
    result = get_converted_files_cells_value_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_converted_files_cells_value_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_converted_files_cells_value_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_converted_files_size_wrapper()
{
    converted_files_size_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_converted_files_size_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_converted_files_size_wrapper(int value)
{
    void *result;
    result = set_converted_files_size_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_converted_files_count_wrapper()
{
    converted_files_count_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_converted_files_count_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_converted_files_count_wrapper(int value)
{
    void *result;
    result = set_converted_files_count_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_converted_files_resize_threshold_wrapper()
{
    converted_files_resize_threshold_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_converted_files_resize_threshold_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_converted_files_resize_threshold_wrapper(int value)
{
    void *result;
    result = set_converted_files_resize_threshold_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_converted_files_prime_offset_wrapper()
{
    converted_files_prime_offset_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_converted_files_prime_offset_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_converted_files_prime_offset_wrapper(int value)
{
    void *result;
    result = set_converted_files_prime_offset_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_log_line_current_wrapper()
{
    log_line_current_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_log_line_current_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_log_line_current_wrapper(int value)
{
    void *result;
    result = set_log_line_current_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

struct options get_opt_wrapper()
{
    opt_ret_t *result_rpc;
    struct options default_value = {0};
    result_rpc = get_opt_1(clnt);
    if (result_rpc == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    default_value.verbose=result_rpc->value.verbose;
    default_value.quiet=result_rpc->value.quiet;
    default_value.ntry=result_rpc->value.ntry;
    default_value.retry_connrefused=result_rpc->value.retry_connrefused;
    default_value.background=result_rpc->value.background;
    default_value.ignore_length=result_rpc->value.ignore_length;
    default_value.recursive=result_rpc->value.recursive;
    default_value.spanhost=result_rpc->value.spanhost;
    default_value.max_redirect=result_rpc->value.max_redirect;
    default_value.relative_only=result_rpc->value.relative_only;
    default_value.no_parent=result_rpc->value.no_parent;
    default_value.reclevel=result_rpc->value.reclevel;
    default_value.dirstruct=result_rpc->value.dirstruct;
    default_value.no_dirstruct=result_rpc->value.no_dirstruct;
    default_value.cut_dirs=result_rpc->value.cut_dirs;
    default_value.add_hostdir=result_rpc->value.add_hostdir;
    default_value.protocol_directories=result_rpc->value.protocol_directories;
    default_value.noclobber=result_rpc->value.noclobber;
    default_value.unlink=result_rpc->value.unlink;
    if (result_rpc->value.dir_prefix.charptr.charptr_val == NULL) {default_value.dir_prefix=NULL;}

    else {default_value.dir_prefix=malloc(result_rpc->value.dir_prefix.charptr.charptr_len);

    memcpy(default_value.dir_prefix,result_rpc->value.dir_prefix.charptr.charptr_val,result_rpc->value.dir_prefix.charptr.charptr_len);

    }

    if (result_rpc->value.lfilename.charptr.charptr_val == NULL) {default_value.lfilename=NULL;}

    else {default_value.lfilename=malloc(result_rpc->value.lfilename.charptr.charptr_len);

    memcpy(default_value.lfilename,result_rpc->value.lfilename.charptr.charptr_val,result_rpc->value.lfilename.charptr.charptr_len);

    }

    if (result_rpc->value.input_filename.charptr.charptr_val == NULL) {default_value.input_filename=NULL;}

    else {default_value.input_filename=malloc(result_rpc->value.input_filename.charptr.charptr_len);

    memcpy(default_value.input_filename,result_rpc->value.input_filename.charptr.charptr_val,result_rpc->value.input_filename.charptr.charptr_len);

    }

    if (result_rpc->value.choose_config.charptr.charptr_val == NULL) {default_value.choose_config=NULL;}

    else {default_value.choose_config=malloc(result_rpc->value.choose_config.charptr.charptr_len);

    memcpy(default_value.choose_config,result_rpc->value.choose_config.charptr.charptr_val,result_rpc->value.choose_config.charptr.charptr_len);

    }

    default_value.noconfig=result_rpc->value.noconfig;
    default_value.force_html=result_rpc->value.force_html;
    if (result_rpc->value.default_page.charptr.charptr_val == NULL) {default_value.default_page=NULL;}

    else {default_value.default_page=malloc(result_rpc->value.default_page.charptr.charptr_len);

    memcpy(default_value.default_page,result_rpc->value.default_page.charptr.charptr_val,result_rpc->value.default_page.charptr.charptr_len);

    }

    default_value.spider=result_rpc->value.spider;
    if (result_rpc->value.accepts.twocharptr.twocharptr_val == NULL) {default_value.accepts=NULL;}

    else {default_value.accepts=(char **)malloc(result_rpc->value.accepts.twocharptr.twocharptr_len * sizeof(char *));

    	if (default_value.accepts == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	for (int i = 0; i < result_rpc->value.accepts.twocharptr.twocharptr_len; i++) {

    		if (result_rpc->value.accepts.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {

    			default_value.accepts[i] = NULL;

    		} else {

    			default_value.accepts[i] = strdup(result_rpc->value.accepts.twocharptr.twocharptr_val[i].charptr.charptr_val);

    		}

    	}

    }

    if (result_rpc->value.rejects.twocharptr.twocharptr_val == NULL) {default_value.rejects=NULL;}

    else {default_value.rejects=(char **)malloc(result_rpc->value.rejects.twocharptr.twocharptr_len * sizeof(char *));

    	if (default_value.rejects == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	for (int i = 0; i < result_rpc->value.rejects.twocharptr.twocharptr_len; i++) {

    		if (result_rpc->value.rejects.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {

    			default_value.rejects[i] = NULL;

    		} else {

    			default_value.rejects[i] = strdup(result_rpc->value.rejects.twocharptr.twocharptr_val[i].charptr.charptr_val);

    		}

    	}

    }

    if (result_rpc->value.excludes.twocharptr.twocharptr_val == NULL) {default_value.excludes=NULL;}

    else {default_value.excludes=(char **)malloc(result_rpc->value.excludes.twocharptr.twocharptr_len * sizeof(char *));

    	if (default_value.excludes == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	for (int i = 0; i < result_rpc->value.excludes.twocharptr.twocharptr_len; i++) {

    		if (result_rpc->value.excludes.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {

    			default_value.excludes[i] = NULL;

    		} else {

    			default_value.excludes[i] = strdup(result_rpc->value.excludes.twocharptr.twocharptr_val[i].charptr.charptr_val);

    		}

    	}

    }

    if (result_rpc->value.includes.twocharptr.twocharptr_val == NULL) {default_value.includes=NULL;}

    else {default_value.includes=(char **)malloc(result_rpc->value.includes.twocharptr.twocharptr_len * sizeof(char *));

    	if (default_value.includes == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	for (int i = 0; i < result_rpc->value.includes.twocharptr.twocharptr_len; i++) {

    		if (result_rpc->value.includes.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {

    			default_value.includes[i] = NULL;

    		} else {

    			default_value.includes[i] = strdup(result_rpc->value.includes.twocharptr.twocharptr_val[i].charptr.charptr_val);

    		}

    	}

    }

    default_value.ignore_case=result_rpc->value.ignore_case;
    if (result_rpc->value.acceptregex_s.charptr.charptr_val == NULL) {default_value.acceptregex_s=NULL;}

    else {default_value.acceptregex_s=malloc(result_rpc->value.acceptregex_s.charptr.charptr_len);

    memcpy(default_value.acceptregex_s,result_rpc->value.acceptregex_s.charptr.charptr_val,result_rpc->value.acceptregex_s.charptr.charptr_len);

    }

    if (result_rpc->value.rejectregex_s.charptr.charptr_val == NULL) {default_value.rejectregex_s=NULL;}

    else {default_value.rejectregex_s=malloc(result_rpc->value.rejectregex_s.charptr.charptr_len);

    memcpy(default_value.rejectregex_s,result_rpc->value.rejectregex_s.charptr.charptr_val,result_rpc->value.rejectregex_s.charptr.charptr_len);

    }

    if (result_rpc->value.acceptregex.acceptregex_val == NULL) {default_value.acceptregex=NULL;}

    else {default_value.acceptregex=malloc(result_rpc->value.acceptregex.acceptregex_len);

    	if (default_value.acceptregex == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(default_value.acceptregex,result_rpc->value.acceptregex.acceptregex_val,result_rpc->value.acceptregex.acceptregex_len);

    }

    if (result_rpc->value.rejectregex.rejectregex_val == NULL) {default_value.rejectregex=NULL;}

    else {default_value.rejectregex=malloc(result_rpc->value.rejectregex.rejectregex_len);

    	if (default_value.rejectregex == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(default_value.rejectregex,result_rpc->value.rejectregex.rejectregex_val,result_rpc->value.rejectregex.rejectregex_len);

    }

    if (result_rpc->value.domains.twocharptr.twocharptr_val == NULL) {default_value.domains=NULL;}

    else {default_value.domains=(char **)malloc(result_rpc->value.domains.twocharptr.twocharptr_len * sizeof(char *));

    	if (default_value.domains == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	for (int i = 0; i < result_rpc->value.domains.twocharptr.twocharptr_len; i++) {

    		if (result_rpc->value.domains.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {

    			default_value.domains[i] = NULL;

    		} else {

    			default_value.domains[i] = strdup(result_rpc->value.domains.twocharptr.twocharptr_val[i].charptr.charptr_val);

    		}

    	}

    }

    if (result_rpc->value.exclude_domains.twocharptr.twocharptr_val == NULL) {default_value.exclude_domains=NULL;}

    else {default_value.exclude_domains=(char **)malloc(result_rpc->value.exclude_domains.twocharptr.twocharptr_len * sizeof(char *));

    	if (default_value.exclude_domains == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	for (int i = 0; i < result_rpc->value.exclude_domains.twocharptr.twocharptr_len; i++) {

    		if (result_rpc->value.exclude_domains.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {

    			default_value.exclude_domains[i] = NULL;

    		} else {

    			default_value.exclude_domains[i] = strdup(result_rpc->value.exclude_domains.twocharptr.twocharptr_val[i].charptr.charptr_val);

    		}

    	}

    }

    default_value.dns_cache=result_rpc->value.dns_cache;
    if (result_rpc->value.follow_tags.twocharptr.twocharptr_val == NULL) {default_value.follow_tags=NULL;}

    else {default_value.follow_tags=(char **)malloc(result_rpc->value.follow_tags.twocharptr.twocharptr_len * sizeof(char *));

    	if (default_value.follow_tags == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	for (int i = 0; i < result_rpc->value.follow_tags.twocharptr.twocharptr_len; i++) {

    		if (result_rpc->value.follow_tags.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {

    			default_value.follow_tags[i] = NULL;

    		} else {

    			default_value.follow_tags[i] = strdup(result_rpc->value.follow_tags.twocharptr.twocharptr_val[i].charptr.charptr_val);

    		}

    	}

    }

    if (result_rpc->value.ignore_tags.twocharptr.twocharptr_val == NULL) {default_value.ignore_tags=NULL;}

    else {default_value.ignore_tags=(char **)malloc(result_rpc->value.ignore_tags.twocharptr.twocharptr_len * sizeof(char *));

    	if (default_value.ignore_tags == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	for (int i = 0; i < result_rpc->value.ignore_tags.twocharptr.twocharptr_len; i++) {

    		if (result_rpc->value.ignore_tags.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {

    			default_value.ignore_tags[i] = NULL;

    		} else {

    			default_value.ignore_tags[i] = strdup(result_rpc->value.ignore_tags.twocharptr.twocharptr_val[i].charptr.charptr_val);

    		}

    	}

    }

    default_value.follow_ftp=result_rpc->value.follow_ftp;
    default_value.retr_symlinks=result_rpc->value.retr_symlinks;
    if (result_rpc->value.output_document.charptr.charptr_val == NULL) {default_value.output_document=NULL;}

    else {default_value.output_document=malloc(result_rpc->value.output_document.charptr.charptr_len);

    memcpy(default_value.output_document,result_rpc->value.output_document.charptr.charptr_val,result_rpc->value.output_document.charptr.charptr_len);

    }

    if (result_rpc->value.warc_filename.charptr.charptr_val == NULL) {default_value.warc_filename=NULL;}

    else {default_value.warc_filename=malloc(result_rpc->value.warc_filename.charptr.charptr_len);

    memcpy(default_value.warc_filename,result_rpc->value.warc_filename.charptr.charptr_val,result_rpc->value.warc_filename.charptr.charptr_len);

    }

    if (result_rpc->value.warc_tempdir.charptr.charptr_val == NULL) {default_value.warc_tempdir=NULL;}

    else {default_value.warc_tempdir=malloc(result_rpc->value.warc_tempdir.charptr.charptr_len);

    memcpy(default_value.warc_tempdir,result_rpc->value.warc_tempdir.charptr.charptr_val,result_rpc->value.warc_tempdir.charptr.charptr_len);

    }

    if (result_rpc->value.warc_cdx_dedup_filename.charptr.charptr_val == NULL) {default_value.warc_cdx_dedup_filename=NULL;}

    else {default_value.warc_cdx_dedup_filename=malloc(result_rpc->value.warc_cdx_dedup_filename.charptr.charptr_len);

    memcpy(default_value.warc_cdx_dedup_filename,result_rpc->value.warc_cdx_dedup_filename.charptr.charptr_val,result_rpc->value.warc_cdx_dedup_filename.charptr.charptr_len);

    }

    default_value.warc_maxsize=result_rpc->value.warc_maxsize;
    default_value.warc_compression_enabled=result_rpc->value.warc_compression_enabled;
    default_value.warc_digests_enabled=result_rpc->value.warc_digests_enabled;
    default_value.warc_cdx_enabled=result_rpc->value.warc_cdx_enabled;
    default_value.warc_keep_log=result_rpc->value.warc_keep_log;
    if (result_rpc->value.warc_user_headers.twocharptr.twocharptr_val == NULL) {default_value.warc_user_headers=NULL;}

    else {default_value.warc_user_headers=(char **)malloc(result_rpc->value.warc_user_headers.twocharptr.twocharptr_len * sizeof(char *));

    	if (default_value.warc_user_headers == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	for (int i = 0; i < result_rpc->value.warc_user_headers.twocharptr.twocharptr_len; i++) {

    		if (result_rpc->value.warc_user_headers.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {

    			default_value.warc_user_headers[i] = NULL;

    		} else {

    			default_value.warc_user_headers[i] = strdup(result_rpc->value.warc_user_headers.twocharptr.twocharptr_val[i].charptr.charptr_val);

    		}

    	}

    }

    if (result_rpc->value.user.charptr.charptr_val == NULL) {default_value.user=NULL;}

    else {default_value.user=malloc(result_rpc->value.user.charptr.charptr_len);

    memcpy(default_value.user,result_rpc->value.user.charptr.charptr_val,result_rpc->value.user.charptr.charptr_len);

    }

    if (result_rpc->value.passwd.charptr.charptr_val == NULL) {default_value.passwd=NULL;}

    else {default_value.passwd=malloc(result_rpc->value.passwd.charptr.charptr_len);

    memcpy(default_value.passwd,result_rpc->value.passwd.charptr.charptr_val,result_rpc->value.passwd.charptr.charptr_len);

    }

    default_value.ask_passwd=result_rpc->value.ask_passwd;
    default_value.always_rest=result_rpc->value.always_rest;
    default_value.start_pos=result_rpc->value.start_pos;
    if (result_rpc->value.ftp_user.charptr.charptr_val == NULL) {default_value.ftp_user=NULL;}

    else {default_value.ftp_user=malloc(result_rpc->value.ftp_user.charptr.charptr_len);

    memcpy(default_value.ftp_user,result_rpc->value.ftp_user.charptr.charptr_val,result_rpc->value.ftp_user.charptr.charptr_len);

    }

    if (result_rpc->value.ftp_passwd.charptr.charptr_val == NULL) {default_value.ftp_passwd=NULL;}

    else {default_value.ftp_passwd=malloc(result_rpc->value.ftp_passwd.charptr.charptr_len);

    memcpy(default_value.ftp_passwd,result_rpc->value.ftp_passwd.charptr.charptr_val,result_rpc->value.ftp_passwd.charptr.charptr_len);

    }

    default_value.netrc=result_rpc->value.netrc;
    default_value.ftp_glob=result_rpc->value.ftp_glob;
    default_value.ftp_pasv=result_rpc->value.ftp_pasv;
    if (result_rpc->value.http_user.charptr.charptr_val == NULL) {default_value.http_user=NULL;}

    else {default_value.http_user=malloc(result_rpc->value.http_user.charptr.charptr_len);

    memcpy(default_value.http_user,result_rpc->value.http_user.charptr.charptr_val,result_rpc->value.http_user.charptr.charptr_len);

    }

    if (result_rpc->value.http_passwd.charptr.charptr_val == NULL) {default_value.http_passwd=NULL;}

    else {default_value.http_passwd=malloc(result_rpc->value.http_passwd.charptr.charptr_len);

    memcpy(default_value.http_passwd,result_rpc->value.http_passwd.charptr.charptr_val,result_rpc->value.http_passwd.charptr.charptr_len);

    }

    if (result_rpc->value.user_headers.twocharptr.twocharptr_val == NULL) {default_value.user_headers=NULL;}

    else {default_value.user_headers=(char **)malloc(result_rpc->value.user_headers.twocharptr.twocharptr_len * sizeof(char *));

    	if (default_value.user_headers == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	for (int i = 0; i < result_rpc->value.user_headers.twocharptr.twocharptr_len; i++) {

    		if (result_rpc->value.user_headers.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {

    			default_value.user_headers[i] = NULL;

    		} else {

    			default_value.user_headers[i] = strdup(result_rpc->value.user_headers.twocharptr.twocharptr_val[i].charptr.charptr_val);

    		}

    	}

    }

    default_value.http_keep_alive=result_rpc->value.http_keep_alive;
    default_value.use_proxy=result_rpc->value.use_proxy;
    default_value.allow_cache=result_rpc->value.allow_cache;
    if (result_rpc->value.http_proxy.charptr.charptr_val == NULL) {default_value.http_proxy=NULL;}

    else {default_value.http_proxy=malloc(result_rpc->value.http_proxy.charptr.charptr_len);

    memcpy(default_value.http_proxy,result_rpc->value.http_proxy.charptr.charptr_val,result_rpc->value.http_proxy.charptr.charptr_len);

    }

    if (result_rpc->value.ftp_proxy.charptr.charptr_val == NULL) {default_value.ftp_proxy=NULL;}

    else {default_value.ftp_proxy=malloc(result_rpc->value.ftp_proxy.charptr.charptr_len);

    memcpy(default_value.ftp_proxy,result_rpc->value.ftp_proxy.charptr.charptr_val,result_rpc->value.ftp_proxy.charptr.charptr_len);

    }

    if (result_rpc->value.https_proxy.charptr.charptr_val == NULL) {default_value.https_proxy=NULL;}

    else {default_value.https_proxy=malloc(result_rpc->value.https_proxy.charptr.charptr_len);

    memcpy(default_value.https_proxy,result_rpc->value.https_proxy.charptr.charptr_val,result_rpc->value.https_proxy.charptr.charptr_len);

    }

    if (result_rpc->value.no_proxy.twocharptr.twocharptr_val == NULL) {default_value.no_proxy=NULL;}

    else {default_value.no_proxy=(char **)malloc(result_rpc->value.no_proxy.twocharptr.twocharptr_len * sizeof(char *));

    	if (default_value.no_proxy == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	for (int i = 0; i < result_rpc->value.no_proxy.twocharptr.twocharptr_len; i++) {

    		if (result_rpc->value.no_proxy.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {

    			default_value.no_proxy[i] = NULL;

    		} else {

    			default_value.no_proxy[i] = strdup(result_rpc->value.no_proxy.twocharptr.twocharptr_val[i].charptr.charptr_val);

    		}

    	}

    }

    if (result_rpc->value.base_href.charptr.charptr_val == NULL) {default_value.base_href=NULL;}

    else {default_value.base_href=malloc(result_rpc->value.base_href.charptr.charptr_len);

    memcpy(default_value.base_href,result_rpc->value.base_href.charptr.charptr_val,result_rpc->value.base_href.charptr.charptr_len);

    }

    if (result_rpc->value.progress_type.charptr.charptr_val == NULL) {default_value.progress_type=NULL;}

    else {default_value.progress_type=malloc(result_rpc->value.progress_type.charptr.charptr_len);

    memcpy(default_value.progress_type,result_rpc->value.progress_type.charptr.charptr_val,result_rpc->value.progress_type.charptr.charptr_len);

    }

    default_value.show_progress=result_rpc->value.show_progress;
    default_value.noscroll=result_rpc->value.noscroll;
    if (result_rpc->value.proxy_user.charptr.charptr_val == NULL) {default_value.proxy_user=NULL;}

    else {default_value.proxy_user=malloc(result_rpc->value.proxy_user.charptr.charptr_len);

    memcpy(default_value.proxy_user,result_rpc->value.proxy_user.charptr.charptr_val,result_rpc->value.proxy_user.charptr.charptr_len);

    }

    if (result_rpc->value.proxy_passwd.charptr.charptr_val == NULL) {default_value.proxy_passwd=NULL;}

    else {default_value.proxy_passwd=malloc(result_rpc->value.proxy_passwd.charptr.charptr_len);

    memcpy(default_value.proxy_passwd,result_rpc->value.proxy_passwd.charptr.charptr_val,result_rpc->value.proxy_passwd.charptr.charptr_len);

    }

    default_value.read_timeout=result_rpc->value.read_timeout;
    default_value.dns_timeout=result_rpc->value.dns_timeout;
    default_value.connect_timeout=result_rpc->value.connect_timeout;
    default_value.random_wait=result_rpc->value.random_wait;
    default_value.wait=result_rpc->value.wait;
    default_value.waitretry=result_rpc->value.waitretry;
    default_value.use_robots=result_rpc->value.use_robots;
    default_value.limit_rate=result_rpc->value.limit_rate;
    default_value.server_response=result_rpc->value.server_response;
    default_value.save_headers=result_rpc->value.save_headers;
    default_value.content_on_error=result_rpc->value.content_on_error;
    default_value.debug=result_rpc->value.debug;
    default_value.timestamping=result_rpc->value.timestamping;
    default_value.if_modified_since=result_rpc->value.if_modified_since;
    default_value.backup_converted=result_rpc->value.backup_converted;
    default_value.backups=result_rpc->value.backups;
    if (result_rpc->value.useragent.charptr.charptr_val == NULL) {default_value.useragent=NULL;}

    else {default_value.useragent=malloc(result_rpc->value.useragent.charptr.charptr_len);

    memcpy(default_value.useragent,result_rpc->value.useragent.charptr.charptr_val,result_rpc->value.useragent.charptr.charptr_len);

    }

    if (result_rpc->value.referer.charptr.charptr_val == NULL) {default_value.referer=NULL;}

    else {default_value.referer=malloc(result_rpc->value.referer.charptr.charptr_len);

    memcpy(default_value.referer,result_rpc->value.referer.charptr.charptr_val,result_rpc->value.referer.charptr.charptr_len);

    }

    default_value.convert_links=result_rpc->value.convert_links;
    default_value.convert_file_only=result_rpc->value.convert_file_only;
    default_value.remove_listing=result_rpc->value.remove_listing;
    default_value.htmlify=result_rpc->value.htmlify;
    if (result_rpc->value.dot_style.charptr.charptr_val == NULL) {default_value.dot_style=NULL;}

    else {default_value.dot_style=malloc(result_rpc->value.dot_style.charptr.charptr_len);

    memcpy(default_value.dot_style,result_rpc->value.dot_style.charptr.charptr_val,result_rpc->value.dot_style.charptr.charptr_len);

    }

    default_value.dot_bytes=result_rpc->value.dot_bytes;
    default_value.dots_in_line=result_rpc->value.dots_in_line;
    default_value.dot_spacing=result_rpc->value.dot_spacing;
    default_value.delete_after=result_rpc->value.delete_after;
    default_value.adjust_extension=result_rpc->value.adjust_extension;
    default_value.page_requisites=result_rpc->value.page_requisites;
    if (result_rpc->value.bind_address.charptr.charptr_val == NULL) {default_value.bind_address=NULL;}

    else {default_value.bind_address=malloc(result_rpc->value.bind_address.charptr.charptr_len);

    memcpy(default_value.bind_address,result_rpc->value.bind_address.charptr.charptr_val,result_rpc->value.bind_address.charptr.charptr_len);

    }

    default_value.check_cert=result_rpc->value.check_cert;
    if (result_rpc->value.cert_file.charptr.charptr_val == NULL) {default_value.cert_file=NULL;}

    else {default_value.cert_file=malloc(result_rpc->value.cert_file.charptr.charptr_len);

    memcpy(default_value.cert_file,result_rpc->value.cert_file.charptr.charptr_val,result_rpc->value.cert_file.charptr.charptr_len);

    }

    if (result_rpc->value.private_key.charptr.charptr_val == NULL) {default_value.private_key=NULL;}

    else {default_value.private_key=malloc(result_rpc->value.private_key.charptr.charptr_len);

    memcpy(default_value.private_key,result_rpc->value.private_key.charptr.charptr_val,result_rpc->value.private_key.charptr.charptr_len);

    }

    default_value.private_key_type=result_rpc->value.private_key_type;
    if (result_rpc->value.ca_directory.charptr.charptr_val == NULL) {default_value.ca_directory=NULL;}

    else {default_value.ca_directory=malloc(result_rpc->value.ca_directory.charptr.charptr_len);

    memcpy(default_value.ca_directory,result_rpc->value.ca_directory.charptr.charptr_val,result_rpc->value.ca_directory.charptr.charptr_len);

    }

    if (result_rpc->value.ca_cert.charptr.charptr_val == NULL) {default_value.ca_cert=NULL;}

    else {default_value.ca_cert=malloc(result_rpc->value.ca_cert.charptr.charptr_len);

    memcpy(default_value.ca_cert,result_rpc->value.ca_cert.charptr.charptr_val,result_rpc->value.ca_cert.charptr.charptr_len);

    }

    if (result_rpc->value.crl_file.charptr.charptr_val == NULL) {default_value.crl_file=NULL;}

    else {default_value.crl_file=malloc(result_rpc->value.crl_file.charptr.charptr_len);

    memcpy(default_value.crl_file,result_rpc->value.crl_file.charptr.charptr_val,result_rpc->value.crl_file.charptr.charptr_len);

    }

    if (result_rpc->value.pinnedpubkey.charptr.charptr_val == NULL) {default_value.pinnedpubkey=NULL;}

    else {default_value.pinnedpubkey=malloc(result_rpc->value.pinnedpubkey.charptr.charptr_len);

    memcpy(default_value.pinnedpubkey,result_rpc->value.pinnedpubkey.charptr.charptr_val,result_rpc->value.pinnedpubkey.charptr.charptr_len);

    }

    if (result_rpc->value.random_file.charptr.charptr_val == NULL) {default_value.random_file=NULL;}

    else {default_value.random_file=malloc(result_rpc->value.random_file.charptr.charptr_len);

    memcpy(default_value.random_file,result_rpc->value.random_file.charptr.charptr_val,result_rpc->value.random_file.charptr.charptr_len);

    }

    if (result_rpc->value.egd_file.charptr.charptr_val == NULL) {default_value.egd_file=NULL;}

    else {default_value.egd_file=malloc(result_rpc->value.egd_file.charptr.charptr_len);

    memcpy(default_value.egd_file,result_rpc->value.egd_file.charptr.charptr_val,result_rpc->value.egd_file.charptr.charptr_len);

    }

    default_value.https_only=result_rpc->value.https_only;
    default_value.ftps_resume_ssl=result_rpc->value.ftps_resume_ssl;
    default_value.ftps_fallback_to_ftp=result_rpc->value.ftps_fallback_to_ftp;
    default_value.ftps_implicit=result_rpc->value.ftps_implicit;
    default_value.ftps_clear_data_connection=result_rpc->value.ftps_clear_data_connection;
    default_value.cookies=result_rpc->value.cookies;
    if (result_rpc->value.cookies_input.charptr.charptr_val == NULL) {default_value.cookies_input=NULL;}

    else {default_value.cookies_input=malloc(result_rpc->value.cookies_input.charptr.charptr_len);

    memcpy(default_value.cookies_input,result_rpc->value.cookies_input.charptr.charptr_val,result_rpc->value.cookies_input.charptr.charptr_len);

    }

    if (result_rpc->value.cookies_output.charptr.charptr_val == NULL) {default_value.cookies_output=NULL;}

    else {default_value.cookies_output=malloc(result_rpc->value.cookies_output.charptr.charptr_len);

    memcpy(default_value.cookies_output,result_rpc->value.cookies_output.charptr.charptr_val,result_rpc->value.cookies_output.charptr.charptr_len);

    }

    default_value.keep_session_cookies=result_rpc->value.keep_session_cookies;
    if (result_rpc->value.post_data.charptr.charptr_val == NULL) {default_value.post_data=NULL;}

    else {default_value.post_data=malloc(result_rpc->value.post_data.charptr.charptr_len);

    memcpy(default_value.post_data,result_rpc->value.post_data.charptr.charptr_val,result_rpc->value.post_data.charptr.charptr_len);

    }

    if (result_rpc->value.post_file_name.charptr.charptr_val == NULL) {default_value.post_file_name=NULL;}

    else {default_value.post_file_name=malloc(result_rpc->value.post_file_name.charptr.charptr_len);

    memcpy(default_value.post_file_name,result_rpc->value.post_file_name.charptr.charptr_val,result_rpc->value.post_file_name.charptr.charptr_len);

    }

    if (result_rpc->value.method.charptr.charptr_val == NULL) {default_value.method=NULL;}

    else {default_value.method=malloc(result_rpc->value.method.charptr.charptr_len);

    memcpy(default_value.method,result_rpc->value.method.charptr.charptr_val,result_rpc->value.method.charptr.charptr_len);

    }

    if (result_rpc->value.body_data.charptr.charptr_val == NULL) {default_value.body_data=NULL;}

    else {default_value.body_data=malloc(result_rpc->value.body_data.charptr.charptr_len);

    memcpy(default_value.body_data,result_rpc->value.body_data.charptr.charptr_val,result_rpc->value.body_data.charptr.charptr_len);

    }

    if (result_rpc->value.body_file.charptr.charptr_val == NULL) {default_value.body_file=NULL;}

    else {default_value.body_file=malloc(result_rpc->value.body_file.charptr.charptr_len);

    memcpy(default_value.body_file,result_rpc->value.body_file.charptr.charptr_val,result_rpc->value.body_file.charptr.charptr_len);

    }

    default_value.restrict_files_ctrl=result_rpc->value.restrict_files_ctrl;
    default_value.restrict_files_nonascii=result_rpc->value.restrict_files_nonascii;
    default_value.strict_comments=result_rpc->value.strict_comments;
    default_value.preserve_perm=result_rpc->value.preserve_perm;
    default_value.ipv4_only=result_rpc->value.ipv4_only;
    default_value.ipv6_only=result_rpc->value.ipv6_only;
    default_value.content_disposition=result_rpc->value.content_disposition;
    default_value.auth_without_challenge=result_rpc->value.auth_without_challenge;
    default_value.enable_iri=result_rpc->value.enable_iri;
    if (result_rpc->value.encoding_remote.charptr.charptr_val == NULL) {default_value.encoding_remote=NULL;}

    else {default_value.encoding_remote=malloc(result_rpc->value.encoding_remote.charptr.charptr_len);

    memcpy(default_value.encoding_remote,result_rpc->value.encoding_remote.charptr.charptr_val,result_rpc->value.encoding_remote.charptr.charptr_len);

    }

    if (result_rpc->value.locale.charptr.charptr_val == NULL) {default_value.locale=NULL;}

    else {default_value.locale=malloc(result_rpc->value.locale.charptr.charptr_len);

    memcpy(default_value.locale,result_rpc->value.locale.charptr.charptr_val,result_rpc->value.locale.charptr.charptr_len);

    }

    default_value.trustservernames=result_rpc->value.trustservernames;
    default_value.useservertimestamps=result_rpc->value.useservertimestamps;
    default_value.show_all_dns_entries=result_rpc->value.show_all_dns_entries;
    default_value.report_bps=result_rpc->value.report_bps;
    if (result_rpc->value.rejected_log.charptr.charptr_val == NULL) {default_value.rejected_log=NULL;}

    else {default_value.rejected_log=malloc(result_rpc->value.rejected_log.charptr.charptr_len);

    memcpy(default_value.rejected_log,result_rpc->value.rejected_log.charptr.charptr_val,result_rpc->value.rejected_log.charptr.charptr_len);

    }

    default_value.hsts=result_rpc->value.hsts;
    if (result_rpc->value.hsts_file.charptr.charptr_val == NULL) {default_value.hsts_file=NULL;}

    else {default_value.hsts_file=malloc(result_rpc->value.hsts_file.charptr.charptr_len);

    memcpy(default_value.hsts_file,result_rpc->value.hsts_file.charptr.charptr_val,result_rpc->value.hsts_file.charptr.charptr_len);

    }

    return default_value;
}

int set_opt_wrapper(struct options opt)
{
    void *result;
    options_rpc opt_rpc;
    opt_rpc.verbose=opt.verbose;
    opt_rpc.quiet=opt.quiet;
    opt_rpc.ntry=opt.ntry;
    opt_rpc.retry_connrefused=opt.retry_connrefused;
    opt_rpc.background=opt.background;
    opt_rpc.ignore_length=opt.ignore_length;
    opt_rpc.recursive=opt.recursive;
    opt_rpc.spanhost=opt.spanhost;
    opt_rpc.max_redirect=opt.max_redirect;
    opt_rpc.relative_only=opt.relative_only;
    opt_rpc.no_parent=opt.no_parent;
    opt_rpc.reclevel=opt.reclevel;
    opt_rpc.dirstruct=opt.dirstruct;
    opt_rpc.no_dirstruct=opt.no_dirstruct;
    opt_rpc.cut_dirs=opt.cut_dirs;
    opt_rpc.add_hostdir=opt.add_hostdir;
    opt_rpc.protocol_directories=opt.protocol_directories;
    opt_rpc.noclobber=opt.noclobber;
    opt_rpc.unlink=opt.unlink;
    if (opt.dir_prefix == NULL) {opt_rpc.dir_prefix.charptr.charptr_len = 0;opt_rpc.dir_prefix.charptr.charptr_val = NULL;}
    else {opt_rpc.dir_prefix.charptr.charptr_len = strlen(opt.dir_prefix) + 1;

    	opt_rpc.dir_prefix.charptr.charptr_val = malloc(opt_rpc.dir_prefix.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.dir_prefix.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.dir_prefix.charptr.charptr_val, opt.dir_prefix, opt_rpc.dir_prefix.charptr.charptr_len);
    }
    if (opt.lfilename == NULL) {opt_rpc.lfilename.charptr.charptr_len = 0;opt_rpc.lfilename.charptr.charptr_val = NULL;}
    else {opt_rpc.lfilename.charptr.charptr_len = strlen(opt.lfilename) + 1;

    	opt_rpc.lfilename.charptr.charptr_val = malloc(opt_rpc.lfilename.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.lfilename.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.lfilename.charptr.charptr_val, opt.lfilename, opt_rpc.lfilename.charptr.charptr_len);
    }
    if (opt.input_filename == NULL) {opt_rpc.input_filename.charptr.charptr_len = 0;opt_rpc.input_filename.charptr.charptr_val = NULL;}
    else {opt_rpc.input_filename.charptr.charptr_len = strlen(opt.input_filename) + 1;

    	opt_rpc.input_filename.charptr.charptr_val = malloc(opt_rpc.input_filename.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.input_filename.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.input_filename.charptr.charptr_val, opt.input_filename, opt_rpc.input_filename.charptr.charptr_len);
    }
    if (opt.choose_config == NULL) {opt_rpc.choose_config.charptr.charptr_len = 0;opt_rpc.choose_config.charptr.charptr_val = NULL;}
    else {opt_rpc.choose_config.charptr.charptr_len = strlen(opt.choose_config) + 1;

    	opt_rpc.choose_config.charptr.charptr_val = malloc(opt_rpc.choose_config.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.choose_config.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.choose_config.charptr.charptr_val, opt.choose_config, opt_rpc.choose_config.charptr.charptr_len);
    }
    opt_rpc.noconfig=opt.noconfig;
    opt_rpc.force_html=opt.force_html;
    if (opt.default_page == NULL) {opt_rpc.default_page.charptr.charptr_len = 0;opt_rpc.default_page.charptr.charptr_val = NULL;}
    else {opt_rpc.default_page.charptr.charptr_len = strlen(opt.default_page) + 1;

    	opt_rpc.default_page.charptr.charptr_val = malloc(opt_rpc.default_page.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.default_page.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.default_page.charptr.charptr_val, opt.default_page, opt_rpc.default_page.charptr.charptr_len);
    }
    opt_rpc.spider=opt.spider;
    if (opt.accepts == NULL) {opt_rpc.accepts.twocharptr.twocharptr_len = 0;opt_rpc.accepts.twocharptr.twocharptr_val = NULL;}
    else {opt_rpc.accepts.twocharptr.twocharptr_len = 1;opt_rpc.accepts.twocharptr.twocharptr_val = malloc(opt_rpc.accepts.twocharptr.twocharptr_len * sizeof(char_ptr));
    	if (opt_rpc.accepts.twocharptr.twocharptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < opt_rpc.accepts.twocharptr.twocharptr_len; i++) {
    		if (opt.accepts[i] == NULL) {
    			opt_rpc.accepts.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;
    			opt_rpc.accepts.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;
    		} else {
    			opt_rpc.accepts.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(opt.accepts[i]) + 1;
    			opt_rpc.accepts.twocharptr.twocharptr_val[i].charptr.charptr_val = opt.accepts[i];
    		}
    	}
    }
    if (opt.rejects == NULL) {opt_rpc.rejects.twocharptr.twocharptr_len = 0;opt_rpc.rejects.twocharptr.twocharptr_val = NULL;}
    else {opt_rpc.rejects.twocharptr.twocharptr_len = 1;opt_rpc.rejects.twocharptr.twocharptr_val = malloc(opt_rpc.rejects.twocharptr.twocharptr_len * sizeof(char_ptr));
    	if (opt_rpc.rejects.twocharptr.twocharptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < opt_rpc.rejects.twocharptr.twocharptr_len; i++) {
    		if (opt.rejects[i] == NULL) {
    			opt_rpc.rejects.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;
    			opt_rpc.rejects.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;
    		} else {
    			opt_rpc.rejects.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(opt.rejects[i]) + 1;
    			opt_rpc.rejects.twocharptr.twocharptr_val[i].charptr.charptr_val = opt.rejects[i];
    		}
    	}
    }
    if (opt.excludes == NULL) {opt_rpc.excludes.twocharptr.twocharptr_len = 0;opt_rpc.excludes.twocharptr.twocharptr_val = NULL;}
    else {opt_rpc.excludes.twocharptr.twocharptr_len = 1;opt_rpc.excludes.twocharptr.twocharptr_val = malloc(opt_rpc.excludes.twocharptr.twocharptr_len * sizeof(char_ptr));
    	if (opt_rpc.excludes.twocharptr.twocharptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < opt_rpc.excludes.twocharptr.twocharptr_len; i++) {
    		if (opt.excludes[i] == NULL) {
    			opt_rpc.excludes.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;
    			opt_rpc.excludes.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;
    		} else {
    			opt_rpc.excludes.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(opt.excludes[i]) + 1;
    			opt_rpc.excludes.twocharptr.twocharptr_val[i].charptr.charptr_val = opt.excludes[i];
    		}
    	}
    }
    if (opt.includes == NULL) {opt_rpc.includes.twocharptr.twocharptr_len = 0;opt_rpc.includes.twocharptr.twocharptr_val = NULL;}
    else {opt_rpc.includes.twocharptr.twocharptr_len = 1;opt_rpc.includes.twocharptr.twocharptr_val = malloc(opt_rpc.includes.twocharptr.twocharptr_len * sizeof(char_ptr));
    	if (opt_rpc.includes.twocharptr.twocharptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < opt_rpc.includes.twocharptr.twocharptr_len; i++) {
    		if (opt.includes[i] == NULL) {
    			opt_rpc.includes.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;
    			opt_rpc.includes.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;
    		} else {
    			opt_rpc.includes.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(opt.includes[i]) + 1;
    			opt_rpc.includes.twocharptr.twocharptr_val[i].charptr.charptr_val = opt.includes[i];
    		}
    	}
    }
    opt_rpc.ignore_case=opt.ignore_case;
    if (opt.acceptregex_s == NULL) {opt_rpc.acceptregex_s.charptr.charptr_len = 0;opt_rpc.acceptregex_s.charptr.charptr_val = NULL;}
    else {opt_rpc.acceptregex_s.charptr.charptr_len = strlen(opt.acceptregex_s) + 1;

    	opt_rpc.acceptregex_s.charptr.charptr_val = malloc(opt_rpc.acceptregex_s.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.acceptregex_s.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.acceptregex_s.charptr.charptr_val, opt.acceptregex_s, opt_rpc.acceptregex_s.charptr.charptr_len);
    }
    if (opt.rejectregex_s == NULL) {opt_rpc.rejectregex_s.charptr.charptr_len = 0;opt_rpc.rejectregex_s.charptr.charptr_val = NULL;}
    else {opt_rpc.rejectregex_s.charptr.charptr_len = strlen(opt.rejectregex_s) + 1;

    	opt_rpc.rejectregex_s.charptr.charptr_val = malloc(opt_rpc.rejectregex_s.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.rejectregex_s.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.rejectregex_s.charptr.charptr_val, opt.rejectregex_s, opt_rpc.rejectregex_s.charptr.charptr_len);
    }
    if (opt.domains == NULL) {opt_rpc.domains.twocharptr.twocharptr_len = 0;opt_rpc.domains.twocharptr.twocharptr_val = NULL;}
    else {opt_rpc.domains.twocharptr.twocharptr_len = 1;opt_rpc.domains.twocharptr.twocharptr_val = malloc(opt_rpc.domains.twocharptr.twocharptr_len * sizeof(char_ptr));
    	if (opt_rpc.domains.twocharptr.twocharptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < opt_rpc.domains.twocharptr.twocharptr_len; i++) {
    		if (opt.domains[i] == NULL) {
    			opt_rpc.domains.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;
    			opt_rpc.domains.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;
    		} else {
    			opt_rpc.domains.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(opt.domains[i]) + 1;
    			opt_rpc.domains.twocharptr.twocharptr_val[i].charptr.charptr_val = opt.domains[i];
    		}
    	}
    }
    if (opt.exclude_domains == NULL) {opt_rpc.exclude_domains.twocharptr.twocharptr_len = 0;opt_rpc.exclude_domains.twocharptr.twocharptr_val = NULL;}
    else {opt_rpc.exclude_domains.twocharptr.twocharptr_len = 1;opt_rpc.exclude_domains.twocharptr.twocharptr_val = malloc(opt_rpc.exclude_domains.twocharptr.twocharptr_len * sizeof(char_ptr));
    	if (opt_rpc.exclude_domains.twocharptr.twocharptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < opt_rpc.exclude_domains.twocharptr.twocharptr_len; i++) {
    		if (opt.exclude_domains[i] == NULL) {
    			opt_rpc.exclude_domains.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;
    			opt_rpc.exclude_domains.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;
    		} else {
    			opt_rpc.exclude_domains.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(opt.exclude_domains[i]) + 1;
    			opt_rpc.exclude_domains.twocharptr.twocharptr_val[i].charptr.charptr_val = opt.exclude_domains[i];
    		}
    	}
    }
    opt_rpc.dns_cache=opt.dns_cache;
    if (opt.follow_tags == NULL) {opt_rpc.follow_tags.twocharptr.twocharptr_len = 0;opt_rpc.follow_tags.twocharptr.twocharptr_val = NULL;}
    else {opt_rpc.follow_tags.twocharptr.twocharptr_len = 1;opt_rpc.follow_tags.twocharptr.twocharptr_val = malloc(opt_rpc.follow_tags.twocharptr.twocharptr_len * sizeof(char_ptr));
    	if (opt_rpc.follow_tags.twocharptr.twocharptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < opt_rpc.follow_tags.twocharptr.twocharptr_len; i++) {
    		if (opt.follow_tags[i] == NULL) {
    			opt_rpc.follow_tags.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;
    			opt_rpc.follow_tags.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;
    		} else {
    			opt_rpc.follow_tags.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(opt.follow_tags[i]) + 1;
    			opt_rpc.follow_tags.twocharptr.twocharptr_val[i].charptr.charptr_val = opt.follow_tags[i];
    		}
    	}
    }
    if (opt.ignore_tags == NULL) {opt_rpc.ignore_tags.twocharptr.twocharptr_len = 0;opt_rpc.ignore_tags.twocharptr.twocharptr_val = NULL;}
    else {opt_rpc.ignore_tags.twocharptr.twocharptr_len = 1;opt_rpc.ignore_tags.twocharptr.twocharptr_val = malloc(opt_rpc.ignore_tags.twocharptr.twocharptr_len * sizeof(char_ptr));
    	if (opt_rpc.ignore_tags.twocharptr.twocharptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < opt_rpc.ignore_tags.twocharptr.twocharptr_len; i++) {
    		if (opt.ignore_tags[i] == NULL) {
    			opt_rpc.ignore_tags.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;
    			opt_rpc.ignore_tags.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;
    		} else {
    			opt_rpc.ignore_tags.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(opt.ignore_tags[i]) + 1;
    			opt_rpc.ignore_tags.twocharptr.twocharptr_val[i].charptr.charptr_val = opt.ignore_tags[i];
    		}
    	}
    }
    opt_rpc.follow_ftp=opt.follow_ftp;
    opt_rpc.retr_symlinks=opt.retr_symlinks;
    if (opt.output_document == NULL) {opt_rpc.output_document.charptr.charptr_len = 0;opt_rpc.output_document.charptr.charptr_val = NULL;}
    else {opt_rpc.output_document.charptr.charptr_len = strlen(opt.output_document) + 1;

    	opt_rpc.output_document.charptr.charptr_val = malloc(opt_rpc.output_document.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.output_document.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.output_document.charptr.charptr_val, opt.output_document, opt_rpc.output_document.charptr.charptr_len);
    }
    if (opt.warc_filename == NULL) {opt_rpc.warc_filename.charptr.charptr_len = 0;opt_rpc.warc_filename.charptr.charptr_val = NULL;}
    else {opt_rpc.warc_filename.charptr.charptr_len = strlen(opt.warc_filename) + 1;

    	opt_rpc.warc_filename.charptr.charptr_val = malloc(opt_rpc.warc_filename.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.warc_filename.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.warc_filename.charptr.charptr_val, opt.warc_filename, opt_rpc.warc_filename.charptr.charptr_len);
    }
    if (opt.warc_tempdir == NULL) {opt_rpc.warc_tempdir.charptr.charptr_len = 0;opt_rpc.warc_tempdir.charptr.charptr_val = NULL;}
    else {opt_rpc.warc_tempdir.charptr.charptr_len = strlen(opt.warc_tempdir) + 1;

    	opt_rpc.warc_tempdir.charptr.charptr_val = malloc(opt_rpc.warc_tempdir.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.warc_tempdir.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.warc_tempdir.charptr.charptr_val, opt.warc_tempdir, opt_rpc.warc_tempdir.charptr.charptr_len);
    }
    if (opt.warc_cdx_dedup_filename == NULL) {opt_rpc.warc_cdx_dedup_filename.charptr.charptr_len = 0;opt_rpc.warc_cdx_dedup_filename.charptr.charptr_val = NULL;}
    else {opt_rpc.warc_cdx_dedup_filename.charptr.charptr_len = strlen(opt.warc_cdx_dedup_filename) + 1;

    	opt_rpc.warc_cdx_dedup_filename.charptr.charptr_val = malloc(opt_rpc.warc_cdx_dedup_filename.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.warc_cdx_dedup_filename.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.warc_cdx_dedup_filename.charptr.charptr_val, opt.warc_cdx_dedup_filename, opt_rpc.warc_cdx_dedup_filename.charptr.charptr_len);
    }
    opt_rpc.warc_maxsize=opt.warc_maxsize;
    opt_rpc.warc_compression_enabled=opt.warc_compression_enabled;
    opt_rpc.warc_digests_enabled=opt.warc_digests_enabled;
    opt_rpc.warc_cdx_enabled=opt.warc_cdx_enabled;
    opt_rpc.warc_keep_log=opt.warc_keep_log;
    if (opt.warc_user_headers == NULL) {opt_rpc.warc_user_headers.twocharptr.twocharptr_len = 0;opt_rpc.warc_user_headers.twocharptr.twocharptr_val = NULL;}
    else {opt_rpc.warc_user_headers.twocharptr.twocharptr_len = 1;opt_rpc.warc_user_headers.twocharptr.twocharptr_val = malloc(opt_rpc.warc_user_headers.twocharptr.twocharptr_len * sizeof(char_ptr));
    	if (opt_rpc.warc_user_headers.twocharptr.twocharptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < opt_rpc.warc_user_headers.twocharptr.twocharptr_len; i++) {
    		if (opt.warc_user_headers[i] == NULL) {
    			opt_rpc.warc_user_headers.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;
    			opt_rpc.warc_user_headers.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;
    		} else {
    			opt_rpc.warc_user_headers.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(opt.warc_user_headers[i]) + 1;
    			opt_rpc.warc_user_headers.twocharptr.twocharptr_val[i].charptr.charptr_val = opt.warc_user_headers[i];
    		}
    	}
    }
    if (opt.user == NULL) {opt_rpc.user.charptr.charptr_len = 0;opt_rpc.user.charptr.charptr_val = NULL;}
    else {opt_rpc.user.charptr.charptr_len = strlen(opt.user) + 1;

    	opt_rpc.user.charptr.charptr_val = malloc(opt_rpc.user.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.user.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.user.charptr.charptr_val, opt.user, opt_rpc.user.charptr.charptr_len);
    }
    if (opt.passwd == NULL) {opt_rpc.passwd.charptr.charptr_len = 0;opt_rpc.passwd.charptr.charptr_val = NULL;}
    else {opt_rpc.passwd.charptr.charptr_len = strlen(opt.passwd) + 1;

    	opt_rpc.passwd.charptr.charptr_val = malloc(opt_rpc.passwd.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.passwd.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.passwd.charptr.charptr_val, opt.passwd, opt_rpc.passwd.charptr.charptr_len);
    }
    opt_rpc.ask_passwd=opt.ask_passwd;
    opt_rpc.always_rest=opt.always_rest;
    opt_rpc.start_pos=opt.start_pos;
    if (opt.ftp_user == NULL) {opt_rpc.ftp_user.charptr.charptr_len = 0;opt_rpc.ftp_user.charptr.charptr_val = NULL;}
    else {opt_rpc.ftp_user.charptr.charptr_len = strlen(opt.ftp_user) + 1;

    	opt_rpc.ftp_user.charptr.charptr_val = malloc(opt_rpc.ftp_user.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.ftp_user.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.ftp_user.charptr.charptr_val, opt.ftp_user, opt_rpc.ftp_user.charptr.charptr_len);
    }
    if (opt.ftp_passwd == NULL) {opt_rpc.ftp_passwd.charptr.charptr_len = 0;opt_rpc.ftp_passwd.charptr.charptr_val = NULL;}
    else {opt_rpc.ftp_passwd.charptr.charptr_len = strlen(opt.ftp_passwd) + 1;

    	opt_rpc.ftp_passwd.charptr.charptr_val = malloc(opt_rpc.ftp_passwd.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.ftp_passwd.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.ftp_passwd.charptr.charptr_val, opt.ftp_passwd, opt_rpc.ftp_passwd.charptr.charptr_len);
    }
    opt_rpc.netrc=opt.netrc;
    opt_rpc.ftp_glob=opt.ftp_glob;
    opt_rpc.ftp_pasv=opt.ftp_pasv;
    if (opt.http_user == NULL) {opt_rpc.http_user.charptr.charptr_len = 0;opt_rpc.http_user.charptr.charptr_val = NULL;}
    else {opt_rpc.http_user.charptr.charptr_len = strlen(opt.http_user) + 1;

    	opt_rpc.http_user.charptr.charptr_val = malloc(opt_rpc.http_user.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.http_user.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.http_user.charptr.charptr_val, opt.http_user, opt_rpc.http_user.charptr.charptr_len);
    }
    if (opt.http_passwd == NULL) {opt_rpc.http_passwd.charptr.charptr_len = 0;opt_rpc.http_passwd.charptr.charptr_val = NULL;}
    else {opt_rpc.http_passwd.charptr.charptr_len = strlen(opt.http_passwd) + 1;

    	opt_rpc.http_passwd.charptr.charptr_val = malloc(opt_rpc.http_passwd.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.http_passwd.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.http_passwd.charptr.charptr_val, opt.http_passwd, opt_rpc.http_passwd.charptr.charptr_len);
    }
    if (opt.user_headers == NULL) {opt_rpc.user_headers.twocharptr.twocharptr_len = 0;opt_rpc.user_headers.twocharptr.twocharptr_val = NULL;}
    else {opt_rpc.user_headers.twocharptr.twocharptr_len = 1;opt_rpc.user_headers.twocharptr.twocharptr_val = malloc(opt_rpc.user_headers.twocharptr.twocharptr_len * sizeof(char_ptr));
    	if (opt_rpc.user_headers.twocharptr.twocharptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < opt_rpc.user_headers.twocharptr.twocharptr_len; i++) {
    		if (opt.user_headers[i] == NULL) {
    			opt_rpc.user_headers.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;
    			opt_rpc.user_headers.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;
    		} else {
    			opt_rpc.user_headers.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(opt.user_headers[i]) + 1;
    			opt_rpc.user_headers.twocharptr.twocharptr_val[i].charptr.charptr_val = opt.user_headers[i];
    		}
    	}
    }
    opt_rpc.http_keep_alive=opt.http_keep_alive;
    opt_rpc.use_proxy=opt.use_proxy;
    opt_rpc.allow_cache=opt.allow_cache;
    if (opt.http_proxy == NULL) {opt_rpc.http_proxy.charptr.charptr_len = 0;opt_rpc.http_proxy.charptr.charptr_val = NULL;}
    else {opt_rpc.http_proxy.charptr.charptr_len = strlen(opt.http_proxy) + 1;

    	opt_rpc.http_proxy.charptr.charptr_val = malloc(opt_rpc.http_proxy.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.http_proxy.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.http_proxy.charptr.charptr_val, opt.http_proxy, opt_rpc.http_proxy.charptr.charptr_len);
    }
    if (opt.ftp_proxy == NULL) {opt_rpc.ftp_proxy.charptr.charptr_len = 0;opt_rpc.ftp_proxy.charptr.charptr_val = NULL;}
    else {opt_rpc.ftp_proxy.charptr.charptr_len = strlen(opt.ftp_proxy) + 1;

    	opt_rpc.ftp_proxy.charptr.charptr_val = malloc(opt_rpc.ftp_proxy.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.ftp_proxy.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.ftp_proxy.charptr.charptr_val, opt.ftp_proxy, opt_rpc.ftp_proxy.charptr.charptr_len);
    }
    if (opt.https_proxy == NULL) {opt_rpc.https_proxy.charptr.charptr_len = 0;opt_rpc.https_proxy.charptr.charptr_val = NULL;}
    else {opt_rpc.https_proxy.charptr.charptr_len = strlen(opt.https_proxy) + 1;

    	opt_rpc.https_proxy.charptr.charptr_val = malloc(opt_rpc.https_proxy.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.https_proxy.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.https_proxy.charptr.charptr_val, opt.https_proxy, opt_rpc.https_proxy.charptr.charptr_len);
    }
    if (opt.no_proxy == NULL) {opt_rpc.no_proxy.twocharptr.twocharptr_len = 0;opt_rpc.no_proxy.twocharptr.twocharptr_val = NULL;}
    else {opt_rpc.no_proxy.twocharptr.twocharptr_len = 1;opt_rpc.no_proxy.twocharptr.twocharptr_val = malloc(opt_rpc.no_proxy.twocharptr.twocharptr_len * sizeof(char_ptr));
    	if (opt_rpc.no_proxy.twocharptr.twocharptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i = 0; i < opt_rpc.no_proxy.twocharptr.twocharptr_len; i++) {
    		if (opt.no_proxy[i] == NULL) {
    			opt_rpc.no_proxy.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;
    			opt_rpc.no_proxy.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;
    		} else {
    			opt_rpc.no_proxy.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(opt.no_proxy[i]) + 1;
    			opt_rpc.no_proxy.twocharptr.twocharptr_val[i].charptr.charptr_val = opt.no_proxy[i];
    		}
    	}
    }
    if (opt.base_href == NULL) {opt_rpc.base_href.charptr.charptr_len = 0;opt_rpc.base_href.charptr.charptr_val = NULL;}
    else {opt_rpc.base_href.charptr.charptr_len = strlen(opt.base_href) + 1;

    	opt_rpc.base_href.charptr.charptr_val = malloc(opt_rpc.base_href.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.base_href.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.base_href.charptr.charptr_val, opt.base_href, opt_rpc.base_href.charptr.charptr_len);
    }
    if (opt.progress_type == NULL) {opt_rpc.progress_type.charptr.charptr_len = 0;opt_rpc.progress_type.charptr.charptr_val = NULL;}
    else {opt_rpc.progress_type.charptr.charptr_len = strlen(opt.progress_type) + 1;

    	opt_rpc.progress_type.charptr.charptr_val = malloc(opt_rpc.progress_type.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.progress_type.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.progress_type.charptr.charptr_val, opt.progress_type, opt_rpc.progress_type.charptr.charptr_len);
    }
    opt_rpc.show_progress=opt.show_progress;
    opt_rpc.noscroll=opt.noscroll;
    if (opt.proxy_user == NULL) {opt_rpc.proxy_user.charptr.charptr_len = 0;opt_rpc.proxy_user.charptr.charptr_val = NULL;}
    else {opt_rpc.proxy_user.charptr.charptr_len = strlen(opt.proxy_user) + 1;

    	opt_rpc.proxy_user.charptr.charptr_val = malloc(opt_rpc.proxy_user.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.proxy_user.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.proxy_user.charptr.charptr_val, opt.proxy_user, opt_rpc.proxy_user.charptr.charptr_len);
    }
    if (opt.proxy_passwd == NULL) {opt_rpc.proxy_passwd.charptr.charptr_len = 0;opt_rpc.proxy_passwd.charptr.charptr_val = NULL;}
    else {opt_rpc.proxy_passwd.charptr.charptr_len = strlen(opt.proxy_passwd) + 1;

    	opt_rpc.proxy_passwd.charptr.charptr_val = malloc(opt_rpc.proxy_passwd.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.proxy_passwd.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.proxy_passwd.charptr.charptr_val, opt.proxy_passwd, opt_rpc.proxy_passwd.charptr.charptr_len);
    }
    opt_rpc.read_timeout=opt.read_timeout;
    opt_rpc.dns_timeout=opt.dns_timeout;
    opt_rpc.connect_timeout=opt.connect_timeout;
    opt_rpc.random_wait=opt.random_wait;
    opt_rpc.wait=opt.wait;
    opt_rpc.waitretry=opt.waitretry;
    opt_rpc.use_robots=opt.use_robots;
    opt_rpc.limit_rate=opt.limit_rate;
    opt_rpc.server_response=opt.server_response;
    opt_rpc.save_headers=opt.save_headers;
    opt_rpc.content_on_error=opt.content_on_error;
    opt_rpc.debug=opt.debug;
    opt_rpc.timestamping=opt.timestamping;
    opt_rpc.if_modified_since=opt.if_modified_since;
    opt_rpc.backup_converted=opt.backup_converted;
    opt_rpc.backups=opt.backups;
    if (opt.useragent == NULL) {opt_rpc.useragent.charptr.charptr_len = 0;opt_rpc.useragent.charptr.charptr_val = NULL;}
    else {opt_rpc.useragent.charptr.charptr_len = strlen(opt.useragent) + 1;

    	opt_rpc.useragent.charptr.charptr_val = malloc(opt_rpc.useragent.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.useragent.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.useragent.charptr.charptr_val, opt.useragent, opt_rpc.useragent.charptr.charptr_len);
    }
    if (opt.referer == NULL) {opt_rpc.referer.charptr.charptr_len = 0;opt_rpc.referer.charptr.charptr_val = NULL;}
    else {opt_rpc.referer.charptr.charptr_len = strlen(opt.referer) + 1;

    	opt_rpc.referer.charptr.charptr_val = malloc(opt_rpc.referer.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.referer.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.referer.charptr.charptr_val, opt.referer, opt_rpc.referer.charptr.charptr_len);
    }
    opt_rpc.convert_links=opt.convert_links;
    opt_rpc.convert_file_only=opt.convert_file_only;
    opt_rpc.remove_listing=opt.remove_listing;
    opt_rpc.htmlify=opt.htmlify;
    if (opt.dot_style == NULL) {opt_rpc.dot_style.charptr.charptr_len = 0;opt_rpc.dot_style.charptr.charptr_val = NULL;}
    else {opt_rpc.dot_style.charptr.charptr_len = strlen(opt.dot_style) + 1;

    	opt_rpc.dot_style.charptr.charptr_val = malloc(opt_rpc.dot_style.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.dot_style.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.dot_style.charptr.charptr_val, opt.dot_style, opt_rpc.dot_style.charptr.charptr_len);
    }
    opt_rpc.dot_bytes=opt.dot_bytes;
    opt_rpc.dots_in_line=opt.dots_in_line;
    opt_rpc.dot_spacing=opt.dot_spacing;
    opt_rpc.delete_after=opt.delete_after;
    opt_rpc.adjust_extension=opt.adjust_extension;
    opt_rpc.page_requisites=opt.page_requisites;
    if (opt.bind_address == NULL) {opt_rpc.bind_address.charptr.charptr_len = 0;opt_rpc.bind_address.charptr.charptr_val = NULL;}
    else {opt_rpc.bind_address.charptr.charptr_len = strlen(opt.bind_address) + 1;

    	opt_rpc.bind_address.charptr.charptr_val = malloc(opt_rpc.bind_address.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.bind_address.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.bind_address.charptr.charptr_val, opt.bind_address, opt_rpc.bind_address.charptr.charptr_len);
    }
    opt_rpc.check_cert=opt.check_cert;
    if (opt.cert_file == NULL) {opt_rpc.cert_file.charptr.charptr_len = 0;opt_rpc.cert_file.charptr.charptr_val = NULL;}
    else {opt_rpc.cert_file.charptr.charptr_len = strlen(opt.cert_file) + 1;

    	opt_rpc.cert_file.charptr.charptr_val = malloc(opt_rpc.cert_file.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.cert_file.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.cert_file.charptr.charptr_val, opt.cert_file, opt_rpc.cert_file.charptr.charptr_len);
    }
    if (opt.private_key == NULL) {opt_rpc.private_key.charptr.charptr_len = 0;opt_rpc.private_key.charptr.charptr_val = NULL;}
    else {opt_rpc.private_key.charptr.charptr_len = strlen(opt.private_key) + 1;

    	opt_rpc.private_key.charptr.charptr_val = malloc(opt_rpc.private_key.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.private_key.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.private_key.charptr.charptr_val, opt.private_key, opt_rpc.private_key.charptr.charptr_len);
    }
    opt_rpc.private_key_type=opt.private_key_type;
    if (opt.ca_directory == NULL) {opt_rpc.ca_directory.charptr.charptr_len = 0;opt_rpc.ca_directory.charptr.charptr_val = NULL;}
    else {opt_rpc.ca_directory.charptr.charptr_len = strlen(opt.ca_directory) + 1;

    	opt_rpc.ca_directory.charptr.charptr_val = malloc(opt_rpc.ca_directory.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.ca_directory.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.ca_directory.charptr.charptr_val, opt.ca_directory, opt_rpc.ca_directory.charptr.charptr_len);
    }
    if (opt.ca_cert == NULL) {opt_rpc.ca_cert.charptr.charptr_len = 0;opt_rpc.ca_cert.charptr.charptr_val = NULL;}
    else {opt_rpc.ca_cert.charptr.charptr_len = strlen(opt.ca_cert) + 1;

    	opt_rpc.ca_cert.charptr.charptr_val = malloc(opt_rpc.ca_cert.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.ca_cert.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.ca_cert.charptr.charptr_val, opt.ca_cert, opt_rpc.ca_cert.charptr.charptr_len);
    }
    if (opt.crl_file == NULL) {opt_rpc.crl_file.charptr.charptr_len = 0;opt_rpc.crl_file.charptr.charptr_val = NULL;}
    else {opt_rpc.crl_file.charptr.charptr_len = strlen(opt.crl_file) + 1;

    	opt_rpc.crl_file.charptr.charptr_val = malloc(opt_rpc.crl_file.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.crl_file.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.crl_file.charptr.charptr_val, opt.crl_file, opt_rpc.crl_file.charptr.charptr_len);
    }
    if (opt.pinnedpubkey == NULL) {opt_rpc.pinnedpubkey.charptr.charptr_len = 0;opt_rpc.pinnedpubkey.charptr.charptr_val = NULL;}
    else {opt_rpc.pinnedpubkey.charptr.charptr_len = strlen(opt.pinnedpubkey) + 1;

    	opt_rpc.pinnedpubkey.charptr.charptr_val = malloc(opt_rpc.pinnedpubkey.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.pinnedpubkey.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.pinnedpubkey.charptr.charptr_val, opt.pinnedpubkey, opt_rpc.pinnedpubkey.charptr.charptr_len);
    }
    if (opt.random_file == NULL) {opt_rpc.random_file.charptr.charptr_len = 0;opt_rpc.random_file.charptr.charptr_val = NULL;}
    else {opt_rpc.random_file.charptr.charptr_len = strlen(opt.random_file) + 1;

    	opt_rpc.random_file.charptr.charptr_val = malloc(opt_rpc.random_file.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.random_file.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.random_file.charptr.charptr_val, opt.random_file, opt_rpc.random_file.charptr.charptr_len);
    }
    if (opt.egd_file == NULL) {opt_rpc.egd_file.charptr.charptr_len = 0;opt_rpc.egd_file.charptr.charptr_val = NULL;}
    else {opt_rpc.egd_file.charptr.charptr_len = strlen(opt.egd_file) + 1;

    	opt_rpc.egd_file.charptr.charptr_val = malloc(opt_rpc.egd_file.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.egd_file.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.egd_file.charptr.charptr_val, opt.egd_file, opt_rpc.egd_file.charptr.charptr_len);
    }
    opt_rpc.https_only=opt.https_only;
    opt_rpc.ftps_resume_ssl=opt.ftps_resume_ssl;
    opt_rpc.ftps_fallback_to_ftp=opt.ftps_fallback_to_ftp;
    opt_rpc.ftps_implicit=opt.ftps_implicit;
    opt_rpc.ftps_clear_data_connection=opt.ftps_clear_data_connection;
    opt_rpc.cookies=opt.cookies;
    if (opt.cookies_input == NULL) {opt_rpc.cookies_input.charptr.charptr_len = 0;opt_rpc.cookies_input.charptr.charptr_val = NULL;}
    else {opt_rpc.cookies_input.charptr.charptr_len = strlen(opt.cookies_input) + 1;

    	opt_rpc.cookies_input.charptr.charptr_val = malloc(opt_rpc.cookies_input.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.cookies_input.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.cookies_input.charptr.charptr_val, opt.cookies_input, opt_rpc.cookies_input.charptr.charptr_len);
    }
    if (opt.cookies_output == NULL) {opt_rpc.cookies_output.charptr.charptr_len = 0;opt_rpc.cookies_output.charptr.charptr_val = NULL;}
    else {opt_rpc.cookies_output.charptr.charptr_len = strlen(opt.cookies_output) + 1;

    	opt_rpc.cookies_output.charptr.charptr_val = malloc(opt_rpc.cookies_output.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.cookies_output.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.cookies_output.charptr.charptr_val, opt.cookies_output, opt_rpc.cookies_output.charptr.charptr_len);
    }
    opt_rpc.keep_session_cookies=opt.keep_session_cookies;
    if (opt.post_data == NULL) {opt_rpc.post_data.charptr.charptr_len = 0;opt_rpc.post_data.charptr.charptr_val = NULL;}
    else {opt_rpc.post_data.charptr.charptr_len = strlen(opt.post_data) + 1;

    	opt_rpc.post_data.charptr.charptr_val = malloc(opt_rpc.post_data.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.post_data.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.post_data.charptr.charptr_val, opt.post_data, opt_rpc.post_data.charptr.charptr_len);
    }
    if (opt.post_file_name == NULL) {opt_rpc.post_file_name.charptr.charptr_len = 0;opt_rpc.post_file_name.charptr.charptr_val = NULL;}
    else {opt_rpc.post_file_name.charptr.charptr_len = strlen(opt.post_file_name) + 1;

    	opt_rpc.post_file_name.charptr.charptr_val = malloc(opt_rpc.post_file_name.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.post_file_name.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.post_file_name.charptr.charptr_val, opt.post_file_name, opt_rpc.post_file_name.charptr.charptr_len);
    }
    if (opt.method == NULL) {opt_rpc.method.charptr.charptr_len = 0;opt_rpc.method.charptr.charptr_val = NULL;}
    else {opt_rpc.method.charptr.charptr_len = strlen(opt.method) + 1;

    	opt_rpc.method.charptr.charptr_val = malloc(opt_rpc.method.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.method.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.method.charptr.charptr_val, opt.method, opt_rpc.method.charptr.charptr_len);
    }
    if (opt.body_data == NULL) {opt_rpc.body_data.charptr.charptr_len = 0;opt_rpc.body_data.charptr.charptr_val = NULL;}
    else {opt_rpc.body_data.charptr.charptr_len = strlen(opt.body_data) + 1;

    	opt_rpc.body_data.charptr.charptr_val = malloc(opt_rpc.body_data.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.body_data.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.body_data.charptr.charptr_val, opt.body_data, opt_rpc.body_data.charptr.charptr_len);
    }
    if (opt.body_file == NULL) {opt_rpc.body_file.charptr.charptr_len = 0;opt_rpc.body_file.charptr.charptr_val = NULL;}
    else {opt_rpc.body_file.charptr.charptr_len = strlen(opt.body_file) + 1;

    	opt_rpc.body_file.charptr.charptr_val = malloc(opt_rpc.body_file.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.body_file.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.body_file.charptr.charptr_val, opt.body_file, opt_rpc.body_file.charptr.charptr_len);
    }
    opt_rpc.restrict_files_ctrl=opt.restrict_files_ctrl;
    opt_rpc.restrict_files_nonascii=opt.restrict_files_nonascii;
    opt_rpc.strict_comments=opt.strict_comments;
    opt_rpc.preserve_perm=opt.preserve_perm;
    opt_rpc.ipv4_only=opt.ipv4_only;
    opt_rpc.ipv6_only=opt.ipv6_only;
    opt_rpc.content_disposition=opt.content_disposition;
    opt_rpc.auth_without_challenge=opt.auth_without_challenge;
    opt_rpc.enable_iri=opt.enable_iri;
    if (opt.encoding_remote == NULL) {opt_rpc.encoding_remote.charptr.charptr_len = 0;opt_rpc.encoding_remote.charptr.charptr_val = NULL;}
    else {opt_rpc.encoding_remote.charptr.charptr_len = strlen(opt.encoding_remote) + 1;

    	opt_rpc.encoding_remote.charptr.charptr_val = malloc(opt_rpc.encoding_remote.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.encoding_remote.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.encoding_remote.charptr.charptr_val, opt.encoding_remote, opt_rpc.encoding_remote.charptr.charptr_len);
    }
    if (opt.locale == NULL) {opt_rpc.locale.charptr.charptr_len = 0;opt_rpc.locale.charptr.charptr_val = NULL;}
    else {opt_rpc.locale.charptr.charptr_len = strlen(opt.locale) + 1;

    	opt_rpc.locale.charptr.charptr_val = malloc(opt_rpc.locale.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.locale.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.locale.charptr.charptr_val, opt.locale, opt_rpc.locale.charptr.charptr_len);
    }
    opt_rpc.trustservernames=opt.trustservernames;
    opt_rpc.useservertimestamps=opt.useservertimestamps;
    opt_rpc.show_all_dns_entries=opt.show_all_dns_entries;
    opt_rpc.report_bps=opt.report_bps;
    if (opt.rejected_log == NULL) {opt_rpc.rejected_log.charptr.charptr_len = 0;opt_rpc.rejected_log.charptr.charptr_val = NULL;}
    else {opt_rpc.rejected_log.charptr.charptr_len = strlen(opt.rejected_log) + 1;

    	opt_rpc.rejected_log.charptr.charptr_val = malloc(opt_rpc.rejected_log.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.rejected_log.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.rejected_log.charptr.charptr_val, opt.rejected_log, opt_rpc.rejected_log.charptr.charptr_len);
    }
    opt_rpc.hsts=opt.hsts;
    if (opt.hsts_file == NULL) {opt_rpc.hsts_file.charptr.charptr_len = 0;opt_rpc.hsts_file.charptr.charptr_val = NULL;}
    else {opt_rpc.hsts_file.charptr.charptr_len = strlen(opt.hsts_file) + 1;

    	opt_rpc.hsts_file.charptr.charptr_val = malloc(opt_rpc.hsts_file.charptr.charptr_len * sizeof(char));
    	if (opt_rpc.hsts_file.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(opt_rpc.hsts_file.charptr.charptr_val, opt.hsts_file, opt_rpc.hsts_file.charptr.charptr_len);
    }
    result = set_opt_1(opt_rpc, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

int get_opt_verbose_wrapper()
{
    opt_verbose_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_opt_verbose_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_verbose_wrapper(int value)
{
    void *result;
    result = set_opt_verbose_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_quiet_wrapper()
{
    opt_quiet_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_quiet_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_quiet_wrapper(bool value)
{
    void *result;
    result = set_opt_quiet_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_opt_ntry_wrapper()
{
    opt_ntry_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_opt_ntry_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_ntry_wrapper(int value)
{
    void *result;
    result = set_opt_ntry_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_retry_connrefused_wrapper()
{
    opt_retry_connrefused_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_retry_connrefused_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_retry_connrefused_wrapper(bool value)
{
    void *result;
    result = set_opt_retry_connrefused_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_background_wrapper()
{
    opt_background_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_background_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_background_wrapper(bool value)
{
    void *result;
    result = set_opt_background_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_ignore_length_wrapper()
{
    opt_ignore_length_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_ignore_length_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_ignore_length_wrapper(bool value)
{
    void *result;
    result = set_opt_ignore_length_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_recursive_wrapper()
{
    opt_recursive_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_recursive_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_recursive_wrapper(bool value)
{
    void *result;
    result = set_opt_recursive_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_spanhost_wrapper()
{
    opt_spanhost_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_spanhost_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_spanhost_wrapper(bool value)
{
    void *result;
    result = set_opt_spanhost_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_opt_max_redirect_wrapper()
{
    opt_max_redirect_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_opt_max_redirect_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_max_redirect_wrapper(int value)
{
    void *result;
    result = set_opt_max_redirect_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_relative_only_wrapper()
{
    opt_relative_only_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_relative_only_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_relative_only_wrapper(bool value)
{
    void *result;
    result = set_opt_relative_only_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_no_parent_wrapper()
{
    opt_no_parent_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_no_parent_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_no_parent_wrapper(bool value)
{
    void *result;
    result = set_opt_no_parent_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_opt_reclevel_wrapper()
{
    opt_reclevel_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_opt_reclevel_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_reclevel_wrapper(int value)
{
    void *result;
    result = set_opt_reclevel_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_dirstruct_wrapper()
{
    opt_dirstruct_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_dirstruct_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_dirstruct_wrapper(bool value)
{
    void *result;
    result = set_opt_dirstruct_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_no_dirstruct_wrapper()
{
    opt_no_dirstruct_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_no_dirstruct_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_no_dirstruct_wrapper(bool value)
{
    void *result;
    result = set_opt_no_dirstruct_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_opt_cut_dirs_wrapper()
{
    opt_cut_dirs_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_opt_cut_dirs_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_cut_dirs_wrapper(int value)
{
    void *result;
    result = set_opt_cut_dirs_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_add_hostdir_wrapper()
{
    opt_add_hostdir_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_add_hostdir_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_add_hostdir_wrapper(bool value)
{
    void *result;
    result = set_opt_add_hostdir_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_protocol_directories_wrapper()
{
    opt_protocol_directories_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_protocol_directories_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_protocol_directories_wrapper(bool value)
{
    void *result;
    result = set_opt_protocol_directories_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_noclobber_wrapper()
{
    opt_noclobber_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_noclobber_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_noclobber_wrapper(bool value)
{
    void *result;
    result = set_opt_noclobber_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_unlink_wrapper()
{
    opt_unlink_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_unlink_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_unlink_wrapper(bool value)
{
    void *result;
    result = set_opt_unlink_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_dir_prefix_wrapper()
{
    opt_dir_prefix_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_dir_prefix_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_dir_prefix_wrapper(const char *value)
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
    result = set_opt_dir_prefix_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_lfilename_wrapper()
{
    opt_lfilename_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_lfilename_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_lfilename_wrapper(const char *value)
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
    result = set_opt_lfilename_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_input_filename_wrapper()
{
    opt_input_filename_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_input_filename_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_input_filename_wrapper(const char *value)
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
    result = set_opt_input_filename_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_choose_config_wrapper()
{
    opt_choose_config_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_choose_config_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_choose_config_wrapper(const char *value)
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
    result = set_opt_choose_config_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_noconfig_wrapper()
{
    opt_noconfig_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_noconfig_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_noconfig_wrapper(bool value)
{
    void *result;
    result = set_opt_noconfig_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_force_html_wrapper()
{
    opt_force_html_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_force_html_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_force_html_wrapper(bool value)
{
    void *result;
    result = set_opt_force_html_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_default_page_wrapper()
{
    opt_default_page_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_default_page_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_default_page_wrapper(const char *value)
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
    result = set_opt_default_page_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_spider_wrapper()
{
    opt_spider_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_spider_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_spider_wrapper(bool value)
{
    void *result;
    result = set_opt_spider_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char ** get_opt_accepts_wrapper()
{
    opt_accepts_ret_t *result;
    char ** ret_val = NULL;
    result = get_opt_accepts_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.twocharptr.twocharptr_val != NULL) {
        int length = result->value.twocharptr.twocharptr_len;
        ret_val = (char **)malloc((length + 1) * sizeof(char *));
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        for (int i = 0; i < length; i++) {
            if (result->value.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {
                ret_val[i] = NULL;
            } else {
                ret_val[i] = malloc(result->value.twocharptr.twocharptr_val[i].charptr.charptr_len);
                if (ret_val[i] == NULL) {
                    perror("malloc failed");
                    // 释放已分配的内存
                    for (int j = 0; j < i; j++) {
                        if (ret_val[j] != NULL) free(ret_val[j]);
                    }
                    free(ret_val);
                    return NULL;
                }
                memcpy(ret_val[i], result->value.twocharptr.twocharptr_val[i].charptr.charptr_val, 
                       result->value.twocharptr.twocharptr_val[i].charptr.charptr_len);
            }
        }
        ret_val[length] = NULL; // 确保以NULL结尾
    }
    return ret_val;
}

int set_opt_accepts_wrapper(char **value, int length)
{
    void *result;
    TwoCharPtr rpc_value;
    if (value == NULL || length <= 0) {
        rpc_value.twocharptr.twocharptr_len = 0;
        rpc_value.twocharptr.twocharptr_val = NULL;
    } else {
        rpc_value.twocharptr.twocharptr_len = length;
        rpc_value.twocharptr.twocharptr_val = malloc(length * sizeof(char_ptr));
        if (rpc_value.twocharptr.twocharptr_val == NULL) {
            perror("malloc failed");
            return 0;
        }
        for (int i = 0; i < length; i++) {
            if (value[i] == NULL) {
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;
            } else {
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(value[i]) + 1;
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_val = value[i];
            }
        }
    }
    result = set_opt_accepts_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        if (rpc_value.twocharptr.twocharptr_val != NULL) {
            free(rpc_value.twocharptr.twocharptr_val);
        }
        return 0;
    }
    if (rpc_value.twocharptr.twocharptr_val != NULL) {
        free(rpc_value.twocharptr.twocharptr_val);
    }
    return 1;
}

char ** get_opt_rejects_wrapper()
{
    opt_rejects_ret_t *result;
    char ** ret_val = NULL;
    result = get_opt_rejects_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.twocharptr.twocharptr_val != NULL) {
        int length = result->value.twocharptr.twocharptr_len;
        ret_val = (char **)malloc((length + 1) * sizeof(char *));
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        for (int i = 0; i < length; i++) {
            if (result->value.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {
                ret_val[i] = NULL;
            } else {
                ret_val[i] = malloc(result->value.twocharptr.twocharptr_val[i].charptr.charptr_len);
                if (ret_val[i] == NULL) {
                    perror("malloc failed");
                    // 释放已分配的内存
                    for (int j = 0; j < i; j++) {
                        if (ret_val[j] != NULL) free(ret_val[j]);
                    }
                    free(ret_val);
                    return NULL;
                }
                memcpy(ret_val[i], result->value.twocharptr.twocharptr_val[i].charptr.charptr_val, 
                       result->value.twocharptr.twocharptr_val[i].charptr.charptr_len);
            }
        }
        ret_val[length] = NULL; // 确保以NULL结尾
    }
    return ret_val;
}

int set_opt_rejects_wrapper(char **value, int length)
{
    void *result;
    TwoCharPtr rpc_value;
    if (value == NULL || length <= 0) {
        rpc_value.twocharptr.twocharptr_len = 0;
        rpc_value.twocharptr.twocharptr_val = NULL;
    } else {
        rpc_value.twocharptr.twocharptr_len = length;
        rpc_value.twocharptr.twocharptr_val = malloc(length * sizeof(char_ptr));
        if (rpc_value.twocharptr.twocharptr_val == NULL) {
            perror("malloc failed");
            return 0;
        }
        for (int i = 0; i < length; i++) {
            if (value[i] == NULL) {
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;
            } else {
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(value[i]) + 1;
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_val = value[i];
            }
        }
    }
    result = set_opt_rejects_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        if (rpc_value.twocharptr.twocharptr_val != NULL) {
            free(rpc_value.twocharptr.twocharptr_val);
        }
        return 0;
    }
    if (rpc_value.twocharptr.twocharptr_val != NULL) {
        free(rpc_value.twocharptr.twocharptr_val);
    }
    return 1;
}

const char ** get_opt_excludes_wrapper()
{
    opt_excludes_ret_t *result;
    const char ** ret_val = NULL;
    result = get_opt_excludes_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.twocharptr.twocharptr_val != NULL) {
        int length = result->value.twocharptr.twocharptr_len;
        ret_val = (char **)malloc((length + 1) * sizeof(char *));
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        for (int i = 0; i < length; i++) {
            if (result->value.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {
                ret_val[i] = NULL;
            } else {
                ret_val[i] = malloc(result->value.twocharptr.twocharptr_val[i].charptr.charptr_len);
                if (ret_val[i] == NULL) {
                    perror("malloc failed");
                    // 释放已分配的内存
                    for (int j = 0; j < i; j++) {
                        if (ret_val[j] != NULL) free(ret_val[j]);
                    }
                    free(ret_val);
                    return NULL;
                }
                memcpy(ret_val[i], result->value.twocharptr.twocharptr_val[i].charptr.charptr_val, 
                       result->value.twocharptr.twocharptr_val[i].charptr.charptr_len);
            }
        }
        ret_val[length] = NULL; // 确保以NULL结尾
    }
    return ret_val;
}

int set_opt_excludes_wrapper(char **value, int length)
{
    void *result;
    TwoCharPtr rpc_value;
    if (value == NULL || length <= 0) {
        rpc_value.twocharptr.twocharptr_len = 0;
        rpc_value.twocharptr.twocharptr_val = NULL;
    } else {
        rpc_value.twocharptr.twocharptr_len = length;
        rpc_value.twocharptr.twocharptr_val = malloc(length * sizeof(char_ptr));
        if (rpc_value.twocharptr.twocharptr_val == NULL) {
            perror("malloc failed");
            return 0;
        }
        for (int i = 0; i < length; i++) {
            if (value[i] == NULL) {
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;
            } else {
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(value[i]) + 1;
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_val = value[i];
            }
        }
    }
    result = set_opt_excludes_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        if (rpc_value.twocharptr.twocharptr_val != NULL) {
            free(rpc_value.twocharptr.twocharptr_val);
        }
        return 0;
    }
    if (rpc_value.twocharptr.twocharptr_val != NULL) {
        free(rpc_value.twocharptr.twocharptr_val);
    }
    return 1;
}

const char ** get_opt_includes_wrapper()
{
    opt_includes_ret_t *result;
    const char ** ret_val = NULL;
    result = get_opt_includes_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.twocharptr.twocharptr_val != NULL) {
        int length = result->value.twocharptr.twocharptr_len;
        ret_val = (char **)malloc((length + 1) * sizeof(char *));
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        for (int i = 0; i < length; i++) {
            if (result->value.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {
                ret_val[i] = NULL;
            } else {
                ret_val[i] = malloc(result->value.twocharptr.twocharptr_val[i].charptr.charptr_len);
                if (ret_val[i] == NULL) {
                    perror("malloc failed");
                    // 释放已分配的内存
                    for (int j = 0; j < i; j++) {
                        if (ret_val[j] != NULL) free(ret_val[j]);
                    }
                    free(ret_val);
                    return NULL;
                }
                memcpy(ret_val[i], result->value.twocharptr.twocharptr_val[i].charptr.charptr_val, 
                       result->value.twocharptr.twocharptr_val[i].charptr.charptr_len);
            }
        }
        ret_val[length] = NULL; // 确保以NULL结尾
    }
    return ret_val;
}

int set_opt_includes_wrapper(char **value, int length)
{
    void *result;
    TwoCharPtr rpc_value;
    if (value == NULL || length <= 0) {
        rpc_value.twocharptr.twocharptr_len = 0;
        rpc_value.twocharptr.twocharptr_val = NULL;
    } else {
        rpc_value.twocharptr.twocharptr_len = length;
        rpc_value.twocharptr.twocharptr_val = malloc(length * sizeof(char_ptr));
        if (rpc_value.twocharptr.twocharptr_val == NULL) {
            perror("malloc failed");
            return 0;
        }
        for (int i = 0; i < length; i++) {
            if (value[i] == NULL) {
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;
            } else {
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(value[i]) + 1;
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_val = value[i];
            }
        }
    }
    result = set_opt_includes_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        if (rpc_value.twocharptr.twocharptr_val != NULL) {
            free(rpc_value.twocharptr.twocharptr_val);
        }
        return 0;
    }
    if (rpc_value.twocharptr.twocharptr_val != NULL) {
        free(rpc_value.twocharptr.twocharptr_val);
    }
    return 1;
}

bool get_opt_ignore_case_wrapper()
{
    opt_ignore_case_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_ignore_case_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_ignore_case_wrapper(bool value)
{
    void *result;
    result = set_opt_ignore_case_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_acceptregex_s_wrapper()
{
    opt_acceptregex_s_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_acceptregex_s_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_acceptregex_s_wrapper(const char *value)
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
    result = set_opt_acceptregex_s_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_rejectregex_s_wrapper()
{
    opt_rejectregex_s_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_rejectregex_s_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_rejectregex_s_wrapper(const char *value)
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
    result = set_opt_rejectregex_s_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

void* get_opt_acceptregex_wrapper(size_t *out_size)
{
    opt_acceptregex_ret_t *result;
    void *ret_val = NULL;
    result = get_opt_acceptregex_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_opt_acceptregex_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_opt_acceptregex_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

void* get_opt_rejectregex_wrapper(size_t *out_size)
{
    opt_rejectregex_ret_t *result;
    void *ret_val = NULL;
    result = get_opt_rejectregex_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_opt_rejectregex_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_opt_rejectregex_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_opt_regex_type_wrapper()
{
    opt_regex_type_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_opt_regex_type_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_regex_type_wrapper(int value)
{
    void *result;
    result = set_opt_regex_type_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

void * get_opt_regex_compile_fun_wrapper()
{
    char_ptr *result;
    result = get_opt_regex_compile_fun_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_opt_regex_compile_fun_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_opt_regex_compile_fun_1(value, clnt);
}

void * get_opt_regex_match_fun_wrapper()
{
    char_ptr *result;
    result = get_opt_regex_match_fun_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_opt_regex_match_fun_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_opt_regex_match_fun_1(value, clnt);
}

char ** get_opt_domains_wrapper()
{
    opt_domains_ret_t *result;
    char ** ret_val = NULL;
    result = get_opt_domains_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.twocharptr.twocharptr_val != NULL) {
        int length = result->value.twocharptr.twocharptr_len;
        ret_val = (char **)malloc((length + 1) * sizeof(char *));
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        for (int i = 0; i < length; i++) {
            if (result->value.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {
                ret_val[i] = NULL;
            } else {
                ret_val[i] = malloc(result->value.twocharptr.twocharptr_val[i].charptr.charptr_len);
                if (ret_val[i] == NULL) {
                    perror("malloc failed");
                    // 释放已分配的内存
                    for (int j = 0; j < i; j++) {
                        if (ret_val[j] != NULL) free(ret_val[j]);
                    }
                    free(ret_val);
                    return NULL;
                }
                memcpy(ret_val[i], result->value.twocharptr.twocharptr_val[i].charptr.charptr_val, 
                       result->value.twocharptr.twocharptr_val[i].charptr.charptr_len);
            }
        }
        ret_val[length] = NULL; // 确保以NULL结尾
    }
    return ret_val;
}

int set_opt_domains_wrapper(char **value, int length)
{
    void *result;
    TwoCharPtr rpc_value;
    if (value == NULL || length <= 0) {
        rpc_value.twocharptr.twocharptr_len = 0;
        rpc_value.twocharptr.twocharptr_val = NULL;
    } else {
        rpc_value.twocharptr.twocharptr_len = length;
        rpc_value.twocharptr.twocharptr_val = malloc(length * sizeof(char_ptr));
        if (rpc_value.twocharptr.twocharptr_val == NULL) {
            perror("malloc failed");
            return 0;
        }
        for (int i = 0; i < length; i++) {
            if (value[i] == NULL) {
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;
            } else {
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(value[i]) + 1;
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_val = value[i];
            }
        }
    }
    result = set_opt_domains_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        if (rpc_value.twocharptr.twocharptr_val != NULL) {
            free(rpc_value.twocharptr.twocharptr_val);
        }
        return 0;
    }
    if (rpc_value.twocharptr.twocharptr_val != NULL) {
        free(rpc_value.twocharptr.twocharptr_val);
    }
    return 1;
}

char ** get_opt_exclude_domains_wrapper()
{
    opt_exclude_domains_ret_t *result;
    char ** ret_val = NULL;
    result = get_opt_exclude_domains_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.twocharptr.twocharptr_val != NULL) {
        int length = result->value.twocharptr.twocharptr_len;
        ret_val = (char **)malloc((length + 1) * sizeof(char *));
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        for (int i = 0; i < length; i++) {
            if (result->value.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {
                ret_val[i] = NULL;
            } else {
                ret_val[i] = malloc(result->value.twocharptr.twocharptr_val[i].charptr.charptr_len);
                if (ret_val[i] == NULL) {
                    perror("malloc failed");
                    // 释放已分配的内存
                    for (int j = 0; j < i; j++) {
                        if (ret_val[j] != NULL) free(ret_val[j]);
                    }
                    free(ret_val);
                    return NULL;
                }
                memcpy(ret_val[i], result->value.twocharptr.twocharptr_val[i].charptr.charptr_val, 
                       result->value.twocharptr.twocharptr_val[i].charptr.charptr_len);
            }
        }
        ret_val[length] = NULL; // 确保以NULL结尾
    }
    return ret_val;
}

int set_opt_exclude_domains_wrapper(char **value, int length)
{
    void *result;
    TwoCharPtr rpc_value;
    if (value == NULL || length <= 0) {
        rpc_value.twocharptr.twocharptr_len = 0;
        rpc_value.twocharptr.twocharptr_val = NULL;
    } else {
        rpc_value.twocharptr.twocharptr_len = length;
        rpc_value.twocharptr.twocharptr_val = malloc(length * sizeof(char_ptr));
        if (rpc_value.twocharptr.twocharptr_val == NULL) {
            perror("malloc failed");
            return 0;
        }
        for (int i = 0; i < length; i++) {
            if (value[i] == NULL) {
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;
            } else {
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(value[i]) + 1;
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_val = value[i];
            }
        }
    }
    result = set_opt_exclude_domains_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        if (rpc_value.twocharptr.twocharptr_val != NULL) {
            free(rpc_value.twocharptr.twocharptr_val);
        }
        return 0;
    }
    if (rpc_value.twocharptr.twocharptr_val != NULL) {
        free(rpc_value.twocharptr.twocharptr_val);
    }
    return 1;
}

bool get_opt_dns_cache_wrapper()
{
    opt_dns_cache_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_dns_cache_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_dns_cache_wrapper(bool value)
{
    void *result;
    result = set_opt_dns_cache_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char ** get_opt_follow_tags_wrapper()
{
    opt_follow_tags_ret_t *result;
    char ** ret_val = NULL;
    result = get_opt_follow_tags_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.twocharptr.twocharptr_val != NULL) {
        int length = result->value.twocharptr.twocharptr_len;
        ret_val = (char **)malloc((length + 1) * sizeof(char *));
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        for (int i = 0; i < length; i++) {
            if (result->value.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {
                ret_val[i] = NULL;
            } else {
                ret_val[i] = malloc(result->value.twocharptr.twocharptr_val[i].charptr.charptr_len);
                if (ret_val[i] == NULL) {
                    perror("malloc failed");
                    // 释放已分配的内存
                    for (int j = 0; j < i; j++) {
                        if (ret_val[j] != NULL) free(ret_val[j]);
                    }
                    free(ret_val);
                    return NULL;
                }
                memcpy(ret_val[i], result->value.twocharptr.twocharptr_val[i].charptr.charptr_val, 
                       result->value.twocharptr.twocharptr_val[i].charptr.charptr_len);
            }
        }
        ret_val[length] = NULL; // 确保以NULL结尾
    }
    return ret_val;
}

int set_opt_follow_tags_wrapper(char **value, int length)
{
    void *result;
    TwoCharPtr rpc_value;
    if (value == NULL || length <= 0) {
        rpc_value.twocharptr.twocharptr_len = 0;
        rpc_value.twocharptr.twocharptr_val = NULL;
    } else {
        rpc_value.twocharptr.twocharptr_len = length;
        rpc_value.twocharptr.twocharptr_val = malloc(length * sizeof(char_ptr));
        if (rpc_value.twocharptr.twocharptr_val == NULL) {
            perror("malloc failed");
            return 0;
        }
        for (int i = 0; i < length; i++) {
            if (value[i] == NULL) {
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;
            } else {
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(value[i]) + 1;
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_val = value[i];
            }
        }
    }
    result = set_opt_follow_tags_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        if (rpc_value.twocharptr.twocharptr_val != NULL) {
            free(rpc_value.twocharptr.twocharptr_val);
        }
        return 0;
    }
    if (rpc_value.twocharptr.twocharptr_val != NULL) {
        free(rpc_value.twocharptr.twocharptr_val);
    }
    return 1;
}

char ** get_opt_ignore_tags_wrapper()
{
    opt_ignore_tags_ret_t *result;
    char ** ret_val = NULL;
    result = get_opt_ignore_tags_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.twocharptr.twocharptr_val != NULL) {
        int length = result->value.twocharptr.twocharptr_len;
        ret_val = (char **)malloc((length + 1) * sizeof(char *));
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        for (int i = 0; i < length; i++) {
            if (result->value.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {
                ret_val[i] = NULL;
            } else {
                ret_val[i] = malloc(result->value.twocharptr.twocharptr_val[i].charptr.charptr_len);
                if (ret_val[i] == NULL) {
                    perror("malloc failed");
                    // 释放已分配的内存
                    for (int j = 0; j < i; j++) {
                        if (ret_val[j] != NULL) free(ret_val[j]);
                    }
                    free(ret_val);
                    return NULL;
                }
                memcpy(ret_val[i], result->value.twocharptr.twocharptr_val[i].charptr.charptr_val, 
                       result->value.twocharptr.twocharptr_val[i].charptr.charptr_len);
            }
        }
        ret_val[length] = NULL; // 确保以NULL结尾
    }
    return ret_val;
}

int set_opt_ignore_tags_wrapper(char **value, int length)
{
    void *result;
    TwoCharPtr rpc_value;
    if (value == NULL || length <= 0) {
        rpc_value.twocharptr.twocharptr_len = 0;
        rpc_value.twocharptr.twocharptr_val = NULL;
    } else {
        rpc_value.twocharptr.twocharptr_len = length;
        rpc_value.twocharptr.twocharptr_val = malloc(length * sizeof(char_ptr));
        if (rpc_value.twocharptr.twocharptr_val == NULL) {
            perror("malloc failed");
            return 0;
        }
        for (int i = 0; i < length; i++) {
            if (value[i] == NULL) {
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;
            } else {
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(value[i]) + 1;
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_val = value[i];
            }
        }
    }
    result = set_opt_ignore_tags_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        if (rpc_value.twocharptr.twocharptr_val != NULL) {
            free(rpc_value.twocharptr.twocharptr_val);
        }
        return 0;
    }
    if (rpc_value.twocharptr.twocharptr_val != NULL) {
        free(rpc_value.twocharptr.twocharptr_val);
    }
    return 1;
}

bool get_opt_follow_ftp_wrapper()
{
    opt_follow_ftp_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_follow_ftp_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_follow_ftp_wrapper(bool value)
{
    void *result;
    result = set_opt_follow_ftp_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_retr_symlinks_wrapper()
{
    opt_retr_symlinks_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_retr_symlinks_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_retr_symlinks_wrapper(bool value)
{
    void *result;
    result = set_opt_retr_symlinks_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_output_document_wrapper()
{
    opt_output_document_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_output_document_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_output_document_wrapper(const char *value)
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
    result = set_opt_output_document_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_warc_filename_wrapper()
{
    opt_warc_filename_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_warc_filename_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_warc_filename_wrapper(const char *value)
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
    result = set_opt_warc_filename_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_warc_tempdir_wrapper()
{
    opt_warc_tempdir_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_warc_tempdir_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_warc_tempdir_wrapper(const char *value)
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
    result = set_opt_warc_tempdir_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_warc_cdx_dedup_filename_wrapper()
{
    opt_warc_cdx_dedup_filename_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_warc_cdx_dedup_filename_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_warc_cdx_dedup_filename_wrapper(const char *value)
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
    result = set_opt_warc_cdx_dedup_filename_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

wgint get_opt_warc_maxsize_wrapper()
{
    opt_warc_maxsize_ret_t *result;
    wgint ret_val = 0; // 默认值
    result = get_opt_warc_maxsize_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_warc_maxsize_wrapper(wgint value)
{
    void *result;
    result = set_opt_warc_maxsize_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_warc_compression_enabled_wrapper()
{
    opt_warc_compression_enabled_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_warc_compression_enabled_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_warc_compression_enabled_wrapper(bool value)
{
    void *result;
    result = set_opt_warc_compression_enabled_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_warc_digests_enabled_wrapper()
{
    opt_warc_digests_enabled_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_warc_digests_enabled_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_warc_digests_enabled_wrapper(bool value)
{
    void *result;
    result = set_opt_warc_digests_enabled_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_warc_cdx_enabled_wrapper()
{
    opt_warc_cdx_enabled_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_warc_cdx_enabled_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_warc_cdx_enabled_wrapper(bool value)
{
    void *result;
    result = set_opt_warc_cdx_enabled_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_warc_keep_log_wrapper()
{
    opt_warc_keep_log_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_warc_keep_log_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_warc_keep_log_wrapper(bool value)
{
    void *result;
    result = set_opt_warc_keep_log_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char ** get_opt_warc_user_headers_wrapper()
{
    opt_warc_user_headers_ret_t *result;
    char ** ret_val = NULL;
    result = get_opt_warc_user_headers_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.twocharptr.twocharptr_val != NULL) {
        int length = result->value.twocharptr.twocharptr_len;
        ret_val = (char **)malloc((length + 1) * sizeof(char *));
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        for (int i = 0; i < length; i++) {
            if (result->value.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {
                ret_val[i] = NULL;
            } else {
                ret_val[i] = malloc(result->value.twocharptr.twocharptr_val[i].charptr.charptr_len);
                if (ret_val[i] == NULL) {
                    perror("malloc failed");
                    // 释放已分配的内存
                    for (int j = 0; j < i; j++) {
                        if (ret_val[j] != NULL) free(ret_val[j]);
                    }
                    free(ret_val);
                    return NULL;
                }
                memcpy(ret_val[i], result->value.twocharptr.twocharptr_val[i].charptr.charptr_val, 
                       result->value.twocharptr.twocharptr_val[i].charptr.charptr_len);
            }
        }
        ret_val[length] = NULL; // 确保以NULL结尾
    }
    return ret_val;
}

int set_opt_warc_user_headers_wrapper(char **value, int length)
{
    void *result;
    TwoCharPtr rpc_value;
    if (value == NULL || length <= 0) {
        rpc_value.twocharptr.twocharptr_len = 0;
        rpc_value.twocharptr.twocharptr_val = NULL;
    } else {
        rpc_value.twocharptr.twocharptr_len = length;
        rpc_value.twocharptr.twocharptr_val = malloc(length * sizeof(char_ptr));
        if (rpc_value.twocharptr.twocharptr_val == NULL) {
            perror("malloc failed");
            return 0;
        }
        for (int i = 0; i < length; i++) {
            if (value[i] == NULL) {
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;
            } else {
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(value[i]) + 1;
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_val = value[i];
            }
        }
    }
    result = set_opt_warc_user_headers_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        if (rpc_value.twocharptr.twocharptr_val != NULL) {
            free(rpc_value.twocharptr.twocharptr_val);
        }
        return 0;
    }
    if (rpc_value.twocharptr.twocharptr_val != NULL) {
        free(rpc_value.twocharptr.twocharptr_val);
    }
    return 1;
}

char * get_opt_user_wrapper()
{
    opt_user_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_user_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_user_wrapper(const char *value)
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
    result = set_opt_user_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_passwd_wrapper()
{
    opt_passwd_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_passwd_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_passwd_wrapper(const char *value)
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
    result = set_opt_passwd_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_ask_passwd_wrapper()
{
    opt_ask_passwd_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_ask_passwd_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_ask_passwd_wrapper(bool value)
{
    void *result;
    result = set_opt_ask_passwd_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_always_rest_wrapper()
{
    opt_always_rest_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_always_rest_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_always_rest_wrapper(bool value)
{
    void *result;
    result = set_opt_always_rest_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

wgint get_opt_start_pos_wrapper()
{
    opt_start_pos_ret_t *result;
    wgint ret_val = 0; // 默认值
    result = get_opt_start_pos_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_start_pos_wrapper(wgint value)
{
    void *result;
    result = set_opt_start_pos_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_ftp_user_wrapper()
{
    opt_ftp_user_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_ftp_user_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_ftp_user_wrapper(const char *value)
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
    result = set_opt_ftp_user_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_ftp_passwd_wrapper()
{
    opt_ftp_passwd_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_ftp_passwd_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_ftp_passwd_wrapper(const char *value)
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
    result = set_opt_ftp_passwd_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_netrc_wrapper()
{
    opt_netrc_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_netrc_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_netrc_wrapper(bool value)
{
    void *result;
    result = set_opt_netrc_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_ftp_glob_wrapper()
{
    opt_ftp_glob_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_ftp_glob_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_ftp_glob_wrapper(bool value)
{
    void *result;
    result = set_opt_ftp_glob_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_ftp_pasv_wrapper()
{
    opt_ftp_pasv_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_ftp_pasv_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_ftp_pasv_wrapper(bool value)
{
    void *result;
    result = set_opt_ftp_pasv_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_http_user_wrapper()
{
    opt_http_user_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_http_user_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_http_user_wrapper(const char *value)
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
    result = set_opt_http_user_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_http_passwd_wrapper()
{
    opt_http_passwd_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_http_passwd_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_http_passwd_wrapper(const char *value)
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
    result = set_opt_http_passwd_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char ** get_opt_user_headers_wrapper()
{
    opt_user_headers_ret_t *result;
    char ** ret_val = NULL;
    result = get_opt_user_headers_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.twocharptr.twocharptr_val != NULL) {
        int length = result->value.twocharptr.twocharptr_len;
        ret_val = (char **)malloc((length + 1) * sizeof(char *));
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        for (int i = 0; i < length; i++) {
            if (result->value.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {
                ret_val[i] = NULL;
            } else {
                ret_val[i] = malloc(result->value.twocharptr.twocharptr_val[i].charptr.charptr_len);
                if (ret_val[i] == NULL) {
                    perror("malloc failed");
                    // 释放已分配的内存
                    for (int j = 0; j < i; j++) {
                        if (ret_val[j] != NULL) free(ret_val[j]);
                    }
                    free(ret_val);
                    return NULL;
                }
                memcpy(ret_val[i], result->value.twocharptr.twocharptr_val[i].charptr.charptr_val, 
                       result->value.twocharptr.twocharptr_val[i].charptr.charptr_len);
            }
        }
        ret_val[length] = NULL; // 确保以NULL结尾
    }
    return ret_val;
}

int set_opt_user_headers_wrapper(char **value, int length)
{
    void *result;
    TwoCharPtr rpc_value;
    if (value == NULL || length <= 0) {
        rpc_value.twocharptr.twocharptr_len = 0;
        rpc_value.twocharptr.twocharptr_val = NULL;
    } else {
        rpc_value.twocharptr.twocharptr_len = length;
        rpc_value.twocharptr.twocharptr_val = malloc(length * sizeof(char_ptr));
        if (rpc_value.twocharptr.twocharptr_val == NULL) {
            perror("malloc failed");
            return 0;
        }
        for (int i = 0; i < length; i++) {
            if (value[i] == NULL) {
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;
            } else {
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(value[i]) + 1;
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_val = value[i];
            }
        }
    }
    result = set_opt_user_headers_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        if (rpc_value.twocharptr.twocharptr_val != NULL) {
            free(rpc_value.twocharptr.twocharptr_val);
        }
        return 0;
    }
    if (rpc_value.twocharptr.twocharptr_val != NULL) {
        free(rpc_value.twocharptr.twocharptr_val);
    }
    return 1;
}

bool get_opt_http_keep_alive_wrapper()
{
    opt_http_keep_alive_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_http_keep_alive_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_http_keep_alive_wrapper(bool value)
{
    void *result;
    result = set_opt_http_keep_alive_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_use_proxy_wrapper()
{
    opt_use_proxy_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_use_proxy_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_use_proxy_wrapper(bool value)
{
    void *result;
    result = set_opt_use_proxy_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_allow_cache_wrapper()
{
    opt_allow_cache_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_allow_cache_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_allow_cache_wrapper(bool value)
{
    void *result;
    result = set_opt_allow_cache_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_http_proxy_wrapper()
{
    opt_http_proxy_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_http_proxy_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_http_proxy_wrapper(const char *value)
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
    result = set_opt_http_proxy_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_ftp_proxy_wrapper()
{
    opt_ftp_proxy_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_ftp_proxy_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_ftp_proxy_wrapper(const char *value)
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
    result = set_opt_ftp_proxy_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_https_proxy_wrapper()
{
    opt_https_proxy_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_https_proxy_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_https_proxy_wrapper(const char *value)
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
    result = set_opt_https_proxy_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char ** get_opt_no_proxy_wrapper()
{
    opt_no_proxy_ret_t *result;
    char ** ret_val = NULL;
    result = get_opt_no_proxy_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.twocharptr.twocharptr_val != NULL) {
        int length = result->value.twocharptr.twocharptr_len;
        ret_val = (char **)malloc((length + 1) * sizeof(char *));
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        for (int i = 0; i < length; i++) {
            if (result->value.twocharptr.twocharptr_val[i].charptr.charptr_val == NULL) {
                ret_val[i] = NULL;
            } else {
                ret_val[i] = malloc(result->value.twocharptr.twocharptr_val[i].charptr.charptr_len);
                if (ret_val[i] == NULL) {
                    perror("malloc failed");
                    // 释放已分配的内存
                    for (int j = 0; j < i; j++) {
                        if (ret_val[j] != NULL) free(ret_val[j]);
                    }
                    free(ret_val);
                    return NULL;
                }
                memcpy(ret_val[i], result->value.twocharptr.twocharptr_val[i].charptr.charptr_val, 
                       result->value.twocharptr.twocharptr_val[i].charptr.charptr_len);
            }
        }
        ret_val[length] = NULL; // 确保以NULL结尾
    }
    return ret_val;
}

int set_opt_no_proxy_wrapper(char **value, int length)
{
    void *result;
    TwoCharPtr rpc_value;
    if (value == NULL || length <= 0) {
        rpc_value.twocharptr.twocharptr_len = 0;
        rpc_value.twocharptr.twocharptr_val = NULL;
    } else {
        rpc_value.twocharptr.twocharptr_len = length;
        rpc_value.twocharptr.twocharptr_val = malloc(length * sizeof(char_ptr));
        if (rpc_value.twocharptr.twocharptr_val == NULL) {
            perror("malloc failed");
            return 0;
        }
        for (int i = 0; i < length; i++) {
            if (value[i] == NULL) {
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_len = 0;
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_val = NULL;
            } else {
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_len = strlen(value[i]) + 1;
                rpc_value.twocharptr.twocharptr_val[i].charptr.charptr_val = value[i];
            }
        }
    }
    result = set_opt_no_proxy_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        if (rpc_value.twocharptr.twocharptr_val != NULL) {
            free(rpc_value.twocharptr.twocharptr_val);
        }
        return 0;
    }
    if (rpc_value.twocharptr.twocharptr_val != NULL) {
        free(rpc_value.twocharptr.twocharptr_val);
    }
    return 1;
}

char * get_opt_base_href_wrapper()
{
    opt_base_href_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_base_href_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_base_href_wrapper(const char *value)
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
    result = set_opt_base_href_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_progress_type_wrapper()
{
    opt_progress_type_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_progress_type_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_progress_type_wrapper(const char *value)
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
    result = set_opt_progress_type_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_opt_show_progress_wrapper()
{
    opt_show_progress_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_opt_show_progress_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_show_progress_wrapper(int value)
{
    void *result;
    result = set_opt_show_progress_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_noscroll_wrapper()
{
    opt_noscroll_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_noscroll_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_noscroll_wrapper(bool value)
{
    void *result;
    result = set_opt_noscroll_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_proxy_user_wrapper()
{
    opt_proxy_user_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_proxy_user_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_proxy_user_wrapper(const char *value)
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
    result = set_opt_proxy_user_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_proxy_passwd_wrapper()
{
    opt_proxy_passwd_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_proxy_passwd_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_proxy_passwd_wrapper(const char *value)
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
    result = set_opt_proxy_passwd_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

double get_opt_read_timeout_wrapper()
{
    opt_read_timeout_ret_t *result;
    double ret_val = 0; // 默认值
    result = get_opt_read_timeout_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_read_timeout_wrapper(double value)
{
    void *result;
    result = set_opt_read_timeout_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

double get_opt_dns_timeout_wrapper()
{
    opt_dns_timeout_ret_t *result;
    double ret_val = 0; // 默认值
    result = get_opt_dns_timeout_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_dns_timeout_wrapper(double value)
{
    void *result;
    result = set_opt_dns_timeout_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

double get_opt_connect_timeout_wrapper()
{
    opt_connect_timeout_ret_t *result;
    double ret_val = 0; // 默认值
    result = get_opt_connect_timeout_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_connect_timeout_wrapper(double value)
{
    void *result;
    result = set_opt_connect_timeout_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_random_wait_wrapper()
{
    opt_random_wait_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_random_wait_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_random_wait_wrapper(bool value)
{
    void *result;
    result = set_opt_random_wait_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

double get_opt_wait_wrapper()
{
    opt_wait_ret_t *result;
    double ret_val = 0; // 默认值
    result = get_opt_wait_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_wait_wrapper(double value)
{
    void *result;
    result = set_opt_wait_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

double get_opt_waitretry_wrapper()
{
    opt_waitretry_ret_t *result;
    double ret_val = 0; // 默认值
    result = get_opt_waitretry_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_waitretry_wrapper(double value)
{
    void *result;
    result = set_opt_waitretry_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_use_robots_wrapper()
{
    opt_use_robots_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_use_robots_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_use_robots_wrapper(bool value)
{
    void *result;
    result = set_opt_use_robots_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

wgint get_opt_limit_rate_wrapper()
{
    opt_limit_rate_ret_t *result;
    wgint ret_val = 0; // 默认值
    result = get_opt_limit_rate_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_limit_rate_wrapper(wgint value)
{
    void *result;
    result = set_opt_limit_rate_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

SUM_SIZE_INT get_opt_quota_wrapper()
{
    opt_quota_ret_t *result;
    SUM_SIZE_INT ret_val = 0; // 默认值
    result = get_opt_quota_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_quota_wrapper(SUM_SIZE_INT value)
{
    void *result;
    result = set_opt_quota_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_server_response_wrapper()
{
    opt_server_response_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_server_response_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_server_response_wrapper(bool value)
{
    void *result;
    result = set_opt_server_response_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_save_headers_wrapper()
{
    opt_save_headers_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_save_headers_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_save_headers_wrapper(bool value)
{
    void *result;
    result = set_opt_save_headers_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_content_on_error_wrapper()
{
    opt_content_on_error_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_content_on_error_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_content_on_error_wrapper(bool value)
{
    void *result;
    result = set_opt_content_on_error_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_debug_wrapper()
{
    opt_debug_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_debug_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_debug_wrapper(bool value)
{
    void *result;
    result = set_opt_debug_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_timestamping_wrapper()
{
    opt_timestamping_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_timestamping_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_timestamping_wrapper(bool value)
{
    void *result;
    result = set_opt_timestamping_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_if_modified_since_wrapper()
{
    opt_if_modified_since_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_if_modified_since_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_if_modified_since_wrapper(bool value)
{
    void *result;
    result = set_opt_if_modified_since_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_backup_converted_wrapper()
{
    opt_backup_converted_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_backup_converted_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_backup_converted_wrapper(bool value)
{
    void *result;
    result = set_opt_backup_converted_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_opt_backups_wrapper()
{
    opt_backups_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_opt_backups_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_backups_wrapper(int value)
{
    void *result;
    result = set_opt_backups_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_useragent_wrapper()
{
    opt_useragent_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_useragent_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_useragent_wrapper(const char *value)
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
    result = set_opt_useragent_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_referer_wrapper()
{
    opt_referer_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_referer_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_referer_wrapper(const char *value)
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
    result = set_opt_referer_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_convert_links_wrapper()
{
    opt_convert_links_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_convert_links_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_convert_links_wrapper(bool value)
{
    void *result;
    result = set_opt_convert_links_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_convert_file_only_wrapper()
{
    opt_convert_file_only_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_convert_file_only_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_convert_file_only_wrapper(bool value)
{
    void *result;
    result = set_opt_convert_file_only_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_remove_listing_wrapper()
{
    opt_remove_listing_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_remove_listing_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_remove_listing_wrapper(bool value)
{
    void *result;
    result = set_opt_remove_listing_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_htmlify_wrapper()
{
    opt_htmlify_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_htmlify_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_htmlify_wrapper(bool value)
{
    void *result;
    result = set_opt_htmlify_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_dot_style_wrapper()
{
    opt_dot_style_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_dot_style_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_dot_style_wrapper(const char *value)
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
    result = set_opt_dot_style_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

wgint get_opt_dot_bytes_wrapper()
{
    opt_dot_bytes_ret_t *result;
    wgint ret_val = 0; // 默认值
    result = get_opt_dot_bytes_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_dot_bytes_wrapper(wgint value)
{
    void *result;
    result = set_opt_dot_bytes_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_opt_dots_in_line_wrapper()
{
    opt_dots_in_line_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_opt_dots_in_line_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_dots_in_line_wrapper(int value)
{
    void *result;
    result = set_opt_dots_in_line_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_opt_dot_spacing_wrapper()
{
    opt_dot_spacing_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_opt_dot_spacing_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_dot_spacing_wrapper(int value)
{
    void *result;
    result = set_opt_dot_spacing_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_delete_after_wrapper()
{
    opt_delete_after_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_delete_after_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_delete_after_wrapper(bool value)
{
    void *result;
    result = set_opt_delete_after_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_adjust_extension_wrapper()
{
    opt_adjust_extension_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_adjust_extension_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_adjust_extension_wrapper(bool value)
{
    void *result;
    result = set_opt_adjust_extension_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_page_requisites_wrapper()
{
    opt_page_requisites_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_page_requisites_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_page_requisites_wrapper(bool value)
{
    void *result;
    result = set_opt_page_requisites_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_bind_address_wrapper()
{
    opt_bind_address_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_bind_address_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_bind_address_wrapper(const char *value)
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
    result = set_opt_bind_address_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_opt_secure_protocol_wrapper()
{
    opt_secure_protocol_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_opt_secure_protocol_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_secure_protocol_wrapper(int value)
{
    void *result;
    result = set_opt_secure_protocol_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_opt_check_cert_wrapper()
{
    opt_check_cert_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_opt_check_cert_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_check_cert_wrapper(int value)
{
    void *result;
    result = set_opt_check_cert_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_cert_file_wrapper()
{
    opt_cert_file_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_cert_file_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_cert_file_wrapper(const char *value)
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
    result = set_opt_cert_file_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_private_key_wrapper()
{
    opt_private_key_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_private_key_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_private_key_wrapper(const char *value)
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
    result = set_opt_private_key_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_opt_cert_type_wrapper()
{
    opt_cert_type_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_opt_cert_type_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_cert_type_wrapper(int value)
{
    void *result;
    result = set_opt_cert_type_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_opt_private_key_type_wrapper()
{
    opt_private_key_type_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_opt_private_key_type_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_private_key_type_wrapper(int value)
{
    void *result;
    result = set_opt_private_key_type_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_ca_directory_wrapper()
{
    opt_ca_directory_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_ca_directory_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_ca_directory_wrapper(const char *value)
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
    result = set_opt_ca_directory_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_ca_cert_wrapper()
{
    opt_ca_cert_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_ca_cert_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_ca_cert_wrapper(const char *value)
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
    result = set_opt_ca_cert_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_crl_file_wrapper()
{
    opt_crl_file_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_crl_file_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_crl_file_wrapper(const char *value)
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
    result = set_opt_crl_file_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_pinnedpubkey_wrapper()
{
    opt_pinnedpubkey_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_pinnedpubkey_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_pinnedpubkey_wrapper(const char *value)
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
    result = set_opt_pinnedpubkey_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_random_file_wrapper()
{
    opt_random_file_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_random_file_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_random_file_wrapper(const char *value)
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
    result = set_opt_random_file_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_egd_file_wrapper()
{
    opt_egd_file_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_egd_file_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_egd_file_wrapper(const char *value)
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
    result = set_opt_egd_file_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_https_only_wrapper()
{
    opt_https_only_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_https_only_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_https_only_wrapper(bool value)
{
    void *result;
    result = set_opt_https_only_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_ftps_resume_ssl_wrapper()
{
    opt_ftps_resume_ssl_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_ftps_resume_ssl_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_ftps_resume_ssl_wrapper(bool value)
{
    void *result;
    result = set_opt_ftps_resume_ssl_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_ftps_fallback_to_ftp_wrapper()
{
    opt_ftps_fallback_to_ftp_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_ftps_fallback_to_ftp_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_ftps_fallback_to_ftp_wrapper(bool value)
{
    void *result;
    result = set_opt_ftps_fallback_to_ftp_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_ftps_implicit_wrapper()
{
    opt_ftps_implicit_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_ftps_implicit_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_ftps_implicit_wrapper(bool value)
{
    void *result;
    result = set_opt_ftps_implicit_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_ftps_clear_data_connection_wrapper()
{
    opt_ftps_clear_data_connection_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_ftps_clear_data_connection_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_ftps_clear_data_connection_wrapper(bool value)
{
    void *result;
    result = set_opt_ftps_clear_data_connection_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_cookies_wrapper()
{
    opt_cookies_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_cookies_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_cookies_wrapper(bool value)
{
    void *result;
    result = set_opt_cookies_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_cookies_input_wrapper()
{
    opt_cookies_input_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_cookies_input_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_cookies_input_wrapper(const char *value)
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
    result = set_opt_cookies_input_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_cookies_output_wrapper()
{
    opt_cookies_output_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_cookies_output_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_cookies_output_wrapper(const char *value)
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
    result = set_opt_cookies_output_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_keep_session_cookies_wrapper()
{
    opt_keep_session_cookies_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_keep_session_cookies_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_keep_session_cookies_wrapper(bool value)
{
    void *result;
    result = set_opt_keep_session_cookies_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_post_data_wrapper()
{
    opt_post_data_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_post_data_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_post_data_wrapper(const char *value)
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
    result = set_opt_post_data_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_post_file_name_wrapper()
{
    opt_post_file_name_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_post_file_name_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_post_file_name_wrapper(const char *value)
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
    result = set_opt_post_file_name_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_method_wrapper()
{
    opt_method_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_method_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_method_wrapper(const char *value)
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
    result = set_opt_method_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_body_data_wrapper()
{
    opt_body_data_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_body_data_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_body_data_wrapper(const char *value)
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
    result = set_opt_body_data_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_body_file_wrapper()
{
    opt_body_file_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_body_file_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_body_file_wrapper(const char *value)
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
    result = set_opt_body_file_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_opt_restrict_files_os_wrapper()
{
    opt_restrict_files_os_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_opt_restrict_files_os_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_restrict_files_os_wrapper(int value)
{
    void *result;
    result = set_opt_restrict_files_os_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_restrict_files_ctrl_wrapper()
{
    opt_restrict_files_ctrl_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_restrict_files_ctrl_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_restrict_files_ctrl_wrapper(bool value)
{
    void *result;
    result = set_opt_restrict_files_ctrl_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_restrict_files_nonascii_wrapper()
{
    opt_restrict_files_nonascii_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_restrict_files_nonascii_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_restrict_files_nonascii_wrapper(bool value)
{
    void *result;
    result = set_opt_restrict_files_nonascii_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_opt_restrict_files_case_wrapper()
{
    opt_restrict_files_case_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_opt_restrict_files_case_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_restrict_files_case_wrapper(int value)
{
    void *result;
    result = set_opt_restrict_files_case_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_strict_comments_wrapper()
{
    opt_strict_comments_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_strict_comments_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_strict_comments_wrapper(bool value)
{
    void *result;
    result = set_opt_strict_comments_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_preserve_perm_wrapper()
{
    opt_preserve_perm_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_preserve_perm_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_preserve_perm_wrapper(bool value)
{
    void *result;
    result = set_opt_preserve_perm_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_ipv4_only_wrapper()
{
    opt_ipv4_only_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_ipv4_only_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_ipv4_only_wrapper(bool value)
{
    void *result;
    result = set_opt_ipv4_only_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_ipv6_only_wrapper()
{
    opt_ipv6_only_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_ipv6_only_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_ipv6_only_wrapper(bool value)
{
    void *result;
    result = set_opt_ipv6_only_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_opt_prefer_family_wrapper()
{
    opt_prefer_family_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_opt_prefer_family_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_prefer_family_wrapper(int value)
{
    void *result;
    result = set_opt_prefer_family_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_content_disposition_wrapper()
{
    opt_content_disposition_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_content_disposition_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_content_disposition_wrapper(bool value)
{
    void *result;
    result = set_opt_content_disposition_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_auth_without_challenge_wrapper()
{
    opt_auth_without_challenge_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_auth_without_challenge_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_auth_without_challenge_wrapper(bool value)
{
    void *result;
    result = set_opt_auth_without_challenge_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_enable_iri_wrapper()
{
    opt_enable_iri_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_enable_iri_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_enable_iri_wrapper(bool value)
{
    void *result;
    result = set_opt_enable_iri_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_encoding_remote_wrapper()
{
    opt_encoding_remote_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_encoding_remote_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_encoding_remote_wrapper(const char *value)
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
    result = set_opt_encoding_remote_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_locale_wrapper()
{
    opt_locale_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_locale_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_locale_wrapper(const char *value)
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
    result = set_opt_locale_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_trustservernames_wrapper()
{
    opt_trustservernames_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_trustservernames_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_trustservernames_wrapper(bool value)
{
    void *result;
    result = set_opt_trustservernames_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_useservertimestamps_wrapper()
{
    opt_useservertimestamps_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_useservertimestamps_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_useservertimestamps_wrapper(bool value)
{
    void *result;
    result = set_opt_useservertimestamps_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_show_all_dns_entries_wrapper()
{
    opt_show_all_dns_entries_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_show_all_dns_entries_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_show_all_dns_entries_wrapper(bool value)
{
    void *result;
    result = set_opt_show_all_dns_entries_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_report_bps_wrapper()
{
    opt_report_bps_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_report_bps_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_report_bps_wrapper(bool value)
{
    void *result;
    result = set_opt_report_bps_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_rejected_log_wrapper()
{
    opt_rejected_log_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_rejected_log_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_rejected_log_wrapper(const char *value)
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
    result = set_opt_rejected_log_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_opt_hsts_wrapper()
{
    opt_hsts_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_opt_hsts_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_opt_hsts_wrapper(bool value)
{
    void *result;
    result = set_opt_hsts_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_opt_hsts_file_wrapper()
{
    opt_hsts_file_ret_t *result;
    char * ret_val = NULL;
    result = get_opt_hsts_file_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_opt_hsts_file_wrapper(const char *value)
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
    result = set_opt_hsts_file_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

double get_total_download_time_wrapper()
{
    total_download_time_ret_t *result;
    double default_value = {0}; // 初始化默认值
    result = get_total_download_time_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_total_download_time_wrapper(double value)
{
    void *result;
    result = set_total_download_time_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

int get_warc_current_file_number_wrapper()
{
    warc_current_file_number_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_warc_current_file_number_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_warc_current_file_number_wrapper(int value)
{
    void *result;
    result = set_warc_current_file_number_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

int get_output_redirected_wrapper()
{
    output_redirected_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_output_redirected_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_output_redirected_wrapper(int value)
{
    void *result;
    result = set_output_redirected_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

struct hash_table* get_transport_map_wrapper()
{
    transport_map_ret_t *result_rpc;
    struct hash_table* value = {0};
    result_rpc = get_transport_map_1(clnt);
        if(result_rpc->value.hash_table_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.hash_table_rpc_ptr_len*sizeof(struct hash_table));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.hash_table_rpc_ptr_len;i0++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {value[i0].cells = NULL;}
    else {
    	value[i0].cells = (struct cell *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof( struct cell));
    	if (value[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {value[i0].cells[i1].key = NULL;}
    else {value[i0].cells[i1].key = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len * sizeof(char));
    	if (value[i0].cells[i1].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].key, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);
    }
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {value[i0].cells[i1].value = NULL;}
    else {value[i0].cells[i1].value = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len * sizeof(char));
    	if (value[i0].cells[i1].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].value, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);
    }
    	}
    }
    value[i0].size=result_rpc->value.hash_table_rpc_ptr_val[i0].size;
    value[i0].count=result_rpc->value.hash_table_rpc_ptr_val[i0].count;
    value[i0].resize_threshold=result_rpc->value.hash_table_rpc_ptr_val[i0].resize_threshold;
    value[i0].prime_offset=result_rpc->value.hash_table_rpc_ptr_val[i0].prime_offset;
            }
        }
    return value;
}

int set_transport_map_wrapper(struct hash_table* value,int len)
{
    void *result;
    hash_table_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.hash_table_rpc_ptr_len=0;
    value_rpc_ptr.hash_table_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.hash_table_rpc_ptr_len=1;
    value_rpc_ptr.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
    if(value_rpc_ptr.hash_table_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.hash_table_rpc_ptr_len;i0++) {
    if (value[i0].cells == NULL) {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}
    else {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));
    	if (value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    	}
    }
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].size=value[i0].size;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].count=value[i0].count;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].resize_threshold=value[i0].resize_threshold;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].prime_offset=value[i0].prime_offset;
    }
    }
    result = set_transport_map_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void * get_transport_map_hash_function_wrapper()
{
    char_ptr *result;
    result = get_transport_map_hash_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_transport_map_hash_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_transport_map_hash_function_1(value, clnt);
}

void * get_transport_map_test_function_wrapper()
{
    char_ptr *result;
    result = get_transport_map_test_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_transport_map_test_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_transport_map_test_function_1(value, clnt);
}

struct cell* get_transport_map_cells_wrapper()
{
    transport_map_cells_ret_t *result_rpc;
    struct cell* value = {0};
    result_rpc = get_transport_map_cells_1(clnt);
        if(result_rpc->value.cell_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.cell_rpc_ptr_len*sizeof(struct cell));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.cell_rpc_ptr_len;i0++) {
    if (result_rpc->value.cell_rpc_ptr_val[i0].key.key_val == NULL) {value[i0].key = NULL;}
    else {value[i0].key = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].key.key_len * sizeof(char));
    	if (value[i0].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].key, result_rpc->value.cell_rpc_ptr_val[i0].key.key_val, result_rpc->value.cell_rpc_ptr_val[i0].key.key_len);
    }
    if (result_rpc->value.cell_rpc_ptr_val[i0].value.value_val == NULL) {value[i0].value = NULL;}
    else {value[i0].value = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].value.value_len * sizeof(char));
    	if (value[i0].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].value, result_rpc->value.cell_rpc_ptr_val[i0].value.value_val, result_rpc->value.cell_rpc_ptr_val[i0].value.value_len);
    }
            }
        }
    return value;
}

int set_transport_map_cells_wrapper(struct cell* value,int len)
{
    void *result;
    cell_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.cell_rpc_ptr_len=0;
    value_rpc_ptr.cell_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.cell_rpc_ptr_len=1;
    value_rpc_ptr.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
    if(value_rpc_ptr.cell_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.cell_rpc_ptr_len;i0++) {
    }
    }
    result = set_transport_map_cells_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void* get_transport_map_cells_key_wrapper(size_t *out_size)
{
    transport_map_cells_key_ret_t *result;
    void *ret_val = NULL;
    result = get_transport_map_cells_key_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_transport_map_cells_key_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_transport_map_cells_key_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

void* get_transport_map_cells_value_wrapper(size_t *out_size)
{
    transport_map_cells_value_ret_t *result;
    void *ret_val = NULL;
    result = get_transport_map_cells_value_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_transport_map_cells_value_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_transport_map_cells_value_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_transport_map_size_wrapper()
{
    transport_map_size_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_transport_map_size_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_transport_map_size_wrapper(int value)
{
    void *result;
    result = set_transport_map_size_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_transport_map_count_wrapper()
{
    transport_map_count_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_transport_map_count_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_transport_map_count_wrapper(int value)
{
    void *result;
    result = set_transport_map_count_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_transport_map_resize_threshold_wrapper()
{
    transport_map_resize_threshold_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_transport_map_resize_threshold_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_transport_map_resize_threshold_wrapper(int value)
{
    void *result;
    result = set_transport_map_resize_threshold_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_transport_map_prime_offset_wrapper()
{
    transport_map_prime_offset_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_transport_map_prime_offset_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_transport_map_prime_offset_wrapper(int value)
{
    void *result;
    result = set_transport_map_prime_offset_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

struct hash_table* get_interesting_tags_wrapper()
{
    interesting_tags_ret_t *result_rpc;
    struct hash_table* value = {0};
    result_rpc = get_interesting_tags_1(clnt);
        if(result_rpc->value.hash_table_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.hash_table_rpc_ptr_len*sizeof(struct hash_table));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.hash_table_rpc_ptr_len;i0++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {value[i0].cells = NULL;}
    else {
    	value[i0].cells = (struct cell *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof( struct cell));
    	if (value[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {value[i0].cells[i1].key = NULL;}
    else {value[i0].cells[i1].key = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len * sizeof(char));
    	if (value[i0].cells[i1].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].key, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);
    }
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {value[i0].cells[i1].value = NULL;}
    else {value[i0].cells[i1].value = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len * sizeof(char));
    	if (value[i0].cells[i1].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].value, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);
    }
    	}
    }
    value[i0].size=result_rpc->value.hash_table_rpc_ptr_val[i0].size;
    value[i0].count=result_rpc->value.hash_table_rpc_ptr_val[i0].count;
    value[i0].resize_threshold=result_rpc->value.hash_table_rpc_ptr_val[i0].resize_threshold;
    value[i0].prime_offset=result_rpc->value.hash_table_rpc_ptr_val[i0].prime_offset;
            }
        }
    return value;
}

int set_interesting_tags_wrapper(struct hash_table* value,int len)
{
    void *result;
    hash_table_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.hash_table_rpc_ptr_len=0;
    value_rpc_ptr.hash_table_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.hash_table_rpc_ptr_len=1;
    value_rpc_ptr.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
    if(value_rpc_ptr.hash_table_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.hash_table_rpc_ptr_len;i0++) {
    if (value[i0].cells == NULL) {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}
    else {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));
    	if (value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    	}
    }
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].size=value[i0].size;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].count=value[i0].count;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].resize_threshold=value[i0].resize_threshold;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].prime_offset=value[i0].prime_offset;
    }
    }
    result = set_interesting_tags_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void * get_interesting_tags_hash_function_wrapper()
{
    char_ptr *result;
    result = get_interesting_tags_hash_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_interesting_tags_hash_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_interesting_tags_hash_function_1(value, clnt);
}

void * get_interesting_tags_test_function_wrapper()
{
    char_ptr *result;
    result = get_interesting_tags_test_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_interesting_tags_test_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_interesting_tags_test_function_1(value, clnt);
}

struct cell* get_interesting_tags_cells_wrapper()
{
    interesting_tags_cells_ret_t *result_rpc;
    struct cell* value = {0};
    result_rpc = get_interesting_tags_cells_1(clnt);
        if(result_rpc->value.cell_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.cell_rpc_ptr_len*sizeof(struct cell));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.cell_rpc_ptr_len;i0++) {
    if (result_rpc->value.cell_rpc_ptr_val[i0].key.key_val == NULL) {value[i0].key = NULL;}
    else {value[i0].key = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].key.key_len * sizeof(char));
    	if (value[i0].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].key, result_rpc->value.cell_rpc_ptr_val[i0].key.key_val, result_rpc->value.cell_rpc_ptr_val[i0].key.key_len);
    }
    if (result_rpc->value.cell_rpc_ptr_val[i0].value.value_val == NULL) {value[i0].value = NULL;}
    else {value[i0].value = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].value.value_len * sizeof(char));
    	if (value[i0].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].value, result_rpc->value.cell_rpc_ptr_val[i0].value.value_val, result_rpc->value.cell_rpc_ptr_val[i0].value.value_len);
    }
            }
        }
    return value;
}

int set_interesting_tags_cells_wrapper(struct cell* value,int len)
{
    void *result;
    cell_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.cell_rpc_ptr_len=0;
    value_rpc_ptr.cell_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.cell_rpc_ptr_len=1;
    value_rpc_ptr.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
    if(value_rpc_ptr.cell_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.cell_rpc_ptr_len;i0++) {
    }
    }
    result = set_interesting_tags_cells_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void* get_interesting_tags_cells_key_wrapper(size_t *out_size)
{
    interesting_tags_cells_key_ret_t *result;
    void *ret_val = NULL;
    result = get_interesting_tags_cells_key_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_interesting_tags_cells_key_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_interesting_tags_cells_key_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

void* get_interesting_tags_cells_value_wrapper(size_t *out_size)
{
    interesting_tags_cells_value_ret_t *result;
    void *ret_val = NULL;
    result = get_interesting_tags_cells_value_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_interesting_tags_cells_value_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_interesting_tags_cells_value_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_interesting_tags_size_wrapper()
{
    interesting_tags_size_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_interesting_tags_size_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_interesting_tags_size_wrapper(int value)
{
    void *result;
    result = set_interesting_tags_size_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_interesting_tags_count_wrapper()
{
    interesting_tags_count_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_interesting_tags_count_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_interesting_tags_count_wrapper(int value)
{
    void *result;
    result = set_interesting_tags_count_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_interesting_tags_resize_threshold_wrapper()
{
    interesting_tags_resize_threshold_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_interesting_tags_resize_threshold_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_interesting_tags_resize_threshold_wrapper(int value)
{
    void *result;
    result = set_interesting_tags_resize_threshold_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_interesting_tags_prime_offset_wrapper()
{
    interesting_tags_prime_offset_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_interesting_tags_prime_offset_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_interesting_tags_prime_offset_wrapper(int value)
{
    void *result;
    result = set_interesting_tags_prime_offset_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

struct hash_table* get_basic_authed_hosts_wrapper()
{
    basic_authed_hosts_ret_t *result_rpc;
    struct hash_table* value = {0};
    result_rpc = get_basic_authed_hosts_1(clnt);
        if(result_rpc->value.hash_table_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.hash_table_rpc_ptr_len*sizeof(struct hash_table));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.hash_table_rpc_ptr_len;i0++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {value[i0].cells = NULL;}
    else {
    	value[i0].cells = (struct cell *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof( struct cell));
    	if (value[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {value[i0].cells[i1].key = NULL;}
    else {value[i0].cells[i1].key = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len * sizeof(char));
    	if (value[i0].cells[i1].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].key, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);
    }
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {value[i0].cells[i1].value = NULL;}
    else {value[i0].cells[i1].value = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len * sizeof(char));
    	if (value[i0].cells[i1].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].value, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);
    }
    	}
    }
    value[i0].size=result_rpc->value.hash_table_rpc_ptr_val[i0].size;
    value[i0].count=result_rpc->value.hash_table_rpc_ptr_val[i0].count;
    value[i0].resize_threshold=result_rpc->value.hash_table_rpc_ptr_val[i0].resize_threshold;
    value[i0].prime_offset=result_rpc->value.hash_table_rpc_ptr_val[i0].prime_offset;
            }
        }
    return value;
}

int set_basic_authed_hosts_wrapper(struct hash_table* value,int len)
{
    void *result;
    hash_table_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.hash_table_rpc_ptr_len=0;
    value_rpc_ptr.hash_table_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.hash_table_rpc_ptr_len=1;
    value_rpc_ptr.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
    if(value_rpc_ptr.hash_table_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.hash_table_rpc_ptr_len;i0++) {
    if (value[i0].cells == NULL) {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}
    else {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));
    	if (value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    	}
    }
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].size=value[i0].size;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].count=value[i0].count;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].resize_threshold=value[i0].resize_threshold;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].prime_offset=value[i0].prime_offset;
    }
    }
    result = set_basic_authed_hosts_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void * get_basic_authed_hosts_hash_function_wrapper()
{
    char_ptr *result;
    result = get_basic_authed_hosts_hash_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_basic_authed_hosts_hash_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_basic_authed_hosts_hash_function_1(value, clnt);
}

void * get_basic_authed_hosts_test_function_wrapper()
{
    char_ptr *result;
    result = get_basic_authed_hosts_test_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_basic_authed_hosts_test_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_basic_authed_hosts_test_function_1(value, clnt);
}

struct cell* get_basic_authed_hosts_cells_wrapper()
{
    basic_authed_hosts_cells_ret_t *result_rpc;
    struct cell* value = {0};
    result_rpc = get_basic_authed_hosts_cells_1(clnt);
        if(result_rpc->value.cell_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.cell_rpc_ptr_len*sizeof(struct cell));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.cell_rpc_ptr_len;i0++) {
    if (result_rpc->value.cell_rpc_ptr_val[i0].key.key_val == NULL) {value[i0].key = NULL;}
    else {value[i0].key = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].key.key_len * sizeof(char));
    	if (value[i0].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].key, result_rpc->value.cell_rpc_ptr_val[i0].key.key_val, result_rpc->value.cell_rpc_ptr_val[i0].key.key_len);
    }
    if (result_rpc->value.cell_rpc_ptr_val[i0].value.value_val == NULL) {value[i0].value = NULL;}
    else {value[i0].value = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].value.value_len * sizeof(char));
    	if (value[i0].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].value, result_rpc->value.cell_rpc_ptr_val[i0].value.value_val, result_rpc->value.cell_rpc_ptr_val[i0].value.value_len);
    }
            }
        }
    return value;
}

int set_basic_authed_hosts_cells_wrapper(struct cell* value,int len)
{
    void *result;
    cell_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.cell_rpc_ptr_len=0;
    value_rpc_ptr.cell_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.cell_rpc_ptr_len=1;
    value_rpc_ptr.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
    if(value_rpc_ptr.cell_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.cell_rpc_ptr_len;i0++) {
    }
    }
    result = set_basic_authed_hosts_cells_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void* get_basic_authed_hosts_cells_key_wrapper(size_t *out_size)
{
    basic_authed_hosts_cells_key_ret_t *result;
    void *ret_val = NULL;
    result = get_basic_authed_hosts_cells_key_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_basic_authed_hosts_cells_key_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_basic_authed_hosts_cells_key_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

void* get_basic_authed_hosts_cells_value_wrapper(size_t *out_size)
{
    basic_authed_hosts_cells_value_ret_t *result;
    void *ret_val = NULL;
    result = get_basic_authed_hosts_cells_value_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_basic_authed_hosts_cells_value_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_basic_authed_hosts_cells_value_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_basic_authed_hosts_size_wrapper()
{
    basic_authed_hosts_size_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_basic_authed_hosts_size_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_basic_authed_hosts_size_wrapper(int value)
{
    void *result;
    result = set_basic_authed_hosts_size_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_basic_authed_hosts_count_wrapper()
{
    basic_authed_hosts_count_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_basic_authed_hosts_count_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_basic_authed_hosts_count_wrapper(int value)
{
    void *result;
    result = set_basic_authed_hosts_count_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_basic_authed_hosts_resize_threshold_wrapper()
{
    basic_authed_hosts_resize_threshold_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_basic_authed_hosts_resize_threshold_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_basic_authed_hosts_resize_threshold_wrapper(int value)
{
    void *result;
    result = set_basic_authed_hosts_resize_threshold_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_basic_authed_hosts_prime_offset_wrapper()
{
    basic_authed_hosts_prime_offset_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_basic_authed_hosts_prime_offset_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_basic_authed_hosts_prime_offset_wrapper(int value)
{
    void *result;
    result = set_basic_authed_hosts_prime_offset_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

FILE* get_warc_current_file_wrapper()
{
    char_ptr *result;
    FILE *ret_val = NULL;
    result = get_warc_current_file_1(clnt);
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

void set_warc_current_file_wrapper(FILE *file)
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
    result = set_warc_current_file_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
    }
}

int get_rnd_seeded_wrapper()
{
    rnd_seeded_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_rnd_seeded_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_rnd_seeded_wrapper(int value)
{
    void *result;
    result = set_rnd_seeded_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

int get_redirect_request_wrapper()
{
    redirect_request_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_redirect_request_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_redirect_request_wrapper(int value)
{
    void *result;
    result = set_redirect_request_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

char* get_warc_current_warcinfo_uuid_str_wrapper()
{
    warc_current_warcinfo_uuid_str_array *result;
    static char local_copy[48];
    result = get_warc_current_warcinfo_uuid_str_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        memset(local_copy, 0, 48);
        return local_copy;
    }
    strncpy(local_copy, result->warc_current_warcinfo_uuid_str, 48);
    local_copy[48 - 1] = '\0'; // 确保字符串正确终止
    return local_copy;
}

int set_warc_current_warcinfo_uuid_str_wrapper(const char *value)
{
    warc_current_warcinfo_uuid_str_array param;
    void *result;
    if (value == NULL) {
        param.warc_current_warcinfo_uuid_str[0] = '\0';
    } else {
        strncpy(param.warc_current_warcinfo_uuid_str, value, 48);
        param.warc_current_warcinfo_uuid_str[48 - 1] = '\0';
    }
    result = set_warc_current_warcinfo_uuid_str_1(param, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
        return 0;
    }
    return 1;
}

char* get_exec_name_wrapper()
{
    char_ptr *result;
    char *ret_val = NULL;
    result = get_exec_name_1(clnt);
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

void set_exec_name_wrapper(const char *value)
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
    result = set_exec_name_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
    }
}

char* get_yytext_wrapper()
{
    char_ptr *result;
    char *ret_val = NULL;
    result = get_yytext_1(clnt);
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

void set_yytext_wrapper(const char *value)
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
    result = set_yytext_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
    }
}

struct hash_table* get_nonexisting_urls_set_wrapper()
{
    nonexisting_urls_set_ret_t *result_rpc;
    struct hash_table* value = {0};
    result_rpc = get_nonexisting_urls_set_1(clnt);
        if(result_rpc->value.hash_table_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.hash_table_rpc_ptr_len*sizeof(struct hash_table));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.hash_table_rpc_ptr_len;i0++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {value[i0].cells = NULL;}
    else {
    	value[i0].cells = (struct cell *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof( struct cell));
    	if (value[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {value[i0].cells[i1].key = NULL;}
    else {value[i0].cells[i1].key = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len * sizeof(char));
    	if (value[i0].cells[i1].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].key, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);
    }
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {value[i0].cells[i1].value = NULL;}
    else {value[i0].cells[i1].value = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len * sizeof(char));
    	if (value[i0].cells[i1].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].value, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);
    }
    	}
    }
    value[i0].size=result_rpc->value.hash_table_rpc_ptr_val[i0].size;
    value[i0].count=result_rpc->value.hash_table_rpc_ptr_val[i0].count;
    value[i0].resize_threshold=result_rpc->value.hash_table_rpc_ptr_val[i0].resize_threshold;
    value[i0].prime_offset=result_rpc->value.hash_table_rpc_ptr_val[i0].prime_offset;
            }
        }
    return value;
}

int set_nonexisting_urls_set_wrapper(struct hash_table* value,int len)
{
    void *result;
    hash_table_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.hash_table_rpc_ptr_len=0;
    value_rpc_ptr.hash_table_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.hash_table_rpc_ptr_len=1;
    value_rpc_ptr.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
    if(value_rpc_ptr.hash_table_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.hash_table_rpc_ptr_len;i0++) {
    if (value[i0].cells == NULL) {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}
    else {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));
    	if (value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    	}
    }
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].size=value[i0].size;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].count=value[i0].count;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].resize_threshold=value[i0].resize_threshold;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].prime_offset=value[i0].prime_offset;
    }
    }
    result = set_nonexisting_urls_set_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void * get_nonexisting_urls_set_hash_function_wrapper()
{
    char_ptr *result;
    result = get_nonexisting_urls_set_hash_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_nonexisting_urls_set_hash_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_nonexisting_urls_set_hash_function_1(value, clnt);
}

void * get_nonexisting_urls_set_test_function_wrapper()
{
    char_ptr *result;
    result = get_nonexisting_urls_set_test_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_nonexisting_urls_set_test_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_nonexisting_urls_set_test_function_1(value, clnt);
}

struct cell* get_nonexisting_urls_set_cells_wrapper()
{
    nonexisting_urls_set_cells_ret_t *result_rpc;
    struct cell* value = {0};
    result_rpc = get_nonexisting_urls_set_cells_1(clnt);
        if(result_rpc->value.cell_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.cell_rpc_ptr_len*sizeof(struct cell));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.cell_rpc_ptr_len;i0++) {
    if (result_rpc->value.cell_rpc_ptr_val[i0].key.key_val == NULL) {value[i0].key = NULL;}
    else {value[i0].key = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].key.key_len * sizeof(char));
    	if (value[i0].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].key, result_rpc->value.cell_rpc_ptr_val[i0].key.key_val, result_rpc->value.cell_rpc_ptr_val[i0].key.key_len);
    }
    if (result_rpc->value.cell_rpc_ptr_val[i0].value.value_val == NULL) {value[i0].value = NULL;}
    else {value[i0].value = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].value.value_len * sizeof(char));
    	if (value[i0].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].value, result_rpc->value.cell_rpc_ptr_val[i0].value.value_val, result_rpc->value.cell_rpc_ptr_val[i0].value.value_len);
    }
            }
        }
    return value;
}

int set_nonexisting_urls_set_cells_wrapper(struct cell* value,int len)
{
    void *result;
    cell_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.cell_rpc_ptr_len=0;
    value_rpc_ptr.cell_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.cell_rpc_ptr_len=1;
    value_rpc_ptr.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
    if(value_rpc_ptr.cell_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.cell_rpc_ptr_len;i0++) {
    }
    }
    result = set_nonexisting_urls_set_cells_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void* get_nonexisting_urls_set_cells_key_wrapper(size_t *out_size)
{
    nonexisting_urls_set_cells_key_ret_t *result;
    void *ret_val = NULL;
    result = get_nonexisting_urls_set_cells_key_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_nonexisting_urls_set_cells_key_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_nonexisting_urls_set_cells_key_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

void* get_nonexisting_urls_set_cells_value_wrapper(size_t *out_size)
{
    nonexisting_urls_set_cells_value_ret_t *result;
    void *ret_val = NULL;
    result = get_nonexisting_urls_set_cells_value_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_nonexisting_urls_set_cells_value_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_nonexisting_urls_set_cells_value_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_nonexisting_urls_set_size_wrapper()
{
    nonexisting_urls_set_size_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_nonexisting_urls_set_size_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_nonexisting_urls_set_size_wrapper(int value)
{
    void *result;
    result = set_nonexisting_urls_set_size_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_nonexisting_urls_set_count_wrapper()
{
    nonexisting_urls_set_count_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_nonexisting_urls_set_count_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_nonexisting_urls_set_count_wrapper(int value)
{
    void *result;
    result = set_nonexisting_urls_set_count_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_nonexisting_urls_set_resize_threshold_wrapper()
{
    nonexisting_urls_set_resize_threshold_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_nonexisting_urls_set_resize_threshold_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_nonexisting_urls_set_resize_threshold_wrapper(int value)
{
    void *result;
    result = set_nonexisting_urls_set_resize_threshold_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_nonexisting_urls_set_prime_offset_wrapper()
{
    nonexisting_urls_set_prime_offset_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_nonexisting_urls_set_prime_offset_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_nonexisting_urls_set_prime_offset_wrapper(int value)
{
    void *result;
    result = set_nonexisting_urls_set_prime_offset_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

struct hash_table* get_interesting_attributes_wrapper()
{
    interesting_attributes_ret_t *result_rpc;
    struct hash_table* value = {0};
    result_rpc = get_interesting_attributes_1(clnt);
        if(result_rpc->value.hash_table_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.hash_table_rpc_ptr_len*sizeof(struct hash_table));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.hash_table_rpc_ptr_len;i0++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {value[i0].cells = NULL;}
    else {
    	value[i0].cells = (struct cell *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof( struct cell));
    	if (value[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {value[i0].cells[i1].key = NULL;}
    else {value[i0].cells[i1].key = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len * sizeof(char));
    	if (value[i0].cells[i1].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].key, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);
    }
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {value[i0].cells[i1].value = NULL;}
    else {value[i0].cells[i1].value = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len * sizeof(char));
    	if (value[i0].cells[i1].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].value, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);
    }
    	}
    }
    value[i0].size=result_rpc->value.hash_table_rpc_ptr_val[i0].size;
    value[i0].count=result_rpc->value.hash_table_rpc_ptr_val[i0].count;
    value[i0].resize_threshold=result_rpc->value.hash_table_rpc_ptr_val[i0].resize_threshold;
    value[i0].prime_offset=result_rpc->value.hash_table_rpc_ptr_val[i0].prime_offset;
            }
        }
    return value;
}

int set_interesting_attributes_wrapper(struct hash_table* value,int len)
{
    void *result;
    hash_table_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.hash_table_rpc_ptr_len=0;
    value_rpc_ptr.hash_table_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.hash_table_rpc_ptr_len=1;
    value_rpc_ptr.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
    if(value_rpc_ptr.hash_table_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.hash_table_rpc_ptr_len;i0++) {
    if (value[i0].cells == NULL) {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}
    else {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));
    	if (value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    	}
    }
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].size=value[i0].size;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].count=value[i0].count;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].resize_threshold=value[i0].resize_threshold;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].prime_offset=value[i0].prime_offset;
    }
    }
    result = set_interesting_attributes_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void * get_interesting_attributes_hash_function_wrapper()
{
    char_ptr *result;
    result = get_interesting_attributes_hash_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_interesting_attributes_hash_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_interesting_attributes_hash_function_1(value, clnt);
}

void * get_interesting_attributes_test_function_wrapper()
{
    char_ptr *result;
    result = get_interesting_attributes_test_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_interesting_attributes_test_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_interesting_attributes_test_function_1(value, clnt);
}

struct cell* get_interesting_attributes_cells_wrapper()
{
    interesting_attributes_cells_ret_t *result_rpc;
    struct cell* value = {0};
    result_rpc = get_interesting_attributes_cells_1(clnt);
        if(result_rpc->value.cell_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.cell_rpc_ptr_len*sizeof(struct cell));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.cell_rpc_ptr_len;i0++) {
    if (result_rpc->value.cell_rpc_ptr_val[i0].key.key_val == NULL) {value[i0].key = NULL;}
    else {value[i0].key = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].key.key_len * sizeof(char));
    	if (value[i0].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].key, result_rpc->value.cell_rpc_ptr_val[i0].key.key_val, result_rpc->value.cell_rpc_ptr_val[i0].key.key_len);
    }
    if (result_rpc->value.cell_rpc_ptr_val[i0].value.value_val == NULL) {value[i0].value = NULL;}
    else {value[i0].value = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].value.value_len * sizeof(char));
    	if (value[i0].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].value, result_rpc->value.cell_rpc_ptr_val[i0].value.value_val, result_rpc->value.cell_rpc_ptr_val[i0].value.value_len);
    }
            }
        }
    return value;
}

int set_interesting_attributes_cells_wrapper(struct cell* value,int len)
{
    void *result;
    cell_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.cell_rpc_ptr_len=0;
    value_rpc_ptr.cell_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.cell_rpc_ptr_len=1;
    value_rpc_ptr.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
    if(value_rpc_ptr.cell_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.cell_rpc_ptr_len;i0++) {
    }
    }
    result = set_interesting_attributes_cells_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void* get_interesting_attributes_cells_key_wrapper(size_t *out_size)
{
    interesting_attributes_cells_key_ret_t *result;
    void *ret_val = NULL;
    result = get_interesting_attributes_cells_key_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_interesting_attributes_cells_key_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_interesting_attributes_cells_key_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

void* get_interesting_attributes_cells_value_wrapper(size_t *out_size)
{
    interesting_attributes_cells_value_ret_t *result;
    void *ret_val = NULL;
    result = get_interesting_attributes_cells_value_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_interesting_attributes_cells_value_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_interesting_attributes_cells_value_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_interesting_attributes_size_wrapper()
{
    interesting_attributes_size_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_interesting_attributes_size_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_interesting_attributes_size_wrapper(int value)
{
    void *result;
    result = set_interesting_attributes_size_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_interesting_attributes_count_wrapper()
{
    interesting_attributes_count_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_interesting_attributes_count_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_interesting_attributes_count_wrapper(int value)
{
    void *result;
    result = set_interesting_attributes_count_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_interesting_attributes_resize_threshold_wrapper()
{
    interesting_attributes_resize_threshold_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_interesting_attributes_resize_threshold_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_interesting_attributes_resize_threshold_wrapper(int value)
{
    void *result;
    result = set_interesting_attributes_resize_threshold_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_interesting_attributes_prime_offset_wrapper()
{
    interesting_attributes_prime_offset_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_interesting_attributes_prime_offset_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_interesting_attributes_prime_offset_wrapper(int value)
{
    void *result;
    result = set_interesting_attributes_prime_offset_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

struct new_pconn get_pconn_wrapper()
{
    pconn_ret_t *result_rpc;
    struct new_pconn default_value = {0};
    result_rpc = get_pconn_1(clnt);
    if (result_rpc == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    default_value.socket=result_rpc->value.socket;
    if (result_rpc->value.host.charptr.charptr_val == NULL) {default_value.host=NULL;}

    else {default_value.host=malloc(result_rpc->value.host.charptr.charptr_len);

    memcpy(default_value.host,result_rpc->value.host.charptr.charptr_val,result_rpc->value.host.charptr.charptr_len);

    }

    default_value.port=result_rpc->value.port;
    default_value.ssl=result_rpc->value.ssl;
    default_value.authorized=result_rpc->value.authorized;
    default_value.ntlm.state=result_rpc->value.ntlm.state;
    return default_value;
}

int set_pconn_wrapper(struct new_pconn pconn)
{
    void *result;
    new_pconn_rpc pconn_rpc;
    pconn_rpc.socket=pconn.socket;
    if (pconn.host == NULL) {pconn_rpc.host.charptr.charptr_len = 0;pconn_rpc.host.charptr.charptr_val = NULL;}
    else {pconn_rpc.host.charptr.charptr_len = strlen(pconn.host) + 1;

    	pconn_rpc.host.charptr.charptr_val = malloc(pconn_rpc.host.charptr.charptr_len * sizeof(char));
    	if (pconn_rpc.host.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(pconn_rpc.host.charptr.charptr_val, pconn.host, pconn_rpc.host.charptr.charptr_len);
    }
    pconn_rpc.port=pconn.port;
    pconn_rpc.ssl=pconn.ssl;
    pconn_rpc.authorized=pconn.authorized;
    pconn_rpc.ntlm.state=pconn.ntlm.state;
    result = set_pconn_1(pconn_rpc, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

int get_pconn_socket_wrapper()
{
    pconn_socket_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_pconn_socket_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_pconn_socket_wrapper(int value)
{
    void *result;
    result = set_pconn_socket_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

char * get_pconn_host_wrapper()
{
    pconn_host_ret_t *result;
    char * ret_val = NULL;
    result = get_pconn_host_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_pconn_host_wrapper(const char *value)
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
    result = set_pconn_host_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_pconn_port_wrapper()
{
    pconn_port_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_pconn_port_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_pconn_port_wrapper(int value)
{
    void *result;
    result = set_pconn_port_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_pconn_ssl_wrapper()
{
    pconn_ssl_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_pconn_ssl_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_pconn_ssl_wrapper(bool value)
{
    void *result;
    result = set_pconn_ssl_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_pconn_authorized_wrapper()
{
    pconn_authorized_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_pconn_authorized_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_pconn_authorized_wrapper(bool value)
{
    void *result;
    result = set_pconn_authorized_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

struct ntlmdata get_pconn_ntlm_wrapper()
{
    pconn_ntlm_ret_t *result_rpc;
    struct ntlmdata default_value = {0};
    result_rpc = get_pconn_ntlm_1(clnt);
    if (result_rpc == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    default_value.state=result_rpc->value.state;
    return default_value;
}

int set_pconn_ntlm_wrapper(struct ntlmdata pconn_ntlm)
{
    void *result;
    ntlmdata_rpc pconn_ntlm_rpc;
    pconn_ntlm_rpc.state=pconn_ntlm.state;
    result = set_pconn_ntlm_1(pconn_ntlm_rpc, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

wgetntlm get_pconn_ntlm_state_wrapper()
{
    pconn_ntlm_state_ret_t *result;
    wgetntlm ret_val = 0; // 默认值
    result = get_pconn_ntlm_state_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_pconn_ntlm_state_wrapper(wgetntlm value)
{
    void *result;
    result = set_pconn_ntlm_state_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

struct hash_table* get_host_name_addresses_map_wrapper()
{
    host_name_addresses_map_ret_t *result_rpc;
    struct hash_table* value = {0};
    result_rpc = get_host_name_addresses_map_1(clnt);
        if(result_rpc->value.hash_table_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.hash_table_rpc_ptr_len*sizeof(struct hash_table));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.hash_table_rpc_ptr_len;i0++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {value[i0].cells = NULL;}
    else {
    	value[i0].cells = (struct cell *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof( struct cell));
    	if (value[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {value[i0].cells[i1].key = NULL;}
    else {value[i0].cells[i1].key = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len * sizeof(char));
    	if (value[i0].cells[i1].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].key, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);
    }
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {value[i0].cells[i1].value = NULL;}
    else {value[i0].cells[i1].value = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len * sizeof(char));
    	if (value[i0].cells[i1].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].value, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);
    }
    	}
    }
    value[i0].size=result_rpc->value.hash_table_rpc_ptr_val[i0].size;
    value[i0].count=result_rpc->value.hash_table_rpc_ptr_val[i0].count;
    value[i0].resize_threshold=result_rpc->value.hash_table_rpc_ptr_val[i0].resize_threshold;
    value[i0].prime_offset=result_rpc->value.hash_table_rpc_ptr_val[i0].prime_offset;
            }
        }
    return value;
}

int set_host_name_addresses_map_wrapper(struct hash_table* value,int len)
{
    void *result;
    hash_table_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.hash_table_rpc_ptr_len=0;
    value_rpc_ptr.hash_table_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.hash_table_rpc_ptr_len=1;
    value_rpc_ptr.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
    if(value_rpc_ptr.hash_table_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.hash_table_rpc_ptr_len;i0++) {
    if (value[i0].cells == NULL) {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}
    else {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));
    	if (value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    	}
    }
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].size=value[i0].size;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].count=value[i0].count;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].resize_threshold=value[i0].resize_threshold;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].prime_offset=value[i0].prime_offset;
    }
    }
    result = set_host_name_addresses_map_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void * get_host_name_addresses_map_hash_function_wrapper()
{
    char_ptr *result;
    result = get_host_name_addresses_map_hash_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_host_name_addresses_map_hash_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_host_name_addresses_map_hash_function_1(value, clnt);
}

void * get_host_name_addresses_map_test_function_wrapper()
{
    char_ptr *result;
    result = get_host_name_addresses_map_test_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_host_name_addresses_map_test_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_host_name_addresses_map_test_function_1(value, clnt);
}

struct cell* get_host_name_addresses_map_cells_wrapper()
{
    host_name_addresses_map_cells_ret_t *result_rpc;
    struct cell* value = {0};
    result_rpc = get_host_name_addresses_map_cells_1(clnt);
        if(result_rpc->value.cell_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.cell_rpc_ptr_len*sizeof(struct cell));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.cell_rpc_ptr_len;i0++) {
    if (result_rpc->value.cell_rpc_ptr_val[i0].key.key_val == NULL) {value[i0].key = NULL;}
    else {value[i0].key = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].key.key_len * sizeof(char));
    	if (value[i0].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].key, result_rpc->value.cell_rpc_ptr_val[i0].key.key_val, result_rpc->value.cell_rpc_ptr_val[i0].key.key_len);
    }
    if (result_rpc->value.cell_rpc_ptr_val[i0].value.value_val == NULL) {value[i0].value = NULL;}
    else {value[i0].value = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].value.value_len * sizeof(char));
    	if (value[i0].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].value, result_rpc->value.cell_rpc_ptr_val[i0].value.value_val, result_rpc->value.cell_rpc_ptr_val[i0].value.value_len);
    }
            }
        }
    return value;
}

int set_host_name_addresses_map_cells_wrapper(struct cell* value,int len)
{
    void *result;
    cell_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.cell_rpc_ptr_len=0;
    value_rpc_ptr.cell_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.cell_rpc_ptr_len=1;
    value_rpc_ptr.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
    if(value_rpc_ptr.cell_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.cell_rpc_ptr_len;i0++) {
    }
    }
    result = set_host_name_addresses_map_cells_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void* get_host_name_addresses_map_cells_key_wrapper(size_t *out_size)
{
    host_name_addresses_map_cells_key_ret_t *result;
    void *ret_val = NULL;
    result = get_host_name_addresses_map_cells_key_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_host_name_addresses_map_cells_key_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_host_name_addresses_map_cells_key_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

void* get_host_name_addresses_map_cells_value_wrapper(size_t *out_size)
{
    host_name_addresses_map_cells_value_ret_t *result;
    void *ret_val = NULL;
    result = get_host_name_addresses_map_cells_value_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_host_name_addresses_map_cells_value_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_host_name_addresses_map_cells_value_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_host_name_addresses_map_size_wrapper()
{
    host_name_addresses_map_size_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_host_name_addresses_map_size_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_host_name_addresses_map_size_wrapper(int value)
{
    void *result;
    result = set_host_name_addresses_map_size_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_host_name_addresses_map_count_wrapper()
{
    host_name_addresses_map_count_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_host_name_addresses_map_count_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_host_name_addresses_map_count_wrapper(int value)
{
    void *result;
    result = set_host_name_addresses_map_count_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_host_name_addresses_map_resize_threshold_wrapper()
{
    host_name_addresses_map_resize_threshold_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_host_name_addresses_map_resize_threshold_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_host_name_addresses_map_resize_threshold_wrapper(int value)
{
    void *result;
    result = set_host_name_addresses_map_resize_threshold_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_host_name_addresses_map_prime_offset_wrapper()
{
    host_name_addresses_map_prime_offset_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_host_name_addresses_map_prime_offset_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_host_name_addresses_map_prime_offset_wrapper(int value)
{
    void *result;
    result = set_host_name_addresses_map_prime_offset_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

struct new_limit_data get_limit_data_wrapper()
{
    limit_data_ret_t *result_rpc;
    struct new_limit_data default_value = {0};
    result_rpc = get_limit_data_1(clnt);
    if (result_rpc == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    default_value.chunk_bytes=result_rpc->value.chunk_bytes;
    default_value.chunk_start=result_rpc->value.chunk_start;
    default_value.sleep_adjust=result_rpc->value.sleep_adjust;
    return default_value;
}

int set_limit_data_wrapper(struct new_limit_data limit_data)
{
    void *result;
    new_limit_data_rpc limit_data_rpc;
    limit_data_rpc.chunk_bytes=limit_data.chunk_bytes;
    limit_data_rpc.chunk_start=limit_data.chunk_start;
    limit_data_rpc.sleep_adjust=limit_data.sleep_adjust;
    result = set_limit_data_1(limit_data_rpc, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

wgint get_limit_data_chunk_bytes_wrapper()
{
    limit_data_chunk_bytes_ret_t *result;
    wgint ret_val = 0; // 默认值
    result = get_limit_data_chunk_bytes_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_limit_data_chunk_bytes_wrapper(wgint value)
{
    void *result;
    result = set_limit_data_chunk_bytes_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

double get_limit_data_chunk_start_wrapper()
{
    limit_data_chunk_start_ret_t *result;
    double ret_val = 0; // 默认值
    result = get_limit_data_chunk_start_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_limit_data_chunk_start_wrapper(double value)
{
    void *result;
    result = set_limit_data_chunk_start_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

double get_limit_data_sleep_adjust_wrapper()
{
    limit_data_sleep_adjust_ret_t *result;
    double ret_val = 0; // 默认值
    result = get_limit_data_sleep_adjust_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_limit_data_sleep_adjust_wrapper(double value)
{
    void *result;
    result = set_limit_data_sleep_adjust_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_screen_width_wrapper()
{
    screen_width_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_screen_width_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_screen_width_wrapper(int value)
{
    void *result;
    result = set_screen_width_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

int get_current_impl_locked_wrapper()
{
    current_impl_locked_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_current_impl_locked_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_current_impl_locked_wrapper(int value)
{
    void *result;
    result = set_current_impl_locked_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

FILE* get_warc_current_cdx_file_wrapper()
{
    char_ptr *result;
    FILE *ret_val = NULL;
    result = get_warc_current_cdx_file_1(clnt);
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

void set_warc_current_cdx_file_wrapper(FILE *file)
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
    result = set_warc_current_cdx_file_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
    }
}

FILE* get_warclogfp_wrapper()
{
    char_ptr *result;
    FILE *ret_val = NULL;
    result = get_warclogfp_1(clnt);
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

void set_warclogfp_wrapper(FILE *file)
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
    result = set_warclogfp_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
    }
}

FILE* get_warc_manifest_fp_wrapper()
{
    char_ptr *result;
    FILE *ret_val = NULL;
    result = get_warc_manifest_fp_1(clnt);
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

void set_warc_manifest_fp_wrapper(FILE *file)
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
    result = set_warc_manifest_fp_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
    }
}

struct hash_table* get_downloaded_html_set_wrapper()
{
    downloaded_html_set_ret_t *result_rpc;
    struct hash_table* value = {0};
    result_rpc = get_downloaded_html_set_1(clnt);
        if(result_rpc->value.hash_table_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.hash_table_rpc_ptr_len*sizeof(struct hash_table));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.hash_table_rpc_ptr_len;i0++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {value[i0].cells = NULL;}
    else {
    	value[i0].cells = (struct cell *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof( struct cell));
    	if (value[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {value[i0].cells[i1].key = NULL;}
    else {value[i0].cells[i1].key = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len * sizeof(char));
    	if (value[i0].cells[i1].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].key, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);
    }
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {value[i0].cells[i1].value = NULL;}
    else {value[i0].cells[i1].value = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len * sizeof(char));
    	if (value[i0].cells[i1].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].value, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);
    }
    	}
    }
    value[i0].size=result_rpc->value.hash_table_rpc_ptr_val[i0].size;
    value[i0].count=result_rpc->value.hash_table_rpc_ptr_val[i0].count;
    value[i0].resize_threshold=result_rpc->value.hash_table_rpc_ptr_val[i0].resize_threshold;
    value[i0].prime_offset=result_rpc->value.hash_table_rpc_ptr_val[i0].prime_offset;
            }
        }
    return value;
}

int set_downloaded_html_set_wrapper(struct hash_table* value,int len)
{
    void *result;
    hash_table_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.hash_table_rpc_ptr_len=0;
    value_rpc_ptr.hash_table_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.hash_table_rpc_ptr_len=1;
    value_rpc_ptr.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
    if(value_rpc_ptr.hash_table_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.hash_table_rpc_ptr_len;i0++) {
    if (value[i0].cells == NULL) {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}
    else {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));
    	if (value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    	}
    }
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].size=value[i0].size;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].count=value[i0].count;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].resize_threshold=value[i0].resize_threshold;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].prime_offset=value[i0].prime_offset;
    }
    }
    result = set_downloaded_html_set_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void * get_downloaded_html_set_hash_function_wrapper()
{
    char_ptr *result;
    result = get_downloaded_html_set_hash_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_downloaded_html_set_hash_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_downloaded_html_set_hash_function_1(value, clnt);
}

void * get_downloaded_html_set_test_function_wrapper()
{
    char_ptr *result;
    result = get_downloaded_html_set_test_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_downloaded_html_set_test_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_downloaded_html_set_test_function_1(value, clnt);
}

struct cell* get_downloaded_html_set_cells_wrapper()
{
    downloaded_html_set_cells_ret_t *result_rpc;
    struct cell* value = {0};
    result_rpc = get_downloaded_html_set_cells_1(clnt);
        if(result_rpc->value.cell_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.cell_rpc_ptr_len*sizeof(struct cell));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.cell_rpc_ptr_len;i0++) {
    if (result_rpc->value.cell_rpc_ptr_val[i0].key.key_val == NULL) {value[i0].key = NULL;}
    else {value[i0].key = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].key.key_len * sizeof(char));
    	if (value[i0].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].key, result_rpc->value.cell_rpc_ptr_val[i0].key.key_val, result_rpc->value.cell_rpc_ptr_val[i0].key.key_len);
    }
    if (result_rpc->value.cell_rpc_ptr_val[i0].value.value_val == NULL) {value[i0].value = NULL;}
    else {value[i0].value = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].value.value_len * sizeof(char));
    	if (value[i0].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].value, result_rpc->value.cell_rpc_ptr_val[i0].value.value_val, result_rpc->value.cell_rpc_ptr_val[i0].value.value_len);
    }
            }
        }
    return value;
}

int set_downloaded_html_set_cells_wrapper(struct cell* value,int len)
{
    void *result;
    cell_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.cell_rpc_ptr_len=0;
    value_rpc_ptr.cell_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.cell_rpc_ptr_len=1;
    value_rpc_ptr.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
    if(value_rpc_ptr.cell_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.cell_rpc_ptr_len;i0++) {
    }
    }
    result = set_downloaded_html_set_cells_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void* get_downloaded_html_set_cells_key_wrapper(size_t *out_size)
{
    downloaded_html_set_cells_key_ret_t *result;
    void *ret_val = NULL;
    result = get_downloaded_html_set_cells_key_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_downloaded_html_set_cells_key_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_downloaded_html_set_cells_key_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

void* get_downloaded_html_set_cells_value_wrapper(size_t *out_size)
{
    downloaded_html_set_cells_value_ret_t *result;
    void *ret_val = NULL;
    result = get_downloaded_html_set_cells_value_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_downloaded_html_set_cells_value_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_downloaded_html_set_cells_value_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_downloaded_html_set_size_wrapper()
{
    downloaded_html_set_size_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_downloaded_html_set_size_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_downloaded_html_set_size_wrapper(int value)
{
    void *result;
    result = set_downloaded_html_set_size_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_downloaded_html_set_count_wrapper()
{
    downloaded_html_set_count_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_downloaded_html_set_count_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_downloaded_html_set_count_wrapper(int value)
{
    void *result;
    result = set_downloaded_html_set_count_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_downloaded_html_set_resize_threshold_wrapper()
{
    downloaded_html_set_resize_threshold_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_downloaded_html_set_resize_threshold_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_downloaded_html_set_resize_threshold_wrapper(int value)
{
    void *result;
    result = set_downloaded_html_set_resize_threshold_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_downloaded_html_set_prime_offset_wrapper()
{
    downloaded_html_set_prime_offset_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_downloaded_html_set_prime_offset_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_downloaded_html_set_prime_offset_wrapper(int value)
{
    void *result;
    result = set_downloaded_html_set_prime_offset_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_numurls_wrapper()
{
    numurls_ret_t *result;
    int default_value = {0}; // 初始化默认值
    result = get_numurls_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_numurls_wrapper(int value)
{
    void *result;
    result = set_numurls_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

char* get_program_argstring_wrapper()
{
    char_ptr *result;
    char *ret_val = NULL;
    result = get_program_argstring_1(clnt);
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

void set_program_argstring_wrapper(const char *value)
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
    result = set_program_argstring_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
    }
}

struct hash_table* get_warc_cdx_dedup_table_wrapper()
{
    warc_cdx_dedup_table_ret_t *result_rpc;
    struct hash_table* value = {0};
    result_rpc = get_warc_cdx_dedup_table_1(clnt);
        if(result_rpc->value.hash_table_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.hash_table_rpc_ptr_len*sizeof(struct hash_table));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.hash_table_rpc_ptr_len;i0++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {value[i0].cells = NULL;}
    else {
    	value[i0].cells = (struct cell *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof( struct cell));
    	if (value[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {value[i0].cells[i1].key = NULL;}
    else {value[i0].cells[i1].key = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len * sizeof(char));
    	if (value[i0].cells[i1].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].key, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);
    }
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {value[i0].cells[i1].value = NULL;}
    else {value[i0].cells[i1].value = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len * sizeof(char));
    	if (value[i0].cells[i1].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].value, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);
    }
    	}
    }
    value[i0].size=result_rpc->value.hash_table_rpc_ptr_val[i0].size;
    value[i0].count=result_rpc->value.hash_table_rpc_ptr_val[i0].count;
    value[i0].resize_threshold=result_rpc->value.hash_table_rpc_ptr_val[i0].resize_threshold;
    value[i0].prime_offset=result_rpc->value.hash_table_rpc_ptr_val[i0].prime_offset;
            }
        }
    return value;
}

int set_warc_cdx_dedup_table_wrapper(struct hash_table* value,int len)
{
    void *result;
    hash_table_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.hash_table_rpc_ptr_len=0;
    value_rpc_ptr.hash_table_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.hash_table_rpc_ptr_len=1;
    value_rpc_ptr.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
    if(value_rpc_ptr.hash_table_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.hash_table_rpc_ptr_len;i0++) {
    if (value[i0].cells == NULL) {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}
    else {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));
    	if (value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    	}
    }
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].size=value[i0].size;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].count=value[i0].count;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].resize_threshold=value[i0].resize_threshold;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].prime_offset=value[i0].prime_offset;
    }
    }
    result = set_warc_cdx_dedup_table_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void * get_warc_cdx_dedup_table_hash_function_wrapper()
{
    char_ptr *result;
    result = get_warc_cdx_dedup_table_hash_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_warc_cdx_dedup_table_hash_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_warc_cdx_dedup_table_hash_function_1(value, clnt);
}

void * get_warc_cdx_dedup_table_test_function_wrapper()
{
    char_ptr *result;
    result = get_warc_cdx_dedup_table_test_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_warc_cdx_dedup_table_test_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_warc_cdx_dedup_table_test_function_1(value, clnt);
}

struct cell* get_warc_cdx_dedup_table_cells_wrapper()
{
    warc_cdx_dedup_table_cells_ret_t *result_rpc;
    struct cell* value = {0};
    result_rpc = get_warc_cdx_dedup_table_cells_1(clnt);
        if(result_rpc->value.cell_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.cell_rpc_ptr_len*sizeof(struct cell));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.cell_rpc_ptr_len;i0++) {
    if (result_rpc->value.cell_rpc_ptr_val[i0].key.key_val == NULL) {value[i0].key = NULL;}
    else {value[i0].key = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].key.key_len * sizeof(char));
    	if (value[i0].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].key, result_rpc->value.cell_rpc_ptr_val[i0].key.key_val, result_rpc->value.cell_rpc_ptr_val[i0].key.key_len);
    }
    if (result_rpc->value.cell_rpc_ptr_val[i0].value.value_val == NULL) {value[i0].value = NULL;}
    else {value[i0].value = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].value.value_len * sizeof(char));
    	if (value[i0].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].value, result_rpc->value.cell_rpc_ptr_val[i0].value.value_val, result_rpc->value.cell_rpc_ptr_val[i0].value.value_len);
    }
            }
        }
    return value;
}

int set_warc_cdx_dedup_table_cells_wrapper(struct cell* value,int len)
{
    void *result;
    cell_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.cell_rpc_ptr_len=0;
    value_rpc_ptr.cell_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.cell_rpc_ptr_len=1;
    value_rpc_ptr.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
    if(value_rpc_ptr.cell_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.cell_rpc_ptr_len;i0++) {
    }
    }
    result = set_warc_cdx_dedup_table_cells_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void* get_warc_cdx_dedup_table_cells_key_wrapper(size_t *out_size)
{
    warc_cdx_dedup_table_cells_key_ret_t *result;
    void *ret_val = NULL;
    result = get_warc_cdx_dedup_table_cells_key_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_warc_cdx_dedup_table_cells_key_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_warc_cdx_dedup_table_cells_key_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

void* get_warc_cdx_dedup_table_cells_value_wrapper(size_t *out_size)
{
    warc_cdx_dedup_table_cells_value_ret_t *result;
    void *ret_val = NULL;
    result = get_warc_cdx_dedup_table_cells_value_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_warc_cdx_dedup_table_cells_value_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_warc_cdx_dedup_table_cells_value_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_warc_cdx_dedup_table_size_wrapper()
{
    warc_cdx_dedup_table_size_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_warc_cdx_dedup_table_size_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_warc_cdx_dedup_table_size_wrapper(int value)
{
    void *result;
    result = set_warc_cdx_dedup_table_size_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_warc_cdx_dedup_table_count_wrapper()
{
    warc_cdx_dedup_table_count_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_warc_cdx_dedup_table_count_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_warc_cdx_dedup_table_count_wrapper(int value)
{
    void *result;
    result = set_warc_cdx_dedup_table_count_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_warc_cdx_dedup_table_resize_threshold_wrapper()
{
    warc_cdx_dedup_table_resize_threshold_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_warc_cdx_dedup_table_resize_threshold_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_warc_cdx_dedup_table_resize_threshold_wrapper(int value)
{
    void *result;
    result = set_warc_cdx_dedup_table_resize_threshold_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_warc_cdx_dedup_table_prime_offset_wrapper()
{
    warc_cdx_dedup_table_prime_offset_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_warc_cdx_dedup_table_prime_offset_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_warc_cdx_dedup_table_prime_offset_wrapper(int value)
{
    void *result;
    result = set_warc_cdx_dedup_table_prime_offset_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

struct progress_implementation* get_current_impl_wrapper()
{
    current_impl_ret_t *result_rpc;
    struct progress_implementation* value = {0};
    result_rpc = get_current_impl_1(clnt);
        if(result_rpc->value.progress_implementation_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.progress_implementation_rpc_ptr_len*sizeof(struct progress_implementation));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.progress_implementation_rpc_ptr_len;i0++) {
    if (result_rpc->value.progress_implementation_rpc_ptr_val[i0].name.charptr.charptr_val == NULL) {value[i0].name = NULL;}
    else {value[i0].name = strdup(result_rpc->value.progress_implementation_rpc_ptr_val[i0].name.charptr.charptr_val);}
    value[i0].interactive=result_rpc->value.progress_implementation_rpc_ptr_val[i0].interactive;
            }
        }
    return value;
}

int set_current_impl_wrapper(struct progress_implementation* value,int len)
{
    void *result;
    progress_implementation_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.progress_implementation_rpc_ptr_len=0;
    value_rpc_ptr.progress_implementation_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.progress_implementation_rpc_ptr_len=1;
    value_rpc_ptr.progress_implementation_rpc_ptr_val=malloc(1*sizeof(progress_implementation_rpc));
    if(value_rpc_ptr.progress_implementation_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.progress_implementation_rpc_ptr_len;i0++) {
    if (value[i0].name == NULL) {value_rpc_ptr.progress_implementation_rpc_ptr_val[i0].name.charptr.charptr_len = 0;value_rpc_ptr.progress_implementation_rpc_ptr_val[i0].name.charptr.charptr_val = NULL;}
    else {value_rpc_ptr.progress_implementation_rpc_ptr_val[i0].name.charptr.charptr_len = strlen(value[i0].name) + 1;

    	value_rpc_ptr.progress_implementation_rpc_ptr_val[i0].name.charptr.charptr_val = malloc(value_rpc_ptr.progress_implementation_rpc_ptr_val[i0].name.charptr.charptr_len * sizeof(char));
    	if (value_rpc_ptr.progress_implementation_rpc_ptr_val[i0].name.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value_rpc_ptr.progress_implementation_rpc_ptr_val[i0].name.charptr.charptr_val, value[i0].name, value_rpc_ptr.progress_implementation_rpc_ptr_val[i0].name.charptr.charptr_len);
    }
    value_rpc_ptr.progress_implementation_rpc_ptr_val[i0].interactive=value[i0].interactive;
    }
    }
    result = set_current_impl_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

const char * get_current_impl_name_wrapper()
{
    current_impl_name_ret_t *result;
    const char * ret_val = NULL;
    result = get_current_impl_name_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    if (result->value.charptr.charptr_val != NULL) {
        ret_val = malloc(result->value.charptr.charptr_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            return NULL;
        }
        memcpy(ret_val, result->value.charptr.charptr_val, result->value.charptr.charptr_len);
    }
    return ret_val;
}

int set_current_impl_name_wrapper(const char *value)
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
    result = set_current_impl_name_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

bool get_current_impl_interactive_wrapper()
{
    current_impl_interactive_ret_t *result;
    bool ret_val = 0; // 默认值
    result = get_current_impl_interactive_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_current_impl_interactive_wrapper(bool value)
{
    void *result;
    result = set_current_impl_interactive_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

void * get_current_impl_create_wrapper()
{
    char_ptr *result;
    result = get_current_impl_create_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_current_impl_create_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_current_impl_create_1(value, clnt);
}

void * get_current_impl_update_wrapper()
{
    char_ptr *result;
    result = get_current_impl_update_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_current_impl_update_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_current_impl_update_1(value, clnt);
}

void * get_current_impl_draw_wrapper()
{
    char_ptr *result;
    result = get_current_impl_draw_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_current_impl_draw_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_current_impl_draw_1(value, clnt);
}

void * get_current_impl_finish_wrapper()
{
    char_ptr *result;
    result = get_current_impl_finish_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_current_impl_finish_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_current_impl_finish_1(value, clnt);
}

void * get_current_impl_set_params_wrapper()
{
    char_ptr *result;
    result = get_current_impl_set_params_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_current_impl_set_params_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_current_impl_set_params_1(value, clnt);
}

struct transport_implementation get_wgnutls_transport_wrapper()
{
    wgnutls_transport_ret_t *result_rpc;
    struct transport_implementation default_value = {0};
    result_rpc = get_wgnutls_transport_1(clnt);
    if (result_rpc == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return default_value;
}

int set_wgnutls_transport_wrapper(struct transport_implementation wgnutls_transport)
{
    void *result;
    transport_implementation_rpc wgnutls_transport_rpc;
    result = set_wgnutls_transport_1(wgnutls_transport_rpc, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void * get_wgnutls_transport_reader_wrapper()
{
    char_ptr *result;
    result = get_wgnutls_transport_reader_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_wgnutls_transport_reader_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_wgnutls_transport_reader_1(value, clnt);
}

void * get_wgnutls_transport_writer_wrapper()
{
    char_ptr *result;
    result = get_wgnutls_transport_writer_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_wgnutls_transport_writer_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_wgnutls_transport_writer_1(value, clnt);
}

void * get_wgnutls_transport_poller_wrapper()
{
    char_ptr *result;
    result = get_wgnutls_transport_poller_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_wgnutls_transport_poller_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_wgnutls_transport_poller_1(value, clnt);
}

void * get_wgnutls_transport_peeker_wrapper()
{
    char_ptr *result;
    result = get_wgnutls_transport_peeker_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_wgnutls_transport_peeker_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_wgnutls_transport_peeker_1(value, clnt);
}

void * get_wgnutls_transport_errstr_wrapper()
{
    char_ptr *result;
    result = get_wgnutls_transport_errstr_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_wgnutls_transport_errstr_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_wgnutls_transport_errstr_1(value, clnt);
}

void * get_wgnutls_transport_closer_wrapper()
{
    char_ptr *result;
    result = get_wgnutls_transport_closer_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_wgnutls_transport_closer_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_wgnutls_transport_closer_1(value, clnt);
}

struct hash_table* get_registered_specs_wrapper()
{
    registered_specs_ret_t *result_rpc;
    struct hash_table* value = {0};
    result_rpc = get_registered_specs_1(clnt);
        if(result_rpc->value.hash_table_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.hash_table_rpc_ptr_len*sizeof(struct hash_table));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.hash_table_rpc_ptr_len;i0++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {value[i0].cells = NULL;}
    else {
    	value[i0].cells = (struct cell *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof( struct cell));
    	if (value[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {value[i0].cells[i1].key = NULL;}
    else {value[i0].cells[i1].key = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len * sizeof(char));
    	if (value[i0].cells[i1].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].key, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);
    }
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {value[i0].cells[i1].value = NULL;}
    else {value[i0].cells[i1].value = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len * sizeof(char));
    	if (value[i0].cells[i1].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].value, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);
    }
    	}
    }
    value[i0].size=result_rpc->value.hash_table_rpc_ptr_val[i0].size;
    value[i0].count=result_rpc->value.hash_table_rpc_ptr_val[i0].count;
    value[i0].resize_threshold=result_rpc->value.hash_table_rpc_ptr_val[i0].resize_threshold;
    value[i0].prime_offset=result_rpc->value.hash_table_rpc_ptr_val[i0].prime_offset;
            }
        }
    return value;
}

int set_registered_specs_wrapper(struct hash_table* value,int len)
{
    void *result;
    hash_table_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.hash_table_rpc_ptr_len=0;
    value_rpc_ptr.hash_table_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.hash_table_rpc_ptr_len=1;
    value_rpc_ptr.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
    if(value_rpc_ptr.hash_table_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.hash_table_rpc_ptr_len;i0++) {
    if (value[i0].cells == NULL) {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}
    else {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));
    	if (value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    	}
    }
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].size=value[i0].size;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].count=value[i0].count;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].resize_threshold=value[i0].resize_threshold;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].prime_offset=value[i0].prime_offset;
    }
    }
    result = set_registered_specs_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void * get_registered_specs_hash_function_wrapper()
{
    char_ptr *result;
    result = get_registered_specs_hash_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_registered_specs_hash_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_registered_specs_hash_function_1(value, clnt);
}

void * get_registered_specs_test_function_wrapper()
{
    char_ptr *result;
    result = get_registered_specs_test_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_registered_specs_test_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_registered_specs_test_function_1(value, clnt);
}

struct cell* get_registered_specs_cells_wrapper()
{
    registered_specs_cells_ret_t *result_rpc;
    struct cell* value = {0};
    result_rpc = get_registered_specs_cells_1(clnt);
        if(result_rpc->value.cell_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.cell_rpc_ptr_len*sizeof(struct cell));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.cell_rpc_ptr_len;i0++) {
    if (result_rpc->value.cell_rpc_ptr_val[i0].key.key_val == NULL) {value[i0].key = NULL;}
    else {value[i0].key = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].key.key_len * sizeof(char));
    	if (value[i0].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].key, result_rpc->value.cell_rpc_ptr_val[i0].key.key_val, result_rpc->value.cell_rpc_ptr_val[i0].key.key_len);
    }
    if (result_rpc->value.cell_rpc_ptr_val[i0].value.value_val == NULL) {value[i0].value = NULL;}
    else {value[i0].value = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].value.value_len * sizeof(char));
    	if (value[i0].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].value, result_rpc->value.cell_rpc_ptr_val[i0].value.value_val, result_rpc->value.cell_rpc_ptr_val[i0].value.value_len);
    }
            }
        }
    return value;
}

int set_registered_specs_cells_wrapper(struct cell* value,int len)
{
    void *result;
    cell_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.cell_rpc_ptr_len=0;
    value_rpc_ptr.cell_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.cell_rpc_ptr_len=1;
    value_rpc_ptr.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
    if(value_rpc_ptr.cell_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.cell_rpc_ptr_len;i0++) {
    }
    }
    result = set_registered_specs_cells_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void* get_registered_specs_cells_key_wrapper(size_t *out_size)
{
    registered_specs_cells_key_ret_t *result;
    void *ret_val = NULL;
    result = get_registered_specs_cells_key_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_registered_specs_cells_key_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_registered_specs_cells_key_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

void* get_registered_specs_cells_value_wrapper(size_t *out_size)
{
    registered_specs_cells_value_ret_t *result;
    void *ret_val = NULL;
    result = get_registered_specs_cells_value_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_registered_specs_cells_value_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_registered_specs_cells_value_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_registered_specs_size_wrapper()
{
    registered_specs_size_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_registered_specs_size_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_registered_specs_size_wrapper(int value)
{
    void *result;
    result = set_registered_specs_size_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_registered_specs_count_wrapper()
{
    registered_specs_count_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_registered_specs_count_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_registered_specs_count_wrapper(int value)
{
    void *result;
    result = set_registered_specs_count_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_registered_specs_resize_threshold_wrapper()
{
    registered_specs_resize_threshold_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_registered_specs_resize_threshold_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_registered_specs_resize_threshold_wrapper(int value)
{
    void *result;
    result = set_registered_specs_resize_threshold_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_registered_specs_prime_offset_wrapper()
{
    registered_specs_prime_offset_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_registered_specs_prime_offset_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_registered_specs_prime_offset_wrapper(int value)
{
    void *result;
    result = set_registered_specs_prime_offset_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

FILE* get_output_stream_wrapper()
{
    char_ptr *result;
    FILE *ret_val = NULL;
    result = get_output_stream_1(clnt);
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

void set_output_stream_wrapper(FILE *file)
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
    result = set_output_stream_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
    }
}

unsigned int get_transport_map_modified_tick_wrapper()
{
    transport_map_modified_tick_ret_t *result;
    unsigned int default_value = {0}; // 初始化默认值
    result = get_transport_map_modified_tick_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return default_value;
    }
    return result->value;
}

int set_transport_map_modified_tick_wrapper(unsigned int value)
{
    void *result;
    result = set_transport_map_modified_tick_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0; // 失败
    }
    return 1; // 成功
}

acc_t* get_netrc_list_wrapper()
{
    netrc_list_ret_t *result_rpc;
    acc_t* value = {0};
    result_rpc = get_netrc_list_1(clnt);
        if(result_rpc->value.acc_t_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.acc_t_rpc_ptr_len*sizeof(struct acc_t));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.acc_t_rpc_ptr_len;i0++) {
    if (result_rpc->value.acc_t_rpc_ptr_val[i0].host.charptr.charptr_val == NULL) {value[i0].host = NULL;}
    else {value[i0].host = strdup(result_rpc->value.acc_t_rpc_ptr_val[i0].host.charptr.charptr_val);}
    if (result_rpc->value.acc_t_rpc_ptr_val[i0].acc.charptr.charptr_val == NULL) {value[i0].acc = NULL;}
    else {value[i0].acc = strdup(result_rpc->value.acc_t_rpc_ptr_val[i0].acc.charptr.charptr_val);}
    if (result_rpc->value.acc_t_rpc_ptr_val[i0].passwd.charptr.charptr_val == NULL) {value[i0].passwd = NULL;}
    else {value[i0].passwd = strdup(result_rpc->value.acc_t_rpc_ptr_val[i0].passwd.charptr.charptr_val);}
    if (result_rpc->value.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val == NULL) {value[i0].next = NULL;}
    else {
    	value[i0].next = (struct _acc_t *)malloc(result_rpc->value.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_len * sizeof( struct _acc_t));
    	if (value[i0].next == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < result_rpc->value.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_len; i1++) {
    if (result_rpc->value.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].host.charptr.charptr_val == NULL) {value[i0].next[i1].host = NULL;}
    else {value[i0].next[i1].host = strdup(result_rpc->value.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].host.charptr.charptr_val);}
    if (result_rpc->value.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].acc.charptr.charptr_val == NULL) {value[i0].next[i1].acc = NULL;}
    else {value[i0].next[i1].acc = strdup(result_rpc->value.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].acc.charptr.charptr_val);}
    if (result_rpc->value.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].passwd.charptr.charptr_val == NULL) {value[i0].next[i1].passwd = NULL;}
    else {value[i0].next[i1].passwd = strdup(result_rpc->value.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].passwd.charptr.charptr_val);}
    if (result_rpc->value.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].next._acc_t_rpc_ptr_val == NULL) {value[i0].next[i1].next = NULL;}
    else {
    	value[i0].next[i1].next = (struct _acc_t *)malloc(result_rpc->value.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].next._acc_t_rpc_ptr_len * sizeof( struct _acc_t));
    	if (value[i0].next[i1].next == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < result_rpc->value.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].next._acc_t_rpc_ptr_len; i1++) {
    	}
    }
    	}
    }
            }
        }
    return value;
}

int set_netrc_list_wrapper(acc_t value,int len)
{
    void *result;
    acc_t_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.acc_t_rpc_ptr_len=0;
    value_rpc_ptr.acc_t_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.acc_t_rpc_ptr_len=1;
    value_rpc_ptr.acc_t_rpc_ptr_val=malloc(1*sizeof(acc_t_rpc));
    if(value_rpc_ptr.acc_t_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.acc_t_rpc_ptr_len;i0++) {
    if (value[i0].host == NULL) {value_rpc_ptr.acc_t_rpc_ptr_val[i0].host.charptr.charptr_len = 0;value_rpc_ptr.acc_t_rpc_ptr_val[i0].host.charptr.charptr_val = NULL;}
    else {value_rpc_ptr.acc_t_rpc_ptr_val[i0].host.charptr.charptr_len = strlen(value[i0].host) + 1;

    	value_rpc_ptr.acc_t_rpc_ptr_val[i0].host.charptr.charptr_val = malloc(value_rpc_ptr.acc_t_rpc_ptr_val[i0].host.charptr.charptr_len * sizeof(char));
    	if (value_rpc_ptr.acc_t_rpc_ptr_val[i0].host.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value_rpc_ptr.acc_t_rpc_ptr_val[i0].host.charptr.charptr_val, value[i0].host, value_rpc_ptr.acc_t_rpc_ptr_val[i0].host.charptr.charptr_len);
    }
    if (value[i0].acc == NULL) {value_rpc_ptr.acc_t_rpc_ptr_val[i0].acc.charptr.charptr_len = 0;value_rpc_ptr.acc_t_rpc_ptr_val[i0].acc.charptr.charptr_val = NULL;}
    else {value_rpc_ptr.acc_t_rpc_ptr_val[i0].acc.charptr.charptr_len = strlen(value[i0].acc) + 1;

    	value_rpc_ptr.acc_t_rpc_ptr_val[i0].acc.charptr.charptr_val = malloc(value_rpc_ptr.acc_t_rpc_ptr_val[i0].acc.charptr.charptr_len * sizeof(char));
    	if (value_rpc_ptr.acc_t_rpc_ptr_val[i0].acc.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value_rpc_ptr.acc_t_rpc_ptr_val[i0].acc.charptr.charptr_val, value[i0].acc, value_rpc_ptr.acc_t_rpc_ptr_val[i0].acc.charptr.charptr_len);
    }
    if (value[i0].passwd == NULL) {value_rpc_ptr.acc_t_rpc_ptr_val[i0].passwd.charptr.charptr_len = 0;value_rpc_ptr.acc_t_rpc_ptr_val[i0].passwd.charptr.charptr_val = NULL;}
    else {value_rpc_ptr.acc_t_rpc_ptr_val[i0].passwd.charptr.charptr_len = strlen(value[i0].passwd) + 1;

    	value_rpc_ptr.acc_t_rpc_ptr_val[i0].passwd.charptr.charptr_val = malloc(value_rpc_ptr.acc_t_rpc_ptr_val[i0].passwd.charptr.charptr_len * sizeof(char));
    	if (value_rpc_ptr.acc_t_rpc_ptr_val[i0].passwd.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value_rpc_ptr.acc_t_rpc_ptr_val[i0].passwd.charptr.charptr_val, value[i0].passwd, value_rpc_ptr.acc_t_rpc_ptr_val[i0].passwd.charptr.charptr_len);
    }
    if (value[i0].next == NULL) {value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_len = 0;value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val = NULL;}
    else {value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_len = 1;value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val = malloc(value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_len * sizeof(_acc_t_rpc));
    	if (value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_len; i1++) {
    if (value[i0].next[i1].host == NULL) {value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].host.charptr.charptr_len = 0;value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].host.charptr.charptr_val = NULL;}
    else {value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].host.charptr.charptr_len = strlen(value[i0].next[i1].host) + 1;

    	value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].host.charptr.charptr_val = malloc(value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].host.charptr.charptr_len * sizeof(char));
    	if (value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].host.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].host.charptr.charptr_val, value[i0].next[i1].host, value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].host.charptr.charptr_len);
    }
    if (value[i0].next[i1].acc == NULL) {value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].acc.charptr.charptr_len = 0;value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].acc.charptr.charptr_val = NULL;}
    else {value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].acc.charptr.charptr_len = strlen(value[i0].next[i1].acc) + 1;

    	value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].acc.charptr.charptr_val = malloc(value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].acc.charptr.charptr_len * sizeof(char));
    	if (value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].acc.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].acc.charptr.charptr_val, value[i0].next[i1].acc, value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].acc.charptr.charptr_len);
    }
    if (value[i0].next[i1].passwd == NULL) {value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].passwd.charptr.charptr_len = 0;value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].passwd.charptr.charptr_val = NULL;}
    else {value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].passwd.charptr.charptr_len = strlen(value[i0].next[i1].passwd) + 1;

    	value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].passwd.charptr.charptr_val = malloc(value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].passwd.charptr.charptr_len * sizeof(char));
    	if (value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].passwd.charptr.charptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].passwd.charptr.charptr_val, value[i0].next[i1].passwd, value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].passwd.charptr.charptr_len);
    }
    if (value[i0].next[i1].next == NULL) {value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].next._acc_t_rpc_ptr_len = 0;value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].next._acc_t_rpc_ptr_val = NULL;}
    else {value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].next._acc_t_rpc_ptr_len = 1;value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].next._acc_t_rpc_ptr_val = malloc(value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].next._acc_t_rpc_ptr_len * sizeof(_acc_t_rpc));
    	if (value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].next._acc_t_rpc_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i2 = 0; i2 < value_rpc_ptr.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].next._acc_t_rpc_ptr_len; i2++) {
    	}
    }
    	}
    }
    }
    }
    result = set_netrc_list_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

FILE* get_logfp_wrapper()
{
    char_ptr *result;
    FILE *ret_val = NULL;
    result = get_logfp_1(clnt);
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

void set_logfp_wrapper(FILE *file)
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
    result = set_logfp_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
    }
}

char* get_meta_charset_wrapper()
{
    char_ptr *result;
    char *ret_val = NULL;
    result = get_meta_charset_1(clnt);
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

void set_meta_charset_wrapper(const char *value)
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
    result = set_meta_charset_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
    }
}

char* get_redirect_request_signal_name_wrapper()
{
    char_ptr *result;
    char *ret_val = NULL;
    result = get_redirect_request_signal_name_1(clnt);
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

void set_redirect_request_signal_name_wrapper(const char *value)
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
    result = set_redirect_request_signal_name_1(rpc_value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "call failed");
    }
}

struct hash_table* get_downloaded_files_hash_wrapper()
{
    downloaded_files_hash_ret_t *result_rpc;
    struct hash_table* value = {0};
    result_rpc = get_downloaded_files_hash_1(clnt);
        if(result_rpc->value.hash_table_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.hash_table_rpc_ptr_len*sizeof(struct hash_table));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.hash_table_rpc_ptr_len;i0++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {value[i0].cells = NULL;}
    else {
    	value[i0].cells = (struct cell *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof( struct cell));
    	if (value[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {value[i0].cells[i1].key = NULL;}
    else {value[i0].cells[i1].key = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len * sizeof(char));
    	if (value[i0].cells[i1].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].key, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);
    }
    if (result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {value[i0].cells[i1].value = NULL;}
    else {value[i0].cells[i1].value = (char *)malloc(result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len * sizeof(char));
    	if (value[i0].cells[i1].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].cells[i1].value, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, result_rpc->value.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);
    }
    	}
    }
    value[i0].size=result_rpc->value.hash_table_rpc_ptr_val[i0].size;
    value[i0].count=result_rpc->value.hash_table_rpc_ptr_val[i0].count;
    value[i0].resize_threshold=result_rpc->value.hash_table_rpc_ptr_val[i0].resize_threshold;
    value[i0].prime_offset=result_rpc->value.hash_table_rpc_ptr_val[i0].prime_offset;
            }
        }
    return value;
}

int set_downloaded_files_hash_wrapper(struct hash_table* value,int len)
{
    void *result;
    hash_table_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.hash_table_rpc_ptr_len=0;
    value_rpc_ptr.hash_table_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.hash_table_rpc_ptr_len=1;
    value_rpc_ptr.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
    if(value_rpc_ptr.hash_table_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.hash_table_rpc_ptr_len;i0++) {
    if (value[i0].cells == NULL) {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}
    else {value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));
    	if (value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < value_rpc_ptr.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    	}
    }
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].size=value[i0].size;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].count=value[i0].count;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].resize_threshold=value[i0].resize_threshold;
    value_rpc_ptr.hash_table_rpc_ptr_val[i0].prime_offset=value[i0].prime_offset;
    }
    }
    result = set_downloaded_files_hash_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void * get_downloaded_files_hash_hash_function_wrapper()
{
    char_ptr *result;
    result = get_downloaded_files_hash_hash_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_downloaded_files_hash_hash_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_downloaded_files_hash_hash_function_1(value, clnt);
}

void * get_downloaded_files_hash_test_function_wrapper()
{
    char_ptr *result;
    result = get_downloaded_files_hash_test_function_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get call failed");
        return NULL;
    }
    if (result->charptr.charptr_val == NULL) {
        return NULL;
    }
    char *fun_name = result->charptr.charptr_val;
    void *fun_ptr = find_function_ptr_by_name(fun_name);
    if (fun_ptr == NULL) {
        clnt_perror(clnt, "find function pointer failed");
        return NULL;
    }
    return fun_ptr;
}

void set_downloaded_files_hash_test_function_wrapper(void *fun_ptr)
{
    char_ptr value;
    char *fun_name = find_function_name_by_address(fun_ptr);
    if (fun_name == NULL) {
        return;
    }
   value.charptr.charptr_len = strlen(fun_name) + 1;
   value.charptr.charptr_val = malloc(value.charptr.charptr_len);
   if (value.charptr.charptr_val == NULL) {
       perror("malloc failed");
       return;
   }
   memcpy(value.charptr.charptr_val, fun_name, value.charptr.charptr_len);
   set_downloaded_files_hash_test_function_1(value, clnt);
}

struct cell* get_downloaded_files_hash_cells_wrapper()
{
    downloaded_files_hash_cells_ret_t *result_rpc;
    struct cell* value = {0};
    result_rpc = get_downloaded_files_hash_cells_1(clnt);
        if(result_rpc->value.cell_rpc_ptr_val==NULL) {
            value=NULL;
        } else {
            value=malloc(result_rpc->value.cell_rpc_ptr_len*sizeof(struct cell));
            if(value==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result_rpc->value.cell_rpc_ptr_len;i0++) {
    if (result_rpc->value.cell_rpc_ptr_val[i0].key.key_val == NULL) {value[i0].key = NULL;}
    else {value[i0].key = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].key.key_len * sizeof(char));
    	if (value[i0].key == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].key, result_rpc->value.cell_rpc_ptr_val[i0].key.key_val, result_rpc->value.cell_rpc_ptr_val[i0].key.key_len);
    }
    if (result_rpc->value.cell_rpc_ptr_val[i0].value.value_val == NULL) {value[i0].value = NULL;}
    else {value[i0].value = (char *)malloc(result_rpc->value.cell_rpc_ptr_val[i0].value.value_len * sizeof(char));
    	if (value[i0].value == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	memcpy(value[i0].value, result_rpc->value.cell_rpc_ptr_val[i0].value.value_val, result_rpc->value.cell_rpc_ptr_val[i0].value.value_len);
    }
            }
        }
    return value;
}

int set_downloaded_files_hash_cells_wrapper(struct cell* value,int len)
{
    void *result;
    cell_rpc_ptr value_rpc_ptr;
    if(value==NULL) {
    value_rpc_ptr.cell_rpc_ptr_len=0;
    value_rpc_ptr.cell_rpc_ptr_val=NULL;
    } else {
    value_rpc_ptr.cell_rpc_ptr_len=1;
    value_rpc_ptr.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
    if(value_rpc_ptr.cell_rpc_ptr_val==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<value_rpc_ptr.cell_rpc_ptr_len;i0++) {
    }
    }
    result = set_downloaded_files_hash_cells_1(value_rpc_ptr, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set call failed");
        return 0;
    }
    return 1;
}

void* get_downloaded_files_hash_cells_key_wrapper(size_t *out_size)
{
    downloaded_files_hash_cells_key_ret_t *result;
    void *ret_val = NULL;
    result = get_downloaded_files_hash_cells_key_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_downloaded_files_hash_cells_key_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_downloaded_files_hash_cells_key_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

void* get_downloaded_files_hash_cells_value_wrapper(size_t *out_size)
{
    downloaded_files_hash_cells_value_ret_t *result;
    void *ret_val = NULL;
    result = get_downloaded_files_hash_cells_value_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        if (out_size) *out_size = 0;
        return NULL;
    }
    if (result->value.bytes.bytes_val != NULL && result->value.bytes.bytes_len > 0) {
        ret_val = malloc(result->value.bytes.bytes_len);
        if (ret_val == NULL) {
            perror("malloc failed");
            if (out_size) *out_size = 0;
            return NULL;
        }
        memcpy(ret_val, result->value.bytes.bytes_val, result->value.bytes.bytes_len);
        if (out_size) *out_size = result->value.size;
    } else {
        if (out_size) *out_size = 0;
    }
    return ret_val;
}

int set_downloaded_files_hash_cells_value_wrapper(void *data, size_t size)
{
    void *result;
    binary_data value;
    if (data == NULL || size == 0) {
        value.bytes.bytes_len = 0;
        value.bytes.bytes_val = NULL;
        value.size = 0;
    } else {
        value.bytes.bytes_len = size;
        value.bytes.bytes_val = data;
        value.size = size;
    }
    result = set_downloaded_files_hash_cells_value_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_downloaded_files_hash_size_wrapper()
{
    downloaded_files_hash_size_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_downloaded_files_hash_size_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_downloaded_files_hash_size_wrapper(int value)
{
    void *result;
    result = set_downloaded_files_hash_size_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_downloaded_files_hash_count_wrapper()
{
    downloaded_files_hash_count_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_downloaded_files_hash_count_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_downloaded_files_hash_count_wrapper(int value)
{
    void *result;
    result = set_downloaded_files_hash_count_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_downloaded_files_hash_resize_threshold_wrapper()
{
    downloaded_files_hash_resize_threshold_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_downloaded_files_hash_resize_threshold_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_downloaded_files_hash_resize_threshold_wrapper(int value)
{
    void *result;
    result = set_downloaded_files_hash_resize_threshold_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
}

int get_downloaded_files_hash_prime_offset_wrapper()
{
    downloaded_files_hash_prime_offset_ret_t *result;
    int ret_val = 0; // 默认值
    result = get_downloaded_files_hash_prime_offset_1(clnt);
    if (result == NULL) {
        clnt_perror(clnt, "get member call failed");
        return ret_val;
    }
    ret_val = result->value;
    return ret_val;
}

int set_downloaded_files_hash_prime_offset_wrapper(int value)
{
    void *result;
    result = set_downloaded_files_hash_prime_offset_1(value, clnt);
    if (result == NULL) {
        clnt_perror(clnt, "set member call failed");
        return 0;
    }
    return 1;
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
int gsetter_warc_current_file_number_postfix_wrapper()
{
   int *result;
   result=gsetter_warc_current_file_number_postfix_1(clnt);
   if (result == NULL) 
       clnt_perror(clnt, "call failed");
   return *result;
}
unsigned int gsetter_transport_map_modified_tick_prefix_wrapper()
{
   unsigned int *result;
   result=gsetter_transport_map_modified_tick_prefix_1(clnt);
   if (result == NULL) 
       clnt_perror(clnt, "call failed");
   return *result;
}
int gsetter_log_line_current_prefix_wrapper()
{
   int *result;
   result=gsetter_log_line_current_prefix_1(clnt);
   if (result == NULL) 
       clnt_perror(clnt, "call failed");
   return *result;
}
int gsetter_numurls_postfix_wrapper()
{
   int *result;
   result=gsetter_numurls_postfix_1(clnt);
   if (result == NULL) 
       clnt_perror(clnt, "call failed");
   return *result;
}
int gsetter_numurls_prefix_wrapper()
{
   int *result;
   result=gsetter_numurls_prefix_1(clnt);
   if (result == NULL) 
       clnt_perror(clnt, "call failed");
   return *result;
}
