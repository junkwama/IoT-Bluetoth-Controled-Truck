
#include <Stepper.h>
#include <Servo.h>

//definitions for each command to be recieved via serial port
String COMMAND_FORWARD = "f";
String COMMAND_BACK = "b";
String COMMAND_STOP = "s";

//enter the steps per rev for your motors here
int stepsInRev = 200;
String truckMoving = "f";
bool isTruckStopped = true;

int truckDirectionAngle = 0;
int angleStepPerChange = 5;
int maxAngle = 90;
int minAngle = 0;

// setup pins for each driver motor1 ~ IN1, IN2, IN3, IN4; motor2 ~ IN1, IN2, IN3, IN4
Stepper truckMotor(stepsInRev,  8, 9, 10, 11);
Servo myservo;


void setup() {
  Serial.begin(9600);//start the bluetooth serial port - send and recieve at 9600 baud
  // set the speed at 60 rpm:
  truckMotor.setSpeed(40);
  myservo.attach(7);
  myservo.write(truckDirectionAngle);
}

void loop() {
  
  //check to see if there is serial communication and if so read the data
  if(Serial.available()){
    String input = Serial.readString();
    if(input.startsWith("m-")){
      truckMoving = getInputData(input);
      Serial.print("Moving side input given: ");
      Serial.println(truckMoving);
    }else if(input.startsWith("d-")){
      String _angle = getInputData(input);
      setDirection(_angle);
    }
  }

  //Serial.println(COMMAND_FORWARD);
  //Serial.println(truckMoving);

  if(truckMoving.equals(COMMAND_FORWARD)){
    forwardStep();
  }else if(truckMoving.equals(COMMAND_BACK)){
    backwardStep();
  }
}


//to move the motors forward
void forwardStep(){
  if(!isTruckStopped){
    Serial.println("moving forward");
     truckMotor.step(stepsInRev);
  }else{
    Serial.println("Stopped");
  }
}

// to move the motors back
void backwardStep(){
  if(!isTruckStopped){
    Serial.println("moving backward");
    truckMotor.step(-stepsInRev);
  }else{
    Serial.println("Stopped");
  }
}

//Stering operation
void setDirection(String _direction){
  if(_direction == "r"){
    
    if(truckDirectionAngle < maxAngle){
      Serial.print("Changing diection : ");
      truckDirectionAngle += angleStepPerChange;
      myservo.write(truckDirectionAngle);
      Serial.println(truckDirectionAngle);
    }else{
      Serial.println("limit direction angle reached! can't go feather");
    }
    
  }else if(_direction == "l"){
    
    if(truckDirectionAngle > minAngle){
      Serial.print("Changing diection : ");
      truckDirectionAngle -= angleStepPerChange;
      myservo.write(truckDirectionAngle);
      Serial.println(truckDirectionAngle);
    }else{
      Serial.println("limit direction angle reached! can't go feather");
    }
    
  }else if(_direction == "s"){
    if(!isTruckStopped){
      isTruckStopped = true;
      Serial.println("stopped");
    }else{
      isTruckStopped = false;
      Serial.println("started");
    }
  }
}

String getInputData(String dataToExtract){
  String _data = dataToExtract.substring(2);
  _data.trim(); // Trim here modifies the strin in place and returns void
  return _data;
}
