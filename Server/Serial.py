import serial
import libscrc

ser = serial.Serial(port = "COM4", baudrate = 115200, 
                    timeout = 10)
received = []



while True:
    #-----Receiving
    data = ser.read(14)
    ser.flush()
    for i in range(7):
        received.append(int.from_bytes(data[2*i:2*i+2], "little", signed = True))
    #------------
    #----Processing
    #---CRC
    calculated_CRC =  libscrc.ccitt_false(data[0:12])
    received_CRC = received[6]+2**16

    #-----
    if (received_CRC == calculated_CRC):
        Accel_X = received[0]/16384.0
        Accel_Y = received[1]/16384.0
        Accel_Z = received[2]/14418.0
        Giro_X = received[3]/131.0
        Giro_Y = received[4]/131.0
        Giro_Z = received[5]/131.0
        print("Received CRC: ", "Decimal", received[6]+2**16, "Hex", hex(received[6]+2**16))
        print("Caculated CRC: ", "Decimal ", calculated_CRC, "Hex ", hex(calculated_CRC))
        print("Ax = ", Accel_X, "Ay = ", Accel_Y, "Az = ", Accel_Z)
        print("Gx = ", Giro_X, "Gy = ", Giro_Y, "Gz = ", Giro_Z)
    received = []