#include "AIDKIT.h"
#include <ADM.h>

namespace inaid
{
	using namespace adm::SHORTSTREAMKEYWORDS;
	namespace _
	{
		bool INPUTAID::mConfigured = false;
		LOOPCOUNTER INPUTAID::mLoopCount;
		HARDINPUT* INPUTAID::mHardInput[MaxHardInputs];
		uint8_t INPUTAID::RegisteredHardInputs = 0;
		uint8_t INPUTAID::StackIterator = 0;

		INPUTAID::INPUTAID(uint32_t RefreshRate)
		{
			Configure(RefreshRate);
		}
		void INPUTAID::Configure(uint32_t RefreshRate)
		{
			if (!mConfigured)
			{
				//mLoopCounter.Configure(COUNTMETHOD::PeriodReset, RefreshRate);
				mConfigured = true;
				RegisteredHardInputs = 0;
				StackIterator = 0;
			}
		}
		void INPUTAID::ResetAndClearAllButtons()
		{
			if (mConfigured)
			{
				uint8_t i = 0;
				while (true)
				{
					uint8_t NextIterator = (i + 1) % (RegisteredHardInputs);
					if (!i) break;
					if(mHardInput[i]->Active())
					{
						mHardInput[i]->ResetAndClearButton();
					}
					i = NextIterator;					
				}
			}
		}
		void INPUTAID::Refresh()
		{
			if (mConfigured)
			{
				if (mLoopCount.Advance())
				{
					uint8_t NextIterator = (StackIterator + 1) % (RegisteredHardInputs);
					if (mHardInput[StackIterator]->Active())
					{
						mHardInput[StackIterator]->Refresh();
					}
					StackIterator = NextIterator;					
				}
			}
		}
		void INPUTAID::setRefreshRate(uint32_t refreshRate)
		{
			if (mConfigured)
			{
				mLoopCount.SetResetPeriod(refreshRate);
			}
		}
		bool INPUTAID::Register(HARDINPUT* _hardInput)
		{
			if (mConfigured)
			{
				uint32_t i = (uint32_t)(RegisteredHardInputs + 1) % (MaxHardInputs + 1);
				if (i)
				{
					mHardInput[RegisteredHardInputs] = _hardInput;
					RegisteredHardInputs = i;
					return true;
				}
			}
			return false;
		}
	}
}
	inaid::_::INPUTAID InputAid = inaid::_::INPUTAID(10);