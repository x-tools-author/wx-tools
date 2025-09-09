print("Start Timer demo...")

while true do
    print("Timer: " .. os.date("%H:%M:%S"))
    wxt_sleep(1000) -- Sleep for 1000 milliseconds (1 second)
end
