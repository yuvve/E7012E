import serial
import io
import threading

from config import PORT, BAUD, TIMEOUT

vals = {"current_speed": "-", "left_distance": "-", "right_distance": "-"}
val_lock = threading.Lock()

_ser = serial.Serial(PORT, BAUD, timeout=TIMEOUT)
sio  = io.TextIOWrapper(io.BufferedRWPair(_ser, _ser))

def _receive_loop():
    while True:
        raw = sio.readline().strip()
        if not raw:
            continue
        parts = raw.split(" ")
        if len(parts) < 3:
            continue
        with val_lock:
            vals["current_speed"], vals["left_distance"], vals["right_distance"] = parts[:3]

def start_receiver():
    t = threading.Thread(target=_receive_loop, daemon=True)
    t.start()

def send(cmd: str):
    sio.write(cmd + "\n")
    sio.flush()

def close():
    _ser.close()
