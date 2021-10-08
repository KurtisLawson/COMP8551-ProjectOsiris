#pragma once
#include "entityx/Event.h"
#include <iostream>

using namespace std;
using entityx::EventManager;
using entityx::Event;
using entityx::Receiver;

struct ExampleEvent {
	explicit ExampleEvent(int a, int b) :a(a) , b(b) {}
	int a, b;
};

class ExampleEmitterSystem : public System<ExampleEmitterSystem> {
		bool emitted = false;

public:
		void update(EntityManager& es, EventManager& events, TimeDelta dt) override {
				emitSample(events);
		};

	void emitSample(EventManager& em) {
		if(!emitted)
				em.emit<ExampleEvent>(1, 2);
		emitted = true;
	}
};

struct ExampleListenerSystem : public System<ExampleListenerSystem>, public Receiver<ExampleListenerSystem> {
	int total = 0;
	
	void configure(EventManager &em) override{
		em.subscribe<ExampleEvent>(*this);
	}

	void update(EntityManager& es, EventManager& events, TimeDelta dt) override {};

	void receive(const ExampleEvent& event) {
		total += event.a * event.b;
		cerr << "Event system" + total << endl;
	}
};