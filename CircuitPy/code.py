""" # Pikkuledejä varten!
import board
import digitalio
import time

led = digitalio.DigitalInOut(board.LED)
led.direction = digitalio.Direction.OUTPUT

while True:
    print("Moi!")
    led.value = True
    time.sleep(1)
    led.value = False
    time.sleep(5)"""
# Blink the neopixel LED.

# SPDX-FileCopyrightText: 2021 Kattni Rembor for Adafruit Industries
#
# SPDX-License-Identifier: MIT

#Tämä koodi on NeoPixel (isoa) lediä varten!
"""
Blink example for boards with ONLY a NeoPixel LED (e.g. without a built-in red LED).
Includes QT Py and various Trinkeys.
Requires two libraries from the Adafruit CircuitPython Library Bundle.
Download the bundle from circuitpython.org/libraries and copy the
following files to your CIRCUITPY/lib folder:
* neopixel.mpy
* adafruit_pixelbuf.mpy
Once the libraries are copied, save this file as code.py to your CIRCUITPY
drive to run it.
"""
import time
import board
import neopixel

pixels = neopixel.NeoPixel(board.NEOPIXEL, 1)

while True:
    print("Keltainen aurinko paistaa.")
    pixels.fill((255, 100, 0)) #NeoPixel ledin väri.
    time.sleep(2)   #Kauanko led on päällä sekunteina.
    pixels.fill((0, 0, 0)) #Ledi on sammuksissa.
    time.sleep(10) #Kauanko ledi on sammuksissa.
