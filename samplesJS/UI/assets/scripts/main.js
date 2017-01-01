loadUI("example.json");

// Find UIs
var quitButton = findUI("QuitButton");
var showDialogButton = findUI("ShowDialogButton");
var closeDialogButton = findUI("CloseDialogButton");
var dialog = findUI("Dialog");

quitButton.setOnClick(function()
{
    quit();
});

showDialogButton.setOnClick(function()
{
    dialog.setVisible(true);
});

closeDialogButton.setOnClick(function()
{
    dialog.setVisible(false);
});

function render()
{
    Renderer.clear(Color.fromHexRGB(0x1d232d));
}
