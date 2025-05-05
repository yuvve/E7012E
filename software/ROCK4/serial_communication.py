import serial
import io
import threading

from config import PORT, BAUD, TIMEOUT

vals = {"speed": "-", "desired_speed": "-", "desired_angle": "-", "pid_output": "-", "left_distance": "-", "right_distance": "-"}
val_lock = threading.Lock()

_ser = serial.Serial(PORT, BAUD, timeout=TIMEOUT)
sio  = io.TextIOWrapper(io.BufferedRWPair(_ser, _ser))

def _receive_loop():
    while True:
        raw = sio.readline().strip()
        if not raw:
            continue
        parts = raw.split(" ")
        if len(parts) < 4:
            continue
        with val_lock:
            vals["speed"], vals["desired_speed"], vals["desired_angle"], vals["pid_output"], vals["left_distance"], vals["right_distance"] = parts[:6]

def start_receiver():
    t = threading.Thread(target=_receive_loop, daemon=True)
    t.start()

def send(cmd: str):
    sio.write(cmd + "\n")
    sio.flush()

def close():
    _ser.close()
