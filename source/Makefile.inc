BaseProgram_COBJS = arm.o colorSensor.o gyro.o motor.o system.o
APPL_COBJS += $(addprefix  BaseProgram/, $(BaseProgram_COBJS))

BasicMove_COBJS = objectMove.o
APPL_COBJS += $(addprefix  BasicMove/, $(BasicMove_COBJS))

MainProgram_COBJS = BridgeStart.o FenceStart.o
APPL_COBJS += $(addprefix  MainProgram/, $(MainProgram_COBJS))