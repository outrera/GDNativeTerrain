#include "polygoniser.h"

#define INTERP_METHOD 0

namespace godot
{
    /*
        Based on Paul Bourke's web page "Polygonising a scalar field"
        http://paulbourke.net/geometry/polygonise/
    */
    Array MarchingCubes::polygonise(GridCell cell, float isolevel)
    {
        /* 
            Determine the index into the edge table
            which tells us which vertices are inside of the surface
        */

        int cubeindex = 0;
        if (cell.values[0] < isolevel) cubeindex |= 1;
        if (cell.values[1] < isolevel) cubeindex |= 2;
        if (cell.values[2] < isolevel) cubeindex |= 4;
        if (cell.values[3] < isolevel) cubeindex |= 8;
        if (cell.values[4] < isolevel) cubeindex |= 16;
        if (cell.values[5] < isolevel) cubeindex |= 32;
        if (cell.values[6] < isolevel) cubeindex |= 64;
        if (cell.values[7] < isolevel) cubeindex |= 128;

        /* Cube is entirely in/out of the surface */
        if (edgeTable[cubeindex] == 0)
        {
            Godot::print("DLL: returning empty array");
            return Array();
        }

        /* Find the vertices where the surface intersects the cube */
        Vector3 vertlist[12];
        if (edgeTable[cubeindex] & 1)
            vertlist[0] =
                VertexInterp(isolevel,cell.positions[0],cell.positions[1],cell.values[0],cell.values[1]);
        if (edgeTable[cubeindex] & 2)
            vertlist[1] =
                VertexInterp(isolevel,cell.positions[1],cell.positions[2],cell.values[1],cell.values[2]);
        if (edgeTable[cubeindex] & 4)
            vertlist[2] =
                VertexInterp(isolevel,cell.positions[2],cell.positions[3],cell.values[2],cell.values[3]);
        if (edgeTable[cubeindex] & 8)
            vertlist[3] =
                VertexInterp(isolevel,cell.positions[3],cell.positions[0],cell.values[3],cell.values[0]);
        if (edgeTable[cubeindex] & 16)
            vertlist[4] =
                VertexInterp(isolevel,cell.positions[4],cell.positions[5],cell.values[4],cell.values[5]);
        if (edgeTable[cubeindex] & 32)
            vertlist[5] =
                VertexInterp(isolevel,cell.positions[5],cell.positions[6],cell.values[5],cell.values[6]);
        if (edgeTable[cubeindex] & 64)
            vertlist[6] =
                VertexInterp(isolevel,cell.positions[6],cell.positions[7],cell.values[6],cell.values[7]);
        if (edgeTable[cubeindex] & 128)
            vertlist[7] =
                VertexInterp(isolevel,cell.positions[7],cell.positions[4],cell.values[7],cell.values[4]);
        if (edgeTable[cubeindex] & 256)
            vertlist[8] =
                VertexInterp(isolevel,cell.positions[0],cell.positions[4],cell.values[0],cell.values[4]);
        if (edgeTable[cubeindex] & 512)
            vertlist[9] =
                VertexInterp(isolevel,cell.positions[1],cell.positions[5],cell.values[1],cell.values[5]);
        if (edgeTable[cubeindex] & 1024)
            vertlist[10] =
                VertexInterp(isolevel,cell.positions[2],cell.positions[6],cell.values[2],cell.values[6]);
        if (edgeTable[cubeindex] & 2048)
            vertlist[11] =
                VertexInterp(isolevel,cell.positions[3],cell.positions[7],cell.values[3],cell.values[7]);
        
        /* Create the triangles */
        Array triangles;
        for(int i = 0; triTable[cubeindex][i] != -1; i+=3) {
            triangles.append(vertlist[triTable[cubeindex][i  ]]);
            triangles.append(vertlist[triTable[cubeindex][i+1]]);
            triangles.append(vertlist[triTable[cubeindex][i+2]]);
        }

        return triangles;
    }

    Vector3 MarchingCubes::VertexInterp(float isolevel, Vector3 pos1, Vector3 pos2, float val1, float val2)
    {
        #if INTERP_METHOD == 1
            if (pos2 < pos1)
            {
                Vector3 temp;
                temp = pos1;
                pos1 = pos2;
                pos2 = temp;
            }

            Vector3 p;
            if(fabs(val1 - val2) > 0.00001)
                p = pos1 + (pos2 - pos1) / (val2 - val1) * (isolevel - val1);
            else
                p = pos1;
            
            return p;
        #else
            float mu;
            Vector3 p;

            if (fabs(isolevel - val1) < 0.00001)
                return pos1;
            if (fabs(isolevel - val2) < 0.00001)
                return pos2;
            if (fabs(val1-val2) < 0.00001)
                return pos1;
            
            mu = (isolevel - val1) / (val2 - val1);
            p = pos1 + Vector3(mu, mu, mu) * (pos2 - pos1);
            return p;
        #endif
    }

    Array MarchingCubes::godot_polygonise(PoolVector3Array positions, PoolRealArray values, float isolevel)
    {
        GridCell newcell;
        for(int i = 0; i < 8; i++)
        {
            newcell.positions[i] = positions[i];
            newcell.values[i] = values[i];
        }
        return this->polygonise(newcell, isolevel);
    }

    MarchingCubes::MarchingCubes() {}

    MarchingCubes::~MarchingCubes() {}

    //GDNative
    void MarchingCubes::_init() {}

    void MarchingCubes::_register_methods() {
        register_method("polygonise", &MarchingCubes::godot_polygonise);
        register_method("_init", &MarchingCubes::_init);
    }
}