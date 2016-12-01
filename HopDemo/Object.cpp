#include <cstdio>
#include <cstdlib>
#include <gl/glut.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

#include "Object.h"

Object::Object() {
    loaded = false;
    path[0] = '\0';

    pi = 0.0f;
    theta = 0.0f;

    vertices.reserve(RESERVE);
    normals.reserve(RESERVE);
    textures.reserve(RESERVE);
    materials.reserve(RESERVE);
    parts.reserve(RESERVE);
}

Object::Object(char *objFile, char *mtlFile) {
    loaded = false;
    path[0] = '\0';

    pi = 0.0f;
    theta = 0.0f;

    vertices.reserve(RESERVE);
    normals.reserve(RESERVE);
    textures.reserve(RESERVE);
    materials.reserve(RESERVE);
    parts.reserve(RESERVE);

    load(objFile, mtlFile);
}

void Object::loadMaterialsTexture(void) {
    for (unsigned int i = 0; i < materials.size(); ++i) {
        if (materials[i].map_Kd[0] && !materials[i].texture) {
            char path[FILELEN];
            _makepath(path, NULL, this->path, materials[i].map_Kd, NULL);
            loadTex(path, &materials[i].texture);
        }
    }
}

bool Object::load(char *objFile, char *mtlFile) {
    _splitpath(objFile, NULL, this->path, NULL, NULL);

    if (mtlFile && *mtlFile)
        loadMtl(mtlFile);
    if (objFile && *objFile)
        loadObj(objFile);

    loadMaterialsTexture();

    return (loaded = true);
}

void Object::draw(float scale) {
    int vtx_size = vertices.size();
    int tex_size = textures.size();
    int nml_size = normals.size();

    for (unsigned int h = 0; h < parts.size(); ++h) {
        int index = findMaterialIndex(parts[h].name);
        std::vector<_face> &faces = parts[h].faces;

        if (0 <= index) {
            _material &material = materials[index];

            if (material.texture) {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, material.texture);
            }
            else
                glColor3f(material.Kd[0], material.Kd[1], material.Kd[2]);
        }
        else
            glColor3f(0.7f, 0.7f, 0.7f);

        for (unsigned int i = 0, ip = faces.size(); i < ip; ++i) {
            glBegin(GL_POLYGON);
            for (int j = 0, jn = faces[i].n; j < jn; ++j) {
                int &v = faces[i].v[j];
                int &vt = faces[i].vt[j];
                int &vn = faces[i].vn[j];

                if (0 < vn && vn <= nml_size) {
                    _vertex &no = normals[vn - 1];
                    glNormal3f(no.x, no.y, no.z);
                }
                if (0 < vt && vt <= tex_size) {
                    _texture &tc = textures[vt - 1];
                    glTexCoord2f(tc.u, tc.v);
                }
                if (0 < v && v <= vtx_size) {
                    _vertex &vt = vertices[v - 1];
                    glVertex3f(scale * vt.x, scale * vt.y, scale * vt.z);
                }
            }
            glEnd();
        }
        glDisable(GL_TEXTURE_2D);
    }
}

float Object::getPi(void) const { return pi; }
void Object::setPi(float _pi) { pi = _pi; }
float Object::getTheta(void) const { return theta; }
void Object::setTheta(float _theta) { theta = _theta; };

bool Object::loadObj(char *fname) {
    FILE *fp = fopen(fname, "r");
    if (!fp)
        return false;

    _part part_ = _part();
    parts.push_back(part_);

    _part *part = (_part *)&(*parts.rbegin());

    char buffer[BUFLEN];
    while (fscanf(fp, "%s", buffer) != EOF) {
        bool go_eol = true;

        if (!strncmp("#", buffer, 1)) {
            // skip comment.
        }
        else if (!strcmp("v", buffer)) {
            // vertex.
            _vertex v;
            fscanf(fp, "%f %f %f", &v.x, &v.y, &v.z);
            vertices.push_back(v);
        }
        else if (!strcmp("vn", buffer)) {
            // normal.
            _vertex v;
            fscanf(fp, "%f %f %f", &v.x, &v.y, &v.z);
            normals.push_back(v);
        }
        else if (!strcmp("vt", buffer)) {
            // texture.
            _texture t;
            fscanf(fp, "%f %f", &t.u, &t.v);
            textures.push_back(t);
        }
        else if (!strcmp("f", buffer)) {
            // face.
            _face f;
            fgets(buffer, 1024, fp);

            char *p = buffer;
            for (int i = 0; i < 5; ++i) {
                while (*p == ' ' || *p == '\t')
                    p++;
                if (*p == '\0' || *p == '\r' || *p == '\n')
                    break;

                f.v[i] = strtoul(p, &p, 10);
                if (*p && *p == '/') {
                    p++;
                    f.vt[i] = strtoul(p, &p, 10);
                    if (*p && *p == '/') {
                        p++;
                        f.vn[i] = strtoul(p, &p, 10);
                    }
                }
                f.n++;
            }

            if (part)
                part->faces.push_back(f);

            go_eol = false;
        }
        else if (!strcmp("usemtl", buffer)) {
            // i don't know.
            _part part_;
            fscanf(fp, "%s", part_.name);
            parts.push_back(part_);
            part = (_part *)&(*parts.rbegin());
        }
        else if (!strcmp("mtllib", buffer)) {
            // use mtl file.
            fscanf(fp, "%s", buffer);
            char path[FILELEN];
            _makepath(path, NULL, this->path, buffer, NULL);
            loadMtl(path);
        }

        if (go_eol)
            fgets(buffer, 1024, fp);
    }
    fclose(fp);

    return true;
}

bool Object::loadMtl(char *fname) {
    FILE *fp = fopen(fname, "r");
    if (!fp)
        return false;

    _material *material = NULL;
    char buffer[BUFLEN];

    while (fscanf(fp, "%s", buffer) != EOF) {
        if (!strncmp("#", buffer, 1)) {
            // skip comment.
        }
        else if (!strcmp("newmtl", buffer)) {
            // i don't know.
            _material material_;
            fscanf(fp, "%s", material_.name);
            materials.push_back(material_);
            material = (_material *)&(*materials.rbegin());
        }
        else if (!strcmp("Ka", buffer)) {
            // ambient color.
            if (material)
                fscanf(fp, "%f %f %f", &material->Ka[0], &material->Ka[1], &material->Ka[2]);
        }
        else if (!strcmp("Kd", buffer)) {
            // difuse color.
            if (material)
                fscanf(fp, "%f %f %f", &material->Kd[0], &material->Kd[1], &material->Kd[2]);
        }
        else if (!strcmp("Ks", buffer)) {
            // specular color.
            if (material)
                fscanf(fp, "%f %f %f", &material->Ks[0], &material->Ks[1], &material->Ks[2]);
        }
        else if (!strcmp("Tf", buffer)) {
            // transmission color.
            if (material)
                fscanf(fp, "%f %f %f", &material->Tf[0], &material->Tf[1], &material->Tf[2]);
        }
        else if (!strcmp("illum", buffer)) {
            // ilumination.
            if (material)
                fscanf(fp, "%i", &material->ilumination);
        }
        else if (!strcmp("map_Kd", buffer)) {
            if (material)
                fscanf(fp, "%s", material->map_Kd);
        }
        else if (!strcmp("Ns", buffer)) {
            if (material)
                fscanf(fp, "%f", &material->Ns);
        }
        else if (!strcmp("Ni", buffer)) {
            if (material)
                fscanf(fp, "%f", &material->Ni);
        }
        else if (!strcmp("d", buffer)) {
            if (material)
                fscanf(fp, "%f", &material->d);
        }
        fgets(buffer, 1024, fp);
    }
    fclose(fp);
    return true;
}

bool Object::loadTex(char *fname, unsigned int *tex) {

    IplImage *img = cvLoadImage(fname);
    if (!img)
        return false;

    cvFlip(img, img);

    glGenTextures(1, tex);
    glBindTexture(GL_TEXTURE_2D, *tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, img->width, img->height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, img->imageData);

    cvReleaseImage(&img);
    return true;
}

int Object::findMaterialIndex(char *fname) {

    for (unsigned int i = 0; i < materials.size(); ++i) {
        if (!strcmp(fname, materials[i].name))
            return i;
    }

    return -1;
}