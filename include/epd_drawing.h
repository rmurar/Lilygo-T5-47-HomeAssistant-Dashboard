#ifndef __EPD_DRAWING_H__
#define __EPD_DRAWING_H__

// epd
#include "epd_driver.h"

class EpdDrawing
{
public:
    enum class alignment 
    {
        LEFT, 
        RIGHT, 
        CENTER
    };

    EpdDrawing(int width, int height);

    void Init();

    void setFont(GFXfont const & font);
    void drawString(int x, int y, String text, alignment align);
    void fillCircle(int x, int y, int r, uint8_t color);
    void drawFastHLine(int16_t x0, int16_t y0, int length, uint16_t color);
    void drawFastVLine(int16_t x0, int16_t y0, int length, uint16_t color);
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void drawCircle(int x0, int y0, int r, uint8_t color);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                    int16_t x2, int16_t y2, uint16_t color);

    void drawPixel(int x, int y, uint8_t color);
    void drawImage(int x, int y, int width, int height, const uint8_t* image_data);
    void update();
    void update_area(const Rect_t& area);

private:
    int m_screen_width;
    int m_screen_height;

    uint8_t *m_framebuffer_full;
    uint8_t *m_framebuffer_area;
    Rect_t m_rect_area;

    uint8_t *m_pBuffer;
    GFXfont  m_currentFont;


    void alloc_area_buffer(const Rect_t& area);
    void copy_to_area_buffer(const Rect_t& area);

};
#endif // __EPD_DRAWING_H__