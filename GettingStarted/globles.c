
#include "includes.h"

struct cal CAL;         // calibration values for scale
struct scale SCALE;     // structure for main scale operations

struct system SYSTEM;   // structure for system operations/maintnence
struct checkweigh CHECKWEIGH; // stucture for checkweigher values
struct com COM[4]; // SERIAL XMIT com structure
struct transaction TRANSACTION; // transaction stored for recording checkweigher activity
struct stat STAT;             // holds statistical information for checkweighing
struct ticket TICKET;         // ticket buffer
struct flash_mem FLASH_MEM;  // structure holding all volatile variables that need to be stored to flash memory
struct menu MENU;   // volatile memory that is backed up in flash
struct display DISPLAY;       // structure that holds display information
struct nvmem NVMEM;     // non volatile memory stored seperatly it's own flash sector flash
