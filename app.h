#ifdef __cplusplus
extern "C" {
#endif

#include "ev3api.h"





#ifndef STACK_SIZE
#define STACK_SIZE      4096
#endif /* STACK_SIZE */

#ifndef TOPPERS_MACRO_ONLY

extern void main_task(intptr_t exinf);
extern void log_task(intptr_t exinf);
extern void walk_task(intptr_t exinf);
extern void color_task(intptr_t exinf);
extern void sonar_task(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif
