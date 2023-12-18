/**
 * @file personalityupdate.cpp
 *
 */
/* Copyright (C) 2021 by Arjan van Vught mailto:info@gd32-dmx.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <cassert>
#include <cstdint>

#include "displayudf.h"
#include "personalities.h"
#include "pixelpatterns.h"
#include "pixeltestpattern.h"
#include "rdmresponder.h"
#include "storepixeldmx.h"
#include "ws28xxdmx.h"

#include "debug.h"

void RDMResponder::PersonalityUpdate(uint32_t nPersonality)  {
	DEBUG_PRINTF("nPersonality=%u", nPersonality);

#if defined(ENABLE_CONFIG_PIDS)
	const auto type = Personalities::toPixelType(Personalities::fromPersonalityIdx(nPersonality));
	StorePixelDmx::Get()->SaveType(type);
#endif

	DisplayUdf::Get()->ClearLine(7);
	DisplayUdf::Get()->Printf(7, "%s:%d G%d %s",
					PixelType::GetType(WS28xxDmx::Get()->GetType()),
					WS28xxDmx::Get()->GetCount(),
					WS28xxDmx::Get()->GetGroupingCount(),
					PixelType::GetMap(WS28xxDmx::Get()->GetMap()));
	DisplayUdf::Get()->Show();

	assert(nPersonality < PERSONALITY_COUNT);
	switch (nPersonality)
	{
		default:
		{
			const auto nTestPattern = PixelTestPattern::GetPattern();

			if (nTestPattern == pixelpatterns::Pattern::NONE) {
			} else {
				DisplayUdf::Get()->ClearLine(6);
				DisplayUdf::Get()->Printf(6, "%s:%u", PixelPatterns::GetName(nTestPattern), static_cast<uint32_t>(nTestPattern));
			}
			break;
		}
#if !defined(ENABLE_CONFIG_PIDS)
		case Personalities::CONFIG_MODE:
		{
			DisplayUdf::Get()->ClearLine(3);
			DisplayUdf::Get()->ClearLine(4);
			DisplayUdf::Get()->Write(4, "Config Mode");
			DisplayUdf::Get()->ClearLine(5);
			break;
		}
#endif
	}
}
