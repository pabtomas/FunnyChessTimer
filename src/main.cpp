#include "main.h"

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
            game.white_time = game.time * MILLIS_PER_MIN;
            game.black_time = game.time * MILLIS_PER_MIN;
            game.turn = WHITE_TURN;
            avatars[game.white_player_avatar]->attributeSounds();
            avatars[game.black_player_avatar]->attributeSounds();
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
            avatars[game.white_player_avatar]->attributeSounds();
            avatars[game.black_player_avatar]->attributeSounds();
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
            avatars[game.white_player_avatar]->attributeSounds();
            avatars[game.black_player_avatar]->attributeSounds();
            game.gong_end = false;
          } else if (event.key.code == sf::Keyboard::BackSpace) {
            std::string tmp_name = game.white_player;
            game.white_player = game.black_player;
            game.black_player = tmp_name;

            size_t tmp_avatar = game.white_player_avatar;
            game.white_player_avatar = game.black_player_avatar;
            game.black_player_avatar = tmp_avatar;

            bool tmp_id = game.white_player_id;
            game.white_player_id = game.black_player_id;
            game.black_player_id = tmp_id;

            if (game.started)
            {
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
              sounds[GONGSOUND_INDEX].play();
              game.started = true;
            } else {
              if (game.turn == WHITE_TURN)
              {
                avatars[game.white_player_avatar]->playSound(sounds,
                  game.white_player_id);
                game.white_time = game.white_time - game.timer.duration();
              } else {
                avatars[game.black_player_avatar]->playSound(sounds,
                  game.black_player_id);
                game.black_time = game.black_time - game.timer.duration();
              }
            }
            game.turn = !game.turn;
            game.timer.tick();
          } else if (event.key.code == sf::Keyboard::LShift) {
            game.white_player_avatar =
              (game.white_player_avatar + 1) % AVATAR_NB;
          } else if (event.key.code == sf::Keyboard::Tab) {
            game.black_player_avatar =
              (game.black_player_avatar + 1) % AVATAR_NB;
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
      window.getView().getSize().y / 2 - text.getCharacterSize()));
    window.draw(text);

    text.setCharacterSize(40);
    text.setString(avatars[game.white_player_avatar]->toString());
    text.setOrigin(sf::Vector2f((text.getCharacterSize() / 4) *
      avatars[game.white_player_avatar]->toString().length(),
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
    text.setString(game.black_player);
    text.setFillColor(sf::Color::White);
    text.setOrigin(sf::Vector2f((text.getCharacterSize() / 4) *
      game.black_player.length(), text.getCharacterSize() / 2));
    text.setPosition(sf::Vector2f(window.getView().getSize().x * 3 / 4,
      window.getView().getSize().y / 2 - text.getCharacterSize()));
    window.draw(text);

    text.setCharacterSize(40);
    text.setString(avatars[game.black_player_avatar]->toString());
    text.setOrigin(sf::Vector2f((text.getCharacterSize() / 4) *
      avatars[game.black_player_avatar]->toString().length(),
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

    // lock the view until users restart a game
    game.timer.tick();

    if (!game.gong_end)
    {
      stopNoises(sounds);
      sounds[GONGSOUND_INDEX].play();
      game.gong_end = true;
    }

    if (black_time <= 0)
    {
      // lock the view until users restart a game
      game.black_time = -1000.;
      game.white_time = 1000.;

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
      // lock the view until users restart a game
      game.white_time = -1000.;
      game.black_time = 1000.;

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
  std::string input;

  std::cout << "Combien de temps (en minutes) par joueur ? (Par défaut 5, \
min 1 et max 60)\n";
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

  std::cout << "Quel avatar pour les Blancs ? (Par défaut LE MAITRE DU \
KUNG-FU)\n0: LE MAITRE DU KUNG-FU\n1: LE ROI DE LA JUNGLE\n";
  getline(std::cin, input);

  if (input.empty())
  {
    input = std::to_string(DEFAULT_AVATAR);
  }

  try
  {
    result.white_player_avatar = std::stoi(input);
  } catch (const std::invalid_argument& ia) {
    result.white_player_avatar = DEFAULT_AVATAR;
  }

  if ((result.white_player_avatar < DEFAULT_AVATAR) ||
    (result.white_player_avatar >= AVATAR_NB))
  {
    result.white_player_avatar = DEFAULT_AVATAR;
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

  std::cout << "Quel avatar pour les Noirs ? (Par défaut LE MAITRE DU \
KUNG-FU)\n0: LE MAITRE DU KUNG-FU\n1: LE ROI DE LA JUNGLE\n";
  getline(std::cin, input);

  if (input.empty())
  {
    input = std::to_string(DEFAULT_AVATAR);
  }

  try
  {
    result.black_player_avatar = std::stoi(input);
  } catch (const std::invalid_argument& ia) {
    result.black_player_avatar = DEFAULT_AVATAR;
  }

  if ((result.black_player_avatar < DEFAULT_AVATAR) ||
    (result.black_player_avatar >= AVATAR_NB))
  {
    result.black_player_avatar = DEFAULT_AVATAR;
  }

  result.turn = WHITE_TURN;
  result.started = false;
  result.pressed = false;
  result.white_player_id = true;
  result.black_player_id = false;
  result.gong_end = false;

  std::cout << "\nCommandes:\n- Echap => Quitte le programme,\n- Control \
gauche ou Entrée => Déclenche le compteur de l'adversaire,\n- Espace => \
Remet les compteurs à 0,\n- Retour Arrière => Echange les couleurs des \
joueurs,\n- \"+\" => Ajoute une minute (max 60) et remet les compteurs à \
0,\n- \"-\" => Enlève une minute (min 1) et remet les compteurs à 0,\n\
- Maj gauche => Change l'avatar des Blancs,\n- Tab => Change l'avatar des \
Noirs.\n\n";

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
    "gong.wav", "kungfu1.wav", "kungfu2.wav", "kungfu3.wav", "kungfu4.wav",
    "monkey1.wav", "monkey2.wav", "monkey3.wav"
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

  std::vector<std::unique_ptr<Avatar>> avatars;
  avatars.reserve(AVATAR_NB);
  avatars.emplace_back(new KungFuMasterAvatar());
  avatars.emplace_back(new KingOfTheJungleAvatar());

  struct CHESSGAME game = prompt();
  avatars[game.white_player_avatar]->attributeSounds();
  avatars[game.black_player_avatar]->attributeSounds();

  sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT),
    "Funny Chess Timer");

  while(window.isOpen())
  {
    catchEvents(window, event, game, sounds, avatars);
    display(window, game, font, sounds, avatars);
  }

  return 0;
}
