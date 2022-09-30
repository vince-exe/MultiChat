#include "client_stats_manager.h"

ClientStatsManager::ClientStatsManager() {}

ClientStatsManager::ClientStatsManager(int nMutes, int nMsgs, bool isMuted) {
    this->nMutes = nMutes;
    this->nMsgs = nMsgs;
    this->isMuted_ = isMuted;
};

void ClientStatsManager::setNMutes(int n) {
    this->nMutes = n;
}

void ClientStatsManager::setNMsgs(int n) {
    this->nMsgs = n;
}

void ClientStatsManager::setMuted(bool x) {
    this->isMuted_ = x;
}

int ClientStatsManager::getMutes() {
    return this->nMutes;
}

int ClientStatsManager::getMsgs() {
    return this->nMsgs;
}

bool ClientStatsManager::isMuted() {
    return this->isMuted_;
};
