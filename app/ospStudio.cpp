// ======================================================================== //
// Copyright 2018-2019 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#include "MainWindow.h"
#include "example_common.h"

using namespace ospray;
using ospcommon::make_unique;

int main(int argc, const char *argv[])
{

  bool denoiser = ospLoadModule("denoiser") == OSP_NO_ERROR;

  initializeOSPRay(argc, argv);

  auto window = make_unique<MainWindow>(vec2i(1024, 768), denoiser);
  window->parseCommandLine(argc, argv);
  window->mainLoop();
  window.reset();

  ospShutdown();

  return 0;
}
