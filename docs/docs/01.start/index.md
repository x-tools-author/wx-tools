---
title: 快速开始
nav_order: 1
---

## 安装

访问[发布页面]下载所需版本即可。

[立即下载](https://github.com/x-tools-author/x-tools/releases){: .btn .btn-green }

{: .note }
> “Continuous Build”是日常构建版本，包含最新的功能，也意味着包含最新的BUG

## 如何选择文件

发布文件格式为：*xtools-[系统]-[版本]-[处理器框架].[后缀]*，例如*xtools-windows-v2025.3.6-amd64.zip*指的是软件包是于2025年3月6日构建的适用于64位windows平台。

关于文件后缀：

* zip: 绿色版软件，解压运行即可
* deb: Linux安装包，可以使用 “*sudo dpkg -i xtools-linux-v7.1.0-x86_64.deb*”来安装
* AppImage: 适用于Linux系统的单文件运行包，下载后双击运行即可，推荐Linux用户使用。
* apk: 安卓安装包
* dmg: macOS软件包

{: .important-title }
> 关于AppImage包
>
> 下载AppImage包后可能需要执行"*sudo chmod +x xtools-xxx-xxx-xxx.AppImage*"来赋予可执行权限后才能运行

{: .important-title }
> 关于deb包
>
> deb包是基于ubuntu-22.04构建的，使用于ubuntu 22.04或更新版本系统

{: .important-title }
> 关于apk包
>
> 安卓包未经测试，只是按原样提供

[发布页面]: https://github.com/x-tools-author/x-tools/releases

## 使用

使用软件的一般步骤如下：

1. 选择连接方式（串口，UDP客户端，UDP服务端，TCP客户端，TCP服务器，WebSocket客户端和WebSocket服务器）
2. 配置连接参数（例如串口号，波特率，IP地址，端口号等）
3. 点击“打开”按钮建立连接，点击“关闭”按钮断开连接
4. 在发送区输入要发送的数据，可以选择数据格式（2进制，8进制，16进制, ASCII及UTF8等）
5. 点击“发送”按钮发送数据
6. 在接收区查看接收到的数据，可以选择数据格式（2进制，8进制，16进制, ASCII及UTF8等）
7. 可以使用“清空”按钮清空接收区
8. 可以使用“保存”按钮将接收区的数据保存到文件
9. 可以使用“设置”按钮配置软件的各种选项，例如自动添加前缀和后缀，输入文本转义，自动计算CRC等
10. 可以使用“帮助”按钮查看软件的帮助文档和关于信息
