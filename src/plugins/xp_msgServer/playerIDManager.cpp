// PlayerIDManager.cpp
#include "PlayerIDManager.h"

PlayerIDManager::PlayerIDManager() : nextUniqueID(1) {
    // start with 1 (0 is reserved for "invalid")
}

uint32_t PlayerIDManager::onNewPlayer(unsigned long playerID) {
    //Be sure to remove a possible old one
    this->removeByPlayerID(playerID);

    // Create the new one
    uint32_t newUniqueID = nextUniqueID++;
    uniqueToPlayer[newUniqueID] = playerID;
    playerToUnique[playerID] = newUniqueID;

    return newUniqueID;
}

std::optional<unsigned long> PlayerIDManager::getPlayerID(uint32_t uniquePlayerID) const {
    auto it = uniqueToPlayer.find(uniquePlayerID);
    if (it != uniqueToPlayer.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::optional<uint32_t> PlayerIDManager::getUniquePlayerID(unsigned long playerID) const {
    auto it = playerToUnique.find(playerID);
    if (it != playerToUnique.end()) {
        return it->second;
    }
    return std::nullopt;
}

bool PlayerIDManager::removeByPlayerID(unsigned long playerID) {
    auto it = playerToUnique.find(playerID);
    if (it != playerToUnique.end()) {
        uint32_t uniqueID = it->second;
        uniqueToPlayer.erase(uniqueID);
        playerToUnique.erase(playerID);
        return true;
    }
    return false;
}

bool PlayerIDManager::removeByUniquePlayerID(uint32_t uniquePlayerID) {
    auto it = uniqueToPlayer.find(uniquePlayerID);
    if (it != uniqueToPlayer.end()) {
        unsigned long playerID = it->second;
        playerToUnique.erase(playerID);
        uniqueToPlayer.erase(uniquePlayerID);
        return true;
    }
    return false;
}
