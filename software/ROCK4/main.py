import csv
import queue

from camera_detection import camera_vals
from camera_interface import stop_camera, camera_val_lock, start_camera_thread
from input_handler import start_input_thread
from interface import clear, display
from serial_communication import start_receiver, send, close, vals, val_lock


def save_data_csv(time, left, right, forward):
    with open("sensor_data.csv", "w", newline="") as csvfile:
        writer = csv.writer(csvfile)

        # write header
        writer.writerow(["time", "left", "right", "forward"])

        # write each pair of values concurrently
        rows = zip(time, left, right, forward)
        writer.writerows(rows)


def main():
    # Initialize serial communication
    cmd_q = queue.Queue()
    start_receiver()
    start_input_thread(cmd_q)
    cmd = ""
    command_history = {

    }

    start_camera_thread()

    try:
        while True:
            clear()
            display(vals, camera_vals, val_lock, camera_val_lock, command_history)

            '''
            right.append(vals['right_distance'])
            left.append(vals['left_distance'])
            forward.append(vals['forward_distance'])

            time.append(data_sampling_time)
            data_sampling_time = data_sampling_time + 0.1
            '''

            with camera_val_lock:
                if camera_vals["color"] == "red" and camera_vals["shape"] == "circle":
                    send("s")

                if camera_vals["color"] == "green" and camera_vals["shape"] == "circle":
                    send("G")

            try:
                cmd = cmd_q.get_nowait()
            except queue.Empty:
                continue

            if cmd.lower() in "exit":
                break

            # print(cmd)
            send(cmd)
            command_history[cmd[0]] = cmd[1:]

    except KeyboardInterrupt:
        pass

    finally:
        send("S")
        stop_camera()
        # save_data_csv(time, left, right, forward)
        close()
        print("Goodbye!")


main()
