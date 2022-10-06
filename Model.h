#ifndef _MODEL_H_
#define _MODEL_H_

#include <array>
#include <vector>
#include "Bitmap.h"
#include "Geometry.h"

class Model
{
public:
	Model(const std::string filename);

    int nverts() const;
    int nfaces() const;
    Vec3d normal(const int iface, const int nthvert) const;  // per triangle corner normal vertex
    Vec3d normal(const int iface) const;
    Vec3d normal(const Vec2d& uv) const;                     // fetch the normal vector from the normal map texture
    Vec3d vert(const int i) const;
    Vec3d vert(const int iface, const int nthvert) const;
    Vec2d uv(const int iface, const int nthvert) const;
    const Bitmap& diffuse()  const;
    const Bitmap& specular() const;

private:
    std::vector<Vec3d> verts{};     // array of vertices
    std::vector<Vec2d> tex_coord{}; // per-vertex array of tex coords
    std::vector<Vec3d> norms{};     // per-vertex array of normal vectors
    std::vector<int> facet_vrt{};
    std::vector<int> facet_tex{};   // per-triangle indices in the above arrays
    std::vector<int> facet_nrm{};
    Bitmap diffusemap;              // diffuse color texture
    Bitmap normalmap;               // normal map texture
    Bitmap specularmap;             // specular map texture
    void load_texture(const std::string filename, const std::string suffix, Bitmap& img);
};

#endif