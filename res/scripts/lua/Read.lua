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
