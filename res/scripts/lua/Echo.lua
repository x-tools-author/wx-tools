-- 简单的Lua脚本示例
print("Hello from Lua!")
print("当前时间:", os.date())

-- 计算斐波那契数列
function fibonacci(n)
    if n <= 1 then
        return n
    end
    return fibonacci(n - 1) + fibonacci(n - 2)
end

print("斐波那契数列前10项:")
for i = 0, 9 do
    print("F(" .. i .. ") =", fibonacci(i))
end

-- 表格操作
local table = { name = "Lua", version = "5.4", rating = 10 }
print("表格内容:")
for k, v in pairs(table) do
    print(k, "=", v)
end
