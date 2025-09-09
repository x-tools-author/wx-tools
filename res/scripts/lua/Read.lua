print("Start Echo demo...")
print("current time is:", os.date())

function wxt_read(str)
    print("wxt_read:", str)
    wxt_write("echo: " .. str .. "\n")
end
