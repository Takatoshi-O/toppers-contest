void GyroSetting();

//Stop = 0 : hold / 1 : break / 2 : stop / 3 : no stop
//void GyroTurn(float Deg, int LeftPower, int RightPower, int stop, bool reset);

//Stop = 0 : hold / 1 : break / 2 : stop / 3 : no stop
//Deg = + : Left, - : Right
void GyroSpinTurn(float Deg, int stop, bool reset);

//Stop = 0 : hold / 1 : break / 2 : stop / 3 : no stop
//Deg = + : Left, - : Right
//UseMotor = -1 : Left / 1 : Right
void GyroPivotTurn(float Deg, int UseMotor, int stop, bool reset);


void GyroReset();

//Start "OFFSET_GYRO_CYC"
void OffsetGyro(bool TaskStart);