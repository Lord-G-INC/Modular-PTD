#include "syati.h"
#include "BlueCoin.h"
#include "Game/Enemy/KuriboExt.h"
#include "Game/Enemy/SamboHeadExt.h"
 // KURIBO

namespace BlueCoinUtil {
    extern LiveActor* createBlueCoinForSpawning(LiveActor* pSourceActor, s32 id);
}
KuriboExt::KuriboExt(const char* pName) : Kuribo(pName) {
    mBlueCoin = 0;
    mBlueCoinArg = -1;
}

KuriboExt* createKuriboExt(const char* pName) {
    return new KuriboExt(pName);
}

kmCall(0x801A2C8C, createKuriboExt);
kmWrite32(0x801A2C90, 0x48000014); // b 0x14
kmCall(0x801E4B34, createKuriboExt);
kmWrite32(0x801E4B38, 0x48000014); // b 0x14
kmCall(0x802B4934, createKuriboExt);
kmWrite32(0x802B4938, 0x48000018); // b 0x18
kmCall(0x8033D4C4, createKuriboExt);
kmWrite32(0x8033D4C8, 0x48000014); // b 0x14

void KuriboSetUpBlueCoin(KuriboExt* pKuribo, const JMapInfoIter& rIter, const char* pStr) {

    if (MR::isValidInfo(rIter)) {
        MR::getJMapInfoArg2NoInit(rIter, &pKuribo->mBlueCoinArg);
    }
    MR::processInitFunction(pKuribo, rIter, pStr, 0);

    if (pKuribo->mBlueCoinArg > -1)
        pKuribo->mBlueCoin = (BlueCoin*)BlueCoinUtil::createBlueCoinForSpawning(pKuribo, pKuribo->mBlueCoinArg);
}

kmCall(0x801B8A68, KuriboSetUpBlueCoin);

void KuriboAppearBlueCoin(KuriboExt* pKuribo, const char* pStr) {
    MR::emitEffect(pKuribo, pStr);

    if (pKuribo->mBlueCoin)
        BlueCoinUtil::appearBlueCoin(pKuribo, pKuribo->mBlueCoin);
}

kmCall(0x801B8EC8, KuriboAppearBlueCoin);

// SAMBOHEAD


SamboHeadExt::SamboHeadExt(const char* pName) : SamboHead(pName) {
    mBlueCoin = 0;
    mBlueCoinArg = -1;
}

SamboHead* createSamboHeadExt(const char* pName) {
    return new SamboHeadExt(pName);
}

kmCall(0x8033DC44, createSamboHeadExt);
kmWrite32(0x8033DC48, 0x48000014); // li r3, 0xC4

void SamboHeadSetUpBlueCoin(SamboHeadExt* pSamboHead, const JMapInfoIter& rIter, const char* pStr) {
    if (MR::isValidInfo(rIter))
        MR::getJMapInfoArg1NoInit(rIter, &pSamboHead->mBlueCoinArg);

    MR::processInitFunction(pSamboHead, rIter, pStr, 0);

    if (pSamboHead->mBlueCoinArg > -1)
        pSamboHead->mBlueCoin = (BlueCoin*)BlueCoinUtil::createBlueCoinForSpawning(pSamboHead, pSamboHead->mBlueCoinArg);
}

kmCall(0x801F8024, SamboHeadSetUpBlueCoin);

void SamboHeadAppearBlueCoin(SamboHeadExt* pSamboHead, const char* pStr) {
    MR::emitEffect(pSamboHead, pStr);

    if (pSamboHead->mBlueCoin)
        BlueCoinUtil::appearBlueCoin(pSamboHead, pSamboHead->mBlueCoin);
}

kmCall(0x801F81D4, SamboHeadAppearBlueCoin);
