#ifndef EV3_UNIT_LOG_H_
#define EV3_UNIT_LOG_H_

#include <string.h>
#include "app.h"
#include "valdef.h"


class execlog_t{
public:
    void startup();
    void log_tsk();
    void finish();

    FILE *file;
    char logstr[LOGSTRLEN];
    long starttime;

private:
    const char LogFileName[LOGSTRLEN] = LOGFILENAME;
};

extern execlog_t execlog;

extern void _log(char *text);      
extern long getTime(long offset);   

#endif  // EV3_UNIT_LOG_H_