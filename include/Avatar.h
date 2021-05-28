#ifndef AVATAR_H
#define AVATAR_H

/* shuffle() needs */
#include <algorithm>

/* and toUpperString() needs */
#include <unicode/unistr.h>
#include <unicode/ustream.h>
#include <unicode/locid.h>

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
#define NB_KUNGFUSOUNDS 4
#define FIRST_KUNGFUSOUND_INDEX 1
#define LAST_KUNGFUSOUND_INDEX (FIRST_KUNGFUSOUND_INDEX + NB_KUNGFUSOUNDS)
#define NB_MONKEYSOUNDS 3
#define FIRST_MONKEYSOUND_INDEX 5
#define LAST_MONKEYSOUND_INDEX (FIRST_MONKEYSOUND_INDEX + NB_MONKEYSOUNDS)
#define NB_DOUGSOUNDS 5
#define FIRST_DOUGSOUND_INDEX 8
#define LAST_DOUGSOUND_INDEX (FIRST_DOUGSOUND_INDEX + NB_DOUGSOUNDS)
#define NB_MEXICANSOUNDS 4
#define FIRST_MEXICANSOUND_INDEX 13
#define LAST_MEXICANSOUND_INDEX (FIRST_MEXICANSOUND_INDEX + NB_MEXICANSOUNDS)
#define NB_SOUNDS (1 + NB_KUNGFUSOUNDS + NB_MONKEYSOUNDS + NB_DOUGSOUNDS + \
  NB_MEXICANSOUNDS)

#define DEFAULT_AVATAR 0
#define NB_AVATAR 4

class Avatar
{
  public:

    Avatar(size_t first, size_t last);

    void playSound(std::vector<sf::Sound>& sounds, bool play_first_sound);
    void attributeSounds();
    virtual std::string toString() = 0;
    icu::UnicodeString toUpperString();

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

class UnquestioningLoveAvatar: public Avatar
{
  public:

    UnquestioningLoveAvatar(): Avatar(FIRST_DOUGSOUND_INDEX,
      LAST_DOUGSOUND_INDEX) {}

    std::string toString();
};

class TheBullRiderAvatar: public Avatar
{
  public:

    TheBullRiderAvatar(): Avatar(FIRST_MEXICANSOUND_INDEX,
      LAST_MEXICANSOUND_INDEX) {}

    std::string toString();
};

#endif
