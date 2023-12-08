/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____  
 * / _ \ /___)/ _ \| | | |/ _ \ / _ \ / ___) _ \|    \ 
 *| |_| |___ | |_| | |_| | |_| | |_| ( (__| |_| | | | |
 * \___/(___/ \___/ \__  |\___/ \___(_)____)___/|_|_|_|
 *                  (____/ 
 * Arduino Smart Car Tutorial Lesson 5
 * Tutorial URL http://osoyoo.com/2018/12/19/osoyoo-robot-car-kit-lesson-4-obstacle-avoidance-robot-car/
 * CopyRight www.osoyoo.com

 * This project will show you how to make Osoyoo robot car in auto drive mode and avoid obstacles
 *   
 * 
 */
  #include <Servo.h>
/*Declare L298N Dual H-Bridge Motor Controller directly since there is not a library to load.*/
//Define L298N Dual H-Bridge Motor Controller Pins
#define speedPinR 3   // RIGHT PWM pin connect MODEL-X ENA
#define RightDirectPin1  12    //  Right Motor direction pin 1 to MODEL-X IN1 
#define RightDirectPin2  11    // Right Motor direction pin 2 to MODEL-X IN2
#define speedPinL 6        //  Left PWM pin connect MODEL-X ENB
#define LeftDirectPin1  7    // Left Motor direction pin 1 to MODEL-X IN3
#define LeftDirectPin2  8   ///Left Motor direction pin 1 to MODEL-X IN4
#define LPT 2 // scan loop coumter

#define SERVO_PIN     9  //servo connect to D9

#define Echo_PIN    2 // Ultrasonic Echo pin connect to D11
#define Trig_PIN    10  // Ultrasonic Trig pin connect to D12

#define BUZZ_PIN     13
#define FAST_SPEED  250     //both sides of the motor speed
#define SPEED  120     //both sides of the motor speed
#define TURN_SPEED  200     //both sides of the motor speed
#define BACK_SPEED1  255     //back speed
#define BACK_SPEED2  90     //back speed

#define MIN_INT -32768

int leftscanval, centerscanval, rightscanval, ldiagonalscanval, rdiagonalscanval;
const int distancelimit = 35; //distance limit for obstacles in front           
const int sidedistancelimit = 40; //minimum distance in cm to obstacles at both sides (the car will allow a shorter distance sideways)
int distance;
int numcycles = 0;
const int turntime = 300; //Time the robot spends turning (miliseconds)
const int backtime = 350; //Time the robot spends turning (miliseconds)

int thereis;
Servo head;
/*motor control*/
void go_Advance(void)  //Forward
{
  digitalWrite(RightDirectPin1, HIGH);
  digitalWrite(RightDirectPin2,LOW);
  digitalWrite(LeftDirectPin1,HIGH);
  digitalWrite(LeftDirectPin2,LOW);
}
void go_Left()  //Turn left
{
  digitalWrite(RightDirectPin1, HIGH);
  digitalWrite(RightDirectPin2,LOW);
  digitalWrite(LeftDirectPin1,LOW);
  digitalWrite(LeftDirectPin2,HIGH);
}
void go_Right()  //Turn right
{
  digitalWrite(RightDirectPin1, LOW);
  digitalWrite(RightDirectPin2,HIGH);
  digitalWrite(LeftDirectPin1,HIGH);
  digitalWrite(LeftDirectPin2,LOW);
}
void go_Back()  //Reverse
{
  digitalWrite(RightDirectPin1, LOW);
  digitalWrite(RightDirectPin2,HIGH);
  digitalWrite(LeftDirectPin1,LOW);
  digitalWrite(LeftDirectPin2,HIGH);
}
void stop_Stop()    //Stop
{
  digitalWrite(RightDirectPin1, LOW);
  digitalWrite(RightDirectPin2,LOW);
  digitalWrite(LeftDirectPin1,LOW);
  digitalWrite(LeftDirectPin2,LOW);
  set_Motorspeed(0,0);
}

/*set motor speed */
void set_Motorspeed(int speed_L,int speed_R)
{
  analogWrite(speedPinL,speed_L); 
  analogWrite(speedPinR,speed_R);   
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
void alarm(){
   buzz_ON();
 
   buzz_OFF();
}

/*detection of ultrasonic distance*/
int watch(){
  long echo_distance;
  digitalWrite(Trig_PIN,LOW);
  delayMicroseconds(5);                                                                              
  digitalWrite(Trig_PIN,HIGH);
  delayMicroseconds(15);
  digitalWrite(Trig_PIN,LOW);
  echo_distance=pulseIn(Echo_PIN,HIGH);
  echo_distance=echo_distance*0.01657; //how far away is the object in cm
  //Serial.println((int)echo_distance);
  
    Serial.println(echo_distance);
  return round(echo_distance);
}
//Meassures distances to the right, left, front, left diagonal, right diagonal and asign them in cm to the variables rightscanval, 
//leftscanval, centerscanval, ldiagonalscanval and rdiagonalscanval (there are 5 points for distance testing)
String watchsurrounding(){
/*  obstacle_status is a binary integer, its last 5 digits stands for if there is any obstacles in 5 directions,
 *   for example B101000 last 5 digits is 01000, which stands for Left front has obstacle, B100111 means front, right front and right ha
 */
  int longestDist = MIN_INT;
  String direction = "Trapped";
  
  
  
  head.write(0); //Didn't use 180 degrees because my servo is not able to take this angle
  delay(100);
  rightscanval = watch();
  if(rightscanval>distancelimit){
    /*stop_Stop();
    alarm();*/
    if (rightscanval > longestDist){
      longestDist = rightscanval;
      Serial.println("rightsee");
    
      direction = "right";
    }
  }
  

  head.write(45);
  delay(200);
  rdiagonalscanval = watch();
  if(rdiagonalscanval>sidedistancelimit){
    /*stop_Stop();
    alarm();*/
    if (rdiagonalscanval > longestDist){
    longestDist = rdiagonalscanval;
    Serial.println("rightdiagsee");

      direction = "rightDiag";
    }
  }
  
  
  head.write(90);
  delay(200);
  centerscanval = watch();
  if(centerscanval>distancelimit){
    /*stop_Stop();
    alarm();*/
    if (centerscanval > longestDist){
      longestDist = centerscanval;
      direction = "center";
      Serial.println("frontsee");
    }
  }
  
  /*head.write(90); //use 90 degrees if you are moving your servo through the whole 180 degrees
  delay(300);
  centerscanval = watch();
  if(centerscanval<distancelimit){
    stop_Stop();
    alarm();
    }
  */
  head.write(135);
  delay(200);
  ldiagonalscanval = watch();
  if(ldiagonalscanval>sidedistancelimit){
    /*stop_Stop();
    alarm();*/
    if (ldiagonalscanval > longestDist){
      longestDist = ldiagonalscanval;
      Serial.println("left diag see");

      direction = "leftDiag";
    }
  }
  
  head.write(180);
  delay(200);
  leftscanval = watch();
  if(leftscanval>distancelimit){
    /*stop_Stop();
    alarm();*/
    if (leftscanval > longestDist){
      longestDist = leftscanval;
      Serial.println("left see");

      direction = "left";
    }  
  }
  
  head.write(90); //Finish looking around (look forward again)
  delay(200);

  return direction; //return 5-character string standing for 5 direction obstacle status
}

void auto_avoidance(){
  stop_Stop();
  String direction = watchsurrounding(); // 5 digits of obstacle_sign binary value means the 5 direction obstacle status
  if (direction == "rightDiag"){
    Serial.println("diag right");
    set_Motorspeed(FAST_SPEED,SPEED);
    go_Right();
 
    delay(turntime);
    stop_Stop();
  }
  else if (direction == "leftDiag"){
    Serial.println("diag LEFT");
    set_Motorspeed(SPEED,FAST_SPEED);
    go_Left();
    delay(turntime);
    stop_Stop();
  }
  else if (direction == "right"){
    Serial.println("hand right");
	  go_Right();
    set_Motorspeed(TURN_SPEED,TURN_SPEED);
    delay(backtime);
    stop_Stop();
  } 
  else if (direction == "left"){
    Serial.println("hand left");
    go_Left();//Turn left
    set_Motorspeed(TURN_SPEED,TURN_SPEED);
    delay(backtime);
    stop_Stop();
  }
  else if (direction == "center"){
    set_Motorspeed(SPEED,SPEED);
    go_Advance();  // if nothing is wrong go forward using go() function above.
    delay(backtime);
    stop_Stop();
  }
  else if (direction == "Trapped"){
    Serial.println("aaaa");
    alarm();
	  go_Right();
    set_Motorspeed(TURN_SPEED,TURN_SPEED);
    delay(backtime*2);

    stop_Stop();
  }

  
  //else  Serial.println(numcycles);
  
  /*distance = watch(); // use the watch() function to see if anything is ahead (when the robot is just moving forward and not looking around it will test the distance in front)
  if (distance<distancelimit){ // The robot will just stop if it is completely sure there's an obstacle ahead (must test 25 times) (needed to ignore ultrasonic sensor's false signals)
 Serial.println("final go back");
    go_Right();
    set_Motorspeed( SPEED,FAST_SPEED);
  delay(backtime*3/2);
      ++thereis;}
  if (distance>distancelimit){
      thereis=0;} //Count is restarted
  if (thereis > 25){
  Serial.println("final stop");
    stop_Stop(); // Since something is ahead, stop moving.
    thereis=0;
  }*/
}

void setup() {
  /*setup L298N pin mode*/
  pinMode(RightDirectPin1, OUTPUT); 
  pinMode(RightDirectPin2, OUTPUT); 
  pinMode(speedPinL, OUTPUT);  
  pinMode(LeftDirectPin1, OUTPUT);
  pinMode(LeftDirectPin2, OUTPUT); 
  pinMode(speedPinR, OUTPUT); 
  stop_Stop();//stop move
  /*init HC-SR04*/
  pinMode(Trig_PIN, OUTPUT); 
  pinMode(Echo_PIN,INPUT); 
  /*init buzzer*/
  pinMode(BUZZ_PIN, OUTPUT);
  digitalWrite(BUZZ_PIN, HIGH);  
  buzz_OFF(); 

  digitalWrite(Trig_PIN,LOW);
  /*init servo*/
  head.attach(SERVO_PIN); 
  head.write(90);
   delay(2000);
  
  Serial.begin(9600);
 
 
}

void loop() {
  auto_avoidance();
}
