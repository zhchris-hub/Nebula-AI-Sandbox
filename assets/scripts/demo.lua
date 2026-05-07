-- Nebula AI Sandbox Demo Script
-- This script demonstrates Lua integration

print("Hello from Nebula AI Sandbox!")
print("Lua version: " .. _VERSION)

-- Simple math
local function fibonacci(n)
    if n <= 1 then return n end
    return fibonacci(n - 1) + fibonacci(n - 2)
end

print("Fibonacci(10) = " .. fibonacci(10))

-- Table operations
local entities = {
    {name = "Red Box", x = -200, y = 100},
    {name = "Green Box", x = 0, y = 200},
    {name = "Blue Triangle", x = 200, y = 150}
}

print("\nScene entities:")
for i, entity in ipairs(entities) do
    print(string.format("  %d. %s at (%d, %d)", i, entity.name, entity.x, entity.y))
end

-- String manipulation
local greeting = "Welcome to Nebula AI Sandbox!"
print("\n" .. string.upper(greeting))
print("Length: " .. #greeting)

print("\nDemo script completed successfully!")
