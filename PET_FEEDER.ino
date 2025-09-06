#define BLYNK_PRINT Serial // Enables Serial Monitor
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
#include <TridentTD_LineNotify.h>
Servo servo; // create servo object to control a servo
int servoPin = 2; // NodeMCU D4 pin / GIO2
int irPin = 15; // NodeMCU D8 pin
int val = 0;
char auth[] = "B1N6SOrfWthfbMAHBWQD79Kpp_le-IJQ"; //array of char for string
char ssid[] = "YoungTae";
char pass[] = "1010101010";
#define LINE_TOKEN "SR3QcIeC8IdSv9aO5MHgGq7PSYu2V1FmSNwzs2LsVYU"
void setup() {
  Serial.begin(115200); 
  Blynk.begin(auth, ssid, pass,"blynk-cloud.com", 8080); // Here your Arduino connects to the Blynk Cloud.
  servo.attach(servoPin); 
  servo.write(0);
  pinMode(irPin, INPUT); 
  Serial.println(LINE.getVersion());
  WiFi.begin(ssid, pass);
  Serial.printf("WiFi connecting to %s\n",  ssid);
  while (WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(400); }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());
}
void loop() {
  Blynk.run();
  val = digitalRead(irPin);
  if (val == 0) {
    Blynk.virtualWrite(V5, "มีอาหาร");
  }
  else {
    Blynk.virtualWrite(V5, "อาหารหมด");
  }
  Serial.print("val = ");
  Serial.println(val);
  delay(1500);
}
BLYNK_WRITE(V1) { //button เป็นฟังก์ชันในการสั่งการทำงานเมื่อกดปุ่ม 
  if (val == 0 && param.asInt() == 0) { // ตรวจจับเจอวัตถุค่าเป็น 0  และ มีการกดปุ่มจริง สั่งให้ Servo ทำงาน
    servo.write(180);
    delay(1000);
    servo.write(0);
    Serial.println("กดปุ่มให้อาหาร");
    LINE.setToken(LINE_TOKEN);
    LINE.notifySticker("กดปุ่มให้อาหาร", 446, 1997);
  }
  else if (val == 1 && param.asInt() == 1) {
    LINE.setToken(LINE_TOKEN);
    Serial.println("อาหารหมด");
    Blynk.notify("กรุณาเติมอาหาร-อาหารหมด");
    LINE.notifySticker("อาหารหมด", 1070, 17857);
  }
}
BLYNK_WRITE(V2) { //timer เป็นฟังก์ชันในการสั่งการทำงานเมื่อถึงเวลา
  if (val == 0) { // ค่าเป็น 0 ตรวจจับเจอวัตถุ สั่งให้ Servo ทำงาน
    servo.write(100);
    delay(1000);
    servo.write(0);
    LINE.setToken(LINE_TOKEN);
    Serial.println("ให้อาหารตามเวลา");
    LINE.notifySticker("ให้อาหารตามเวลา", 6325, 10979908);
  }
  else if (val == 1) {
    Serial.println("อาหารหมด");
    Blynk.notify("กรุณาเติมอาหาร-อาหารหมด");
    LINE.setToken(LINE_TOKEN);
    LINE.notifySticker("อาหารหมด", 1070, 17857);
  }
}
