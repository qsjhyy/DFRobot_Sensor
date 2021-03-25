/*!
 * @file readSound.ino
 * @brief 读取环境声音强度，单位DB
 * @n 实验现象：每秒读取一次环境光线强度，并打印到串口
 * @n 给传感器以不同的光照强度，可以得到不同的采集结果
 * @n 由于只有4 bits存放声音强度，最低分辨率8dB，所以测量范围是 0dB-120dB
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [qsj](qsj.huang@dfrobot.com)
 * @version  V0.1
 * @date  2021-3-25
 * @get from https://www.dfrobot.com
 * @url https://github.com/qsjhyy/DFRobot_Sensor
 */
#include <DFRobot_Sensor_Hyy.h>

DFRobot_Sensor_Hyy_IIC sensor(&Wire, DFRobot_Sensor_Hyy::eLowPower+DFRobot_Sensor_Hyy::eNomalPrecision);

void setup(void)
{
  Serial.begin(115200);
  /*在这里一致等到芯片初始化完成才能退出*/
  while(sensor.begin() != 0)
  {
    Serial.println("初始化芯片失败，请确认芯片连接是否正确");
    delay(1000);
  }
}

void loop(void)
{
  /*读取声音强度*/
  uint16_t soundStrength = sensor.soundStrengthDB();
  Serial.print("Sound Strength=");
  Serial.print(soundStrength);
  Serial.println(" DB");
  delay(1000);
}