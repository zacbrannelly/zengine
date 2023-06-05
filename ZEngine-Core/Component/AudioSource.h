#pragma once

#include "Component.h"

class AudioAsset;

class AudioSource : public Component
{
public:
	AudioSource();
	~AudioSource();

	void Init() override;
	void Update() override;
	void Render(int viewId) override;

	void SetAudio(AudioAsset* asset);
	AudioAsset* GetAudio() const;

	void SetAutoPlay(bool autoPlay);
	bool IsAutoPlay() const;

	void Play();
	void Pause();
	void Stop();

private:
	AudioAsset* _audioAsset;
	bool _autoPlay;
	bool _hasAutoPlayed;
	int _playingChannel;

public:
	static ZObject* CreateInstance(std::string name, ObjectType type);
	static ZObject* Copy(std::string name, ZObject* object);
};

