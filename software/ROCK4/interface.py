import os
import time

from config import TIMEOUT

def clear():
    os.system("cls" if os.name == "nt" else "clear")

def display(vals, camera_vals, lock, camera_lock):
    with lock:
        print("Serial Communication Values:")
        print(f"speed: {vals['speed']}")
        print(f"desired_speed: {vals['desired_speed']}")
        print(f"desired_angle: {vals['desired_angle']}")
        print(f"pid_output: {vals['pid_output']}\n")
        print(f"left_distance: {vals['left_distance']}")
        print(f"right_distance {vals['right_distance']}\n")

    with camera_lock:
        print("Camera Values:")
        print(f"detected_shape: {camera_vals['shape']}")
        print(f"detected_color: {camera_vals['color']}")
        print(f"detected_depth: {camera_vals['depth']}\n")

    print("Type a command and hit Enter (or 'exit'):")

    time.sleep(TIMEOUT)
