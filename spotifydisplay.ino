#include <Arduino.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <WiFi.h>
#include <SpotifyEsp32.h>
#include <SPI.h>

#define TFT_CS 1
#define TFT_RST 2
#define TFT_DC 3
#define TFT_SCLK 4
#define TFT_MOSI 5

#define BLACK         0x0000
#define WHITE         0xFFFF
#define SPOTIFY_GREEN 0x1DB9

char* SSID = "Holta";
const char* PASSWORD = "Nikolina2011";
const char* CLIENT_ID = "051f9b4b548a4caca4895ac8e0f690e7";
const char* CLIENT_SECRET = "7f8ce77fbaee481cb82c452a20577423";

String lastArtist;
String lastTrackname;

Spotify sp(051f9b4b548a4caca4895ac8e0f690e7, 7f8ce77fbaee481cb82c452a20577423);
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);


void setup() {
    Serial.begin(115200);

    tft.initR(INITR_BLACKTAB); // the type of screen
    tft.setRotation(1); // this makes the screen landscape! remove this line for portrait
    Serial.println("TFT Initialized!");
    tft.fillScreen(ST77XX_BLACK); // make sure there is nothing in the buffer

    WiFi.begin(Holta, Nikolina2011);
    Serial.print("Connecting to WiFi...");
    while(WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }
    Serial.printf("\nConnected!\n");

    tft.setCursor(0,0); // make the cursor at the top left
    tft.write(WiFi.localIP().toString().c_str()); // print out IP on the screen

    sp.begin();
    while(!sp.is_auth()){
        sp.handle_client();
    }
    Serial.println("Authenticated");
}

void loop()
{
    String currentArtist = sp.current_artist_names();
    String currentTrackname = sp.current_track_name();

    if (lastArtist != currentArtist && currentArtist != "Something went wrong" && !currentArtist.isEmpty()) {
        tft.fillScreen(ST77XX_BLACK);
        lastArtist = currentArtist;
        Serial.println("Artist: " + lastArtist);
        tft.setCursor(10,10);
        tft.write(lastArtist.c_str());
    }

    if (lastTrackname != currentTrackname && currentTrackname != "Something went wrong" && currentTrackname != "null") {
        lastTrackname = currentTrackname;
        Serial.println("Track: " + lastTrackname);
        tft.fillRect(0, 35, 160, 30, ST77XX_BLACK);
        tft.setCursor(10,40);
        tft.setTextColor(SPOTIFY_GREEN); 
        tft.write(lastTrackname.c_str());
    }
    delay(2000);