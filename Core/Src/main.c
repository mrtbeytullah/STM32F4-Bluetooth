/*******************************************************************************************
 *  Bu uygulamada HC-05 Bluetoooth modulu ile kartimiz olan STM32F407 gelistirme kartindaki*
 * ledlerimizi acip sondurecegiz bu islemi USART2 kullanarak gerceklestirecegiz.           *
 *                                                                                         *
 *  Kartimizdaki A3 pini USART2 nin RX pini                                                *
 *  kartimizdaki A2 pini USART2 nin TX pini dir.                                           *
 *                                                                                         *
 *   Bluetooth modulundeki baglantıları yapar iken:                                        *
 *                                                                                         *
 *  kartdaki RX --> Bluetooth modulundeki TX 'e                                            *
 *  kartdaki TX --> Bluetooth modulundeki RX 'e                                            *
 *  Kartin 3.3V 'u --> Bluetooth modulundeki Vcc 'ye                                       *
 *  kartin GND 'si --> Bluetooth modulundeki GND pinlerine baglanir.                       *
 *******************************************************************************************
 *Linkedin:mrtbeytullah                                                                *
 *******************************************************************************************
 */

#include "main.h"
#include "stdbool.h"//BOOL degisken tipini kullanmak icin bu kutuphaneyi ekliyoruz.
#include "stdio.h"//Veri gonderirken sprintf fonksiyonunu kullanacagimizdan bu kutuphaneyi ekliyoruz.

UART_HandleTypeDef huart2;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

char rx_buffer[10],tx_buffer[20];//Gelen verimizi almak icin rx_buffer gonderecegimiz veri icin tx_buffer degiskenini kullanacagız.
bool led_state;//Bu degisken ile led durumumuzu kontrol edecegiz.

int main(void)
{

  HAL_Init();

  SystemClock_Config();


  MX_GPIO_Init();
  MX_USART2_UART_Init();

  while (1)
  {

	  HAL_UART_Receive(&huart2,(uint8_t*)rx_buffer, 10, 100);//Bluetooth dan verimizi almak icin bu fonksiyonu kullanarak rx_buffer degiskenimize atiyoruz.

	  if(rx_buffer[0] == 'O' && rx_buffer[1] == 'N')//Bluetooth modulune ON bilgisi gonderdigimizde kartimizda ki ledleri acacak.
	  {
           HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_PIN_SET);
           if(led_state!=true)
           {
        	   HAL_UART_Transmit(&huart2, (uint8_t*)tx_buffer,sprintf(tx_buffer,"led on.\n") ,100);//ledler yandıgında bize Led On ifadesi ile geri bildirimde bulunuyor.
        	   led_state=true;//Ledler acik oldugundan led durumunu True yapiyoruz
           }

	  }
	  else if(rx_buffer[0] == 'O'&& rx_buffer[1] == 'F' && rx_buffer[2] == 'F')//bluetooth modulune OFF bilgisi gonderdigimizde Kartımızda ki ledleri kapatacak.
	  {
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);
		  if(led_state!=false)
		  {
			  HAL_UART_Transmit(&huart2, (uint8_t*)tx_buffer, sprintf(tx_buffer,"led off.\n"), 100);//ledler sondugunde bize Led Off ifadesi ile geri bildirimde bulunuyor.
			  led_state=false;//Ledler kapalı oldugundan led durumunu False yapiyoruz
		  }
	  }
  }

}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}


static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}


void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}




