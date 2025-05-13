import os
import time

from config import TIMEOUT


def clear():
    os.system("cls" if os.name == "nt" else "clear")


def display(vals, camera_vals, lock, camera_lock, command_history):
    print_command_history(command_history)
    print_commands(COMMANDS)

    with lock:
        print("Serial Communication Values:")
        print(f"speed: {vals['current_speed']}")
        print(f"target_speed: {vals['target_speed']}")
        print(f"forward_distance: {vals['forward_distance']}")
        print(f"left_distance: {vals['left_distance']}")
        print(f"right_distance {vals['right_distance']}\n")

    with camera_lock:
        print("Camera Values:")
        print(f"detected_shape: {camera_vals['shape']}")
        print(f"detected_color: {camera_vals['color']}")
        print(f"detected_depth: {camera_vals['depth']}\n")
    
    time.sleep(TIMEOUT)

def print_command_history(command_history):
    print("Lastest values sent:")
    for key, value in command_history.items():
        print(f"{key}: {value}")
    print("")

def print_commands(commands):
    print("Commands:")
    for key, value in commands.items():
        print(f"{key}: {value}")
    print("")

COMMANDS = {
    'G': "Go Fast",
    'g': "Go Slow",
    'S': "Stop motor & steering",
    's': "Stop motor",
    'T/t': "Set turning routine timer",
    'R/r': "Free steering",
    'P': "Set fast PID P",
    'p': "Set slow PID P",
    'I': "Set fast PID I",
    'i': "Set slow PID I",
    'D': "Set fast PID D",
    'd': "Set slow PID D",
    'M': "Set fast speed percent",
    'm': "Set slow speed percent",
    'W': "Set min distance to front wall",
    'w': "Set front distance to start turning",
    'q/Q': "Quit",
}