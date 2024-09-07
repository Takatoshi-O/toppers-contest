//Updown = 1 : floor1 Up / 2 ; floor2 Up / 3 : AllUp
//Updown = -1 : floor1 Down / -2 : floor2 Down / 0 : AllDown 
void ArmUpdown(int Updown);

//FrontArm reset
void ArmClose();

//FrontArm Open / Close
void ArmOpen(int Angle);

//Start "ARM_LOCK_CYC"
void ArmLock(bool TaskStart);