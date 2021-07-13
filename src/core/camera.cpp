#include "camera.h"

namespace rt3{

pair<real_type, real_type> Camera::get_uv_pos(int i, int j){
    real_type u_pos = sw.width() * (j + 0.5);
    u_pos /= film->width(); u_pos += sw.left;

    real_type v_pos = sw.height() * (i + 0.5);
    v_pos /= film->height(); v_pos += sw.bottom;

    return {u_pos, v_pos};
}

Camera::Camera(unique_ptr<Film> &&_film, Point3f _eye, Point3f _center, Vector3f _up, ScreenWindow _sw):
    film(std::move(_film)), eye(_eye), sw(_sw){
        Vector3f gaze = _center - _eye; 
        w = gaze.normalize();
        std::cout << w.toString() << std::endl;
        u = _up.cross(w).normalize();
        std::cout << u.toString() << std::endl;
        v = u.cross(w).normalize();
        std::cout << v.toString() << std::endl;
    }


Ray OrthographicCamera::generate_ray(int i, int j){
    auto [u_, v_] = get_uv_pos(i, j);
    Point3f origin = eye + (u * u_) + (v * v_);
    std::stringstream ss; 
    ss << "i " << i << "\n";
    ss << "j " << j << "\n";
    // ss << "u_ " << u_ << "\n";
    // ss << "v_ " << v_ << "\n";
    // ss << "origin " << origin.toString() << "\n";
    // RT3_MESSAGE(ss.str());
    return Ray(origin, w);
}


Ray PerspectiveCamera::generate_ray(int i, int j){
    auto [u_, v_] = get_uv_pos(i, j);
    Vector3f direction = w + (u * u_) + (v * v_);
    // std::stringstream ss; 
    // ss << "u_ " << u_ << "\n";
    // ss << "v_ " << v_ << "\n";
    // ss << "w " << w.at(0) << " " << w.at(1) << " " << w.at(2) << "\n";
    // ss << "u " << u.at(0) << " " << u.at(1) << " " << u.at(2) << "\n";
    // ss << "v " << v.at(0) << " " << v.at(1) << " " << v.at(2) << "\n";
    // ss << "eye " << eye.at(0) << " " << eye.at(1) << " " << eye.at(2) << "\n";
    // ss << "direction " << direction.at(0) << " " << direction.at(1) << " " << direction.at(2) << "\n";
    // RT3_MESSAGE(ss.str());
    return Ray(eye, direction);
}


OrthographicCamera::OrthographicCamera(
    unique_ptr<Film> &&_film, Point3f _eye, Point3f _center, Vector3f _up, ScreenWindow _sw): 
    Camera(std::move(_film), _eye, _center, _up, _sw){}


PerspectiveCamera::PerspectiveCamera(
    unique_ptr<Film> &&_film, Point3f _eye, Point3f _center, Vector3f _up, ScreenWindow _sw): 
    Camera(std::move(_film), _eye, _center, _up, _sw){}


OrthographicCamera* create_orthographic_camera(
    const ParamSet& ps_camera,
    const ParamSet& ps_look_at, unique_ptr<Film>&& the_film
){

    OrthographicCamera* camera = new OrthographicCamera(
        std::move(the_film),
        retrieve( ps_look_at,   "look_from",        Point3f({0.0, 0.1, 0.0})),
        retrieve( ps_look_at,   "look_at",          Point3f({0.0, 0.1, 0.0})),
        retrieve( ps_look_at,   "up",               Vector3f({0.0, 0.1, 0.0})),
        retrieve( ps_camera,    "screen_window",    ScreenWindow())
    );

    return camera;
}


PerspectiveCamera* create_perspective_camera(
    const ParamSet& ps_camera,
    const ParamSet& ps_look_at, unique_ptr<Film>&& the_film
){

    ScreenWindow sw;

    if( ps_camera.count("screen_window")){
        sw = retrieve(ps_camera, "screen_window", ScreenWindow());
    }else if(ps_camera.count("fovy")){
        
        real_type aspect = the_film->get_aspect();
        real_type fovy = retrieve(ps_camera, "fovy", real_type());
        real_type h = fabs(tan(fovy / 2));

        sw = ScreenWindow(
            h * aspect * -1,
            h * aspect,
            h * -1,
            h
        );

        std::cout << sw.left << " " << sw.right << " " << sw.top << " " << sw.bottom << std::endl;
    }else{
        RT3_ERROR("Can't compute screen window with given parameters.");
    }

    PerspectiveCamera* camera = new PerspectiveCamera(
        std::move(the_film),
        retrieve( ps_look_at, "look_from",  Point3f({0.0, 0.1, 0.0})),
        retrieve( ps_look_at, "look_at",    Point3f({0.0, 0.1, 0.0})),
        retrieve( ps_look_at, "up",         Vector3f({0.0, 0.1, 0.0})),
        sw
    );

    return camera;
}

} // namespace rt3