#ifndef __WS2801__INCLUDE
#define __WS2801__INCLUDE

#include <stdint.h>
#define byte uint8_t

#define RED_FIXED 16 
#define GRN_FIXED 8
#define BLU_FIXED 0

#define RED 0 
#define GRN 8
#define BLU 16

class WS2801 
{

public:
    WS2801(const char* spi_device_arg, uint16_t n);
    void refresh(void);
    void set_length(uint16_t n); 
    const uint16_t get_length(void) { return strip_length; }

    void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
    void setPixelColor(uint16_t n, uint32_t c);
    uint32_t Color(uint8_t, uint8_t, uint8_t);
    uint32_t getPixelColor(uint16_t n);
    void set_pixels(uint8_t *data, bool _refresh=true);

private:
    void spi_start(void);
    void spi_send(uint8_t* data, uint32_t data_len);

    uint16_t strip_length;
    uint8_t spi_delay;
    uint8_t *pixels;

    /* SPI specific */
    int spi_fd;
    uint8_t spi_bits;
    uint8_t spi_mode;
    uint32_t spi_speed;
    const char *spi_device;
};

#endif // __WS2801__INCLUDE
