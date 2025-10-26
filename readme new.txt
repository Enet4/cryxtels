Crystal Pixels for modern systems has been brought to you by Eduardo Pinho AKA E_net4 (enet4mikeenet@gmail.com) in 2013.

This development version has been uploaded to a GitHub repository, thus making Cryxtels an easily accessible open-source project.

The old readme file was included without any modifications with the name "crystal pixels.txt". If you still don't know what Crystal Pixels is, then I suggest you read "crystal pixels.txt" first, THEN come back here. The old file "pixels definition.txt" still applies in this version, and pixel definitions follow the format described there.

_______________
What's New?

    There is hardly anything new, except for the fact that it no longer runs under MS-DOS. All old API calls and code segments were converted in order to become runnable in modern operating systems, such as Windows and Linux. This was achieved by using the Simple Directmedia Library: www.libsdl.org
In the process, a few parts of the program were tweaked and examined, such as the keyboard mappings. The Solid Box primitive was manually added. Plus, audio has been brought back, a feature which was not documented in the English version and only worked on a very specific soundcard.

___________________________
So which are the new controls?

In Mouse mode (default):
    Left Mouse Button : Increase thrust while in The Fly ; move foward in a pixel.
    Right Mouse Button : Stop thrusting ; Enter/leave The Fly in a pixel.
        Holding down RMB for about half a second makes The Fly invert direction while flying.
In Keyboard mode:
    Left/Right/Up/Down : Rotate The Fly. Each press increases the speed of rotation. Press the opposite key to decrease the speed.
    A : Increase thrust while in The Fly ; move foward in a pixel.
    Z : Stop thrusting ; move backwards in a pixel.
In either mode:
    Alt+Enter : Toggle fullscreen mode.
    Page Up : Generate new object while inside Sunny's atmosphere ; pick up object otherwise.
    Page Down : Drop object.
    Left Shift + <LETTER> : Saves the situation (game save) in "CRYXTELS.?IT" (? is the pressed letter).
    Right Shift + <LETTER> : Loads the situation (game save) from "CRYXTELS.?IT" (? is the pressed letter).
    F1 : Invert the direction of The Fly.
    F2 : Point towards where The Fly is heading to.
    F3 : Enter / leave The Fly.
    F4 : Dock / Undock.
    F5 : Point towards Sunny ; while carrying a pixel, switch the pixel being carried to the next one.
    F6 : Take a snapshot and save it with the name "SNAP_<date>_<time>.BMP".
    F7 : Toggle text typing mode, so that you can type in notepad objects.
    TAB : Toggle capability of climbing onto objects.
    M : Toggle between Keyboard mode and Mouse mode.
    X : Show/hide The Fly.
    I : Toggle direction indicators.
    E : Toggle Echo sonar.
    Alt gr (right alt) : Keep it pressed for a hallucinating blur effect.
    Left/Right/Up/Down : Move the cursor when writing to a notepad.
   
__________________
Tell me about the audio!

The original music and sound effects from the Italian version were reintroduced.
This includes the intro screen music, the Echo sonar which provides audio feedback on how far away you are from the nearest pixel, and the audio CD capability from the audio player.
In the definition of an object in the pixels definitions, any "associated file" that is neither of the reserved names for special objects is interpreted as the name of an audio file, without extension.
To play the audio in-game, drop the object on top of an audio player object (it is the one which looks like a flat square surface with circular rays and some text in the top-left corner).
To make the audio loop automatically, drop a "REPEAT" object onto the audio player once, then keep the audio there for as long as you wish. Drop the REPEAT object again to disable the loop, so that it does not loop even when the CD is still on the player.

A few examples of audio CDs were added in this version. To create your own, you can copy one of them and change the presented title and file name. Take note that the file name is currently limited to a maximum of 8 characters (without extension) in upper case. So "wares.ogg" will not be found on Linux, but "WARES.OGG" is fine.

For the silent experience, you can delete all audio files and the game will continue to operate.

__________________
Command line parameters.

   There actually is more than one syntax for running Cryxtels in the original. These were slightly modified as well. See below:

   CRYXTELS n PIXELS m OBJECTS
    - Generates the microcosm with n pixels and m objects. n and m are numbers ranging from 1 to 500.

   CRYXTELS
    - Generates the microcosm with 250 pixels and 250 objects (it no longer loads CRYXTELS.SIT).

   CRYXTELS <LETTER>
    - Loads situation from the respective situation letter ("CRYXTELS.?IT").

__________________
General settings.

    Edit SETTINGS.INI to customize your Crystal Pixels experience.
    With this file you can:

    - Increase the display size and resolution
    - Change the frame rates
    - Change the default generation parameters
    - Use a different file for the pixel definitions

