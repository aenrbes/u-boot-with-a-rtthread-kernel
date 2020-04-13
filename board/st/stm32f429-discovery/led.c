// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2015
 * Kamil Lulko, <kamil.lulko@gmail.com>
 */

#include <common.h>
#include <asm-generic/gpio.h>
#include <asm/sections.h>
#include <asm/io.h>

DECLARE_GLOBAL_DATA_PTR;

#define CONFIG_RT_HEAP_LEN 0x10000

struct uboot_to_rt{
    void (*board_init_r)(ulong, ulong);
    void *rt_ugd;
    void *rt_heap_addr;
    void *rt_heapend_addr;
    void *u_start;
    int (*printf)(const char*, ...); 
};

extern void rt_kernel_init(void);
extern struct uboot_to_rt uboot_to_rt;
extern struct uboot_to_rt *uboot_to_rt_p;

int reserve_arch(void)
{
	gd->start_addr_sp -= CONFIG_RT_HEAP_LEN;
	disable_mpu();
	printf("rt heap addr = %x\n", gd->start_addr_sp);
	return 0;
}

void coloured_LED_init(void)
{
    uboot_to_rt_p = &uboot_to_rt;

    uboot_to_rt_p->board_init_r = board_init_r;
   	uboot_to_rt_p->rt_ugd = gd;
    uboot_to_rt_p->rt_heap_addr = gd->start_addr_sp + 8;
    uboot_to_rt_p->rt_heapend_addr = gd->start_addr_sp + 8 + CONFIG_RT_HEAP_LEN;
    uboot_to_rt_p->u_start = _start;
    uboot_to_rt_p->printf = printf;

	// gpio_request(CONFIG_RED_LED, "red led");
	// gpio_direction_output(CONFIG_RED_LED, 0);
	// gpio_request(CONFIG_GREEN_LED, "green led");
	// gpio_direction_output(CONFIG_GREEN_LED, 0);

	printf("start_rtthread_kernel...\n");
	rt_kernel_init();
}

void rt_coloured_LED_init(void)
{
	gpio_request(CONFIG_RED_LED, "red led");
	gpio_direction_output(CONFIG_RED_LED, 0);
	gpio_request(CONFIG_GREEN_LED, "green led");
	gpio_direction_output(CONFIG_GREEN_LED, 0);
}

void red_led_off(void)
{
	gpio_set_value(CONFIG_RED_LED, 0);
}

void green_led_off(void)
{
	gpio_set_value(CONFIG_GREEN_LED, 0);
}

void red_led_on(void)
{
	gpio_set_value(CONFIG_RED_LED, 1);
}

void green_led_on(void)
{
	gpio_set_value(CONFIG_GREEN_LED, 1);
}
