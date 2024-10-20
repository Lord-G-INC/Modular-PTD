#include "ExtStationedFileTable.h"



asm void checkName() {
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r28, 0x10(r1)
    stw r29, 0xC(r1)

    lwz r28, 4(r30) 
    lwz r29, 8(r30)

    cmpwi r29, 0
    bne end
    cmpwi r28, 9
    beq end
    lis r3, cNewStationedFileEntries@ha
    addi r3, r3, cNewStationedFileEntries@l
    addi r30, r3, 0xC
    lwz r28, 4(r30)
    lwz r29, 8(r30)

    end:
    mr r3, r29
    lwz r0, 0x14(r1)
    lwz r28, 0x10(r1)
    lwz r29, 0xC(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
}

kmWrite32(0x804CDF44, 0x2C030000);
kmCall(0x804CDF40, checkName);


asm void checkName2() {
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r28, 0x10(r1)
    stw r29, 0xC(r1)

    lwz r28, 4(r31) 
    lwz r29, 8(r31)

    cmpwi r29, 0
    bne end
    cmpwi r28, 9
    beq end
    lis r3, cNewStationedFileEntries@ha
    addi r3, r3, cNewStationedFileEntries@l
    addi r31, r3, 0xC
    lwz r28, 4(r31)
    lwz r29, 8(r31)

    end:
    mr r3, r29
    lwz r0, 0x14(r1)
    lwz r28, 0x10(r1)
    lwz r29, 0xC(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
}

kmWrite32(0x804CDE68, 0x2C030000);
kmCall(0x804CDE6C, checkName2);

kmWrite32(0x804CDD14, 0x2C030000);
kmCall(0x804CDD18, checkName2);