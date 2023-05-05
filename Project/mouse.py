import serial
import macmouse 

ser = serial.Serial('/dev/tty.usbserial-0001', 1200, 8 , stopbits=2)
print(ser.name)

while 1:
    temp = ser.read()
    print(temp)
    if(temp == b'\x1f'):
        break

lc_prev = 0
rc_prev = 0

while 1:
    val1 = int.from_bytes(ser.read(), "little", signed="True")
    val2 = int.from_bytes(ser.read(), "little", signed="True")
    lc = int.from_bytes(ser.read(), "little", signed="True")
    rc = int.from_bytes(ser.read(), "little", signed="True")

    print(val1, val2, lc, rc, sep=' ')
    macmouse.move(val2/5, -val1/5, absolute=False, duration=0.1)

    
    if((lc == 1) and (lc_prev == 0)):
        #print('PUSH')            
        macmouse.hold()

    if((lc == 0) and (lc_prev == 1)):
        #print('RELEASE')
        macmouse.release()
    #print(lc_prev)
    lc_prev = lc

    if(rc):
        macmouse.click()
    




