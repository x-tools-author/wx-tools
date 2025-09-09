print("Start Echo demo...")
print("current time is:", os.date())

function wxt_read(str)
    print("wxt_read:", str)
    wxt_write("echo: " .. str .. "\n")
end

-- 使用sys.taskInit()新建一个任务，每秒打印一次hello world
sys.taskInit(function()
    while true do
        sys.wait(1000)
        print("hello world")
    end
end)
