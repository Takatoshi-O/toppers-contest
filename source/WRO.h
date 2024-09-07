/*
 * Copyright (c) 2023 Afrel Co., Ltd. 
 * All rights reserved
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <kernel.h>

/*
 *  タスクの優先度の定義(最低でも5)
 */
#define MAIN_PRIORITY	8	/* メインタスクの優先度 */
#define GYRO_PRIORITY 5
#define TRI_MOTOR_PRIOITY 6
#define GYRO_MOTOR_PRIOITY 6
#define ADJ_GAIN_PRIORITY 6
#define ARM_LOCK_PRIORITY 7

/*
 * Definitions of Port ID for Serial Adapter
 */
#define SIO_USART_F_PORTID    1
#define SIO_USB_PORTID        2
#define SIO_BLUETOOTH_PORTID  3
#define SIO_TEST_PORTID       4

#ifndef STACK_SIZE
#define	STACK_SIZE		4096		/* タスクのスタックサイズ */
#endif /* STACK_SIZE */

/*
 *  関数のプロトタイプ宣言
 */
#ifndef TOPPERS_MACRO_ONLY

extern void Main(intptr_t exinf);
extern void gyro_monitor(intptr_t exinf);
extern void OffsetGyroTsk(intptr_t exinf);
extern void ADJMotorOnTsk(intptr_t exinf);
extern void ArmLockTsk(intptr_t exinf);
extern void GyroMotorOnTsk(intptr_t exinf);
extern void TriMotorOnTsk(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */
