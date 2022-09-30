#ifndef CLIENTSTATSMANAGER_H
#define CLIENTSTATSMANAGER_H


class ClientStatsManager {
private:
    int nMutes;

    int nMsgs;

    bool isMuted_;

public:
    ClientStatsManager();

    ClientStatsManager(int nMutes, int nMsgs, bool isMuted);

    void setNMutes(int n);

    void setNMsgs(int n);

    void setMuted(bool x);

    int getMutes();

    int getMsgs();

    bool isMuted();
};

#endif // CLIENTSTATSMANAGER_H
