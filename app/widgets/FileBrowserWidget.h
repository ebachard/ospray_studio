// Copyright 2018-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

namespace ospray_studio {

using FileList = std::vector<std::string>;
using FilterList = std::vector<std::string>;

// XXX tie this into the Importer extensions list
static const FilterList defaultFilterList = {".*",
    "Scene (.sg){.sg}",
    "Geometry (.obj .gltf .glb){.obj,.gltf,.glb}",
    "Volume (.raw .vdb){.raw,.vdb}",
    "Image (.jpg .hdr .exr){.jpg,.jpeg,.hdr,.exr}",
    "EULUMDAT (.ldt ){.ldt}"};

bool fileBrowser(FileList &fileList,
    const std::string &prompt,
    const bool allowMultipleSelection = false,
    const FilterList &filters = defaultFilterList);

} // namespace ospray_studio
