#include <Smoothed.h>
Smoothed<float> smoothedFilter;

double Q_angle = 0.001;   // Sürekli bileşen
double Q_bias = 0.003;    // Hızlanma sensöründen kaynaklanan hız sabiti
double R_measure = 0.03;  // Ölçüm hataları

double angle = 0;  // Süzülmüş açı
double bias = 0;   // Süzülmüş açı hızı

double P[2][2] = { { 0, 0 }, { 0, 0 } };  // Süzülmüş açı kovaryans matrisi
const int potPin = A0;

void setup() {
  Serial.begin(115200);

  // Smooth filtresini başlat
  smoothedFilter.begin(SMOOTHED_AVERAGE, 20);
}

void loop() {
  
  int rawValue = analogRead(potPin);
  double angleMeasurement = map(rawValue, 0, 1023, 0, 180);

  // Smooth filtresine yeni ölçümü ekle
  smoothedFilter.add(angleMeasurement);

  // Smooth filtrelenmiş açı değerini al
  float smoothed_angle = smoothedFilter.get();

  // Kalman filtresini güncelle
  updateKalman(smoothed_angle);
  
  Serial.print("Açı (Potansiyometre Değeri): ");
  Serial.print(angleMeasurement);
  Serial.print(" derece, Süzülmüş Açı (Smooth Filtre): ");
  Serial.print(smoothed_angle);
  Serial.print(" derece, Süzülmüş Açı (Kalman Filtre): ");
  Serial.print(angle);
  Serial.print(" derece");
  Serial.println();

  Serial.print(angleMeasurement);
  Serial.print(",");
  Serial.print(smoothed_angle);
  Serial.print(",");
  Serial.println(angle);

  delay(100);
}

void updateKalman(double newAngle) {
 
  double y = newAngle - angle;  // Yeni ölçüm ile süzülmüş açı arasındaki fark

  // Kalman kazancını hesapla
  double K[2];
  K[0] = P[0][0] / (P[0][0] + R_measure);
  K[1] = P[1][0] / (P[0][0] + R_measure);

  // Süzülmüş açı ve hızı güncelle
  angle += K[0] * y;
  bias += K[1] * y;

  // Süzülmüş açı kovaryans matrisini güncelle
  P[0][0] -= K[0] * P[0][0];
  P[0][1] -= K[0] * P[0][1];
  P[1][0] -= K[1] * P[1][0];
  P[1][1] -= K[1] * P[1][1];

  // Açı değeri kovaryans matrisini güncelle
  P[0][0] += Q_angle;
  P[0][1] += Q_angle;
  P[1][0] += Q_angle;
  P[1][1] += Q_angle;
}

// Bu kodlar Emre Konca Youtube Kanalının kodlarına ekleme yapılarak elde edilmiştir.
