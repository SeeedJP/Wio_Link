/*
 * grove_oled_12864.cpp
 *
 * Copyright (c) 2012 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Jack Shao
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include "suli2.h"
#include "grove_oled_12864.h"
#include "base64.h"

const uint8_t BasicFont[][8] =
{
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x5F,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x07,0x00,0x07,0x00,0x00,0x00},
    {0x00,0x14,0x7F,0x14,0x7F,0x14,0x00,0x00},
    {0x00,0x24,0x2A,0x7F,0x2A,0x12,0x00,0x00},
    {0x00,0x23,0x13,0x08,0x64,0x62,0x00,0x00},
    {0x00,0x36,0x49,0x55,0x22,0x50,0x00,0x00},
    {0x00,0x00,0x05,0x03,0x00,0x00,0x00,0x00},
    {0x00,0x1C,0x22,0x41,0x00,0x00,0x00,0x00},
    {0x00,0x41,0x22,0x1C,0x00,0x00,0x00,0x00},
    {0x00,0x08,0x2A,0x1C,0x2A,0x08,0x00,0x00},
    {0x00,0x08,0x08,0x3E,0x08,0x08,0x00,0x00},
    {0x00,0xA0,0x60,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x08,0x08,0x08,0x08,0x08,0x00,0x00},
    {0x00,0x60,0x60,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x20,0x10,0x08,0x04,0x02,0x00,0x00},
    {0x00,0x3E,0x51,0x49,0x45,0x3E,0x00,0x00},
    {0x00,0x00,0x42,0x7F,0x40,0x00,0x00,0x00},
    {0x00,0x62,0x51,0x49,0x49,0x46,0x00,0x00},
    {0x00,0x22,0x41,0x49,0x49,0x36,0x00,0x00},
    {0x00,0x18,0x14,0x12,0x7F,0x10,0x00,0x00},
    {0x00,0x27,0x45,0x45,0x45,0x39,0x00,0x00},
    {0x00,0x3C,0x4A,0x49,0x49,0x30,0x00,0x00},
    {0x00,0x01,0x71,0x09,0x05,0x03,0x00,0x00},
    {0x00,0x36,0x49,0x49,0x49,0x36,0x00,0x00},
    {0x00,0x06,0x49,0x49,0x29,0x1E,0x00,0x00},
    {0x00,0x00,0x36,0x36,0x00,0x00,0x00,0x00},
    {0x00,0x00,0xAC,0x6C,0x00,0x00,0x00,0x00},
    {0x00,0x08,0x14,0x22,0x41,0x00,0x00,0x00},
    {0x00,0x14,0x14,0x14,0x14,0x14,0x00,0x00},
    {0x00,0x41,0x22,0x14,0x08,0x00,0x00,0x00},
    {0x00,0x02,0x01,0x51,0x09,0x06,0x00,0x00},
    {0x00,0x32,0x49,0x79,0x41,0x3E,0x00,0x00},
    {0x00,0x7E,0x09,0x09,0x09,0x7E,0x00,0x00},
    {0x00,0x7F,0x49,0x49,0x49,0x36,0x00,0x00},
    {0x00,0x3E,0x41,0x41,0x41,0x22,0x00,0x00},
    {0x00,0x7F,0x41,0x41,0x22,0x1C,0x00,0x00},
    {0x00,0x7F,0x49,0x49,0x49,0x41,0x00,0x00},
    {0x00,0x7F,0x09,0x09,0x09,0x01,0x00,0x00},
    {0x00,0x3E,0x41,0x41,0x51,0x72,0x00,0x00},
    {0x00,0x7F,0x08,0x08,0x08,0x7F,0x00,0x00},
    {0x00,0x41,0x7F,0x41,0x00,0x00,0x00,0x00},
    {0x00,0x20,0x40,0x41,0x3F,0x01,0x00,0x00},
    {0x00,0x7F,0x08,0x14,0x22,0x41,0x00,0x00},
    {0x00,0x7F,0x40,0x40,0x40,0x40,0x00,0x00},
    {0x00,0x7F,0x02,0x0C,0x02,0x7F,0x00,0x00},
    {0x00,0x7F,0x04,0x08,0x10,0x7F,0x00,0x00},
    {0x00,0x3E,0x41,0x41,0x41,0x3E,0x00,0x00},
    {0x00,0x7F,0x09,0x09,0x09,0x06,0x00,0x00},
    {0x00,0x3E,0x41,0x51,0x21,0x5E,0x00,0x00},
    {0x00,0x7F,0x09,0x19,0x29,0x46,0x00,0x00},
    {0x00,0x26,0x49,0x49,0x49,0x32,0x00,0x00},
    {0x00,0x01,0x01,0x7F,0x01,0x01,0x00,0x00},
    {0x00,0x3F,0x40,0x40,0x40,0x3F,0x00,0x00},
    {0x00,0x1F,0x20,0x40,0x20,0x1F,0x00,0x00},
    {0x00,0x3F,0x40,0x38,0x40,0x3F,0x00,0x00},
    {0x00,0x63,0x14,0x08,0x14,0x63,0x00,0x00},
    {0x00,0x03,0x04,0x78,0x04,0x03,0x00,0x00},
    {0x00,0x61,0x51,0x49,0x45,0x43,0x00,0x00},
    {0x00,0x7F,0x41,0x41,0x00,0x00,0x00,0x00},
    {0x00,0x02,0x04,0x08,0x10,0x20,0x00,0x00},
    {0x00,0x41,0x41,0x7F,0x00,0x00,0x00,0x00},
    {0x00,0x04,0x02,0x01,0x02,0x04,0x00,0x00},
    {0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00},
    {0x00,0x01,0x02,0x04,0x00,0x00,0x00,0x00},
    {0x00,0x20,0x54,0x54,0x54,0x78,0x00,0x00},
    {0x00,0x7F,0x48,0x44,0x44,0x38,0x00,0x00},
    {0x00,0x38,0x44,0x44,0x28,0x00,0x00,0x00},
    {0x00,0x38,0x44,0x44,0x48,0x7F,0x00,0x00},
    {0x00,0x38,0x54,0x54,0x54,0x18,0x00,0x00},
    {0x00,0x08,0x7E,0x09,0x02,0x00,0x00,0x00},
    {0x00,0x18,0xA4,0xA4,0xA4,0x7C,0x00,0x00},
    {0x00,0x7F,0x08,0x04,0x04,0x78,0x00,0x00},
    {0x00,0x00,0x7D,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x80,0x84,0x7D,0x00,0x00,0x00,0x00},
    {0x00,0x7F,0x10,0x28,0x44,0x00,0x00,0x00},
    {0x00,0x41,0x7F,0x40,0x00,0x00,0x00,0x00},
    {0x00,0x7C,0x04,0x18,0x04,0x78,0x00,0x00},
    {0x00,0x7C,0x08,0x04,0x7C,0x00,0x00,0x00},
    {0x00,0x38,0x44,0x44,0x38,0x00,0x00,0x00},
    {0x00,0xFC,0x24,0x24,0x18,0x00,0x00,0x00},
    {0x00,0x18,0x24,0x24,0xFC,0x00,0x00,0x00},
    {0x00,0x00,0x7C,0x08,0x04,0x00,0x00,0x00},
    {0x00,0x48,0x54,0x54,0x24,0x00,0x00,0x00},
    {0x00,0x04,0x7F,0x44,0x00,0x00,0x00,0x00},
    {0x00,0x3C,0x40,0x40,0x7C,0x00,0x00,0x00},
    {0x00,0x1C,0x20,0x40,0x20,0x1C,0x00,0x00},
    {0x00,0x3C,0x40,0x30,0x40,0x3C,0x00,0x00},
    {0x00,0x44,0x28,0x10,0x28,0x44,0x00,0x00},
    {0x00,0x1C,0xA0,0xA0,0x7C,0x00,0x00,0x00},
    {0x00,0x44,0x64,0x54,0x4C,0x44,0x00,0x00},
    {0x00,0x08,0x36,0x41,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x7F,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x41,0x36,0x08,0x00,0x00,0x00,0x00},
    {0x00,0x02,0x01,0x01,0x02,0x01,0x00,0x00},
    {0x00,0x02,0x05,0x05,0x02,0x00,0x00,0x00}
};


GroveOLED12864::GroveOLED12864(int pinsda, int pinscl)
{
    this->i2c = (I2C_T *)malloc(sizeof(I2C_T));
    suli_i2c_init(this->i2c, pinsda, pinscl);
    oled_128x64_cmd(SeeedOLED_Display_Off_Cmd);     //display off
    suli_delay_ms(5);
    oled_128x64_cmd(SeeedOLED_Display_On_Cmd);     //display on
    suli_delay_ms(5);
    oled_128x64_cmd(SeeedOLED_Normal_Display_Cmd);  //Set Normal Display (default)
}

// send command
void GroveOLED12864::oled_128x64_cmd(uint8_t command)
{
    suli_i2c_write_reg(i2c, SeeedOLED_Address, SeeedOLED_Command_Mode, &command, 1);
}

// send data
void GroveOLED12864::oled_128x64_dta(uint8_t data)
{
    suli_i2c_write_reg(i2c, SeeedOLED_Address, SeeedOLED_Data_Mode, &data, 1);
}

// horizontal mode
void GroveOLED12864::oled_128x64_hmode()
{
    addressingMode = HORIZONTAL_MODE;
    oled_128x64_cmd(0x20);             //set addressing mode
    oled_128x64_cmd(0x00);             //set horizontal addressing mode
}

// page mode
void GroveOLED12864::oled_128x64_pagemode()
{
    addressingMode = PAGE_MODE;
    oled_128x64_cmd(0x20);             //set addressing mode
    oled_128x64_cmd(0x02);             //set page addressing mode
}

void GroveOLED12864::oled_128x64_XY(uint8_t row, uint8_t column)
{
    last_row = row;
    oled_128x64_cmd(0xB0 + row);             //set page address
    oled_128x64_cmd(0x00 + (8*column & 0x0F));     //set column lower address
    oled_128x64_cmd(0x10 + ((8*column>>4)&0x0F));     //set column higher address
}

void GroveOLED12864::oled_128x64_char(uint8_t C)
{
    if (C == 255)
    {
        return;
    }

    if(C < 32 || C > 127) //Ignore non-printable ASCII characters. This can be modified for multilingual font.
    {
        if (C=='\r')
        {
            oled_128x64_XY(last_row, 0);
            return;
        } else if (C=='\n')
        {
            oled_128x64_XY(last_row + 1, 0);
            return;
        } else
        {
            C = ' ';
        }
    }
    unsigned char i=0;
    for(i=0;i<8;i++)
    {
        oled_128x64_dta(BasicFont[C-32][i]);
    } 
}

uint8_t GroveOLED12864::oled_128x64_num(int32_t long_num)
{
    unsigned char char_buffer[10]="";
    unsigned char i = 0;
    unsigned char f = 0;

    if (long_num < 0)
    {
        f=1;
        oled_128x64_char('-');
        long_num = -long_num;
    }
    else if (long_num == 0)
    {
        f=1;
        oled_128x64_char('0');
        return f;
    }

    while (long_num > 0)
    {
        char_buffer[i++] = long_num % 10;
        long_num /= 10;
    }

    f=f+i;
    for(; i > 0; i--)
    {
        oled_128x64_char('0'+ char_buffer[i - 1]);
    }
    return f;
}

void GroveOLED12864::oled_128x64_string(const char *str)
{
    unsigned char i=0;
    while(str[i])
    {
        oled_128x64_char(str[i]);
        i++;
    }
}

void GroveOLED12864::oled_128x64_hsp(bool direction, uint8_t startPage, uint8_t endPage, uint8_t scrollSpeed)
{
    if(Scroll_Right == direction)
    {
        //Scroll Right
        oled_128x64_cmd(0x26);
    }
    else
    {
        //Scroll Left
        oled_128x64_cmd(0x27);

    }
    oled_128x64_cmd(0x00);
    oled_128x64_cmd(startPage);
    oled_128x64_cmd(scrollSpeed);
    oled_128x64_cmd(endPage);
    oled_128x64_cmd(0x00);
    oled_128x64_cmd(0xFF);
}

void GroveOLED12864::oled_128x64_active_scroll()
{
    oled_128x64_cmd(SeeedOLED_Activate_Scroll_Cmd);
}

void GroveOLED12864::oled_128x64_deactive_scroll()
{
    oled_128x64_cmd(SeeedOLED_Dectivate_Scroll_Cmd);
}

void GroveOLED12864::oled_128x64_normal_display()
{
    oled_128x64_cmd(SeeedOLED_Normal_Display_Cmd);
}

void GroveOLED12864::oled_128x64_inversel_display()
{
    oled_128x64_cmd(SeeedOLED_Inverse_Display_Cmd);
}

///
/// 
bool GroveOLED12864::write_clear()
{
    unsigned char i,j;
    oled_128x64_cmd(SeeedOLED_Display_Off_Cmd);     //display off
    for(j=0;j<8;j++)
    {
        oled_128x64_XY(j,0);
        {
            for(i=0;i<16;i++)  //clear all columns
            {
                oled_128x64_char(' ');
            }
        }
    }
    oled_128x64_cmd(SeeedOLED_Display_On_Cmd);     //display on
    oled_128x64_XY(0,0);
    return true;
}

bool GroveOLED12864::write_brightness(uint8_t brightness)
{
    oled_128x64_cmd(SeeedOLED_Set_Brightness_Cmd);
    oled_128x64_cmd(brightness);
    return true;
}

bool GroveOLED12864::write_integer(uint8_t row, uint8_t col, int32_t i)
{
    oled_128x64_XY(row,col);
    oled_128x64_num(i);
    return true;
}

bool GroveOLED12864::write_float(uint8_t row, uint8_t col, float floatNumber, uint8_t decimal)
{
    oled_128x64_XY(row,col);
    
    unsigned int temp=0;
    float decy=0.0;
    float rounding = 0.5;
    unsigned char f=0;
    if(floatNumber<0.0)
    {
        oled_128x64_string("-");
        floatNumber = -floatNumber;
        f +=1;
    }
    for (unsigned char i=0; i<decimal; ++i)
    {
        rounding /= 10.0;
    }
    floatNumber += rounding;

    temp = floatNumber;
    f += oled_128x64_num(temp);
    if(decimal>0)
    {
        oled_128x64_char('.');
        f +=1;
    }
    decy = floatNumber-temp;//decimal part,
    for(unsigned char i=0;i<decimal;i++)//4
    {
        decy *=10;// for the next decimal
        temp = decy;//get the decimal
        oled_128x64_num(temp);
        decy -= temp;
    }
    f +=decimal;
    return (bool)f;
}

bool GroveOLED12864::write_string(uint8_t row, uint8_t col, char *str)
{
    oled_128x64_XY(row,col);
    oled_128x64_string(str);
    return true;
}

bool GroveOLED12864::write_base64_string(uint8_t row, uint8_t col, char *b64_str)
{
    
    int len = strlen(b64_str);
    uint8_t *buf = (uint8_t *)malloc(len);
    
    if (!buf)
    {
        error_desc = "run out of memory";
        return false;
    }
    if (base64_decode(buf, &len, (const unsigned char *)b64_str, len) != 0)
    {
        error_desc = "base64_decode error";
        return false;
    }
    buf[len] = '\0';
        
    //len = strlen(buf);
    int i = 0;
    
    while (buf[i])
    {
        if (buf[i] == '\\')
        {
            if (i < len-1)
            {
                if (buf[i+1] == 'r')
                {
                    buf[i] = '\r';
                    buf[i + 1] = 255;
                    i++;
                }else if (buf[i+1] == 'n')
                {
                    buf[i] = '\n';
                    buf[i + 1] = 255;
                    i++;
                }
            }
        }
        i++;
    }
    write_string(row, col, buf);
    return true;
}

bool GroveOLED12864::write_scroll_left(uint8_t start_row, uint8_t end_row, uint8_t speed)
{
    oled_128x64_deactive_scroll();
    oled_128x64_hsp(Scroll_Left, start_row, end_row, speed);
    oled_128x64_active_scroll();
    return true;
}

bool GroveOLED12864::write_scroll_right(uint8_t start_row, uint8_t end_row, uint8_t speed)
{
    oled_128x64_deactive_scroll();
    oled_128x64_hsp(Scroll_Right, start_row, end_row, speed);
    oled_128x64_active_scroll();
    return true;
}

bool GroveOLED12864::write_stop_scroll()
{
    oled_128x64_deactive_scroll();
    return true;
}

bool GroveOLED12864::write_inverse_display(uint8_t inverse_or_not)
{
    if (inverse_or_not)
    {
        oled_128x64_inversel_display();
    } else
    {
        oled_128x64_normal_display();
    }
    return true;
}

bool GroveOLED12864::write_rotate_display(uint8_t rotate_or_not)
{
    if (rotate_or_not)
    {
        oled_128x64_cmd(0xA1);
        suli_delay_ms(5);
        oled_128x64_cmd(0xC8);
    }
    else
    {
        oled_128x64_cmd(0xA0);
        suli_delay_ms(5);
        oled_128x64_cmd(0xC0);
    }

    return true;
}