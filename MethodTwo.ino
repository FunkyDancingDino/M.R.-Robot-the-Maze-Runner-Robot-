#include <Servo.h>

#define speedPinR 3   // RIGHT PWM pin connect MODEL-X ENA
#define RightDirectPin1  12    //  Right Motor direction pin 1 to MODEL-X IN1 
#define RightDirectPin2  11    // Right Motor direction pin 2 to MODEL-X IN2
#define speedPinL 6        //  Left PWM pin connect MODEL-X ENB
#define LeftDirectPin1  7    // Left Motor direction pin 1 to MODEL-X IN3
#define LeftDirectPin2  8   ///Left Motor direction pin 1 to MODEL-X IN4

#define SERVO_PIN     9  //servo connect to D9

#define Echo_PIN    2 // Ultrasonic Echo pin connect to D11
#define Trig_PIN    10  // Ultrasonic Trig pin connect to D12

#define SPEED 120
#define TURNSPEED 200

#define BUZZ_PIN     13

const int fTime = 550;
const int tTime = 430;
const int lookTime = 500;
int front, left, right, lDiag, rDiag;
int minSideDist = 50, minFrontDist = 45;
bool rightClear = false, rDiagClear = false, leftClear = false, lDiagClear = false, frontClear = false;

Servo head;

//Pins initialize
void init_GPIO()
{
	pinMode(RightDirectPin1, OUTPUT); 
	pinMode(RightDirectPin2, OUTPUT); 
	pinMode(speedPinL, OUTPUT); 
  
  pinMode(Trig_PIN, OUTPUT); // Sets the trigPin as an Output
  pinMode(Echo_PIN, INPUT); // Sets the echoPin as an Input 
 
	pinMode(LeftDirectPin1, OUTPUT);
  pinMode(LeftDirectPin2, OUTPUT); 
  pinMode(speedPinR, OUTPUT); 
	stop_Stop();
  /*init buzzer*/
  /*pinMode(BUZZ_PIN, OUTPUT);
  digitalWrite(BUZZ_PIN, HIGH);  
  buzz_OFF(); */

  digitalWrite(Trig_PIN,LOW);
  /*init servo*/
  head.attach(SERVO_PIN); 
  head.write(90);
  delay(2000);
  
  Serial.begin(9600);
}
void buzz_ON()   //open buzzer
{
  
  for(int i=0;i<100;i++)
  {
   digitalWrite(BUZZ_PIN,LOW);
   delay(2);//wait for 1ms
   digitalWrite(BUZZ_PIN,HIGH);
   delay(2);//wait for 1ms
  }
}
void buzz_OFF()  //close buzzer
{
  digitalWrite(BUZZ_PIN, HIGH);
  
}
void set_Motorspeed(int speed_L,int speed_R)
{
  analogWrite(speedPinL,speed_L); 
  analogWrite(speedPinR,speed_R);   
}

void go_Advance(void)  //Forward
{
  digitalWrite(RightDirectPin1, HIGH);
  digitalWrite(RightDirectPin2,LOW);
  digitalWrite(LeftDirectPin1,HIGH);
  digitalWrite(LeftDirectPin2,LOW);
  set_Motorspeed(SPEED,SPEED);
  delay(fTime);
  Serial.println("Adv");
  stop_Stop();
}

void go_Left(void)  //Turn left
{
  digitalWrite(RightDirectPin1, HIGH);
  digitalWrite(RightDirectPin2,LOW);
  digitalWrite(LeftDirectPin1,LOW);
  digitalWrite(LeftDirectPin2,HIGH);
  set_Motorspeed(TURNSPEED,TURNSPEED);
  delay(tTime+180);
  Serial.println("Left");
  stop_Stop();
}

void go_Right(void)  //Turn right
{
  digitalWrite(RightDirectPin1, LOW);
  digitalWrite(RightDirectPin2,HIGH);
  digitalWrite(LeftDirectPin1,HIGH);
  digitalWrite(LeftDirectPin2,LOW);
  set_Motorspeed(TURNSPEED,TURNSPEED);
  delay(tTime);
  Serial.println("Right");
  stop_Stop();
}

void go_Back(void)  //Reverse
{
  digitalWrite(RightDirectPin1, LOW);
  digitalWrite(RightDirectPin2,HIGH);
  digitalWrite(LeftDirectPin1,LOW);
  digitalWrite(LeftDirectPin2,HIGH);
  set_Motorspeed(SPEED,SPEED);
  delay(fTime);
  Serial.println("Back");
  stop_Stop();
}

void go_Diag_Left(void)  //Turn left diagonally
{
  digitalWrite(RightDirectPin1, HIGH);
  digitalWrite(RightDirectPin2,LOW);
  digitalWrite(LeftDirectPin1,LOW);
  digitalWrite(LeftDirectPin2,HIGH);
  set_Motorspeed(TURNSPEED,TURNSPEED);
  delay((tTime+80) / 2);
  Serial.println("lDiag");
  stop_Stop();
}

void go_Diag_Right(void)  //Turn right diagonally
{
  digitalWrite(RightDirectPin1, LOW);
  digitalWrite(RightDirectPin2,HIGH);
  digitalWrite(LeftDirectPin1,HIGH);
  digitalWrite(LeftDirectPin2,LOW);
  set_Motorspeed(TURNSPEED,TURNSPEED);
  delay(tTime / 2);
  Serial.println("rDiag");
  stop_Stop();
}

void stop_Stop()    //Stop
{
  digitalWrite(RightDirectPin1, LOW);
  digitalWrite(RightDirectPin2,LOW);
  digitalWrite(LeftDirectPin1,LOW);
  digitalWrite(LeftDirectPin2,LOW);
  set_Motorspeed(0,0);
  Serial.println("Stop");
}

int watch()
{
  // Clears the trigPin
  digitalWrite(Trig_PIN, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(Trig_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig_PIN, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(Echo_PIN, HIGH);
  // Calculating the distance
  int distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  return distance;
}

void watchSurrounding()
{
  head.write(0);
  delay(lookTime);
  if((watch() > minSideDist))
  {
    rightClear = true;
    Serial.println("Noseer"); 
  }
  delay(lookTime);

  /*head.write(45);
  delay(lookTime);
  rDiag = watch();
  if((rDiag > minSideDist))
  {
    rDiagClear = true;
  }*/

  head.write(90);
  delay(lookTime);
  //front = watch();
  if((watch() > minFrontDist))
  {
    frontClear = true;
    Serial.println("Noseef");
  }
  delay(lookTime);

  /*head.write(135);
  delay(lookTime);
  lDiag = watch();
  if((lDiag > minSideDist))
  {
    lDiagClear = true;
  }*/

  head.write(180);
  //left = watch();
  delay(lookTime);
  if(watch() > minSideDist)
  {
    leftClear = true;
    Serial.println("Noseer");
  }
  delay(lookTime);
  
  head.write(90);
  delay(lookTime);
  return;
}

void setup() {
  // put your setup code here, to run once:
  init_GPIO();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  watchSurrounding();
  delay(lookTime);
  if (rightClear)
  {
    go_Right();
    go_Advance();
    go_Advance();
    /*if (rDiagClear)
    {
      go_Advance();
      rDiagClear = false;
    }*/
    rightClear = false;
  }
  else if (frontClear)
  {
    go_Advance();
    go_Advance();
    frontClear = false;
  }
  else if (leftClear)
  {
    go_Left();
    go_Advance();
    go_Advance();
    /*if (lDiagClear)
    {
      go_Advance();
      
      
      lDiagClear = false;
    }*/
    leftClear = false;
  }
  else
  {
    go_Right();
    go_Right();
    go_Advance();
    go_Advance();
  }

  delay(lookTime);
  
}
