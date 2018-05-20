import image, sensor, time
from math import atan2, sqrt, pi, degrees
from pyb import UART

DRAW_CROSSES = True
DRAW_RECTANGLES = False

NO_GOAL_ANGLE = 400

MAX_VALID_RADIUS = 55
MIN_VALID_RADIUS = 27

CENTRE_X = 76
CENTRE_Y = 48

YELLOW_THRESHOLD = (56, 99, -16, 19, 15, 73)
BLUE_THRESHOLD = (32, 58, -21, -1, -44, -6)

clock = time.clock()

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time=500)

sensor.set_auto_whitebal(True, rgb_gain_db=(-6.02073, -6.02073, 1.717804)) #Must remain false for blob tracking
sensor.set_auto_exposure(False, exposure_us=3000)
sensor.skip_frames(time=500)

curr_gain = sensor.get_gain_db()
sensor.set_auto_gain(False, gain_db=curr_gain*1.3) #Must remain false for blob tracking

sensor.set_brightness(0)
sensor.set_contrast(3)
sensor.set_saturation(0)

sensor.skip_frames(time=500)

uart = UART(3, 9600, timeout_char=10)

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

    img.draw_circle(CENTRE_X, CENTRE_Y, MIN_VALID_RADIUS, color=(0, 0, 0), fill=True);
    img.draw_circle(CENTRE_X, CENTRE_Y, MAX_VALID_RADIUS, color=(0, 0, 0));

    if (DRAW_CROSSES):
        img.draw_cross(CENTRE_X, CENTRE_Y)

    yellowBlobs = img.find_blobs([YELLOW_THRESHOLD], x_stride=5, y_stride=5, area_threshold=1, pixel_threshold=1, merge=False, roi=(27, 2, 111, 109))
    blueBlobs = img.find_blobs([BLUE_THRESHOLD], x_stride=5, y_stride=5, area_threshold=1, pixel_threshold=1, merge=False, roi=(27, 2, 111, 109))

    yellowAngle, yellowDistance = sortBlobs(yellowBlobs, img)
    blueAngle, blueDistance = sortBlobs(blueBlobs, img)

    send([yellowAngle, yellowDistance, blueAngle, blueDistance])
