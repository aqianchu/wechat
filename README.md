﻿# wechat
提供项目框架搭建
通过tcp通信使多个客户端与服务端直接进行通信,传送文件
在开发板(客户端)显示客户端连接后的头像,点击头像可以进行聊天
使用字库,并添加简单小键盘

后续功能之后添加


扩展功能
==============
    1.删除好友功能(通过双击屏幕大头贴，或者点击按钮删除)
    2.好友分类--》黑名单，陌生人，好机友，好同学
           仅供参考：
            #define BLACKMINGDAN  1
            #define MOSHENGREN    2
            
             struc t clientlist
             {
                  ip
                  port
                  sock
                  int friendflag=0; //区分好友的类别
             }
    3.发送的文字信息在对方的开发板屏幕上显示，而不是在CRT打印