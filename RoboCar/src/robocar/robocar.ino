// --- НАСТРОЙКИ ПИНОВ ---
const int S_L = PA0; // Левый датчик
const int S_R = PA1; // Правый датчик

const int ENA = PA8; // Скорость левого мотора
const int ENB = PA9; // Скорость правого мотора

const int IN1 = PB6; const int IN2 = PB7; // Левый мотор (OUT1/2)
const int IN3 = PB8; const int IN4 = PB9; // Правый мотор (OUT3/4)

const int BUZZER = PB0; // Твоя пищалка/индикатор

// --- НАСТРОЙКИ СКОРОСТИ (для 9V) ---
const int MAX_SPEED = 230;   // Скорость на прямой
const int TURN_SPEED = 250;  // Мощность в повороте
const int BRAKE_SPEED = 100; // Сила торможения (реверс колеса)

void setup() {
  pinMode(S_L, INPUT); pinMode(S_R, INPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // Сигнал готовности: пискнем один раз
  digitalWrite(BUZZER, HIGH); delay(100); digitalWrite(BUZZER, LOW);

  // Пауза 3 секунды, чтобы успеть поставить на линию
  delay(3000);
}

// Универсальная функция управления
void drive(int left, int right) {
  // Левый мотор
  if (left >= 0) {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    analogWrite(ENA, left);
  } else {
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
    analogWrite(ENA, -left);
  }

  // Правый мотор (Твоя инверсия из видео: LOW/HIGH вместо HIGH/LOW)
  if (right >= 0) {
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); 
    analogWrite(ENB, right);
  } else {
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    analogWrite(ENB, -right);
  }
}

void loop() {
  int L = digitalRead(S_L);
  int R = digitalRead(S_R);

  if (L == HIGH && R == HIGH) {
    // 1. НА ЛИНИИ: Едем вперед
    drive(MAX_SPEED, MAX_SPEED);
  } 
  else if (L == LOW && R == HIGH) {
    // 2. УШЛИ ВЛЕВО: Нужно повернуть ВПРАВО
    // Левое колесо вперед, правое — назад (тормозим)
    drive(TURN_SPEED, -BRAKE_SPEED);
  } 
  else if (L == HIGH && R == LOW) {
    // 3. УШЛИ ВПРАВО: Нужно повернуть ВЛЕВО
    // Правое колесо вперед, левое — назад
    drive(-BRAKE_SPEED, TURN_SPEED);
  } 
  else {
    // 4. ПОТЕРЯЛИ ЛИНИЮ: Стоп
    drive(0, 0);
  }
}
