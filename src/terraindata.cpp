#include "terraindata.h"

namespace godot
{
    float TerrainData::get_voxel(Vector3 position)
    {
        if(_data.has(position))
            return _data[position];
        else
            return MAX_VOXEL_VALUE;
    }

    void TerrainData::set_voxel(Vector3 position, float value)
    {
        if(position != position.floor() && position != position.ceil())
            return;
        
        _data[position] = value;
        if(position.x > highest_values.x)
            highest_values.x = position.x;
        else if(position.x < lowest_values.x)
            lowest_values.x = position.x;
        
        if(position.y > highest_values.y)
            highest_values.y = position.y;
        else if(position.y < lowest_values.y)
            lowest_values.y = position.y;
        
        if(position.z > highest_values.z)
            highest_values.z = position.z;
        else if(position.z < lowest_values.z)
            lowest_values.z = position.z;
        
        Array args;
        args.append(position);
        emit_signal("voxels_changed", args);
    }

    GridCell TerrainData::get_grid_cell(Vector3 bottom_left)
    {
        GridCell cell;
        for(int i = 0; i < 8; i++)
        {
            cell.positions[i] = bottom_left + get_cube_vertex(i);
            cell.values[i] = this->get_voxel(cell.positions[i]);
        }
        return cell;
    }

    // GDNative
    void TerrainData::_init()
    {
        Array keys = _data.keys();
        for(int i = 0; i < keys.size(); i++)
        {
            Vector3 val = keys[i];
            if(val.x > highest_values.x)
                highest_values.x = val.x;
            else if(val.x < lowest_values.x)
                lowest_values.x = val.x;
            
            if(val.y > highest_values.y)
                highest_values.y = val.y;
            else if(val.y < lowest_values.y)
                lowest_values.y = val.y;
            
            if(val.z > highest_values.z)
                highest_values.z = val.z;
            else if(val.z < lowest_values.z)
                lowest_values.z = val.z;
        }
    }

    void TerrainData::_register_methods()
    {
        register_method("get_voxel", &TerrainData::get_voxel);
        register_method("set_voxel", &TerrainData::set_voxel);
        register_method("_init", &TerrainData::_init);
        register_method("get_cube_vertex", &TerrainData::get_cube_vertex);
        register_property<TerrainData, Dictionary>("_data", &TerrainData::_data, Dictionary());
        Dictionary args;
        args["changes"] = GODOT_VARIANT_TYPE_ARRAY;
        register_signal<TerrainData>("voxels_changed", args);
        register_method("get_highest_values", &TerrainData::get_highest_values);
        register_method("get_lowest_values", &TerrainData::get_lowest_values);
    }
}