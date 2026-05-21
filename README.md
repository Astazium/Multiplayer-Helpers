# Multiplayer-Helpers
A couple of useful (or not) features for [ClassiCube](https://github.com/ClassiCube/ClassiCube) multiplayer in one plugin!
# Components:
## AntiAFK
Rotates player with specified interval in seconds
| Command | Description | Default Value |
|---------|-------------|---------------|
| /client AntiAFK [true/false] | Turn AntiAFK on/off                 | false |
| /client AntiAFK [float]      | Sets rotating interval (in seconds) |  1.0  |

## FreeCam
FreeCam. Controls on arrow keys and numpad's 1 to up, 0 to down
| Command | Description | Default Value |
|---------|-------------|---------------|
| /client FreeCam [true/false] | Turn FreeCam on/off | false |
| /client FreeCam [float]      | Changes speed       | 19.2  |

## ArtBuilder
Builds an png image from blocks with specified parameters
| Command | Description |
|---------|-------------|
| /client ArtBuilder build [path to png] [posX posY posZ] [degX degY] | Builds an [path to png] from blocks at [posX posY posZ] and degree [degX degY]. [path to png] must be relative to ClassiCube executable |
| /client ArtBuilder [parameter] [value]                              | Sets [parameter] to [value] |

### Multiplayer mode
ArtBuilder, like other features from this plugin, fully supports multiplayer!

For multiplayer, ArtBuilder provides some customizable parameters and commands to control how ArtBuilder would build your image.

| Command | Description | Default Value |
|---------|-------------|---------------|
| /client ArtBuilder build stop                             | Stops building process (only for multiplayer mode!)             | none  |
| /client ArtBuilder build eta                              | Prints remaining time of building                               | none  |
| /client ArtBuilder build pause [true/false]               | Pauses building process                                         | none  |
| /client ArtBuilder [multiplayer/mp] [true/false]          | Turn multiplayer mode on/off, sets automaticly                  | auto  |
| /client ArtBuilder exitOnFinish [true/false]              | Turn exit game on finish on/off                                 | false |
| /client ArtBuilder [teleportRange/tpRange] [int]          | Distance in blocks before teleport to current building position | 6     |
| /client ArtBuilder placeInterval [float]                  | How often (in seconds) blocks will be placed                    | 0.08  |

# Setting up for Visual Studio
1. Open console and clone Multiplayer-Helpers repo: `git clone https://github.com/Astazium/Multiplayer-Helpers.git`
2. Go to Multiplayer-Helpers directory: `cd Multiplayer-Helpers`
3. Clone ClassiCube repo: `git clone https://github.com/ClassiCube/ClassiCube.git`
4. Open ClassiCube solution in Visual Studio and create new project in it
6. Open your project properties and in "Configuration Type" choose "Dynamic Library (.dll)". Don`t forget to change it for all configurations!
7. Click on project, Add -> Existing Item, go to Multiplayer-Helpers/src directory and select all files
