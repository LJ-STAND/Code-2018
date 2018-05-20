import image, sensor, time
from math import atan2, sqrt, pi, degrees
from pyb import UART

DRAW_CROSSES = True
DRAW_RECTANGLES = True

NO_GOAL_ANGLE = 400

MAX_VALID_RADIUS = 160
MIN_VALID_RADIUS = 10

YELLOW_THRESHOLD = (56, 99, -16, 19, 15, 73)
BLUE_THRESHOLD = (39, 80, -28, 0, -40, 0)

clock = time.clock()

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=500)

sensor.set_auto_whitebal(True, rgb_gain_db=(-6.02073, -6.02073, 1.717804)) #Must remain false for blob tracking
sensor.set_auto_exposure(False, exposure_us=6000)
sensor.skip_frames(time=500)

curr_gain = sensor.get_gain_db()
sensor.set_auto_gain(False, gain_db=curr_gain*1.3) #Must remain false for blob tracking

sensor.set_brightness(0)
sensor.set_contrast(3)
sensor.set_saturation(3)

sensor.skip_frames(time=500)

uart = UART(3, 9600, timeout_char=10)

centerX = int(sensor.width() / 2)
centerY = int(sensor.height() / 2) - 20

def send(data):
    sendData = [0x80]

    for num in data:
        num = round(num)
        sendData.append((num >> 7) & 0x7F)
        sendData.append(num & 0x7F)

    for num in sendData:
        uart.writechar(num)

def calculateAngleDistance(object, img):
    dx = object.cx() - centerX
    dy = object.cy() - centerY
    angle = (450
    - degrees(atan2(dy, dx))) % 360
    distance = (sqrt(dx**2 + dy**2))
    return (angle, distance)

def sortBlobs(blobs, img):
    if len(blobs) > 0:
        for blob in sorted(blobs, key=lambda x: x.pixels(), reverse = True):
            if DRAW_CROSSES:
                img.draw_cross(blob.cx(), blob.cy())

            angle, distance = calculateAngleDistance(blob, img)
            if distance > MIN_VALID_RADIUS and distance < MAX_VALID_RADIUS:
                if DRAW_RECTANGLES:
                    img.draw_rectangle(blob.rect())
                return (angle, distance)

    return (NO_GOAL_ANGLE, 0)

while True:
    img = sensor.snapshot()

    if (DRAW_CROSSES):
        img.draw_cross(centerX, centerY)

    yellowBlobs = img.find_blobs([YELLOW_THRESHOLD], x_stride=5, y_stride=5, area_threshold=1, pixel_threshold=1, merge=False)
    blueBlobs = img.find_blobs([BLUE_THRESHOLD], x_stride=5, y_stride=5, area_threshold=1, pixel_threshold=1, merge=False)

    yellowAngle, yellowDistance = sortBlobs(yellowBlobs, img)
    blueAngle, blueDistance = sortBlobs(blueBlobs, img)

    print(str(yellowAngle) + " " + str(yellowDistance))

    send([yellowAngle, yellowDistance, blueAngle, blueDistance])
