var clearColor = Color.fromHexRGB(0x1d232d);
var color2 = new Color(1, 1, 1, 1);
var nutTexture = getTexture("nut.png");
var check1 = false;
var radio1 = 0;
var combo1 = 0;
var progressBarAnim = new NumberAnim(0);
progressBarAnim.playSingle(0, 1, 2, Tween.EASE_BOTH, Loop.PING_PONG_LOOP);
var dragFloat1 = 0;
var dragVector2 = new Vector2(0, 0);
var dragVector3 = new Vector3(0, 0, 0);
var dragVector4 = new Vector4(0, 0, 0, 0);
var dragInt = 0;
var playerName = "John Doe";
var slider1 = 0;

function update(dt)
{
}

function render()
{
    switch (combo1)
    {
        case 0:
            Renderer.clear(clearColor);
            break;
        case 1:
            Renderer.clear(new Color(1, 0, 0));
            break;
        case 2:
            Renderer.clear(new Color(0, 1, 0));
            break;
        case 3:
            Renderer.clear(new Color(0, 0, 1));
            break;
    }
}

function renderUI()
{
    if (GUI.begin("ImGui Sample"))
    {
        if (GUI.collapsingHeader("main"))
        {
            if (GUI.button("Change bg color"))
            {
                clearColor = new Color(0, .1, 0);
            }
            if (GUI.smallButton("Revert bg color"))
            {
                clearColor = Color.fromHexRGB(0x1d232d);
            }
            if (GUI.invisibleButton("Change bg color invisible"))
            {
                clearColor = new Color(0, 0, .1);
            }
            GUI.image(nutTexture, new Vector2(128, 128));
            if (GUI.imageButton(nutTexture, new Vector2(32, 32)))
            {
                clearColor = new Color(0, .1, 0);
            }
        }

        if (GUI.collapsingHeader("Check boxes"))
        {
            check1 = GUI.checkbox("Checked", check1);
            GUI.spacing();
            radio1 = GUI.radioButton("Radio 1", radio1, 0); GUI.sameLine();
            radio1 = GUI.radioButton("Radio 2", radio1, 1); GUI.sameLine();
            radio1 = GUI.radioButton("Radio 3", radio1, 2);
            combo1 = GUI.combo("Colors", combo1, ["Default", "Red", "Green", "Blue"]);
            GUI.progressBar(progressBarAnim.get(), new Vector2(-1, 0), "" + Math.round(progressBarAnim.get() * 100) / 100);
        }

        if (GUI.collapsingHeader("Text"))
        {
            GUI.text("text");
            GUI.textColored(new Color(1, 1, 0), "text colored");
            GUI.textDisabled("text disabled");
            GUI.textWrapped("text wrapped text wrapped text wrapped text wrapped text wrapped text wrapped text wrapped text wrapped text wrapped text wrapped text wrapped text wrapped text wrapped text wrapped text wrapped text wrapped text wrapped text wrapped text wrapped text wrapped text wrapped ");
            GUI.labelText("label", "text");
            GUI.bulletText("bullet text");
            GUI.bullet(); GUI.text("text");
            GUI.separator();
            playerName = GUI.inputText("Enter your name", playerName);
        }

        if (GUI.collapsingHeader("Sliders"))
        {
            dragFloat1 = GUI.dragNumber("Drag Number", dragFloat1, .1, 0, 100);
            dragVector2 = GUI.dragVector2("Drag Vector2", dragVector2, .5, 0, 100);
            dragVector3 = GUI.dragVector3("Drag Vector3", dragVector3, .5, 0, 100);
            dragVector4 = GUI.dragVector4("Drag Vector4", dragVector4, .5, 0, 100);
            dragInt = GUI.dragInt("drag Int", dragInt, .25, 0, 100);
            slider1 = GUI.sliderNumber("Slider Number", slider1, 0, 100);
        }

        if (GUI.collapsingHeader("Colors"))
        {
            clearColor = GUI.colorPickerRGB("Background Color", clearColor);
            color2 = GUI.colorPickerRGBA("Alpha", color2);
        }
    }
    GUI.end();
}
