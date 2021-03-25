/*!
 * @file DFRobot_Sensor_Hyy_Hyy.h
 * @brief (仿写、学习)定义DFRobot_Sensor_Hyy 类的基础结构
 * @n 这是一个虚拟的传感器，IIC地址不可改变,不对应任何实物，可以通过IIC和SPI口来控制它，假设它有下面这些功能
 * @n 向寄存器0里写入数据，点亮不同颜色的LED灯
 * @n 从寄存器2里读出数据，高四位表示光线强度，低四位表示声音强度
 * @n 从寄存器3 bit0里写入数据，写1表示正常模式，写0表示低功耗模式
 * @n 从寄存器4 读取数据，读到的是芯片版本0xDF
 * @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence The MIT License (MIT)
 * @author [qsj](qsj.huang@dfrobot.com)
 * @version V0.1
 * @date 2021-03-25
 * @get from https://www.dfrobot.com
 * @url https://github.com/qsjhyy/DFRobot_Sensor_Hyy
 */
#ifndef __DFROBOT_SENSOR_HYY_H__
#define __DFROBOT_SENSOR_HYY_H__

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

// 打开这个宏，可以看到程序的详细运行过程
#define ENABLE_DBG

/*
__LINE__ 编译时的行号，十进制数
__FUNCTION__ 编译时的函数名，字符串类型
__VA_ARGS__ 可变参数宏，用于可变参数的函数，如printf()。作用类似"..."，这里就用它来替换DBG(…)里的省略号，可以打印出DBG()里的参数
*/
#ifdef ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif

#define DFRobot_Sensor_IIC_ADDR 0x08   // 从设备IIC地址，无变化地址功能
#define DFRobot_Sensor_ID 0xDF         // 芯片版本，无变化地址功能

#define SENSOR_ADDR_LED    0   // LED控制地址  这里的描述从芯片手册上抄写
#define SENSOR_ADDR_DATA   2   // 组合数据地址  这里的描述从芯片手册上抄写
#define SENSOR_ADDR_CONFIG 3   // 配置寄存器地址 这里的描述从芯片手册上抄写
#define SENSOR_ADDR_ID     4   // 芯片ID寄存器地址 这里的描述从芯片手册上抄写

#define  COLOR_RGB565_BLACK     0x0000   // 黑色    
#define  COLOR_RGB565_NAVY      0x000F   // 深蓝色  
#define  COLOR_RGB565_DGREEN    0x03E0   // 深绿色  
#define  COLOR_RGB565_DCYAN     0x03EF   // 深青色  
#define  COLOR_RGB565_PURPLE    0x780F   // 紫色  
#define  COLOR_RGB565_MAROON    0x7800   // 深红色      
#define  COLOR_RGB565_OLIVE     0x7BE0   // 橄榄绿      
#define  COLOR_RGB565_LGRAY     0xC618   // 灰白色
#define  COLOR_RGB565_DGRAY     0x7BEF   // 深灰色      
#define  COLOR_RGB565_BLUE      0x001F   // 蓝色    
#define  COLOR_RGB565_GREEN     0x07E0   // 绿色          
#define  COLOR_RGB565_CYAN      0x07FF   // 青色  
#define  COLOR_RGB565_RED       0xF800   // 红色       
#define  COLOR_RGB565_MAGENTA   0xF81F   // 品红    
#define  COLOR_RGB565_YELLOW    0xFFE0   // 黄色        
#define  COLOR_RGB565_WHITE     0xFFFF   // 白色  


class DFRobot_Sensor_Hyy
{
public:
#define ERR_OK             0   // 无错误
#define ERR_DATA_BUS      -1   // 数据总线错误
#define ERR_IC_VERSION    -2   // 芯片版本不匹配

  /*
    这里从数据手册上抄写关于这个寄存器的描述
  */
  typedef struct
  {
    uint8_t   b: 5;
    uint8_t   g: 6;
    uint8_t   r: 5;
  } __attribute__ ((packed)) sColor_t;
  // 紧凑的结构体变量（知识点：__attribute__ ((packed))是避免字节对齐，紧凑存放），我们用来保存寄存器相关的内容。

  /*
    这里从数据手册上抄写关于这个寄存器的描述
    * ------------------------------------------------------------------------------------------
    * |    b7    |    b6    |    b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
    * ------------------------------------------------------------------------------------------
    * |                 声音强度                  |                  光线强度                  |
    * ------------------------------------------------------------------------------------------
  */
  typedef struct
  {
    uint8_t   light: 4;
    uint8_t   sound: 4;
  } __attribute__ ((packed)) sCombinedData_t;

  /*
    这里从数据手册上抄写关于这个寄存器的描述
    * -----------------------------------------------------------------------------------------
    * |    b7    |    b6    |    b5    |    b4    |    b3    |    b2   |    b1     |    b0    |
    * -----------------------------------------------------------------------------------------
    * |   ready  |         reversed               |      precision     | highspeed | lowpower |
    * -----------------------------------------------------------------------------------------
    *
    *上电后，ready位默认为1，不可更改
  */
  typedef struct
  {
    uint8_t   lowpower: 1; /*!< 上电为0，1：低功耗模式 0：正常功耗模式 */
    uint8_t   highspeed: 1; /*!< 上电为0，1：告诉模式 0：正常速度模式 */
    uint8_t   precision: 2; /*!< 上电为0，0：低精度模式，1：正常精度模式，2：高精度模式，3：超高精度模式 */
    uint8_t   reserved: 3; /*!< 上电为0，1：低功耗模式 0：正常功耗模式 */
    uint8_t   ready:1; /*!< 上电为0，1：低功耗模式 0：正常功耗模式 */
  } __attribute__ ((packed)) sMode_t;

  /*
  从数据手册上抄写
  */
  typedef enum
  {
    eNormalPower = 0, /**< 正常功耗模式，功耗范围20mW-60mW，可以搭配任意采集速度eSpeedMode_t和采集精度ePrecisionMode_t */
    eLowPower = 1, /**< 低功耗模式，功耗范围2mW-4mW, 注意在低功耗模式下，采集速度eSpeedMode_t只能搭配eNormalSpeed，采集精度ePrecisionMode_t只能搭配eLowPrecision和eNomalPrecision*/
  }eLowPowerMode_t;

  /*
  从数据手册上抄写
  */
  typedef enum
  {
    eNormalSpeed = 0<<1, /**< 正常采集速度，可以和任意精度搭配使用 */
    eHighSpeed = 1<<1,   /**< 高速采集模式，采集周期10ms，可以进入低功耗，可以配置为eLowPrecision和eNomalPrecision两种精度模式 */
  }eSpeedMode_t;

  /*
  从数据手册上抄写
  */
  typedef enum
  {
    eLowPrecision   = 0<<2, /**< 低精度，精度大概在xxx，在低精度模式下，可以进入低功耗 */
    eNomalPrecision = 1<<2, /**< 正常精度，精度大概在xxx，在正常精度模式下，可以进入低功耗 */
    eHighPrecision  = 2<<2, /**< 高精度，精度大概在xxx，在高精度模式下，采集速率会降低，采集周期100ms，不能进入低功耗 */
    eUltraPrecision = 3<<2, /**< 超高精度，精度大概在xxx，在超高精度模式下，采集速率会极低，采集周期1000ms，不能进入低功耗 */
  }ePrecisionMode_t;

public:
  /**
  * @brief 构造函数
  * @param mode 构造设备时，可以指定它的默认工作模式
  */
  DFRobot_Sensor_Hyy(uint8_t mode);

  /**
  * @brief 初始化函数
  * @return 返回0表示初始化成功，返回其他值表示初始化失败，返回错误码
  */
  virtual int begin(void);

  /**
  * @brief 获取声音强度值
  * @return 返回声音强度，单位是DB
  */
  uint16_t soundStrengthDB(void);

  /**
  * @brief 获取光线强度值
  * @return 返回光线强度，单位是勒克斯(Lux)
  */
  uint32_t lightStrengthLux(void);

  /**
  * @brief 切换模式
  * @param mode 可以是eLowPowerMode_t和eHighSpeedMode_t类型和ePrecisionMode_t;也可以是三者组合
  * @return 返回true表示模式设置成功, 返回false表示模式设置失败
  */
  bool switchMode(uint8_t mode);

  /**
  * @brief 设置LED灯的颜色
  * @note  设置颜色后，0.2秒后生效
  * @param r 红色通道颜色值，范围0-255
  * @param g 绿色通道颜色值，范围0-255
  * @param b 蓝色通道颜色值，范围0-255
  */
  void setLED(uint8_t r, uint8_t g, uint8_t b);

  /**
  * @brief 设置LED灯的颜色
  * @note  设置颜色后，0.2秒后生效
  * @param color rgb565格式的颜色值
  */
  void setLED(uint16_t color);

protected:
  /**
  * @brief 写寄存器函数，设计为纯虚函数，由派生类实现函数体
  * @param reg  寄存器地址 8bits
  * @param pBuf 要写入数据的存放缓存
  * @param size 要写入数据的长度
  */
  virtual void writeReg(uint8_t reg, void* pBuf, size_t size) = 0;

  /**
  * @brief 读取寄存器函数，设计为纯虚函数，由派生类实现函数体
  * @param reg  寄存器地址 8bits
  * @param pBuf 要读取数据的存放缓存
  * @param size 要读取数据的长度
  * @return 返回读取的长度，返回0表示读取失败
  */
  virtual uint8_t readReg(uint8_t reg, void* pBuf, size_t size) = 0;

private:
  // 私有化定义的变量
  uint8_t _r, _g, _b;   // 颜色RGB值变量
  uint8_t _mode;   // 设置传感器模式的变量
};

class DFRobot_Sensor_Hyy_IIC:public DFRobot_Sensor_Hyy
{
public:
  /**
  * @brief 构造函数
  * @param pWire Wire.h里定义了Wire对象，因此使用&Wire就能够指向并使用Wire中的方法
  * @param mode 构造设备时，可以指定它的默认工作模式
  */
  DFRobot_Sensor_Hyy_IIC(TwoWire *pWire=&Wire, uint8_t mode=eNomalPrecision+eNormalSpeed+eNormalPower);

  /**
  * @brief 子类初始化函数
  * @return 返回0表示初始化成功，返回其他值表示初始化失败
  */
  virtual int begin(void);

protected:
  /**
  * @brief 通过IIC总线写入寄存器值
  * @param reg  寄存器地址 8bits
  * @param pBuf 要写入数据的存放缓存
  * @param size 要写入数据的长度
  */
  virtual void writeReg(uint8_t reg, void* pBuf, size_t size);

  /**
  * @brief 通过IIC总线读取寄存器值
  * @param reg  寄存器地址 8bits
  * @param pBuf 要读取数据的存放缓存
  * @param size 要读取数据的长度
  * @return 返回读取的长度，返回0表示读取失败
  */
  virtual uint8_t readReg(uint8_t reg, void* pBuf, size_t size);

private:
  TwoWire *_pWire;   // IIC通信方式的指针
  uint8_t _deviceAddr;   // IIC通信的设备地址
};


class DFRobot_Sensor_Hyy_SPI:public DFRobot_Sensor_Hyy
{
public:
  /**
  * @brief 构造函数
  * @param pSpi SPI.h里定义了extern SPIClass SPI;因此取SPI对象的地址就能够指向并使用SPI中的方法
  * @param csPin 是指定cs接的数字引脚
  * @param mode 构造设备时，可以指定它的默认工作模式
  */
  DFRobot_Sensor_Hyy_SPI(SPIClass *pSpi=&SPI, uint8_t csPin=4, uint8_t mode=eNomalPrecision+eNormalSpeed+eNormalPower);

  /**
  * @brief 子类初始化函数
  * @return 返回0表示初始化成功，返回其他值表示初始化失败
  */
  virtual int begin(void);

protected:
  /**
  * @brief 通过SPI总线写入寄存器值
  * @param reg  寄存器地址 8bits
  * @param pBuf 要写入数据的存放缓存
  * @param size 要写入数据的长度
  */
  virtual void writeReg(uint8_t reg, void* pBuf, size_t size);

  /**
  * @brief 通过SPI总线读取寄存器值
  * @param reg  寄存器地址 8bits
  * @param pBuf 要读取数据的存放缓存
  * @param size 要读取数据的长度
  * @return 返回读取的长度，返回0表示读取失败
  */
  virtual uint8_t readReg(uint8_t reg, void* pBuf, size_t size);

private:
  SPIClass *_pSpi;   // SPI通信方式的指针
  uint8_t _csPin;   // SPI通信的片选引脚
};

#endif
