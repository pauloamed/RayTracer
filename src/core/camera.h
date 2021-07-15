#ifndef CAMERA_H
#define CAMERA_H

#include "rt3.h"
#include "film.h"

namespace rt3{

class Camera{
public:
    unique_ptr<Film> film;
    Point3f eye; // position of the camera
    Vector3f w, u, v;
    ScreenWindow sw;

    Camera(unique_ptr<Film> &&film, Point3f eye, Point3f center, Vector3f up, ScreenWindow sw);
    
    pair<real_type, real_type> get_uv_pos(int i, int j);

    virtual Ray generate_ray(int x, int y) = 0;
    // virtual ~Camera() = 0;
};


class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(unique_ptr<Film> &&_film, Point3f _eye, Point3f _center, Vector3f _up, ScreenWindow sw);
    ~PerspectiveCamera();
    
    Ray generate_ray(int x, int y);
};


class OrthographicCamera : public Camera {
public:
    OrthographicCamera(unique_ptr<Film> &&_film, Point3f _eye, Point3f _center, Vector3f _up, ScreenWindow sw);
    ~OrthographicCamera();
    
    Ray generate_ray(int x, int y);
};


OrthographicCamera* create_orthographic_camera(
    const ParamSet& ps_camera,
    const ParamSet& ps_look_at, unique_ptr<Film>&& the_film
);


PerspectiveCamera* create_perspective_camera(
    const ParamSet& ps_camera,
    const ParamSet& ps_look_at, unique_ptr<Film>&& the_film
);


} // namespace rt3

#endif