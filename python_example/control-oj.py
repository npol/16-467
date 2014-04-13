# -*- coding: utf-8 -*-

import serial
import pygame
from time import sleep
joystick = pygame.joystick

# Axis 0 : left -> right
# Axis 1:  front -> back
# Axis 2:  top -> bottom (scroll wheel)
# Buttons: NUM - 1, 0 is trigger

welcome = """
______  __                        _________      _____ 
___  / / /_________  _______________  ____/_____ __  /_
__  /_/ /_  __ \  / / /_  ___/  _ \  /    _  __ `/  __/
_  __  / / /_/ / /_/ /_(__  )/  __/ /___  / /_/ // /_  
/_/ /_/  \____/\__,_/ /____/ \___/\____/  \__,_/ \__/  
                                                       
"""


def cap(num, lo, hi) :
    return min(max(num, lo), hi)

def toByte(real):
    return min(int((real + 1) * 128), 255)

def filter(real):
    if abs(real) < 0.1:
        return 0
    return real

def main():
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
    
    ser = serial.Serial("/dev/tty.SLAB_USBtoUART", 9600, timeout=0)
    # ser = serial.Serial("/dev/tty.usbserial-A1024G3E", 9600, timeout=0)
    print "[Serial] Connected to HouseCat via %s" % ser.name

    def writeBytes(L):
        for c in L:
            ser.write(chr(c))
            # print (chr(c).__repr__())

    brightness = 0
    panSpeed = 0
    tilt = 140

    # while True:
    #     for i in xrange(stick.get_numaxes()):
    #         pygame.event.pump()
    #         print "Axis %d = %f" % (i, stick.get_axis(i))
    #     sleep(0.1)
    print welcome
    print "\n\n\n\n"
    while True:
        pygame.event.pump()
        brightness = toByte(-stick.get_axis(1))
        
        panSpeed = int(filter(stick.get_axis(2))*5 + 93)

        if (stick.get_button(0)):
            tiltAxis = stick.get_axis(1)
            tilt = cap(tilt + tiltAxis*1.5, 0, 180)

        writeBytes([0, 1, brightness])
        writeBytes([0, 2, brightness])
        writeBytes([0, 3, brightness])
        writeBytes([0, 4, brightness])
        writeBytes([0, 9, panSpeed])
        writeBytes([0, 8, int(tilt)])
        
        print "\r\033[4A"
        print "Brightness:", brightness, "        "
        print "PanSpeed  :", panSpeed, "       "
        print "Tilt      :", tilt, "                    "
        
        sleep(.1)




if __name__ == '__main__':
    main()