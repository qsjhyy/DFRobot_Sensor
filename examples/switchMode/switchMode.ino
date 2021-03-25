/*!
 * @file switchMode.ino
 * @brief 各种模式切换
 * @n 实验现象：演示各种模式切换，保证板子正常工作
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [qsj](qsj.huang@dfrobot.com)
 * @version  V0.1
 * @date  2021-3-25
 * @get from https://www.dfrobot.com
 * @url https://github.com/qsjhyy/DFRobot_Sensor
 */
#include <DFRobot_Sensor.h>
#include <SPI.h>

DFRobot_Sensor_SPI sensor(&SPI, DFRobot_Sensor::eLowPower+DFRobot_Sensor::eNomalPrecision, 13);

void setup(void)
{
  Serial.begin(115200);
  /*在这里一致等到芯片初始化完成才能退出*/
  while(sensor.begin() != 0){
    Serial.println("初始化芯片失败，请确认芯片连接是否正确");
    delay(1000);
  }
}

void loop(void)
{
  /*设置模式*/
  int ret;
  // 切换到正常功耗模式, 
  if((ret = sensor.switchMode(sensor.eNormalPower)) != 0){
    Serial.print("切换到eNormalPower失败 ret=");
    Serial.println(ret);
  }
  // 切换到高速度模式
  if((ret = sensor.switchMode(sensor.eHighSpeed)) != 0){
    Serial.print("切换到eNormalPower失败 ret=");
    Serial.println(ret);
  }
  // 切换到正常速度模式
  if((ret = sensor.switchMode(sensor.eNormalSpeed)) != 0){
    Serial.print("切换到eNormalPower失败 ret=");
    Serial.println(ret);
  }
  // 切换到低功耗模式
  if((ret = sensor.switchMode(sensor.eLowPower)) != 0){
    Serial.print("切换到eNormalPower失败 ret=");
    Serial.println(ret);
  }
  // 切换到eNormalPower+eHighSpeed模式
  if((ret = sensor.switchMode(sensor.eNormalPower+sensor.eHighSpeed)) != 0){
    Serial.print("切换到eNormalPower+eHighSpeed失败 ret=");
    Serial.println(ret);
  }
  // 切换到eHighPrecision+eNormalSpeed模式
  if((ret = sensor.switchMode(sensor.eHighPrecision+sensor.eNormalSpeed)) != 0){
    Serial.print("切换到eHighPrecision+eNormalSpeed失败 ret=");
    Serial.println(ret);
  }
}