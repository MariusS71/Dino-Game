
#include <Wire.h> // Library for I2C communication
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header

hd44780_I2Cexp lcd;

int switchState=0;
int switchFirstPress=0;
int pauseButton=0;

int j,k;    //variabile de control ale pozitiilor cactusului, respectiv pasarii
int score=0;
int reset1=0;
int reset2=0;
unsigned long t; //pastreaza timpul ultimei deplasari a obstacolelor
unsigned long t1; // pentru deplasarea dino ului
unsigned long t2;
unsigned long t3; // timpul de punctaj pentru cactus
unsigned long t4; //timpul de punctaj pentru pasare

int objectDelay=500; //basic cactus moving speed
int objectDecrease=20; //decreases delay

bool dinoUp=0;    //dino in aer
bool dinoDown=0; //dino sa fie pe crouch

long two=0;

byte hat[8] = {
  B00000,
  B00000,
  B01110,
  B01110,
  B01110,
  B01110,
  B11111,
  B11111
};


byte bird[8] = {
  B00011,
  B11010,
  B11110,
  B01110,
  B11110,
  B10011,
  B10001,
  B10000
};


byte cactus[8] = { //cactus
  B00101,
  B10101,
  B10101,
  B10101,
  B10110,
  B01100,
  B00100,
  B00100,
};

byte dino[8] = { //dino
  B01110,
  B10111,
  B11110,
  B11111,
  B11000,
  B11111,
  B11010,
  B10010,
};

byte dinoU[8] = {
  B01110,
  B11111,
  B01110,
  B10111,
  B11111,
  B11000,
  B11111,
  B10010
};

void setup() {
pinMode(A3, INPUT);
lcd.begin(16, 2);

lcd.createChar(8, bird);
lcd.createChar(7, cactus);
lcd.createChar(6, dino);
lcd.createChar(5, hat);
lcd.createChar(4, dinoU);

j=15;
k=15+random(3,12);

t=millis();		// t tine timpul la care se misca cactusul 
t1=millis();
t2=millis();
t3=millis();
Serial.begin(9600);

switchFirstPress = analogRead(A1);
while(switchFirstPress >450) {
  for(int i=15;i>=0;i--) {
    lcd.setCursor(i, 1);
    lcd.print("PRESS TO START");
    lcd.setCursor(0, 0);
    lcd.print("DINO GAME");
    lcd.setCursor(14, 0);
    lcd.write(6);
      delay(400);
    lcd.clear();
switchFirstPress = analogRead(A1);
  if (switchFirstPress < 50) {
    break;
  }
  }
}
delay(500);
}


void loop() {
switchState = analogRead(A1);

//pause 
pauseButton=analogRead(A0);
if(pauseButton<400){
    while(switchState <600 ) {
        switchState = analogRead(A0);
        lcd.setCursor(6,1);
        lcd.print("Pause");
  }
}




//se ocupa cu movement ul cactus + bird
if((millis()-t)>=objectDelay) { //objectDelay- timpul la care se muta la stg cactusul + bird
  t=millis();			// seteaza noul t de referinta
  lcd.clear();	
  
  
  if(two==0){     //un cactus
  lcd.setCursor(j, 1);
  lcd.write(7);
  j=j-1;
    if(j == -1) {
      j=15;
//buclele de acest tip asigura faptul ca intre pasare si cactus va fi cel putin un spatiu liber si nu se vor genera pe acceasi pozitie
      while(j<=k && j>=k-2){    
        j=j+1;
      }
      two=random(0,2);
    }
  }else if(two==1){   // doi cactusi
    lcd.setCursor(j, 1);
    lcd.write(7);
    lcd.setCursor(j+1, 1);
    lcd.write(7);

    j=j-1;
    
    if(j == -1) {
      j=15;
      while(j<=k && j>=k-3){
        j=j+1;
      }
      two=random(0,2);
    }
  }

    
  lcd.setCursor(6, 0);
  lcd.print("Score:");
  lcd.print(score);

  lcd.setCursor(k, 0);
  lcd.write(8);
  k=k-1;
    if(k == -1) {
      k=15+random(1,13);
      if(two==0){
        while(k<=j+1 && k>=j-1){
          k=k+1;
        }
      }
      else if(two==1){
        while(k<=j+2 && k>=j-1){
          k=k+1;
        }
      }
    }
}



// se ocupa cu pozitia dino ului
if((millis()-t1)>=50) { // tipul la care se face refresh ul (50ms) dino ului
  t1=millis();			

  if(dinoDown==false && dinoUp==false && switchState>600){  //daca nu e in aer, nu e pe crouch dar a primit comanda de crouch
    lcd.setCursor(2,0);
    lcd.write(" ");
    lcd.setCursor(2, 1);
    lcd.write(4);
    dinoDown=true;
  }
  else if(switchState<600)  //daca nu trebuie sa fie pe crouch modificam variabila de crouch(modificarea pozitiei se va face mai jos)
    dinoDown=false;
  else if(dinoDown==true){  //daca variabila de crouch e activa il desenam tot pe crouch
    lcd.setCursor(2, 1);
    lcd.write(4);
  }
  

//daca dino e jos, dar e apasat butonul, il desenam sus si t2 incepe sa numere cat trebuie sa fie dino in aer
  if (switchState<50 && dinoUp == false) {
    lcd.setCursor(2, 0);
    lcd.write(4);
    dinoUp = true; 
    t2=millis();
  }
  else if(dinoUp == true) {	//daca dino inca trebuie sa fie in aer il desenam tot in aer
    lcd.setCursor(2, 0);
    lcd.write(4);
  }
  else if(dinoUp == false && dinoDown==false) {	//daca dino trebuie sa fie jos, il desenam jos
    lcd.setCursor(2, 0);
    lcd.write(5);	
    lcd.setCursor(2, 1);
    lcd.write(6);
  }
}

//verifica daca a trecut timpul pe care dino il petrece in aer, ca sa modifice starea de sus ( = 3 * delay ul obiectelor)
if((millis()-t2)>=objectDelay*3 && dinoUp == true) {
  t2=millis();
  lcd.setCursor(2, 1);
  lcd.write(6);
  lcd.setCursor(2, 0);
  lcd.write(5);
  dinoUp = false;
}


//se ocupa cu intersectia intre cactus si a dino
if(((millis()-t3)>=1001) && ((j == 1 && two==0)||(two==1 &&(j==1 || j+1==1)))) {  // daca cactusul ajunge pe pozitia 1 (t3 - delay ul, cat trece cactusul prin pozitia 1
// ca sa nu intre in if de mai multe ori 
 t3=millis();
  if (dinoUp == true) {   //daca dino e sus, inseamna ca a evitat cactusul
    score=score+1;
    if (score%2 == 0) {			// creste viteza cu objectDecrease din 2 in 2 puncte
    objectDelay=objectDelay-objectDecrease;
      if(objectDelay <= 200) { //makes 200 highest speed 
        objectDecrease=0; //keeps cactus/object at max speed and wont keep increasing it
      }
    }
  }
  else {	
    reset1=1;   // notam in variabila de reset ciocnirea de cactus
  }
}

//se ocupa cu intersectia intre bird si a dino
if(((millis()-t4)>=1001) && k==1) {  // daca bird ajunge pe pozitia 1 (t4 - delay ul, cat trece bird prin pozitia 1
// ca sa nu intre in if de mai multe ori 
 t4=millis();
  if (dinoDown == true) {   //daca dino e pe crouch, inseamna ca a evitat pasarea
    score=score+1;
    if (score%2 == 0) {      // creste viteza cu objectDecrease din 2 in 2 puncte
    objectDelay=objectDelay-objectDecrease;
      if(objectDelay <= 200) { //makes 200 highest speed 
        objectDecrease=0; //keeps cactus/object at max speed and wont keep increasing it
      }
    }
  }
  else {  
    reset2=1;     // notam in variabila de reset ciocnirea de pasare
  }
}

if(reset1 == 1 || reset2 == 1){//resteaza nivel
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("GAME OVER!");
    lcd.setCursor(0, 1);
    lcd.print("TAP TO RETRY");
    delay(1500);
    score=0;
    j=15;
    k=15+random(5,12);
    reset1=0;
    reset2=0;
    objectDelay=500; //toggles back to basic speed
    switchState = analogRead(A1);
      while(switchState >450 ) {
        switchState = analogRead(A1);
      }
      delay(500);
}

}
