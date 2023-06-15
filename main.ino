// FireFighter Robot by PlutoVerse, with MIT license
/*
 This code is to build a fire fighting moving moving vehicle.
 The vehicle can avoid obstacles using the feedback from ultrasonic sensor.
 
 */

#include <Servo.h>

#define servo_motor 8  // attach pin D9 of Arduino to signal pin of servo
#define echoPin 7      // attach pin D2 of Arduino to pin Echo of HC-SR04
#define trigPin 6      // attach pin D3 of Arduino to pin Trig of HC-SR04  
#define motor1_1 13    // attach pin D13 of Arduino to the Motor
#define motor1_2 12    // attach pin D12 of Arduino to the Motor
#define motor2_1 11    // attach pin D11 of Arduino to the Motor
#define motor2_2 10    // attach pin D10 of Arduino to the Motor
#define pump 9        // attach pin D13 of Arduino to the Pump
#define ldr1 A1
#define ldr2 A2
#define ldr3 A3
Servo myservo;         // create servo object to control a servo


int distance;
int threshold = 25;
int fire =600;
int indicate_fire =600;
int directions;
  
void setup() {

  myservo.attach(servo_motor , 600, 2300);  // (pin, min, max)
  pinMode(trigPin,OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(motor1_1, OUTPUT); // set motor1 as OUPUT
  pinMode(motor1_2, OUTPUT); // set motor1 as OUPUT
  pinMode(motor2_1, OUTPUT); // set motor2 as OUPUT
  pinMode(motor2_2, OUTPUT); // set motor2 as OUPUT
  pinMode(pump , OUTPUT);    // set pump as OUPUT
  pinMode(ldr1, INPUT);
  pinMode(ldr2, INPUT);
  pinMode(ldr2, INPUT);
//  myservo.write(90);       // set the servo to the front
//  delay(500);
//  myservo.write(0);
//  delay(500);
//  myservo.write(180); 
//  delay(500);
//  myservo.write(90);
//  delay(500);
  digitalWrite(pump,HIGH);
  delay(1000);
  digitalWrite(pump,LOW);
  delay(1000);
  Serial.begin(9600);
}


void move_forward() {
  digitalWrite(motor1_1,HIGH);
  digitalWrite(motor1_2, LOW);
  digitalWrite(motor2_1,HIGH);
  digitalWrite(motor2_2, LOW);
  delay(50);
  digitalWrite(motor1_1,LOW);
  digitalWrite(motor1_2, LOW);
  digitalWrite(motor2_1,LOW);
  digitalWrite(motor2_2, LOW);
  delay(40);
}
void move_left() {
  digitalWrite(motor1_1,HIGH);
  digitalWrite(motor1_2, LOW);
  digitalWrite(motor2_1,LOW);
  digitalWrite(motor2_2, HIGH);
  delay(200);
  digitalWrite(motor1_1,LOW);
  digitalWrite(motor1_2, LOW);
  digitalWrite(motor2_1,LOW);
  digitalWrite(motor2_2, LOW);
  delay(100);
}
void move_right() {
  digitalWrite(motor1_1,LOW);
  digitalWrite(motor1_2, HIGH);
  digitalWrite(motor2_1,HIGH);
  digitalWrite(motor2_2, LOW);
  delay(200);
  digitalWrite(motor1_1,LOW);
  digitalWrite(motor1_2, LOW);
  digitalWrite(motor2_1,LOW);
  digitalWrite(motor2_2, LOW);
  delay(100);
}
void smove_left() {
  digitalWrite(motor1_1,HIGH);
  digitalWrite(motor1_2, LOW);
  digitalWrite(motor2_1,LOW);
  digitalWrite(motor2_2, HIGH);
  delay(10);
  digitalWrite(motor1_1,LOW);
  digitalWrite(motor1_2, LOW);
  digitalWrite(motor2_1,LOW);
  digitalWrite(motor2_2, LOW);
  delay(100);
}
void smove_right() {
  digitalWrite(motor1_1,LOW);
  digitalWrite(motor1_2, HIGH);
  digitalWrite(motor2_1,HIGH);
  digitalWrite(motor2_2, LOW);
  delay(10);
  digitalWrite(motor1_1,LOW);
  digitalWrite(motor1_2, LOW);
  digitalWrite(motor2_1,LOW);
  digitalWrite(motor2_2, LOW);
  delay(100);
}
void move_halt() {
  digitalWrite(motor1_1,LOW);
  digitalWrite(motor1_2, LOW);
  digitalWrite(motor2_1,LOW);
  digitalWrite(motor2_2, LOW);
}

int find_distance(){
  /*
   Function will find the distance to the obstacle from the ultrasonci sensor.
   Returns the distance in cm unit.
   */
  long duration;               // variable to store duration
  int distance;                // varaible to store distance
  digitalWrite(trigPin, LOW);  // turn of the Trigger if its already ON
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); // turn on the Trigger to generate pulse
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);  // trun off the Trigger to stop generating pulse
  duration = pulseIn(echoPin, HIGH); // time taken to recieve the generated pulse
  distance = duration * 0.0344 / 2;  // finding distance using the speed of the wave and duration taken to reach the reciver
  return distance;
}

int survilance(){
  /*
   Function will move the servo motor in 180 degree back and forth 
   with 90 degree steps as 90,0,90,180 at 1 sec interval.
   Return the direction which is open 0=>left 1=> Right 2=> Back
   */
  int straight, left, right;
  
  myservo.write(90); // 
  straight = find_distance();
  right= find_distance();
  delay(1000);
  myservo.write(90);
  
  // finding which direction to move
  if(left>right)
    return 0; // to move left

  else if (right>left)
    return 1; // to move right

  else
    return 0; // to move back

  
}

void pumpOn(){
  digitalWrite(pump,HIGH);
}
void pumpOff(){
  digitalWrite(pump,LOW);
}

void loop() {

  int left,right,straight;
  char moveTo ='L';
  left =analogRead(ldr1);
  right =analogRead(ldr2);
  straight =analogRead(ldr3);
  Serial.print("Right:");
  Serial.println(right);
  Serial.print("Left:");
  Serial.println(left);
  Serial.print("Straight:");
  Serial.println(straight);
  if(left > right){
    if(left > indicate_fire){
      smove_left();
      Serial.println("moving left");
      moveTo ='L';
    }
  }
    else if (right>indicate_fire){
      smove_right();
      Serial.println("moving right");
      moveTo ='R';
    }
 
  else{
  if( straight > fire){
    pumpOn();
    Serial.println("pumpON");
    if (moveTo == 'R'){
      for(int i=0;i<=20;i++){
        smove_right();
      }
      for(int i=0;i<=10;i++){
        smove_left();
      }
      }
    if (moveTo == 'L'){
    for(int i=0;i<=20;i++){
      smove_left();
    }
    for(int i=0;i<=10;i++){
      smove_right();
    }
    }
    
  }
  else{
    pumpOff();
    Serial.println("pumpOFF");
  }

  distance = find_distance();
  Serial.print("Distance:");
  Serial.println(distance);
  
  if (distance <= threshold){
    if (straight >fire){
      pumpOn();
      Serial.println("pumpON");
      if (moveTo == 'R'){
      for(int i=0;i<=20;i++){
        smove_right();
      }
      for(int i=0;i<=10;i++){
        smove_left();
      }
      }
      if (moveTo == 'L'){
      for(int i=0;i<=20;i++){
        smove_left();
      }
      for(int i=0;i<=10;i++){
        smove_right();
      }
      }
    }
    else{
      pumpOff();
      if (right> left)
      move_right();
      else
      move_left();
      Serial.println("moving left");
    }
    
    
  }

  else{
    move_forward();
    pumpOff();
    Serial.println("pumpOFF");
  }
  delay(10);
  }
}
