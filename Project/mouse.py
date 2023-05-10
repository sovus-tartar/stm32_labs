import serial
import macmouse 

ser = serial.Serial('/dev/tty.usbserial-0001', 2400, 8 , stopbits=2)
print(ser.name)


lc_prev = 0
rc_prev = 0

while 1:

    while 1:
        s = ser.read()
        #print(s)
        if(s == b'\x1f'):
            break

    val1 = int.from_bytes(ser.read(), "little", signed="True")
    if (abs(val1) <= 7):
        val1 = 0
    
    val2 = int.from_bytes(ser.read(), "little", signed="True")
    if (abs(val2) <= 7):
        val2 = 0
    lc = int.from_bytes(ser.read(), "little", signed="True")
    rc = int.from_bytes(ser.read(), "little", signed="True")

    print(val1, val2, lc, rc, sep=' ')
    #macmouse.move(val2/5, -val1/5, absolute=False, duration=0)

    
    if((lc == 1) and (lc_prev == 0)):
        #print('PUSH')            
        macmouse.hold(button = 'left')
    else:
        macmouse.move(val2/5, -val1/5, absolute=False, duration=0)

    if((lc == 0) and (lc_prev == 1)):
        #print('RELEASE')
        macmouse.release(button = 'left')
    else:
        macmouse.move(val2/5, -val1/5, absolute=False, duration=0)
    #print(lc_prev)
    lc_prev = lc

    if((rc == 1) and (rc_prev == 0)):
        #print('PUSH')            
        macmouse.hold(str('right'))

    if((rc == 0) and (rc_prev == 1)):
        #print('RELEASE')
        macmouse.release(str('right'))
    




