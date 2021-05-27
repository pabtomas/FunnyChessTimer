#include "Avatar.h"

void Avatar::attributeSounds()
{
  std::vector<size_t> playable_sounds = {};
  for (size_t i = m_FirstIndex; i < m_LastIndex; i++)
  {
    playable_sounds.push_back(i);
  }

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle(playable_sounds.begin(), playable_sounds.end(),
    std::default_random_engine(seed));

  m_FirstSound = playable_sounds[0];
  m_SecondSound = playable_sounds[1];
}

void Avatar::playSound(std::vector<sf::Sound>& sounds, bool play_first_sound)
{
  if (play_first_sound)
  {
    sounds[m_FirstSound].play();
  } else {
    sounds[m_SecondSound].play();
  }
}

icu::UnicodeString Avatar::toUpperString()
{
  std::string s = this->toString();
  icu::UnicodeString u(s.c_str());
  return u.toUpper();
}

std::string KungFuMasterAvatar::toString()
{
  return std::string("Le Maitre du Kung-Fu");
}

std::string KingOfTheJungleAvatar::toString()
{
  return std::string("Le Roi de la Jungle");
}

std::string UnquestioningLoveAvatar::toString()
{
  return std::string("L'Amour Inconditionnel");
}
