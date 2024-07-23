#include "Log.h"

execlog_t execlog;

void execlog_t::startup(){
    file = fopen(LogFileName,"w");
    if(file == NULL){
        printf("file open error\n");
    }
    starttime = getTime(0L);
}

void execlog_t::log_tsk(){
    fprintf(file, logstr);
}

void execlog_t::finish(){
    fclose(file);
}
long getTime(long offset){
	SYSTIM tim;
	get_tim(&tim);
	return ((long)tim - offset);
}
void _syslog(int level, char* text){
    syslog(level, text); 
}

void _log(char *text){
    _syslog(LOG_NOTICE, text);
}