import argparse
import sys
from typing import List, Tuple

import cv2 as cv
import numpy as np


# --- COLOR THRESHOLDS (HSV) ---
RED_LOWER1 = np.array([0, 120, 70], dtype=np.uint8)
RED_UPPER1 = np.array([10, 255, 255], dtype=np.uint8)
RED_LOWER2 = np.array([170, 120, 70], dtype=np.uint8)
RED_UPPER2 = np.array([180, 255, 255], dtype=np.uint8)

GREEN_LOWER = np.array([50,  80,  80], dtype=np.uint8)
GREEN_UPPER = np.array([90, 255, 255], dtype=np.uint8)

def parse_args() -> argparse.Namespace:
    """Parse command-line arguments."""
    parser = argparse.ArgumentParser(
        description="Detect colored shapes and estimate depth of circular disks."
    )
    parser.add_argument(
        "-c", "--camera", type=int, default=0,
        help="Camera index (default: 1)"
    )
    parser.add_argument(
        "-f", "--focal-length", type=float, default=645.0,
        help="Camera focal length in pixels (default: 645.0)"
    )
    parser.add_argument(
        "-r", "--real-radius", type=float, default=0.095,
        help="Real radius of the disk in meters (default: 0.095)"
    )
    parser.add_argument(
        "--width", type=int, default=800,
        help="Frame width for display (default: 800)"
    )
    parser.add_argument(
        "--height", type=int, default=600,
        help="Frame height for display (default: 600)"
    )
    return parser.parse_args()


def get_color_masks(hsv: np.ndarray) -> Tuple[np.ndarray, np.ndarray]:
    """Return binary masks for red and green regions in an HSV image."""
    red_mask = (
        cv.inRange(hsv, RED_LOWER1, RED_UPPER1) |
        cv.inRange(hsv, RED_LOWER2, RED_UPPER2)
    )
    green_mask = cv.inRange(hsv, GREEN_LOWER, GREEN_UPPER)
    return red_mask, green_mask


def find_significant_contours(
    gray: np.ndarray, min_area: float = 1_000
) -> List[np.ndarray]:
    """Detect edges, find contours, and filter out small ones."""
    blurred = cv.GaussianBlur(gray, (5, 5), 0)
    edges = cv.Canny(blurred, 50, 150)
    contours, _ = cv.findContours(edges, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
    return [c for c in contours if cv.contourArea(c) >= min_area]


def classify_shape(contour: np.ndarray) -> str:
    """
    Approximate contour to polygon and classify as 'triangle',
    'square', 'rectangle', or 'circle'.
    """
    peri = cv.arcLength(contour, True)
    approx = cv.approxPolyDP(contour, 0.04 * peri, True)
    verts = len(approx)

    if verts == 3:
        return "triangle"
    if verts == 4:
        x, y, w, h = cv.boundingRect(approx)
        ar = w / float(h)
        return "square" if 0.95 <= ar <= 1.05 else "rectangle"
    return "circle"


def detect_color(
    contour: np.ndarray,
    red_mask: np.ndarray,
    green_mask: np.ndarray
) -> str:
    """Determine whether the contour is mostly red, green, or none."""
    area = cv.contourArea(contour)
    mask_shape = np.zeros(red_mask.shape, dtype="uint8")
    cv.drawContours(mask_shape, [contour], -1, 255, -1)

    red_overlap = cv.countNonZero(cv.bitwise_and(mask_shape, red_mask))
    green_overlap = cv.countNonZero(cv.bitwise_and(mask_shape, green_mask))

    if red_overlap > 0.3 * area:
        return "red"
    if green_overlap > 0.3 * area:
        return "green"
    return "none"


def estimate_depth(real_radius: float, focal_length: float, pixel_radius: float) -> float:
    """
    Depth (meters) = (real object size * focal length) / image size.
    """
    return (real_radius * focal_length) / pixel_radius


def annotate_frame(
    frame: np.ndarray,
    contour: np.ndarray,
    shape: str,
    color: str,
    real_radius: float,
    focal_length: float
) -> None:
    """Draw contours, fitted shapes, and labels onto the frame."""
    area = cv.contourArea(contour)
    cv.drawContours(frame, [contour], -1, (0, 255, 0), 2)

    if shape == "circle":
        (x_c, y_c), radius = cv.minEnclosingCircle(contour)
        if radius > 10:
            depth = estimate_depth(real_radius, focal_length, radius)
            label = f"{shape}, {color}, {depth:.2f} m"
            cv.circle(frame, (int(x_c), int(y_c)), int(radius), (255, 0, 0), 2)
            cv.putText(
                frame, label,
                (int(x_c - radius), int(y_c - radius) - 10),
                cv.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 255), 2
            )
    else:
        M = cv.moments(contour)
        if M["m00"] != 0:
            cX = int(M["m10"] / M["m00"])
            cY = int(M["m01"] / M["m00"])
            label = f"{shape}, {color}"
            cv.putText(
                frame, label,
                (cX - 50, cY - 10),
                cv.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2
            )


def main():
    args = parse_args()

    cap = cv.VideoCapture(args.camera)
    if not cap.isOpened():
        print(f"ERROR: Cannot open camera index {args.camera}", file=sys.stderr)
        sys.exit(1)

    try:
        while True:
            ret, frame = cap.read()
            if not ret:
                print("ERROR: Frame capture failed, exiting.", file=sys.stderr)
                break

            frame = cv.resize(frame, (args.width, args.height))
            hsv = cv.cvtColor(frame, cv.COLOR_BGR2HSV)
            gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)

            red_mask, green_mask = get_color_masks(hsv)
            contours = find_significant_contours(gray)

            for cnt in contours:
                shape = classify_shape(cnt)
                color = detect_color(cnt, red_mask, green_mask)
                annotate_frame(
                    frame, cnt, shape, color,
                    real_radius=args.real_radius,
                    focal_length=args.focal_length
                )

            cv.imshow("Detection", frame)
            if cv.waitKey(1) & 0xFF == ord('q'):
                break

    finally:
        cap.release()
        cv.destroyAllWindows()


if __name__ == "__main__":
    main()
