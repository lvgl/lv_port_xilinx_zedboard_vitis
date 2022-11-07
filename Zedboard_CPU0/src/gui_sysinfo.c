/*
 * gui_sysinfo.c
 *
 *  Created on: 18 Apr 2019
 *      Author: peterb
 */
#include <stdlib.h>

#include "gui.h"
#include "support.h"

extern shared_memp_t				shmem_p;

static lv_obj_t 					*tsk_table0;
static lv_obj_t 					*tsk_table1;
static lv_obj_t 					*cpu0_label;
static lv_obj_t 					*cpu1_label;

#define NUM_COLUMNS		5

static int compare_proc_name( void *a, void *b ) {

	OurTaskStatus_t		*taska = (OurTaskStatus_t*)a;
	OurTaskStatus_t		*taskb = (OurTaskStatus_t*)b;

	return( strcmp( taska->pcTaskName, taskb->pcTaskName ));

}

static void update_task_tables( uint8_t cpu_id, lv_obj_t *tsk_table ) {

	TaskStatus_t		ProcessStatusOS[MAX_TASKS];
	OurTaskStatus_t		*ProcessStatus;
    uint32_t			task_idx, row_idx;//, col_idx;
	char     			buf[32];

	ProcessStatus = shmem_p->cpud[cpu_id].ProcessStatus;
    if( cpu_id == 0 ) {
    	shmem_p->cpud[cpu_id].task_cnt = uxTaskGetNumberOfTasks();
    	uxTaskGetSystemState( ProcessStatusOS, shmem_p->cpud[cpu_id].task_cnt, NULL );		// Only get info if local CPU!
        for( int i = 0; i < shmem_p->cpud[cpu_id].task_cnt; i++ ) {
        	strcpy( (ProcessStatus+i)->pcTaskName, ProcessStatusOS[i].pcTaskName );
        	(ProcessStatus+i)->CurrentState = ProcessStatusOS[i].eCurrentState;
        	(ProcessStatus+i)->CurrentPriority = ProcessStatusOS[i].uxCurrentPriority;
        	(ProcessStatus+i)->StackHighWaterMark = ProcessStatusOS[i].usStackHighWaterMark;
        }
    }
    qsort( ProcessStatus, shmem_p->cpud[cpu_id].task_cnt, sizeof(OurTaskStatus_t), (void*)compare_proc_name ); // Sort Alphabetically by name

    lv_table_set_row_cnt(tsk_table, shmem_p->cpud[cpu_id].task_cnt + 1);
    lv_table_set_cell_value( tsk_table, 0, 0, "Task" );
    lv_table_set_cell_value( tsk_table, 0, 1, "Priority" );
    lv_table_set_cell_value( tsk_table, 0, 2, "State" );
    lv_table_set_cell_value( tsk_table, 0, 3, "Lowest Stack" );

    for( task_idx = 0; task_idx < shmem_p->cpud[cpu_id].task_cnt; task_idx++ ) {
    	row_idx =  task_idx+1;
    	lv_table_set_cell_value(tsk_table, row_idx, 0, (ProcessStatus[task_idx].pcTaskName));
		snprintf(buf, sizeof(buf), "%lu", ProcessStatus[task_idx].CurrentPriority);
		lv_table_set_cell_value(tsk_table, row_idx, 1, buf);
		switch (ProcessStatus[task_idx].CurrentState) {
			case eReady:
				lv_table_set_cell_value(tsk_table, row_idx, 2, "Ready");
				break;

			case eBlocked:
				lv_table_set_cell_value(tsk_table, row_idx, 2, "Blocked");
				break;

			case eDeleted:
				lv_table_set_cell_value(tsk_table, row_idx, 2, "Deleted");
				break;

			case eSuspended:
				lv_table_set_cell_value(tsk_table, row_idx, 2, "Suspended");
				break;

			case eRunning:
				lv_table_set_cell_value(tsk_table, row_idx, 2, "Running");
				break;

			default:
				lv_table_set_cell_value(tsk_table, row_idx, 2, "Unknown");
				break;
		}
		snprintf( buf, sizeof(buf), "%d", (ProcessStatus[task_idx].StackHighWaterMark * 4));
		lv_table_set_cell_value(tsk_table, row_idx, 3, buf);

    }
}

#define TASK_TIME_MS 2001


static void update_task_list( lv_timer_t	*timer ) {

    char						s_buf[80];
    static uint32_t				call_count = 0;


    if( !(call_count % 2000) ) {		// Update task list approximately every 2 seconds
		snprintf( s_buf, sizeof(s_buf), "CPU0 Task List:(%lu items)",shmem_p->cpud[0].task_cnt );
		lv_label_set_text(cpu0_label, s_buf);
		update_task_tables( 0, tsk_table0 );			// Update CPU 0 Task table
		snprintf( s_buf, sizeof(s_buf), "CPU1 Task List:(%lu items)",shmem_p->cpud[1].task_cnt );
		lv_label_set_text(cpu1_label, s_buf);
		update_task_tables( 1, tsk_table1 );		// Update CPU 1 Task table only if CPU 1 not updating and if it's changed
    }
    return;
}

void sysinfo_create( lv_obj_t * parent ) {


    uint32_t				col_idx;
    static lv_obj_t			*page1, *page2;


	cpu0_label = lv_label_create(parent);
	cpu1_label = lv_label_create(parent);

	lv_obj_set_pos(cpu0_label, 96, 10);
	lv_obj_set_style_text_font (cpu0_label, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_label_set_text(cpu0_label, "CPU-0");
	lv_obj_set_pos(cpu1_label, 96, (lv_obj_get_height( parent ) / 2)+10);
	lv_obj_set_style_text_font (cpu1_label, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_label_set_text(cpu1_label, "CPU-1");

    page1 = lv_obj_create( parent );
    lv_obj_set_size( page1, 800, ((lv_obj_get_height( parent ) / 2)-45) );
    lv_obj_set_pos( page1, 96, 35 );
    setup_scrl_bar( page1, LV_PART_SCROLLBAR, 8);
    lv_obj_set_scrollbar_mode(page1, LV_SCROLLBAR_MODE_AUTO);
	tsk_table0 = lv_table_create(page1);
	lv_obj_set_style_text_font (tsk_table0, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_pos(tsk_table0, 0, 0 );
    lv_table_set_col_cnt(tsk_table0, NUM_COLUMNS);
    lv_table_set_row_cnt(tsk_table0, 1);
    for( col_idx = 0; col_idx < NUM_COLUMNS; col_idx++ ) {
		lv_table_set_col_width(tsk_table0, col_idx, 150);
    }

    page2 = lv_obj_create( parent );
    lv_obj_set_size( page2, 800, ((lv_obj_get_height( parent ) / 2)-45) );
    lv_obj_set_pos( page2, 96, (lv_obj_get_height( parent ) / 2)+35 );
    setup_scrl_bar( page2, LV_PART_SCROLLBAR, 8);
    lv_obj_set_scrollbar_mode(page2, LV_SCROLLBAR_MODE_AUTO);
    tsk_table1 = lv_table_create(page2);
	lv_obj_set_style_text_font (tsk_table1, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_pos( tsk_table1, 0, 0 );
    lv_table_set_col_cnt(tsk_table1, NUM_COLUMNS);
    lv_table_set_row_cnt(tsk_table1, 1);
    for( col_idx = 0; col_idx < NUM_COLUMNS; col_idx++ ) {
		lv_table_set_col_width(tsk_table1, col_idx, 150);
    }

    update_task_list( NULL );
    lv_timer_create(update_task_list, TASK_TIME_MS, NULL );
}
