print("Start Echo demo...")
print("current time is:", os.date())

local function on_bytes_read(rx_string)
    print("Bytes read:", rx_string)
end
