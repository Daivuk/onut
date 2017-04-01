var font = getFont("font.fnt");

var originIP = "";
var userAgent = "";
var downloadedTexture = null;
var postResult = "";

function update(dt) {
    if (Input.isJustDown(Key._1)) {
        // Stalls the main loop
        originIP = Http.getString("http://httpbin.org/ip");
    }
    if (Input.isJustDown(Key._2)) {
        // Doesn't stall the main loop
        Http.getStringAsync("http://httpbin.org/user-agent", {}, function (result) {
            userAgent = result;
        });
    }
    if (Input.isJustDown(Key._3)) {
        Http.getTextureAsync("https://upload.wikimedia.org/wikipedia/commons/thumb/4/47/PNG_transparency_demonstration_1.png/560px-PNG_transparency_demonstration_1.png", {}, function (texture) {
            downloadedTexture = texture;
        });
    }
    if (Input.isJustDown(Key._4)) {
        postResult = Http.post("http://httpbin.org/post", {key:"Hello World!"});
    }
}

function render() {
    // Clear
    Renderer.clear(Color.fromHexRGB(0x1d232d));

    SpriteBatch.begin();

    SpriteBatch.drawText(font, "Press ^9911^999 to GET Origin IP.", new Vector2(10, 10));
    SpriteBatch.drawText(font, originIP, new Vector2(10, 30), Vector2.TOP_LEFT, new Color(.7, .7, .7, 1));

    SpriteBatch.drawText(font, "Press ^9912^999 to GET User Agent Asynchronously.", new Vector2(10, 110));
    SpriteBatch.drawText(font, userAgent, new Vector2(10, 130), Vector2.TOP_LEFT, new Color(.7, .7, .7, 1));

    SpriteBatch.drawText(font, "Press ^9913^999 to download a picture.", new Vector2(10, 210));
    if (downloadedTexture)
        SpriteBatch.drawRect(downloadedTexture, new Rect(10, 230, downloadedTexture.getSize()));

    SpriteBatch.drawText(font, "Press ^9914^999 to POST.", new Vector2(400, 10));
    SpriteBatch.drawText(font, postResult, new Vector2(400, 30), Vector2.TOP_LEFT, new Color(.7, .7, .7, 1));

    SpriteBatch.end();
}
