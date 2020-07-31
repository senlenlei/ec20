
#include "board.h"

TIM_HandleTypeDef TIM2_Handler; // 定义定时器初始化结构体参数

void time_config(void )
{
	#ifdef tim1_config
	
	__HAL_RCC_TIM2_CLK_ENABLE(); //使能TIM3时钟
	
	TIM2_Handler.Channel = HAL_TIM_ACTIVE_CHANNEL_1; // 选择定时器通道
	TIM2_Handler.Instance = TIM2; // 选择定时器1
	TIM2_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4; // 定时器时钟分频 不分频
	TIM2_Handler.Init.CounterMode = TIM_COUNTERMODE_DOWN; //向下计数
	TIM2_Handler.Init.Period = 0xFFFFFFFF; // 自动重装载值
	TIM2_Handler.Init.Prescaler = 335; // 预分频
	TIM2_Handler.Init.RepetitionCounter = 0x00; // 定义重复次数
	HAL_TIM_Base_Init(&TIM2_Handler); // 初始化定时器

	HAL_TIM_Base_Start_IT(&TIM2_Handler); //使能定时器2和定时器2更新中断：TIM_IT_UPDATE

	
	#endif
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM2)
		{
			HAL_NVIC_SetPriority(TIM2_IRQn,1,1);    //设置中断优先级，抢占优先级1，子优先级3
			//HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM3中断   
		}
}

//定时器2中断服务函数
void TIM2_IRQHandler(void)
{
	rt_kprintf("enter time2_irq  success!\r\n");
	for(uint8_t i=0;i<usart1_rec_length;i++)
	{
		rt_kprintf("usart1_rec_buff DATE = %d\r\n",usart1_rec_buff[i]);
		rt_kprintf("usart1_rec_length DATE = %d\r\n",usart1_rec_length);
	}
	usart1_rec_length = 0;
	stop_timer();
	
	//数据接收完成 发送事件标志
	rt_event_send(&event,EVENT_FLAG_usart1);

}

void start_timer(void)
{
	HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM2中断 
}

void stop_timer(void)
{
	HAL_NVIC_DisableIRQ(TIM2_IRQn);          //关闭tim2中断
}

void restart_timer(void)
{
	HAL_NVIC_DisableIRQ(TIM2_IRQn);          //关闭tim2中断
	
	//rt_thread_delay(100);
	
	HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM2中断 
}
