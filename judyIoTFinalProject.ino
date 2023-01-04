
#include <Stepper.h>
#include <Servo.h>

//definitions for each command to be recieved via serial port
#define COMMAND_FORWARD 'f'
#define COMMAND_BACK 'b'
#define COMMAND_STOP 's'

//enter the steps per rev for your motors here
int stepsInRev = 200;
char truckMoving = 'f';
bool isTruckStopped = true;

// setup pins for each driver motor1 ~ IN1, IN2, IN3, IN4; motor2 ~ IN1, IN2, IN3, IN4
Stepper truckMotor(stepsInRev,  8, 9, 10, 11);
Servo myservo; 

//to move the motors forward
void forwardStep(){
  Serial.println("forward");
  if(!isTruckStopped){
    truckMotor.step(stepsInRev);
  }else{
    isTruckStopped = false;
  }
}

// to move the motors back
void backwardStep(){
  Serial.println("backward");
  if(!isTruckStopped){
    truckMotor.step(-stepsInRev);
  }else{
    isTruckStopped = false;
  }
}

// to move the motors in opposite directions (left)
void leftStep(){
  backwardStep();
}

// to move the motors in opposite directions (right)
void rightStep(){
  //for (int pos = 0; pos <= 180; pos++) { // goes from 0 degrees to 180 degrees
    //myservo.write(pos);              // tell servo to go to position in variable 'pos'
  //}
  Serial.println("Right");
  myservo.write(0); 
  delay(1000); 
  myservo.write(180);
}

String getInputData(String dataToExtract){
  String input = new String(dataToExtract);
  input = dataToExtract.trim();
  return input.substring(2);
}

// to power down the motor drivers and stop the motors
void allStop(){
  Serial.println("stop");
  if(!isTruckStopped){
    isTruckStopped = true;
  }
}
void setup() {
  Serial.begin(9600);//start the bluetooth serial port - send and recieve at 9600 baud
  // set the speed at 60 rpm:
  truckMotor.setSpeed(60);
   myservo.attach(7);
}



void loop() {
  
  //check to see if there is serial communication and if so read the data
  String input = Serial.readString();
  if(input.startsWith("m-")){
    truckMoving = getInputData(input);
    Serial.print("Moving side input given: ");
    Serial.println(truckMoving);
  }else if(input.startsWith("d-")){
    String _angle = getInputData(input);
    truckDirectionAngle = _angle.toInt();
    Serial.print("Direction input given: ");
    Serial.println(truckDirectionAngle);
  }

    switch(truckMoving) {
      case COMMAND_FORWARD:
        forwardStep();
        break; 
      case COMMAND_BACK:
        backwardStep();
        break;
      case COMMAND_STOP:
        allStop();
        break;
    }

}
