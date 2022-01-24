#pragma once


#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "PinBase.h"
#include <ClockCache.h>
#include <LEDplexux.h>

namespace inaid
{

	namespace _
	{
		class HARDINPUT
		{
		public:
			HARDINPUT();
			virtual bool Refresh() {}
			virtual bool Initialize() {}
			void ResetAndClearButton();
			void Activate();
			void Deactivate();
			/*	Check if Button is currently Active. */
			bool Active();
			/*	Check if a Single Click is on the Que. Clears the Que when checked. */
			bool SingleClick();
			/*	Check if a Double Click is on the Que. Clears the Que when checked. */
			bool DoubleClick();
			/*	Check if a Triple Click is on the Que. Clears the Que when checked. */
			bool TripleClick();
			/*	Check if a Long Press is on the Que. Clears the Que when checked. */
			bool LongPress();
			/* Not Set Up - To peek state without clearing. */
			bool Peek();

		protected:
			/* Initializing/Settings Values*/
				/*	Pin of button */
			uint32_t mPinNo;
				/*	Time before confirming a button press. */
			uint32_t mMinDebounceThresholdTime;
				/* Time after debounce is confirmed and button is UP before confirming the click to the user. Should always be longer than inter-click time. */
			uint32_t mWaitToConfirmTime;
				/* Time after debounce confirmed and button lifted, before accepting next click. */
			uint32_t mInterClickWaitTime;	
			uint32_t mLongPressThresholdTime;
			uint32_t mIgnoreAfterCycleEnd;
			plx::SingleLEDunit* mButtonLED;
			String mName;
			/* Operation Vars */
			bool mConfigured = false;
			bool mInitalized = false;
			bool mActive;
			bool mLastClickInQueHasBeenRead;
			bool mHasLED;

			bool inPlay;
			uint8_t mCurrentClick;
			bool mClickTriggered[3];
			bool mClickConfirmed[3];
			bool mLongPressConfirmed;
			bool mLongPressThresholdMet;
			bool mCurrentTriggered;
			bool mCurrentConfirmed;
			bool mInterClickPeriod;
			bool mLastCycleState;

			bool mBlockNewCycleStart;

			pinbase::PINBASE mPinBase;
			uint32_t mUpTime;
			uint32_t mDownTime;
			ClockCache mLastExitTime;
			ClockCache mClickCycleStartClock;
			bool CloseOutPlayCycle();
			bool InPlay(bool pinState);
			bool StartPlay(bool pinState);
			bool Register(HARDINPUT* _inInput);

		private:

		};
	}
}