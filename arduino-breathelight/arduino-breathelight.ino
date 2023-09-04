// rampTime is the length of one cycle in milliseconds
int rampTime = 14000;
// cycleDelay is the length of the delay at the end of the dim cycle
int cycleDelay=1000;
// this variable contains the current position in the cycle (in degrees)
float curPos = 0;
// frameRate is the number of times per second that the LED brightness will change - 
float frameRate=60;
// this calculates the number of steps per cycle
float numOfSteps=rampTime/1000*frameRate;
// this calculates the delay time between steps
float delayAmt = 1000/frameRate;
// this is the maximum brightness multiplier (range 0 to 1)
float maxBrightness = 1;
// Set 'TOP' for PWM resolution.  Assumes 16 MHz clock.
// un-comment one of the choices
const unsigned int TOP = 0xFFFF; // 16-bit resolution.   244 Hz PWM
// const unsigned int TOP = 0x7FFF; // 15-bit resolution.   488 Hz PWM
// const unsigned int TOP = 0x3FFF; // 14-bit resolution.   976 Hz PWM
// const unsigned int TOP = 0x1FFF; // 13-bit resolution.  1953 Hz PWM
// const unsigned int TOP = 0x0FFF; // 12-bit resolution.  3906 Hz PWM
// const unsigned int TOP = 0x07FF; // 11-bit resolution.  7812 Hz PWM
//  const unsigned int TOP = 0x03FF; // 10-bit resolution. 15624 Hz PWM

float degToRad(float degree)
{
  return degree * (M_PI / 180.0);
}

void PWM16Begin()
{
  // Stop Timer/Counter1
  TCCR1A = 0; // Timer/Counter1 Control Register A
  TCCR1B = 0; // Timer/Counter1 Control Register B
  TIMSK1 = 0; // Timer/Counter1 Interrupt Mask Register
  TIFR1 = 0;  // Timer/Counter1 Interrupt Flag Register
  ICR1 = TOP;
  OCR1A = 0; // Default to 0% PWM
  OCR1B = 0; // Default to 0% PWM

  // Set clock prescale to 1 for maximum PWM frequency
  TCCR1B |= (1 << CS10);

  // Set to Timer/Counter1 to Waveform Generation Mode 14: Fast PWM with TOP set by ICR1
  TCCR1A |= (1 << WGM11);
  TCCR1B |= (1 << WGM13) | (1 << WGM12);
}

void PWM16EnableA()
{
  // Enable Fast PWM on Pin 9: Set OC1A at BOTTOM and clear OC1A on OCR1A compare
  TCCR1A |= (1 << COM1A1);
  pinMode(9, OUTPUT);
}

void PWM16EnableB()
{
  // Enable Fast PWM on Pin 10: Set OC1B at BOTTOM and clear OC1B on OCR1B compare
  TCCR1A |= (1 << COM1B1);
  pinMode(10, OUTPUT);
}

inline void PWM16A(unsigned int PWMValue)
{
  OCR1A = constrain(PWMValue, 0, TOP);
}

inline void PWM16B(unsigned int PWMValue)
{
  OCR1B = constrain(PWMValue, 0, TOP);
}

void setup()
{
  Serial.begin(9600);
  PWM16Begin();

  // On the Arduino UNO T1A is Pin 9 and T1B is Pin 10

  //  PWM16A(0);  // Set initial PWM value for Pin 9
  //  PWM16EnableA();  // Turn PWM on for Pin 9

  PWM16B(0);      // Set initial PWM value for Pin 10
  PWM16EnableB(); // Turn PWM on for Pin 10
}
// this function takes a radian value and maps it to a cosine curve for a fade on-off effect.
double curveVal(float value)
{
  
  double normalizedCos = (-cos(value) + 1) / 2;
  double powerCos = pow(normalizedCos, 3) * .9999 + .0001;
  // Serial.println(powerCos);
  double retVal= powerCos*65535;
  // Serial.println(retVal);
  // Serial.println("");
  return retVal;
}

void loop()
{
  
  float normalizedDeg=curPos/numOfSteps*360;
  // Serial.println(curPos);
  // Serial.println(numOfSteps);
  float curRad = degToRad(normalizedDeg);
  // Serial.println(curveVal(curRad));
  PWM16B(curveVal(curRad));
  delay(delayAmt);
  curPos = curPos + 1;
  if (curPos > numOfSteps)
  {
    curPos = 0;
    delay(cycleDelay);
  }
}
