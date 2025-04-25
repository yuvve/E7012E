import queue

from interface import clear, display
from serial_communication import start_receiver, send, close, vals, val_lock
from input_handler import start_input_thread

def main():

    cmd_q = queue.Queue()
    start_receiver()
    start_input_thread(cmd_q)

    try:
        while True:
            clear()
            display(vals, val_lock)

            try:
                cmd = cmd_q.get_nowait()
            except queue.Empty:
                continue

            if cmd.lower() in ("exit"):
                break
            
            #print(cmd)
            send(cmd)

    except KeyboardInterrupt:
        pass
    
    finally:
        close()
        print("Goodbye!")

main()
