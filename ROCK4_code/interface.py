import os
import time

from config import TIMEOUT

def clear():
    os.system("cls" if os.name == "nt" else "clear")

def display(vals, lock):
    with lock:
        print(f"speed: {vals['speed']}")
        print(f"desired_speed: {vals['desired_speed']}")
        print(f"desired_angle: {vals['desired_angle']}\n")
    print("Type a command and hit Enter (or 'exit'):")

    time.sleep(TIMEOUT)
