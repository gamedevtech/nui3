/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiAnimation_h__
#define __nuiAnimation_h__

//#include "nui.h"
#include "nuiTimer.h"

#include "nuiEventRegistry.h"

enum nuiAnimWhence
{
  eAnimFromStart,  ///< Move forward, from the beginning of the file (offset 0).
  eAnimForward,    ///< Move forward from the current read/write position.
  eAnimRewind,     ///< Move backwards from the current read/write position.
  eAnimFromEnd     ///< Move backwards from the end of the file.
};

enum nuiAnimLoop
{
  eAnimLoopForward,
  eAnimLoopReverse,
  eAnimLoopPingPong,
};


/// nuiAnimation is a base class that implements a basic animation framework.
class NUI_API nuiAnimation : public nuiObject
{
public:
  nuiAnimation();
  virtual ~nuiAnimation();

  virtual bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.
  virtual nuiXMLNode* Serialize(nuiXMLNode* pNode, bool CreateNewNode) const;

  virtual void SetFrameTime(double FrameTime); ///< Set the frame rate by giving the number of seconds per frame.
  virtual void SetFrameRate(double FrameRate); ///< Set the frame rate in Hertz.
  virtual double GetFrameRate(); ///< Get the frame rate in Hertz.
  virtual double GetFrameTime(); ///< Get the frame rate in seconds per frame.

  virtual bool SetFrame(double Frame, nuiAnimWhence Whence = eAnimFromStart); ///< Set the frame to display. Returns false if Frame is out of range (but the current time will be capped on the nearest boundary).
  virtual bool SetTime(double Time, nuiAnimWhence Whence = eAnimFromStart); ///< Set the frame to display. Returns false if Frame is out of range (but the current time will be capped on the nearest boundary).
  virtual double GetFrame(); ///< Return the currently displayed (non integral) frame number.
  virtual double GetTime(); ///< Return the currently displayed time in the animation.

  virtual double TimeToFrame(double Tm); ///< Return the frame at time Tm.
  virtual double FrameToTime(double Frame); ///< Return the time at frame Frame.

  virtual double GetFrameCount(); ///< Return the number of whole frames.
  virtual double GetDuration(); ///< Return the total duration of the Animation in seconds.

  virtual void Play(uint32 Count = 1, nuiAnimLoop LoopMode = eAnimLoopForward); ///< Start playing the animation. Stop after count iterations. 
  virtual void Stop(); ///< Stop Playing the animation.

  virtual bool IsPlaying(); ///< Return true if the animation is currently playing.

  virtual void OnFrame(); ///< Overload this method to get notified of each timer tick, for exemple to call Invalidate() in order to redraw the animation.

  nuiSimpleEventSource<nuiAnimationStart> AnimStart;
  nuiSimpleEventSource<nuiAnimationStop> AnimStop;
  nuiSimpleEventSource<nuiAnimationLoop> AnimLoop;

  bool Play(const nuiEvent& rEvent); ///< The animation will start playing as soon as this method is called. Use this method if you want to start playing an animation when an nuiEvent is fired.
  bool Stop(const nuiEvent& rEvent); ///< The animation will stop playing as soon as this method is called. Use this method if you want to stop playing an animation when an nuiEvent is fired.

protected:
  virtual double UpdateTime(); ///< You have to call this method before drawing in order to update the animation frame if you want to draw in realtime. This method returns the number time elapsed since the last call to UpdateTime.
  bool OnTick(const nuiEvent& rEvent);

  double mFrameTime;
  double mCurrentTime;
  uint32 mCount;

  double mDirection; ///< Either 1 or -1. This sets the current direction of play back.
  nuiAnimLoop mLoopMode;
  nuiTimer mTimer;
  nglTime mLastTime;
  bool mUpdatingTime;

  bool mEnableCallbacks;

  void EnableCallbacks(bool enable)
  {
    mEnableCallbacks = enable;
  }

  nuiEventSink<nuiAnimation> mAnimSink;
};

class NUI_API nuiWaitAnimation : public nuiAnimation
{
public:
  nuiWaitAnimation(double duration)
  {
    mUserDuration = duration;
    SetFrameRate(1.0);
  }

  virtual ~nuiWaitAnimation()
  {
  }

  double GetDuration()
  {
    return mUserDuration;
  }

protected:
  double mUserDuration;
};

class NUI_API nuiMetaAnimation : public nuiAnimation
{
public:
  nuiMetaAnimation ();
  bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.
  virtual ~nuiMetaAnimation();

  virtual nuiXMLNode* Serialize(nuiXMLNode* pNode, bool CreateNewNode) const;

  virtual void Play(uint32 Count = 1.0f, nuiAnimLoop LoopMode = eAnimLoopForward); ///< Start playing the animation. Stop after count iterations. 
  virtual void Stop(); ///< Stop Playing the animation.

  virtual double GetFrameCount(); ///< Return the number of whole frames.
  virtual double GetDuration(); ///< Return the total duration of the Animation in seconds.

  void AddAnimation(nuiAnimation* pAnim);
  void Clear();
protected:
  std::list<nuiAnimation*> mpAnimations;
};

class NUI_API nuiAnimationSequence : public nuiAnimation
{
public:
  nuiAnimationSequence ();
  bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.
  virtual ~nuiAnimationSequence();

  virtual nuiXMLNode* Serialize(nuiXMLNode* pNode, bool CreateNewNode) const;

  virtual void Play(uint32 Count = 1.0f, nuiAnimLoop LoopMode = eAnimLoopForward); ///< Start playing the animation. Stop after count iterations. 
  virtual void Stop(); ///< Stop Playing the animation.

  virtual double GetFrameCount(); ///< Return the number of whole frames.
  virtual double GetDuration(); ///< Return the total duration of the Animation in seconds.

  void AddAnimation(nuiAnimation* pAnim);
  void Clear();

  virtual void OnFrame();
protected:
  std::list<nuiAnimation*> mpAnimations;

  bool OnAnimStopped(const nuiEvent& rEvent);
  nuiEventSink<nuiAnimationSequence> mAnimSequenceSink;
};

#endif // __nuiAnimation_h__
