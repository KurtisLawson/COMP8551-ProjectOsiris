#pragma once
#include "../components.hpp"
#include <entityx/Event.h>

using entityx::EventManager;
using entityx::Event;
using entityx::Receiver;

// Custom event for Movement Inputs
// InpDir - Enum used to indicate which direction input was detected
struct MoveInput {
	enum InpDir {
		UP,
		LEFT,
		DOWN,
		RIGHT
	};
	explicit MoveInput(InpDir dir) : dir(dir){}
	InpDir dir;
};

// Custom event for Attack Inputs
// InpDir - Enum used to indicate which direction input was detected
struct AttackInput {
	enum InpDir {
		UP,
		LEFT,
		DOWN,
		RIGHT
	};
	explicit AttackInput(InpDir dir) : dir(dir) {}
	InpDir dir;
};

// Custom event for Interact Inputs
struct InteractInput {
	explicit InteractInput(){}
};

// Custom event for Enemy movement debugging
struct EnemyDebugInput {
	enum InpDir {
		UP,
		LEFT,
		DOWN,
		RIGHT
	};
	explicit EnemyDebugInput(InpDir dir) : dir(dir){}
	InpDir dir;
};

// Custom event for Game State Control
struct ControlInput {
	enum Cmd {
		X,
		Y,
		SPACE,
	};
	explicit ControlInput(Cmd cmd) : cmd(cmd) {}
	Cmd cmd;
};



//Collision struct to be created for collision events
struct Collision
{
	Entity* a; //Left colliding element
	Entity* b; //Right colliding element

	string fA = ""; //Fixture name for left element
	string fB = ""; //Fixture name for right element

	//Regular collider, no fixture data
	Collision(Entity* _a, Entity* _b) : a(_a), b(_b) {}

	//Collider including fixture names
	Collision(Entity* _a, Entity* _b, string _fA, string _fB) : a(_a), b(_b), fA(_fA), fB(_fB) {}
};

//Collision struct to be created for collision events
struct EndCollision
{
	Entity* a; //Left colliding element
	Entity* b; //Right colliding element

	string fA = ""; //Fixture name for left element
	string fB = ""; //Fixture name for right element

	//Regular collider, no fixture data
	EndCollision(Entity* _a, Entity* _b) : a(_a), b(_b) {}

	//Collider including fixture names
	EndCollision(Entity* _a, Entity* _b, string _fA, string _fB) : a(_a), b(_b), fA(_fA), fB(_fB) {}
};

struct PlayerTurnEnd {}; // Empty event to signal player end turn.
struct PlayerAttack {
	enum Dir {
		CENTER,
		LEFT,
		UP,
		DOWN,
		RIGHT,
	};
	Dir dir = CENTER;
	explicit PlayerAttack(Dir dir) : dir(dir) {};
}; //Event to signal player attack connecting with enemy and direction.
struct EnemyTurnEnd {}; //Empty event to signal enemy end turn.
struct EnemyAttack {
	enum Dir {
		CENTER,
		LEFT,
		UP,
		DOWN,
		RIGHT,
	};
	Dir dir = CENTER;
	explicit EnemyAttack(Dir dir) : dir(dir) {};
}; //Event to signal enemy attack with postion.
struct GameOver {}; //Empty game over event to listen for.
struct GameWon {}; //Empty game won event to listen for.

//Event sending the ration of remaining time to update UI elements.
struct TimerUpdate
{
	float ratio;
	TimerUpdate(float _ratio) :ratio(_ratio) {};
};

//Custom event for keeping track of key capture
struct KeyCaptureUpdate {
	explicit KeyCaptureUpdate(bool captured) : captured(captured) {}
	bool captured;
};

//Custom event for keeping track of the player health
struct PlayerHealthUpdate {
	explicit PlayerHealthUpdate(int health) : health(health) {}
	int health;
};

//Create the custom events as such
struct ExampleEvent {
	explicit ExampleEvent(int a, int b) : a(a), b(b) {}
	int a, b;
};

//Animation event to update model index
struct ModelSwapEvent {
	explicit ModelSwapEvent(int index) :index(index) {}
	int index;

};

//The emitter will be called using .emit<custom events>
class ExampleEmitterSystem : public System<ExampleEmitterSystem> {
	bool emitted = false;

public:
	void update(EntityManager& es, EventManager& events, TimeDelta dt) override {
		emitSample(events);
	};

	void emitSample(EventManager& em) {
		if (!emitted)
			em.emit<ExampleEvent>(4, 2);
		emitted = true;
	}
};

//The listener needs to subscribe .subscribe<custom event>() before enabling recieve the emitted events from .emit<custom event>()
struct ExampleListenerSystem : public System<ExampleListenerSystem>, public Receiver<ExampleListenerSystem> {
	int total = 0;

	void configure(EventManager& em) override {
		em.subscribe<ExampleEvent>(*this);
	}

	void update(EntityManager& es, EventManager& events, TimeDelta dt) override {};

	void receive(const ExampleEvent& event) {
		total += event.a * event.b;
	}
};