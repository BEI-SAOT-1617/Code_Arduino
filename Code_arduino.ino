/*
Watchdog

*/

#define D_COUNTER 50
const int outVenus=2;
const int outSaturne=3;
const int inVenus=4;
const int inSaturne=5;
const int greenLEDVenus = 8;
const int redLEDVenus = 9;
const int greenLEDSaturne = 10;
const int redLEDSaturne = 11;

int idLeader; // 0 pour rien, 1 pour Venus, 2 pour Saturne
int counter;


void setup() {
    idLeader=0;
    counter=0;
    
    pinMode(outVenus, OUTPUT); // Sortie vers Venus (1) - état watchdog : HIGH leader vivant, LOW leader mort
    pinMode(outSaturne, OUTPUT); // Sortie vers Saturne (2) - état watchdog : HIGH leader vivant, LOW leader mort
    pinMode(inVenus, INPUT); // Entrée depuis Venus (1) - signal impulsionnel "i'm alive" (HIGH)
    pinMode(inSaturne, INPUT); // Entrée depuis Saturne (2) - signal impulsionnel "i'm alive" (HIGH)
    pinMode(greenLEDVenus, OUTPUT); // LED verte Venus (1)
    pinMode(redLEDVenus, OUTPUT); // LED rouge Venus (1)
    pinMode(greenLEDSaturne, OUTPUT); // LED verte Saturne (2)
    pinMode(redLEDSaturne, OUTPUT); // LED rouge Saturne (2)
    
    // HIGH = niveau 0, LOW = niveau 1
    digitalWrite(outVenus,HIGH);
    digitalWrite(outSaturne,HIGH);
    digitalWrite(greenLEDVenus, HIGH);
    digitalWrite(redLEDVenus, HIGH);
    digitalWrite(greenLEDSaturne, HIGH);
    digitalWrite(redLEDSaturne, HIGH);
    // à régler : problème si les deux passent en LEADER...
}


void loop() {
    
 switch (idLeader) {   
    case 0: // Aucun leader, "initialisation"
        delay(1);
        if (digitalRead(inVenus)==HIGH) {
            idLeader=1;
            digitalWrite(outSaturne,HIGH);
            digitalWrite(outVenus,LOW);
            digitalWrite(greenLEDVenus, LOW);
            digitalWrite(redLEDVenus, HIGH);
            
        } else if (digitalRead(inSaturne)==HIGH) {
            idLeader=2;
            digitalWrite(outVenus,HIGH);
            digitalWrite(outSaturne,LOW);
            digitalWrite(greenLEDSaturne, LOW);
            digitalWrite(redLEDSaturne, HIGH);
        }
    break; 
    case 1:
        while ((counter<D_COUNTER) && (digitalRead(inVenus)==LOW)) {
            delay(1); // attente 1ms
            counter=counter+1;
        }
        if (counter==D_COUNTER) { // leader mort
            digitalWrite(greenLEDVenus, HIGH);
            digitalWrite(redLEDVenus, LOW);
            digitalWrite(outSaturne,LOW);
            digitalWrite(outVenus,HIGH);
            idLeader=0;
        } else { // leader vivant
            //digitalWrite(outSaturne,HIGH);
            digitalWrite(greenLEDVenus, LOW);
        } 
    break;
    case 2:
        while ((counter<D_COUNTER) && (digitalRead(inSaturne)==LOW)) {
            delay(1); // attente 1ms
            counter=counter+1;
        }
        if (counter==D_COUNTER) { // leader mort
            digitalWrite(greenLEDSaturne, HIGH);
            digitalWrite(redLEDSaturne, LOW);
            digitalWrite(outVenus,LOW);
            digitalWrite(outSaturne,HIGH);
            idLeader=0;
        } else { // leader vivant
            //digitalWrite(outVenus,HIGH);
            digitalWrite(greenLEDSaturne, LOW);
        }    
        break;
    
    default:
        break;
  }
    
    counter=0;
    
    
}
