import sys
import threading
import cv2 as cv

from config import (
    CAMERA_INDEX, DISPLAY_WIDTH, DISPLAY_HEIGHT,
    REAL_DISK_RADIUS_M, FOCAL_LENGTH_PX
)
from camera_detection import (
    get_color_masks, find_significant_contours,
    classify_shape, detect_color, annotate_frame
)

_running = False
camera_val_lock = threading.Lock()

def _camera_loop():
    cap = cv.VideoCapture(CAMERA_INDEX)
    if not cap.isOpened():
        print(f"ERROR: Cannot open camera {CAMERA_INDEX}", file=sys.stderr)
        return

    try:
        while _running:
            ret, frame = cap.read()
            if not ret:
                continue

            frame = cv.resize(frame, (DISPLAY_WIDTH, DISPLAY_HEIGHT))
            hsv  = cv.cvtColor(frame, cv.COLOR_BGR2HSV)
            gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)

            red_mask, green_mask = get_color_masks(hsv)
            contours = find_significant_contours(gray)

            for cnt in contours:
                shape = classify_shape(cnt)
                color = detect_color(cnt, red_mask, green_mask)

                with camera_val_lock:
                    annotate_frame(
                        frame, cnt, shape, color,
                        real_radius=REAL_DISK_RADIUS_M,
                        focal_length=FOCAL_LENGTH_PX
                    )

            #cv.imshow("Detection", frame)
            if cv.waitKey(1) & 0xFF == ord('q'):
                break
    finally:
        cap.release()
        cv.destroyAllWindows()

def start_camera_thread():
    """Spawn the camera + detection GUI thread."""
    global _running
    _running = True
    t = threading.Thread(target=_camera_loop, daemon=True)
    t.start()

def stop_camera():
    """Signal the camera thread to stop (it will clean up on its own)."""
    global _running
    _running = False
