#ifndef TERRAIN_DATA_H
#define TERRAIN_DATA_H
#include <Godot.hpp>

#include "polygoniser.h"

namespace godot
{
    class TerrainData : public Resource
    {
        // TODO: Redesign for file streaming, chunking, LOD
        // https://docs.godotengine.org/en/latest/development/cpp/custom_resource_format_loaders.html
        GODOT_CLASS(TerrainData, Resource)

        protected:
        Dictionary _data;

        Vector3 lowest_values;
        Vector3 highest_values;

        public:
        inline Vector3 get_cube_vertex(int index)
        {
            Vector3 CUBE_VERTICES[8] = {
            Vector3(0,0,0),
            Vector3(1,0,0),
            Vector3(1,0,1),
            Vector3(0,0,1),
            Vector3(0,1,0),
            Vector3(1,1,0),
            Vector3(1,1,1),
            Vector3(0,1,1)
            };
            return CUBE_VERTICES[index % 8];
        }
        static constexpr const float ISOLEVEL = 0;
        static constexpr const float MAX_VOXEL_VALUE = 1;
        static constexpr const float MIN_VOXEL_VALUE = -1;

        float get_voxel(Vector3 position);
        void set_voxel(Vector3 position, float value);

        GridCell get_grid_cell(Vector3 bottom_left);

        // GDNative
        void _init();
        static void _register_methods();
    };
}

#endif