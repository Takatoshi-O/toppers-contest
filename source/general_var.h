extern pup_motor_t *motorL;             //左側モーターを使う変数 left > right
extern pup_motor_t *motorR;             //右側モーターを使う変数 left > right
extern pup_motor_t *armF;               //前のアームを使う変数 open + / close -
extern pup_motor_t *armB;               //後ろのアームを使う変数 up + / down -
extern pup_device_t *ColorSensor;       //カラーセンサーを使う変数
extern pup_device_t *UnderColorSensor;  //カラーセンサーを使う変数 
extern float ang_v[3];                  //ジャイロセンサーの角度で使う配列
extern int ADJBasePower;
extern int GyroMotorBaseAngle;
extern int BeforeRotation;
extern double MotorSin, MotorCos;
extern const int startSide;            //Fence Side : 0 / Bridge Side : 1
#define PI 3.1415926535897932384626
extern char objectColor[4][2];   //0 : near Start / 1 : near Waterpipe / 2 : in bridge / 3 : in step