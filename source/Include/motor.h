//Mode = 0 : hold / 1 : break / 2 : stop / 3 : no stop
void MotorStop(int Mode);

//void TriMotorReset();

//Mode = 0 : hold / 1 : break / 2 : stop / 3 : no stop
void BaseMotor(int LeftPower, int RightPower, int MotorAngle, int Stop, bool Reset);

//Stop = 0 : hold / 1 : break / 2 : stop / 3 : no stop
void ADJMotor(int MotorPower, int MotorAngle, int Stop, bool Reset);

//Stop = 0 : hold / 1 : break / 2 : stop / 3 : no stop
//TargetAngle = + : Left / - : Right
void GyroMotor(int MotorPower, int motorAngle, int TargetAngle, int Stop, bool Reset);

//Stop = 0 : hold / 1 : break / 2 : stop / 3 : no stop
//TargetAngle = + : Left / - : Right
//void TriMotor(int MotorPower, int motorAngle, int TargetAngle, int Stop, bool Reset);

//Stop = 0 : hold / 1 : break / 2 : stop / 3 : no stop
void tADJMotor(int FrontBack, int MotorPower, int StartPower, int FinPower, int MotorAngle, int Stop, bool Reset);

//Stop = 0 : hold / 1 : break / 2 : stop / 3 : no stop
//TargetAngle = + : Left / - : Right
void tGyroMotor(int FrontBack, int MotorPower, int StartPower, int FinPower, int MotorAngle, int TargetAngle, int Stop, bool Reset);

//Stop = 0 : hold / 1 : break / 2 : stop / 3 : no stop
//TargetAngle = + : Left / - : Right
//void tTriMotor(int FrontBack, int MotorPower, int StartPower, int FinPower, int MotorAngle, int TargetAngle, int Stop, bool Reset);

//Stop = 0 : hold / 1 : break / 2 : stop / 3 : no stop
void SecMotor(int MotorPower, float Sec, int stop);

//Start "ADJ_ON_CYC"
void ADJMotorOn(bool TaskStart);

//Start "GYRO_MOTOR_ON_CYC"
void GyroMotorOn(bool TaskStart);

//Start "TRI_MOTOR_ON_CYC"
//void TriMotorOn(bool TaskStart);