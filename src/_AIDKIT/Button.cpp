#include "Button.h"
#include <ADM.h>
#include "HardInput.h"

namespace inaid
{
	using namespace adm::SHORTSTREAMKEYWORDS;

	PullupButton::PullupButton(uint32_t pinNumber, uint32_t debounceThresholdTime, uint32_t interClickTime, uint32_t waitToConfirmTime, uint32_t longPressThresholdTime, uint32_t ignoreAfterCycleEnd, plx::SingleLEDunit& _inLED, String name)
	{
		if (!mConfigured)
		{	mPinNo = pinNumber;
			if (mPinBase.Configure(pinNumber, INPUT_PULLUP, false))
			{
				mName = name;
				mMinDebounceThresholdTime = debounceThresholdTime;
				mInterClickWaitTime = interClickTime;
				mWaitToConfirmTime = waitToConfirmTime;
				mLongPressThresholdTime = longPressThresholdTime;
				mIgnoreAfterCycleEnd = ignoreAfterCycleEnd;
				mButtonLED = &_inLED;
				mClickTriggered[0] = false;
				mClickTriggered[1] = false;
				mClickTriggered[2] = false;
				mClickConfirmed[0] = false;
				mClickConfirmed[1] = false;
				mClickConfirmed[2] = false;
				mLastClickInQueHasBeenRead = true;
				mLongPressConfirmed = false;
				mLongPressThresholdMet = false;
				mCurrentTriggered = false;
				mCurrentConfirmed = false;
				mLastCycleState = false;
				inPlay = false;
				mBlockNewCycleStart = false;
				mConfigured = true;
				mInitalized = false;
			}
		}
	}
	PullupButton::operator bool()
	{
	}

	bool PullupButton::Initialize()
	{		
		if(mConfigured && !mInitalized)
		{			
			if(mPinBase.Initialize())
			{				
				_::HARDINPUT* me = this;
				mInitalized = Register(me);
				mActive = mInitalized;			
				return mInitalized;
			}
		}
		return false;
	}

	bool PullupButton::Refresh()
	{
		if (inPlay)
		{
			bool pinState = !mPinBase.Read();			
			InPlay(pinState);
		}
		else
		{
			if(mBlockNewCycleStart)
			{
				if(mLastExitTime.Delta() > mIgnoreAfterCycleEnd)
				{
					mBlockNewCycleStart = false;
				}
				else
				{
					return true;
				}
			}
			if(mLastClickInQueHasBeenRead)
			{
				if (!mPinBase.Read()) StartPlay(true);
			}
		}
		return false;
	}
}
