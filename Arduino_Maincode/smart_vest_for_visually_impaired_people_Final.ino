#include <DFRobot_TFmini.h> // 라이더 구동용
#include <Servo.h>
#include <NewPing.h>
#include <SoftwareSerial.h> // 블루투스 연결용

#define BT_TXD 2
#define BT_RXD 3
#define TRIGGER_PIN 13                   // trigPin을 13으로 설정합니다.
#define ECHO_PIN 12                // echoPin을 12로 설정합니다.

Servo servomotor;
SoftwareSerial mySerial(8, 7); // RX, TX 라이더
SoftwareSerial bluetooth(BT_RXD, BT_TXD);
NewPing sonar(TRIGGER_PIN, ECHO_PIN);
DFRobot_TFmini  TFmini; // 라이더
uint16_t distance_ld; // 라이더 거리 값
long distance1, distance2 , distance_abs = 0;

int pos; //각도 변수
int v;      //속도
int vmax;       //최대 속도
int Sigpin =  10 ;  // 레이더 신호 입력 핀


void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  Serial.println("START");
  TFmini.begin(mySerial);
  pinMode (Sigpin , INPUT);
  servomotor.attach(9);
}

void loop() {




  unsigned  long T;          // 주기
  double f;                 // 주파수

  for (pos = 0; pos < 180; pos += 2) {
    servomotor.write(pos);
    delay(30);

    vmax = 0 ;
    while (digitalRead (Sigpin));
    while ( ! digitalRead (Sigpin)); //여기까지 전방 쪽 핀에 데이터 받는 부분

    T = pulseIn (Sigpin , HIGH) + pulseIn (Sigpin , LOW); // 주기 측정
    f = 1 / ( double ) T;            // 주파수 측정
    v = int ((f * 1e6 ) / 44.0 );   // 속도 측정
    vmax = max (v, vmax);       // 속도의 Max값 측정

    delay(30);

    servomotor.detach();


    if (vmax > 5)
    {
      Serial.print("속도");
      Serial.println(vmax);
      bluetooth.println("전방에 빠른 장애물");
    }

    delay(30);

    if (TFmini.measure()) {                 // 거리와 신호의 강도를 측정합니다. 성공하면 을 반환하여 if문이 작동합니다.
      if ((pos / 2) % 2 == 0) {
        distance1 = TFmini.getDistance();
        Serial.print(distance1);
        Serial.println("= D1");
      }
      if ((pos / 2) % 2 == 1) {
        distance2 = TFmini.getDistance();;
        Serial.print(distance2);
        Serial.println("= D2");
      }
      distance_abs = distance2 - distance1;
      Serial.println(distance_abs);

      if (abs(distance_abs) > 50) {
        distance_ld = min(distance1, distance2);
        if (pos > 0 && pos < 90) {
          Serial.print("장애물까지 거리는 ");
          Serial.print(distance_ld);
          Serial.println(" 우측 cm 입니다");
          Serial.print(pos);
          bluetooth.print(distance_ld);
          bluetooth.println("cm에 우측 전방 장애물");

        }
        if (pos > 90 && pos < 180) {
          Serial.print("장애물까지 거리는 ");
          Serial.print(distance_ld);
          Serial.println("좌측cm 입니다");
          Serial.print(pos);
          bluetooth.print(distance_ld);
          bluetooth.println("cm에 좌측 전방 장애물");

        }
        if (pos == 180) {
          Serial.print("장애물까지 거리는 ");
          Serial.print(distance_ld);
          Serial.println("cm 입니다");
          Serial.print(pos);
          bluetooth.print(distance_ld);
          bluetooth.println("cm에 전방 장애물");

        }
      }
    }

    delay(30);

    int fdist = 0;

    fdist = (int)sonar.ping_cm();

    if (fdist > 40 && fdist < 103) {
      Serial.print(fdist);
      Serial.println(" Cm");
      bluetooth.print(fdist);
      bluetooth.println("cm에 머리위 장애물");
    }
    servomotor.attach(9);

  }

  delay(30);


  for (pos = 180; pos > 0; pos -= 2) {
    servomotor.write(pos);
    delay(30);

    vmax = 0 ;
    while (digitalRead (Sigpin));
    while ( ! digitalRead (Sigpin)); //여기까지 전방 쪽 핀에 데이터 받는 부분

    T = pulseIn (Sigpin , HIGH) + pulseIn (Sigpin , LOW); // 주기 측정
    f = 1 / ( double ) T;            // 주파수 측정
    v = int ((f * 1e6 ) / 44.0 );   // 속도 측정
    vmax = max (v, vmax);       // 속도의 Max값 측정
    delay(30);

    servomotor.detach();

    if (vmax > 5)
    {
      Serial.print("속도");
      Serial.println(vmax);
      bluetooth.println("전방에 빠른 장애물");
    }

    delay(30);


    if (TFmini.measure()) {                 // 거리와 신호의 강도를 측정합니다. 성공하면 을 반환하여 if문이 작동합니다.
      if ((pos / 2) % 2 == 0) {
        distance1 = TFmini.getDistance();
        Serial.print(distance1);
        Serial.println("= D1");
      }
      if ((pos / 2) % 2 == 1) {
        distance2 = TFmini.getDistance();
        Serial.print(distance1);
        Serial.println("= D2");
      }
      distance_abs = distance2 - distance1;

      if (abs(distance_abs) > 50) {
        distance_ld = min(distance1, distance2);

        if (pos < 180 && pos > 90) {
          Serial.print("장애물까지 거리는 ");
          Serial.print(distance_ld);
          Serial.println("좌측 cm 입니다");
          Serial.print(pos);
          bluetooth.print(distance_ld);

          bluetooth.println("cm에 좌측 전방 장애물");

        }
        if (pos < 90 && pos > 0) {
          Serial.print("장애물까지 거리는 ");
          Serial.print(distance_ld);
          Serial.println("우측 cm 입니다");
          Serial.print(pos);
          bluetooth.print(distance_ld);
          bluetooth.println("cm에 우측 전방 장애물");

        }
        if (pos == 180) {
          Serial.print("장애물까지 거리는 ");
          Serial.print(distance_ld);
          Serial.println("cm 입니다");
          Serial.print(pos);
          bluetooth.print(distance_ld);
          bluetooth.println("cm에 전방 장애물");

        }
      }
    }


    delay(30);

    int fdist = 0;

    fdist = (int)sonar.ping_cm();

    if (fdist > 40 && fdist < 103) {
      Serial.print(fdist);
      Serial.println(" Cm");
      bluetooth.print(fdist);
      bluetooth.println("cm에 머리위 장애물");
    }
    servomotor.attach(9);

  }
}
