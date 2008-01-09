#include <stack.h>

void  c_int00(void);

#define STACK_SIZE    (128)

int svcStackSize = STACK_SIZE;
int svcStack[STACK_SIZE];
static int fiqStack[STACK_SIZE];
static int irqStack[STACK_SIZE];
static int abtStack[STACK_SIZE];
static int undStack[STACK_SIZE];

void
    init (
        void
)
{
    setupStack(ARM_MODE_FIQ, fiqStack + (sizeof(fiqStack) >> 2) - 1);
    setupStack(ARM_MODE_IRQ, irqStack + (sizeof(irqStack) >> 2) - 1);
    setupStack(ARM_MODE_ABT, abtStack + (sizeof(abtStack) >> 2) - 1);
    setupStack(ARM_MODE_UND, undStack + (sizeof(undStack) >> 2) - 1);
    c_int00();
}

