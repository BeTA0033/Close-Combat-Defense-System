#include <Servo.h> 
#include <math.h>

#define trigPin 12 // Ultrasonik mesafe sensörünün trig bacağı için pin tanımlıyoruz
#define echoPin 13 // Ultrasonik mesafe sensörünün echo bacağı için pin tanımlıyoruz

Servo ServoMotor; 
int aci = 0;

int kirmiziPin = 5; // RGB'nin kırmızı led bacağı için pin tanımlıyoruz
int yesilPin = 10; // RGB'nin yeşil led bacağı için pin tanımlıyoruz
int maviPin = 11; // RGB'nin mavi led bacağı için pin tanımlıyoruz

int gIndex = 0; //Yeşil renk için başlangıç değerini 0 olarak belirtiyoruz
int bIndex = 0; //Mavi renk için başlangıç değerini 0 olarak belirtiyoruz
int rIndex = 0; //Kırmızı renk için başlangıç değerini 0 olarak belirtiyoruz

const int analogPin = A0; //LM-35'den veri almak için pin tanımladık
float gerilimDeger = 0;
float sensorDeger = 0;
float sicaklikDeger = 0;


long mesafeOlcumu(){
  long sure, distance; // Süreyi ve mesafeyi tanımlıyoruz
  float x,V;//LM35 için tanımlanan değişkenler.
  digitalWrite(trigPin, LOW);  // Sensör kapatıldı
  delayMicroseconds(5); // 5 ms beklenildi
  digitalWrite(trigPin, HIGH); // Sensöre dalga üretmesi için komut gönderildi
  delayMicroseconds(10); // 10 ms beklenildi
  digitalWrite(trigPin, LOW); // Yeni dalgaların üretilmemesi için tekrar kapatıldı.
  sure = pulseIn(echoPin, HIGH); // Süreyi hesaplıyoruz

  /* -----LM-35 Begin----- */
  sensorDeger = analogRead(analogPin); // LM-35 sensör degeri alındı.
  gerilimDeger = (sensorDeger/1023)*5000; // gerilim degeri hesaplandı.
  sicaklikDeger = gerilimDeger / 10.0; // sıcaklık degeri hesaplandı 
  delayMicroseconds(5); // 5 ms beklenildi
  
  x=(1+sicaklikDeger/273);
  V=331*(sqrt(x));// sesin havadaki yayılma hızını m/sn cinsinden ölçer
  V=V*100; // hızı m/sn'den cm/sn'ye çevirir.
  V=V/1000000;
  V=1/V;
  /* ------LM-35 End-------- */
  
  distance = (sure / 2) / V; // Ölçülen süre ile uzaklığı hesaplıyoruz
  return distance;
}

void led(long distance){
     int rIndex, gIndex, bIndex;
  if (distance >= 0 && distance < 100) //eğer mesafe 0-100 cm arasında ise,
  {
    rIndex = 255; // Kırmızı rengi 255 yap
    gIndex = 0; // Yeşil rengi 0 yap 
    bIndex = 0; // Mavi rengide 0 yap
    changeColour(rIndex, gIndex, bIndex);
    delay(500);
  }
  if (distance >= 100 && distance < 150) // eğer mesage 100-150 cm arasında ise,
  {
    rIndex = 255; // kırmızı rengi 255 yap
    gIndex = 255;// Yeşil rengi 255 yap 
    bIndex = 0; // mavi rengi 0 yap
    changeColour(rIndex, gIndex, bIndex);//sarı renk verir
    delay(500);
    
  }
  if (distance >= 150 && distance < 200) // eğer mesafe 150-200 cm arasında ise,
  {
    rIndex = 0; // Kırmızı rengi 0 yap
    gIndex = 255; // Yeşil rengi 255 yap
    bIndex = 0; // Mavi rengide 0 yap
    changeColour(rIndex, gIndex, bIndex);
    delay(500);
  }
  else // eğer yukarıdaki koşulların hiçbirini karşılamıyorsa,
  {
    rIndex = 0; //kırmızıyı sıfır yap
    gIndex = 0; // yeşili sıfır yap
    bIndex = 0; //maviyi sıfır yap
    changeColour(rIndex, gIndex, bIndex);
    delay(500);
  }
}

void changeColour(int kirmizi, int yesil, int mavi) //renk değişimleri için renkleri tanımlıyoruz
{
  kirmizi = 255 - kirmizi; //ölçülen kırmızı değerden yeni bir kırmızı elde edilerek sürekli kırmızı yanmasını sağlar, tek seferlik çalışmaz
  yesil = 255 - yesil; //ölçülen yesil  değerden yeni bir yeşil elde edilerek sürekli yeşişl yanmasını sağlar, tek seferlik çalışmaz
  mavi = 255 - mavi; //ölçülen mavi değerden yeni bir mavi elde edilerek sürekli mavi yanmasını sağlar, tek seferlik çalışmaz
  
  analogWrite(kirmiziPin, kirmizi); //kirmiziPin'de kırmızı rengin yandığını belirtiyoruz
  analogWrite(yesilPin, yesil); //yesilPin'de yeşil rengin yandığını belirtiyoruz
  analogWrite(maviPin, mavi); //maviPin'de mavi rengin yandığını belirtiyoruz
}

void setup(){
  //ultrasonik sensör üzerindeki trig pin sinyali gönderir, echo pin ise sinyali alır.
  //Yani Trig pin veri gönderdiği için output, Echo pin veri aldığı için inputtur. Aşağıda bunları belirliyoruz.
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(kirmiziPin, OUTPUT); //Kırmızı bacağın bağlı olduğu pini output olarak ayarlıyoruz.
  pinMode(yesilPin, OUTPUT); //Yeşil bacağın bağlı olduğu pini output olarak ayarlıyoruz.
  pinMode(maviPin, OUTPUT); //Mavi bacağın bağlı olduğu pini output olarak ayarlıyoruz.
  ServoMotor.attach(9);
  Serial.begin(9600); /* Seri haberleşme başlatıldı */
}

void loop()
{
   
  for(aci = 0; aci < 180; aci+=10) /* Motorumuzu 0'dan 180 kadar 10'ar 10'ar dönecektir */
  { 
    ServoMotor.write(aci); /* Motoru donduruyoruz */
    delay(15); /* Motorun donmesini bekliyoruz */
    led(mesafeOlcumu());
  } 
  for(aci = 180; aci>=1; aci-=10) /* Motorumuz 180'dan 0 kadar 10'ar 10'ar dönecektir */
  {                                
    ServoMotor.write(aci); /* Motoru donduruyoruz */
    delay(15); /* Motorun dönmesini bekliyoruz */
    led(mesafeOlcumu());
  } 
}

///Coding by Berke Temel ATAK
