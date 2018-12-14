// Include section
#include <Godot.hpp>

#include "polygoniser.h"
#include "terraindata.h"

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
    godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
    godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
    godot::Godot::nativescript_init(handle);

    // Register classes
    godot::register_class<godot::MarchingCubes>();
    godot::register_class<godot::MarchingTetra>();
    godot::register_class<godot::TerrainData>();
}