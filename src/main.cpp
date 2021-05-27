#include "main.h"

void update(struct CHESSGAME& game)
{
  if (game.started)
  {
    if (game.turn == WHITE_TURN)
    {
      if (game.is_player1_playing_white)
      {
        game.player1.time = game.player1.time - game.timer.duration();
      } else {
        game.player2.time = game.player2.time - game.timer.duration();
      }
    } else {
      if (game.is_player1_playing_white)
      {
        game.player2.time = game.player2.time - game.timer.duration();
      } else {
        game.player1.time = game.player1.time - game.timer.duration();
      }
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

void catchEvents(sf::RenderWindow& window, sf::Event& event,
  struct CHESSGAME& game, std::vector<sf::Sound>& sounds,
  std::vector<std::unique_ptr<Avatar>>& avatars)
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
            game.ended = false;
            game.player1.time = game.time * MILLIS_PER_MIN;
            game.player2.time = game.time * MILLIS_PER_MIN;
            game.turn = WHITE_TURN;
            avatars[game.player1.avatar]->attributeSounds();
            avatars[game.player2.avatar]->attributeSounds();
          } else if (event.key.code == sf::Keyboard::Add) {
            game.started = false;
            game.ended = false;

            if (game.time < MAX_TIME)
            {
              game.time++;
            }

            game.player1.time = game.time * MILLIS_PER_MIN;
            game.player2.time = game.time * MILLIS_PER_MIN;
            game.turn = WHITE_TURN;
            avatars[game.player1.avatar]->attributeSounds();
            avatars[game.player2.avatar]->attributeSounds();
          } else if (event.key.code == sf::Keyboard::Subtract) {
            game.started = false;
            game.ended = false;

            if (game.time > MIN_TIME)
            {
              game.time--;
            }

            game.player1.time = game.time * MILLIS_PER_MIN;
            game.player2.time = game.time * MILLIS_PER_MIN;
            game.turn = WHITE_TURN;
            avatars[game.player1.avatar]->attributeSounds();
            avatars[game.player2.avatar]->attributeSounds();
          } else if (event.key.code == sf::Keyboard::BackSpace) {
            if (!game.ended)
            {
              game.is_player1_playing_white = !game.is_player1_playing_white;

              if (game.started)
              {
                game.turn = !game.turn;
              }
            }
          } else if (event.key.code == sf::Keyboard::LControl) {
            if (((game.is_player1_playing_white && (game.turn == WHITE_TURN))
              || ((game.turn != WHITE_TURN) && !game.is_player1_playing_white))
              && !game.ended)
            {
              stopNoises(sounds);
              if (!game.started)
              {
                sounds[GONGSOUND_INDEX].play();
                game.started = true;
              } else {
                avatars[game.player1.avatar]->playSound(sounds, true);
                game.player1.time = game.player1.time - game.timer.duration();
              }
              game.turn = !game.turn;
              game.timer.tick();
            }
          } else if (event.key.code == sf::Keyboard::Enter) {
            if (((game.is_player1_playing_white && (game.turn != WHITE_TURN))
              || ((game.turn == WHITE_TURN) && !game.is_player1_playing_white))
              && !game.ended)
            {
              stopNoises(sounds);
              if (!game.started)
              {
                sounds[GONGSOUND_INDEX].play();
                game.started = true;
              } else {
                avatars[game.player2.avatar]->playSound(sounds, false);
                game.player2.time = game.player2.time - game.timer.duration();
              }
              game.turn = !game.turn;
              game.timer.tick();
            }
          } else if (event.key.code == sf::Keyboard::LShift) {
            if (!game.ended)
            {
              game.player1.avatar = (game.player1.avatar + 1) % NB_AVATAR;
            }
          } else if (event.key.code == sf::Keyboard::Period) {
            if (!game.ended)
            {
              game.player2.avatar = (game.player2.avatar + 1) % NB_AVATAR;
            }
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
  std::vector<sf::Sound>& sounds,
  std::vector<std::unique_ptr<Avatar>>& avatars)
{
  struct PLAYER white_player;
  struct PLAYER black_player;

  if (game.is_player1_playing_white)
  {
    white_player = game.player1;
    black_player = game.player2;
  } else {
    white_player = game.player2;
    black_player = game.player1;
  }

  long int white_time = white_player.time;
  long int black_time = black_player.time;

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
    text.setString(white_player.name);
    text.setFillColor(sf::Color::Black);
    text.setOrigin(sf::Vector2f((text.getCharacterSize() / 4) *
      white_player.name.length(), text.getCharacterSize() / 2));
    text.setPosition(sf::Vector2f(window.getView().getSize().x / 4,
      window.getView().getSize().y / 2 - text.getCharacterSize()));
    window.draw(text);

    text.setCharacterSize(40);
    text.setString(avatars[white_player.avatar]->toString());
    text.setOrigin(sf::Vector2f((text.getCharacterSize() / 4) *
      avatars[white_player.avatar]->toString().length(),
      text.getCharacterSize() / 2));
    text.setPosition(sf::Vector2f(window.getView().getSize().x / 4,
      window.getView().getSize().y / 2));
    window.draw(text);

    std::string white_timer = white_min + ":" + white_sec + "." + white_millis;
    text.setCharacterSize(50);
    text.setString(white_timer);
    text.setOrigin(sf::Vector2f((text.getCharacterSize() / 4) *
      white_timer.length(), text.getCharacterSize() / 2));
    text.setPosition(sf::Vector2f(window.getView().getSize().x / 4,
      window.getView().getSize().y / 2 + text.getCharacterSize()));
    window.draw(text);

    text.setCharacterSize(60);
    text.setString(black_player.name);
    text.setFillColor(sf::Color::White);
    text.setOrigin(sf::Vector2f((text.getCharacterSize() / 4) *
      black_player.name.length(), text.getCharacterSize() / 2));
    text.setPosition(sf::Vector2f(window.getView().getSize().x * 3 / 4,
      window.getView().getSize().y / 2 - text.getCharacterSize()));
    window.draw(text);

    text.setCharacterSize(40);
    text.setString(avatars[black_player.avatar]->toString());
    text.setOrigin(sf::Vector2f((text.getCharacterSize() / 4) *
      avatars[black_player.avatar]->toString().length(),
      text.getCharacterSize() / 2));
    text.setPosition(sf::Vector2f(window.getView().getSize().x * 3 / 4,
      window.getView().getSize().y / 2));
    window.draw(text);

    std::string black_timer = black_min + ":" + black_sec + "." + black_millis;
    text.setCharacterSize(50);
    text.setString(black_timer);
    text.setOrigin(sf::Vector2f((text.getCharacterSize() / 4) *
      black_timer.length(), text.getCharacterSize() / 2));
    text.setPosition(sf::Vector2f(window.getView().getSize().x * 3 / 4,
      window.getView().getSize().y / 2 + text.getCharacterSize()));
    window.draw(text);
  } else {

    if (!game.ended)
    {
      stopNoises(sounds);
      sounds[GONGSOUND_INDEX].play();
    }

    // lock the view until users restart a game
    game.ended = true;

    if (black_time <= 0)
    {
      window.clear(sf::Color::White);

      text.setCharacterSize(100);
      text.setString(white_player.name);
      text.setFillColor(sf::Color::Black);
      text.setOrigin(sf::Vector2f((text.getCharacterSize() / 4) *
        white_player.name.length(), text.getCharacterSize() / 2));
      text.setPosition(sf::Vector2f(window.getView().getSize().x / 2,
        (window.getView().getSize().y - text.getCharacterSize()) / 2));
      window.draw(text);
    } else {
      window.clear(sf::Color::Black);

      text.setCharacterSize(100);
      text.setString(black_player.name);
      text.setFillColor(sf::Color::White);
      text.setOrigin(sf::Vector2f((text.getCharacterSize() / 4) *
        black_player.name.length(), text.getCharacterSize() / 2));
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

struct CHESSGAME prompt(std::vector<std::unique_ptr<Avatar>>& avatars)
{
  struct CHESSGAME result;
  std::string input;
  size_t index = 0;

  std::cout << "Combien de temps (en minutes) par joueur ? (Par défaut " <<
    std::to_string(DEFAULT_TIME) << ", min " << std::to_string(MIN_TIME) <<
    " et max " << std::to_string(MAX_TIME) << ")\n";
  getline(std::cin, input);

  if (input.empty())
  {
    input = std::to_string(DEFAULT_TIME);
  }

  try
  {
    result.time = std::stoi(input);
  } catch (const std::invalid_argument& ia) {
    result.time = DEFAULT_TIME;
  }

  if (result.time <= MIN_TIME)
  {
    result.time = MIN_TIME;
  } else if (result.time > MAX_TIME) {
    result.time = MAX_TIME;
  }

  result.player1.time = result.time * MILLIS_PER_MIN;
  result.player2.time = result.time * MILLIS_PER_MIN;

  std::cout << "Qui joue les Blancs ? (Par défaut \"" << DEFAULT_PLAYER1
    << "\", " << std::to_string(MAX_LENGTH_PLAYER_NAME) << " caractères max)\n";
  getline(std::cin, result.player1.name);

  trim(result.player1.name);

  if (result.player1.name.empty() ||
    (result.player1.name.length() > MAX_LENGTH_PLAYER_NAME))
  {
    result.player1.name = DEFAULT_PLAYER1;
  }

  std::cout << "Quel avatar pour les Blancs ? (Par défaut " <<
    avatars[0]->toUpperString() << ")\n";
  for (const std::unique_ptr<Avatar>& avatar: avatars)
  {
    std::cout << std::to_string(index) << ": " << avatar->toUpperString() <<
      '\n';
    ++index;
  }
  getline(std::cin, input);

  if (input.empty())
  {
    input = std::to_string(DEFAULT_AVATAR);
  }

  try
  {
    result.player1.avatar = std::stoi(input);
  } catch (const std::invalid_argument& ia) {
    result.player1.avatar = DEFAULT_AVATAR;
  }

  if ((result.player1.avatar < DEFAULT_AVATAR) ||
    (result.player1.avatar >= NB_AVATAR))
  {
    result.player1.avatar = DEFAULT_AVATAR;
  }

  std::cout << "Qui joue les Noirs ? (Par défaut \"" << DEFAULT_PLAYER1
    << "\", " << std::to_string(MAX_LENGTH_PLAYER_NAME) << " caractères max)\n";
  getline(std::cin, result.player2.name);

  trim(result.player2.name);

  if (result.player2.name.empty() ||
    (result.player2.name.length() > MAX_LENGTH_PLAYER_NAME))
  {
    result.player2.name = DEFAULT_PLAYER2;
  }

  if (result.player1.name.compare(result.player2.name) == 0)
  {
    result.player1.name = DEFAULT_PLAYER1;
    result.player2.name = DEFAULT_PLAYER2;
  }

  index = 0;
  std::cout << "Quel avatar pour les Blancs ? (Par défaut " <<
    avatars[0]->toUpperString() << ")\n";
  for (const std::unique_ptr<Avatar>& avatar: avatars)
  {
    std::cout << std::to_string(index) << ": " << avatar->toUpperString() <<
      '\n';
    ++index;
  }
  getline(std::cin, input);

  if (input.empty())
  {
    input = std::to_string(DEFAULT_AVATAR);
  }

  try
  {
    result.player2.avatar = std::stoi(input);
  } catch (const std::invalid_argument& ia) {
    result.player2.avatar = DEFAULT_AVATAR;
  }

  if ((result.player2.avatar < DEFAULT_AVATAR) ||
    (result.player2.avatar >= NB_AVATAR))
  {
    result.player2.avatar = DEFAULT_AVATAR;
  }

  result.turn = WHITE_TURN;
  result.started = false;
  result.ended = false;
  result.pressed = false;
  result.is_player1_playing_white = true;

  std::cout << "\n\u250C\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\
\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\
\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\
\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\
\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\
\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\
\u2500\u2500\u2500\u2500\u2500\u2510\n\u2502 - Echap => Quitte le programme, \
                                        \u2502\n\u2502 - Ctrl gauche ou \
Entrée => Déclenche le compteur de l'adversaire,       \u2502\n\u2502 - \
Espace => Réinitialise les compteurs,                                 \u2502\
\n\u2502 - Retour Arrière => Echange les couleurs des joueurs,              \
     \u2502\n\u2502 - \"+\" => Ajoute une minute (max 60) et réinitialise les\
 compteurs,      \u2502\n\u2502 - \"-\" => Enlève une minute (min 1) et\
 réinitialise les compteurs,       \u2502\n\u2502 - Maj gauche => Change \
l'avatar du Joueur 1,                            \u2502\n\u2502 - \".\" => \
Change l'avatar du Joueur 2.                                   \u2502\n\u2514\
\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\
\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\
\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\
\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\
\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\
\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\
\u2500\u2518\n\n";

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
    return EXIT_FAILURE;
  }

  std::vector<sf::Sound> sounds = {};
  sounds.reserve(NB_SOUNDS);
  std::vector<sf::SoundBuffer> buffers(NB_SOUNDS, sf::SoundBuffer());

  std::vector<std::string> wav_files = {};
  wav_files.reserve(NB_SOUNDS);

  for (const std::filesystem::directory_entry& file :
    std::filesystem::directory_iterator("./assets/sounds/"))
  {
    wav_files.push_back(file.path().stem().string() + ".wav");
  }

  std::sort(wav_files.begin(), wav_files.end(),
    [](std::string a, std::string b) {
      return a < b;
    }
  );

  size_t index = 0;
  for (std::vector<std::string>::iterator wav = wav_files.begin();
    wav < wav_files.end(); ++wav)
  {
    if (!buffers[index].loadFromFile("./assets/sounds/" + *wav))
    {
      std::cerr << "couldn't load " << *wav << '\n';
      return EXIT_FAILURE;
    }

    sounds.emplace_back(sf::Sound(buffers[index]));
    ++index;
  }

  std::vector<std::unique_ptr<Avatar>> avatars;
  avatars.reserve(NB_AVATAR);
  avatars.emplace_back(new KungFuMasterAvatar());
  avatars.emplace_back(new KingOfTheJungleAvatar());
  avatars.emplace_back(new UnquestioningLoveAvatar());

  for (std::unique_ptr<Avatar>& avatar: avatars)
  {
    avatar->attributeSounds();
  }

  struct CHESSGAME game = prompt(avatars);

  sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT),
    "Funny Chess Timer");

  while(window.isOpen())
  {
    catchEvents(window, event, game, sounds, avatars);
    display(window, game, font, sounds, avatars);
  }

  return EXIT_SUCCESS;
}
