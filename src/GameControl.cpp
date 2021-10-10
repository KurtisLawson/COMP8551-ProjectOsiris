#include "GameControl.h"

// TEMP - These will likely be adapted into an asset system?
const char* modelSource = "./src/Renderer3D/Models/wayfarer.obj";
const char* vertSource = "./src/Renderer3D/Shaders/Default.vert";
const char* fragSource = "./src/Renderer3D/Shaders/Default.frag";
const char* texSource = "./src/Renderer3D/Textures/container.jpg";

GameControl::GameControl(GLFWwindow* window, string filename)
{
  // Create systems
  systems.add<InputSystem>();
  systems.add<RenderSystem>();
  systems.add<PhysicsEngine>();
  systems.configure();

  //Testing Scene Manager
  glfwSetWindowTitle(window, filename.c_str());

  // TODO make it easier to create entities
  // Create entities
  entityx::Entity entity = entities.create();
  entity.assign<Window>(window);
  entity.assign<Color>(0.2f, 0.3f, 0.3f, 1.0f);

  // TEST - Create a 3D model entity for the RenderSystem to use
  entityx::Entity playerEntity = entities.create();
  playerEntity.assign<Model3D>(modelSource, vertSource, fragSource, texSource);

  //TEST - Load up configuration file, run the test function, grab position data of specified object
  TinyXMLWrapper txml("./src/Configuration/gameobjects.xml");
  txml.test();
  cout << txml.getAttribute("Cube 2", "position") << endl;
}

void GameControl::Update(TimeDelta dt)
{
  systems.update<InputSystem>(dt);
  systems.update<RenderSystem>(dt);
  systems.update<PhysicsEngine>(dt);
}
