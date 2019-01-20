//D0-D7 ADC
//D8 lcd
//D9 ADC clock
//D10 lcd
//D11 lcd
//D12 lcd
//D13 lcd
//A0 ADC второй канал 
//A1 Измерение опорнгого напряжения
//A2   Free
//A3 encoder
//A4 encoder
//A5 encoder button
//A6   Free
//A7 Измерение смещения земли

uint8_t MyBuff_ADC[256];
uint8_t MyBuff_AI0[256];
uint8_t AI; 
 byte old_ADCSRA;
 byte old_ADMUX;
 
#include "ST7565.h"
#include "ADCRead.h"
#include <SPI.h>


ST7565 glcd(11, 13, 8, 12, 10); //Екран Налаштунок екрана

int EncoderGo = 0;                //Меню. Номер активного меню
int StartPoint = 0;            //Синхронізаія Зсув, як результат синхронізації
int LastPoint = 0;            //Остання точка
byte CountPoint = 0;            //Кылькысть точок
int SyncVal = 127+20;           //Синхронізаія Значення синхронызацыъ
int Tinc = 20;
long  Fr=0;

#define Epin_A  A4              //Енкодер
#define Epin_B  A3              //Енкодер
#define Epin_SW A5              //Енкодер Кнопка
     

byte NumberMenu ;               //Меню. Поточне меню
int MeasureDelay = 3;           //Меню. Частота вибырки
byte Scale = 2 ;                //Меню. Масштаб /  0, 2, 4
int  Bias = 0 ;                 //Меню. Змыщення по осі У
int BiasAI0 = 0;
int NullPoint = 128;            //Меню. Нульова точка
int Uref     ;                   //опорна напруга
int UrefD;
int GroundValue    ;             //Зміщення землі

byte FlagSyncOk = true;         //АЦП закінчило зчитування
byte ADCGoes = true;            //АЦП в процесі зчитування
byte N =0;
unsigned long TimeClickB;  // Коли клыкнули кнопкою

void testdrawchar(void) {
  for (int i=0; i < 168; i++) {
    glcd.drawchar((i % 21) * 6, i/21, i+127);
  }    
}


void setup()   {
  SPI.begin();
  glcd.begin(12);
  glcd.display(); delay(500); glcd.clear();
  glcd.drawstringS(0, 0, "Memory= " + String(freeRam())) ;
  glcd.display();
  delay(1000);
//  glcd.clear();
//  testdrawchar();
//  glcd.display();
//  delay(100000);
  



  PORTC|=(1<<PC3)|(1<<PC4)|(1<<PC5); //подтяжка кнопок на A3, A4 , A5
  DDRB |= (1<<PB1); // Настройка D9  на вихід  9-8=1
//  ADMUX =  (0 << REFS1) | (0 << REFS0) ; // Зовнішня опорна напруга
  
  ADCSRA = (1 << ADEN) | (1 << ADSC) | (0 << ADATE) | (0 << ADIF) | (0 << ADIE) | (0 << ADPS2) | (1 << ADPS1) | (0 << ADPS0);
  ADMUX =  (0 << REFS1) | (0 << REFS0) | (1 << ADLAR)  | (0 << MUX3) | (0 << MUX2)  | (0 << MUX1)  | (0 << MUX0) ; 

//Енкодер включити переривання
  PCICR = 1 << PCIE1; //разрешить пренрывание
  PCMSK1 = B00111000 ; //(1<<PCINT12)|(1<<PCINT11); //выбрать входы
  //PCMSK1=(1<<PCINT9)|(1<<PCINT10)|(1<<PCINT11); //выбор пинов прерывания (9-A1, 10-A2, 11-A3)
  //    B00000000
  //     76543210
  //PCINT14   PCINT13   PCINT12   PCINT11   PCINT10   PCINT9    PCINT8
  

//Таймер ногодриг для АЦП
  TCCR1A=0b01000000; //подключить выход OC1A первого таймера
  TCCR1B=0b00001001;//

}

void loop()
{   

    if (FlagSyncOk)  {ExecEncoder(); glcd.clear(); synchronization();  DrawChart();
        ReadRefValue (); scanframe ();
        DrawGrid();
        DravValue();
        glcd.display();
        
        };
  
};

void stretchChart(byte M)
{
             for (int k = 0; k <= M; k++)
             {
                    for (int i = 254; i >= 1; i-=2)
                     {
                      MyBuff_ADC[i]= MyBuff_ADC[i/2];
                    
                     }  
                    for (int i = 253; i >= 1; i-=2)
                     {
                      MyBuff_ADC[i]= (MyBuff_ADC[i-1]+MyBuff_ADC[i+1])/2;
                    
                     } 
             } 
                                         
}

void setADCSRA () {
        TCCR1A=0b00000000;   
        FlagSyncOk=false;      N=0;
        ADCSRA = (1 << ADEN) | (1 << ADSC) | (0 << ADATE) | (0 << ADIF) | (0 << ADIE) | (0 << ADPS2) | (1 << ADPS1) | (0 << ADPS0);
        ADMUX =  (0 << REFS1) | (0 << REFS0) | (1 << ADLAR)  | (0 << MUX3) | (0 << MUX2)  | (0 << MUX1)  | (0 << MUX0) ; 
//        PORTB  = PORTB  | 1<<1;  PORTB = PORTB & ~(1<<1);
//        PORTB  = PORTB  | 1<<1;  PORTB = PORTB & ~(1<<1);
//        PORTB  = PORTB  | 1<<1;  PORTB = PORTB & ~(1<<1);
//        PORTB  = PORTB  | 1<<1;  PORTB = PORTB & ~(1<<1); 
  }


void scanframe ()
{

 switch (MeasureDelay) {
          case 0: { TCCR1A=0b01000000;  noInterrupts(); LoadPin4();  interrupts();  stretchChart(2);  FlagSyncOk=true; break; }  
          case 1: { TCCR1A=0b01000000;  noInterrupts(); LoadPin4();  interrupts();  stretchChart(1);  FlagSyncOk=true;  break;}  
          case 2: { TCCR1A=0b01000000;  noInterrupts(); LoadPin4();  interrupts();  stretchChart(0);  FlagSyncOk=true;   break;}
          case 3: { TCCR1A=0b01000000;  noInterrupts(); LoadPin4();  interrupts();  FlagSyncOk=true;  break;}
          case 4: { TCCR1A=0b01000000;  noInterrupts(); LoadPin8();  interrupts();  FlagSyncOk=true;  break;}
          case 5: { TCCR1A=0b01000000;  noInterrupts(); LoadPin16(); interrupts();  FlagSyncOk=true;  break;}
          case 6: { noInterrupts(); setADCSRA (); LoadPin32();  interrupts();  FlagSyncOk=true;  break;}
          case 7: { noInterrupts(); setADCSRA ();  LoadPin64();  interrupts();  FlagSyncOk=true;  break;}                                        
          case 8: {  setADCSRA ();  TCCR2A=0;  TCCR2B=(0<<CS22)|(0<<CS21)|(1<<CS20);  OCR2A=100;   TIMSK2=(1<<OCIE2A);    break;}
          case 9: {  setADCSRA ();  TCCR2A=0;  TCCR2B=(0<<CS22)|(0<<CS21)|(1<<CS20);  OCR2A=227;  TIMSK2=(1<<OCIE2A);  break;}   
          case 10: { setADCSRA ();  TCCR2A=0;  TCCR2B=(0<<CS22)|(1<<CS21)|(0<<CS20);  OCR2A=61;  TIMSK2=(1<<OCIE2A); break;}   
          case 11: { setADCSRA ();  TCCR2A=0;  TCCR2B=(0<<CS22)|(1<<CS21)|(0<<CS20);   OCR2A=127; TIMSK2=(1<<OCIE2A);    break;}                               
          case 12: { setADCSRA ();  TCCR2A=0;  TCCR2B=(0<<CS22)|(1<<CS21)|(0<<CS20);  OCR2A=255;  TIMSK2=(1<<OCIE2A); break;}    
          case 13: { setADCSRA ();  TCCR2A=0;  TCCR2B=(0<<CS22)|(1<<CS21)|(1<<CS20);  OCR2A=128;  TIMSK2=(1<<OCIE2A);     break;}  
          case 14: { setADCSRA ();  TCCR2A=0;  TCCR2B=(0<<CS22)|(1<<CS21)|(1<<CS20);  OCR2A=255;  TIMSK2=(1<<OCIE2A);    break;}                                                             
          default: { ; break;}
        }
}


ISR(TIMER2_COMPA_vect) {
   TCNT2 = 0; // Работает, если вручную обнулять
   PORTB  = PORTB  | 1<<1;  PORTB = PORTB & ~(1<<1);
   ADCSRA |= (1<<ADSC);
   MyBuff_ADC[N] = PIND ;
   MyBuff_AI0[N+2] = ADCH; 
   N++;
   if (N>= 250) {TIMSK2=0b00000000; FlagSyncOk = true; ; };
 }
  
ISR (PCINT1_vect) {
  TimeClickB = millis();
  static byte old_n = PINC & B00011000; // маска B00011000 что б читать только нужные 2 бита
  byte new_n = PINC & B00011000;
  if (old_n == 8 && new_n == 24 || old_n == 16 && new_n == 0) {
    if ((PINC&(1<<5))==0){NumberMenu = constrain(NumberMenu-1,0,4) ;} else {--EncoderGo /*-= 1*/;}
  }
  if (old_n == 16 && new_n == 24 || old_n == 8 && new_n == 0) {
    if ((PINC&(1<<5))==0){NumberMenu = constrain(NumberMenu+1,0,4) ;} else {++EncoderGo /*+= 1*/;}
  }
  old_n = new_n;
  //FlagSyncOk=true;
}


void ExecEncoder()
{

  if (EncoderGo != 0) {
    ;
    switch (NumberMenu) {
      case 0: {//час розгортки
          MeasureDelay = MeasureDelay + (EncoderGo);   //*0*/byte MUX = 2;  // Частота вибырки
          MeasureDelay = constrain(MeasureDelay, 0, 14);
          break;
        }
      case 1: { // точка синхронізації
          SyncVal += EncoderGo<<Scale; //  constrain(EncoderGo,-10,10)/*<<Scale*/;
          break;
      }
      case 2: {// масштаб по вертикалі
          Scale = constrain(Scale + EncoderGo, 0, 2);  //*2*/byte Scale = 0 ;//Масштаб /  0, 2, 4
          break;
        }// constrain(Scale+K,0,2); break;}
      case 3: {
          Bias = Bias + EncoderGo  ;   //*3*/int  Bias ; //Змыщення по осі У
          break;
        }
      case 4: {
          BiasAI0 = BiasAI0 + EncoderGo  ;   //*3*/int  Bias ; //Змыщення по осі У
          break;
        }        

        

    };

   EncoderGo = 0; 
  }
}


void synchronization()
{
  StartPoint = 0;
  LastPoint = 0;           
  CountPoint = 0;     
  for (int i = 6; i <= 253 ; i++) // тут 
  {
    if( ((MyBuff_ADC[(i-3 )]*2+MyBuff_ADC[(i-2 )]*4+MyBuff_ADC[(i-1 )]*8)<=SyncVal*14)&&
       ((MyBuff_ADC[(i+3 )]*2+MyBuff_ADC[(i+2 )]*4+MyBuff_ADC[(i+1 )]*8)>=SyncVal*14) 
       )
            {// Тут знайшли точку.
             if (StartPoint==0) {StartPoint = (i) ; i= i+8;  }
               else {
                     if ((i-StartPoint)>125 ) {  break;}
                     else {
                     CountPoint++; 
                     LastPoint = i;
                     i= i+8; }
                    }
            };

  };

}




void ReadRefValue ()
{
     NullPoint =  (NullPoint*3+analogread_map(7,0,1024,0,255))/4   ; 
   UrefD =  (UrefD*3+analogread256(1))/4  ;       ///13*3.6 Калыбровка дыапазону
   Uref = UrefD*13/11;
   

}


void DravValue() {
  
//  glcd.drawstringS(0, 0, "LP-SP= " + String(LastPoint-StartPoint)) ;
 // glcd.drawstringS(0,2, "Cnt= " + String(CountPoint)) ;
//  glcd.drawstringS(0, 2, "MDelay= " + String(MeasureDelay)) ;
//  glcd.drawstringS(0, 0, "Fr= " + String((32000000>>MeasureDelay)/((LastPoint-StartPoint)/CountPoint))) ;
  
  //glcd.drawstringS(0, 1, "Memory= " + String(freeRam())) ;
  unsigned long  kU = 65535;
  byte show =    !((millis()-TimeClickB)>2000) ;
  String period = String(MeasureDelay);
  switch (MeasureDelay) {
  case 0:  {Tinc = 16 ; period = "0.5ms"; period[3]=230; break ;}  // 0.625мс  32000000
  case 1:  {Tinc = 16 ; period = "1ms" ; period[1]=230; break ;}   // 1.25мс 16000000 
  case 2:  {Tinc = 20 ; period = "2.5ms"  ; period[3]=230; break ;}    // 2.5µs  8000000
  case 3:  {Tinc = 20 ; period = "5ms"    ; period[1]=230; break ;}     // 5мс*******400000**************
  case 4:  {Tinc = 20 ; period = "10ms"   ; period[2]=230; break ;}     // 5мс
  case 5:  {Tinc = 20 ; period = "20ms"   ; period[2]=230; break ;}
  case 6:  {Tinc = 25 ; period = "50ms"   ; period[2]=230; break ;}
  case 7:  {Tinc = 25 ; period = "100ms"  ; period[3]=230; break ;}
  case 8:  {Tinc = 25 ; period = "200ms"  ; period[3]=230; break ;}
  case 9:  {Tinc = 25 ; period = "400ms"  ; period[3]=230; break ;}
  case 10: {Tinc = 25 ; period = "800ms"  ; period[3]=230; break ;}
  case 11: {Tinc = 25 ; period = "1.6ms"  ;  break ;}
  case 12: {Tinc = 25 ; period = "3.2ms"  ;  break ;}
  case 13: {Tinc = 25 ; period = "6.4ms"  ;  break ;}
  case 14: {Tinc = 25 ; period = "12.8ms"  ;  break ;}
  };

  if ((show)|(NumberMenu==0)) {glcd.drawstringS(127-6*period.length(), 0, period) ;
                      if (CountPoint!=0)
                      { Fr = ((32000000*CountPoint/(LastPoint-StartPoint)>>MeasureDelay));
                        glcd.drawstringS(0, 0, "Fr= " + String(
                                                              Fr
                                                              )) ;
                      } else {glcd.drawstringS(0, 0,"Fr= ----");}
                      } ; // 0 Частота вибырки
  if ((show)|(NumberMenu==1)) {glcd.drawstringS(127-6*String(SyncVal).length(), 1, String(SyncVal) ) ;} ;
  if ((show)|(NumberMenu==2)) {glcd.drawstringS(127-6*String(Scale).length(), 2 , String(Scale) )  ;} ; //1 Масштаб /  0, 2, 4
  if ((show)|(NumberMenu==3)) {glcd.drawstringS(127-6*String(Bias).length(), 3 , String(Bias) )  ;} ;
  if ((show)|(NumberMenu==4)) {glcd.drawstringS(127-6*String(BiasAI0).length(), 4 , String(BiasAI0) )  ;} ;

  if (show){
  glcd.drawline(127, 0+NumberMenu*8, 127, 7+NumberMenu*8, BLACK);
  glcd.drawline(126, 0+NumberMenu*8, 126, 7+NumberMenu*8, BLACK);
  }
  ;
  
}

void drawL (int y) {
  if (y <= 255) {
                   for (int i = 0; i <= 127; i = i + 4)
                  {
                    setpixel(i,y,BLACK);
                  }
  }
}


void DrawGrid() {
  int y = 0;
  drawL (NullPoint+Uref/2);
  drawL (NullPoint+Uref/2-Uref/16);
  drawL (NullPoint+Uref/4);
  drawL (NullPoint+Uref/8);
  drawL (NullPoint+Uref/16);      
  drawL (NullPoint+Uref/2-Uref/8);
  drawL (NullPoint+Uref/4+Uref/16);
  drawL (NullPoint+Uref/4-Uref/16);
    
  drawL (NullPoint-(Uref/2));
  drawL (NullPoint-(Uref/2-Uref/16));    
  drawL (NullPoint-(Uref/4));
  drawL (NullPoint-(Uref/8));
  drawL (NullPoint-(Uref/16));        
  drawL (NullPoint-(Uref/2-Uref/8));  
  drawL (NullPoint-(Uref/4+Uref/16));
  drawL (NullPoint-(Uref/4-Uref/16));
  
  for (int i = 0; i <= 127; i = i + 4)
  {
    setpixel(i,NullPoint,BLACK);
    setpixel(i+1,NullPoint,BLACK);
  };
   
  for (int x = Tinc; x <= 127; x = x + Tinc)
  {
    for ( y = 0; y <= 256; y = y + (4<<Scale))  {
        setpixel(x, y, BLACK); 
      }
  };
  
   glcd.drawline(3, ScalePint(SyncVal), 7, ScalePint(SyncVal), BLACK);

   glcd.drawline(LastPoint-StartPoint, 58,LastPoint-StartPoint, 63, BLACK);

   

}

void DrawChart()
{
  uint8_t aY1 ;
  uint8_t aY2;
  uint8_t bY1 ;
  uint8_t bY2;  
  for (int i = StartPoint; i <= 126+StartPoint; i++)
  {
    
    aY1= ScalePint(MyBuff_ADC[i]);
    aY2= ScalePint(MyBuff_ADC[i+1]);

    glcd.drawline(i-StartPoint, aY1, i+1-StartPoint, aY2, BLACK);
    if ((MeasureDelay==6)|(MeasureDelay==7)|(MeasureDelay==8)|(MeasureDelay==9)|(MeasureDelay==10)|(MeasureDelay==11)|(MeasureDelay==12)|(MeasureDelay==13)|(MeasureDelay==14))
    {
        bY1= ScalePint(MyBuff_AI0[i]+(BiasAI0<<Scale));
        bY2= ScalePint(MyBuff_AI0[i+1]+(BiasAI0<<Scale));      
        glcd.drawline(i-StartPoint, bY1, i+1-StartPoint, bY2, BLACK);
    }
   
  };

  

};

void setpixel(int X , int Y, byte color )
{
  Y = ScalePint( Y);
  if ((Y>0)&&(Y<64)) {glcd.setpixel(X, Y, color);}
};


int ScalePint( int Y)
{ 
  
  return constrain(    (((255 - Y)>>Scale)- Bias-((127 >> Scale) - 31))        , 0 , 255) ;
  
}



int freeRam(void)
{
  extern int  __bss_end;
  extern int  *__brkval;
  int free_memory;
  if ((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__bss_end);
  }
  else {
    free_memory = ((int)&free_memory) - ((int)__brkval);
  }
  return free_memory;
}
