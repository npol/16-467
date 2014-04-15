WRITE_INTERVAL = 0.5

import serial
import pygame
from time import sleep
from random import random
import math
joystick = pygame.joystick


def turnOnLED():
    pass

def changeColor(r, g, b, ser):
    ser.write(chr(0x80))
    sleep(WRITE_INTERVAL)
    ser.write(chr(0x10))
    sleep(WRITE_INTERVAL)
    ser.write(chr(r))
    sleep(WRITE_INTERVAL)
    ser.write(chr(g))
    sleep(WRITE_INTERVAL)
    ser.write(chr(b))
    sleep(WRITE_INTERVAL)
    ser.write(chr(0))
    sleep(WRITE_INTERVAL)
    #callToLED('\x80', '\x10', chr(r), chr(g), chr(b), 1)

def main():
    global ser
    pygame.init()
    print "%d joystick(s) found" % joystick.get_count()
    if joystick.get_count() == 0 : return
    stick = joystick.Joystick(0)
    stick.init()
    
    print "[Joystick] Connected to %s Joystick" % stick.get_name()
    print "[Joystick] %d axes" % stick.get_numaxes()
    print "[Joystick] %d balls" % stick.get_numballs()
    print "[Joystick] %d buttons" % stick.get_numbuttons()
    print "[Joystick] %d hats" % stick.get_numhats()
    
    ser = serial.Serial("COM18", 9600, timeout=0)
    print "Serial connected"
    print "[Serial] Connected to Robot via %s" % ser.name
    changeColor(int(0),int(0),int(0),ser);
    while True:
         pygame.event.pump()
         red = 127 + 127*stick.get_axis(0)
         green = 127 + 127*stick.get_axis(1)
         if(red > 255):
             red = 255
         if(green > 255):
             green = 255
         print (green)
         print (red)
         changeColor(int(red), int(green),0,ser)
         #ser.write(chr(0x4f))
         #sleep(0.1)
         #Event listeners.
         #changeColor(0,0,0,ser)
         #if (stick.get_button(2)):
         #    changeColor(0, 0, 0, ser)
         #    print "Button 3"
         #if (stick.get_button(3)):
         #    print "Button 4"
         #if (stick.get_button(4)):
         #    print "Button 5"
        
main()


""""
Func->arduino(6-byte buffer, )
buffer = [80, ]
    
    
80 for the mega, 0x10 LED, R, G, B, byte (TBD)



Joystick (up and down -> Red, button -> Blue)
"""