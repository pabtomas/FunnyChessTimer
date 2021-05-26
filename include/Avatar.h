#ifndef AVATAR_H
#define AVATAR_H

/* shuffle() needs */
#include <algorithm>

/* init shuffle() seed */
#include <chrono>

/* default_random_engine need */
#include <random>

/* play sounds */
#include <SFML/Audio.hpp>

/* toString() method */
#include <string>

/* sounds vector */
#include <vector>

#define GONGSOUND_INDEX 0
#define KUNGFUSOUNDS_NB 4
#define FIRST_KUNGFUSOUND_INDEX 1
#define LAST_KUNGFUSOUND_INDEX (FIRST_KUNGFUSOUND_INDEX + KUNGFUSOUNDS_NB)
#define MONKEYSOUNDS_NB 3
#define FIRST_MONKEYSOUND_INDEX 5
#define LAST_MONKEYSOUND_INDEX (FIRST_MONKEYSOUND_INDEX + MONKEYSOUNDS_NB)
#define NB_SOUNDS (1 + KUNGFUSOUNDS_NB + MONKEYSOUNDS_NB)

#define DEFAULT_AVATAR 0
#define AVATAR_NB 2

class Avatar
{
  public:

    Avatar(size_t first, size_t last): m_FirstIndex(first),
      m_LastIndex(last) {};

    void playSound(std::vector<sf::Sound>& sounds, bool play_first_sound);
    void attributeSounds();
    virtual std::string toString() = 0;

  protected:

    size_t m_FirstSound;
    size_t m_SecondSound;
    size_t m_FirstIndex;
    size_t m_LastIndex;
};

class KungFuMasterAvatar: public Avatar
{
  public:

    KungFuMasterAvatar(): Avatar(FIRST_KUNGFUSOUND_INDEX,
      LAST_KUNGFUSOUND_INDEX) {}

    std::string toString();
};

class KingOfTheJungleAvatar: public Avatar
{
  public:

    KingOfTheJungleAvatar(): Avatar(FIRST_MONKEYSOUND_INDEX,
      LAST_MONKEYSOUND_INDEX) {}

    std::string toString();
};

#endif
