//the setup scripts
void SetUpRGB()
{
  LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);         // For WS2812B
  FastLED.setBrightness(max_bright);
  
  for(int i = 0; i < NUM_LEDS; i++)
  {
    if(previewRGB){
      leds[i] = 0x00FF00;
      FastLED.show();
      delay(1);
    }
    leds[i] = 0x000000;
  }
  FastLED.show();
  Serial.println("RGB is ready!");
}

// control all the LEDs with a byte array containing a gradient
// all values: (0,255)
// place_1, H1, S1, L1,
// ...
// place_n, Hn, Sn, Ln
void ControlRGBWithGradient(int gradientBytes[], int nrOfColors)
{
  //currentPalette.loadDynamicGradientPalette(gradientBytes);

  //interpolate between the different colors to get the value at each led

  Serial.print("The number of colors= ");
  Serial.println(nrOfColors);

  if(nrOfColors == 1){
    Serial.print("One color active: ");
    
    fill_solid( leds, NUM_LEDS, CHSV(gradientBytes[1],gradientBytes[2],gradientBytes[3])); 
    FastLED.show();
    /*
    Serial.print("- R: ");
    Serial.print(leds[0].r);
    Serial.print(", ");
    Serial.print("G: ");
    Serial.print(leds[0].g);
    Serial.print(", ");
    Serial.print("B: ");
    Serial.print(leds[0].b);
    Serial.println(" ");
    */
    return;
  }

  

  int startEndColorDistance  = map(gradientBytes[0],0,(255 - gradientBytes[(nrOfColors-1)*4] + gradientBytes[0]), 0, 255) ;

  CHSV pixelColor = blend( 
    CHSV(gradientBytes[1],gradientBytes[2],gradientBytes[3]), 
    CHSV(gradientBytes[(nrOfColors-1)*4+1],gradientBytes[(nrOfColors-1)*4+2],gradientBytes[(nrOfColors-1)*4+3]), 
    startEndColorDistance);

  int startBytes[4]  = {0, 
    pixelColor.h,
    pixelColor.s,
    pixelColor.v
    //(gradientBytes[0] * gradientBytes[1]) + ((255-gradientBytes[(nrOfColors-1)*4]) * gradientBytes[(nrOfColors-1)*4+1])/startEndColorDistance,
    //(gradientBytes[0] * gradientBytes[2]) + ((255-gradientBytes[(nrOfColors-1)*4]) * gradientBytes[(nrOfColors-1)*4+2])/startEndColorDistance,
    //(gradientBytes[0] * gradientBytes[3]) + ((255-gradientBytes[(nrOfColors-1)*4]) * gradientBytes[(nrOfColors-1)*4+3])/startEndColorDistance
  };

  for(int i =0; i< nrOfColors-1;i++){
    int next = (i+1)%nrOfColors;
    /*
    Serial.print("Setting color nr: ");
    Serial.print(i);
    Serial.print(" , the color is: ");
    Serial.print("- H: ");
    Serial.print(gradientBytes[i*4+1]);
    Serial.print(", ");
    Serial.print("S: ");
    Serial.print(gradientBytes[i*4+2]);
    Serial.print(", ");
    Serial.print("V: ");
    Serial.print(gradientBytes[i*4+3]);
    Serial.print(", Going from LEDnr: ");
    Serial.print(gradientBytes[i*4] * NUM_LEDS / 255);
    Serial.print(", To: ");
    Serial.println(gradientBytes[next*4] * NUM_LEDS / 255);
    */
    fill_gradient(leds, gradientBytes[i*4] * NUM_LEDS / 255, CHSV(gradientBytes[i*4+1],gradientBytes[i*4+2],gradientBytes[i*4+3]), gradientBytes[next*4] * NUM_LEDS / 255, CHSV(gradientBytes[next*4+1],gradientBytes[next*4+2],gradientBytes[next*4+3]));
  }
  
  //fill the last part with ovelap at the start
  fill_gradient(leds, 
    gradientBytes[(nrOfColors-1)*4] * NUM_LEDS / 255, CHSV(gradientBytes[(nrOfColors-1)*4+1],gradientBytes[(nrOfColors-1)*4+2],gradientBytes[(nrOfColors-1)*4+3]), 
    NUM_LEDS, CHSV(startBytes[1],startBytes[2],startBytes[3])
  );
  /*
  Serial.print("Setting color nr: ");
  Serial.print(nrOfColors-1);
  Serial.print(" , the color is: ");
  Serial.print("- H: ");
  Serial.print(gradientBytes[(nrOfColors-1)*4+1]);
  Serial.print(", ");
  Serial.print("S: ");
  Serial.print(gradientBytes[(nrOfColors-1)*4+2]);
  Serial.print(", ");
  Serial.print("V: ");
  Serial.print(gradientBytes[(nrOfColors-1)*4+3]);
  Serial.print(", Going from LEDnr: ");
  Serial.print(gradientBytes[(nrOfColors-1)*4] * NUM_LEDS / 255);
  Serial.print(", To: ");
  Serial.println(gradientBytes[0] * NUM_LEDS / 255);
  */
  
  fill_gradient(leds, 
    0, CHSV(startBytes[1],startBytes[2],startBytes[3]), 
    gradientBytes[0] * NUM_LEDS / 255, CHSV(gradientBytes[1],gradientBytes[2],gradientBytes[3])
  );
  

/* 
  second way: byte defBytes[sizeof(gradientBytes)+8];
*/
  //fill_palette(leds, NUM_LEDS, 0, 255 / NUM_LEDS, currentPalette, 255, LINEARBLEND);
  FastLED.show();
  /*
  Serial.println("These are the resulting colors");
  for(int i = 0; i<NUM_LEDS; i++){
    Serial.print(i);
    Serial.print("- R: ");
    Serial.print(leds[i].r);
    Serial.print(", ");
    Serial.print("G: ");
    Serial.print(leds[i].g);
    Serial.print(", ");
    Serial.print("B: ");
    Serial.print(leds[i].b);
    Serial.println(" ");
  }
  */
  Serial.println("RGB Is Ready");
  
}

int currentBytes[256];

// string format: "/GET {pos},{H},{S},{L}/C"
void ControlRGBWithText(String data)
{
  int numbercount = 0;
  int firstIndex = data.indexOf('/');
  for(int i = firstIndex+1; i < data.length()-2;)
  {
    int nextIndex = data.indexOf(',', i);
    if(nextIndex == -1) nextIndex = data.length()-2;    
    int nr = max(0,min(data.substring(i, nextIndex).toInt(), 255));
    currentBytes[numbercount] = nr;
    
    numbercount++;
    i = nextIndex+1;

  }

  int defBytes[numbercount];
  //Serial.println(" ");
  //Serial.println("these are the bytes: ");

  for(int i = 0; i < numbercount;i++)
  {
    defBytes[i] = currentBytes[i];
    //Serial.println(defBytes[i]);
  }

  //Serial.println("end");
  ControlRGBWithGradient(defBytes, numbercount/4);

}
//todo add fade
void TurnOffRGB(){
  FastLED.clear(true);
}
