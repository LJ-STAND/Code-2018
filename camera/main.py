import image, sensor, time
from math import atan2, sqrt, pi, degrees, radians, sin, cos
from pyb import UART

sensor.reset()

sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.VGA)
sensor.set_windowing((147, 0, 330, 312))
#sensor.set_windowing((73, 0, 166, 158))
sensor.skip_frames(time=500)

sensor.set_auto_whitebal(True, rgb_gain_db=(-6.02073, -5.623446, -0.4892338))
sensor.set_auto_exposure(False, exposure_us=5000)
sensor.set_auto_gain(False, gain_db=18)
sensor.skip_frames(time=500)

sensor.set_brightness(2)
sensor.set_contrast(3)
sensor.set_saturation(3)
sensor.skip_frames(time=500)

uart = UART(3, 9600, timeout_char=10)

DRAW_CROSSES = False
DRAW_RECTANGLES = False
DRAW_CIRCLES = False

NO_GOAL_ANGLE = 400

MAX_VALID_RADIUS = 83
MIN_VALID_RADIUS = 10

CENTRE_X = 83
CENTRE_Y = 72

YELLOW_THRESHOLD = (40, 90, -30, 23, 46, 81)
BLUE_THRESHOLD = (35, 50, -22, -7, -33, -11)

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
            if distance > MIN_VALID_RADIUS and distance < MAX_VALID_RADIUS:
                if DRAW_CROSSES:
                    img.draw_cross(blob.cx(), blob.cy())
                if DRAW_RECTANGLES:
                    img.draw_rectangle(blob.rect())
                return (angle, distance)

    return (NO_GOAL_ANGLE, 0)

while True:
    img = sensor.snapshot()

    #if DRAW_CIRCLES:
        #img.draw_circle(CENTRE_X, CENTRE_Y, MIN_VALID_RADIUS, color=(0, 0, 0), fill=True);
        #img.draw_circle(CENTRE_X, CENTRE_Y, MAX_VALID_RADIUS, color=(0, 0, 0));

    #if DRAW_CROSSES:
        #img.draw_cross(CENTRE_X, CENTRE_Y)

    #yellowBlobs = img.find_blobs([YELLOW_THRESHOLD], x_stride=5, y_stride=5, area_threshold=5, pixel_threshold=5, merge=True, margin=12)
    #blueBlobs = img.find_blobs([BLUE_THRESHOLD], x_stride=5, y_stride=5, area_threshold=5, pixel_threshold=5, merge=True, margin=12)

    #yellowAngle, yellowDistance = sortBlobs(yellowBlobs, img)
    #blueAngle, blueDistance = sortBlobs(blueBlobs, img)

    #send([yellowAngle, yellowDistance, blueAngle, blueDistance])
