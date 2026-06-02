/*
===============================
# The problem: A fat Interface
===============================

If an intreface is to long that the concrete classes which implements the
interface are force to depend on methods they do not use.

like for example an MediaPlayer interface can have methods like playAudio,
playVidio, stopAudio, adjustBrightness etc, which may seem convinient at the
first look because it covers all the audio and vidio related methods in a single
interface.

But as your app grows, problems start to show.

Let's say you want to create a pure audio player, a class that should only
handle sound: Even though AudioOnlyPlayer only needs audio methods, it is forced
to implement unrelated video functionality. You either throw exceptions or write
empty methods. Neither is a good outcome.

What’s Wrong With This?

- Interface Pollution: The MediaPlayer interface is doing too much. It combines
multiple unrelated responsibilities: audio playback, video playback, subtitle
handling, and brightness control.

Any class that implements this interface must carry the weight of all seven
methods, even when it only needs three. This is what's known as a "fat" or
"polluted" interface.

- Fragile Code: Introducing new features related to video does break the
implementation of AudioOnlyPlayer also, which is non disirable as it should
effect only the videoPlayers.

- Violates Liskov Substitution: A client may expect any MediaPlayer to support
video, but passing in an AudioOnlyPlayer will crash the program with an
UnsupportedOperationException.

=======================
The  Solution: LSP
=======================
# LSP:
Keep your interfaces focused. Each interface should represent a specific
capability or behavior. If a class doesn’t need a method, it shouldn’t be forced
to implement it.

For example in our previous example lets try to break down our MediaPlayer
interface into more logical, focused pieces.
- Audio-only capabilities: AudioPlayerControls
- Video-only capabilities: VideoPlayerControls

Classes Implement Only the Interfaces They Need

- ModernAudioPlayer (Audio-only) implements AudioPlayerControls
- SilentVideoPlayer (Video-only) implements VideoPlayerControls
- ComprehensiveMediaPlayer (Both audio + video) implements both the interfaces

=================================================================
"How does ISP relate to the Liskov Substitution Principle (LSP)?"
=================================================================
ISP and LSP are closely aligned.

- ISP ensures that interfaces are minimal and relevant.
- LSP ensures that implementations of those interfaces behave correctly and
predictably.

When interfaces are too broad (violating ISP), classes are often
forced to implement methods they don’t support. This commonly leads to LSP
violations like throwing UnsupportedOperationException where the client expects
normal behavior.

By applying ISP, you make LSP easier to follow because each interface becomes a
clean, reliable contract that implementers can fulfill completely and correctly.
*/

#include <iostream>
using namespace std;

// ====================
// Before ISP
// ====================
class MediaPlayer {
public:
  virtual void playAudio(const string &audioFile) = 0;
  virtual void stopAudio() = 0;
  virtual void adjustAudioVolume(int volume) = 0;

  virtual void playVideo(const string &videoFile) = 0;
  virtual void stopVideo() = 0;
  virtual void adjustVideoBrightness(int brightness) = 0;
  virtual void displaySubtitles(const string &subtitleFile) = 0;

  virtual ~MediaPlayer() = default;
};

class AudioOnlyPlayer : public MediaPlayer {
public:
  void playAudio(const string &audioFile) override {
    cout << "Playing audio file: " << audioFile << endl;
  }

  void stopAudio() override { cout << "Audio stopped." << endl; }

  void adjustAudioVolume(int volume) override {
    cout << "Audio volume set to: " << volume << endl;
  }

  // Unwanted methods forced by the interface
  void playVideo(const string & /*videoFile*/) override {
    throw runtime_error("Not supported.");
  }

  void stopVideo() override {
    // no-op
  }

  void adjustVideoBrightness(int /*brightness*/) override {
    throw runtime_error("Not supported.");
  }

  void displaySubtitles(const string & /*subtitleFile*/) override {
    throw runtime_error("Not supported.");
  }
};

// ===========================
// After ISP
// ===========================

/*
=========================================================
                     INTERFACE
=========================================================
*/
// Audio-only capabilities
class AudioPlayerControls {
public:
  virtual void playAudio(const string &audioFile) = 0;
  virtual void stopAudio() = 0;
  virtual void adjustAudioVolume(int volume) = 0;
  virtual ~AudioPlayerControls() = default;
};

// Video-only capabilities
class VideoPlayerControls {
public:
  virtual void playVideo(const string &videoFile) = 0;
  virtual void stopVideo() = 0;
  virtual void adjustVideoBrightness(int brightness) = 0;
  virtual void displaySubtitles(const string &subtitleFile) = 0;
  virtual ~VideoPlayerControls() = default;
};

/*
=========================================================
                CONCRETE Inplementations
=========================================================
*/
class ModernAudioPlayer : public AudioPlayerControls {
public:
  void playAudio(const string &audioFile) override {
    cout << "ModernAudioPlayer: Playing audio - " << audioFile << endl;
  }

  void stopAudio() override {
    cout << "ModernAudioPlayer: Audio stopped." << endl;
  }

  void adjustAudioVolume(int volume) override {
    cout << "ModernAudioPlayer: Volume set to " << volume << endl;
  }
};

class SilentVideoPlayer : public VideoPlayerControls {
public:
  void playVideo(const string &videoFile) override {
    cout << "SilentVideoPlayer: Playing video - " << videoFile << endl;
  }

  void stopVideo() override {
    cout << "SilentVideoPlayer: Video stopped." << endl;
  }

  void adjustVideoBrightness(int brightness) override {
    cout << "SilentVideoPlayer: Brightness set to " << brightness << endl;
  }

  void displaySubtitles(const string &subtitleFile) override {
    cout << "SilentVideoPlayer: Subtitles from " << subtitleFile << endl;
  }
};

class ComprehensiveMediaPlayer : public AudioPlayerControls,
                                 public VideoPlayerControls {
public:
  void playAudio(const string &audioFile) override {
    cout << "ComprehensiveMediaPlayer: Playing audio - " << audioFile << endl;
  }

  void stopAudio() override {
    cout << "ComprehensiveMediaPlayer: Audio stopped." << endl;
  }

  void adjustAudioVolume(int volume) override {
    cout << "ComprehensiveMediaPlayer: Audio volume set to " << volume << endl;
  }

  void playVideo(const string &videoFile) override {
    cout << "ComprehensiveMediaPlayer: Playing video - " << videoFile << endl;
  }

  void stopVideo() override {
    cout << "ComprehensiveMediaPlayer: Video stopped." << endl;
  }

  void adjustVideoBrightness(int brightness) override {
    cout << "ComprehensiveMediaPlayer: Brightness set to " << brightness
         << endl;
  }

  void displaySubtitles(const string &subtitleFile) override {
    cout << "ComprehensiveMediaPlayer: Subtitles from " << subtitleFile << endl;
  }
};

/*
=========================================================
                Driver Code
=========================================================
*/

int main() {
  // Before ISP
  cout << "----------- Before ISP -----------" << endl;
  AudioOnlyPlayer audioplayer;
  // audio related operations
  audioplayer.playAudio("song.mp3");
  audioplayer.stopAudio();

  // video related operations
  //   audioplayer.playVideo("video.mp4"); // thows error
  //   audioplayer.stopVideo();            // does noting

  // after ISP
  cout << "----------- After ISP -----------" << endl;

  ModernAudioPlayer modernAudioPlayer;
  cout << "\n----------- Audio Player (audio only) -----------" << endl;
  modernAudioPlayer.playAudio("song.mp3");
  modernAudioPlayer.stopAudio();
  modernAudioPlayer.adjustAudioVolume(50);

  SilentVideoPlayer silentVideoPlayer;
  cout << "\n----------- Video Player (video only) -----------" << endl;
  silentVideoPlayer.playVideo("video.mp4");
  silentVideoPlayer.stopVideo();
  silentVideoPlayer.adjustVideoBrightness(50);
  silentVideoPlayer.displaySubtitles("subtitles.srt");

  ComprehensiveMediaPlayer comprehensiveMediaPlayer;
  cout << "\n----------- Comprehensive Player (audio + video) -----------"
       << endl;
  comprehensiveMediaPlayer.playAudio("song.mp3");
  comprehensiveMediaPlayer.stopAudio();
  comprehensiveMediaPlayer.adjustAudioVolume(50);
  comprehensiveMediaPlayer.playVideo("video.mp4");
  comprehensiveMediaPlayer.stopVideo();
  comprehensiveMediaPlayer.adjustVideoBrightness(50);
  comprehensiveMediaPlayer.displaySubtitles("subtitles.srt");

  return 0;
}