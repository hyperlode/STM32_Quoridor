/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_host.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "eeprom.h"
#include "ssd1306.h"
#include "ssd1306_tests.h"
#include "quoridor.h"
#include "quoridor_config.h"
#include "quoridor_tests.h"
#include <string.h>
#include "buttons.h"
#include "button_interrupt.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

I2S_HandleTypeDef hi2s3;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */
/* Virtual address defined by the user: 0xFFFF value is prohibited */
uint16_t VirtAddVarTab[NB_OF_VAR] = {0x5555, 0x6666, 0x7777};
uint16_t VarDataTab[NB_OF_VAR] = {0, 0, 0};
uint16_t VarValue, VarDataTmp = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2S3_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM2_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM3_Init(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */
    //https://github.com/afiskon/stm32-ssd1306
    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_I2S3_Init();
    MX_SPI1_Init();
    MX_USB_HOST_Init();
    MX_TIM2_Init();
    MX_SPI2_Init();
    MX_TIM3_Init();
    /* USER CODE BEGIN 2 */
    HAL_TIM_Base_Start_IT(&htim2);

    //     // Update SystemCoreClock variable
    // SystemCoreClockUpdate();
    // // Make SysTick overflow every 1ms
    // SysTick_Config(SystemCoreClock / 1000);

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    oled_init();
#define EEPROM_TEST

#ifdef EEPROM_TEST

//setup memory
// HAL_FLASH_Unlock();
// FLASH_Erase_Sector(11, FLASH_VOLTAGE_RANGE_3);
// HAL_FLASH_Lock();

//write to memory
uint8_t rdBuf[5];
uint8_t wrBuf[5] = {0x11, 0x22, 0x33, 0x44, 0x55};
uint32_t flashAddress = 0x080E0000;
// //uint8_t wrBuf[5] = {0xFE, 0xFE, 0xFE, 0xFE, 0xFE};

// HAL_FLASH_Unlock();
// for(uint32_t i=0; i<5; i++)
// {
//     HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, flashAddress, ((uint8_t *)wrBuf)[i]);
//     flashAddress++;
// }
// HAL_FLASH_Lock();

//read
flashAddress = 0x080E0000;
for(uint32_t i=0; i<5; i++)
{
    *((uint8_t *)rdBuf + i) = *(uint8_t *)flashAddress;
    flashAddress++;
}




    /* Unlock the Flash Program Erase controller */
    // HAL_FLASH_Unlock();

    // /* EEPROM Init */
    // if (EE_Init() != EE_OK)
    // {
    //     Error_Handler();
    // }

    // /* --- Store successively many values of the three variables in the EEPROM ---*/
    // /* Store 0x1000 values of Variable1 in EEPROM */
    // for (VarValue = 1; VarValue <= 5; VarValue++)
    // // for (VarValue = 1; VarValue <= 0x1000; VarValue++)
    // {
    //     /* Sequence 1 */
    //     if ((EE_WriteVariable(VirtAddVarTab[0], VarValue)) != HAL_OK)
    //     {
    //         Error_Handler();
    //     }
    //     if ((EE_ReadVariable(VirtAddVarTab[0], &VarDataTab[0])) != HAL_OK)
    //     {
    //         Error_Handler();
    //     }
    //     if (VarValue != VarDataTab[0])
    //     {
    //         Error_Handler();
    //     }

    //     /* Sequence 2 */
    //     if (EE_WriteVariable(VirtAddVarTab[1], ~VarValue) != HAL_OK)
    //     {
    //         Error_Handler();
    //     }
    //     if (EE_ReadVariable(VirtAddVarTab[1], &VarDataTab[1]) != HAL_OK)
    //     {
    //         Error_Handler();
    //     }
    //     if (((uint16_t)~VarValue) != VarDataTab[1])
    //     {
    //         Error_Handler();
    //     }

    //     /* Sequence 3 */
    //     if (EE_WriteVariable(VirtAddVarTab[2], VarValue << 1) != HAL_OK)
    //     {
    //         Error_Handler();
    //     }
    //     if (EE_ReadVariable(VirtAddVarTab[2], &VarDataTab[2]) != HAL_OK)
    //     {
    //         Error_Handler();
    //     }
    //     if ((VarValue << 1) != VarDataTab[2])
    //     {
    //         Error_Handler();
    //     }
    // }
    // HAL_FLASH_Lock();
#endif
    // /* Store 0x2000 values of Variable2 in EEPROM */
    // for (VarValue = 1; VarValue <= 0x2000; VarValue++)
    // {
    //     if (EE_WriteVariable(VirtAddVarTab[1], VarValue) != HAL_OK)
    //     {
    //         Error_Handler();
    //     }
    //     if (EE_ReadVariable(VirtAddVarTab[1], &VarDataTab[1]) != HAL_OK)
    //     {
    //         Error_Handler();
    //     }
    //     if (VarValue != VarDataTab[1])
    //     {
    //         Error_Handler();
    //     }
    // }

    // /* read the last stored variables data*/
    // if (EE_ReadVariable(VirtAddVarTab[0], &VarDataTmp) != HAL_OK)
    // {
    //     Error_Handler();
    // }
    // if (VarDataTmp != VarDataTab[0])
    // {
    //     Error_Handler();
    // }

    // if (EE_ReadVariable(VirtAddVarTab[1], &VarDataTmp) != HAL_OK)
    // {
    //     Error_Handler();
    // }
    // if (VarDataTmp != VarDataTab[1])
    // {
    //     Error_Handler();
    // }

    // if (EE_ReadVariable(VirtAddVarTab[2], &VarDataTmp) != HAL_OK)
    // {
    //     Error_Handler();
    // }
    // if (VarDataTmp != VarDataTab[2])
    // {
    //     Error_Handler();
    // }

    // /* Store 0x3000 values of Variable3 in EEPROM */
    // for (VarValue = 1; VarValue <= 0x3000; VarValue++)
    // {
    //     if (EE_WriteVariable(VirtAddVarTab[2], VarValue) != HAL_OK)
    //     {
    //         Error_Handler();
    //     }
    //     if (EE_ReadVariable(VirtAddVarTab[2], &VarDataTab[2]) != HAL_OK)
    //     {
    //         Error_Handler();
    //     }
    //     if (VarValue != VarDataTab[2])
    //     {
    //         Error_Handler();
    //     }
    // }

    // /* read the last stored variables data*/
    // if (EE_ReadVariable(VirtAddVarTab[0], &VarDataTmp) != HAL_OK)
    // {
    //     Error_Handler();
    // }
    // if (VarDataTmp != VarDataTab[0])
    // {
    //     Error_Handler();
    // }

    // if (EE_ReadVariable(VirtAddVarTab[1], &VarDataTmp) != HAL_OK)
    // {
    //     Error_Handler();
    // }
    // if (VarDataTmp != VarDataTab[1])
    // {
    //     Error_Handler();
    // }

    // if (EE_ReadVariable(VirtAddVarTab[2], &VarDataTmp) != HAL_OK)
    // {
    //     Error_Handler();
    // }
    // if (VarDataTmp != VarDataTab[2])
    // {
    //     Error_Handler();
    // }

    uint8_t button_pressed_edge_memory;

#define DEFAULT_GAME_AT_STARTUP

#ifdef DEFAULT_GAME_AT_STARTUP

    uint8_t moves_count;
    char* moves_lode_notation;

    char game_0[MOVES_STRING_LENGTH] = "2b,d2,N,S,N,S,N,S,N,SS,a2,h3";
    uint8_t game_0_moves_count = 12;
    char game_1[MOVES_STRING_LENGTH] = "N,3d,N,3f,2c,2e,E,S,a1,b1,c1,d1,e1,f1,g1,h1,a8,b8,c8,d8,e8,S,g8,S,h8,S,W,N,E,N,E,S,E,N,N,S,N,S,N,N,N,N,N,S"; //44
    uint8_t game_1_moves_count = 44;


    if (rdBuf[0] == 0x11){
        moves_count = game_0_moves_count;
        moves_lode_notation = game_0;
    
    }else{
        moves_count = game_1_moves_count;
        moves_lode_notation = game_1;

    }

    // char moves_lode_notation[MOVES_STRING_LENGTH] = "N,3d,N,3f,2c,2e,E,S,a1,b1,c1,d1,e1,f1,g1,h1,a8,b8,c8,d8,e8,S,g8,S,h8,S,W,N,E,N,E,S,E,N,N,S,N,S,N,E,N,S,N"; //43
    // char moves_lode_notation[MOVES_STRING_LENGTH] = "N,S,1d,S,1f,S,1h,E,c1"; //9
    // char moves_lode_notation[MOVES_STRING_LENGTH] = "N,S,N,S,N,S,2d,4d,2f,E,2b,a1,E,E,N,5f,E,E,2h,g4,c5,S,c3,N,c7,8b,3h,W,W,W,W,N,N,6d,f6,N,E,W,N,7e,W,N,W,8d,f8,W,N,W,E,W,E,W,N";
    //char moves_lode_notation [MOVES_STRING_LENGTH] = "h1,N,S,N,S,N,S,N,SS,a2,h3";
    //char moves_lode_notation [MOVES_STRING_LENGTH] = "2b,d2,N,S,N,S,N,S,N,SS,a2,h3";

    quoridor_init(moves_lode_notation, moves_count);
#else
    char moves_lode_notation[MOVES_STRING_LENGTH] = "";
    quoridor_init(moves_lode_notation, 0);
#endif

    button_interupt_set_button(0, GPIOE, GPIO_PIN_15);

    while (1)
    {

        // update_game_move();
        read_buttons();

        uint8_t edges_up[6];

        edges_up[0] = button_get_edge_up_single_readout(BUTTON_NORTH);
        edges_up[1] = button_get_edge_up_single_readout(BUTTON_EAST);
        edges_up[2] = button_get_edge_up_single_readout(BUTTON_SOUTH);
        edges_up[3] = button_get_edge_up_single_readout(BUTTON_WEST);
        edges_up[4] = button_get_edge_up_single_readout(BUTTON_ENTER);
        //edges_up[5] = button_get_edge_up_single_readout(BUTTON_TOGGLE);
        edges_up[5] = button_interrupt_get_edge_up_single_readout(0);

        // toggle light at each button press to relief myself of the stress if it's a software problem or a fucking bad button problem
        if (
            edges_up[0] ||
            edges_up[1] ||
            edges_up[2] ||
            edges_up[3] ||
            edges_up[4] ||
            edges_up[5])
        {
            HAL_GPIO_TogglePin(led_orange_GPIO_Port, led_orange_Pin);
        }

        program_state_manager(
            edges_up[0],
            edges_up[1],
            edges_up[2],
            edges_up[3],
            edges_up[4],
            edges_up[5]);

        // #ifdef QUORIDOR_AUTOPLAY
        //             autoplay_game_next_move();
        // #else
        //             replay_game_next_move();
        // #endif
        //         }
        // #endif
        // button_pressed_edge_memory = button_pressed;

        /* USER CODE END WHILE */
        MX_USB_HOST_Process();

        /* USER CODE BEGIN 3 */

        // HAL_GPIO_TogglePin(led_orange_GPIO_Port,led_orange_Pin);
        // HAL_Delay(40);
    }

    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
  */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
  */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

    /* USER CODE BEGIN I2C1_Init 0 */

    /* USER CODE END I2C1_Init 0 */

    /* USER CODE BEGIN I2C1_Init 1 */

    /* USER CODE END I2C1_Init 1 */
    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN I2C1_Init 2 */

    /* USER CODE END I2C1_Init 2 */
}

/**
  * @brief I2S3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2S3_Init(void)
{

    /* USER CODE BEGIN I2S3_Init 0 */

    /* USER CODE END I2S3_Init 0 */

    /* USER CODE BEGIN I2S3_Init 1 */

    /* USER CODE END I2S3_Init 1 */
    hi2s3.Instance = SPI3;
    hi2s3.Init.Mode = I2S_MODE_MASTER_TX;
    hi2s3.Init.Standard = I2S_STANDARD_PHILIPS;
    hi2s3.Init.DataFormat = I2S_DATAFORMAT_16B;
    hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
    hi2s3.Init.AudioFreq = I2S_AUDIOFREQ_96K;
    hi2s3.Init.CPOL = I2S_CPOL_LOW;
    hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
    hi2s3.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
    if (HAL_I2S_Init(&hi2s3) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN I2S3_Init 2 */

    /* USER CODE END I2S3_Init 2 */
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

    /* USER CODE BEGIN SPI1_Init 0 */

    /* USER CODE END SPI1_Init 0 */

    /* USER CODE BEGIN SPI1_Init 1 */

    /* USER CODE END SPI1_Init 1 */
    /* SPI1 parameter configuration*/
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN SPI1_Init 2 */

    /* USER CODE END SPI1_Init 2 */
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

    /* USER CODE BEGIN SPI2_Init 0 */

    /* USER CODE END SPI2_Init 0 */

    /* USER CODE BEGIN SPI2_Init 1 */

    /* USER CODE END SPI2_Init 1 */
    /* SPI2 parameter configuration*/
    hspi2.Instance = SPI2;
    hspi2.Init.Mode = SPI_MODE_MASTER;
    hspi2.Init.Direction = SPI_DIRECTION_1LINE;
    hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi2.Init.NSS = SPI_NSS_SOFT;
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
    hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi2.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&hspi2) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN SPI2_Init 2 */

    /* USER CODE END SPI2_Init 2 */
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

    /* USER CODE BEGIN TIM2_Init 0 */

    /* USER CODE END TIM2_Init 0 */

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM2_Init 1 */

    /* USER CODE END TIM2_Init 1 */
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 84 - 1;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 20 - 1;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM2_Init 2 */

    /* USER CODE END TIM2_Init 2 */
}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

    /* USER CODE BEGIN TIM3_Init 0 */

    /* USER CODE END TIM3_Init 0 */

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM3_Init 1 */

    /* USER CODE END TIM3_Init 1 */
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 8400 - 1;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    //   htim3.Init.Period = 10000;
    htim3.Init.Period = 500;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM3_Init 2 */

    /* USER CODE END TIM3_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOD, OLED_RES_Pin | OLED_CS_Pin | OLED_DC_Pin | LD4_Pin | LD3_Pin | LD5_Pin | LD6_Pin | Audio_RST_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin : CS_I2C_SPI_Pin */
    GPIO_InitStruct.Pin = CS_I2C_SPI_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(CS_I2C_SPI_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
    GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : PDM_OUT_Pin */
    GPIO_InitStruct.Pin = PDM_OUT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(PDM_OUT_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : PA0 */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : BOOT1_Pin */
    GPIO_InitStruct.Pin = BOOT1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : PE10 */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pins : PE11 PE12 PE13 PE14 */
    GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pin : PE15 */
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pin : CLK_IN_Pin */
    GPIO_InitStruct.Pin = CLK_IN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(CLK_IN_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : PB11 PB12 PB14 */
    GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : OLED_RES_Pin OLED_CS_Pin OLED_DC_Pin LD4_Pin
                           LD3_Pin LD5_Pin LD6_Pin Audio_RST_Pin */
    GPIO_InitStruct.Pin = OLED_RES_Pin | OLED_CS_Pin | OLED_DC_Pin | LD4_Pin | LD3_Pin | LD5_Pin | LD6_Pin | Audio_RST_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
    GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : MEMS_INT2_Pin */
    GPIO_InitStruct.Pin = MEMS_INT2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(MEMS_INT2_GPIO_Port, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/* USER CODE BEGIN 4 */

void read_buttons()
{
    button_set_state(BUTTON_SOUTH, HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11));
    button_set_state(BUTTON_NORTH, HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12));
    button_set_state(BUTTON_ENTER, HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14));
    button_set_state(BUTTON_EAST, HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_13));
    button_set_state(BUTTON_WEST, HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_14));
    //button_set_state(BUTTON_TOGGLE, HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_15));
    buttons_refresh();
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_15)
    {
        //button_set_state(BUTTON_TOGGLE, 1);
        //
        HAL_TIM_Base_Start_IT(&htim3);
        //buttons_refresh();
        //read_buttons();
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    if (htim->Instance == TIM2)
    {
        HAL_GPIO_WritePin(led_red_GPIO_Port, led_red_Pin, GPIO_PIN_SET);
    }
    else if (htim->Instance == TIM3)
    {
        HAL_TIM_Base_Stop_IT(&htim3);
        button_interrupt_set(0);
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();

    raise_error(ERROR_GENERAL_HAL_GENERATED);
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
