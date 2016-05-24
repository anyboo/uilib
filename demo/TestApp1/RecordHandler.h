#pragma once
class CRecordHandler
{
public:
	static CRecordHandler& singleton();
	~CRecordHandler();

	void start();
	void stop();
	void pause();
	void resume();
	void SetVolume(bool mute = false);
	void SetMicro(bool mute = false);

protected:

	void init();
	void uninit();

private:
	CRecordHandler();
	CRecordHandler(CRecordHandler&);
	CRecordHandler operator = (CRecordHandler&);

	bool MuteVolume;
	bool MuteMicroPhone;
	bool Recording;
};

