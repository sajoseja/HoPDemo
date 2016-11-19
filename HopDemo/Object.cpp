#include <cstdio>

#include "Object.h"

Object::Object()
    : nFace(0)
{}

Object::Object(const char *fname) {

    FILE *fp = fopen(fname, "r");

    fscanf(fp, " %d", &nFace);
    for (int i = 0; i < nFace; ++i) {
        
        Polygon tmp;
        fscanf(fp, " %d %d", &tmp.nVtx, &tmp.nAttr);
        tmp.polygon.assign(tmp.nVtx, std::vector<float>(tmp.nAttr, 0));
        
        for (int j = 0; j < tmp.nVtx; ++j) {
            for (int k = 0; k < tmp.nAttr; ++k)
                fscanf(fp, " %f", &tmp.polygon[j][k]);
        }

        face.push_back(tmp);
    }

    fclose(fp);
}

int Object::getNFace(void) const { return nFace; }
void Object::setNFace(int _nFace) { nFace = _nFace; }

const Polygon& Object::getFace(int idx) const { return face[idx]; }
void Object::pushFace(const Polygon& polygon) { face.push_back(polygon); }