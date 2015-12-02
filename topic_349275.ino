
// need 5 characters - 4 characters and the null terminator
char lcd_output[5];

// the conversion table is a series of pairs of float.
// first value is the temp, second value is the ABV
// the first values (temperatures) **MUST** be in ascending order
// however, they don't have to be consecutive

float conversionTable[][2] = 
{
  { 5, 90},
  { 15, 80},
  { 40, 40},
  { 60, 10},
  { 75, 0}
};

const int LEN = sizeof(conversionTable) / sizeof(*conversionTable);


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  while(!Serial);
  
  Serial.print("Starting demo in:");
  for(int i = 3;i>0;i--) {
    Serial.print(' ');
    Serial.print(i);
    delay(1000);
  }
  Serial.println("\nOK! Beginning demo!");
  
  float temp;
  
  // we will simulate a range of temperatures being read from the sensor
  
  for(temp = -5; temp <= 85; temp += 5) {
    Serial.print("Calculating ABV at ");
    Serial.print(temp);
    Serial.print(" deg C.");
    
    // this function computes the ABV. A result below 0 or 100 or higher
    // indicates that the value is out of range
    
    float abv = calculateABV(temp);
    Serial.print(" Computed ABV is ");
    Serial.print(abv);
    Serial.print(".");
    
    // this function works out what to display on the LCD.
    // if the value is out of range, then we will see 
    // -LO- or -HI- rather than a number
    
    pct2string(abv, lcd_output);
    
    Serial.print(" LCD output is [");
    Serial.print(lcd_output);
    Serial.println("].");
    
    
  }
  
}

void loop() {
}

float calculateABV(float temp) {
  // is the temp less than the first element of our table?

  if(temp < conversionTable[0][0]) return 1000; // boy, that's strong stuff!
  
  if(temp > conversionTable[LEN-1][0]) return -1; // no alcohol at all

  // ok. lets find the pair of entries n the table between which our
  // temperature lies
  
  int lo = 0;
  int hi = LEN-1;
  
  while(hi-lo > 1) {
    int mid = (hi+lo)/2;
    
    if(temp >= conversionTable[mid][0]) {
      lo = mid;
    }
    else {
      hi = mid;
    }
  }
  
  if(conversionTable[hi][1] == conversionTable[lo][1]) {
    return conversionTable[hi][1];
  }

  // aaaand interpolate

  return conversionTable[lo][1] 
    + (conversionTable[hi][1]-conversionTable[lo][1])
    * (temp - conversionTable[lo][0]) / 
    (conversionTable[hi][0]-conversionTable[lo][0]);
}

void pct2string(float pct, char *str) {
  if(pct < 0) {
    strcpy(str, "-LO-");
    return;
  }
  if(pct >= 100) {
    strcpy(str, "-HI-");
    return;
  }
  
  int rounded = (int)(pct*10+.5);
  
  str[0] = (rounded/100) % 10 + '0';
  str[1] = (rounded/10) % 10 + '0';
  str[2] = '.';
  str[3] = (rounded) % 10 + '0';
  str[4] = '\0';
}


