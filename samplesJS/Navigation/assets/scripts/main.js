setUINavigation(true);
loadUI("example.json");

findUI("QuitButton").setOnClick(quit);
findUI("ShowDialogButton").setOnClick(function()
{
    findUI("Dialog").setVisible(true);
});
findUI("CloseDialogButton").setOnClick(function()
{
    findUI("Dialog").setVisible(false);
});

function render()
{
    Renderer.clear(Color.fromHexRGB(0x1d232d));
}
