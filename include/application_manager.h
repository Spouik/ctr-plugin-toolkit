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
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef CTR_PLUGIN_TOOLKIT_APPLICATION_MANAGER_H
#define CTR_PLUGIN_TOOLKIT_APPLICATION_MANAGER_H

#include <CTRPluginFramework/System/Controller.hpp>

#include "entity.h"

class ApplicationManager {
public:
  ApplicationManager(const ApplicationManager&) = delete;
  ApplicationManager& operator=(const ApplicationManager&) = delete;
  ApplicationManager(ApplicationManager&&) = delete;
  ApplicationManager& operator=(ApplicationManager&&) = delete;

  static ApplicationManager& GetInstance() {
    static ApplicationManager instance;
    return instance;
  }

  void SetApplication(Application* application) {
    application_ = application;
  }

  void Run() const;

private:
  static constexpr CTRPluginFramework::Key kMenuToggleKey =
      CTRPluginFramework::Select;
  static constexpr int kSleepTimeMs = 1;

  ApplicationManager() = default;

  Application* application_ = nullptr;
};

#endif // CTR_PLUGIN_TOOLKIT_APPLICATION_MANAGER_H