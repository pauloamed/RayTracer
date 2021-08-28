#include "api.h"

namespace rt3 {

API::APIState API::curr_state = API::APIState::Uninitialized;
RunningOptions API::curr_run_opt;
unique_ptr<RenderOptions> API::render_opt;
GraphicsState API::curr_GS;
GraphicsContext API::curr_GC;

//=== API's public methods implementation
void API::init_engine(const RunningOptions &opt) {
  // Save running option sent from the main().
  curr_run_opt = opt;
  // Check current machine state.
  if (curr_state != APIState::Uninitialized)
    RT3_ERROR("API::init_engine() has already been called! ");
  // Set proper machine state
  curr_state = APIState::SetupBlock;
  // Preprare render infrastructure for a new scene.
  render_opt = make_unique<RenderOptions>();
  // Create a new initial GS
  curr_GS = GraphicsState();
  RT3_MESSAGE("[1] Rendering engine initiated.\n");
}

void API::clean_world_elements(void){
  render_opt->primitives.clear();
  render_opt->lights.clear();
}

void API::clean_up(void) {
  // Check for correct machine state
  if (curr_state == APIState::Uninitialized)
    RT3_ERROR("API::clean_up() called before engine initialization.");
  else if (curr_state == APIState::WorldBlock)
    RT3_ERROR("API::clean_up() called inside world definition section.");
  curr_state = APIState::Uninitialized;

  RT3_MESSAGE("[4] Rendering engine clean up concluded. Shutting down...\n");
}

void API::run(void) {
  // Try to load and parse the scene from a file.
  RT3_MESSAGE("[2] Beginning scene file parsing...\n");
  // Recall that the file name comes from the running option struct.
  parse(curr_run_opt.filename.c_str());
}

void API::world_begin(void) {
  VERIFY_SETUP_BLOCK("API::world_begin"); // check for correct machine state.
  curr_state = APIState::WorldBlock;      // correct machine state.
}

void API::world_end(void) {
  VERIFY_WORLD_BLOCK("API::world_end");
  // The scene has been properly set up and the scene has
  // already been parsed. It's time to render the scene.

  unique_ptr<Scene> the_scene;
  unique_ptr<Integrator> the_integrator;

  // LOADING SCENE
  {
    unique_ptr<Background> the_background{make_background(render_opt->bkg_ps)};

    vector<shared_ptr<Light>> the_lights;
    for (auto light_ps : render_opt->lights) {
      the_lights.push_back(shared_ptr<Light>(make_light(light_ps)));
    }

    vector<shared_ptr<BoundedPrimitive>> the_primitive;
    for (auto [object_ps, mat] : render_opt->primitives) {

      unique_ptr<Shape> shape(make_shape(object_ps));

      the_primitive.push_back(shared_ptr<BoundedPrimitive>(
          make_geometric_primitive(std::move(shape), mat)));
    }

    for (auto [mesh_ps, mat] : render_opt->mesh_primitives) {
      for (Shape * s : make_triangles(mesh_ps)){

        the_primitive.push_back(shared_ptr<BoundedPrimitive>(
          make_geometric_primitive(std::move(unique_ptr<Shape>(s)), mat)
        ));

      }
    }


    shared_ptr<Primitive> primitive = make_primitive(render_opt->accelerator_ps, std::move(the_primitive));

    the_scene =
        make_unique<Scene>(std::move(the_background), std::move(primitive), std::move(the_lights));
  }

  // LOADING INTEGRATOR
  {
    unique_ptr<Film> the_film{make_film(render_opt->film_ps)};

    // Same with the camera
    unique_ptr<Camera> the_camera{make_camera(
        render_opt->camera_ps, render_opt->look_at_ps, std::move(the_film))};

    // Integrator
    the_integrator = unique_ptr<Integrator>(
        make_integrator(render_opt->integrator_ps, std::move(the_camera)));
  }

  // Run only if we got film and background.
  if (the_integrator and the_scene) {
    RT3_MESSAGE("\tParsing scene successfuly done!\n");
    RT3_MESSAGE("[2] Starting ray tracing progress.\n");

    // Structure biding, c++17.
    // auto res  = the_integrator->camera->film->get_resolution();
    // size_t w = res[0];
    // size_t h = res[1];
    // RT3_MESSAGE( "    Image dimensions in pixels (W x H): " +
    // std::to_string(w) + " x " + std::to_string(h) + ".\n" ); RT3_MESSAGE( "
    // Ray tracing is usually a slow process, please be patient: \n" );

    //================================================================================
    auto start = std::chrono::steady_clock::now();
    the_integrator->render(the_scene);
    auto end = std::chrono::steady_clock::now();
    //================================================================================
    auto diff = end - start; // Store the time difference between start and end
    // Seconds
    auto diff_sec = std::chrono::duration_cast<std::chrono::seconds>(diff);
    RT3_MESSAGE("    Time elapsed: " + std::to_string(diff_sec.count()) +
                " seconds (" +
                std::to_string(
                    std::chrono::duration<double, std::milli>(diff).count()) +
                " ms) \n");
  }
  // [4] Basic clean up
  curr_state = APIState::SetupBlock; // correct machine state.
  clean_world_elements();
}

/// This api function is called when we need to re-render the *same* scene (i.e.
/// objects, lights, materials, etc) , maybe with different integrator, and
/// camera setup. Hard reset on the engine. User needs to setup all entities,
/// such as camera, integrator, accelerator, etc.
void API::reset_engine(void) {
  // curr_GS = GraphicsState();
  // This will delete all information on integrator, cameras, filters,
  // acceleration structures, etc., that has been set previously.
  render_opt.reset(new RenderOptions);
}

////////////////////////////////////////////////////////////////////////////
// SETUP BLOCK

void API::integrator(const ParamSet &ps) {
  std::cout << ">>> Inside API::integrator()\n";
  VERIFY_SETUP_BLOCK("API::lookat");

  render_opt->integrator_ps = ps;
}

void API::film(const ParamSet &ps) {
  std::cout << ">>> Inside API::film()\n";
  VERIFY_SETUP_BLOCK("API::film");

  render_opt->film_ps = ps;
}

void API::camera(const ParamSet &ps) {
  std::cout << ">>> Inside API::camera()\n";
  VERIFY_SETUP_BLOCK("API::camera");

  render_opt->camera_ps = ps;
}

void API::lookat(const ParamSet &ps) {
  std::cout << ">>> Inside API::lookat()\n";
  VERIFY_SETUP_BLOCK("API::lookat");

  render_opt->look_at_ps = ps;
}

void API::accelerator(const ParamSet &ps) {
  std::cout << ">>> Inside API::accelerator()\n";
  VERIFY_SETUP_BLOCK("API::accelerator");

  render_opt->accelerator_ps = ps;
}

///////////////////////////////////////////////////////////////////////
// WORLD BLOCK

void API::create_named_material(const ParamSet &ps) {
  std::cout << ">>> Inside API::create_named_material()\n";
  VERIFY_WORLD_BLOCK("API::create_named_material");

  string material_name = retrieve(ps, "name", string());

  curr_GC.named_materials[material_name] = shared_ptr<Material>(make_material(ps));
}

void API::pop_gs( void ){

}

void API::push_gs( void ){

}

void API::translate( const ParamSet& ps ){

}

void API::rotate( const ParamSet& ps ){

}

void API::scale( const ParamSet& ps ){

}

void API::background(const ParamSet &ps) {
  std::cout << ">>> Inside API::background()\n";
  VERIFY_WORLD_BLOCK("API::background");

  // Store current background object.
  render_opt->bkg_ps = ps;
}

void API::material(const ParamSet &ps) {
  std::cout << ">>> Inside API::material()\n";
  VERIFY_WORLD_BLOCK("API::material");

  shared_ptr<Material> new_material(make_material(ps));

  curr_GS.setMaterial(new_material);
}

void API::named_material(const ParamSet &ps) {
  std::cout << ">>> Inside API::named_material()\n";
  VERIFY_WORLD_BLOCK("API::named_material");

  string material_name = retrieve(ps, "name", string());
  curr_GS.setMaterial(curr_GC.named_materials[material_name]);
}

void API::object(const ParamSet &ps) {
  std::cout << ">>> Inside API::object()\n";
  VERIFY_WORLD_BLOCK("API::object");

  auto type = retrieve(ps, "type", object_type_t::trianglemesh);

  if(type == object_type_t::trianglemesh){
    if(ps.count("filename")){
      shared_ptr<TriangleMesh> md{ new TriangleMesh()};

      auto status = load_mesh_data(
        retrieve(ps, "filename", string()), 
        retrieve(ps, "reverse_vertex_order", false), 
        retrieve(ps, "compute_normals", false),
        retrieve(ps, "flip_normals", false), 
        md
      );

      if(status){
        md->backface_cull = retrieve(ps, "backface_cull", false);
        render_opt->mesh_primitives.push_back({md, curr_GS.getCurrMaterial()});
      }else{
        RT3_ERROR("Couldn't load obj file");
      }
    }else{
      render_opt->mesh_primitives.push_back({
        shared_ptr<TriangleMesh>(create_triangle_mesh(ps)), 
        curr_GS.getCurrMaterial()
      });
    }
  }else{
    render_opt->primitives.push_back({ps, curr_GS.getCurrMaterial()});
  }  
}

void API::light(const ParamSet &ps) {
  std::cout << ">>> Inside API::light()\n";
  VERIFY_WORLD_BLOCK("API::light");

  render_opt->lights.push_back(ps);
}

} // namespace rt3
