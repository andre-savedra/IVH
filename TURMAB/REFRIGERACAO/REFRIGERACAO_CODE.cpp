(*The main code inserted by the user and executed associated with the MainTask must be inserted into this POU.*)
PROGRAM UserPrg
VAR
	ON_BUTTON : BOOL;
	LEVEL_SENSOR : BOOL;
	QUALITY_SENSOR : BOOL;
	PRESSURE_SENSOR : BOOL;
	MOTOR : BOOL;
	GREEN_LAMP : BOOL;
	RED_LAMP : BOOL;
	EMERGENCY_BUTTON : BOOL;		
	BUFFER_ON_BUTTON : BOOL;
	SYSTEM_ON : BOOL;
	
	LAMP_TIMER : TON;
	LAMP_TIMER_DELAY : TIME := T#500MS;
	LAMP_TIMER_FLAG : BOOL;
	
	VIRTUAL_ON_BUTTON : BOOL;
	TEST_MODE : BOOL := TRUE; 
END_VAR




// I/O's

//INPUTS:
ON_BUTTON := I00 OR VIRTUAL_ON_BUTTON;

IF ( TEST_MODE = FALSE) THEN
	LEVEL_SENSOR := I01;
	QUALITY_SENSOR := I02;
	PRESSURE_SENSOR := I03;
	EMERGENCY_BUTTON := NOT I04;	
END_IF

//OUTPUTS:
Q00 := MOTOR;
Q01 := GREEN_LAMP;
Q02 := RED_LAMP;

//BORDER DETECTION
IF ( ON_BUTTON = 1) THEN
	BUFFER_ON_BUTTON := 1;
END_IF

IF ( ON_BUTTON = 0 AND BUFFER_ON_BUTTON = 1) THEN
	SYSTEM_ON := NOT SYSTEM_ON;
	BUFFER_ON_BUTTON := 0;
END_IF

//SYSTEM ON LOGIC:
IF ( SYSTEM_ON = 1) THEN
	
	// EMERGENCY STATE!
	IF (EMERGENCY_BUTTON = 1) THEN
		SYSTEM_ON := 0;
	END_IF	

	MOTOR := LEVEL_SENSOR = 1 AND QUALITY_SENSOR = 1 AND PRESSURE_SENSOR = 0;
	GREEN_LAMP := MOTOR;
	
	// SENSORS ARE NOT OK:
	IF ( MOTOR = 0 ) THEN
		LAMP_TIMER(IN := LAMP_TIMER_FLAG, PT:= LAMP_TIMER_DELAY);	
	
		IF(LAMP_TIMER.Q = 1) THEN
			RED_LAMP := NOT RED_LAMP;
			LAMP_TIMER_FLAG := 0;
		ELSE
			LAMP_TIMER_FLAG := 1;			
		END_IF
		
	ELSE 
		RED_LAMP := 0;
	END_IF;

//SYSTEM OFF:
ELSE	
	MOTOR := 0;
	GREEN_LAMP := 0;
	RED_LAMP := EMERGENCY_BUTTON;	
END_IF
	