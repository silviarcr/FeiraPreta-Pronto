// cd Documentos/arduino/libraries
// git clone https://github.com/paolobueno/arduino-restclient.git
#include "RestClient.h"
#include <UIPEthernet.h>

const int qtdeLEDs = 5;
const int pinosLEDs[] = {7, 3, 4, 5, 6};
int intervalo_atualizacao = 2000;

// Enter a MAC address for your controller below.
byte mac[] = {0xDE, 0xED, 0xBA, 0xFE, 0xF1, 0x77};

EthernetClient client;
// TODO: substituir pelo dominio do backend
// ex: restClient("xxx.heroku.com", 80, client);
//RestClient restClient("agile-mountain-42100.herokuapp.com", 80, client);
RestClient restClient("feirapreta-001-site1.ctempurl.com", 80, client);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  if (Ethernet.begin(mac)) {
    Serial.println(Ethernet.localIP());
  } else {
    Serial.println(F("Falha na inicialização Ethernet"));
  }
}

void loop() {
  char resposta[32] = {};
  // TODO: substituir por caminho do endpoint de média das notas
  // ex: int statusCode = restClient.get("/media", resposta, 16);
  int statusCode = restClient.get("/api/eventscore/average", resposta, 32);
  if (statusCode == 200) {
    Serial.println(F("Recebi resposta"));
    String respostaString = String(resposta);

    int doisPontos = respostaString.indexOf(':');
    if (doisPontos > 0 && doisPontos < 31) {
      char voto = resposta[doisPontos + 1];
      Serial.println(voto);
      voto = voto - '0';
      atualizarLampadas(voto);
    }


  } else {
    Serial.println(F("Falhou!"));
    Serial.println(statusCode);
  }

  resposta[0] = 0;

  delay(intervalo_atualizacao);
}

void atualizarLampadas(int voto) {
  int ledsAAcender = 0;
  switch (voto) {
    case 1:
      ledsAAcender = 1;
      break;
    case 2:
      ledsAAcender = 2;
      break;
    case 3:
      ledsAAcender = 3;
      break;
    case 4:
      ledsAAcender = 4;
      break;
    case 5:
      ledsAAcender = 5;
      break;
  }

  for (int i = 0; i < qtdeLEDs; i++) {
    digitalWrite(pinosLEDs[i], i < ledsAAcender ? HIGH : LOW);
  }
}
