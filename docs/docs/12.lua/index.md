---
title: Lua脚本
nav_order: 12
---

{: .note-title }
> wxTools Lua
>
> 基于Lua 5.4...

Lua脚本功能，可以让用户通过编写Lua脚本，来实现一些自动化操作，比如定时发送数据、处理接收数据等。Lua脚本运行在独立的线程中，不会阻塞主界面。

## wxt_write

发送数据接口，这个接口是内部实现的，可以直接调用：

```lua
wxt_write("Hello, wxTools!\n")
```

> 使用该接口时，需要设备正常打开。

## wxt_sleep

睡眠接口，单位为毫秒：

```lua
wxt_sleep(1000) -- 睡眠1秒
```

## wxt_is_interruption_requested

检查脚本是否被请求中断，返回true或false：

```lua
ret = wxt_is_interruption_requested()
if ret then
    print("Script interrupted.")
    break
end
```

> 如果程序有循环操作，必须调用该接口判断是否被中断，否则无法停止脚本。

## wxt_read

接收数据回调函数，当有数据接收时会调用该函数，参数为接收到的数据字符串。这个函数必须再脚本中定义：

```lua
function wxt_read(data)
    print("Received data: " .. data)
end
```

在实际应用中，还要确保脚本中有循环操作，否则该函数不会被调用。

```lua
function wxt_read(data)
    print("Received data: " .. data)
end

while true do
    -- 其他操作
    wxt_sleep(1000)

    ret = wxt_is_interruption_requested()
    if ret then
        print("Script interrupted.")
        break
    end
end
```

## 程序实例

### 发送数据

```lua
print("Start 'Write' demo...")

-- wxt_write: send a string to the device.
-- The function is defined in wxTools inner Lua environment.
wxt_write("Hello, wxTools!")
```

### 定时发送数据

```lua
print("Start 'Timer' demo...")

while true do
    print("Current time: " .. os.date("%H:%M:%S"))
    wxt_write(os.date("%H:%M:%S") .. "\n")

    ret = wxt_is_interruption_requested()
    if ret then
        print("Script interrupted.")
        break
    end

    wxt_sleep(1000) -- Sleep for 1000 milliseconds (1 second)
end
```

### 回显

```lua
print("Start 'Echo' demo...")

function wxt_read(str)
    print("[Rx]:", str)
    wxt_write(str)
end

while true do
    ret = wxt_is_interruption_requested()
    if ret then
        print("Script interrupted.")
        break
    end

    wxt_sleep(1000) -- Sleep for 1000 milliseconds (1 second)
end
```

### 接收数据处理

```lua
print("Start 'Read' demo...")

function wxt_read(str)
    print("wxt_read:", str)
end

while true do
    ret = wxt_is_interruption_requested()
    if ret then
        print("Script interrupted.")
        break
    end

    wxt_sleep(1000) -- Sleep for 1000 milliseconds (1 second)
end
```
