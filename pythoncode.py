import serial  
import pyautogui 
import io  
import time
import sys
alive = True
ser = serial.Serial()
ser.baudrate = 115200  
ser.port = 'COM9'  
ser.timeout = 0.1 
ser.open()  
print('COM 9 Open: ', ser.is_open)
sio = io.TextIOWrapper(io.BufferedRWPair(ser, ser))
ser.flush()  # wait until all data is written

pyautogui.FAILSAFE = False 
width, height = pyautogui.size()  # set size of screen
width, height = int(str(width)),int(str(height)) # convert value to int

center_width = int(width/2)
center_height = int(height/2)
pyautogui.moveTo(center_width, center_height)  # set mouse to middle

speed_up_prev = 0
speed_down_prev = 0
open_flap_prev = 0
close_flap_prev = 0
land_gear_prev = 0
center_plane_prev = 0
left_brake_prev = 0
right_brake_prev = 0
off_system  =0
count = 0

print('Press Ctrl-C to quit.')
while (alive):  # kill switch not asserted
    line = sio.readline()
    data = line.split()

    inc_speed =      int(data[0])
    dec_speed =      int(data[1])
    open_flap =     int(data[2])
    close_flap =    int(data[3])
    landing_gear =     int(data[4])
    center_plane =  int(data[5])
    l_brake =    int(data[6])
    r_brake =   int(data[7])
    roll =          int(data[8])
    pitch =         int(data[9])

 
    if inc_speed != speed_up_prev:
        if inc_speed != 0:
            pyautogui.keyDown('9')
        else:
            pyautogui.keyUp('9')
        speed_up_prev = inc_speed
    
    if dec_speed != speed_down_prev:
        if dec_speed != 0:
            pyautogui.keyDown('3')
        else:
            pyautogui.keyUp('3')
        speed_down_prev = dec_speed

    if open_flap != open_flap_prev:
        pyautogui.press('f')
        open_flap_prev = open_flap

    if close_flap != close_flap_prev:
        pyautogui.keyDown('shift')
        pyautogui.press('f')
        pyautogui.keyUp('shift')
        close_flap_prev = close_flap

    if landing_gear != land_gear_prev:
        pyautogui.press('g')
        land_gear_prev = landing_gear

    if center_plane != center_plane_prev:
        pyautogui.press('c')
        center_plane_prev = center_plane

    if l_brake != left_brake_prev:
        if l_brake != 0:
            pyautogui.keyDown(',')
        else:
            pyautogui.keyUp(',')
        left_brake_prev = l_brake

    if r_brake != right_brake_prev:
        if r_brake != 0:
            pyautogui.keyDown('.')
        else:
            pyautogui.keyUp('.')
        right_brake_prev = r_brake

    # roll, pitch
    # avoid over load
    if roll > 90:
        roll = 90
    if roll < -90:
        roll = -90
    if pitch > 90:
        pitch = 90
    if pitch < -90:
        pitch = -90

    xn = pitch * 10 + 900
    yn = -1 * roll * 6 + 540
    if (count == 0):
        pyautogui.moveTo(xn, yn)
    count = count + 1
    if count == 8:
        count = 0
        # tat code
    if off_system == 1 :
        sys.exit()
