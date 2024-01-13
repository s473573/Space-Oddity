#include <Game.h>

using namespace so;
using namespace std;

Game::Game() {}
Game
&Game::instance()
{
    static Game game;
    return game;
}
bool
Game::init()
{
    srand(time(nullptr));
    // readSettingsFile();
    //WINDOW.init(settings.resHeight, settings.resWidth,settings.fullScreen);
    WINDOW.init(1280, 720, false);
    RENDERER.init();

    // MENU.init(settings.levels, settings.levelsLabels);
    // int i = MENU.loop();
    // if (i == -1)
    // {
    //     terminate();
    //     return false;
    // }
    // HUD.setStatus(LOADING);
    // HUD.draw();
    WINDOW.internal.display();

    // music.openFromFile(settings.levelsMusic[i]);
    // music.setVolume(60);
    // music.setLoop(true);
    // music.play();
    // 

    // CREATE A BLACK SCREEN USING SHADERS
    
    currentScene = new Scene("assets/scenes/main.json");
    // //currentScene = new Scene(settings.levels[i].c_str());
    currentScene->camera.setViewPerspectiveLocation(RENDERER.getUniformLocation("transformation"));
    currentScene->camera.setPosLoaction(RENDERER.getUniformLocation("cameraPos"));
    GameObject::setModelLocation(RENDERER.getUniformLocation("modelMatrix"));
    Material::setUniformsLocation(RENDERER.getMainShader());
    // currentScene->lights[0].toggle();

    assert(glGetError() == 0);

    // easterEggSoundBuffer.loadFromFile("Assets/Audio/easterEgg.ogg");
    // easterEggSound.setBuffer(easterEggSoundBuffer);
    // auto player = ((Player *)(currentScene->gameObjects[0]));
    // HUD.setHP(player->hp * 10);
    // HUD.setInHandAmmo(player->inHandAmmo);
    // HUD.setTotalAmmo(player->totalAmmo);
    start();
    // HUD.setStatus(ONGOING);
    //
    gameStatus = ONGOING;
    return true;
}
void
Game::terminate()
{
    RENDERER.terminate();
    WINDOW.terminate();
    delete currentScene;
}
void
Game::loop()
{
    // auto player = ((Player *)(currentScene->gameObjects[0]));
    // sf::Clock clock, shootTimer, reloadTimer;
    while (true)
    {
        sf::Event event;
        while (WINDOW.internal.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                terminate();
                return;
            case sf::Event::Resized:
                glViewport(0, 0, event.size.width, event.size.height);
                currentScene->camera.setAspectRatio((float)WINDOW.internal.getSize().x / (float)WINDOW.internal.getSize().y);
                break;
            case sf::Event::MouseButtonReleased:
                // if (
                //     event.mouseButton.button == sf::Mouse::Left &&
                //     reloadTimer.getElapsedTime().asSeconds() > 1.5f &&
                //     shootTimer.getElapsedTime().asSeconds() > 0.2f &&
                //     gameStatus == ONGOING)
                // {
                //     if (player->inHandAmmo > 0)
                //     {
                //         player->dispatchBullet();
                //         player->inHandAmmo--;
                //         shootTimer.restart();
                //         HUD.setInHandAmmo(player->inHandAmmo);
                //         HUD.setTotalAmmo(player->totalAmmo);
                //     }
                //     else
                //     {
                //         if (player->totalAmmo > 0 && reloadTimer.getElapsedTime().asSeconds() > 1.5f)
                //         {
                //             reloadTimer.restart();
                //             player->reload();
                //         }
                //     }
                // }
                break;
            case sf::Event::KeyReleased:
                // if (event.key.code == sf::Keyboard::F && gameStatus == ONGOING)
                // {
                //     currentScene->lights[0].toggle();
                // }
                // if (event.key.code == sf::Keyboard::R && reloadTimer.getElapsedTime().asSeconds() > 1.5f && gameStatus == ONGOING)
                // {
                //     if (player->totalAmmo > 0 && player->inHandAmmo < player->maxAmmo)
                //     {
                //         reloadTimer.restart();
                //         player->reload();
                //     }
                // }
                // if (event.key.code == sf::Keyboard::M && gameStatus == ONGOING)
                // {
                //     easterEggSound.play();
                // }
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                {
                    terminate();
                    return;
                }
            default:
                break;
            }
        }
        if (gameStatus == ONGOING)
        {
            // PHYSICS.apply(currentScene, deltaTime);
            update();
            RENDERER.renderScene(currentScene);
            // deltaTime = clock.restart().asSeconds();
        }
        else
        {
            //HUD.draw();
            RENDERER.renderScene(currentScene);
        }
    }
}
void
Game::update()
{
    // for (unsigned i = 0; i < currentScene->gameObjects.size(); ++i)
    // {
    //     currentScene->gameObjects[i]->update();
    // }
    ////  currentScene->camera.transform = currentScene->gameObjects[0]->transform;
    ////  currentScene->camera.transform.translate({0, 0.5f, 0});

    ////  auto player = ((Player *)(currentScene->gameObjects[0])); // TODO hold in class
    // auto bullets = player->bullets;
    // auto gameObjectsSize = currentScene->gameObjects.size();
    // auto bulletsSize = player->bullets.size();
    // bool isWin = true;
    // for (unsigned i = 0; i < gameObjectsSize; ++i)
    // {
    //     if (currentScene->gameObjects[i]->getClass() == 3)
    //     {
    //         isWin = false;
    //         auto ant = ((Ant *)(currentScene->gameObjects[i]));
    //         if (ant->timeToDestroy())
    //         {
    //             currentScene->destroyGameObject(i);
    //             --i;
    //             --gameObjectsSize;
    //         }
    //         for (unsigned j = 0; j < bulletsSize; ++j)
    //         {
    //             if (ant->aabb.isColliding(bullets[j].transform.getPosition()))
    //             {
    //                 ant->damage(1);
    //                 player->destroyBullet(j);
    //                 --j;
    //                 --bulletsSize;
    //             }
    //         }

    //         if (ant->aabb.isColliding(currentScene->gameObjects[0]->aabb) &&
    //             getTime() > 2.0f &&
    //             ant->timeSinceDealtDamage.getElapsedTime().asSeconds() >= 1.0f) // TODO check player's children collisions with ant too
    //         {
    //             ant->timeSinceDealtDamage.restart();
    //             player->damage(1.0f);
    //             HUD.setIsHurting(true);
    //             player->timeSinceDamage = getTime();
    //             HUD.setHP(player->hp * 10);
    //         }
    //     }
    // }
    // if (getTime() - player->timeSinceDamage > 0.25f)
    // {
    //     HUD.setIsHurting(false);
    // }

    // if (isWin)
    // {
    //     gameStatus = WIN;
    //     HUD.setStatus(WIN);
    //     player->killSound();
    // }
    // else if (player->isDead() || (player->inHandAmmo <= 0 && player->totalAmmo <= 0))
    // {
    //     gameStatus = LOSE;
    //     HUD.setStatus(LOSE);
    //     player->killSound();
    // }
}
void Game::start()
{
    // for (unsigned i = 0; i < currentScene->gameObjects.size(); ++i)
    // {
    //     currentScene->gameObjects[i]->start();
    // }
    //timeSinceStart.restart();
}
float Game::getTime()
{
    //return timeSinceStart.getElapsedTime().asSeconds();
    return 0.0;
}
void
Game::readSettingsFile() {}