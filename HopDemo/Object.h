#pragma once

#include <vector>

typedef std::vector<std::vector<float> > Arr2d;

struct Polygon {

    int nVtx;       ///< the number of vertices.
    int nAttr;      ///< the number of vertex attributes.
    Arr2d polygon;  ///< vertices.
};

class Object {

private:
    int nFace;                  ///< the number of faces.
    std::vector<Polygon> face;  ///< faces.

public:
    Object();
    Object(const char *fname);

    int getNFace(void) const;
    void setNFace(int _nFace);

    const Polygon& getFace(int idx) const;
    void pushFace(const Polygon& polygon);
};