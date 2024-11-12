byte const LED_COMER = 11;
byte const LED_BRINCAR = 12;
byte const LED_DORMIR = 13;
const int BOTAO_COMER = 2, BOTAO_BRINCAR = 3, BOTAO_DORMIR = 4;
const long CONVERTER_EM_MILISEGUNDOS = 20000;
const float VALOR_MAX_LDR = 1023 * (2.0 / 3.0);

int contador, alterar_brilho, direcao, soma_ldr;
long debouncing_comer_tempo, debouncing_brincar_tempo, debouncing_fadiga_tempo, comer_tempo, brincar_tempo, fadiga_tempo, dormir_tempo, countdown_comer, countdown_brincar, countdown_dormir, countdown_ldr, countdown_retirar_pontos_comer, countdown_retirar_pontos_brincar, countdown_retirar_pontos_dormir;
bool dormir, vivo, ldr_adormecer;
unsigned long manter_valores_dormir;
float random_num_comer, random_num_brincar, random_num_dormir, media_ldr;
unsigned long delay_com_millis,delay_com_micros;
int resultados_ldr[6];

void setup() {
  Serial.begin(9600);
  pinMode(BOTAO_COMER, INPUT_PULLUP);
  pinMode(BOTAO_BRINCAR, INPUT_PULLUP);
  pinMode(BOTAO_DORMIR, INPUT_PULLUP);
  for (int i=11; i<=13; i++) {
    pinMode(i, OUTPUT);
  }
  randomSeed(analogRead(0));
  debouncing_comer_tempo = millis();
  debouncing_brincar_tempo = millis();
  debouncing_fadiga_tempo = millis();
  comer_tempo = millis();
  brincar_tempo = millis();
  fadiga_tempo = millis();

  countdown_comer = millis();
  countdown_brincar = millis();
  countdown_dormir = millis();

  countdown_ldr = millis();

  random_num_comer = random(0, 121)/60.00;
  random_num_brincar = random(0, 121)/60.00;
  random_num_dormir = random(0, 121)/60.00;

  dormir = false;
  vivo = true;
  direcao = 1;
  alterar_brilho = 1;
  contador = 0;
  soma_ldr = 0;
  media_ldr = 0;
  manter_valores_dormir = 0;
  ldr_adormecer = false;
  
  for (int i = 0; i<6; i++) {
    resultados_ldr[i] = 0;
  }
}

void delay_millis(float tempo_para_delay) {
  delay_com_millis = millis();
  while (millis() - delay_com_millis < tempo_para_delay) {
    // Não fazer nada
  }
}

void delay_micros(float tempo_para_delay) {
  delay_com_micros = micros();
  while (micros() - delay_com_micros < tempo_para_delay) {
    // Não fazer nada
  }
}
  
void loop() {
  if (vivo == true) {
    if (dormir == false) {
      if (millis() - countdown_ldr >= CONVERTER_EM_MILISEGUNDOS) {
        for (int i = 5; i>0;i--) {
          resultados_ldr[i] = resultados_ldr[i-1];
        }
        resultados_ldr[0] = analogRead(A0);
        countdown_ldr = millis();
      }
      for (int i = 0; i<6;i++) {
        soma_ldr += resultados_ldr[i];
      }
      media_ldr = soma_ldr/6.00;
      soma_ldr = 0;
      if ((resultados_ldr[0] > 0) && (resultados_ldr[1] > 0) && (resultados_ldr[2] > 0)&& (resultados_ldr[3] > 0)&& (resultados_ldr[4] > 0)&& (resultados_ldr[5] > 0)&&(media_ldr > VALOR_MAX_LDR)) {
        ldr_adormecer = true;
      } else {
        ldr_adormecer = false;
      }
      
      if ((millis() - comer_tempo >= (3+random_num_comer)*CONVERTER_EM_MILISEGUNDOS)&&(digitalRead(LED_COMER)==LOW)) {
        digitalWrite(LED_COMER, HIGH);
        countdown_comer = millis();
        countdown_retirar_pontos_comer = millis();
      }
      if ((millis() - brincar_tempo >= (2+random_num_brincar)*CONVERTER_EM_MILISEGUNDOS)&&(digitalRead(LED_BRINCAR)==LOW)) {
        digitalWrite(LED_BRINCAR, HIGH);
        countdown_brincar = millis();
        countdown_retirar_pontos_brincar = millis();
      }
      if (((millis() - fadiga_tempo >= (9+random_num_dormir)*CONVERTER_EM_MILISEGUNDOS)&&(digitalRead(LED_DORMIR)==LOW))||((ldr_adormecer)&&(digitalRead(LED_DORMIR)==LOW))) {
        digitalWrite(LED_DORMIR, HIGH);
        countdown_dormir = millis();
        countdown_retirar_pontos_dormir = millis();
      }

      if ((millis() - countdown_comer >= CONVERTER_EM_MILISEGUNDOS)&&(digitalRead(LED_COMER)==HIGH)) {
        countdown_comer = millis();
        contador += 5;
        Serial.println("Adicionado 5 pontos ao contador. Total:");
        Serial.println(contador);
      }
      if ((millis() - countdown_brincar >= CONVERTER_EM_MILISEGUNDOS)&&(digitalRead(LED_BRINCAR)==HIGH)) {
        countdown_brincar = millis();
        contador += 5;
        Serial.println("Adicionado 5 pontos ao contador. Total:");
        Serial.println(contador);
      }
      if ((millis() - countdown_dormir >= CONVERTER_EM_MILISEGUNDOS)&&(digitalRead(LED_DORMIR)==HIGH)) {
        countdown_dormir = millis();
        contador += 5;
        Serial.println("Adicionado 5 pontos ao contador. Total:");
        Serial.println(contador);
      }

      int BOTAO_COMERState = digitalRead(BOTAO_COMER);
      int BOTAO_BRINCARState = digitalRead(BOTAO_BRINCAR);
      int BOTAO_DORMIRState = digitalRead(BOTAO_DORMIR);

      if ((BOTAO_COMERState == LOW)&&(millis() - debouncing_comer_tempo > 200)) {
            if ((millis() - countdown_retirar_pontos_comer < 15*1000)&&(contador>=5)) {
              contador -= 5;
              Serial.println("Retirados 5 pontos ao contador. Total:");
              Serial.println(contador);
            }
            debouncing_comer_tempo = millis();
            digitalWrite(LED_COMER, LOW);
            random_num_comer = random(0, 121)/60.00;
            comer_tempo = millis();
      }
      if ((BOTAO_BRINCARState == LOW)&&(millis() - debouncing_brincar_tempo > 200)) {
            if ((millis() - countdown_retirar_pontos_brincar < 15*1000)&&(contador>=5)) {
              contador -= 5;
              Serial.println("Retirados 5 pontos ao contador. Total:");
              Serial.println(contador);
            }
            debouncing_brincar_tempo = millis();
            digitalWrite(LED_BRINCAR, LOW);
            random_num_brincar = random(0, 121)/60.00;
            brincar_tempo = millis();
      }
      if ((BOTAO_DORMIRState == LOW)&&(millis() - debouncing_fadiga_tempo > 200)) {
            if ((millis() - countdown_retirar_pontos_dormir < 15*1000)&&(contador>=5)) {
              contador -= 5;
              Serial.println("Retirados 5 pontos ao contador. Total:");
              Serial.println(contador);
            }
            debouncing_fadiga_tempo = millis();
            digitalWrite(LED_DORMIR, LOW);
            dormir = true;
            random_num_dormir = random(0, 121)/60.00;
            dormir_tempo = millis();
      }
      if (contador > 25) {
        vivo = false;
        Serial.println("O Ardugotchi morreu! Para o ressuscitar pressiona nos três botões por mais ou menos 2 segundos.");
      }
    } else if (dormir == true) {
      Serial.println("Dormindo... ZZZ");
      fadiga_tempo = 0;
      digitalWrite(LED_COMER, LOW);
      digitalWrite(LED_BRINCAR, LOW);
      
      digitalWrite(LED_DORMIR, HIGH);
      delay_micros(alterar_brilho +1);
      if (alterar_brilho < 100) {
        alterar_brilho += direcao;
      } else {
        alterar_brilho -= 1;
        direcao = -1;
      }
      if (alterar_brilho == 1) {
        alterar_brilho += 1;
        direcao = 1;
      }
      digitalWrite(LED_DORMIR, LOW);
      if (millis() - dormir_tempo >= 5*CONVERTER_EM_MILISEGUNDOS) {
        dormir = false;
        Serial.println("Acordei!");
        direcao = 1;
        alterar_brilho = 1;
        manter_valores_dormir = (millis() - dormir_tempo);
        comer_tempo += manter_valores_dormir;
        brincar_tempo += manter_valores_dormir;
        countdown_comer += manter_valores_dormir;
        countdown_brincar += manter_valores_dormir;

        fadiga_tempo = millis();
        for (int i = 0; i<5; i++) {
          resultados_ldr[i] = 0;
        }
      }
    }
  } else {
    for (int i=11; i<=13; i++) {
      digitalWrite(i, HIGH);
    }
    delay_millis(1000);
    for (int i=11; i<=13; i++) {
      digitalWrite(i, LOW);
    }
    delay_millis(1000);
    if ((digitalRead(BOTAO_COMER)==LOW)&&(digitalRead(BOTAO_BRINCAR)==LOW)&&(digitalRead(BOTAO_DORMIR)==LOW)) {
      vivo = true;
      contador = 0;
      manter_valores_dormir = 0;
      delay_millis(2*1000);
      for (int i=11; i<=13; i++) {
        digitalWrite(i, HIGH);
        delay_millis(0.5*1000);
      }
      for (int i=11; i<=13; i++) {
        digitalWrite(i, LOW);
        delay_millis(0.5*1000);
      }
      setup();
    }
  }
}