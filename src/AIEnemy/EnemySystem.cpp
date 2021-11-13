#include "EnemySystem.h"

void EnemySystem::update(EntityManager& es, EventManager& events, TimeDelta dt)
{
	ComponentHandle<Rigidbody> rigidbody;
	for (Entity entity : es.entities_with_components(rigidbody))
	{
		ComponentHandle<Rigidbody> rigidbody = entity.component<Rigidbody>();
		ComponentHandle<Transform> transform = entity.component<Transform>();
		ComponentHandle<GameObject> object = entity.component<GameObject>();
		

		//Only do this for rigidbodies that are the player
		//Probably poor practice, but works for testing
		if (object && object->name == "enemy")
		{
			
			//First get current position
			glm::vec2 position = rigidbody->GetPosition();
			//For each direction, check if the movement flag is set and if you can move in that direction
			//First reset move flags before moving so future collisions update them correctly.
			//Then move the Rigidbody which will update the Transform as well
			//Velocity probably needs to be better scaled in-engine
			//Spaces between centers of tiles is 2.
			if (left && canMoveLeft)
			{
				ResetMoveFlags();
				rigidbody->MoveToPosition(glm::vec2(position.x - 2, position.y), 0.5);
			}
			if (right && canMoveRight)
			{
				ResetMoveFlags();
				rigidbody->MoveToPosition(glm::vec2(position.x + 2, position.y), 0.5);
			}
			if (up && canMoveUp)
			{
				ResetMoveFlags();
				rigidbody->MoveToPosition(glm::vec2(position.x, position.y - 2), 0.5);
			}
			if (down && canMoveDown)
			{
				ResetMoveFlags();
				rigidbody->MoveToPosition(glm::vec2(position.x, position.y + 2), 0.5);
			}

			

			if (attackLeft && leftEntity)
			{
				ComponentHandle<GameObject> targetObj = leftEntity->component<GameObject>();
				cout << "attacking " << targetObj->name << " to the left" << endl;

				ComponentHandle<Rigidbody> targetRb = leftEntity->component<Rigidbody>();
				targetRb->DeleteBody();
				leftEntity->destroy();
				canMoveLeft = true;
				this->leftEntity = nullptr;
			}
			if (attackRight && rightEntity)
			{
				ComponentHandle<GameObject> targetObj = rightEntity->component<GameObject>();
				cout << "attacking " << targetObj->name << " to the right" << endl;

				ComponentHandle<Rigidbody> targetRb = rightEntity->component<Rigidbody>();
				targetRb->DeleteBody();
				rightEntity->destroy();
				canMoveRight = true;
				this->rightEntity = nullptr;
			}
			if (attackUp && upEntity)
			{
				ComponentHandle<GameObject> targetObj = upEntity->component<GameObject>();
				cout << "attacking " << targetObj->name << " to the top" << endl;

				ComponentHandle<Rigidbody> targetRb = upEntity->component<Rigidbody>();
				targetRb->DeleteBody();
				upEntity->destroy();
				canMoveUp = true;
				this->upEntity = nullptr;
			}
			if (attackDown && downEntity)
			{
				ComponentHandle<GameObject> targetObj = downEntity->component<GameObject>();
				cout << "attacking " << targetObj->name << " to the bottom" << endl;

				ComponentHandle<Rigidbody> targetRb = downEntity->component<Rigidbody>();
				targetRb->DeleteBody();
				downEntity->destroy();
				canMoveDown = true;
				this->downEntity = nullptr;
			}
			
		}
	}

	// Reset input flags, needed here in case the movement didn't fire (moves would stack otherwise)
	right = false;
	up = false;
	down = false;
	left = false;

	attackRight = false;
	attackUp = false;
	attackDown = false;
	attackLeft = false;
}

//Convenience function for reset collision flags
void EnemySystem::ResetMoveFlags()
{
	canMoveLeft = true;
	canMoveRight = true;
	canMoveUp = true;
	canMoveDown = true;
	ResetCollisionEntities(); //Will have to reset detected entities on move as well
}

//Convenience function for resetting detected entities
void EnemySystem::ResetCollisionEntities()
{
	this->leftEntity = nullptr;
	this->rightEntity = nullptr;
	this->upEntity = nullptr;
	this->downEntity = nullptr;
}

void EnemySystem::configure(EventManager& em) {
	
	em.subscribe<Collision>(*this);
	em.subscribe<EnemyDebugInput>(*this);
	//em.subscribe<AttackInput>(*this);
}

//Receive function just sets boolean flags to be picked up by update loop
void EnemySystem::receive(const EnemyDebugInput& event) {
	// This event will be replaced with the "OnPlayerAction" Event.
	// It will be responsible for

	//	1. Get the next command from the AI component.
	//	2. Determine if the command should be a move or attack flag
	cout << "Enemy Input Fired" << endl;
	EnemyDebugInput::InpDir dir = event.dir; // This will be the "Command"

	switch (dir) {
		case EnemyDebugInput::UP:
			up = true;
			break;
		case EnemyDebugInput::LEFT:
			left = true;
			break;
		case EnemyDebugInput::DOWN:
			down = true;
			break;
		case EnemyDebugInput::RIGHT:
			right = true;
			break;
	}
}

void EnemySystem::receive(const Collision& event)
{
	cout << "collision event received for " << event.fA << " and " << event.fB << endl;

	//Direct body collision
	//if (event.fA == "body" && event.fB == "body")
	//{
	//	//So far we only have treasure entities so we can delete that
	//	ComponentHandle<GameObject> objectA = event.a->component<GameObject>();
	//	ComponentHandle<GameObject> objectB = event.b->component<GameObject>();
	//	if (objectA->name == "treasure") event.a->destroy();
	//	else if (objectB->name == "treasure") event.b->destroy();
	//}
	
	//One collider was a named sensor

	if (event.fA == "left" || event.fB == "left")
	{
		this->leftEntity = event.fA == "left" ? event.b : event.a;
		ComponentHandle<GameObject> objectA = event.a->component<GameObject>();
		ComponentHandle<GameObject> objectB = event.b->component<GameObject>();
		if (objectA->name == "player") this->canMoveLeft = false;
		else if (objectB->name == "player") this->canMoveLeft = false;
	}
	else if (event.fA == "right" || event.fB == "right")
	{
		this->rightEntity = event.fA == "right" ? event.b : event.a;
		ComponentHandle<GameObject> objectA = event.a->component<GameObject>();
		ComponentHandle<GameObject> objectB = event.b->component<GameObject>();
		if (objectA->name == "player") this->canMoveRight = false;
		else if (objectB->name == "player") this->canMoveRight = false;
	}
	else if (event.fA == "top" || event.fB == "top")
	{
		this->upEntity = event.fA == "top" ? event.b : event.a;
		ComponentHandle<GameObject> objectA = event.a->component<GameObject>();
		ComponentHandle<GameObject> objectB = event.b->component<GameObject>();
		if (objectA->name == "player") this->canMoveUp = false;
		else if (objectB->name == "player") this->canMoveUp = false;
	}
	else if (event.fA == "bottom" || event.fB == "bottom")
	{
		this->downEntity = event.fA == "bottom" ? event.b : event.a;
		ComponentHandle<GameObject> objectA = event.a->component<GameObject>();
		ComponentHandle<GameObject> objectB = event.b->component<GameObject>();
		if (objectA->name == "player") this->canMoveDown = false;
		else if (objectB->name == "player") this->canMoveDown = false;
	}
}


