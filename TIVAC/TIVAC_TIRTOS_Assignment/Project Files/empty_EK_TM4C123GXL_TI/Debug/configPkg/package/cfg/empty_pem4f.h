/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-B06
 */

#include <xdc/std.h>

#include <ti/sysbios/knl/Task.h>
extern const ti_sysbios_knl_Task_Handle ADCtask;

#include <ti/sysbios/knl/Task.h>
extern const ti_sysbios_knl_Task_Handle UARTtask;

#include <ti/sysbios/knl/Task.h>
extern const ti_sysbios_knl_Task_Handle SwitchRead;

#include <ti/sysbios/knl/Semaphore.h>
extern const ti_sysbios_knl_Semaphore_Handle ADCSem;

#include <ti/sysbios/hal/Timer.h>
extern const ti_sysbios_hal_Timer_Handle timer0;

#include <ti/sysbios/knl/Semaphore.h>
extern const ti_sysbios_knl_Semaphore_Handle UARTSem;

#include <ti/sysbios/knl/Semaphore.h>
extern const ti_sysbios_knl_Semaphore_Handle SRSem;

#include <ti/sysbios/hal/Hwi.h>
extern const ti_sysbios_hal_Hwi_Handle HWI_TIMER2;

extern int xdc_runtime_Startup__EXECFXN__C;

extern int xdc_runtime_Startup__RESETFXN__C;

