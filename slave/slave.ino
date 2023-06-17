#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5);  // Cria uma porta serial virtual nos pinos digitais 4 (RX) e 5 (TX)
// No caso do nodemcu, tx = gpio5 = D1; rx = gpio4 = D2 -> entao, conectar rx ao pin d2, tx ao pin d1; no codigo, referenciar (4,5)

void setup() {
  Serial.begin(115200);  // Inicializa a comunicação serial padrão
  mySerial.begin(115200);  // Inicializa a porta serial virtual
}

void loop() {
  // Verifica se há dados disponíveis na porta serial padrão
  if (Serial.available()) {
    String mensagem = Serial.readString();  // Lê a mensagem recebida pela porta serial padrão

    // Realiza alguma ação com a mensagem recebida pela porta serial padrão
    Serial.print("Mensagem recebida (Porta Serial Padrão): ");
    Serial.println(mensagem);
  }

  // Verifica se há dados disponíveis na porta serial virtual
  if (mySerial.available()) {
    String mensagem = mySerial.readString();  // Lê a mensagem recebida pela porta serial virtual

    // Realiza alguma ação com a mensagem recebida pela porta serial virtual
    Serial.print("Mensagem recebida (Porta Serial Virtual): ");
    Serial.println(mensagem);
  }
}
