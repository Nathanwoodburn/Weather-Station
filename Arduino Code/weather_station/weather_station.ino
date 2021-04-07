//General Settings
int screens = 4; // Times the weather screens loop
int loop_delay = 8; //Time in seconds between each screen
int screen_rotation = 0; //screen rotation (0-4)
bool show_time = true; //Whether time should be displayed

//WIFI Settings
const char* ssid = "YOUR WIFI SSID";
const char* password = "WIFI PASSWORD";
const char* host = "api.thingspeak.com";

//Includes
#include <Adafruit_ST7735.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <time.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include "bitmaps.h"
#define TFT_RST -1 //for TFT I2C Connector Shield V1.0.0 and TFT 1.4 Shield V1.0.0
#define TFT_CS D4  //for TFT I2C Connector Shield V1.0.0 and TFT 1.4 Shield V1.0.0
#define TFT_DC D3  //for TFT I2C Connector Shield V1.0.0 and TFT 1.4 Shield V1.0.0
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);


WiFiClient client;

//Permenent Settings
String APIKEY = "YOUR API KEY"; //OpenWeathermap.org api key
String location = "Canberra,AU"; //City, Country Code
char server[] = "api.openweathermap.org";
//Code variables DO NOT CHANGE!!!
String errors;
int main_temp1;
String main_weather1;
int main_max1;
int main_min1;
String main_2;
String main_3;
int max_0;
int max_1;
int max_2;
int max_3;
int min_0;
int min_1;
int min_2;
int min_3;
int pop_0;
int pop_1;
int pop_2;
int pop_3;
String day_2;
String message_0;
String icon_1;
String icon_2;
String icon_3;

void setup() {
  Serial.begin(9600);
  bool load1used = false;
  errors = "";
  tft.initR(INITR_144GREENTAB);
  tft.setRotation(tft.getRotation() + screen_rotation);

  tft.setTextWrap(false); // Allow text to run off right edge
  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_BLACK);
  loadimage(true);


  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(" .");
    loadimage(load1used);
    load1used = !load1used;
    delay(1000);
  }
  Serial.println("\nConnected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  Serial.println("Getting Time");
  String time_now = "2021-00-00T00:00:00.000000+11:00";
  int day_0 = 0;
  HTTPClient http;  //Declare an object of class HTTPClient
  http.begin("http://worldtimeapi.org/api/timezone/Australia/Canberra");  //connect to the weather site
  int httpCode = http.GET();                                  //Send the request

  if (httpCode > 0) { //Check the returning code
    String input = http.getString();   //Get the request response payload
    StaticJsonDocument<768> doc;

    DeserializationError error = deserializeJson(doc, input);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    const char* abbreviation = doc["abbreviation"]; // "AEDT"
    const char* client_ip = doc["client_ip"]; // "121.45.204.2"
    const char* datetime = doc["datetime"]; // "2021-03-05T09:29:52.136250+11:00"
    int day_of_week = doc["day_of_week"]; // 5
    int day_of_year = doc["day_of_year"]; // 64
    bool dst = doc["dst"]; // true
    const char* dst_from = doc["dst_from"]; // "2020-10-03T16:00:00+00:00"
    int dst_offset = doc["dst_offset"]; // 3600
    const char* dst_until = doc["dst_until"]; // "2021-04-03T16:00:00+00:00"
    long raw_offset = doc["raw_offset"]; // 36000
    const char* timezone = doc["timezone"]; // "Australia/Canberra"
    long unixtime = doc["unixtime"]; // 1614896992
    const char* utc_datetime = doc["utc_datetime"]; // "2021-03-04T22:29:52.136250+00:00"
    const char* utc_offset = doc["utc_offset"]; // "+11:00"
    int week_number = doc["week_number"]; // 9
    time_now = datetime;
    day_0 = day_of_week;
    Serial.print("Time: ");
    Serial.println(time_now.substring(11, 16));
  }
  else
  {
    Serial.println("Failed to get time");
  }
  http.end();   //Close connection
  Serial.println("Ready");
  time_now = time_now.substring(11, 16);
  //getWeather(day_0, time_now.substring(11, 16));


  int dayofweek = day_0;
  int main_humidity1;
  int main_pressure1;
  int retry = 0;
  errors = "";
  if (time_now == "00:00")
  {
    errors = "1";
    Serial.println("Failed to get time");
  }
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
currentweather:
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin("http://api.openweathermap.org/data/2.5/weather?q=Canberra,AU&APPID=YOUR API KEY&units=metric");  //connect to the weather site
    int httpCode = http.GET();                                  //Send the request

    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      http.end();   //Close connection
      StaticJsonDocument<1024> doc;
      deserializeJson(doc, payload);

      float coord_lon = doc["coord"]["lon"]; // 149.1281
      float coord_lat = doc["coord"]["lat"]; // -35.2835

      JsonObject weather_0 = doc["weather"][0];
      int weather_0_id = weather_0["id"]; // 800
      const char* weather_0_main = weather_0["main"]; // "Clear"
      const char* weather_0_description = weather_0["description"]; // "clear sky"
      const char* weather_0_icon = weather_0["icon"]; // "01d"

      const char* base = doc["base"]; // "stations"

      JsonObject main = doc["main"];
      float main_temp = main["temp"]; // 25.15
      float main_feels_like = main["feels_like"]; // 20.67
      float main_temp_min = main["temp_min"]; // 23.89
      float main_temp_max = main["temp_max"]; // 26.11
      int main_pressure = main["pressure"]; // 1017
      int main_humidity = main["humidity"]; // 16

      int visibility = doc["visibility"]; // 10000

      float wind_speed = doc["wind"]["speed"]; // 3.09
      int wind_deg = doc["wind"]["deg"]; // 210

      int clouds_all = doc["clouds"]["all"]; // 0

      long dt = doc["dt"]; // 1611020364

      JsonObject sys = doc["sys"];
      int sys_type = sys["type"]; // 1
      int sys_id = sys["id"]; // 9588
      const char* sys_country = sys["country"]; // "AU"
      long sys_sunrise = sys["sunrise"]; // 1610996907
      long sys_sunset = sys["sunset"]; // 1611047963

      long timezone = doc["timezone"]; // 39600
      long id = doc["id"]; // 2172517
      const char* name = doc["name"]; // "Canberra"
      int cod = doc["cod"]; // 200


      Serial.println("Current:");
      main_weather1 = weather_0_main;
      Serial.println(main_temp);
      main_temp1 = main_temp;
      Serial.print(" C and ");
      Serial.println(weather_0_main);
      Serial.println("Icon");
      Serial.println(weather_0_icon);
      icon_1 = weather_0_icon;
      Serial.print("Humidity: ");
      Serial.print(main_humidity);
      main_humidity1 = main_humidity;
      Serial.println("%");
      Serial.print("Pressure: ");
      Serial.print(main_pressure);
      main_pressure1 = main_pressure;
      Serial.println(" hPa");
    }
    else
    {
      errors = errors + "2";
      Serial.println("Error Getting Weather Data");
      retry += 1;
      if (retry < 5)
      {
        Serial.println("Retrying . . .");
        goto currentweather;
      }
      else
      {
        Serial.println("Retry timeout");
      }
    }
forecast:
    HTTPClient http3;  //Declare an object of class HTTPClient
    http3.begin("http://api.openweathermap.org/data/2.5/onecall?lat=-35.2835&lon=149.1281&exclude=current,minutely,hourly,alerts&appid=YOUR API KEY&units=metric");  //Request forecast
    int httpCode3 = http3.GET();                                  //Send the request
    if (httpCode3 > 0) { //Check the returning code
      String input = http3.getString();   //Get the request response payload
      http3.end();   //Close connection
      //Deserialize Json
      // String input;
      DynamicJsonDocument doc(6144);
      deserializeJson(doc, input);

      float lat = doc["lat"]; // -35.2835
      float lon = doc["lon"]; // 149.1281
      const char* timezone = doc["timezone"]; // "Australia/Sydney"
      long timezone_offset = doc["timezone_offset"]; // 39600

      JsonArray daily = doc["daily"];

      JsonObject daily_0 = daily[0];
      long daily_0_dt = daily_0["dt"]; // 1611021600
      long daily_0_sunrise = daily_0["sunrise"]; // 1610996907
      long daily_0_sunset = daily_0["sunset"]; // 1611047963

      JsonObject daily_0_temp = daily_0["temp"];
      float daily_0_temp_day = daily_0_temp["day"]; // 28.57
      float daily_0_temp_min = daily_0_temp["min"]; // 12.38
      float daily_0_temp_max = daily_0_temp["max"]; // 29.62
      float daily_0_temp_night = daily_0_temp["night"]; // 12.7
      float daily_0_temp_eve = daily_0_temp["eve"]; // 19.45
      float daily_0_temp_morn = daily_0_temp["morn"]; // 13.39

      JsonObject daily_0_feels_like = daily_0["feels_like"];
      float daily_0_feels_like_day = daily_0_feels_like["day"]; // 25.91
      float daily_0_feels_like_night = daily_0_feels_like["night"]; // 9.17
      float daily_0_feels_like_eve = daily_0_feels_like["eve"]; // 14.13
      float daily_0_feels_like_morn = daily_0_feels_like["morn"]; // 11.79

      int daily_0_pressure = daily_0["pressure"]; // 1016
      int daily_0_humidity = daily_0["humidity"]; // 17
      float daily_0_dew_point = daily_0["dew_point"]; // 1.88
      float daily_0_wind_speed = daily_0["wind_speed"]; // 1.2
      int daily_0_wind_deg = daily_0["wind_deg"]; // 127

      JsonObject daily_0_weather_0 = daily_0["weather"][0];
      int daily_0_weather_0_id = daily_0_weather_0["id"]; // 800
      const char* daily_0_weather_0_main = daily_0_weather_0["main"]; // "Clear"
      const char* daily_0_weather_0_description = daily_0_weather_0["description"]; // "clear sky"
      const char* daily_0_weather_0_icon = daily_0_weather_0["icon"]; // "01d"

      int daily_0_clouds = daily_0["clouds"]; // 0
      float daily_0_pop = daily_0["pop"]; // 0
      float daily_0_uvi = daily_0["uvi"]; // 13.56

      JsonObject daily_1 = daily[1];
      long daily_1_dt = daily_1["dt"]; // 1611108000
      long daily_1_sunrise = daily_1["sunrise"]; // 1611083369
      long daily_1_sunset = daily_1["sunset"]; // 1611134338

      JsonObject daily_1_temp = daily_1["temp"];
      float daily_1_temp_day = daily_1_temp["day"]; // 23.68
      float daily_1_temp_min = daily_1_temp["min"]; // 9.17
      float daily_1_temp_max = daily_1_temp["max"]; // 25.96
      float daily_1_temp_night = daily_1_temp["night"]; // 11.95
      float daily_1_temp_eve = daily_1_temp["eve"]; // 22.88
      float daily_1_temp_morn = daily_1_temp["morn"]; // 9.93

      JsonObject daily_1_feels_like = daily_1["feels_like"];
      float daily_1_feels_like_day = daily_1_feels_like["day"]; // 20.25
      float daily_1_feels_like_night = daily_1_feels_like["night"]; // 9.3
      float daily_1_feels_like_eve = daily_1_feels_like["eve"]; // 18.21
      float daily_1_feels_like_morn = daily_1_feels_like["morn"]; // 6.99

      int daily_1_pressure = daily_1["pressure"]; // 1021
      int daily_1_humidity = daily_1["humidity"]; // 27
      float daily_1_dew_point = daily_1["dew_point"]; // 3.62
      float daily_1_wind_speed = daily_1["wind_speed"]; // 2.9
      int daily_1_wind_deg = daily_1["wind_deg"]; // 79

      JsonObject daily_1_weather_0 = daily_1["weather"][0];
      int daily_1_weather_0_id = daily_1_weather_0["id"]; // 800
      const char* daily_1_weather_0_main = daily_1_weather_0["main"]; // "Clear"
      const char* daily_1_weather_0_description = daily_1_weather_0["description"]; // "clear sky"
      const char* daily_1_weather_0_icon = daily_1_weather_0["icon"]; // "01d"

      int daily_1_clouds = daily_1["clouds"]; // 0
      float daily_1_pop = daily_1["pop"]; // 0
      float daily_1_uvi = daily_1["uvi"]; // 13.86

      JsonObject daily_2 = daily[2];
      long daily_2_dt = daily_2["dt"]; // 1611194400
      long daily_2_sunrise = daily_2["sunrise"]; // 1611169831
      long daily_2_sunset = daily_2["sunset"]; // 1611220712

      JsonObject daily_2_temp = daily_2["temp"];
      float daily_2_temp_day = daily_2_temp["day"]; // 27.95
      float daily_2_temp_min = daily_2_temp["min"]; // 9.11
      float daily_2_temp_max = daily_2_temp["max"]; // 30.21
      float daily_2_temp_night = daily_2_temp["night"]; // 20.78
      float daily_2_temp_eve = daily_2_temp["eve"]; // 27.91
      float daily_2_temp_morn = daily_2_temp["morn"]; // 10.79

      JsonObject daily_2_feels_like = daily_2["feels_like"];
      float daily_2_feels_like_day = daily_2_feels_like["day"]; // 23.89
      float daily_2_feels_like_night = daily_2_feels_like["night"]; // 19.78
      float daily_2_feels_like_eve = daily_2_feels_like["eve"]; // 25.03
      float daily_2_feels_like_morn = daily_2_feels_like["morn"]; // 9.37

      int daily_2_pressure = daily_2["pressure"]; // 1014
      int daily_2_humidity = daily_2["humidity"]; // 25
      float daily_2_dew_point = daily_2["dew_point"]; // 6.22
      float daily_2_wind_speed = daily_2["wind_speed"]; // 4.52
      int daily_2_wind_deg = daily_2["wind_deg"]; // 322

      JsonObject daily_2_weather_0 = daily_2["weather"][0];
      int daily_2_weather_0_id = daily_2_weather_0["id"]; // 804
      const char* daily_2_weather_0_main = daily_2_weather_0["main"]; // "Clouds"
      const char* daily_2_weather_0_description = daily_2_weather_0["description"]; // "overcast clouds"
      const char* daily_2_weather_0_icon = daily_2_weather_0["icon"]; // "04d"

      int daily_2_clouds = daily_2["clouds"]; // 98
      float daily_2_pop = daily_2["pop"]; // 0
      float daily_2_uvi = daily_2["uvi"]; // 13.27

      JsonObject daily_3 = daily[3];
      long daily_3_dt = daily_3["dt"]; // 1611280800
      long daily_3_sunrise = daily_3["sunrise"]; // 1611256293
      long daily_3_sunset = daily_3["sunset"]; // 1611307084

      JsonObject daily_3_temp = daily_3["temp"];
      float daily_3_temp_day = daily_3_temp["day"]; // 31.01
      float daily_3_temp_min = daily_3_temp["min"]; // 14.87
      float daily_3_temp_max = daily_3_temp["max"]; // 31.36
      float daily_3_temp_night = daily_3_temp["night"]; // 19.37
      float daily_3_temp_eve = daily_3_temp["eve"]; // 26.68
      float daily_3_temp_morn = daily_3_temp["morn"]; // 16.01

      JsonObject daily_3_feels_like = daily_3["feels_like"];
      float daily_3_feels_like_day = daily_3_feels_like["day"]; // 27.24
      float daily_3_feels_like_night = daily_3_feels_like["night"]; // 17.88
      float daily_3_feels_like_eve = daily_3_feels_like["eve"]; // 24.26
      float daily_3_feels_like_morn = daily_3_feels_like["morn"]; // 16.29

      int daily_3_pressure = daily_3["pressure"]; // 1008
      int daily_3_humidity = daily_3["humidity"]; // 25
      float daily_3_dew_point = daily_3["dew_point"]; // 9.2
      float daily_3_wind_speed = daily_3["wind_speed"]; // 4.95
      int daily_3_wind_deg = daily_3["wind_deg"]; // 303

      JsonObject daily_3_weather_0 = daily_3["weather"][0];
      int daily_3_weather_0_id = daily_3_weather_0["id"]; // 500
      const char* daily_3_weather_0_main = daily_3_weather_0["main"]; // "Rain"
      const char* daily_3_weather_0_description = daily_3_weather_0["description"]; // "light rain"
      const char* daily_3_weather_0_icon = daily_3_weather_0["icon"]; // "10d"

      int daily_3_clouds = daily_3["clouds"]; // 38
      float daily_3_pop = daily_3["pop"]; // 0.71
      float daily_3_rain = daily_3["rain"]; // 2.86
      float daily_3_uvi = daily_3["uvi"]; // 13.6

      JsonObject daily_4 = daily[4];
      long daily_4_dt = daily_4["dt"]; // 1611367200
      long daily_4_sunrise = daily_4["sunrise"]; // 1611342756
      long daily_4_sunset = daily_4["sunset"]; // 1611393454

      JsonObject daily_4_temp = daily_4["temp"];
      float daily_4_temp_day = daily_4_temp["day"]; // 33.35
      float daily_4_temp_min = daily_4_temp["min"]; // 14.88
      float daily_4_temp_max = daily_4_temp["max"]; // 34.09
      float daily_4_temp_night = daily_4_temp["night"]; // 21.83
      float daily_4_temp_eve = daily_4_temp["eve"]; // 28.06
      float daily_4_temp_morn = daily_4_temp["morn"]; // 19.71

      JsonObject daily_4_feels_like = daily_4["feels_like"];
      float daily_4_feels_like_day = daily_4_feels_like["day"]; // 29.29
      float daily_4_feels_like_night = daily_4_feels_like["night"]; // 20.21
      float daily_4_feels_like_eve = daily_4_feels_like["eve"]; // 25.79
      float daily_4_feels_like_morn = daily_4_feels_like["morn"]; // 19.98

      int daily_4_pressure = daily_4["pressure"]; // 1010
      int daily_4_humidity = daily_4["humidity"]; // 21
      float daily_4_dew_point = daily_4["dew_point"]; // 8.25
      float daily_4_wind_speed = daily_4["wind_speed"]; // 5.14
      int daily_4_wind_deg = daily_4["wind_deg"]; // 310

      JsonObject daily_4_weather_0 = daily_4["weather"][0];
      int daily_4_weather_0_id = daily_4_weather_0["id"]; // 802
      const char* daily_4_weather_0_main = daily_4_weather_0["main"]; // "Clouds"
      const char* daily_4_weather_0_description = daily_4_weather_0["description"]; // "scattered clouds"
      const char* daily_4_weather_0_icon = daily_4_weather_0["icon"]; // "03d"

      int daily_4_clouds = daily_4["clouds"]; // 27
      int daily_4_pop = daily_4["pop"]; // 0
      float daily_4_uvi = daily_4["uvi"]; // 14.05

      JsonObject daily_5 = daily[5];
      long daily_5_dt = daily_5["dt"]; // 1611453600
      long daily_5_sunrise = daily_5["sunrise"]; // 1611429219
      long daily_5_sunset = daily_5["sunset"]; // 1611479823

      JsonObject daily_5_temp = daily_5["temp"];
      float daily_5_temp_day = daily_5_temp["day"]; // 35.07
      float daily_5_temp_min = daily_5_temp["min"]; // 18.43
      float daily_5_temp_max = daily_5_temp["max"]; // 35.07
      float daily_5_temp_night = daily_5_temp["night"]; // 23.48
      float daily_5_temp_eve = daily_5_temp["eve"]; // 28.54
      float daily_5_temp_morn = daily_5_temp["morn"]; // 23.34

      JsonObject daily_5_feels_like = daily_5["feels_like"];
      float daily_5_feels_like_day = daily_5_feels_like["day"]; // 31.7
      float daily_5_feels_like_night = daily_5_feels_like["night"]; // 23.18
      float daily_5_feels_like_eve = daily_5_feels_like["eve"]; // 27.05
      float daily_5_feels_like_morn = daily_5_feels_like["morn"]; // 23.57

      int daily_5_pressure = daily_5["pressure"]; // 1009
      int daily_5_humidity = daily_5["humidity"]; // 21
      float daily_5_dew_point = daily_5["dew_point"]; // 9.77
      float daily_5_wind_speed = daily_5["wind_speed"]; // 4.67
      int daily_5_wind_deg = daily_5["wind_deg"]; // 317

      JsonObject daily_5_weather_0 = daily_5["weather"][0];
      int daily_5_weather_0_id = daily_5_weather_0["id"]; // 800
      const char* daily_5_weather_0_main = daily_5_weather_0["main"]; // "Clear"
      const char* daily_5_weather_0_description = daily_5_weather_0["description"]; // "clear sky"
      const char* daily_5_weather_0_icon = daily_5_weather_0["icon"]; // "01d"

      int daily_5_clouds = daily_5["clouds"]; // 0
      int daily_5_pop = daily_5["pop"]; // 0
      int daily_5_uvi = daily_5["uvi"]; // 15

      JsonObject daily_6 = daily[6];
      long daily_6_dt = daily_6["dt"]; // 1611540000
      long daily_6_sunrise = daily_6["sunrise"]; // 1611515682
      long daily_6_sunset = daily_6["sunset"]; // 1611566189

      JsonObject daily_6_temp = daily_6["temp"];
      float daily_6_temp_day = daily_6_temp["day"]; // 35.71
      float daily_6_temp_min = daily_6_temp["min"]; // 20.43
      float daily_6_temp_max = daily_6_temp["max"]; // 35.71
      float daily_6_temp_night = daily_6_temp["night"]; // 23.37
      float daily_6_temp_eve = daily_6_temp["eve"]; // 27.91
      float daily_6_temp_morn = daily_6_temp["morn"]; // 24.67

      JsonObject daily_6_feels_like = daily_6["feels_like"];
      float daily_6_feels_like_day = daily_6_feels_like["day"]; // 31.64
      float daily_6_feels_like_night = daily_6_feels_like["night"]; // 21.55
      float daily_6_feels_like_eve = daily_6_feels_like["eve"]; // 24.22
      float daily_6_feels_like_morn = daily_6_feels_like["morn"]; // 23.38

      int daily_6_pressure = daily_6["pressure"]; // 1004
      int daily_6_humidity = daily_6["humidity"]; // 21
      float daily_6_dew_point = daily_6["dew_point"]; // 10.47
      float daily_6_wind_speed = daily_6["wind_speed"]; // 5.87
      int daily_6_wind_deg = daily_6["wind_deg"]; // 312

      JsonObject daily_6_weather_0 = daily_6["weather"][0];
      int daily_6_weather_0_id = daily_6_weather_0["id"]; // 500
      const char* daily_6_weather_0_main = daily_6_weather_0["main"]; // "Rain"
      const char* daily_6_weather_0_description = daily_6_weather_0["description"]; // "light rain"
      const char* daily_6_weather_0_icon = daily_6_weather_0["icon"]; // "10d"

      int daily_6_clouds = daily_6["clouds"]; // 0
      float daily_6_pop = daily_6["pop"]; // 0.53
      float daily_6_rain = daily_6["rain"]; // 0.31
      int daily_6_uvi = daily_6["uvi"]; // 15

      JsonObject daily_7 = daily[7];
      long daily_7_dt = daily_7["dt"]; // 1611626400
      long daily_7_sunrise = daily_7["sunrise"]; // 1611602145
      long daily_7_sunset = daily_7["sunset"]; // 1611652554

      JsonObject daily_7_temp = daily_7["temp"];
      float daily_7_temp_day = daily_7_temp["day"]; // 34.61
      float daily_7_temp_min = daily_7_temp["min"]; // 17.67
      float daily_7_temp_max = daily_7_temp["max"]; // 34.61
      float daily_7_temp_night = daily_7_temp["night"]; // 17.67
      float daily_7_temp_eve = daily_7_temp["eve"]; // 18.63
      float daily_7_temp_morn = daily_7_temp["morn"]; // 22.46

      JsonObject daily_7_feels_like = daily_7["feels_like"];
      float daily_7_feels_like_day = daily_7_feels_like["day"]; // 29.59
      float daily_7_feels_like_night = daily_7_feels_like["night"]; // 17.09
      float daily_7_feels_like_eve = daily_7_feels_like["eve"]; // 16.96
      float daily_7_feels_like_morn = daily_7_feels_like["morn"]; // 23.69

      int daily_7_pressure = daily_7["pressure"]; // 1004
      int daily_7_humidity = daily_7["humidity"]; // 22
      float daily_7_dew_point = daily_7["dew_point"]; // 10.28
      float daily_7_wind_speed = daily_7["wind_speed"]; // 7.14
      int daily_7_wind_deg = daily_7["wind_deg"]; // 312

      JsonObject daily_7_weather_0 = daily_7["weather"][0];
      int daily_7_weather_0_id = daily_7_weather_0["id"]; // 500
      const char* daily_7_weather_0_main = daily_7_weather_0["main"]; // "Rain"
      const char* daily_7_weather_0_description = daily_7_weather_0["description"]; // "light rain"
      const char* daily_7_weather_0_icon = daily_7_weather_0["icon"]; // "10d"

      int daily_7_clouds = daily_7["clouds"]; // 96
      float daily_7_pop = daily_7["pop"]; // 0.85
      float daily_7_rain = daily_7["rain"]; // 1.05
      int daily_7_uvi = daily_7["uvi"]; // 15


      Serial.print("Forecast for rest of Today: ");
      Serial.println(daily_0_weather_0_main);

      Serial.print("Max Temp: ");
      Serial.println(daily_0_temp_max);
      //delay(1500);
      Serial.print("Min Temp: ");
      Serial.println(daily_0_temp_min);
      int pop = daily_0_pop * 100;
      Serial.println(pop);
      // delay(1500);
      Serial.println("% chance of rain");

      //      tft.print(pop);
      //      tft.print("% rain\n Max:");
      //      tft.print(daily_0_temp_max);
      main_max1 = daily_0_temp_max;
      //      tft.print("\n Min:");
      //      tft.print(daily_0_temp_min);
      max_0 = daily_0_temp_max;
      min_0 = daily_0_temp_min;
      main_min1 = daily_0_temp_min;
      pop_0 = pop;
      Serial.print("Forecast for Tomorrow: ");
      icon_2 = daily_1_weather_0_icon;
      Serial.println(daily_1_weather_0_main);
      main_2 = daily_1_weather_0_main;
      // delay(1500);
      Serial.print("Max Temp: ");
      Serial.println(daily_1_temp_max);
      max_1 = daily_1_temp_max;
      min_1 = daily_1_temp_min;
      // delay(1500);
      Serial.print("Min Temp: ");
      Serial.println(daily_1_temp_min);
      pop = daily_1_pop * 100;
      Serial.println(pop);
      // delay(1500);
      Serial.println("% chance of rain");
      pop_1 = pop;
      Serial.print("UV Index: ");
      if (daily_1_uvi > 11.00)
      {
        Serial.println("Extreme");
      }
      else if (daily_1_uvi > 8.00)
      {
        Serial.println("Very High");
      }
      else if (daily_1_uvi > 6.00)
      {
        Serial.println("High");
      }
      else if (daily_1_uvi > 3.00)
      {
        Serial.println("Medium");
      }
      else
      {
        Serial.println("Low");
      }
      //Work out days
      String daytwo;
      String daythree;
      if (dayofweek == 0)
      {
        daytwo = "Tuesday";
        daythree = "Wednesday";
      }
      else if (dayofweek == 1)
      {
        daytwo = "Wednesday";
        daythree = "Thursday";
      }
      else if (dayofweek == 2)
      {
        daytwo = "Thursday";
        daythree = "Friday";
      }
      else if (dayofweek == 3)
      {
        daytwo = "Friday";
        daythree = "Saturday";
      }
      else if (dayofweek == 4)
      {
        daytwo = "Saturday";
        daythree = "Sunday";
      }
      else if (dayofweek == 5)
      {
        daytwo = "Sunday";
        daythree = "Monday";
      }
      else if (dayofweek == 6)
      {
        daytwo = "Monday";
        daythree = "Tuesday";
      }

      day_2 = daytwo;
      Serial.print("Forecast for ");
      Serial.print(daytwo);
      Serial.print(": ");
      Serial.println(daily_2_weather_0_main);
      main_3 = daily_2_weather_0_main;
      //      delay(1500);
      Serial.print("Max Temp: ");
      Serial.println(daily_2_temp_max);
      max_2 = daily_2_temp_max;
      min_2 = daily_2_temp_min;
      //      delay(1500);
      Serial.print("Min Temp: ");
      Serial.println(daily_2_temp_min);
      pop = daily_2_pop * 100;
      Serial.println(pop);
      pop_2 = pop;
      //      delay(1500);
      Serial.println("% chance of rain");
      Serial.print("UV Index: ");
      if (daily_2_uvi > 11.00)
      {
        Serial.println("Extreme");
      }
      else if (daily_2_uvi > 8.00)
      {
        Serial.println("Very High");
      }
      else if (daily_2_uvi > 6.00)
      {
        Serial.println("High");
      }
      else if (daily_2_uvi > 3.00)
      {
        Serial.println("Medium");
      }
      else
      {
        Serial.println("Low");
      }

      Serial.print("Forecast for ");
      Serial.print(daythree);
      Serial.print(": ");
      icon_3 = daily_2_weather_0_icon;
      Serial.println(daily_3_weather_0_main);
      //      delay(1500);
      Serial.print("Max Temp: ");
      Serial.println(daily_3_temp_max);
      max_3 = daily_3_temp_max;
      min_3 = daily_3_temp_min;
      //      delay(1500);
      Serial.print("Min Temp: ");
      Serial.println(daily_3_temp_min);
      pop = daily_3_pop * 100;
      Serial.println(pop);
      //      delay(1500);
      Serial.println("% chance of rain");
      pop_3 = pop;
      Serial.print("UV Index: ");
      if (daily_3_uvi > 11.00)
      {
        Serial.println("Extreme");
      }
      else if (daily_3_uvi > 8.00)
      {
        Serial.println("Very High");
      }
      else if (daily_3_uvi > 6.00)
      {
        Serial.println("High");
      }
      else if (daily_3_uvi > 3.00)
      {
        Serial.println("Medium");
      }
      else
      {
        Serial.println("Low");
      }


    }
    else
    {
      errors = errors + "3";
      Serial.println("Error Getting Forecast Data");
      retry += 1;

      if (retry < 5)
      {
        Serial.println("Retrying . . .");
        goto forecast;
      }
      else
      {
        Serial.println("Retry Timeout");
      }
    }

    //start custom text

    HTTPClient thinkspeak;
    thinkspeak.begin("http://api.thingspeak.com/talkbacks/TALK BACK ID/commands.json?apikey=TALK BACK API KEY");
    int httpCode9 = thinkspeak.GET();
    if (httpCode9 > 0) { //Check the returning code
      String input = thinkspeak.getString();   //Get the request response payload
      thinkspeak.end();
      // String input;

      StaticJsonDocument<512> doc;

      DeserializationError error = deserializeJson(doc, input);

      if (error) {
        errors = true;
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }

      //      JsonObject root_0 = doc[0];
      //      long root_0_id = root_0["id"]; // 22014661
      //      const char* root_0_command_string = root_0["command_string"]; // "This is the custom text for this month ...
      //      int root_0_position = root_0["position"]; // 1
      //      // root_0["executed_at"] is null
      //      const char* root_0_created_at = root_0["created_at"]; // "2021-03-01T08:46:57Z"
      int number1 = 1;
      String custom_text;
      for (JsonObject elem : doc.as<JsonArray>()) {

        long id = elem["id"]; // 22014661, 22051257, 22051285
        const char* command_string = elem["command_string"]; // "Go to weather.woodburn.tk", "True", "True"
        if (number1 == 1)
        {
          custom_text = command_string;
        }
        else if (number1 == 2)
        {
          String useclock(command_string);
          if (useclock == "False")
          {
            show_time = false;
          }
          else
          {
            show_time = true;
          }
        }
        else
        {

        }
        number1++;
        int position = elem["position"]; // 1, 2, 3
        // elem["executed_at"] is null
        const char* created_at = elem["created_at"]; // "2021-03-01T08:46:57Z", "2021-03-04T06:29:41Z", ...

      }

      Serial.println("Custom Input:");
      Serial.println(custom_text);
      Serial.println("Show Time:");
      Serial.println(show_time);

      //      tft.setTextSize(1);
      //      tft.setCursor(0, 110);
      //      tft.setTextWrap(true);
      //      tft.print(" ");
      //      tft.print(custom_text);
      message_0 = custom_text;
    }
    else
    {
      errors = errors + "4";
      Serial.println("Error");
    }

    //if getting time failed retry now
    if (time_now == "00:00") {
      time_now = get_time_now();
    }



    //end custom message
    //Print screens

if (errors == "")
    {
      Serial.println("No Errors");
      errors = "0";
    }
    else
    {
      Serial.print("Error code: ");
      Serial.println(errors);
    }
    Serial.println("Printing Screens");
    for (int i = 0; i < screens; i++) {

      Serial.println("Printing First Screen");
      tft.fillScreen(ST7735_BLACK);
      //draw background
      if (drawbackground(icon_1))
      {
        errors = errors + "5";
      }
      tft.setCursor(60, 5);
      tft.setTextWrap(false);
      tft.setTextSize(2);
      tft.print(time_now);
      tft.setTextSize(2);
      tft.print("\n\n ");
      tft.print(main_temp1);
      tft.print("C\n ");
      tft.print(pop_0);
      tft.print("% rain\n Max:");
      tft.print(main_max1);
      tft.print("C\n Min:");
      tft.print(main_min1);
      tft.print("C");
      tft.setTextSize(1);
      tft.setCursor(0, 110);
      tft.setTextWrap(true);
      tft.print(" ");
      tft.print(message_0);
      delay(loop_delay * 1000);


      Serial.println("Printing Second Screen");
      tft.fillScreen(ST7735_BLACK);
      //draw background
      if (drawbackground(icon_2))
      {
        errors = errors + "5";
      }
      tft.setCursor(60, 5);
      tft.setTextWrap(false);
      tft.setTextSize(2);
      tft.print(time_now);
      tft.setTextSize(2);
      tft.print("\n\n Tomorrow\n ");
      tft.print(min_1);
      tft.print("C - ");
      tft.print(max_1);
      tft.print("C\n ");
      tft.print(pop_1);
      tft.print("% Rain");
      tft.setTextSize(1);
      tft.setCursor(0, 110);
      tft.setTextWrap(true);
      tft.print(" ");
      tft.print(message_0);
      delay(loop_delay * 1000);


      Serial.println("Printing Third Screen");
      tft.fillScreen(ST7735_BLACK);
      //draw background
      if (drawbackground(icon_3))
      {
        errors = errors + "5";
      }
      tft.setCursor(60, 5);
      tft.setTextWrap(false);
      tft.setTextSize(2);
      tft.print(time_now);
      tft.setTextSize(2);
      tft.print("\n\n ");
      tft.print(day_2);
      tft.print("\n ");
      tft.print(min_2);
      tft.print("C - ");
      tft.print(max_2);
      tft.print("C\n ");
      tft.print(pop_2);
      tft.println("% Rain");
      tft.setTextSize(1);
      tft.setCursor(0, 110);
      tft.setTextWrap(true);
      tft.print(" ");
      tft.print(message_0);
      delay(loop_delay * 1000);
    }
    Serial.println("Finished Printing Screens");
    
    Serial.println("Giving Status Report");
    if (client.connect(host, 80))
    {
      String postStr = "THINGSPEAK API KEY";
      postStr += "&field1=";
      postStr += String(main_temp1);
      postStr += "&field2=";
      postStr += String(main_humidity1);
      postStr += "&field3=";
      postStr += String(main_pressure1);
      postStr += "&field4=";
      postStr += errors;
      postStr += "\r\n\r\n";

      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: THINGSPEAK API KEY\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr.length());
      client.print("\n\n");
      client.print(postStr);
      Serial.println("Status Saved");
    }
    else
    {
      Serial.println("Failed to send Status");
    }
    delay(1000);
    client.stop();
  }
  else
  {
    Serial.println("WIFI Not Connected");
    int h = 128, w = 128, row, col, buffidx = 0;
    for (row = 0; row < h; row++) { // For each scanline...
      for (col = 0; col < w; col++) { // For each pixel...
        tft.drawPixel(col, row, pgm_read_word(internet + buffidx));
        buffidx++;
      } // end pixel
    }
    delay(2000);
    ESP.restart();
  }

  Serial.println("Entering Deep Sleep");
  tft.fillScreen(ST7735_BLACK);
  delay(20000);
  ESP.deepSleep(0);
  delay(10); //let arduino go to sleep
}
void loop() {
  Serial.println("Sleep Failed"); //tell user if deep sleep failed
  tft.fillScreen(ST7735_BLUE);
  delay(100000); //Wait 100 sec
}

bool drawbackground(String icon_0) {
  bool result = false;
  int h = 128, w = 128, row, col, buffidx = 0;
  for (row = 0; row < h; row++) { // For each scanline...
    for (col = 0; col < w; col++) { // For each pixel...
      if (icon_0 == "01d" || icon_0 == "01n") {
        tft.drawPixel(col, row, pgm_read_word(clearsky + buffidx));
      }
      else if (icon_0 == "02d" || icon_0 == "02n" || icon_0 == "03d" || icon_0 == "03n")
      {
        tft.drawPixel(col, row, pgm_read_word(cloudy + buffidx));
      }
      else if (icon_0 == "04d" || icon_0 == "04n" ) {
        tft.drawPixel(col, row, pgm_read_word(overcast + buffidx));
      }
      else if (icon_0 == "09d" || icon_0 == "09n" || icon_0 == "10d" || icon_0 == "10n" || icon_0 == "13d" || icon_0 == "13n" || icon_0 == "50d" || icon_0 == "50n") {
        tft.drawPixel(col, row, pgm_read_word(shower + buffidx));
      }
      else if (icon_0 == "11d" || icon_0 == "11n") {
        tft.drawPixel(col, row, pgm_read_word(lightning + buffidx));
      }
      else
      {
        tft.drawPixel(col, row, pgm_read_word(clearsky + buffidx));
        result = false;
      }

      buffidx++;
    } // end pixel
  }
  return result;
}
String get_time_now() {
  String time_now = "2021-00-00T00:00:00.000000+11:00";
  int day_0 = 0;
  HTTPClient http;  //Declare an object of class HTTPClient
  http.begin("http://worldtimeapi.org/api/timezone/Australia/Canberra");  //connect to the weather site
  int httpCode = http.GET();                                  //Send the request

  if (httpCode > 0) { //Check the returning code
    String input = http.getString();   //Get the request response payload
    StaticJsonDocument<768> doc;

    DeserializationError error = deserializeJson(doc, input);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return "00:00";
    }

    const char* abbreviation = doc["abbreviation"]; // "AEDT"
    const char* client_ip = doc["client_ip"]; // "121.45.204.2"
    const char* datetime = doc["datetime"]; // "2021-03-05T09:29:52.136250+11:00"
    int day_of_week = doc["day_of_week"]; // 5
    int day_of_year = doc["day_of_year"]; // 64
    bool dst = doc["dst"]; // true
    const char* dst_from = doc["dst_from"]; // "2020-10-03T16:00:00+00:00"
    int dst_offset = doc["dst_offset"]; // 3600
    const char* dst_until = doc["dst_until"]; // "2021-04-03T16:00:00+00:00"
    long raw_offset = doc["raw_offset"]; // 36000
    const char* timezone = doc["timezone"]; // "Australia/Canberra"
    long unixtime = doc["unixtime"]; // 1614896992
    const char* utc_datetime = doc["utc_datetime"]; // "2021-03-04T22:29:52.136250+00:00"
    const char* utc_offset = doc["utc_offset"]; // "+11:00"
    int week_number = doc["week_number"]; // 9
    time_now = datetime;
    day_0 = day_of_week;
    Serial.print("Time: ");
    Serial.println(time_now.substring(11, 16));
    return time_now.substring(11, 16);
  }
  else
  {
    Serial.println("Failed to get time");
    return "00:00";
  }
  http.end();   //Close connection
}
void loadimage(bool image1)
{
  if (image1)
  {
    int h = 128, w = 128, row, col, buffidx = 0;
    for (row = 0; row < h; row++) { // For each scanline...
      for (col = 0; col < w; col++) { // For each pixel...
        tft.drawPixel(col, row, pgm_read_word(load1 + buffidx));
        buffidx++;
      } // end pixel
    }
  }
  else
  {
    int h = 128, w = 128, row, col, buffidx = 0;
    for (row = 0; row < h; row++) { // For each scanline...
      for (col = 0; col < w; col++) { // For each pixel...
         tft.drawPixel(col, row, pgm_read_word(load2 + buffidx));
        buffidx++;
      } // end pixel
    }
  }
}
