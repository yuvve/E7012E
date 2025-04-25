import cv2 as cv
import numpy as np
from typing import List, Tuple

# --- COLOR THRESHOLDS (HSV) ---
RED_LOWER1 = np.array([0, 120, 70], dtype=np.uint8)
RED_UPPER1 = np.array([10, 255, 255], dtype=np.uint8)
RED_LOWER2 = np.array([170, 120, 70], dtype=np.uint8)
RED_UPPER2 = np.array([180, 255, 255], dtype=np.uint8)
GREEN_LOWER = np.array([50,  80,  80], dtype=np.uint8)
GREEN_UPPER = np.array([90, 255, 255], dtype=np.uint8)

def get_color_masks(hsv: np.ndarray) -> Tuple[np.ndarray, np.ndarray]:
    red_mask = (
        cv.inRange(hsv, RED_LOWER1, RED_UPPER1) |
        cv.inRange(hsv, RED_LOWER2, RED_UPPER2)
    )
    green_mask = cv.inRange(hsv, GREEN_LOWER, GREEN_UPPER)
    return red_mask, green_mask

def find_significant_contours(gray: np.ndarray, min_area: float = 1_000) -> List[np.ndarray]:
    blurred = cv.GaussianBlur(gray, (5, 5), 0)
    edges = cv.Canny(blurred, 50, 150)
    cnts, _ = cv.findContours(edges, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
    return [c for c in cnts if cv.contourArea(c) >= min_area]

def classify_shape(cnt: np.ndarray) -> str:
    peri = cv.arcLength(cnt, True)
    approx = cv.approxPolyDP(cnt, 0.04 * peri, True)
    verts = len(approx)
    if verts == 3:
        return "triangle"
    if verts == 4:
        x, y, w, h = cv.boundingRect(approx)
        ar = w / float(h)
        return "square" if 0.95 <= ar <= 1.05 else "rectangle"
    return "circle"

def detect_color(cnt: np.ndarray, red_mask: np.ndarray, green_mask: np.ndarray) -> str:
    area = cv.contourArea(cnt)
    mask_shape = np.zeros(red_mask.shape, dtype="uint8")
    cv.drawContours(mask_shape, [cnt], -1, 255, -1)
    red_overlap   = cv.countNonZero(cv.bitwise_and(mask_shape, red_mask))
    green_overlap = cv.countNonZero(cv.bitwise_and(mask_shape, green_mask))
    if red_overlap   > 0.3 * area: return "red"
    if green_overlap > 0.3 * area: return "green"
    return "none"

def estimate_depth(real_radius: float, focal_length: float, pixel_radius: float) -> float:
    return (real_radius * focal_length) / pixel_radius

def annotate_frame(frame, cnt, shape, color, real_radius, focal_length) -> None:
    cv.drawContours(frame, [cnt], -1, (0,255,0), 2)
    if shape == "circle":
        (x,y), r = cv.minEnclosingCircle(cnt)
        if r > 10:
            depth = estimate_depth(real_radius, focal_length, r)
            label = f"{shape}, {color}, {depth:.2f} m"
            cv.circle(frame, (int(x),int(y)), int(r), (255,0,0), 2)
            cv.putText(frame, label,
                       (int(x-r), int(y-r)-10),
                       cv.FONT_HERSHEY_SIMPLEX, 0.6, (255,255,255), 2)
    else:
        M = cv.moments(cnt)
        if M["m00"] != 0:
            cx = int(M["m10"]/M["m00"])
            cy = int(M["m01"]/M["m00"])
            label = f"{shape}, {color}"
            cv.putText(frame, label,
                       (cx-50, cy-10),
                       cv.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255), 2)
