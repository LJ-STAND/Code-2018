// Configuration values

#ifndef CONFIG_H
#define CONFIG_H

// --- Robot ID EEPROM --- //

#define ROBOT_ID_EEPROM_ADDRESS 10
#define WRITE_ROBOT_ID_EEPROM false
#define ROBOT_ID_WRITE 1

// --- Light Sensors --- //

#define LS_NUM 36
#define LS_NUM_MULTIPLIER 10 // = 360 / LS_NUM

// Number of times to read light sensors for calibration
#define LS_CALIBRATION_COUNT 50

// Amount to add onto calibration value
#define LS_CALIBRATION_BUFFER 60

// Default value for cluster end-start array
#define LS_ES_DEFAULT 100

// Angle and size values if no line is seen
#define NO_LINE_ANGLE 400
#define NO_LINE_SIZE 3

// If an angle mod 90 is within value and 90 - value, then it is considered a corner
#define LINE_CORNER_ANGLE_THRESHOLD 30

// If an angle is within value of the line angle, it is outside the line. A value for corner and not corner
#define LINE_ANGLE_BUFFER -10
#define LINE_ANGLE_BUFFER_CORNER -10

// < small size, robot will ignore line. < big size, > small size, robot will sit on the line.
// > big size and the robot will go back in
#define LINE_SMALL_SIZE 0.2
#define LINE_BIG_SIZE 0.5

// Speeds for slow and fast line avoidance (depending on line size from big size to 2)
#define LINE_SPEED 20
#define OVER_LINE_SPEED 60

// Enable / disable line avoidance
#define AVOID_LINE true

// EEPROM address of light sensor calibration values
#define LIGHT_SENSOR_CALIBRATION_EEPROM 0

// --- Motors --- //

// Encoder PID values
#define MOTOR_PID_KP 4.0
#define MOTOR_PID_KI 0.0
#define MOTOR_PID_KD 0.3

#define MAX_PWM 65535
#define MAX_SPEED 1200 // RPM

// Max time between encoder state changes for an rpm to not be considered 0
#define ENCODER_UPDATE_TIME 40000

#define MOTOR_MULTIPLIER 0.707106781186547572737310929369 // = sin&cos(45)

// --- IR Sensors --- //

// Number of 833us periods in a sensor read
#define TSOP_TIMER_PERIOD_NUMBER 4

#define TSOP_NUM 24
#define TSOP_NUM_MULTIPLIER 15 // 360 / TSOP_NUM

// Number of strongest sensors used to calculate angle and strength
#define TSOP_BEST_TSOP_NUMBER 5

// Angle if there is no ball
#define TSOP_NO_BALL 400

#define DEBUG_TSOP false

// --- IMU --- //

// Straight correction PID
#define HEADING_KP 1.0
#define HEADING_KI 0.0
#define HEADING_KD 0.05
#define HEADING_MAX_CORRECTION 40

// Straight correction PID if the goal is being tracked
#define GOAL_HEADING_ATTACK_KP 0.6
#define GOAL_HEADING_ATTACK_KI 0
#define GOAL_HEADING_ATTACK_KD 0.01
#define GOAL_HEADING_ATTACK_MAX_CORRECTION 30

// Number of reads when calibrating an IMU
#define IMU_CALIBRATION_COUNT 50

// Delay between calibration reads
#define IMU_CALIBRATION_TIME 2

// EEPROM addresses for calibration values of each IMU
#define IMU_1_CALIBRATION_EEPROM 0
#define IMU_2_CALIBRATION_EEPROM 2
#define IMU_3_CALIBRATION_EEPROM 4

// --- Orbit --- //

// Strength values for the ball being close and far
#define BALL_CLOSE_STRENGTH 220
#define BALL_FAR_STRENGTH 0

// Orbit speeds. When going straight to the ball, the speed is fast and going perpendicular to the ball the speed is slow
#define ORBIT_SPEED_FAST 60
#define ORBIT_SPEED_SLOW 42

// --- Debug --- //

// LED blink times for each Teensy. Different to give an indication that the right program is on each Teensy
#define LED_BLINK_TIME_SLAVE_DEBUG 100000
#define LED_BLINK_TIME_MASTER 200000
#define LED_BLINK_TIME_SLAVE_MOTOR 400000
#define LED_BLINK_TIME_SLAVE_SENSOR 800000

#define RGB_LED_COUNT 12
#define RAINBOW_TIME 80

// Number of cells in the battery, high and low voltages for the battery meter
#define BATTERY_CELLS 2
#define MIN_BATTERY_VOLTAGE 3.2 * BATTERY_CELLS
#define MAX_BATTERY_VOLTAGE 4.2 * BATTERY_CELLS

// Time between communication with the debug slave
#define SLAVE_DEBUG_UPDATE_TIME 100000

// EEPROM address of the debug settings
#define DEBUG_SETTINGS_EEPROM_ADDRESS 0

// --- Screen --- //

// Min and max X and Y values to map touch screen values to X and Y values
#define TS_MINX 145
#define TS_MINY 80
#define TS_MAXX 905
#define TS_MAXY 910

// Resistance of the screen with no pressure
#define TS_RESISTANCE 255

// Min and max pressure values to be considered a touch
#define MIN_PRESSURE 100
#define MAX_PRESSURE 1300

// Common colours as hex values
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

// Y value and font size of "LJ STAND" title text
#define TITLE_Y 2
#define TITLE_FONT_SIZE 3

// Battery meter

#define BATTERY_METER_RIGHT_X 45
#define BATTERY_METER_Y 5
#define BATTERY_METER_WIDTH 40
#define BATTERY_METER_HEIGHT 20
#define BATTERY_METER_ROUNDED_RADIUS 4
#define BATTERY_METER_END_WIDTH 3
#define BATTERY_METER_END_HEIGHT 10
#define BATTERY_METER_END_ROUNDED_RADIUS 2
#define BATTERY_METER_INSET 4

// Horizontal line

#define LINE_Y 30

// Engine start button

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

// Home button

#define HOME_BUTTON_OUTER_RADIUS 10
#define HOME_BUTTON_INNER_RADIUS 7
#define HOME_BUTTON_X 15
#define HOME_BUTTON_Y 15

// Back button

#define BACK_BUTTON_WIDTH 20
#define BACK_BUTTON_HEIGHT 20
#define BACK_BUTTON_X 40
#define BACK_BUTTON_Y 5

// Message popups

#define MESSAGE_BOX_PADDING 50
#define MESSAGE_BOX_FONT_SIZE 2

// Check box

#define CHECK_BOX_OUTER_SIZE 36
#define CHECK_BOX_INNER_SIZE 28

// Switch

#define SWITCH_WIDTH 72
#define SWITCH_HEIGHT 36
#define SWITCH_TOGGLE_INSET 4

// Round text buttons on main screen

#define MAIN_SCREEN_BUTTON_INSET 50
#define MAIN_SCREEN_BUTTON_RADIUS 35

// Paddings for settings screen

#define SETTINGS_SCREEN_INSET 10
#define SETTINGS_SCREEN_LABEL_Y_OFFSET 10

// Ball view

#define BALL_VIEW_MAX_STRENGTH 300
#define BALL_VIEW_ROBOT_RADIUS 20
#define BALL_VIEW_BALL_RADIUS 10

// Line view

#define LINE_VIEW_ROBOT_RADIUS 10
#define LIGHT_SENSOR_VIEW_SENSOR_RADIUS 5

// Camera view

#define GOAL_VIEW_ROBOT_RADIUS 10
#define GOAL_VIEW_GOAL_RADIUS 10
#define GOAL_VIEW_MAX_DISTANCE 80

#define ROBOT_POSITION_VIEW_ROBOT_RADIUS 10

// Bluetooth icon

#define BLUETOOTH_ICON_X 255
#define BLUETOOTH_ICON_Y 5
#define BLUETOOTH_ICON_WIDTH 10
#define BLUETOOTH_ICON_HEIGHT 20

// --- Camera --- //

// UART Baudrate
#define CAMERA_BAUD 115200

#define CAMERA_ENABLED true

// Camera communication settings
#define CAMERA_START_BYTE 0x80
#define CAMERA_PACKET_SIZE 9

// Field dimensions
#define FIELD_WIDTH_CENTIMETERS 182
#define FIELD_LENGTH_CENTIMETERS 243
#define GOAL_EDGE_OFFSET_CENTIMETERS 30
#define DEFEND_BOX_WIDTH_CENTIMETERS 30

#define NO_GOAL_ANGLE 400

// --- Robot Position --- //

// Position to move if there's no ball
#define NO_BALL_CENTRE_X 0
#define NO_BALL_CENTRE_Y 0

// Position to move if other robot sees the ball out of the line
#define BALL_OUT_CENTRE_X 0
#define BALL_OUT_CENTRE_Y -40

// Distance to sit from goal when defending
#define DEFEND_GOAL_DISTANCE 35

// If the robot is within a distance of the goal, don't orbit behind the ball, instead move to the side
#define DEFEND_GOAL_DISTANCE_ORBIT 25

// When moving to the side, go close to the goal
#define DEFEND_GOAL_DISTANCE_CLOSE 15

// Max horizontal distance either side of the centre when defending
#define MAX_DEFEND_X 35

// PID for moving sideways when defending
#define DEFEND_BALL_ANGLE_KP -0.6
#define DEFEND_BALL_ANGLE_KI 0
#define DEFEND_BALL_ANGLE_KD 0.008
#define MAX_DEFEND_MOVEMENT_X 45

// If the ball strength is large enough and within a certain angle of the front, move forward with a certain speed until a max y value
#define DEFEND_CHARGE_STRENGTH 200
#define DEFEND_CHARGE_ANGLE 20
#define DEFEND_CHARGE_SPEED 60
#define DEFEND_CHARGE_MAX_Y -20

// PID for moving to a coordinate
#define MOVE_TO_COORDINATE_KP 1.6
#define MOVE_TO_COORDINATE_KI 0
#define MOVE_TO_COORDINATE_KD 0.0
#define MOVE_TO_COORDINATE_MAX_SPEED 65

// If the robot is within a certain distance of a coordinate, it is considered at the coordinate
#define AT_COORDINATE_THRESHOLD_DISTANCE 2

// Move to the nearest side when switching to attack to allow the defender to go through
#define ATTACK_SWITCH_SIDEWAYS_X 40

#define AVOID_DOUBLE_DEFENCE false

// Minimum y value when avoiding double defence
#define DOUBLE_DEFENCE_MIN_Y -30

// --- Play Mode --- //

// Ball must be within the front of the defender
#define PLAYMODE_SWITCH_DEFENDER_ANGLE 15

// And in front of the attacker or far away from it
#define PLAYMODE_SWITCH_ATTACKER_ANGLE 90
#define PLAYMODE_SWITCH_ATTACKER_STRENGTH_FAR 120

// And close to the defender
#define PLAYMODE_SWITCH_DEFENDER_STRENGTH 205

// And far from the attacker
#define PLAYMODE_SWITCH_ATTACKER_STRENGTH 150

// --- Bluetooth --- //

// UART baudrate
#define BLUETOOTH_BAUD 115200

// Packet start value
#define BLUETOOTH_START_BYTE 0x80

// Packet size
#define BLUETOOTH_PACKET_SIZE 14

// If the last communication was too long ago, it is disconnected
#define BLUETOOTH_LOST_COMMUNICATION_TIME 1000000

// Time between communications
#define BLUETOOTH_UPDATE_TIME 100000

#endif // CONFIG_H
