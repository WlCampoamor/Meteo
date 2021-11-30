


/* Circuito de valores de  temperatura y Humedad DHT-11 y segunda medida temperatura por termistor en display LCD 16,2, refresco pantalla cada seg, blink.   

 * El esquema electrico es el usado por ELEGOO tanto de termistor, DHT-11 y Display 16,2
 
 * Rev. 2.0  20.NOV.2021  by  Walter L.Campoamor Stursberg.
 
 * 
 */

#include <LiquidCrystal.h>                                      // incluimos la libreria LCD
#include <dht_nonblocking.h>                                    // incluimos la libreria sensor de humedad y temperatura DHT-11 distribuido por Elegoo
#define DHT_SENSOR_TYPE DHT_TYPE_11                             // define el sensor DHT-11
static const int DHT_SENSOR_PIN = 2;                            // declara el PIN 2 Digital como entero constante estatica de entrada DHT-11, tambien podiamos declararlo como #define.
int tempPin = 0;                                                //  PIN A0 analogico para lectura de datos del termistor distribuido por Elegoo
 
//                BS  E  D4 D5  D6 D7
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);                   // Declaración de los pines de la pantalla LCD 16-2 para la bliblioteca.
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);                                  // declaro Pin 13 como salida
  lcd.begin(16, 2);                                              // declaro filas y columnas del display utilizado
}

/*
 * Poll for a measurement, keeping the state machine alive.  Returns
 * true if a measurement is available.
 */
static bool measure_environment( float *temperature, float *humidity )  
  {
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
 
}


void loop()
{
 
   float temperature;
   float humidity;


int tempReading = analogRead(tempPin);
  // This is OK
  double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
  float tempC = tempK - 273.15;            // Convert Kelvin to Celcius
  float tempF = (tempC * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
  
  
  /* Measure temperature and humidity.  If the functions returns
     true, then a measurement is available. */
     
  if( measure_environment( &temperature, &humidity ) == true )
  {

  /* Seccion DHT-11 */
   
    lcd.setCursor(0, 0);
    lcd.print("In:");
    lcd.setCursor(4, 0);
    lcd.print(temperature, 1 );
    lcd.print((char)223);
    lcd.print("C");
    lcd.setCursor(11, 0);
    lcd.print( humidity, 1 );
    lcd.println( "%" );
   
   /* Sección termistor */
    
    lcd.setCursor(0, 1);
    lcd.print("Out:");
    lcd.setCursor(4, 1);
    lcd.print(tempC);
    lcd.print((char)223);
    lcd.print("C");
    
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level) 
    delay(150);
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(150);
  }
  
}
