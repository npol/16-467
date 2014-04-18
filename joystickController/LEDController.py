WRITE_INTERVAL = 0.005
#globals 
r = chr(0)
g = chr(0)
b = chr(0)
eyebrowLeft = 0
eyebrowRight = 0
gripper = 0
tilt = 0
motor = 0
ARDUINO = chr(0x80)
LED_SERVO     = chr(0x10)
EYEBROW_SERVO = chr(0x20)
MOTOR_SERVO   = chr(0x30)


import serial
import pygame
from time import sleep
from random import random
import math
joystick = pygame.joystick

#LED              : 0x80 0x10 R G B 0x08-0x4d            
#Eyebrows         : 0x80 0x20 [eyebrow1(right) eyebrow2(left) 0x06-0x10] gripper 0x10-0x50, 0                         (0x06 - 0x20)
#Motor (Side-Side): 0x80 0x30 non-zero signed8bit 0 0    (0x06 - 0x20)

def writeToSer(a, b, c, d, e, f, ser):
    ser.write(a); sleep(WRITE_INTERVAL)
    ser.write(b); sleep(WRITE_INTERVAL)
    ser.write(c); sleep(WRITE_INTERVAL)
    ser.write(d); sleep(WRITE_INTERVAL)
    ser.write(e); sleep(WRITE_INTERVAL)
    ser.write(f); sleep(WRITE_INTERVAL)

def changeEyeColor(newR, newG, newB, ser):
    global r; r = newR 
    global g; g= newG
    global b; b = newB
    global tilt
    writeToSer(ARDUINO, LED_SERVO, r, g, b, chr(tilt), ser)
    
    #callToLED('\x80', '\x10', chr(r), chr(g), chr(b), 1)
    
def changeHeadTilt(newTilt, ser):
    global tilt; tilt = newTilt
    global r, g, b
    writeToSer(ARDUINO, LED_SERVO, r, g, b, tilt, ser)
    
def changeEyeBrows(browLeft, browRight, ser):
    global eyebrowLeft; eyebrowLeft = browLeft
    global eyebrowRight; eyebrowRight = browRight
    global gripper
    writeToSer(ARDUINO, EYEBROW_SERVO, eyebrowLeft, eyebrowRight, gripper, 0, ser)

def changeGripper(newGripper, ser):
    global eyebrowLeft, eyebrowRight
    global gripper; gripper = newGripper
    writeToSer(ARDUINO, EYEBROW_SERVO, eyebrowLeft, eyebrowRight, gripper, 0, ser)

#dir: 0 left rotation, 1 right rotation. speed between 0-70. 
def changeMotor(dir, speed, ser):
    writeToSer(ARDUINO, MOTOR_SERVO, chr(1), chr(dir), chr(speed), chr(0), ser)


def changeServo(val, ser):
    ser.write(chr(0x80))
    sleep(WRITE_INTERVAL)
    ser.write(chr(0x10))
    sleep(WRITE_INTERVAL)
    ser.write(chr(0))
    sleep(WRITE_INTERVAL)
    ser.write(chr(0))
    sleep(WRITE_INTERVAL)
    ser.write(chr(0))
    sleep(WRITE_INTERVAL)
    ser.write(chr(val))
    sleep(WRITE_INTERVAL)
    
def changeServo2(val, ser, num):
    cmd0 = 0
    cmd1 = 0
    cmd2 = 0
    cmd3 = 0
    if(num == 0): cmd0 = val
    if(num == 1): cmd1 = val
    if(num == 2): cmd2 = val
    if(num == 3): cmd3 = val
    ser.write(chr(0x80))
    sleep(WRITE_INTERVAL)
    ser.write(chr(0x20))
    sleep(WRITE_INTERVAL)
    ser.write(chr(cmd0))
    sleep(WRITE_INTERVAL)
    ser.write(chr(cmd1))
    sleep(WRITE_INTERVAL)
    ser.write(chr(cmd2))
    sleep(WRITE_INTERVAL)
    ser.write(chr(cmd3))
    sleep(WRITE_INTERVAL)
    
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
    
    ser = serial.Serial("COM16", 9600, timeout=0)
    print "Serial connected"
    print "[Serial] Connected to Robot via %s" % ser.name
    changeEyeColor(0,0,0,ser)
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
        #ser.write(chr(0x4f))
        #sleep(0.1)
     
     
        if (stick.get_button(2)):
            changeEyeColor(int(0), 0, 0, ser)
            print "Button 3"
        if (stick.get_button(3)):
            print "Button 4"
        if (stick.get_button(4)):
            print "Button 5"
        if (stick.get_axis(0)):
            speed = int(50*stick.get_axis(0) +20)#-16 to 16 L-R joystick
            dir = speed >= 0
            changeMotor(dir, speed, ser)
            sleep(.02)
        
        

main()


""""
Func->arduino(6-byte buffer, )
buffer = [80, ]
    
    
80 for the mega, 0x10 LED, R, G, B, byte (TBD)



Joystick (up and down -> Red, button -> Blue)
"""
