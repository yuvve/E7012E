import queue

from interface import clear, display
from serial_communication import start_receiver, send, close, vals, val_lock
from input_handler import start_input_thread
from camera_interface  import start_camera_thread, stop_camera, camera_val_lock
from camera_detection import camera_vals

def main():

    # Initialize serial communication
    cmd_q = queue.Queue()
    start_receiver()
    start_input_thread(cmd_q)

    # Initialize video capture
    start_camera_thread()

    try:
        while True:
            clear()
            display(vals, camera_vals, val_lock, camera_val_lock)

            try:
                cmd = cmd_q.get_nowait()
            except queue.Empty:
                continue

            if cmd.lower() in "exit":
                break
            
            #print(cmd)
            send(cmd)

    except KeyboardInterrupt:
        pass
    
    finally:
        stop_camera()
        close()
        print("Goodbye!")

main()
