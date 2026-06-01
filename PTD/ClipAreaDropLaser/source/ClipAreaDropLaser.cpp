#include "ClipAreaDropLaser.h"
#include "Game/Util/DirectDraw.h"

extern "C" {
    bool __kAutoMap_80053D90(const LiveActor* pActor, s32, s32, f32*);

    const char* __kAutoMap_80053C90(s32);
    bool __kAutoMap_8023DB60(RailRider*, const char*, s32*, s32);
};

bool getRailPointArgS32AtPoint(const LiveActor* pActor, s32 pnt, s32 l, s32* pL) {
    const char* pntName = __kAutoMap_80053C90(pnt);
    return __kAutoMap_8023DB60(pActor->mRailRider, pntName, pL, l);
}

bool getRailPointArgF32NoInit(const LiveActor* pActor, s32 pnt, s32 l, f32* pF) {
    return __kAutoMap_80053D90(pActor, pnt, l, pF);
}

ClipAreaDropLaser::ClipAreaDropLaser(const char* pName) : LiveActor(pName) {
    mSphereHoleGroup = 0;
    mBoxHoleGroup = 0;
    mNumPointsToDraw = 0;
    mPointIndexToSkipDraw = -1;
    mDrawCount = -1;
    mSpeed = 20.0f;
    mTrailColor = 0x40F080;
    mCooldown = 0;
    mCooldownTimer = 0;
    mTimerToResetDraw = 0;
}

void ClipAreaDropLaser::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToScene(this, 0x22, -1, -1, 0x14);
    initRailRider(rIter);
    MR::moveCoordAndTransToRailStartPoint(this);
    MR::getJMapInfoArg0NoInit(rIter, &mCooldown);
    initHoleGroups(rIter);
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
        getRailPointArgF32NoInit(this, 0, railPoint, &arg);


        f32 speed = -1.0f;
        getRailPointArgF32NoInit(this, 1, railPoint, &speed);


        if (speed >= 0.0f)
            mSpeed = speed;

        u32 colorForMove = getRgba(railPoint);
        if (colorForMove)
            mTrailColor = colorForMove;
        
        if (arg > 0.0f) {
            MR::emitEffectHit(this, pos, "Splash");
            MR::startSound(this, "SE_OJ_DROP_LASER_SPLASH", -1, -1);

            s32 shape = 0;
            getRailPointArgS32AtPoint(this, 3, railPoint, &shape);
            appearClipAreaDropScale(pos, arg, shape);
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
    s32 rgba = 0;
    getRailPointArgS32AtPoint(this, 2, railPoint, &rgba);

    if (rgba == -2)
        return 0x40F080;

    return (u32)rgba;
}

bool ClipAreaDropLaser::appearClipAreaDropScale(const TVec3f& rPos, f32 arg, u32 shape) {
    ClipAreaDropScale* pClipArea = 0;
    if (shape == 0) 
        pClipArea = (ClipAreaDropScale*)mSphereHoleGroup->getDeadActor();
    else if (shape == 1) 
        pClipArea = (ClipAreaDropScale*)mBoxHoleGroup->getDeadActor();

    if (!pClipArea) 
        return false;

    pClipArea->mTranslation.set(rPos);
    pClipArea->setBaseSize(arg);
    pClipArea->appear();
    return true;
}

void ClipAreaDropLaser::initHoleGroups(const JMapInfoIter& rIter) {
    s32 sphereDropNum = 8;
    s32 boxDropNum = 8;
    MR::getJMapInfoArg1NoInit(rIter, &sphereDropNum);
    MR::getJMapInfoArg2NoInit(rIter, &boxDropNum);

    s32 railPointNum = MR::getRailPointNum(this);
    s32 railPointForHoleNum = railPointNum;
    s32 numSpherePoints = 0;
    s32 numBoxPoints = 0;

    for (s32 i = 0; i < railPointNum; i++) {
        if (numSpherePoints > 0 && numBoxPoints > 0)
            break;

        f32 arg = -1.0;
        getRailPointArgF32NoInit(this, 0, i, &arg);

        if (arg < 0.0f) {
            railPointForHoleNum--;
            continue;
        }

        s32 val = 0;
        getRailPointArgS32AtPoint(this, 3, i, &val);

        if (val == 0)
            numSpherePoints++;

        if (val == 1)
            numBoxPoints++;
    }

    if (numSpherePoints == railPointForHoleNum)
        boxDropNum = 0;

    if (numBoxPoints == railPointForHoleNum)
        sphereDropNum = 0;

    OSReport("%d, %d\n", sphereDropNum, boxDropNum);
    mSphereHoleGroup = new DeriveActorGroup<ClipAreaDropScale>("クリップエリアのしずく管理ScaleSphere", sphereDropNum);
    for (int i = 0; i < sphereDropNum; i++) {
        ClipAreaDropScale* newScaleSphere = new ClipAreaDropScale("Yes", 0);
        newScaleSphere->initWithoutIter();
        mSphereHoleGroup->registerActor(newScaleSphere);
    }

    mBoxHoleGroup = new DeriveActorGroup<ClipAreaDropScale>("クリップエリアのしずく管理ScaleBox", boxDropNum);
    for (int i = 0; i < boxDropNum; i++) {
        ClipAreaDropScale* newScaleBox = new ClipAreaDropScale("Yes", 1);
        newScaleBox->initWithoutIter();
        mBoxHoleGroup->registerActor(newScaleBox);
    }
}


ClipAreaDropLaser::~ClipAreaDropLaser() {

}

namespace NrvClipAreaDropLaser {
    ClipAreaDropLaserNrvWait(ClipAreaDropLaserNrvWait::sInstance);
    ClipAreaDropLaserNrvMove(ClipAreaDropLaserNrvMove::sInstance);
}