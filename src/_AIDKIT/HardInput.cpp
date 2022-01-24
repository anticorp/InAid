#include "HardInput.h"
#include <ADM.h>
#include "AIDKIT.h"

namespace inaid
{
	using namespace adm::SHORTSTREAMKEYWORDS;
	namespace _
	{
		HARDINPUT::HARDINPUT()
		{
		}
		bool HARDINPUT::Register(HARDINPUT* _inInput)
		{
			return InputAid.Register(_inInput);
		}
		void HARDINPUT::ResetAndClearButton()
		{
			mLastClickInQueHasBeenRead = true;
			mClickTriggered[0] = false;
			mClickTriggered[1] = false;
			mClickTriggered[2] = false;
			mClickConfirmed[0] = false;
			mClickConfirmed[1] = false;
			mClickConfirmed[2] = false;
			mLongPressConfirmed = false;
			mLongPressThresholdMet = false;
			mCurrentTriggered = false;
			mCurrentConfirmed = false;
			mInterClickPeriod = false;
			mLastCycleState = false;
			mBlockNewCycleStart = false;
		}
		void HARDINPUT::Deactivate()
		{
			CloseOutPlayCycle();
		}
		bool HARDINPUT::Active()
		{
			return mActive;
		}
		bool HARDINPUT::SingleClick()
		{
			if(mClickConfirmed[0])
			{
				mClickConfirmed[0] = false;
				mLastClickInQueHasBeenRead = true;
				return true;
			}
			return false;
		}
		bool HARDINPUT::DoubleClick()
		{
			if(mClickConfirmed[1])
			{
				mClickConfirmed[1] = false;
				mLastClickInQueHasBeenRead = true;
				return true;
			}			
			return false;
		}
		bool HARDINPUT::TripleClick()
		{
			if(mClickConfirmed[2])
			{
				mClickConfirmed[2] = false;
				mLastClickInQueHasBeenRead = true;
				return true;
			}
			return false;
		}
		bool HARDINPUT::LongPress()
		{
			if (mLongPressConfirmed)
			{
				mLongPressConfirmed = false;
				mLastClickInQueHasBeenRead = true;
				return true;
			}
			return false;
		}
		bool HARDINPUT::Peek()
		{
			return mCurrentTriggered;
		}
		bool HARDINPUT::StartPlay(bool pinState)
		{
			mButtonLED->PreBlipDarken(0);
			/*		Only called if button pressed, and the last confirmed
				button press has been read from buffer */
			mLastClickInQueHasBeenRead = false;
			inPlay = true;
			mUpTime = 0;
			mDownTime = 0;
			mCurrentClick = 0;
			mLongPressConfirmed = false;
			mClickTriggered[0] = true;
			mClickCycleStartClock.Mark();
			mLastExitTime.Mark();
			mLastCycleState = true;
			return true;
		}
		bool HARDINPUT::InPlay(bool pinState)
		{
			uint32_t lastExitDelta = mLastExitTime.Delta();
			if(pinState)
			{
				mUpTime = mUpTime + lastExitDelta;
				if(mLongPressThresholdMet) {
					/* If Long Press is confirmed already, do nothing until the button is lifted. */
					return true;
				}
				if(mInterClickPeriod)
				{ /* ON again after a short OFF */
					
					if((mDownTime + mLastExitTime) > mInterClickPeriod)
					{
						if(mCurrentClick < 2)
						{
							mClickTriggered[mCurrentClick] = false;
							mCurrentClick++;
							mCurrentConfirmed = false;
							mUpTime = 0;
							mDownTime = 0;
							mButtonLED->PreBlipDarken(0);
							mClickTriggered[mCurrentClick] = true;
							mClickCycleStartClock.Mark();
							mLastExitTime.Mark();
							mLastCycleState = true;
							mInterClickPeriod = false;
							mBlockNewCycleStart = true; // Prevents start of next cycle immediately after this one is done. Usually because button is still down for a few moments.
							return true;
						}
						else
						{
							CloseOutPlayCycle();
							return true;
						}
					}
				}

				if(mCurrentConfirmed)
				{
					if(mUpTime > mLongPressThresholdTime)
					{
						mButtonLED->Blip(900,255);
						mLongPressThresholdMet = true;
						mCurrentConfirmed = false;
						mClickTriggered[mCurrentClick] = false;
						mLastExitTime.Mark();
						return true;
					}
					mLastExitTime.Mark();
					return true;
				}
				if(mUpTime > mMinDebounceThresholdTime)
				{
					mCurrentConfirmed = true;
					mLastExitTime.Mark();
					return true;
				}
				mCurrentTriggered = true;
				mLastCycleState = true;
				mLastExitTime.Mark();
				return true;
			}
			else  /* PIN OFF */
			{
				mDownTime = mDownTime + lastExitDelta;
				
				if(mCurrentConfirmed || mLongPressThresholdMet)
				{
					if(mDownTime > mWaitToConfirmTime)
					{
						mButtonLED->Blip(400, 255);
						CloseOutPlayCycle();
						return true;
					}
					if(mDownTime > mInterClickWaitTime)
					{
						mInterClickPeriod = true;
						mLastExitTime.Mark();
						mLastCycleState = false;
						return true;
					}
				}
				/* Short Click Bailout -> */
				if((mDownTime > mWaitToConfirmTime) && !mCurrentConfirmed)
				{
					CloseOutPlayCycle();
					return true;
				}
				mLastCycleState = false;
				mLastExitTime.Mark();
			}
		}
		bool HARDINPUT::CloseOutPlayCycle()
		{
			/* Only here after timeout is confirmed. */
			mClickConfirmed[mCurrentClick] =  mClickTriggered[mCurrentClick] && mCurrentConfirmed;
			mLongPressConfirmed = mLongPressThresholdMet;		
			mLastClickInQueHasBeenRead = !mClickConfirmed[mCurrentClick] || !mLongPressConfirmed;
			mLongPressThresholdMet = false;
			inPlay = false;
			mClickTriggered[mCurrentClick] = false;
			mCurrentConfirmed = false;
			mInterClickPeriod = false;
			mLastCycleState = false;
			mCurrentClick = 0;
			mUpTime = 0;
			mDownTime = 0;
			mBlockNewCycleStart = true;
			mButtonLED->UnlockBlipDarken();
			return true;

		}
	}
}

