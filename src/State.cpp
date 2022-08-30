#include "State.h"
#include "GameObject.h"

State::State() : popRequested(false), quitRequested(false), started(false) {}
State::~State()
{
	objectArray.clear();
}

bool State::PopRequested() { return popRequested; }

bool State::QuitRequested() { return quitRequested; }

void State::StartArray()
{
	for (unsigned i = 0; i < objectArray.size(); i++)
	{
		objectArray[i]->Start();
	}
}

void State::UpdateArray(float dt)
{
	for (unsigned i = 0; i < objectArray.size(); i++)
	{
		objectArray[i]->Update(dt);
	}
}

void State::RenderArray()
{
	for (unsigned i = 0; i < objectArray.size(); i++)
	{
		objectArray[i]->Render();
	}
}

std::weak_ptr<GameObject> State::AddObject(GameObject *go)
{
	std::shared_ptr<GameObject> sharedGo(go);
	objectArray.push_back(sharedGo);

	if (started)
	{
		go->Start();
	}

	return std::weak_ptr<GameObject>(sharedGo);
}

std::weak_ptr<GameObject> State::GetObjectPtr(GameObject *go)
{
	for (unsigned i = 0; i < objectArray.size(); i++)
	{
		if (objectArray[i].get() == go)
		{
			return std::weak_ptr<GameObject>(objectArray[i]);
		}
	}

	return {};
}