
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>
#include <ngx_channel.h>

#include "nginx_rpc_wrapper.h"


static void ngx_start_worker_processes(ngx_cycle_t *cycle, ngx_int_t n,
    ngx_int_t type);
static void ngx_start_cache_manager_processes(ngx_cycle_t *cycle,
    ngx_uint_t respawn);
static void ngx_pass_open_channel(ngx_cycle_t *cycle, ngx_channel_t *ch);
static void ngx_signal_worker_processes(ngx_cycle_t *cycle, int signo);
static ngx_uint_t ngx_reap_children(ngx_cycle_t *cycle);
static void ngx_master_process_exit(ngx_cycle_t *cycle);
static void ngx_worker_process_cycle(ngx_cycle_t *cycle, void *data);
static void ngx_worker_process_init(ngx_cycle_t *cycle, ngx_int_t worker);
static void ngx_worker_process_exit(ngx_cycle_t *cycle);
static void ngx_channel_handler(ngx_event_t *ev);
static void ngx_cache_manager_process_cycle(ngx_cycle_t *cycle, void *data);
static void ngx_cache_manager_process_handler(ngx_event_t *ev);
static void ngx_cache_loader_process_handler(ngx_event_t *ev);


ngx_uint_t    ngx_process;
ngx_uint_t    ngx_worker;
ngx_pid_t     ngx_pid;
ngx_pid_t     ngx_parent;

sig_atomic_t  ngx_reap;
sig_atomic_t  ngx_sigio;
sig_atomic_t  ngx_sigalrm;
sig_atomic_t  ngx_terminate;
sig_atomic_t  ngx_quit;
sig_atomic_t  ngx_debug_quit;
ngx_uint_t    ngx_exiting;
sig_atomic_t  ngx_reconfigure;
sig_atomic_t  ngx_reopen;

sig_atomic_t  ngx_change_binary;
ngx_pid_t     ngx_new_binary;
ngx_uint_t    ngx_inherited;
ngx_uint_t    ngx_daemonized;

sig_atomic_t  ngx_noaccept;
ngx_uint_t    ngx_noaccepting;
ngx_uint_t    ngx_restart;


static u_char  master_process[] = "master process";


static ngx_cache_manager_ctx_t  ngx_cache_manager_ctx = {
    ngx_cache_manager_process_handler, "cache manager process", 0
};

static ngx_cache_manager_ctx_t  ngx_cache_loader_ctx = {
    ngx_cache_loader_process_handler, "cache loader process", 60000
};


static ngx_cycle_t      ngx_exit_cycle;
static ngx_log_t        ngx_exit_log;
static ngx_open_file_t  ngx_exit_log_file;


void ngx_master_process_cycle(ngx_cycle_t *cycle)
{
  char *title;
  u_char *p;
  size_t size;
  ngx_int_t i;
  ngx_uint_t n;
  ngx_uint_t sigio;
  sigset_t set;
  struct itimerval itv;
  ngx_uint_t live;
  ngx_msec_t delay;
  ngx_listening_t *ls;
  ngx_core_conf_t *ccf;
  sigemptyset(&set);
  sigaddset(&set, SIGCHLD);
  sigaddset(&set, SIGALRM);
  sigaddset(&set, SIGIO);
  sigaddset(&set, SIGINT);
  sigaddset(&set, SIGHUP);
  sigaddset(&set, SIGUSR1);
  sigaddset(&set, 28);
  sigaddset(&set, SIGTERM);
  sigaddset(&set, SIGQUIT);
  sigaddset(&set, SIGUSR2);
  if (sigprocmask(SIG_BLOCK, &set, 0) == (-1))
  {
    if (cycle->log->log_level >= 2)
      ngx_log_error_core(2, cycle->log, errno, "sigprocmask() failed");
  }
  sigemptyset(&set);
  size = sizeof(master_process);
  for (i = 0; i < ngx_argc; i++)
  {
    size += strlen((const char *) ngx_argv[i]) + 1;
  }

  title = ngx_pnalloc(cycle->pool, size);
  if (title == 0)
  {
    exit(2);
  }
  p = ((u_char *) memcpy(title, master_process, (sizeof(master_process)) - 1)) + ((sizeof(master_process)) - 1);
  for (i = 0; i < ngx_argc; i++)
  {
    *(p++) = ' ';
    p = ngx_cpystrn(p, (u_char *) ngx_argv[i], size);
  }

  ngx_setproctitle(title);
  ccf = (ngx_core_conf_t *) cycle->conf_ctx[ngx_core_module.index];
  ngx_start_worker_processes(cycle, ccf->worker_processes, -3);
  ngx_start_cache_manager_processes(cycle, 0);
  ngx_new_binary = 0;
  delay = 0;
  sigio = 0;
  live = 1;
  for (;;)
  {
    if (delay)
    {
      if (ngx_sigalrm)
      {
        sigio = 0;
        delay *= 2;
        ngx_sigalrm = 0;
      }
      ;
      itv.it_interval.tv_sec = 0;
      itv.it_interval.tv_usec = 0;
      itv.it_value.tv_sec = delay / 1000;
      itv.it_value.tv_usec = (delay % 1000) * 1000;
      if (setitimer(ITIMER_REAL, &itv, 0) == (-1))
      {
        if (cycle->log->log_level >= 2)
          ngx_log_error_core(2, cycle->log, errno, "setitimer() failed");
      }
    }
    ;
    sigsuspend(&set);
    ngx_time_update();
    ;
    if (ngx_reap)
    {
      ngx_reap = 0;
      ;
      live = ngx_reap_children(cycle);
    }
    if ((!live) && (get_ngx_terminate_wrapper() || ngx_quit))
    {
      ngx_master_process_exit(cycle);
    }
    if (get_ngx_terminate_wrapper())
    {
      if (delay == 0)
      {
        delay = 50;
      }
      if (sigio)
      {
        sigio--;
        continue;
      }
      sigio = ccf->worker_processes + 2;
      if (delay > 1000)
      {
        ngx_signal_worker_processes(cycle, SIGKILL);
      }
      else
      {
        ngx_signal_worker_processes(cycle, SIGTERM);
      }
      continue;
    }
    if (ngx_quit)
    {
      ngx_signal_worker_processes(cycle, SIGQUIT);
      ls = cycle->listening.elts;
      for (n = 0; n < cycle->listening.nelts; n++)
      {
        if (close(ls[n].fd) == (-1))
        {
          if (cycle->log->log_level >= 1)
            ngx_log_error_core(1, cycle->log, errno, "close() socket %V failed", &ls[n].addr_text);
        }
      }

      cycle->listening.nelts = 0;
      continue;
    }
    if (ngx_reconfigure)
    {
      ngx_reconfigure = 0;
      if (ngx_new_binary)
      {
        ngx_start_worker_processes(cycle, ccf->worker_processes, -3);
        ngx_start_cache_manager_processes(cycle, 0);
        ngx_noaccepting = 0;
        continue;
      }
      if (cycle->log->log_level >= 6)
        ngx_log_error_core(6, cycle->log, 0, "reconfiguring");
      cycle = ngx_init_cycle(cycle);
      if (cycle == 0)
      {
        cycle = (ngx_cycle_t *) get_ngx_cycle_wrapper();
        continue;
      }
      set_ngx_cycle_wrapper(cycle);
      ccf = (ngx_core_conf_t *) cycle->conf_ctx[ngx_core_module.index];
      ngx_start_worker_processes(cycle, ccf->worker_processes, -4);
      ngx_start_cache_manager_processes(cycle, 1);
      (void) usleep(100 * 1000);
      live = 1;
      ngx_signal_worker_processes(cycle, SIGQUIT);
    }
    if (ngx_restart)
    {
      ngx_restart = 0;
      ngx_start_worker_processes(cycle, ccf->worker_processes, -3);
      ngx_start_cache_manager_processes(cycle, 0);
      live = 1;
    }
    if (ngx_reopen)
    {
      ngx_reopen = 0;
      if (cycle->log->log_level >= 6)
        ngx_log_error_core(6, cycle->log, 0, "reopening logs");
      ngx_reopen_files(cycle, ccf->user);
      ngx_signal_worker_processes(cycle, SIGUSR1);
    }
    if (ngx_change_binary)
    {
      ngx_change_binary = 0;
      if (cycle->log->log_level >= 6)
        ngx_log_error_core(6, cycle->log, 0, "changing binary");
      ngx_new_binary = ngx_exec_new_binary(cycle, ngx_argv);
    }
    if (ngx_noaccept)
    {
      ngx_noaccept = 0;
      ngx_noaccepting = 1;
      ngx_signal_worker_processes(cycle, SIGQUIT);
    }
  }

}




void ngx_single_process_cycle(ngx_cycle_t *cycle)
{
  ngx_uint_t i;
  if (ngx_set_environment(cycle, 0) == 0)
  {
    exit(2);
  }
  for (i = 0; cycle->modules[i]; i++)
  {
    if (cycle->modules[i]->init_process)
    {
      if (cycle->modules[i]->init_process(cycle) == (-1))
      {
        exit(2);
      }
    }
  }

  for (;;)
  {
    ;
    ngx_process_events_and_timers(cycle);
    if (get_ngx_terminate_wrapper() || ngx_quit)
    {
      for (i = 0; cycle->modules[i]; i++)
      {
        if (cycle->modules[i]->exit_process)
        {
          cycle->modules[i]->exit_process(cycle);
        }
      }

      ngx_master_process_exit(cycle);
    }
    if (ngx_reconfigure)
    {
      ngx_reconfigure = 0;
      if (cycle->log->log_level >= 6)
        ngx_log_error_core(6, cycle->log, 0, "reconfiguring");
      cycle = ngx_init_cycle(cycle);
      if (cycle == 0)
      {
        cycle = (ngx_cycle_t *) get_ngx_cycle_wrapper();
        continue;
      }
      set_ngx_cycle_wrapper(cycle);
    }
    if (ngx_reopen)
    {
      ngx_reopen = 0;
      if (cycle->log->log_level >= 6)
        ngx_log_error_core(6, cycle->log, 0, "reopening logs");
      ngx_reopen_files(cycle, (ngx_uid_t) (-1));
    }
  }

}




static void
ngx_start_worker_processes(ngx_cycle_t *cycle, ngx_int_t n, ngx_int_t type)
{
    ngx_int_t      i;
    ngx_channel_t  ch;

    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "start worker processes");

    ngx_memzero(&ch, sizeof(ngx_channel_t));

    ch.command = NGX_CMD_OPEN_CHANNEL;

    for (i = 0; i < n; i++) {

        ngx_spawn_process(cycle, ngx_worker_process_cycle,
                          (void *) (intptr_t) i, "worker process", type);

        ch.pid = ngx_processes[ngx_process_slot].pid;
        ch.slot = ngx_process_slot;
        ch.fd = ngx_processes[ngx_process_slot].channel[0];

        ngx_pass_open_channel(cycle, &ch);
    }
}


static void ngx_start_cache_manager_processes(ngx_cycle_t *cycle, ngx_uint_t respawn)
{
  ngx_uint_t i;
  ngx_uint_t manager;
  ngx_uint_t loader;
  ngx_path_t **path;
  ngx_channel_t ch;
  manager = 0;
  loader = 0;
  path = get_ngx_cycle_wrapper()->paths.elts;
  for (i = 0; i < get_ngx_cycle_wrapper()->paths.nelts; i++)
  {
    if (path[i]->manager)
    {
      manager = 1;
    }
    if (path[i]->loader)
    {
      loader = 1;
    }
  }

  if (manager == 0)
  {
    return;
  }
  ngx_spawn_process(cycle, ngx_cache_manager_process_cycle, &ngx_cache_manager_ctx, "cache manager process", (respawn) ? (-4) : (-3));
  (void) memset(&ch, 0, sizeof(ngx_channel_t));
  ch.command = 1;
  ch.pid = ngx_processes[ngx_process_slot].pid;
  ch.slot = ngx_process_slot;
  ch.fd = ngx_processes[ngx_process_slot].channel[0];
  ngx_pass_open_channel(cycle, &ch);
  if (loader == 0)
  {
    return;
  }
  ngx_spawn_process(cycle, ngx_cache_manager_process_cycle, &ngx_cache_loader_ctx, "cache loader process", (respawn) ? (-2) : (-1));
  ch.command = 1;
  ch.pid = ngx_processes[ngx_process_slot].pid;
  ch.slot = ngx_process_slot;
  ch.fd = ngx_processes[ngx_process_slot].channel[0];
  ngx_pass_open_channel(cycle, &ch);
}




static void
ngx_pass_open_channel(ngx_cycle_t *cycle, ngx_channel_t *ch)
{
    ngx_int_t  i;

    for (i = 0; i < ngx_last_process; i++) {

        if (i == ngx_process_slot
            || ngx_processes[i].pid == -1
            || ngx_processes[i].channel[0] == -1)
        {
            continue;
        }

        ngx_log_debug6(NGX_LOG_DEBUG_CORE, cycle->log, 0,
                      "pass channel s:%i pid:%P fd:%d to s:%i pid:%P fd:%d",
                      ch->slot, ch->pid, ch->fd,
                      i, ngx_processes[i].pid,
                      ngx_processes[i].channel[0]);

        /* TODO: NGX_AGAIN */

        ngx_write_channel(ngx_processes[i].channel[0],
                          ch, sizeof(ngx_channel_t), cycle->log);
    }
}


static void
ngx_signal_worker_processes(ngx_cycle_t *cycle, int signo)
{
    ngx_int_t      i;
    ngx_err_t      err;
    ngx_channel_t  ch;

    ngx_memzero(&ch, sizeof(ngx_channel_t));

#if (NGX_BROKEN_SCM_RIGHTS)

    ch.command = 0;

#else

    switch (signo) {

    case ngx_signal_value(NGX_SHUTDOWN_SIGNAL):
        ch.command = NGX_CMD_QUIT;
        break;

    case ngx_signal_value(NGX_TERMINATE_SIGNAL):
        ch.command = NGX_CMD_TERMINATE;
        break;

    case ngx_signal_value(NGX_REOPEN_SIGNAL):
        ch.command = NGX_CMD_REOPEN;
        break;

    default:
        ch.command = 0;
    }

#endif

    ch.fd = -1;


    for (i = 0; i < ngx_last_process; i++) {

        ngx_log_debug7(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                       "child: %i %P e:%d t:%d d:%d r:%d j:%d",
                       i,
                       ngx_processes[i].pid,
                       ngx_processes[i].exiting,
                       ngx_processes[i].exited,
                       ngx_processes[i].detached,
                       ngx_processes[i].respawn,
                       ngx_processes[i].just_spawn);

        if (ngx_processes[i].detached || ngx_processes[i].pid == -1) {
            continue;
        }

        if (ngx_processes[i].just_spawn) {
            ngx_processes[i].just_spawn = 0;
            continue;
        }

        if (ngx_processes[i].exiting
            && signo == ngx_signal_value(NGX_SHUTDOWN_SIGNAL))
        {
            continue;
        }

        if (ch.command) {
            if (ngx_write_channel(ngx_processes[i].channel[0],
                                  &ch, sizeof(ngx_channel_t), cycle->log)
                == NGX_OK)
            {
                if (signo != ngx_signal_value(NGX_REOPEN_SIGNAL)) {
                    ngx_processes[i].exiting = 1;
                }

                continue;
            }
        }

        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cycle->log, 0,
                       "kill (%P, %d)", ngx_processes[i].pid, signo);

        if (kill(ngx_processes[i].pid, signo) == -1) {
            err = ngx_errno;
            ngx_log_error(NGX_LOG_ALERT, cycle->log, err,
                          "kill(%P, %d) failed", ngx_processes[i].pid, signo);

            if (err == NGX_ESRCH) {
                ngx_processes[i].exited = 1;
                ngx_processes[i].exiting = 0;
                ngx_reap = 1;
            }

            continue;
        }

        if (signo != ngx_signal_value(NGX_REOPEN_SIGNAL)) {
            ngx_processes[i].exiting = 1;
        }
    }
}


static ngx_uint_t ngx_reap_children(ngx_cycle_t *cycle)
{
  ngx_int_t i;
  ngx_int_t n;
  ngx_uint_t live;
  ngx_channel_t ch;
  ngx_core_conf_t *ccf;
  (void) memset(&ch, 0, sizeof(ngx_channel_t));
  ch.command = 2;
  ch.fd = -1;
  live = 0;
  for (i = 0; i < ngx_last_process; i++)
  {
    ;
    if (ngx_processes[i].pid == (-1))
    {
      continue;
    }
    if (ngx_processes[i].exited)
    {
      if (!ngx_processes[i].detached)
      {
        ngx_close_channel(ngx_processes[i].channel, cycle->log);
        ngx_processes[i].channel[0] = -1;
        ngx_processes[i].channel[1] = -1;
        ch.pid = ngx_processes[i].pid;
        ch.slot = i;
        for (n = 0; n < ngx_last_process; n++)
        {
          if ((ngx_processes[n].exited || (ngx_processes[n].pid == (-1))) || (ngx_processes[n].channel[0] == (-1)))
          {
            continue;
          }
          ;
          ngx_write_channel(ngx_processes[n].channel[0], &ch, sizeof(ngx_channel_t), cycle->log);
        }

      }
      if (((ngx_processes[i].respawn && (!ngx_processes[i].exiting)) && (!get_ngx_terminate_wrapper())) && (!ngx_quit))
      {
        if (ngx_spawn_process(cycle, ngx_processes[i].proc, ngx_processes[i].data, ngx_processes[i].name, i) == (-1))
        {
          if (cycle->log->log_level >= 2)
            ngx_log_error_core(2, cycle->log, 0, "could not respawn %s", ngx_processes[i].name);
          continue;
        }
        ch.command = 1;
        ch.pid = ngx_processes[ngx_process_slot].pid;
        ch.slot = ngx_process_slot;
        ch.fd = ngx_processes[ngx_process_slot].channel[0];
        ngx_pass_open_channel(cycle, &ch);
        live = 1;
        continue;
      }
      if (ngx_processes[i].pid == ngx_new_binary)
      {
        ccf = (ngx_core_conf_t *) cycle->conf_ctx[ngx_core_module.index];
        if (rename((const char *) ((char *) ccf->oldpid.data), (const char *) ((char *) ccf->pid.data)) == (-1))
        {
          if (cycle->log->log_level >= 2)
            ngx_log_error_core(2, cycle->log, errno, "rename() %s back to %s failed after the new binary process \"%s\" exited", ccf->oldpid.data, ccf->pid.data, ngx_argv[0]);
        }
        ngx_new_binary = 0;
        if (ngx_noaccepting)
        {
          ngx_restart = 1;
          ngx_noaccepting = 0;
        }
      }
      if (i == (ngx_last_process - 1))
      {
        ngx_last_process--;
      }
      else
      {
        ngx_processes[i].pid = -1;
      }
    }
    else
      if (ngx_processes[i].exiting || (!ngx_processes[i].detached))
    {
      live = 1;
    }
  }

  return live;
}




static void ngx_master_process_exit(ngx_cycle_t *cycle)
{
  ngx_uint_t i;
  ngx_delete_pidfile(cycle);
  if (cycle->log->log_level >= 6)
    ngx_log_error_core(6, cycle->log, 0, "exit");
  for (i = 0; cycle->modules[i]; i++)
  {
    if (cycle->modules[i]->exit_master)
    {
      cycle->modules[i]->exit_master(cycle);
    }
  }

  ngx_close_listening_sockets(cycle);
  ngx_exit_log = *ngx_log_get_file_log(get_ngx_cycle_wrapper()->log);
  ngx_exit_log_file.fd = ngx_exit_log.file->fd;
  ngx_exit_log.file = &ngx_exit_log_file;
  ngx_exit_log.next = 0;
  ngx_exit_log.writer = 0;
  ngx_exit_cycle.log = &ngx_exit_log;
  ngx_exit_cycle.files = get_ngx_cycle_wrapper()->files;
  ngx_exit_cycle.files_n = get_ngx_cycle_wrapper()->files_n;
  set_ngx_cycle_wrapper(&ngx_exit_cycle);
  ngx_destroy_pool(cycle->pool);
  exit(0);
}




static void ngx_worker_process_cycle(ngx_cycle_t *cycle, void *data)
{
  ngx_int_t worker = (intptr_t) data;
  ngx_process = 3;
  ngx_worker = worker;
  ngx_worker_process_init(cycle, worker);
  ngx_setproctitle("worker process");
  for (;;)
  {
    if (get_ngx_exiting_wrapper())
    {
      if (ngx_event_no_timers_left() == 0)
      {
        if (cycle->log->log_level >= 6)
          ngx_log_error_core(6, cycle->log, 0, "exiting");
        ngx_worker_process_exit(cycle);
      }
    }
    ;
    ngx_process_events_and_timers(cycle);
    if (get_ngx_terminate_wrapper())
    {
      if (cycle->log->log_level >= 6)
        ngx_log_error_core(6, cycle->log, 0, "exiting");
      ngx_worker_process_exit(cycle);
    }
    if (ngx_quit)
    {
      ngx_quit = 0;
      if (cycle->log->log_level >= 6)
        ngx_log_error_core(6, cycle->log, 0, "gracefully shutting down");
      ngx_setproctitle("worker process is shutting down");
      if (!get_ngx_exiting_wrapper())
      {
        set_ngx_exiting_wrapper(1);
        ngx_set_shutdown_timer(cycle);
        ngx_close_listening_sockets(cycle);
        ngx_close_idle_connections(cycle);
      }
    }
    if (ngx_reopen)
    {
      ngx_reopen = 0;
      if (cycle->log->log_level >= 6)
        ngx_log_error_core(6, cycle->log, 0, "reopening logs");
      ngx_reopen_files(cycle, -1);
    }
  }

}




static void ngx_worker_process_init(ngx_cycle_t *cycle, ngx_int_t worker)
{
  sigset_t set;
  ngx_int_t n;
  ngx_time_t *tp;
  ngx_uint_t i;
  ngx_cpuset_t *cpu_affinity;
  struct rlimit rlmt;
  ngx_core_conf_t *ccf;
  ngx_listening_t *ls;
  if (ngx_set_environment(cycle, 0) == 0)
  {
    exit(2);
  }
  ccf = (ngx_core_conf_t *) cycle->conf_ctx[ngx_core_module.index];
  if ((worker >= 0) && (ccf->priority != 0))
  {
    if (setpriority(PRIO_PROCESS, 0, ccf->priority) == (-1))
    {
      if (cycle->log->log_level >= 2)
        ngx_log_error_core(2, cycle->log, errno, "setpriority(%d) failed", ccf->priority);
    }
  }
  if (ccf->rlimit_nofile != (-1))
  {
    rlmt.rlim_cur = (rlim_t) ccf->rlimit_nofile;
    rlmt.rlim_max = (rlim_t) ccf->rlimit_nofile;
    if (setrlimit(RLIMIT_NOFILE, &rlmt) == (-1))
    {
      if (cycle->log->log_level >= 2)
        ngx_log_error_core(2, cycle->log, errno, "setrlimit(RLIMIT_NOFILE, %i) failed", ccf->rlimit_nofile);
    }
  }
  if (ccf->rlimit_core != (-1))
  {
    rlmt.rlim_cur = (rlim_t) ccf->rlimit_core;
    rlmt.rlim_max = (rlim_t) ccf->rlimit_core;
    if (setrlimit(RLIMIT_CORE, &rlmt) == (-1))
    {
      if (cycle->log->log_level >= 2)
        ngx_log_error_core(2, cycle->log, errno, "setrlimit(RLIMIT_CORE, %O) failed", ccf->rlimit_core);
    }
  }
  if (geteuid() == 0)
  {
    if (setgid(ccf->group) == (-1))
    {
      if (cycle->log->log_level >= 1)
        ngx_log_error_core(1, cycle->log, errno, "setgid(%d) failed", ccf->group);
      exit(2);
    }
    if (initgroups(ccf->username, ccf->group) == (-1))
    {
      if (cycle->log->log_level >= 1)
        ngx_log_error_core(1, cycle->log, errno, "initgroups(%s, %d) failed", ccf->username, ccf->group);
    }
    if (ccf->transparent && ccf->user)
    {
      if (prctl(PR_SET_KEEPCAPS, 1, 0, 0, 0) == (-1))
      {
        if (cycle->log->log_level >= 1)
          ngx_log_error_core(1, cycle->log, errno, "prctl(PR_SET_KEEPCAPS, 1) failed");
        exit(2);
      }
    }
    if (setuid(ccf->user) == (-1))
    {
      if (cycle->log->log_level >= 1)
        ngx_log_error_core(1, cycle->log, errno, "setuid(%d) failed", ccf->user);
      exit(2);
    }
    if (ccf->transparent && ccf->user)
    {
      struct __user_cap_data_struct data;
      struct __user_cap_header_struct header;
      (void) memset(&header, 0, sizeof(struct __user_cap_header_struct));
      (void) memset(&data, 0, sizeof(struct __user_cap_data_struct));
      header.version = 0x19980330;
      data.effective = 1U << (13 & 31);
      data.permitted = data.effective;
      if (syscall(126, &header, &data) == (-1))
      {
        if (cycle->log->log_level >= 1)
          ngx_log_error_core(1, cycle->log, errno, "capset() failed");
        exit(2);
      }
    }
  }
  if (worker >= 0)
  {
    cpu_affinity = ngx_get_cpu_affinity(worker);
    if (cpu_affinity)
    {
      ngx_setaffinity(cpu_affinity, cycle->log);
    }
  }
  if (prctl(PR_SET_DUMPABLE, 1, 0, 0, 0) == (-1))
  {
    if (cycle->log->log_level >= 2)
      ngx_log_error_core(2, cycle->log, errno, "prctl(PR_SET_DUMPABLE) failed");
  }
  if (ccf->working_directory.len)
  {
    if (chdir((char *) ccf->working_directory.data) == (-1))
    {
      if (cycle->log->log_level >= 2)
        ngx_log_error_core(2, cycle->log, errno, "chdir(\"%s\") failed", ccf->working_directory.data);
      exit(2);
    }
  }
  sigemptyset(&set);
  if (sigprocmask(SIG_SETMASK, &set, 0) == (-1))
  {
    if (cycle->log->log_level >= 2)
      ngx_log_error_core(2, cycle->log, errno, "sigprocmask() failed");
  }
  tp = (ngx_time_t *) get_ngx_cached_time_wrapper();
  srandom(((((unsigned) get_ngx_pid_wrapper()) << 16) ^ tp->sec) ^ tp->msec);
  ls = cycle->listening.elts;
  for (i = 0; i < cycle->listening.nelts; i++)
  {
    ls[i].previous = 0;
  }

  for (i = 0; cycle->modules[i]; i++)
  {
    if (cycle->modules[i]->init_process)
    {
      if (cycle->modules[i]->init_process(cycle) == (-1))
      {
        exit(2);
      }
    }
  }

  for (n = 0; n < ngx_last_process; n++)
  {
    if (ngx_processes[n].pid == (-1))
    {
      continue;
    }
    if (n == ngx_process_slot)
    {
      continue;
    }
    if (ngx_processes[n].channel[1] == (-1))
    {
      continue;
    }
    if (close(ngx_processes[n].channel[1]) == (-1))
    {
      if (cycle->log->log_level >= 2)
        ngx_log_error_core(2, cycle->log, errno, "close() channel failed");
    }
  }

  if (close(ngx_processes[ngx_process_slot].channel[0]) == (-1))
  {
    if (cycle->log->log_level >= 2)
      ngx_log_error_core(2, cycle->log, errno, "close() channel failed");
  }
  if (ngx_add_channel_event(cycle, ngx_channel, EPOLLIN | EPOLLRDHUP, ngx_channel_handler) == (-1))
  {
    exit(2);
  }
}




static void ngx_worker_process_exit(ngx_cycle_t *cycle)
{
  ngx_uint_t i;
  ngx_connection_t *c;
  for (i = 0; cycle->modules[i]; i++)
  {
    if (cycle->modules[i]->exit_process)
    {
      cycle->modules[i]->exit_process(cycle);
    }
  }

  if (get_ngx_exiting_wrapper())
  {
    c = cycle->connections;
    for (i = 0; i < cycle->connection_n; i++)
    {
      if (((((c[i].fd != (-1)) && c[i].read) && (!c[i].read->accept)) && (!c[i].read->channel)) && (!c[i].read->resolver))
      {
        if (cycle->log->log_level >= 2)
          ngx_log_error_core(2, cycle->log, 0, "*%uA open socket #%d left in connection %ui", c[i].number, c[i].fd, i);
        ngx_debug_quit = 1;
      }
    }

    if (ngx_debug_quit)
    {
      if (cycle->log->log_level >= 2)
        ngx_log_error_core(2, cycle->log, 0, "aborting");
      ngx_debug_point();
    }
  }
  ngx_exit_log = *ngx_log_get_file_log(get_ngx_cycle_wrapper()->log);
  ngx_exit_log_file.fd = ngx_exit_log.file->fd;
  ngx_exit_log.file = &ngx_exit_log_file;
  ngx_exit_log.next = 0;
  ngx_exit_log.writer = 0;
  ngx_exit_cycle.log = &ngx_exit_log;
  ngx_exit_cycle.files = get_ngx_cycle_wrapper()->files;
  ngx_exit_cycle.files_n = get_ngx_cycle_wrapper()->files_n;
  set_ngx_cycle_wrapper(&ngx_exit_cycle);
  ngx_destroy_pool(cycle->pool);
  if (get_ngx_cycle_wrapper()->log->log_level >= 6)
    ngx_log_error_core(6, get_ngx_cycle_wrapper()->log, 0, "exit");
  exit(0);
}




static void ngx_channel_handler(ngx_event_t *ev)
{
  ngx_int_t n;
  ngx_channel_t ch;
  ngx_connection_t *c;
  if (ev->timedout)
  {
    ev->timedout = 0;
    return;
  }
  c = ev->data;
  ;
  for (;;)
  {
    n = ngx_read_channel(c->fd, &ch, sizeof(ngx_channel_t), ev->log);
    ;
    if (n == (-1))
    {
      if (get_ngx_event_flags_wrapper() & 0x00000040)
      {
        ngx_event_actions.del_conn(c, 0);
      }
      ngx_close_connection(c);
      return;
    }
    if (get_ngx_event_flags_wrapper() & 0x00001000)
    {
      if (ngx_event_actions.add(ev, EPOLLIN | EPOLLRDHUP, 0) == (-1))
      {
        return;
      }
    }
    if (n == (-2))
    {
      return;
    }
    ;
    switch (ch.command)
    {
      case 3:
        ngx_quit = 1;
        break;

      case 4:
        set_ngx_terminate_wrapper(1);
        break;

      case 5:
        ngx_reopen = 1;
        break;

      case 1:
        ;
        ngx_processes[ch.slot].pid = ch.pid;
        ngx_processes[ch.slot].channel[0] = ch.fd;
        break;

      case 2:
        ;
        if (close(ngx_processes[ch.slot].channel[0]) == (-1))
      {
        if (ev->log->log_level >= 2)
          ngx_log_error_core(2, ev->log, errno, "close() channel failed");
      }
        ngx_processes[ch.slot].channel[0] = -1;
        break;

    }

  }

}




static void ngx_cache_manager_process_cycle(ngx_cycle_t *cycle, void *data)
{
  ngx_cache_manager_ctx_t *ctx = data;
  void *ident[4];
  ngx_event_t ev;
  ngx_process = 4;
  ngx_close_listening_sockets(cycle);
  cycle->connection_n = 512;
  ngx_worker_process_init(cycle, -1);
  (void) memset(&ev, 0, sizeof(ngx_event_t));
  ev.handler = ctx->handler;
  ev.data = ident;
  ev.log = cycle->log;
  ident[3] = (void *) (-1);
  ngx_use_accept_mutex = 0;
  ngx_setproctitle(ctx->name);
  ngx_event_add_timer(&ev, ctx->delay);
  for (;;)
  {
    if (get_ngx_terminate_wrapper() || ngx_quit)
    {
      if (cycle->log->log_level >= 6)
        ngx_log_error_core(6, cycle->log, 0, "exiting");
      exit(0);
    }
    if (ngx_reopen)
    {
      ngx_reopen = 0;
      if (cycle->log->log_level >= 6)
        ngx_log_error_core(6, cycle->log, 0, "reopening logs");
      ngx_reopen_files(cycle, -1);
    }
    ngx_process_events_and_timers(cycle);
  }

}




static void ngx_cache_manager_process_handler(ngx_event_t *ev)
{
  ngx_uint_t i;
  ngx_msec_t next;
  ngx_msec_t n;
  ngx_path_t **path;
  next = (60 * 60) * 1000;
  path = get_ngx_cycle_wrapper()->paths.elts;
  for (i = 0; i < get_ngx_cycle_wrapper()->paths.nelts; i++)
  {
    if (path[i]->manager)
    {
      n = path[i]->manager(path[i]->data);
      next = (n <= next) ? (n) : (next);
      ngx_time_update();
    }
  }

  if (next == 0)
  {
    next = 1;
  }
  ngx_event_add_timer(ev, next);
}




static void ngx_cache_loader_process_handler(ngx_event_t *ev)
{
  ngx_uint_t i;
  ngx_path_t **path;
  ngx_cycle_t *cycle;
  cycle = (ngx_cycle_t *) get_ngx_cycle_wrapper();
  path = cycle->paths.elts;
  for (i = 0; i < cycle->paths.nelts; i++)
  {
    if (get_ngx_terminate_wrapper() || ngx_quit)
    {
      break;
    }
    if (path[i]->loader)
    {
      path[i]->loader(path[i]->data);
      ngx_time_update();
    }
  }

  exit(0);
}


