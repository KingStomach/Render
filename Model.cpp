//=====================================================================
//
// Model.cpp - 该文件参考了 tinyrender 的 Model.cpp 
//
//=====================================================================

#include <fstream>
#include <iostream>
#include <sstream>
#include "Model.h"

Model::Model(const std::string filename)
{
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3d v;
            for (int i = 0; i < 3; i++) iss >> v[i];
            verts.emplace_back(v);
        }
        else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            Vec3d n;
            for (int i = 0; i < 3; i++) iss >> n[i];
            //norms.emplace_back(n.normalize());
        }
        else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            Vec2d uv;
            for (int i = 0; i < 2; i++) iss >> uv[i];
            tex_coord.emplace_back(Vec2d(uv.x, 1 - uv.y));
        }
        else if (!line.compare(0, 2, "f ")) {
            int f, t, n;
            iss >> trash;
            int cnt = 0;
            while (iss >> f >> trash >> t >> trash >> n) {
                facet_vrt.emplace_back(--f);
                facet_tex.emplace_back(--t);
                facet_nrm.emplace_back(--n);
                cnt++;
            }
            if (3 != cnt) {
                std::cerr << "Error: the obj file is supposed to be triangulated" << std::endl;
                in.close();
                return;
            }
        }
    }
    in.close();
    std::cerr << "# v# " << nverts() << " f# " << nfaces() << " vt# " << tex_coord.size() << " vn# " << norms.size() << std::endl;
    load_texture(filename, "_diffuse.bmp", diffusemap);
    load_texture(filename, "_nm.bmp", normalmap);
    load_texture(filename, "_spec.bmp", specularmap);
}

int Model::nverts() const {
    return verts.size();
}

int Model::nfaces() const {
    return facet_vrt.size() / 3;
}

Vec3d Model::vert(const int i) const {
    return verts[i];
}

Vec3d Model::vert(const int iface, const int nthvert) const {
    return verts[facet_vrt[iface * 3 + nthvert]];
}

void Model::load_texture(std::string filename, const std::string suffix, Bitmap& img) {
    size_t dot = filename.find_last_of(".");
    if (dot == std::string::npos) return;
    std::string texfile = filename.substr(0, dot) + suffix;
    std::cerr << "texture file " << texfile << " loading " << (img.LoadFile(texfile.c_str()) ? "ok" : "failed") << std::endl;
}

Vec3d Model::normal(const Vec2d& uvf) const {
    const Color &c = normalmap.Sample2D(uvf);
    return Vec3d((double)c.r, (double)c.g, (double)c.b) * 2.0 / 255.0 - Vec3d(1, 1, 1);
}

Vec2d Model::uv(const int iface, const int nthvert) const {
    return tex_coord[facet_tex[iface * 3 + nthvert]];
}

Vec3d Model::normal(const int iface, const int nthvert) const {
    return norms[facet_nrm[iface * 3 + nthvert]];
}