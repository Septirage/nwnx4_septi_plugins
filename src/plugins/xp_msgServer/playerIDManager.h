#if !defined(SEPT_MSGSERV_PLAYERIDMANAGER)
#define SEPT_MSGSERV_PLAYERIDMANAGER

#include <unordered_map>
#include <cstdint>
#include <optional>

class PlayerIDManager {
private:
    std::unordered_map<uint32_t, unsigned long> uniqueToPlayer;  // UniquePlayerID -> playerID
    std::unordered_map<unsigned long, uint32_t> playerToUnique;  // playerID -> UniquePlayerID
    uint32_t nextUniqueID;

public:
    PlayerIDManager();

    // Call for a "new player connection". Will return the UniqueID
    uint32_t onNewPlayer(unsigned long playerID);

    // Get the playerID from a UniquePlayerID
    std::optional<unsigned long> getPlayerID(uint32_t uniquePlayerID) const;

    // Get the UniquePlayerID from a  playerID
    std::optional<uint32_t> getUniquePlayerID(unsigned long playerID) const;

    //
    bool removeByPlayerID(unsigned long playerID);
    bool removeByUniquePlayerID(uint32_t uniquePlayerID);
};

#endif