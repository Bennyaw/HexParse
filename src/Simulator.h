#ifndef _SIMULATOR_H
#define _SIMULATOR_H
#include <stdint.h>

#define K			1024

#define SRAM_SIZE   0x900
#define FLASH_SIZE  (256 * K)

//#define spRegPtr		(uint16_t *)(sram[*(uint16_t *)spl])

typedef int (*AvrOperator)(uint8_t *codePtr);
typedef struct SregRegister SregRegister;
struct SregRegister{
	uint8_t C: 1;
	uint8_t Z: 1;
	uint8_t N: 1;
	uint8_t V: 1;
	uint8_t S: 1;
	uint8_t H: 1;
	uint8_t T: 1;
	uint8_t I: 1;
};

int add(uint8_t *codePtr);
int adc(uint8_t *codePtr);
int and(uint8_t *codePtr);
int andi(uint8_t *codePtr);
int adiw(uint8_t *codePtr);
int sub(uint8_t *codePtr);
int subi(uint8_t *codePtr);
int sbc(uint8_t *codePtr);
int sbci(uint8_t *codePtr);
int sbiw(uint8_t *codePtr);
int or(uint8_t *codePtr);
int ori(uint8_t *codePtr);
int eor(uint8_t *codePtr);
int com(uint8_t *codePtr);
int neg(uint8_t *codePtr);
int dec(uint8_t *codePtr);
int inc(uint8_t *codePtr);
int sbr(uint8_t *codePtr);
int tst(uint8_t *codePtr);
int clr(uint8_t *codePtr);
int ser(uint8_t *codePtr);
int mul(uint8_t *codePtr);
int muls(uint8_t *codePtr);
int mulsu(uint8_t *codePtr);
int lsl(uint8_t *codePtr);
int lsr(uint8_t *codePtr);
int ror(uint8_t *codePtr);
int asr(uint8_t *codePtr);
int swap(uint8_t *codePtr);
int bset(uint8_t *codePtr);
int bclr(uint8_t *codePtr);
int bst(uint8_t *codePtr);
int bld(uint8_t *codePtr);
int sec(uint8_t *codePtr);
int clc(uint8_t *codePtr);
int sen(uint8_t *codePtr);
int cln(uint8_t *codePtr);
int sez(uint8_t *codePtr);
int clz(uint8_t *codePtr);
int sei(uint8_t *codePtr);
int cli(uint8_t *codePtr);
int ses(uint8_t *codePtr);
int cls(uint8_t *codePtr);
int sev(uint8_t *codePtr);
int clv(uint8_t *codePtr);
int set(uint8_t *codePtr);
int clt(uint8_t *codePtr);
int seh(uint8_t *codePtr);
int clh(uint8_t *codePtr);
int mov(uint8_t *codePtr);
int rjmp(uint8_t *codePtr);
int jmp(uint8_t *codePtr);
int brbs(uint8_t *codePtr);
int brbc(uint8_t *codePtr);
int nop(uint8_t *codePtr);
int wdr(uint8_t *codePtr);
int Break(uint8_t *codePtr);
int sleep(uint8_t *codePtr);
int cp(uint8_t *codePtr);
int cpc(uint8_t *codePtr);
int cpi(uint8_t *codePtr);
int ldi(uint8_t *codePtr);
int cpse(uint8_t *codePtr);

int instructionWith1001010(uint8_t *codePtr);
int instructionWith10010100(uint8_t *codePtr);
int instructionWith10010101(uint8_t *codePtr);

uint32_t getPc(uint8_t *progCounter);
uint8_t *getCodePtr(uint32_t pc);
void substractStackPointer(int value);
void pushWord(uint16_t data);
uint16_t popWord();
void initSimulator();
int is2wordInstruction(uint8_t *codePtr);

//Status Register
int is8bitZero(uint8_t data8bit);
int is16bitADIWCarry(uint16_t result, uint16_t operand);
int is16BitZero(uint16_t data16Bit);
int is16bitNeg(uint16_t result);
int is16bitADIWOverflow(uint16_t result, uint16_t operand);
int is16bitSigned(uint16_t result, uint16_t operand);
int handleStatusRegForAddImmWordOperation(uint16_t result, uint16_t operand);
int is16bitSBIWCarry(uint16_t result, uint16_t operand);
int is16bitSBIWOverflow(uint16_t result, uint16_t operand);
int handleStatusRegForSubImmWordOperation(uint16_t result, uint16_t operand);
int is8bitAdcAddCarry(uint8_t operand1, uint8_t operand2, uint8_t result);
int is8bitNeg(uint8_t result);
int is8bitOverflow(uint8_t operand1, uint8_t operand2, uint8_t result);
int is8bitSigned(uint8_t operand1, uint8_t operand2, uint8_t result);
int is8bitAdcAddHalfCarry(uint8_t operand1, uint8_t operand2, uint8_t result);
int handleStatusRegForAddAdcOperation(uint8_t operand1, uint8_t operand2, uint8_t result);
int is8bitAndAndiOrOriEorCbrSbrTstOverflow();
int is8bitAndAndiOrOriEorCbrSbrTstSigned(uint8_t result);
int handleStatusRegForAndAndiOrOriEorCbrSbrTstOperation(uint8_t result);
int is8bitSubSubiSbcSbciCarry(uint8_t operand1, uint8_t operand2, uint8_t result);
int is8bitSubSubiSbcSbciHalfCarry(uint8_t operand1, uint8_t operand2, uint8_t result);
int is8bitSubSubiSbcSbciSigned(uint8_t operand1, uint8_t operand2, uint8_t result);
int is8bitSubSubiSbcSbciOverflow(uint8_t operand1, uint8_t operand2, uint8_t result);
int handleStatusRegForSubSubiSbcSbciOperation(uint8_t operand1, uint8_t operand2, uint8_t result);
int is8bitComCarry();
int handleStatusRegForComOperation(uint8_t result);
int is8bitNegCarry(uint8_t data8bit);
int is8bitNegOverflow(uint8_t data8bit);
int is8bitNegSigned(uint8_t result);
int is8bitNegHalfCarry(uint8_t operand1, uint8_t result);
int handleStatusRegForNegOperation(uint8_t operand1, uint8_t result);
int handleStatusRegForIncOperation(uint8_t result);
int is8bitDecOverflow(uint8_t data8bit);
int handleStatusRegForDecOperation(uint8_t result);
int handleStatusRegForClrOperation();
int is16bitMulMulsMulsuCarry(uint16_t result);
int handleStatusRegForMulMulsMulsuOperation(uint16_t result);
int is8bitLslRolCarry(uint8_t operand1);
int is8bitLslRolOverflow(uint8_t operand1, uint8_t result);
int is8bitLslRolSigned(uint8_t operand1, uint8_t result);
int is8bitLslRolHalfCarry(uint8_t operand1);
int handleStatusRegForLslRolOperation(uint8_t operand1, uint8_t result);
int is8bitLsrRorCarry(uint8_t operand1);
int is8bitLsrOverflow(uint8_t operand1);
int is8bitLsrSigned(uint8_t operand1);
int handleStatusRegForLsrOperation(uint8_t operand1, uint8_t result);
int is8bitRorOverflow(uint8_t operand1, uint8_t result);
int is8bitRorSigned(uint8_t operand1, uint8_t result);
int handleStatusRegForRorAsrOperation(uint8_t operand1, uint8_t result);
int handleStatusRegForRorBstOperation(uint8_t operand1);
int simulateOneInstruction(uint8_t *codePtr);

//Export Variable(s)
extern AvrOperator avrOperatorTable[256];
extern uint8_t sram[SRAM_SIZE];
extern uint8_t flashMemory[FLASH_SIZE];

//AVR Register
extern uint8_t *r;
extern SregRegister *sreg;
extern uint8_t *sph;
extern uint8_t *spl;
extern uint8_t *flash;

#endif // _SIMULATOR_H
