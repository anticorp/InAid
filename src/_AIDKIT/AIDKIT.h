#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define HARDINPUT_STACK_SIZE_MAX 20

#include <ClockCache.h>
#include "HardInput.h"
#include <LoopTools.h>

namespace inaid
{
	namespace _
	{
		class INPUTAID
		{
		public:
			INPUTAID(uint32_t RefreshRate);
			void Refresh();
			void setRefreshRate(uint32_t);
			void Configure(uint32_t RefreshRate);
			void ResetAndClearAllButtons();

		protected:
			bool Register(HARDINPUT* _hardInput);
			friend class HARDINPUT;

		private:
			static bool mConfigured;
			static const size_t MaxHardInputs = HARDINPUT_STACK_SIZE_MAX;
			static HARDINPUT* mHardInput[MaxHardInputs];
			static uint8_t RegisteredHardInputs;
			static uint8_t StackIterator;
			static LOOPCOUNTER mLoopCount;
		};

	}
}
	extern inaid::_::INPUTAID InputAid;