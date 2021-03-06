#include "InputSystem.h"

vector<int> InputSystem::movementKeys = { GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D };
vector<int> InputSystem::attackKeys = { GLFW_KEY_UP, GLFW_KEY_LEFT, GLFW_KEY_DOWN, GLFW_KEY_RIGHT };
vector<int> InputSystem::interactKeys = { GLFW_KEY_E };
vector<int> InputSystem::controlKeys = { GLFW_KEY_X, GLFW_KEY_Y, GLFW_KEY_SPACE };

void InputSystem::update(EntityManager& es, EventManager& ev, TimeDelta dt)
{
  // Create component handles to filter components
  ComponentHandle<Window> hwindow;

  // Loop through components
  auto en = es.entities_with_components(hwindow);
  for (auto entity = en.begin(); entity != en.end(); ++entity)
  {
    Window* window = (*entity).component<Window>().get();
    glfwSetWindowUserPointer(window->window, (void*)&ev); // Pass a pointer to EventManager to enable emition of events
    glfwSetKeyCallback(window->window, keyCallback); // set the function to be run when a keyboard event occurs

    // If Escape key is pressed, send the "Close Window" signal to GLFW and break the Render Loop.
    if (glfwGetKey(window->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // GLFW_RELEASE is the opposite of GLFW_PRESS, like KeyUp and KeyDown
      glfwSetWindowShouldClose(window->window, true);

    // checks if any events are triggered, updates the window state, and calls the corresponding functions (which we can register via callback methods)
    glfwPollEvents();
  }
}

void InputSystem::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) // function is called whenever a keyboard input is detected
{
  EventManager* eve = (EventManager*)glfwGetWindowUserPointer(window); // Reference to Event Manager to emit input events.

  if (find(movementKeys.begin(), movementKeys.end(), key) != movementKeys.end()) { // Handle movement input group
    switch (action) { // Will emit movement event containing direction based on key input
    case GLFW_PRESS: // decide what to do when the key is pressed
      if (key == movementKeys[UP]) {
        eve->emit<MoveInput>(MoveInput::UP);
      }
      else if (key == movementKeys[LEFT]) {
        eve->emit<MoveInput>(MoveInput::LEFT);
      }
      else if (key == movementKeys[DOWN]) {
        eve->emit<MoveInput>(MoveInput::DOWN);
      }
      else if (key == movementKeys[RIGHT]) {
        eve->emit<MoveInput>(MoveInput::RIGHT);
      }
      break;
    case GLFW_RELEASE: // decide what to do when the key is released
      break;
    }
  }

  if (find(attackKeys.begin(), attackKeys.end(), key) != attackKeys.end()) { // Handle attack input group
    switch (action) { // Will emit attack event containing direction based on key input
    case GLFW_PRESS: // decide what to do when the key is pressed
      if (key == attackKeys[UP]) {
        eve->emit<AttackInput>(AttackInput::UP);
      }
      else if (key == attackKeys[LEFT]) {
        eve->emit<AttackInput>(AttackInput::LEFT);
      }
      else if (key == attackKeys[DOWN]) {
        eve->emit<AttackInput>(AttackInput::DOWN);
      }
      else if (key == attackKeys[RIGHT]) {
        eve->emit<AttackInput>(AttackInput::RIGHT);
      }
      break;
    case GLFW_RELEASE: // decide what to do when the key is released
      break;
    }
  }

  if (find(interactKeys.begin(), interactKeys.end(), key) != interactKeys.end()) { // Handle interact input group
    switch (action) { // Will emit interact event based on key input
    case GLFW_PRESS: // decide what to do when the key is pressed
      if (key == interactKeys[INTERACT])
        eve->emit<InteractInput>();
      break;
    case GLFW_RELEASE: // decide what to do when the key is released
      break;
    }
  }

  if (find(controlKeys.begin(), controlKeys.end(), key) != controlKeys.end()) { // Handle control input group
      switch (action) { // Will emit control event containing based on key input
      case GLFW_PRESS: // decide what to do when the key is pressed
          if (key == controlKeys[X]) //X 
              eve->emit<ControlInput>(ControlInput::X);
          else if (key == controlKeys[Y]) //Y
              eve->emit<ControlInput>(ControlInput::Y);
          else if (key == controlKeys[SPACE]) // space
              eve->emit<ControlInput>(ControlInput::SPACE);
          break;
      case GLFW_RELEASE: // decide what to do when the key is released
          break;
      }
  }
}
