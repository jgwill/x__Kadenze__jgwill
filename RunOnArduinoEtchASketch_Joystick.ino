

/*  Etch a Sketch Feature Extractor - Arduino Sketch
 * By J.Guillaume D.-Isabelle, 2021
 *
 * @STATUS Getting Better, even though it has interesting code
 * I think that what I would like to feel is the speed 
 * at which I am turning the wheel. How can I achieve that ?
 * 
 * 
2 Joysticks : 1 for moving up down, 1 for left - right
1 distance mesurement for Pen size
1 button for something
*/

int outValBase = 1; //The value we send as output

int j2pinX = A3;
int j2pinY = A4;

int j1pinX = A2;
int j1pinY = A1;

int s3 = 0 ; //a temp value for other signal
int distance = 3; // temp value before we implement distance mesurement
int distance2 = 0; 

void setup() {
  // put your setup code here, to run once:

 Serial.begin(9600); //Begin Serial Communication with a baud rate of 9600
}
//Remapping range
int mapValue = 64;
//A Range where we are neutral
int rangeValue = mapValue / 2;

int mapXmin = mapValue * -1;
int mapXmax = mapValue;
int mapYmin = mapValue * -1;
int mapYmax = mapValue;
int minRangeNeutral = rangeValue * -1;
int maxRangeNeutral = rangeValue;
boolean remapping = true;
//keep track of previous move
int j1lX = 0;
int j1lY = 0;
int j2lX = 0;
int j2lY = 0;

int v1 = 0;
int v2 = 0;
int pv1 = 0;
int pv2 = 0;

boolean moving = true;
boolean firstRun = true; 

void loop() {
  // put your main code here, to run repeatedly:

  
  int j1valX = 
      analogRead(j1pinX) ; 
      
  int j1valY = 
      analogRead(j1pinY) ; 
  
  int j2valX = 
      analogRead(j2pinX) ; 
      
  int j2valY = 
      analogRead(j2pinY) ; 

//Remapping ??
      if (remapping)
      {
        
         j1valX = map(
            j1valX,0,1023,mapXmin,mapXmax
             );  
         j1valY = map(
            j1valY,0,1023,mapXmin,mapXmax
             );
         j2valX = map(
            j2valX,0,1023,mapXmin,mapXmax
             );  
         j2valY = map(
            j2valY,0,1023,mapXmin,mapXmax
             );

      
      }

  
        
        //Create a neutral value for very low value (it oscillate a bit)
  //if (j1valX > minRangeNeutral && j1valX < maxRangeNeutral) j1valX = 0;
  //else if (j1valX > rangeValue) j1valX = 1;
  //else j1valX = -1;
  j1valX = mapRange(j1valX);
  j2valX = mapRange(j2valX);
  j1valY = mapRange(j1valY);
  j2valY = mapRange(j2valY);
  //Now all value are map to 1,0,-1
  
  
  if (false)
    printDebug(j1valX,j1valY,j2valX,j2valY);
  
    v1 = parseRange(j1valX,j1lX,j1valY,j1lY);
 
    v2 = parseRange(j2valX,j2lX,j2valY,j2lY);
  
  
  if (j1valX ==  j1lX && j1valY == j1lY && j2valX == j2lX && j2valY == j2lY  )
    {  
       
      moving=false; 
    } 
  else moving = true;
    
    
  //  Serial.print("v1:");Serial.print(v1);Serial.print(", pv1:");
   // Serial.println ( pv1);

  
  if (moving || firstRun)
  {
    firstRun = false;
    sendData(v1,j1valX,j1valY,s3,v2,j2valX,j2valY);
     delay(1);
   //  sendData(0,0,0,0,s3,distance,distance2);
    moving = false;
  
  }
  //Store our last data for the next loop
  j1lX = j1valX;
  j2lX = j2valX;
  j1lY = j1valY;
  j2lY = j2valY;

   
  pv1 = v1;
  pv2 = v2;

if (!moving)
  delay(5);
  else delay(1);

//send resets
  

}

void sendData(int v1,int v2,int v3,int v4,int v5,int v6,int v7)
{
    Serial.print(v1, DEC); 
    Serial.print(",");
    Serial.print(v2, DEC); 
    Serial.print(",");
    Serial.print(v3, DEC); 
    Serial.print(",");
    Serial.print(v4, DEC); 
    Serial.print(",");
    Serial.print(v5, DEC); 
    Serial.print(",");
    Serial.print(v6, DEC); 
    Serial.print(",");
    Serial.print(v7, DEC); 
    Serial.println();
}
void printDebug(int v1,int v2,int v3,int v4)
{
  
    Serial.println();
    Serial.print(v1, DEC); 
    Serial.print(",");
    Serial.print(v2, DEC); 
    Serial.print(",");
    Serial.print(v3, DEC); 
    Serial.print(",");
    Serial.print(v4, DEC); 
    Serial.print(" - ");
}

int mapRange(int _v)
{


    if (_v > minRangeNeutral && _v < maxRangeNeutral) return 0;
     else 
     if (_v > rangeValue) return 1;
      else return -1;

}

int parseRange(int vx,int px,int vy,int py)
{
    /*
    Possible sequence   for direction 1:
    1,0
    1,1
    0,1
    -1,1
    -1,0
    -1,-1
    0,-1
    1,-1
    */
    if (vx == 1 && vy == 0 && px== 1 && py == -1) return outValBase;
    else 
    if (vx == 1 && vy == 0 && px== 1 && py == 1) return outValBase * -1;
    
    if (vx == 1 && vy == 1 && px== 1 && py == 0) return outValBase;
    else
    if (vx == 1 && vy == 1 && px== 0 && py == 1) return outValBase * -1;
    
    if (vx == 0 && vy == 1 && px== 1 && py == 1) return outValBase;
    else
    if (vx == 0 && vy == 1 && px== -1 && py == 1) return outValBase * -1;
    
    if (vx == -1 && vy == 1 && px== 0 && py == 1) return outValBase;
    else
    if (vx == -1 && vy == 1 && px== -1 && py == 0) return outValBase * -1;
    
    if (vx == -1 && vy == 0 && px== -1 && py == 1) return outValBase;
    else
    if (vx == -1 && vy == 0 && px== -1 && py == -1) return outValBase * -1;
    
    if (vx == -1 && vy == -1 && px== -1 && py == 0) return outValBase;
    else
    if (vx == -1 && vy == -1 && px== 0 && py == -1) return outValBase * -1;
    
    if (vx == 0 && vy == -1 && px== -1 && py == -1) return outValBase;
    else
    if (vx == 0 && vy == -1 && px== 1 && py == -1) return outValBase * -1;

    
    if (vx == 1 && vy == -1 && px== 1 && py == 0) return outValBase * -1;
    else
    if (vx == 1 && vy == -1 && px== 0 && py == -1) return outValBase;
    
    
    
    
    
    

    return 0;
    

}
