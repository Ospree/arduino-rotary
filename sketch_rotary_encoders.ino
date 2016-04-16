
/*
 *     Rotary encoders example
 *     Tsvetelin Velkov 2016
 *     
 *     This small example reads two rotary encoders 
 *     Timer 0 is used to generate 100us update signal
 *     The encoders CLK signals are connected to pin 2 and 3 respectively
 *     Interrupt on change is used to read the encoders
 *     Debouncing time is 2ms
 * 
 */

// HW pins assignment
// Encoder A pin  CLK  to Arduino  D2
//                DT   to          D9
//                SW   to          D8
// Encoder B pin  CLK  to Arduino  D3
//                DT   to          D11
//                SW   to          D10
// Both enc. pin  +    to          +5V (VCC)
//                GND  to          GND
enum PinAssignments {
  enAPinA = 2,
  enAPinB = 9,
  enAbutton = 8,
  enBPinA = 3,
  enBPinB = 11,
  enBbutton = 10
};

volatile unsigned int ticks = 0;
boolean A_set = false;
boolean B_set = false;

volatile int encoderPosA = 0;
volatile int encoderPosB = 0;

volatile unsigned int d_potA = 0;
volatile unsigned int d_potB = 0;

unsigned int buttonA = 0;
unsigned int buttonB = 0;

int potA = 0;
int potB = 0;

void setup() {
  
  // set pins for encoder 1
  pinMode(enAPinA, INPUT);
  pinMode(enAPinB, INPUT);
  pinMode(enAbutton, INPUT);
  digitalWrite(enAPinA, HIGH);    // turn on pullup resistor
  digitalWrite(enAPinB, HIGH);    // turn on pullup resistor
  digitalWrite(enAbutton, HIGH);  // turn on pullup resistor
  
  // set pins for encoder 2
  pinMode(enBPinA, INPUT);
  pinMode(enBPinB, INPUT);
  pinMode(enBbutton, INPUT);
  digitalWrite(enBPinA, HIGH);    // turn on pullup resistor
  digitalWrite(enBPinB, HIGH);    // turn on pullup resistor
  digitalWrite(enBbutton, HIGH);  // turn on pullup resistor
  
  //stop interrupts
  cli();

  //set timer0 interrupt at 10kHz or 100us
  TCCR0A = 0;// set entire TCCR0A register to 0
  TCCR0B = 0;// same for TCCR0B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 10khz increments
  OCR0A = 24; // = ((16*10^6) / (10000*64)) - 1 (must be <256!)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (1 << CS01) | (1 << CS00);   
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);
  
  //allow interrupts
  sei();
  
  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(0, doEncoderA, RISING);

  // encoder pin on interrupt 1 (pin 3)
  attachInterrupt(1, doEncoderB, RISING);

  // Initialize the UART, 8n1 57600 baud
  Serial.begin(57600);

  // Hello world message
  Serial.println("Velkov 2016, encoders example started!");
}


void loop(){ 

  // debouncing, encoder is read >2ms after activity
  if((d_potA != 0) && ((ticks - d_potA) >= 20)){
    
    A_set = digitalRead(enAPinA) == HIGH;
    B_set = digitalRead(enAPinB) == HIGH;

    // and adjust counter + if A leads B
    encoderPosA += (A_set != B_set) ? +1 : -1;

    d_potA=0;
  }

  // debouncing, encoder is read >2ms after activity
  if((d_potB != 0) && ((ticks - d_potB) >= 20)){
    
    A_set = digitalRead(enBPinA) == HIGH;
    B_set = digitalRead(enBPinB) == HIGH;

    // and adjust counter + if A leads B
    encoderPosB += (A_set != B_set) ? +1 : -1;

    d_potB=0;
  }
  
  // adjust the pot position every 100ms
  if ((ticks >= 1000) && ((encoderPosA != 0) || (encoderPosB != 0))){

    cli();
    
    if (encoderPosA>0) potA ++;
    if (encoderPosA<0) potA --;
    if (buttonA>0) potA = 0;

    if (encoderPosB>0) potB ++;
    if (encoderPosB<0) potB --;
    if (buttonB>0) potB = 0;
   
    sei();

    encoderPosA = 0;
    buttonA=0;

    encoderPosB = 0;
    buttonB=0;
    
    Serial.print("Pot A = ");
    Serial.println(potA, DEC);

    Serial.print("Pot B = ");
    Serial.println(potB, DEC);

    ticks = 0;
  }
  
  if (digitalRead(enAbutton) == LOW){
    buttonA ++;
  }

  if (digitalRead(enBbutton) == LOW){
    buttonB ++;
  }

}

//timer0 interrupt 10kHz or 100us, this is the encoder clock 
ISR(TIMER0_COMPA_vect){
  ticks++;
}

// Interrupt on A (pin2) changing state
void doEncoderA(){
  d_potA = ticks;
}

// Interrupt on B (pin3) changing state
void doEncoderB(){
  d_potB = ticks;
}



