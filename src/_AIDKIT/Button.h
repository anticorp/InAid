#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "HardInput.h"
#include <PinBase.h>
#include "Enums.h"
#include <LEDplexux.h>

namespace inaid
{
	class PullupButton : public _::HARDINPUT
	{
	public:
		PullupButton(uint32_t pinNumber, uint32_t debounceThresholdTime, uint32_t interClickTime, uint32_t waitToConfirmTime, uint32_t longPressThresholdTime, uint32_t ignoreAfterCycleEnd, plx::SingleLEDunit& _inLED, String name);
		operator bool();
		bool Initialize() override;
		bool Refresh() override;

	private:
	
	};	
}