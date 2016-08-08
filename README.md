#基于STC芯片的DS1302温度时钟

## 结构
  使用STC15W402AS芯片([宏晶科技](http://www.stcmcu.com/)生产)作为主控芯片，利用LED小灯组成的4个“8”作为显示部分，同时使用DS1302([Maxim](https://www.maximintegrated.com)生产)作为系统时钟芯片，使用DS18B20([Maxim](https://www.maximintegrated.com)生产)作为温度传感器芯片。电源使用MicroUSB+5V供电。电路图为自行设计。
## 图片
![overview](https://blog.hackerchai.com/wp-content/uploads/2016/08/DSC_0072-min.jpg)

![front](https://blog.hackerchai.com/wp-content/uploads/2016/08/DSC_0093.jpg)

![back](https://blog.hackerchai.com/wp-content/uploads/2016/08/DSC_0087-min.jpg)

## 文件说明
<pre>
.
├── circuit-diagram                 //电路图文件夹
│   ├── CLOCK.PCB                   //基础电路图
│   └── CLOCKPT.PCB                 //铺铜电路图
├── header-file                     //头文件文件夹
│   ├── definecontrol.h             //定义引脚以及字模
│   ├── DS1302.H                    //定义DS1302引脚
│   └── STC15W402AS.h               //单片机头文件
├── LICENSE.md
├── main-code                       //主要逻辑代码
│   ├── 1302BatteryDebug.c          //DS1302掉电走时调试
│   ├── 1302Debug.c                 //DS1302调试
│   ├── 18B20Test.c                 //DS18B20测试
│   ├── buttonTest.c                //按钮测试
│   ├── chartTest.c                 //字模测试
│   ├── Clock.xmp                   //TKStudio项目文件
│   ├── Debug                       //二进制文件夹
│   │   ├── 1302BatteryDebug.obj    //DS1302掉电走时调试
│   │   ├── 1302TEST.obj            //DS1302调试
│   │   ├── 18B20Test.obj           //DS18B20测试
│   │   ├── buttonTest.obj          //按钮测试
│   │   ├── Clock
│   │   ├── Clock.hex
│   │   ├── Clock.lnp
│   │   ├── Clock.M51
│   │   ├── lightTest.obj           //LED灯测试
│   │   ├── main.obj                //最终代码
│   ├── definecontrol.h             //定义引脚以及字模
│   ├── DS1302.h                    //定义DS1302引脚
│   ├── lightTest.c                 //LED灯测试
│   ├── main.c                      //最终代码
│   └── STC15W402AS.h               //单片机头文件
└── README.md
</pre>  
## 功能
  提供时钟显示，修改，以及温度显示功能。
## 制作教程
### Windows
  1.克隆或下载本仓库，PCB电路图在仓库内，印刷后按照引脚焊接元件。

  2.下载[宏晶科技](http://www.stcmcu.com/)官网所提供的相关烧录软件与教程，并安装。

  3.下载[Keil](http://www.keil.com/download/product/)C51开发工具，并安装。

  4.下载TKStudio，并安装(可选)。

  5.使用USB转TTl烧录器连接板载串口与计算机(如果需要驱动请自行安装)

  6.使用STC烧录软件，选择STC15W402AS型号，使用默认配置(单片机震荡频率11.0592MHz)，载入想要烧录的二进制文件，上电烧录。

  7.如果需要增加修改逻辑代码，可以使用TKStudio打开main-code/Clock.xmp进行二次开发，其他IDE也可。  
## 详细教程
  参见作者博客文章[DS1302温度时钟DIY](https://blog.hackerchai.com/ds1302-ds18b20-stc15-clock/)。
## 用途
  可作为家用时钟，同时提供了针对STC系列单片机较为成熟的DS1302和DS18B20驱动方案，可以作为院校单片机学习项目。
## 关于作者
主要设计者&代码贡献者：[hackerchai](https://github.com/hackerchai)([柴轶晟](https://hackerchai.com))

参与开发者：沈冠霖 王荣

主要硬件设计：李沐东 沈冠霖

感谢辽宁省沈阳市东北育才学校高中部 机器人实验室 [龚鹏](13644056236@126.com)老师的指导。
