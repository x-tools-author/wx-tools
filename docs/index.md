---
title: 项目简介
layout: home
nav_order: 0
description: "wxTools项目简介。"
permalink: /
---

{: .note-title }
> wxTools
>
> wxTools始于2024，支持Lua扩展，实现无限可能...

`wxTools`是一款基于wxWidgets开发的多功能、开源、免费、跨平台、的开发调试工具集，支持串口，UDP客户端，UDP服务端，TCP客户端，TCP服务器，WebSocket客户端和WebSocket服务器数据收发调试。支持Windows（x86/x64/arm64），Linux(amd64)和MacOS(arm64)等操作系统。

`wxTools`软件简单易用，界面简洁，功能实用，支持多种数据格式的输入与输出，支持自动添加前缀和后缀，支持输入文本转义，支持自动计算CRC等功能。`wxTools`软件适用于嵌入式开发、物联网开发、网络开发等领域，是开发者调试和测试的好帮手。

`wxTools`项目开源，代码托管在GitHub上，欢迎大家下载使用和反馈问题，也欢迎大家参与开发和贡献代码。项目具有一定的实用性和综合性，适合初学者学习和实践，也适合有经验的开发者使用和二次开发。许可协议（wxWindows Library Licence, Version 3.1）可查看项目根目录下的LICENSE文件。

{: .note }
> 软件可以从GitHub发布页面和应用商店获得，但是软件商店版本需要付费购买，GitHub发布版本永久免费。从软件商店购买本软件是一种对开发者的支持，其功能与开源本版相同。

软件功能包括：

* 支持串口，UDP客户端，UDP服务端，TCP客户端，TCP服务器，WebSocket客户端和WebSocket服务器的数据收发调试。
* 支持2进制，8进制，16进制, ASCII及UTF8等文本格式的输入与输出。
* 支持自动添加前缀，后缀：'\r', '\n', '\r\n'或'\n\r'。
* 支持输入文本转义：'\r', '\n', '\r\n，\n\r'等。
* 支持自动自动计算CRC，支持以下（但不限于）算法：
  * CRC_8,
  * CRC_8_ITU,
  * CRC_8_ROHC,
  * CRC_8_MAXIM,
  * CRC_16_IBM,
  * CRC_16_MAXIM,
  * CRC_16_USB,
  * CRC_16_MODBUS,
  * CRC_16_CCITT,
  * CRC_16_CCITT_FALSE,
  * CRC_16_x25,
  * CRC_16_XMODEM,
  * CRC_16_DNP,
  * CRC_32,
  * CRC_32_MPEG2,
  * SUM_8,
  * SUM_16,
  * SUM_32,
  * SUM_64
