// Represents a room in the dungeon
function Room() {
    this.bound = new Rect();
    this.tileIdSet = new Set();
}

// The dungeon class
function Dungeon(filename) {
    // Private properties
    var m_width = 0;
    var m_height = 0;
    var m_player = null;
    var m_rooms = [];
    var m_tiledMap = null;
    
    // Private methods
    function createMap(filename) {
        m_dungeonEntity = Scene.load(filename);

        // Create our tiled map entity.
        // The TiledMap component will look for the "entities" object layer and 
        // instantiate entities and their components based on properties
        // set in Tiled editor.
        m_dungeonEntity = EntityFactory.createTiledMap(filename);

        // Get the TiledMap from this entity
        var tiledMapComponent = m_dungeonEntity.getComponent(TiledMapComponent) as TiledMapComponent;
        m_tiledMap = getTiledMap(filename);

        // Store map dimensions
        m_width = m_tiledMap.getWidth();
        m_height = m_tiledMap.getHeight();
    }

    function createRooms() {

    }

    function createRoomAt(mapPos, tiles) {

    }

    // Load the map
    createMap(filename);

    // Find our rooms
    createRooms();

    // Find our player entity
    m_player = Scene.findEntity("player");
}

// Public methods
Dungeon.prototype.getRoomAt = function(position) {
    return null;
}

Dungeon.prototype.isInRoom = function(position, room) {
    return false;
}

Dungeon.prototype.getPlayer = function() {
    return player;
}

Dungeon.prototype.showCoinAt = function(position, count) {
}

Dungeon.prototype.showBombAt = function(position, count) {
}

// Instantiate our dungeon
var dungeon = new Dungeon("dungeon.tmx");
