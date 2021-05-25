#include <algorithm>
#include <cctype>
#include <chrono>
#include <ctime>
#include <iostream>
#include <iterator>
#include <locale>
#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>

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

#define GONG_SOUND 0
#define FIRST_KUNGFU_SOUND 1
#define NB_SOUNDS 5

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
  size_t white_player_sound;
  size_t black_player_sound;
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

void update(struct CHESSGAME& game)
{
  if (game.started)
  {
    if (game.turn == WHITE_TURN)
    {
      game.white_time = game.white_time - game.timer.duration();
    } else {
      game.black_time = game.black_time - game.timer.duration();
    }
  }
}

void stopNoises(std::vector<sf::Sound>& sounds)
{
  for (std::vector<sf::Sound>::iterator sound = sounds.begin();
      sound != sounds.end(); ++sound)
  {
    sound->stop();
  }
}

void attributeSounds(struct CHESSGAME& game)
{
  std::vector<size_t> playable_sounds = {};
  for (size_t i = FIRST_KUNGFU_SOUND; i < NB_SOUNDS; i++)
  {
    playable_sounds.push_back(i);
  }

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle(playable_sounds.begin(), playable_sounds.end(),
    std::default_random_engine(seed));

  game.white_player_sound = playable_sounds[0];
  game.black_player_sound = playable_sounds[1];
}

void catchEvents(sf::RenderWindow& window, sf::Event& event,
  struct CHESSGAME& game, std::vector<sf::Sound>& sounds)
{
  sf::View view;
  while(window.pollEvent(event))
  {
    switch (event.type)
    {
      case sf::Event::Closed:
        window.close();
        break;
      case sf::Event::KeyReleased:
        game.pressed = false;
        break;
      case sf::Event::KeyPressed:
        if (!game.pressed)
        {
          if (event.key.code == sf::Keyboard::Escape)
          {
            window.close();
          } else if (event.key.code == sf::Keyboard::Space) {
            game.started = false;
            game.white_time = game.time * MILLIS_PER_MIN;
            game.black_time = game.time * MILLIS_PER_MIN;
            game.turn = WHITE_TURN;
            attributeSounds(game);
            game.gong_end = false;
          } else if (event.key.code == sf::Keyboard::Add) {
            game.started = false;

            if (game.time < MAX_TIME)
            {
              game.time++;
            }

            game.white_time = game.time * MILLIS_PER_MIN;
            game.black_time = game.time * MILLIS_PER_MIN;
            game.turn = WHITE_TURN;
            attributeSounds(game);
            game.gong_end = false;
          } else if (event.key.code == sf::Keyboard::Subtract) {
            game.started = false;

            if (game.time > MIN_TIME)
            {
              game.time--;
            }

            game.white_time = game.time * MILLIS_PER_MIN;
            game.black_time = game.time * MILLIS_PER_MIN;
            game.turn = WHITE_TURN;
            attributeSounds(game);
            game.gong_end = false;
          } else if (event.key.code == sf::Keyboard::BackSpace) {
            std::string tmp_name = game.white_player;
            game.white_player = game.black_player;
            game.black_player = tmp_name;

            if (game.started)
            {
              size_t tmp_sound = game.white_player_sound;
              game.white_player_sound = game.black_player_sound;
              game.black_player_sound = tmp_sound;
              long int tmp_time = game.white_time;
              game.white_time = game.black_time;
              game.black_time = tmp_time;
              game.turn = !game.turn;
            }
          } else if ((event.key.code == sf::Keyboard::LControl) ||
            (event.key.code == sf::Keyboard::Enter))
          {
            stopNoises(sounds);
            if (!game.started)
            {
              sounds[GONG_SOUND].play();
              game.started = true;
            } else {
              if (game.turn == WHITE_TURN)
              {
                sounds[game.white_player_sound].play();
                game.white_time = game.white_time - game.timer.duration();
              } else {
                sounds[game.black_player_sound].play();
                game.black_time = game.black_time - game.timer.duration();
              }
            }
            game.turn = !game.turn;
            game.timer.tick();
          }
        }
        game.pressed = true;
        break;
      default:
        break;
    }
  }
}

void display(sf::RenderWindow& window, struct CHESSGAME& game, sf::Font& font,
  std::vector<sf::Sound>& sounds)
{
  long int white_time = game.white_time;
  long int black_time = game.black_time;

  sf::Text text;
  text.setFont(font);

  if (game.started)
  {
    if (game.turn == WHITE_TURN)
    {
      white_time -= game.timer.duration();
    } else {
      black_time -= game.timer.duration();
    }
  }

  if ((white_time > 0) && (black_time > 0))
  {
    window.clear(sf::Color::Black);

    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(window.getView().getSize().x / 2,
      window.getView().getSize().y));
    rect.setFillColor(sf::Color::White);

    window.draw(rect);

    std::string white_min = std::to_string(white_time / MILLIS_PER_MIN);
    std::string black_min = std::to_string(black_time / MILLIS_PER_MIN);

    std::string white_sec =
      std::to_string((white_time % MILLIS_PER_MIN) / MILLIS_PER_SEC);
    std::string black_sec =
      std::to_string((black_time % MILLIS_PER_MIN) / MILLIS_PER_SEC);

    while (white_sec.length() < 2)
    {
      white_sec.insert(0, "0");
    }

    while (black_sec.length() < 2)
    {
      black_sec.insert(0, "0");
    }

    std::string white_millis = std::to_string(white_time % MILLIS_PER_SEC);
    std::string black_millis = std::to_string(black_time % MILLIS_PER_SEC);

    while (white_millis.length() < 3)
    {
      white_millis.insert(0, "0");
    }

    while (black_millis.length() < 3)
    {
      black_millis.insert(0, "0");
    }

    text.setCharacterSize(60);
    text.setString(game.white_player);
    text.setFillColor(sf::Color::Black);
    text.setOrigin(sf::Vector2f((text.getCharacterSize() / 4) *
      game.white_player.length(), text.getCharacterSize() / 2));
    text.setPosition(sf::Vector2f(window.getView().getSize().x / 4,
      (window.getView().getSize().y - text.getCharacterSize()) / 2));
    window.draw(text);

    std::string white_timer = white_min + ":" + white_sec + "." + white_millis;
    text.setCharacterSize(50);
    text.setString(white_timer);
    text.setOrigin(sf::Vector2f((text.getCharacterSize() / 4) *
      white_timer.length(), text.getCharacterSize() / 2));
    text.setPosition(sf::Vector2f(window.getView().getSize().x / 4,
      (window.getView().getSize().y + text.getCharacterSize()) / 2));
    window.draw(text);

    text.setCharacterSize(60);
    text.setString(game.black_player);
    text.setFillColor(sf::Color::White);
    text.setOrigin(sf::Vector2f((text.getCharacterSize() / 4) *
      game.black_player.length(), text.getCharacterSize() / 2));
    text.setPosition(sf::Vector2f(window.getView().getSize().x * 3 / 4,
      (window.getView().getSize().y - text.getCharacterSize()) / 2));
    window.draw(text);

    std::string black_timer = black_min + ":" + black_sec + "." + black_millis;
    text.setCharacterSize(50);
    text.setString(black_timer);
    text.setOrigin(sf::Vector2f((text.getCharacterSize() / 4) *
      black_timer.length(), text.getCharacterSize() / 2));
    text.setPosition(sf::Vector2f(window.getView().getSize().x * 3 / 4,
      (window.getView().getSize().y + text.getCharacterSize()) / 2));
    window.draw(text);
  } else {
    if (!game.gong_end)
    {
      stopNoises(sounds);
      sounds[GONG_SOUND].play();
      game.gong_end = true;
    }

    if (black_time <= 0)
    {
      window.clear(sf::Color::White);

      text.setCharacterSize(100);
      text.setString(game.white_player);
      text.setFillColor(sf::Color::Black);
      text.setOrigin(sf::Vector2f((text.getCharacterSize() / 4) *
        game.white_player.length(), text.getCharacterSize() / 2));
      text.setPosition(sf::Vector2f(window.getView().getSize().x / 2,
        (window.getView().getSize().y - text.getCharacterSize()) / 2));
      window.draw(text);
    } else {
      window.clear(sf::Color::Black);

      text.setCharacterSize(100);
      text.setString(game.black_player);
      text.setFillColor(sf::Color::White);
      text.setOrigin(sf::Vector2f((text.getCharacterSize() / 4) *
        game.black_player.length(), text.getCharacterSize() / 2));
      text.setPosition(sf::Vector2f(window.getView().getSize().x / 2,
        (window.getView().getSize().y - text.getCharacterSize()) / 2));
      window.draw(text);
    }

    std::string victory_details = "est vainqueur au temps !";
    text.setCharacterSize(80);
    text.setString(victory_details);
    text.setOrigin(sf::Vector2f((text.getCharacterSize() / 4) *
      victory_details.length(), text.getCharacterSize() / 2));
    text.setPosition(sf::Vector2f(window.getView().getSize().x / 2,
      (window.getView().getSize().y + text.getCharacterSize()) / 2));
    window.draw(text);
  }
  window.display();
}

struct CHESSGAME prompt()
{
  struct CHESSGAME result;
  std::string input_time;

  std::cout << "Combien de temps (en minutes) par joueur ? (Par défaut 5, \
min 1 et max 60)\n";
  getline(std::cin, input_time);

  if (input_time.empty())
  {
    input_time = "5";
  }

  try
  {
    result.time = std::stoi(input_time);
  } catch (const std::invalid_argument& ia) {
    result.time = DEFAULT_TIME;
  }

  if (result.time <= MIN_TIME)
  {
    result.time = MIN_TIME;
  } else if (result.time > MAX_TIME) {
    result.time = MAX_TIME;
  }

  result.white_time = result.time * MILLIS_PER_MIN;
  result.black_time = result.time * MILLIS_PER_MIN;

  std::cout << "Qui joue les blancs ? (Par défaut \"Joueur 1\", \
32 caractères max)\n";
  getline(std::cin, result.white_player);

  trim(result.white_player);

  if (result.white_player.empty() ||
    (result.white_player.length() > MAX_LENGTH_PLAYER_NAME))
  {
    result.white_player = DEFAULT_WHITE_PLAYER;
  }

  std::cout << "Qui joue les noirs ? (Par défaut \"Joueur 2\", \
32 caractères max)\n";
  getline(std::cin, result.black_player);

  trim(result.black_player);

  if (result.black_player.empty() ||
    (result.black_player.length() > MAX_LENGTH_PLAYER_NAME))
  {
    result.black_player = DEFAULT_BLACK_PLAYER;
  }

  if (result.white_player.compare(result.black_player) == 0)
  {
    result.white_player = DEFAULT_WHITE_PLAYER;
    result.black_player = DEFAULT_BLACK_PLAYER;
  }

  result.turn = WHITE_TURN;
  result.started = false;
  result.pressed = false;
  result.gong_end = false;

  attributeSounds(result);

  std::cout << "\nCommandes:\n- Echap => Quitte le programme,\n- Control \
gauche ou Entrée: Déclenche le compteur de l'adversaire,\n- Espace => \
Remet les compteurs à 0,\n- Retour Arrière => Echange les couleurs des \
joueurs,\n- \"+\" => Ajoute une minute (max 60) et remet les compteurs à \
0,\n- \"-\" => Enlève une minute (min 1) et remet les compteurs à 0.\n\n";

  return result;
}

int main()
{
  srand(time(NULL));

  sf::Event event;

  sf::Font font;
  if (!font.loadFromFile("./assets/fonts/UbuntuMono-B.ttf"))
  {
    std::cerr << "couldn't load font\n";
    return 1;
  }

  std::vector<sf::Sound> sounds = {};
  sounds.reserve(NB_SOUNDS);
  std::vector<sf::SoundBuffer> buffers(NB_SOUNDS, sf::SoundBuffer());

  std::vector<std::string> wav_files = {
    "gong.wav", "kungfu1.wav", "kungfu2.wav", "kungfu3.wav", "kungfu4.wav"
  };

  size_t index = 0;
  for (std::vector<std::string>::iterator wav = wav_files.begin();
    wav < wav_files.end(); ++wav)
  {
    if (!buffers[index].loadFromFile("./assets/sounds/" + *wav))
    {
      std::cerr << "couldn't load " << *wav << '\n';
      return 1;
    }

    sounds.emplace_back(sf::Sound(buffers[index]));
    ++index;
  }

  struct CHESSGAME game = prompt();

  sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT),
    "Funny Chess Timer");

  while(window.isOpen())
  {
    catchEvents(window, event, game, sounds);
    display(window, game, font, sounds);
  }

  return 0;
}
