import numpy as np
import cv2 as cv

# --- CAMERA SETUP ---
cap = cv.VideoCapture(1)
if not cap.isOpened():
    print("Cannot open camera")
    exit()

# --- CALIBRATION PARAMETERS FOR DEPTH ESTIMATION ---
# (you need to measure these for your camera + object)
focal_length = 645.0       # in pixels (fx from your camera intrinsics)
real_radius   = 0.095       # real radius of your disk in meters

while True:
    ret, frame = cap.read()
    if not ret:
        print("Can't receive frame. Exiting …")
        break

    # resize for speed / display
    frame = cv.resize(frame, (800, 600))

    hsv = cv.cvtColor(frame, cv.COLOR_BGR2HSV)

    lower_red1 = np.array([0,120,70])
    upper_red1 = np.array([10,255,255])
    lower_red2 = np.array([170,120,70])
    upper_red2 = np.array([180,255,255])
    mask_red = cv.inRange(hsv, lower_red1, upper_red1) | cv.inRange(hsv, lower_red2, upper_red2)

    lower_green = np.array([40, 40, 40])
    upper_green = np.array([70, 255, 255])
    mask_green = cv.inRange(hsv, lower_green, upper_green)

    gray    = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    blurred = cv.GaussianBlur(gray, (5,5), 0)
    edges   = cv.Canny(blurred, 50, 150)
    contours, _ = cv.findContours(edges, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)

    for cnt in contours:
        area = cv.contourArea(cnt)
        if area < 1_000:
            continue  # skip small noise

        # approximate polygon
        peri   = cv.arcLength(cnt, True)
        approx = cv.approxPolyDP(cnt, 0.04 * peri, True)

        # classify shape
        shape = "unidentified"
        if   len(approx) == 3:
            shape = "triangle"
        elif len(approx) == 4:
            # check square vs rectangle by aspect ratio
            x,y,w,h = cv.boundingRect(approx)
            ar = w / float(h)
            shape = "square" if 0.95 <= ar <= 1.05 else "rectangle"
        else:
            shape = "circle"

        # determine color by overlap with masks
        mask_shape = np.zeros(gray.shape, dtype="uint8")
        cv.drawContours(mask_shape, [cnt], -1, 255, -1)

        red_overlap   = cv.countNonZero(cv.bitwise_and(mask_shape, mask_red))
        green_overlap = cv.countNonZero(cv.bitwise_and(mask_shape, mask_green))
        color = "none"
        if red_overlap   > 0.3 * area: color = "red"
        if green_overlap > 0.3 * area: color = "green"

        # draw contour outline
        cv.drawContours(frame, [cnt], -1, (0,255,0), 2)

        if shape == "circle":
            (x_c, y_c), radius = cv.minEnclosingCircle(cnt)
            if radius > 10:
                depth_m = (real_radius * focal_length) / radius
                cv.putText(frame,
                           f"{shape},{color},{depth_m:.2f}m",
                           (int(x_c - radius), int(y_c - radius - 10)),
                           cv.FONT_HERSHEY_SIMPLEX, 0.6, (255,255,255), 2)
                # draw the fitted circle
                cv.circle(frame, (int(x_c), int(y_c)), int(radius), (255,0,0), 2)
        else:
            # label non-circles without depth
            M = cv.moments(cnt)
            if M["m00"] != 0:
                cX = int(M["m10"] / M["m00"])
                cY = int(M["m01"] / M["m00"])
                cv.putText(frame,
                           f"{shape},{color}",
                           (cX - 50, cY - 10),
                           cv.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255), 2)

    cv.imshow("frame", frame)
    if cv.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv.destroyAllWindows()
