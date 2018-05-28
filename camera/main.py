import image, sensor, time
from math import atan2, sqrt, pi, degrees, radians, sin, cos
from pyb import UART

sensor.reset()

ROI1 = (58, 0, 174, 164)
ROI2 = (74, 0, 176, 162)

roi = ROI1

CENTRE_X_1 = 86
CENTRE_X_2 = 86

CENTRE_Y_1 = 75
CENTRE_Y_2 = 65

CENTRE_X = CENTRE_X_1
CENTRE_Y = CENTRE_Y_1

MAX_VALID_RADIUS_1 = 88
MAX_VALID_RADIUS_2 = 87

MAX_VALID_RADIUS = MAX_VALID_RADIUS_1

sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing(roi)
sensor.skip_frames(time=100)

sensor.set_auto_whitebal(False, rgb_gain_db=(-6.02073, -5.623446, -1.160657))
sensor.set_auto_exposure(False, exposure_us=5000)
sensor.set_auto_gain(False, gain_db=18.5)
sensor.skip_frames(time=500)

sensor.set_brightness(0)
sensor.set_contrast(3)
sensor.set_saturation(3)
sensor.skip_frames(time=500)

uart = UART(3, 115200, timeout_char=10)

DRAW_CROSSES = False
DRAW_RECTANGLES = False
DRAW_CIRCLES = False

NO_GOAL_ANGLE = 400

YELLOW_THRESHOLD = (50, 97, -28, 9, 51, 95)
BLUE_THRESHOLD = (49, 71, -24, -6, -31, -3)

def send(data):
    sendData = [0x80]

    for num in data:
        num = round(num)
        sendData.append((num >> 7) & 0x7F)
        sendData.append(num & 0x7F)

    for num in sendData:
        try:
            uart.writechar(num)
        except:
            pass

def calculateAngleDistance(object, img):
    dx = object.cx() - CENTRE_X
    dy = object.cy() - CENTRE_Y
    angle = (450
    - degrees(atan2(dy, dx))) % 360
    distance = (sqrt(dx**2 + dy**2))
    return (angle, distance)

def sortBlobs(blobs, img):
    if len(blobs) > 0:
        for blob in sorted(blobs, key=lambda x: x.pixels(), reverse = True):
            angle, distance = calculateAngleDistance(blob, img)
            if distance < MAX_VALID_RADIUS:
                if DRAW_CROSSES:
                    img.draw_cross(blob.cx(), blob.cy())
                if DRAW_RECTANGLES:
                    img.draw_rectangle(blob.rect())
                return (angle, distance)

    return (NO_GOAL_ANGLE, 0)

while True:
    img = sensor.snapshot()

    if DRAW_CIRCLES:
        img.draw_circle(CENTRE_X, CENTRE_Y, MAX_VALID_RADIUS)

    if DRAW_CROSSES:
        img.draw_cross(CENTRE_X, CENTRE_Y)

    yellowBlobs = img.find_blobs([YELLOW_THRESHOLD], x_stride=5, y_stride=5, area_threshold=5, pixel_threshold=5, merge=True, margin=12)
    blueBlobs = img.find_blobs([BLUE_THRESHOLD], x_stride=5, y_stride=5, area_threshold=5, pixel_threshold=5, merge=True, margin=12)

    yellowAngle, yellowDistance = sortBlobs(yellowBlobs, img)
    blueAngle, blueDistance = sortBlobs(blueBlobs, img)

    send([yellowAngle, yellowDistance, blueAngle, blueDistance])
