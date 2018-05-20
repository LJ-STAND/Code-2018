#include "ScreenUI.h"

View::View(uint16_t x, uint16_t y, uint16_t w, uint16_t h) : x(x), y(y), w(w), h(h) {}

Button::Button(uint16_t x, uint16_t y, uint16_t w, uint16_t h) : View(x, y, w, h) {}

void View::checkDraw() {
    if (needsDrawing) {
        draw();
        needsDrawing = false;
    }
}

void View::setNeedsDraw() {
    needsDrawing = true;
}

Dial::Dial(uint16_t cx, uint16_t cy, uint16_t r, int maxValue) : View(cx - r, cy - r, r * 2, r * 2), cx(cx), cy(cy), r(r), maxValue(maxValue) {}

void Dial::draw() {
    TFT.drawCircle(cx, cy, r, FOREGROUND_COLOR);

    double oldAngleRadians = ((double)min(oldValue, maxValue) / (double)maxValue) * 2*PI - 0.5*PI;
    double angleRadians = ((double)min(value, maxValue) / (double)maxValue) * 2*PI - 0.5*PI;
    TFT.drawLine(cx, cy, cx + r * 0.8 * cos(oldAngleRadians), cy + r * 0.8 * sin(oldAngleRadians), BACKGROUND_COLOR);
    TFT.fillCircle(cx, cy, 10, FOREGROUND_COLOR);
    TFT.drawLine(cx, cy, cx + r * 0.8 * cos(angleRadians), cy + r * 0.8 * sin(angleRadians), HIGHLIGHT_COLOR);

    oldValue = value;
}

void Dial::setValue(int newValue) {
    if (value != newValue) {
        value = newValue;
        setNeedsDraw();
    }
}

int Dial::getValue() {
    return value;
}

BallView::BallView(uint16_t x, uint16_t y, uint16_t w, uint16_t h) : View(x, y, w, h) {}

void BallView::draw() {
    TFT.drawCircle(x + w / 2, y + h / 2, 20, FOREGROUND_COLOR);

    if (oldData.visible()) {
        TFT.fillCircle(x + w / 2 + ((BALL_VIEW_MAX_STRENGTH - (double)oldData.strength) / (double)BALL_VIEW_MAX_STRENGTH) * (double)min((w - BALL_VIEW_BALL_RADIUS) / 2, (h - BALL_VIEW_BALL_RADIUS) / 2) * cos(degreesToRadians(oldData.angle - 90)), y + h / 2 + ((BALL_VIEW_MAX_STRENGTH - (double)oldData.strength) / (double)BALL_VIEW_MAX_STRENGTH) * (double)min((w - BALL_VIEW_BALL_RADIUS) / 2, (h - BALL_VIEW_BALL_RADIUS) / 2) * sin(degreesToRadians(oldData.angle - 90)), BALL_VIEW_BALL_RADIUS, BACKGROUND_COLOR);
    }

    if (ballData.visible()) {
        TFT.fillCircle(x + w / 2 + ((BALL_VIEW_MAX_STRENGTH - (double)ballData.strength) / (double)BALL_VIEW_MAX_STRENGTH) * (double)min((w - BALL_VIEW_BALL_RADIUS) / 2, (h - BALL_VIEW_BALL_RADIUS) / 2) * cos(degreesToRadians(ballData.angle - 90)), y + h / 2 + ((BALL_VIEW_MAX_STRENGTH - (double)ballData.strength) / (double)BALL_VIEW_MAX_STRENGTH) * (double)min((w - BALL_VIEW_BALL_RADIUS) / 2, (h - BALL_VIEW_BALL_RADIUS) / 2) * sin(degreesToRadians(ballData.angle - 90)), BALL_VIEW_BALL_RADIUS, FOREGROUND_COLOR);
    }

    oldData = ballData;
}

void BallView::setBallData(BallData data) {
    if (ballData.angle != data.angle) {
        ballData = data;
        setNeedsDraw();
    }
}

LineView::LineView(uint16_t x, uint16_t y, uint16_t w, uint16_t h) : View(x, y, w, h) {}

void LineView::draw() {
    TFT.fillCircle(oldData.onField ? x + w / 2 : x + w / 2 + (min(w, h) / 4 - LINE_VIEW_ROBOT_RADIUS + (oldData.size / 3.0) * 2.0 * (double)LINE_VIEW_ROBOT_RADIUS) * cos(degreesToRadians(oldData.angle - 90)), oldData.onField ? y + h / 2 : y + h / 2 + (min(w, h) / 4 - LINE_VIEW_ROBOT_RADIUS + (oldData.size / 3.0) * 2.0 * (double)LINE_VIEW_ROBOT_RADIUS) * sin(degreesToRadians(oldData.angle - 90)), LINE_VIEW_ROBOT_RADIUS, BACKGROUND_COLOR);
    TFT.fillCircle(lineData.onField ? x + w / 2 : x + w / 2 + (min(w, h) / 4 - LINE_VIEW_ROBOT_RADIUS + (lineData.size / 3.0) * 2.0 * (double)LINE_VIEW_ROBOT_RADIUS) * cos(degreesToRadians(lineData.angle - 90)), lineData.onField ? y + h / 2 : y + h / 2 + (min(w, h) / 4 - LINE_VIEW_ROBOT_RADIUS + (lineData.size / 3.0) * 2.0 * (double)LINE_VIEW_ROBOT_RADIUS) * sin(degreesToRadians(lineData.angle - 90)), LINE_VIEW_ROBOT_RADIUS, FOREGROUND_COLOR);

    TFT.drawCircle(x + w / 2, y + h / 2, min(w, h) / 4, FOREGROUND_COLOR);

    oldData = lineData;
}

void LineView::setLineData(LineData data) {
    if (lineData != data) {
        lineData = data;
        setNeedsDraw();
    }
}

void Button::setEnabled(bool isEnabled) {
    if (enabled != isEnabled) {
        enabled = isEnabled;
        setNeedsDraw(true);
    }
}

GoalView::GoalView(uint16_t x, uint16_t y, uint16_t w, uint16_t h) : View(x, y, w, h) {}

void GoalView::draw() {
    int16_t centreX = x + w / 2;
    int16_t centreY = y + h / 2;

    if (oldYellowAngle != 400) {
        int16_t oldYellowX = centreX + (min(w, h) / 2 - GOAL_VIEW_GOAL_RADIUS) * ((double)oldYellowDistance / (double)GOAL_VIEW_MAX_DISTANCE) * cos(degreesToRadians(oldYellowAngle - 90));
        int16_t oldYellowY = centreY + (min(w, h) / 2 - GOAL_VIEW_GOAL_RADIUS) * ((double)oldYellowDistance / (double)GOAL_VIEW_MAX_DISTANCE) * sin(degreesToRadians(oldYellowAngle - 90));
        TFT.fillCircle(oldYellowX, oldYellowY, GOAL_VIEW_GOAL_RADIUS, BACKGROUND_COLOR);
    }

    if (yellowAngle != 400) {
        int16_t yellowX = centreX + (min(w, h) / 2 - GOAL_VIEW_GOAL_RADIUS) * ((double)yellowDistance / (double)GOAL_VIEW_MAX_DISTANCE) * cos(degreesToRadians(yellowAngle - 90));
        int16_t yellowY = centreY + (min(w, h) / 2 - GOAL_VIEW_GOAL_RADIUS) * ((double)yellowDistance / (double)GOAL_VIEW_MAX_DISTANCE) * sin(degreesToRadians(yellowAngle - 90));  
        TFT.fillCircle(yellowX, yellowY, GOAL_VIEW_GOAL_RADIUS, YELLOW);
    }

    if (oldBlueAngle != 400) {
        int16_t oldBlueX = centreX + (min(w, h) / 2 - GOAL_VIEW_GOAL_RADIUS) * ((double)oldBlueDistance / (double)GOAL_VIEW_MAX_DISTANCE) * cos(degreesToRadians(oldBlueAngle - 90));
        int16_t oldBlueY = centreY + (min(w, h) / 2 - GOAL_VIEW_GOAL_RADIUS) * ((double)oldBlueDistance / (double)GOAL_VIEW_MAX_DISTANCE) * sin(degreesToRadians(oldBlueAngle - 90));
        TFT.fillCircle(oldBlueX, oldBlueY, GOAL_VIEW_GOAL_RADIUS, BACKGROUND_COLOR);
    }

    if (blueAngle != 400) {
        int16_t blueX = centreX + (min(w, h) / 2 - GOAL_VIEW_GOAL_RADIUS) * ((double)blueDistance / (double)GOAL_VIEW_MAX_DISTANCE) * cos(degreesToRadians(blueAngle - 90));
        int16_t blueY = centreY + (min(w, h) / 2 - GOAL_VIEW_GOAL_RADIUS) * ((double)blueDistance / (double)GOAL_VIEW_MAX_DISTANCE) * sin(degreesToRadians(blueAngle - 90));
        TFT.fillCircle(blueX, blueY, GOAL_VIEW_GOAL_RADIUS, BLUE);
    }
     
    TFT.fillCircle(centreX, centreY, GOAL_VIEW_ROBOT_RADIUS, WHITE);

    oldYellowAngle = yellowAngle;
    oldYellowDistance = yellowDistance;
    oldBlueAngle = blueAngle;
    oldBlueDistance = blueDistance;
}

void GoalView::setGoalData(int ya, int yd, int ba, int bd) {
    if (ya != yellowAngle || yd != yellowDistance || ba != blueAngle || bd != blueDistance)  {
        yellowAngle = ya;
        yellowDistance = yd;
        blueAngle = ba;
        blueDistance = bd;

        setNeedsDraw();
    }
}

LightSensorView::LightSensorView(uint16_t x, uint16_t y, uint16_t w, uint16_t h) : View(x, y, w, h) {}

void LightSensorView::draw() {
    int16_t centreX = x + w / 2;
    int16_t centreY = y + h / 2;

    for (int i = 0; i < LS_NUM; i++) {
        bool on = (lightSensorData >> i) & 0x1;

        int16_t sensorX = centreX + (min(w, h) / 2 - LIGHT_SENSOR_VIEW_SENSOR_RADIUS * 2) * cos(degreesToRadians(i * LS_NUM_MULTIPLIER - 90));
        int16_t sensorY = centreY + (min(w, h) / 2 - LIGHT_SENSOR_VIEW_SENSOR_RADIUS * 2) * sin(degreesToRadians(i * LS_NUM_MULTIPLIER - 90));

        TFT.drawCircle(sensorX, sensorY, LIGHT_SENSOR_VIEW_SENSOR_RADIUS, WHITE);
        TFT.fillCircle(sensorX, sensorY, LIGHT_SENSOR_VIEW_SENSOR_RADIUS / 2, on ? WHITE : BLACK);
    }
}

void LightSensorView::setLightSensorData(int data) {
    if (lightSensorData != data) {
        lightSensorData = data;
        setNeedsDraw();
    }
}

RobotPositionView::RobotPositionView(uint16_t x, uint16_t y, uint16_t w, uint16_t h) : View(x, y, w, h) {}

void RobotPositionView::draw() {
    TFT.fillCircle(x + w / 2 + (double)(w - 2 * ROBOT_POSITION_VIEW_ROBOT_RADIUS) * ((double)oldX / (double)FIELD_WIDTH_CENTIMETERS), y + h - ROBOT_POSITION_VIEW_ROBOT_RADIUS - (double)(h - 2 * ROBOT_POSITION_VIEW_ROBOT_RADIUS) * ((double)oldY / (double)FIELD_LENGTH_CENTIMETERS), ROBOT_POSITION_VIEW_ROBOT_RADIUS, BACKGROUND_COLOR);
    TFT.fillCircle(x + w / 2 + (double)(w - 2 * ROBOT_POSITION_VIEW_ROBOT_RADIUS) * ((double)positionX / (double)FIELD_WIDTH_CENTIMETERS), y + h - ROBOT_POSITION_VIEW_ROBOT_RADIUS - (double)(h - 2 * ROBOT_POSITION_VIEW_ROBOT_RADIUS) * ((double)positionY / (double)FIELD_LENGTH_CENTIMETERS), ROBOT_POSITION_VIEW_ROBOT_RADIUS, FOREGROUND_COLOR);

    oldX = positionX;
    oldY = positionY;

    TFT.drawRect(x, y, w, h, FOREGROUND_COLOR);
}

void RobotPositionView::setRobotPositionData(double px, double py) {
    if (positionX != px || positionY != py) {
        positionX = px;
        positionY = py;

        setNeedsDraw();
    }
}

bool Button::getEnabled() {
    return enabled;
}

void Button::setNeedsDraw(bool highlightOnly) {
    needsDrawing = true;
    needsHighlightOnly = highlightOnly;
}

bool Button::isTouched(uint16_t tx, uint16_t ty, bool touchDown) {
    bool in = inButton(tx, ty);

    if ((in && touchDown) != highlighted) {
        highlighted = in && touchDown;
        setNeedsDraw(true);
    }

    return in && !touchDown;
}

bool Button::inButton(uint16_t tx, uint16_t ty) {
    return (tx > x) && (tx < x + w) && (ty > y) && (ty < y + h);
}

Label::Label(uint16_t x, uint16_t y, uint16_t w, uint16_t h, char *str, uint8_t textSize) : View(x, y, w, h), str(str), textSize(textSize) {}

void Label::draw() {
    TFT.fillRect(x, y, w, h, BACKGROUND_COLOR);
    TFT.setTextColor(FOREGROUND_COLOR);
    TFT.setCursor(x, y);
    TFT.setTextSize(textSize);
    TFT.print(str);
}

void Label::setText(char *newStr) {
    str = newStr;
    setNeedsDraw();
}

CircleButton::CircleButton(uint16_t cx, uint16_t cy, uint16_t r) : Button(cx - r, cy - r, r * 2, r * 2), cx(cx), cy(cy), r(r) {}

bool CircleButton::inButton(uint16_t tx, uint16_t ty) {
    return distanceBetween(cx, tx, cy, ty) < r;
}

EngineStartButton::EngineStartButton(uint16_t cx, uint16_t cy) : CircleButton(cx, cy, ENGINE_START_BUTTON_OUTER_RADIUS) {}

void EngineStartButton::draw() {
    if (needsHighlightOnly) {
        TFT.fillRoundRect(cx - ENGINE_START_BUTTON_INDICATOR_WIDTH / 2, cy + ENGINE_START_BUTTON_INDICATOR_Y, ENGINE_START_BUTTON_INDICATOR_WIDTH, ENGINE_START_BUTTON_INDICATOR_HEIGHT, ENGINE_START_BUTTON_INDICATOR_ROUNDED_RADIUS, enabled ? GREEN : RED);

        TFT.drawCircle(cx, cy, ENGINE_START_BUTTON_INNER_RADIUS - 3, highlighted ? TFT.color565(150, 150, 150) : TFT.color565(30, 30, 30));
    } else {
        uint16_t widthEngine, heightEngine, widthStart, heightStart;
        int16_t tempX, tempY;

        TFT.fillCircle(cx, cy, ENGINE_START_BUTTON_OUTER_RADIUS, TFT.color565(200, 200, 200));
        TFT.fillCircle(cx, cy, ENGINE_START_BUTTON_MIDDLE_RADIUS, TFT.color565(150, 150, 150));
        TFT.fillCircle(cx, cy, ENGINE_START_BUTTON_INNER_RADIUS, TFT.color565(30, 30, 30));

        TFT.setFont(&Org_01);
        TFT.setTextColor(WHITE);

        TFT.setTextSize(ENGINE_START_BUTTON_ENGINE_TEXT_SIZE);
        TFT.getTextBounds("ENGINE", 0, 0, &tempX, &tempY, &widthEngine, &heightEngine);
        TFT.setCursor(cx - widthEngine / 2, cy + ENGINE_START_BUTTON_ENGINE_TEXT_Y);
        TFT.print("ENGINE");
        TFT.setTextSize(ENGINE_START_BUTTON_START_TEXT_SIZE);
        TFT.getTextBounds("START", 0, 0, &tempX, &tempY, &widthStart, &heightStart);
        TFT.setCursor(cx - widthStart / 2, cy + ENGINE_START_BUTTON_START_TEXT_Y);
        TFT.print("START");

        TFT.setTextColor(FOREGROUND_COLOR);
        TFT.setFont();

        TFT.fillRoundRect(cx - ENGINE_START_BUTTON_INDICATOR_WIDTH / 2, cy + ENGINE_START_BUTTON_INDICATOR_Y, ENGINE_START_BUTTON_INDICATOR_WIDTH, ENGINE_START_BUTTON_INDICATOR_HEIGHT, ENGINE_START_BUTTON_INDICATOR_ROUNDED_RADIUS, enabled ? GREEN : RED);

        TFT.drawCircle(cx, cy, ENGINE_START_BUTTON_INNER_RADIUS - 3, highlighted ? TFT.color565(150, 150, 150) : TFT.color565(30, 30, 30));
    }
}

TextButton::TextButton(uint16_t cx, uint16_t cy, uint16_t r, char *str, uint8_t textSize) : CircleButton(cx, cy, r), str(str), textSize(textSize) {
    int16_t tempX, tempY;

    TFT.setTextSize(textSize);
    TFT.getTextBounds(str, 0, 0, &tempX, &tempY, &textWidth, &textHeight);
}

void TextButton::draw() {
    TFT.drawCircle(cx, cy, r, highlighted ? HIGHLIGHT_COLOR : FOREGROUND_COLOR);
    TFT.setCursor(cx - textWidth / 2, cy - textHeight / 2);
    TFT.setTextSize(textSize);
    TFT.print(str);
}

HomeButton::HomeButton(uint16_t cx, uint16_t cy) : CircleButton(cx, cy, HOME_BUTTON_OUTER_RADIUS * 1.5) {}

void HomeButton::draw() {
    TFT.drawCircle(cx, cy, HOME_BUTTON_OUTER_RADIUS, FOREGROUND_COLOR);
    TFT.fillCircle(cx, cy, HOME_BUTTON_INNER_RADIUS, highlighted ? BACKGROUND_COLOR : FOREGROUND_COLOR);
}

BackButton::BackButton(uint16_t x, uint16_t y) : Button(x, y, BACK_BUTTON_WIDTH, BACK_BUTTON_HEIGHT) {}

void BackButton::draw() {
    TFT.drawLine(x, y + h / 2, x + w, y + h / 2, highlighted ? HIGHLIGHT_COLOR : FOREGROUND_COLOR);
    TFT.drawLine(x, y + h / 2, x + w / 2, y, highlighted ? HIGHLIGHT_COLOR : FOREGROUND_COLOR);
    TFT.drawLine(x, y + h / 2, x + w / 2, y + h, highlighted ? HIGHLIGHT_COLOR : FOREGROUND_COLOR);
}

CheckBox::CheckBox(uint16_t x, uint16_t y) : Button(x, y, CHECK_BOX_OUTER_SIZE, CHECK_BOX_OUTER_SIZE) {}

void CheckBox::draw() {
    TFT.drawRect(x, y, CHECK_BOX_OUTER_SIZE, CHECK_BOX_OUTER_SIZE, highlighted ? HIGHLIGHT_COLOR : FOREGROUND_COLOR);
    TFT.fillRect(x + (CHECK_BOX_OUTER_SIZE - CHECK_BOX_INNER_SIZE) / 2, y + (CHECK_BOX_OUTER_SIZE - CHECK_BOX_INNER_SIZE) / 2, CHECK_BOX_INNER_SIZE, CHECK_BOX_INNER_SIZE, enabled ? FOREGROUND_COLOR : BACKGROUND_COLOR);
}

Switch::Switch(uint16_t x, uint16_t y, uint16_t onColor, char *onChar, uint16_t offColor, char *offChar) : Button(x, y, SWITCH_WIDTH, SWITCH_HEIGHT), onColor(onColor), onChar(onChar), offColor(offColor), offChar(offChar) {}

void Switch::draw() {
    TFT.drawRect(x, y, SWITCH_WIDTH, SWITCH_HEIGHT, highlighted ? HIGHLIGHT_COLOR : FOREGROUND_COLOR);
    TFT.fillRect(enabled ? x + SWITCH_TOGGLE_INSET : x + SWITCH_WIDTH / 2, y + SWITCH_TOGGLE_INSET, SWITCH_WIDTH / 2 - SWITCH_TOGGLE_INSET, SWITCH_HEIGHT - SWITCH_TOGGLE_INSET * 2, enabled ? onColor : offColor);
    TFT.fillRect(!enabled ? x + SWITCH_TOGGLE_INSET : x + SWITCH_WIDTH / 2, y + SWITCH_TOGGLE_INSET, SWITCH_WIDTH / 2 - SWITCH_TOGGLE_INSET, SWITCH_HEIGHT - SWITCH_TOGGLE_INSET * 2, BLACK);
    
    uint16_t textWidth, textHeight;
    int16_t textX, textY;

    TFT.setTextSize(2);
    TFT.getTextBounds(enabled ? onChar : offChar, 0, 0, &textX, &textY, &textWidth, &textHeight);

    TFT.setCursor(enabled ? x + SWITCH_TOGGLE_INSET + ((SWITCH_WIDTH / 2 - SWITCH_TOGGLE_INSET) - textWidth) / 2 : x + SWITCH_WIDTH / 2 + ((SWITCH_WIDTH / 2 - SWITCH_TOGGLE_INSET) - textWidth) / 2, y + (SWITCH_HEIGHT - textHeight) / 2);
    TFT.setTextColor(BACKGROUND_COLOR);
    TFT.print(enabled ? onChar : offChar);
}

Terminal::Terminal(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t textSize, uint16_t textColor) : View(x, y, w, h), textSize(textSize), textColor(textColor) {
    needsDrawing = false;
}

void Terminal::clear() {
    TFT.fillRect(x, y, w, h, BACKGROUND_COLOR);
    TFT.setCursor(x, y);
}

size_t Terminal::write(uint8_t c) {
    if (bufferSize == 100) {
        return 0;
    } else {
        bufferSize++;
        charBuffer[tail] = c;
        tail = mod(tail + 1, 100);
    }

    return 1;
}

void Terminal::clearIfOverflow() {
    if (TFT.getCursorY() > y + h - textSize * 8) {
        clear();
    }
}

void Terminal::draw() {
    clear();
}

void Terminal::drawChar(uint8_t c) {
    TFT.setMaxCursor(x + w, y + h);
    TFT.setBaseCursor(x);
    TFT.write(c);
   
    clearIfOverflow();
}

void Terminal::drawFromBuffer() {
    TFT.setTextColor(textColor);
    TFT.setTextSize(textSize);

    while (bufferSize > 0) {
        drawChar(charBuffer[head]);
        bufferSize--;
        head = mod(head + 1, 100);
    }

    TFT.setBaseCursor(0);
    TFT.setMaxCursor(TFT.width(), TFT.height());
}