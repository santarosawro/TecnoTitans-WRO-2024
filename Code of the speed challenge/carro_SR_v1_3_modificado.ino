/*---------- DEFINICION DE LIBRERIAS, CONSTANTES Y VARIABLES ----------*/
#include <Servo.h>

/*---------- DATOS DE LOS SENSORES ULTRASONICOS ----------*/
//sonar frontal
const int f_trigPin = 7;  
const int f_echoPin = 6;  
//sonar derecho
const int r_trigPin = 11; 
const int r_echoPin = 10;
//sonar izquierdo
const int l_trigPin = 13;
const int l_echoPin = 12;
long duration_f, duration_r, duration_l, distance_f, distance_r, distance_l;  // variables de duraciones y distancias de los hcsr04

const int sampleNum = 5;        // numero de muestras a tomar para filtrar distancias derecha e izquierda
int sonarSample_r[sampleNum];   // muestras de sensor derecho
int sonarSample_l[sampleNum];   // muestras de sensor izquierdo
long distance_rFiltered;        // distancia derecha filtrada
long distance_lFiltered;        // distancia izquierda filtrada

long critDistance_f = 25; // distancia critica frontal | 30 para alto y 30 para bajo
long critDistance_r = 25; // distancia critica derecha | 20 para alto y 25 para bajo prev 15
long critDistance_l = 25; // distancia critica izquierda |20 para alto y 25 para bajo prev 15

/*----------- DATOS DEL SENSOR DE COLORES -----------*/
#define S0 4
#define S1 A3	
#define S2 A4	
#define S3 A5	
#define sensorOut 8	

int redMin = 23; // Red minimum value | alto: 23 bajo: 57
int redMax = 156; // Red maximum value  | alto: 156 bajo: 160
int greenMin = 25; // Green minimum value | alto: 25 bajo: 63
int greenMax = 193; // Green maximum value | alto: 193 bajo: 230
int blueMin = 8; // Blue minimum value | alto: 8 bajo: 53
int blueMax = 54; // Blue maximum value  | alto: 54 bajo: 198

// Variables for Color Pulse Width Measurements
int redPW = 0;
int greenPW = 0;
int bluePW = 0;

// Variables for final Color values
int redValue;
int greenValue;
int blueValue;

// color timers
// readings delay timer  
unsigned long color_tnow;       // tiempo actual del cronometro para detectar color
unsigned long color_tset = 25; // tiempo objetivo del cronometro para detectar color
bool color_delayGo = true;  // delay entre mediciones de colores
int colorReadOrder = 0;     // variable para determinar el orden de que color detectar
bool colorRead_complete = false;  // variable para saber si todos los colres fueron leidos
bool searchingLine = true;  // variable para entrar en la funcion de detectar color de linea
int lineFound = 0;  // contador de lineas detectadas
bool go_searchLine = true;  // para entrar a chequear por la linea
unsigned long searchLine_tnow;  // tiempo actual del cronometro para entrar a buscar la linea
unsigned long searchLine_tset = 3000; // tiempo objetivo del cronometro para entrar a buscar la linea
bool go_endOp = true; // variable para actualizar el cronometro del final
unsigned long end_tnow; // tiempo actual del cronometro para el final
unsigned long end_tset = 1000; // tiempo objetivo del cronometro para el al final

/*---------- DATOS DEL MOTOR DC ----------- */
#define dc_1 2        // pin de direccion 1 del motor
#define dc_2 3        // pin de direccion 2 del motor
#define dc_speedPin 5 // pin de velocidad del motor 

const int dc_max_speed_drive = 160; // velocidad maxima del motor dc   alto: 230 bajo:250
const int dc_max_speed_turn = 250; // velocidad maxima del motor dc
int t_turn = 1300;  // tiempo de giro | 1300 para alto y 1300 para bajo
int t_back = 1000;  // tiempo de retroceso 1000 para alto y 1500 para bajo
bool clock_wise = false;  // variable para saber la direccion de giro
bool first_turn = true;   // variable para saber si no se da el primer giro
int phantom_distance = 7;  // distancia fantasma para corregir vueltas

/*----------- DATOS DE LOS SENSORES INFRARROJOS ----------*/
#define irSensor_dl A1
#define irSensor_dr A2

bool left_alert = true;
bool right_alert = true;
bool diagonalLeft_alert = true;
bool diagonalRight_alert = true;

/*---------- CRONOMETROS DE SENSORES Y OPERACION DE GIRO -----------*/
unsigned long sensors_tnow;
unsigned long sensors_tset = 100;
bool sensors_on = true;
bool turning = false; 

unsigned long turn_tnow;
unsigned long turn_tset = 500;
bool turn_on = true;

/*---------- DATOS DEL SERVOMOTOR -----------*/
Servo servo_dir;
int diagonalCorrection_angle = 0; // valor para correccion de angulos en deteccion diagonal alto: 5 bajo: 0 
int angle_right = 120;  //120 para alto y  para bajo
int angle_rightCorr = angle_right - diagonalCorrection_angle;
int angle_left = 70;    // 50 para alto y   para bajo
int angle_leftCorr = angle_left + diagonalCorrection_angle;
int angle_softRight = 120;  // 100 para alto y  para bajo
int angle_softLeft = 70;    // 70 para alto y  para bajo
int angle_straight = 95; 
bool diagonalTurn = false;  // variable para saber si se detecto un obstaculo en diagonal

/*---------- BOTON DE ARRANQUE -----------*/
const int startButton = A0;            // pin del boton de inicio
int buttonState_start;                // estado actual del boton start
int lastButtonState_start = LOW;      // estado previo del boton start
unsigned long lastDebounceTime = 0;   // ultima vez que cambio el estado de los botones
unsigned long debounceDelay = 50;     // tiempo de espera para el rebote del boton
bool start = true;           // estado actual del proceso de arranque
bool start_prev = true;      // estado previo del proceso de arranque
bool start_process = false;  // variable para iniciar el arranque

/*----------- SETUP -----------*/
void setup() {
  Serial.begin(9600);

  pinMode(startButton, INPUT_PULLUP);
  pinMode(f_trigPin, OUTPUT); 
  pinMode(f_echoPin, INPUT); 
  pinMode(r_trigPin, OUTPUT); 
  pinMode(r_echoPin, INPUT); 
  pinMode(l_trigPin, OUTPUT); 
  pinMode(l_echoPin, INPUT);

  // Set S0 - S3 as outputs
	pinMode(S0, OUTPUT);
	pinMode(S1, OUTPUT);
	pinMode(S2, OUTPUT);
	pinMode(S3, OUTPUT);

	// Set Sensor output as input
	pinMode(sensorOut, INPUT);

	// Set Frequency scaling to 20%
	digitalWrite(S0,HIGH);
	digitalWrite(S1,LOW);

  pinMode(dc_1, OUTPUT);
  pinMode(dc_2, OUTPUT);
  pinMode(dc_speedPin, OUTPUT);
  digitalWrite(dc_1, LOW);
  digitalWrite(dc_2, LOW);
  analogWrite(dc_speedPin, 0);
  servo_dir.attach(9);
  servo_dir.write(angle_straight);
  delay(1000);
}

/*---------- RUTINA PARA EL BOTON DE ARRANQUE -----------*/
void buttonPressStart(){
  int reading = digitalRead(startButton); // registra la lectura como el valor recibido por el boton
  if(reading != lastButtonState_start){   // si la lectura es diferente a la previa registrada
    lastDebounceTime = millis();          // da el valor de tiempo transcurrido al tiempo del rebote del boton
  }
  if((millis() - lastDebounceTime) > debounceDelay){  // si el tiempo transcurrido es mayor al tiempo limite del rebote
    if(reading != buttonState_start){                 // y si la lectura actual es diferente a la previa
      buttonState_start = reading;    // iguala el estado actual del boton a la lectura actual 
      if(buttonState_start == LOW){   // y revisa si el estado del boton es BAJO (0)
        start = !start;    // si es cierto entonces cambia el estado de la variable start, la cual es quien interactua con las demas funciones del sistema
      }
    }
  }
  lastButtonState_start = reading;  // finalmente iguala el estado previo del boton a la lectura actual para resetear el sistema y repetir 
}

/*---------- RUTINAS DE ULTRASONICOS FRONTAL, DERECHO Y IZQUIERDO ----------*/
// FRONTAL
void sonar_f(){
  digitalWrite(f_trigPin, LOW);
  delay(5);
  digitalWrite(f_trigPin, HIGH);
  delay(10);
  digitalWrite(f_trigPin, LOW); 
  duration_f = pulseIn(f_echoPin, HIGH); 
  distance_f = (duration_f*0.034)/2;
}
// DERECHO
void sonar_r(){
  digitalWrite(r_trigPin, LOW);
  delay(5);
  digitalWrite(r_trigPin, HIGH);
  delay(10);
  digitalWrite(r_trigPin, LOW); 
  duration_r = pulseIn(r_echoPin, HIGH); 
  distance_r = (duration_r*0.034)/2;
}
// IZQUIERDO
void sonar_l(){
  digitalWrite(l_trigPin, LOW);
  delay(5);
  digitalWrite(l_trigPin, HIGH);
  delay(10);
  digitalWrite(l_trigPin, LOW); 
  duration_l = pulseIn(l_echoPin, HIGH); 
  distance_l = (duration_l*0.034)/2;
}
/*---------- OPERACIONES DE LOS SONARES (ULTRASONICOS) ----------*/

// FILTRO PARA LAS DISTANCIAS MEDIDAS EN LOS LATERALES DEL VEHICULO
void sonarFilter(){
  for(int i = 0; i < sampleNum; i++){
    sonar_r();
    sonarSample_r[i] = distance_r;
    delay(30);
  }
  distance_rFiltered = 0;
  for(int j = 0; j < sampleNum; j++){
    distance_rFiltered = distance_rFiltered + sonarSample_r[j];
  }
  distance_rFiltered = distance_rFiltered / sampleNum; 
  delay(30);
  for(int i = 0; i < sampleNum; i++){
    sonar_l();
    sonarSample_l[i] = distance_l;
    delay(30);
  }
  distance_lFiltered = 0;
  for(int j = 0; j < sampleNum; j++){
    distance_lFiltered = distance_lFiltered + sonarSample_l[j];
  }
  distance_lFiltered = distance_lFiltered / sampleNum; 
  delay(30);
  
  // Serial.print("R:");
  // Serial.print(distance_rFiltered);
  // Serial.print(" | L:");
  // Serial.println(distance_lFiltered);
  // delay(30);
}

// RUTINA PARA LA DETECCION DE OBSTACULOS FRONTALES Y LATERALES 
void sonarOp_1(){
  if(sensors_on){
    sensors_on = false;
    sensors_tnow = millis();
  }
  if((millis() - sensors_tnow) > sensors_tset){
    //correccion de posicion por us
    sonar_r();
    sonar_l();
  
    diagonalRight_alert = digitalRead(irSensor_dr);
    diagonalLeft_alert = digitalRead(irSensor_dl);
    
    sonar_f();

    //Serial.println(distance_f);
    //if(distance_f < critDistance_f || !diagonalRight_alert || !diagonalLeft_alert){turning = true;} // con correccion en diagonales

    if(distance_f < critDistance_f){turning = true;}
    else if(!diagonalRight_alert || !diagonalLeft_alert){
      diagonalTurn = true;
      turning = true;
    }

    else{turning = false;}
    sensors_on = true;
  }
}

// RUTINA PARA BUSCAR EL CAMINO LATERAL MAS DESPEJADO CUANDO SE DETECTA UN OBSTACULO FRONTAL
void sonarOp_2(){
  if(turn_on){
    turn_on = false;
    turn_tnow = millis();
  }
  if((millis() - turn_tnow) > turn_tset){
    if(first_turn){
      // medir ambos lados con filtros
      sonarFilter();
      if(distance_rFiltered > distance_lFiltered){
        if(diagonalTurn){
          int angle_rightCorr = 0;
          rotate_dir(angle_rightCorr);
        }
        else{rotate_dir(angle_right);}
        drive_forward();
        delay(t_turn);
        clock_wise = true;
        turning = false;
        turn_on = true;
        diagonalTurn = false;
        first_turn = false;
      }
      else if(distance_rFiltered < distance_lFiltered){
        if(diagonalTurn){
          rotate_dir(angle_leftCorr);
        }
        else{rotate_dir(angle_left);}
        drive_forward();
        delay(t_turn);
        clock_wise = false;
        turning = false;
        turn_on = true;
        diagonalTurn = false;
        first_turn = false;
      }
    }
    else{
      if(clock_wise){   // giro preferencial horario
        // medir ambos lados con filtros
        sonarFilter();
        if(distance_rFiltered > distance_lFiltered - phantom_distance){
          if(diagonalTurn){
          rotate_dir(angle_rightCorr);
        }
        else{rotate_dir(angle_right);}
          drive_forward();
          delay(t_turn);
          clock_wise = true;
          turning = false;
          turn_on = true;
          diagonalTurn = false;
        }
        else if(distance_rFiltered < distance_lFiltered - phantom_distance){
          if(diagonalTurn){
          rotate_dir(angle_leftCorr);
        }
        else{rotate_dir(angle_left);}
          drive_forward();
          delay(t_turn);
          clock_wise = false;
          turning = false;
          turn_on = true;
          diagonalTurn = false;
        }
      }
      else{
        // medir ambos lados con filtros
        sonarFilter();
        if(distance_rFiltered - phantom_distance > distance_lFiltered){
          if(diagonalTurn){
          rotate_dir(angle_rightCorr);
        }
        else{rotate_dir(angle_right);}
          drive_forward();
          delay(t_turn);
          clock_wise = true;
          turning = false;
          turn_on = true;
          diagonalTurn = false;
        }
        else if(distance_rFiltered - phantom_distance < distance_lFiltered){
          if(diagonalTurn){
          rotate_dir(angle_leftCorr);
        }
        else{rotate_dir(angle_left);}
          drive_forward();
          delay(t_turn);
          clock_wise = false;
          turning = false;
          turn_on = true;
          diagonalTurn = false;
        }
      }
    }
  }
  else{
    rotate_dir(angle_straight);
    drive_backward();
    delay(t_back);
    stop();
  }
}

/*----------------------------- RUTINAS PARA DETECCION DE LINEAS DE COLOR --------------------------------*/
void check_colorLine(){

  if(color_delayGo){
    color_delayGo = false;
    color_tnow = millis();
  }
  if((millis() - color_tnow) > color_tset){
    if(colorReadOrder == 0){
      // Read Red value
      redPW = getRedPW();
      // Map to value from 0-255
      redValue = map(redPW, redMin,redMax,255,0);
      // Delay to stabilize sensor
    }
    else if(colorReadOrder == 1){
      // Read Green value
      greenPW = getGreenPW();
      // Map to value from 0-255
      greenValue = map(greenPW, greenMin,greenMax,255,0);
      // Delay to stabilize sensor
    }
    else if(colorReadOrder == 2){
      // Read Blue value
      bluePW = getBluePW();
      // Map to value from 0-255
      blueValue = map(bluePW, blueMin,blueMax,255,0);
      // Delay to stabilize sensor
    }
    color_delayGo = true;
    colorReadOrder += 1;
    if(colorReadOrder > 2){
      colorRead_complete = true;
      colorReadOrder = 0;
    }

    Serial.print("Red = ");
    Serial.print(redValue);
    Serial.print(" - Green = ");
    Serial.print(greenValue);
    Serial.print(" - Blue = ");
    Serial.println(blueValue);
  }
}

/*--------------- RUTINA PARA DETECTAR LINEA NARANJA -----------------*/
void detect_line(){
  
  if(searchingLine){
    check_colorLine();
    if(colorRead_complete){
      colorRead_complete = false;
      if(redValue > 100 && redValue > greenValue && greenValue > -20 && blueValue < 50){ // resta de 200 para bajo y 50 para alto
        lineFound += 1;
        searchingLine = false;
        go_searchLine = true;
        Serial.print(lineFound);  // for testing
        Serial.print(" | Red = ");
        Serial.print(redValue);
        Serial.print(" - Green = ");
        Serial.print(greenValue);
        Serial.print(" - Blue = ");
        Serial.println(blueValue);
      }
    }
  }
  else{
    if(go_searchLine){
      go_searchLine = false;
      searchLine_tnow = millis();
    }
    if(millis() - searchLine_tnow > searchLine_tset){
      searchingLine = true;
      //Serial.print(searchingLine);  // for testing
    }
  }
}

/*---------- RUTINA PARA DETENER EL ROBOT DESPUES DE 3 VUELTAS ----------*/
void trial_1_complete(){
  if(lineFound == 12){
    if(go_endOp){
      go_endOp = false;
      end_tnow = millis();
    }
    if(millis() - end_tnow > end_tset){
      start_process = false;
      lineFound = 0;
      go_endOp = true;
    }
  }
}

/*--------------------------------------------------------------------*/
// Function to read Red Pulse Widths
int getRedPW() {
	// Set sensor to read Red only
	digitalWrite(S2,LOW);
	digitalWrite(S3,LOW);
	// Define integer to represent Pulse Width
	int PW;
	// Read the output Pulse Width
	PW = pulseIn(sensorOut, LOW);
	// Return the value
	return PW;
}

// Function to read Green Pulse Widths
int getGreenPW() {
	// Set sensor to read Green only
	digitalWrite(S2,HIGH);
	digitalWrite(S3,HIGH);
	// Define integer to represent Pulse Width
	int PW;
	// Read the output Pulse Width
	PW = pulseIn(sensorOut, LOW);
	// Return the value
	return PW;
}

// Function to read Blue Pulse Widths
int getBluePW() {
	// Set sensor to read Blue only
	digitalWrite(S2,LOW);
	digitalWrite(S3,HIGH);
	// Define integer to represent Pulse Width
	int PW;
	// Read the output Pulse Width
	PW = pulseIn(sensorOut, LOW);
	// Return the value
	return PW;
}
/*-------------------------------------------------------------*/

/*---------- RUTINAS PARA EL MOVIMIENTO DEL VEHICULO ----------*/
// ROTAR LA DIRECCION DEL VEHICULO
void rotate_dir(int target_angle){
  servo_dir.write(target_angle);
}
// AVANZAR 
void drive_forward(){
  if(turning){analogWrite(dc_speedPin, dc_max_speed_turn);}
  else{analogWrite(dc_speedPin, dc_max_speed_drive);}
  digitalWrite(dc_1, HIGH);
  digitalWrite(dc_2, LOW);
}
// RETROCEDER
void drive_backward(){
  analogWrite(dc_speedPin, dc_max_speed_drive);
  digitalWrite(dc_1, LOW);
  digitalWrite(dc_2, HIGH);
}
// DETENERSE
void stop(){
  digitalWrite(dc_1, LOW);
  digitalWrite(dc_2, LOW);
}
/*---------- BUCLE PRINCIPAL ----------*/
void loop() {
 //////////////////////////////////////////////////////////////////// 
  // if(!start_process){
  //   stop();
  //   buttonPressStart();
  //   bool result = start;
  //   if(result != start_prev){
  //     start_prev = start;
  //     start_process = true;
  //     delay(500);
  //   }
  // }
  // else{
  //   ///////////////////
  //   trial_1_complete();
  //   detect_line();
  //   ///////////////////

  //   // SI NO SE DETECTAN OBSTACULOS AL FRENTE
  //   if(!turning){
  //     sonarOp_1();
  //     // correccion de posicion por us
  //     //------------------------------
  //     if(distance_l < critDistance_l){
  //       rotate_dir(angle_softRight);
  //       drive_forward();
  //     }
  //     else if(distance_r < critDistance_r){
  //       rotate_dir(angle_softLeft);
  //       drive_forward();
  //     }
  //     else if(distance_r > critDistance_r && distance_l > critDistance_l){
  //       rotate_dir(angle_straight);
  //       drive_forward();
  //     }
  //     else if(distance_r < critDistance_r && distance_l < critDistance_l){
  //       rotate_dir(angle_straight);
  //       drive_backward(); 
  //     }
  //   }
  //   // SI SE DETECTA UN OBSTACULO AL FRENTE
  //   else{
  //     //Serial.println("sonar 2");
  //     sonarOp_2();
  //   }
  // } 
////////////////////////////////////////////////////////////////////
    // tests
    //sonar_test();
    //IR_test();
    //sonarFilter();
    check_colorLine();
    detect_line();
}