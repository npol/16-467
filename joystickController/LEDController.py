import serial
import pygame
from time import sleep
from random import random
joystick = pygame.joystick

def main():
    pygame.init()
    print "%d joystick(s) found" % joystick.get_count()

    


""""
Func->arduino(6-byte buffer, )
buffer = [80, ]
    
    
80 for the mega, 0x10 LED, R, G, B, byte (TBD)



Joystick (up and down -> Red, button -> Blue)
"""