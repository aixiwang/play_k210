import machine
fpioa=machine.fpioa()
fpioa.set_function(47,137)#FUNC_CMOS_PCLK
fpioa.set_function(46,132)#FUNC_CMOS_XCLK
fpioa.set_function(45,136)#FUNC_CMOS_HREF                             
fpioa.set_function(44,134)#FUNC_CMOS_PWDN                             
fpioa.set_function(43,135)#FUNC_CMOS_VSYN             
fpioa.set_function(42,133)#FUNC_CMOS_RST               
fpioa.set_function(41,146)#FUNC_SCCB_SCLK
fpioa.set_function(40,147)#FUNC_SCCB_SDA
fpioa.set_function(37, 25)#FUNC_GPIOHS1 init_rst
fpioa.set_function(38, 26)#FUNC_GPIOHS2 tft_hard_init
fpioa.set_function(36, 15)#FUNC_SPI0_SS3 tft_hard_init
fpioa.set_function(39, 17)#FUNC_SPI0_SCLK tft_hard_init
fpioa.set_function(12, 190)#FUNC_TIMER0_TOGGLE1
fpioa.set_function(13, 191)#FUNC_TIMER0_TOGGLE2
fpioa.set_function(14, 192)#FUNC_TIMER0_TOGGLE3
