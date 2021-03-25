/*!
 * @file DFRobot_Sensor_Hyy.cpp
 * @brief (仿写、学习)定义DFRobot_Sensor 类的基础结构
 * @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence The MIT License (MIT)
 * @author [qsj](qsj.huang@dfrobot.com)
 * @version V0.1
 * @date 2021-03-25
 * @get from https://www.dfrobot.com
 * @url https://github.com/qsjhyy/DFRobot_Sensor
 */
#include "DFRobot_Sensor_Hyy.h"

DFRobot_Sensor_Hyy::DFRobot_Sensor_Hyy(uint8_t mode)
  :_mode(*((uint8_t*)&mode))   // 知识点：C++中构造函数才有的初始化列表，可以进行变量的初始化，这里的功能是初始化
{

}

int DFRobot_Sensor_Hyy::begin(void)
{
  uint8_t id=0;

  if(0 == readReg(SENSOR_ADDR_ID, &id, 1))   // 判断数据总线是否成功
  {
    DBG("data bus access error");
    return ERR_DATA_BUS;
  }

  DBG("real sensor id=");DBG(id);
  if(DFRobot_Sensor_ID != id)   // 判断芯片版本是否匹配
  {
    return ERR_IC_VERSION;
  }

  writeReg(SENSOR_ADDR_CONFIG, &_mode, 1);   // 向配置寄存器的地址写入要设置的模式，初始化从机
  DBG("begin ok!");
  return ERR_OK;
}

uint16_t DFRobot_Sensor_Hyy::soundStrengthDB(void)
{
  sCombinedData_t data;   // data这个结构体变量占一个字节，根据手册上这个寄存器定义的，前4位存放声音强度，后4位存放光线强度
  readReg(SENSOR_ADDR_DATA, &data, sizeof(data));   // 从SENSOR_ADDR_DATA（组合数据地址）读数据，存放到data中，所以传的是data的地址		

  DBG("sound reg raw data is");DBG(data.sound);
  return data.sound << 3;   // 单位是DB，所以要移位
}

uint32_t DFRobot_Sensor_Hyy::lightStrengthLux(void)
{
  sCombinedData_t data;
  readReg(SENSOR_ADDR_DATA, &data, sizeof(data));

  DBG("light reg raw data is");DBG(data.light);
  return data.light * 10000;
}

void DFRobot_Sensor_Hyy::setLED(uint8_t r, uint8_t g, uint8_t b)
{
  sColor_t data = {.b=b>>3, .g=g>>2, .r=r>>3};
  writeReg(SENSOR_ADDR_LED, &data, sizeof(data));
}

void DFRobot_Sensor_Hyy::setLED(uint16_t color)
{
  writeReg(SENSOR_ADDR_LED, &color, sizeof(color));
}

bool DFRobot_Sensor_Hyy::switchMode(uint8_t mode)
{
  uint8_t tmp;

#ifdef ENABLE_DBG
  readReg(SENSOR_ADDR_CONFIG, &tmp, sizeof(tmp));
  DBG("before switch Mode, ModeReg = ");DBG(tmp);
#endif

  writeReg(SENSOR_ADDR_CONFIG, &mode, sizeof(mode));

#ifdef ENABLE_DBG
  DBG("switch Mode = ");DBG(mode);
  readReg(SENSOR_ADDR_CONFIG, &tmp, sizeof(tmp));
  DBG("after switch Mode, ModeReg = ");DBG(tmp);
  if(tmp != mode)   // 判断模式是否设置成功
  {
    return false;
  }
#endif

  return true;
}

DFRobot_Sensor_Hyy_IIC::DFRobot_Sensor_Hyy_IIC(TwoWire *pWire, uint8_t mode)
  :DFRobot_Sensor_Hyy(mode)   // 初始化列表：子类用父类给的构造函数从而初始化了父类的私有变量_mode
{
  _deviceAddr = DFRobot_Sensor_IIC_ADDR;
  _pWire = pWire;
}

int DFRobot_Sensor_Hyy_IIC::begin(void)
{
  _pWire->begin();   // Wire.h（IIC）库函数 初始化wire库
  return DFRobot_Sensor_Hyy::begin();   // 利用父类的初始化函数
}

void DFRobot_Sensor_Hyy_IIC::writeReg(uint8_t reg, void* pBuf, size_t size)
{
  if(NULL == pBuf)
  {
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;

  _pWire->beginTransmission(_deviceAddr);
  for(uint16_t i = 0; i < size; i++)
  {
    _pWire->write(_pBuf[i]);
  }
  _pWire->endTransmission();
}

uint8_t DFRobot_Sensor_Hyy_IIC::readReg(uint8_t reg, void* pBuf, size_t size)
{
  if(NULL == pBuf)
  {
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t*)pBuf;

  _pWire->beginTransmission(_deviceAddr);
  _pWire -> write(&reg, 1);
  if(0 != _pWire->endTransmission())   // 用到了 Wire.endTransmission()，结束一个由beginTransmission（）开始的并且由write（）排列的从机的传输。
  {
    return 0;
  }

  _pWire->requestFrom(_deviceAddr, (uint8_t)size);   // 主设备请求从设备size个字节，这个字节可以被主设备用 read()或available()接受

  for(uint16_t i = 0; i < size; i++)
  {
    _pBuf[i] = _pWire->read();   // 用read()接收放到buf里
  }
  _pWire->endTransmission();
  return size;
}

DFRobot_Sensor_Hyy_SPI::DFRobot_Sensor_Hyy_SPI(SPIClass *pSpi, uint8_t csPin, uint8_t mode)
  :DFRobot_Sensor_Hyy(mode)
{
  _pSpi = pSpi;
  _csPin = csPin;
}

int DFRobot_Sensor_Hyy_SPI::begin(void)
{
  pinMode(_csPin, OUTPUT);
  _pSpi->begin();
  return DFRobot_Sensor_Hyy::begin();
}

void DFRobot_Sensor_Hyy_SPI::writeReg(uint8_t reg, void* pBuf, size_t size)
{
  if(NULL == pBuf)
  {
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;

  _pSpi->beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  digitalWrite(_csPin, LOW);
  _pSpi->transfer(reg);
  while(size--)
  {
    _pSpi->transfer(*_pBuf);
    _pBuf++;
  }
  digitalWrite(_csPin, HIGH);
  _pSpi->endTransaction();
}

uint8_t DFRobot_Sensor_Hyy_SPI::readReg(uint8_t reg, void* pBuf, size_t size)
{
  if(NULL == pBuf)
  {
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;

  size_t count = 0;
  _pSpi->beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  digitalWrite(_csPin, LOW);
  _pSpi->transfer(reg);
  while(size--)
  {
    *_pBuf = _pSpi->transfer(0x00);
    _pBuf++;
    count++;
  }
  digitalWrite(_csPin, HIGH);
  _pSpi->endTransaction();
  return count;
}
