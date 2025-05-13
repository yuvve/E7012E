import os
import time

from config import TIMEOUT


def clear():
    os.system("cls" if os.name == "nt" else "clear")


def display(vals, camera_vals, lock, camera_lock, cmd, fast, slow):
    with lock:
        print("Serial Communication Values:")
        print(f"speed: {vals['current_speed']}")
        print(f"forward_distance: {vals['forward_distance']}")
        print(f"left_distance: {vals['left_distance']}")
        print(f"right_distance {vals['right_distance']}\n")

    with camera_lock:
        print("Camera Values:")
        print(f"detected_shape: {camera_vals['shape']}")
        print(f"detected_color: {camera_vals['color']}")
        print(f"detected_depth: {camera_vals['depth']}\n")

    print("PID Parameters:")
    print(f"{fast['P']}, {fast['I']}, {fast['D']}")
    print(f"{slow['p']}, {slow['i']}, {slow['d']}")

    print("Type a command and hit Enter (or 'exit'):")
    print(cmd)

    time.sleep(TIMEOUT)
