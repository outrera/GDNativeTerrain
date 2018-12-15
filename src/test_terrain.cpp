#include <Texture.hpp>
#include <SurfaceTool.hpp>
#include <ArrayMesh.hpp>

#include "test_terrain.h"

namespace godot
{
    void TestTerrain::update_terrain()
    {
        if(data.is_null())
            return;
        Vector3 high = data->get_highest_values();
        Vector3 low = data->get_lowest_values();
        if (high == low)
            return;
        
        MarchingCubes poly = MarchingCubes();
        st->begin(Mesh::PRIMITIVE_TRIANGLES);
        for(int x = low.x - 1;x <= high.x; x++)
        {
            for(int y = low.y - 1; y <= high.y; y++)
            {
                for(int z = low.z - 1; z <= high.z; z++)
                {
                    Array vertices = poly.polygonise(data->get_grid_cell(Vector3(x, y, z)), data->ISOLEVEL);
                    for(int v = 0; v < vertices.size(); v++)
                    {
                        st->add_vertex(vertices[v]);
                    }
                }
            }
        }
        st->generate_normals();
        this->set_mesh(st->commit());
    }

    void TestTerrain::_init()
    {
        st.instance();
    }

    void TestTerrain::_register_methods()
    {
        register_method("update_terrain", &TestTerrain::update_terrain);
        register_method("_init", &TestTerrain::_init);
        register_property<TestTerrain, Ref<TerrainData>>("terrain_data", &TestTerrain::data, Ref<TerrainData>());
    }
}