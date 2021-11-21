#include "ScoreTest.h"

void ScoreTest::update(EntityManager& es, EventManager& events, TimeDelta dt)
{
	if (collectedTreasure)
	{
		events.emit<ScoreUpdate>(10);
		collectedTreasure = false;
	}
	if (killedEnemy)
	{
		events.emit<ScoreUpdate>(2);
		killedEnemy = false;
	}
}

void ScoreTest::configure(EventManager& em) {
	em.subscribe<EntityDestroyedEvent>(*this);
	em.subscribe<ScoreUpdate>(*this);
}


void ScoreTest::receive(const EntityDestroyedEvent& events) {
	Entity e = events.entity;
	ComponentHandle<GameObject> object = e.component<GameObject>();
	if (object->name == "treasure") collectedTreasure = true;
	if (object->name == "enemy") killedEnemy = true;
}

void ScoreTest::receive(const ScoreUpdate& event) {
	totalScore += event.a;
	cout << "Total score: " << totalScore << endl;
}