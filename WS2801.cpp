#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <iostream>
#include <string.h>
#include "WS2801.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

// Arduino library to control WS2801-based RGB LED Strips
// (c) Adafruit industries
// (c) Giles Hall
// MIT license

/*****************************************************************************/

void pabort()
{
    perror("abort");
    exit(-1);
}

// Constructor for use with hardware SPI (specific clock/data pins):
WS2801::WS2801(const char* spi_device_arg, uint16_t n) :
    spi_delay(3),
    pixels(0)
{
    spi_device = strdup(spi_device_arg);
    spi_start();
    set_length(n);
}

void WS2801::refresh(void) 
{
    // 3 bytes per LED
    spi_send(pixels, strip_length * 3);
}

void WS2801::set_pixels(uint8_t *data, bool _refresh)
{
    if (_refresh)
    {
        spi_send(data, strip_length * 3);
    } else
    }
        memcpy(pixels, data, strip_length * 3);
    }
}

void WS2801::set_length(uint16_t n)
{
    if(pixels != NULL) { free(pixels); }
    pixels = (uint8_t *)calloc(n * 3, 1);
    assert(pixels);
    strip_length = n;
    refresh();
}

uint32_t WS2801::Color(uint8_t r, uint8_t g, uint8_t b) 
{
    return ((uint32_t)r << RED_FIXED) | ((uint32_t)g << GRN_FIXED) | ((uint32_t)b << BLU_FIXED);
}

void WS2801::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) 
{
    setPixelColor(n, Color(r, g, b));
}

void WS2801::setPixelColor(uint16_t n, uint32_t c) 
{
    if(n < strip_length) 
    {
        uint8_t *p = pixels + (n * 3);
        *p++ = (c >> RED) & 0xFF;
        *p++ = (c >> GRN) & 0xFF;
        *p++ = (c >> BLU) & 0xFF;
    }
}

uint32_t WS2801::getPixelColor(uint16_t n) 
{
    if(n < strip_length) 
    {
        uint16_t ofs = n * 3;
        return ((uint32_t)((uint32_t)pixels[ofs] << RED) |
                (uint32_t)((uint32_t)pixels[ofs + 1] << GRN) |
                (uint32_t)((uint32_t)pixels[ofs + 2] << BLU));
    }
    // Pixel # is out of bounds
    return 0;
}

/**
 ** SPI
 **/

void WS2801::spi_send(uint8_t* data, uint32_t data_len) 
{
	struct spi_ioc_transfer tr;
	tr.rx_buf = (__u64)NULL;
    tr.tx_buf = (__u64)data;
	tr.len = data_len;
	tr.delay_usecs = 0;
	tr.speed_hz = spi_speed;
	tr.bits_per_word = spi_bits;
    int8_t ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret == -1) { pabort(); }
}

// Enable SPI hardware and set up protocol details:
void WS2801::spi_start(void) 
{
    int ret;
    spi_mode = 0;
    spi_bits = 8;
    spi_speed = 2000000;
    spi_delay = 0;

	spi_fd = open(spi_device, O_RDWR);
    if (spi_fd == -1) { pabort(); }

	ret = ioctl(spi_fd, SPI_IOC_WR_MODE, &spi_mode);
    if (ret == -1) { pabort(); }
	ret = ioctl(spi_fd, SPI_IOC_RD_MODE, &spi_mode);
    if (ret == -1) { pabort(); }

	ret = ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bits);
    if (ret == -1) { pabort(); }
	ret = ioctl(spi_fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bits);
    if (ret == -1) { pabort(); }

	ret = ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
    if (ret == -1) { pabort(); }
	ret = ioctl(spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
    if (ret == -1) { pabort(); }
}
