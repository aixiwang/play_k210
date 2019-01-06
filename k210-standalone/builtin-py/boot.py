import gc
import uos
import os
import machine
import platform
#import app
import board
from fpioa_manager import *
import pwmc
board_info=board.board_info()
file_list = os.ls()
print('file_list:',file_list)

z = machine.zmodem
z.init()
z.rz()

#import face_detect
#for i in range(len(file_list)):
# if file_list[i] == '/init.py':
import init

#import machine
#camera=machine.ov2640();
#camera.init();
#lcd=machine.st7789();
#lcd.init();
#demo=machine.face_detect();
#demo.init();
#image=bytearray(320*240*2);
#while(1):
#    print('get_image')
#    #camera.get_image(image);
#    print('process_image')
#    demo.process_image(image);
#    print('draw_picture_default')
##   #lcd.draw_picture_default(image);



