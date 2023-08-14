// rampTime is the length of one cycle in milliseconds
int rampTime=1000;


int curPos=0;
int curDirection=1;
int delayAmt=rampTime/255;
int curLevel=0;
void setup() {
pinMode(13,OUTPUT);
}

int curveVal(int value){
  return value*value/255;

}



void loop() {
curLevel=curveVal(curPos);
analogWrite(13, curLevel);
delay(delayAmt);
curPos=curPos+curDirection;
if (curPos==255){curDirection=-1;}
if (curPos==0){curDirection=1;}
}
