
#include <Arduino.h>

#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

#include <menu.h>//menu macros and objects
#include <menuIO/PCF8574Out.h>//arduino I2C LCD
#include <menuIO/serialIn.h>

#include <Keypad.h>
#include<CountUpDownTimer.h>
#include <TM1638.h>
#include <ButtonDebounce.h>
//#include <PololuLedStrip.h>
#include <NeoPixelBus.h>

#include <MemoryFree.h>

#include <ArduboyTones.h>

#define LED_CLK 12
#define LED_DIO 13
#define LED_STB 11

#define KP_C_1 38
#define KP_C_2 40
#define KP_C_3 42
#define KP_C_4 44

#define KP_R_1 46
#define KP_R_2 48
#define KP_R_3 50
#define KP_R_4 52

TM1638 ledDisplay(LED_DIO, LED_CLK, LED_STB);
char ledDisplayOutput[16];
char lcdLineOutput[24];

// Settings
#define NameText "Game Prop - 060218"
#define ByText "PyroSpud"
#define FastFlashDelay 250

// Pins
#define RedButtonPin 53
#define BlueButtonPin 51
#define LedStripPin 10
#define LedStripLength 3

// Magic Numbers
#define NO_TEAM 0
#define RED_TEAM 1
#define BLUE_TEAM 2

#define GAME_DOMINATION 0x01
#define GAME_BOMB 0x02
#define GAME_PINBOMB 0x03
#define GAME_DEADMANSWITCH 0x04

// KEYPAD INIT

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {KP_R_1, KP_R_2, KP_R_3, KP_R_4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {KP_C_1, KP_C_2, KP_C_3, KP_C_4}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// END KEYPAD

using namespace Menu;

LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// Timer, Used for all games
CountUpDownTimer MainTimer(DOWN);

ButtonDebounce redButton(RedButtonPin, 100);
ButtonDebounce blueButton(BlueButtonPin, 100);

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(LedStripLength, LedStripPin);

bool SoundEnabled(){ return true; }
ArduboyTones sound(SoundEnabled);

bool inGame = false;
byte currentGame = 0x00;
bool currentlyPlaying = false;


void SetLedStripRGB(int r, int g, int b)
{
      strip.SetPixelColor(0, RgbColor(r, b, g));
      strip.SetPixelColor(1, RgbColor(r, b, g));
      strip.SetPixelColor(2, RgbColor(r, b, g));
      strip.Show();
}

// Domination
int dominationTotalTimeSeconds = 2700;
int dominationTotalTimeMins = 45;
int dominationCurrentTeam = 0;


CountUpDownTimer DominationRedTimer(UP);
CountUpDownTimer DominationBlueTimer(UP);

result startDomination(eventMask e,navNode& nav, prompt &item) {
  dominationTotalTimeSeconds = dominationTotalTimeMins * 60;

  MainTimer.SetTimer(0,0,dominationTotalTimeSeconds);
  MainTimer.StartTimer();

  inGame = true;
  currentGame = GAME_DOMINATION;
  
  lcd.clear();

  DominationRedTimer.StartTimer();
  DominationBlueTimer.StartTimer();
  DominationRedTimer.PauseTimer();
  DominationBlueTimer.PauseTimer();

  currentlyPlaying = true;

  SetLedStripRGB(120, 120, 120);


  lcd.setCursor(0, 0);
  lcd.print("     DOMINATION     ");
  
}

void DominationLoop()
{
  
  DominationRedTimer.Timer();
  DominationBlueTimer.Timer();
  
  lcd.setCursor(0, 1);
  sprintf(lcdLineOutput,"Red:    %02ld:%02ld", DominationRedTimer.ShowMinutes(), DominationRedTimer.ShowSeconds());
  lcd.print(lcdLineOutput);
  
  lcd.setCursor(0, 2);
  sprintf(lcdLineOutput,"Blue:   %02ld:%02ld", DominationBlueTimer.ShowMinutes(), DominationBlueTimer.ShowSeconds());
  lcd.print(lcdLineOutput);  

  
  MainTimer.Timer();
  if (MainTimer.TimeHasChanged() ) // this prevents the time from being constantly shown.
  {
    sprintf(ledDisplayOutput, " %02ld  %02ld ", MainTimer.ShowMinutes(), MainTimer.ShowSeconds());
    ledDisplay.setDisplayToString(ledDisplayOutput);

    sound.tone(500,20,300,20);

    // Has Timer Ended?
    if(MainTimer.TimeCheck())
    {
        DominationEndGame();
    }
  }

  char key = keypad.getKey();
  if (key == 'B'){
    //DominationEndGame();
  }
}

void DominationEndGame()
{
  DominationRedTimer.PauseTimer();
  DominationBlueTimer.PauseTimer();

  MainTimer.StopTimer();

  currentlyPlaying = false;

  lcd.setCursor(0, 3);
  
  sound.tone(1000,80,600,50,1300,90);
  
  if(DominationRedTimer.ShowTotalSeconds() > DominationBlueTimer.ShowTotalSeconds())
  { 
    lcd.print("   RED TEAM WINS!   ");  
    while(true)
    {
      SetLedStripRGB(255, 0, 0);
      delay(FastFlashDelay);
      SetLedStripRGB(0, 0, 0);
      delay(FastFlashDelay);
    }
  }

  if(DominationRedTimer.ShowTotalSeconds() < DominationBlueTimer.ShowTotalSeconds())
  {
    lcd.print("   BLUE TEAM WINS   ");  
    while(true)
    {
      SetLedStripRGB(0, 0, 255);
      delay(FastFlashDelay);
      SetLedStripRGB(0, 0, 0);
      delay(FastFlashDelay);
    }
  }
  
  
  //currentlyPlaying = !true;
}

void DominationRedButton()
{
  if(dominationCurrentTeam != RED_TEAM && currentlyPlaying == true)
  {
    DominationRedTimer.ResumeTimer();
    DominationBlueTimer.PauseTimer();
    dominationCurrentTeam = RED_TEAM;

    SetLedStripRGB(255, 0, 0);

    sound.tone(800,50,600,50);
  }

  
}

void DominationBlueButton()
{
  if(dominationCurrentTeam != BLUE_TEAM && currentlyPlaying == true)
  {
    DominationBlueTimer.ResumeTimer();
    DominationRedTimer.PauseTimer();
    dominationCurrentTeam = BLUE_TEAM;

    SetLedStripRGB(0, 0, 255);

    sound.tone(800,50,600,50);
  }
}
// End domination


// Start Bomb
int bombStartTimeSeconds = 120;
bool bombRestartOnDisarm = false;

void startBomb()
{
  MainTimer.SetTimer(0,0,bombStartTimeSeconds);
  ledDisplay.setDisplayToString("        ");
  
  inGame = true;
  currentGame = GAME_BOMB;
 
  lcd.clear();

  currentlyPlaying = true;

  lcd.setCursor(0, 0);
  lcd.print("        BOMB        ");

  bombDisarm();
}

void bombLoop()
{
  MainTimer.Timer();
  if (MainTimer.TimeHasChanged() ) // this prevents the time from being constantly shown.
  {
    sprintf(ledDisplayOutput, " %02ld  %02ld ", MainTimer.ShowMinutes(), MainTimer.ShowSeconds());
    ledDisplay.setDisplayToString(ledDisplayOutput);

    sound.tone(500,20,300,20);

    // Has Timer Ended?
    if(MainTimer.TimeCheck())
    {
        bombEnd();
    }
  }
}

void bombArm()
{
  lcd.setCursor(0, 2);
  lcd.print("       ARMED        ");

  SetLedStripRGB(255, 0, 0);

  Serial.println(bombRestartOnDisarm);

  MainTimer.StartTimer();
}

void bombDisarm()
{
  lcd.setCursor(0, 2);
  lcd.print("      DISARMED      ");

  SetLedStripRGB(0, 255, 0); 
  
  Serial.println(bombRestartOnDisarm);
  
  if(bombRestartOnDisarm != false)
  {
    Serial.println("Restarting Timer");
    Serial.println(bombStartTimeSeconds);
    MainTimer.ResetTimer();
    MainTimer.SetTimer(0,0,bombStartTimeSeconds);
    MainTimer.ResetTimer();
    ledDisplay.setDisplayToString("        ");
  }
  
  MainTimer.StopTimer();

}

void bombEnd()
{
  
  MainTimer.StopTimer();
  ledDisplay.setDisplayToString("  BOOM  ");

  lcd.setCursor(0, 2);
  lcd.print("     !!!BOOM!!!     ");
  
  currentlyPlaying = false;

  int rounds = 0;
  
  while(true)
  {
    if(rounds < 10)
    {
      sound.tone(1800,80,1300,50,800,90);
    }
    SetLedStripRGB(255, 120, 64); 
    ledDisplay.setDisplayToString("  BOOM  ");
    lcd.setBacklight(255);
    delay(300);
    SetLedStripRGB(0, 0, 0); 
    if(rounds < 10)
    {
      sound.tone(1200,80,950,50,600,90);
    }
    ledDisplay.setDisplayToString("        ");
    lcd.setBacklight(0);
    delay(300);
    SetLedStripRGB(255, 120, 64); 
    ledDisplay.setDisplayToString("  BOOM  ");
    lcd.setBacklight(255);
    delay(300);
    SetLedStripRGB(0, 0, 0);
    ledDisplay.setDisplayToString("        ");
    lcd.setBacklight(0);
    delay(300);

    rounds++;
  }

}
// End Bomb

// Start PinBomb
int PinBomb_D1 = 9;
int PinBomb_D2 = 2;
int PinBomb_D3 = 7;
int PinBomb_D4 = 1;

result doAlert(eventMask e, prompt &item);

result showEvent(eventMask e,navNode& nav,prompt& item) {
  Serial.print("event: ");
  Serial.println(e);
  return proceed;
}



MENU(testMenu,"Test",showEvent,anyEvent,noStyle
  ,OP("Test Sounder",showEvent,anyEvent)
  ,OP("Test LEDs",showEvent,anyEvent)
  ,EXIT("<Back")
);

MENU(dominationMenu,"Domination",showEvent,anyEvent,noStyle
  ,FIELD(dominationTotalTimeMins,"Timer","Mins",5,180,5,1,doNothing,noEvent,wrapStyle)
  ,OP("Start Game",startDomination,enterEvent)
  ,EXIT("<Back")
);

TOGGLE(bombRestartOnDisarm,setBombRestartOnDisarm,"Disarm Restart: ",doNothing,noEvent,noStyle//,doExit,enterEvent,noStyle
  ,VALUE("Yes",true,doNothing,noEvent)
  ,VALUE("No",false,doNothing,noEvent)
);

MENU(bombMenu,"Bomb",showEvent,anyEvent,noStyle
  ,FIELD(bombStartTimeSeconds,"Timer","Seconds",30,600,10,5,doNothing,noEvent,wrapStyle)
  ,SUBMENU(setBombRestartOnDisarm)
  ,OP("Start Game",startBomb,enterEvent)
  ,EXIT("<Back")
);



MENU(gameMenu,"Select Game",showEvent,anyEvent,noStyle
  ,SUBMENU(dominationMenu)
  ,SUBMENU(bombMenu)
  ,EXIT("<Back")
);



MENU(mainMenu,"Main menu",doNothing,noEvent,noStyle
  ,SUBMENU(gameMenu)
  ,SUBMENU(testMenu)
  //,EXIT("<Back")
);
    //,OP("Op1",action1,anyEvent)
  //,OP("Op2",action2,enterEvent)
  //,FIELD(test,"Test","%",0,100,10,1,doNothing,noEvent,wrapStyle)

  //,OP("LED On",ledOn,enterEvent)
  //,OP("LED Off",ledOff,enterEvent)
  //,SUBMENU(chooseMenu)
  //,OP("Alert test",doAlert,enterEvent)
  //,EDIT("Hex",buf1,hexNr,doNothing,noEvent,noStyle)

#define MAX_DEPTH 4

MENU_OUTPUTS(out,MAX_DEPTH
  ,LCD_OUT(lcd,{0,0,20,4})
  ,NONE
);

serialIn serial(Serial);
NAVROOT(nav,mainMenu,MAX_DEPTH,serial,out);

result alert(menuOut& o,idleEvent e) {
  if (e==idling) {
    o.setCursor(0,0);
    o.print("alert test");
    o.setCursor(0,1);
    o.print("[select] to continue...");
  }
  return proceed;
}

result doAlert(eventMask e, prompt &item) {
  nav.idleOn(alert);
  return proceed;
}

result idle(menuOut& o,idleEvent e) {
  switch(e) {
    case idleStart:o.print("suspending menu!");break;
    case idling:o.print("suspended...");break;
    case idleEnd:o.print("resuming menu.");break;
  }
  return proceed;
}

void OnRedButton(int state)
{
  Serial.println("OnRedButton()");
  
  if(inGame && state == 0)
  {
    switch(currentGame)
    {
      case GAME_DOMINATION:
        DominationRedButton();
        break;
      case GAME_BOMB:
        bombArm();
        break;
    }
  }
}

void OnBlueButton(int state)
{
  Serial.println("OnBlueButton()");
  
  if(inGame && state == 0)
  {
    switch(currentGame)
    {
      case GAME_DOMINATION:
        DominationBlueButton();
        break;
      case GAME_BOMB:
        bombDisarm();
        break;
    }
  }
}



void setup() {
  
  // Set up IO
  pinMode(RedButtonPin, INPUT_PULLUP);
  pinMode(BlueButtonPin, INPUT_PULLUP);

  //digitalWrite(RedButtonPin, HIGH);
  //digitalWrite(BlueButonPin, HIGH);

  // Set up callbacks
  redButton.setCallback(OnRedButton);
  blueButton.setCallback(OnBlueButton);
  
 
  // Debug Serial
  Serial.begin(115200);
  while(!Serial);
  Serial.println(NameText);Serial.flush();
  lcd.begin(20,4);
  
  nav.idleTask=idle;//point a function to be used when menu is suspended
  mainMenu[1].enabled=disabledStatus;
  nav.showTitle=true;

  // Init LED Strip, Test Displays
  //PololuLedStripBase::interruptFriendly = true;
  strip.Begin();
  strip.Show();
  SetLedStripRGB(255, 255, 255);

  // Test Digits
  ledDisplay.setDisplayToString("88888888");

  sound.tone(1000,200,2000,200,4000,200);

  // Init LCD
  lcd.setBacklight(255);
  lcd.setCursor(0, 0);
  lcd.print(NameText);
  lcd.setCursor(0, 1);
  lcd.print(ByText);
  delay(2000);

  // Reset displays
  SetLedStripRGB(0, 0, 0);
  ledDisplay.clearDisplay();

  
}

void loop() {
  if(!inGame)
  {
    nav.poll();

    char key = keypad.getKey();

    // Menu Input
    if (key == 'D'){
      nav.doNav(Menu::downCmd);
    }
    else if (key == 'A'){
      nav.doNav(Menu::upCmd);
    }
    else if (key == '#'){
      nav.doNav(Menu::enterCmd);
    }
    else if (key == '*'){
      nav.doNav(Menu::escCmd);
    }
  }else{
    // We are in game, so now run game loops
    switch(currentGame)
    {
      case GAME_DOMINATION:
        DominationLoop();
        break;
      case GAME_BOMB:
        bombLoop();
        break;
    }
    
  }
  
  redButton.update();
  blueButton.update();

  //Serial.print("freeMemory()=");
  //Serial.println(bombRestartOnDisarm);
}
