import time
import colorsys
import WS2801

port = "/dev/spidev2.0"
num = 160
obj = WS2801.WS2801(port, num)

def fast_rainbow():
    # ps: not actually fast
    subdiv = 2
    slices = num * subdiv
    rgb = WS2801.uint8_array(num * 3)
    for xs in range(slices):
        for l in range(num):
            h = (float(l * subdiv + xs) / slices) % 1.0
            for (idx, val) in enumerate(colorsys.hsv_to_rgb(h, 1, 1)):
                val = int(val * 0xFF)
                rgb[(l * 3) + idx] = val
        obj.set_pixels(rgb)

def rainbow():
    subdiv = 2
    slices = num * subdiv
    for xs in range(slices):
        for l in range(num):
            h = (float(l * subdiv + xs) / slices) % 1.0
            rgb = [int(0xff * x) for x in colorsys.hsv_to_rgb(h, 1, 1)]
            obj.setPixelColor(l, rgb[0], rgb[1], rgb[2])
        obj.refresh()

now = time.time()
rainbow()
later = time.time()
print later - now

now = time.time()
fast_rainbow()
later = time.time()
print later - now
