/*
 * Перезаписывает переменную color с новым значением
 */
void updateColor(){
  color = strip.Color(colorSet[colorI][0],colorSet[colorI][1],colorSet[colorI][2]);
}

/*
 * Линия заполняется с начала и до конца по одному светодиоду
 * Переключается на следующий цвет
 * Повторяет все с новым цветом
 */
void colorWipe() {
  if (index >= LED_COUNT){
    index = 0;
    colorI = (colorI+1)%10;
    color = strip.Color(colorSet[colorI][0],colorSet[colorI][1],colorSet[colorI][2]);
  }
    
  Serial.println("Color Wipe");
  strip.setPixelColor(index, color);
  strip.show();
  index++;
}


/*
 * Один бегущий светодиод
 * Светодиод До и После горит тем же цветом, но менее ярко
 */
void RunningLed()
{
  if (index >= LED_COUNT)
  {
    index = 0;
    strip.setPixelColor(LED_COUNT-1, 0,0,0);
    strip.setPixelColor(LED_COUNT-2, 0,0,0);
  }
  if (index >1)
    strip.setPixelColor(index-2, strip.Color(0,0,0));
  if (index >0)
    strip.setPixelColor(index-1, strip.Color(colorSet[colorI][0]/20,colorSet[colorI][1]/20,colorSet[colorI][2]/20));
    
  strip.setPixelColor(index, color);

  if(index < LED_COUNT-1)
    strip.setPixelColor(index+1, strip.Color(colorSet[colorI][0]/20,colorSet[colorI][1]/20,colorSet[colorI][2]/20));
    
  strip.show();
  index++;
}

/*
 * Переливание цветов
 * Вся лента одновременно
 */
void rainbow()
{
  uint16_t i;
  Serial.println("RainBow");
  if (jndex >= 256)
    jndex = 0;

    for(i=0; i<strip.numPixels(); i++)
      strip.setPixelColor(i, Wheel((i+jndex) & 255));
      
    strip.show();
    jndex++;

}

/*
 * Движение пикселей разных цветов
 */
void rainbowCycle()
{
  uint16_t i;

  if (jndex >= 256*5)
    jndex = 0;

  
    for(i=0; i< strip.numPixels(); i++)
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + jndex) & 255));
      
    strip.show();
    jndex++;
}

/*
 * Вспомогательная функция для функций rainbowCycle и rainbow
 */
uint32_t Wheel(byte WheelPos)
{
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85)
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    
  if(WheelPos < 170){
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void setColor()
{
  for(int i = 0; i < LED_COUNT; i++)
    strip.setPixelColor(i, color);
  strip.show();
}

/*
 * Выключение всех светодиодов
 */
void OffAllLeds()
{
  for(int i = 0; i < LED_COUNT; i++)
    strip.setPixelColor(i, 0,0,0);
  strip.show();
}
