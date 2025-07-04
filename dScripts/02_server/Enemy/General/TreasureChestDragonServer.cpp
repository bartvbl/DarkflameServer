#include "TreasureChestDragonServer.h"
#include "ScriptedActivityComponent.h"
#include "TeamManager.h"
#include "EntityManager.h"
#include "Loot.h"

void TreasureChestDragonServer::OnStartup(Entity* self) {

}

void TreasureChestDragonServer::OnUse(Entity* self, Entity* user) {
	if (self->GetVar<bool>(u"bUsed")) {
		return;
	}

	self->SetVar<bool>(u"bUsed", true);

	auto* scriptedActivityComponent = self->GetComponent<ScriptedActivityComponent>();

	if (scriptedActivityComponent == nullptr) {
		return;
	}

	auto rating = 1;

	auto* team = TeamManager::Instance()->GetTeam(user->GetObjectID());

	if (team != nullptr) {
		rating = team->members.size();

		for (const auto member : team->members) {
			auto* memberObject = Game::entityManager->GetEntity(member);

			if (memberObject == nullptr) continue;

			Loot::DropActivityLoot(memberObject, self->GetObjectID(), scriptedActivityComponent->GetActivityID(), rating);
		}
	} else {
		Loot::DropActivityLoot(user, self->GetObjectID(), scriptedActivityComponent->GetActivityID(), rating);
	}

	self->Smash(self->GetObjectID());
}
