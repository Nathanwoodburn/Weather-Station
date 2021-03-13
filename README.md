# Weather Station
Weather Station using a Wemos D1 Mini Pro V1.0 with a 1.44" TFT LCD Screen

This project gets weather updates from [openweathermap.org](openweathermap.org) using a free API Key.
This project also sends updates to [ThingSpeak IoT](https://thingspeak.com) using a free API Key.


Using the [instructable to convert images to Flash memory](https://www.instructables.com/Converting-Images-to-Flash-Memory-Iconsimages-for-/).




<p align="center">Latest Status</p>
<p align="center"><iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1314847/widgets/280945"></iframe></p>

# How It Works
1. The Wemos will connect to the local WiFi network.
2. It gets the current time from http://worldtimeapi.org
3. It desterilizes the result from the time server. This just gets the current time.
4. It gets the current weather from http://openweathermap.org
5. It desterilizes the weather input. This gets the current temperature and weather (Clear, Rain, Clouds...)
6. It gets the weather forecast from http://openweathermap.org
7. It desterilizes the forecast input. This gets the todays and the next two days Max/Min temp, % of rain, weather (Clear, Rain, Clouds...)
8. It gets a custom message from http://thingspeak.com
9. It desterilizes the custom message.
10. It then prints on the screen the background to match the weather, then the temp now and todays Max/Min, % of rain and custom message
11. It then cycles through the three days (today, tomorrow and day after)
12. It then sends a status report to http://thingspeak.com This is just number of errors and what weather info it got from the weather server (this is used to monitor whether the wemos is working correctly)
13. The Wemos then goes into deep sleep only to be waken by pressing the Reset button


Code for Getting current time, getting weather and forecast, desterilize json, get custom message and display on screen.

<script src="https://emgithub.com/embed.js?target=https%3A%2F%2Fgithub.com%2FNathanwoodburn%2FWeather-Station%2Fblob%2Fmain%2Fweather_station%2Fweather_station.ino&style=atom-one-dark&showLineNumbers=on"></script>
