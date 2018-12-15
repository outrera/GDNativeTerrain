#ifndef TEST_TERRAIN_H
#define TEST_TERRAIN_H

#include <Godot.hpp>
#include <MeshInstance.hpp>
#include <Mesh.hpp>
#include <SurfaceTool.hpp>

#include "polygoniser.h"
#include "terraindata.h"

namespace godot
{
    class TestTerrain : public MeshInstance
    {
        GODOT_CLASS(TestTerrain, MeshInstance)
        protected:
        Ref<SurfaceTool> st;

        public:
        Ref<TerrainData> data;
        
        void update_terrain();

        // GDNative
        void _init();
        static void _register_methods();
    };
}

#endif