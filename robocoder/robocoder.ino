//Servo
const int servoPin  = 13;
#define Left 2500
#define Centre 1500
#define Right 500
unsigned int servoPos;
long int j;

//Sensor Readings
unsigned char x;  

//Motors
const int leftMotor[2] = {10,9};
const int rightMotor[2] = {6,5};
const unsigned char leftFS = 255;  //left full speed - to allow for alignment
const unsigned char rightFS = 255; //right full speed


char state;  //0- At Start Line
             //1- Look for 1st Stop line
             //2- Push through Stop Line
             //3- Look for 2nd Stop Line
             //4- Push through Stop line
             //5- Look for Finishing Line

unsigned long int currentMillis, previousMillis=0;
unsigned long int loopCount;


void setup(){  
  Serial.begin(9600);
  //Line sensors setup
  pinMode(A1,INPUT);
  pinMode(A2,INPUT); 
  pinMode(A3,INPUT);
  pinMode(A4,INPUT);
  pinMode(A5,INPUT);
  x = 0b00011111;

  //Motor Setup
  pinMode(leftMotor[0],OUTPUT);
  pinMode(leftMotor[1],OUTPUT);
  pinMode(rightMotor[0],OUTPUT);
  pinMode(rightMotor[1],OUTPUT);

  //Servo Setup
  pinMode(servoPin, OUTPUT);

  //Global Variables
  state = 0;   
  servoPos = Centre;
   
  //Serial.begin(115200);
}

void loop() {
  
  //Read Linesensors
    bitWrite(x,0,digitalRead(A1));  //OUT5 of Linesensor
    bitWrite(x,1,digitalRead(A2));  //OUT4
    bitWrite(x,2,digitalRead(A3));  //OUT3
    bitWrite(x,3,digitalRead(A4));  //OUT2
    bitWrite(x,4,digitalRead(A5));  //OUT1


  //Send pulse to servo every 20ms
   currentMillis = millis();
    if (currentMillis - previousMillis >= 18) 
    {
      previousMillis = currentMillis; // save the last time you send pulse
      sendPulse(servoPos);
    }

  switch(state){
  case 0:  //at start line 
    if (x == 0b00000000)   // see long black START line 
      {
       x = 0b00011011;  //move robot forward
      }
    else{  
        state = 1;
        loopCount =0;      
    }
    break;

  case 1:  //look for the first stop line
    if (x == 0b00000000)   // see long black FIRST line 
      {
      moveRobot (255, 255, 255, 255);  //stop robot
      loopCount++;
      Serial.println(loopCount);
      if (loopCount <= 100) //loop, for servo to stabalize
        servoPos = Left;  //first 20000 loops servo in left pos. 
      else if (loopCount ==200) //after another 10000 loops, move on. 
        state = 2; 
      else //loop for servo to stabalize
        servoPos = Centre; //next 20000 loops, servo in centre
      }
     break;
     
  case 2:
     if (x == 0b00000000)   // still first long black line 
      {
       x = 0b00011011;  //move robot forward to Exit 
      }
    else{  
        state = 3;
        loopCount=0;
        }       //out of black line
     break;

   case 3: 
    if (x == 0b00000000)   // see long black SECOND line 
      {
      moveRobot (255, 255, 255, 255);  //stop robot
      loopCount++;
      if (loopCount <= 2000) //loop, for servo to stabalize
        servoPos = Right;  //first 20000 loops servo in right pos. 
      else if (loopCount <4000)//loop for servo to stabalize
        servoPos = Right; //next 20000 loops, servo in centre
      else if (loopCount ==5000) //after another 10000 loops, move on. 
         state = 4;        
      }
     break;
     
   case 4:
     if (x == 0b00000000)   // still first long black line 
      {
       x = 0b00011011;  //move robot forward to Exit 
      }
    else{  
        state = 5;
        loopCount=0;
        }       //out of black line
     break;

   case 5: 
   if (x == 0b00000000)   // see long black FIRST line 
      {
      moveRobot (255, 255, 255, 255);  //stop robot
      }
   else {
   }     
   break;
  
   default:  break;

  }//end switch
  
  
  switch (x){
  case 0b00000000:moveRobot(255,255,255,255); break;     //0  - brake
  case 0b00011111:moveRobot(255,0,245,0); break; //31    //coast
  
  case 0b00000001:moveRobot(0,0,255,0); break; //1
  case 0b00000011:moveRobot(0,0,255,0); break; //3
  case 0b00000111:moveRobot(0,0,255,0); break; //7
  case 0b00001111:moveRobot(0,0,255,0); break; //15

  case 0b00011011:moveRobot(255,0,245,0);break;  //27
  case 0b00010001:moveRobot(255,0,255,0); break; //17
  case 0b00011001:moveRobot(255,0,150,0);break;  //25
  case 0b00010011:moveRobot(150,0,255,0); break; //19
  case 0b00011101:moveRobot(255,0,50,0);break;  //29
  case 0b00010111:moveRobot(50,0,255,0);break;//23
  
  case 0b00010000:moveRobot(255,0,0,0); break; //16
  case 0b00011000:moveRobot(255,0,0,0);break; //24
  case 0b00011100:moveRobot(255,0,0,0);break;  //28
  case 0b00011110:moveRobot(255,0,0,0);break;  //30
  
  default: break;  //scan again
  } //switch
}

//send Servo Pulse
void sendPulse(long int k)
{
  digitalWrite(servoPin, HIGH);
  delayMicroseconds (k);
  digitalWrite (servoPin, LOW);
}
  
//Motor Control
void moveRobot (char leftPWM0, char leftPWM1, char rightPWM0, char rightPWM1){
  analogWrite(leftMotor[0], leftPWM0);
  analogWrite (leftMotor[1], leftPWM1); 
  analogWrite (rightMotor[0],rightPWM0);
  analogWrite (rightMotor[1],rightPWM1);
}