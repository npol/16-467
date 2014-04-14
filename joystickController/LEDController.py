import serial
import pygame
from time import sleep
from random import random
joystick = pygame.joystick


def turnOnLED():
    pass

def changeColor(r, g, b)
    callToLED('\x80', '\x10', chr(r), chr(g), chr(b), 1)

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
    
    while True:
         pygame.event.pump()
         #Event listeners.
         if (stick.get_button(2)):
             # changeColor(0, 0, 0)
             print "Button 1"
         if (stick.get_button(3)):
             print "Button 2"
         if (stick.get_button(4)):
             print "Button 3"
        
main()


""""
Func->arduino(6-byte buffer, )
buffer = [80, ]
    
    
80 for the mega, 0x10 LED, R, G, B, byte (TBD)



Joystick (up and down -> Red, button -> Blue)
"""