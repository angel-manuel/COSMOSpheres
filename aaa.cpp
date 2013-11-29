//The following line actives debugging. If you comment it out you will save a lot de code usage
#define DEBUG_ACTIVE

#define SPHERE_ANGULAR_INERTIA 0.023f

#define MAX_FORCE 0.1f

#define MAX_NET_DIST 0.45f

#define SPHERE_RADIUS 0.11f
#define DEBRIS_RADIUS 0.04f
#define COMET_RADIUS 0.07f

#define MIN_LASSO_DIST 0.15f
#define MAX_LASSO_DIST 0.25f
#define LASSO_TOLERANCE (PI/12.0f)

#define MAX_ITEM_START_DIST 0.05f
#define MAX_ITEM_START_VEL 0.01f
#define MAX_ITEM_START_RATE 0.0401f
#define MAX_ITEM_RATE 1.3962f

//Indexes of the values contained in ZRState
#define POS 0
#define POS_X 0
#define POS_Y 1
#define POS_Z 2
#define VEL 3
#define VEL_X 3
#define VEL_Y 4
#define VEL_Z 5
#define ATT 6
#define ATT_X 6
#define ATT_Y 7
#define ATT_Z 8
#define RATE 9
//THe following ones are currently defined
//#define RATE_X 9
//#define RATE_Y 10
//#define RATE_Z 11

#define NUMBER_OF_DEBRIS 16

#define NUMBER_OF_ITEMS 2

#define OUR 0
#define THEIR 1

#define MIN(a, b) ((a < b) ? a : b)
#define MAX(a, b) ((a > b) ? a : b)

#define ABS(val) ((val > 0) ? val : -val)

//Force strategies
//define PHASE2_FORCE_STRATEGY_STAY_AND_SHOOT
