import queue

from interface import clear, display
from serial_communication import start_receiver, send, close, vals, val_lock
from input_handler import start_input_thread
from camera_interface import start_camera_thread, stop_camera, camera_val_lock
from camera_detection import camera_vals
import csv


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

    # Initialize video capture
    start_camera_thread()
    right = []
    left = []
    forward = []
    time = []
    data_sampling_time = 0

    try:
        while True:
            clear()
            display(vals, camera_vals, val_lock, camera_val_lock)

            right.append(vals["right_distance"])
            left.append(vals["left_distance"])
            forward.append(vals["forward_distance"])

            time.append(data_sampling_time)
            data_sampling_time = data_sampling_time + 0.1

            with camera_val_lock:
                if camera_vals["color"] == "red":
                    send("M0")

                if camera_vals["color"] == "green":
                    if camera_vals["shape"] == "circle":
                        send("M20")

            try:
                cmd = cmd_q.get_nowait()
            except queue.Empty:
                continue

            if cmd.lower() in "exit":
                break

            # print(cmd)
            send(cmd)

    except KeyboardInterrupt:
        pass

    finally:
        send("M0")
        stop_camera()
        save_data_csv(time, left, right, forward)
        close()
        print("Goodbye!")


main()
