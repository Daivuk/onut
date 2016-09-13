var font = getFont("font.fnt");

var originalText = "One does nut simply";
var based64;
var hash;
var sha1;
var emails = [
    { email: "daivuk@gmail.com", isValid: false },
    { email: "email@example.com", isValid: false },
    { email: "firstname.lastname@example.com", isValid: false },
    { email: "email@subdomain.example.com", isValid: false },
    { email: "firstname+lastname@example.com", isValid: false },
    { email: "email@123.123.123.123", isValid: false },
    { email: "email@[123.123.123.123]", isValid: false },
    { email: "\"email\"@example.com", isValid: false },
    { email: "1234567890@example.com", isValid: false },
    { email: "email@example-one.com", isValid: false },
    { email: "_______@example.com", isValid: false },
    { email: "email@example.name", isValid: false },
    { email: "email@example.museum", isValid: false },
    { email: "email@example.co.jp", isValid: false },
    { email: "firstname-lastname@example.com", isValid: false },
    { email: "plainaddress", isValid: false },
    { email: "#@%%#$@#$@#.com", isValid: false },
    { email: "@example.com", isValid: false },
    { email: "Joe Smith <email@example.com>", isValid: false },
    { email: "email.example.com", isValid: false },
    { email: "email@example@example.com", isValid: false },
    { email: ".email@example.com", isValid: false },
    { email: "email.@example.com", isValid: false },
    { email: "email..email@example.com", isValid: false },
    { email: "email@example.com (Joe Smith)", isValid: false },
    { email: "email@example", isValid: false },
    { email: "email@-example.com", isValid: false },
    { email: "email@example.web", isValid: false },
    { email: "email@111.222.333.44444", isValid: false },
    { email: "email@example..com", isValid: false },
    { email: "Abc..123@example.com", isValid: false }
];

// Base 64
based64 = Cryptography.encodeBase64(originalText);

// Simple hash
hash = Cryptography.hash(originalText);

// Sha1
sha1 = Cryptography.sha1(originalText);

// Validate emails
for (var i = 0; i < emails.length; ++i) {
    var e = emails[i];
    e.isValid = Cryptography.validateEmail(e.email);
}

function render() {
    // Clear
    Renderer.clear(Color.fromHexRGB(0x1d232d));

    // Draw Info
    SpriteBatch.begin();

    SpriteBatch.drawText(font, "Original: ", new Vector2(10, 10));
    SpriteBatch.drawText(font, originalText, new Vector2(200, 10), Vector2.TOP_LEFT, new Color(.7, .7, .7, 1));

    SpriteBatch.drawText(font, "Based64: ", new Vector2(10, 30));
    SpriteBatch.drawText(font, based64, new Vector2(200, 30), Vector2.TOP_LEFT, new Color(.7, .7, .7, 1));

    SpriteBatch.drawText(font, "Simple Hash: ", new Vector2(10, 50));
    SpriteBatch.drawText(font, hash, new Vector2(200, 50), Vector2.TOP_LEFT, new Color(.7, .7, .7, 1));

    SpriteBatch.drawText(font, "Sha1: ", new Vector2(10, 70));
    SpriteBatch.drawText(font, sha1, new Vector2(200, 70), Vector2.TOP_LEFT, new Color(.7, .7, .7, 1));

    SpriteBatch.drawText(font, "Shitty email validation: ", new Vector2(10, 90));
    var pos = new Vector2(200, 90);
    for (var i = 0; i < emails.length; ++i) {
        var e = emails[i];
        if (e.isValid) {
            SpriteBatch.drawText(font, e.email, pos, Vector2.TOP_LEFT, new Color(0, 1, 0, 1));
        }
        else {
            SpriteBatch.drawText(font, e.email, pos, Vector2.TOP_LEFT, new Color(1, 0, 0, 1));
        }
        pos.y += 12;
    }

    SpriteBatch.end();
}
