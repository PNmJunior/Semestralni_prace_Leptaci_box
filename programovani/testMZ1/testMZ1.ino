void setup() {
  // put your setup code here, to run once:
pinMode(17,OUTPUT);
pinMode(16,OUTPUT);
pinMode(15,OUTPUT);
pinMode(14,OUTPUT);
}

void u(bool a, bool b, bool c, bool d, int t)
{
  digitalWrite(17,a);
  digitalWrite(16,b);
  digitalWrite(15,c);
  digitalWrite(14,d);
  delay(t);
}

void loop() {
  // put your main code here, to run repeatedly:
int t = 10;

u(1,0,1,0,t);
u(0,1,1,0,t);
u(0,1,0,1,t);
u(1,0,0,1,t);
}


//https://eluc.kr-olomoucky.cz/verejne/lekce/809
