/*
Watchdog on the Arduino Uno

*/

#define D_COUNTER 50
const int outVenus=2; // pin number for telling Venus the leader's state
const int outSaturne=3; // pin number for telling Saturne the leader's state
const int inVenus=4; // pin number for getting "I'm alive" message from Venus
const int inSaturne=5; // pin number for getting "I'm alive" message from Saturne
const int greenLEDVenus = 8; // pin number for the LED indicating that Venus is Leader
const int redLEDVenus = 9; // pin number for the LED indicating that Venus is not Leader (dead or follower)
const int greenLEDSaturne = 10; // pin number for the LED indicating that Saturne is Leader
const int redLEDSaturne = 11; // pin number for the LED indicating that Saturne is not Leader (dead or follower)

int idLeader; // 0 for none, 1 for Venus, 2 pour Saturne
int counter;


void setup() {
    idLeader=0;
    counter=0;
    
    pinMode(outVenus, OUTPUT); // Setting output to Venus (1) - watchdog state : HIGH = leader alive, LOW = leader dead
    pinMode(outSaturne, OUTPUT); // Setting output to Saturne (2) - watchdog state : HIGH = leader alive, LOW = leader dead
    pinMode(inVenus, INPUT); // Setting pin input from Venus (1) - impulse "i'm alive" (HIGH)
    pinMode(inSaturne, INPUT); // Setting pin input from Saturne (2) - impulse "i'm alive" (HIGH)
    pinMode(greenLEDVenus, OUTPUT); // Setting green LED for Venus (1)
    pinMode(redLEDVenus, OUTPUT); // Setting red LED for Venus (1)
    pinMode(greenLEDSaturne, OUTPUT); // Setting green LED for Saturne (2)
    pinMode(redLEDSaturne, OUTPUT); // Setting red LED for Saturne (2)
    

    // Value initialisation
    digitalWrite(outVenus,LOW); // to Venus : Saturne is not leader
    digitalWrite(outSaturne,LOW); // to Saturne : Venus is not leader
    // WARNING : for the LED HIGH = level 0, LOW = level 1 (negative logic)
    digitalWrite(greenLEDVenus, HIGH);
    digitalWrite(redLEDVenus, HIGH);
    digitalWrite(greenLEDSaturne, HIGH);
    digitalWrite(redLEDSaturne, HIGH);

}


void loop() {
    
 switch (idLeader) {   
    case 0: // No leader, "initialisation" and recovery
        delay(1);
        if (digitalRead(inVenus)==HIGH) { // Venus changed its mode to Leader
            idLeader=1; // Leader = Venus
            digitalWrite(outSaturne,HIGH); // sending a signal to Saturne saying the leader is alive
            digitalWrite(outVenus,LOW); // reseting the signal to Venus (unused when leader)
            digitalWrite(greenLEDVenus, LOW);
            digitalWrite(redLEDVenus, HIGH);
            
        } else if (digitalRead(inSaturne)==HIGH) { // Saturne changed its mode to Leader
            idLeader=2; // Leader = Saturne
            digitalWrite(outVenus,HIGH); // sending a signal to Venus saying the leader is alive
            digitalWrite(outSaturne,LOW); // reseting the signal to Saturne (unused when leader)
            digitalWrite(greenLEDSaturne, LOW);
            digitalWrite(redLEDSaturne, HIGH);
        }
    break; 
         
    case 1: // Leader = Venus
         // Waiting until the "I'm alive" signal, or the end of the counter
        while ((counter<D_COUNTER) && (digitalRead(inVenus)==LOW)) {
            delay(1); // delay 1ms
            counter=counter+1;
        }
         
        if (counter==D_COUNTER) { // leader dead (timeout, end of the counter)
            digitalWrite(greenLEDVenus, HIGH);
            digitalWrite(redLEDVenus, LOW);
            digitalWrite(outSaturne,LOW);
            digitalWrite(outVenus,HIGH);
            idLeader=0;
        } else { // leader alive ("I'm alive" signal received)
            //digitalWrite(outSaturne,HIGH);
            digitalWrite(greenLEDVenus, LOW);
        } 
    break;
         
    case 2: // Leader = Saturne
         // Waiting until the "I'm alive" signal, or the end of the counter
        while ((counter<D_COUNTER) && (digitalRead(inSaturne)==LOW)) {
            delay(1); // delay 1ms
            counter=counter+1;
        }
         
        if (counter==D_COUNTER) { // leader dead (timeout, end of the counter)
            digitalWrite(greenLEDSaturne, HIGH);
            digitalWrite(redLEDSaturne, LOW);
            digitalWrite(outVenus,LOW);
            digitalWrite(outSaturne,HIGH);
            idLeader=0;
        } else { // leader alive ("I'm alive" signal received)
            //digitalWrite(outVenus,HIGH);
            digitalWrite(greenLEDSaturne, LOW);
        }    
        break;
    
    default:
        break;
  }
    
    counter=0; // counter reset
    
    
}
