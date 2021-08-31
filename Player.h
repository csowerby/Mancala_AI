//
//  Player.hpp
//  Project3_draft
//
//  Created by Charlie Sowerby on 5/15/19.
//  Copyright © 2019 Charlie Sowerby. All rights reserved.
//

#ifndef Player_h
#define Player_h

#include "Board.h"
#include <string>
#include <iostream>
using namespace std;

class AlarmClock;

class Player{
public:
    Player(string name);
    string name() const;
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const = 0;
    virtual ~Player();
private:
    string m_name; 
};


class HumanPlayer: public Player{
public:
    HumanPlayer(string name);
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const;
};

class BadPlayer: public Player{
public:
    BadPlayer(string name);
    virtual int chooseMove(const Board& b, Side s) const;
    
};

class SmartPlayer: public Player{
public:
    SmartPlayer(string name);
    virtual int chooseMove(const Board& b, Side s) const;
private:
    void smartChoice(Side s, Board& b, int &bestHole, int &bestValue, int depth, AlarmClock &ac) const;
    int evaluate(Board &b) const; 
};





#include <chrono>
#include <future>
#include <atomic>

class AlarmClock
{
public:
    AlarmClock(int ms)
    {
        m_timedOut = false;
        m_isRunning = true;
        m_alarmClockFuture = std::async([=]() {
            for (int k = 0; k < ms  &&  m_isRunning; k++)
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            if (m_isRunning)
                m_timedOut = true;
        });
    }
    
    ~AlarmClock()
    {
        m_isRunning = false;
        m_alarmClockFuture.get();
    }
    
    bool timedOut() const
    {
        return m_timedOut;
    }
    
    AlarmClock(const AlarmClock&) = delete;
    AlarmClock& operator=(const AlarmClock&) = delete;
private:
    std::atomic<bool> m_isRunning;
    std::atomic<bool> m_timedOut;
    std::future<void> m_alarmClockFuture;
};

#endif /* Player_h */


