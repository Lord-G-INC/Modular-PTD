#include "ClipAreaDropLaser.h"
#include "ClipAreaDropScaleHolder.h"
#include "Game/Util/DirectDraw.h"

ClipAreaDropLaser::ClipAreaDropLaser(const char* pName) : LiveActor(pName) {
    _390 = 0.0f;
    _394 = -1;
    _398 = -1;
    _39C = 20.0f;
    mInitialTrailColor = 0x40F080;
    mTrailColor = 0x40F080;
    MR::createClipAreaDropScaleHolder();
}

void ClipAreaDropLaser::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToScene(this, 0x22, -1, -1, 0x14);
    initRailRider(rIter);
    MR::moveCoordAndTransToRailStartPoint(this);
    _39C = 20.0f;
    MR::getJMapInfoArg0NoInit(rIter, &_39C);
    initEffectKeeper(0, "ClipAreaDropLaser", false);
    initSound(4, "ClipAreaDropLaser", &mTranslation, TVec3f(0.0f, 0.0f, 0.0f));
    initNerve(&NrvClipAreaDropLaser::ClipAreaDropLaserNrvMove::sInstance, 0);
    MR::invalidateClipping(this);

    s32 r = 0;
    s32 g = 0;
    s32 b = 0;
    s32 a = 0;
    MR::getJMapInfoArg1NoInit(rIter, &r);
    MR::getJMapInfoArg2NoInit(rIter, &g);
    MR::getJMapInfoArg3NoInit(rIter, &b);
    MR::getJMapInfoArg4NoInit(rIter, &a);
    Color8 color = Color8(r, g, b, a);
    mTrailColor = color.mColor;
    mInitialTrailColor = color.mColor;

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        setNerve(&NrvClipAreaDropLaser::ClipAreaDropLaserNrvWait::sInstance);
    }
    makeActorAppeared();
}
void ClipAreaDropLaser::control() {

}

void ClipAreaDropLaser::draw() const {
    TDDraw::cameraInit3D();
    TDDraw::setup(0, 2, 0);
    GXSetLineWidth(0x14, GX_TO_ZERO);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);

    for (int r29 = 1; r29 < _390; r29++) {
        int u1 = _398-(r29-1);
        int u2 = _398-r29;
        
        if (u1 < 0)
            u1 += 0x40;
        if (u2 < 0)
            u2 += 0x40;

        if (u1 != _394) {
            TDDraw::drawLine(_90[u1], _90[u2], mTrailColor);
        }
    }
}

void ClipAreaDropLaser::exeWait() {
    if (_390 > 0.0f) {
        _390 = _390 - 1.0f;
    }

    if (MR::isValidSwitchAppear(this) && MR::isOnSwitchAppear(this))
        setNerve(&NrvClipAreaDropLaser::ClipAreaDropLaserNrvMove::sInstance);
}

extern "C" {
    void __kAutoMap_80053D90(const LiveActor* pActor, s32, s32, f32*);

    const char* __kAutoMap_80053C90(s32);
    bool __kAutoMap_8023DB60(RailRider*, const char*, s32*, s32);
};

bool getRailPointArgS32AtPoint(const LiveActor* pActor, s32 pnt, s32 l, s32* pL) {
    const char* pntName = __kAutoMap_80053C90(pnt);
    return __kAutoMap_8023DB60(pActor->mRailRider, pntName, pL, l);
}

void ClipAreaDropLaser::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::moveCoordAndTransToRailStartPoint(this);
        _390 = 0.0f;
    }
    s32 railPoint = 0;
    railPoint = MR::moveCoordAndCheckPassPointNo(this, _39C);
    MR::moveTransToCurrentRailPos(this);
    incrementDrawCount();
    if (MR::isRailReachedGoal(this)) {
        _394 = _398;
        railPoint = MR::getRailPointNum(this)-1;
        MR::moveCoordAndTransToRailStartPoint(this);
        mTrailColor = mInitialTrailColor;
    }
    _90[_398].set(mTranslation);

    OSReport("%d\n", railPoint);

    if (railPoint != -1) {
        TVec3f pos;
        MR::calcRailPointPos(&pos, this, railPoint);
        f32 arg = -1.0;

        __kAutoMap_80053D90(this, 0, railPoint, &arg);

        s32 r = 0;
        s32 g = 0;
        s32 b = 0;
        s32 a = 0;
        s32 def = 0;
        getRailPointArgS32AtPoint(this, 1, railPoint, &r);
        getRailPointArgS32AtPoint(this, 2, railPoint, &g);
        getRailPointArgS32AtPoint(this, 3, railPoint, &b);
        getRailPointArgS32AtPoint(this, 4, railPoint, &a);
        getRailPointArgS32AtPoint(this, 5, railPoint, &def);
        
        Color8 color = Color8(r,g,b,a);
        
        if (def)
            mTrailColor = 0x40F080;
        else if (color.mColor != 0)
            mTrailColor = color.mColor;
        
        
        if (arg > 0.0f) {
            MR::emitEffectHit(this, pos, "Splash");
            MR::startSound(this, "SE_OJ_DROP_LASER_SPLASH", -1, -1);
            MR::appearClipAreaDropScale(pos, arg);
        }
    }

    if (MR::isValidSwitchAppear(this) && !MR::isOnSwitchAppear(this))
        setNerve(&NrvClipAreaDropLaser::ClipAreaDropLaserNrvWait::sInstance);
}

void ClipAreaDropLaser::incrementDrawCount() {
    _398+= 1;
    if (_398 >= 0x40) {
        _398 += -0x40;
    }
    if (_390 < 64.0f) {
        _390 += 1.0f;
    }
}

ClipAreaDropLaser::~ClipAreaDropLaser() {

}

namespace NrvClipAreaDropLaser {
    ClipAreaDropLaserNrvWait(ClipAreaDropLaserNrvWait::sInstance);
    ClipAreaDropLaserNrvMove(ClipAreaDropLaserNrvMove::sInstance);
}
