#pragma once
#include "../components.hpp"
#include <entityx/Event.h>

using entityx::EventManager;
using entityx::Event;
using entityx::Receiver;

//Create the custom events as such
struct ExampleEvent {
	explicit ExampleEvent(int a, int b) :a(a), b(b) {}
	int a, b;
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
		cout << "Event system :" << total << endl;
	}
};