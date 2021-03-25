/*!
 * @file transferMyLibrary.ino
 * @brief 仿写
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [qsj](qsj.huang@dfrobot.com)
 * @version  V0.1
 * @date  2021-3-25
 * @get from https://www.dfrobot.com
 * @url https://github.com/qsjhyy/DFRobot_Sensor
 */
#include <DFRobot_Sensor_Hyy.h>  

//模式(mode)是根据定义的三个枚举变量确定，这里eLowPower（用于指定功耗的枚举变量）值是1，其他均为0，即正常模式
DFRobot_Sensor_Hyy_IIC sensor(&Wire, DFRobot_Sensor_IIC::eLowPower);  

void setup(void)
{
  Serial.begin(9600);        //初始化串口，波特率
  while(sensor.begin() != 0){
  Serial.println("arduino发现初始化芯片失败，请确认芯片连接是否正确");      //串口上打印错误
  delay(1000);
  }
}

void loop(void)
{
  /*读取光线强度*/
  uint16_t soundStrength = sensor.soundStrengthDB();
  Serial.print("sound strength=");
  Serial.print(soundStrength);
  Serial.println(" dB");
  delay(1000);
}