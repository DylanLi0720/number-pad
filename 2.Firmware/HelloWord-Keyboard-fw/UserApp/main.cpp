#include <valarray>

#include "common_inc.h"
#include "configurations.h"
#include "lcd.h"
#include "HelloWord/hw_keyboard.h"
#include "math.h"
#include "lcd_init.h"
#include "pic.h"

#define PERIOD 256
#define PI 3.1415
#define DISPLAYUSAGI 0
/* Component Definitions -----------------------------------------------------*/
KeyboardConfig_t config;
HWKeyboard keyboard(&hspi1);

static bool isSoftWareControlColor = false;
static bool isReceiveSuccess = false;
static uint16_t times = 0;

/* Main Entry ----------------------------------------------------------------*/
void Main()
{
    bool eyeBlinkFlag = false;
    EEPROM eeprom;
    eeprom.Pull(0, config);
    if (config.configStatus != CONFIG_OK)
    {
        // Use default settings
        config = KeyboardConfig_t{
            .configStatus = CONFIG_OK,
            .serialNum=123,
            .keyMap={}
        };
        memset(config.keyMap, -1, 128);
        eeprom.Push(0, config);
    }

    //LCD Init
    LCD_Init();
    LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
    /*---- This is a demo LCD display ----*/
#if DISPLAYUSAGI
    LCD_ShowPicture(41, 25, 158, 190, gImage_usagi);
#else
    LCD_ShowPicture(20,0,200,200,gImage_1);
#endif

    // Keyboard Report Start
    HAL_TIM_Base_Start_IT(&htim4);


    while (true)
    {
        if (times > 800)// It is not possible to capture the exact time every time because of the RGB effect
        {
            eyeBlinkFlag = !eyeBlinkFlag;
            times = 0;
#if DISPLAYUSAGI
            if (eyeBlinkFlag)
            {
                LCD_Fill(90,25,112,39,WHITE);
                LCD_Fill(137,25,156,39,WHITE);
                LCD_ShowPicture(64,39,113,56,gImage_openEars);
            }
            else
            {
                LCD_Fill(50,40,70,55,WHITE);
                LCD_Fill(170,40,200,58,WHITE);
                LCD_ShowPicture(67,26,106,69,gImage_closeEars);
            }
#else
            if (eyeBlinkFlag)LCD_ShowPicture(89,37,32,39,gImage_yanjing);
            else LCD_ShowPicture(86,39,37,40,gImage_yanjing2);
#endif
        }


        // LCD_ShowFloatNum1(160,100,time,4,RED,WHITE,16);
        /*---- This is a demo RGB effect ----*/
        static uint32_t t = 1;
        static uint8_t r, g, b;
        static bool fadeDir = true;
        // static uint8_t step = 255 / HWKeyboard::LED_NUMBER;
        fadeDir ? t++ : t--;
        // if (t > 254) fadeDir = false;
        // else if (t < 1) fadeDir = true;
        if (t > 255 * 254 * 254) fadeDir = false;
        else if (t < 1) fadeDir = true;
        r = (uint8_t)(255 * (1 - abs(sin(PI*t/(2*PERIOD)))));
        g = (uint8_t)(255*(1 - abs(sin(PI*t / (2*PERIOD) + PI/3))));
        b = (uint8_t)(255*(1 - abs(sin(PI*t / (2*PERIOD) + 2*PI/3))));//螺旋函数
        if(isReceiveSuccess){
            keyboard.SyncLights();
            isReceiveSuccess = false;
        }
        if(!isSoftWareControlColor)
        {
            /*---- This is a demo RGB effect ----*/
            // for (uint8_t i = 0; i < HWKeyboard::LED_NUMBER; i++)
            //     keyboard.SetRgbBufferByID(i, HWKeyboard::Color_t{t, (uint8_t)(255 - t), (uint8_t)t});
            for (uint8_t i = 0; i < HWKeyboard::LED_NUMBER; i++)
                keyboard.SetRgbBufferByID(i, HWKeyboard::Color_t{r, g, b});

            /*-----------------------------------*/
            keyboard.SyncLights();
            HAL_Delay(5);
        }
        else
        {
            for (uint8_t i = 0; i < HWKeyboard::LED_NUMBER; i++)
                keyboard.SetRgbBufferByID(i, HWKeyboard::Color_t{0, 0, 0});
            keyboard.SyncLights();
        }
    }
}

/* Event Callbacks -----------------------------------------------------------*/
extern "C" void OnTimerCallback() // 1000Hz callback
{
    times++;
    keyboard.ScanKeyStates();  // Around 40us use 4MHz SPI clk
    keyboard.ApplyDebounceFilter(100);
    keyboard.Remap(1);  // When Fn pressed use layer-2

    if (keyboard.KeyPressed(HWKeyboard::PAD_MINUS) && keyboard.KeyPressed(HWKeyboard::PAD_PLUS))
    {
        // do something...
        // or trigger some keys...
        // keyboard.Press(HWKeyboard::LEFT_CTRL);
        // keyboard.Press(HWKeyboard::V);
        isSoftWareControlColor = !isSoftWareControlColor;
    }
    // Report HID key states
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS,
                               keyboard.GetHidReportBuffer(1),
                               HWKeyboard::KEY_REPORT_SIZE);
}


extern "C"
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi)
{
    keyboard.isRgbTxBusy = false;
}

extern "C"
void HID_RxCpltCallback(uint8_t* _data)
{
    if(_data[1] == 0xbd)  isSoftWareControlColor= false;
    if(_data[1] == 0xac) {
        isSoftWareControlColor = true;
        uint8_t pageIndex = _data[2];
        for (uint8_t i = 0; i < 10; i++) {
            if(i+pageIndex*10>=HWKeyboard::LED_NUMBER) {
                isReceiveSuccess = true;
                break;
            }
            keyboard.SetRgbBufferByID(i+pageIndex*10,
                                      HWKeyboard::Color_t{_data[3+i*3], _data[4+i*3], _data[5+i*3]});
        }
    }
}