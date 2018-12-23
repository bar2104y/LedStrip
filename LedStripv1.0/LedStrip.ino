#include <Adafruit_NeoPixel.h>
#include "IRLremote.h"

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED, NEO_GRB + NEO_KHZ800);
HashIR IRLremote;

/*
 * IR settings Amperka
 * 0xECCC87A9 Power
 * 0xCE65B4A9 -
 * 0xFE1762A9 +
 * 0x7F0B0A9 RED
 * 0x988829A9 GREEN
 * 0x4DBEBA9 BLUE
 * 0x592049A9 X
 * 0x3AB976A9 Cube
 * 0x9B9CEEA9 Triangle
 * 0xEFE14CA9 LeftTop
 * 0xD17A79A9 Top
 * 0x12C27A9 RightTop
 * 0x5C350EA9 Left
 * 0xA35D74A9 Play/pause
 * 0x6A6B24A9 Right
 * 0x6D7FE9A9 Left Bottom
 * 0xA048AFA9 Bottom
 * 0x3DCE3BA9 RightBottom
 * 0x921299A9 X
 * 0x8EFDD4A9 Y
 * 0x25BED7A9 Z
 */

/*
 * Режимы 
 * colorWipe    все светодиоды загораются по очереди
 * RunningLed   бежит один светодиод
 * rainbow      лента заполняется одним цветом, потом другим итд
 * rainbowCycle вся лента переливается цветами
 * setColor     установка одного цвета для всей ленты
 * OffAllLeds   выключение ленты
 */

#define LED_COUNT 30 // Кол-во светодиодов
#define LED 10       // Пин ленты
#define pinIR 2      // Пин ИК приемника (НЕ МЕНЯТЬ!!!)





byte NumsColors = 10; // Кол-во цветов
byte colorSet[10][3] = { //массив с цветами RGB
  {255, 0, 0},      // 0  Красный
  {255, 102, 0},    // 1  Оранжевый
  {255, 255, 0},    // 2  Желтый
  {0, 255, 0},      // 3  Зеленый
  {100, 100, 255},  // 4  Голубой
  {0, 0, 255},      // 5  Синий
  {100, 0, 100},    // 6  Фиолетовый
  {255, 0, 255},    // 7  Фиолетовый 2
  {255, 255, 255},  // 8  Белый
  {50,50,50}        // 9 Тусклый белый
};

uint32_t IRdata; //переменная для хранения
uint32_t color = strip.Color(colorSet[0][0],colorSet[0][1],colorSet[0][2]); // стартовый цвет


bool StripStatus = true; // Триггер включения ленты
bool ir_flag = false;    // Наличие данных с ИК порта

int StripSpeed = 100;    // задержка между этапами режимов
int mode = 1;            //номер режима
int index = 0;           // номер светодиода
int jndex= 0;            // счетчик для режима
byte countMode = 5;      //Кол-во режимов
byte colorI = 9;         // номер активного цвета


void setup()
{
  Serial.begin(9600);   // подключаем монитор порта
  
  if (!IRLremote.begin(pinIR))
    Serial.println(F("Ошибка соединения ИК-приемника"));

    
  pinMode(LED, OUTPUT); // пинмод управления ленты

  strip.begin();        // инициализируем светодиодную ленту
  setColor();           // Установка цвета для всей ленты
  strip.show();
}

void loop()
{
  
  if (IRLremote.available())//Если пришли данные
  {
    auto data = IRLremote.read();
    IRdata = data.command;
    if (IRdata != 292984781)
      Serial.println(IRdata, HEX);
      
    ir_flag = true; // поднимаем флаг данных

    
  }

  if (!IRLremote.receiving())//Обязательная конструкция
  {
    if (ir_flag)
    { // если данные пришли
      switch (IRdata) {
        case 0xECCC87A9: // Power
          StripStatus = !StripStatus;
          break;
        case 0xCE65B4A9: // -
          StripSpeed += 10;
          break;
        case 0xFE1762A9: // +
          StripSpeed -= 10;
          
          break;
        case 0x7F0B0A9:  // RED
          colorI = 0;
          updateColor();
          break; 
        case 0x988829A9: // GREEN
          colorI = 3;
          updateColor();
          break;
        case 0x4DBEBA9:  // BLUE
          colorI = 5;
          updateColor();
          break;
        /*case 0x592049A9: // X
        case 0x3AB976A9: // Cube
        case 0x9B9CEEA9: // Triangle
        case 0xEFE14CA9: // LeftTop*/
        case 0xD17A79A9: // Top
          colorI = (colorI+1)%NumsColors;
          updateColor();
          break; 
        //case 0x12C27A9:  // RightTop
        case 0x5C350EA9: // Left
          mode = (mode-1)%countMode;
          break;
        case 0xA35D74A9: // Play/pause
        case 0x6A6B24A9: // Right
          mode = (mode+1)%countMode;
          break;
        case 0x6D7FE9A9: // Left Bottom
        case 0xA048AFA9: // Bottom
          colorI = (colorI-1)%NumsColors;
          updateColor();
          break;
        /*case 0x3DCE3BA9: // RightBottom
        case 0x921299A9: // X
        case 0x8EFDD4A9: // Y
        case 0x25BED7A9: // Z
        default:
          mode = 0;
          break;*/
      }
      //Вывод параметров после их обновления
      /*Serial.print("Delay: ");Serial.println(StripSpeed);
      Serial.print("Mode: ");Serial.println(mode);
      Serial.print("Color: ");Serial.println(colorI);*/
      ir_flag = false; //Опускаем флаг данных
    }
    
    
    if (StripStatus) //Если лента включена
    {
      switch (mode)// В зависимости от номера режима запускаем функцию
      {
        case 0:
          RunningLed();
          break;
        case 1:
          setColor();
          break;
        case 2:
          colorWipe();
          break;
        case 3:
          rainbow();
          break;
        case 4:
          rainbowCycle();
          break;
        case 5:
          RunningLed();
          break;
        default:
          setColor();
          break;
      }
    }
    else //иначе выключить ленту
      OffAllLeds();
  }
  delay(StripSpeed); //Задержка
}
