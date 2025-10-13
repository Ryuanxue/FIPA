
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>

#include "nginx_rpc_wrapper.h"


ngx_rbtree_t              ngx_event_timer_rbtree;
static ngx_rbtree_node_t  ngx_event_timer_sentinel;

/*
 * the event timer rbtree may contain the duplicate keys, however,
 * it should not be a problem, because we use the rbtree to find
 * a minimum timer value only
 */

ngx_int_t ngx_event_timer_init(ngx_log_t *log)
{
  (&ngx_event_timer_sentinel)->color = 0;
  (&get_ngx_event_timer_rbtree_wrapper())->root = &ngx_event_timer_sentinel;
  (&get_ngx_event_timer_rbtree_wrapper())->sentinel = &ngx_event_timer_sentinel;
  (&get_ngx_event_timer_rbtree_wrapper())->insert = ngx_rbtree_insert_timer_value;
  return 0;
}




ngx_msec_t ngx_event_find_timer(void)
{
  ngx_msec_int_t timer;
  ngx_rbtree_node_t *node;
  ngx_rbtree_node_t *root;
  ngx_rbtree_node_t *sentinel;
  if (get_ngx_event_timer_rbtree_wrapper().root == (&ngx_event_timer_sentinel))
  {
    return (ngx_msec_t) (-1);
  }
  root = get_ngx_event_timer_rbtree_wrapper().root;
  sentinel = get_ngx_event_timer_rbtree_wrapper().sentinel;
  node = ngx_rbtree_min(root, sentinel);
  timer = (ngx_msec_int_t) (node->key - get_ngx_current_msec_wrapper());
  return (ngx_msec_t) ((timer > 0) ? (timer) : (0));
}




void ngx_event_expire_timers(void)
{
  ngx_event_t *ev;
  ngx_rbtree_node_t *node;
  ngx_rbtree_node_t *root;
  ngx_rbtree_node_t *sentinel;
  sentinel = get_ngx_event_timer_rbtree_wrapper().sentinel;
  for (;;)
  {
    root = get_ngx_event_timer_rbtree_wrapper().root;
    if (root == sentinel)
    {
      return;
    }
    node = ngx_rbtree_min(root, sentinel);
    if (((ngx_msec_int_t) (node->key - get_ngx_current_msec_wrapper())) > 0)
    {
      return;
    }
    ev = (ngx_event_t *) (((char *) node) - offsetof(ngx_event_t, timer));
    ;
    {
      ngx_rbtree_t temp_ngx_event_timer_rbtree = get_ngx_event_timer_rbtree_wrapper();
      ngx_rbtree_delete(&temp_ngx_event_timer_rbtree, &ev->timer);
      set_ngx_event_timer_rbtree_wrapper(temp_ngx_event_timer_rbtree);
    }
    ev->timer_set = 0;
    ev->timedout = 1;
    ev->handler(ev);
  }

}




ngx_int_t ngx_event_no_timers_left(void)
{
  ngx_event_t *ev;
  ngx_rbtree_node_t *node;
  ngx_rbtree_node_t *root;
  ngx_rbtree_node_t *sentinel;
  sentinel = get_ngx_event_timer_rbtree_wrapper().sentinel;
  root = get_ngx_event_timer_rbtree_wrapper().root;
  if (root == sentinel)
  {
    return 0;
  }
  for (node = ngx_rbtree_min(root, sentinel); node;   {
    ngx_rbtree_t temp_ngx_event_timer_rbtree = get_ngx_event_timer_rbtree_wrapper();
    ngx_rbtree_node_t * temp_result_0 = ngx_rbtree_next(&temp_ngx_event_timer_rbtree, node);
    set_ngx_event_timer_rbtree_wrapper(temp_ngx_event_timer_rbtree);
    node = temp_result_0;
  }
)
  {
    ev = (ngx_event_t *) (((char *) node) - offsetof(ngx_event_t, timer));
    if (!ev->cancelable)
    {
      return -2;
    }
  }

  return 0;
}


