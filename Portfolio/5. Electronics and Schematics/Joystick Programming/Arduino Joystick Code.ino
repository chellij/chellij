
// Fan and Joystick
#define analogX A0    // Analog input pin Joystick X direction
#define analogY A1    // Analog input pin Joystick Y direction
#define JoySwitch 13  // Analog input pin Joystick Button
#define MotorA 6      // Input 1 from Arduino to DC Motor via L293D Motor Driver
#define MotorB 5      // Input 2 from Arduino to DC Motor via L293D Motor Driver

#define ENDriver 7    // L293D Motor Driver Enable Pin

#define JoyXCentre 496  // analog readings for X and Y when joystick is in centre
#define JoyYCentre 511

int valX = analogRead(analogX); 
int valXneg = analogRead(analogX);
int valY = analogRead(analogY);
int valYneg = analogRead(analogY);
int JoyX = 0; 
int JoyY = 0;  


//Servo
#define ServoSignal 4   // Servo Control Signal 
int on;
int off;

//Stepper Motor Controller
#define StepperCoilA 8 
#define StepperCoilB 9  
#define StepperCoilC 10 
#define StepperCoilD 11

void setup() 
{
  Serial.begin(9600);
  pinMode(MotorA, OUTPUT);
  pinMode(MotorB, OUTPUT);
  pinMode(ENDriver, OUTPUT);
}

void pos()
{
  int loc = 0;
  loc = analogRead(JoyX); 
  loc = map(loc, 0, 1023, 2500, 500);
  on = loc;
  off = 20000-loc;
  int deg = loc;
  deg = map(deg, 500, 2500, 180, 0);
  Serial.println(deg);
}

void loop() 
{
  valX = map(valX, 0, 515, 255, 0);
  valXneg = map(valXneg, 519, 1023, 0, 255);
  
  JoyY = analogRead(analogY);
  // print the results to the Serial Monitor:
  Serial.print("JOYSTICK X=");
  Serial.print(JoyX);
  Serial.print(" JOYSTICK Y=");
  Serial.print(JoyY);
  Serial.print(" Servo Angle=");
  Serial.print(ServoSignal);
  Serial.print(" PWM and direction=");
  Serial.print(" ");
  Serial.print(" Stepper Motor direction=");
  Serial.println(" ");
  
  JoyX = analogRead(analogX);
  valX = map(valX, 0, 515, 255, 0);
  valXneg = map(valXneg, 519, 1023, 0, 255);

  if (JoyX < JoyXCentre-5)
  {
    //Drive Motor one direction
    digitalWrite(MotorA, HIGH);
    digitalWrite(MotorB, LOW);
  }

  else if (JoyX > JoyXCentre+5)
  {
    //Drive Motor other direction
    digitalWrite(MotorA, LOW);
    delayMicroseconds(off);
    digitalWrite(MotorB, HIGH);
    delayMicroseconds(on);
  }

  else
  {
    digitalWrite(MotorA, LOW);
    digitalWrite(MotorB, LOW);
  }
  
}
