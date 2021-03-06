#include "SceneManager.h"
#include "SceneMgrSys.h"
#include "../Physics/ScoreTest.h"

SceneManager::SceneManager()
    : m_curScene(TScene::none), m_newScene(TScene::none),
      m_sceneName(c_sceneNameText[int(TScene::none)]),
      m_isNewScene(false),
      m_sceneLoadFile("") 
{}

const SceneManager::TScene SceneManager::assignNewScene()
{
    if (m_isNewScene)
    {
        m_isNewScene = false;
        m_curScene = m_newScene;
        m_sceneName = c_sceneNameText[int(m_newScene)];
        m_sceneLoadFile = c_sceneLoadFileText[int(m_newScene)];

        // Switch to the new scene
        switch (m_newScene)
        {
        case TScene::mainMenu:
            gameState = GameState::MENU;
            UISystem::getInstance().LoadStartMenu();
            ScoreTest::getInstance().loadHighScore();
            break;

        case TScene::gameOver:
            gameState = GameState::GAMEOVER;
            ScoreTest::getInstance().evalHighScore();
            break;

        case TScene::level_1:
            UISystem::getInstance().LoadTimer();
        case TScene::level_2:
            gameState = GameState::PREPARING;
        default:
            break;
        }

        // Clear the new scene so no confusion
        m_newScene = TScene::none;
    }

    return m_curScene;
}

// MUST call assignNewScene() to make it official
void SceneManager::setScene(const SceneManager::TScene newScene)
{
    if (newScene != m_curScene)
    {
        m_isNewScene = true;
        m_newScene = newScene;
    }
}

SceneManager::TScene SceneManager::nextScene(void)
{
    m_newScene = (TScene)((int)m_curScene + 1);
    if (m_newScene == TScene::TotalScenes) { m_newScene = TScene::mainMenu; }
    if (m_newScene == TScene::gameOver)
    {
        // We have a win scenario so save any high score change and forward 
        // our message emitted to other systems
        SceneMgrSystem::getInstance().sendEvent(SceneMgrSystem::TSndEvt::GAMEWON);
    }
    m_isNewScene = true;

    return m_curScene;
}

void SceneManager::cmdNotification(TCmd keyPressed)
{
//    MENU, PREPARING, RUNNING, GAMEOVER
    switch (keyPressed)
    {
    case TCmd::START:
        if (gameState == GameState::MENU)
        {
            nextScene();

            // NOTE : Running state is triggered when the level XML file
            //        has completed loading. Possible to create a method
            //        to control all game changed here only and a call to
            //        get gameState from here
        }
        break;

    case TCmd::MENU:
        if (gameState == GameState::GAMEOVER)
        {
            ScoreTest::getInstance().resetScore();
            gameState = GameState::MENU;
            setScene(TScene::mainMenu);
        }
        break;

    case TCmd::RESTART:
        if (gameState == GameState::GAMEOVER)
        {
            ScoreTest::getInstance().resetScore();
            ScoreTest::getInstance().loadHighScore();
            gameState = GameState::PREPARING;
            setScene(TScene::level_1);
        }
        break;

    case TCmd::GAMEOVER:
        // Does not matter what state we are in when its received we are done
        gameState = GameState::GAMEOVER;
        setScene(TScene::gameOver);
        break;

    default:
        cout << "ERROR: Unhandled game state in SystemManager" << endl;
        break;
    }
}

