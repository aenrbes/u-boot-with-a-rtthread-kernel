/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 * 2019-10-19     xuzhuoyi     add stm32f429-st-disco bsp
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define UBOOT_MAIN_STACK_SIZE 65536
#define UBOOT_THREAD_PRIORITY (RT_MAIN_THREAD_PRIORITY + 1)

register volatile void *gd asm ("r9");
static struct rt_thread uboot_thread;

extern rt_bool_t board_init_r_done;
extern rt_base_t rt_init_cpsid_level;
extern struct uboot_to_rt *uboot_to_rt_p;
extern void red_led_on(void);
extern void red_led_off(void);
extern void rt_coloured_LED_init(void);

static void another_entry(void *param)
{
    while (board_init_r_done == RT_FALSE)
    {
        rt_kprintf("board_init_r_done = %d\n", board_init_r_done);
    }
    
    rt_coloured_LED_init();
    while (1)
    {
        red_led_on();
        rt_thread_mdelay(500);
        red_led_off();
        rt_thread_mdelay(500);
    }
}

static void uboot_thread_entry(void *param)
{
    rt_init_cpsid_level = rt_hw_interrupt_disable(); 
    uboot_to_rt_p->board_init_r(0, 0);
}

/*do not use u-boot internal printf in thread which stack is smaller than 1k!*/
int main(void)
{
//    rt_uint8_t sys_work_new_prio = UBOOT_THREAD_PRIORITY;

//    rt_work_sys_workqueue_init();
//    rt_thread_control(rt_thread_find("sys_work"), RT_THREAD_CTRL_CHANGE_PRIORITY, &sys_work_new_prio);
    rt_thread_t another, uboot;
    another = rt_thread_create("another", another_entry, RT_NULL, 1024*16, UBOOT_THREAD_PRIORITY, 10);
    rt_thread_startup(another);

    uboot = rt_thread_create("ubootrt", uboot_thread_entry, RT_NULL, 1024*32, UBOOT_THREAD_PRIORITY, 5);
    rt_thread_startup(uboot);

    // rt_thread_init(&uboot_thread,
    //                "uboot",
    //                uboot_thread_entry,
    //                RT_NULL,
    //                &uboot_stack[0],
    //                UBOOT_MAIN_STACK_SIZE, UBOOT_THREAD_PRIORITY, 5);
    // rt_thread_startup(&uboot_thread);
    // rt_kprintf("rt_thread_startup uboot\n");
    return RT_EOK;
}
