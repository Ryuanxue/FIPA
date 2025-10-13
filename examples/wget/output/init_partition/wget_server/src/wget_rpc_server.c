#include "wget_idl.h"
wget_read_file_ret_ *wget_read_file_rpc_1_svc(char_ptr arg1,struct svc_req *rqstp)
{
	static wget_read_file_ret_ result;
char *file=NULL;
if(arg1.charptr.charptr_val!=NULL) {
file=arg1.charptr.charptr_val;
}
struct file_memory * sensefun_ret;
sensefun_ret=wget_read_file(file);
if (sensefun_ret == NULL) {result->ret0.file_memoryrpc_ptr_len = 0;relust->ret0.file_memoryrpc_ptr_val = NULL;}
else {
	result->ret0.file_memoryrpc_ptr_len = 1;
	for (int i = 0; i < result->ret0.file_memoryrpc_ptr_len; i++) {
if (sensefun_ret[i]->content == NULL) {result->ret0.file_memoryrpc_ptr_val[i]->content.charptr.charptr_len = 0;result->ret0.file_memoryrpc_ptr_val[i]->content.charptr.charptr_val = NULL;}

else {result->ret0.file_memoryrpc_ptr_val[i]->content.charptr.charptr_len = strlen(sensefun_ret[i]->content)+1;result->ret0.file_memoryrpc_ptr_val[i]->content.charptr.charptr_val = sensefun_ret[i]->content;}

result->ret0.file_memoryrpc_ptr_val[i]->length=sensefun_ret[i]->length;
result->ret0.file_memoryrpc_ptr_val[i]->mmap_p=sensefun_ret[i]->mmap_p;
	}
if(file==NULL) result.file.charptr.charptr_len=0;else result.file.charptr.charptr_len=strlen(file)+1;
result.file.charptr.charptr_val=malloc(result.file.charptr.charptr_len);
if(result.file.charptr.charptr_val==NULL) {
perror("malloc failed");
exit(1);
}
memcpy(result.file.charptr.charptr_val,file,result.file.charptr.charptr_len);
return &result;
}
sock_read_ret_ *sock_read_rpc_1_svc(int arg1,char_ptr arg2,int arg3,struct svc_req *rqstp)
{
	static sock_read_ret_ result;
int fd=arg1;
char *buf=NULL;
if(arg2.charptr.charptr_val!=NULL) {
buf=arg2.charptr.charptr_val;
}
int bufsize=arg3;
int sensefun_ret;
sensefun_ret=sock_read(fd,buf,bufsize);
result.ret0=sensefun_ret;
if(buf==NULL) result.buf.charptr.charptr_len=0;else result.buf.charptr.charptr_len=strlen(buf)+1;
result.buf.charptr.charptr_val=malloc(result.buf.charptr.charptr_len);
if(result.buf.charptr.charptr_val==NULL) {
perror("malloc failed");
exit(1);
}
memcpy(result.buf.charptr.charptr_val,buf,result.buf.charptr.charptr_len);
return &result;
}
main_sense_1_ret_ *main_sense_1_rpc_1_svc(TwoCharPtr arg1,TwoCharPtr arg2,int arg3,ptimer_rpc_ptr arg4,double arg5,struct svc_req *rqstp)
{
	static main_sense_1_ret_ result;
char **url=NULL;
if(arg1.twocharptr.twocharptr_val!=NULL) {
url=(char **)malloc(arg1.twocharptr.twocharptr_len*sizeof(char *));
if(url==NULL) {
perror("malloc failed");
exit(1);
}
for(int i=0;i<arg1.twocharptr.twocharptr_len;i++) {
if(arg1.twocharptr.twocharptr_val[i].charptr.charptr_val==NULL) {
url[i]==NULL;
}else{
url[i]=malloc(arg1.twocharptr.twocharptr_val[i].charptr.charptr_len*sizeof(char));
if(url[i]==NULL) {
perror("malloc failed");
exit(1);
}
memcpy(url[i],arg1.twocharptr.twocharptr_val[i].charptr.charptr_val,arg1.twocharptr.twocharptr_val[i].charptr.charptr_len);
			}
		}
	}
char **t=NULL;
if(arg2.twocharptr.twocharptr_val!=NULL) {
t=(char **)malloc(arg2.twocharptr.twocharptr_len*sizeof(char *));
if(t==NULL) {
perror("malloc failed");
exit(1);
}
for(int i=0;i<arg2.twocharptr.twocharptr_len;i++) {
if(arg2.twocharptr.twocharptr_val[i].charptr.charptr_val==NULL) {
t[i]==NULL;
}else{
t[i]=malloc(arg2.twocharptr.twocharptr_val[i].charptr.charptr_len*sizeof(char));
if(t[i]==NULL) {
perror("malloc failed");
exit(1);
}
memcpy(t[i],arg2.twocharptr.twocharptr_val[i].charptr.charptr_val,arg2.twocharptr.twocharptr_val[i].charptr.charptr_len);
			}
		}
	}
int nurl=arg3;
struct ptimer *timer=NULL;
if(arg4.ptimer_rpc_ptr_val!=NULL) {
timer=(struct ptimer *)malloc(arg4.ptimer_rpc_ptr_len*sizeof(struct ptimer));
if(timer==NULL) {
perror("malloc failed");
exit(1);
}
for(int i0=0;i0<arg4.ptimer_rpc_ptr_len;i0++) {
timer[i0].elapsed_last=arg4.ptimer_rpc_ptr_val[i0].elapsed_last;
timer[i0].elapsed_pre_start=arg4.ptimer_rpc_ptr_val[i0].elapsed_pre_start;
	}
}
double start_time=arg5;
main_sense_1(url,t,nurl,timer,start_time);
    if(url==NULL) {
        result.url.twocharptr.twocharptr_len=0;
        result.url.twocharptr.twocharptr_val=NULL;
    } else {
        result.url.twocharptr.twocharptr_len=1;
        result.url.twocharptr.twocharptr_val=malloc(1*sizeof(char_ptr));
        if(result.url.twocharptr.twocharptr_val==NULL) {
            perror("malloc failed");
            exit(1);
        }
        for(int i=0;i<result.url.twocharptr.twocharptr_len;i++) {
            if(url[i]==NULL) {
                result.url.twocharptr.twocharptr_val[i].charptr.charptr_len=0;
                result.url.twocharptr.twocharptr_val[i].charptr.charptr_val=NULL;
            } else {
                result.url.twocharptr.twocharptr_val[i].charptr.charptr_len=strlen(url[i])+1;
                result.url.twocharptr.twocharptr_val[i].charptr.charptr_val=malloc(result.url.twocharptr.twocharptr_val[i].charptr.charptr_len*sizeof(char));
                if(result.url.twocharptr.twocharptr_val[i].charptr.charptr_val==NULL) {
            perror("malloc failed");
            exit(1);
                }
                memcpy(result.url.twocharptr.twocharptr_val[i].charptr.charptr_val,url[i],result.url.twocharptr.twocharptr_val[i].charptr.charptr_len);
            }
        }
    }
    if(t==NULL) {
        result.t.twocharptr.twocharptr_len=0;
        result.t.twocharptr.twocharptr_val=NULL;
    } else {
        result.t.twocharptr.twocharptr_len=1;
        result.t.twocharptr.twocharptr_val=malloc(1*sizeof(char_ptr));
        if(result.t.twocharptr.twocharptr_val==NULL) {
            perror("malloc failed");
            exit(1);
        }
        for(int i=0;i<result.t.twocharptr.twocharptr_len;i++) {
            if(t[i]==NULL) {
                result.t.twocharptr.twocharptr_val[i].charptr.charptr_len=0;
                result.t.twocharptr.twocharptr_val[i].charptr.charptr_val=NULL;
            } else {
                result.t.twocharptr.twocharptr_val[i].charptr.charptr_len=strlen(t[i])+1;
                result.t.twocharptr.twocharptr_val[i].charptr.charptr_val=malloc(result.t.twocharptr.twocharptr_val[i].charptr.charptr_len*sizeof(char));
                if(result.t.twocharptr.twocharptr_val[i].charptr.charptr_val==NULL) {
            perror("malloc failed");
            exit(1);
                }
                memcpy(result.t.twocharptr.twocharptr_val[i].charptr.charptr_val,t[i],result.t.twocharptr.twocharptr_val[i].charptr.charptr_len);
            }
        }
    }
    if(timer==NULL) {
        result.timer.ptimer_rpc_ptr_len=0;
        result.timer.ptimer_rpc_ptr_val=NULL;
    } else {
        result.timer.ptimer_rpc_ptr_len=1;
        result.timer.ptimer_rpc_ptr_val=malloc(1*sizeof(ptimer_rpc));
        if(result.timer.ptimer_rpc_ptr_val==NULL) {
            perror("malloc failed");
            exit(1);
        }
        for(int i0=0;i0<result.timer.ptimer_rpc_ptr_len;i0++) {
result.timer.ptimer_rpc_ptr_val[i0].elapsed_last=timer[i0].elapsed_last;
result.timer.ptimer_rpc_ptr_val[i0].elapsed_pre_start=timer[i0].elapsed_pre_start;
        }
    }
return &result;
}

/* 全局变量访问函数 - 服务端 */
dummy_iri_ret_t *get_dummy_iri_1_svc(struct svc_req *rqstp)
{
    static dummy_iri_ret_t result;
    return &result;
}

void *set_dummy_iri_1_svc(iri_rpc value, struct svc_req *rqstp)
{
    static int result = 1;
    iri_rpc arg1
    return (void *)&result;
}

dummy_iri_uri_encoding_ret_t *get_dummy_iri_uri_encoding_1_svc(struct svc_req *rqstp)
{
    static dummy_iri_uri_encoding_ret_t result;
    if (dummy_iri.uri_encoding != NULL) {
        result.value = strdup(dummy_iri.uri_encoding);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_dummy_iri_uri_encoding_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (dummy_iri.uri_encoding != NULL) {
        free(dummy_iri.uri_encoding);
    }
    if (value != NULL) {
        dummy_iri.uri_encoding = strdup(value);
    } else {
        dummy_iri.uri_encoding = NULL;
    }
    return (void *)&result;
}

dummy_iri_content_encoding_ret_t *get_dummy_iri_content_encoding_1_svc(struct svc_req *rqstp)
{
    static dummy_iri_content_encoding_ret_t result;
    if (dummy_iri.content_encoding != NULL) {
        result.value = strdup(dummy_iri.content_encoding);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_dummy_iri_content_encoding_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (dummy_iri.content_encoding != NULL) {
        free(dummy_iri.content_encoding);
    }
    if (value != NULL) {
        dummy_iri.content_encoding = strdup(value);
    } else {
        dummy_iri.content_encoding = NULL;
    }
    return (void *)&result;
}

dummy_iri_orig_url_ret_t *get_dummy_iri_orig_url_1_svc(struct svc_req *rqstp)
{
    static dummy_iri_orig_url_ret_t result;
    if (dummy_iri.orig_url != NULL) {
        result.value = strdup(dummy_iri.orig_url);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_dummy_iri_orig_url_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (dummy_iri.orig_url != NULL) {
        free(dummy_iri.orig_url);
    }
    if (value != NULL) {
        dummy_iri.orig_url = strdup(value);
    } else {
        dummy_iri.orig_url = NULL;
    }
    return (void *)&result;
}

dummy_iri_utf8_encode_ret_t *get_dummy_iri_utf8_encode_1_svc(struct svc_req *rqstp)
{
    static dummy_iri_utf8_encode_ret_t result;
    result.value = dummy_iri.utf8_encode;
    return &result;
}

void *set_dummy_iri_utf8_encode_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    dummy_iri.utf8_encode = value;
    return (void *)&result;
}

wget_cookie_jar_ret_t *get_wget_cookie_jar_1_svc(struct svc_req *rqstp)
{
    static wget_cookie_jar_ret_t result;
        if(wget_cookie_jar==NULL) {
            result.wget_cookie_jar.cookie_jar_rpc_ptr_len=0;
            result.wget_cookie_jar.cookie_jar_rpc_ptr_val=NULL;
        } else {
            result.wget_cookie_jar.cookie_jar_rpc_ptr_len=1;
            result.wget_cookie_jar.cookie_jar_rpc_ptr_val=malloc(1*sizeof(cookie_jar_rpc));
            if(result.wget_cookie_jar.cookie_jar_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.wget_cookie_jar.cookie_jar_rpc_ptr_len;i0++) {
    if (wget_cookie_jar[i0].chains == NULL) {result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_len = 0;result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val = NULL;}

    else {result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_len = 1;result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val = malloc(result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_len * sizeof(hash_table_rpc));

    	if (result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val == NULL) {perror("malloc failed");exit(1);
}
    	for (int i1 = 0; i1 < result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_len; i1++) {

    if (wget_cookie_jar[i0].chains.cells == NULL) {result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_len = 0;result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val = NULL;}

    else {result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_len = 1;result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val = malloc(result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_len * sizeof(cell_rpc));

    	if (result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val == NULL) {perror("malloc failed");exit(1);
}
    	for (int i1 = 0; i1 < result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_len; i1++) {

    if (wget_cookie_jar[i0].chains->.cells.key == NULL) {result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].key.key_len = 0;result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].key.key_val = NULL;}

    else{result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].key.key_len = sizeof(wget_cookie_jar[i0].chains->.cells.key);result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].key.key_val = malloc(result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].key.key_val, wget_cookie_jar[i0].chains->.cells.key, result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].key.key_len);}

    if (wget_cookie_jar[i0].chains->.cells.value == NULL) {result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].value.value_len = 0;result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].value.value_val = NULL;}

    else{result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].value.value_len = sizeof(wget_cookie_jar[i0].chains->.cells.value);result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].value.value_val = malloc(result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].value.value_val, wget_cookie_jar[i0].chains->.cells.value, result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].value.value_len);}

    	}
    }
    result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].size=wget_cookie_jar[i0].chains->size;
    result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].count=wget_cookie_jar[i0].chains->count;
    result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].resize_threshold=wget_cookie_jar[i0].chains->resize_threshold;
    result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].prime_offset=wget_cookie_jar[i0].chains->prime_offset;
    	}
    }
    result.wget_cookie_jar.cookie_jar_rpc_ptr_val[i0].cookie_count=wget_cookie_jar[i0].cookie_count;
            }
        }
    return &result;
}

void *set_wget_cookie_jar_1_svc(cookie_jar_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct cookie_jar *local_wget_cookie_jar=NULL;
    if(arg1.cookie_jar_rpc_ptr_val!=NULL) {
    local_wget_cookie_jar=(struct cookie_jar *)malloc(arg1.cookie_jar_rpc_ptr_len*sizeof(struct cookie_jar));
    if(local_wget_cookie_jar==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.cookie_jar_rpc_ptr_len;i0++) {
    if (arg1.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val == NULL) {local_wget_cookie_jar[i0].chains=NULL;}
    else {local_wget_cookie_jar[i0].chains=(struct hash_table *)malloc(arg1.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_len * sizeof(struct hash_table));
    	if (local_wget_cookie_jar[i0].chains == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < arg1.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_len; i1++) {
    if (arg1.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val == NULL) {local_wget_cookie_jar[i0].chains[i1].cells=NULL;}
    else {local_wget_cookie_jar[i0].chains[i1].cells=(struct cell *)malloc(arg1.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_len * sizeof(struct cell));
    	if (local_wget_cookie_jar[i0].chains[i1].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < arg1.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_len; i1++) {
    if (arg1.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {local_wget_cookie_jar[i0].chains[i1].cells[i1].key=NULL;}

    else {local_wget_cookie_jar[i0].chains[i1].cells[i1].key=malloc(arg1.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (local_wget_cookie_jar[i0].chains[i1].cells[i1].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_wget_cookie_jar[i0].chains[i1].cells[i1].key,arg1.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].key.key_val,arg1.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].key.key_len);

    }

    if (arg1.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {local_wget_cookie_jar[i0].chains[i1].cells[i1].value=NULL;}

    else {local_wget_cookie_jar[i0].chains[i1].cells[i1].value=malloc(arg1.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (local_wget_cookie_jar[i0].chains[i1].cells[i1].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_wget_cookie_jar[i0].chains[i1].cells[i1].value,arg1.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].value.value_val,arg1.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].cells.cell_rpc_ptr_val[i1].value.value_len);

    }

    	}
    }
    local_wget_cookie_jar[i0].chains[i1].size=arg1.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].size;
    local_wget_cookie_jar[i0].chains[i1].count=arg1.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].count;
    local_wget_cookie_jar[i0].chains[i1].resize_threshold=arg1.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].resize_threshold;
    local_wget_cookie_jar[i0].chains[i1].prime_offset=arg1.cookie_jar_rpc_ptr_val[i0].chains.hash_table_rpc_ptr_val[i1].prime_offset;
    	}
    }
    local_wget_cookie_jar[i0].cookie_count=arg1.cookie_jar_rpc_ptr_val[i0].cookie_count;
    	}
    }
    memcpy(&wget_cookie_jar, &local_wget_cookie_jar, sizeof(cookie_jar));
    return (void *)&result;
}

wget_cookie_jar_chains_ret_t *get_wget_cookie_jar_chains_1_svc(struct svc_req *rqstp)
{
    static wget_cookie_jar_chains_ret_t result;
        if(wget_cookie_jar_chains==NULL) {
            result.wget_cookie_jar_chains.hash_table_rpc_ptr_len=0;
            result.wget_cookie_jar_chains.hash_table_rpc_ptr_val=NULL;
        } else {
            result.wget_cookie_jar_chains.hash_table_rpc_ptr_len=1;
            result.wget_cookie_jar_chains.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
            if(result.wget_cookie_jar_chains.hash_table_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.wget_cookie_jar_chains.hash_table_rpc_ptr_len;i0++) {
    if (wget_cookie_jar_chains[i0].cells == NULL) {result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}

    else {result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));

    	if (result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {perror("malloc failed");exit(1);
}
    	for (int i1 = 0; i1 < result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {

    if (wget_cookie_jar_chains[i0].cells->key == NULL) {result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = 0;result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = NULL;}

    else{result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = sizeof(wget_cookie_jar_chains[i0].cells->key);result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = malloc(result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, wget_cookie_jar_chains[i0].cells->key, result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);}

    if (wget_cookie_jar_chains[i0].cells->value == NULL) {result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = 0;result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = NULL;}

    else{result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = sizeof(wget_cookie_jar_chains[i0].cells->value);result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = malloc(result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, wget_cookie_jar_chains[i0].cells->value, result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);}

    	}
    }
    result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].size=wget_cookie_jar_chains[i0].size;
    result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].count=wget_cookie_jar_chains[i0].count;
    result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].resize_threshold=wget_cookie_jar_chains[i0].resize_threshold;
    result.wget_cookie_jar_chains.hash_table_rpc_ptr_val[i0].prime_offset=wget_cookie_jar_chains[i0].prime_offset;
            }
        }
    return &result;
}

void *set_wget_cookie_jar_chains_1_svc(hash_table_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct hash_table *local_wget_cookie_jar_chains=NULL;
    if(arg1.hash_table_rpc_ptr_val!=NULL) {
    local_wget_cookie_jar_chains=(struct hash_table *)malloc(arg1.hash_table_rpc_ptr_len*sizeof(struct hash_table));
    if(local_wget_cookie_jar_chains==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.hash_table_rpc_ptr_len;i0++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {local_wget_cookie_jar_chains[i0].cells=NULL;}
    else {local_wget_cookie_jar_chains[i0].cells=(struct cell *)malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(struct cell));
    	if (local_wget_cookie_jar_chains[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {local_wget_cookie_jar_chains[i0].cells[i1].key=NULL;}

    else {local_wget_cookie_jar_chains[i0].cells[i1].key=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (local_wget_cookie_jar_chains[i0].cells[i1].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_wget_cookie_jar_chains[i0].cells[i1].key,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    }

    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {local_wget_cookie_jar_chains[i0].cells[i1].value=NULL;}

    else {local_wget_cookie_jar_chains[i0].cells[i1].value=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (local_wget_cookie_jar_chains[i0].cells[i1].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_wget_cookie_jar_chains[i0].cells[i1].value,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    }

    	}
    }
    local_wget_cookie_jar_chains[i0].size=arg1.hash_table_rpc_ptr_val[i0].size;
    local_wget_cookie_jar_chains[i0].count=arg1.hash_table_rpc_ptr_val[i0].count;
    local_wget_cookie_jar_chains[i0].resize_threshold=arg1.hash_table_rpc_ptr_val[i0].resize_threshold;
    local_wget_cookie_jar_chains[i0].prime_offset=arg1.hash_table_rpc_ptr_val[i0].prime_offset;
    	}
    }
    memcpy(&wget_cookie_jar_chains, &local_wget_cookie_jar_chains, sizeof(hash_table));
    return (void *)&result;
}

char_ptr *get_wget_cookie_jar_chains_hash_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (wget_cookie_jar_chains.hash_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_wget_cookie_jar_chains_hash_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        wget_cookie_jar_chains.hash_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        wget_cookie_jar_chains.hash_function = fun_ptr;
    }
    return (void *)&result;
}

char_ptr *get_wget_cookie_jar_chains_test_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (wget_cookie_jar_chains.test_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_wget_cookie_jar_chains_test_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        wget_cookie_jar_chains.test_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        wget_cookie_jar_chains.test_function = fun_ptr;
    }
    return (void *)&result;
}

wget_cookie_jar_chains_cells_ret_t *get_wget_cookie_jar_chains_cells_1_svc(struct svc_req *rqstp)
{
    static wget_cookie_jar_chains_cells_ret_t result;
        if(wget_cookie_jar_chains_cells==NULL) {
            result.wget_cookie_jar_chains_cells.cell_rpc_ptr_len=0;
            result.wget_cookie_jar_chains_cells.cell_rpc_ptr_val=NULL;
        } else {
            result.wget_cookie_jar_chains_cells.cell_rpc_ptr_len=1;
            result.wget_cookie_jar_chains_cells.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
            if(result.wget_cookie_jar_chains_cells.cell_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.wget_cookie_jar_chains_cells.cell_rpc_ptr_len;i0++) {
    if (wget_cookie_jar_chains_cells[i0].key == NULL) {result.wget_cookie_jar_chains_cells.cell_rpc_ptr_val[i0].key.key_len = 0;result.wget_cookie_jar_chains_cells.cell_rpc_ptr_val[i0].key.key_val = NULL;}

    else{result.wget_cookie_jar_chains_cells.cell_rpc_ptr_val[i0].key.key_len = sizeof(wget_cookie_jar_chains_cells[i0].key);result.wget_cookie_jar_chains_cells.cell_rpc_ptr_val[i0].key.key_val = malloc(result.wget_cookie_jar_chains_cells.cell_rpc_ptr_val[i0].key.key_len);

    	if (result.wget_cookie_jar_chains_cells.cell_rpc_ptr_val[i0].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.wget_cookie_jar_chains_cells.cell_rpc_ptr_val[i0].key.key_val, wget_cookie_jar_chains_cells[i0].key, result.wget_cookie_jar_chains_cells.cell_rpc_ptr_val[i0].key.key_len);}

    if (wget_cookie_jar_chains_cells[i0].value == NULL) {result.wget_cookie_jar_chains_cells.cell_rpc_ptr_val[i0].value.value_len = 0;result.wget_cookie_jar_chains_cells.cell_rpc_ptr_val[i0].value.value_val = NULL;}

    else{result.wget_cookie_jar_chains_cells.cell_rpc_ptr_val[i0].value.value_len = sizeof(wget_cookie_jar_chains_cells[i0].value);result.wget_cookie_jar_chains_cells.cell_rpc_ptr_val[i0].value.value_val = malloc(result.wget_cookie_jar_chains_cells.cell_rpc_ptr_val[i0].value.value_len);

    	if (result.wget_cookie_jar_chains_cells.cell_rpc_ptr_val[i0].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.wget_cookie_jar_chains_cells.cell_rpc_ptr_val[i0].value.value_val, wget_cookie_jar_chains_cells[i0].value, result.wget_cookie_jar_chains_cells.cell_rpc_ptr_val[i0].value.value_len);}

            }
        }
    return &result;
}

void *set_wget_cookie_jar_chains_cells_1_svc(cell_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct cell *local_wget_cookie_jar_chains_cells=NULL;
    if(arg1.cell_rpc_ptr_val!=NULL) {
    local_wget_cookie_jar_chains_cells=(struct cell *)malloc(arg1.cell_rpc_ptr_len*sizeof(struct cell));
    if(local_wget_cookie_jar_chains_cells==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.cell_rpc_ptr_len;i0++) {
    if (arg1.cell_rpc_ptr_val[i0].key.key_val == NULL) {local_wget_cookie_jar_chains_cells[i0].key=NULL;}

    else {local_wget_cookie_jar_chains_cells[i0].key=malloc(arg1.cell_rpc_ptr_val[i0].key.key_len);

    	if (local_wget_cookie_jar_chains_cells[i0].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_wget_cookie_jar_chains_cells[i0].key,arg1.cell_rpc_ptr_val[i0].key.key_val,arg1.cell_rpc_ptr_val[i0].key.key_len);

    }

    if (arg1.cell_rpc_ptr_val[i0].value.value_val == NULL) {local_wget_cookie_jar_chains_cells[i0].value=NULL;}

    else {local_wget_cookie_jar_chains_cells[i0].value=malloc(arg1.cell_rpc_ptr_val[i0].value.value_len);

    	if (local_wget_cookie_jar_chains_cells[i0].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_wget_cookie_jar_chains_cells[i0].value,arg1.cell_rpc_ptr_val[i0].value.value_val,arg1.cell_rpc_ptr_val[i0].value.value_len);

    }

    	}
    }
    memcpy(&wget_cookie_jar_chains_cells, &local_wget_cookie_jar_chains_cells, sizeof(cell));
    return (void *)&result;
}

wget_cookie_jar_chains_cells_key_ret_t *get_wget_cookie_jar_chains_cells_key_1_svc(struct svc_req *rqstp)
{
    static wget_cookie_jar_chains_cells_key_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 wget_cookie_jar_chains_cells.key 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t wget_cookie_jar_chains_cells_key_size; // 假设有这样的辅助变量
    if (wget_cookie_jar_chains_cells.key == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = wget_cookie_jar_chains_cells_key_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = wget_cookie_jar_chains_cells.key;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t wget_cookie_jar_chains_cells_key_size; // 假设有这样的辅助变量
void *set_wget_cookie_jar_chains_cells_key_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (wget_cookie_jar_chains_cells.key != NULL) {
        free(wget_cookie_jar_chains_cells.key);
        wget_cookie_jar_chains_cells.key = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        wget_cookie_jar_chains_cells.key = malloc(value.bytes.bytes_len);
        if (wget_cookie_jar_chains_cells.key == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(wget_cookie_jar_chains_cells.key, value.bytes.bytes_val, value.bytes.bytes_len);
            wget_cookie_jar_chains_cells_key_size = value.size;
        }
    }
    return (void *)&result;
}

wget_cookie_jar_chains_cells_value_ret_t *get_wget_cookie_jar_chains_cells_value_1_svc(struct svc_req *rqstp)
{
    static wget_cookie_jar_chains_cells_value_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 wget_cookie_jar_chains_cells.value 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t wget_cookie_jar_chains_cells_value_size; // 假设有这样的辅助变量
    if (wget_cookie_jar_chains_cells.value == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = wget_cookie_jar_chains_cells_value_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = wget_cookie_jar_chains_cells.value;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t wget_cookie_jar_chains_cells_value_size; // 假设有这样的辅助变量
void *set_wget_cookie_jar_chains_cells_value_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (wget_cookie_jar_chains_cells.value != NULL) {
        free(wget_cookie_jar_chains_cells.value);
        wget_cookie_jar_chains_cells.value = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        wget_cookie_jar_chains_cells.value = malloc(value.bytes.bytes_len);
        if (wget_cookie_jar_chains_cells.value == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(wget_cookie_jar_chains_cells.value, value.bytes.bytes_val, value.bytes.bytes_len);
            wget_cookie_jar_chains_cells_value_size = value.size;
        }
    }
    return (void *)&result;
}

wget_cookie_jar_chains_size_ret_t *get_wget_cookie_jar_chains_size_1_svc(struct svc_req *rqstp)
{
    static wget_cookie_jar_chains_size_ret_t result;
    result.value = wget_cookie_jar_chains.size;
    return &result;
}

void *set_wget_cookie_jar_chains_size_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    wget_cookie_jar_chains.size = value;
    return (void *)&result;
}

wget_cookie_jar_chains_count_ret_t *get_wget_cookie_jar_chains_count_1_svc(struct svc_req *rqstp)
{
    static wget_cookie_jar_chains_count_ret_t result;
    result.value = wget_cookie_jar_chains.count;
    return &result;
}

void *set_wget_cookie_jar_chains_count_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    wget_cookie_jar_chains.count = value;
    return (void *)&result;
}

wget_cookie_jar_chains_resize_threshold_ret_t *get_wget_cookie_jar_chains_resize_threshold_1_svc(struct svc_req *rqstp)
{
    static wget_cookie_jar_chains_resize_threshold_ret_t result;
    result.value = wget_cookie_jar_chains.resize_threshold;
    return &result;
}

void *set_wget_cookie_jar_chains_resize_threshold_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    wget_cookie_jar_chains.resize_threshold = value;
    return (void *)&result;
}

wget_cookie_jar_chains_prime_offset_ret_t *get_wget_cookie_jar_chains_prime_offset_1_svc(struct svc_req *rqstp)
{
    static wget_cookie_jar_chains_prime_offset_ret_t result;
    result.value = wget_cookie_jar_chains.prime_offset;
    return &result;
}

void *set_wget_cookie_jar_chains_prime_offset_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    wget_cookie_jar_chains.prime_offset = value;
    return (void *)&result;
}

wget_cookie_jar_cookie_count_ret_t *get_wget_cookie_jar_cookie_count_1_svc(struct svc_req *rqstp)
{
    static wget_cookie_jar_cookie_count_ret_t result;
    result.value = wget_cookie_jar.cookie_count;
    return &result;
}

void *set_wget_cookie_jar_cookie_count_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    wget_cookie_jar.cookie_count = value;
    return (void *)&result;
}

dl_file_url_map_ret_t *get_dl_file_url_map_1_svc(struct svc_req *rqstp)
{
    static dl_file_url_map_ret_t result;
        if(dl_file_url_map==NULL) {
            result.dl_file_url_map.hash_table_rpc_ptr_len=0;
            result.dl_file_url_map.hash_table_rpc_ptr_val=NULL;
        } else {
            result.dl_file_url_map.hash_table_rpc_ptr_len=1;
            result.dl_file_url_map.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
            if(result.dl_file_url_map.hash_table_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.dl_file_url_map.hash_table_rpc_ptr_len;i0++) {
    if (dl_file_url_map[i0].cells == NULL) {result.dl_file_url_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;result.dl_file_url_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}

    else {result.dl_file_url_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;result.dl_file_url_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(result.dl_file_url_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));

    	if (result.dl_file_url_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {perror("malloc failed");exit(1);
}
    	for (int i1 = 0; i1 < result.dl_file_url_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {

    if (dl_file_url_map[i0].cells->key == NULL) {result.dl_file_url_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = 0;result.dl_file_url_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = NULL;}

    else{result.dl_file_url_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = sizeof(dl_file_url_map[i0].cells->key);result.dl_file_url_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = malloc(result.dl_file_url_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (result.dl_file_url_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.dl_file_url_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, dl_file_url_map[i0].cells->key, result.dl_file_url_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);}

    if (dl_file_url_map[i0].cells->value == NULL) {result.dl_file_url_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = 0;result.dl_file_url_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = NULL;}

    else{result.dl_file_url_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = sizeof(dl_file_url_map[i0].cells->value);result.dl_file_url_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = malloc(result.dl_file_url_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (result.dl_file_url_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.dl_file_url_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, dl_file_url_map[i0].cells->value, result.dl_file_url_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);}

    	}
    }
    result.dl_file_url_map.hash_table_rpc_ptr_val[i0].size=dl_file_url_map[i0].size;
    result.dl_file_url_map.hash_table_rpc_ptr_val[i0].count=dl_file_url_map[i0].count;
    result.dl_file_url_map.hash_table_rpc_ptr_val[i0].resize_threshold=dl_file_url_map[i0].resize_threshold;
    result.dl_file_url_map.hash_table_rpc_ptr_val[i0].prime_offset=dl_file_url_map[i0].prime_offset;
            }
        }
    return &result;
}

void *set_dl_file_url_map_1_svc(hash_table_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct hash_table *local_dl_file_url_map=NULL;
    if(arg1.hash_table_rpc_ptr_val!=NULL) {
    local_dl_file_url_map=(struct hash_table *)malloc(arg1.hash_table_rpc_ptr_len*sizeof(struct hash_table));
    if(local_dl_file_url_map==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.hash_table_rpc_ptr_len;i0++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {local_dl_file_url_map[i0].cells=NULL;}
    else {local_dl_file_url_map[i0].cells=(struct cell *)malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(struct cell));
    	if (local_dl_file_url_map[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {local_dl_file_url_map[i0].cells[i1].key=NULL;}

    else {local_dl_file_url_map[i0].cells[i1].key=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (local_dl_file_url_map[i0].cells[i1].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_dl_file_url_map[i0].cells[i1].key,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    }

    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {local_dl_file_url_map[i0].cells[i1].value=NULL;}

    else {local_dl_file_url_map[i0].cells[i1].value=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (local_dl_file_url_map[i0].cells[i1].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_dl_file_url_map[i0].cells[i1].value,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    }

    	}
    }
    local_dl_file_url_map[i0].size=arg1.hash_table_rpc_ptr_val[i0].size;
    local_dl_file_url_map[i0].count=arg1.hash_table_rpc_ptr_val[i0].count;
    local_dl_file_url_map[i0].resize_threshold=arg1.hash_table_rpc_ptr_val[i0].resize_threshold;
    local_dl_file_url_map[i0].prime_offset=arg1.hash_table_rpc_ptr_val[i0].prime_offset;
    	}
    }
    memcpy(&dl_file_url_map, &local_dl_file_url_map, sizeof(hash_table));
    return (void *)&result;
}

char_ptr *get_dl_file_url_map_hash_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (dl_file_url_map.hash_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_dl_file_url_map_hash_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        dl_file_url_map.hash_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        dl_file_url_map.hash_function = fun_ptr;
    }
    return (void *)&result;
}

char_ptr *get_dl_file_url_map_test_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (dl_file_url_map.test_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_dl_file_url_map_test_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        dl_file_url_map.test_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        dl_file_url_map.test_function = fun_ptr;
    }
    return (void *)&result;
}

dl_file_url_map_cells_ret_t *get_dl_file_url_map_cells_1_svc(struct svc_req *rqstp)
{
    static dl_file_url_map_cells_ret_t result;
        if(dl_file_url_map_cells==NULL) {
            result.dl_file_url_map_cells.cell_rpc_ptr_len=0;
            result.dl_file_url_map_cells.cell_rpc_ptr_val=NULL;
        } else {
            result.dl_file_url_map_cells.cell_rpc_ptr_len=1;
            result.dl_file_url_map_cells.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
            if(result.dl_file_url_map_cells.cell_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.dl_file_url_map_cells.cell_rpc_ptr_len;i0++) {
    if (dl_file_url_map_cells[i0].key == NULL) {result.dl_file_url_map_cells.cell_rpc_ptr_val[i0].key.key_len = 0;result.dl_file_url_map_cells.cell_rpc_ptr_val[i0].key.key_val = NULL;}

    else{result.dl_file_url_map_cells.cell_rpc_ptr_val[i0].key.key_len = sizeof(dl_file_url_map_cells[i0].key);result.dl_file_url_map_cells.cell_rpc_ptr_val[i0].key.key_val = malloc(result.dl_file_url_map_cells.cell_rpc_ptr_val[i0].key.key_len);

    	if (result.dl_file_url_map_cells.cell_rpc_ptr_val[i0].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.dl_file_url_map_cells.cell_rpc_ptr_val[i0].key.key_val, dl_file_url_map_cells[i0].key, result.dl_file_url_map_cells.cell_rpc_ptr_val[i0].key.key_len);}

    if (dl_file_url_map_cells[i0].value == NULL) {result.dl_file_url_map_cells.cell_rpc_ptr_val[i0].value.value_len = 0;result.dl_file_url_map_cells.cell_rpc_ptr_val[i0].value.value_val = NULL;}

    else{result.dl_file_url_map_cells.cell_rpc_ptr_val[i0].value.value_len = sizeof(dl_file_url_map_cells[i0].value);result.dl_file_url_map_cells.cell_rpc_ptr_val[i0].value.value_val = malloc(result.dl_file_url_map_cells.cell_rpc_ptr_val[i0].value.value_len);

    	if (result.dl_file_url_map_cells.cell_rpc_ptr_val[i0].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.dl_file_url_map_cells.cell_rpc_ptr_val[i0].value.value_val, dl_file_url_map_cells[i0].value, result.dl_file_url_map_cells.cell_rpc_ptr_val[i0].value.value_len);}

            }
        }
    return &result;
}

void *set_dl_file_url_map_cells_1_svc(cell_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct cell *local_dl_file_url_map_cells=NULL;
    if(arg1.cell_rpc_ptr_val!=NULL) {
    local_dl_file_url_map_cells=(struct cell *)malloc(arg1.cell_rpc_ptr_len*sizeof(struct cell));
    if(local_dl_file_url_map_cells==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.cell_rpc_ptr_len;i0++) {
    if (arg1.cell_rpc_ptr_val[i0].key.key_val == NULL) {local_dl_file_url_map_cells[i0].key=NULL;}

    else {local_dl_file_url_map_cells[i0].key=malloc(arg1.cell_rpc_ptr_val[i0].key.key_len);

    	if (local_dl_file_url_map_cells[i0].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_dl_file_url_map_cells[i0].key,arg1.cell_rpc_ptr_val[i0].key.key_val,arg1.cell_rpc_ptr_val[i0].key.key_len);

    }

    if (arg1.cell_rpc_ptr_val[i0].value.value_val == NULL) {local_dl_file_url_map_cells[i0].value=NULL;}

    else {local_dl_file_url_map_cells[i0].value=malloc(arg1.cell_rpc_ptr_val[i0].value.value_len);

    	if (local_dl_file_url_map_cells[i0].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_dl_file_url_map_cells[i0].value,arg1.cell_rpc_ptr_val[i0].value.value_val,arg1.cell_rpc_ptr_val[i0].value.value_len);

    }

    	}
    }
    memcpy(&dl_file_url_map_cells, &local_dl_file_url_map_cells, sizeof(cell));
    return (void *)&result;
}

dl_file_url_map_cells_key_ret_t *get_dl_file_url_map_cells_key_1_svc(struct svc_req *rqstp)
{
    static dl_file_url_map_cells_key_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 dl_file_url_map_cells.key 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t dl_file_url_map_cells_key_size; // 假设有这样的辅助变量
    if (dl_file_url_map_cells.key == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = dl_file_url_map_cells_key_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = dl_file_url_map_cells.key;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t dl_file_url_map_cells_key_size; // 假设有这样的辅助变量
void *set_dl_file_url_map_cells_key_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (dl_file_url_map_cells.key != NULL) {
        free(dl_file_url_map_cells.key);
        dl_file_url_map_cells.key = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        dl_file_url_map_cells.key = malloc(value.bytes.bytes_len);
        if (dl_file_url_map_cells.key == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(dl_file_url_map_cells.key, value.bytes.bytes_val, value.bytes.bytes_len);
            dl_file_url_map_cells_key_size = value.size;
        }
    }
    return (void *)&result;
}

dl_file_url_map_cells_value_ret_t *get_dl_file_url_map_cells_value_1_svc(struct svc_req *rqstp)
{
    static dl_file_url_map_cells_value_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 dl_file_url_map_cells.value 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t dl_file_url_map_cells_value_size; // 假设有这样的辅助变量
    if (dl_file_url_map_cells.value == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = dl_file_url_map_cells_value_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = dl_file_url_map_cells.value;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t dl_file_url_map_cells_value_size; // 假设有这样的辅助变量
void *set_dl_file_url_map_cells_value_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (dl_file_url_map_cells.value != NULL) {
        free(dl_file_url_map_cells.value);
        dl_file_url_map_cells.value = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        dl_file_url_map_cells.value = malloc(value.bytes.bytes_len);
        if (dl_file_url_map_cells.value == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(dl_file_url_map_cells.value, value.bytes.bytes_val, value.bytes.bytes_len);
            dl_file_url_map_cells_value_size = value.size;
        }
    }
    return (void *)&result;
}

dl_file_url_map_size_ret_t *get_dl_file_url_map_size_1_svc(struct svc_req *rqstp)
{
    static dl_file_url_map_size_ret_t result;
    result.value = dl_file_url_map.size;
    return &result;
}

void *set_dl_file_url_map_size_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    dl_file_url_map.size = value;
    return (void *)&result;
}

dl_file_url_map_count_ret_t *get_dl_file_url_map_count_1_svc(struct svc_req *rqstp)
{
    static dl_file_url_map_count_ret_t result;
    result.value = dl_file_url_map.count;
    return &result;
}

void *set_dl_file_url_map_count_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    dl_file_url_map.count = value;
    return (void *)&result;
}

dl_file_url_map_resize_threshold_ret_t *get_dl_file_url_map_resize_threshold_1_svc(struct svc_req *rqstp)
{
    static dl_file_url_map_resize_threshold_ret_t result;
    result.value = dl_file_url_map.resize_threshold;
    return &result;
}

void *set_dl_file_url_map_resize_threshold_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    dl_file_url_map.resize_threshold = value;
    return (void *)&result;
}

dl_file_url_map_prime_offset_ret_t *get_dl_file_url_map_prime_offset_1_svc(struct svc_req *rqstp)
{
    static dl_file_url_map_prime_offset_ret_t result;
    result.value = dl_file_url_map.prime_offset;
    return &result;
}

void *set_dl_file_url_map_prime_offset_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    dl_file_url_map.prime_offset = value;
    return (void *)&result;
}

downloaded_css_set_ret_t *get_downloaded_css_set_1_svc(struct svc_req *rqstp)
{
    static downloaded_css_set_ret_t result;
        if(downloaded_css_set==NULL) {
            result.downloaded_css_set.hash_table_rpc_ptr_len=0;
            result.downloaded_css_set.hash_table_rpc_ptr_val=NULL;
        } else {
            result.downloaded_css_set.hash_table_rpc_ptr_len=1;
            result.downloaded_css_set.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
            if(result.downloaded_css_set.hash_table_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.downloaded_css_set.hash_table_rpc_ptr_len;i0++) {
    if (downloaded_css_set[i0].cells == NULL) {result.downloaded_css_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;result.downloaded_css_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}

    else {result.downloaded_css_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;result.downloaded_css_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(result.downloaded_css_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));

    	if (result.downloaded_css_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {perror("malloc failed");exit(1);
}
    	for (int i1 = 0; i1 < result.downloaded_css_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {

    if (downloaded_css_set[i0].cells->key == NULL) {result.downloaded_css_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = 0;result.downloaded_css_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = NULL;}

    else{result.downloaded_css_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = sizeof(downloaded_css_set[i0].cells->key);result.downloaded_css_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = malloc(result.downloaded_css_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (result.downloaded_css_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.downloaded_css_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, downloaded_css_set[i0].cells->key, result.downloaded_css_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);}

    if (downloaded_css_set[i0].cells->value == NULL) {result.downloaded_css_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = 0;result.downloaded_css_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = NULL;}

    else{result.downloaded_css_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = sizeof(downloaded_css_set[i0].cells->value);result.downloaded_css_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = malloc(result.downloaded_css_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (result.downloaded_css_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.downloaded_css_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, downloaded_css_set[i0].cells->value, result.downloaded_css_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);}

    	}
    }
    result.downloaded_css_set.hash_table_rpc_ptr_val[i0].size=downloaded_css_set[i0].size;
    result.downloaded_css_set.hash_table_rpc_ptr_val[i0].count=downloaded_css_set[i0].count;
    result.downloaded_css_set.hash_table_rpc_ptr_val[i0].resize_threshold=downloaded_css_set[i0].resize_threshold;
    result.downloaded_css_set.hash_table_rpc_ptr_val[i0].prime_offset=downloaded_css_set[i0].prime_offset;
            }
        }
    return &result;
}

void *set_downloaded_css_set_1_svc(hash_table_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct hash_table *local_downloaded_css_set=NULL;
    if(arg1.hash_table_rpc_ptr_val!=NULL) {
    local_downloaded_css_set=(struct hash_table *)malloc(arg1.hash_table_rpc_ptr_len*sizeof(struct hash_table));
    if(local_downloaded_css_set==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.hash_table_rpc_ptr_len;i0++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {local_downloaded_css_set[i0].cells=NULL;}
    else {local_downloaded_css_set[i0].cells=(struct cell *)malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(struct cell));
    	if (local_downloaded_css_set[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {local_downloaded_css_set[i0].cells[i1].key=NULL;}

    else {local_downloaded_css_set[i0].cells[i1].key=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (local_downloaded_css_set[i0].cells[i1].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_downloaded_css_set[i0].cells[i1].key,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    }

    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {local_downloaded_css_set[i0].cells[i1].value=NULL;}

    else {local_downloaded_css_set[i0].cells[i1].value=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (local_downloaded_css_set[i0].cells[i1].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_downloaded_css_set[i0].cells[i1].value,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    }

    	}
    }
    local_downloaded_css_set[i0].size=arg1.hash_table_rpc_ptr_val[i0].size;
    local_downloaded_css_set[i0].count=arg1.hash_table_rpc_ptr_val[i0].count;
    local_downloaded_css_set[i0].resize_threshold=arg1.hash_table_rpc_ptr_val[i0].resize_threshold;
    local_downloaded_css_set[i0].prime_offset=arg1.hash_table_rpc_ptr_val[i0].prime_offset;
    	}
    }
    memcpy(&downloaded_css_set, &local_downloaded_css_set, sizeof(hash_table));
    return (void *)&result;
}

char_ptr *get_downloaded_css_set_hash_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (downloaded_css_set.hash_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_downloaded_css_set_hash_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        downloaded_css_set.hash_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        downloaded_css_set.hash_function = fun_ptr;
    }
    return (void *)&result;
}

char_ptr *get_downloaded_css_set_test_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (downloaded_css_set.test_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_downloaded_css_set_test_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        downloaded_css_set.test_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        downloaded_css_set.test_function = fun_ptr;
    }
    return (void *)&result;
}

downloaded_css_set_cells_ret_t *get_downloaded_css_set_cells_1_svc(struct svc_req *rqstp)
{
    static downloaded_css_set_cells_ret_t result;
        if(downloaded_css_set_cells==NULL) {
            result.downloaded_css_set_cells.cell_rpc_ptr_len=0;
            result.downloaded_css_set_cells.cell_rpc_ptr_val=NULL;
        } else {
            result.downloaded_css_set_cells.cell_rpc_ptr_len=1;
            result.downloaded_css_set_cells.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
            if(result.downloaded_css_set_cells.cell_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.downloaded_css_set_cells.cell_rpc_ptr_len;i0++) {
    if (downloaded_css_set_cells[i0].key == NULL) {result.downloaded_css_set_cells.cell_rpc_ptr_val[i0].key.key_len = 0;result.downloaded_css_set_cells.cell_rpc_ptr_val[i0].key.key_val = NULL;}

    else{result.downloaded_css_set_cells.cell_rpc_ptr_val[i0].key.key_len = sizeof(downloaded_css_set_cells[i0].key);result.downloaded_css_set_cells.cell_rpc_ptr_val[i0].key.key_val = malloc(result.downloaded_css_set_cells.cell_rpc_ptr_val[i0].key.key_len);

    	if (result.downloaded_css_set_cells.cell_rpc_ptr_val[i0].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.downloaded_css_set_cells.cell_rpc_ptr_val[i0].key.key_val, downloaded_css_set_cells[i0].key, result.downloaded_css_set_cells.cell_rpc_ptr_val[i0].key.key_len);}

    if (downloaded_css_set_cells[i0].value == NULL) {result.downloaded_css_set_cells.cell_rpc_ptr_val[i0].value.value_len = 0;result.downloaded_css_set_cells.cell_rpc_ptr_val[i0].value.value_val = NULL;}

    else{result.downloaded_css_set_cells.cell_rpc_ptr_val[i0].value.value_len = sizeof(downloaded_css_set_cells[i0].value);result.downloaded_css_set_cells.cell_rpc_ptr_val[i0].value.value_val = malloc(result.downloaded_css_set_cells.cell_rpc_ptr_val[i0].value.value_len);

    	if (result.downloaded_css_set_cells.cell_rpc_ptr_val[i0].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.downloaded_css_set_cells.cell_rpc_ptr_val[i0].value.value_val, downloaded_css_set_cells[i0].value, result.downloaded_css_set_cells.cell_rpc_ptr_val[i0].value.value_len);}

            }
        }
    return &result;
}

void *set_downloaded_css_set_cells_1_svc(cell_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct cell *local_downloaded_css_set_cells=NULL;
    if(arg1.cell_rpc_ptr_val!=NULL) {
    local_downloaded_css_set_cells=(struct cell *)malloc(arg1.cell_rpc_ptr_len*sizeof(struct cell));
    if(local_downloaded_css_set_cells==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.cell_rpc_ptr_len;i0++) {
    if (arg1.cell_rpc_ptr_val[i0].key.key_val == NULL) {local_downloaded_css_set_cells[i0].key=NULL;}

    else {local_downloaded_css_set_cells[i0].key=malloc(arg1.cell_rpc_ptr_val[i0].key.key_len);

    	if (local_downloaded_css_set_cells[i0].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_downloaded_css_set_cells[i0].key,arg1.cell_rpc_ptr_val[i0].key.key_val,arg1.cell_rpc_ptr_val[i0].key.key_len);

    }

    if (arg1.cell_rpc_ptr_val[i0].value.value_val == NULL) {local_downloaded_css_set_cells[i0].value=NULL;}

    else {local_downloaded_css_set_cells[i0].value=malloc(arg1.cell_rpc_ptr_val[i0].value.value_len);

    	if (local_downloaded_css_set_cells[i0].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_downloaded_css_set_cells[i0].value,arg1.cell_rpc_ptr_val[i0].value.value_val,arg1.cell_rpc_ptr_val[i0].value.value_len);

    }

    	}
    }
    memcpy(&downloaded_css_set_cells, &local_downloaded_css_set_cells, sizeof(cell));
    return (void *)&result;
}

downloaded_css_set_cells_key_ret_t *get_downloaded_css_set_cells_key_1_svc(struct svc_req *rqstp)
{
    static downloaded_css_set_cells_key_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 downloaded_css_set_cells.key 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t downloaded_css_set_cells_key_size; // 假设有这样的辅助变量
    if (downloaded_css_set_cells.key == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = downloaded_css_set_cells_key_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = downloaded_css_set_cells.key;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t downloaded_css_set_cells_key_size; // 假设有这样的辅助变量
void *set_downloaded_css_set_cells_key_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (downloaded_css_set_cells.key != NULL) {
        free(downloaded_css_set_cells.key);
        downloaded_css_set_cells.key = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        downloaded_css_set_cells.key = malloc(value.bytes.bytes_len);
        if (downloaded_css_set_cells.key == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(downloaded_css_set_cells.key, value.bytes.bytes_val, value.bytes.bytes_len);
            downloaded_css_set_cells_key_size = value.size;
        }
    }
    return (void *)&result;
}

downloaded_css_set_cells_value_ret_t *get_downloaded_css_set_cells_value_1_svc(struct svc_req *rqstp)
{
    static downloaded_css_set_cells_value_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 downloaded_css_set_cells.value 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t downloaded_css_set_cells_value_size; // 假设有这样的辅助变量
    if (downloaded_css_set_cells.value == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = downloaded_css_set_cells_value_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = downloaded_css_set_cells.value;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t downloaded_css_set_cells_value_size; // 假设有这样的辅助变量
void *set_downloaded_css_set_cells_value_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (downloaded_css_set_cells.value != NULL) {
        free(downloaded_css_set_cells.value);
        downloaded_css_set_cells.value = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        downloaded_css_set_cells.value = malloc(value.bytes.bytes_len);
        if (downloaded_css_set_cells.value == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(downloaded_css_set_cells.value, value.bytes.bytes_val, value.bytes.bytes_len);
            downloaded_css_set_cells_value_size = value.size;
        }
    }
    return (void *)&result;
}

downloaded_css_set_size_ret_t *get_downloaded_css_set_size_1_svc(struct svc_req *rqstp)
{
    static downloaded_css_set_size_ret_t result;
    result.value = downloaded_css_set.size;
    return &result;
}

void *set_downloaded_css_set_size_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    downloaded_css_set.size = value;
    return (void *)&result;
}

downloaded_css_set_count_ret_t *get_downloaded_css_set_count_1_svc(struct svc_req *rqstp)
{
    static downloaded_css_set_count_ret_t result;
    result.value = downloaded_css_set.count;
    return &result;
}

void *set_downloaded_css_set_count_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    downloaded_css_set.count = value;
    return (void *)&result;
}

downloaded_css_set_resize_threshold_ret_t *get_downloaded_css_set_resize_threshold_1_svc(struct svc_req *rqstp)
{
    static downloaded_css_set_resize_threshold_ret_t result;
    result.value = downloaded_css_set.resize_threshold;
    return &result;
}

void *set_downloaded_css_set_resize_threshold_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    downloaded_css_set.resize_threshold = value;
    return (void *)&result;
}

downloaded_css_set_prime_offset_ret_t *get_downloaded_css_set_prime_offset_1_svc(struct svc_req *rqstp)
{
    static downloaded_css_set_prime_offset_ret_t result;
    result.value = downloaded_css_set.prime_offset;
    return &result;
}

void *set_downloaded_css_set_prime_offset_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    downloaded_css_set.prime_offset = value;
    return (void *)&result;
}

char_ptr *get_warc_current_filename_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    if (warc_current_filename == NULL) {
        result.charptr.charptr_len = 0;
        result.charptr.charptr_val = NULL;
    } else {
        result.charptr.charptr_len = strlen(warc_current_filename) + 1;
        result.charptr.charptr_val = warc_current_filename;
    }
    return &result;
}

void *set_warc_current_filename_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result;
    if (warc_current_filename != NULL) {
        free(warc_current_filename);
    }
    if (value.charptr.charptr_val == NULL) {
        warc_current_filename = NULL;
    } else {
        warc_current_filename=malloc(value.charptr.charptr_len);
        memcpy(warc_current_filename, value.charptr.charptr_val, value.charptr.charptr_len);
    }
    return (void *)&result;
}

dl_url_file_map_ret_t *get_dl_url_file_map_1_svc(struct svc_req *rqstp)
{
    static dl_url_file_map_ret_t result;
        if(dl_url_file_map==NULL) {
            result.dl_url_file_map.hash_table_rpc_ptr_len=0;
            result.dl_url_file_map.hash_table_rpc_ptr_val=NULL;
        } else {
            result.dl_url_file_map.hash_table_rpc_ptr_len=1;
            result.dl_url_file_map.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
            if(result.dl_url_file_map.hash_table_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.dl_url_file_map.hash_table_rpc_ptr_len;i0++) {
    if (dl_url_file_map[i0].cells == NULL) {result.dl_url_file_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;result.dl_url_file_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}

    else {result.dl_url_file_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;result.dl_url_file_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(result.dl_url_file_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));

    	if (result.dl_url_file_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {perror("malloc failed");exit(1);
}
    	for (int i1 = 0; i1 < result.dl_url_file_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {

    if (dl_url_file_map[i0].cells->key == NULL) {result.dl_url_file_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = 0;result.dl_url_file_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = NULL;}

    else{result.dl_url_file_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = sizeof(dl_url_file_map[i0].cells->key);result.dl_url_file_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = malloc(result.dl_url_file_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (result.dl_url_file_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.dl_url_file_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, dl_url_file_map[i0].cells->key, result.dl_url_file_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);}

    if (dl_url_file_map[i0].cells->value == NULL) {result.dl_url_file_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = 0;result.dl_url_file_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = NULL;}

    else{result.dl_url_file_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = sizeof(dl_url_file_map[i0].cells->value);result.dl_url_file_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = malloc(result.dl_url_file_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (result.dl_url_file_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.dl_url_file_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, dl_url_file_map[i0].cells->value, result.dl_url_file_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);}

    	}
    }
    result.dl_url_file_map.hash_table_rpc_ptr_val[i0].size=dl_url_file_map[i0].size;
    result.dl_url_file_map.hash_table_rpc_ptr_val[i0].count=dl_url_file_map[i0].count;
    result.dl_url_file_map.hash_table_rpc_ptr_val[i0].resize_threshold=dl_url_file_map[i0].resize_threshold;
    result.dl_url_file_map.hash_table_rpc_ptr_val[i0].prime_offset=dl_url_file_map[i0].prime_offset;
            }
        }
    return &result;
}

void *set_dl_url_file_map_1_svc(hash_table_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct hash_table *local_dl_url_file_map=NULL;
    if(arg1.hash_table_rpc_ptr_val!=NULL) {
    local_dl_url_file_map=(struct hash_table *)malloc(arg1.hash_table_rpc_ptr_len*sizeof(struct hash_table));
    if(local_dl_url_file_map==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.hash_table_rpc_ptr_len;i0++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {local_dl_url_file_map[i0].cells=NULL;}
    else {local_dl_url_file_map[i0].cells=(struct cell *)malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(struct cell));
    	if (local_dl_url_file_map[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {local_dl_url_file_map[i0].cells[i1].key=NULL;}

    else {local_dl_url_file_map[i0].cells[i1].key=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (local_dl_url_file_map[i0].cells[i1].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_dl_url_file_map[i0].cells[i1].key,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    }

    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {local_dl_url_file_map[i0].cells[i1].value=NULL;}

    else {local_dl_url_file_map[i0].cells[i1].value=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (local_dl_url_file_map[i0].cells[i1].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_dl_url_file_map[i0].cells[i1].value,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    }

    	}
    }
    local_dl_url_file_map[i0].size=arg1.hash_table_rpc_ptr_val[i0].size;
    local_dl_url_file_map[i0].count=arg1.hash_table_rpc_ptr_val[i0].count;
    local_dl_url_file_map[i0].resize_threshold=arg1.hash_table_rpc_ptr_val[i0].resize_threshold;
    local_dl_url_file_map[i0].prime_offset=arg1.hash_table_rpc_ptr_val[i0].prime_offset;
    	}
    }
    memcpy(&dl_url_file_map, &local_dl_url_file_map, sizeof(hash_table));
    return (void *)&result;
}

char_ptr *get_dl_url_file_map_hash_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (dl_url_file_map.hash_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_dl_url_file_map_hash_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        dl_url_file_map.hash_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        dl_url_file_map.hash_function = fun_ptr;
    }
    return (void *)&result;
}

char_ptr *get_dl_url_file_map_test_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (dl_url_file_map.test_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_dl_url_file_map_test_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        dl_url_file_map.test_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        dl_url_file_map.test_function = fun_ptr;
    }
    return (void *)&result;
}

dl_url_file_map_cells_ret_t *get_dl_url_file_map_cells_1_svc(struct svc_req *rqstp)
{
    static dl_url_file_map_cells_ret_t result;
        if(dl_url_file_map_cells==NULL) {
            result.dl_url_file_map_cells.cell_rpc_ptr_len=0;
            result.dl_url_file_map_cells.cell_rpc_ptr_val=NULL;
        } else {
            result.dl_url_file_map_cells.cell_rpc_ptr_len=1;
            result.dl_url_file_map_cells.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
            if(result.dl_url_file_map_cells.cell_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.dl_url_file_map_cells.cell_rpc_ptr_len;i0++) {
    if (dl_url_file_map_cells[i0].key == NULL) {result.dl_url_file_map_cells.cell_rpc_ptr_val[i0].key.key_len = 0;result.dl_url_file_map_cells.cell_rpc_ptr_val[i0].key.key_val = NULL;}

    else{result.dl_url_file_map_cells.cell_rpc_ptr_val[i0].key.key_len = sizeof(dl_url_file_map_cells[i0].key);result.dl_url_file_map_cells.cell_rpc_ptr_val[i0].key.key_val = malloc(result.dl_url_file_map_cells.cell_rpc_ptr_val[i0].key.key_len);

    	if (result.dl_url_file_map_cells.cell_rpc_ptr_val[i0].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.dl_url_file_map_cells.cell_rpc_ptr_val[i0].key.key_val, dl_url_file_map_cells[i0].key, result.dl_url_file_map_cells.cell_rpc_ptr_val[i0].key.key_len);}

    if (dl_url_file_map_cells[i0].value == NULL) {result.dl_url_file_map_cells.cell_rpc_ptr_val[i0].value.value_len = 0;result.dl_url_file_map_cells.cell_rpc_ptr_val[i0].value.value_val = NULL;}

    else{result.dl_url_file_map_cells.cell_rpc_ptr_val[i0].value.value_len = sizeof(dl_url_file_map_cells[i0].value);result.dl_url_file_map_cells.cell_rpc_ptr_val[i0].value.value_val = malloc(result.dl_url_file_map_cells.cell_rpc_ptr_val[i0].value.value_len);

    	if (result.dl_url_file_map_cells.cell_rpc_ptr_val[i0].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.dl_url_file_map_cells.cell_rpc_ptr_val[i0].value.value_val, dl_url_file_map_cells[i0].value, result.dl_url_file_map_cells.cell_rpc_ptr_val[i0].value.value_len);}

            }
        }
    return &result;
}

void *set_dl_url_file_map_cells_1_svc(cell_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct cell *local_dl_url_file_map_cells=NULL;
    if(arg1.cell_rpc_ptr_val!=NULL) {
    local_dl_url_file_map_cells=(struct cell *)malloc(arg1.cell_rpc_ptr_len*sizeof(struct cell));
    if(local_dl_url_file_map_cells==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.cell_rpc_ptr_len;i0++) {
    if (arg1.cell_rpc_ptr_val[i0].key.key_val == NULL) {local_dl_url_file_map_cells[i0].key=NULL;}

    else {local_dl_url_file_map_cells[i0].key=malloc(arg1.cell_rpc_ptr_val[i0].key.key_len);

    	if (local_dl_url_file_map_cells[i0].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_dl_url_file_map_cells[i0].key,arg1.cell_rpc_ptr_val[i0].key.key_val,arg1.cell_rpc_ptr_val[i0].key.key_len);

    }

    if (arg1.cell_rpc_ptr_val[i0].value.value_val == NULL) {local_dl_url_file_map_cells[i0].value=NULL;}

    else {local_dl_url_file_map_cells[i0].value=malloc(arg1.cell_rpc_ptr_val[i0].value.value_len);

    	if (local_dl_url_file_map_cells[i0].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_dl_url_file_map_cells[i0].value,arg1.cell_rpc_ptr_val[i0].value.value_val,arg1.cell_rpc_ptr_val[i0].value.value_len);

    }

    	}
    }
    memcpy(&dl_url_file_map_cells, &local_dl_url_file_map_cells, sizeof(cell));
    return (void *)&result;
}

dl_url_file_map_cells_key_ret_t *get_dl_url_file_map_cells_key_1_svc(struct svc_req *rqstp)
{
    static dl_url_file_map_cells_key_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 dl_url_file_map_cells.key 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t dl_url_file_map_cells_key_size; // 假设有这样的辅助变量
    if (dl_url_file_map_cells.key == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = dl_url_file_map_cells_key_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = dl_url_file_map_cells.key;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t dl_url_file_map_cells_key_size; // 假设有这样的辅助变量
void *set_dl_url_file_map_cells_key_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (dl_url_file_map_cells.key != NULL) {
        free(dl_url_file_map_cells.key);
        dl_url_file_map_cells.key = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        dl_url_file_map_cells.key = malloc(value.bytes.bytes_len);
        if (dl_url_file_map_cells.key == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(dl_url_file_map_cells.key, value.bytes.bytes_val, value.bytes.bytes_len);
            dl_url_file_map_cells_key_size = value.size;
        }
    }
    return (void *)&result;
}

dl_url_file_map_cells_value_ret_t *get_dl_url_file_map_cells_value_1_svc(struct svc_req *rqstp)
{
    static dl_url_file_map_cells_value_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 dl_url_file_map_cells.value 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t dl_url_file_map_cells_value_size; // 假设有这样的辅助变量
    if (dl_url_file_map_cells.value == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = dl_url_file_map_cells_value_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = dl_url_file_map_cells.value;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t dl_url_file_map_cells_value_size; // 假设有这样的辅助变量
void *set_dl_url_file_map_cells_value_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (dl_url_file_map_cells.value != NULL) {
        free(dl_url_file_map_cells.value);
        dl_url_file_map_cells.value = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        dl_url_file_map_cells.value = malloc(value.bytes.bytes_len);
        if (dl_url_file_map_cells.value == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(dl_url_file_map_cells.value, value.bytes.bytes_val, value.bytes.bytes_len);
            dl_url_file_map_cells_value_size = value.size;
        }
    }
    return (void *)&result;
}

dl_url_file_map_size_ret_t *get_dl_url_file_map_size_1_svc(struct svc_req *rqstp)
{
    static dl_url_file_map_size_ret_t result;
    result.value = dl_url_file_map.size;
    return &result;
}

void *set_dl_url_file_map_size_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    dl_url_file_map.size = value;
    return (void *)&result;
}

dl_url_file_map_count_ret_t *get_dl_url_file_map_count_1_svc(struct svc_req *rqstp)
{
    static dl_url_file_map_count_ret_t result;
    result.value = dl_url_file_map.count;
    return &result;
}

void *set_dl_url_file_map_count_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    dl_url_file_map.count = value;
    return (void *)&result;
}

dl_url_file_map_resize_threshold_ret_t *get_dl_url_file_map_resize_threshold_1_svc(struct svc_req *rqstp)
{
    static dl_url_file_map_resize_threshold_ret_t result;
    result.value = dl_url_file_map.resize_threshold;
    return &result;
}

void *set_dl_url_file_map_resize_threshold_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    dl_url_file_map.resize_threshold = value;
    return (void *)&result;
}

dl_url_file_map_prime_offset_ret_t *get_dl_url_file_map_prime_offset_1_svc(struct svc_req *rqstp)
{
    static dl_url_file_map_prime_offset_ret_t result;
    result.value = dl_url_file_map.prime_offset;
    return &result;
}

void *set_dl_url_file_map_prime_offset_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    dl_url_file_map.prime_offset = value;
    return (void *)&result;
}

char_ptr *get_warc_log_fp_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *filename=get_filename_from_fp(warc_log_fp);
    if (filename == NULL) {
        result.charptr.charptr_len = 0;
        result.charptr.charptr_val = NULL;
    } else {
        result.charptr.charptr_len = strlen(filename) + 1;
        result.charptr.charptr_val = filename;
    }
    return &result;
}

            warc_log_fp = stdin;
        } else if (strcmp(value.charptr.charptr_val, "stdout") == 0) {
            warc_log_fp = stdout;
        } else if (strcmp(value.charptr.charptr_val, "stderr") == 0) {
            warc_log_fp = stderr;
        } else {
            warc_log_fp = fopen(value.charptr.charptr_val, "r+");
            if ({var_name} == NULL) {
                perror("fopen failed");
                return NULL;
            }
        }
void *set_warc_log_fp_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result;
    if (value.charptr.charptr_val == NULL) {
        warc_log_fp = NULL;
    } else {
        // 假设文件模式为读写
      if (strcmp(value.charptr.charptr_val, "stdin") == 0) {
j    }
    return (void *)&result;
}

final_exit_status_ret_t *get_final_exit_status_1_svc(struct svc_req *rqstp)
{
    static final_exit_status_ret_t result;
    result.value = final_exit_status;
    return &result;
}

void *set_final_exit_status_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1; // 成功标志
    final_exit_status = value;
    return (void *)&result;
}

converted_files_ret_t *get_converted_files_1_svc(struct svc_req *rqstp)
{
    static converted_files_ret_t result;
        if(converted_files==NULL) {
            result.converted_files.hash_table_rpc_ptr_len=0;
            result.converted_files.hash_table_rpc_ptr_val=NULL;
        } else {
            result.converted_files.hash_table_rpc_ptr_len=1;
            result.converted_files.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
            if(result.converted_files.hash_table_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.converted_files.hash_table_rpc_ptr_len;i0++) {
    if (converted_files[i0].cells == NULL) {result.converted_files.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;result.converted_files.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}

    else {result.converted_files.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;result.converted_files.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(result.converted_files.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));

    	if (result.converted_files.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {perror("malloc failed");exit(1);
}
    	for (int i1 = 0; i1 < result.converted_files.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {

    if (converted_files[i0].cells->key == NULL) {result.converted_files.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = 0;result.converted_files.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = NULL;}

    else{result.converted_files.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = sizeof(converted_files[i0].cells->key);result.converted_files.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = malloc(result.converted_files.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (result.converted_files.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.converted_files.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, converted_files[i0].cells->key, result.converted_files.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);}

    if (converted_files[i0].cells->value == NULL) {result.converted_files.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = 0;result.converted_files.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = NULL;}

    else{result.converted_files.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = sizeof(converted_files[i0].cells->value);result.converted_files.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = malloc(result.converted_files.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (result.converted_files.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.converted_files.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, converted_files[i0].cells->value, result.converted_files.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);}

    	}
    }
    result.converted_files.hash_table_rpc_ptr_val[i0].size=converted_files[i0].size;
    result.converted_files.hash_table_rpc_ptr_val[i0].count=converted_files[i0].count;
    result.converted_files.hash_table_rpc_ptr_val[i0].resize_threshold=converted_files[i0].resize_threshold;
    result.converted_files.hash_table_rpc_ptr_val[i0].prime_offset=converted_files[i0].prime_offset;
            }
        }
    return &result;
}

void *set_converted_files_1_svc(hash_table_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct hash_table *local_converted_files=NULL;
    if(arg1.hash_table_rpc_ptr_val!=NULL) {
    local_converted_files=(struct hash_table *)malloc(arg1.hash_table_rpc_ptr_len*sizeof(struct hash_table));
    if(local_converted_files==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.hash_table_rpc_ptr_len;i0++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {local_converted_files[i0].cells=NULL;}
    else {local_converted_files[i0].cells=(struct cell *)malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(struct cell));
    	if (local_converted_files[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {local_converted_files[i0].cells[i1].key=NULL;}

    else {local_converted_files[i0].cells[i1].key=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (local_converted_files[i0].cells[i1].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_converted_files[i0].cells[i1].key,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    }

    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {local_converted_files[i0].cells[i1].value=NULL;}

    else {local_converted_files[i0].cells[i1].value=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (local_converted_files[i0].cells[i1].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_converted_files[i0].cells[i1].value,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    }

    	}
    }
    local_converted_files[i0].size=arg1.hash_table_rpc_ptr_val[i0].size;
    local_converted_files[i0].count=arg1.hash_table_rpc_ptr_val[i0].count;
    local_converted_files[i0].resize_threshold=arg1.hash_table_rpc_ptr_val[i0].resize_threshold;
    local_converted_files[i0].prime_offset=arg1.hash_table_rpc_ptr_val[i0].prime_offset;
    	}
    }
    memcpy(&converted_files, &local_converted_files, sizeof(hash_table));
    return (void *)&result;
}

char_ptr *get_converted_files_hash_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (converted_files.hash_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_converted_files_hash_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        converted_files.hash_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        converted_files.hash_function = fun_ptr;
    }
    return (void *)&result;
}

char_ptr *get_converted_files_test_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (converted_files.test_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_converted_files_test_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        converted_files.test_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        converted_files.test_function = fun_ptr;
    }
    return (void *)&result;
}

converted_files_cells_ret_t *get_converted_files_cells_1_svc(struct svc_req *rqstp)
{
    static converted_files_cells_ret_t result;
        if(converted_files_cells==NULL) {
            result.converted_files_cells.cell_rpc_ptr_len=0;
            result.converted_files_cells.cell_rpc_ptr_val=NULL;
        } else {
            result.converted_files_cells.cell_rpc_ptr_len=1;
            result.converted_files_cells.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
            if(result.converted_files_cells.cell_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.converted_files_cells.cell_rpc_ptr_len;i0++) {
    if (converted_files_cells[i0].key == NULL) {result.converted_files_cells.cell_rpc_ptr_val[i0].key.key_len = 0;result.converted_files_cells.cell_rpc_ptr_val[i0].key.key_val = NULL;}

    else{result.converted_files_cells.cell_rpc_ptr_val[i0].key.key_len = sizeof(converted_files_cells[i0].key);result.converted_files_cells.cell_rpc_ptr_val[i0].key.key_val = malloc(result.converted_files_cells.cell_rpc_ptr_val[i0].key.key_len);

    	if (result.converted_files_cells.cell_rpc_ptr_val[i0].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.converted_files_cells.cell_rpc_ptr_val[i0].key.key_val, converted_files_cells[i0].key, result.converted_files_cells.cell_rpc_ptr_val[i0].key.key_len);}

    if (converted_files_cells[i0].value == NULL) {result.converted_files_cells.cell_rpc_ptr_val[i0].value.value_len = 0;result.converted_files_cells.cell_rpc_ptr_val[i0].value.value_val = NULL;}

    else{result.converted_files_cells.cell_rpc_ptr_val[i0].value.value_len = sizeof(converted_files_cells[i0].value);result.converted_files_cells.cell_rpc_ptr_val[i0].value.value_val = malloc(result.converted_files_cells.cell_rpc_ptr_val[i0].value.value_len);

    	if (result.converted_files_cells.cell_rpc_ptr_val[i0].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.converted_files_cells.cell_rpc_ptr_val[i0].value.value_val, converted_files_cells[i0].value, result.converted_files_cells.cell_rpc_ptr_val[i0].value.value_len);}

            }
        }
    return &result;
}

void *set_converted_files_cells_1_svc(cell_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct cell *local_converted_files_cells=NULL;
    if(arg1.cell_rpc_ptr_val!=NULL) {
    local_converted_files_cells=(struct cell *)malloc(arg1.cell_rpc_ptr_len*sizeof(struct cell));
    if(local_converted_files_cells==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.cell_rpc_ptr_len;i0++) {
    if (arg1.cell_rpc_ptr_val[i0].key.key_val == NULL) {local_converted_files_cells[i0].key=NULL;}

    else {local_converted_files_cells[i0].key=malloc(arg1.cell_rpc_ptr_val[i0].key.key_len);

    	if (local_converted_files_cells[i0].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_converted_files_cells[i0].key,arg1.cell_rpc_ptr_val[i0].key.key_val,arg1.cell_rpc_ptr_val[i0].key.key_len);

    }

    if (arg1.cell_rpc_ptr_val[i0].value.value_val == NULL) {local_converted_files_cells[i0].value=NULL;}

    else {local_converted_files_cells[i0].value=malloc(arg1.cell_rpc_ptr_val[i0].value.value_len);

    	if (local_converted_files_cells[i0].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_converted_files_cells[i0].value,arg1.cell_rpc_ptr_val[i0].value.value_val,arg1.cell_rpc_ptr_val[i0].value.value_len);

    }

    	}
    }
    memcpy(&converted_files_cells, &local_converted_files_cells, sizeof(cell));
    return (void *)&result;
}

converted_files_cells_key_ret_t *get_converted_files_cells_key_1_svc(struct svc_req *rqstp)
{
    static converted_files_cells_key_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 converted_files_cells.key 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t converted_files_cells_key_size; // 假设有这样的辅助变量
    if (converted_files_cells.key == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = converted_files_cells_key_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = converted_files_cells.key;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t converted_files_cells_key_size; // 假设有这样的辅助变量
void *set_converted_files_cells_key_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (converted_files_cells.key != NULL) {
        free(converted_files_cells.key);
        converted_files_cells.key = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        converted_files_cells.key = malloc(value.bytes.bytes_len);
        if (converted_files_cells.key == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(converted_files_cells.key, value.bytes.bytes_val, value.bytes.bytes_len);
            converted_files_cells_key_size = value.size;
        }
    }
    return (void *)&result;
}

converted_files_cells_value_ret_t *get_converted_files_cells_value_1_svc(struct svc_req *rqstp)
{
    static converted_files_cells_value_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 converted_files_cells.value 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t converted_files_cells_value_size; // 假设有这样的辅助变量
    if (converted_files_cells.value == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = converted_files_cells_value_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = converted_files_cells.value;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t converted_files_cells_value_size; // 假设有这样的辅助变量
void *set_converted_files_cells_value_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (converted_files_cells.value != NULL) {
        free(converted_files_cells.value);
        converted_files_cells.value = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        converted_files_cells.value = malloc(value.bytes.bytes_len);
        if (converted_files_cells.value == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(converted_files_cells.value, value.bytes.bytes_val, value.bytes.bytes_len);
            converted_files_cells_value_size = value.size;
        }
    }
    return (void *)&result;
}

converted_files_size_ret_t *get_converted_files_size_1_svc(struct svc_req *rqstp)
{
    static converted_files_size_ret_t result;
    result.value = converted_files.size;
    return &result;
}

void *set_converted_files_size_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    converted_files.size = value;
    return (void *)&result;
}

converted_files_count_ret_t *get_converted_files_count_1_svc(struct svc_req *rqstp)
{
    static converted_files_count_ret_t result;
    result.value = converted_files.count;
    return &result;
}

void *set_converted_files_count_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    converted_files.count = value;
    return (void *)&result;
}

converted_files_resize_threshold_ret_t *get_converted_files_resize_threshold_1_svc(struct svc_req *rqstp)
{
    static converted_files_resize_threshold_ret_t result;
    result.value = converted_files.resize_threshold;
    return &result;
}

void *set_converted_files_resize_threshold_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    converted_files.resize_threshold = value;
    return (void *)&result;
}

converted_files_prime_offset_ret_t *get_converted_files_prime_offset_1_svc(struct svc_req *rqstp)
{
    static converted_files_prime_offset_ret_t result;
    result.value = converted_files.prime_offset;
    return &result;
}

void *set_converted_files_prime_offset_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    converted_files.prime_offset = value;
    return (void *)&result;
}

log_line_current_ret_t *get_log_line_current_1_svc(struct svc_req *rqstp)
{
    static log_line_current_ret_t result;
    result.value = log_line_current;
    return &result;
}

void *set_log_line_current_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1; // 成功标志
    log_line_current = value;
    return (void *)&result;
}

opt_ret_t *get_opt_1_svc(struct svc_req *rqstp)
{
    static opt_ret_t result;
    return &result;
}

void *set_opt_1_svc(options_rpc value, struct svc_req *rqstp)
{
    static int result = 1;
    options_rpc arg1
    return (void *)&result;
}

opt_verbose_ret_t *get_opt_verbose_1_svc(struct svc_req *rqstp)
{
    static opt_verbose_ret_t result;
    result.value = opt.verbose;
    return &result;
}

void *set_opt_verbose_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.verbose = value;
    return (void *)&result;
}

opt_quiet_ret_t *get_opt_quiet_1_svc(struct svc_req *rqstp)
{
    static opt_quiet_ret_t result;
    result.value = opt.quiet;
    return &result;
}

void *set_opt_quiet_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.quiet = value;
    return (void *)&result;
}

opt_ntry_ret_t *get_opt_ntry_1_svc(struct svc_req *rqstp)
{
    static opt_ntry_ret_t result;
    result.value = opt.ntry;
    return &result;
}

void *set_opt_ntry_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.ntry = value;
    return (void *)&result;
}

opt_retry_connrefused_ret_t *get_opt_retry_connrefused_1_svc(struct svc_req *rqstp)
{
    static opt_retry_connrefused_ret_t result;
    result.value = opt.retry_connrefused;
    return &result;
}

void *set_opt_retry_connrefused_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.retry_connrefused = value;
    return (void *)&result;
}

opt_background_ret_t *get_opt_background_1_svc(struct svc_req *rqstp)
{
    static opt_background_ret_t result;
    result.value = opt.background;
    return &result;
}

void *set_opt_background_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.background = value;
    return (void *)&result;
}

opt_ignore_length_ret_t *get_opt_ignore_length_1_svc(struct svc_req *rqstp)
{
    static opt_ignore_length_ret_t result;
    result.value = opt.ignore_length;
    return &result;
}

void *set_opt_ignore_length_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.ignore_length = value;
    return (void *)&result;
}

opt_recursive_ret_t *get_opt_recursive_1_svc(struct svc_req *rqstp)
{
    static opt_recursive_ret_t result;
    result.value = opt.recursive;
    return &result;
}

void *set_opt_recursive_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.recursive = value;
    return (void *)&result;
}

opt_spanhost_ret_t *get_opt_spanhost_1_svc(struct svc_req *rqstp)
{
    static opt_spanhost_ret_t result;
    result.value = opt.spanhost;
    return &result;
}

void *set_opt_spanhost_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.spanhost = value;
    return (void *)&result;
}

opt_max_redirect_ret_t *get_opt_max_redirect_1_svc(struct svc_req *rqstp)
{
    static opt_max_redirect_ret_t result;
    result.value = opt.max_redirect;
    return &result;
}

void *set_opt_max_redirect_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.max_redirect = value;
    return (void *)&result;
}

opt_relative_only_ret_t *get_opt_relative_only_1_svc(struct svc_req *rqstp)
{
    static opt_relative_only_ret_t result;
    result.value = opt.relative_only;
    return &result;
}

void *set_opt_relative_only_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.relative_only = value;
    return (void *)&result;
}

opt_no_parent_ret_t *get_opt_no_parent_1_svc(struct svc_req *rqstp)
{
    static opt_no_parent_ret_t result;
    result.value = opt.no_parent;
    return &result;
}

void *set_opt_no_parent_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.no_parent = value;
    return (void *)&result;
}

opt_reclevel_ret_t *get_opt_reclevel_1_svc(struct svc_req *rqstp)
{
    static opt_reclevel_ret_t result;
    result.value = opt.reclevel;
    return &result;
}

void *set_opt_reclevel_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.reclevel = value;
    return (void *)&result;
}

opt_dirstruct_ret_t *get_opt_dirstruct_1_svc(struct svc_req *rqstp)
{
    static opt_dirstruct_ret_t result;
    result.value = opt.dirstruct;
    return &result;
}

void *set_opt_dirstruct_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.dirstruct = value;
    return (void *)&result;
}

opt_no_dirstruct_ret_t *get_opt_no_dirstruct_1_svc(struct svc_req *rqstp)
{
    static opt_no_dirstruct_ret_t result;
    result.value = opt.no_dirstruct;
    return &result;
}

void *set_opt_no_dirstruct_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.no_dirstruct = value;
    return (void *)&result;
}

opt_cut_dirs_ret_t *get_opt_cut_dirs_1_svc(struct svc_req *rqstp)
{
    static opt_cut_dirs_ret_t result;
    result.value = opt.cut_dirs;
    return &result;
}

void *set_opt_cut_dirs_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.cut_dirs = value;
    return (void *)&result;
}

opt_add_hostdir_ret_t *get_opt_add_hostdir_1_svc(struct svc_req *rqstp)
{
    static opt_add_hostdir_ret_t result;
    result.value = opt.add_hostdir;
    return &result;
}

void *set_opt_add_hostdir_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.add_hostdir = value;
    return (void *)&result;
}

opt_protocol_directories_ret_t *get_opt_protocol_directories_1_svc(struct svc_req *rqstp)
{
    static opt_protocol_directories_ret_t result;
    result.value = opt.protocol_directories;
    return &result;
}

void *set_opt_protocol_directories_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.protocol_directories = value;
    return (void *)&result;
}

opt_noclobber_ret_t *get_opt_noclobber_1_svc(struct svc_req *rqstp)
{
    static opt_noclobber_ret_t result;
    result.value = opt.noclobber;
    return &result;
}

void *set_opt_noclobber_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.noclobber = value;
    return (void *)&result;
}

opt_unlink_ret_t *get_opt_unlink_1_svc(struct svc_req *rqstp)
{
    static opt_unlink_ret_t result;
    result.value = opt.unlink;
    return &result;
}

void *set_opt_unlink_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.unlink = value;
    return (void *)&result;
}

opt_dir_prefix_ret_t *get_opt_dir_prefix_1_svc(struct svc_req *rqstp)
{
    static opt_dir_prefix_ret_t result;
    if (opt.dir_prefix != NULL) {
        result.value = strdup(opt.dir_prefix);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_dir_prefix_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.dir_prefix != NULL) {
        free(opt.dir_prefix);
    }
    if (value != NULL) {
        opt.dir_prefix = strdup(value);
    } else {
        opt.dir_prefix = NULL;
    }
    return (void *)&result;
}

opt_lfilename_ret_t *get_opt_lfilename_1_svc(struct svc_req *rqstp)
{
    static opt_lfilename_ret_t result;
    if (opt.lfilename != NULL) {
        result.value = strdup(opt.lfilename);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_lfilename_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.lfilename != NULL) {
        free(opt.lfilename);
    }
    if (value != NULL) {
        opt.lfilename = strdup(value);
    } else {
        opt.lfilename = NULL;
    }
    return (void *)&result;
}

opt_input_filename_ret_t *get_opt_input_filename_1_svc(struct svc_req *rqstp)
{
    static opt_input_filename_ret_t result;
    if (opt.input_filename != NULL) {
        result.value = strdup(opt.input_filename);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_input_filename_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.input_filename != NULL) {
        free(opt.input_filename);
    }
    if (value != NULL) {
        opt.input_filename = strdup(value);
    } else {
        opt.input_filename = NULL;
    }
    return (void *)&result;
}

opt_choose_config_ret_t *get_opt_choose_config_1_svc(struct svc_req *rqstp)
{
    static opt_choose_config_ret_t result;
    if (opt.choose_config != NULL) {
        result.value = strdup(opt.choose_config);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_choose_config_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.choose_config != NULL) {
        free(opt.choose_config);
    }
    if (value != NULL) {
        opt.choose_config = strdup(value);
    } else {
        opt.choose_config = NULL;
    }
    return (void *)&result;
}

opt_noconfig_ret_t *get_opt_noconfig_1_svc(struct svc_req *rqstp)
{
    static opt_noconfig_ret_t result;
    result.value = opt.noconfig;
    return &result;
}

void *set_opt_noconfig_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.noconfig = value;
    return (void *)&result;
}

opt_force_html_ret_t *get_opt_force_html_1_svc(struct svc_req *rqstp)
{
    static opt_force_html_ret_t result;
    result.value = opt.force_html;
    return &result;
}

void *set_opt_force_html_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.force_html = value;
    return (void *)&result;
}

opt_default_page_ret_t *get_opt_default_page_1_svc(struct svc_req *rqstp)
{
    static opt_default_page_ret_t result;
    if (opt.default_page != NULL) {
        result.value = strdup(opt.default_page);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_default_page_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.default_page != NULL) {
        free(opt.default_page);
    }
    if (value != NULL) {
        opt.default_page = strdup(value);
    } else {
        opt.default_page = NULL;
    }
    return (void *)&result;
}

opt_spider_ret_t *get_opt_spider_1_svc(struct svc_req *rqstp)
{
    static opt_spider_ret_t result;
    result.value = opt.spider;
    return &result;
}

void *set_opt_spider_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.spider = value;
    return (void *)&result;
}

opt_accepts_ret_t *get_opt_accepts_1_svc(struct svc_req *rqstp)
{
    static opt_accepts_ret_t result;
    if (opt.accepts != NULL) {
        result.value = strdup(opt.accepts);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_accepts_1_svc(TwoCharPtr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.accepts != NULL) {
        free(opt.accepts);
    }
    if (value != NULL) {
        opt.accepts = strdup(value);
    } else {
        opt.accepts = NULL;
    }
    return (void *)&result;
}

opt_rejects_ret_t *get_opt_rejects_1_svc(struct svc_req *rqstp)
{
    static opt_rejects_ret_t result;
    if (opt.rejects != NULL) {
        result.value = strdup(opt.rejects);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_rejects_1_svc(TwoCharPtr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.rejects != NULL) {
        free(opt.rejects);
    }
    if (value != NULL) {
        opt.rejects = strdup(value);
    } else {
        opt.rejects = NULL;
    }
    return (void *)&result;
}

opt_excludes_ret_t *get_opt_excludes_1_svc(struct svc_req *rqstp)
{
    static opt_excludes_ret_t result;
    if (opt.excludes != NULL) {
        result.value = strdup(opt.excludes);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_excludes_1_svc(TwoCharPtr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.excludes != NULL) {
        free(opt.excludes);
    }
    if (value != NULL) {
        opt.excludes = strdup(value);
    } else {
        opt.excludes = NULL;
    }
    return (void *)&result;
}

opt_includes_ret_t *get_opt_includes_1_svc(struct svc_req *rqstp)
{
    static opt_includes_ret_t result;
    if (opt.includes != NULL) {
        result.value = strdup(opt.includes);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_includes_1_svc(TwoCharPtr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.includes != NULL) {
        free(opt.includes);
    }
    if (value != NULL) {
        opt.includes = strdup(value);
    } else {
        opt.includes = NULL;
    }
    return (void *)&result;
}

opt_ignore_case_ret_t *get_opt_ignore_case_1_svc(struct svc_req *rqstp)
{
    static opt_ignore_case_ret_t result;
    result.value = opt.ignore_case;
    return &result;
}

void *set_opt_ignore_case_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.ignore_case = value;
    return (void *)&result;
}

opt_acceptregex_s_ret_t *get_opt_acceptregex_s_1_svc(struct svc_req *rqstp)
{
    static opt_acceptregex_s_ret_t result;
    if (opt.acceptregex_s != NULL) {
        result.value = strdup(opt.acceptregex_s);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_acceptregex_s_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.acceptregex_s != NULL) {
        free(opt.acceptregex_s);
    }
    if (value != NULL) {
        opt.acceptregex_s = strdup(value);
    } else {
        opt.acceptregex_s = NULL;
    }
    return (void *)&result;
}

opt_rejectregex_s_ret_t *get_opt_rejectregex_s_1_svc(struct svc_req *rqstp)
{
    static opt_rejectregex_s_ret_t result;
    if (opt.rejectregex_s != NULL) {
        result.value = strdup(opt.rejectregex_s);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_rejectregex_s_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.rejectregex_s != NULL) {
        free(opt.rejectregex_s);
    }
    if (value != NULL) {
        opt.rejectregex_s = strdup(value);
    } else {
        opt.rejectregex_s = NULL;
    }
    return (void *)&result;
}

opt_acceptregex_ret_t *get_opt_acceptregex_1_svc(struct svc_req *rqstp)
{
    static opt_acceptregex_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 opt.acceptregex 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t opt_acceptregex_size; // 假设有这样的辅助变量
    if (opt.acceptregex == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = opt_acceptregex_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = opt.acceptregex;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t opt_acceptregex_size; // 假设有这样的辅助变量
void *set_opt_acceptregex_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (opt.acceptregex != NULL) {
        free(opt.acceptregex);
        opt.acceptregex = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        opt.acceptregex = malloc(value.bytes.bytes_len);
        if (opt.acceptregex == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(opt.acceptregex, value.bytes.bytes_val, value.bytes.bytes_len);
            opt_acceptregex_size = value.size;
        }
    }
    return (void *)&result;
}

opt_rejectregex_ret_t *get_opt_rejectregex_1_svc(struct svc_req *rqstp)
{
    static opt_rejectregex_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 opt.rejectregex 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t opt_rejectregex_size; // 假设有这样的辅助变量
    if (opt.rejectregex == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = opt_rejectregex_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = opt.rejectregex;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t opt_rejectregex_size; // 假设有这样的辅助变量
void *set_opt_rejectregex_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (opt.rejectregex != NULL) {
        free(opt.rejectregex);
        opt.rejectregex = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        opt.rejectregex = malloc(value.bytes.bytes_len);
        if (opt.rejectregex == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(opt.rejectregex, value.bytes.bytes_val, value.bytes.bytes_len);
            opt_rejectregex_size = value.size;
        }
    }
    return (void *)&result;
}

opt_regex_type_ret_t *get_opt_regex_type_1_svc(struct svc_req *rqstp)
{
    static opt_regex_type_ret_t result;
    result.value = opt.regex_type;
    return &result;
}

void *set_opt_regex_type_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.regex_type = value;
    return (void *)&result;
}

char_ptr *get_opt_regex_compile_fun_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (opt.regex_compile_fun);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_opt_regex_compile_fun_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        opt.regex_compile_fun = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        opt.regex_compile_fun = fun_ptr;
    }
    return (void *)&result;
}

char_ptr *get_opt_regex_match_fun_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (opt.regex_match_fun);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_opt_regex_match_fun_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        opt.regex_match_fun = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        opt.regex_match_fun = fun_ptr;
    }
    return (void *)&result;
}

opt_domains_ret_t *get_opt_domains_1_svc(struct svc_req *rqstp)
{
    static opt_domains_ret_t result;
    if (opt.domains != NULL) {
        result.value = strdup(opt.domains);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_domains_1_svc(TwoCharPtr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.domains != NULL) {
        free(opt.domains);
    }
    if (value != NULL) {
        opt.domains = strdup(value);
    } else {
        opt.domains = NULL;
    }
    return (void *)&result;
}

opt_exclude_domains_ret_t *get_opt_exclude_domains_1_svc(struct svc_req *rqstp)
{
    static opt_exclude_domains_ret_t result;
    if (opt.exclude_domains != NULL) {
        result.value = strdup(opt.exclude_domains);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_exclude_domains_1_svc(TwoCharPtr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.exclude_domains != NULL) {
        free(opt.exclude_domains);
    }
    if (value != NULL) {
        opt.exclude_domains = strdup(value);
    } else {
        opt.exclude_domains = NULL;
    }
    return (void *)&result;
}

opt_dns_cache_ret_t *get_opt_dns_cache_1_svc(struct svc_req *rqstp)
{
    static opt_dns_cache_ret_t result;
    result.value = opt.dns_cache;
    return &result;
}

void *set_opt_dns_cache_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.dns_cache = value;
    return (void *)&result;
}

opt_follow_tags_ret_t *get_opt_follow_tags_1_svc(struct svc_req *rqstp)
{
    static opt_follow_tags_ret_t result;
    if (opt.follow_tags != NULL) {
        result.value = strdup(opt.follow_tags);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_follow_tags_1_svc(TwoCharPtr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.follow_tags != NULL) {
        free(opt.follow_tags);
    }
    if (value != NULL) {
        opt.follow_tags = strdup(value);
    } else {
        opt.follow_tags = NULL;
    }
    return (void *)&result;
}

opt_ignore_tags_ret_t *get_opt_ignore_tags_1_svc(struct svc_req *rqstp)
{
    static opt_ignore_tags_ret_t result;
    if (opt.ignore_tags != NULL) {
        result.value = strdup(opt.ignore_tags);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_ignore_tags_1_svc(TwoCharPtr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.ignore_tags != NULL) {
        free(opt.ignore_tags);
    }
    if (value != NULL) {
        opt.ignore_tags = strdup(value);
    } else {
        opt.ignore_tags = NULL;
    }
    return (void *)&result;
}

opt_follow_ftp_ret_t *get_opt_follow_ftp_1_svc(struct svc_req *rqstp)
{
    static opt_follow_ftp_ret_t result;
    result.value = opt.follow_ftp;
    return &result;
}

void *set_opt_follow_ftp_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.follow_ftp = value;
    return (void *)&result;
}

opt_retr_symlinks_ret_t *get_opt_retr_symlinks_1_svc(struct svc_req *rqstp)
{
    static opt_retr_symlinks_ret_t result;
    result.value = opt.retr_symlinks;
    return &result;
}

void *set_opt_retr_symlinks_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.retr_symlinks = value;
    return (void *)&result;
}

opt_output_document_ret_t *get_opt_output_document_1_svc(struct svc_req *rqstp)
{
    static opt_output_document_ret_t result;
    if (opt.output_document != NULL) {
        result.value = strdup(opt.output_document);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_output_document_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.output_document != NULL) {
        free(opt.output_document);
    }
    if (value != NULL) {
        opt.output_document = strdup(value);
    } else {
        opt.output_document = NULL;
    }
    return (void *)&result;
}

opt_warc_filename_ret_t *get_opt_warc_filename_1_svc(struct svc_req *rqstp)
{
    static opt_warc_filename_ret_t result;
    if (opt.warc_filename != NULL) {
        result.value = strdup(opt.warc_filename);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_warc_filename_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.warc_filename != NULL) {
        free(opt.warc_filename);
    }
    if (value != NULL) {
        opt.warc_filename = strdup(value);
    } else {
        opt.warc_filename = NULL;
    }
    return (void *)&result;
}

opt_warc_tempdir_ret_t *get_opt_warc_tempdir_1_svc(struct svc_req *rqstp)
{
    static opt_warc_tempdir_ret_t result;
    if (opt.warc_tempdir != NULL) {
        result.value = strdup(opt.warc_tempdir);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_warc_tempdir_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.warc_tempdir != NULL) {
        free(opt.warc_tempdir);
    }
    if (value != NULL) {
        opt.warc_tempdir = strdup(value);
    } else {
        opt.warc_tempdir = NULL;
    }
    return (void *)&result;
}

opt_warc_cdx_dedup_filename_ret_t *get_opt_warc_cdx_dedup_filename_1_svc(struct svc_req *rqstp)
{
    static opt_warc_cdx_dedup_filename_ret_t result;
    if (opt.warc_cdx_dedup_filename != NULL) {
        result.value = strdup(opt.warc_cdx_dedup_filename);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_warc_cdx_dedup_filename_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.warc_cdx_dedup_filename != NULL) {
        free(opt.warc_cdx_dedup_filename);
    }
    if (value != NULL) {
        opt.warc_cdx_dedup_filename = strdup(value);
    } else {
        opt.warc_cdx_dedup_filename = NULL;
    }
    return (void *)&result;
}

opt_warc_maxsize_ret_t *get_opt_warc_maxsize_1_svc(struct svc_req *rqstp)
{
    static opt_warc_maxsize_ret_t result;
    result.value = opt.warc_maxsize;
    return &result;
}

void *set_opt_warc_maxsize_1_svc(wgint value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.warc_maxsize = value;
    return (void *)&result;
}

opt_warc_compression_enabled_ret_t *get_opt_warc_compression_enabled_1_svc(struct svc_req *rqstp)
{
    static opt_warc_compression_enabled_ret_t result;
    result.value = opt.warc_compression_enabled;
    return &result;
}

void *set_opt_warc_compression_enabled_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.warc_compression_enabled = value;
    return (void *)&result;
}

opt_warc_digests_enabled_ret_t *get_opt_warc_digests_enabled_1_svc(struct svc_req *rqstp)
{
    static opt_warc_digests_enabled_ret_t result;
    result.value = opt.warc_digests_enabled;
    return &result;
}

void *set_opt_warc_digests_enabled_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.warc_digests_enabled = value;
    return (void *)&result;
}

opt_warc_cdx_enabled_ret_t *get_opt_warc_cdx_enabled_1_svc(struct svc_req *rqstp)
{
    static opt_warc_cdx_enabled_ret_t result;
    result.value = opt.warc_cdx_enabled;
    return &result;
}

void *set_opt_warc_cdx_enabled_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.warc_cdx_enabled = value;
    return (void *)&result;
}

opt_warc_keep_log_ret_t *get_opt_warc_keep_log_1_svc(struct svc_req *rqstp)
{
    static opt_warc_keep_log_ret_t result;
    result.value = opt.warc_keep_log;
    return &result;
}

void *set_opt_warc_keep_log_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.warc_keep_log = value;
    return (void *)&result;
}

opt_warc_user_headers_ret_t *get_opt_warc_user_headers_1_svc(struct svc_req *rqstp)
{
    static opt_warc_user_headers_ret_t result;
    if (opt.warc_user_headers != NULL) {
        result.value = strdup(opt.warc_user_headers);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_warc_user_headers_1_svc(TwoCharPtr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.warc_user_headers != NULL) {
        free(opt.warc_user_headers);
    }
    if (value != NULL) {
        opt.warc_user_headers = strdup(value);
    } else {
        opt.warc_user_headers = NULL;
    }
    return (void *)&result;
}

opt_user_ret_t *get_opt_user_1_svc(struct svc_req *rqstp)
{
    static opt_user_ret_t result;
    if (opt.user != NULL) {
        result.value = strdup(opt.user);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_user_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.user != NULL) {
        free(opt.user);
    }
    if (value != NULL) {
        opt.user = strdup(value);
    } else {
        opt.user = NULL;
    }
    return (void *)&result;
}

opt_passwd_ret_t *get_opt_passwd_1_svc(struct svc_req *rqstp)
{
    static opt_passwd_ret_t result;
    if (opt.passwd != NULL) {
        result.value = strdup(opt.passwd);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_passwd_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.passwd != NULL) {
        free(opt.passwd);
    }
    if (value != NULL) {
        opt.passwd = strdup(value);
    } else {
        opt.passwd = NULL;
    }
    return (void *)&result;
}

opt_ask_passwd_ret_t *get_opt_ask_passwd_1_svc(struct svc_req *rqstp)
{
    static opt_ask_passwd_ret_t result;
    result.value = opt.ask_passwd;
    return &result;
}

void *set_opt_ask_passwd_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.ask_passwd = value;
    return (void *)&result;
}

opt_always_rest_ret_t *get_opt_always_rest_1_svc(struct svc_req *rqstp)
{
    static opt_always_rest_ret_t result;
    result.value = opt.always_rest;
    return &result;
}

void *set_opt_always_rest_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.always_rest = value;
    return (void *)&result;
}

opt_start_pos_ret_t *get_opt_start_pos_1_svc(struct svc_req *rqstp)
{
    static opt_start_pos_ret_t result;
    result.value = opt.start_pos;
    return &result;
}

void *set_opt_start_pos_1_svc(wgint value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.start_pos = value;
    return (void *)&result;
}

opt_ftp_user_ret_t *get_opt_ftp_user_1_svc(struct svc_req *rqstp)
{
    static opt_ftp_user_ret_t result;
    if (opt.ftp_user != NULL) {
        result.value = strdup(opt.ftp_user);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_ftp_user_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.ftp_user != NULL) {
        free(opt.ftp_user);
    }
    if (value != NULL) {
        opt.ftp_user = strdup(value);
    } else {
        opt.ftp_user = NULL;
    }
    return (void *)&result;
}

opt_ftp_passwd_ret_t *get_opt_ftp_passwd_1_svc(struct svc_req *rqstp)
{
    static opt_ftp_passwd_ret_t result;
    if (opt.ftp_passwd != NULL) {
        result.value = strdup(opt.ftp_passwd);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_ftp_passwd_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.ftp_passwd != NULL) {
        free(opt.ftp_passwd);
    }
    if (value != NULL) {
        opt.ftp_passwd = strdup(value);
    } else {
        opt.ftp_passwd = NULL;
    }
    return (void *)&result;
}

opt_netrc_ret_t *get_opt_netrc_1_svc(struct svc_req *rqstp)
{
    static opt_netrc_ret_t result;
    result.value = opt.netrc;
    return &result;
}

void *set_opt_netrc_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.netrc = value;
    return (void *)&result;
}

opt_ftp_glob_ret_t *get_opt_ftp_glob_1_svc(struct svc_req *rqstp)
{
    static opt_ftp_glob_ret_t result;
    result.value = opt.ftp_glob;
    return &result;
}

void *set_opt_ftp_glob_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.ftp_glob = value;
    return (void *)&result;
}

opt_ftp_pasv_ret_t *get_opt_ftp_pasv_1_svc(struct svc_req *rqstp)
{
    static opt_ftp_pasv_ret_t result;
    result.value = opt.ftp_pasv;
    return &result;
}

void *set_opt_ftp_pasv_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.ftp_pasv = value;
    return (void *)&result;
}

opt_http_user_ret_t *get_opt_http_user_1_svc(struct svc_req *rqstp)
{
    static opt_http_user_ret_t result;
    if (opt.http_user != NULL) {
        result.value = strdup(opt.http_user);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_http_user_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.http_user != NULL) {
        free(opt.http_user);
    }
    if (value != NULL) {
        opt.http_user = strdup(value);
    } else {
        opt.http_user = NULL;
    }
    return (void *)&result;
}

opt_http_passwd_ret_t *get_opt_http_passwd_1_svc(struct svc_req *rqstp)
{
    static opt_http_passwd_ret_t result;
    if (opt.http_passwd != NULL) {
        result.value = strdup(opt.http_passwd);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_http_passwd_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.http_passwd != NULL) {
        free(opt.http_passwd);
    }
    if (value != NULL) {
        opt.http_passwd = strdup(value);
    } else {
        opt.http_passwd = NULL;
    }
    return (void *)&result;
}

opt_user_headers_ret_t *get_opt_user_headers_1_svc(struct svc_req *rqstp)
{
    static opt_user_headers_ret_t result;
    if (opt.user_headers != NULL) {
        result.value = strdup(opt.user_headers);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_user_headers_1_svc(TwoCharPtr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.user_headers != NULL) {
        free(opt.user_headers);
    }
    if (value != NULL) {
        opt.user_headers = strdup(value);
    } else {
        opt.user_headers = NULL;
    }
    return (void *)&result;
}

opt_http_keep_alive_ret_t *get_opt_http_keep_alive_1_svc(struct svc_req *rqstp)
{
    static opt_http_keep_alive_ret_t result;
    result.value = opt.http_keep_alive;
    return &result;
}

void *set_opt_http_keep_alive_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.http_keep_alive = value;
    return (void *)&result;
}

opt_use_proxy_ret_t *get_opt_use_proxy_1_svc(struct svc_req *rqstp)
{
    static opt_use_proxy_ret_t result;
    result.value = opt.use_proxy;
    return &result;
}

void *set_opt_use_proxy_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.use_proxy = value;
    return (void *)&result;
}

opt_allow_cache_ret_t *get_opt_allow_cache_1_svc(struct svc_req *rqstp)
{
    static opt_allow_cache_ret_t result;
    result.value = opt.allow_cache;
    return &result;
}

void *set_opt_allow_cache_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.allow_cache = value;
    return (void *)&result;
}

opt_http_proxy_ret_t *get_opt_http_proxy_1_svc(struct svc_req *rqstp)
{
    static opt_http_proxy_ret_t result;
    if (opt.http_proxy != NULL) {
        result.value = strdup(opt.http_proxy);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_http_proxy_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.http_proxy != NULL) {
        free(opt.http_proxy);
    }
    if (value != NULL) {
        opt.http_proxy = strdup(value);
    } else {
        opt.http_proxy = NULL;
    }
    return (void *)&result;
}

opt_ftp_proxy_ret_t *get_opt_ftp_proxy_1_svc(struct svc_req *rqstp)
{
    static opt_ftp_proxy_ret_t result;
    if (opt.ftp_proxy != NULL) {
        result.value = strdup(opt.ftp_proxy);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_ftp_proxy_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.ftp_proxy != NULL) {
        free(opt.ftp_proxy);
    }
    if (value != NULL) {
        opt.ftp_proxy = strdup(value);
    } else {
        opt.ftp_proxy = NULL;
    }
    return (void *)&result;
}

opt_https_proxy_ret_t *get_opt_https_proxy_1_svc(struct svc_req *rqstp)
{
    static opt_https_proxy_ret_t result;
    if (opt.https_proxy != NULL) {
        result.value = strdup(opt.https_proxy);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_https_proxy_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.https_proxy != NULL) {
        free(opt.https_proxy);
    }
    if (value != NULL) {
        opt.https_proxy = strdup(value);
    } else {
        opt.https_proxy = NULL;
    }
    return (void *)&result;
}

opt_no_proxy_ret_t *get_opt_no_proxy_1_svc(struct svc_req *rqstp)
{
    static opt_no_proxy_ret_t result;
    if (opt.no_proxy != NULL) {
        result.value = strdup(opt.no_proxy);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_no_proxy_1_svc(TwoCharPtr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.no_proxy != NULL) {
        free(opt.no_proxy);
    }
    if (value != NULL) {
        opt.no_proxy = strdup(value);
    } else {
        opt.no_proxy = NULL;
    }
    return (void *)&result;
}

opt_base_href_ret_t *get_opt_base_href_1_svc(struct svc_req *rqstp)
{
    static opt_base_href_ret_t result;
    if (opt.base_href != NULL) {
        result.value = strdup(opt.base_href);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_base_href_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.base_href != NULL) {
        free(opt.base_href);
    }
    if (value != NULL) {
        opt.base_href = strdup(value);
    } else {
        opt.base_href = NULL;
    }
    return (void *)&result;
}

opt_progress_type_ret_t *get_opt_progress_type_1_svc(struct svc_req *rqstp)
{
    static opt_progress_type_ret_t result;
    if (opt.progress_type != NULL) {
        result.value = strdup(opt.progress_type);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_progress_type_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.progress_type != NULL) {
        free(opt.progress_type);
    }
    if (value != NULL) {
        opt.progress_type = strdup(value);
    } else {
        opt.progress_type = NULL;
    }
    return (void *)&result;
}

opt_show_progress_ret_t *get_opt_show_progress_1_svc(struct svc_req *rqstp)
{
    static opt_show_progress_ret_t result;
    result.value = opt.show_progress;
    return &result;
}

void *set_opt_show_progress_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.show_progress = value;
    return (void *)&result;
}

opt_noscroll_ret_t *get_opt_noscroll_1_svc(struct svc_req *rqstp)
{
    static opt_noscroll_ret_t result;
    result.value = opt.noscroll;
    return &result;
}

void *set_opt_noscroll_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.noscroll = value;
    return (void *)&result;
}

opt_proxy_user_ret_t *get_opt_proxy_user_1_svc(struct svc_req *rqstp)
{
    static opt_proxy_user_ret_t result;
    if (opt.proxy_user != NULL) {
        result.value = strdup(opt.proxy_user);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_proxy_user_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.proxy_user != NULL) {
        free(opt.proxy_user);
    }
    if (value != NULL) {
        opt.proxy_user = strdup(value);
    } else {
        opt.proxy_user = NULL;
    }
    return (void *)&result;
}

opt_proxy_passwd_ret_t *get_opt_proxy_passwd_1_svc(struct svc_req *rqstp)
{
    static opt_proxy_passwd_ret_t result;
    if (opt.proxy_passwd != NULL) {
        result.value = strdup(opt.proxy_passwd);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_proxy_passwd_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.proxy_passwd != NULL) {
        free(opt.proxy_passwd);
    }
    if (value != NULL) {
        opt.proxy_passwd = strdup(value);
    } else {
        opt.proxy_passwd = NULL;
    }
    return (void *)&result;
}

opt_read_timeout_ret_t *get_opt_read_timeout_1_svc(struct svc_req *rqstp)
{
    static opt_read_timeout_ret_t result;
    result.value = opt.read_timeout;
    return &result;
}

void *set_opt_read_timeout_1_svc(double value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.read_timeout = value;
    return (void *)&result;
}

opt_dns_timeout_ret_t *get_opt_dns_timeout_1_svc(struct svc_req *rqstp)
{
    static opt_dns_timeout_ret_t result;
    result.value = opt.dns_timeout;
    return &result;
}

void *set_opt_dns_timeout_1_svc(double value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.dns_timeout = value;
    return (void *)&result;
}

opt_connect_timeout_ret_t *get_opt_connect_timeout_1_svc(struct svc_req *rqstp)
{
    static opt_connect_timeout_ret_t result;
    result.value = opt.connect_timeout;
    return &result;
}

void *set_opt_connect_timeout_1_svc(double value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.connect_timeout = value;
    return (void *)&result;
}

opt_random_wait_ret_t *get_opt_random_wait_1_svc(struct svc_req *rqstp)
{
    static opt_random_wait_ret_t result;
    result.value = opt.random_wait;
    return &result;
}

void *set_opt_random_wait_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.random_wait = value;
    return (void *)&result;
}

opt_wait_ret_t *get_opt_wait_1_svc(struct svc_req *rqstp)
{
    static opt_wait_ret_t result;
    result.value = opt.wait;
    return &result;
}

void *set_opt_wait_1_svc(double value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.wait = value;
    return (void *)&result;
}

opt_waitretry_ret_t *get_opt_waitretry_1_svc(struct svc_req *rqstp)
{
    static opt_waitretry_ret_t result;
    result.value = opt.waitretry;
    return &result;
}

void *set_opt_waitretry_1_svc(double value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.waitretry = value;
    return (void *)&result;
}

opt_use_robots_ret_t *get_opt_use_robots_1_svc(struct svc_req *rqstp)
{
    static opt_use_robots_ret_t result;
    result.value = opt.use_robots;
    return &result;
}

void *set_opt_use_robots_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.use_robots = value;
    return (void *)&result;
}

opt_limit_rate_ret_t *get_opt_limit_rate_1_svc(struct svc_req *rqstp)
{
    static opt_limit_rate_ret_t result;
    result.value = opt.limit_rate;
    return &result;
}

void *set_opt_limit_rate_1_svc(wgint value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.limit_rate = value;
    return (void *)&result;
}

opt_quota_ret_t *get_opt_quota_1_svc(struct svc_req *rqstp)
{
    static opt_quota_ret_t result;
    result.value = opt.quota;
    return &result;
}

void *set_opt_quota_1_svc(SUM_SIZE_INT value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.quota = value;
    return (void *)&result;
}

opt_server_response_ret_t *get_opt_server_response_1_svc(struct svc_req *rqstp)
{
    static opt_server_response_ret_t result;
    result.value = opt.server_response;
    return &result;
}

void *set_opt_server_response_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.server_response = value;
    return (void *)&result;
}

opt_save_headers_ret_t *get_opt_save_headers_1_svc(struct svc_req *rqstp)
{
    static opt_save_headers_ret_t result;
    result.value = opt.save_headers;
    return &result;
}

void *set_opt_save_headers_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.save_headers = value;
    return (void *)&result;
}

opt_content_on_error_ret_t *get_opt_content_on_error_1_svc(struct svc_req *rqstp)
{
    static opt_content_on_error_ret_t result;
    result.value = opt.content_on_error;
    return &result;
}

void *set_opt_content_on_error_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.content_on_error = value;
    return (void *)&result;
}

opt_debug_ret_t *get_opt_debug_1_svc(struct svc_req *rqstp)
{
    static opt_debug_ret_t result;
    result.value = opt.debug;
    return &result;
}

void *set_opt_debug_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.debug = value;
    return (void *)&result;
}

opt_timestamping_ret_t *get_opt_timestamping_1_svc(struct svc_req *rqstp)
{
    static opt_timestamping_ret_t result;
    result.value = opt.timestamping;
    return &result;
}

void *set_opt_timestamping_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.timestamping = value;
    return (void *)&result;
}

opt_if_modified_since_ret_t *get_opt_if_modified_since_1_svc(struct svc_req *rqstp)
{
    static opt_if_modified_since_ret_t result;
    result.value = opt.if_modified_since;
    return &result;
}

void *set_opt_if_modified_since_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.if_modified_since = value;
    return (void *)&result;
}

opt_backup_converted_ret_t *get_opt_backup_converted_1_svc(struct svc_req *rqstp)
{
    static opt_backup_converted_ret_t result;
    result.value = opt.backup_converted;
    return &result;
}

void *set_opt_backup_converted_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.backup_converted = value;
    return (void *)&result;
}

opt_backups_ret_t *get_opt_backups_1_svc(struct svc_req *rqstp)
{
    static opt_backups_ret_t result;
    result.value = opt.backups;
    return &result;
}

void *set_opt_backups_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.backups = value;
    return (void *)&result;
}

opt_useragent_ret_t *get_opt_useragent_1_svc(struct svc_req *rqstp)
{
    static opt_useragent_ret_t result;
    if (opt.useragent != NULL) {
        result.value = strdup(opt.useragent);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_useragent_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.useragent != NULL) {
        free(opt.useragent);
    }
    if (value != NULL) {
        opt.useragent = strdup(value);
    } else {
        opt.useragent = NULL;
    }
    return (void *)&result;
}

opt_referer_ret_t *get_opt_referer_1_svc(struct svc_req *rqstp)
{
    static opt_referer_ret_t result;
    if (opt.referer != NULL) {
        result.value = strdup(opt.referer);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_referer_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.referer != NULL) {
        free(opt.referer);
    }
    if (value != NULL) {
        opt.referer = strdup(value);
    } else {
        opt.referer = NULL;
    }
    return (void *)&result;
}

opt_convert_links_ret_t *get_opt_convert_links_1_svc(struct svc_req *rqstp)
{
    static opt_convert_links_ret_t result;
    result.value = opt.convert_links;
    return &result;
}

void *set_opt_convert_links_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.convert_links = value;
    return (void *)&result;
}

opt_convert_file_only_ret_t *get_opt_convert_file_only_1_svc(struct svc_req *rqstp)
{
    static opt_convert_file_only_ret_t result;
    result.value = opt.convert_file_only;
    return &result;
}

void *set_opt_convert_file_only_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.convert_file_only = value;
    return (void *)&result;
}

opt_remove_listing_ret_t *get_opt_remove_listing_1_svc(struct svc_req *rqstp)
{
    static opt_remove_listing_ret_t result;
    result.value = opt.remove_listing;
    return &result;
}

void *set_opt_remove_listing_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.remove_listing = value;
    return (void *)&result;
}

opt_htmlify_ret_t *get_opt_htmlify_1_svc(struct svc_req *rqstp)
{
    static opt_htmlify_ret_t result;
    result.value = opt.htmlify;
    return &result;
}

void *set_opt_htmlify_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.htmlify = value;
    return (void *)&result;
}

opt_dot_style_ret_t *get_opt_dot_style_1_svc(struct svc_req *rqstp)
{
    static opt_dot_style_ret_t result;
    if (opt.dot_style != NULL) {
        result.value = strdup(opt.dot_style);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_dot_style_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.dot_style != NULL) {
        free(opt.dot_style);
    }
    if (value != NULL) {
        opt.dot_style = strdup(value);
    } else {
        opt.dot_style = NULL;
    }
    return (void *)&result;
}

opt_dot_bytes_ret_t *get_opt_dot_bytes_1_svc(struct svc_req *rqstp)
{
    static opt_dot_bytes_ret_t result;
    result.value = opt.dot_bytes;
    return &result;
}

void *set_opt_dot_bytes_1_svc(wgint value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.dot_bytes = value;
    return (void *)&result;
}

opt_dots_in_line_ret_t *get_opt_dots_in_line_1_svc(struct svc_req *rqstp)
{
    static opt_dots_in_line_ret_t result;
    result.value = opt.dots_in_line;
    return &result;
}

void *set_opt_dots_in_line_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.dots_in_line = value;
    return (void *)&result;
}

opt_dot_spacing_ret_t *get_opt_dot_spacing_1_svc(struct svc_req *rqstp)
{
    static opt_dot_spacing_ret_t result;
    result.value = opt.dot_spacing;
    return &result;
}

void *set_opt_dot_spacing_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.dot_spacing = value;
    return (void *)&result;
}

opt_delete_after_ret_t *get_opt_delete_after_1_svc(struct svc_req *rqstp)
{
    static opt_delete_after_ret_t result;
    result.value = opt.delete_after;
    return &result;
}

void *set_opt_delete_after_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.delete_after = value;
    return (void *)&result;
}

opt_adjust_extension_ret_t *get_opt_adjust_extension_1_svc(struct svc_req *rqstp)
{
    static opt_adjust_extension_ret_t result;
    result.value = opt.adjust_extension;
    return &result;
}

void *set_opt_adjust_extension_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.adjust_extension = value;
    return (void *)&result;
}

opt_page_requisites_ret_t *get_opt_page_requisites_1_svc(struct svc_req *rqstp)
{
    static opt_page_requisites_ret_t result;
    result.value = opt.page_requisites;
    return &result;
}

void *set_opt_page_requisites_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.page_requisites = value;
    return (void *)&result;
}

opt_bind_address_ret_t *get_opt_bind_address_1_svc(struct svc_req *rqstp)
{
    static opt_bind_address_ret_t result;
    if (opt.bind_address != NULL) {
        result.value = strdup(opt.bind_address);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_bind_address_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.bind_address != NULL) {
        free(opt.bind_address);
    }
    if (value != NULL) {
        opt.bind_address = strdup(value);
    } else {
        opt.bind_address = NULL;
    }
    return (void *)&result;
}

opt_secure_protocol_ret_t *get_opt_secure_protocol_1_svc(struct svc_req *rqstp)
{
    static opt_secure_protocol_ret_t result;
    result.value = opt.secure_protocol;
    return &result;
}

void *set_opt_secure_protocol_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.secure_protocol = value;
    return (void *)&result;
}

opt_check_cert_ret_t *get_opt_check_cert_1_svc(struct svc_req *rqstp)
{
    static opt_check_cert_ret_t result;
    result.value = opt.check_cert;
    return &result;
}

void *set_opt_check_cert_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.check_cert = value;
    return (void *)&result;
}

opt_cert_file_ret_t *get_opt_cert_file_1_svc(struct svc_req *rqstp)
{
    static opt_cert_file_ret_t result;
    if (opt.cert_file != NULL) {
        result.value = strdup(opt.cert_file);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_cert_file_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.cert_file != NULL) {
        free(opt.cert_file);
    }
    if (value != NULL) {
        opt.cert_file = strdup(value);
    } else {
        opt.cert_file = NULL;
    }
    return (void *)&result;
}

opt_private_key_ret_t *get_opt_private_key_1_svc(struct svc_req *rqstp)
{
    static opt_private_key_ret_t result;
    if (opt.private_key != NULL) {
        result.value = strdup(opt.private_key);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_private_key_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.private_key != NULL) {
        free(opt.private_key);
    }
    if (value != NULL) {
        opt.private_key = strdup(value);
    } else {
        opt.private_key = NULL;
    }
    return (void *)&result;
}

opt_cert_type_ret_t *get_opt_cert_type_1_svc(struct svc_req *rqstp)
{
    static opt_cert_type_ret_t result;
    result.value = opt.cert_type;
    return &result;
}

void *set_opt_cert_type_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.cert_type = value;
    return (void *)&result;
}

opt_private_key_type_ret_t *get_opt_private_key_type_1_svc(struct svc_req *rqstp)
{
    static opt_private_key_type_ret_t result;
    result.value = opt.private_key_type;
    return &result;
}

void *set_opt_private_key_type_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.private_key_type = value;
    return (void *)&result;
}

opt_ca_directory_ret_t *get_opt_ca_directory_1_svc(struct svc_req *rqstp)
{
    static opt_ca_directory_ret_t result;
    if (opt.ca_directory != NULL) {
        result.value = strdup(opt.ca_directory);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_ca_directory_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.ca_directory != NULL) {
        free(opt.ca_directory);
    }
    if (value != NULL) {
        opt.ca_directory = strdup(value);
    } else {
        opt.ca_directory = NULL;
    }
    return (void *)&result;
}

opt_ca_cert_ret_t *get_opt_ca_cert_1_svc(struct svc_req *rqstp)
{
    static opt_ca_cert_ret_t result;
    if (opt.ca_cert != NULL) {
        result.value = strdup(opt.ca_cert);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_ca_cert_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.ca_cert != NULL) {
        free(opt.ca_cert);
    }
    if (value != NULL) {
        opt.ca_cert = strdup(value);
    } else {
        opt.ca_cert = NULL;
    }
    return (void *)&result;
}

opt_crl_file_ret_t *get_opt_crl_file_1_svc(struct svc_req *rqstp)
{
    static opt_crl_file_ret_t result;
    if (opt.crl_file != NULL) {
        result.value = strdup(opt.crl_file);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_crl_file_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.crl_file != NULL) {
        free(opt.crl_file);
    }
    if (value != NULL) {
        opt.crl_file = strdup(value);
    } else {
        opt.crl_file = NULL;
    }
    return (void *)&result;
}

opt_pinnedpubkey_ret_t *get_opt_pinnedpubkey_1_svc(struct svc_req *rqstp)
{
    static opt_pinnedpubkey_ret_t result;
    if (opt.pinnedpubkey != NULL) {
        result.value = strdup(opt.pinnedpubkey);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_pinnedpubkey_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.pinnedpubkey != NULL) {
        free(opt.pinnedpubkey);
    }
    if (value != NULL) {
        opt.pinnedpubkey = strdup(value);
    } else {
        opt.pinnedpubkey = NULL;
    }
    return (void *)&result;
}

opt_random_file_ret_t *get_opt_random_file_1_svc(struct svc_req *rqstp)
{
    static opt_random_file_ret_t result;
    if (opt.random_file != NULL) {
        result.value = strdup(opt.random_file);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_random_file_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.random_file != NULL) {
        free(opt.random_file);
    }
    if (value != NULL) {
        opt.random_file = strdup(value);
    } else {
        opt.random_file = NULL;
    }
    return (void *)&result;
}

opt_egd_file_ret_t *get_opt_egd_file_1_svc(struct svc_req *rqstp)
{
    static opt_egd_file_ret_t result;
    if (opt.egd_file != NULL) {
        result.value = strdup(opt.egd_file);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_egd_file_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.egd_file != NULL) {
        free(opt.egd_file);
    }
    if (value != NULL) {
        opt.egd_file = strdup(value);
    } else {
        opt.egd_file = NULL;
    }
    return (void *)&result;
}

opt_https_only_ret_t *get_opt_https_only_1_svc(struct svc_req *rqstp)
{
    static opt_https_only_ret_t result;
    result.value = opt.https_only;
    return &result;
}

void *set_opt_https_only_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.https_only = value;
    return (void *)&result;
}

opt_ftps_resume_ssl_ret_t *get_opt_ftps_resume_ssl_1_svc(struct svc_req *rqstp)
{
    static opt_ftps_resume_ssl_ret_t result;
    result.value = opt.ftps_resume_ssl;
    return &result;
}

void *set_opt_ftps_resume_ssl_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.ftps_resume_ssl = value;
    return (void *)&result;
}

opt_ftps_fallback_to_ftp_ret_t *get_opt_ftps_fallback_to_ftp_1_svc(struct svc_req *rqstp)
{
    static opt_ftps_fallback_to_ftp_ret_t result;
    result.value = opt.ftps_fallback_to_ftp;
    return &result;
}

void *set_opt_ftps_fallback_to_ftp_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.ftps_fallback_to_ftp = value;
    return (void *)&result;
}

opt_ftps_implicit_ret_t *get_opt_ftps_implicit_1_svc(struct svc_req *rqstp)
{
    static opt_ftps_implicit_ret_t result;
    result.value = opt.ftps_implicit;
    return &result;
}

void *set_opt_ftps_implicit_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.ftps_implicit = value;
    return (void *)&result;
}

opt_ftps_clear_data_connection_ret_t *get_opt_ftps_clear_data_connection_1_svc(struct svc_req *rqstp)
{
    static opt_ftps_clear_data_connection_ret_t result;
    result.value = opt.ftps_clear_data_connection;
    return &result;
}

void *set_opt_ftps_clear_data_connection_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.ftps_clear_data_connection = value;
    return (void *)&result;
}

opt_cookies_ret_t *get_opt_cookies_1_svc(struct svc_req *rqstp)
{
    static opt_cookies_ret_t result;
    result.value = opt.cookies;
    return &result;
}

void *set_opt_cookies_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.cookies = value;
    return (void *)&result;
}

opt_cookies_input_ret_t *get_opt_cookies_input_1_svc(struct svc_req *rqstp)
{
    static opt_cookies_input_ret_t result;
    if (opt.cookies_input != NULL) {
        result.value = strdup(opt.cookies_input);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_cookies_input_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.cookies_input != NULL) {
        free(opt.cookies_input);
    }
    if (value != NULL) {
        opt.cookies_input = strdup(value);
    } else {
        opt.cookies_input = NULL;
    }
    return (void *)&result;
}

opt_cookies_output_ret_t *get_opt_cookies_output_1_svc(struct svc_req *rqstp)
{
    static opt_cookies_output_ret_t result;
    if (opt.cookies_output != NULL) {
        result.value = strdup(opt.cookies_output);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_cookies_output_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.cookies_output != NULL) {
        free(opt.cookies_output);
    }
    if (value != NULL) {
        opt.cookies_output = strdup(value);
    } else {
        opt.cookies_output = NULL;
    }
    return (void *)&result;
}

opt_keep_session_cookies_ret_t *get_opt_keep_session_cookies_1_svc(struct svc_req *rqstp)
{
    static opt_keep_session_cookies_ret_t result;
    result.value = opt.keep_session_cookies;
    return &result;
}

void *set_opt_keep_session_cookies_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.keep_session_cookies = value;
    return (void *)&result;
}

opt_post_data_ret_t *get_opt_post_data_1_svc(struct svc_req *rqstp)
{
    static opt_post_data_ret_t result;
    if (opt.post_data != NULL) {
        result.value = strdup(opt.post_data);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_post_data_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.post_data != NULL) {
        free(opt.post_data);
    }
    if (value != NULL) {
        opt.post_data = strdup(value);
    } else {
        opt.post_data = NULL;
    }
    return (void *)&result;
}

opt_post_file_name_ret_t *get_opt_post_file_name_1_svc(struct svc_req *rqstp)
{
    static opt_post_file_name_ret_t result;
    if (opt.post_file_name != NULL) {
        result.value = strdup(opt.post_file_name);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_post_file_name_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.post_file_name != NULL) {
        free(opt.post_file_name);
    }
    if (value != NULL) {
        opt.post_file_name = strdup(value);
    } else {
        opt.post_file_name = NULL;
    }
    return (void *)&result;
}

opt_method_ret_t *get_opt_method_1_svc(struct svc_req *rqstp)
{
    static opt_method_ret_t result;
    if (opt.method != NULL) {
        result.value = strdup(opt.method);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_method_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.method != NULL) {
        free(opt.method);
    }
    if (value != NULL) {
        opt.method = strdup(value);
    } else {
        opt.method = NULL;
    }
    return (void *)&result;
}

opt_body_data_ret_t *get_opt_body_data_1_svc(struct svc_req *rqstp)
{
    static opt_body_data_ret_t result;
    if (opt.body_data != NULL) {
        result.value = strdup(opt.body_data);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_body_data_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.body_data != NULL) {
        free(opt.body_data);
    }
    if (value != NULL) {
        opt.body_data = strdup(value);
    } else {
        opt.body_data = NULL;
    }
    return (void *)&result;
}

opt_body_file_ret_t *get_opt_body_file_1_svc(struct svc_req *rqstp)
{
    static opt_body_file_ret_t result;
    if (opt.body_file != NULL) {
        result.value = strdup(opt.body_file);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_body_file_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.body_file != NULL) {
        free(opt.body_file);
    }
    if (value != NULL) {
        opt.body_file = strdup(value);
    } else {
        opt.body_file = NULL;
    }
    return (void *)&result;
}

opt_restrict_files_os_ret_t *get_opt_restrict_files_os_1_svc(struct svc_req *rqstp)
{
    static opt_restrict_files_os_ret_t result;
    result.value = opt.restrict_files_os;
    return &result;
}

void *set_opt_restrict_files_os_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.restrict_files_os = value;
    return (void *)&result;
}

opt_restrict_files_ctrl_ret_t *get_opt_restrict_files_ctrl_1_svc(struct svc_req *rqstp)
{
    static opt_restrict_files_ctrl_ret_t result;
    result.value = opt.restrict_files_ctrl;
    return &result;
}

void *set_opt_restrict_files_ctrl_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.restrict_files_ctrl = value;
    return (void *)&result;
}

opt_restrict_files_nonascii_ret_t *get_opt_restrict_files_nonascii_1_svc(struct svc_req *rqstp)
{
    static opt_restrict_files_nonascii_ret_t result;
    result.value = opt.restrict_files_nonascii;
    return &result;
}

void *set_opt_restrict_files_nonascii_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.restrict_files_nonascii = value;
    return (void *)&result;
}

opt_restrict_files_case_ret_t *get_opt_restrict_files_case_1_svc(struct svc_req *rqstp)
{
    static opt_restrict_files_case_ret_t result;
    result.value = opt.restrict_files_case;
    return &result;
}

void *set_opt_restrict_files_case_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.restrict_files_case = value;
    return (void *)&result;
}

opt_strict_comments_ret_t *get_opt_strict_comments_1_svc(struct svc_req *rqstp)
{
    static opt_strict_comments_ret_t result;
    result.value = opt.strict_comments;
    return &result;
}

void *set_opt_strict_comments_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.strict_comments = value;
    return (void *)&result;
}

opt_preserve_perm_ret_t *get_opt_preserve_perm_1_svc(struct svc_req *rqstp)
{
    static opt_preserve_perm_ret_t result;
    result.value = opt.preserve_perm;
    return &result;
}

void *set_opt_preserve_perm_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.preserve_perm = value;
    return (void *)&result;
}

opt_ipv4_only_ret_t *get_opt_ipv4_only_1_svc(struct svc_req *rqstp)
{
    static opt_ipv4_only_ret_t result;
    result.value = opt.ipv4_only;
    return &result;
}

void *set_opt_ipv4_only_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.ipv4_only = value;
    return (void *)&result;
}

opt_ipv6_only_ret_t *get_opt_ipv6_only_1_svc(struct svc_req *rqstp)
{
    static opt_ipv6_only_ret_t result;
    result.value = opt.ipv6_only;
    return &result;
}

void *set_opt_ipv6_only_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.ipv6_only = value;
    return (void *)&result;
}

opt_prefer_family_ret_t *get_opt_prefer_family_1_svc(struct svc_req *rqstp)
{
    static opt_prefer_family_ret_t result;
    result.value = opt.prefer_family;
    return &result;
}

void *set_opt_prefer_family_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.prefer_family = value;
    return (void *)&result;
}

opt_content_disposition_ret_t *get_opt_content_disposition_1_svc(struct svc_req *rqstp)
{
    static opt_content_disposition_ret_t result;
    result.value = opt.content_disposition;
    return &result;
}

void *set_opt_content_disposition_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.content_disposition = value;
    return (void *)&result;
}

opt_auth_without_challenge_ret_t *get_opt_auth_without_challenge_1_svc(struct svc_req *rqstp)
{
    static opt_auth_without_challenge_ret_t result;
    result.value = opt.auth_without_challenge;
    return &result;
}

void *set_opt_auth_without_challenge_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.auth_without_challenge = value;
    return (void *)&result;
}

opt_enable_iri_ret_t *get_opt_enable_iri_1_svc(struct svc_req *rqstp)
{
    static opt_enable_iri_ret_t result;
    result.value = opt.enable_iri;
    return &result;
}

void *set_opt_enable_iri_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.enable_iri = value;
    return (void *)&result;
}

opt_encoding_remote_ret_t *get_opt_encoding_remote_1_svc(struct svc_req *rqstp)
{
    static opt_encoding_remote_ret_t result;
    if (opt.encoding_remote != NULL) {
        result.value = strdup(opt.encoding_remote);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_encoding_remote_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.encoding_remote != NULL) {
        free(opt.encoding_remote);
    }
    if (value != NULL) {
        opt.encoding_remote = strdup(value);
    } else {
        opt.encoding_remote = NULL;
    }
    return (void *)&result;
}

opt_locale_ret_t *get_opt_locale_1_svc(struct svc_req *rqstp)
{
    static opt_locale_ret_t result;
    if (opt.locale != NULL) {
        result.value = strdup(opt.locale);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_locale_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.locale != NULL) {
        free(opt.locale);
    }
    if (value != NULL) {
        opt.locale = strdup(value);
    } else {
        opt.locale = NULL;
    }
    return (void *)&result;
}

opt_trustservernames_ret_t *get_opt_trustservernames_1_svc(struct svc_req *rqstp)
{
    static opt_trustservernames_ret_t result;
    result.value = opt.trustservernames;
    return &result;
}

void *set_opt_trustservernames_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.trustservernames = value;
    return (void *)&result;
}

opt_useservertimestamps_ret_t *get_opt_useservertimestamps_1_svc(struct svc_req *rqstp)
{
    static opt_useservertimestamps_ret_t result;
    result.value = opt.useservertimestamps;
    return &result;
}

void *set_opt_useservertimestamps_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.useservertimestamps = value;
    return (void *)&result;
}

opt_show_all_dns_entries_ret_t *get_opt_show_all_dns_entries_1_svc(struct svc_req *rqstp)
{
    static opt_show_all_dns_entries_ret_t result;
    result.value = opt.show_all_dns_entries;
    return &result;
}

void *set_opt_show_all_dns_entries_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.show_all_dns_entries = value;
    return (void *)&result;
}

opt_report_bps_ret_t *get_opt_report_bps_1_svc(struct svc_req *rqstp)
{
    static opt_report_bps_ret_t result;
    result.value = opt.report_bps;
    return &result;
}

void *set_opt_report_bps_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.report_bps = value;
    return (void *)&result;
}

opt_rejected_log_ret_t *get_opt_rejected_log_1_svc(struct svc_req *rqstp)
{
    static opt_rejected_log_ret_t result;
    if (opt.rejected_log != NULL) {
        result.value = strdup(opt.rejected_log);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_rejected_log_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.rejected_log != NULL) {
        free(opt.rejected_log);
    }
    if (value != NULL) {
        opt.rejected_log = strdup(value);
    } else {
        opt.rejected_log = NULL;
    }
    return (void *)&result;
}

opt_hsts_ret_t *get_opt_hsts_1_svc(struct svc_req *rqstp)
{
    static opt_hsts_ret_t result;
    result.value = opt.hsts;
    return &result;
}

void *set_opt_hsts_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    opt.hsts = value;
    return (void *)&result;
}

opt_hsts_file_ret_t *get_opt_hsts_file_1_svc(struct svc_req *rqstp)
{
    static opt_hsts_file_ret_t result;
    if (opt.hsts_file != NULL) {
        result.value = strdup(opt.hsts_file);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_opt_hsts_file_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (opt.hsts_file != NULL) {
        free(opt.hsts_file);
    }
    if (value != NULL) {
        opt.hsts_file = strdup(value);
    } else {
        opt.hsts_file = NULL;
    }
    return (void *)&result;
}

total_download_time_ret_t *get_total_download_time_1_svc(struct svc_req *rqstp)
{
    static total_download_time_ret_t result;
    result.value = total_download_time;
    return &result;
}

void *set_total_download_time_1_svc(double value, struct svc_req *rqstp)
{
    static int result = 1; // 成功标志
    total_download_time = value;
    return (void *)&result;
}

warc_current_file_number_ret_t *get_warc_current_file_number_1_svc(struct svc_req *rqstp)
{
    static warc_current_file_number_ret_t result;
    result.value = warc_current_file_number;
    return &result;
}

void *set_warc_current_file_number_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1; // 成功标志
    warc_current_file_number = value;
    return (void *)&result;
}

output_redirected_ret_t *get_output_redirected_1_svc(struct svc_req *rqstp)
{
    static output_redirected_ret_t result;
    result.value = output_redirected;
    return &result;
}

void *set_output_redirected_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1; // 成功标志
    output_redirected = value;
    return (void *)&result;
}

transport_map_ret_t *get_transport_map_1_svc(struct svc_req *rqstp)
{
    static transport_map_ret_t result;
        if(transport_map==NULL) {
            result.transport_map.hash_table_rpc_ptr_len=0;
            result.transport_map.hash_table_rpc_ptr_val=NULL;
        } else {
            result.transport_map.hash_table_rpc_ptr_len=1;
            result.transport_map.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
            if(result.transport_map.hash_table_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.transport_map.hash_table_rpc_ptr_len;i0++) {
    if (transport_map[i0].cells == NULL) {result.transport_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;result.transport_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}

    else {result.transport_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;result.transport_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(result.transport_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));

    	if (result.transport_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {perror("malloc failed");exit(1);
}
    	for (int i1 = 0; i1 < result.transport_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {

    if (transport_map[i0].cells->key == NULL) {result.transport_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = 0;result.transport_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = NULL;}

    else{result.transport_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = sizeof(transport_map[i0].cells->key);result.transport_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = malloc(result.transport_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (result.transport_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.transport_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, transport_map[i0].cells->key, result.transport_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);}

    if (transport_map[i0].cells->value == NULL) {result.transport_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = 0;result.transport_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = NULL;}

    else{result.transport_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = sizeof(transport_map[i0].cells->value);result.transport_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = malloc(result.transport_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (result.transport_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.transport_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, transport_map[i0].cells->value, result.transport_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);}

    	}
    }
    result.transport_map.hash_table_rpc_ptr_val[i0].size=transport_map[i0].size;
    result.transport_map.hash_table_rpc_ptr_val[i0].count=transport_map[i0].count;
    result.transport_map.hash_table_rpc_ptr_val[i0].resize_threshold=transport_map[i0].resize_threshold;
    result.transport_map.hash_table_rpc_ptr_val[i0].prime_offset=transport_map[i0].prime_offset;
            }
        }
    return &result;
}

void *set_transport_map_1_svc(hash_table_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct hash_table *local_transport_map=NULL;
    if(arg1.hash_table_rpc_ptr_val!=NULL) {
    local_transport_map=(struct hash_table *)malloc(arg1.hash_table_rpc_ptr_len*sizeof(struct hash_table));
    if(local_transport_map==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.hash_table_rpc_ptr_len;i0++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {local_transport_map[i0].cells=NULL;}
    else {local_transport_map[i0].cells=(struct cell *)malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(struct cell));
    	if (local_transport_map[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {local_transport_map[i0].cells[i1].key=NULL;}

    else {local_transport_map[i0].cells[i1].key=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (local_transport_map[i0].cells[i1].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_transport_map[i0].cells[i1].key,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    }

    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {local_transport_map[i0].cells[i1].value=NULL;}

    else {local_transport_map[i0].cells[i1].value=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (local_transport_map[i0].cells[i1].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_transport_map[i0].cells[i1].value,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    }

    	}
    }
    local_transport_map[i0].size=arg1.hash_table_rpc_ptr_val[i0].size;
    local_transport_map[i0].count=arg1.hash_table_rpc_ptr_val[i0].count;
    local_transport_map[i0].resize_threshold=arg1.hash_table_rpc_ptr_val[i0].resize_threshold;
    local_transport_map[i0].prime_offset=arg1.hash_table_rpc_ptr_val[i0].prime_offset;
    	}
    }
    memcpy(&transport_map, &local_transport_map, sizeof(hash_table));
    return (void *)&result;
}

char_ptr *get_transport_map_hash_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (transport_map.hash_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_transport_map_hash_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        transport_map.hash_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        transport_map.hash_function = fun_ptr;
    }
    return (void *)&result;
}

char_ptr *get_transport_map_test_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (transport_map.test_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_transport_map_test_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        transport_map.test_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        transport_map.test_function = fun_ptr;
    }
    return (void *)&result;
}

transport_map_cells_ret_t *get_transport_map_cells_1_svc(struct svc_req *rqstp)
{
    static transport_map_cells_ret_t result;
        if(transport_map_cells==NULL) {
            result.transport_map_cells.cell_rpc_ptr_len=0;
            result.transport_map_cells.cell_rpc_ptr_val=NULL;
        } else {
            result.transport_map_cells.cell_rpc_ptr_len=1;
            result.transport_map_cells.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
            if(result.transport_map_cells.cell_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.transport_map_cells.cell_rpc_ptr_len;i0++) {
    if (transport_map_cells[i0].key == NULL) {result.transport_map_cells.cell_rpc_ptr_val[i0].key.key_len = 0;result.transport_map_cells.cell_rpc_ptr_val[i0].key.key_val = NULL;}

    else{result.transport_map_cells.cell_rpc_ptr_val[i0].key.key_len = sizeof(transport_map_cells[i0].key);result.transport_map_cells.cell_rpc_ptr_val[i0].key.key_val = malloc(result.transport_map_cells.cell_rpc_ptr_val[i0].key.key_len);

    	if (result.transport_map_cells.cell_rpc_ptr_val[i0].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.transport_map_cells.cell_rpc_ptr_val[i0].key.key_val, transport_map_cells[i0].key, result.transport_map_cells.cell_rpc_ptr_val[i0].key.key_len);}

    if (transport_map_cells[i0].value == NULL) {result.transport_map_cells.cell_rpc_ptr_val[i0].value.value_len = 0;result.transport_map_cells.cell_rpc_ptr_val[i0].value.value_val = NULL;}

    else{result.transport_map_cells.cell_rpc_ptr_val[i0].value.value_len = sizeof(transport_map_cells[i0].value);result.transport_map_cells.cell_rpc_ptr_val[i0].value.value_val = malloc(result.transport_map_cells.cell_rpc_ptr_val[i0].value.value_len);

    	if (result.transport_map_cells.cell_rpc_ptr_val[i0].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.transport_map_cells.cell_rpc_ptr_val[i0].value.value_val, transport_map_cells[i0].value, result.transport_map_cells.cell_rpc_ptr_val[i0].value.value_len);}

            }
        }
    return &result;
}

void *set_transport_map_cells_1_svc(cell_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct cell *local_transport_map_cells=NULL;
    if(arg1.cell_rpc_ptr_val!=NULL) {
    local_transport_map_cells=(struct cell *)malloc(arg1.cell_rpc_ptr_len*sizeof(struct cell));
    if(local_transport_map_cells==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.cell_rpc_ptr_len;i0++) {
    if (arg1.cell_rpc_ptr_val[i0].key.key_val == NULL) {local_transport_map_cells[i0].key=NULL;}

    else {local_transport_map_cells[i0].key=malloc(arg1.cell_rpc_ptr_val[i0].key.key_len);

    	if (local_transport_map_cells[i0].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_transport_map_cells[i0].key,arg1.cell_rpc_ptr_val[i0].key.key_val,arg1.cell_rpc_ptr_val[i0].key.key_len);

    }

    if (arg1.cell_rpc_ptr_val[i0].value.value_val == NULL) {local_transport_map_cells[i0].value=NULL;}

    else {local_transport_map_cells[i0].value=malloc(arg1.cell_rpc_ptr_val[i0].value.value_len);

    	if (local_transport_map_cells[i0].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_transport_map_cells[i0].value,arg1.cell_rpc_ptr_val[i0].value.value_val,arg1.cell_rpc_ptr_val[i0].value.value_len);

    }

    	}
    }
    memcpy(&transport_map_cells, &local_transport_map_cells, sizeof(cell));
    return (void *)&result;
}

transport_map_cells_key_ret_t *get_transport_map_cells_key_1_svc(struct svc_req *rqstp)
{
    static transport_map_cells_key_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 transport_map_cells.key 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t transport_map_cells_key_size; // 假设有这样的辅助变量
    if (transport_map_cells.key == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = transport_map_cells_key_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = transport_map_cells.key;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t transport_map_cells_key_size; // 假设有这样的辅助变量
void *set_transport_map_cells_key_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (transport_map_cells.key != NULL) {
        free(transport_map_cells.key);
        transport_map_cells.key = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        transport_map_cells.key = malloc(value.bytes.bytes_len);
        if (transport_map_cells.key == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(transport_map_cells.key, value.bytes.bytes_val, value.bytes.bytes_len);
            transport_map_cells_key_size = value.size;
        }
    }
    return (void *)&result;
}

transport_map_cells_value_ret_t *get_transport_map_cells_value_1_svc(struct svc_req *rqstp)
{
    static transport_map_cells_value_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 transport_map_cells.value 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t transport_map_cells_value_size; // 假设有这样的辅助变量
    if (transport_map_cells.value == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = transport_map_cells_value_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = transport_map_cells.value;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t transport_map_cells_value_size; // 假设有这样的辅助变量
void *set_transport_map_cells_value_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (transport_map_cells.value != NULL) {
        free(transport_map_cells.value);
        transport_map_cells.value = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        transport_map_cells.value = malloc(value.bytes.bytes_len);
        if (transport_map_cells.value == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(transport_map_cells.value, value.bytes.bytes_val, value.bytes.bytes_len);
            transport_map_cells_value_size = value.size;
        }
    }
    return (void *)&result;
}

transport_map_size_ret_t *get_transport_map_size_1_svc(struct svc_req *rqstp)
{
    static transport_map_size_ret_t result;
    result.value = transport_map.size;
    return &result;
}

void *set_transport_map_size_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    transport_map.size = value;
    return (void *)&result;
}

transport_map_count_ret_t *get_transport_map_count_1_svc(struct svc_req *rqstp)
{
    static transport_map_count_ret_t result;
    result.value = transport_map.count;
    return &result;
}

void *set_transport_map_count_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    transport_map.count = value;
    return (void *)&result;
}

transport_map_resize_threshold_ret_t *get_transport_map_resize_threshold_1_svc(struct svc_req *rqstp)
{
    static transport_map_resize_threshold_ret_t result;
    result.value = transport_map.resize_threshold;
    return &result;
}

void *set_transport_map_resize_threshold_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    transport_map.resize_threshold = value;
    return (void *)&result;
}

transport_map_prime_offset_ret_t *get_transport_map_prime_offset_1_svc(struct svc_req *rqstp)
{
    static transport_map_prime_offset_ret_t result;
    result.value = transport_map.prime_offset;
    return &result;
}

void *set_transport_map_prime_offset_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    transport_map.prime_offset = value;
    return (void *)&result;
}

interesting_tags_ret_t *get_interesting_tags_1_svc(struct svc_req *rqstp)
{
    static interesting_tags_ret_t result;
        if(interesting_tags==NULL) {
            result.interesting_tags.hash_table_rpc_ptr_len=0;
            result.interesting_tags.hash_table_rpc_ptr_val=NULL;
        } else {
            result.interesting_tags.hash_table_rpc_ptr_len=1;
            result.interesting_tags.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
            if(result.interesting_tags.hash_table_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.interesting_tags.hash_table_rpc_ptr_len;i0++) {
    if (interesting_tags[i0].cells == NULL) {result.interesting_tags.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;result.interesting_tags.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}

    else {result.interesting_tags.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;result.interesting_tags.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(result.interesting_tags.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));

    	if (result.interesting_tags.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {perror("malloc failed");exit(1);
}
    	for (int i1 = 0; i1 < result.interesting_tags.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {

    if (interesting_tags[i0].cells->key == NULL) {result.interesting_tags.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = 0;result.interesting_tags.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = NULL;}

    else{result.interesting_tags.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = sizeof(interesting_tags[i0].cells->key);result.interesting_tags.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = malloc(result.interesting_tags.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (result.interesting_tags.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.interesting_tags.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, interesting_tags[i0].cells->key, result.interesting_tags.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);}

    if (interesting_tags[i0].cells->value == NULL) {result.interesting_tags.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = 0;result.interesting_tags.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = NULL;}

    else{result.interesting_tags.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = sizeof(interesting_tags[i0].cells->value);result.interesting_tags.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = malloc(result.interesting_tags.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (result.interesting_tags.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.interesting_tags.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, interesting_tags[i0].cells->value, result.interesting_tags.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);}

    	}
    }
    result.interesting_tags.hash_table_rpc_ptr_val[i0].size=interesting_tags[i0].size;
    result.interesting_tags.hash_table_rpc_ptr_val[i0].count=interesting_tags[i0].count;
    result.interesting_tags.hash_table_rpc_ptr_val[i0].resize_threshold=interesting_tags[i0].resize_threshold;
    result.interesting_tags.hash_table_rpc_ptr_val[i0].prime_offset=interesting_tags[i0].prime_offset;
            }
        }
    return &result;
}

void *set_interesting_tags_1_svc(hash_table_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct hash_table *local_interesting_tags=NULL;
    if(arg1.hash_table_rpc_ptr_val!=NULL) {
    local_interesting_tags=(struct hash_table *)malloc(arg1.hash_table_rpc_ptr_len*sizeof(struct hash_table));
    if(local_interesting_tags==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.hash_table_rpc_ptr_len;i0++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {local_interesting_tags[i0].cells=NULL;}
    else {local_interesting_tags[i0].cells=(struct cell *)malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(struct cell));
    	if (local_interesting_tags[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {local_interesting_tags[i0].cells[i1].key=NULL;}

    else {local_interesting_tags[i0].cells[i1].key=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (local_interesting_tags[i0].cells[i1].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_interesting_tags[i0].cells[i1].key,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    }

    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {local_interesting_tags[i0].cells[i1].value=NULL;}

    else {local_interesting_tags[i0].cells[i1].value=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (local_interesting_tags[i0].cells[i1].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_interesting_tags[i0].cells[i1].value,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    }

    	}
    }
    local_interesting_tags[i0].size=arg1.hash_table_rpc_ptr_val[i0].size;
    local_interesting_tags[i0].count=arg1.hash_table_rpc_ptr_val[i0].count;
    local_interesting_tags[i0].resize_threshold=arg1.hash_table_rpc_ptr_val[i0].resize_threshold;
    local_interesting_tags[i0].prime_offset=arg1.hash_table_rpc_ptr_val[i0].prime_offset;
    	}
    }
    memcpy(&interesting_tags, &local_interesting_tags, sizeof(hash_table));
    return (void *)&result;
}

char_ptr *get_interesting_tags_hash_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (interesting_tags.hash_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_interesting_tags_hash_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        interesting_tags.hash_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        interesting_tags.hash_function = fun_ptr;
    }
    return (void *)&result;
}

char_ptr *get_interesting_tags_test_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (interesting_tags.test_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_interesting_tags_test_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        interesting_tags.test_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        interesting_tags.test_function = fun_ptr;
    }
    return (void *)&result;
}

interesting_tags_cells_ret_t *get_interesting_tags_cells_1_svc(struct svc_req *rqstp)
{
    static interesting_tags_cells_ret_t result;
        if(interesting_tags_cells==NULL) {
            result.interesting_tags_cells.cell_rpc_ptr_len=0;
            result.interesting_tags_cells.cell_rpc_ptr_val=NULL;
        } else {
            result.interesting_tags_cells.cell_rpc_ptr_len=1;
            result.interesting_tags_cells.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
            if(result.interesting_tags_cells.cell_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.interesting_tags_cells.cell_rpc_ptr_len;i0++) {
    if (interesting_tags_cells[i0].key == NULL) {result.interesting_tags_cells.cell_rpc_ptr_val[i0].key.key_len = 0;result.interesting_tags_cells.cell_rpc_ptr_val[i0].key.key_val = NULL;}

    else{result.interesting_tags_cells.cell_rpc_ptr_val[i0].key.key_len = sizeof(interesting_tags_cells[i0].key);result.interesting_tags_cells.cell_rpc_ptr_val[i0].key.key_val = malloc(result.interesting_tags_cells.cell_rpc_ptr_val[i0].key.key_len);

    	if (result.interesting_tags_cells.cell_rpc_ptr_val[i0].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.interesting_tags_cells.cell_rpc_ptr_val[i0].key.key_val, interesting_tags_cells[i0].key, result.interesting_tags_cells.cell_rpc_ptr_val[i0].key.key_len);}

    if (interesting_tags_cells[i0].value == NULL) {result.interesting_tags_cells.cell_rpc_ptr_val[i0].value.value_len = 0;result.interesting_tags_cells.cell_rpc_ptr_val[i0].value.value_val = NULL;}

    else{result.interesting_tags_cells.cell_rpc_ptr_val[i0].value.value_len = sizeof(interesting_tags_cells[i0].value);result.interesting_tags_cells.cell_rpc_ptr_val[i0].value.value_val = malloc(result.interesting_tags_cells.cell_rpc_ptr_val[i0].value.value_len);

    	if (result.interesting_tags_cells.cell_rpc_ptr_val[i0].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.interesting_tags_cells.cell_rpc_ptr_val[i0].value.value_val, interesting_tags_cells[i0].value, result.interesting_tags_cells.cell_rpc_ptr_val[i0].value.value_len);}

            }
        }
    return &result;
}

void *set_interesting_tags_cells_1_svc(cell_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct cell *local_interesting_tags_cells=NULL;
    if(arg1.cell_rpc_ptr_val!=NULL) {
    local_interesting_tags_cells=(struct cell *)malloc(arg1.cell_rpc_ptr_len*sizeof(struct cell));
    if(local_interesting_tags_cells==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.cell_rpc_ptr_len;i0++) {
    if (arg1.cell_rpc_ptr_val[i0].key.key_val == NULL) {local_interesting_tags_cells[i0].key=NULL;}

    else {local_interesting_tags_cells[i0].key=malloc(arg1.cell_rpc_ptr_val[i0].key.key_len);

    	if (local_interesting_tags_cells[i0].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_interesting_tags_cells[i0].key,arg1.cell_rpc_ptr_val[i0].key.key_val,arg1.cell_rpc_ptr_val[i0].key.key_len);

    }

    if (arg1.cell_rpc_ptr_val[i0].value.value_val == NULL) {local_interesting_tags_cells[i0].value=NULL;}

    else {local_interesting_tags_cells[i0].value=malloc(arg1.cell_rpc_ptr_val[i0].value.value_len);

    	if (local_interesting_tags_cells[i0].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_interesting_tags_cells[i0].value,arg1.cell_rpc_ptr_val[i0].value.value_val,arg1.cell_rpc_ptr_val[i0].value.value_len);

    }

    	}
    }
    memcpy(&interesting_tags_cells, &local_interesting_tags_cells, sizeof(cell));
    return (void *)&result;
}

interesting_tags_cells_key_ret_t *get_interesting_tags_cells_key_1_svc(struct svc_req *rqstp)
{
    static interesting_tags_cells_key_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 interesting_tags_cells.key 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t interesting_tags_cells_key_size; // 假设有这样的辅助变量
    if (interesting_tags_cells.key == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = interesting_tags_cells_key_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = interesting_tags_cells.key;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t interesting_tags_cells_key_size; // 假设有这样的辅助变量
void *set_interesting_tags_cells_key_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (interesting_tags_cells.key != NULL) {
        free(interesting_tags_cells.key);
        interesting_tags_cells.key = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        interesting_tags_cells.key = malloc(value.bytes.bytes_len);
        if (interesting_tags_cells.key == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(interesting_tags_cells.key, value.bytes.bytes_val, value.bytes.bytes_len);
            interesting_tags_cells_key_size = value.size;
        }
    }
    return (void *)&result;
}

interesting_tags_cells_value_ret_t *get_interesting_tags_cells_value_1_svc(struct svc_req *rqstp)
{
    static interesting_tags_cells_value_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 interesting_tags_cells.value 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t interesting_tags_cells_value_size; // 假设有这样的辅助变量
    if (interesting_tags_cells.value == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = interesting_tags_cells_value_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = interesting_tags_cells.value;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t interesting_tags_cells_value_size; // 假设有这样的辅助变量
void *set_interesting_tags_cells_value_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (interesting_tags_cells.value != NULL) {
        free(interesting_tags_cells.value);
        interesting_tags_cells.value = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        interesting_tags_cells.value = malloc(value.bytes.bytes_len);
        if (interesting_tags_cells.value == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(interesting_tags_cells.value, value.bytes.bytes_val, value.bytes.bytes_len);
            interesting_tags_cells_value_size = value.size;
        }
    }
    return (void *)&result;
}

interesting_tags_size_ret_t *get_interesting_tags_size_1_svc(struct svc_req *rqstp)
{
    static interesting_tags_size_ret_t result;
    result.value = interesting_tags.size;
    return &result;
}

void *set_interesting_tags_size_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    interesting_tags.size = value;
    return (void *)&result;
}

interesting_tags_count_ret_t *get_interesting_tags_count_1_svc(struct svc_req *rqstp)
{
    static interesting_tags_count_ret_t result;
    result.value = interesting_tags.count;
    return &result;
}

void *set_interesting_tags_count_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    interesting_tags.count = value;
    return (void *)&result;
}

interesting_tags_resize_threshold_ret_t *get_interesting_tags_resize_threshold_1_svc(struct svc_req *rqstp)
{
    static interesting_tags_resize_threshold_ret_t result;
    result.value = interesting_tags.resize_threshold;
    return &result;
}

void *set_interesting_tags_resize_threshold_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    interesting_tags.resize_threshold = value;
    return (void *)&result;
}

interesting_tags_prime_offset_ret_t *get_interesting_tags_prime_offset_1_svc(struct svc_req *rqstp)
{
    static interesting_tags_prime_offset_ret_t result;
    result.value = interesting_tags.prime_offset;
    return &result;
}

void *set_interesting_tags_prime_offset_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    interesting_tags.prime_offset = value;
    return (void *)&result;
}

basic_authed_hosts_ret_t *get_basic_authed_hosts_1_svc(struct svc_req *rqstp)
{
    static basic_authed_hosts_ret_t result;
        if(basic_authed_hosts==NULL) {
            result.basic_authed_hosts.hash_table_rpc_ptr_len=0;
            result.basic_authed_hosts.hash_table_rpc_ptr_val=NULL;
        } else {
            result.basic_authed_hosts.hash_table_rpc_ptr_len=1;
            result.basic_authed_hosts.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
            if(result.basic_authed_hosts.hash_table_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.basic_authed_hosts.hash_table_rpc_ptr_len;i0++) {
    if (basic_authed_hosts[i0].cells == NULL) {result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}

    else {result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));

    	if (result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {perror("malloc failed");exit(1);
}
    	for (int i1 = 0; i1 < result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {

    if (basic_authed_hosts[i0].cells->key == NULL) {result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = 0;result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = NULL;}

    else{result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = sizeof(basic_authed_hosts[i0].cells->key);result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = malloc(result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, basic_authed_hosts[i0].cells->key, result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);}

    if (basic_authed_hosts[i0].cells->value == NULL) {result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = 0;result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = NULL;}

    else{result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = sizeof(basic_authed_hosts[i0].cells->value);result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = malloc(result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, basic_authed_hosts[i0].cells->value, result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);}

    	}
    }
    result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].size=basic_authed_hosts[i0].size;
    result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].count=basic_authed_hosts[i0].count;
    result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].resize_threshold=basic_authed_hosts[i0].resize_threshold;
    result.basic_authed_hosts.hash_table_rpc_ptr_val[i0].prime_offset=basic_authed_hosts[i0].prime_offset;
            }
        }
    return &result;
}

void *set_basic_authed_hosts_1_svc(hash_table_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct hash_table *local_basic_authed_hosts=NULL;
    if(arg1.hash_table_rpc_ptr_val!=NULL) {
    local_basic_authed_hosts=(struct hash_table *)malloc(arg1.hash_table_rpc_ptr_len*sizeof(struct hash_table));
    if(local_basic_authed_hosts==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.hash_table_rpc_ptr_len;i0++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {local_basic_authed_hosts[i0].cells=NULL;}
    else {local_basic_authed_hosts[i0].cells=(struct cell *)malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(struct cell));
    	if (local_basic_authed_hosts[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {local_basic_authed_hosts[i0].cells[i1].key=NULL;}

    else {local_basic_authed_hosts[i0].cells[i1].key=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (local_basic_authed_hosts[i0].cells[i1].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_basic_authed_hosts[i0].cells[i1].key,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    }

    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {local_basic_authed_hosts[i0].cells[i1].value=NULL;}

    else {local_basic_authed_hosts[i0].cells[i1].value=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (local_basic_authed_hosts[i0].cells[i1].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_basic_authed_hosts[i0].cells[i1].value,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    }

    	}
    }
    local_basic_authed_hosts[i0].size=arg1.hash_table_rpc_ptr_val[i0].size;
    local_basic_authed_hosts[i0].count=arg1.hash_table_rpc_ptr_val[i0].count;
    local_basic_authed_hosts[i0].resize_threshold=arg1.hash_table_rpc_ptr_val[i0].resize_threshold;
    local_basic_authed_hosts[i0].prime_offset=arg1.hash_table_rpc_ptr_val[i0].prime_offset;
    	}
    }
    memcpy(&basic_authed_hosts, &local_basic_authed_hosts, sizeof(hash_table));
    return (void *)&result;
}

char_ptr *get_basic_authed_hosts_hash_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (basic_authed_hosts.hash_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_basic_authed_hosts_hash_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        basic_authed_hosts.hash_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        basic_authed_hosts.hash_function = fun_ptr;
    }
    return (void *)&result;
}

char_ptr *get_basic_authed_hosts_test_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (basic_authed_hosts.test_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_basic_authed_hosts_test_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        basic_authed_hosts.test_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        basic_authed_hosts.test_function = fun_ptr;
    }
    return (void *)&result;
}

basic_authed_hosts_cells_ret_t *get_basic_authed_hosts_cells_1_svc(struct svc_req *rqstp)
{
    static basic_authed_hosts_cells_ret_t result;
        if(basic_authed_hosts_cells==NULL) {
            result.basic_authed_hosts_cells.cell_rpc_ptr_len=0;
            result.basic_authed_hosts_cells.cell_rpc_ptr_val=NULL;
        } else {
            result.basic_authed_hosts_cells.cell_rpc_ptr_len=1;
            result.basic_authed_hosts_cells.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
            if(result.basic_authed_hosts_cells.cell_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.basic_authed_hosts_cells.cell_rpc_ptr_len;i0++) {
    if (basic_authed_hosts_cells[i0].key == NULL) {result.basic_authed_hosts_cells.cell_rpc_ptr_val[i0].key.key_len = 0;result.basic_authed_hosts_cells.cell_rpc_ptr_val[i0].key.key_val = NULL;}

    else{result.basic_authed_hosts_cells.cell_rpc_ptr_val[i0].key.key_len = sizeof(basic_authed_hosts_cells[i0].key);result.basic_authed_hosts_cells.cell_rpc_ptr_val[i0].key.key_val = malloc(result.basic_authed_hosts_cells.cell_rpc_ptr_val[i0].key.key_len);

    	if (result.basic_authed_hosts_cells.cell_rpc_ptr_val[i0].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.basic_authed_hosts_cells.cell_rpc_ptr_val[i0].key.key_val, basic_authed_hosts_cells[i0].key, result.basic_authed_hosts_cells.cell_rpc_ptr_val[i0].key.key_len);}

    if (basic_authed_hosts_cells[i0].value == NULL) {result.basic_authed_hosts_cells.cell_rpc_ptr_val[i0].value.value_len = 0;result.basic_authed_hosts_cells.cell_rpc_ptr_val[i0].value.value_val = NULL;}

    else{result.basic_authed_hosts_cells.cell_rpc_ptr_val[i0].value.value_len = sizeof(basic_authed_hosts_cells[i0].value);result.basic_authed_hosts_cells.cell_rpc_ptr_val[i0].value.value_val = malloc(result.basic_authed_hosts_cells.cell_rpc_ptr_val[i0].value.value_len);

    	if (result.basic_authed_hosts_cells.cell_rpc_ptr_val[i0].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.basic_authed_hosts_cells.cell_rpc_ptr_val[i0].value.value_val, basic_authed_hosts_cells[i0].value, result.basic_authed_hosts_cells.cell_rpc_ptr_val[i0].value.value_len);}

            }
        }
    return &result;
}

void *set_basic_authed_hosts_cells_1_svc(cell_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct cell *local_basic_authed_hosts_cells=NULL;
    if(arg1.cell_rpc_ptr_val!=NULL) {
    local_basic_authed_hosts_cells=(struct cell *)malloc(arg1.cell_rpc_ptr_len*sizeof(struct cell));
    if(local_basic_authed_hosts_cells==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.cell_rpc_ptr_len;i0++) {
    if (arg1.cell_rpc_ptr_val[i0].key.key_val == NULL) {local_basic_authed_hosts_cells[i0].key=NULL;}

    else {local_basic_authed_hosts_cells[i0].key=malloc(arg1.cell_rpc_ptr_val[i0].key.key_len);

    	if (local_basic_authed_hosts_cells[i0].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_basic_authed_hosts_cells[i0].key,arg1.cell_rpc_ptr_val[i0].key.key_val,arg1.cell_rpc_ptr_val[i0].key.key_len);

    }

    if (arg1.cell_rpc_ptr_val[i0].value.value_val == NULL) {local_basic_authed_hosts_cells[i0].value=NULL;}

    else {local_basic_authed_hosts_cells[i0].value=malloc(arg1.cell_rpc_ptr_val[i0].value.value_len);

    	if (local_basic_authed_hosts_cells[i0].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_basic_authed_hosts_cells[i0].value,arg1.cell_rpc_ptr_val[i0].value.value_val,arg1.cell_rpc_ptr_val[i0].value.value_len);

    }

    	}
    }
    memcpy(&basic_authed_hosts_cells, &local_basic_authed_hosts_cells, sizeof(cell));
    return (void *)&result;
}

basic_authed_hosts_cells_key_ret_t *get_basic_authed_hosts_cells_key_1_svc(struct svc_req *rqstp)
{
    static basic_authed_hosts_cells_key_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 basic_authed_hosts_cells.key 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t basic_authed_hosts_cells_key_size; // 假设有这样的辅助变量
    if (basic_authed_hosts_cells.key == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = basic_authed_hosts_cells_key_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = basic_authed_hosts_cells.key;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t basic_authed_hosts_cells_key_size; // 假设有这样的辅助变量
void *set_basic_authed_hosts_cells_key_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (basic_authed_hosts_cells.key != NULL) {
        free(basic_authed_hosts_cells.key);
        basic_authed_hosts_cells.key = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        basic_authed_hosts_cells.key = malloc(value.bytes.bytes_len);
        if (basic_authed_hosts_cells.key == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(basic_authed_hosts_cells.key, value.bytes.bytes_val, value.bytes.bytes_len);
            basic_authed_hosts_cells_key_size = value.size;
        }
    }
    return (void *)&result;
}

basic_authed_hosts_cells_value_ret_t *get_basic_authed_hosts_cells_value_1_svc(struct svc_req *rqstp)
{
    static basic_authed_hosts_cells_value_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 basic_authed_hosts_cells.value 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t basic_authed_hosts_cells_value_size; // 假设有这样的辅助变量
    if (basic_authed_hosts_cells.value == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = basic_authed_hosts_cells_value_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = basic_authed_hosts_cells.value;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t basic_authed_hosts_cells_value_size; // 假设有这样的辅助变量
void *set_basic_authed_hosts_cells_value_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (basic_authed_hosts_cells.value != NULL) {
        free(basic_authed_hosts_cells.value);
        basic_authed_hosts_cells.value = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        basic_authed_hosts_cells.value = malloc(value.bytes.bytes_len);
        if (basic_authed_hosts_cells.value == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(basic_authed_hosts_cells.value, value.bytes.bytes_val, value.bytes.bytes_len);
            basic_authed_hosts_cells_value_size = value.size;
        }
    }
    return (void *)&result;
}

basic_authed_hosts_size_ret_t *get_basic_authed_hosts_size_1_svc(struct svc_req *rqstp)
{
    static basic_authed_hosts_size_ret_t result;
    result.value = basic_authed_hosts.size;
    return &result;
}

void *set_basic_authed_hosts_size_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    basic_authed_hosts.size = value;
    return (void *)&result;
}

basic_authed_hosts_count_ret_t *get_basic_authed_hosts_count_1_svc(struct svc_req *rqstp)
{
    static basic_authed_hosts_count_ret_t result;
    result.value = basic_authed_hosts.count;
    return &result;
}

void *set_basic_authed_hosts_count_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    basic_authed_hosts.count = value;
    return (void *)&result;
}

basic_authed_hosts_resize_threshold_ret_t *get_basic_authed_hosts_resize_threshold_1_svc(struct svc_req *rqstp)
{
    static basic_authed_hosts_resize_threshold_ret_t result;
    result.value = basic_authed_hosts.resize_threshold;
    return &result;
}

void *set_basic_authed_hosts_resize_threshold_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    basic_authed_hosts.resize_threshold = value;
    return (void *)&result;
}

basic_authed_hosts_prime_offset_ret_t *get_basic_authed_hosts_prime_offset_1_svc(struct svc_req *rqstp)
{
    static basic_authed_hosts_prime_offset_ret_t result;
    result.value = basic_authed_hosts.prime_offset;
    return &result;
}

void *set_basic_authed_hosts_prime_offset_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    basic_authed_hosts.prime_offset = value;
    return (void *)&result;
}

char_ptr *get_warc_current_file_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *filename=get_filename_from_fp(warc_current_file);
    if (filename == NULL) {
        result.charptr.charptr_len = 0;
        result.charptr.charptr_val = NULL;
    } else {
        result.charptr.charptr_len = strlen(filename) + 1;
        result.charptr.charptr_val = filename;
    }
    return &result;
}

            warc_current_file = stdin;
        } else if (strcmp(value.charptr.charptr_val, "stdout") == 0) {
            warc_current_file = stdout;
        } else if (strcmp(value.charptr.charptr_val, "stderr") == 0) {
            warc_current_file = stderr;
        } else {
            warc_current_file = fopen(value.charptr.charptr_val, "r+");
            if ({var_name} == NULL) {
                perror("fopen failed");
                return NULL;
            }
        }
void *set_warc_current_file_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result;
    if (value.charptr.charptr_val == NULL) {
        warc_current_file = NULL;
    } else {
        // 假设文件模式为读写
      if (strcmp(value.charptr.charptr_val, "stdin") == 0) {
j    }
    return (void *)&result;
}

rnd_seeded_ret_t *get_rnd_seeded_1_svc(struct svc_req *rqstp)
{
    static rnd_seeded_ret_t result;
    result.value = rnd_seeded;
    return &result;
}

void *set_rnd_seeded_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1; // 成功标志
    rnd_seeded = value;
    return (void *)&result;
}

redirect_request_ret_t *get_redirect_request_1_svc(struct svc_req *rqstp)
{
    static redirect_request_ret_t result;
    result.value = redirect_request;
    return &result;
}

void *set_redirect_request_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1; // 成功标志
    redirect_request = value;
    return (void *)&result;
}

warc_current_warcinfo_uuid_str_array *get_warc_current_warcinfo_uuid_str_1_svc(struct svc_req *rqstp)
{
    static warc_current_warcinfo_uuid_str_array result;
    strncpy(result.warc_current_warcinfo_uuid_str, warc_current_warcinfo_uuid_str, 48);
    result.warc_current_warcinfo_uuid_str[48 - 1] = '\0'; // 确保字符串正确终止
    return &result;
}

void *set_warc_current_warcinfo_uuid_str_1_svc(warc_current_warcinfo_uuid_str_array value, struct svc_req *rqstp)
{
    static int result = 1;
    strncpy(warc_current_warcinfo_uuid_str, value.warc_current_warcinfo_uuid_str, 48);
    warc_current_warcinfo_uuid_str[48 - 1] = '\0'; // 确保字符串正确终止
    return (void *)&result;
}

char_ptr *get_exec_name_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    if (exec_name == NULL) {
        result.charptr.charptr_len = 0;
        result.charptr.charptr_val = NULL;
    } else {
        result.charptr.charptr_len = strlen(exec_name) + 1;
        result.charptr.charptr_val = exec_name;
    }
    return &result;
}

void *set_exec_name_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result;
    if (exec_name != NULL) {
        free(exec_name);
    }
    if (value.charptr.charptr_val == NULL) {
        exec_name = NULL;
    } else {
        exec_name=malloc(value.charptr.charptr_len);
        memcpy(exec_name, value.charptr.charptr_val, value.charptr.charptr_len);
    }
    return (void *)&result;
}

char_ptr *get_yytext_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    if (yytext == NULL) {
        result.charptr.charptr_len = 0;
        result.charptr.charptr_val = NULL;
    } else {
        result.charptr.charptr_len = strlen(yytext) + 1;
        result.charptr.charptr_val = yytext;
    }
    return &result;
}

void *set_yytext_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result;
    if (yytext != NULL) {
        free(yytext);
    }
    if (value.charptr.charptr_val == NULL) {
        yytext = NULL;
    } else {
        yytext=malloc(value.charptr.charptr_len);
        memcpy(yytext, value.charptr.charptr_val, value.charptr.charptr_len);
    }
    return (void *)&result;
}

nonexisting_urls_set_ret_t *get_nonexisting_urls_set_1_svc(struct svc_req *rqstp)
{
    static nonexisting_urls_set_ret_t result;
        if(nonexisting_urls_set==NULL) {
            result.nonexisting_urls_set.hash_table_rpc_ptr_len=0;
            result.nonexisting_urls_set.hash_table_rpc_ptr_val=NULL;
        } else {
            result.nonexisting_urls_set.hash_table_rpc_ptr_len=1;
            result.nonexisting_urls_set.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
            if(result.nonexisting_urls_set.hash_table_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.nonexisting_urls_set.hash_table_rpc_ptr_len;i0++) {
    if (nonexisting_urls_set[i0].cells == NULL) {result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}

    else {result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));

    	if (result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {perror("malloc failed");exit(1);
}
    	for (int i1 = 0; i1 < result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {

    if (nonexisting_urls_set[i0].cells->key == NULL) {result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = 0;result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = NULL;}

    else{result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = sizeof(nonexisting_urls_set[i0].cells->key);result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = malloc(result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, nonexisting_urls_set[i0].cells->key, result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);}

    if (nonexisting_urls_set[i0].cells->value == NULL) {result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = 0;result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = NULL;}

    else{result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = sizeof(nonexisting_urls_set[i0].cells->value);result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = malloc(result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, nonexisting_urls_set[i0].cells->value, result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);}

    	}
    }
    result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].size=nonexisting_urls_set[i0].size;
    result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].count=nonexisting_urls_set[i0].count;
    result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].resize_threshold=nonexisting_urls_set[i0].resize_threshold;
    result.nonexisting_urls_set.hash_table_rpc_ptr_val[i0].prime_offset=nonexisting_urls_set[i0].prime_offset;
            }
        }
    return &result;
}

void *set_nonexisting_urls_set_1_svc(hash_table_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct hash_table *local_nonexisting_urls_set=NULL;
    if(arg1.hash_table_rpc_ptr_val!=NULL) {
    local_nonexisting_urls_set=(struct hash_table *)malloc(arg1.hash_table_rpc_ptr_len*sizeof(struct hash_table));
    if(local_nonexisting_urls_set==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.hash_table_rpc_ptr_len;i0++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {local_nonexisting_urls_set[i0].cells=NULL;}
    else {local_nonexisting_urls_set[i0].cells=(struct cell *)malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(struct cell));
    	if (local_nonexisting_urls_set[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {local_nonexisting_urls_set[i0].cells[i1].key=NULL;}

    else {local_nonexisting_urls_set[i0].cells[i1].key=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (local_nonexisting_urls_set[i0].cells[i1].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_nonexisting_urls_set[i0].cells[i1].key,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    }

    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {local_nonexisting_urls_set[i0].cells[i1].value=NULL;}

    else {local_nonexisting_urls_set[i0].cells[i1].value=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (local_nonexisting_urls_set[i0].cells[i1].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_nonexisting_urls_set[i0].cells[i1].value,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    }

    	}
    }
    local_nonexisting_urls_set[i0].size=arg1.hash_table_rpc_ptr_val[i0].size;
    local_nonexisting_urls_set[i0].count=arg1.hash_table_rpc_ptr_val[i0].count;
    local_nonexisting_urls_set[i0].resize_threshold=arg1.hash_table_rpc_ptr_val[i0].resize_threshold;
    local_nonexisting_urls_set[i0].prime_offset=arg1.hash_table_rpc_ptr_val[i0].prime_offset;
    	}
    }
    memcpy(&nonexisting_urls_set, &local_nonexisting_urls_set, sizeof(hash_table));
    return (void *)&result;
}

char_ptr *get_nonexisting_urls_set_hash_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (nonexisting_urls_set.hash_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_nonexisting_urls_set_hash_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        nonexisting_urls_set.hash_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        nonexisting_urls_set.hash_function = fun_ptr;
    }
    return (void *)&result;
}

char_ptr *get_nonexisting_urls_set_test_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (nonexisting_urls_set.test_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_nonexisting_urls_set_test_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        nonexisting_urls_set.test_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        nonexisting_urls_set.test_function = fun_ptr;
    }
    return (void *)&result;
}

nonexisting_urls_set_cells_ret_t *get_nonexisting_urls_set_cells_1_svc(struct svc_req *rqstp)
{
    static nonexisting_urls_set_cells_ret_t result;
        if(nonexisting_urls_set_cells==NULL) {
            result.nonexisting_urls_set_cells.cell_rpc_ptr_len=0;
            result.nonexisting_urls_set_cells.cell_rpc_ptr_val=NULL;
        } else {
            result.nonexisting_urls_set_cells.cell_rpc_ptr_len=1;
            result.nonexisting_urls_set_cells.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
            if(result.nonexisting_urls_set_cells.cell_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.nonexisting_urls_set_cells.cell_rpc_ptr_len;i0++) {
    if (nonexisting_urls_set_cells[i0].key == NULL) {result.nonexisting_urls_set_cells.cell_rpc_ptr_val[i0].key.key_len = 0;result.nonexisting_urls_set_cells.cell_rpc_ptr_val[i0].key.key_val = NULL;}

    else{result.nonexisting_urls_set_cells.cell_rpc_ptr_val[i0].key.key_len = sizeof(nonexisting_urls_set_cells[i0].key);result.nonexisting_urls_set_cells.cell_rpc_ptr_val[i0].key.key_val = malloc(result.nonexisting_urls_set_cells.cell_rpc_ptr_val[i0].key.key_len);

    	if (result.nonexisting_urls_set_cells.cell_rpc_ptr_val[i0].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.nonexisting_urls_set_cells.cell_rpc_ptr_val[i0].key.key_val, nonexisting_urls_set_cells[i0].key, result.nonexisting_urls_set_cells.cell_rpc_ptr_val[i0].key.key_len);}

    if (nonexisting_urls_set_cells[i0].value == NULL) {result.nonexisting_urls_set_cells.cell_rpc_ptr_val[i0].value.value_len = 0;result.nonexisting_urls_set_cells.cell_rpc_ptr_val[i0].value.value_val = NULL;}

    else{result.nonexisting_urls_set_cells.cell_rpc_ptr_val[i0].value.value_len = sizeof(nonexisting_urls_set_cells[i0].value);result.nonexisting_urls_set_cells.cell_rpc_ptr_val[i0].value.value_val = malloc(result.nonexisting_urls_set_cells.cell_rpc_ptr_val[i0].value.value_len);

    	if (result.nonexisting_urls_set_cells.cell_rpc_ptr_val[i0].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.nonexisting_urls_set_cells.cell_rpc_ptr_val[i0].value.value_val, nonexisting_urls_set_cells[i0].value, result.nonexisting_urls_set_cells.cell_rpc_ptr_val[i0].value.value_len);}

            }
        }
    return &result;
}

void *set_nonexisting_urls_set_cells_1_svc(cell_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct cell *local_nonexisting_urls_set_cells=NULL;
    if(arg1.cell_rpc_ptr_val!=NULL) {
    local_nonexisting_urls_set_cells=(struct cell *)malloc(arg1.cell_rpc_ptr_len*sizeof(struct cell));
    if(local_nonexisting_urls_set_cells==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.cell_rpc_ptr_len;i0++) {
    if (arg1.cell_rpc_ptr_val[i0].key.key_val == NULL) {local_nonexisting_urls_set_cells[i0].key=NULL;}

    else {local_nonexisting_urls_set_cells[i0].key=malloc(arg1.cell_rpc_ptr_val[i0].key.key_len);

    	if (local_nonexisting_urls_set_cells[i0].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_nonexisting_urls_set_cells[i0].key,arg1.cell_rpc_ptr_val[i0].key.key_val,arg1.cell_rpc_ptr_val[i0].key.key_len);

    }

    if (arg1.cell_rpc_ptr_val[i0].value.value_val == NULL) {local_nonexisting_urls_set_cells[i0].value=NULL;}

    else {local_nonexisting_urls_set_cells[i0].value=malloc(arg1.cell_rpc_ptr_val[i0].value.value_len);

    	if (local_nonexisting_urls_set_cells[i0].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_nonexisting_urls_set_cells[i0].value,arg1.cell_rpc_ptr_val[i0].value.value_val,arg1.cell_rpc_ptr_val[i0].value.value_len);

    }

    	}
    }
    memcpy(&nonexisting_urls_set_cells, &local_nonexisting_urls_set_cells, sizeof(cell));
    return (void *)&result;
}

nonexisting_urls_set_cells_key_ret_t *get_nonexisting_urls_set_cells_key_1_svc(struct svc_req *rqstp)
{
    static nonexisting_urls_set_cells_key_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 nonexisting_urls_set_cells.key 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t nonexisting_urls_set_cells_key_size; // 假设有这样的辅助变量
    if (nonexisting_urls_set_cells.key == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = nonexisting_urls_set_cells_key_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = nonexisting_urls_set_cells.key;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t nonexisting_urls_set_cells_key_size; // 假设有这样的辅助变量
void *set_nonexisting_urls_set_cells_key_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (nonexisting_urls_set_cells.key != NULL) {
        free(nonexisting_urls_set_cells.key);
        nonexisting_urls_set_cells.key = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        nonexisting_urls_set_cells.key = malloc(value.bytes.bytes_len);
        if (nonexisting_urls_set_cells.key == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(nonexisting_urls_set_cells.key, value.bytes.bytes_val, value.bytes.bytes_len);
            nonexisting_urls_set_cells_key_size = value.size;
        }
    }
    return (void *)&result;
}

nonexisting_urls_set_cells_value_ret_t *get_nonexisting_urls_set_cells_value_1_svc(struct svc_req *rqstp)
{
    static nonexisting_urls_set_cells_value_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 nonexisting_urls_set_cells.value 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t nonexisting_urls_set_cells_value_size; // 假设有这样的辅助变量
    if (nonexisting_urls_set_cells.value == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = nonexisting_urls_set_cells_value_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = nonexisting_urls_set_cells.value;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t nonexisting_urls_set_cells_value_size; // 假设有这样的辅助变量
void *set_nonexisting_urls_set_cells_value_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (nonexisting_urls_set_cells.value != NULL) {
        free(nonexisting_urls_set_cells.value);
        nonexisting_urls_set_cells.value = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        nonexisting_urls_set_cells.value = malloc(value.bytes.bytes_len);
        if (nonexisting_urls_set_cells.value == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(nonexisting_urls_set_cells.value, value.bytes.bytes_val, value.bytes.bytes_len);
            nonexisting_urls_set_cells_value_size = value.size;
        }
    }
    return (void *)&result;
}

nonexisting_urls_set_size_ret_t *get_nonexisting_urls_set_size_1_svc(struct svc_req *rqstp)
{
    static nonexisting_urls_set_size_ret_t result;
    result.value = nonexisting_urls_set.size;
    return &result;
}

void *set_nonexisting_urls_set_size_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    nonexisting_urls_set.size = value;
    return (void *)&result;
}

nonexisting_urls_set_count_ret_t *get_nonexisting_urls_set_count_1_svc(struct svc_req *rqstp)
{
    static nonexisting_urls_set_count_ret_t result;
    result.value = nonexisting_urls_set.count;
    return &result;
}

void *set_nonexisting_urls_set_count_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    nonexisting_urls_set.count = value;
    return (void *)&result;
}

nonexisting_urls_set_resize_threshold_ret_t *get_nonexisting_urls_set_resize_threshold_1_svc(struct svc_req *rqstp)
{
    static nonexisting_urls_set_resize_threshold_ret_t result;
    result.value = nonexisting_urls_set.resize_threshold;
    return &result;
}

void *set_nonexisting_urls_set_resize_threshold_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    nonexisting_urls_set.resize_threshold = value;
    return (void *)&result;
}

nonexisting_urls_set_prime_offset_ret_t *get_nonexisting_urls_set_prime_offset_1_svc(struct svc_req *rqstp)
{
    static nonexisting_urls_set_prime_offset_ret_t result;
    result.value = nonexisting_urls_set.prime_offset;
    return &result;
}

void *set_nonexisting_urls_set_prime_offset_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    nonexisting_urls_set.prime_offset = value;
    return (void *)&result;
}

interesting_attributes_ret_t *get_interesting_attributes_1_svc(struct svc_req *rqstp)
{
    static interesting_attributes_ret_t result;
        if(interesting_attributes==NULL) {
            result.interesting_attributes.hash_table_rpc_ptr_len=0;
            result.interesting_attributes.hash_table_rpc_ptr_val=NULL;
        } else {
            result.interesting_attributes.hash_table_rpc_ptr_len=1;
            result.interesting_attributes.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
            if(result.interesting_attributes.hash_table_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.interesting_attributes.hash_table_rpc_ptr_len;i0++) {
    if (interesting_attributes[i0].cells == NULL) {result.interesting_attributes.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;result.interesting_attributes.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}

    else {result.interesting_attributes.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;result.interesting_attributes.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(result.interesting_attributes.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));

    	if (result.interesting_attributes.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {perror("malloc failed");exit(1);
}
    	for (int i1 = 0; i1 < result.interesting_attributes.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {

    if (interesting_attributes[i0].cells->key == NULL) {result.interesting_attributes.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = 0;result.interesting_attributes.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = NULL;}

    else{result.interesting_attributes.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = sizeof(interesting_attributes[i0].cells->key);result.interesting_attributes.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = malloc(result.interesting_attributes.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (result.interesting_attributes.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.interesting_attributes.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, interesting_attributes[i0].cells->key, result.interesting_attributes.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);}

    if (interesting_attributes[i0].cells->value == NULL) {result.interesting_attributes.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = 0;result.interesting_attributes.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = NULL;}

    else{result.interesting_attributes.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = sizeof(interesting_attributes[i0].cells->value);result.interesting_attributes.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = malloc(result.interesting_attributes.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (result.interesting_attributes.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.interesting_attributes.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, interesting_attributes[i0].cells->value, result.interesting_attributes.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);}

    	}
    }
    result.interesting_attributes.hash_table_rpc_ptr_val[i0].size=interesting_attributes[i0].size;
    result.interesting_attributes.hash_table_rpc_ptr_val[i0].count=interesting_attributes[i0].count;
    result.interesting_attributes.hash_table_rpc_ptr_val[i0].resize_threshold=interesting_attributes[i0].resize_threshold;
    result.interesting_attributes.hash_table_rpc_ptr_val[i0].prime_offset=interesting_attributes[i0].prime_offset;
            }
        }
    return &result;
}

void *set_interesting_attributes_1_svc(hash_table_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct hash_table *local_interesting_attributes=NULL;
    if(arg1.hash_table_rpc_ptr_val!=NULL) {
    local_interesting_attributes=(struct hash_table *)malloc(arg1.hash_table_rpc_ptr_len*sizeof(struct hash_table));
    if(local_interesting_attributes==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.hash_table_rpc_ptr_len;i0++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {local_interesting_attributes[i0].cells=NULL;}
    else {local_interesting_attributes[i0].cells=(struct cell *)malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(struct cell));
    	if (local_interesting_attributes[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {local_interesting_attributes[i0].cells[i1].key=NULL;}

    else {local_interesting_attributes[i0].cells[i1].key=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (local_interesting_attributes[i0].cells[i1].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_interesting_attributes[i0].cells[i1].key,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    }

    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {local_interesting_attributes[i0].cells[i1].value=NULL;}

    else {local_interesting_attributes[i0].cells[i1].value=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (local_interesting_attributes[i0].cells[i1].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_interesting_attributes[i0].cells[i1].value,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    }

    	}
    }
    local_interesting_attributes[i0].size=arg1.hash_table_rpc_ptr_val[i0].size;
    local_interesting_attributes[i0].count=arg1.hash_table_rpc_ptr_val[i0].count;
    local_interesting_attributes[i0].resize_threshold=arg1.hash_table_rpc_ptr_val[i0].resize_threshold;
    local_interesting_attributes[i0].prime_offset=arg1.hash_table_rpc_ptr_val[i0].prime_offset;
    	}
    }
    memcpy(&interesting_attributes, &local_interesting_attributes, sizeof(hash_table));
    return (void *)&result;
}

char_ptr *get_interesting_attributes_hash_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (interesting_attributes.hash_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_interesting_attributes_hash_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        interesting_attributes.hash_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        interesting_attributes.hash_function = fun_ptr;
    }
    return (void *)&result;
}

char_ptr *get_interesting_attributes_test_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (interesting_attributes.test_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_interesting_attributes_test_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        interesting_attributes.test_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        interesting_attributes.test_function = fun_ptr;
    }
    return (void *)&result;
}

interesting_attributes_cells_ret_t *get_interesting_attributes_cells_1_svc(struct svc_req *rqstp)
{
    static interesting_attributes_cells_ret_t result;
        if(interesting_attributes_cells==NULL) {
            result.interesting_attributes_cells.cell_rpc_ptr_len=0;
            result.interesting_attributes_cells.cell_rpc_ptr_val=NULL;
        } else {
            result.interesting_attributes_cells.cell_rpc_ptr_len=1;
            result.interesting_attributes_cells.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
            if(result.interesting_attributes_cells.cell_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.interesting_attributes_cells.cell_rpc_ptr_len;i0++) {
    if (interesting_attributes_cells[i0].key == NULL) {result.interesting_attributes_cells.cell_rpc_ptr_val[i0].key.key_len = 0;result.interesting_attributes_cells.cell_rpc_ptr_val[i0].key.key_val = NULL;}

    else{result.interesting_attributes_cells.cell_rpc_ptr_val[i0].key.key_len = sizeof(interesting_attributes_cells[i0].key);result.interesting_attributes_cells.cell_rpc_ptr_val[i0].key.key_val = malloc(result.interesting_attributes_cells.cell_rpc_ptr_val[i0].key.key_len);

    	if (result.interesting_attributes_cells.cell_rpc_ptr_val[i0].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.interesting_attributes_cells.cell_rpc_ptr_val[i0].key.key_val, interesting_attributes_cells[i0].key, result.interesting_attributes_cells.cell_rpc_ptr_val[i0].key.key_len);}

    if (interesting_attributes_cells[i0].value == NULL) {result.interesting_attributes_cells.cell_rpc_ptr_val[i0].value.value_len = 0;result.interesting_attributes_cells.cell_rpc_ptr_val[i0].value.value_val = NULL;}

    else{result.interesting_attributes_cells.cell_rpc_ptr_val[i0].value.value_len = sizeof(interesting_attributes_cells[i0].value);result.interesting_attributes_cells.cell_rpc_ptr_val[i0].value.value_val = malloc(result.interesting_attributes_cells.cell_rpc_ptr_val[i0].value.value_len);

    	if (result.interesting_attributes_cells.cell_rpc_ptr_val[i0].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.interesting_attributes_cells.cell_rpc_ptr_val[i0].value.value_val, interesting_attributes_cells[i0].value, result.interesting_attributes_cells.cell_rpc_ptr_val[i0].value.value_len);}

            }
        }
    return &result;
}

void *set_interesting_attributes_cells_1_svc(cell_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct cell *local_interesting_attributes_cells=NULL;
    if(arg1.cell_rpc_ptr_val!=NULL) {
    local_interesting_attributes_cells=(struct cell *)malloc(arg1.cell_rpc_ptr_len*sizeof(struct cell));
    if(local_interesting_attributes_cells==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.cell_rpc_ptr_len;i0++) {
    if (arg1.cell_rpc_ptr_val[i0].key.key_val == NULL) {local_interesting_attributes_cells[i0].key=NULL;}

    else {local_interesting_attributes_cells[i0].key=malloc(arg1.cell_rpc_ptr_val[i0].key.key_len);

    	if (local_interesting_attributes_cells[i0].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_interesting_attributes_cells[i0].key,arg1.cell_rpc_ptr_val[i0].key.key_val,arg1.cell_rpc_ptr_val[i0].key.key_len);

    }

    if (arg1.cell_rpc_ptr_val[i0].value.value_val == NULL) {local_interesting_attributes_cells[i0].value=NULL;}

    else {local_interesting_attributes_cells[i0].value=malloc(arg1.cell_rpc_ptr_val[i0].value.value_len);

    	if (local_interesting_attributes_cells[i0].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_interesting_attributes_cells[i0].value,arg1.cell_rpc_ptr_val[i0].value.value_val,arg1.cell_rpc_ptr_val[i0].value.value_len);

    }

    	}
    }
    memcpy(&interesting_attributes_cells, &local_interesting_attributes_cells, sizeof(cell));
    return (void *)&result;
}

interesting_attributes_cells_key_ret_t *get_interesting_attributes_cells_key_1_svc(struct svc_req *rqstp)
{
    static interesting_attributes_cells_key_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 interesting_attributes_cells.key 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t interesting_attributes_cells_key_size; // 假设有这样的辅助变量
    if (interesting_attributes_cells.key == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = interesting_attributes_cells_key_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = interesting_attributes_cells.key;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t interesting_attributes_cells_key_size; // 假设有这样的辅助变量
void *set_interesting_attributes_cells_key_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (interesting_attributes_cells.key != NULL) {
        free(interesting_attributes_cells.key);
        interesting_attributes_cells.key = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        interesting_attributes_cells.key = malloc(value.bytes.bytes_len);
        if (interesting_attributes_cells.key == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(interesting_attributes_cells.key, value.bytes.bytes_val, value.bytes.bytes_len);
            interesting_attributes_cells_key_size = value.size;
        }
    }
    return (void *)&result;
}

interesting_attributes_cells_value_ret_t *get_interesting_attributes_cells_value_1_svc(struct svc_req *rqstp)
{
    static interesting_attributes_cells_value_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 interesting_attributes_cells.value 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t interesting_attributes_cells_value_size; // 假设有这样的辅助变量
    if (interesting_attributes_cells.value == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = interesting_attributes_cells_value_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = interesting_attributes_cells.value;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t interesting_attributes_cells_value_size; // 假设有这样的辅助变量
void *set_interesting_attributes_cells_value_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (interesting_attributes_cells.value != NULL) {
        free(interesting_attributes_cells.value);
        interesting_attributes_cells.value = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        interesting_attributes_cells.value = malloc(value.bytes.bytes_len);
        if (interesting_attributes_cells.value == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(interesting_attributes_cells.value, value.bytes.bytes_val, value.bytes.bytes_len);
            interesting_attributes_cells_value_size = value.size;
        }
    }
    return (void *)&result;
}

interesting_attributes_size_ret_t *get_interesting_attributes_size_1_svc(struct svc_req *rqstp)
{
    static interesting_attributes_size_ret_t result;
    result.value = interesting_attributes.size;
    return &result;
}

void *set_interesting_attributes_size_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    interesting_attributes.size = value;
    return (void *)&result;
}

interesting_attributes_count_ret_t *get_interesting_attributes_count_1_svc(struct svc_req *rqstp)
{
    static interesting_attributes_count_ret_t result;
    result.value = interesting_attributes.count;
    return &result;
}

void *set_interesting_attributes_count_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    interesting_attributes.count = value;
    return (void *)&result;
}

interesting_attributes_resize_threshold_ret_t *get_interesting_attributes_resize_threshold_1_svc(struct svc_req *rqstp)
{
    static interesting_attributes_resize_threshold_ret_t result;
    result.value = interesting_attributes.resize_threshold;
    return &result;
}

void *set_interesting_attributes_resize_threshold_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    interesting_attributes.resize_threshold = value;
    return (void *)&result;
}

interesting_attributes_prime_offset_ret_t *get_interesting_attributes_prime_offset_1_svc(struct svc_req *rqstp)
{
    static interesting_attributes_prime_offset_ret_t result;
    result.value = interesting_attributes.prime_offset;
    return &result;
}

void *set_interesting_attributes_prime_offset_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    interesting_attributes.prime_offset = value;
    return (void *)&result;
}

pconn_ret_t *get_pconn_1_svc(struct svc_req *rqstp)
{
    static pconn_ret_t result;
    return &result;
}

void *set_pconn_1_svc(new_pconn_rpc value, struct svc_req *rqstp)
{
    static int result = 1;
    new_pconn_rpc arg1
    return (void *)&result;
}

pconn_socket_ret_t *get_pconn_socket_1_svc(struct svc_req *rqstp)
{
    static pconn_socket_ret_t result;
    result.value = pconn.socket;
    return &result;
}

void *set_pconn_socket_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    pconn.socket = value;
    return (void *)&result;
}

pconn_host_ret_t *get_pconn_host_1_svc(struct svc_req *rqstp)
{
    static pconn_host_ret_t result;
    if (pconn.host != NULL) {
        result.value = strdup(pconn.host);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_pconn_host_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (pconn.host != NULL) {
        free(pconn.host);
    }
    if (value != NULL) {
        pconn.host = strdup(value);
    } else {
        pconn.host = NULL;
    }
    return (void *)&result;
}

pconn_port_ret_t *get_pconn_port_1_svc(struct svc_req *rqstp)
{
    static pconn_port_ret_t result;
    result.value = pconn.port;
    return &result;
}

void *set_pconn_port_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    pconn.port = value;
    return (void *)&result;
}

pconn_ssl_ret_t *get_pconn_ssl_1_svc(struct svc_req *rqstp)
{
    static pconn_ssl_ret_t result;
    result.value = pconn.ssl;
    return &result;
}

void *set_pconn_ssl_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    pconn.ssl = value;
    return (void *)&result;
}

pconn_authorized_ret_t *get_pconn_authorized_1_svc(struct svc_req *rqstp)
{
    static pconn_authorized_ret_t result;
    result.value = pconn.authorized;
    return &result;
}

void *set_pconn_authorized_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    pconn.authorized = value;
    return (void *)&result;
}

pconn_ntlm_ret_t *get_pconn_ntlm_1_svc(struct svc_req *rqstp)
{
    static pconn_ntlm_ret_t result;
    return &result;
}

void *set_pconn_ntlm_1_svc(ntlmdata_rpc value, struct svc_req *rqstp)
{
    static int result = 1;
    ntlmdata_rpc arg1
    return (void *)&result;
}

pconn_ntlm_state_ret_t *get_pconn_ntlm_state_1_svc(struct svc_req *rqstp)
{
    static pconn_ntlm_state_ret_t result;
    result.value = pconn_ntlm.state;
    return &result;
}

void *set_pconn_ntlm_state_1_svc(wgetntlm value, struct svc_req *rqstp)
{
    static int result = 1;
    pconn_ntlm.state = value;
    return (void *)&result;
}

host_name_addresses_map_ret_t *get_host_name_addresses_map_1_svc(struct svc_req *rqstp)
{
    static host_name_addresses_map_ret_t result;
        if(host_name_addresses_map==NULL) {
            result.host_name_addresses_map.hash_table_rpc_ptr_len=0;
            result.host_name_addresses_map.hash_table_rpc_ptr_val=NULL;
        } else {
            result.host_name_addresses_map.hash_table_rpc_ptr_len=1;
            result.host_name_addresses_map.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
            if(result.host_name_addresses_map.hash_table_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.host_name_addresses_map.hash_table_rpc_ptr_len;i0++) {
    if (host_name_addresses_map[i0].cells == NULL) {result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}

    else {result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));

    	if (result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {perror("malloc failed");exit(1);
}
    	for (int i1 = 0; i1 < result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {

    if (host_name_addresses_map[i0].cells->key == NULL) {result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = 0;result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = NULL;}

    else{result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = sizeof(host_name_addresses_map[i0].cells->key);result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = malloc(result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, host_name_addresses_map[i0].cells->key, result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);}

    if (host_name_addresses_map[i0].cells->value == NULL) {result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = 0;result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = NULL;}

    else{result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = sizeof(host_name_addresses_map[i0].cells->value);result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = malloc(result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, host_name_addresses_map[i0].cells->value, result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);}

    	}
    }
    result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].size=host_name_addresses_map[i0].size;
    result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].count=host_name_addresses_map[i0].count;
    result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].resize_threshold=host_name_addresses_map[i0].resize_threshold;
    result.host_name_addresses_map.hash_table_rpc_ptr_val[i0].prime_offset=host_name_addresses_map[i0].prime_offset;
            }
        }
    return &result;
}

void *set_host_name_addresses_map_1_svc(hash_table_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct hash_table *local_host_name_addresses_map=NULL;
    if(arg1.hash_table_rpc_ptr_val!=NULL) {
    local_host_name_addresses_map=(struct hash_table *)malloc(arg1.hash_table_rpc_ptr_len*sizeof(struct hash_table));
    if(local_host_name_addresses_map==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.hash_table_rpc_ptr_len;i0++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {local_host_name_addresses_map[i0].cells=NULL;}
    else {local_host_name_addresses_map[i0].cells=(struct cell *)malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(struct cell));
    	if (local_host_name_addresses_map[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {local_host_name_addresses_map[i0].cells[i1].key=NULL;}

    else {local_host_name_addresses_map[i0].cells[i1].key=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (local_host_name_addresses_map[i0].cells[i1].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_host_name_addresses_map[i0].cells[i1].key,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    }

    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {local_host_name_addresses_map[i0].cells[i1].value=NULL;}

    else {local_host_name_addresses_map[i0].cells[i1].value=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (local_host_name_addresses_map[i0].cells[i1].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_host_name_addresses_map[i0].cells[i1].value,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    }

    	}
    }
    local_host_name_addresses_map[i0].size=arg1.hash_table_rpc_ptr_val[i0].size;
    local_host_name_addresses_map[i0].count=arg1.hash_table_rpc_ptr_val[i0].count;
    local_host_name_addresses_map[i0].resize_threshold=arg1.hash_table_rpc_ptr_val[i0].resize_threshold;
    local_host_name_addresses_map[i0].prime_offset=arg1.hash_table_rpc_ptr_val[i0].prime_offset;
    	}
    }
    memcpy(&host_name_addresses_map, &local_host_name_addresses_map, sizeof(hash_table));
    return (void *)&result;
}

char_ptr *get_host_name_addresses_map_hash_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (host_name_addresses_map.hash_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_host_name_addresses_map_hash_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        host_name_addresses_map.hash_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        host_name_addresses_map.hash_function = fun_ptr;
    }
    return (void *)&result;
}

char_ptr *get_host_name_addresses_map_test_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (host_name_addresses_map.test_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_host_name_addresses_map_test_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        host_name_addresses_map.test_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        host_name_addresses_map.test_function = fun_ptr;
    }
    return (void *)&result;
}

host_name_addresses_map_cells_ret_t *get_host_name_addresses_map_cells_1_svc(struct svc_req *rqstp)
{
    static host_name_addresses_map_cells_ret_t result;
        if(host_name_addresses_map_cells==NULL) {
            result.host_name_addresses_map_cells.cell_rpc_ptr_len=0;
            result.host_name_addresses_map_cells.cell_rpc_ptr_val=NULL;
        } else {
            result.host_name_addresses_map_cells.cell_rpc_ptr_len=1;
            result.host_name_addresses_map_cells.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
            if(result.host_name_addresses_map_cells.cell_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.host_name_addresses_map_cells.cell_rpc_ptr_len;i0++) {
    if (host_name_addresses_map_cells[i0].key == NULL) {result.host_name_addresses_map_cells.cell_rpc_ptr_val[i0].key.key_len = 0;result.host_name_addresses_map_cells.cell_rpc_ptr_val[i0].key.key_val = NULL;}

    else{result.host_name_addresses_map_cells.cell_rpc_ptr_val[i0].key.key_len = sizeof(host_name_addresses_map_cells[i0].key);result.host_name_addresses_map_cells.cell_rpc_ptr_val[i0].key.key_val = malloc(result.host_name_addresses_map_cells.cell_rpc_ptr_val[i0].key.key_len);

    	if (result.host_name_addresses_map_cells.cell_rpc_ptr_val[i0].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.host_name_addresses_map_cells.cell_rpc_ptr_val[i0].key.key_val, host_name_addresses_map_cells[i0].key, result.host_name_addresses_map_cells.cell_rpc_ptr_val[i0].key.key_len);}

    if (host_name_addresses_map_cells[i0].value == NULL) {result.host_name_addresses_map_cells.cell_rpc_ptr_val[i0].value.value_len = 0;result.host_name_addresses_map_cells.cell_rpc_ptr_val[i0].value.value_val = NULL;}

    else{result.host_name_addresses_map_cells.cell_rpc_ptr_val[i0].value.value_len = sizeof(host_name_addresses_map_cells[i0].value);result.host_name_addresses_map_cells.cell_rpc_ptr_val[i0].value.value_val = malloc(result.host_name_addresses_map_cells.cell_rpc_ptr_val[i0].value.value_len);

    	if (result.host_name_addresses_map_cells.cell_rpc_ptr_val[i0].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.host_name_addresses_map_cells.cell_rpc_ptr_val[i0].value.value_val, host_name_addresses_map_cells[i0].value, result.host_name_addresses_map_cells.cell_rpc_ptr_val[i0].value.value_len);}

            }
        }
    return &result;
}

void *set_host_name_addresses_map_cells_1_svc(cell_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct cell *local_host_name_addresses_map_cells=NULL;
    if(arg1.cell_rpc_ptr_val!=NULL) {
    local_host_name_addresses_map_cells=(struct cell *)malloc(arg1.cell_rpc_ptr_len*sizeof(struct cell));
    if(local_host_name_addresses_map_cells==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.cell_rpc_ptr_len;i0++) {
    if (arg1.cell_rpc_ptr_val[i0].key.key_val == NULL) {local_host_name_addresses_map_cells[i0].key=NULL;}

    else {local_host_name_addresses_map_cells[i0].key=malloc(arg1.cell_rpc_ptr_val[i0].key.key_len);

    	if (local_host_name_addresses_map_cells[i0].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_host_name_addresses_map_cells[i0].key,arg1.cell_rpc_ptr_val[i0].key.key_val,arg1.cell_rpc_ptr_val[i0].key.key_len);

    }

    if (arg1.cell_rpc_ptr_val[i0].value.value_val == NULL) {local_host_name_addresses_map_cells[i0].value=NULL;}

    else {local_host_name_addresses_map_cells[i0].value=malloc(arg1.cell_rpc_ptr_val[i0].value.value_len);

    	if (local_host_name_addresses_map_cells[i0].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_host_name_addresses_map_cells[i0].value,arg1.cell_rpc_ptr_val[i0].value.value_val,arg1.cell_rpc_ptr_val[i0].value.value_len);

    }

    	}
    }
    memcpy(&host_name_addresses_map_cells, &local_host_name_addresses_map_cells, sizeof(cell));
    return (void *)&result;
}

host_name_addresses_map_cells_key_ret_t *get_host_name_addresses_map_cells_key_1_svc(struct svc_req *rqstp)
{
    static host_name_addresses_map_cells_key_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 host_name_addresses_map_cells.key 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t host_name_addresses_map_cells_key_size; // 假设有这样的辅助变量
    if (host_name_addresses_map_cells.key == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = host_name_addresses_map_cells_key_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = host_name_addresses_map_cells.key;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t host_name_addresses_map_cells_key_size; // 假设有这样的辅助变量
void *set_host_name_addresses_map_cells_key_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (host_name_addresses_map_cells.key != NULL) {
        free(host_name_addresses_map_cells.key);
        host_name_addresses_map_cells.key = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        host_name_addresses_map_cells.key = malloc(value.bytes.bytes_len);
        if (host_name_addresses_map_cells.key == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(host_name_addresses_map_cells.key, value.bytes.bytes_val, value.bytes.bytes_len);
            host_name_addresses_map_cells_key_size = value.size;
        }
    }
    return (void *)&result;
}

host_name_addresses_map_cells_value_ret_t *get_host_name_addresses_map_cells_value_1_svc(struct svc_req *rqstp)
{
    static host_name_addresses_map_cells_value_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 host_name_addresses_map_cells.value 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t host_name_addresses_map_cells_value_size; // 假设有这样的辅助变量
    if (host_name_addresses_map_cells.value == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = host_name_addresses_map_cells_value_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = host_name_addresses_map_cells.value;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t host_name_addresses_map_cells_value_size; // 假设有这样的辅助变量
void *set_host_name_addresses_map_cells_value_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (host_name_addresses_map_cells.value != NULL) {
        free(host_name_addresses_map_cells.value);
        host_name_addresses_map_cells.value = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        host_name_addresses_map_cells.value = malloc(value.bytes.bytes_len);
        if (host_name_addresses_map_cells.value == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(host_name_addresses_map_cells.value, value.bytes.bytes_val, value.bytes.bytes_len);
            host_name_addresses_map_cells_value_size = value.size;
        }
    }
    return (void *)&result;
}

host_name_addresses_map_size_ret_t *get_host_name_addresses_map_size_1_svc(struct svc_req *rqstp)
{
    static host_name_addresses_map_size_ret_t result;
    result.value = host_name_addresses_map.size;
    return &result;
}

void *set_host_name_addresses_map_size_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    host_name_addresses_map.size = value;
    return (void *)&result;
}

host_name_addresses_map_count_ret_t *get_host_name_addresses_map_count_1_svc(struct svc_req *rqstp)
{
    static host_name_addresses_map_count_ret_t result;
    result.value = host_name_addresses_map.count;
    return &result;
}

void *set_host_name_addresses_map_count_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    host_name_addresses_map.count = value;
    return (void *)&result;
}

host_name_addresses_map_resize_threshold_ret_t *get_host_name_addresses_map_resize_threshold_1_svc(struct svc_req *rqstp)
{
    static host_name_addresses_map_resize_threshold_ret_t result;
    result.value = host_name_addresses_map.resize_threshold;
    return &result;
}

void *set_host_name_addresses_map_resize_threshold_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    host_name_addresses_map.resize_threshold = value;
    return (void *)&result;
}

host_name_addresses_map_prime_offset_ret_t *get_host_name_addresses_map_prime_offset_1_svc(struct svc_req *rqstp)
{
    static host_name_addresses_map_prime_offset_ret_t result;
    result.value = host_name_addresses_map.prime_offset;
    return &result;
}

void *set_host_name_addresses_map_prime_offset_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    host_name_addresses_map.prime_offset = value;
    return (void *)&result;
}

limit_data_ret_t *get_limit_data_1_svc(struct svc_req *rqstp)
{
    static limit_data_ret_t result;
    return &result;
}

void *set_limit_data_1_svc(new_limit_data_rpc value, struct svc_req *rqstp)
{
    static int result = 1;
    new_limit_data_rpc arg1
    return (void *)&result;
}

limit_data_chunk_bytes_ret_t *get_limit_data_chunk_bytes_1_svc(struct svc_req *rqstp)
{
    static limit_data_chunk_bytes_ret_t result;
    result.value = limit_data.chunk_bytes;
    return &result;
}

void *set_limit_data_chunk_bytes_1_svc(wgint value, struct svc_req *rqstp)
{
    static int result = 1;
    limit_data.chunk_bytes = value;
    return (void *)&result;
}

limit_data_chunk_start_ret_t *get_limit_data_chunk_start_1_svc(struct svc_req *rqstp)
{
    static limit_data_chunk_start_ret_t result;
    result.value = limit_data.chunk_start;
    return &result;
}

void *set_limit_data_chunk_start_1_svc(double value, struct svc_req *rqstp)
{
    static int result = 1;
    limit_data.chunk_start = value;
    return (void *)&result;
}

limit_data_sleep_adjust_ret_t *get_limit_data_sleep_adjust_1_svc(struct svc_req *rqstp)
{
    static limit_data_sleep_adjust_ret_t result;
    result.value = limit_data.sleep_adjust;
    return &result;
}

void *set_limit_data_sleep_adjust_1_svc(double value, struct svc_req *rqstp)
{
    static int result = 1;
    limit_data.sleep_adjust = value;
    return (void *)&result;
}

screen_width_ret_t *get_screen_width_1_svc(struct svc_req *rqstp)
{
    static screen_width_ret_t result;
    result.value = screen_width;
    return &result;
}

void *set_screen_width_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1; // 成功标志
    screen_width = value;
    return (void *)&result;
}

current_impl_locked_ret_t *get_current_impl_locked_1_svc(struct svc_req *rqstp)
{
    static current_impl_locked_ret_t result;
    result.value = current_impl_locked;
    return &result;
}

void *set_current_impl_locked_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1; // 成功标志
    current_impl_locked = value;
    return (void *)&result;
}

char_ptr *get_warc_current_cdx_file_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *filename=get_filename_from_fp(warc_current_cdx_file);
    if (filename == NULL) {
        result.charptr.charptr_len = 0;
        result.charptr.charptr_val = NULL;
    } else {
        result.charptr.charptr_len = strlen(filename) + 1;
        result.charptr.charptr_val = filename;
    }
    return &result;
}

            warc_current_cdx_file = stdin;
        } else if (strcmp(value.charptr.charptr_val, "stdout") == 0) {
            warc_current_cdx_file = stdout;
        } else if (strcmp(value.charptr.charptr_val, "stderr") == 0) {
            warc_current_cdx_file = stderr;
        } else {
            warc_current_cdx_file = fopen(value.charptr.charptr_val, "r+");
            if ({var_name} == NULL) {
                perror("fopen failed");
                return NULL;
            }
        }
void *set_warc_current_cdx_file_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result;
    if (value.charptr.charptr_val == NULL) {
        warc_current_cdx_file = NULL;
    } else {
        // 假设文件模式为读写
      if (strcmp(value.charptr.charptr_val, "stdin") == 0) {
j    }
    return (void *)&result;
}

char_ptr *get_warclogfp_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *filename=get_filename_from_fp(warclogfp);
    if (filename == NULL) {
        result.charptr.charptr_len = 0;
        result.charptr.charptr_val = NULL;
    } else {
        result.charptr.charptr_len = strlen(filename) + 1;
        result.charptr.charptr_val = filename;
    }
    return &result;
}

            warclogfp = stdin;
        } else if (strcmp(value.charptr.charptr_val, "stdout") == 0) {
            warclogfp = stdout;
        } else if (strcmp(value.charptr.charptr_val, "stderr") == 0) {
            warclogfp = stderr;
        } else {
            warclogfp = fopen(value.charptr.charptr_val, "r+");
            if ({var_name} == NULL) {
                perror("fopen failed");
                return NULL;
            }
        }
void *set_warclogfp_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result;
    if (value.charptr.charptr_val == NULL) {
        warclogfp = NULL;
    } else {
        // 假设文件模式为读写
      if (strcmp(value.charptr.charptr_val, "stdin") == 0) {
j    }
    return (void *)&result;
}

char_ptr *get_warc_manifest_fp_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *filename=get_filename_from_fp(warc_manifest_fp);
    if (filename == NULL) {
        result.charptr.charptr_len = 0;
        result.charptr.charptr_val = NULL;
    } else {
        result.charptr.charptr_len = strlen(filename) + 1;
        result.charptr.charptr_val = filename;
    }
    return &result;
}

            warc_manifest_fp = stdin;
        } else if (strcmp(value.charptr.charptr_val, "stdout") == 0) {
            warc_manifest_fp = stdout;
        } else if (strcmp(value.charptr.charptr_val, "stderr") == 0) {
            warc_manifest_fp = stderr;
        } else {
            warc_manifest_fp = fopen(value.charptr.charptr_val, "r+");
            if ({var_name} == NULL) {
                perror("fopen failed");
                return NULL;
            }
        }
void *set_warc_manifest_fp_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result;
    if (value.charptr.charptr_val == NULL) {
        warc_manifest_fp = NULL;
    } else {
        // 假设文件模式为读写
      if (strcmp(value.charptr.charptr_val, "stdin") == 0) {
j    }
    return (void *)&result;
}

downloaded_html_set_ret_t *get_downloaded_html_set_1_svc(struct svc_req *rqstp)
{
    static downloaded_html_set_ret_t result;
        if(downloaded_html_set==NULL) {
            result.downloaded_html_set.hash_table_rpc_ptr_len=0;
            result.downloaded_html_set.hash_table_rpc_ptr_val=NULL;
        } else {
            result.downloaded_html_set.hash_table_rpc_ptr_len=1;
            result.downloaded_html_set.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
            if(result.downloaded_html_set.hash_table_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.downloaded_html_set.hash_table_rpc_ptr_len;i0++) {
    if (downloaded_html_set[i0].cells == NULL) {result.downloaded_html_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;result.downloaded_html_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}

    else {result.downloaded_html_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;result.downloaded_html_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(result.downloaded_html_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));

    	if (result.downloaded_html_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {perror("malloc failed");exit(1);
}
    	for (int i1 = 0; i1 < result.downloaded_html_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {

    if (downloaded_html_set[i0].cells->key == NULL) {result.downloaded_html_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = 0;result.downloaded_html_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = NULL;}

    else{result.downloaded_html_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = sizeof(downloaded_html_set[i0].cells->key);result.downloaded_html_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = malloc(result.downloaded_html_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (result.downloaded_html_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.downloaded_html_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, downloaded_html_set[i0].cells->key, result.downloaded_html_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);}

    if (downloaded_html_set[i0].cells->value == NULL) {result.downloaded_html_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = 0;result.downloaded_html_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = NULL;}

    else{result.downloaded_html_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = sizeof(downloaded_html_set[i0].cells->value);result.downloaded_html_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = malloc(result.downloaded_html_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (result.downloaded_html_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.downloaded_html_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, downloaded_html_set[i0].cells->value, result.downloaded_html_set.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);}

    	}
    }
    result.downloaded_html_set.hash_table_rpc_ptr_val[i0].size=downloaded_html_set[i0].size;
    result.downloaded_html_set.hash_table_rpc_ptr_val[i0].count=downloaded_html_set[i0].count;
    result.downloaded_html_set.hash_table_rpc_ptr_val[i0].resize_threshold=downloaded_html_set[i0].resize_threshold;
    result.downloaded_html_set.hash_table_rpc_ptr_val[i0].prime_offset=downloaded_html_set[i0].prime_offset;
            }
        }
    return &result;
}

void *set_downloaded_html_set_1_svc(hash_table_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct hash_table *local_downloaded_html_set=NULL;
    if(arg1.hash_table_rpc_ptr_val!=NULL) {
    local_downloaded_html_set=(struct hash_table *)malloc(arg1.hash_table_rpc_ptr_len*sizeof(struct hash_table));
    if(local_downloaded_html_set==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.hash_table_rpc_ptr_len;i0++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {local_downloaded_html_set[i0].cells=NULL;}
    else {local_downloaded_html_set[i0].cells=(struct cell *)malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(struct cell));
    	if (local_downloaded_html_set[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {local_downloaded_html_set[i0].cells[i1].key=NULL;}

    else {local_downloaded_html_set[i0].cells[i1].key=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (local_downloaded_html_set[i0].cells[i1].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_downloaded_html_set[i0].cells[i1].key,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    }

    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {local_downloaded_html_set[i0].cells[i1].value=NULL;}

    else {local_downloaded_html_set[i0].cells[i1].value=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (local_downloaded_html_set[i0].cells[i1].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_downloaded_html_set[i0].cells[i1].value,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    }

    	}
    }
    local_downloaded_html_set[i0].size=arg1.hash_table_rpc_ptr_val[i0].size;
    local_downloaded_html_set[i0].count=arg1.hash_table_rpc_ptr_val[i0].count;
    local_downloaded_html_set[i0].resize_threshold=arg1.hash_table_rpc_ptr_val[i0].resize_threshold;
    local_downloaded_html_set[i0].prime_offset=arg1.hash_table_rpc_ptr_val[i0].prime_offset;
    	}
    }
    memcpy(&downloaded_html_set, &local_downloaded_html_set, sizeof(hash_table));
    return (void *)&result;
}

char_ptr *get_downloaded_html_set_hash_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (downloaded_html_set.hash_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_downloaded_html_set_hash_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        downloaded_html_set.hash_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        downloaded_html_set.hash_function = fun_ptr;
    }
    return (void *)&result;
}

char_ptr *get_downloaded_html_set_test_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (downloaded_html_set.test_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_downloaded_html_set_test_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        downloaded_html_set.test_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        downloaded_html_set.test_function = fun_ptr;
    }
    return (void *)&result;
}

downloaded_html_set_cells_ret_t *get_downloaded_html_set_cells_1_svc(struct svc_req *rqstp)
{
    static downloaded_html_set_cells_ret_t result;
        if(downloaded_html_set_cells==NULL) {
            result.downloaded_html_set_cells.cell_rpc_ptr_len=0;
            result.downloaded_html_set_cells.cell_rpc_ptr_val=NULL;
        } else {
            result.downloaded_html_set_cells.cell_rpc_ptr_len=1;
            result.downloaded_html_set_cells.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
            if(result.downloaded_html_set_cells.cell_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.downloaded_html_set_cells.cell_rpc_ptr_len;i0++) {
    if (downloaded_html_set_cells[i0].key == NULL) {result.downloaded_html_set_cells.cell_rpc_ptr_val[i0].key.key_len = 0;result.downloaded_html_set_cells.cell_rpc_ptr_val[i0].key.key_val = NULL;}

    else{result.downloaded_html_set_cells.cell_rpc_ptr_val[i0].key.key_len = sizeof(downloaded_html_set_cells[i0].key);result.downloaded_html_set_cells.cell_rpc_ptr_val[i0].key.key_val = malloc(result.downloaded_html_set_cells.cell_rpc_ptr_val[i0].key.key_len);

    	if (result.downloaded_html_set_cells.cell_rpc_ptr_val[i0].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.downloaded_html_set_cells.cell_rpc_ptr_val[i0].key.key_val, downloaded_html_set_cells[i0].key, result.downloaded_html_set_cells.cell_rpc_ptr_val[i0].key.key_len);}

    if (downloaded_html_set_cells[i0].value == NULL) {result.downloaded_html_set_cells.cell_rpc_ptr_val[i0].value.value_len = 0;result.downloaded_html_set_cells.cell_rpc_ptr_val[i0].value.value_val = NULL;}

    else{result.downloaded_html_set_cells.cell_rpc_ptr_val[i0].value.value_len = sizeof(downloaded_html_set_cells[i0].value);result.downloaded_html_set_cells.cell_rpc_ptr_val[i0].value.value_val = malloc(result.downloaded_html_set_cells.cell_rpc_ptr_val[i0].value.value_len);

    	if (result.downloaded_html_set_cells.cell_rpc_ptr_val[i0].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.downloaded_html_set_cells.cell_rpc_ptr_val[i0].value.value_val, downloaded_html_set_cells[i0].value, result.downloaded_html_set_cells.cell_rpc_ptr_val[i0].value.value_len);}

            }
        }
    return &result;
}

void *set_downloaded_html_set_cells_1_svc(cell_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct cell *local_downloaded_html_set_cells=NULL;
    if(arg1.cell_rpc_ptr_val!=NULL) {
    local_downloaded_html_set_cells=(struct cell *)malloc(arg1.cell_rpc_ptr_len*sizeof(struct cell));
    if(local_downloaded_html_set_cells==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.cell_rpc_ptr_len;i0++) {
    if (arg1.cell_rpc_ptr_val[i0].key.key_val == NULL) {local_downloaded_html_set_cells[i0].key=NULL;}

    else {local_downloaded_html_set_cells[i0].key=malloc(arg1.cell_rpc_ptr_val[i0].key.key_len);

    	if (local_downloaded_html_set_cells[i0].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_downloaded_html_set_cells[i0].key,arg1.cell_rpc_ptr_val[i0].key.key_val,arg1.cell_rpc_ptr_val[i0].key.key_len);

    }

    if (arg1.cell_rpc_ptr_val[i0].value.value_val == NULL) {local_downloaded_html_set_cells[i0].value=NULL;}

    else {local_downloaded_html_set_cells[i0].value=malloc(arg1.cell_rpc_ptr_val[i0].value.value_len);

    	if (local_downloaded_html_set_cells[i0].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_downloaded_html_set_cells[i0].value,arg1.cell_rpc_ptr_val[i0].value.value_val,arg1.cell_rpc_ptr_val[i0].value.value_len);

    }

    	}
    }
    memcpy(&downloaded_html_set_cells, &local_downloaded_html_set_cells, sizeof(cell));
    return (void *)&result;
}

downloaded_html_set_cells_key_ret_t *get_downloaded_html_set_cells_key_1_svc(struct svc_req *rqstp)
{
    static downloaded_html_set_cells_key_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 downloaded_html_set_cells.key 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t downloaded_html_set_cells_key_size; // 假设有这样的辅助变量
    if (downloaded_html_set_cells.key == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = downloaded_html_set_cells_key_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = downloaded_html_set_cells.key;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t downloaded_html_set_cells_key_size; // 假设有这样的辅助变量
void *set_downloaded_html_set_cells_key_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (downloaded_html_set_cells.key != NULL) {
        free(downloaded_html_set_cells.key);
        downloaded_html_set_cells.key = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        downloaded_html_set_cells.key = malloc(value.bytes.bytes_len);
        if (downloaded_html_set_cells.key == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(downloaded_html_set_cells.key, value.bytes.bytes_val, value.bytes.bytes_len);
            downloaded_html_set_cells_key_size = value.size;
        }
    }
    return (void *)&result;
}

downloaded_html_set_cells_value_ret_t *get_downloaded_html_set_cells_value_1_svc(struct svc_req *rqstp)
{
    static downloaded_html_set_cells_value_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 downloaded_html_set_cells.value 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t downloaded_html_set_cells_value_size; // 假设有这样的辅助变量
    if (downloaded_html_set_cells.value == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = downloaded_html_set_cells_value_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = downloaded_html_set_cells.value;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t downloaded_html_set_cells_value_size; // 假设有这样的辅助变量
void *set_downloaded_html_set_cells_value_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (downloaded_html_set_cells.value != NULL) {
        free(downloaded_html_set_cells.value);
        downloaded_html_set_cells.value = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        downloaded_html_set_cells.value = malloc(value.bytes.bytes_len);
        if (downloaded_html_set_cells.value == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(downloaded_html_set_cells.value, value.bytes.bytes_val, value.bytes.bytes_len);
            downloaded_html_set_cells_value_size = value.size;
        }
    }
    return (void *)&result;
}

downloaded_html_set_size_ret_t *get_downloaded_html_set_size_1_svc(struct svc_req *rqstp)
{
    static downloaded_html_set_size_ret_t result;
    result.value = downloaded_html_set.size;
    return &result;
}

void *set_downloaded_html_set_size_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    downloaded_html_set.size = value;
    return (void *)&result;
}

downloaded_html_set_count_ret_t *get_downloaded_html_set_count_1_svc(struct svc_req *rqstp)
{
    static downloaded_html_set_count_ret_t result;
    result.value = downloaded_html_set.count;
    return &result;
}

void *set_downloaded_html_set_count_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    downloaded_html_set.count = value;
    return (void *)&result;
}

downloaded_html_set_resize_threshold_ret_t *get_downloaded_html_set_resize_threshold_1_svc(struct svc_req *rqstp)
{
    static downloaded_html_set_resize_threshold_ret_t result;
    result.value = downloaded_html_set.resize_threshold;
    return &result;
}

void *set_downloaded_html_set_resize_threshold_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    downloaded_html_set.resize_threshold = value;
    return (void *)&result;
}

downloaded_html_set_prime_offset_ret_t *get_downloaded_html_set_prime_offset_1_svc(struct svc_req *rqstp)
{
    static downloaded_html_set_prime_offset_ret_t result;
    result.value = downloaded_html_set.prime_offset;
    return &result;
}

void *set_downloaded_html_set_prime_offset_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    downloaded_html_set.prime_offset = value;
    return (void *)&result;
}

numurls_ret_t *get_numurls_1_svc(struct svc_req *rqstp)
{
    static numurls_ret_t result;
    result.value = numurls;
    return &result;
}

void *set_numurls_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1; // 成功标志
    numurls = value;
    return (void *)&result;
}

char_ptr *get_program_argstring_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    if (program_argstring == NULL) {
        result.charptr.charptr_len = 0;
        result.charptr.charptr_val = NULL;
    } else {
        result.charptr.charptr_len = strlen(program_argstring) + 1;
        result.charptr.charptr_val = program_argstring;
    }
    return &result;
}

void *set_program_argstring_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result;
    if (program_argstring != NULL) {
        free(program_argstring);
    }
    if (value.charptr.charptr_val == NULL) {
        program_argstring = NULL;
    } else {
        program_argstring=malloc(value.charptr.charptr_len);
        memcpy(program_argstring, value.charptr.charptr_val, value.charptr.charptr_len);
    }
    return (void *)&result;
}

warc_cdx_dedup_table_ret_t *get_warc_cdx_dedup_table_1_svc(struct svc_req *rqstp)
{
    static warc_cdx_dedup_table_ret_t result;
        if(warc_cdx_dedup_table==NULL) {
            result.warc_cdx_dedup_table.hash_table_rpc_ptr_len=0;
            result.warc_cdx_dedup_table.hash_table_rpc_ptr_val=NULL;
        } else {
            result.warc_cdx_dedup_table.hash_table_rpc_ptr_len=1;
            result.warc_cdx_dedup_table.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
            if(result.warc_cdx_dedup_table.hash_table_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.warc_cdx_dedup_table.hash_table_rpc_ptr_len;i0++) {
    if (warc_cdx_dedup_table[i0].cells == NULL) {result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}

    else {result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));

    	if (result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {perror("malloc failed");exit(1);
}
    	for (int i1 = 0; i1 < result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {

    if (warc_cdx_dedup_table[i0].cells->key == NULL) {result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = 0;result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = NULL;}

    else{result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = sizeof(warc_cdx_dedup_table[i0].cells->key);result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = malloc(result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, warc_cdx_dedup_table[i0].cells->key, result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);}

    if (warc_cdx_dedup_table[i0].cells->value == NULL) {result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = 0;result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = NULL;}

    else{result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = sizeof(warc_cdx_dedup_table[i0].cells->value);result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = malloc(result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, warc_cdx_dedup_table[i0].cells->value, result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);}

    	}
    }
    result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].size=warc_cdx_dedup_table[i0].size;
    result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].count=warc_cdx_dedup_table[i0].count;
    result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].resize_threshold=warc_cdx_dedup_table[i0].resize_threshold;
    result.warc_cdx_dedup_table.hash_table_rpc_ptr_val[i0].prime_offset=warc_cdx_dedup_table[i0].prime_offset;
            }
        }
    return &result;
}

void *set_warc_cdx_dedup_table_1_svc(hash_table_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct hash_table *local_warc_cdx_dedup_table=NULL;
    if(arg1.hash_table_rpc_ptr_val!=NULL) {
    local_warc_cdx_dedup_table=(struct hash_table *)malloc(arg1.hash_table_rpc_ptr_len*sizeof(struct hash_table));
    if(local_warc_cdx_dedup_table==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.hash_table_rpc_ptr_len;i0++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {local_warc_cdx_dedup_table[i0].cells=NULL;}
    else {local_warc_cdx_dedup_table[i0].cells=(struct cell *)malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(struct cell));
    	if (local_warc_cdx_dedup_table[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {local_warc_cdx_dedup_table[i0].cells[i1].key=NULL;}

    else {local_warc_cdx_dedup_table[i0].cells[i1].key=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (local_warc_cdx_dedup_table[i0].cells[i1].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_warc_cdx_dedup_table[i0].cells[i1].key,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    }

    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {local_warc_cdx_dedup_table[i0].cells[i1].value=NULL;}

    else {local_warc_cdx_dedup_table[i0].cells[i1].value=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (local_warc_cdx_dedup_table[i0].cells[i1].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_warc_cdx_dedup_table[i0].cells[i1].value,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    }

    	}
    }
    local_warc_cdx_dedup_table[i0].size=arg1.hash_table_rpc_ptr_val[i0].size;
    local_warc_cdx_dedup_table[i0].count=arg1.hash_table_rpc_ptr_val[i0].count;
    local_warc_cdx_dedup_table[i0].resize_threshold=arg1.hash_table_rpc_ptr_val[i0].resize_threshold;
    local_warc_cdx_dedup_table[i0].prime_offset=arg1.hash_table_rpc_ptr_val[i0].prime_offset;
    	}
    }
    memcpy(&warc_cdx_dedup_table, &local_warc_cdx_dedup_table, sizeof(hash_table));
    return (void *)&result;
}

char_ptr *get_warc_cdx_dedup_table_hash_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (warc_cdx_dedup_table.hash_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_warc_cdx_dedup_table_hash_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        warc_cdx_dedup_table.hash_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        warc_cdx_dedup_table.hash_function = fun_ptr;
    }
    return (void *)&result;
}

char_ptr *get_warc_cdx_dedup_table_test_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (warc_cdx_dedup_table.test_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_warc_cdx_dedup_table_test_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        warc_cdx_dedup_table.test_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        warc_cdx_dedup_table.test_function = fun_ptr;
    }
    return (void *)&result;
}

warc_cdx_dedup_table_cells_ret_t *get_warc_cdx_dedup_table_cells_1_svc(struct svc_req *rqstp)
{
    static warc_cdx_dedup_table_cells_ret_t result;
        if(warc_cdx_dedup_table_cells==NULL) {
            result.warc_cdx_dedup_table_cells.cell_rpc_ptr_len=0;
            result.warc_cdx_dedup_table_cells.cell_rpc_ptr_val=NULL;
        } else {
            result.warc_cdx_dedup_table_cells.cell_rpc_ptr_len=1;
            result.warc_cdx_dedup_table_cells.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
            if(result.warc_cdx_dedup_table_cells.cell_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.warc_cdx_dedup_table_cells.cell_rpc_ptr_len;i0++) {
    if (warc_cdx_dedup_table_cells[i0].key == NULL) {result.warc_cdx_dedup_table_cells.cell_rpc_ptr_val[i0].key.key_len = 0;result.warc_cdx_dedup_table_cells.cell_rpc_ptr_val[i0].key.key_val = NULL;}

    else{result.warc_cdx_dedup_table_cells.cell_rpc_ptr_val[i0].key.key_len = sizeof(warc_cdx_dedup_table_cells[i0].key);result.warc_cdx_dedup_table_cells.cell_rpc_ptr_val[i0].key.key_val = malloc(result.warc_cdx_dedup_table_cells.cell_rpc_ptr_val[i0].key.key_len);

    	if (result.warc_cdx_dedup_table_cells.cell_rpc_ptr_val[i0].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.warc_cdx_dedup_table_cells.cell_rpc_ptr_val[i0].key.key_val, warc_cdx_dedup_table_cells[i0].key, result.warc_cdx_dedup_table_cells.cell_rpc_ptr_val[i0].key.key_len);}

    if (warc_cdx_dedup_table_cells[i0].value == NULL) {result.warc_cdx_dedup_table_cells.cell_rpc_ptr_val[i0].value.value_len = 0;result.warc_cdx_dedup_table_cells.cell_rpc_ptr_val[i0].value.value_val = NULL;}

    else{result.warc_cdx_dedup_table_cells.cell_rpc_ptr_val[i0].value.value_len = sizeof(warc_cdx_dedup_table_cells[i0].value);result.warc_cdx_dedup_table_cells.cell_rpc_ptr_val[i0].value.value_val = malloc(result.warc_cdx_dedup_table_cells.cell_rpc_ptr_val[i0].value.value_len);

    	if (result.warc_cdx_dedup_table_cells.cell_rpc_ptr_val[i0].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.warc_cdx_dedup_table_cells.cell_rpc_ptr_val[i0].value.value_val, warc_cdx_dedup_table_cells[i0].value, result.warc_cdx_dedup_table_cells.cell_rpc_ptr_val[i0].value.value_len);}

            }
        }
    return &result;
}

void *set_warc_cdx_dedup_table_cells_1_svc(cell_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct cell *local_warc_cdx_dedup_table_cells=NULL;
    if(arg1.cell_rpc_ptr_val!=NULL) {
    local_warc_cdx_dedup_table_cells=(struct cell *)malloc(arg1.cell_rpc_ptr_len*sizeof(struct cell));
    if(local_warc_cdx_dedup_table_cells==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.cell_rpc_ptr_len;i0++) {
    if (arg1.cell_rpc_ptr_val[i0].key.key_val == NULL) {local_warc_cdx_dedup_table_cells[i0].key=NULL;}

    else {local_warc_cdx_dedup_table_cells[i0].key=malloc(arg1.cell_rpc_ptr_val[i0].key.key_len);

    	if (local_warc_cdx_dedup_table_cells[i0].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_warc_cdx_dedup_table_cells[i0].key,arg1.cell_rpc_ptr_val[i0].key.key_val,arg1.cell_rpc_ptr_val[i0].key.key_len);

    }

    if (arg1.cell_rpc_ptr_val[i0].value.value_val == NULL) {local_warc_cdx_dedup_table_cells[i0].value=NULL;}

    else {local_warc_cdx_dedup_table_cells[i0].value=malloc(arg1.cell_rpc_ptr_val[i0].value.value_len);

    	if (local_warc_cdx_dedup_table_cells[i0].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_warc_cdx_dedup_table_cells[i0].value,arg1.cell_rpc_ptr_val[i0].value.value_val,arg1.cell_rpc_ptr_val[i0].value.value_len);

    }

    	}
    }
    memcpy(&warc_cdx_dedup_table_cells, &local_warc_cdx_dedup_table_cells, sizeof(cell));
    return (void *)&result;
}

warc_cdx_dedup_table_cells_key_ret_t *get_warc_cdx_dedup_table_cells_key_1_svc(struct svc_req *rqstp)
{
    static warc_cdx_dedup_table_cells_key_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 warc_cdx_dedup_table_cells.key 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t warc_cdx_dedup_table_cells_key_size; // 假设有这样的辅助变量
    if (warc_cdx_dedup_table_cells.key == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = warc_cdx_dedup_table_cells_key_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = warc_cdx_dedup_table_cells.key;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t warc_cdx_dedup_table_cells_key_size; // 假设有这样的辅助变量
void *set_warc_cdx_dedup_table_cells_key_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (warc_cdx_dedup_table_cells.key != NULL) {
        free(warc_cdx_dedup_table_cells.key);
        warc_cdx_dedup_table_cells.key = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        warc_cdx_dedup_table_cells.key = malloc(value.bytes.bytes_len);
        if (warc_cdx_dedup_table_cells.key == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(warc_cdx_dedup_table_cells.key, value.bytes.bytes_val, value.bytes.bytes_len);
            warc_cdx_dedup_table_cells_key_size = value.size;
        }
    }
    return (void *)&result;
}

warc_cdx_dedup_table_cells_value_ret_t *get_warc_cdx_dedup_table_cells_value_1_svc(struct svc_req *rqstp)
{
    static warc_cdx_dedup_table_cells_value_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 warc_cdx_dedup_table_cells.value 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t warc_cdx_dedup_table_cells_value_size; // 假设有这样的辅助变量
    if (warc_cdx_dedup_table_cells.value == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = warc_cdx_dedup_table_cells_value_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = warc_cdx_dedup_table_cells.value;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t warc_cdx_dedup_table_cells_value_size; // 假设有这样的辅助变量
void *set_warc_cdx_dedup_table_cells_value_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (warc_cdx_dedup_table_cells.value != NULL) {
        free(warc_cdx_dedup_table_cells.value);
        warc_cdx_dedup_table_cells.value = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        warc_cdx_dedup_table_cells.value = malloc(value.bytes.bytes_len);
        if (warc_cdx_dedup_table_cells.value == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(warc_cdx_dedup_table_cells.value, value.bytes.bytes_val, value.bytes.bytes_len);
            warc_cdx_dedup_table_cells_value_size = value.size;
        }
    }
    return (void *)&result;
}

warc_cdx_dedup_table_size_ret_t *get_warc_cdx_dedup_table_size_1_svc(struct svc_req *rqstp)
{
    static warc_cdx_dedup_table_size_ret_t result;
    result.value = warc_cdx_dedup_table.size;
    return &result;
}

void *set_warc_cdx_dedup_table_size_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    warc_cdx_dedup_table.size = value;
    return (void *)&result;
}

warc_cdx_dedup_table_count_ret_t *get_warc_cdx_dedup_table_count_1_svc(struct svc_req *rqstp)
{
    static warc_cdx_dedup_table_count_ret_t result;
    result.value = warc_cdx_dedup_table.count;
    return &result;
}

void *set_warc_cdx_dedup_table_count_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    warc_cdx_dedup_table.count = value;
    return (void *)&result;
}

warc_cdx_dedup_table_resize_threshold_ret_t *get_warc_cdx_dedup_table_resize_threshold_1_svc(struct svc_req *rqstp)
{
    static warc_cdx_dedup_table_resize_threshold_ret_t result;
    result.value = warc_cdx_dedup_table.resize_threshold;
    return &result;
}

void *set_warc_cdx_dedup_table_resize_threshold_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    warc_cdx_dedup_table.resize_threshold = value;
    return (void *)&result;
}

warc_cdx_dedup_table_prime_offset_ret_t *get_warc_cdx_dedup_table_prime_offset_1_svc(struct svc_req *rqstp)
{
    static warc_cdx_dedup_table_prime_offset_ret_t result;
    result.value = warc_cdx_dedup_table.prime_offset;
    return &result;
}

void *set_warc_cdx_dedup_table_prime_offset_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    warc_cdx_dedup_table.prime_offset = value;
    return (void *)&result;
}

current_impl_ret_t *get_current_impl_1_svc(struct svc_req *rqstp)
{
    static current_impl_ret_t result;
        if(current_impl==NULL) {
            result.current_impl.progress_implementation_rpc_ptr_len=0;
            result.current_impl.progress_implementation_rpc_ptr_val=NULL;
        } else {
            result.current_impl.progress_implementation_rpc_ptr_len=1;
            result.current_impl.progress_implementation_rpc_ptr_val=malloc(1*sizeof(progress_implementation_rpc));
            if(result.current_impl.progress_implementation_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.current_impl.progress_implementation_rpc_ptr_len;i0++) {
    if (current_impl[i0].name == NULL) {result.current_impl.progress_implementation_rpc_ptr_val[i0].name.charptr.charptr_len = 0;result.current_impl.progress_implementation_rpc_ptr_val[i0].name.charptr.charptr_val = NULL;}

    else {result.current_impl.progress_implementation_rpc_ptr_val[i0].name.charptr.charptr_len = strlen(current_impl[i0].name)+1;result.current_impl.progress_implementation_rpc_ptr_val[i0].name.charptr.charptr_val = current_impl[i0].name;}

    result.current_impl.progress_implementation_rpc_ptr_val[i0].interactive=current_impl[i0].interactive;
            }
        }
    return &result;
}

void *set_current_impl_1_svc(progress_implementation_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct progress_implementation *local_current_impl=NULL;
    if(arg1.progress_implementation_rpc_ptr_val!=NULL) {
    local_current_impl=(struct progress_implementation *)malloc(arg1.progress_implementation_rpc_ptr_len*sizeof(struct progress_implementation));
    if(local_current_impl==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.progress_implementation_rpc_ptr_len;i0++) {
    if (arg1.progress_implementation_rpc_ptr_val[i0].name.charptr.charptr_val == NULL) {local_current_impl[i0].name=NULL;}

    else {local_current_impl[i0].name=malloc(arg1.progress_implementation_rpc_ptr_val[i0].name.charptr.charptr_len);

    memcpy(local_current_impl[i0].name,arg1.progress_implementation_rpc_ptr_val[i0].name.charptr.charptr_val,arg1.progress_implementation_rpc_ptr_val[i0].name.charptr.charptr_len);

    }

    local_current_impl[i0].interactive=arg1.progress_implementation_rpc_ptr_val[i0].interactive;
    	}
    }
    memcpy(&current_impl, &local_current_impl, sizeof(progress_implementation));
    return (void *)&result;
}

current_impl_name_ret_t *get_current_impl_name_1_svc(struct svc_req *rqstp)
{
    static current_impl_name_ret_t result;
    if (current_impl.name != NULL) {
        result.value = strdup(current_impl.name);
    } else {
        result.value = NULL;
    }
    return &result;
}

void *set_current_impl_name_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (current_impl.name != NULL) {
        free(current_impl.name);
    }
    if (value != NULL) {
        current_impl.name = strdup(value);
    } else {
        current_impl.name = NULL;
    }
    return (void *)&result;
}

current_impl_interactive_ret_t *get_current_impl_interactive_1_svc(struct svc_req *rqstp)
{
    static current_impl_interactive_ret_t result;
    result.value = current_impl.interactive;
    return &result;
}

void *set_current_impl_interactive_1_svc(bool value, struct svc_req *rqstp)
{
    static int result = 1;
    current_impl.interactive = value;
    return (void *)&result;
}

char_ptr *get_current_impl_create_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (current_impl.create);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_current_impl_create_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        current_impl.create = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        current_impl.create = fun_ptr;
    }
    return (void *)&result;
}

char_ptr *get_current_impl_update_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (current_impl.update);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_current_impl_update_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        current_impl.update = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        current_impl.update = fun_ptr;
    }
    return (void *)&result;
}

char_ptr *get_current_impl_draw_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (current_impl.draw);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_current_impl_draw_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        current_impl.draw = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        current_impl.draw = fun_ptr;
    }
    return (void *)&result;
}

char_ptr *get_current_impl_finish_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (current_impl.finish);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_current_impl_finish_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        current_impl.finish = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        current_impl.finish = fun_ptr;
    }
    return (void *)&result;
}

char_ptr *get_current_impl_set_params_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (current_impl.set_params);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_current_impl_set_params_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        current_impl.set_params = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        current_impl.set_params = fun_ptr;
    }
    return (void *)&result;
}

wgnutls_transport_ret_t *get_wgnutls_transport_1_svc(struct svc_req *rqstp)
{
    static wgnutls_transport_ret_t result;
    return &result;
}

void *set_wgnutls_transport_1_svc(transport_implementation_rpc value, struct svc_req *rqstp)
{
    static int result = 1;
    transport_implementation_rpc arg1
    return (void *)&result;
}

char_ptr *get_wgnutls_transport_reader_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (wgnutls_transport.reader);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_wgnutls_transport_reader_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        wgnutls_transport.reader = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        wgnutls_transport.reader = fun_ptr;
    }
    return (void *)&result;
}

char_ptr *get_wgnutls_transport_writer_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (wgnutls_transport.writer);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_wgnutls_transport_writer_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        wgnutls_transport.writer = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        wgnutls_transport.writer = fun_ptr;
    }
    return (void *)&result;
}

char_ptr *get_wgnutls_transport_poller_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (wgnutls_transport.poller);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_wgnutls_transport_poller_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        wgnutls_transport.poller = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        wgnutls_transport.poller = fun_ptr;
    }
    return (void *)&result;
}

char_ptr *get_wgnutls_transport_peeker_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (wgnutls_transport.peeker);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_wgnutls_transport_peeker_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        wgnutls_transport.peeker = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        wgnutls_transport.peeker = fun_ptr;
    }
    return (void *)&result;
}

char_ptr *get_wgnutls_transport_errstr_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (wgnutls_transport.errstr);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_wgnutls_transport_errstr_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        wgnutls_transport.errstr = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        wgnutls_transport.errstr = fun_ptr;
    }
    return (void *)&result;
}

char_ptr *get_wgnutls_transport_closer_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (wgnutls_transport.closer);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_wgnutls_transport_closer_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        wgnutls_transport.closer = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        wgnutls_transport.closer = fun_ptr;
    }
    return (void *)&result;
}

registered_specs_ret_t *get_registered_specs_1_svc(struct svc_req *rqstp)
{
    static registered_specs_ret_t result;
        if(registered_specs==NULL) {
            result.registered_specs.hash_table_rpc_ptr_len=0;
            result.registered_specs.hash_table_rpc_ptr_val=NULL;
        } else {
            result.registered_specs.hash_table_rpc_ptr_len=1;
            result.registered_specs.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
            if(result.registered_specs.hash_table_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.registered_specs.hash_table_rpc_ptr_len;i0++) {
    if (registered_specs[i0].cells == NULL) {result.registered_specs.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;result.registered_specs.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}

    else {result.registered_specs.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;result.registered_specs.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(result.registered_specs.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));

    	if (result.registered_specs.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {perror("malloc failed");exit(1);
}
    	for (int i1 = 0; i1 < result.registered_specs.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {

    if (registered_specs[i0].cells->key == NULL) {result.registered_specs.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = 0;result.registered_specs.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = NULL;}

    else{result.registered_specs.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = sizeof(registered_specs[i0].cells->key);result.registered_specs.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = malloc(result.registered_specs.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (result.registered_specs.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.registered_specs.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, registered_specs[i0].cells->key, result.registered_specs.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);}

    if (registered_specs[i0].cells->value == NULL) {result.registered_specs.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = 0;result.registered_specs.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = NULL;}

    else{result.registered_specs.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = sizeof(registered_specs[i0].cells->value);result.registered_specs.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = malloc(result.registered_specs.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (result.registered_specs.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.registered_specs.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, registered_specs[i0].cells->value, result.registered_specs.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);}

    	}
    }
    result.registered_specs.hash_table_rpc_ptr_val[i0].size=registered_specs[i0].size;
    result.registered_specs.hash_table_rpc_ptr_val[i0].count=registered_specs[i0].count;
    result.registered_specs.hash_table_rpc_ptr_val[i0].resize_threshold=registered_specs[i0].resize_threshold;
    result.registered_specs.hash_table_rpc_ptr_val[i0].prime_offset=registered_specs[i0].prime_offset;
            }
        }
    return &result;
}

void *set_registered_specs_1_svc(hash_table_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct hash_table *local_registered_specs=NULL;
    if(arg1.hash_table_rpc_ptr_val!=NULL) {
    local_registered_specs=(struct hash_table *)malloc(arg1.hash_table_rpc_ptr_len*sizeof(struct hash_table));
    if(local_registered_specs==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.hash_table_rpc_ptr_len;i0++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {local_registered_specs[i0].cells=NULL;}
    else {local_registered_specs[i0].cells=(struct cell *)malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(struct cell));
    	if (local_registered_specs[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {local_registered_specs[i0].cells[i1].key=NULL;}

    else {local_registered_specs[i0].cells[i1].key=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (local_registered_specs[i0].cells[i1].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_registered_specs[i0].cells[i1].key,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    }

    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {local_registered_specs[i0].cells[i1].value=NULL;}

    else {local_registered_specs[i0].cells[i1].value=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (local_registered_specs[i0].cells[i1].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_registered_specs[i0].cells[i1].value,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    }

    	}
    }
    local_registered_specs[i0].size=arg1.hash_table_rpc_ptr_val[i0].size;
    local_registered_specs[i0].count=arg1.hash_table_rpc_ptr_val[i0].count;
    local_registered_specs[i0].resize_threshold=arg1.hash_table_rpc_ptr_val[i0].resize_threshold;
    local_registered_specs[i0].prime_offset=arg1.hash_table_rpc_ptr_val[i0].prime_offset;
    	}
    }
    memcpy(&registered_specs, &local_registered_specs, sizeof(hash_table));
    return (void *)&result;
}

char_ptr *get_registered_specs_hash_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (registered_specs.hash_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_registered_specs_hash_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        registered_specs.hash_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        registered_specs.hash_function = fun_ptr;
    }
    return (void *)&result;
}

char_ptr *get_registered_specs_test_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (registered_specs.test_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_registered_specs_test_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        registered_specs.test_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        registered_specs.test_function = fun_ptr;
    }
    return (void *)&result;
}

registered_specs_cells_ret_t *get_registered_specs_cells_1_svc(struct svc_req *rqstp)
{
    static registered_specs_cells_ret_t result;
        if(registered_specs_cells==NULL) {
            result.registered_specs_cells.cell_rpc_ptr_len=0;
            result.registered_specs_cells.cell_rpc_ptr_val=NULL;
        } else {
            result.registered_specs_cells.cell_rpc_ptr_len=1;
            result.registered_specs_cells.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
            if(result.registered_specs_cells.cell_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.registered_specs_cells.cell_rpc_ptr_len;i0++) {
    if (registered_specs_cells[i0].key == NULL) {result.registered_specs_cells.cell_rpc_ptr_val[i0].key.key_len = 0;result.registered_specs_cells.cell_rpc_ptr_val[i0].key.key_val = NULL;}

    else{result.registered_specs_cells.cell_rpc_ptr_val[i0].key.key_len = sizeof(registered_specs_cells[i0].key);result.registered_specs_cells.cell_rpc_ptr_val[i0].key.key_val = malloc(result.registered_specs_cells.cell_rpc_ptr_val[i0].key.key_len);

    	if (result.registered_specs_cells.cell_rpc_ptr_val[i0].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.registered_specs_cells.cell_rpc_ptr_val[i0].key.key_val, registered_specs_cells[i0].key, result.registered_specs_cells.cell_rpc_ptr_val[i0].key.key_len);}

    if (registered_specs_cells[i0].value == NULL) {result.registered_specs_cells.cell_rpc_ptr_val[i0].value.value_len = 0;result.registered_specs_cells.cell_rpc_ptr_val[i0].value.value_val = NULL;}

    else{result.registered_specs_cells.cell_rpc_ptr_val[i0].value.value_len = sizeof(registered_specs_cells[i0].value);result.registered_specs_cells.cell_rpc_ptr_val[i0].value.value_val = malloc(result.registered_specs_cells.cell_rpc_ptr_val[i0].value.value_len);

    	if (result.registered_specs_cells.cell_rpc_ptr_val[i0].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.registered_specs_cells.cell_rpc_ptr_val[i0].value.value_val, registered_specs_cells[i0].value, result.registered_specs_cells.cell_rpc_ptr_val[i0].value.value_len);}

            }
        }
    return &result;
}

void *set_registered_specs_cells_1_svc(cell_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct cell *local_registered_specs_cells=NULL;
    if(arg1.cell_rpc_ptr_val!=NULL) {
    local_registered_specs_cells=(struct cell *)malloc(arg1.cell_rpc_ptr_len*sizeof(struct cell));
    if(local_registered_specs_cells==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.cell_rpc_ptr_len;i0++) {
    if (arg1.cell_rpc_ptr_val[i0].key.key_val == NULL) {local_registered_specs_cells[i0].key=NULL;}

    else {local_registered_specs_cells[i0].key=malloc(arg1.cell_rpc_ptr_val[i0].key.key_len);

    	if (local_registered_specs_cells[i0].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_registered_specs_cells[i0].key,arg1.cell_rpc_ptr_val[i0].key.key_val,arg1.cell_rpc_ptr_val[i0].key.key_len);

    }

    if (arg1.cell_rpc_ptr_val[i0].value.value_val == NULL) {local_registered_specs_cells[i0].value=NULL;}

    else {local_registered_specs_cells[i0].value=malloc(arg1.cell_rpc_ptr_val[i0].value.value_len);

    	if (local_registered_specs_cells[i0].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_registered_specs_cells[i0].value,arg1.cell_rpc_ptr_val[i0].value.value_val,arg1.cell_rpc_ptr_val[i0].value.value_len);

    }

    	}
    }
    memcpy(&registered_specs_cells, &local_registered_specs_cells, sizeof(cell));
    return (void *)&result;
}

registered_specs_cells_key_ret_t *get_registered_specs_cells_key_1_svc(struct svc_req *rqstp)
{
    static registered_specs_cells_key_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 registered_specs_cells.key 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t registered_specs_cells_key_size; // 假设有这样的辅助变量
    if (registered_specs_cells.key == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = registered_specs_cells_key_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = registered_specs_cells.key;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t registered_specs_cells_key_size; // 假设有这样的辅助变量
void *set_registered_specs_cells_key_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (registered_specs_cells.key != NULL) {
        free(registered_specs_cells.key);
        registered_specs_cells.key = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        registered_specs_cells.key = malloc(value.bytes.bytes_len);
        if (registered_specs_cells.key == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(registered_specs_cells.key, value.bytes.bytes_val, value.bytes.bytes_len);
            registered_specs_cells_key_size = value.size;
        }
    }
    return (void *)&result;
}

registered_specs_cells_value_ret_t *get_registered_specs_cells_value_1_svc(struct svc_req *rqstp)
{
    static registered_specs_cells_value_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 registered_specs_cells.value 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t registered_specs_cells_value_size; // 假设有这样的辅助变量
    if (registered_specs_cells.value == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = registered_specs_cells_value_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = registered_specs_cells.value;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t registered_specs_cells_value_size; // 假设有这样的辅助变量
void *set_registered_specs_cells_value_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (registered_specs_cells.value != NULL) {
        free(registered_specs_cells.value);
        registered_specs_cells.value = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        registered_specs_cells.value = malloc(value.bytes.bytes_len);
        if (registered_specs_cells.value == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(registered_specs_cells.value, value.bytes.bytes_val, value.bytes.bytes_len);
            registered_specs_cells_value_size = value.size;
        }
    }
    return (void *)&result;
}

registered_specs_size_ret_t *get_registered_specs_size_1_svc(struct svc_req *rqstp)
{
    static registered_specs_size_ret_t result;
    result.value = registered_specs.size;
    return &result;
}

void *set_registered_specs_size_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    registered_specs.size = value;
    return (void *)&result;
}

registered_specs_count_ret_t *get_registered_specs_count_1_svc(struct svc_req *rqstp)
{
    static registered_specs_count_ret_t result;
    result.value = registered_specs.count;
    return &result;
}

void *set_registered_specs_count_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    registered_specs.count = value;
    return (void *)&result;
}

registered_specs_resize_threshold_ret_t *get_registered_specs_resize_threshold_1_svc(struct svc_req *rqstp)
{
    static registered_specs_resize_threshold_ret_t result;
    result.value = registered_specs.resize_threshold;
    return &result;
}

void *set_registered_specs_resize_threshold_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    registered_specs.resize_threshold = value;
    return (void *)&result;
}

registered_specs_prime_offset_ret_t *get_registered_specs_prime_offset_1_svc(struct svc_req *rqstp)
{
    static registered_specs_prime_offset_ret_t result;
    result.value = registered_specs.prime_offset;
    return &result;
}

void *set_registered_specs_prime_offset_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    registered_specs.prime_offset = value;
    return (void *)&result;
}

char_ptr *get_output_stream_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *filename=get_filename_from_fp(output_stream);
    if (filename == NULL) {
        result.charptr.charptr_len = 0;
        result.charptr.charptr_val = NULL;
    } else {
        result.charptr.charptr_len = strlen(filename) + 1;
        result.charptr.charptr_val = filename;
    }
    return &result;
}

            output_stream = stdin;
        } else if (strcmp(value.charptr.charptr_val, "stdout") == 0) {
            output_stream = stdout;
        } else if (strcmp(value.charptr.charptr_val, "stderr") == 0) {
            output_stream = stderr;
        } else {
            output_stream = fopen(value.charptr.charptr_val, "r+");
            if ({var_name} == NULL) {
                perror("fopen failed");
                return NULL;
            }
        }
void *set_output_stream_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result;
    if (value.charptr.charptr_val == NULL) {
        output_stream = NULL;
    } else {
        // 假设文件模式为读写
      if (strcmp(value.charptr.charptr_val, "stdin") == 0) {
j    }
    return (void *)&result;
}

transport_map_modified_tick_ret_t *get_transport_map_modified_tick_1_svc(struct svc_req *rqstp)
{
    static transport_map_modified_tick_ret_t result;
    result.value = transport_map_modified_tick;
    return &result;
}

void *set_transport_map_modified_tick_1_svc(unsigned int value, struct svc_req *rqstp)
{
    static int result = 1; // 成功标志
    transport_map_modified_tick = value;
    return (void *)&result;
}

netrc_list_ret_t *get_netrc_list_1_svc(struct svc_req *rqstp)
{
    static netrc_list_ret_t result;
        if(netrc_list==NULL) {
            result.netrc_list.acc_t_rpc_ptr_len=0;
            result.netrc_list.acc_t_rpc_ptr_val=NULL;
        } else {
            result.netrc_list.acc_t_rpc_ptr_len=1;
            result.netrc_list.acc_t_rpc_ptr_val=malloc(1*sizeof(acc_t_rpc));
            if(result.netrc_list.acc_t_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.netrc_list.acc_t_rpc_ptr_len;i0++) {
    if (netrc_list[i0].host == NULL) {result.netrc_list.acc_t_rpc_ptr_val[i0].host.charptr.charptr_len = 0;result.netrc_list.acc_t_rpc_ptr_val[i0].host.charptr.charptr_val = NULL;}

    else {result.netrc_list.acc_t_rpc_ptr_val[i0].host.charptr.charptr_len = strlen(netrc_list[i0].host)+1;result.netrc_list.acc_t_rpc_ptr_val[i0].host.charptr.charptr_val = netrc_list[i0].host;}

    if (netrc_list[i0].acc == NULL) {result.netrc_list.acc_t_rpc_ptr_val[i0].acc.charptr.charptr_len = 0;result.netrc_list.acc_t_rpc_ptr_val[i0].acc.charptr.charptr_val = NULL;}

    else {result.netrc_list.acc_t_rpc_ptr_val[i0].acc.charptr.charptr_len = strlen(netrc_list[i0].acc)+1;result.netrc_list.acc_t_rpc_ptr_val[i0].acc.charptr.charptr_val = netrc_list[i0].acc;}

    if (netrc_list[i0].passwd == NULL) {result.netrc_list.acc_t_rpc_ptr_val[i0].passwd.charptr.charptr_len = 0;result.netrc_list.acc_t_rpc_ptr_val[i0].passwd.charptr.charptr_val = NULL;}

    else {result.netrc_list.acc_t_rpc_ptr_val[i0].passwd.charptr.charptr_len = strlen(netrc_list[i0].passwd)+1;result.netrc_list.acc_t_rpc_ptr_val[i0].passwd.charptr.charptr_val = netrc_list[i0].passwd;}

    if (netrc_list[i0].next == NULL) {result.netrc_list.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_len = 0;result.netrc_list.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val = NULL;}

    else {result.netrc_list.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_len = 1;result.netrc_list.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val = malloc(result.netrc_list.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_len * sizeof(_acc_t_rpc));

    	if (result.netrc_list.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val == NULL) {perror("malloc failed");exit(1);
}
    	for (int i1 = 0; i1 < result.netrc_list.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_len; i1++) {

    if (netrc_list[i0].next->host == NULL) {result.netrc_list.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].host.charptr.charptr_len = 0;result.netrc_list.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].host.charptr.charptr_val = NULL;}

    else {result.netrc_list.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].host.charptr.charptr_len = strlen(netrc_list[i0].next->host)+1;result.netrc_list.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].host.charptr.charptr_val = netrc_list[i0].next->host;}

    if (netrc_list[i0].next->acc == NULL) {result.netrc_list.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].acc.charptr.charptr_len = 0;result.netrc_list.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].acc.charptr.charptr_val = NULL;}

    else {result.netrc_list.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].acc.charptr.charptr_len = strlen(netrc_list[i0].next->acc)+1;result.netrc_list.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].acc.charptr.charptr_val = netrc_list[i0].next->acc;}

    if (netrc_list[i0].next->passwd == NULL) {result.netrc_list.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].passwd.charptr.charptr_len = 0;result.netrc_list.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].passwd.charptr.charptr_val = NULL;}

    else {result.netrc_list.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].passwd.charptr.charptr_len = strlen(netrc_list[i0].next->passwd)+1;result.netrc_list.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].passwd.charptr.charptr_val = netrc_list[i0].next->passwd;}

    if (netrc_list[i0].next.next == NULL) {result.netrc_list.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].next._acc_t_rpc_ptr_len = 0;result.netrc_list.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].next._acc_t_rpc_ptr_val = NULL;}

    else {result.netrc_list.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].next._acc_t_rpc_ptr_len = 1;result.netrc_list.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].next._acc_t_rpc_ptr_val = malloc(result.netrc_list.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].next._acc_t_rpc_ptr_len * sizeof(_acc_t_rpc));

    	if (result.netrc_list.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].next._acc_t_rpc_ptr_val == NULL) {perror("malloc failed");exit(1);
}
    	for (int i1 = 0; i1 < result.netrc_list.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].next._acc_t_rpc_ptr_len; i1++) {

    	}
    }
    	}
    }
            }
        }
    return &result;
}

void *set_netrc_list_1_svc(acc_t_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    acc_t *local_netrc_list=NULL;
    if(arg1.acc_t_rpc_ptr_val!=NULL) {
    local_netrc_list=( acc_t *)malloc(arg1.acc_t_rpc_ptr_len*sizeof( acc_t *));
    if(local_netrc_list==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.acc_t_rpc_ptr_len;i0++) {
    if (arg1.acc_t_rpc_ptr_val[i0].host.charptr.charptr_val == NULL) {local_netrc_list[i0].host=NULL;}

    else {local_netrc_list[i0].host=malloc(arg1.acc_t_rpc_ptr_val[i0].host.charptr.charptr_len);

    memcpy(local_netrc_list[i0].host,arg1.acc_t_rpc_ptr_val[i0].host.charptr.charptr_val,arg1.acc_t_rpc_ptr_val[i0].host.charptr.charptr_len);

    }

    if (arg1.acc_t_rpc_ptr_val[i0].acc.charptr.charptr_val == NULL) {local_netrc_list[i0].acc=NULL;}

    else {local_netrc_list[i0].acc=malloc(arg1.acc_t_rpc_ptr_val[i0].acc.charptr.charptr_len);

    memcpy(local_netrc_list[i0].acc,arg1.acc_t_rpc_ptr_val[i0].acc.charptr.charptr_val,arg1.acc_t_rpc_ptr_val[i0].acc.charptr.charptr_len);

    }

    if (arg1.acc_t_rpc_ptr_val[i0].passwd.charptr.charptr_val == NULL) {local_netrc_list[i0].passwd=NULL;}

    else {local_netrc_list[i0].passwd=malloc(arg1.acc_t_rpc_ptr_val[i0].passwd.charptr.charptr_len);

    memcpy(local_netrc_list[i0].passwd,arg1.acc_t_rpc_ptr_val[i0].passwd.charptr.charptr_val,arg1.acc_t_rpc_ptr_val[i0].passwd.charptr.charptr_len);

    }

    if (arg1.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val == NULL) {local_netrc_list[i0].next=NULL;}
    else {local_netrc_list[i0].next=(struct _acc_t *)malloc(arg1.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_len * sizeof(struct _acc_t));
    	if (local_netrc_list[i0].next == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < arg1.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_len; i1++) {
    if (arg1.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].host.charptr.charptr_val == NULL) {local_netrc_list[i0].next[i1].host=NULL;}

    else {local_netrc_list[i0].next[i1].host=malloc(arg1.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].host.charptr.charptr_len);

    memcpy(local_netrc_list[i0].next[i1].host,arg1.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].host.charptr.charptr_val,arg1.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].host.charptr.charptr_len);

    }

    if (arg1.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].acc.charptr.charptr_val == NULL) {local_netrc_list[i0].next[i1].acc=NULL;}

    else {local_netrc_list[i0].next[i1].acc=malloc(arg1.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].acc.charptr.charptr_len);

    memcpy(local_netrc_list[i0].next[i1].acc,arg1.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].acc.charptr.charptr_val,arg1.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].acc.charptr.charptr_len);

    }

    if (arg1.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].passwd.charptr.charptr_val == NULL) {local_netrc_list[i0].next[i1].passwd=NULL;}

    else {local_netrc_list[i0].next[i1].passwd=malloc(arg1.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].passwd.charptr.charptr_len);

    memcpy(local_netrc_list[i0].next[i1].passwd,arg1.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].passwd.charptr.charptr_val,arg1.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].passwd.charptr.charptr_len);

    }

    if (arg1.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].next._acc_t_rpc_ptr_val == NULL) {local_netrc_list[i0].next[i1].next=NULL;}
    else {local_netrc_list[i0].next[i1].next=(struct _acc_t *)malloc(arg1.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].next._acc_t_rpc_ptr_len * sizeof(struct _acc_t));
    	if (local_netrc_list[i0].next[i1].next == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < arg1.acc_t_rpc_ptr_val[i0].next._acc_t_rpc_ptr_val[i1].next._acc_t_rpc_ptr_len; i1++) {
    	}
    }
    	}
    }
    	}
    }
    memcpy(&netrc_list, &local_netrc_list, sizeof(acc_t));
    return (void *)&result;
}

char_ptr *get_logfp_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *filename=get_filename_from_fp(logfp);
    if (filename == NULL) {
        result.charptr.charptr_len = 0;
        result.charptr.charptr_val = NULL;
    } else {
        result.charptr.charptr_len = strlen(filename) + 1;
        result.charptr.charptr_val = filename;
    }
    return &result;
}

            logfp = stdin;
        } else if (strcmp(value.charptr.charptr_val, "stdout") == 0) {
            logfp = stdout;
        } else if (strcmp(value.charptr.charptr_val, "stderr") == 0) {
            logfp = stderr;
        } else {
            logfp = fopen(value.charptr.charptr_val, "r+");
            if ({var_name} == NULL) {
                perror("fopen failed");
                return NULL;
            }
        }
void *set_logfp_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result;
    if (value.charptr.charptr_val == NULL) {
        logfp = NULL;
    } else {
        // 假设文件模式为读写
      if (strcmp(value.charptr.charptr_val, "stdin") == 0) {
j    }
    return (void *)&result;
}

char_ptr *get_meta_charset_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    if (meta_charset == NULL) {
        result.charptr.charptr_len = 0;
        result.charptr.charptr_val = NULL;
    } else {
        result.charptr.charptr_len = strlen(meta_charset) + 1;
        result.charptr.charptr_val = meta_charset;
    }
    return &result;
}

void *set_meta_charset_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result;
    if (meta_charset != NULL) {
        free(meta_charset);
    }
    if (value.charptr.charptr_val == NULL) {
        meta_charset = NULL;
    } else {
        meta_charset=malloc(value.charptr.charptr_len);
        memcpy(meta_charset, value.charptr.charptr_val, value.charptr.charptr_len);
    }
    return (void *)&result;
}

char_ptr *get_redirect_request_signal_name_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    if (redirect_request_signal_name == NULL) {
        result.charptr.charptr_len = 0;
        result.charptr.charptr_val = NULL;
    } else {
        result.charptr.charptr_len = strlen(redirect_request_signal_name) + 1;
        result.charptr.charptr_val = redirect_request_signal_name;
    }
    return &result;
}

void *set_redirect_request_signal_name_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result;
    if (redirect_request_signal_name != NULL) {
        free(redirect_request_signal_name);
    }
    if (value.charptr.charptr_val == NULL) {
        redirect_request_signal_name = NULL;
    } else {
        redirect_request_signal_name=malloc(value.charptr.charptr_len);
        memcpy(redirect_request_signal_name, value.charptr.charptr_val, value.charptr.charptr_len);
    }
    return (void *)&result;
}

downloaded_files_hash_ret_t *get_downloaded_files_hash_1_svc(struct svc_req *rqstp)
{
    static downloaded_files_hash_ret_t result;
        if(downloaded_files_hash==NULL) {
            result.downloaded_files_hash.hash_table_rpc_ptr_len=0;
            result.downloaded_files_hash.hash_table_rpc_ptr_val=NULL;
        } else {
            result.downloaded_files_hash.hash_table_rpc_ptr_len=1;
            result.downloaded_files_hash.hash_table_rpc_ptr_val=malloc(1*sizeof(hash_table_rpc));
            if(result.downloaded_files_hash.hash_table_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.downloaded_files_hash.hash_table_rpc_ptr_len;i0++) {
    if (downloaded_files_hash[i0].cells == NULL) {result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 0;result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = NULL;}

    else {result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len = 1;result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val = malloc(result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(cell_rpc));

    	if (result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {perror("malloc failed");exit(1);
}
    	for (int i1 = 0; i1 < result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {

    if (downloaded_files_hash[i0].cells->key == NULL) {result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = 0;result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = NULL;}

    else{result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len = sizeof(downloaded_files_hash[i0].cells->key);result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val = malloc(result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val, downloaded_files_hash[i0].cells->key, result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);}

    if (downloaded_files_hash[i0].cells->value == NULL) {result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = 0;result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = NULL;}

    else{result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len = sizeof(downloaded_files_hash[i0].cells->value);result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val = malloc(result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val, downloaded_files_hash[i0].cells->value, result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);}

    	}
    }
    result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].size=downloaded_files_hash[i0].size;
    result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].count=downloaded_files_hash[i0].count;
    result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].resize_threshold=downloaded_files_hash[i0].resize_threshold;
    result.downloaded_files_hash.hash_table_rpc_ptr_val[i0].prime_offset=downloaded_files_hash[i0].prime_offset;
            }
        }
    return &result;
}

void *set_downloaded_files_hash_1_svc(hash_table_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct hash_table *local_downloaded_files_hash=NULL;
    if(arg1.hash_table_rpc_ptr_val!=NULL) {
    local_downloaded_files_hash=(struct hash_table *)malloc(arg1.hash_table_rpc_ptr_len*sizeof(struct hash_table));
    if(local_downloaded_files_hash==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.hash_table_rpc_ptr_len;i0++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val == NULL) {local_downloaded_files_hash[i0].cells=NULL;}
    else {local_downloaded_files_hash[i0].cells=(struct cell *)malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len * sizeof(struct cell));
    	if (local_downloaded_files_hash[i0].cells == NULL) {
    		perror("malloc failed");
    		exit(1);
    	}
    	for (int i1 = 0; i1 < arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_len; i1++) {
    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val == NULL) {local_downloaded_files_hash[i0].cells[i1].key=NULL;}

    else {local_downloaded_files_hash[i0].cells[i1].key=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    	if (local_downloaded_files_hash[i0].cells[i1].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_downloaded_files_hash[i0].cells[i1].key,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].key.key_len);

    }

    if (arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val == NULL) {local_downloaded_files_hash[i0].cells[i1].value=NULL;}

    else {local_downloaded_files_hash[i0].cells[i1].value=malloc(arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    	if (local_downloaded_files_hash[i0].cells[i1].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_downloaded_files_hash[i0].cells[i1].value,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_val,arg1.hash_table_rpc_ptr_val[i0].cells.cell_rpc_ptr_val[i1].value.value_len);

    }

    	}
    }
    local_downloaded_files_hash[i0].size=arg1.hash_table_rpc_ptr_val[i0].size;
    local_downloaded_files_hash[i0].count=arg1.hash_table_rpc_ptr_val[i0].count;
    local_downloaded_files_hash[i0].resize_threshold=arg1.hash_table_rpc_ptr_val[i0].resize_threshold;
    local_downloaded_files_hash[i0].prime_offset=arg1.hash_table_rpc_ptr_val[i0].prime_offset;
    	}
    }
    memcpy(&downloaded_files_hash, &local_downloaded_files_hash, sizeof(hash_table));
    return (void *)&result;
}

char_ptr *get_downloaded_files_hash_hash_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (downloaded_files_hash.hash_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_downloaded_files_hash_hash_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        downloaded_files_hash.hash_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        downloaded_files_hash.hash_function = fun_ptr;
    }
    return (void *)&result;
}

char_ptr *get_downloaded_files_hash_test_function_1_svc(struct svc_req *rqstp)
{
    static char_ptr result;
    char *funname= find_function_name_by_address  (downloaded_files_hash.test_function);
    if (funname == NULL) {
        result.charptr.charptr_val = NULL;
        result.charptr.charptr_len = 0;
    } else {
        result.charptr.charptr_len = strlen(funname)+1;
        result.charptr.charptr_val = malloc(result.charptr.charptr_len);
        if (result.charptr.charptr_val == NULL) {
            perror("malloc failed");
            result.charptr.charptr_len = 0;
            result.charptr.charptr_val = NULL;
        } else {
            memcpy(result.charptr.charptr_val, funname, result.charptr.charptr_len);
        }
    }
void *set_downloaded_files_hash_test_function_1_svc(char_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    if (value.charptr.charptr_val == NULL) {
        downloaded_files_hash.test_function = NULL;
        return (void *)&result;
    }
    if (value.charptr.charptr_val != NULL ) {
        // 这里假设函数指针是一个字符串表示的函数名
        void *fun_ptr = find_function_ptr_by_name(value.charptr.charptr_val);
        if (fun_ptr == NULL) {
            printf("Failed to find function pointer for: %s\n", value.charptr.charptr_val);
            return (void *)&result;
        }
        downloaded_files_hash.test_function = fun_ptr;
    }
    return (void *)&result;
}

downloaded_files_hash_cells_ret_t *get_downloaded_files_hash_cells_1_svc(struct svc_req *rqstp)
{
    static downloaded_files_hash_cells_ret_t result;
        if(downloaded_files_hash_cells==NULL) {
            result.downloaded_files_hash_cells.cell_rpc_ptr_len=0;
            result.downloaded_files_hash_cells.cell_rpc_ptr_val=NULL;
        } else {
            result.downloaded_files_hash_cells.cell_rpc_ptr_len=1;
            result.downloaded_files_hash_cells.cell_rpc_ptr_val=malloc(1*sizeof(cell_rpc));
            if(result.downloaded_files_hash_cells.cell_rpc_ptr_val==NULL) {
                perror("malloc failed");
                exit(1);
            }
            for(int i0=0;i0<result.downloaded_files_hash_cells.cell_rpc_ptr_len;i0++) {
    if (downloaded_files_hash_cells[i0].key == NULL) {result.downloaded_files_hash_cells.cell_rpc_ptr_val[i0].key.key_len = 0;result.downloaded_files_hash_cells.cell_rpc_ptr_val[i0].key.key_val = NULL;}

    else{result.downloaded_files_hash_cells.cell_rpc_ptr_val[i0].key.key_len = sizeof(downloaded_files_hash_cells[i0].key);result.downloaded_files_hash_cells.cell_rpc_ptr_val[i0].key.key_val = malloc(result.downloaded_files_hash_cells.cell_rpc_ptr_val[i0].key.key_len);

    	if (result.downloaded_files_hash_cells.cell_rpc_ptr_val[i0].key.key_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.downloaded_files_hash_cells.cell_rpc_ptr_val[i0].key.key_val, downloaded_files_hash_cells[i0].key, result.downloaded_files_hash_cells.cell_rpc_ptr_val[i0].key.key_len);}

    if (downloaded_files_hash_cells[i0].value == NULL) {result.downloaded_files_hash_cells.cell_rpc_ptr_val[i0].value.value_len = 0;result.downloaded_files_hash_cells.cell_rpc_ptr_val[i0].value.value_val = NULL;}

    else{result.downloaded_files_hash_cells.cell_rpc_ptr_val[i0].value.value_len = sizeof(downloaded_files_hash_cells[i0].value);result.downloaded_files_hash_cells.cell_rpc_ptr_val[i0].value.value_val = malloc(result.downloaded_files_hash_cells.cell_rpc_ptr_val[i0].value.value_len);

    	if (result.downloaded_files_hash_cells.cell_rpc_ptr_val[i0].value.value_val == NULL) {perror("malloc failed");exit(1);}
    	memcpy(result.downloaded_files_hash_cells.cell_rpc_ptr_val[i0].value.value_val, downloaded_files_hash_cells[i0].value, result.downloaded_files_hash_cells.cell_rpc_ptr_val[i0].value.value_len);}

            }
        }
    return &result;
}

void *set_downloaded_files_hash_cells_1_svc(cell_rpc_ptr value, struct svc_req *rqstp)
{
    static int result = 1;
    struct cell *local_downloaded_files_hash_cells=NULL;
    if(arg1.cell_rpc_ptr_val!=NULL) {
    local_downloaded_files_hash_cells=(struct cell *)malloc(arg1.cell_rpc_ptr_len*sizeof(struct cell));
    if(local_downloaded_files_hash_cells==NULL) {
    perror("malloc failed");
    exit(1);
    }
    for(int i0=0;i0<arg1.cell_rpc_ptr_len;i0++) {
    if (arg1.cell_rpc_ptr_val[i0].key.key_val == NULL) {local_downloaded_files_hash_cells[i0].key=NULL;}

    else {local_downloaded_files_hash_cells[i0].key=malloc(arg1.cell_rpc_ptr_val[i0].key.key_len);

    	if (local_downloaded_files_hash_cells[i0].key == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_downloaded_files_hash_cells[i0].key,arg1.cell_rpc_ptr_val[i0].key.key_val,arg1.cell_rpc_ptr_val[i0].key.key_len);

    }

    if (arg1.cell_rpc_ptr_val[i0].value.value_val == NULL) {local_downloaded_files_hash_cells[i0].value=NULL;}

    else {local_downloaded_files_hash_cells[i0].value=malloc(arg1.cell_rpc_ptr_val[i0].value.value_len);

    	if (local_downloaded_files_hash_cells[i0].value == NULL) {

    		perror("malloc failed");

    		exit(1);

    	}

    	memcpy(local_downloaded_files_hash_cells[i0].value,arg1.cell_rpc_ptr_val[i0].value.value_val,arg1.cell_rpc_ptr_val[i0].value.value_len);

    }

    	}
    }
    memcpy(&downloaded_files_hash_cells, &local_downloaded_files_hash_cells, sizeof(cell));
    return (void *)&result;
}

downloaded_files_hash_cells_key_ret_t *get_downloaded_files_hash_cells_key_1_svc(struct svc_req *rqstp)
{
    static downloaded_files_hash_cells_key_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 downloaded_files_hash_cells.key 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t downloaded_files_hash_cells_key_size; // 假设有这样的辅助变量
    if (downloaded_files_hash_cells.key == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = downloaded_files_hash_cells_key_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = downloaded_files_hash_cells.key;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t downloaded_files_hash_cells_key_size; // 假设有这样的辅助变量
void *set_downloaded_files_hash_cells_key_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (downloaded_files_hash_cells.key != NULL) {
        free(downloaded_files_hash_cells.key);
        downloaded_files_hash_cells.key = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        downloaded_files_hash_cells.key = malloc(value.bytes.bytes_len);
        if (downloaded_files_hash_cells.key == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(downloaded_files_hash_cells.key, value.bytes.bytes_val, value.bytes.bytes_len);
            downloaded_files_hash_cells_key_size = value.size;
        }
    }
    return (void *)&result;
}

downloaded_files_hash_cells_value_ret_t *get_downloaded_files_hash_cells_value_1_svc(struct svc_req *rqstp)
{
    static downloaded_files_hash_cells_value_ret_t result;
    // void* 指针不能直接获取大小，需要额外信息
    // 注意：客户端和服务端需要协商 downloaded_files_hash_cells.value 指向的数据大小
    // 此处使用全局变量表记录数据大小（如果可用）
    extern size_t downloaded_files_hash_cells_value_size; // 假设有这样的辅助变量
    if (downloaded_files_hash_cells.value == NULL) {
        result.value.bytes.bytes_len = 0;
        result.value.bytes.bytes_val = NULL;
        result.value.size = 0;
    } else {
        size_t data_size = downloaded_files_hash_cells_value_size;
        result.value.bytes.bytes_len = data_size;
        result.value.bytes.bytes_val = downloaded_files_hash_cells.value;
        result.value.size = data_size;
    }
    return &result;
}

    extern size_t downloaded_files_hash_cells_value_size; // 假设有这样的辅助变量
void *set_downloaded_files_hash_cells_value_1_svc(binary_data value, struct svc_req *rqstp)
{
    static int result = 1;
    // 释放旧内存
    if (downloaded_files_hash_cells.value != NULL) {
        free(downloaded_files_hash_cells.value);
        downloaded_files_hash_cells.value = NULL;
    }
    // 分配新内存并复制数据
    if (value.bytes.bytes_val != NULL && value.bytes.bytes_len > 0) {
        downloaded_files_hash_cells.value = malloc(value.bytes.bytes_len);
        if (downloaded_files_hash_cells.value == NULL) {
            perror("malloc failed");
            result = 0;
        } else {
            memcpy(downloaded_files_hash_cells.value, value.bytes.bytes_val, value.bytes.bytes_len);
            downloaded_files_hash_cells_value_size = value.size;
        }
    }
    return (void *)&result;
}

downloaded_files_hash_size_ret_t *get_downloaded_files_hash_size_1_svc(struct svc_req *rqstp)
{
    static downloaded_files_hash_size_ret_t result;
    result.value = downloaded_files_hash.size;
    return &result;
}

void *set_downloaded_files_hash_size_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    downloaded_files_hash.size = value;
    return (void *)&result;
}

downloaded_files_hash_count_ret_t *get_downloaded_files_hash_count_1_svc(struct svc_req *rqstp)
{
    static downloaded_files_hash_count_ret_t result;
    result.value = downloaded_files_hash.count;
    return &result;
}

void *set_downloaded_files_hash_count_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    downloaded_files_hash.count = value;
    return (void *)&result;
}

downloaded_files_hash_resize_threshold_ret_t *get_downloaded_files_hash_resize_threshold_1_svc(struct svc_req *rqstp)
{
    static downloaded_files_hash_resize_threshold_ret_t result;
    result.value = downloaded_files_hash.resize_threshold;
    return &result;
}

void *set_downloaded_files_hash_resize_threshold_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    downloaded_files_hash.resize_threshold = value;
    return (void *)&result;
}

downloaded_files_hash_prime_offset_ret_t *get_downloaded_files_hash_prime_offset_1_svc(struct svc_req *rqstp)
{
    static downloaded_files_hash_prime_offset_ret_t result;
    result.value = downloaded_files_hash.prime_offset;
    return &result;
}

void *set_downloaded_files_hash_prime_offset_1_svc(int value, struct svc_req *rqstp)
{
    static int result = 1;
    downloaded_files_hash.prime_offset = value;
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
int *gsetter_warc_current_file_number_postfix_1_svc( struct svc_req *rqstp)
{
   static int result;
 result= warc_current_file_number++;
    return &result;
}
unsigned int *gsetter_transport_map_modified_tick_prefix_1_svc(struct svc_req *rqstp)
{
    ++transport_map_modified_tick;
    return &transport_map_modified_tick;
}
int *gsetter_log_line_current_prefix_1_svc(struct svc_req *rqstp)
{
    ++log_line_current;
    return &log_line_current;
}
int *gsetter_numurls_postfix_1_svc( struct svc_req *rqstp)
{
   static int result;
 result= numurls++;
    return &result;
}
int *gsetter_numurls_prefix_1_svc(struct svc_req *rqstp)
{
    ++numurls;
    return &numurls;
}
