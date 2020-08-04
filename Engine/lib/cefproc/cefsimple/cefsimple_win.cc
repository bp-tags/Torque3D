// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include <windows.h>

#include "cefsimple/simple_app.h"
#include "include/cef_sandbox_win.h"


// When generating projects with CMake the CEF_USE_SANDBOX value will be defined
// automatically if using the required compiler version. Pass -DUSE_SANDBOX=OFF
// to the CMake command-line to disable use of the sandbox.
// Uncomment this line to manually enable sandbox support.
// #define CEF_USE_SANDBOX 1

#if defined(CEF_USE_SANDBOX)
// The cef_sandbox.lib static library is currently built with VS2013. It may not
// link successfully with other VS versions.
#pragma comment(lib, "cef_sandbox.lib")
#endif


// Entry point function for all processes.
int APIENTRY wWinMain(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPTSTR    lpCmdLine,
                      int       nCmdShow)
{
   // This process is only ever the sub-processs, so the parameters are not needed
   UNREFERENCED_PARAMETER(hInstance);
   UNREFERENCED_PARAMETER(hPrevInstance);
   UNREFERENCED_PARAMETER(lpCmdLine);
   UNREFERENCED_PARAMETER(nCmdShow);

  void* sandbox_info = NULL;

  // Enable High-DPI support on Windows 7 or newer.
  CefEnableHighDPISupport();

#if defined(CEF_USE_SANDBOX)
  // Manage the life span of the sandbox information object. This is necessary
  // for sandbox support on Windows. See cef_sandbox_win.h for complete details.
  CefScopedSandboxInfo scoped_sandbox;
  sandbox_info = scoped_sandbox.sandbox_info();
#endif

  // Provide CEF with command-line arguments.
  CefMainArgs main_args(hInstance);

  // SimpleApp implements application-level callbacks for the render process.
  // It will bind the JS to TS function in OnContextCreated() after CEF has initialized.
  CefRefPtr<SimpleApp> app(new SimpleApp);

  // CEF applications have multiple sub-processes (render, plugin, GPU, etc)
  // that share the same executable. This function checks the command-line and,
  // if this is a sub-process, executes the appropriate logic.
  int exit_code = CefExecuteProcess(main_args, app.get(), sandbox_info);
  if (exit_code >= 0) {
    // The sub-process has completed so return here.
    return exit_code;
  }

  return 0;
}