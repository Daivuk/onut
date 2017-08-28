// Oak Nut include
#include <onut/onut.h>
#include <onut/Multiplayer.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/UIControl.h>
#include <onut/UILabel.h>
#include <onut/UITextBox.h>

void lobbyCallback(bool success, const OMultiplayer::Lobby& lobby);

void initSettings()
{
    oSettings->setGameName("Multiplayer Sample");

    // The app id can be anything that uniquely identifies your game. You can use the
    // same app id you are using for braincloud. The Master server needs to define the
    // game modes for this app id. See how to create your own master server here:
    // https://github.com/Daivuk/MatchMaker
    oSettings->setAppId("MultiplayerSample");

    // Setup default username as a user setting
    oSettings->setUserSettingDefault("username", "John Doe");

    //oSettings->setMatchMakingAddress("127.0.0.1");
}

void init()
{
    // Show our UI
    oUI->add(OLoadUI("example.json"));

    // Quit
    OFindUI("btnQuit")->onClick = [](const OUIControlRef&, const onut::UIMouseEvent&)
    {
        OQuit();
    };

    // Find match
    OFindUI<OUITextBox>("txtUsername")->textComponent.text = oSettings->getUserSetting("username");
    OFindUI("btnFindMatch")->onClick = [](const OUIControlRef&, const onut::UIMouseEvent&)
    {
        // Update username settings
        const auto& username = OFindUI<OUITextBox>("txtUsername")->textComponent.text;
        oSettings->setUserSetting("username", username);

        // For this sample, we base it on the username.
        // Each player testing the sample need to change their name to something unique.
        OMultiplayer::Player player(username, username, 0);

        // Start the match making process.
        oMultiplayer->findMatch(player, "DeathMatch", lobbyCallback);

        // Show loading
        OFindUI("LoadingDialog")->isVisible = true;
    };

    OFindUI("btnLeaveLobby")->onClick = [](const OUIControlRef&, const onut::UIMouseEvent&)
    {
        oMultiplayer->stopMatchMaking();
        OFindUI("Lobby")->isVisible = false;
        OFindUI("MainMenu")->isVisible = true;
    };
}

void update()
{
}

void render()
{
    // Clear
    oRenderer->clear(OColorHex(1d232d));
}

void postRender()
{
}

void lobbyCallback(bool success, const OMultiplayer::Lobby& lobby)
{
    if (success)
    {
        OFindUI("MainMenu")->isVisible = false;
        OFindUI("LoadingDialog")->isVisible = false;
        OFindUI("Lobby")->isVisible = true;
    }
    else
    {
        OFindUI("LoadingDialog")->isVisible = false;
        OFindUI("Lobby")->isVisible = false;
        return;
    }

    // Populate the lobby players with players found in the lobby
    const auto& players = lobby.getPlayers();
    OFindUI("player0")->isVisible = false;
    OFindUI("player1")->isVisible = false;
    OFindUI("player2")->isVisible = false;
    OFindUI("player3")->isVisible = false;
    for (size_t i = 0; i < players.size(); ++i)
    {
        const auto& player = players[i];
        const auto& pPlayerUI = OFindUI("player" + std::to_string(player.getTeamIndex()));
        pPlayerUI->isVisible = true;
        pPlayerUI->getChild<OUILabel>("lblPlayerName")->textComponent.text = player.getUsername();
    }
}

void renderUI()
{
}
