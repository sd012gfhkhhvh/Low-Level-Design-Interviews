/*
=========================================================================
  Adapter Pattern — Practical Example: Media Player Adapter
=========================================================================

Scenario:
You are building an AudioPlayer application that natively plays MP3 files.
It works perfectly with your `Mp3Player` implementation.

Then, the product team asks for a new feature:

  "We need to support VLC and MP4 formats. There are existing external
   codec libraries (VlcCodec, Mp4Codec) that can decode these formats.
   Please integrate them."

The problem? The external codecs don't follow your `MediaPlayer` interface.
They have their own method names (`playVlc`, `playMp4`), and you cannot
change their source code.

Solution:
  Create Adapters (`VlcPlayerAdapter`, `Mp4PlayerAdapter`) that implement your
  `MediaPlayer` interface. Internally, these adapters will call the
  corresponding methods on the external codecs. This allows your AudioPlayer
  to play VLC and MP4 files without modifying its core logic.

=========================================================================
  Component Mapping
=========================================================================

  Target    → MediaPlayer (the interface your app expects)
  Adaptee   → VlcCodec, Mp4Codec (the external libraries)
  Adapter   → VlcPlayerAdapter, Mp4PlayerAdapter (the translator bridges)
  Client    → AudioPlayer (your application)

=========================================================================
*/

#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

/*
=========================================================================
  STEP 1: Target Interface — MediaPlayer
=========================================================================
This is the interface your application expects. Any class that wants to
play media in your application must implement this interface.
*/
class MediaPlayer {
public:
  virtual void play(string filename) = 0;
  virtual ~MediaPlayer() = default;
};

/*
=========================================================================
  STEP 2: Native Implementation — Mp3Player
=========================================================================
Your native player that already implements the target interface.
No adapter is needed here.
*/
class Mp3Player : public MediaPlayer {
public:
  void play(string filename) override {
    cout << "MP3 Player: Playing " << filename << endl;
  }
};

/*
=========================================================================
  STEP 3: Adaptees — External Codec Libraries
=========================================================================
These are the third-party libraries. They are completely incompatible with
your MediaPlayer interface (`play(string)` vs `playVlc(string)`).
*/
class VlcCodec {
public:
  void playVlc(string filename) {
    cout << "VLC Codec: Decoding and playing " << filename << endl;
  }
};

class Mp4Codec {
public:
  void playMp4(string filename) {
    cout << "MP4 Codec: Decoding and playing " << filename << endl;
  }
};

/*
=========================================================================
  STEP 4: Adapters — The Bridges
=========================================================================
These adapters implement your target interface (MediaPlayer) and hold an
instance of the adaptee (the codec). They translate the `play()` call
into the specific codec's playback method.
*/
class VlcPlayerAdapter : public MediaPlayer {
private:
  VlcCodec *codec;

public:
  VlcPlayerAdapter(VlcCodec *codec) : codec(codec) {}
  
  void play(string filename) override { 
    // Translate the call
    codec->playVlc(filename); 
  }
};

class Mp4PlayerAdapter : public MediaPlayer {
private:
  Mp4Codec *codec;

public:
  Mp4PlayerAdapter(Mp4Codec *codec) : codec(codec) {}
  
  void play(string filename) override { 
    // Translate the call
    codec->playMp4(filename); 
  }
};

/*
=========================================================================
  STEP 5: Client Code — AudioPlayer
=========================================================================
Your core application. It routes the file to the correct player or adapter
based on the file extension. Notice how it interacts with the adapters
using the unified `play()` method.
*/
class AudioPlayer {
public:
  void playFile(string filename) {
    // Extract file extension and convert to lowercase
    string ext = filename.substr(filename.rfind('.') + 1);
    transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    cout << "\n--- Requesting to play: " << filename << " ---" << endl;

    if (ext == "mp3") {
      Mp3Player player;
      player.play(filename);
    } 
    else if (ext == "vlc") {
      VlcCodec codec;
      VlcPlayerAdapter adapter(&codec);
      adapter.play(filename);
    } 
    else if (ext == "mp4") {
      Mp4Codec codec;
      Mp4PlayerAdapter adapter(&codec);
      adapter.play(filename);
    } 
    else {
      cout << "Error: Unsupported media format (" << ext << ")" << endl;
    }
  }
};

int main() {
  AudioPlayer player;
  
  player.playFile("song.mp3");
  player.playFile("movie.mp4");
  player.playFile("documentary.vlc");
  player.playFile("image.png"); // Unsupported
  
  return 0;
}