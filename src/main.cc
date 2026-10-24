/*
 * MIT License
 *
 * Copyright (c) 2026 David Darras
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <CTRPluginFramework.hpp>
#include <string>
#include "application_manager.h"
#include "home_application.h"

namespace CTRPluginFramework {

    enum Location { NONE, TOP, BOTTOM };

    bool DebugOSD(const Screen& screen) {
        u32 y = 10;
        u8 readValue = 0;
        y = screen.Draw("SSM loaded", 10, y);
        Process::Read8(0x0014F690, readValue); 
        char buf[32];
        snprintf(buf, sizeof(buf), "Lock Address : 0x%02X", readValue);
        y = screen.Draw(buf, 10, y);
        if (readValue == 0x09)
        {
            y = screen.Draw("UNLOCKED", 10, y, Color::Green);
        }
        else
        {
            y = screen.Draw("LOCKED", 10, y, Color::Red);
        }
        Process::Read8(0x00168adc, readValue); 
        buf[32];
        snprintf(buf, sizeof(buf), "Odds Address : 0x%02X", readValue);
        y = screen.Draw(buf, 10, y);
        if (readValue == 0x08)
        {
            y = screen.Draw("8192 ODDS", 10, y, Color::Green);
        }
        else
        {
            y = screen.Draw("4096 ODDS", 10, y, Color::Red);
        }
        
        
        return true;
    }

    bool delock = false;
    bool oldOdds = false;
    bool notifyScreen = TOP;
    bool debug = true;

    int main() {
        Process::Write8(0x0014F690, 0x09);
        Process::Write8(0x00168adc, 0x08);
        if (debug)
        {
            OSD::Run(DebugOSD);
        }
        
        
        // File* monFichier = new File();
        // monFichier->Create("./monFichier");
        ApplicationManager& application_manager = ApplicationManager::GetInstance();
        HomeApplication home_application;
        application_manager.SetApplication(&home_application);
        application_manager.Run();
        return 0;
    }
}  // namespace CTRPluginFramework