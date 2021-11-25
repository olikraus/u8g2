--- 模块功能：u8g2demo
-- @module u8g2
-- @author Dozingfiretruck
-- @release 2021.01.25

--[[ 注意：如需使用u8g2的全中文字库需将 luat_base.h中26行#define USE_U8G2_WQY12_T_GB2312 打开]]

-- LuaTools需要PROJECT和VERSION这两个信息
PROJECT = "u8g2demo"
VERSION = "1.0.1"

log.info("main", PROJECT, VERSION)

-- sys库是标配
_G.sys = require("sys")

--[[
I2C0
I2C0_SCL               (PA1)
I2C0_SDA               (PA4)
]]

--添加硬狗防止程序卡死
wdt.init(15000)--初始化watchdog设置为15s
sys.timerLoopStart(wdt.feed, 10000)--10s喂一次狗

-- 日志TAG, 非必须
local TAG = "main"

-- 初始化显示屏
log.info(TAG, "init ssd1306")

-- u8g2.begin({ic ="ssd1306",mode="i2c_sw", pin0=1, pin1=4}) -- 通过PA1 SLK/PA4 SDA模拟, 也可以用硬件i2c脚
u8g2.begin({ic ="ssd1306", mode="i2c_hw", i2c_id=0, i2c_speed = i2c.FAST}) -- 硬件i2c
--u8g2.begin({ic ="st7567",mode="spi_hw_4pin",spi_id=1,spi_res=19,spi_dc=17,cs=20})

--u8g2.begin("ssd1306")
u8g2.SetFontMode(1)
u8g2.ClearBuffer()
u8g2.SetFont(u8g2.font_opposansm8)
u8g2.DrawUTF8("U8g2+LuatOS", 32, 22)
u8g2.SetFont(u8g2.font_opposansm12_chinese)
u8g2.DrawUTF8("中文测试", 40, 38)
u8g2.SendBuffer()

-- 联网主流程
sys.taskInit(function()
    sys.wait(2000)
    u8g2.ClearBuffer()
    u8g2.SetFont(u8g2.font_opposansm12_chinese)
    u8g2.DrawUTF8("屏幕宽度", 20, 24)
    u8g2.DrawUTF8("屏幕高度", 20, 42)
    u8g2.SetFont(u8g2.font_opposansm8)
    u8g2.DrawUTF8(":"..u8g2.GetDisplayWidth(), 72, 24)
    u8g2.DrawUTF8(":"..u8g2.GetDisplayHeight(), 72, 42)
    u8g2.SendBuffer()

    sys.wait(2000)
    u8g2.ClearBuffer()
    u8g2.SetFont(u8g2.font_opposansm12_chinese)
    u8g2.DrawUTF8("画线测试：", 30, 24)
    for i = 0, 128, 8 do
        u8g2.DrawLine(0,40,i,40)
        u8g2.DrawLine(0,60,i,60)
        u8g2.SendBuffer()
    end

    sys.wait(1000)
    u8g2.ClearBuffer()
    u8g2.SetFont(u8g2.font_opposansm12_chinese)
    u8g2.DrawUTF8("画圆测试：", 30, 24)
    u8g2.DrawCircle(30,50,10,15)
    u8g2.DrawDisc(90,50,10,15)
    u8g2.SendBuffer()

    sys.wait(1000)
    u8g2.ClearBuffer()
    u8g2.SetFont(u8g2.font_opposansm12_chinese)
    u8g2.DrawUTF8("椭圆测试：", 30, 24)
    u8g2.DrawEllipse(30,50,6,10,15)
    u8g2.DrawFilledEllipse(90,50,6,10,15)
    u8g2.SendBuffer()

    sys.wait(1000)
    u8g2.ClearBuffer()
    u8g2.SetFont(u8g2.font_opposansm12_chinese)
    u8g2.DrawUTF8("方框测试：", 30, 24)
    u8g2.DrawBox(30,40,30,24)
    u8g2.DrawFrame(90,40,30,24)
    u8g2.SendBuffer()

    sys.wait(1000)
    u8g2.ClearBuffer()
    u8g2.SetFont(u8g2.font_opposansm12_chinese)
    u8g2.DrawUTF8("圆角方框：", 30, 24)
    u8g2.DrawRBox(30,40,30,24,8)
    u8g2.DrawRFrame(90,40,30,24,8)
    u8g2.SendBuffer()

    sys.wait(1000)
    u8g2.ClearBuffer()
    u8g2.SetFont(u8g2.font_opposansm12_chinese)
    u8g2.DrawUTF8("符号测试：", 30, 24)
    u8g2.DrawUTF8("显示雪人", 30, 38)
    u8g2.SetFont(u8g2.font_unifont_t_symbols)
    u8g2.DrawGlyph( 50, 60, 0x2603 )
    u8g2.SendBuffer()

    sys.wait(1000)
    u8g2.ClearBuffer()
    u8g2.SetFont(u8g2.font_opposansm12_chinese)
    u8g2.DrawUTF8("三角测试：", 30, 24)
    u8g2.DrawTriangle(30,60, 60,30, 90,60)
    u8g2.SendBuffer()

    sys.wait(3000)
    u8g2.close()
    while true do
        sys.wait(1000)
    end
end)

-- 主循环, 必须加
sys.run()
