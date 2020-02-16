loadUI("example.json");

// Find UIs
var quitButton = findUI("QuitButton");
var showDialogButton = findUI("ShowDialogButton");
var closeDialogButton = findUI("CloseDialogButton");
var dialog = findUI("Dialog");
var textBox = findUI("TextBox");

quitButton.setOnClick(function()
{
    quit();
});

showDialogButton.setOnClick(function(evt)
{
    dialog.setVisible(true);
});

closeDialogButton.setOnClick(function()
{
    dialog.setVisible(false);
});

textBox.setOnTextChanged(function()
{
    playSound("click.wav");
});

function render()
{
    Renderer.clear(Color.fromHexRGB(0x1d232d));
}
