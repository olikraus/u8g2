/*
@module  u8g2
@summary u8g2图形处理库
@author  Dozingfiretruck
@version 1.0
@date    2021.01.25
*/
#include "luat_base.h"
#include "luat_malloc.h"

#include "luat_u8g2.h"
#include "luat_gpio.h"
#include "luat_timer.h"
#include "luat_i2c.h"
#include "luat_spi.h"

#include "u8g2.h"
#include "u8g2_luat_fonts.h"

#define LUAT_LOG_TAG "luat.u8g2"
#include "luat_log.h"

enum
{
	font_opposansm8,
	font_opposansm12_chinese,
	font_unifont_t_symbols,
	font_open_iconic_weather_6x_t,
};


static u8g2_t* u8g2;
static int u8g2_lua_ref;
static uint8_t i2c_id;
static uint8_t i2c_speed;
// static uint8_t i2c_addr = 0x3C;
static uint8_t spi_id;
static uint8_t spi_res;
static uint8_t spi_dc;
static uint8_t spi_cs;

/*
u8g2显示屏初始化
@api u8g2.begin(conf)
@table conf 配置信息
@return int 正常初始化1,已经初始化过2,内存不够3,初始化失败返回4
@usage
-- 初始化i2c1的ssd1306
u8g2.begin({ic = "SSD1306",mode="i2c_hw",i2c_id=0})
*/
static int l_u8g2_begin(lua_State *L) {
    if (u8g2 != NULL) {
        LLOGW("disp is aready inited");
        lua_pushinteger(L, 2);
        return 1;
    }
    u8g2 = (u8g2_t*)lua_newuserdata(L, sizeof(u8g2_t));
    if (u8g2 == NULL) {
        LLOGE("lua_newuserdata return NULL, out of memory ?!");
        lua_pushinteger(L, 3);
        return 1;
    }

    luat_u8g2_conf_t conf = {0};
    conf.pinType = 2; // I2C 硬件(或者是个假硬件)
    conf.ptr = u8g2;
    if (lua_istable(L, 1)) {
        // 参数解析
        lua_pushliteral(L, "ic");
        lua_gettable(L, 1);
        if (lua_isstring(L, -1)) {
            conf.cname = (char*)luaL_checkstring(L, -1);
            LLOGD("using ic: %s",conf.cname);
        }
        lua_pop(L, 1);

        lua_pushliteral(L, "mode");
        lua_gettable(L, 1);
        if (lua_isstring(L, -1)) {
            const char* mode = luaL_checkstring(L, -1);
            LLOGD("mode = [%s]", mode);
            if (strcmp("i2c_sw", mode) == 0) {
                LLOGD("using i2c_sw");
                conf.pinType = 1;
            }
            else if (strcmp("i2c_hw", mode) == 0) {
                LLOGD("using i2c_hw");
                conf.pinType = 2;
            }
            else if (strcmp("spi_sw_3pin", mode) == 0) {
                LLOGD("using spi_sw_3pin");
                conf.pinType = 3;
            }
            else if (strcmp("spi_sw_4pin", mode) == 0) {
                LLOGD("using spi_sw_4pin");
                conf.pinType = 4;
            }
            else if (strcmp("spi_hw_4pin", mode) == 0) {
                LLOGD("using spi_hw_4pin");
                conf.pinType = 5;
            }
        }
        lua_pop(L, 1);

        // 解析pin0 ~ pin7
        lua_pushliteral(L, "pin0");
        lua_gettable(L, 1);
        if (lua_isinteger(L, -1)) {
            conf.pin0 = luaL_checkinteger(L, -1);
        }
        lua_pop(L, 1);

        lua_pushliteral(L, "pin1");
        lua_gettable(L, 1);
        if (lua_isinteger(L, -1)) {
            conf.pin1 = luaL_checkinteger(L, -1);
        }
        lua_pop(L, 1);

        lua_pushliteral(L, "pin2");
        lua_gettable(L, 1);
        if (lua_isinteger(L, -1)) {
            conf.pin2 = luaL_checkinteger(L, -1);
        }
        lua_pop(L, 1);

        lua_pushliteral(L, "pin3");
        lua_gettable(L, 1);
        if (lua_isinteger(L, -1)) {
            conf.pin3 = luaL_checkinteger(L, -1);
        }
        lua_pop(L, 1);

        lua_pushliteral(L, "i2c_id");
        lua_gettable(L, 1);
        if (lua_isinteger(L, -1)) {
            i2c_id = luaL_checkinteger(L, -1);
        }
        lua_pop(L, 1);

        lua_pushliteral(L, "i2c_speed");
        lua_gettable(L, 1);
        if (lua_isinteger(L, -1)) {
            i2c_speed = luaL_checkinteger(L, -1);
        }
        lua_pop(L, 1);

        // lua_pushliteral(L, "i2c_addr");
        // lua_gettable(L, 1);
        // if (lua_isinteger(L, -1)) {
        //     i2c_addr = luaL_checkinteger(L, -1);
        // }
        // lua_pop(L, 1);

        lua_pushliteral(L, "spi_id");
        lua_gettable(L, 1);
        if (lua_isinteger(L, -1)) {
            spi_id = luaL_checkinteger(L, -1);
            LLOGD("spi_id=%d", spi_id);
        }
        lua_pop(L, 1);

        lua_pushliteral(L, "spi_res");
        lua_gettable(L, 1);
        if (lua_isinteger(L, -1)) {
            spi_res = luaL_checkinteger(L, -1);
            LLOGD("spi_res=%d", spi_res);
        }
        lua_pop(L, 1);

        lua_pushliteral(L, "spi_dc");
        lua_gettable(L, 1);
        if (lua_isinteger(L, -1)) {
            spi_dc = luaL_checkinteger(L, -1);
            LLOGD("spi_dc=%d", spi_dc);
        }
        lua_pop(L, 1);

        lua_pushliteral(L, "spi_cs");
        lua_gettable(L, 1);
        if (lua_isinteger(L, -1)) {
            spi_cs = luaL_checkinteger(L, -1);
            LLOGD("spi_cs=%d", spi_cs);
        }
        lua_pop(L, 1);

        // lua_pushliteral(L, "spi_id");
        // lua_gettable(L, 1);
        // if (lua_isinteger(L, -1)) {
        //     spi_id = luaL_checkinteger(L, -1);
        // }
        // lua_pop(L, 1);

        // pin4 ~ pin7暂时用不到,先不设置了
    }
    LLOGD("pinType=%d", conf.pinType);
    if (luat_u8g2_setup(&conf)) {
        u8g2 = NULL;
        LLOGW("disp init fail");
        lua_pushinteger(L, 4);
        return 1; // 初始化失败
    }

    u8g2_lua_ref = luaL_ref(L, LUA_REGISTRYINDEX);

    u8g2_SetFont(u8g2, u8g2_font_ncenB08_tr); // 设置默认字体

    lua_pushinteger(L, 1);
    return 1;
}

/*
关闭显示屏
@api u8g2.close()
@usage
-- 关闭disp,再次使用disp相关API的话,需要重新初始化
u8g2.close()
*/
static int l_u8g2_close(lua_State *L) {
    if (u8g2_lua_ref != 0) {
        lua_geti(L, LUA_REGISTRYINDEX, u8g2_lua_ref);
        if (lua_isuserdata(L, -1)) {
            luaL_unref(L, LUA_REGISTRYINDEX, u8g2_lua_ref);
        }
        u8g2_lua_ref = 0;
    }
    lua_gc(L, LUA_GCCOLLECT, 0);
    u8g2 = NULL;
    return 0;
}

/*
清屏，清除内存帧缓冲区中的所有像素
@api u8g2.ClearBuffer()
@usage
-- 清屏
u8g2.ClearBuffer()
*/
static int l_u8g2_ClearBuffer(lua_State *L) {
    if (u8g2 == NULL) return 0;
    u8g2_ClearBuffer(u8g2);
    return 0;
}

/*
将数据更新到屏幕，将存储器帧缓冲区的内容发送到显示器
@api u8g2.SendBuffer()
@usage
-- 把显示数据更新到屏幕
u8g2.SendBuffer()
*/
static int l_u8g2_SendBuffer(lua_State *L) {
    if (u8g2 == NULL) return 0;
    u8g2_SendBuffer(u8g2);
    return 0;
}

/*
在显示屏上画一段文字，在显示屏上画一段文字,要调用u8g2.SendBuffer()才会更新到屏幕
@api u8g2.DrawUTF8(str, x, y)
@string 文件内容
@int 横坐标
@int 竖坐标
@usage
u8g2.DrawUTF8("wifi is ready", 10, 20)
*/
static int l_u8g2_DrawUTF8(lua_State *L) {
    if (u8g2 == NULL) {
        LLOGW("disp not init yet!!!");
        return 0;
    }
    size_t len;
    size_t x, y;
    const char* str = luaL_checklstring(L, 1, &len);

    x = luaL_checkinteger(L, 2);
    y = luaL_checkinteger(L, 3);

    u8g2_DrawUTF8(u8g2, x, y, str);
    return 0;
}

/*
设置字体模式
@api u8g2.SetFontMode(mode)
@int mode字体模式，启用（1）或禁用（0）透明模式
@usage
u8g2.SetFontMode(1)
*/
static int l_u8g2_SetFontMode(lua_State *L){
    if (u8g2 == NULL) return 0;
    int font_mode = luaL_checkinteger(L, 1);
    if (font_mode < 0) {
        lua_pushboolean(L, 0);
    }
    u8g2_SetFontMode(u8g2, font_mode);
    u8g2_SetFontDirection(u8g2, 0);
    lua_pushboolean(L, 1);
    return 1;
}

/*
设置字体
@api u8g2.SetFont(font)
@int font, u8g2.font_ncenB08_tr为纯英文8x8字节,u8g2.font_opposansm12_chinese 为12x12全中文,u8g2.font_unifont_t_symbols 为符号.
@usage
-- 设置为中文字体,对之后的drawStr有效,使用中文字体需在luat_base.h开启#define USE_U8G2_OPPOSANSM12_CHINESE
u8g2.setFont(u8g2.font_wqy12_t_gb2312)
*/
static int l_u8g2_SetFont(lua_State *L) {
    if (u8g2 == NULL) {
        LLOGI("disp not init yet!!!");
        lua_pushboolean(L, 0);
        return 1;
    }
    int font = luaL_checkinteger(L, 1);
    switch (font)
        {
        case font_opposansm8:
            LLOGI("font_opposansm8");
            u8g2_SetFont(u8g2, u8g2_font_opposansm8);
            lua_pushboolean(L, 1);
            break;
#if defined USE_U8G2_OPPOSANSM12_CHINESE
        case font_opposansm12_chinese:
            LLOGI("font_opposansm12_chinese");
            u8g2_SetFont(u8g2, u8g2_font_opposansm12_chinese);
            lua_pushboolean(L, 1);
            break;
#endif
#if defined USE_U8G2_UNIFONT_SYMBOLS
        case font_unifont_t_symbols:
            LLOGI("font_unifont_t_symbols");
            u8g2_SetFont(u8g2, u8g2_font_unifont_t_symbols);
            lua_pushboolean(L, 1);
            break;
#endif
#if defined USE_U8G2_ICONIC_WEATHER_6X
        case font_open_iconic_weather_6x_t:
            LLOGI("font_open_iconic_weather_6x_t");
            u8g2_SetFont(u8g2, u8g2_font_open_iconic_weather_6x_t);
            lua_pushboolean(L, 1);
            break;
#endif
        default:
            lua_pushboolean(L, 0);
            LLOGI("default");
            break;
        }
    return 0;
}

/*
获取显示屏高度
@api u8g2.GetDisplayHeight()
@return int 显示屏高度
@usage
u8g2.GetDisplayHeight()
*/
static int l_u8g2_GetDisplayHeight(lua_State *L){
    if (u8g2 == NULL) return 0;
    lua_pushinteger(L, u8g2_GetDisplayHeight(u8g2));
    return 1;
}

/*
获取显示屏宽度
@api u8g2.GetDisplayWidth()
@return int 显示屏宽度
@usage
u8g2.GetDisplayWidth()
*/
static int l_u8g2_GetDisplayWidth(lua_State *L){
    if (u8g2 == NULL) return 0;
    lua_pushinteger(L, u8g2_GetDisplayWidth(u8g2));
    return 1;
}

/*
画一个点.
@api u8g2.DrawPixel(x,y)
@int X位置.
@int Y位置.
@usage
u8g2.DrawPixel(20, 5)
*/
static int l_u8g2_DrawPixel(lua_State *L){
    if (u8g2 == NULL) return 0;
    u8g2_DrawPixel(u8g2,luaL_checkinteger(L, 1),luaL_checkinteger(L, 2));
    return 1;
}

/*
在两点之间画一条线.
@api u8g2.DrawLine(x0,y0,x1,y1)
@int 第一个点的X位置.
@int 第一个点的Y位置.
@int 第二个点的X位置.
@int 第二个点的Y位置.
@usage
u8g2.DrawLine(20, 5, 5, 32)
*/
static int l_u8g2_DrawLine(lua_State *L){
    if (u8g2 == NULL) return 0;
    u8g2_DrawLine(u8g2,luaL_checkinteger(L, 1),luaL_checkinteger(L, 2),luaL_checkinteger(L, 3),luaL_checkinteger(L, 4));
    return 1;
}

/*
在x,y位置画一个半径为rad的空心圆.
@api u8g2.DrawCircle(x0,y0,rad,opt)
@int 圆心位置
@int 圆心位置
@int 圆半径.
@int 选择圆的部分或全部.
右上： 0x01
左上：  0x02
左下： 0x04
右下：  0x08
完整圆： (0x01|0x02|0x04|0x08)
@usage
u8g2.DrawCircle(60,30,8,15)
*/
static int l_u8g2_DrawCircle(lua_State *L){
    if (u8g2 == NULL) return 0;
    u8g2_DrawCircle(u8g2,luaL_checkinteger(L, 1),luaL_checkinteger(L, 2),luaL_checkinteger(L, 3),luaL_checkinteger(L, 4));
    return 1;
}

/*
在x,y位置画一个半径为rad的实心圆.
@api u8g2.DrawDisc(x0,y0,rad,opt)
@int 圆心位置
@int 圆心位置
@int 圆半径.
@int 选择圆的部分或全部.
右上： 0x01
左上：  0x02
左下： 0x04
右下：  0x08
完整圆： (0x01|0x02|0x04|0x08)
@usage
u8g2.DrawDisc(60,30,8,15)
*/
static int l_u8g2_DrawDisc(lua_State *L){
    if (u8g2 == NULL) return 0;
    u8g2_DrawDisc(u8g2,luaL_checkinteger(L, 1),luaL_checkinteger(L, 2),luaL_checkinteger(L, 3),luaL_checkinteger(L, 4));
    return 1;
}

/*
在x,y位置画一个半径为rad的空心椭圆.
@api u8g2.DrawEllipse(x0,y0,rx,ry,opt)
@int 圆心位置
@int 圆心位置
@int 椭圆大小
@int 椭圆大小
@int 选择圆的部分或全部.
右上： 0x01
左上：  0x02
左下： 0x04
右下：  0x08
完整圆： (0x01|0x02|0x04|0x08)
@usage
u8g2.DrawEllipse(60,30,8,15)
*/
static int l_u8g2_DrawEllipse(lua_State *L){
    if (u8g2 == NULL) return 0;
    u8g2_DrawEllipse(u8g2,luaL_checkinteger(L, 1),luaL_checkinteger(L, 2),luaL_checkinteger(L, 3),luaL_checkinteger(L, 4),luaL_checkinteger(L, 5));
    return 0;
}

/*
在x,y位置画一个半径为rad的实心椭圆.
@api u8g2.DrawFilledEllipse(x0,y0,rx,ry,opt)
@int 圆心位置
@int 圆心位置
@int 椭圆大小
@int 椭圆大小
@int 选择圆的部分或全部.
右上： 0x01
左上：  0x02
左下： 0x04
右下：  0x08
完整圆： (0x01|0x02|0x04|0x08)
@usage
u8g2.DrawFilledEllipse(60,30,8,15)
*/
static int l_u8g2_DrawFilledEllipse(lua_State *L){
    if (u8g2 == NULL) return 0;
    u8g2_DrawFilledEllipse(u8g2,luaL_checkinteger(L, 1),luaL_checkinteger(L, 2),luaL_checkinteger(L, 3),luaL_checkinteger(L, 4),luaL_checkinteger(L, 5));
    return 0;
}

/*
从x / y位置（左上边缘）开始绘制一个框（填充的框）.
@api u8g2.DrawBox(x,y,w,h)
@int 左上边缘的X位置
@int 左上边缘的Y位置
@int 盒子的宽度
@int 盒子的高度
@usage
u8g2.DrawBox(3,7,25,15)
*/
static int l_u8g2_DrawBox(lua_State *L){
    if (u8g2 == NULL) return 0;
    u8g2_DrawBox(u8g2,luaL_checkinteger(L, 1),luaL_checkinteger(L, 2),luaL_checkinteger(L, 3),luaL_checkinteger(L, 4));
    return 0;
}

/*
从x / y位置（左上边缘）开始绘制一个框（空框）.
@api u8g2.DrawFrame(x,y,w,h)
@int 左上边缘的X位置
@int 左上边缘的Y位置
@int 盒子的宽度
@int 盒子的高度
@usage
u8g2.DrawFrame(3,7,25,15)
*/
static int l_u8g2_DrawFrame(lua_State *L){
    if (u8g2 == NULL) return 0;
    u8g2_DrawFrame(u8g2,luaL_checkinteger(L, 1),luaL_checkinteger(L, 2),luaL_checkinteger(L, 3),luaL_checkinteger(L, 4));
    return 0;
}

/*
绘制一个从x / y位置（左上边缘）开始具有圆形边缘的填充框/框架.
@api u8g2.DrawRBox(x,y,w,h,r)
@int 左上边缘的X位置
@int 左上边缘的Y位置
@int 盒子的宽度
@int 盒子的高度
@int 四个边缘的半径
@usage
u8g2.DrawRBox(3,7,25,15)
*/
static int l_u8g2_DrawRBox(lua_State *L){
    if (u8g2 == NULL) return 0;
    u8g2_DrawRBox(u8g2,luaL_checkinteger(L, 1),luaL_checkinteger(L, 2),luaL_checkinteger(L, 3),luaL_checkinteger(L, 4),luaL_checkinteger(L, 5));
    return 0;
}

/*
绘制一个从x / y位置（左上边缘）开始具有圆形边缘的空框/框架.
@api u8g2.DrawRFrame(x,y,w,h,r)
@int 左上边缘的X位置
@int 左上边缘的Y位置
@int 盒子的宽度
@int 盒子的高度
@int 四个边缘的半径
@usage
u8g2.DrawRFrame(3,7,25,15)
*/
static int l_u8g2_DrawRFrame(lua_State *L){
    if (u8g2 == NULL) return 0;
    u8g2_DrawRFrame(u8g2,luaL_checkinteger(L, 1),luaL_checkinteger(L, 2),luaL_checkinteger(L, 3),luaL_checkinteger(L, 4),luaL_checkinteger(L, 5));
    return 0;
}

/*
绘制一个图形字符。字符放置在指定的像素位置x和y.
@api u8g2.DrawGlyph(x,y,encoding)
@int 字符在显示屏上的位置
@int 字符在显示屏上的位置
@int 字符的Unicode值
@usage
u8g2.SetFont(u8g2_font_unifont_t_symbols)
u8g2.DrawGlyph(5, 20, 0x2603)	-- dec 9731/hex 2603 Snowman
*/
static int l_u8g2_DrawGlyph(lua_State *L){
    if (u8g2 == NULL) return 0;
    u8g2_DrawGlyph(u8g2,luaL_checkinteger(L, 1),luaL_checkinteger(L, 2),luaL_checkinteger(L, 3));
    return 0;
}

/*
绘制一个三角形（实心多边形）.
@api u8g2.DrawTriangle(x0,y0,x1,y1,x2,y2)
@int 点0X位置
@int 点0Y位置
@int 点1X位置
@int 点1Y位置
@int 点2X位置
@int 点2Y位置
@usage
u8g2.DrawTriangle(20,5, 27,50, 5,32)
*/
static int l_u8g2_DrawTriangle(lua_State *L){
    if (u8g2 == NULL) return 0;
    u8g2_DrawTriangle(u8g2,luaL_checkinteger(L, 1),luaL_checkinteger(L, 2),luaL_checkinteger(L, 3),luaL_checkinteger(L, 4),luaL_checkinteger(L, 5),luaL_checkinteger(L, 6));
    return 0;
}

/*
定义位图函数是否将写入背景色
@api u8g2.SetBitmapMode(mode)
@int mode字体模式，启用（1）或禁用（0）透明模式
@usage
u8g2.SetBitmapMode(1)
*/
static int l_u8g2_SetBitmapMode(lua_State *L){
    if (u8g2 == NULL) return 0;
    u8g2_SetBitmapMode(u8g2,luaL_checkinteger(L, 1));
    return 0;
}

/*
绘制位图
@api u8g2.DrawBitmap(x, y, h, data)
@int X坐标
@int y坐标
@int 行数
@int 位图数据,每一位代表一个像素
@usage
-- 在(10,10)为左上角,绘制 10x4 的位图
u8g2.DrawBitmapMode(10, 10, 10, string.char(0x20, 0xFF, 0xFF, 0xAF, 0xDE))
*/
static int l_u8g2_DrawBitmap(lua_State *L){
    if (u8g2 == NULL) return 0;
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int h = luaL_checkinteger(L, 3);
    size_t len = 0;
    const char* data = luaL_checklstring(L, 4, &len);
    if (h < 1) return 0; // 行数必须大于0
    if (len < h) return 0; // 起码要填满一行
    //if (len % h != 0) return 0; // 必须是行数的整数倍
    u8g2_DrawBitmap(u8g2, x, y, len, h, (const uint8_t*)data);
    lua_pushboolean(L, 1);
    return 1;
}


#ifdef LUAT_USE_GTFONT

#include "GT5SLCD2E_1A.h"
extern void gtfont_draw_w(unsigned char *pBits,unsigned int x,unsigned int y,unsigned int widt,unsigned int high,int(*point)(void*),void* userdata,int mode);
extern void gtfont_draw_gray_hz(unsigned char *data,unsigned short x,unsigned short y,unsigned short w ,unsigned short h,unsigned char grade, unsigned char HB_par,int(*point)(void*,uint16_t, uint16_t, uint32_t),void* userdata,int mode);

static int gtfont_u8g2_DrawPixel(u8g2_t *u8g2, uint16_t x, uint16_t y,uint32_t color){
    u8g2_DrawPixel(u8g2,x, y);
    return 1;
}


/*
使用gtfont显示gb2312字符串
@api u8g2.drawGtfontGb2312(str,size,x,y)
@string str 显示字符串
@int size 字体大小 (支持16-192号大小字体)
@int x 横坐标
@int y 竖坐标
@usage
u8g2.drawGtfontGb2312("啊啊啊",32,0,0)
*/
static int l_u8g2_draw_gtfont_gb2312(lua_State *L) {
    unsigned char buf[128];
	int len;
	int i = 0;
	uint8_t strhigh,strlow ;
	uint16_t str;
    const char *fontCode = luaL_checklstring(L, 1,&len);
    unsigned char size = luaL_checkinteger(L, 2);
	int x = luaL_checkinteger(L, 3);
	int y = luaL_checkinteger(L, 4);
	while ( i < len){
		strhigh = *fontCode;
		fontCode++;
		strlow = *fontCode;
		str = (strhigh<<8)|strlow;
		fontCode++;
		get_font(buf, 1, str, size, size, size);
		gtfont_draw_w(buf , x ,y , size , size,gtfont_u8g2_DrawPixel,u8g2,2);
		x+=size;
		i+=2;
	}
    return 0;
}

#ifdef LUAT_USE_GTFONT_UTF8
extern unsigned short unicodetogb2312 ( unsigned short	chr);

static uint8_t utf8_state;
static uint16_t encoding;
static uint16_t utf8_next(uint8_t b)
{
  if ( b == 0 )  /* '\n' terminates the string to support the string list procedures */
    return 0x0ffff; /* end of string detected, pending UTF8 is discarded */
  if ( utf8_state == 0 )
  {
    if ( b >= 0xfc )  /* 6 byte sequence */
    {
      utf8_state = 5;
      b &= 1;
    }
    else if ( b >= 0xf8 )
    {
      utf8_state = 4;
      b &= 3;
    }
    else if ( b >= 0xf0 )
    {
      utf8_state = 3;
      b &= 7;
    }
    else if ( b >= 0xe0 )
    {
      utf8_state = 2;
      b &= 15;
    }
    else if ( b >= 0xc0 )
    {
      utf8_state = 1;
      b &= 0x01f;
    }
    else
    {
      /* do nothing, just use the value as encoding */
      return b;
    }
    encoding = b;
    return 0x0fffe;
  }
  else
  {
    utf8_state--;
    /* The case b < 0x080 (an illegal UTF8 encoding) is not checked here. */
    encoding<<=6;
    b &= 0x03f;
    encoding |= b;
    if ( utf8_state != 0 )
      return 0x0fffe; /* nothing to do yet */
  }
  return encoding;
}
/*
使用gtfont显示UTF8字符串
@api u8g2.drawGtfontUtf8(str,size,x,y)
@string str 显示字符串
@int size 字体大小 (支持16-192号大小字体)
@int x 横坐标
@int y 竖坐标
@usage
u8g2.drawGtfontUtf8("啊啊啊",32,0,0)
*/
static int l_u8g2_draw_gtfont_utf8(lua_State *L) {
    unsigned char buf[128];
    int len;
    int i = 0;
    uint8_t strhigh,strlow ;
    uint16_t e,str;
    const char *fontCode = luaL_checklstring(L, 1,&len);
    unsigned char size = luaL_checkinteger(L, 2);
    int x = luaL_checkinteger(L, 3);
    int y = luaL_checkinteger(L, 4);
    for(;;){
        e = utf8_next((uint8_t)*fontCode);
        if ( e == 0x0ffff )
        break;
        fontCode++;
        if ( e != 0x0fffe ){
            uint16_t str = unicodetogb2312(e);
            get_font(buf, 1, str, size, size, size);
            gtfont_draw_w(buf , x ,y , size , size,gtfont_u8g2_DrawPixel,u8g2,2);
            x+=size;    
        }
    }
    return 0;
}

#endif // LUAT_USE_GTFONT_UTF8

#endif // LUAT_USE_GTFONT

#include "rotable.h"
static const rotable_Reg reg_u8g2[] =
{
    { "begin",       l_u8g2_begin,        0},
    { "close",      l_u8g2_close,       0},
    { "ClearBuffer",      l_u8g2_ClearBuffer,       0},
    { "SendBuffer",     l_u8g2_SendBuffer,      0},
    { "DrawUTF8",    l_u8g2_DrawUTF8,   0},
    { "SetFontMode",    l_u8g2_SetFontMode,    0},
    { "SetFont",    l_u8g2_SetFont,    0},
    { "GetDisplayHeight",    l_u8g2_GetDisplayHeight,    0},
    { "GetDisplayWidth",    l_u8g2_GetDisplayWidth,    0},
    { "DrawPixel",    l_u8g2_DrawPixel,    0},
    { "DrawLine",    l_u8g2_DrawLine,    0},
    { "DrawCircle",    l_u8g2_DrawCircle,    0},
    { "DrawDisc",    l_u8g2_DrawDisc,    0},
    { "DrawEllipse",    l_u8g2_DrawEllipse,    0},
    { "DrawFilledEllipse",    l_u8g2_DrawFilledEllipse,    0},
    { "DrawBox",    l_u8g2_DrawBox,    0},
    { "DrawFrame",    l_u8g2_DrawFrame,    0},
    { "DrawRBox",    l_u8g2_DrawRBox,    0},
    { "DrawRFrame",    l_u8g2_DrawRFrame,    0},
    { "DrawGlyph",    l_u8g2_DrawGlyph,    0},
    { "DrawTriangle",    l_u8g2_DrawTriangle,    0},
    { "SetBitmapMode",    l_u8g2_SetBitmapMode,    0},
    { "DrawBitmap",       l_u8g2_DrawBitmap, 0},
#ifdef LUAT_USE_GTFONT
    { "drawGtfontGb2312", l_u8g2_draw_gtfont_gb2312, 0},
#ifdef LUAT_USE_GTFONT_UTF8
    { "drawGtfontUtf8", l_u8g2_draw_gtfont_utf8, 0},
#endif // LUAT_USE_GTFONT_UTF8
#endif // LUAT_USE_GTFONT
    { "font_opposansm8", NULL,       font_opposansm8},
    { "font_opposansm12_chinese", NULL,       font_opposansm12_chinese},
    { "font_unifont_t_symbols", NULL,       font_unifont_t_symbols},
    { "font_open_iconic_weather_6x_t", NULL,       font_open_iconic_weather_6x_t},
	{ NULL, NULL, 0}
};

LUAMOD_API int luaopen_u8g2( lua_State *L ) {
    u8g2_lua_ref = 0;
    u8g2 = NULL;
    luat_newlib(L, reg_u8g2);
    return 1;
}

//-----------------------------
// 往下是一些U8G2方法的默认实现

uint8_t u8x8_luat_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_luat_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_luat_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

LUAT_WEAK int luat_u8g2_setup(luat_u8g2_conf_t *conf) {
    if (conf->pinType == 1) {
        u8g2_t* u8g2 = (u8g2_t*)conf->ptr;
        if (strncmp("ssd1306", conf->cname, 7) == 0 || strncmp("SSD1306", conf->cname, 7) == 0){
            u8g2_Setup_ssd1306_i2c_128x64_noname_f( u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_luat_gpio_and_delay);
#ifdef U8G2_USE_SH1106
        }else if (strncmp("sh1106", conf->cname, 6) == 0 || strncmp("SH1106", conf->cname, 6) == 0){
            u8g2_Setup_sh1106_i2c_128x64_noname_f( u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_luat_gpio_and_delay);
#endif
        }else{
            u8g2_Setup_ssd1306_i2c_128x64_noname_f( u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_luat_gpio_and_delay);
        }
        u8g2->u8x8.pins[U8X8_PIN_I2C_CLOCK] = conf->pin0;
        u8g2->u8x8.pins[U8X8_PIN_I2C_DATA] = conf->pin1;
        LLOGD("setup disp i2c.sw SCL=%ld SDA=%ld", conf->pin0, conf->pin1);
        u8g2_InitDisplay(u8g2);
        u8g2_SetPowerSave(u8g2, 0);
        return 0;
    }
    else if (conf->pinType == 2) {
        u8g2_t* u8g2 = (u8g2_t*)conf->ptr;
        if (strncmp("ssd1306", conf->cname, 7) == 0 || strncmp("SSD1306", conf->cname, 7) == 0){
            u8g2_Setup_ssd1306_i2c_128x64_noname_f( u8g2, U8G2_R0, u8x8_luat_byte_hw_i2c, u8x8_luat_gpio_and_delay);
#ifdef U8G2_USE_SH1106
        }else if (strncmp("sh1106", conf->cname, 6) == 0 || strncmp("SH1106", conf->cname, 6) == 0){
            u8g2_Setup_sh1106_i2c_128x64_noname_f( u8g2, U8G2_R0, u8x8_luat_byte_hw_i2c, u8x8_luat_gpio_and_delay);
#endif
        }else{
            u8g2_Setup_ssd1306_i2c_128x64_noname_f( u8g2, U8G2_R0, u8x8_luat_byte_hw_i2c, u8x8_luat_gpio_and_delay);
        }
        LLOGD("setup disp i2c.hw");
        u8g2_InitDisplay(u8g2);
        u8g2_SetPowerSave(u8g2, 0);
        return 0;
    }
    else if (conf->pinType == 5) {
        u8g2_t* u8g2 = (u8g2_t*)conf->ptr;
        if (strncmp("ssd1306", conf->cname, 7) == 0 || strncmp("SSD1306", conf->cname, 7) == 0){
            u8g2_Setup_ssd1306_128x64_noname_f( u8g2, U8G2_R0, u8x8_luat_byte_4wire_hw_spi, u8x8_luat_gpio_and_delay);
#ifdef U8G2_USE_SH1106
        }else if (strncmp("sh1106", conf->cname, 6) == 0 || strncmp("SH1106", conf->cname, 6) == 0){
            u8g2_Setup_sh1106_128x64_noname_f( u8g2, U8G2_R0, u8x8_luat_byte_4wire_hw_spi, u8x8_luat_gpio_and_delay);
#endif
#ifdef U8G2_USE_ST7567
        }else if (strncmp("st7567", conf->cname, 6) == 0 || strncmp("ST7567", conf->cname, 6) == 0){
            u8g2_Setup_st7567_jlx12864_f( u8g2, U8G2_R0, u8x8_luat_byte_4wire_hw_spi, u8x8_luat_gpio_and_delay);
#endif
        }else{
            u8g2_Setup_ssd1306_128x64_noname_f( u8g2, U8G2_R0, u8x8_luat_byte_4wire_hw_spi, u8x8_luat_gpio_and_delay);
        }
        LLOGD("setup disp spi.hw");
        u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_CS, spi_cs);
        u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_DC, spi_dc);
        u8x8_SetPin(u8g2_GetU8x8(u8g2), U8X8_PIN_RESET, spi_res);
        u8g2_InitDisplay(u8g2);
        u8g2_SetPowerSave(u8g2, 0);
        return 0;
    }
    LLOGI("only i2c sw mode is support, by default impl");
    return -1;
}

LUAT_WEAK int luat_u8g2_close(luat_u8g2_conf_t *conf) {
    return 0;
}

LUAT_WEAK uint8_t u8x8_luat_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
  static uint8_t buffer[32];		/* u8g2/u8x8 will never send more than 32 bytes */
  static uint8_t buf_idx;
  uint8_t *data;

  switch(msg)
  {
    case U8X8_MSG_BYTE_SEND:
      data = (uint8_t *)arg_ptr;
      while( arg_int > 0 )
      {
        buffer[buf_idx++] = *data;
        data++;
        arg_int--;
      }
      break;
    case U8X8_MSG_BYTE_INIT:
      //i2c_init(u8x8);			/* init i2c communication */
      luat_i2c_setup(i2c_id,i2c_speed,NULL);
      break;
    case U8X8_MSG_BYTE_SET_DC:
      /* ignored for i2c */
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      buf_idx = 0;
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:
      luat_i2c_send(i2c_id, u8x8_GetI2CAddress(u8x8) >> 1, buffer, buf_idx);
      break;
    default:
      return 0;
  }
  return 1;
}

int hw_spi_begin(uint8_t spi_mode, uint32_t max_hz, uint8_t cs_pin )
{

    luat_spi_t u8g2_spi = {0};
    u8g2_spi.id = spi_id;
    switch(spi_mode)
    {
        case 0: u8g2_spi.CPHA = 0;u8g2_spi.CPOL = 0; break;
        case 1: u8g2_spi.CPHA = 1;u8g2_spi.CPOL = 0; break;
        case 2: u8g2_spi.CPHA = 0;u8g2_spi.CPOL = 1; break;
        case 3: u8g2_spi.CPHA = 1;u8g2_spi.CPOL = 1; break;
    }
    u8g2_spi.dataw = 8;
    u8g2_spi.bit_dict = 1;
    u8g2_spi.master = 1;
    u8g2_spi.mode = 1;
    u8g2_spi.bandrate = max_hz;
    u8g2_spi.cs = cs_pin;
    LLOGI("cs_pin=%d",cs_pin);
    luat_spi_setup(&u8g2_spi);
    return 1;
}
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
uint8_t u8x8_luat_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {

    uint8_t i;
    uint8_t *data;

    uint8_t tx[256];
    uint8_t rx[256];

    static uint8_t buf_idx;
    static uint8_t buffer_tx[256];

    switch(msg)
    {
        case U8X8_MSG_BYTE_SEND:
            data = (uint8_t *)arg_ptr;

            while( arg_int > 0)
            {
                buffer_tx[buf_idx++] = (uint8_t)*data;
                luat_spi_send(spi_id, (const char*)data, 1);
                data++;
                arg_int--;
            }
            //luat_spi_send(spi_id, (uint8_t*)data, arg_int);
            break;

        case U8X8_MSG_BYTE_INIT:
            /* SPI mode has to be mapped to the mode of the current controller, at least Uno, Due, 101 have different SPI_MODEx values */
            /*   0: clock active high, data out on falling edge, clock default value is zero, takover on rising edge */
            /*   1: clock active high, data out on rising edge, clock default value is zero, takover on falling edge */
            /*   2: clock active low, data out on rising edge */
            /*   3: clock active low, data out on falling edge */
            u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
            hw_spi_begin(u8x8->display_info->spi_mode, u8x8->display_info->sck_clock_hz, u8x8->pins[U8X8_PIN_CS]);
            break;

        case U8X8_MSG_BYTE_SET_DC:
            u8x8_gpio_SetDC(u8x8, arg_int);
            break;

        case U8X8_MSG_BYTE_START_TRANSFER:
            u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_enable_level);
            u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);
            break;

        case U8X8_MSG_BYTE_END_TRANSFER:
            memset( tx, 0, ARRAY_SIZE(tx)*sizeof(uint8_t) );
            memset( rx, 0, ARRAY_SIZE(rx)*sizeof(uint8_t) );

            for (i = 0; i < buf_idx; ++i)
            {
                tx[i] = buffer_tx[i];
            }
            u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
            u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
            buf_idx = 0;
            break;

        default:
            return 0;
    }
    return 1;
}

LUAT_WEAK uint8_t u8x8_luat_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch(msg)
    {
        case U8X8_MSG_DELAY_NANO:            // delay arg_int * 1 nano second
            __asm__ volatile("nop");
            break;

        case U8X8_MSG_DELAY_100NANO:        // delay arg_int * 100 nano seconds
            __asm__ volatile("nop");
            break;

        case U8X8_MSG_DELAY_10MICRO:        // delay arg_int * 10 micro seconds
            for (uint16_t n = 0; n < 320; n++)
            {
                __asm__ volatile("nop");
            }
        break;

        case U8X8_MSG_DELAY_MILLI:            // delay arg_int * 1 milli second
            luat_timer_mdelay(arg_int);
            break;

        case U8X8_MSG_GPIO_AND_DELAY_INIT:
            // Function which implements a delay, arg_int contains the amount of ms

            // set spi pin mode
            if (u8x8->pins[U8X8_PIN_SPI_CLOCK] != 255) {
                luat_gpio_mode(u8x8->pins[U8X8_PIN_SPI_CLOCK],Luat_GPIO_OUTPUT, Luat_GPIO_PULLUP, Luat_GPIO_HIGH);//d0 a5 15 d1 a7 17 res b0 18 dc b1 19 cs a4 14
                luat_gpio_mode(u8x8->pins[U8X8_PIN_SPI_DATA],Luat_GPIO_OUTPUT, Luat_GPIO_PULLUP, Luat_GPIO_HIGH);
                luat_gpio_mode(u8x8->pins[U8X8_PIN_RESET],Luat_GPIO_OUTPUT, Luat_GPIO_PULLUP, Luat_GPIO_HIGH);
                luat_gpio_mode(u8x8->pins[U8X8_PIN_DC],Luat_GPIO_OUTPUT, Luat_GPIO_PULLUP, Luat_GPIO_HIGH);
                luat_gpio_mode(u8x8->pins[U8X8_PIN_CS],Luat_GPIO_OUTPUT, Luat_GPIO_PULLUP, Luat_GPIO_HIGH);
            }

            // set i2c pin mode
            if (u8x8->pins[U8X8_PIN_I2C_DATA] != 255) {
                luat_gpio_mode(u8x8->pins[U8X8_PIN_I2C_DATA],Luat_GPIO_OUTPUT, Luat_GPIO_DEFAULT, Luat_GPIO_HIGH);
                luat_gpio_mode(u8x8->pins[U8X8_PIN_I2C_CLOCK],Luat_GPIO_OUTPUT, Luat_GPIO_DEFAULT, Luat_GPIO_HIGH);
            }

            // set 8080 pin mode
            if (u8x8->pins[U8X8_PIN_D0] != 255) {
                luat_gpio_mode(u8x8->pins[U8X8_PIN_D0],Luat_GPIO_OUTPUT, Luat_GPIO_PULLUP, Luat_GPIO_HIGH);
                luat_gpio_mode(u8x8->pins[U8X8_PIN_D1],Luat_GPIO_OUTPUT, Luat_GPIO_PULLUP, Luat_GPIO_HIGH);
                luat_gpio_mode(u8x8->pins[U8X8_PIN_D2],Luat_GPIO_OUTPUT, Luat_GPIO_PULLUP, Luat_GPIO_HIGH);
                luat_gpio_mode(u8x8->pins[U8X8_PIN_D3],Luat_GPIO_OUTPUT, Luat_GPIO_PULLUP, Luat_GPIO_HIGH);
                luat_gpio_mode(u8x8->pins[U8X8_PIN_D4],Luat_GPIO_OUTPUT, Luat_GPIO_PULLUP, Luat_GPIO_HIGH);
                luat_gpio_mode(u8x8->pins[U8X8_PIN_D5],Luat_GPIO_OUTPUT, Luat_GPIO_PULLUP, Luat_GPIO_HIGH);
                luat_gpio_mode(u8x8->pins[U8X8_PIN_D6],Luat_GPIO_OUTPUT, Luat_GPIO_PULLUP, Luat_GPIO_HIGH);
                luat_gpio_mode(u8x8->pins[U8X8_PIN_D7],Luat_GPIO_OUTPUT, Luat_GPIO_PULLUP, Luat_GPIO_HIGH);
                luat_gpio_mode(u8x8->pins[U8X8_PIN_E],Luat_GPIO_OUTPUT, Luat_GPIO_PULLUP, Luat_GPIO_HIGH);
                luat_gpio_mode(u8x8->pins[U8X8_PIN_DC],Luat_GPIO_OUTPUT, Luat_GPIO_PULLUP, Luat_GPIO_HIGH);
                luat_gpio_mode(u8x8->pins[U8X8_PIN_RESET],Luat_GPIO_OUTPUT, Luat_GPIO_PULLUP, Luat_GPIO_HIGH);
            }

            // set value
            luat_gpio_set(u8x8->pins[U8X8_PIN_SPI_CLOCK],1);
            luat_gpio_set(u8x8->pins[U8X8_PIN_SPI_DATA],1);
            luat_gpio_set(u8x8->pins[U8X8_PIN_RESET],1);
            luat_gpio_set(u8x8->pins[U8X8_PIN_DC],1);
            luat_gpio_set(u8x8->pins[U8X8_PIN_CS],1);
            break;

        case U8X8_MSG_DELAY_I2C:
            // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
            // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
            for (uint16_t n = 0; n < (arg_int<=2?160:40); n++)
            {
                __asm__ volatile("nop");
            }
            break;

        //case U8X8_MSG_GPIO_D0:                // D0 or SPI clock pin: Output level in arg_int
        //case U8X8_MSG_GPIO_SPI_CLOCK:

        //case U8X8_MSG_GPIO_D1:                // D1 or SPI data pin: Output level in arg_int
        //case U8X8_MSG_GPIO_SPI_DATA:

        case U8X8_MSG_GPIO_D2:                // D2 pin: Output level in arg_int
            if (arg_int) luat_gpio_set(u8x8->pins[U8X8_PIN_D2],1);
            else luat_gpio_set(u8x8->pins[U8X8_PIN_D2],0);
            break;

        case U8X8_MSG_GPIO_D3:                // D3 pin: Output level in arg_int
            if (arg_int) luat_gpio_set(u8x8->pins[U8X8_PIN_D3],1);
            else luat_gpio_set(u8x8->pins[U8X8_PIN_D3],0);
            break;

        case U8X8_MSG_GPIO_D4:                // D4 pin: Output level in arg_int
            if (arg_int) luat_gpio_set(u8x8->pins[U8X8_PIN_D4],1);
            else luat_gpio_set(u8x8->pins[U8X8_PIN_D4],0);
            break;

        case U8X8_MSG_GPIO_D5:                // D5 pin: Output level in arg_int
            if (arg_int) luat_gpio_set(u8x8->pins[U8X8_PIN_D5],1);
            else luat_gpio_set(u8x8->pins[U8X8_PIN_D5],0);
            break;

        case U8X8_MSG_GPIO_D6:                // D6 pin: Output level in arg_int
            if (arg_int) luat_gpio_set(u8x8->pins[U8X8_PIN_D6],1);
            else luat_gpio_set(u8x8->pins[U8X8_PIN_D6],0);
            break;

        case U8X8_MSG_GPIO_D7:                // D7 pin: Output level in arg_int
            if (arg_int) luat_gpio_set(u8x8->pins[U8X8_PIN_D7],1);
            else luat_gpio_set(u8x8->pins[U8X8_PIN_D7],0);
            break;

        case U8X8_MSG_GPIO_E:                // E/WR pin: Output level in arg_int
            if (arg_int) luat_gpio_set(u8x8->pins[U8X8_PIN_E],1);
            else luat_gpio_set(u8x8->pins[U8X8_PIN_E],0);
            break;

        case U8X8_MSG_GPIO_I2C_CLOCK:
            // arg_int=0: Output low at I2C clock pin
            // arg_int=1: Input dir with pullup high for I2C clock pin
            if (arg_int) luat_gpio_set(u8x8->pins[U8X8_PIN_I2C_CLOCK],1);
            else luat_gpio_set(u8x8->pins[U8X8_PIN_I2C_CLOCK],0);
            break;

        case U8X8_MSG_GPIO_I2C_DATA:
            // arg_int=0: Output low at I2C data pin
            // arg_int=1: Input dir with pullup high for I2C data pin
            if (arg_int) luat_gpio_set(u8x8->pins[U8X8_PIN_I2C_DATA],1);
            else luat_gpio_set(u8x8->pins[U8X8_PIN_I2C_DATA],0);
      break;

        case U8X8_MSG_GPIO_SPI_CLOCK:
            //Function to define the logic level of the clockline
            if (arg_int) luat_gpio_set(u8x8->pins[U8X8_PIN_SPI_CLOCK],1);
            else luat_gpio_set(u8x8->pins[U8X8_PIN_SPI_CLOCK],0);
            break;

        case U8X8_MSG_GPIO_SPI_DATA:
            //Function to define the logic level of the data line to the display
            if (arg_int) luat_gpio_set(u8x8->pins[U8X8_PIN_SPI_DATA],1);
            else luat_gpio_set(u8x8->pins[U8X8_PIN_SPI_DATA],0);
            break;

        case U8X8_MSG_GPIO_CS:
            // Function to define the logic level of the CS line
            if(arg_int) luat_gpio_set(u8x8->pins[U8X8_PIN_CS],1);
            else luat_gpio_set(u8x8->pins[U8X8_PIN_CS],0);
            break;

        case U8X8_MSG_GPIO_DC:
            //Function to define the logic level of the Data/ Command line
            if(arg_int) luat_gpio_set(u8x8->pins[U8X8_PIN_DC],1);
            else luat_gpio_set(u8x8->pins[U8X8_PIN_DC],0);
            break;

        case U8X8_MSG_GPIO_RESET:
            //Function to define the logic level of the RESET line
            if (arg_int) luat_gpio_set(u8x8->pins[U8X8_PIN_RESET],1);
            else luat_gpio_set(u8x8->pins[U8X8_PIN_RESET],0);
            break;

        default:
            //A message was received which is not implemented, return 0 to indicate an error
            return 0;
    }
    return 1;
}

