import threading

def start_input_thread(cmd_queue):

    def _input_loop():
        while True:
            line = input()
            cmd_queue.put(line)
            if line.lower() in ("exit", "quit", "leave"):
                break

    t = threading.Thread(target=_input_loop, daemon=True)
    t.start()
