import machine
camera=machine.ov2640();
camera.init();
lcd=machine.st7789();
lcd.init();
demo=machine.demo_face_detect();
demo.init();
image=bytearray(320*240*2);
while(1):
    camera.get_image(image);
    demo.process_image(image);
    lcd.draw_picture_default(image);

