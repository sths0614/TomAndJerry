
#define leftReceiverT 23
#define leftEmitterT 2
#define rightReceiverT 22
#define rightEmitterT 3

#define leftReceiverB 18
#define leftEmitterB 6
#define rightReceiverB 19
#define rightEmitterB 7

#define leftEnable 10
#define rightEnable 21

#define leftEncoder 16
#define rightEncoder 17

#define leftWheelT 9
#define leftWheelB 4
#define rightWheelB 5
#define rightWheelT 20

void straight();

int topx, topy, botx, boty;
void setup(){
  // Input pins
  pinMode(leftReceiverT, INPUT);
  pinMode(rightReceiverT, INPUT);
  pinMode(leftReceiverB, INPUT);
  pinMode(rightReceiverB, INPUT);
  
  // Output pins
  pinMode(leftEmitterT, OUTPUT);
  pinMode(rightEmitterT, OUTPUT);
  pinMode(leftEmitterB, OUTPUT);
  pinMode(rightEmitterB, OUTPUT);

  pinMode(leftEnable, OUTPUT);
  pinMode(rightEnable, OUTPUT);
  
  // set hbridge pins to OUTPUT
  pinMode(leftEnable, OUTPUT); //EN1
  pinMode(rightEnable, OUTPUT);
  // left wheel
  pinMode(leftWheelT, OUTPUT);
  pinMode(leftWheelB, OUTPUT);
  // right wheel
  pinMode(rightWheelT, OUTPUT);
  pinMode(rightWheelB, OUTPUT);
  Serial.begin(9600);
}

void loop(){
  straight();
//  analogWrite(leftEnable, 0);
//  analogWrite(rightEnable, 0);
//  testLeftTLED();
//  delay(1000);
//  testRightTLED();
//  delay(1000);
//  testLeftBLED();
//  delay(1000);
//  testRightBLED();
//  delay(1000);
//  digitalWrite(rightEmitterB, HIGH);
  
//  Serial.println(" Top right: ");
//  Serial.print(topy);
//  delay(500);
 
//  Serial.println("Bot left: ");
//  Serial.print(botx);
  
//  Serial.println(" Bot right: ");
//  Serial.println(boty);
//  delay(500);  
  /*
  digitalWrite(frontREmitter, HIGH);
  delay(100);
  frontR = analogRead(frontRReceiver);
  delay(140);
  digitalWrite(frontREmitter, LOW);

  digitalWrite(frontLEmitter, HIGH);
  delay(100);
  frontL = analogRead(frontLReceiver);
  delay(140);
  digitalWrite(frontLEmitter, LOW);  
  
  digitalWrite(rightEmitter, HIGH);
  delay(100);
  right = analogRead(rightReceiver);
  delay(140);
  digitalWrite(rightEmitter, LOW);

  digitalWrite(leftEmitter, HIGH);
  delay(100);
  left = analogRead(leftReceiver);
  delay(140);
  digitalWrite(leftEmitter, LOW);
  
//  if(frontR >= 68){
//    if(frontL >= 40)
//      Serial.println("Wall front");
//  }
  
  if(right >= 80 && right <= 150){
    if(left <= 52)
      Serial.println("Slightly wall right");
  }
  if(right > 150)
    Serial.println("Very close right wall");
    
 if(left >= 40 && left <= 65){
   if(right <= 80)
     Serial.println("Slightly wall left");
 } 
 if(left > 65)
   Serial.println("Very close left wall");
 //Theoretically, frontL*0.90 == frontR
// if(frontL *0.90 - frontR >= 20)
//   Serial.println("Turn left");
// else if(frontR - frontL * 0.90 >= 30)
 //  Serial.println("Turn right");
 */
}

void testLeftTLED(){
  digitalWrite(leftEmitterT, HIGH);
  topx = analogRead(leftReceiverT);
  delay(500);
  digitalWrite(leftEmitterT, LOW);
  Serial.print("Top Left: ");
  Serial.println(topx);
}

void testRightTLED(){
  digitalWrite(rightEmitterT, HIGH);
  topy = analogRead(rightReceiverT);
  delay(500);
  digitalWrite(rightEmitterT, LOW);
  Serial.print("Top Right: ");
  Serial.println(topy);
}

void testLeftBLED(){
  digitalWrite(leftEmitterB, HIGH);
  botx = analogRead(leftReceiverB);
  delay(500);
  digitalWrite(leftEmitterB, LOW);
  Serial.print("Bot Left: ");
  Serial.println(botx);
}

void testRightBLED(){
  digitalWrite(rightEmitterB, HIGH);
  boty = analogRead(rightReceiverB);
  delay(500);
  digitalWrite(rightEmitterB, LOW);
  Serial.print("Bot Right: ");
  Serial.println(boty);
}

void straight(){
  digitalWrite(leftEnable, HIGH);
  digitalWrite(rightEnable, HIGH);
  analogWrite(leftWheelT, 100);
  analogWrite(leftWheelB, 0);
  analogWrite(rightWheelT, 100);
  analogWrite(rightWheelB, 0);
}
