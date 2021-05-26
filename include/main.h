/* trim() functions needs */
#include <algorithm>
#include <cctype>
#include <locale>

/* Timer class needs */
#include <chrono>

/* srand() & rand() */
#include <ctime>
#include <random>

/* display() function needs */
#include <SFML/Graphics.hpp>

/* sounds vector needs */
#include <SFML/Audio.hpp>

#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "Avatar.h"

#define DEFAULT_TIME 5
#define MIN_TIME 1
#define MAX_TIME 60
#define MAX_LENGTH_PLAYER_NAME 32

#define WIN_WIDTH 1000
#define WIN_HEIGHT 715

#define WHITE_TURN true
#define SECS_PER_MIN 60
#define MILLIS_PER_SEC 1000
#define MILLIS_PER_MIN (MILLIS_PER_SEC * SECS_PER_MIN)

#define DEFAULT_WHITE_PLAYER "Joueur 1"
#define DEFAULT_BLACK_PLAYER "Joueur 2"

class Timer
{
  private:

    std::chrono::steady_clock::time_point m_Start;

  public:

    Timer() {}

    void tick()
    {
      m_Start = std::chrono::steady_clock::now();
    }

    long int duration() const
    {
      return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - m_Start).count();
    }
};

struct CHESSGAME
{
  int time;
  bool turn;
  bool started;
  bool pressed;
  bool gong_end;
  Timer timer;
  long int white_time;
  long int black_time;
  std::string white_player;
  std::string black_player;
  size_t white_player_avatar;
  size_t black_player_avatar;
  bool white_player_id;
  bool black_player_id;
};

void leftTrim(std::string &s)
{
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
    return !std::isspace(ch);
  }));
}

void rightTrim(std::string &s)
{
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
    return !std::isspace(ch);
  }).base(), s.end());
}

void trim(std::string &s)
{
  leftTrim(s);
  rightTrim(s);
}
