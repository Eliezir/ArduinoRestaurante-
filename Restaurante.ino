#include<Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <MD_MAX72xx.h>
#include <MD_Parola.h>
// Led RGB
const int vermelho = A3;
const int verde = A2;
// KeyPad Cliente
const uint8_t COLUNAS = 3;
const uint8_t LINHAS = 4;
uint8_t linPins[LINHAS] = { 9, 8, 7, 6 };
uint8_t colPins[3] = { 5, 4, 3};

char keys[LINHAS][COLUNAS] = 
{
  { 1, 2, 3},
  { 4, 5, 6},
  { 7, 8, 9},
  { 'X', '0', 'C'}
};

Keypad keypad = Keypad(makeKeymap(keys), linPins, colPins, LINHAS, COLUNAS);
// Display 
#define MAX_DEVICES 10
#define SPEED_TIME 70
#define PAUSE_TIME  500
#define CLK_PIN   13 
#define DATA_PIN  11 
#define CS_PIN    10 
MD_Parola mx = MD_Parola(MD_MAX72XX::PAROLA_HW, CS_PIN, MAX_DEVICES);

// Variaveis
int pedido;
int tela ;
String mesa = "8";
String pedidos [10] = {};
int num_pedidos = 0;
int itens;
int pedido_cozinha = 0;
int num_mesas = 0;
int tem_pedido = 0;

String PizzaP;
String BebidaP;
String TamanhoP;

// Init lcd
LiquidCrystal_I2C lcd(0x27,20,4);


// Cardapio
String bebida[] ={"Sem Bebida","Sprite","CocaCola","Suco Acerola","agua com gas"};
String sabor[]= {"Sem Pizza","Mucarela","Calabresa","Milho","Portuguesa","Brigadeiro"};
String tamanho[] = {"","Kids","Pequena","Media","Grande","Gigante"};

void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  mx.begin();
  mx.setTextAlignment(PA_CENTER);
    // Não sei o porquê, mas ele não funciona com o Serial ligado
  // Serial.begin(9600);
  // Serial.println("Aperte ✓ para confirmar o pedido");
  // Serial.println("Bebida: 0-Sem Bebida | 1-Sprite | 2-CocaCola | 3-Suco Acerola | 4-Água com gás");
  // Serial.println("Sabor Pizzas: 0-Sem Pizza | 1-Muçarela | 2- Calabresa | 3- Milho | 4-Portuguesa | 5-Brigadeiro");
  // Serial.println("Tamanho Pizzas: 1-KIDS | 2-Pequena | 3-Média | 4-Grande | 5-Gigante");

  lcd.init();
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Bem-Vindo!");
  lcd.setCursor(3.,1);
  lcd.print("Mesa:");
  lcd.setCursor(8,1);
  lcd.print(mesa);
  delay(1000);
  lcd.clear();
  tela = 0;

}

void tecla(){
  lcd.setCursor(10,1);
  lcd.print(pedido);}

void print_tela(String texto){
  if(tela == 0){lcd.setCursor(3,0);}
  else{lcd.setCursor(0,0);}
  lcd.print(texto);
  lcd.setCursor(3,1);
  lcd.print("Pedido:");
}

void pisca_led(int pin){
    analogWrite(pin,255);
    delay(1000);
    analogWrite(pin,0);}

void confirma_pedido(){
    tela ++;
    pedido = 0;
    pisca_led(verde);
}
void faz_pedido(String bebi,String Piz, String Tam){
  if(num_pedidos > 6){
    num_pedidos = 0;
    pedido_cozinha = 0;
    num_mesas = 0;
    tem_pedido = 0;}
  pedidos[num_pedidos] = "Mesa: " + mesa;
  num_pedidos ++;
  pedidos[num_pedidos] = bebi;
  num_pedidos ++;
  pedidos[num_pedidos] = Piz;
    num_pedidos ++;
  pedidos[num_pedidos] = Tam;
  num_pedidos ++;
}


void loop() {
  //funcoes do keypad cliente
  char key = keypad.getKey();
  if (key != NO_KEY ) {
    if(key == 'X' ){
      pedido = 0;
      lcd.clear();}

    else if(key == 'C'){
      if(tela == 0 && pedido >= 0 && pedido <5 ){
            BebidaP = bebida[pedido];
        itens = 1;
      confirma_pedido();}
      else if(tela ==1 && pedido >= 0 && pedido <6){
          PizzaP = sabor[pedido];
        itens = 2;
        if(pedido == 0){tela ++;}
          confirma_pedido();}
      else if(tela ==2 && pedido >= 1 && pedido <6){
          TamanhoP = tamanho[pedido];
        itens = 3;
      confirma_pedido();}
      else{pisca_led(vermelho);}
      lcd.clear();}
    
    else{
      if(key == '0'){key = 0;}
      

      if(pedido == 0){pedido += key;}

      else if(pedido >= 1 && pedido <100)
         {pedido *= 10;
          pedido += key;}

      else{
      pisca_led(vermelho);}
    }}
    // Tela lcd cliente 
    tecla();
    switch(tela){
    case 0: //bebida
    print_tela("Bebida");
    break;
    case 1: // Sabor
    print_tela("Sabor da Pizza");
    break;
    case 2: //Tamanho Pizza
    print_tela("Tamanho Pizza");
    break;}
    // Adicionar o pedido ao display da cozinha
    if(tela == 3){
    if(itens == 1 || itens == 2){
      TamanhoP = "Sem Pizza";
      if(BebidaP == "Sem Bebida")
      {pisca_led(vermelho);
      pedido = 0;
      itens = 0;
      tela = 0;}}
      if(BebidaP != "Sem Bebida" || PizzaP != "Sem Pizza"){
        faz_pedido(BebidaP,PizzaP,TamanhoP);
        //mx.print(pedidos[0]);
        tem_pedido += 1;
        pedido = 0;
        itens = 0;
        tela = 0;}
    }
    if(tem_pedido == 0 || num_mesas / 3 == tem_pedido){
      mx.print("Sem pedidos");
    }
    else{
    //Printar o pedido no display
    mx.print(pedidos[pedido_cozinha]);
    delay(1000);
    pedido_cozinha++;
    if(pedido_cozinha > num_mesas + 3)
    {pedido_cozinha = num_mesas;}
    }
  //Botão Cozinha para passar para o proximo pedido
  int estBotao = HIGH;
  int valor = digitalRead(2);
  if (estBotao != valor) {
    estBotao = valor;
    if (valor == LOW) {
      if(num_mesas / 4 < tem_pedido){
      num_mesas +=4;
      pedido_cozinha = num_mesas;}
    }
  }



}
  


