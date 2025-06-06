(*The main code inserted by the user and executed associated with the MainTask must be inserted into this POU.*)
PROGRAM UserPrg
VAR
	ON_BUTTON : BOOL;
	BUFFER_BUTTON : BOOL;
	SYSTEM_ON : BOOL;
	PRESSURE_SENSOR : BOOL;
	MOTOR_ON : BOOL;
	MOTOR : BOOL;
	LAMP : BOOL;
	MOTOR_TIMEOUT : TON;
	ALARM_STATE : BOOL;
	TIMEOUT_DELAY : TIME := T#4S;
	TIMER_LAMP : TON;
	BUFFER_TIMER_LAMP : BOOL;
	TIMER_LAMP_DELAY : TIME := T#500MS;
	LAMP_ALARM_STATE : BOOL;
END_VAR


// I/OS:
 ON_BUTTON := I00;
 PRESSURE_SENSOR := I01;
 MOTOR_ON := I02;
 Q00 := MOTOR;
 Q01 := LAMP;
 MOTOR_TIMEOUT(IN:= MOTOR, PT:= TIMEOUT_DELAY);
 TIMER_LAMP(IN:= BUFFER_TIMER_LAMP, PT:=TIMER_LAMP_DELAY);
  
 //LOGIC:
 IF ( ON_BUTTON = 1) THEN
	 BUFFER_BUTTON := 1;
 END_IF
 
 IF ( ON_BUTTON = 0 AND BUFFER_BUTTON = 1 ) THEN
	 SYSTEM_ON := NOT SYSTEM_ON;
	 BUFFER_BUTTON := 0;
 END_IF
 
 IF ( SYSTEM_ON = 1 ) THEN
	 
 	IF(TIMER_LAMP.Q = 1) THEN
		 BUFFER_TIMER_LAMP := 0;
	 	 LAMP_ALARM_STATE := NOT LAMP_ALARM_STATE;
	 ELSE
		 BUFFER_TIMER_LAMP := 1;
	 END_IF 
 
	 MOTOR := PRESSURE_SENSOR = 1 AND ALARM_STATE = 0;
	 LAMP := MOTOR_ON OR 
             ( ALARM_STATE = 1 AND LAMP_ALARM_STATE = 1); 
	 
	 IF ( MOTOR_TIMEOUT.Q = 1 AND MOTOR_ON = 0 ) THEN
		 ALARM_STATE := 1;
	 END_IF
	 
 //SISTEMA DESLIGADO:
 ELSE	 
	 MOTOR := 0;
	 LAMP := 0;
	 ALARM_STATE := 0;
	 LAMP_ALARM_STATE := 0;
 END_IF
   
 //MOTOR := SYSTEM_ON = 1 AND PRESSURE_SENSOR = 1;
 //LAMP := SYSTEM_ON = 1 AND MOTOR_ON = 1;
 
 
 
 
 
 
 
 
 
 