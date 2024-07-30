#include <LiquidCrystal_I2C.h>
#include <NewPing.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <UniversalTelegramBot.h>
#include <HTTPClient.h>
#include <NTPClient.h>

// WiFi Connection Details
const char* ssid = "Awoakwok";
const char* password = "rifqi56789";

// Waktu Tunggu sebelum mengirim pesan
unsigned long waitm = 10000; // 10000 ms  =10 detik,[/]

// Waktu output buzzer
const unsigned long buzzerm = 10000; // 10000 ms = 10 detik

// HiveMQ Cloud Broker settings
const char* mqtt_server = "030bd9397a6e41f8b3d0689cc236aee0.s1.eu.hivemq.cloud";
const char* mqtt_username = "Gacorr48";
const char* mqtt_password = "8989,Rifzee";
const int mqtt_port = 8883;
const char* mqtt_topic_ultrasonic = "/test/SIC21/SensorUltrasonic ";

// Setup Telebot -- @Botfather
#define BOT_TOKEN "7292307199:AAHAnti-_vZQo1jm2PyvqF04uLaZ-FlWTB8"
const unsigned long BOT_MTBS = 500; // mean time in ms. between scan messages

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime;          // last time messages' scan has been done

// HiveMQ Cloud Let's Encrypt CA certificate
static const char *root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";

// Pin Definitions
const int trigPin = 5;
const int echoPin = 18;
const int buzzerPin = 19;

// LCD Configuration
const int LCD_COLS = 20; // Number of columns in the LCD
const int LCD_ROWS = 4;  // Number of rows in the LCD
const int LCD_ADDRESS = 0x27; // I2C address of the LCD

// Initialize LCD
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

// Ultrasonic sensor configuration
unsigned int maxDistance = 500; // Maximum distance in cm
NewPing sonar(trigPin, echoPin, maxDistance); // Ultrasonic sensor object

WiFiClientSecure espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
bool notificationSent = false; // Flag to avoid repeated notifications

// OpenWeatherMap API details
const String weatherApiKey = "a0cb4aad9823a209a6512bc72ad21633";
const String city = "Pekanbaru";
const String weatherServer = "https://api.openweathermap.org/data/2.5/weather?q=pekanbaru,id&appid=a0cb4aad9823a209a6512bc72ad21633&units=metric&lang=id";

// NTP Client to fetch time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// Function to get weather forecast data from OpenWeatherMap
String getWeatherForecast()
{
  HTTPClient http;
  String url = "https://api.openweathermap.org/data/2.5/forecast?q=" + city + "&appid=" + weatherApiKey + "&units=metric&lang=id";
  http.begin(url);
  int httpCode = http.GET();

  String payload;
  if (httpCode > 0)
  {
    payload = http.getString();
  }
  else
  {
    payload = "Unable to retrieve weather data";
  }
  http.end();

  JsonDocument doc;
  deserializeJson(doc, payload);

  // Extract weather forecast for the next 1 hour
  JsonObject forecast = doc["list"][0]["main"];
  String description = doc["list"][0]["weather"][0]["description"].as<String>();
  float temp = forecast["temp"].as<float>();

  // Get the current time
  timeClient.update();
  String formattedTime = timeClient.getFormattedTime();

  String weatherReport = "Perkiraan cuaca di " + city + " untuk beberapa jam ke depan:\n";
  weatherReport += "Cuaca: " + description + "\n";
  weatherReport += "Suhu: " + String(temp) + "°C\n";
  weatherReport += "Waktu: " + formattedTime;

  return weatherReport;
}

// Handle messages received from Telebot
void handleNewMessages(int numNewMessages)
{
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++)
  {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;

    if (text == "/start")
    {
      String welcome = "Selamat Datang di Prototipe Bot, " + from_name + ".\n";
      welcome += "/check_distance : Untuk mengecek jarak dalam satuan cm\n";
      welcome += "/weather : Untuk mendapatkan prediksi cuaca di Pekanbaru\n";
      bot.sendChatAction(chat_id, "typing");
      bot.sendMessage(chat_id, welcome);
    }

    if (text == "/check_distance")
    {
      bot.sendChatAction(chat_id, "typing");
      unsigned int distance = sonar.ping_cm();
      String message = "Pengukuran " + String(distance) + " cm";
      bot.sendMessage(chat_id, message, "");
    }

    if (text == "/weather")
    {
      bot.sendChatAction(chat_id, "typing");
      String weather = getWeatherForecast();
      bot.sendMessage(chat_id, weather, "");
    }
  }
}

// Setup Telebot
void bot_setup()
{
  const String commands = F("["
                            "{\"command\":\"help\",  \"description\":\"Memberikan list perintah\"},"
                            "{\"command\":\"start\", \"description\":\"Awalan untuk bot (selamat datang)\"},"
                            "{\"command\":\"check_distance\",\"description\":\"Mengukur jarak air dalam satuan cm\"},"
                            "{\"command\":\"weather\",\"description\":\"Mendapatkan perkiraan cuaca di Pekanbaru untuk beberapa jam ke depan\"}" // no comma on last command
                            "]");
  bot.setMyCommands(commands);
}

// Function to send Telegram notification
void sendTelegramNotification() {
  unsigned int distance = sonar.ping_cm();
  String alert = getWeatherForecast();

  if (distance <= 17 && distance >= 14) {
    String message = "SIAGA: Batas air sudah mecapai level siaga " + String(distance) + " cm.\n\n";
    message += alert;
    bot.sendMessage("7349117375", message, ""); // Replace "7349117375" with your actual chat ID
  } else if (distance <= 14 ) {
    String message = "BAHAYA: Ketinggian air sudah mencapai level bahaya, tolong segera evakuasi " + String(distance) + " cm.\n\n";
    message += alert;
    bot.sendMessage("7349117375", message, ""); // Replace "7349117375" with your actual chat ID
  }
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  configTime(0, 0, "time.google.com"); // get UTC time via NTP
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    if (millis() >= 60000) {
      Serial.println("Connection timeout, entering sleep mode");
      delay(1000);
      break;
    }
  }

  randomSeed(micros());
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // Check NTP/Time, usually it is instantaneous and you can delete the code below.
    Serial.print("Retrieving time: ");
    time_t now = time(nullptr);
    while (now < 24 * 3600) {
      Serial.print(".");
      delay(1000);
      now = time(nullptr);
    }
    Serial.println(now);
    bot_setup();
    Serial.println("Commands are set up");
  } else {
    Serial.println("WiFi not connected");
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // Create a client ID
    String clientId = "ESP32Client";

    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected!");

      // Once connected, publish an announcement
      client.publish(mqtt_topic_ultrasonic, "ESP32 connected");

      // Subscribe to topics here if needed
      // client.subscribe("topic");

    } else {
      // If failed, retry in 5 seconds
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// Filter rata-rata bergerak (Moving Average Filter)
#define NUM_READINGS 10 // Jumlah pembacaan untuk rata-rata bergerak

long readings[NUM_READINGS]; // Array untuk menyimpan pembacaan
int readIndex = 0;           // Indeks pembacaan saat ini
long total = 0;              // Total semua pembacaan
long average = 0;            // Rata-rata pembacaan

String lastDisplayedLine1 = "";
String lastDisplayedLine2 = "";
String lastDisplayedLine3 = "";
String lastDisplayedLine4 = "";

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.begin(20, 4); // Adjust according to your LCD configuration

  // Initialize buzzer
  pinMode(buzzerPin, OUTPUT);

  // Initialize serial communication
  Serial.begin(115200);

  // Connect to WiFi
  setup_wifi();

  // Set CA certificate for MQTT connection
  espClient.setCACert(root_ca);
  client.setServer(mqtt_server, mqtt_port);

  // Start NTP time client
  timeClient.begin();
  timeClient.setTimeOffset(25200); // Set time offset to GMT+7 for Pekanbaru

  // Inisialisasi array pembacaan dengan 0
  for (int i = 0; i < NUM_READINGS; i++) {
    readings[i] = 0;
  }
}

void updateLCDLine(int line, String text) {
  String currentText;
  switch (line) {
    case 0:
      currentText = lastDisplayedLine1;
      break;
    case 1:
      currentText = lastDisplayedLine2;
      break;
    case 2:
      currentText = lastDisplayedLine3;
      break;
    case 3:
      currentText = lastDisplayedLine4;
      break;
  }

  if (currentText != text) {
    lcd.setCursor(0, line);
    lcd.print(text);
    for (int i = text.length(); i < 20; i++) {
      lcd.print(" ");
    }

    switch (line) {
      case 0:
        lastDisplayedLine1 = text;
        break;
      case 1:
        lastDisplayedLine2 = text;
        break;
      case 2:
        lastDisplayedLine3 = text;
        break;
      case 3:
        lastDisplayedLine4 = text;
        break;
    }
  }
}

void loop() {
  static unsigned long previousMillis = 0;
  const unsigned long interval = 50; // Mengurangi interval pengukuran menjadi 50ms
  static unsigned long lastPing = 0;
  static String lastFormattedTime = "";
  static unsigned int lastAverage = 0;

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Measure distance with ultrasonic sensor if enough time has passed
    if (currentMillis - lastPing >= interval) {
      lastPing = currentMillis;
      unsigned int distance = sonar.ping_cm();
      Serial.print("Distance: ");
      Serial.println(distance);  // Print distance to serial monitor for debugging

      // Update the moving average filter
      total = total - readings[readIndex];
      readings[readIndex] = distance;
      total = total + readings[readIndex];
      readIndex = (readIndex + 1) % NUM_READINGS;
      average = total / NUM_READINGS;

      // Get the current time
      timeClient.update();
      String formattedTime = timeClient.getFormattedTime();

      // Check if the display needs to be updated
      bool updateLCD = false;

      if (formattedTime != lastFormattedTime) {
        updateLCD = true;
        lastFormattedTime = formattedTime;
      }

      if (average != lastAverage) {
        updateLCD = true;
        lastAverage = average;
      }

      if (updateLCD) {
        // Display SIC KELOMPOK 21
        updateLCDLine(0, "SIC KELOMPOK 21");

        // Display distance on LCD
        updateLCDLine(1, "Distance: " + String(average) + " cm");

        // Display current time on LCD
        updateLCDLine(3, "Time: " + formattedTime);

        // Check if average distance is within alert levels
        if (average <= 17 && average >= 14) {
          // Display object detected message on LCD
          updateLCDLine(2, "Water In Increases");

          // Activate buzzer
          tone(buzzerPin, 500, buzzerm); // Activate buzzer at 500 Hz
          Serial.println("Water In Increases! Buzzer active.");
          Serial.println(average);

          // Send Telegram notification if not already sent
          if (!notificationSent) {
            sendTelegramNotification();
            notificationSent = true; // Set flag to true to avoid repeated notifications
          }

          // Publish message to MQTT topic
          StaticJsonDocument<256> doc;
          doc["status"] = "Water In Increases!";
          doc["distance_cm"] = average;
          char buffer[256];
          size_t n = serializeJson(doc, buffer);
          if (client.publish(mqtt_topic_ultrasonic, buffer, n)) {
            Serial.println("MQTT message sent successfully");
          } else {
            Serial.println("Failed to send MQTT message");
          }

        } else if (average <= 14) {
          // Display object detected message on LCD
          updateLCDLine(2, "Water In Increases");

          // Activate buzzer
          tone(buzzerPin, 2000, buzzerm); // Activate buzzer at 2000 kHz
          Serial.println("Water In Increases! Buzzer active.");
          Serial.println(average);

          // Send Telegram notification if not already sent
          if (!notificationSent) {
            sendTelegramNotification();
            notificationSent = true; // Set flag to true to avoid repeated notifications
          }

          // Publish message to MQTT topic
          StaticJsonDocument<256> doc;
          doc["status"] = "Water In Increases!";
          doc["distance_cm"] = average;
          char buffer[256];
          size_t n = serializeJson(doc, buffer);
          if (client.publish(mqtt_topic_ultrasonic, buffer, n)) {
            Serial.println("MQTT message sent successfully");
          } else {
            Serial.println("Failed to send MQTT message");
          }

        } else {
          // Display distance on LCD
          updateLCDLine(2, "Safe: " + String(average) + " cm   ");

          // Deactivate buzzer
          noTone(buzzerPin);

          // Reset notification flag when distance is safe
          notificationSent = false;
        }
      }
    }
  }

  // Telebot
  if (millis() - bot_lasttime > BOT_MTBS) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }

  // Check if it's time to fetch weather forecast (every hour)
  if (timeClient.update() && timeClient.getMinutes() == 0) {
    bot.sendMessage("7349117375", getWeatherForecast(), ""); // Replace "7349117375" with your actual chat ID
  }

  // Reconnect to MQTT broker if disconnected
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
