#include "ClipAreaDropLaser.h"
#include "ClipAreaDropScaleHolder.h"
#include "Game/Util/DirectDraw.h"

ClipAreaDropLaser::ClipAreaDropLaser(const char* pName) : LiveActor(pName) {
    mNumPointsToDraw = 0;
    mPointIndexToSkipDraw = -1;
    mDrawCount = -1;
    mSpeed = 20.0f;
    mTrailColor = 0x40F080;
    mCooldown = 0;
    mCooldownTimer = 0;
    mTimerToResetDraw = 0;
    MR::createClipAreaDropScaleHolder();
}

void ClipAreaDropLaser::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToScene(this, 0x22, -1, -1, 0x14);
    initRailRider(rIter);
    MR::moveCoordAndTransToRailStartPoint(this);
    MR::getJMapInfoArg0NoInit(rIter, &mCooldown);
    initEffectKeeper(0, "ClipAreaDropLaser", false);
    initSound(4, "ClipAreaDropLaser", &mTranslation, TVec3f(0.0f, 0.0f, 0.0f));
    initNerve(&NrvClipAreaDropLaser::ClipAreaDropLaserNrvMove::sInstance, 0);
    MR::invalidateClipping(this);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        setNerve(&NrvClipAreaDropLaser::ClipAreaDropLaserNrvWait::sInstance);
    }

    makeActorAppeared();
}
void ClipAreaDropLaser::control() {
    if(mPointIndexToSkipDraw != -1 && mTimerToResetDraw != 0)
        mTimerToResetDraw--; 

    if (mTimerToResetDraw == 0) {
        mPointIndexToSkipDraw = -1;
    }
}

void ClipAreaDropLaser::draw() const {
    TDDraw::cameraInit3D();
    TDDraw::setup(0, 2, 0);
    GXSetLineWidth(0x14, GX_TO_ZERO);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);

    for (int i = 1; i < mNumPointsToDraw; i++) {
        int point1st = mDrawCount-(i-1);
        int point2nd = mDrawCount-i;

        if (point1st < 0)
            point1st += 0x40;
        if (point2nd < 0)
            point2nd += 0x40;

        if (point1st != mPointIndexToSkipDraw) {
            TDDraw::drawLine(mDrawPoints[point1st], mDrawPoints[point2nd], mTrailColor);
        }
    }
}

void ClipAreaDropLaser::exeWait() {
    if (mNumPointsToDraw != 0)
        mNumPointsToDraw--;

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
        mNumPointsToDraw = 0;

        u32 color = getRgba(0);
        if (color)
            mTrailColor = color;
            
        f32 speedInitial = 20.0f;
        __kAutoMap_80053D90(this, 1, 0, &speedInitial);
        mSpeed = speedInitial;
    }
    s32 railPoint = 0;
    
    if (mCooldownTimer == 0) {
        railPoint = MR::moveCoordAndCheckPassPointNo(this, mSpeed);
        MR::moveTransToCurrentRailPos(this);
    }
    incrementDrawCount();
    
    if (MR::isRailReachedGoal(this)) {
        mPointIndexToSkipDraw = mDrawCount;
        mTimerToResetDraw = 64;
        MR::moveCoordAndTransToRailStartPoint(this);
        railPoint = MR::getRailPointNum(this)-1;
        mCooldownTimer = mCooldown;
    }
    
    mDrawPoints[mDrawCount].set(mTranslation);

    if (railPoint != -1) {
        TVec3f pos;
        MR::calcRailPointPos(&pos, this, railPoint);
        f32 arg = -1.0;
        __kAutoMap_80053D90(this, 0, railPoint, &arg);

        f32 speed = -1.0f;
        __kAutoMap_80053D90(this, 1, railPoint, &speed);

        if (speed >= 0.0f)
            mSpeed = speed;

        u32 colorForMove = getRgba(railPoint);
        if (colorForMove)
            mTrailColor = colorForMove;
        
        if (arg > 0.0f) {
            MR::emitEffectHit(this, pos, "Splash");
            MR::startSound(this, "SE_OJ_DROP_LASER_SPLASH", -1, -1);
            MR::appearClipAreaDropScale(pos, arg);
        }
    }

    if (mCooldownTimer > 0)
        mCooldownTimer--;

    if (MR::isValidSwitchAppear(this) && !MR::isOnSwitchAppear(this))
        setNerve(&NrvClipAreaDropLaser::ClipAreaDropLaserNrvWait::sInstance);
}

void ClipAreaDropLaser::incrementDrawCount() {
    mDrawCount++;
    if (mDrawCount >= 64)
        mDrawCount += -64;

    if (mNumPointsToDraw < 64)
        mNumPointsToDraw++;
}

u32 ClipAreaDropLaser::getRgba(s32 railPoint) {
    s32 r = 0;
    s32 g = 0;
    s32 b = 0;
    s32 a = 0;
    getRailPointArgS32AtPoint(this, 2, railPoint, &r);
    getRailPointArgS32AtPoint(this, 3, railPoint, &g);
    getRailPointArgS32AtPoint(this, 4, railPoint, &b);
    getRailPointArgS32AtPoint(this, 5, railPoint, &a);

    Color8 color = Color8(r, g, b, a);
    return color.mColor;
}

ClipAreaDropLaser::~ClipAreaDropLaser() {

}

namespace NrvClipAreaDropLaser {
    ClipAreaDropLaserNrvWait(ClipAreaDropLaserNrvWait::sInstance);
    ClipAreaDropLaserNrvMove(ClipAreaDropLaserNrvMove::sInstance);
}