#pragma once

#include <vector>
#include <cstring>

#define FILELEN 256
#define BUFLEN 1024
#define RESERVE 100000

struct _vertex {
    float x, y, z;
};

struct _texture {
    float u, v;
};

struct _material {
    char name[FILELEN];
    char map_Kd[FILELEN];

    int ilumination;
    unsigned int texture;
    float Ns, Ni, d;
    float Ka[3], Kd[3], Ks[3], Tf[3];

    _material() {
        memset(this, 0, sizeof(_material));
    }
};

struct _face {
    int n;
    int v[5], vt[5], vn[5];

    _face() {
        memset(this, 0, sizeof(_face));
    }
};

struct _part {
    char name[FILELEN];
    std::vector<_face> faces;

    _part() {
        memset(this, 0, sizeof(_part));
    }
};

class Object {

private:
    bool loaded;
    char path[FILELEN];

    float pi;
    float theta;

    std::vector<_vertex> vertices;
    std::vector<_vertex> normals;
    std::vector<_texture> textures;
    std::vector<_material> materials;
    std::vector<_part> parts;

    bool loadObj(char *fname);
    bool loadMtl(char *fname);
    bool loadTex(char *fname, unsigned int *tex);

    int findMaterialIndex(char *fname);

public:
    Object();
    Object(char *objFile, char *mtlFile);

    void loadMaterialsTexture(void);
    bool load(char *objFile, char *mtlFile);
    void draw(float scale = 1.0f);

    float getPi(void) const;
    void setPi(float _pi);
    float getTheta(void) const;
    void setTheta(float _theta);
};