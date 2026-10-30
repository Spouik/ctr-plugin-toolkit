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
#include "config_file.h"

namespace CTRPluginFramework {



    static Config gConfig;

    bool debugOSD(const Screen& screen) {
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
        std::string maChaine = "Read result = " + std::to_string(gConfig.result);
        y = screen.Draw(maChaine, 10, y);
        y = screen.Draw(std::to_string(gConfig.debug), 10, y);
        y = screen.Draw(std::to_string(gConfig.delock), 10, y);
        y = screen.Draw(std::to_string(gConfig.old_odds), 10, y);
        y = screen.Draw(std::to_string(gConfig.notify), 10, y);




        return true;
    }

    int main() {
        if (!(File::Exists("./config.ini")))
        {
            File* configFile = new File();
            File::Create("./config.ini");
            File::Open(*configFile, "./config.ini");
            configFile->WriteLine("# SETTINGS\ndebug = true\ndelock = false\nold_odds = false\nnotify = TOP");
            configFile->Close();
        }
        ConfigFile::load(gConfig);
        // gConfig.test2 = ConfigFile::clean(ConfigFile::readLineFromPath("config.ini", 1));
            // File configFile = File("config.ini", File::READ);
            // std::string content(configFile.GetSize(), '\0');
            // configFile.Read(&content[0], content.size());
            // configFile.Close();
        // gConfig.test2 = std::to_string(ConfigFile::countLineFromContent(content));
        // gConfig.test2 = ConfigFile::testReadParamLocation("notify", content);
        // gConfig.test2 = ConfigFile::clean(ConfigFile::readLineFromContent(content, 1));
        gConfig.delock = true;
        ConfigFile::write(gConfig, "config.ini");


        if (gConfig.delock)
        {
            CTRPluginFramework::Process::Write8(0x0014F690, 0x09);
        }
        else
        {
            CTRPluginFramework::Process::Write8(0x0014F690, 0x18);
        }

        if (gConfig.old_odds)
        {
            CTRPluginFramework::Process::Write8(0x00168adc, 0x08);
        }
        else
        {
            CTRPluginFramework::Process::Write8(0x00168adc, 0x10);
        }
        if (gConfig.debug)
        {
            OSD::Run(debugOSD);
        }


        // File* monFichier = new File();
        // monFichier->Create("./monFichier");
        ApplicationManager& application_manager = ApplicationManager::GetInstance();
        HomeApplication home_application;
        application_manager.SetApplication(&home_application);
        application_manager.Run(gConfig);
        return 0;
    }
}  // namespace CTRPluginFramework