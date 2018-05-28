#ifndef CONFIG_H
#define CONFIG_H

// --- Light Sensors --- //

#define LS_NUM 36
#define LS_NUM_MULTIPLIER 10 // = 360 / LS_NUM

#define LS_CALIBRATION_COUNT 50

#define LS_CALIBRATION_BUFFER 300

#define LS_ES_DEFAULT 100

#define NO_LINE_ANGLE 400
#define NO_LINE_SIZE 3

#define LINE_CORNER_ANGLE_THRESHOLD 30
#define LINE_ANGLE_BUFFER -10
#define LINE_ANGLE_BUFFER_CORNER -10

#define LINE_SMALL_SIZE 0.1
#define LINE_BIG_SIZE 0.4

#define LINE_SPEED 40
#define OVER_LINE_SPEED 60

#define AVOID_LINE true

#define LIGHT_SENSOR_CALIBRATION_EEPROM 0

// --- Motors --- //

#define MOTOR_PID_KP 4.0
#define MOTOR_PID_KI 0.0
#define MOTOR_PID_KD 0.3

#define MAX_PWM 65535
#define MAX_SPEED 1200

#define ENCODER_UPDATE_TIME 40000

#define MOTOR_MULTIPLIER 0.707106781186547572737310929369 // = sin&cos(45)

#define MOTOR_MAX_ACCELERATION 5000 // RPM/s

// --- IR Sensors --- //

#define TSOP_TIMER_PERIOD_NUMBER 8

#define TSOP_NUM 24
#define TSOP_NUM_MULTIPLIER 15 // 360 / TSOP_NUM

#define TSOP_LOOP_COUNT 255

#define TSOP_UNLOCK_DELAY 2

#define TSOP_BEST_TSOP_NUMBER 5

#define TSOP_K1 10
#define TSOP_K2 2
#define TSOP_K3 1

#define TSOP_MIN_IGNORE 10
#define TSOP_MAX_IGNORE 220

#define TSOP_FIRST_TSOP_WEIGHT 3
#define TSOP_SECOND_TSOP_WEIGHT 2

#define TSOP_FILTER_NOISE false
#define TSOP_FILTER_SURROUNDING false

#define TSOP_NO_BALL 400

#define TSOP_HAS_BALL_STRENGTH 130

#define DEBUG_TSOP false

// --- IMU --- //

#define GOAL_HEADING_KP 0.8
#define GOAL_HEADING_KI 0.0
#define GOAL_HEADING_KD 0.01

#define HEADING_KP 1.0
#define HEADING_KI 0.0
#define HEADING_KD 0.05
#define HEADING_MAX_CORRECTION 50

#define IMU_CALIBRATION_COUNT 50
#define IMU_CALIBRATION_TIME 2

#define IMU_1_CALIBRATION_EEPROM 0
#define IMU_2_CALIBRATION_EEPROM 2
#define IMU_3_CALIBRATION_EEPROM 4

// --- Orbit --- //

#define ORBIT_SHORT_STRENGTH 155
#define ORBIT_BIG_STRENGTH 100

#define ORBIT_SMALL_ANGLE 5
#define ORBIT_BIG_ANGLE 60

#define ORBIT_BALL_FORWARD_ANGLE_TIGHTENER 0.6

#define FAR_ORBIT_SPEED 70
#define MEDIUM_ORBIT_SPEED 60
#define CLOSE_ORBIT_SPEED 50
#define FRONT_ORBIT_SPEED 40

#define BALL_FRONT_BUFFER 10

// --- Debug --- //

#define LED_BLINK_TIME_SLAVE_DEBUG 100000
#define LED_BLINK_TIME_MASTER 200000
#define LED_BLINK_TIME_SLAVE_MOTOR 400000
#define LED_BLINK_TIME_SLAVE_SENSOR 800000

#define RGB_LED_COUNT 12
#define RAINBOW_TIME 80

#define BATTERY_VOLTAGE_MULTIPILER 0.01767938931
#define BATTERY_CELLS 3
#define MIN_BATTERY_VOLTAGE 3 * BATTERY_CELLS
#define MAX_BATTERY_VOLTAGE 4.2 * BATTERY_CELLS

#define SLAVE_DEBUG_UPDATE_TIME 100000

#define DEBUG_SETTINGS_EEPROM_ADDRESS 0

// --- Screen --- //

#define TS_MINX 145
#define TS_MINY 80
#define TS_MAXX 905
#define TS_MAXY 910

#define TS_RESISTANCE 255

#define MIN_PRESSURE 100
#define MAX_PRESSURE 1300

#define	BLACK 0x0000
#define	BLUE 0x001F
#define	RED 0xF800
#define	GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

#define BACKGROUND_COLOR BLACK
#define FOREGROUND_COLOR WHITE
#define HIGHLIGHT_COLOR RED

#define SCREEN_ROTATION 3

#define TITLE_Y 2
#define TITLE_FONT_SIZE 3

#define BATTERY_METER_RIGHT_X 45
#define BATTERY_METER_Y 5
#define BATTERY_METER_WIDTH 40
#define BATTERY_METER_HEIGHT 20
#define BATTERY_METER_ROUNDED_RADIUS 4
#define BATTERY_METER_END_WIDTH 3
#define BATTERY_METER_END_HEIGHT 10
#define BATTERY_METER_END_ROUNDED_RADIUS 2
#define BATTERY_METER_INSET 4

#define LINE_Y 30

#define ENGINE_START_BUTTON_OUTER_RADIUS 68
#define ENGINE_START_BUTTON_MIDDLE_RADIUS 65
#define ENGINE_START_BUTTON_INNER_RADIUS 60

#define ENGINE_START_BUTTON_INDICATOR_Y -40
#define ENGINE_START_BUTTON_INDICATOR_WIDTH 40
#define ENGINE_START_BUTTON_INDICATOR_HEIGHT 10
#define ENGINE_START_BUTTON_INDICATOR_ROUNDED_RADIUS 3
#define ENGINE_START_BUTTON_ENGINE_TEXT_SIZE 2
#define ENGINE_START_BUTTON_START_TEXT_SIZE 3
#define ENGINE_START_BUTTON_ENGINE_TEXT_Y -6
#define ENGINE_START_BUTTON_START_TEXT_Y 15

#define HOME_BUTTON_OUTER_RADIUS 10
#define HOME_BUTTON_INNER_RADIUS 7
#define HOME_BUTTON_X 15
#define HOME_BUTTON_Y 15

#define BACK_BUTTON_WIDTH 20
#define BACK_BUTTON_HEIGHT 20
#define BACK_BUTTON_X 40
#define BACK_BUTTON_Y 5

#define MESSAGE_BOX_PADDING 50
#define MESSAGE_BOX_FONT_SIZE 2

#define SCREEN_UPDATE_TEXT_TIME 100000

#define CHECK_BOX_OUTER_SIZE 36
#define CHECK_BOX_INNER_SIZE 28

#define SWITCH_WIDTH 72
#define SWITCH_HEIGHT 36
#define SWITCH_TOGGLE_INSET 4

#define MAIN_SCREEN_BUTTON_INSET 50
#define MAIN_SCREEN_BUTTON_RADIUS 35

#define SETTINGS_SCREEN_INSET 10
#define SETTINGS_SCREEN_LABEL_Y_OFFSET 10

#define BALL_VIEW_MAX_STRENGTH 200
#define BALL_VIEW_ROBOT_RADIUS 20
#define BALL_VIEW_BALL_RADIUS 10

#define LINE_VIEW_ROBOT_RADIUS 10

#define GOAL_VIEW_ROBOT_RADIUS 10
#define GOAL_VIEW_GOAL_RADIUS 10
#define GOAL_VIEW_MAX_DISTANCE 80

#define LIGHT_SENSOR_VIEW_SENSOR_RADIUS 5

#define ROBOT_POSITION_VIEW_ROBOT_RADIUS 10

#define BLUETOOTH_ICON_X 255
#define BLUETOOTH_ICON_Y 5
#define BLUETOOTH_ICON_WIDTH 10
#define BLUETOOTH_ICON_HEIGHT 20

// --- Camera --- //

#define CAMERA_BAUD 115200

#define CAMERA_ENABLED false

#define CAMERA_START_BYTE 0x80
#define CAMERA_PACKET_SIZE 9

#define FIELD_WIDTH_CENTIMETERS 182
#define FIELD_LENGTH_CENTIMETERS 183

#define NO_GOAL_ANGLE 400

// --- Robot Position --- //

#define COORDINATE_X_PID_KP 3
#define COORDINATE_X_PID_KI 0
#define COORDINATE_X_PID_KD 0

#define COORDINATE_Y_PID_KP 3
#define COORDINATE_Y_PID_KI 0
#define COORDINATE_Y_PID_KD 0

#define NO_BALL_CENTRE_X 0
#define NO_BALL_CENTRE_Y 0

#define DEFEND_GOAL_DISTANCE 30

#define MIN_DEFEND_X -30
#define MAX_DEFEND_X 30

// --- Bluetooth --- //

#define BLUETOOTH_ENABLED false

#define BLUETOOTH_BAUD 115200
#define BLUETOOTH_START_BYTE 0x80
#define BLUETOOTH_PACKET_SIZE 9
#define BLUETOOTH_LOST_COMMUNICATION_TIME 1000000
#define BLUETOOTH_UPDATE_TIME 100000

#endif // CONFIG_H
