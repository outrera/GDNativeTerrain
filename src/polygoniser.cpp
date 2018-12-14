#include "polygoniser.h"

namespace godot
{
    Vector3 VertexInterp(float isolevel, Vector3 pos1, Vector3 pos2, float val1, float val2, bool alt_interp = false)
    {
        if (alt_interp)
        {
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
        } else
        {
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
        }
    }

    GridCell gen_gridcell_from_godot(PoolVector3Array positions, PoolRealArray values)
    {
        GridCell newcell;
        for(int i = 0; i < 8; i++)
        {
            newcell.positions[i] = positions[i];
            newcell.values[i] = values[i];
        }
        return newcell;
    }

    Array MarchingCubes::godot_polygonise(PoolVector3Array positions, PoolRealArray values, float isolevel)
    {
        return this->polygonise(gen_gridcell_from_godot(positions, values), isolevel);
    }

    Array MarchingTetra::godot_polygonise(PoolVector3Array positions, PoolRealArray values, float isolevel)
    {
        return this->polygonise(gen_gridcell_from_godot(positions, values), isolevel);
    }

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
            triangles.push_back(vertlist[triTable[cubeindex][i  ]]);
            triangles.push_back(vertlist[triTable[cubeindex][i+1]]);
            triangles.push_back(vertlist[triTable[cubeindex][i+2]]);
        }

        return triangles;
    }

    MarchingCubes::MarchingCubes() {}
    MarchingCubes::~MarchingCubes() {}

    /*
        Based on Paul Bourke's web page "Polygonising a scalar field"
        http://paulbourke.net/geometry/polygonise/source1.c
    */
    Array MarchingTetra::polygonise(GridCell cell, float isolevel)
    {
        Array vertices;
        PolygoniseTri(cell,isolevel,vertices,0,2,3,7);
        PolygoniseTri(cell,isolevel,vertices,0,2,6,7);
        PolygoniseTri(cell,isolevel,vertices,0,4,6,7);
        PolygoniseTri(cell,isolevel,vertices,0,6,1,2);
        PolygoniseTri(cell,isolevel,vertices,0,6,1,4);
        PolygoniseTri(cell,isolevel,vertices,5,6,1,4);
        return vertices;
    }

    /*
        Polygonise a tetrahedron given its vertices within a cube
        This is an alternative algorithm to polygonisegrid.
        It results in a smoother surface but more triangular facets.

                            + 0
                           /|\
                          / | \
                         /  |  \
                        /   |   \
                       /    |    \
                      /     |     \
                     +-------------+ 1
                    3 \     |     /
                       \    |    /
                        \   |   /
                         \  |  /
                          \ | /
                           \|/
                            + 2

        It's main purpose is still to polygonise a gridded dataset and
        would normally be called 6 times, one for each tetrahedron making
        up the grid cell.
        Given the grid labelling as in PolygniseGrid one would call
            PolygoniseTri(grid,iso,triangles,0,2,3,7);
            PolygoniseTri(grid,iso,triangles,0,2,6,7);
            PolygoniseTri(grid,iso,triangles,0,4,6,7);
            PolygoniseTri(grid,iso,triangles,0,6,1,2);
            PolygoniseTri(grid,iso,triangles,0,6,1,4);
            PolygoniseTri(grid,iso,triangles,5,6,1,4);
    */
    int MarchingTetra::PolygoniseTri(GridCell cell, float isolevel, Array &vertices, int v0, int v1, int v2, int v3)
    {
        int nverts = 0;
        int triindex = 0;

        /*
            Determine which of the 16 cases we have given which vertices
            are above or below the isosurface
        */
       if (cell.values[v0] < isolevel) triindex |= 1;
       if (cell.values[v1] < isolevel) triindex |= 2;
       if (cell.values[v2] < isolevel) triindex |= 4;
       if (cell.values[v3] < isolevel) triindex |= 8;

       /* Form the vertices of the triangles for each case */
       switch(triindex)
       {
            case 0x00:
            case 0x0F:
                break;
            case 0x0E:
            case 0x01:
                vertices.push_back(VertexInterp(isolevel, cell.positions[v0], cell.positions[v1], cell.values[v0], cell.values[v1]));
                vertices.push_back(VertexInterp(isolevel, cell.positions[v0], cell.positions[v2], cell.values[v0], cell.values[v2]));
                vertices.push_back(VertexInterp(isolevel, cell.positions[v0], cell.positions[v3], cell.values[v0], cell.values[v3]));
                nverts += 3;
                break;
            case 0x0D:
            case 0x02:
                vertices.push_back(VertexInterp(isolevel, cell.positions[v1], cell.positions[v0], cell.values[v1], cell.values[v0]));
                vertices.push_back(VertexInterp(isolevel, cell.positions[v1], cell.positions[v3], cell.values[v1], cell.values[v3]));
                vertices.push_back(VertexInterp(isolevel, cell.positions[v1], cell.positions[v2], cell.values[v1], cell.values[v2]));
                nverts += 3;
                break;
            case 0x0C:
            case 0x03:
                vertices.push_back(VertexInterp(isolevel, cell.positions[v0], cell.positions[v3], cell.values[v0], cell.values[v3]));
                vertices.push_back(VertexInterp(isolevel, cell.positions[v0], cell.positions[v2], cell.values[v0], cell.values[v2]));
                vertices.push_back(VertexInterp(isolevel, cell.positions[v1], cell.positions[v3], cell.values[v1], cell.values[v3]));
                nverts += 3;
                vertices.push_back(vertices[2]);
                vertices.push_back(VertexInterp(isolevel, cell.positions[v1], cell.positions[v2], cell.values[v1], cell.values[v2]));
                vertices.push_back(vertices[1]);
                nverts += 3;
                break;
            case 0x0B:
            case 0x04:
                vertices.push_back(VertexInterp(isolevel, cell.positions[v2], cell.positions[v0], cell.values[v2], cell.values[v0]));
                vertices.push_back( VertexInterp(isolevel, cell.positions[v2], cell.positions[v1], cell.values[v2], cell.values[v1]));
                vertices.push_back( VertexInterp(isolevel, cell.positions[v2], cell.positions[v3], cell.values[v2], cell.values[v3]));
                nverts += 3;
                break;
            case 0x0A:
            case 0x05:
                vertices.push_back(VertexInterp(isolevel, cell.positions[v0], cell.positions[v1], cell.values[v0], cell.values[v1]));
                vertices.push_back(VertexInterp(isolevel, cell.positions[v2], cell.positions[v3], cell.values[v2], cell.values[v3]));
                vertices.push_back(VertexInterp(isolevel, cell.positions[v0], cell.positions[v3], cell.values[v0], cell.values[v3]));
                nverts += 3;
                vertices.push_back(vertices[0]);
                vertices.push_back(VertexInterp(isolevel, cell.positions[v1], cell.positions[v2], cell.values[v1], cell.values[v2]));
                vertices.push_back(vertices[1]);
                nverts += 3;
                break;
            case 0x09:
            case 0x06:
                vertices.push_back(VertexInterp(isolevel, cell.positions[v0], cell.positions[v1], cell.values[v0], cell.values[v1]));
                vertices.push_back(VertexInterp(isolevel, cell.positions[v1], cell.positions[v3], cell.values[v1], cell.values[v3]));
                vertices.push_back(VertexInterp(isolevel, cell.positions[v2], cell.positions[v3], cell.values[v2], cell.values[v3]));
                nverts += 3;
                vertices.push_back(vertices[0]);
                vertices.push_back(VertexInterp(isolevel, cell.positions[v0], cell.positions[v2], cell.values[v0], cell.values[v2]));
                vertices.push_back(vertices[2]);
                nverts += 3;
                break;
            case 0x07:
            case 0x08:
                vertices.push_back(VertexInterp(isolevel, cell.positions[v3], cell.positions[v0], cell.values[v3], cell.values[v0]));
                vertices.push_back(VertexInterp(isolevel, cell.positions[v3], cell.positions[v2], cell.values[v3], cell.values[v2]));
                vertices.push_back(VertexInterp(isolevel, cell.positions[v3], cell.positions[v1], cell.values[v3], cell.values[v1]));
                nverts += 3;
                break;
       }
       return nverts;
    }

    MarchingTetra::MarchingTetra() {godot::Godot::print("Marching Tetrahedron is not completely functional. Meshes generated using it WILL be distorted and wrong.");};
    MarchingTetra::~MarchingTetra() {};

    // GDNative

    void MarchingTetra::_init() {};
    void MarchingTetra::_register_methods() {
        register_method("polygonise", &MarchingTetra::godot_polygonise);
        register_method("_init", &MarchingTetra::_init);
        register_property("use_alt_interp", &MarchingTetra::alt_interp, false);
    }

    void MarchingCubes::_init() {}
    void MarchingCubes::_register_methods() {
        register_method("polygonise", &MarchingCubes::godot_polygonise);
        register_method("_init", &MarchingCubes::_init);
        register_property("use_alt_interp", &MarchingCubes::alt_interp, false);
    }
}