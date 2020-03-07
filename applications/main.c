/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-27     balanceTWK   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <finsh.h>
#include "dfs.h"
#include "dfs_fs.h"

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

/* defined the LED0 pin: PC0 */
#define LED1_PIN    GET_PIN(C, 1)
#define LED2_PIN    GET_PIN(C, 2)
#define LCD0_PWM    GET_PIN(A, 2)

#define MSH_DEVICE_NAME "vcom"

/*
 * 程序清单：这是一个 PWM 设备使用例程
 * 例程导出了 pwm_led_sample 命令到控制终端
 * 命令调用格式：pwm_led_sample
 * 程序功能：通过 PWM 设备控制 LED 灯的亮度，可以看到LED不停的由暗变到亮，然后又从亮变到暗。
*/

#include <rtthread.h>
#include <rtdevice.h>

//#define PWM_DEV_NAME        "pwm3"  /* PWM设备名称 */
//#define PWM_DEV_CHANNEL     2       /* PWM通道 */

//struct rt_device_pwm *pwm_dev;      /* PWM设备句柄 */

//static int pwm_led_sample(int argc, char *argv[])
//{
//    rt_uint32_t period, pulse;

//    period = 1000000;    /* 周期为0.5ms，单位为纳秒ns */
//    pulse = period/2;    /* PWM脉冲宽度值，单位为纳秒ns */

//    /* 查找设备 */
//    pwm_dev = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME);
//    if (pwm_dev == RT_NULL)
//    {
//        rt_kprintf("pwm sample run failed! can't find %s device!\n", PWM_DEV_NAME);
//        return RT_ERROR;
//    }
// 
//    /* 设置PWM周期和脉冲宽度默认值 */
//    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse);
//    /* 使能设备 */
//    rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL);
//	
//	return 0; 
//}
//MSH_CMD_EXPORT(pwm_led_sample, pwm sample);

int main(void)
{
    int count = 1;

	rt_console_set_device(MSH_DEVICE_NAME);
	finsh_set_device(MSH_DEVICE_NAME);

    /* 挂载 sdcard 到 "filesystem" 的分区上的文件系统 */
    if(dfs_mount("w25q64", "/", "elm", 0, 0) == 0)
    {
        LOG_I("Filesystem initialized!");
    }
    else
    {
        LOG_E("Failed to initialize filesystem!");
        LOG_D("You should create a filesystem on the block device first!");
    }
	
	struct rt_device_pwm *pwm3 = (struct rt_device_pwm *)rt_device_find("pwm3");
	rt_pwm_set(pwm3, 2, 1000000, 1000000/2);
	rt_pwm_enable(pwm3, 2); 

    /* set LED0 pin mode to output */
    rt_pin_mode(LED1_PIN, PIN_MODE_OUTPUT);
	rt_pin_mode(LED2_PIN, PIN_MODE_OUTPUT);

    /* lcd PWM */ 
    rt_pin_mode(LCD0_PWM, PIN_MODE_OUTPUT);
    rt_pin_write(LCD0_PWM, PIN_LOW); 
	
//    /* lcd PWM */ 
//	#define PWM GET_PIN(C, 7)
//    rt_pin_mode(PWM, PIN_MODE_OUTPUT);
//    rt_pin_write(PWM, PIN_LOW); 

    while (count++)
    {
		rt_pin_write(LED1_PIN, PIN_HIGH);
//        rt_pin_write(PWM, PIN_HIGH);
        rt_thread_mdelay(1);
		rt_pin_write(LED1_PIN, PIN_LOW);
//        rt_pin_write(PWM, PIN_LOW);
        rt_thread_mdelay(1);
    }

    return RT_EOK;
}
