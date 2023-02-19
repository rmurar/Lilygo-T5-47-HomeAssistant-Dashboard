#include <Arduino.h>

#include "epd_drawing.h"

EpdDrawing::EpdDrawing(int width, int height)
: m_screen_width(width), m_screen_height(height),
  m_framebuffer_full(nullptr), m_framebuffer_area(nullptr),
  m_pBuffer(nullptr)
{

}

void EpdDrawing::Init()
{
    m_framebuffer_full = (uint8_t *)ps_calloc(sizeof(uint8_t), m_screen_width * m_screen_height / 2);
    if(m_framebuffer_full == nullptr)
    {
        Serial.println("Memory alloc failed!");
    }

    memset(m_framebuffer_full, 0xFF, m_screen_width * m_screen_height / 2);
    
    m_pBuffer = m_framebuffer_full;
}

void EpdDrawing::setFont(GFXfont const & font) 
{
    m_currentFont = font;
}

void EpdDrawing::drawString(int x, int y, String text, alignment align) 
{
    char * data  = const_cast<char*>(text.c_str());
    int  x1, y1; //the bounds of x,y and w and h of the variable 'text' in pixels.
    int w, h;
    int xx = x, yy = y;
    
    get_text_bounds(&m_currentFont, data, &xx, &yy, &x1, &y1, &w, &h, NULL);
    
    if(align == EpdDrawing::alignment::RIGHT)
    {
        x = x - w;
    }

    if(align == EpdDrawing::alignment::CENTER)
    {
        x = x - w / 2;
    }

    write_string(&m_currentFont, data, &x, &y, m_pBuffer);
}

void EpdDrawing::fillCircle(int x, int y, int r, uint8_t color) 
{
    epd_fill_circle(x, y, r, color, m_pBuffer);
}

void EpdDrawing::drawFastHLine(int16_t x0, int16_t y0, int length, uint16_t color) 
{
    epd_draw_hline(x0, y0, length, color, m_pBuffer);
}

void EpdDrawing::drawFastVLine(int16_t x0, int16_t y0, int length, uint16_t color) 
{
    epd_draw_vline(x0, y0, length, color, m_pBuffer);
}

void EpdDrawing::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) 
{
    epd_write_line(x0, y0, x1, y1, color, m_pBuffer);
}

void EpdDrawing::drawCircle(int x0, int y0, int r, uint8_t color) 
{
    epd_draw_circle(x0, y0, r, color, m_pBuffer);
}

void EpdDrawing::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) 
{
    epd_draw_rect(x, y, w, h, color, m_pBuffer);
}

void EpdDrawing::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) 
{
    epd_fill_rect(x, y, w, h, color, m_pBuffer);
}

void EpdDrawing::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                  int16_t x2, int16_t y2, uint16_t color) 
{
    epd_fill_triangle(x0, y0, x1, y1, x2, y2, color, m_pBuffer);
}

void EpdDrawing::drawPixel(int x, int y, uint8_t color) 
{
    epd_draw_pixel(x, y, color, m_pBuffer);
}

void EpdDrawing::drawImage(int x, int y, int width, int height, const uint8_t* image_data) 
{
    Rect_t image_area = {
      .x = x,
      .y = y,
      .width = width,
      .height = height,
    };
    epd_copy_to_framebuffer(image_area, (uint8_t *)image_data, m_pBuffer);  
}

void EpdDrawing::update() 
{
    epd_draw_grayscale_image(epd_full_screen(), m_pBuffer); // Update the screen
}

void EpdDrawing::update_area(const Rect_t& area)
{
    alloc_area_buffer(area);
    
    copy_to_area_buffer(area);

    epd_draw_grayscale_image(area, m_framebuffer_area); // Update the area
}

void EpdDrawing::alloc_area_buffer(const Rect_t& area)
{
    if(m_framebuffer_area != nullptr)
    {
        if((m_rect_area.width < area.width) || (m_rect_area.height < area.height))
        {
            free(m_framebuffer_area);
            m_framebuffer_area = nullptr;
        }
    }

    if(m_framebuffer_area == nullptr)
    {
        m_framebuffer_area = (uint8_t *)ps_calloc(sizeof(uint8_t), area.width * area.height / 2);
        if(m_framebuffer_area == nullptr)
        {
            Serial.println("FramebufferArea: Memory alloc failed!");
        }

        memset(m_framebuffer_area, 0xFF, area.width * area.height / 2);

        m_rect_area = area;
    }
}

void EpdDrawing::copy_to_area_buffer(const Rect_t& area)
{
    //copy from full-screen buffer to area buffer
    for (uint32_t i = 0; i < area.width * area.height; i += 2)
    {
        int32_t xx = area.x + i % area.width;
        if (xx < 0 || xx >= m_screen_width)
        {
            continue;
        }
        int32_t yy = area.y + i / area.width;
        if (yy < 0 || yy >= m_screen_height)
        {
            continue;
        }
        uint8_t *buf_ptr = &m_framebuffer_full[yy * m_screen_width / 2 + xx / 2];

        m_framebuffer_area[i/2] = *buf_ptr;
    }

}

