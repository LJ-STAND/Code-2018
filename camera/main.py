import image, sensor, time
from math import atan2, sqrt, pi, degrees, radians, sin, cos
from pyb import UART

# --- Robot 13A --- #
ROI_A = (77, 0, 180, 159) 

CENTRE_X_A = 91
CENTRE_Y_A = 66

MAX_VALID_RADIUS_A = 87

# --- Robot 13B --- #
ROI_B = (58, 0, 174, 164)

CENTRE_X_B = 88
CENTRE_Y_B = 85

MAX_VALID_RADIUS_B = 88

# --- Stuff to Change --- #

ROBOT = "A"

YELLOW_THRESHOLD = (81, 100, -20, -7, 15, 78)
BLUE_THRESHOLD = (39, 73, -35, 18, -68, 0)

DEBUG_WHITEBALANCE = False
DEBUG_PRINTING = False

# -------------- #

exec("ROI = ROI_" + ROBOT)
exec("CENTRE_X = CENTRE_X_" + ROBOT)
exec("CENTRE_Y = CENTRE_Y_" + ROBOT)
exec("MAX_VALID_RADIUS = MAX_VALID_RADIUS_" + ROBOT)

debugCount = 0
DEBUG_COUNT_MAX = 30

DRAW_CROSSES = True
DRAW_RECTANGLES = True
DRAW_CIRCLES = False

# Angle to send if a goal colour isn't found
NO_GOAL_ANGLE = 400

# Reset the sensor and set all the settings

sensor.reset()

sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing(ROI)
sensor.skip_frames(time=100)

# Print the whitebalance for calibration

if DEBUG_WHITEBALANCE:
    sensor.set_auto_whitebal(True)
else:
    sensor.set_auto_whitebal(False, rgb_gain_db=(-6.02073, -3.454361, 3.00945))

if not DEBUG_WHITEBALANCE:
    sensor.set_brightness(0)
    sensor.set_contrast(3)
    sensor.set_saturation(2)
    sensor.set_auto_exposure(False, exposure_us=10000)
    sensor.set_auto_gain(False, gain_db=15)
    sensor.skip_frames(time=500)

uart = UART(3, 115200, timeout_char=10)

def send(data):
    """ Send 'data' over UART. The first byte is 128 (10000000) which is a start of packet indicator.
    All other values are split into first and second 14 bit integer values.
    Max of 2^14 or 16,384.
    """
    
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
    """ Calculates the angle and distance to a blob. Returns a tuple (angle, distance). """
    dx = object.cx() - CENTRE_X
    dy = object.cy() - CENTRE_Y

    # atan2 is converted to degrees and then converted so that an angle of 0 is the front of the robot.
    angle = (450 - degrees(atan2(dy, dx))) % 360
    distance = (sqrt(dx**2 + dy**2))
    return (angle, distance)

def sortBlobs(blobs, img):
    """ Sorts the array of blobs returned by findBlobs().
    Ensures the blob is within the mirror circle and chooses the largest blob.
    Returns a tuple (angle, distance).
    """

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

    if DEBUG_WHITEBALANCE:
        # Print out a certain number of white balance values

        debugCount += 1
        if debugCount >= DEBUG_COUNT_MAX:
            print(sensor.get_rgb_gain_db())
            debugCount = 0

    else:
        if DRAW_CIRCLES:
            img.draw_circle(CENTRE_X, CENTRE_Y, MAX_VALID_RADIUS)

        if DRAW_CROSSES:
            img.draw_cross(CENTRE_X, CENTRE_Y)

        # Search for yellow and blue blobs.
        # Note that the merge margin is quite large, this is to account for the goal being split into two blobs when the robot is next to it.
        yellowBlobs = img.find_blobs([YELLOW_THRESHOLD], x_stride=5, y_stride=5, area_threshold=200, pixel_threshold=200, merge=True, margin=23)
        blueBlobs = img.find_blobs([BLUE_THRESHOLD], x_stride=5, y_stride=5, area_threshold=200, pixel_threshold=200, merge=True, margin=23)

        yellowAngle, yellowDistance = sortBlobs(yellowBlobs, img)
        blueAngle, blueDistance = sortBlobs(blueBlobs, img)

        if DEBUG_PRINTING:
            # Print out the two angles and distances
            print([yellowAngle, yellowDistance, blueAngle, blueDistance])

        send([yellowAngle, yellowDistance, blueAngle, blueDistance])
