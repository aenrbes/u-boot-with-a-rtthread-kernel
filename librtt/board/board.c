/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 * 2019-10-13     xuzhuoyi     add stm32f429-st-disco bsp
 */

#include "board.h"

extern void HardFault_Handler(void);
extern void SysTick_Handler(void);
extern void PendSV_Handler(void);

rt_base_t rt_init_cpsid_level;
extern void rt_hw_systick_init(void);
extern void rtthread_startup(void);

/*these param are set by uboot*/
struct uboot_to_rt uboot_to_rt;
struct uboot_to_rt *uboot_to_rt_p;
rt_bool_t board_init_r_done = RT_FALSE;

// void set_uboot_to_rt(void **list)
// {
//     uboot_to_rt_p = &uboot_to_rt;

//     uboot_to_rt.board_init_r = *(list++);
//     uboot_to_rt.rt_ugd = *(list++);
//     uboot_to_rt.rt_heap_addr = *(list++);
//     uboot_to_rt.rt_heapend_addr = *(list++);
//     uboot_to_rt.u_start = *(list++);
//     uboot_to_rt.printf = *(list++);
// }

void rt_hw_board_init(void)
{
#ifdef SCB_EnableICache
    /* Enable I-Cache---------------------------------------------------------*/
    SCB_EnableICache();
#endif

#ifdef SCB_EnableDCache
    /* Enable D-Cache---------------------------------------------------------*/
    SCB_EnableDCache();
#endif

    /* HAL_Init() function is called at the beginning of the program */
    HAL_Init();

    /* enable interrupt */
    // __set_PRIMASK(0);
    /* System clock initialization */
    SystemClock_Config();
    /* disable interrupt */
    // __set_PRIMASK(1);

    rt_hw_systick_init();

    /* Heap initialization */
#if defined(RT_USING_HEAP)
    rt_system_heap_init(uboot_to_rt_p->rt_heap_addr, uboot_to_rt_p->rt_heapend_addr);
#endif

    /* Pin driver initialization is open by default */
#ifdef RT_USING_PIN
    rt_hw_pin_init();//already init in u-boot device model
#endif

    /* USART driver initialization is open by default */
#ifdef RT_USING_SERIAL
    rt_hw_usart_init();//already init in u-boot device model
#endif

    /* Set the shell console output device */
#ifdef RT_USING_CONSOLE
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif

    /* Board underlying hardware initialization */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif
}

/*called from u-boot board_init_r before call main_loop*/
int last_stage_init(void)
{
    rt_hw_interrupt_enable(rt_init_cpsid_level);
    rt_kprintf("u-boot: last_stage_init...\n");
    board_init_r_done = RT_TRUE;
    return 0;
}

/*called from crt0 after relocation*/
void rt_kernel_init(void)
{
    /*set irq vectors*/
    // *(rt_ubase_t *)(uboot_to_rt_p->u_start + 3 * sizeof(rt_base_t)) = (rt_ubase_t)HardFault_Handler;
    *(rt_ubase_t *)(uboot_to_rt_p->u_start + 0 * sizeof(rt_base_t)) = (rt_ubase_t)STM32_SRAM_END;
    *(rt_ubase_t *)(uboot_to_rt_p->u_start + 15 * sizeof(rt_base_t)) = (rt_ubase_t)SysTick_Handler;
    *(rt_ubase_t *)(uboot_to_rt_p->u_start + 14 * sizeof(rt_base_t)) = (rt_ubase_t)PendSV_Handler;

    rtthread_startup();

    return;
}

void SystemClock_Config(void)
{
//   RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//   RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
//   RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

//   /** Configure the main internal regulator output voltage 
//   */
//   __HAL_RCC_PWR_CLK_ENABLE();
//   __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
//   /** Initializes the CPU, AHB and APB busses clocks 
//   */
//   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
//   RCC_OscInitStruct.HSEState = RCC_HSE_ON;
//   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
//   RCC_OscInitStruct.PLL.PLLM = 4;
//   RCC_OscInitStruct.PLL.PLLN = 168;
//   RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
//   RCC_OscInitStruct.PLL.PLLQ = 7;
//   if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /** Initializes the CPU, AHB and APB busses clocks 
//   */
//   RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                               |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
//   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//   RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
//   RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

//   if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
//   PeriphClkInitStruct.PLLSAI.PLLSAIN = 50;
//   PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
//   PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
//   if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
//   {
//     Error_Handler();
//   }

    SystemCoreClockUpdate();
    rt_kprintf("SystemCoreClock = %d\n", HAL_RCC_GetHCLKFreq());
    rt_kprintf("HAL_NVIC_PriorityGrouping = %d\n", HAL_NVIC_GetPriorityGrouping());
}


