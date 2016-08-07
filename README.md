# 基于STC芯片的DS1302温度时钟

## 结构
  使用STC15W402AS芯片([宏晶科技](http://www.stcmcu.com/)生产)作为主控芯片，利用LED小灯组成的4个“8”作为显示部分，同时使用DS1302([Maxim](https://www.maximintegrated.com)生产)作为系统时钟芯片，使用DS18B20([Maxim](https://www.maximintegrated.com)生产)作为温度传感器芯片。电源使用MicroUSB+5V供电。电路图为自行设计。
## 图片
![overview](https://blog.hackerchai.com/wp-content/uploads/2016/08/DSC_0072-min.jpg)

![front](https://blog.hackerchai.com/wp-content/uploads/2016/08/DSC_0093.jpg)

![back](https://blog.hackerchai.com/wp-content/uploads/2016/08/DSC_0087-min.jpg)

#功能
  提供时钟显示，修改，以及温度显示功能。
## 制作教程
  PCB电路图请在仓库内自行下载，印刷后按照引脚焊接元件，下载[宏晶科技](http://www.stcmcu.com/)官网所提供的相关烧录软件与教程，配合板载串口与USB-TTL烧录器进行编程。
## 详细教程
  参见作者博客文章[DS1302温度时钟DIY](https://blog.hackerchai.com/ds1302-ds18b20-stc15-clock/)。
## 关于作者
主要代码贡献者：[柴轶晟](https://hackerchai.com) （hackerchai.com@gmail.com）

参与开发：沈冠霖 王荣

主要硬件设计：李沐东
