#include "Game/LiveActor.h"
#include "BlueCoin.h"


class ExtActorActionKeeper : public ActorActionKeeper {
public:
    ExtActorActionKeeper(LiveActor *pActor, ActorAnimKeeper *pAnimKeeper, ActionFlagCtrl *pFlagCtrl, ActorPadAndCameraCtrl *pPadAndCameraCtrl,
        ActorHitReactionCtrl *pHitReactionCtrl, ActorItemGenerator *pItemGenerator, ActorParam *pActorParam, ActionScreenBlurCtrl *pScreenBlurCtrl,
        ActionFootPrintCtrl *pFootPrintCtrl, ActionBgmCtrl *pBgmCtrl);

    BlueCoin* mBlueCoin;
};