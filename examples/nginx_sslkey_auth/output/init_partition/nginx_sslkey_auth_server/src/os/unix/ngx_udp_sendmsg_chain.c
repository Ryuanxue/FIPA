
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>


static ngx_chain_t *ngx_udp_output_chain_to_iovec(ngx_iovec_t *vec,
    ngx_chain_t *in, ngx_log_t *log);
static ssize_t ngx_sendmsg(ngx_connection_t *c, ngx_iovec_t *vec);






