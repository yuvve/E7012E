import serial
import io
import threading

ser = serial.Serial("/dev/ttyS2", 115200, timeout = 0.1)
sio = io.TextIOWrapper(io.BufferedRWPair(ser, ser))

def ser_send():
    cmd = str(input(">>>")) + "\n"
    try:
        sio.write(cmd)
        sio.flush()
    except Exception as e:
        print(e)
    ser_send()
    
def receive_loop():
    send_thread = threading.Thread(target=ser_send)
    send_thread.start()
    
    while True:
        try:
            output = sio.readline().strip()
            if output != "":
                print(output)
        except Exception as e:
            print(e)