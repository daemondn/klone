#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <klone/session.h>
#include <klone/request.h>
#include <klone/response.h>
#include <klone/vars.h>
#include <klone/utils.h>
#include <klone/ses_prv.h>
#include <u/libu.h>
#include "klone_conf.h"

static int session_file_save(session_t *ss)
{
    io_t *io = NULL;

    /* delete old file (we'll rewrite it from scratch) */
    dbg_if(unlink(ss->filename));

    if(vars_count(ss->vars) == 0)
        return 0; /* nothing to save */

    // FIXME may be busy, must retry
    dbg_err_if(u_file_open(ss->filename, O_WRONLY | O_CREAT, &io));

    dbg_err_if(session_prv_save_to_io(ss, io));

    io_free(io);

    return 0;
err:
    if(io)
        io_free(io);
    return ~0;
}

static int session_file_load(session_t *ss)
{
    io_t *io = NULL;

    // FIXME may be busy, must retry
    dbg_err_if(u_file_open(ss->filename, O_RDONLY | O_CREAT, &io));

    dbg_err_if(session_prv_load_from_io(ss, io));

    io_free(io);

    return 0;
err:
    if(io)
        io_free(io);
    return ~0;
}

static int session_file_term(session_t *ss)
{
    u_unused_args(ss);
    return 0;
}

static int session_file_remove(session_t *ss)
{
    dbg_if(unlink(ss->filename));

    return 0;
}

int session_file_create(session_opt_t *so, request_t *rq, response_t *rs, 
        session_t **pss)
{
    session_t *ss = NULL;
    struct stat st;
    addr_t *addr;

    ss = u_zalloc(sizeof(session_t));
    dbg_err_if(ss == NULL);

    ss->load = session_file_load;
    ss->save = session_file_save;
    ss->remove = session_file_remove;
    ss->term = session_file_term;
    ss->so = so;

    dbg_err_if(session_prv_init(ss, rq, rs));

    if(stat(ss->filename, &st))
        ss->mtime = time(0); /* file not found or err */
    else
        ss->mtime = st.st_mtime;

    *pss = ss;

    return 0;
err:
    if(ss)
        session_free(ss);
    return ~0;
}

int session_file_module_term(session_opt_t *so)
{
    u_unused_args(so);
    return 0;
}

int session_file_module_init(u_config_t *config, session_opt_t *so)
{
    const char *v;

    if((v = u_config_get_subkey_value(config, "file.path")) == NULL)
    {
    #ifdef OS_WIN
        GetTempPath(U_FILENAME_MAX, so->path);
    #else
        strncpy(so->path, "/tmp", U_FILENAME_MAX);
    #endif
    } else
        strncpy(so->path, v, U_FILENAME_MAX);

    return 0;
err:
    return ~0;
}

